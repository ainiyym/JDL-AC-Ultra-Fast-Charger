//****************************************************************************************
//*
//* File Name: STD_ErrorHandler.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/* polyspace:begin<MISRA-C3:11.4:Not a defect:Justified> Normal */
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_ErrorHandler.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define ERRHDL_MODE_IDLE							(0U)
#define ERRHDL_MODE_NORMAL							(1U)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	ErrHdlLevel_Enum enErrorL[ERRHDL_GUN_MAX_NUM];		/*Error level */
	ErrHdlLevel_Enum enFinalEeeorL[ERRHDL_GUN_MAX_NUM]; /*Last Error level*/
	uint8_t ucMode;										/*Error Module mode */
	uint8_t ucEnStatus;									/*Error Module Enable */
	uint8_t ucCurrIdx;									/*Current error id */
	uint8_t ucChargeConditions[ERRHDL_GUN_MAX_NUM];		/*Charging Conditions*/
	uint8_t arFltData[ERRHDL_BYTE_MAX_NUM];				/*Fault data*/
	uint8_t arFltCnt[ERRHDL_ID_MAX_NUM];				/*Number of failures*/
	uint16_t arRecoveryCnt[ERRHDL_ID_MAX_NUM];			/*Duration of fault occurrence*/
} ErrHdl_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static ErrHdl_Struct gv_stErrHdl;
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void ERRHDL_JudgeChargeConditions(void);
static void ERRHDL_ClearErrorLevelJudgy(void);
static void ERRHDL_SetErrorLevelJudgy(void);
static void ERRHDL_SetErrorLevel(uint8_t FaultAttribution, ErrHdlLevel_Enum enFltLevel);
static void ERRHDL_MultiErrorSelfRecoveryProcess(uint8_t lv_ucErrHdlId);
static void ERRHDL_SetErrorLevelProcess(uint8_t lv_ucErrHdlId);
static void ERRHDL_NormalProcess(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : ERRHDL_InitMemory
Syntax          : void ERRHDL_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : ErrorHandler Module memory variables initialized
Call By         :
|******************************************************************************/
void ERRHDL_InitMemory(void)
{
	LIB_SetMemory( (uint8_t *)(&gv_stErrHdl), 0u, (uint16_t)(sizeof(ErrHdl_Struct) / sizeof(uint8_t)));/*PRQA S 0310*/
}

/*******************************************************************************
Name            : ERRHDL_ClearAuthInfor
Syntax          : void ERRHDL_ClearAuthInfor(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Clearing Fault Information
Call By         : When receiving an authorization command
|******************************************************************************/
void ERRHDL_ClearAuthInfor(void)
{
	uint8_t lv_ucIdx = 0u;
	uint8_t lv_ucByteIdx = 0, lv_ucBitOffset = 0;

	for (lv_ucIdx = 0u; lv_ucIdx < (uint8_t)ERRHDL_ID_MAX_NUM; lv_ucIdx++)
	{
		gv_stErrHdl.arFltCnt[lv_ucIdx] = 0;
		gv_stErrHdl.arRecoveryCnt[lv_ucIdx] = 0;

		if ((uint8_t)ERRHDL_FLT_FLAG_CLEAR == (uint8_t)(c_arErrHdlCfgArray[lv_ucIdx].ucFltFlag & ERRHDL_FLT_FLAG_CLEAR))
		{
			LIB_RESET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset);
		}
	}
	LIB_RESET_UINT8_BIT(gv_stErrHdl.arFltData[ERRHDL_BYTE_IDX3], ERRHDL_BIT_1);	/*Clear the current fault flag*/
	LIB_RESET_UINT8_BIT(gv_stErrHdl.arFltData[ERRHDL_BYTE_IDX3], ERRHDL_BIT_3);	/*Clear the current fault flag*/
}

/*******************************************************************************
Name            : ERRHDL_GetErrorLevelStatus
Syntax          : ErrHdlLevel_Enum ERRHDL_GetErrorLevelStatus(uint32_t gun_index)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : gun_index: 0--gun1; 1--gun2
Parameters(out) : none
Return value    : ErrHdlLevel_Enum
Description     : Obtaining the Fault level
Call By         :
|******************************************************************************/
ErrHdlLevel_Enum ERRHDL_GetErrorLevelStatus(uint32_t gun_index)
{
	return (ErrHdlLevel_Enum)gv_stErrHdl.enFinalEeeorL[gun_index];
}

/*******************************************************************************
Name            : ERRHDL_GetChargeConditions
Syntax          : uint8_t ERRHDL_GetChargeConditions(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : gun_index: 0--gun1; 1--gun2
Parameters(out) : none
Return value    : uint8_t
Description     : Obtaining charging status information
Call By         :
|******************************************************************************/
uint8_t ERRHDL_GetChargeConditions(uint32_t gun_index)
{
	return gv_stErrHdl.ucChargeConditions[gun_index];
}

