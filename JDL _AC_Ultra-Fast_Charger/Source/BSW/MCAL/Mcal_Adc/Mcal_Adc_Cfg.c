/*
 * Mcal_Adc_Cfg.c
 * ADC Configuration Source File
 */

#include "adc.h"
#include "Mcal_Adc_Cfg.h"
#include "SwitchM.h"
#include "Mcal_Gpio_Cfg.h"
#include "STD_Os_Timer.h"

Mcal_Adc_Ctrl_t Mcal_AdcCtrl = {0};
uint16_t AdcHardWareTriggerBuffer[MCAL_ADC2_TEMP_CHANNEL_MAX] = {0};

Mcal_Adc_ChCfg_t Mcal_Adc_ChConfig[MCAL_ADC_CHANNEL_NUM_MAX] =
{
    {MCAL_ADC_CHANNEL_1, &hadc1},
    {MCAL_ADC_CHANNEL_2, &hadc2}
};

Mcal_Adc_MapCfg_t Mcal_Adc_MapConfig[MCAL_ADC2_TEMP_CHANNEL_MAX] = 
{
    {MCAL_ADC1_CP1_CH, &hadc1, &AdcHardWareTriggerBuffer[MCAL_ADC1_CP1_CH]},
    {MCAL_ADC1_CP2_CH, &hadc1, &AdcHardWareTriggerBuffer[MCAL_ADC1_CP2_CH]},
    {MCAL_ADC1_12V_CH, &hadc1, &AdcHardWareTriggerBuffer[MCAL_ADC1_12V_CH]},
    {MCAL_ADC1_5V_CH, &hadc1, &AdcHardWareTriggerBuffer[MCAL_ADC1_5V_CH]},
    {MCAL_ADC1_3V3_CH, &hadc1, &AdcHardWareTriggerBuffer[MCAL_ADC1_3V3_CH]},
    {MCAL_ADC1_CHANNEL_MAX, NULL, &AdcHardWareTriggerBuffer[MCAL_ADC1_CHANNEL_MAX]}, /* ADC1 MAX */
    /* ADC2 START */
    {MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH]},
    {MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH]},
    {MCAL_ADC2_OIL1_INLET_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_OIL1_INLET_TEMP_CH]},
    {MCAL_ADC2_OIL1_OUTLET_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_OIL1_OUTLET_TEMP_CH]},
    {MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH]},
    {MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH]},
    {MCAL_ADC2_OIL2_INLET_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_OIL2_INLET_TEMP_CH]},
    {MCAL_ADC2_OIL2_OUTLET_TEMP_CH, &hadc2, &AdcHardWareTriggerBuffer[MCAL_ADC2_OIL2_OUTLET_TEMP_CH]},
};

static void Mcal_Adc_Enable(Mcal_Adc_Channel_Num_e Channel)
{
    if (Channel < MCAL_ADC_CHANNEL_NUM_MAX)
    {
        __HAL_ADC_ENABLE(Mcal_Adc_ChConfig[Channel].Handle);
    }
}

static void Mcal_Adc_Disable(Mcal_Adc_Channel_Num_e Channel)
{
    if (Channel < MCAL_ADC_CHANNEL_NUM_MAX)
    {
        __HAL_ADC_DISABLE(Mcal_Adc_ChConfig[Channel].Handle);
    }
}

void Mcal_Adc_EnableInjected(Mcal_Adc_Channel_e Channel)
{
    __HAL_ADC_ENABLE_IT(Mcal_Adc_MapConfig[Channel].Handle, ADC_IT_JEOC);
    HAL_ADCEx_InjectedStart_IT(Mcal_Adc_MapConfig[Channel].Handle);
}

void Mcal_Adc_DisableInjected(Mcal_Adc_Channel_e Channel)
{
    __HAL_ADC_DISABLE_IT(Mcal_Adc_MapConfig[Channel].Handle, ADC_IT_JEOC);
    HAL_ADCEx_InjectedStop_IT(Mcal_Adc_MapConfig[Channel].Handle);
}

static McalRetVal_t Mcal_Adc_SelfCalibration_Start(Mcal_Adc_Channel_Num_e Channel)
{
    McalRetVal_t Ret = MCAL_RET_SUCCESS;
    HAL_StatusTypeDef Status = HAL_OK;

    if (Channel < MCAL_ADC_CHANNEL_NUM_MAX)
    {
        Mcal_Adc_Disable(Channel);
        Status = HAL_ADCEx_Calibration_Start(Mcal_Adc_ChConfig[Channel].Handle);
        if (HAL_OK != Status)
        {
            Ret = MCAL_RET_FAILED;
            MCAL_DEBUG("%s Status: %d\r\n", __FUNCTION__, Status);
        }
        Mcal_Adc_Enable(Channel);
    }
    else
    {
        Ret = MCAL_RET_PARAMETE_ERROR;
    }

    return Ret;
}

