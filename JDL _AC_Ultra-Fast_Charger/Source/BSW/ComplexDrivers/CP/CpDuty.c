//****************************************************************************************
//*
//* File Name: CpDuty.c
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
#include "CpDuty.h"
#include "STD_Lib.h"
#include "STD_Filter.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CPD_CFG_CURR_MIN            (6U)			/* cp can config min current value */
#define CPD_CFG_CURR_51A          	(51U)			/* cp config current threshold value */
#define CPD_CFG_CURR_63A          	(63U)			/* cp config current threshold value */
#define CPD_CFG_CURR_MAX            (125U)			/* cp can config max current value */
#define CPD_PERCENT_MAX             (100U)			/* cp max percent */
#define CPD_PWM_DFLT				(1000U)			/* cp default pwm freq and duty */

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	CPD_MODE_IDLE = 0U, /*cp duty idle mode*/
	CPD_MODE_NORMAL,	/*cp duty normal mode*/
	CPD_MODE_ADJ,		/*cp duty adjust mode*/
	CPD_MODE_ADJ_WAIT,	/*cp duty adjust wait mode*/
} CpDutyMode_Enum;

typedef enum
{
	CPD_DIODE_IDLE = 0u,
	CPD_DIODE_REQ_DETECT,
	CPD_DIODE_OUTPUT_NEGATIVE_VOL,
	CPD_DIODE_WATI_OUTPUT_STABLE,
	CPD_DIODE_READ_NEGATIVE_VOL,
	CPD_DIODE_DETECT_RESULT
} CpDutyDiode_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	CpDutyMode_Enum enMode;	  /*cp duty module mode enum*/
	uint8_t ucEnStatus;		  /*cp duty module enable status*/
	uint8_t ucPwmOutputFlag;  /*cp pwm output flag*/
	uint8_t ucPwmOutNegativeFlag;  /*cp pwm output -12V flag*/
	uint8_t ucAdjWaitTimeCnt; /*cp adjust pwm output wait time cnt*/
	uint8_t ucErrorResetCnt;  /*cp duty error reser cnt*/
	uint16_t usDfltCurrMax;	  /*cp default max current*/
	uint16_t usTargCurrMax;	  /*set targe max current*/
	uint16_t usReqCurrMax;	  /*request max current*/

	uint32_t ulDutyWrite;		 /*cp pwm duty write*/
	uint32_t ulDutyRead;		 /*cp pwn duty read*/
	uint32_t ulFreqRead;		 /*cp pwm freq read*/
	LibFilterStruct stFreqError; /*cp pwm freq error status filter*/
	LibFilterStruct stDutyError; /*cp duty error status filter*/

#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
	struct
	{
		CpDutyDiode_Enum MachineState;
		uint8_t ucDiodeDetectResult;
		uint8_t ucDiodeDetectFlag;
		uint8_t ucReqSelfCheckFlag;
		Filter_Struct ucSelfCheckStatus;
		uint16_t usWaitCnt;
	} Diode_Struct;
#endif

	/* polyspace<MISRA-C3:5.6:Not a defect:Justified> NO fluence */
} CpDuty_Struct;

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */
static CpDuty_Struct gv_stCpD[SYS_CONNECTOR_NUM_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint8_t CPD_CurrConvertToDuty(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue,uint32_t *plv_usDuty);
static void CPD_PwmDutyFreqErrCheck(SysConnector_Num_Enum ch);
#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
static void CPD_CarDiodeDetect(SysConnector_Num_Enum ch);
#endif
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : CPD_InitMemory
Syntax          : void CPD_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPD Module Initialize RAM
Call By         :
|******************************************************************************/
void CPD_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)(&gv_stCpD), 0u, (uint16_t)(sizeof(gv_stCpD) / sizeof(uint8_t)));
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCpD[ch].usDfltCurrMax = CPD_CFG_MAX_CURR_DFLT;
		gv_stCpD[ch].usReqCurrMax = CPD_CFG_MAX_CURR_DFLT;
		gv_stCpD[ch].stFreqError.ucValidStatus = 0xff;
		gv_stCpD[ch].stDutyError.ucValidStatus = 0xff;
	}
}

