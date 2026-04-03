//****************************************************************************************
//*
//* File Name: STD_EvseM.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//*
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_EvseM.h"
#include "CanM_Rte.h"
#include "STD_Lib.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define EVSEM_SELF_CHECK_NONE (0U)		 /*slefcheck state is none*/
#define EVSEM_SELF_CHECK_SUCCESSFUL (1U) /*slefcheck state is successful*/
#define EVSEM_SELF_CHECK_FAILURE (2U)	 /*slefcheck state is failure*/

#define EVSEM_SELFCHECK_STEP0 (0u) /*slefcheck init*/
#define EVSEM_SELFCHECK_STEP1 (1u) /*slefcheck processing*/
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	EVSEM_REQ_CHARGE_OFF = 0U, /* request charge off */
	EVSEM_REQ_CHARGE_ON		   /* request charge on */
} EvseM_ReqChargeType_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	EvseM_ReqChargeType_Enum enChargeStatus; /* Charge Status */
	uint8_t ucState;						 /* Evse state */
	uint8_t ucSelfCheckStep;				 /* SelfCheck Step */
	uint8_t ucStatus2SelfCheckStep;			 /* Relay selfCheck Step */
	uint8_t ucDiodefCheckStep;				 /* diode selfcheck step */
	uint8_t ucDiodefCheckResult;			 /* diode selfcheck result */
	uint8_t ucPowerOnSelfCheckStatus;		 /* PowerOn SelfCheck Status */
	uint8_t ucStartSelfCheckStatus;			 /* Start SelfCheck Status */
	uint8_t ucCpStatus;						 /* Cp Status */
	uint8_t ucStopChargeReason;				 /* stop charging reason */
	uint16_t usWaitCnt;						 /* wait count */
	uint16_t usStateTwoDlyTick;				 /* StateTwo delay count */
} EvseM_Struct;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static EvseM_Struct gv_stEvseM[SYS_CONNECTOR_NUM_MAX];
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void EVSEM_ChargingModeJudgy(SysConnector_Num_Enum ch);
static void EVSEM_StateCanModelHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateZeroHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateOneHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateOneDotHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateTwoHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateTwoDotHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateThreeHandle(SysConnector_Num_Enum ch);
static void EVSEM_StateThreeDotHandle(SysConnector_Num_Enum ch);
static void EVSEM_EnterStateFourHandle(SysConnector_Num_Enum ch);

#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
static uint8_t EVSEM_RcdSelfCheck(SysConnector_Num_Enum ch);
#endif

#if (EVSEM_RELAY_CHECKSELF_ENABLE == STD_ON)
static uint8_t EVSEM_RelaySelfCheck(SysConnector_Num_Enum ch);
#endif

#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
static uint8_t EVSEM_DiodeSelfCheck(SysConnector_Num_Enum ch);
#endif
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : EVSEM_InitMemory
Syntax          : void EVSEM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Memory initialization
Call By         : SYSM_InitOne
|******************************************************************************/
void EVSEM_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)(&gv_stEvseM), 0u, (uint16_t)(sizeof(gv_stEvseM) / sizeof(uint8_t))); /*PRQA S 0310*/
	CanM_Rte_Evse_Init(); /* Initialize the RTE for CanM */
}

/*******************************************************************************
Name            : EVSEM_SetChargeStopReason
Syntax          : void EVSEM_SetChargeStopReason(uint8_t lv_ucReson)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Set Charge stop Reason
Call By         : Auth
|******************************************************************************/
void EVSEM_SetChargeStopReason(SysConnector_Num_Enum ch, uint8_t lv_ucReson)
{
	gv_stEvseM[ch].ucStopChargeReason = lv_ucReson;
}

/*******************************************************************************
Name            : EVSEM_GetChargeStopReason
Syntax          : uint8_t EVSEM_GetChargeStopReason(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : uint8_t
Description     : Get Charge stop Reason
Call By         :
|******************************************************************************/
uint8_t EVSEM_GetChargeStopReason(SysConnector_Num_Enum ch)
{
	return gv_stEvseM[ch].ucStopChargeReason;
}

/*******************************************************************************
Name            : EVSEM_GetChargeStatus
Syntax          : uint8_t EVSEM_GetChargeStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : uint8_t
Description     : Get Charge Status
Call By         : -
|******************************************************************************/
uint8_t EVSEM_GetChargeStatus(SysConnector_Num_Enum ch)
{
	return gv_stEvseM[ch].ucState;
}

