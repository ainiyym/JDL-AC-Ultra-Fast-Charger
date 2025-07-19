//****************************************************************************************
//*
//* File Name: STD_Volt.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-19 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include "STD_Volt.h"
#include "STD_Lib.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define VOLT_JOB_OK												(0u)		/*voltage module job result success*/
#define VOLT_JOB_FAILED											(1u)		/*voltage module job result failed*/
#define VOLT_JOB_PENDING										(2u)		/*voltage module job result pending*/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    VOLT_MODE_IDLE = 0u,					/*Voltage module idle mode*/
    VOLT_MODE_SELFTEST,						/*Voltage module selftest mode*/
    VOLT_MODE_NORMAL,						/*Voltage module normal mode*/
}VoltMode_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucVoltEnCaliVal;		   /*Voltage enablement scale*/
	uint16_t usVoltTempVal;			   /*Channel voltage value*/
	LibFilterStruct stLv1OverVoltFilt; /*First stage overvoltage fault state filtering*/
	LibFilterStruct stLv2OverVoltFilt; /*Second stage overvoltage fault state filtering*/
	LibFilterStruct stLv1LowVoltFilt;  /*First stage lowvoltage fault status filtering*/
	LibFilterStruct stLv2LowVoltFilt;  /*First stage lowvoltage fault status filtering*/
} VoltCh_Struct;

typedef struct
{
	VoltMode_Enum enMode;							  /*Voltage module mode status*/
	uint8_t ucEnStatus;								  /*Module enablement status*/
	uint8_t ucFltPhsJobResult;						  /*Fire zero error phase working result*/
	uint16_t usFltPhsWaitCnt;						  /*Fire zero error phase detection wait count*/
	LibFilterStruct stFltPhsStatusFilt;				  /*Fire zero fault phase fault state filtering*/
	VoltCh_Struct stChanVartArray[VOLT_CHAN_MAX_NUM]; /*Channel structure*/
} Volt_Struct;

/*******************************************************************************
|    Global NVM KAM Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static  Volt_Struct gv_stVolt[SYS_CONNECTOR_NUM_MAX];

/****************************************************************************************
 	Global Variable Definition Section
*****************************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void VOLT_ModeIdleHandle(SysConnector_Num_Enum ch);
static void VOLT_ModeSelfTestHandle(SysConnector_Num_Enum ch);
static void VOLT_ModeNormalHandle(SysConnector_Num_Enum ch);
static void VOLT_InputModeInitHandle(SysConnector_Num_Enum ch);
#if (STD_ON == VOLT_FAULT_PHASE_FUN_EN)
static void VOLT_FaultPhaseHandle(SysConnector_Num_Enum ch);
#endif
#if (STD_ON == VOLT_LV1_OVERVOLT_FUN_EN)
static void VOLT_Lv1OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan);
#endif
#if (STD_ON == VOLT_LV2_OVERVOLT_FUN_EN)
static void VOLT_Lv2OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan);
#endif
#if (STD_ON == VOLT_LV1_LOWVOLT_FUN_EN)
static void VOLT_Lv1LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan);
#endif
#if (STD_ON == VOLT_LV2_LOWVOLT_FUN_EN)
static void VOLT_Lv2LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan);
#endif
static void VOLT_NormalModeErrHandle(SysConnector_Num_Enum ch);

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : VOLT_InitMemory
Syntax          : void VOLT_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : VOLT Module Initialize RAM
Call By         : STD_SysM
|******************************************************************************/
void VOLT_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)&gv_stVolt, (0u), (uint16_t)(sizeof(gv_stVolt) / sizeof(uint8_t)));
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		for (uint8_t lv_ucChan = 0u; lv_ucChan < VOLT_CHAN_MAX_NUM; lv_ucChan++)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucValidStatus = 0xff;
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucValidStatus = 0xff;
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucValidStatus = 0xff;
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucValidStatus = 0xff;
		}
	}
}

