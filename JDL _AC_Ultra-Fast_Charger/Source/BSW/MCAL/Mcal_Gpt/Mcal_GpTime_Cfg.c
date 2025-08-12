//****************************************************************************************
//*
//* File Name: Mcal_GpTime_Cfg.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#include "tim.h"
#include "Mcal_GpTime_Cfg.h"
#include "STD_Os_Timer.h"
#include "FreeRTOS.h"
#include "task.h"
// #include "Mcal_Gpio_Cfg.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Union Definition
|******************************************************************************/

/*******************************************************************************
|    Struct Definition
|******************************************************************************/

/*******************************************************************************
|    Variables Definition
|******************************************************************************/
volatile uint32_t CPU_RunTime = 0UL; /* Used for counting the running time of CPU */

Mcal_GPT_Map_t Mcal_GPT_Pwm_Out_Map[MCAL_GPT_PWM_OUT_CH_FUNC_NUM] =
    {
        {MCAL_GPT_CH_PWM_OUT_CP1, MCAL_GPT_CH_FUNC_PWM_OUT, MCAL_TIM1_CLK_FREQ, MCAL_TIM_PRESCALER, &htim1, TIM_CHANNEL_1},
        {MCAL_GPT_CH_PWM_OUT_CP2, MCAL_GPT_CH_FUNC_PWM_OUT, MCAL_TIM1_CLK_FREQ, MCAL_TIM_PRESCALER, &htim1, TIM_CHANNEL_4},
};

#if 0
Mcal_GPT_Map_t Mcal_GPT_Pwm_IC_Map[MCAL_GPT_PWM_IC_CH_FUNC_NUM] =
    {
        {MCAL_GPT_PWM_CAPTURE1_TEST, MCAL_GPT_CH_FUNC_PWM_CAPTURE, MCAL_TIM1_CLK_FREQ, MCAL_TIM_PRESCALER, &htim1, TIM_CHANNEL_1},
        {MCAL_GPT_PWM_CAPTURE_CP, MCAL_GPT_CH_FUNC_PWM_CAPTURE, MCAL_TIM1_CLK_FREQ, MCAL_TIM_PRESCALER, &htim1, TIM_CHANNEL_2},
};
#endif
/*******************************************************************************
|    Constant Definition
|******************************************************************************/

void Mcal_GptDrv_Init(void)
{
  // 初始化TIM1
  MX_TIM1_Init();
  MX_TIM6_Init();
  HAL_TIM_Base_Start_IT(&htim1);
}

McalRetVal_t Mcal_GptDrv_Pwm_Start(Mcal_GPT_PWM_OUT_Channel_Id_t Id)
{
    McalRetVal_t ret = MCAL_RET_SUCCESS;
    if (Mcal_GPT_Pwm_Out_Map[Id].ChannelFunc != MCAL_GPT_CH_FUNC_PWM_OUT)
    {
        ret = MCAL_RET_FAILED;
    }
    else
    {
        // 启动PWM输出
        if (HAL_TIM_PWM_Start_IT(Mcal_GPT_Pwm_Out_Map[Id].htim, Mcal_GPT_Pwm_Out_Map[Id].Channel) != HAL_OK)
        {
            ret = MCAL_RET_FAILED; 
        }
    }
		
		return ret;
}

McalRetVal_t Mcal_GptDrv_Pwm_Stop(Mcal_GPT_PWM_OUT_Channel_Id_t Id)
{
    McalRetVal_t ret = MCAL_RET_SUCCESS;
    if (Mcal_GPT_Pwm_Out_Map[Id].ChannelFunc != MCAL_GPT_CH_FUNC_PWM_OUT)
    {
        ret = MCAL_RET_FAILED;
    }
    else
    {
        // 停止PWM输出
        if (HAL_TIM_PWM_Stop_IT(Mcal_GPT_Pwm_Out_Map[Id].htim, Mcal_GPT_Pwm_Out_Map[Id].Channel) != HAL_OK)
        {
            ret = MCAL_RET_FAILED;
        }
    }
    return ret;
}