#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
/*******************************************************************************
Name            : EVSEM_GetDiodeCheckResult
Syntax          : uint8_t EVSEM_GetDiodeCheckResult(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : uint8_t
Description     : Get Diode Check Result
Call By         : -
|******************************************************************************/
uint8_t EVSEM_GetDiodeCheckResult(SysConnector_Num_Enum ch)
{
	return gv_stEvseM[ch].ucDiodefCheckResult;
}
#endif

/*******************************************************************************
Name            : EVSEM_ResetStartSelfCheckStatus
Syntax          : void EVSEM_ResetStartSelfCheckStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Reset Start SelfCheck Status
Call By         : -
|******************************************************************************/
void EVSEM_ResetStartSelfCheckStatus(SysConnector_Num_Enum ch)
{
	gv_stEvseM[ch].ucStartSelfCheckStatus = (uint8_t)EVSEM_SELF_CHECK_NONE;
}

/*******************************************************************************
Name            : EVSEM_ReqChargeOn
Syntax          : void EVSEM_ReqChargeOn(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Request Charge On
Call By         : -
|******************************************************************************/
void EVSEM_ReqChargeOn(SysConnector_Num_Enum ch)
{
	gv_stEvseM[ch].enChargeStatus = EVSEM_REQ_CHARGE_ON;
}
/*******************************************************************************
Name            : EVSEM_ReqChargeOff
Syntax          : void EVSEM_ReqChargeOff(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Request Charge Off
Call By         : -
|******************************************************************************/
void EVSEM_ReqChargeOff(SysConnector_Num_Enum ch)
{
	gv_stEvseM[ch].enChargeStatus = EVSEM_REQ_CHARGE_OFF;
}
#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
/*******************************************************************************
Name            : EVSEM_RcdSelfCheck
Syntax          : void EVSEM_RcdSelfCheck(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     :
Call By         : -
|******************************************************************************/
static uint8_t EVSEM_RcdSelfCheck(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucResult = EVSEM_SELF_CHECK_NONE;
	if ((uint8_t)EVSEM_SELFCHECK_STEP0 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		EVSEM_DEBUG("EVSE request RCD selfcheck\r\n");
		EVSEM_ReqRCDSelfCheck(ch);
		gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP1;
		gv_stEvseM[ch].usWaitCnt = 0u;
	}
	else if ((uint8_t)EVSEM_SELFCHECK_STEP1 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		if ((uint8_t)EVSEM_RCD_MONITOR_PROCESS == EVSEM_GetRCDSelfCheckStatus(ch)) /*self-checking*/
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt >= EVSEM_SELFCHECK_TIMEOUT_CNT)
			{
				lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].usWaitCnt = 0u;
			}
		}
		else if ((uint8_t)EVSEM_RCD_MONITOR_SUCCESS == EVSEM_GetRCDSelfCheckStatus(ch)) /*successful*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
		}
		else if ((uint8_t)EVSEM_RCD_MONITOR_FAILED == EVSEM_GetRCDSelfCheckStatus(ch)) /*failure*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
		}
		else
		{
		}
	}
	else
	{
	}
	return lv_ucResult;
}
#endif

#if (EVSEM_RELAY_CHECKSELF_ENABLE == STD_ON)
/*******************************************************************************
Name            : EVSEM_RelaySelfCheck
Syntax          : void EVSEM_RelaySelfCheck(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     :
Call By         : -
|******************************************************************************/
static uint8_t EVSEM_RelaySelfCheck(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucResult = EVSEM_SELF_CHECK_NONE;
	if ((uint8_t)EVSEM_SELFCHECK_STEP0 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		EVSEM_ReqRelaySelfCheck(ch);
		gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP1;
		gv_stEvseM[ch].usWaitCnt = 0u;
	}
	else if ((uint8_t)EVSEM_SELFCHECK_STEP1 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		if ((uint8_t)EVSEM_RLY_SELFCHECK_PROCESS == EVSEM_GetRelaySelfCheckStatus(ch)) /*self-checking*/
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt >= EVSEM_SELFCHECK_TIMEOUT_CNT)
			{
				lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].usWaitCnt = 0u;
			}
		}
		else if ((uint8_t)EVSEM_RLY_SELFCHECK_SUCCESS == EVSEM_GetRelaySelfCheckStatus(ch)) /*successful*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
		}
		else if ((uint8_t)EVSEM_RLY_SELFCHECK_FAILED == EVSEM_GetRelaySelfCheckStatus(ch)) /*failure*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
		}
		else
		{
		}
	}
	else
	{
	}
	return lv_ucResult;
}
#endif

#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
/*******************************************************************************
Name            : EVSEM_DiodeSelfCheck
Syntax          : void EVSEM_DiodeSelfCheck(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     :
Call By         : -
|******************************************************************************/
static uint8_t EVSEM_DiodeSelfCheck(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucResult = EVSEM_SELF_CHECK_NONE;
	if ((uint8_t)EVSEM_SELFCHECK_STEP0 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		EVSEM_ReqDiodeSelfCheck(ch);
		EVSEM_DEBUG("EVSE request diode check\r\n");
		gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP1;
		gv_stEvseM[ch].usWaitCnt = 0u;
	}
	else if ((uint8_t)EVSEM_SELFCHECK_STEP1 == gv_stEvseM[ch].ucSelfCheckStep)
	{
		if ((uint8_t)EVSEM_DIODE_SELFCHECK_PROCESS == EVSEM_GetDiodeSelfCheckStatus(ch)) /*self-checking*/
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt >= EVSEM_SELFCHECK_TIMEOUT_CNT)
			{
				lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
				//				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].usWaitCnt = 0u;
			}
		}
		else if ((uint8_t)EVSEM_DIODE_SELFCHECK_SUCCESS == EVSEM_GetDiodeSelfCheckStatus(ch)) /*successful*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
			EVSEM_DEBUG("EVSE diode check success\r\n");
		}
		else if ((uint8_t)EVSEM_DIODE_SELFCHECK_FAILED == EVSEM_GetDiodeSelfCheckStatus(ch)) /*failure*/
		{
			lv_ucResult = (uint8_t)EVSEM_SELF_CHECK_FAILURE;
			//			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].usWaitCnt = 0u;
		}
		else
		{
		}
	}
	else
	{
	}
	return lv_ucResult;
}
#endif

