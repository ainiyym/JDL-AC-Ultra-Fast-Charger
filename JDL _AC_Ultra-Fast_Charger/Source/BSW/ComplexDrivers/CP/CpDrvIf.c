//****************************************************************************************
//*
//* File Name: CpDrvIf.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//*
/*******************************************************************************/
/* polyspace:begin<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Fifo.h"
#include "CpDrvIf.h"
#include "STD_LogService.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static uint8_t uc_enGptChannel = 0u;

/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
const CPDRV_ConnectorCfg_s CPDRV_ConnectorCfgTable[SYS_CONNECTOR_NUM_MAX] =
	{
		{SYS_CONNECTOR1, MCAL_GPT_PWM_CAPTURE_CP, MCAL_GPT_CH_PWM_OUT_CP1, MCAL_ADC1_CP1_CH, FIFO_CHAN_CP1_VOLT},
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
		{SYS_CONNECTOR2, MCAL_GPT_PWM_CAPTURE_CP, MCAL_GPT_CH_PWM_OUT_CP2, MCAL_ADC1_CP2_CH, FIFO_CHAN_CP2_VOLT}
#endif
};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : CPDRV_GetDutyPeriod
Syntax          : uint8_t CPDRV_GetDutyPeriod(SysConnector_Num_Enum ch, uint32_t *lv_pPeriod, uint32_t *lv_pDuty)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get Duty And Period From Mcal Level
				  *Duty_p = 100% (100000); *Period_p = 1000us (1kHz);
Call By         :
|******************************************************************************/
uint8_t CPDRV_GetDutyPeriod(SysConnector_Num_Enum ch, uint32_t *lv_pPeriod, uint32_t *lv_pDuty)
{
	uint8_t lv_ucRtn = STD_FALSE;
	float lv_flDuty = 0;

	if (lv_pDuty == STD_NULL || lv_pPeriod == STD_NULL)
	{
		lv_ucRtn = STD_FALSE;
	}
	else
	{
		if (MCAL_RET_SUCCESS == CpDrvif_GetDutyCycle(CPDRV_ConnectorCfgTable[ch].PwmIcCh, &lv_flDuty) \
		&& MCAL_RET_SUCCESS == CpDrvif_GetDutyFrequency(CPDRV_ConnectorCfgTable[ch].PwmIcCh, lv_pPeriod))
		{
			*lv_pDuty = (uint32_t)lv_flDuty;
			lv_ucRtn = STD_TRUE;
		}
	}
	return lv_ucRtn;
}

/*******************************************************************************
Name            : CPDRV_SetPwm
Syntax          : uint8_t CPDRV_SetPwm(SysConnector_Num_Enum ch, uint32_t lv_ulFreqValue, uint32_t lv_ulDutyValue)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Set Pwm To Mcal Level
Call By         :
|******************************************************************************/
extern uint8_t CPD_IsOutputEnabled(SysConnector_Num_Enum ch);

uint8_t CPDRV_SetPwm(SysConnector_Num_Enum ch, uint32_t lv_ulFreqValue, uint32_t lv_ulDutyValue)
{
	uint8_t lv_ucRtn = STD_TRUE;
	static uint32_t st_ulFreqValueCh1 = 0;
	static uint32_t st_ulDutyValueCh1 = 0;
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
	static uint32_t st_ulFreqValueCh2 = 0;
	static uint32_t st_ulDutyValueCh2 = 0;
#endif

	if (SYS_CONNECTOR1 == ch && (lv_ulFreqValue != st_ulFreqValueCh1 || lv_ulDutyValue != st_ulDutyValueCh1))
	{
		st_ulFreqValueCh1 = lv_ulFreqValue;
		st_ulDutyValueCh1 = lv_ulDutyValue;

		if (lv_ulFreqValue == 1000 && (lv_ulDutyValue == 1000 || lv_ulDutyValue == 0))
		{
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
			if (STD_TRUE != CPD_IsOutputEnabled(SYS_CONNECTOR2))
#endif
			{
				CpDrvif_CpAdVolCollEnable(CPDRVIF_CP_ADC1);
			}
		}
		else
		{
			CpDrvif_CpAdVolCollDisable(CPDRVIF_CP_ADC1);
		}
		CPDrvif_SetPwm(CPDRV_ConnectorCfgTable[ch].PwmOutCh, lv_ulFreqValue, lv_ulDutyValue);
	}
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
	if (SYS_CONNECTOR2 == ch && (lv_ulFreqValue != st_ulFreqValueCh2 || lv_ulDutyValue != st_ulDutyValueCh2))
	{
		st_ulFreqValueCh2 = lv_ulFreqValue;
		st_ulDutyValueCh2 = lv_ulDutyValue;

		if (lv_ulFreqValue == 1000 && (lv_ulDutyValue == 1000 || lv_ulDutyValue == 0))
		{
			if (STD_TRUE != CPD_IsOutputEnabled(SYS_CONNECTOR1))
			{
				CpDrvif_CpAdVolCollEnable(CPDRVIF_CP_ADC1);
			}
		}
		else
		{
			CpDrvif_CpAdVolCollDisable(CPDRVIF_CP_ADC1);
		}
		CPDrvif_SetPwm(CPDRV_ConnectorCfgTable[ch].PwmOutCh, lv_ulFreqValue, lv_ulDutyValue);
	}
#endif
	return lv_ucRtn;
}

/*******************************************************************************
Name            : CPDRV_GetAdcValue
Syntax          : static void CPDRV_GetAdcValue(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get Adc Value From Mcal Level
Call By         :
|******************************************************************************/
static void CPDRV_GetAdcValue(void)
{
	uint16_t lv_ulAdcValue = 0;

	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		lv_ulAdcValue = CPDrvif_GetCpVolAdcValue(CPDRV_ConnectorCfgTable[ch].CpAdVolCollectionCh);
		(void)FIFO_InsertData(CPDRV_ConnectorCfgTable[ch].CpAdVolFifoCh, (uint16_t)lv_ulAdcValue);
	}
}
/*******************************************************************************
Name            : CPDRV_StartAdcCollection
Syntax          : void CPDRV_StartAdcCollection(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Start Adc Collection
Call By         :
|******************************************************************************/
void CPDRV_StartAdcCollection(void)
{
	(void)OS_SoftTimerStart(&uc_enGptChannel, CPDRV_GetAdcValue, CP_ADC_COLECTION_PERIOD, 1);  
}

/*******************************************************************************
Name            : CPDRV_StopAdcCollection
Syntax          : void CPDRV_StopAdcCollection(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Stop Adc Collection
Call By         :
|******************************************************************************/
void CPDRV_StopAdcCollection(void)
{
	OS_SoftTimerStop(uc_enGptChannel);
}