// 通用接口，主频56MHz，预分频值为56-1，设置PWM的脉冲频率freq(1Hz-10kHz)、占空比参数 pulse (0-1000 --> 0-100%)
McalRetVal_t Mcal_Set_Pwm_Param(Mcal_GPT_PWM_OUT_Channel_Id_t Id, uint32_t freq, uint16_t duty)
{
  McalRetVal_t ret = MCAL_RET_SUCCESS;

  if ((1 < freq) && (freq < 10000) && (duty <= 1000) && (Id < MCAL_GPT_PWM_OUT_CH_FUNC_NUM))
  {
    uint16_t prescaler = Mcal_GPT_Pwm_Out_Map[Id].Prescaler; // 预分频值
    // TIM时钟频率为56MHz
    uint64_t tim_clk_freq = Mcal_GPT_Pwm_Out_Map[Id].ClockVal; // TIM1时钟频率
    // 计算PWM频率，所对应的自动重装载值   ---> ARR = 主频 / (预分频+1) / 预期PWM频率(Hz) - 1
    float pwm_freq_arr = (tim_clk_freq * 1.0) / (prescaler + 1) / freq * 1.0 - 1;
    // 计算PWM占空比，所对应比较寄存器的值 ---> CCR = 预期占空比 * (自动重装载值+1)
    // 占空比则由捕获/比较寄存器（TIMx_CRx）寄存器决定。占空比:duty = Pluse / (ARR+1)
    float pwm_duty_pulse = (1000 - duty) * 1.0 / 1000 * (pwm_freq_arr + 1);

    // 配置PSC预分频值
    __HAL_TIM_SET_PRESCALER(Mcal_GPT_Pwm_Out_Map[Id].htim, prescaler);
    // 配置PWM频率 ARR
    __HAL_TIM_SetAutoreload(Mcal_GPT_Pwm_Out_Map[Id].htim, (uint16_t)pwm_freq_arr);
    // 配置PWM占空比
    __HAL_TIM_SetCompare(Mcal_GPT_Pwm_Out_Map[Id].htim, Mcal_GPT_Pwm_Out_Map[Id].Channel, (uint16_t)pwm_duty_pulse);
  }
  else
  {
    ret = MCAL_RET_PARAMETE_ERROR;
  }

  return ret;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim1)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      Mcal_Adc_ConvCollection_Disable(MCAL_ADC_CHANNEL_1);
      Mcal_Adc_EnableInjected(MCAL_ADC1_CP1_CH);
    }
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      Mcal_Adc_ConvCollection_Disable(MCAL_ADC_CHANNEL_1);
      Mcal_Adc_EnableInjected(MCAL_ADC1_CP2_CH);
    }
  }
}

void Mcal_GpTime_AdcCollection_Start(void)
{
  HAL_TIM_Base_Start_IT(&htim6);
}