/*******************************************************************************
Name            : EVSEM_ChargingModeJudgy
Syntax          : static void EVSEM_ChargingModeJudgy(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : judgy evse working in GB or JDL
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_ChargingModeJudgy(SysConnector_Num_Enum ch)
{
	static uint8_t lv_ucLastCpStatus[SYS_CONNECTOR_NUM_MAX] = { (uint8_t)EVSEM_CP_NULL };

	if (lv_ucLastCpStatus[ch] != gv_stEvseM[ch].ucCpStatus)
	{
		lv_ucLastCpStatus[ch] = gv_stEvseM[ch].ucCpStatus;
		gv_stEvseM[ch].usWaitCnt = 0u; /* reset wait count */
	}

	if ((uint8_t)EVSEM_CP_4V == gv_stEvseM[ch].ucCpStatus)
	{
		gv_stEvseM[ch].usWaitCnt = 0u;
	}
	else
	{
		if ((uint8_t)EVSEM_CP_3V == gv_stEvseM[ch].ucCpStatus)
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt > EVSEM_S2OFF_TIMEOUT_CNT)
			{
				EVSEM_DEBUG("ch:%d Cp 3V into 2V timeout!!\r\n", ch);
				gv_stEvseM[ch].usWaitCnt = 0u;
				EVSEM_SET_CP_OUT_12V(ch);
			}
		}
		else if ((uint8_t)EVSEM_CP_2V == gv_stEvseM[ch].ucCpStatus)
		{
			EVSEM_DEBUG("ch:%d EVSE into CAN model!\r\n", ch);
			gv_stEvseM[ch].usWaitCnt = 0u;
			gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_CAN_MODEL;
			EVSEM_SetCanModeStatus(ch, STD_TRUE);
			EVSEM_SET_CP_MOS_STATUS(ch, SWITCHM_CAN_MODE); /* Set to CAN mode */
			EVSEM_SET_CAN_START_COM(ch); /* Set CAN communication start */
		}
		else
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt > EVSEM_CP_FILTER_MAX_CNT)
			{
				EVSEM_DEBUG("ch:%d EVSE into 1!\r\n", ch);
				EVSEM_SET_CP_OUT_12V(ch);
				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].usWaitCnt = 0u;
				gv_stEvseM[ch].usStateTwoDlyTick = 0;
				gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ONE;
			}
		}
	}
}