void Mcal_Adc_SelfCalibration_Init(void)
{
    for (Mcal_Adc_Channel_Num_e ch = MCAL_ADC_CHANNEL_1; ch < MCAL_ADC_CHANNEL_NUM_MAX; ch++)
    {
        Mcal_Adc_SelfCalibration_Start(ch);
    }
}

static McalRetVal_t Mcal_Adc_Start_Collection(Mcal_Adc_Channel_e Channel)
{
    McalRetVal_t Ret = MCAL_RET_SUCCESS;
    HAL_StatusTypeDef Status = HAL_OK;
    
    if (Channel < MCAL_ADC1_CHANNEL_MAX)
    {
        Status = HAL_ADC_Start_DMA(Mcal_Adc_MapConfig[Channel].Handle, (uint32_t *)Mcal_Adc_MapConfig[Channel].RcvBuff, 1);
        if (HAL_OK != Status)
        {
            Ret = MCAL_RET_FAILED;
            MCAL_DEBUG("%s Status: %d\r\n", __FUNCTION__, Status);
        }
    }
    else if ((Channel != MCAL_ADC1_CHANNEL_MAX) && (Channel < MCAL_ADC2_TEMP_CHANNEL_MAX))
    {
        Status = HAL_ADC_Start_IT(Mcal_Adc_MapConfig[Channel].Handle);
        if (HAL_OK != Status)
        {
            Ret = MCAL_RET_FAILED;
            MCAL_DEBUG("%s Status: %d\r\n", __FUNCTION__, Status);
        }
    }
    else
    {
        Ret = MCAL_RET_PARAMETE_ERROR;
    }

    return Ret;
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // HAL_GPIO_TogglePin(TEST_E1_Port, TEST_E1_Pin);

    uint32_t i = MCAL_ADC1_CP1_CH;
    uint16_t AdVol = (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);

    if (AdVol > 50)
    {
        for (i = MCAL_ADC1_CP1_CH; i < MCAL_ADC1_CHANNEL_MAX; i++)
        {
            if (hadc == Mcal_Adc_MapConfig[i].Handle)
            {
                switch (SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC0))
                {
                case SWITCHM_74HCT4851D_CP1:
                    AdcHardWareTriggerBuffer[MCAL_ADC1_CP1_CH] = AdVol;
                        break;

                case SWITCHM_74HCT4851D_CP2:
                    AdcHardWareTriggerBuffer[MCAL_ADC1_CP2_CH] = AdVol;
                    break;

                case SWITCHM_74HCT4851D_12V:
                    AdcHardWareTriggerBuffer[MCAL_ADC1_12V_CH] = AdVol;
                    break;

                case SWITCHM_74HCT4851D_5V:
                    AdcHardWareTriggerBuffer[MCAL_ADC1_5V_CH] = AdVol;
                    break;

                case SWITCHM_74HCT4851D_3V3:
                    AdcHardWareTriggerBuffer[MCAL_ADC1_3V3_CH] = AdVol;
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{   
    //uint8_t AO = 0;
    uint16_t AdVol1 = (uint16_t)HAL_ADC_GetValue(&hadc1);
    uint16_t AdVol2 = (uint16_t)HAL_ADC_GetValue(&hadc2);

    // AO = SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC0);
    // AdVol = HAL_ADC_GetValue(&hadc1);
    // MCAL_DEBUG("--conv--AO :%d VOL:%d \r\n", AO, AdVol);
    if (hadc == &hadc1)
    {
        switch (SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC0))
        {
        case SWITCHM_74HCT4851D_CP1:
            AdcHardWareTriggerBuffer[MCAL_ADC1_CP1_CH] = AdVol1;
            break;

        case SWITCHM_74HCT4851D_CP2:
            AdcHardWareTriggerBuffer[MCAL_ADC1_CP2_CH] = AdVol1;
            break;

        case SWITCHM_74HCT4851D_12V:
            AdcHardWareTriggerBuffer[MCAL_ADC1_12V_CH] = AdVol1;
            break;

        case SWITCHM_74HCT4851D_5V:
            AdcHardWareTriggerBuffer[MCAL_ADC1_5V_CH] = AdVol1;
            break;

        case SWITCHM_74HCT4851D_3V3:
            AdcHardWareTriggerBuffer[MCAL_ADC1_3V3_CH] = AdVol1;
            break;

        default:
            break;
        }
    }
    else if (hadc == &hadc2)
    {
        switch (SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC1))
        {
        case SWITCHM_74HCT4851D_CONNECTOR1_FRONT_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_CONNECTOR1_REAR_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_OIL1_INLET_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_OIL1_INLET_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_OIL1_OUTLET_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_OIL1_OUTLET_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_CONNECTOR2_FRONT_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_CONNECTOR2_REAR_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_OIL2_INLET_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_OIL2_INLET_TEMP_CH] = AdVol2;
            break;

        case SWITCHM_74HCT4851D_OIL2_OUTLET_TEMP:
            AdcHardWareTriggerBuffer[MCAL_ADC2_OIL2_OUTLET_TEMP_CH] = AdVol2;
            break;

        default:
            break;
        }
    }
    else
    {
    }
}

void Mcal_Adc_Init()
{
  MX_ADC1_Init();
  MX_ADC2_Init();
  Mcal_Adc_SelfCalibration_Init();
}

static void Mcal_Adc1_ConvCollection_Start(void)
{
    if (ENABLE == Mcal_AdcCtrl.EnableStatus[MCAL_ADC_CHANNEL_1])
    {
        switch (SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC0))
        {
        case SWITCHM_74HCT4851D_CP1:
            Mcal_Adc_Start_Collection(MCAL_ADC1_CP1_CH);
            break;

        case SWITCHM_74HCT4851D_CP2:
            Mcal_Adc_Start_Collection(MCAL_ADC1_CP2_CH);
            break;

        case SWITCHM_74HCT4851D_12V:
            Mcal_Adc_Start_Collection(MCAL_ADC1_12V_CH);
            break;

        case SWITCHM_74HCT4851D_5V:
            Mcal_Adc_Start_Collection(MCAL_ADC1_5V_CH);
            break;

        case SWITCHM_74HCT4851D_3V3:
            Mcal_Adc_Start_Collection(MCAL_ADC1_3V3_CH);
            break;

        default:
            break;
        }
    }
}