/*******************************************************************************
Name            : CPD_Open
Syntax          : void CPD_Open(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Enable CPD Module
Call By         :
|******************************************************************************/
void CPD_Open(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCpD[ch].ucEnStatus = STD_TRUE;
		gv_stCpD[ch].ucPwmOutputFlag = STD_FALSE;
		(void)CPD_SetPwmOutput(ch, CPD_PWM_DFLT, CPD_PWM_DFLT);
	}
	// Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 1000, 1000); // Set cp1 level hight
	// Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP2, 1000, 1000); // Set cp2 level hight
}

/*******************************************************************************
Name            : CPD_Close
Syntax          : void CPD_Close(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Disable CPD Module
Call By         :
|******************************************************************************/
void CPD_Close(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCpD[ch].ucEnStatus = STD_FALSE;
	}
}

/*******************************************************************************
Name            : CPD_OutputEnable
Syntax          : void CPD_OutputEnable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Output Enable
Call By         :
|******************************************************************************/
void CPD_OutputEnable(SysConnector_Num_Enum ch)
{
	gv_stCpD[ch].ucPwmOutputFlag = STD_TRUE;
}

/*******************************************************************************
Name            : CPD_OutputDisable
Syntax          : void CPD_OutputDisable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Output Disable
Call By         :
|******************************************************************************/
void CPD_OutputDisable(SysConnector_Num_Enum ch)
{
	gv_stCpD[ch].ucPwmOutputFlag = STD_FALSE;
}

uint8_t CPD_IsOutputEnabled(SysConnector_Num_Enum ch)
{
	return gv_stCpD[ch].ucPwmOutputFlag;
}

void CPD_OutputNegative12VEnable(SysConnector_Num_Enum ch)
{
	gv_stCpD[ch].ucPwmOutNegativeFlag = STD_TRUE;
}

void CPD_OutputNegative12VDisable(SysConnector_Num_Enum ch)
{
	gv_stCpD[ch].ucPwmOutNegativeFlag = STD_FALSE;
}
/*******************************************************************************
Name            : CPD_ConfigCurrentValue
Syntax          : void CPD_ConfigCurrentValue(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :- 
Description     : Config Curren Value
Call By         :
|******************************************************************************/
void CPD_ConfigCurrentValue(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue)
{
	if (lv_usCurrValue < CPD_CFG_CURR_MIN)
	{
		gv_stCpD[ch].usDfltCurrMax = CPD_CFG_CURR_MIN;
	}
	else if (lv_usCurrValue > CPD_CFG_CURR_MAX)
	{
		gv_stCpD[ch].usDfltCurrMax = CPD_CFG_CURR_MAX;
	}
	else
	{
		gv_stCpD[ch].usDfltCurrMax = lv_usCurrValue;
	}

	if (gv_stCpD[ch].usDfltCurrMax > CPD_CFG_MAX_CURR_DFLT)
	{
		gv_stCpD[ch].usDfltCurrMax = CPD_CFG_MAX_CURR_DFLT;
	}

	gv_stCpD[ch].usReqCurrMax = gv_stCpD[ch].usDfltCurrMax;
}

/*******************************************************************************
Name            : CPD_AdjustCurrentValue
Syntax          : void CPD_AdjustCurrentValue(SysConnector_Num_Enum ch, uint8_t lv_ucPercent, uint8_t lv_ucEn)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Adjust Curren Value
Call By         :
|******************************************************************************/
void CPD_AdjustCurrentValue(SysConnector_Num_Enum ch, uint8_t lv_ucPercent, uint8_t lv_ucEn)
{
	if (STD_TRUE == lv_ucEn)
	{
		if (lv_ucPercent <= CPD_PERCENT_MAX)
		{
			gv_stCpD[ch].usReqCurrMax = (gv_stCpD[ch].usDfltCurrMax * (uint16_t)lv_ucPercent) / (uint16_t)CPD_PERCENT_MAX;
		}
		else
		{
	 	}
	}
	else
	{
		gv_stCpD[ch].usReqCurrMax = gv_stCpD[ch].usDfltCurrMax;
	}
}