/*******************************************************************************
Name            : VOLT_Enable
Syntax          : void VOLT_Enable(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Module Enable
Call By         : STD_SysM
|******************************************************************************/
void VOLT_Enable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stVolt[ch].ucEnStatus = STD_TRUE;
#if VOLT_FAULT_PHASE_FUN_EN == STD_OFF
		gv_stVolt[ch].ucFltPhsJobResult = VOLT_JOB_OK;
#elif VOLT_FAULT_PHASE_FUN_EN == STD_ON
		gv_stVolt[ch].ucFltPhsJobResult = VOLT_JOB_PENDING;
#endif
	}
}

/*******************************************************************************
Name            : VOLT_Enable
Syntax          : void VOLT_Enable(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Module Disable
Call By         : STD_SysM
|******************************************************************************/
void VOLT_Disable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stVolt[ch].ucEnStatus = STD_FALSE;
	}
}

uint16_t VOLT_GetL1VoltValue(SysConnector_Num_Enum ch)
{
	return gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal;
}

uint16_t VOLT_GetL2VoltValue(SysConnector_Num_Enum ch)
{
	return gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal;
}

uint16_t VOLT_GetL3VoltValue(SysConnector_Num_Enum ch)
{
	return gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal;
}

/*******************************************************************************
Name            : VOLT_IdleHandle
Syntax          : static void VOLT_ModeIdleHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Idle mode handle
Call By         : VOLT_20msFunctionControl
|******************************************************************************/
static void VOLT_ModeIdleHandle(SysConnector_Num_Enum ch)
{
	if ((STD_TRUE == VOLT_GetSysPrepareStatus()) && (STD_TRUE == gv_stVolt[ch].ucEnStatus) && (STD_TRUE == VOLT_GetMeterPrepareStatus(ch)))
	{
		gv_stVolt[ch].enMode = VOLT_MODE_SELFTEST;
		VOLT_DEBUG("Into VOLT_MODE_SELFTEST\r\n");
	}
}

/*******************************************************************************
Name            : VOLT_SelfTestHandle
Syntax          : static VOLT_ModeSelfTestHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : SelfTest mode handle
Call By         : VOLT_20msFunctionControl
|******************************************************************************/
static void VOLT_ModeSelfTestHandle(SysConnector_Num_Enum ch)
{
	VOLT_GetAllVoltVailVal(ch);;
#if VOLT_FAULT_PHASE_FUN_EN == STD_ON
	VOLT_FaultPhaseHandle(ch);
#endif
	VOLT_InputModeInitHandle(ch);
	if (VOLT_JOB_PENDING != gv_stVolt[ch].ucFltPhsJobResult)
	{
		gv_stVolt[ch].enMode = VOLT_MODE_NORMAL;
		VOLT_DEBUG("Into VOLT_MODE_NORMAL\r\n");
	}
}