/*******************************************************************************
Name            : EVSEM_StateZeroHandle
Syntax          : static void EVSEM_StateZeroHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State Zero Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateZeroHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucChargeConditions = EVSEM_GetChargeConditions(ch);

	if ((uint8_t)STD_TRUE == EVSEM_GetSysReadyState())
	{
		if (lv_ucChargeConditions < (uint8_t)EVSEM_CHARGE_SUSPENDED)
		{
			EVSEM_CpStopOutputNegative12V(ch);
			EVSEM_ChargingModeJudgy(ch);
			// EVSEM_DEBUG("ch:%d Init! \n", ch);
		}
	}
}

/*******************************************************************************
Name            : EVSEM_StateOneHandle
Syntax          : static void EVSEM_StateOneHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State One Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateOneHandle(SysConnector_Num_Enum ch)
{
#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
	if ((uint8_t)EVSEM_SELF_CHECK_NONE == gv_stEvseM[ch].ucPowerOnSelfCheckStatus)
	{
		gv_stEvseM[ch].ucPowerOnSelfCheckStatus = EVSEM_RcdSelfCheck(ch);
	}
	else if ((uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL == gv_stEvseM[ch].ucPowerOnSelfCheckStatus)
	{
#endif
		if (((uint8_t)EVSEM_CP_9V == gv_stEvseM[ch].ucCpStatus) || ((uint8_t)EVSEM_CP_6V == gv_stEvseM[ch].ucCpStatus))
		{
			gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO;
			gv_stEvseM[ch].ucStatus2SelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].ucDiodefCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			gv_stEvseM[ch].ucDiodefCheckResult = (uint8_t)EVSEM_SELF_CHECK_NONE;
			gv_stEvseM[ch].usWaitCnt = 0u;
			EVSEM_DEBUG("ch:%d EVSE 1 into 2!\r\n",ch);
		}
#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
	}
	else
	{
	}
#endif
}

/*******************************************************************************
Name            : EVSEM_StateOneDotHandle
Syntax          : static void EVSEM_StateOneDotHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State OneDot Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateOneDotHandle(SysConnector_Num_Enum ch)
{
	if ((uint8_t)EVSEM_CP_9V == gv_stEvseM[ch].ucCpStatus || (uint8_t)EVSEM_CP_6V == gv_stEvseM[ch].ucCpStatus)
	{
		gv_stEvseM[ch].ucState = EVSEM_STATE_TWO_dot;
		gv_stEvseM[ch].usWaitCnt = 0u;
	}
	else
	{
		EVSEM_DEBUG("ch:%d 1' Into init! \n",ch);
		EVSEM_EnterStateZero(ch);
	}
}

/*******************************************************************************
Name            : EVSEM_StateTwoHandle
Syntax          : static void EVSEM_StateTwoHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State Two Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateTwoHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucSelfCheck = EVSEM_SELF_CHECK_NONE;

	if (gv_stEvseM[ch].usStateTwoDlyTick < EVSEM_STATUS2_DLY_CNT)
	{
		gv_stEvseM[ch].usStateTwoDlyTick++;
	}
#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
	if (gv_stEvseM[ch].usStateTwoDlyTick >= EVSEM_STATUS2_DLY_CNT)
	{
		if (gv_stEvseM[ch].ucDiodefCheckStep == (uint8_t)EVSEM_SELFCHECK_STEP0)
		{
			gv_stEvseM[ch].ucDiodefCheckResult = EVSEM_SELF_CHECK_NONE;
			gv_stEvseM[ch].ucDiodefCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP1;
			gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
			EVSEM_DEBUG("EVSE diode check init\r\n");
		}
		else if (((uint8_t)EVSEM_SELFCHECK_STEP1 == gv_stEvseM[ch].ucDiodefCheckStep) && (gv_stEvseM[ch].ucDiodefCheckResult != EVSEM_SELF_CHECK_SUCCESSFUL))
		{
			gv_stEvseM[ch].ucDiodefCheckResult = EVSEM_DiodeSelfCheck(ch);
		}
		else
		{
		}
	}
#endif
	if ((uint8_t)EVSEM_CP_12V == gv_stEvseM[ch].ucCpStatus)
	{
		EVSEM_EnterStateZero(ch);
	}
#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
	else if (((uint8_t)EVSEM_REQ_CHARGE_ON == (uint8_t)gv_stEvseM[ch].enChargeStatus) && (gv_stEvseM[ch].usStateTwoDlyTick >= EVSEM_STATUS2_DLY_CNT) && (gv_stEvseM[ch].ucDiodefCheckResult == (uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL))
#else
	else if (((uint8_t)EVSEM_REQ_CHARGE_ON == (uint8_t)gv_stEvseM[ch].enChargeStatus) && (gv_stEvseM[ch].usStateTwoDlyTick >= EVSEM_STATUS2_DLY_CNT))
#endif
	{

		if ((uint8_t)EVSEM_SELF_CHECK_NONE == gv_stEvseM[ch].ucStartSelfCheckStatus)
		{
			if ((uint8_t)EVSEM_SELFCHECK_STEP0 == gv_stEvseM[ch].ucStatus2SelfCheckStep)
			{
#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
				lv_ucSelfCheck = EVSEM_RcdSelfCheck(ch);
#endif
				lv_ucSelfCheck = EVSEM_SELF_CHECK_SUCCESSFUL;
#if (EVSEM_RELAY_CHECKSELF_ENABLE == STD_ON)
				if (EVSEM_SELF_CHECK_SUCCESSFUL == lv_ucSelfCheck)
				{
					gv_stEvseM[ch].ucStatus2SelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP1;
				}
				else if (EVSEM_SELF_CHECK_FAILURE == lv_ucSelfCheck)
				{
					gv_stEvseM[ch].ucStartSelfCheckStatus = EVSEM_SELF_CHECK_FAILURE;
					gv_stEvseM[ch].ucStatus2SelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
					gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
					EVSEM_DEBUG("2 RCD Selfcheck Failed!\n");
				}
				else
				{
				}
			}
			else if ((uint8_t)EVSEM_SELFCHECK_STEP1 == gv_stEvseM[ch].ucStatus2SelfCheckStep)
			{
				lv_ucSelfCheck = EVSEM_RelaySelfCheck(ch);
#endif
				if (EVSEM_SELF_CHECK_NONE != lv_ucSelfCheck)
				{
					gv_stEvseM[ch].ucStartSelfCheckStatus = lv_ucSelfCheck;
					gv_stEvseM[ch].ucStatus2SelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
					gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
					EVSEM_DEBUG("ch:%d 2 Selfcheck %d!\n", ch, gv_stEvseM[ch].ucStartSelfCheckStatus);
				}
			}
			else
			{
			}
		}
		else if ((uint8_t)EVSEM_SELF_CHECK_SUCCESSFUL == gv_stEvseM[ch].ucStartSelfCheckStatus)
		{
			if ((uint8_t)EVSEM_CP_6V == gv_stEvseM[ch].ucCpStatus)
			{
				EVSEM_DEBUG("ch:%d 2 Go to status 3!\n",ch);
				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].ucStatus2SelfCheckStep = EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_THREE;
				gv_stEvseM[ch].usWaitCnt = 0;
			}
			else if ((uint8_t)EVSEM_CP_9V == gv_stEvseM[ch].ucCpStatus)
			{
				EVSEM_DEBUG("ch:%d 2 Start CP Output!\n", ch);
				gv_stEvseM[ch].ucSelfCheckStep = (uint8_t)EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].ucStatus2SelfCheckStep = EVSEM_SELFCHECK_STEP0;
				gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO_dot;
				gv_stEvseM[ch].usWaitCnt = 0u;
				EVSEM_StartCpOutput(ch);
			}
			else
			{
			}
		}
		else
		{
		}
	}
	else
	{
	}
}

/*******************************************************************************
Name            : EVSEM_StateTwoDotHandle
Syntax          : static void EVSEM_StateTwoDotHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State TwoDot Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateTwoDotHandle(SysConnector_Num_Enum ch)
{
	if ((uint8_t)EVSEM_REQ_CHARGE_ON != (uint8_t)gv_stEvseM[ch].enChargeStatus)
	{
		EVSEM_DEBUG("ch:%d 2' Stop CP Output! 1\n", ch);
		gv_stEvseM[ch].usWaitCnt = 0u;
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO;
		EVSEM_StopCpOutput(ch);
		EVSEM_SetRelayOff(ch);
	}
	else if ((uint8_t)EVSEM_CP_12V == gv_stEvseM[ch].ucCpStatus)
	{
		EVSEM_DEBUG("ch:%d 2' Turn off Relay! 2\n", ch);
		gv_stEvseM[ch].usWaitCnt = 0u;
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ONE_dot;
		EVSEM_SetRelayOff(ch);
	}
	else
	{
		if ((uint8_t)EVSEM_CP_6V == gv_stEvseM[ch].ucCpStatus)
		{
			if (gv_stEvseM[ch].usWaitCnt >= (uint8_t)EVSEM_S2OFF_DLY_CNT)
			{
				EVSEM_DEBUG("%s : ch:%d 2' Relay on \n", __FUNCTION__, ch);
				EVSEM_SetRelayOn(ch);
				if ((uint8_t)EVSEM_RLYCTRL_STATE_ON == EVSEM_GetRelayStatus(ch))
				{
					EVSEM_DEBUG("ch:%d 2' Turn on Relay! 2\n", ch);
					gv_stEvseM[ch].usWaitCnt = 0;
					gv_stEvseM[ch].ucStopChargeReason = EVSEM_STOP_CHARGE_NONE;
					gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_THREE_dot;
				}
			}
			else
			{
				gv_stEvseM[ch].usWaitCnt++;
			}
		}
		else
		{
			if ((uint8_t)EVSEM_RLYCTRL_STATE_ON == EVSEM_GetRelayStatus(ch))
			{
				EVSEM_SetRelayOff(ch);
			}
		}
	}
}

/*******************************************************************************
Name            : EVSEM_StateThreeHandle
Syntax          : static void EVSEM_StateThreeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State Three Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateThreeHandle(SysConnector_Num_Enum ch)
{
	if ((uint8_t)EVSEM_CP_6V == gv_stEvseM[ch].ucCpStatus)
	{
		if ((uint8_t)EVSEM_REQ_CHARGE_ON == (uint8_t)gv_stEvseM[ch].enChargeStatus)
		{
			EVSEM_DEBUG("ch:%d 3 Turn on Relay and cp! \n", ch);
			EVSEM_SetRelayOn(ch);
			EVSEM_StartCpOutput(ch);
			gv_stEvseM[ch].ucStopChargeReason = EVSEM_STOP_CHARGE_NONE;
			gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_THREE_dot;
		}
		else
		{
			gv_stEvseM[ch].usWaitCnt++;
			if (gv_stEvseM[ch].usWaitCnt > EVSEM_STATUS3_DLY_CNT)
			{
				EVSEM_DEBUG("ch:%d 3 Turn off Relay! 1\n", ch);
				gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO;
				gv_stEvseM[ch].usWaitCnt = 0u;
				EVSEM_SetRelayOff(ch);
			}
		}
	}
	else if (((uint8_t)EVSEM_CP_9V == gv_stEvseM[ch].ucCpStatus) || ((uint8_t)EVSEM_CP_12V == gv_stEvseM[ch].ucCpStatus))
	{
		EVSEM_DEBUG("ch:%d 3 Turn off Relay! 2\n", ch);
		gv_stEvseM[ch].usWaitCnt = 0;
		EVSEM_SetRelayOff(ch);
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO;
	} 
	else
	{
	}
}

/*******************************************************************************
Name            : EVSEM_StateThreeDotHandle
Syntax          : static void EVSEM_StateThreeDotHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : State ThreeDot Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateThreeDotHandle(SysConnector_Num_Enum ch)
{
	if ((uint8_t)EVSEM_CP_12V == gv_stEvseM[ch].ucCpStatus)
	{
		EVSEM_DEBUG("ch:%d 3' Turn off Relay! 3\n", ch);
		EVSEM_SetRelayOff(ch);
		gv_stEvseM[ch].ucStopChargeReason = EVSEM_STOP_CHARGE_CP_OFF;
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ONE_dot;
	}
	else if ((uint8_t)EVSEM_REQ_CHARGE_ON != (uint8_t)gv_stEvseM[ch].enChargeStatus)
	{
		EVSEM_DEBUG("ch:%d 3' Stop CP Output! 1\n", ch);
		EVSEM_StopCpOutput(ch);
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_THREE;
		gv_stEvseM[ch].usWaitCnt = 0;
	}
	else if ((uint8_t)EVSEM_CP_9V == gv_stEvseM[ch].ucCpStatus) /*S2 off*/
	{
		EVSEM_DEBUG("ch:%d 3' Turn off Relay! 2\n", ch);
		EVSEM_SetRelayOff(ch);
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_TWO_dot;
		gv_stEvseM[ch].usWaitCnt = 0;
		gv_stEvseM[ch].ucStopChargeReason = EVSEM_STOP_CHARGE_S2_OFF;
	}
	else
	{
	}
}