/*******************************************************************************
Name            : CPD_GetCpDuty
Syntax          : uint32_t CPD_GetCpDuty(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get CP Duty
Call By         :
|******************************************************************************/
uint32_t CPD_GetCpDuty(SysConnector_Num_Enum ch)
{
	return gv_stCpD[ch].ulDutyRead;
}

/*******************************************************************************
Name            : CPD_GetCpFreq
Syntax          : uint32_t CPD_GetCpFreq(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get CP Freq
Call By         :
|******************************************************************************/
uint32_t CPD_GetCpFreq(SysConnector_Num_Enum ch)
{
	return gv_stCpD[ch].ulFreqRead;
}

/*******************************************************************************
Name            : CPD_MainFunction
Syntax          : void CPD_MainFunction(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPD Main Function
Call By         :
|******************************************************************************/
void CPD_MainFunction(SysConnector_Num_Enum ch)
{
	if ((STD_FALSE == gv_stCpD[ch].ucEnStatus) || (STD_FALSE == gv_stCpD[ch].ucPwmOutputFlag) || (STD_TRUE == gv_stCpD[ch].ucPwmOutNegativeFlag))
	{
#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
		if (STD_FALSE == gv_stCpD[ch].Diode_Struct.ucDiodeDetectFlag)
#endif
		{
			gv_stCpD[ch].enMode = CPD_MODE_IDLE;
			if (STD_FALSE == gv_stCpD[ch].ucPwmOutNegativeFlag)
			{
				(void)CPD_SetPwmOutput(ch, CPD_PWM_DFLT, CPD_PWM_DFLT);
				gv_stCpD[ch].ulDutyRead = CPD_PWM_DFLT;
			}
			else
			{
				(void)CPD_SetPwmOutput(ch, CPD_PWM_DFLT, 0);
				gv_stCpD[ch].ulDutyRead = 0;
			}
		}
	}
	else
	{
		switch (gv_stCpD[ch].enMode)
		{
		case CPD_MODE_IDLE:
		{
			gv_stCpD[ch].ucAdjWaitTimeCnt = 0u;
			gv_stCpD[ch].enMode = CPD_MODE_ADJ_WAIT;
			gv_stCpD[ch].ucErrorResetCnt = 0u;
			gv_stCpD[ch].usTargCurrMax = gv_stCpD[ch].usDfltCurrMax;
			(void)CPD_CurrConvertToDuty(ch, gv_stCpD[ch].usTargCurrMax, &gv_stCpD[ch].ulDutyWrite);
			(void)CPD_SetPwmOutput(ch, CPD_PWM_DFLT, gv_stCpD[ch].ulDutyWrite);
		}
		break;

		case CPD_MODE_NORMAL:
		{
			if (gv_stCpD[ch].usTargCurrMax != gv_stCpD[ch].usReqCurrMax)
			{
				gv_stCpD[ch].usTargCurrMax = gv_stCpD[ch].usReqCurrMax;
				gv_stCpD[ch].enMode = CPD_MODE_ADJ;
			}
			else
			{
				CPD_PwmDutyFreqErrCheck(ch);
			}
		}
		break;

		case CPD_MODE_ADJ:
		{
			(void)CPD_CurrConvertToDuty(ch, gv_stCpD[ch].usTargCurrMax, &gv_stCpD[ch].ulDutyWrite);
			(void)CPD_SetPwmOutput(ch, CPD_PWM_DFLT, gv_stCpD[ch].ulDutyWrite);
			gv_stCpD[ch].ucAdjWaitTimeCnt = 0u;
			gv_stCpD[ch].enMode = CPD_MODE_ADJ_WAIT;
		}
		break;

		case CPD_MODE_ADJ_WAIT:
		{
			gv_stCpD[ch].ucAdjWaitTimeCnt++;
			if (gv_stCpD[ch].ucAdjWaitTimeCnt >= CPD_ADJ_WAIT_TIME)
			{
				gv_stCpD[ch].enMode = CPD_MODE_NORMAL;
			}
		}
		break;

		default:
		{
			gv_stCpD[ch].enMode = CPD_MODE_IDLE;
		}
		break;
		}
	}
#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
	CPD_CarDiodeDetect(ch);
#endif
}