static void Mcal_Adc2_ConvCollection_Start(void)
{
    if (ENABLE == Mcal_AdcCtrl.EnableStatus[MCAL_ADC_CHANNEL_2])
    {
        switch (SwitchM_Get74hct4851dOutputStatusCallBack(SWITCHM_74HCT4851D_MULTIPLEX_ADC1))
        {
        case SWITCHM_74HCT4851D_CONNECTOR1_FRONT_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_CONNECTOR1_REAR_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_OIL1_INLET_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_OIL1_INLET_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_OIL1_OUTLET_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_OIL1_OUTLET_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_CONNECTOR2_FRONT_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_CONNECTOR2_REAR_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_OIL2_INLET_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_OIL2_INLET_TEMP_CH);
            break;

        case SWITCHM_74HCT4851D_OIL2_OUTLET_TEMP:
            Mcal_Adc_Start_Collection(MCAL_ADC2_OIL2_OUTLET_TEMP_CH);
            break;
        default:
            break;
        }
    }
}

void Mcal_Adc_SoftTimer_Enable(void)
{
  (void)OS_SoftTimerStart(&Mcal_AdcCtrl.SoftGptChannel[MCAL_ADC_CHANNEL_1], Mcal_Adc1_ConvCollection_Start, MCAL_ADC1_COLECTION_PERIOD, 1);
  (void)OS_SoftTimerStart(&Mcal_AdcCtrl.SoftGptChannel[MCAL_ADC_CHANNEL_2], Mcal_Adc2_ConvCollection_Start, MCAL_ADC2_COLECTION_PERIOD, 1); 
}

void Mcal_Adc_ConvCollection_Enable(Mcal_Adc_Channel_Num_e ch)
{
    Mcal_AdcCtrl.EnableStatus[ch] = ENABLE;
}

void Mcal_Adc_ConvCollection_Disable(Mcal_Adc_Channel_Num_e ch)
{
    Mcal_AdcCtrl.EnableStatus[ch] = DISABLE;
}

uint16_t Mcal_AdcDrv_GetAdcValue(Mcal_Adc_Channel_e ch)
{
    return AdcHardWareTriggerBuffer[ch];
}
/* End of file */