/*******************************************************************************
Name            : VOLT_NormalHandle
Syntax          : static VOLT_ModeNormalHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Normal mode handle
Call By         : VOLT_20msFunctionControl
|******************************************************************************/
static void VOLT_ModeNormalHandle(SysConnector_Num_Enum ch)
{
	if (STD_FALSE == gv_stVolt[ch].ucEnStatus)
	{
		gv_stVolt[ch].enMode = VOLT_MODE_IDLE;
		VOLT_DEBUG("Into VOLT_MODE_IDLE\r\n");
		LIB_SetMemory((uint8_t *)&gv_stVolt[ch].stChanVartArray, (0u), (uint16_t)(sizeof(gv_stVolt[ch].stChanVartArray) / sizeof(uint8_t)));
	}
	else
	{
		VOLT_GetAllVoltVailVal(ch);
		for (uint8_t lv_ucChan = 0u; lv_ucChan < VOLT_CHAN_MAX_NUM; lv_ucChan++)
		{
			if (STD_TRUE == gv_stVolt[ch].stChanVartArray[lv_ucChan].ucVoltEnCaliVal)
			{
#if (STD_ON == VOLT_LV1_OVERVOLT_FUN_EN)
				VOLT_Lv1OverVoltHandle(lv_ucChan);
#endif
#if (STD_ON == VOLT_LV2_OVERVOLT_FUN_EN)
				VOLT_Lv2OverVoltHandle(lv_ucChan);
#endif
#if (VOLT_LV1_LOWVOLT_FUN_EN == STD_ON)
				VOLT_Lv1LowVoltHandle(lv_ucChan);
#endif

#if (VOLT_LV2_LOWVOLT_FUN_EN == STD_ON)
				VOLT_Lv2LowVoltHandle(lv_ucChan);
#endif
			}
			else
			{
				LIB_SetMemory((uint8_t *)&gv_stVolt[ch].stChanVartArray[lv_ucChan], (0u), (uint16_t)(sizeof(gv_stVolt[ch].stChanVartArray[lv_ucChan]) / sizeof(uint8_t)));
			}
		}
		VOLT_NormalModeErrHandle(ch);
	}
}

/*******************************************************************************
Name            : VOLT_InputModeInitHandle
Syntax          : static void VOLT_InputModeInitHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Input mode init handle
Call By         : VOLT_ModeSelfTestHandle
|******************************************************************************/
static void VOLT_InputModeInitHandle(SysConnector_Num_Enum ch)
{
#if VOLT_ONE_PHASE_INPUT_MODE == VOLT_INPUT_MODE_TYPE
	gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].ucVoltEnCaliVal = STD_TRUE;
	gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].ucVoltEnCaliVal = STD_FALSE;
	gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].ucVoltEnCaliVal = STD_FALSE;
#elif VOLT_THREE_PHASE_INPUT_MODE == VOLT_INPUT_MODE_TYPE
	gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].ucVoltEnCaliVal = STD_TRUE;
	gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].ucVoltEnCaliVal = STD_TRUE;
	gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].ucVoltEnCaliVal = STD_TRUE;
#endif
}

#if VOLT_FAULT_PHASE_FUN_EN == STD_ON
/*******************************************************************************
Name            : VOLT_FaultPhaseHandle
Syntax          : static void VOLT_FaultPhaseHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : gv_stVolt[ch].ucFltPhsJobResult
Description     : Fault phase test handle
Call By         : VOLT_ModeSelfTestHandle
|******************************************************************************/
static void VOLT_FaultPhaseHandle(SysConnector_Num_Enum ch)
{

}
#endif