/*******************************************************************************
Name            : CPD_DiagPwmOutput
Syntax          : static void CPD_DiagPwmOutput(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Diag CP Pwm Output Status
Call By         :
|******************************************************************************/
static void CPD_PwmDutyFreqErrCheck(SysConnector_Num_Enum ch)
{
#if (STD_ON == CPD_DIAG_PWM_FUN_EN)
	uint8_t lv_ucDutyTestResult;
	uint8_t lv_ucFreqTestResult;
	uint16_t lv_usFreqAbs = 0;
	uint16_t lv_usDutyAbs = 0;

	(void)CPD_GetPwmFreqDuty(ch, &gv_stCpD[ch].ulFreqRead, &gv_stCpD[ch].ulDutyRead);

	if (CPD_PWM_DFLT == gv_stCpD[ch].ulDutyRead)
	{
		if (gv_stCpD[ch].ulFreqRead >= CPD_PWM_DFLT)
		{
			lv_usFreqAbs = (uint16_t)(gv_stCpD[ch].ulFreqRead - CPD_PWM_DFLT);
		}
		else
		{
			lv_usFreqAbs = (uint16_t)(CPD_PWM_DFLT - gv_stCpD[ch].ulFreqRead);
		}
		if (lv_usFreqAbs > CPD_DIAG_IN_FREQ_ERR_ABS_VALUE)
		{
			gv_stCpD[ch].stFreqError.ucStatus = STD_TRUE;
			// CP_DEBUG("stFreqError:ulFreqRead:%d,ulDutyRead:%d\r\n",gv_stCpD[ch].ulFreqRead,gv_stCpD[ch].ulDutyRead);
		}
		else if (lv_usFreqAbs < CPD_DIAG_OUT_FREQ_ERR_ABS_VALUE)
		{
			gv_stCpD[ch].stFreqError.ucStatus = STD_FALSE;
		}
		else
		{
			if (STD_FALSE == gv_stCpD[ch].stFreqError.ucValidStatus)
			{
				gv_stCpD[ch].stFreqError.ucStatus = STD_FALSE;
			}
			else if (STD_TRUE == gv_stCpD[ch].stFreqError.ucValidStatus)
			{
				gv_stCpD[ch].stFreqError.ucStatus = STD_TRUE;
			}
			else
			{
			}
		}
	}
	lv_ucFreqTestResult = LIB_StatusFilter(&gv_stCpD[ch].stFreqError, CPD_DUTY_FILTER_MAX_CNT);

	//	CP_DEBUG("gv_stCpD[ch].ulDutyRead = %d  gv_stCpD[ch].ulDutyWrite = %d\r\n",gv_stCpD[ch].ulDutyRead,gv_stCpD[ch].ulDutyWrite);
	if (gv_stCpD[ch].ulDutyRead >= gv_stCpD[ch].ulDutyWrite)
	{
		lv_usDutyAbs = (uint16_t)(gv_stCpD[ch].ulDutyRead - gv_stCpD[ch].ulDutyWrite);
	}
	else
	{
		lv_usDutyAbs = (uint16_t)(gv_stCpD[ch].ulDutyWrite - gv_stCpD[ch].ulDutyRead);
	}
	if (lv_usDutyAbs > CPD_DIAG_IN_DUTY_ERRABS_VALUE)
	{
		CP_DEBUG("stDutyError:ulFreqRead:%d,ulDutyRead:%d\r\n", gv_stCpD[ch].ulFreqRead, gv_stCpD[ch].ulDutyRead);
		gv_stCpD[ch].stDutyError.ucStatus = STD_TRUE;
	}
	else if (lv_usDutyAbs < CPD_DIAG_OUT_DUTY_ERRABS_VALUE)
	{
		gv_stCpD[ch].stDutyError.ucStatus = STD_FALSE;
	}
	else
	{
		if (STD_FALSE == gv_stCpD[ch].stDutyError.ucValidStatus)
		{
			gv_stCpD[ch].stDutyError.ucStatus = STD_FALSE;
		}
		else if (STD_TRUE == gv_stCpD[ch].stDutyError.ucValidStatus)
		{
			gv_stCpD[ch].stDutyError.ucStatus = STD_TRUE;
		}
		else
		{
		}
	}
	lv_ucDutyTestResult = LIB_StatusFilter(&gv_stCpD[ch].stDutyError, CPD_FREQ_FILTER_MAX_CNT);

	if ((STD_TRUE == gv_stCpD[ch].stDutyError.ucValidStatus) || (STD_TRUE == gv_stCpD[ch].stFreqError.ucValidStatus))
	{
		if (gv_stCpD[ch].ucErrorResetCnt < CPD_ERROR_RESET_MAX_CNT)
		{
			gv_stCpD[ch].ucErrorResetCnt++;
			gv_stCpD[ch].enMode = CPD_MODE_ADJ;

			LIB_SetMemory((uint8_t *)(&gv_stCpD[ch].stDutyError), 0u, (uint16_t)(sizeof(gv_stCpD[ch].stDutyError) / sizeof(uint8_t)));
			LIB_SetMemory((uint8_t *)(&gv_stCpD[ch].stFreqError), 0u, (uint16_t)(sizeof(gv_stCpD[ch].stFreqError) / sizeof(uint8_t)));
		}
		else
		{
			CpDrvif_SetFaultStatus(ERRHDL_ID_CP_PWM, STD_TRUE);
			gv_stCpD[ch].enMode = CPD_MODE_IDLE;
			LIB_SetMemory((uint8_t *)(&gv_stCpD[ch].stDutyError), 0u, (uint16_t)(sizeof(gv_stCpD[ch].stDutyError) / sizeof(uint8_t)));
			LIB_SetMemory((uint8_t *)(&gv_stCpD[ch].stFreqError), 0u, (uint16_t)(sizeof(gv_stCpD[ch].stFreqError) / sizeof(uint8_t)));
		}
	}
	else
	{
		CpDrvif_SetFaultStatus(ERRHDL_ID_CP_PWM, STD_FALSE);
	}

#endif
}