/*******************************************************************************
Name            : EVSEM_ErrHandle
Syntax          : static void EVSEM_ErrHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : EVSE Error Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_EnterStateFourHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucChargeConditions = EVSEM_GetChargeConditions(ch);

	if (lv_ucChargeConditions >= (uint8_t)EVSEM_CHARGE_SUSPENDED)
	{
		EVSEM_SetRelayOff(ch);
		EVSEM_StopCpOutput(ch);
		EVSEM_CpOutputNegative12V(ch);
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ZERO;
		EVSEM_DEBUG("ch:%d State four enter init! \n", ch);
	}
}

/*******************************************************************************
Name            : EVSEM_EnterStateZero
Syntax          : void EVSEM_EnterStateZero(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none 
Return value    : void
Description     : EVSE Error Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
void EVSEM_EnterStateZero(SysConnector_Num_Enum ch)
{
	EVSEM_DEBUG("ch:%d EVSEM_EnterStateZero! \n", ch);
	EVSEM_SetRelayOff(ch);
	EVSEM_StopCpOutput(ch);
	EVSEM_SET_CP_OUT_4V(ch);
	gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ZERO;
}

/*******************************************************************************
Name            : EVSEM_StateCanModelHandle
Syntax          : static void EVSEM_StateCanModelHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : EVSE Error Handle
Call By         : EVSEM_10msMainFunction
|******************************************************************************/
static void EVSEM_StateCanModelHandle(SysConnector_Num_Enum ch)
{
	if (STD_TRUE == EVSEM_GET_CAN_END_COM(ch))
	{
		gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ZERO;
		EVSEM_SET_CP_MOS_STATUS(ch, SWITCHM_CC_CP_MODE); /* Set to CC CP mode */
		EVSEM_STOP_CAN_COM(ch);
		EVSEM_SetCanModeStatus(ch, STD_FALSE);
		EVSEM_DEBUG("ch:%d CAN into zero! \n", ch);
	}
	CanM_Rte_EVSE_Main_Task();
}