/*******************************************************************************
Name            : ERRHDL_JudgeChargeConditions
Syntax          : static void ERRHDL_JudgeChargeConditions(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : Judge the charging status task
Call By         : ERRHDL_NormalProcess
|******************************************************************************/
static void ERRHDL_JudgeChargeConditions(void)
{
	for (uint8_t gun_index = 0; gun_index < ERRHDL_GUN_MAX_NUM; gun_index++)
	{
		if ((uint8_t)gv_stErrHdl.enFinalEeeorL[gun_index] != (uint8_t)gv_stErrHdl.enErrorL[gun_index])
		{
			gv_stErrHdl.enFinalEeeorL[gun_index] = gv_stErrHdl.enErrorL[gun_index];
		}
		else
		{
			break;
		}

		switch (gv_stErrHdl.enFinalEeeorL[gun_index])
		{
			case ERRHDL_FLT_L_NONE:
			case ERRORH_FLT_L_ONE:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_ALLOW;
				break;
			}
			case ERRORH_FLT_L_TWO:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_DERATE;
				break;
			}
			case ERRORH_FLT_L_THREE:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_SUSPENDED;
				break;
			}
			case ERRORH_FLT_L_FOUR:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_CANCEL;
				break;
			}
			case ERRORH_FLT_L_FIVE:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_SAFETY;
				break;
			}
			default:
			{
				gv_stErrHdl.ucChargeConditions[gun_index] = (uint8_t)ERRHDL_CHARGE_ALLOW;
				break;
			}
		}
	}
}

/*******************************************************************************
 * Name            : ERRHDL_ClearErrorLevelJudgy
 * Syntax          : static void ERRHDL_ClearErrorLevelJudgy(void)
 * Sync/Async      : Synchronous
 * Reentrancy      :
 * Parameters(in)  : none
 * Parameters(out) : none
 * Return value    : void
 * Description     : Error level clearing judgment task
 * Call By         : ERRHDL_NormalProcess
 |******************************************************************************/
static void ERRHDL_ClearErrorLevelJudgy(void)
{
	uint8_t lv_ucByteIdx = 0, lv_ucBitOffset = 0;
	uint8_t FaultAttribution = 0;

	if (gv_stErrHdl.ucCurrIdx < (uint8_t)ERRHDL_ID_MAX_NUM)
	{
		lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltByteIdx;
		lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltBitOffset;
		if ((uint8_t)STD_FALSE == LIB_GET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset))
		{
			gv_stErrHdl.ucCurrIdx = 0u;

			FaultAttribution = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFaultAttribution;
			switch (FaultAttribution)
			{
				case ERRHDL_ATTIBUTION_NONE:
				{
					gv_stErrHdl.enErrorL[0] = ERRHDL_FLT_L_NONE;
					gv_stErrHdl.enErrorL[1] = ERRHDL_FLT_L_NONE;
					break;
				}
				case ERRHDL_ATTIBUTION_GUN1:
				{
					gv_stErrHdl.enErrorL[0] = ERRHDL_FLT_L_NONE;
					break;
				}
				case ERRHDL_ATTIBUTION_GUN2:
				{
					gv_stErrHdl.enErrorL[1] = ERRHDL_FLT_L_NONE;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
		}
	}
	else
	{
		gv_stErrHdl.ucCurrIdx = 0u;
		gv_stErrHdl.enErrorL[0] = ERRHDL_FLT_L_NONE;
		gv_stErrHdl.enErrorL[1] = ERRHDL_FLT_L_NONE;
	}
}

/*******************************************************************************
 * Name            : ERRHDL_SetErrorLevelJudgy
 * Syntax          : static void ERRHDL_SetErrorLevelJudgy(void)
 * Sync/Async      : Synchronous
 * Reentrancy      :
 * Parameters(in)  : none
 * Parameters(out) : none
 * Return value    : void
 * Description     : Error level setting judgment task
 * Call By         : ERRHDL_NormalProcess
 |******************************************************************************/
static void ERRHDL_SetErrorLevelJudgy(void)
{
	uint8_t lv_ucByteIdx = 0, lv_ucBitOffset = 0;
	uint8_t lv_ucErrHdlId = 0;

	for (lv_ucErrHdlId = 0u; lv_ucErrHdlId < (uint8_t)ERRHDL_ID_MAX_NUM; lv_ucErrHdlId++)
	{
		lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltByteIdx;
		lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltBitOffset;

		if ((uint8_t)STD_TRUE == LIB_GET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset))
		{
			ERRHDL_SetErrorLevelProcess(lv_ucErrHdlId);
			gv_stErrHdl.ucCurrIdx = lv_ucErrHdlId;
		}
		else
		{
			gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId] = 0u;
		}
	}
}