/*******************************************************************************
Name            : CPD_CurrConvertToDuty
Syntax          : static uint8_t CPD_CurrConvertToDuty(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue, uint32_t *plv_usDuty)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Conver Crurren To Duty
Call By         :
|******************************************************************************/
static uint8_t CPD_CurrConvertToDuty(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue, uint32_t *plv_usDuty)
{
	uint8_t lv_ucRet = STD_TRUE;

	if ((lv_usCurrValue >= CPD_CFG_CURR_MIN) && (lv_usCurrValue <= CPD_CFG_CURR_51A))
	{
		*plv_usDuty = (uint32_t)((lv_usCurrValue * 100U) / 6u); /* [100-850] */
	}
	else if ((CPD_CFG_CURR_51A < lv_usCurrValue) && (lv_usCurrValue <= CPD_CFG_CURR_63A))
	{
		*plv_usDuty = (uint32_t)(lv_usCurrValue * 100u / 25u + 640u); /* [850-900] */
	}
	else if ((CPD_CFG_CURR_63A < lv_usCurrValue) && (lv_usCurrValue <= CPD_CFG_CURR_MAX))
	{
		*plv_usDuty = (uint32_t)(lv_usCurrValue - 63u + 900u); /* [900-970] */
	}
	else if (lv_usCurrValue < CPD_CFG_CURR_MIN)
	{
		*plv_usDuty = 1000u; /*[100]*/
	}
	else
	{
		lv_ucRet = STD_FALSE;
	}
	return lv_ucRet;
}