/*******************************************************************************
Name            : EVSEM_10msMainFunction
Syntax          : void EVSEM_10msMainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : void
Parameters(out) : none
Return value    : void
Description     : 10ms MainFunction
Call By         : TASK( OS_Task10msA )
|******************************************************************************/
void EVSEM_10msMainFunction(void)
{
	SysConnector_Num_Enum ch;

	for (ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stEvseM[ch].ucCpStatus = EVSEM_GetCpStatus(ch);

		switch (gv_stEvseM[ch].ucState)
		{
		case EVSEM_STATE_ZERO:
		{
			EVSEM_StateZeroHandle(ch);
			break;
		}

		case EVSEM_STATE_ONE:
		{
			EVSEM_StateOneHandle(ch);
			break;
		}

		case EVSEM_STATE_ONE_dot:
		{
			EVSEM_StateOneDotHandle(ch);
			break;
		}

		case EVSEM_STATE_TWO:
		{
			EVSEM_StateTwoHandle(ch);
			break;
		}

		case EVSEM_STATE_TWO_dot:
		{
			EVSEM_StateTwoDotHandle(ch);
			break;
		}
		case EVSEM_STATE_THREE:
		{
			EVSEM_StateThreeHandle(ch);
			break;
		}
		case EVSEM_STATE_THREE_dot:
		{
			EVSEM_StateThreeDotHandle(ch);
			break;
		}
		case EVSEM_STATE_CAN_MODEL:
		{
			EVSEM_StateCanModelHandle(ch);
			break;
		}
		default:
		{
			gv_stEvseM[ch].ucState = (uint8_t)EVSEM_STATE_ZERO;
			break;
		}
		}

		EVSEM_EnterStateFourHandle(ch);
	}
}
/*EOF*/