/*******************************************************************************
 * Name            : ERRHDL_SetErrorLevel
 * Syntax          : static void ERRHDL_SetErrorLevel(uint8_t FaultAttribution, ErrHdlLevel_Enum enFltLevel)
 * Sync/Async      : Synchronous
 * Reentrancy      :
 * Parameters(in)  : FaultAttribution: Fault attribution, enFltLevel: Fault level
 * Parameters(out) : none
 * Return value    : void
 * Description     : Setting the fault level task
 * Call By         : ERRHDL_SetErrorLevelProcess
 * 	***************************************************************************/
static void ERRHDL_SetErrorLevel(uint8_t FaultAttribution, ErrHdlLevel_Enum enFltLevel)
{
	switch (FaultAttribution)
	{
		case ERRHDL_ATTIBUTION_NONE:
		{
			for (uint8_t gun_index = 0; gun_index < ERRHDL_GUN_MAX_NUM; gun_index++)
			{
				if ((uint8_t)enFltLevel > (uint8_t)gv_stErrHdl.enErrorL[gun_index])
				{
					gv_stErrHdl.enErrorL[gun_index] = enFltLevel;
				}
			}
			break;
		}
		case ERRHDL_ATTIBUTION_GUN1:
		{
			if ((uint8_t)enFltLevel > (uint8_t)gv_stErrHdl.enErrorL[0])
			{
				gv_stErrHdl.enErrorL[0] = enFltLevel;
			}
			break;
		}
		case ERRHDL_ATTIBUTION_GUN2:
		{
			if ((uint8_t)enFltLevel > (uint8_t)gv_stErrHdl.enErrorL[1])
			{
				gv_stErrHdl.enErrorL[1] = enFltLevel;
			}
			break;
		}
		default:
			break;
	}
}

/*******************************************************************************
 * Name            : ERRHDL_MultiErrorSelfRecoveryProcess
 * Syntax          : static void ERRHDL_MultiErrorSelfRecoveryProcess(uint8_t lv_ucErrHdlId)
 * Sync/Async      : Synchronous
 * Reentrancy      :
 * Parameters(in)  : lv_ucErrHdlId: Error ID
 * Parameters(out) : none
 * Return value    : void
 * Description     : Multi-fault self-recovery processing task
 * Call By         : ERRHDL_SetErrorLevelProcess
 * ***************************************************************************/
static void ERRHDL_MultiErrorSelfRecoveryProcess(uint8_t lv_ucErrHdlId)
{
	uint8_t lv_ucByteIdx = 0, lv_ucBitOffset = 0;

	lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltByteIdx;
	lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltBitOffset;

	if ((uint8_t)ERRHDL_FLT_FLAG_CLEAR == (uint8_t)(c_arErrHdlCfgArray[lv_ucErrHdlId].ucFltFlag & ERRHDL_FLT_FLAG_CLEAR))
	{
		gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId]++;
		if (gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId] >= c_arErrHdlCfgArray[lv_ucErrHdlId].ulRecoveryTime)
		{
			gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId] = 0u;
			LIB_RESET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset);
		}
		else
		{
		}
	}
	else
	{
		gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId] = c_arErrHdlCfgArray[lv_ucErrHdlId].ulRecoveryTime;
	}
}

/*******************************************************************************
 * Name            : ERRHDL_SetErrorLevelProcess
 * Syntax          : static void ERRHDL_SetErrorLevelProcess(uint8_t lv_ucErrHdlId)
 * Sync/Async      : Synchronous
 * Reentrancy      :
 * Parameters(in)  : lv_ucErrHdlId: Error ID
 * Parameters(out) : none
 * Return value    : void
 * Description     : Setting the fault level processing task
 * Call By         : ERRHDL_NormalProcess
 * ***************************************************************************/
static void ERRHDL_SetErrorLevelProcess(uint8_t lv_ucErrHdlId)
{
	uint8_t FaultAttribution = (uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFaultAttribution;

	if ((uint8_t)ERRHDL_FLT_FLAG_MULTI != (uint8_t)(c_arErrHdlCfgArray[lv_ucErrHdlId].ucFltFlag & ERRHDL_FLT_FLAG_MULTI))
	{
		ERRHDL_SetErrorLevel(FaultAttribution, c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel);
	}
	else
	{
		if (gv_stErrHdl.arFltCnt[lv_ucErrHdlId] < c_arErrHdlCfgArray[lv_ucErrHdlId].ucMultiFltNum)
		{
			if (0u == gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId])
			{
				gv_stErrHdl.arFltCnt[lv_ucErrHdlId]++;
			}
			else
			{
			}

			if (gv_stErrHdl.arFltCnt[lv_ucErrHdlId] == c_arErrHdlCfgArray[lv_ucErrHdlId].ucMultiFltNum)
			{
				ERRHDL_SetErrorLevel(FaultAttribution, c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel);
			}
			else
			{
				ERRHDL_SetErrorLevel(FaultAttribution, c_arErrHdlCfgArray[lv_ucErrHdlId].enTempFltLevel);
			}
			ERRHDL_MultiErrorSelfRecoveryProcess(lv_ucErrHdlId);
		}
		else
		{
			ERRHDL_SetErrorLevel(FaultAttribution, c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel);
		}
	}
}