extern TaskHandle_t OsTimer_Task_Handle;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim6)
  { 
    // HAL_GPIO_TogglePin(TEST_E1_Port, TEST_E1_Pin); 
    CPU_RunTime++;

    configASSERT( OsTimer_Task_Handle != NULL );
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // 发送通知或信号量给任务
    vTaskNotifyGiveFromISR(OsTimer_Task_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

#if 0
McalRetVal_t Mcal_GptDrrv_Icu_It_Start(Mcal_GPT_PWM_IC_Channel_Id_t Id)
{
  McalRetVal_t ret = MCAL_RET_SUCCESS;

  if (Mcal_GPT_Pwm_IC_Map[Id].ChannelFunc != MCAL_GPT_CH_FUNC_PWM_CAPTURE)
  {
    ret = MCAL_RET_FAILED;
  }
  else
  {
    // 启动输入捕获
    if (HAL_TIM_IC_Start_IT(Mcal_GPT_Pwm_IC_Map[Id].htim, Mcal_GPT_Pwm_IC_Map[Id].Channel) != HAL_OK)
    {
      ret = MCAL_RET_FAILED;
    }
  }

  return ret;
}

McalRetVal_t Mcal_GptDrrv_Icu_It_Stop(Mcal_GPT_PWM_IC_Channel_Id_t Id)
{
  McalRetVal_t ret = MCAL_RET_SUCCESS;

  if (Mcal_GPT_Pwm_IC_Map[Id].ChannelFunc != MCAL_GPT_CH_FUNC_PWM_CAPTURE)
  {
    ret = MCAL_RET_FAILED;
  }
  else
  {
    // 停止输入捕获
    if (HAL_TIM_IC_Stop_IT(Mcal_GPT_Pwm_IC_Map[Id].htim, Mcal_GPT_Pwm_IC_Map[Id].Channel) != HAL_OK)
    {
      ret = MCAL_RET_FAILED;
    }
  }

  return ret;
}

McalRetVal_t Mcal_GptDrrv_Icu_It_GetValue(Mcal_GPT_PWM_IC_Channel_Id_t Id, uint32_t *capture_value)
{
    McalRetVal_t ret = MCAL_RET_SUCCESS;

    if (Mcal_GPT_Pwm_IC_Map[Id].ChannelFunc != MCAL_GPT_CH_FUNC_PWM_CAPTURE)
    {
        ret = MCAL_RET_FAILED;
    }
    else
    {
        // 获取捕获值
        *capture_value = McalIcu_IcuResult[Id].CaptureValue;
    }

    return ret;
}

McalRetVal_t Mcal_GptDrrv_Icu_It_GetFrequency(Mcal_GPT_PWM_IC_Channel_Id_t Id, uint32_t *frequency)
{
    McalRetVal_t ret = MCAL_RET_SUCCESS;

    *frequency = McalIcu_IcuResult[Id].Frequency;

    return ret;
}

McalRetVal_t Mcal_GptDrrv_Icu_It_GetDutyCycle(Mcal_GPT_PWM_IC_Channel_Id_t Id, float *duty_cycle)
{
    McalRetVal_t ret = MCAL_RET_SUCCESS;

    *duty_cycle = McalIcu_IcuResult[Id].DutyCycle;

    return ret;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == Mcal_GPT_Pwm_IC_Map[MCAL_GPT_PWM_CAPTURE1_TEST].htim->Instance)
  {
        // 清除捕获中断标志
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);                  // 清除捕获中断标志
    // 读取捕获值
    McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].CaptureValue = HAL_TIM_ReadCapturedValue(htim, Mcal_GPT_Pwm_IC_Map[MCAL_GPT_PWM_CAPTURE1_TEST].Channel);
    McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE_CP].CaptureValue = HAL_TIM_ReadCapturedValue(htim, Mcal_GPT_Pwm_IC_Map[MCAL_GPT_PWM_CAPTURE_CP].Channel);
    if (McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].CaptureValue != 0)
    {
      // 计算频率和占空比
      // 时钟频率为56MHz，预分频值为56
      McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].Frequency = MCAL_TIM1_CLK_FREQ / ((McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].CaptureValue + 1) * (MCAL_TIM_PRESCALER + 1)); // 计算频率，单位为Hz
      if (McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE_CP].CaptureValue != 0)
      {
        // 计算占空比
        McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].DutyCycle = (float)((McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE_CP].CaptureValue + 1) * 1000) / (McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].CaptureValue + 1); // 计算占空比，单位为‰
        if (McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].DutyCycle > 1000)
        {
          McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].DutyCycle = 1000; // 确保占空比不超过1000‰
        }
      }
      else
      {
        McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].DutyCycle = 0; // 如果捕获值为0，则占空比为0%
      }
    }
    else
    {
      McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].Frequency = 0;
      McalIcu_IcuResult[MCAL_GPT_PWM_CAPTURE1_TEST].DutyCycle = 0;
    }
  }
}
#endif  
/*EOF*/
