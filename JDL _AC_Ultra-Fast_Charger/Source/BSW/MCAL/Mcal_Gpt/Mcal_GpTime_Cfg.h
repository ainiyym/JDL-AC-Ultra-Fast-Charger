#ifndef MCAL_GPTIME_CFG_H
#define MCAL_GPTIME_CFG_H

#include "stm32f1xx_hal.h"
#include "Mcal_Types.h"
#include "float.h"

#define MCAL_TIM1_CLK_FREQ 56000000 /* TIM1时钟频率 */
#define MCAL_TIM_PRESCALER 55 /* TIM预分频值，56-1 */

typedef enum
{
    MCAL_GPT_CH_FUNC_PWM_OUT = 0, /* PWM输出功能 */
    MCAL_GPT_CH_FUNC_PWM_CAPTURE /* PWM捕获功能 */
} Mcal_GPT_Channel_Func_Type_t;

/* 配置软定时器pwm输出通道 */
typedef enum
{
    MCAL_GPT_CH_PWM_OUT_CP1, /* Cp1 PWM out */
    MCAL_GPT_CH_PWM_OUT_CP2, /* Cp2 PWM out */
    MCAL_GPT_PWM_OUT_CH_FUNC_NUM /* PWM输出功能通道数量 */
} Mcal_GPT_PWM_OUT_Channel_Id_t;

/* 配置软定时器捕获通道 */
typedef enum
{
    MCAL_GPT_PWM_CAPTURE1_TEST, /* PWM捕获测试通道 */
    MCAL_GPT_PWM_CAPTURE_CP, /* PWM捕获测试通道 */
    MCAL_GPT_PWM_IC_CH_FUNC_NUM /* PWM捕获功能通道数量 */
} Mcal_GPT_PWM_IC_Channel_Id_t;

typedef struct
{
    uint32_t ChannelId; /* 通道ID */
    Mcal_GPT_Channel_Func_Type_t ChannelFunc; /* 通道功能 */
    uint32_t ClockVal; /* 定时器时钟频率 */
    uint32_t Prescaler; /* 定时器预分频值 */
    TIM_HandleTypeDef *htim; /* 定时器句柄 */
    uint32_t Channel; /* 定时器通道 */
} Mcal_GPT_Map_t;

typedef struct
{   
    uint32_t CaptureValue; /* 捕获值 */
    uint32_t Frequency; /* 频率 */
    float DutyCycle; /* 占空比 ‰ */
} McalIcu_ParaCfg_t;

extern void Mcal_GptDrv_Init(void);
extern void Mcal_GpTime_AdcCollection_Start(void);
extern McalRetVal_t Mcal_GptDrv_Pwm_Start(Mcal_GPT_PWM_OUT_Channel_Id_t Id);
extern McalRetVal_t Mcal_GptDrv_Pwm_Stop(Mcal_GPT_PWM_OUT_Channel_Id_t Id);
extern McalRetVal_t Mcal_Set_Pwm_Param(Mcal_GPT_PWM_OUT_Channel_Id_t Id, uint32_t freq, uint16_t duty);
#if 0
extern McalRetVal_t Mcal_GptDrrv_Icu_It_Start(Mcal_GPT_PWM_IC_Channel_Id_t Id);
extern McalRetVal_t Mcal_GptDrrv_Icu_It_Stop(Mcal_GPT_PWM_IC_Channel_Id_t Id);
extern McalRetVal_t Mcal_GptDrrv_Icu_It_GetValue(Mcal_GPT_PWM_IC_Channel_Id_t Id, uint32_t *capture_value);
extern McalRetVal_t Mcal_GptDrrv_Icu_It_GetFrequency(Mcal_GPT_PWM_IC_Channel_Id_t Id, uint32_t *frequency);
extern McalRetVal_t Mcal_GptDrrv_Icu_It_GetDutyCycle(Mcal_GPT_PWM_IC_Channel_Id_t Id, float *duty_cycle);
extern void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
#endif
#endif /* MCAL_GPTIME_CFG_H */