/*******************************************************************************
Name            : ERRHDL_10msMainFunction
Syntax          : void ERRHDL_10msMainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : void
Description     : ERRHDL task Main
Call By         : 10ms Cyclic Executing Function
|******************************************************************************/
static void ERRHDL_NormalProcess(void)
{
	if ((uint8_t)STD_FALSE == gv_stErrHdl.ucEnStatus)
	{
		gv_stErrHdl.ucMode = ERRHDL_MODE_IDLE;
		ERRHDL_InitMemory();
	}
	else
	{
		ERRHDL_ClearErrorLevelJudgy();
		ERRHDL_SetErrorLevelJudgy();
		ERRHDL_JudgeChargeConditions();
	}
}

/*******************************************************************************
Name            : ERRHDL_FaultStatusUpdata_CallBack
Syntax          : void ERRHDL_FaultStatusUpdata_CallBack(ErrHdlId_Enum lv_enFaultIdNum,uint8_t lv_ucFaultStatus)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : ErrHdlId_Enum
Parameters(in)  : uint8_t
Parameters(out) : none
Return value    : void
Description     : Fault status update callback function
Call By         :
|******************************************************************************/
void ERRHDL_FaultStatusUpdata_CallBack(ErrHdlId_Enum lv_enFaultIdNum,uint8_t lv_ucFaultStatus)
{
	uint8_t lv_ucByteIdx = 0,lv_ucBitOffset = 0;
	if((uint8_t)lv_enFaultIdNum < (uint8_t)ERRHDL_ID_MAX_NUM)
	{
		lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[(uint8_t)lv_enFaultIdNum].enFltByteIdx;
		lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[(uint8_t)lv_enFaultIdNum].enFltBitOffset;

		if((uint8_t)STD_TRUE == lv_ucFaultStatus)
		{
			LIB_SET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset);
		}
		else
		{
			LIB_RESET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset);
		}
	}
	else
	{}
}

/*******************************************************************************
Name            : ERRHDL_GetPwrFaultData
Syntax          : void ERRHDL_GetPwrFaultData(uint8_t *lv_arrBuff)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : uint8_t *
Parameters(out) : none
Return value    : void
Description     : Getting fault Data  ,len = ERRHDL_ALL_PROTO_MAX_NUM(6)
Call By         : To obtain fault data
|******************************************************************************/
void ERRHDL_GetAllFaultData(uint8_t *lv_arrBuff)
{
	if(lv_arrBuff != NULL)
	{
		LIB_Copy(lv_arrBuff, gv_stErrHdl.arFltData, (uint16_t)ERRHDL_ALL_PROTO_MAX_NUM);
	}
	else
	{}
}

/*******************************************************************************
Name            : ERRHDL_Enable
Syntax          : void ERRHDL_Enable()
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Enable ERRHDL Moudle
Call By         : -
|******************************************************************************/
void ERRHDL_Enable(void)
{
	gv_stErrHdl.ucEnStatus = STD_TRUE;
}

/*******************************************************************************
Name            : ERRHDL_Enable
Syntax          : void ERRHDL_Enable()
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Disable ERRHDL Moudle
Call By         : -
|******************************************************************************/
void ERRHDL_Disable(void)
{
	gv_stErrHdl.ucEnStatus = STD_FALSE;
}

/*******************************************************************************
Name            : ERRHDL_10msMainFunction
Syntax          : void ERRHDL_10msMainFunction()
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : 10ms mainFunction
Call By         : -
|******************************************************************************/
void ERRHDL_10msMainFunction(void)
{
	switch(gv_stErrHdl.ucMode)
	{
		case ERRHDL_MODE_IDLE:
		{
			if(((uint8_t)STD_TRUE == ERRHDL_GetSysReadyState())\
				&&((uint8_t)STD_TRUE == gv_stErrHdl.ucEnStatus))
			{
				gv_stErrHdl.ucMode = ERRHDL_MODE_NORMAL;
			}
			else
			{}
			break;
		}
		case ERRHDL_MODE_NORMAL:
		{
			ERRHDL_NormalProcess();
			break;
		}
		default:
		{
			gv_stErrHdl.ucMode = ERRHDL_MODE_IDLE;
			ERRHDL_InitMemory();
			break;
		}
	}
}

/*EOF*/