/*******************************************************************************
Name            : VOLT_NormalModeErrHandle
Syntax          : static void VOLT_NormalModeErrHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Normal mode status error handle
Call By         : VOLT_ModeNormalHandle
|******************************************************************************/
static void VOLT_NormalModeErrHandle(SysConnector_Num_Enum ch)
{
#if VOLT_INPUT_MODE_TYPE == VOLT_THREE_PHASE_INPUT_MODE
	static uint8_t lv_ucLv2OverVoltFaultStatus = STD_FALSE, lv_ucLv1OverVoltFaultStatus = STD_FALSE, lv_ucLv1LowVoltFaultStatus = STD_FALSE, lv_ucLv2LowVoltFaultStatus = STD_FALSE;
#if (VOLT_LV2_OVERVOLT_FUN_EN == STD_ON)
	if ((STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv2OverVoltFilt.ucValidStatus) || (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv2OverVoltFilt.ucValidStatus))
	{
		if (STD_FALSE == lv_ucLv2OverVoltFaultStatus)
		{
			lv_ucLv2OverVoltFaultStatus = STD_TRUE;
			VOLT_SetLv2OverVoltL2L3P(lv_ucLv2OverVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv2OverVoltStatus: %d \r\n", lv_ucLv2OverVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else if ((STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv2OverVoltFilt.ucValidStatus) && (STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv2OverVoltFilt.ucValidStatus))
	{
		if (STD_TRUE == lv_ucLv2OverVoltFaultStatus)
		{
			lv_ucLv2OverVoltFaultStatus = STD_FALSE;
			VOLT_SetLv2OverVoltL2L3P(lv_ucLv2OverVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv2OverVoltStatus: %d \r\n", lv_ucLv2OverVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else
	{
	}
#endif
#if (VOLT_LV1_OVERVOLT_FUN_EN == STD_ON)
	if ((STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv1OverVoltFilt.ucValidStatus) || (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv1OverVoltFilt.ucValidStatus))
	{
		if (STD_FALSE == lv_ucLv1OverVoltFaultStatus)
		{
			lv_ucLv1OverVoltFaultStatus = STD_TRUE;
			VOLT_SetLv1OverVoltL2L3P(lv_ucLv1OverVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv1OverVoltStatus: %d \r\n", lv_ucLv1OverVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else if ((STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv1OverVoltFilt.ucValidStatus) && (STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv1OverVoltFilt.ucValidStatus))
	{
		if (STD_TRUE == lv_ucLv1OverVoltFaultStatus)
		{
			lv_ucLv1OverVoltFaultStatus = STD_FALSE;
			VOLT_SetLv1OverVoltL2L3P(lv_ucLv1OverVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv1OverVoltStatus: %d \r\n", lv_ucLv1OverVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else
	{
	}
#endif
#if (VOLT_LV1_LOWVOLT_FUN_EN == STD_ON)

	if ((STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv1LowVoltFilt.ucValidStatus) || (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv1LowVoltFilt.ucValidStatus))
	{
		if (STD_FALSE == lv_ucLv1LowVoltFaultStatus)
		{
			lv_ucLv1LowVoltFaultStatus = STD_TRUE;
			VOLT_SetLv1LowVoltL2L3P(lv_ucLv1LowVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv1LowVoltStatus: %d \r\n", lv_ucLv1LowVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else if ((STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv1LowVoltFilt.ucValidStatus) && (STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv1LowVoltFilt.ucValidStatus))
	{
		if (STD_TRUE == lv_ucLv1LowVoltFaultStatus)
		{
			lv_ucLv1LowVoltFaultStatus = STD_FALSE;
			VOLT_SetLv1LowVoltL2L3P(lv_ucLv1LowVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv1LowVoltStatus: %d \r\n", lv_ucLv1LowVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else
	{
	}
#endif
#if (VOLT_LV2_LOWVOLT_FUN_EN == STD_ON)
	if ((STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv2LowVoltFilt.ucValidStatus) || (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv2LowVoltFilt.ucValidStatus))
	{
		if (STD_FALSE == lv_ucLv2LowVoltFaultStatus)
		{
			lv_ucLv2LowVoltFaultStatus = STD_TRUE;
			VOLT_SetLv2LowVoltL2L3P(lv_ucLv2LowVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv2LowVoltStatus: %d \r\n", lv_ucLv2LowVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else if ((STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].stLv2LowVoltFilt.ucValidStatus) && (STD_FALSE == gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].stLv2LowVoltFilt.ucValidStatus))
	{
		if (STD_TRUE == lv_ucLv2LowVoltFaultStatus)
		{
			lv_ucLv2LowVoltFaultStatus = STD_FALSE;
			VOLT_SetLv2LowVoltL2L3P(lv_ucLv2LowVoltFaultStatus);
			VOLT_DEBUG("L2L3Lv2LowVoltStatus: %d \r\n", lv_ucLv2LowVoltFaultStatus);
			VOLT_DEBUG("L2VoltVal :%d     L3VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal, gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
	else
	{
	}
#endif

#endif

#if (VOLT_LV1_OVERVOLT_FUN_EN == STD_ON)
static uint8_t lv_ucL1Lv1OverVoltStatus = STD_FALSE, lv_ucL1Lv2OverVoltStatus = STD_FALSE, lv_ucL1Lv1LowVoltStatus = STD_FALSE, lv_ucL1Lv2LowVoltStatus = STD_FALSE;

	if (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1OverVoltFilt.ucValidStatus)
	{
		if (lv_ucL1Lv1OverVoltStatus != gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1OverVoltFilt.ucStatus)
		{
			lv_ucL1Lv1OverVoltStatus = gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1OverVoltFilt.ucStatus;
			VOLT_SetLv1OverVoltL1P(lv_ucL1Lv1OverVoltStatus);
			VOLT_DEBUG("L1Lv1OverVoltStatus: %d \r\n", lv_ucL1Lv1OverVoltStatus);
			VOLT_DEBUG("VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
#endif

#if (VOLT_LV2_OVERVOLT_FUN_EN == STD_ON)
	if (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2OverVoltFilt.ucValidStatus)
	{
		if (lv_ucL1Lv2OverVoltStatus != gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2OverVoltFilt.ucStatus)
		{
			lv_ucL1Lv2OverVoltStatus = gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2OverVoltFilt.ucStatus;
			VOLT_SetLv2OverVoltL1P(lv_ucL1Lv2OverVoltStatus);
			VOLT_DEBUG("L1Lv2OverVoltStatus: %d \r\n", lv_ucL1Lv2OverVoltStatus);
			VOLT_DEBUG("VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
#endif

#if (VOLT_LV1_LOWVOLT_FUN_EN == STD_ON)
	if (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1LowVoltFilt.ucValidStatus)
	{
		if (lv_ucL1Lv1LowVoltStatus != gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1LowVoltFilt.ucStatus)
		{
			lv_ucL1Lv1LowVoltStatus = gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv1LowVoltFilt.ucStatus;
			VOLT_SetLv1LowVoltL1P(lv_ucL1Lv1LowVoltStatus);
			VOLT_DEBUG("L1Lv1LowVoltStatus: %d \r\n", lv_ucL1Lv1LowVoltStatus);
			VOLT_DEBUG("VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
#endif

#if (VOLT_LV2_LOWVOLT_FUN_EN == STD_ON)
	if (STD_TRUE == gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2LowVoltFilt.ucValidStatus)
	{
		if (lv_ucL1Lv2LowVoltStatus != gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2LowVoltFilt.ucStatus)
		{
			lv_ucL1Lv2LowVoltStatus = gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].stLv2LowVoltFilt.ucStatus;
			VOLT_SetLv2LowVoltL1P(lv_ucL1Lv2LowVoltStatus);
			VOLT_DEBUG("L1Lv2LowVoltStatus: %d \r\n", lv_ucL1Lv2LowVoltStatus);
			VOLT_DEBUG("VoltVal :%d \r\n", gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal);
		}
		else
		{
		}
	}
#endif
}

/*******************************************************************************
Name            : VOLT_Lv1OverVoltHandle
Syntax          : static void VOLT_Lv1OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Level 1 overvoltage filter handle
Call By         : VOLT_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == VOLT_LV1_OVERVOLT_FUN_EN)
static void VOLT_Lv1OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
{
	if (gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal > VOLT_LV1_IN_OVERVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucStatus = STD_TRUE;
	}
	else if (gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal < VOLT_LV1_OUT_OVERVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if (STD_FALSE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucStatus = STD_FALSE;
		}
		else if (STD_TRUE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt.ucStatus = STD_TRUE;
		}
		else
		{
		}
	}
	(SysConnector_Num_Enum ch)LIB_StatusFilterType2(&(gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1OverVoltFilt), VOLT_LV1_IN_OVERVOLT_WAIT_NUM, VOLT_LV1_OUT_OVERVOLT_WAIT_NUM);
}
#endif

/*******************************************************************************
Name            : VOLT_Lv2OverVoltHandle
Syntax          : static void VOLT_Lv2OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Level 2 overvoltage filter handle
Call By         : VOLT_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == VOLT_LV2_OVERVOLT_FUN_EN)
static void VOLT_Lv2OverVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
{
	if(gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal >= VOLT_LV2_IN_OVERVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucStatus = STD_TRUE;
	}
	else if(gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal < VOLT_LV2_OUT_OVERVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if(STD_FALSE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucStatus = STD_FALSE;
		}
		else if(STD_TRUE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt.ucStatus = STD_TRUE;
		}
		else
		{}
	}

	(SysConnector_Num_Enum ch)LIB_StatusFilterType2(&(gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2OverVoltFilt), VOLT_LV2_IN_OVERVOLT_WAIT_NUM,VOLT_LV2_OUT_OVERVOLT_WAIT_NUM);
}
#endif
/*******************************************************************************
Name            : VOLT_Lv1LowVoltHandle
Syntax          : static void VOLT_Lv1LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Level 1 lowvoltage filter handle
Call By         : VOLT_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == VOLT_LV1_LOWVOLT_FUN_EN)
static void VOLT_Lv1LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
{
	if(gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal <= VOLT_LV1_IN_LOWVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucStatus = STD_TRUE;
	}
	else if(gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal >= VOLT_LV1_OUT_LOWVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if(STD_FALSE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucStatus = STD_FALSE;
		}
		else if(STD_TRUE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt.ucStatus = STD_TRUE;
		}
		else
		{}
	}

	(SysConnector_Num_Enum ch)STD_TRUE == LIB_StatusFilterType2(&(gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv1LowVoltFilt),VOLT_LV1_IN_LOWVOLT_WAIT_NUM,VOLT_LV1_OUT_LOWVOLT_WAIT_NUM);
}
#endif

/*******************************************************************************
Name            : VOLT_Lv2LowVoltHandle
Syntax          : static void VOLT_Lv2LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Level 2 lowvoltage filter handle
Call By         : VOLT_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == VOLT_LV2_LOWVOLT_FUN_EN)
static void VOLT_Lv2LowVoltHandle(SysConnector_Num_Enum ch, uint8_t lv_ucChan)
{
	if (gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal <= VOLT_LV2_IN_LOWVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucStatus = STD_TRUE;
	}
	else if (gv_stVolt[ch].stChanVartArray[lv_ucChan].usVoltTempVal >= VOLT_LV2_OUT_LOWVOLT_VALUE)
	{
		gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if (STD_FALSE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucStatus = STD_FALSE;
		}
		else if (STD_TRUE == gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucValidStatus)
		{
			gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt.ucStatus = STD_TRUE;
		}
		else
		{
		}
	}
	(SysConnector_Num_Enum ch)LIB_StatusFilterType2(&(gv_stVolt[ch].stChanVartArray[lv_ucChan].stLv2LowVoltFilt), VOLT_LV2_IN_LOWVOLT_WAIT_NUM, VOLT_LV2_OUT_LOWVOLT_WAIT_NUM);
}
#endif

/*******************************************************************************
Name            : VOLT_20msFunctionControl
Syntax          : void VOLT_20msFunctionControl(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : VOLT module 20ms task function
Call By         : Task
|******************************************************************************/
void VOLT_20msFunctionControl(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		switch (gv_stVolt[ch].enMode)
		{
		case VOLT_MODE_IDLE:
		{
			VOLT_ModeIdleHandle(ch);
		}
		break;

		case VOLT_MODE_SELFTEST:
		{
			VOLT_ModeSelfTestHandle(ch);
		}
		break;

		case VOLT_MODE_NORMAL:
		{
			VOLT_ModeNormalHandle(ch);
		}
		break;

		default:
		{
			gv_stVolt[ch].enMode = VOLT_MODE_IDLE;
		}
		break;
		}
	}
}
