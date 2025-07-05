//****************************************************************************************
//*
//* File Name: CpDrvIf_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (_CPDRVIF_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _CPDRVIF_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "Std_Types.h"
#include "Mcal_App.h"
#include "STD_Os_Timer.h"
#include "STD_ErrorHandler.h"
#include"SwitchM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CPDRVIF_CP_ADC1                 MCAL_ADC_CHANNEL_1
#define CPDRVIF_CP_ADC2                 MCAL_ADC_CHANNEL_2
#define CPDRVIF_CP_ADC_CH_NUM           MCAL_ADC_CHANNEL_NUM_MAX
#define CpDrvif_CpAdVolCollEnable       Mcal_Adc_ConvCollection_Enable              /* enable collection cp ad vol */
#define CpDrvif_CpAdVolCollDisable      Mcal_Adc_ConvCollection_Disable             /* disable collection cp ad vol */

#define CpDrvif_GetDutyCycle(ch, DutyCycle)                         0//Mcal_GptDrrv_Icu_It_GetDutyCycle(ch, DutyCycle)
#define CpDrvif_GetDutyFrequency(ch, Frequency)                     0//Mcal_GptDrrv_Icu_It_GetFrequency(ch, Frequency)
#define CPDrvif_SetPwm(ch, FreqValue, DutyValue)                    Mcal_Set_Pwm_Param(ch, FreqValue, DutyValue);Mcal_GptDrv_Pwm_Start(ch)                                
#define CPDrvif_GetCpVolAdcValue(ch)                                Mcal_AdcDrv_GetAdcValue(ch)
#define CpDrvif_SetFaultStatus(lv_enFaultIdNum, lv_ucFaultStatus)   ERRHDL_FaultStatusUpdata_CallBack(lv_enFaultIdNum, lv_ucFaultStatus)

#define CP_DEBUG(fmt, ...)                                          LOG_DEBUG(LOG_MODULE_CP, fmt, ##__VA_ARGS__) /* cp module log output */
#define CP_ERROR(fmt, ...)                                          LOG_ERROR(LOG_MODULE_CP, fmt, ##__VA_ARGS__) /* cp module log output */
#define CP_PRINT_Hex(A, B)                                          LogService_Print_Hex_Array(LOG_MODULE_CP, A, B, 1)
#define CP_ADC_COLECTION_PERIOD                                     (SWITCHM_74HCT4851D_SOFTTIMER_PERIOD * 8)   /* CP ADC Collection period */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct 
{
    SysConnector_Num_Enum ConnectorNum;             /* connector number */
    Mcal_GPT_PWM_IC_Channel_Id_t PwmIcCh;           /* PWM capture channel */
    Mcal_GPT_PWM_OUT_Channel_Id_t PwmOutCh;         /* PWM out chanel */
    Mcal_Adc_Channel_e  CpAdVolCollectionCh;        /* Cp ad vol collect chanel */
    uint32_t  CpAdVolFifoCh;                        /* Cp ad vol fifo chanel */
}CPDRV_ConnectorCfg_s;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/