#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
static void CPD_CarDiodeDetect(SysConnector_Num_Enum ch)
{
	static uint8_t lastMachineState = 0xff;
	if (lastMachineState != gv_stCpD[ch].Diode_Struct.MachineState)
	{
		lastMachineState = gv_stCpD[ch].Diode_Struct.MachineState;
	}
	switch (gv_stCpD[ch].Diode_Struct.MachineState)
	{
		case CPD_DIODE_IDLE:
			if (gv_stCpD[ch].Diode_Struct.ucReqSelfCheckFlag == STD_TRUE)
			{
				gv_stCpD[ch].Diode_Struct.ucReqSelfCheckFlag = STD_FALSE;
				gv_stCpD[ch].Diode_Struct.ucDiodeDetectFlag = STD_TRUE;
				gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_REQ_DETECT;
				gv_stCpD[ch].Diode_Struct.ucSelfCheckStatus.ucValidStatus = CPD_DIODE_SELF_FAILED;
				gv_stCpD[ch].Diode_Struct.ucDiodeDetectResult = CPD_DIODE_SELF_PROCESS;
			}
			break;

		case CPD_DIODE_REQ_DETECT:
			gv_stCpD[ch].Diode_Struct.usWaitCnt++;
			if (gv_stCpD[ch].Diode_Struct.usWaitCnt >= CPD_DIODE_WAIT_TO_OUTPUT)
			{
				gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_OUTPUT_NEGATIVE_VOL;
				gv_stCpD[ch].Diode_Struct.usWaitCnt = 0;
			}
			break;

		case CPD_DIODE_OUTPUT_NEGATIVE_VOL:
			CPD_SetPwmOutput(CPD_PWM_DFLT, 0u);
			gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_WATI_OUTPUT_STABLE;
			break;

		case CPD_DIODE_WATI_OUTPUT_STABLE:
			gv_stCpD[ch].Diode_Struct.usWaitCnt++;
			if (gv_stCpD[ch].Diode_Struct.usWaitCnt >= CPD_DIODE_WAIT_TO_STABLE)
			{
				gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_READ_NEGATIVE_VOL;
				gv_stCpD[ch].Diode_Struct.usWaitCnt = 0;
			}
			break;

		case CPD_DIODE_READ_NEGATIVE_VOL:
			if (GPIO_PIN_SET == CPD_GET_DIODE_GPIO())
			{
				gv_stCpD[ch].Diode_Struct.ucSelfCheckStatus.ucStatus = CPD_DIODE_SELF_SUCCESS;
			}
			else
			{
				gv_stCpD[ch].Diode_Struct.ucSelfCheckStatus.ucStatus = CPD_DIODE_SELF_FAILED;
			}

			if (STD_TRUE == FILTER_StatusFiltT1(&gv_stCpD[ch].Diode_Struct.ucSelfCheckStatus, CPD_SET_DIODE_DETECT_CNT))
			{
				if (CPD_DIODE_SELF_SUCCESS == gv_stCpD[ch].Diode_Struct.ucSelfCheckStatus.ucValidStatus)
				{
					gv_stCpD[ch].Diode_Struct.ucDiodeDetectResult = CPD_DIODE_SELF_SUCCESS;
				}
				else
				{
					gv_stCpD[ch].Diode_Struct.ucDiodeDetectResult = CPD_DIODE_SELF_FAILED;
				}
				gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_DETECT_RESULT;
			}

			break;
		case CPD_DIODE_DETECT_RESULT:

			gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_IDLE;
			gv_stCpD[ch].Diode_Struct.ucDiodeDetectFlag = STD_FALSE;
			gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_IDLE;
			break;
		default:
			gv_stCpD[ch].Diode_Struct.MachineState = CPD_DIODE_IDLE;
		}
}

void CPD_ReqCarDiodeDetect(void)
{
	gv_stCpD[ch].Diode_Struct.ucReqSelfCheckFlag = STD_TRUE;
}

uint8_t CPD_CarDiodeDetectResult(void)
{
	return gv_stCpD[ch].Diode_Struct.ucDiodeDetectResult;
}

uint8_t CPD_DiodeDetectFlag(void)
{
	uint8_t lv_ucRtn = STD_FALSE;

	if ((STD_TRUE == gv_stCpD[ch].ucPwmOutputFlag && 1000u == gv_stCpD[ch].ulDutyRead) || (STD_TRUE == gv_stCpD[ch].Diode_Struct.ucDiodeDetectFlag))
	{
		lv_ucRtn = STD_TRUE;
	}
	return lv_ucRtn;
}
#endif
/* polyspace:end<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*EOF*/
