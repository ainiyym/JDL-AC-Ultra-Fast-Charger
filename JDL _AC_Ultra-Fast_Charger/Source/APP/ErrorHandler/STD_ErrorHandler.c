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
	ErrHdlLevel_Enum enErrorL;				   /*Error level */
	ErrHdlLevel_Enum enFinalEeeorL;			   /*Last Error level*/
	uint8_t ucMode;							   /*Error Module mode */
	uint8_t ucEnStatus;						   /*Error Module Enable */
	uint8_t ucCurrIdx;						   /*Current error id */
	uint8_t ucChargeConditions;				   /*Charging Conditions*/
	uint8_t arFltData[ERRHDL_BYTE_MAX_NUM];	   /*Fault data*/
	uint8_t arFltCnt[ERRHDL_ID_MAX_NUM];	   /*Number of failures*/
	uint32_t arRecoveryCnt[ERRHDL_ID_MAX_NUM]; /*Duration of fault occurrence*/
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
}

/*******************************************************************************
Name            : ERRHDL_GetErrorLevelStatus
Syntax          : ErrHdlLevel_Enum ERRHDL_GetErrorLevelStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : ErrHdlLevel_Enum
Description     : Obtaining the Fault level
Call By         :
|******************************************************************************/
ErrHdlLevel_Enum ERRHDL_GetErrorLevelStatus(void)
{
	return gv_stErrHdl.enFinalEeeorL;
}

/*******************************************************************************
Name            : ERRHDL_GetChargeConditions
Syntax          : uint8_t ERRHDL_GetChargeConditions(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none
Parameters(out) : none
Return value    : uint8_t
Description     : Obtaining charging status information
Call By         :
|******************************************************************************/
uint8_t ERRHDL_GetChargeConditions(void)
{
	return gv_stErrHdl.ucChargeConditions;
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
Call By         : Cyclic Executing Function
|******************************************************************************/
static void ERRHDL_JudgeChargeConditions(void)
{
	switch (gv_stErrHdl.enFinalEeeorL)
	{
	case ERRHDL_FLT_L_NONE:
	case ERRORH_FLT_L_ONE:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_ALLOW;
		break;
	}
	case ERRORH_FLT_L_TWO:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_DERATE;
		break;
	}
	case ERRORH_FLT_L_THREE:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_SUSPENDED;
		break;
	}
	case ERRORH_FLT_L_FOUR:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_CANCEL;
		break;
	}
	case ERRORH_FLT_L_FIVE:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_SAFETY;
		break;
	}
	default:
	{
		gv_stErrHdl.ucChargeConditions = (uint8_t)ERRHDL_CHARGE_ALLOW;
		break;
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
	uint8_t lv_ucByteIdx = 0, lv_ucBitOffset = 0;
	uint8_t lv_ucErrHdlId = 0;

	if ((uint8_t)STD_FALSE == gv_stErrHdl.ucEnStatus)
	{
		gv_stErrHdl.ucMode = ERRHDL_MODE_IDLE;
		ERRHDL_InitMemory();
	}
	else
	{
		if (gv_stErrHdl.ucCurrIdx < (uint8_t)ERRHDL_ID_MAX_NUM)
		{
			lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltByteIdx;
			lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[gv_stErrHdl.ucCurrIdx].enFltBitOffset;
			if ((uint8_t)STD_FALSE == LIB_GET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset))
			{
				gv_stErrHdl.ucCurrIdx = 0u;
				gv_stErrHdl.enErrorL = ERRHDL_FLT_L_NONE;
			}
			else
			{
			}
		}
		else
		{
			gv_stErrHdl.ucCurrIdx = 0u;
			gv_stErrHdl.enErrorL = ERRHDL_FLT_L_NONE;
		}

		for (lv_ucErrHdlId = 0u; lv_ucErrHdlId < (uint8_t)ERRHDL_ID_MAX_NUM; lv_ucErrHdlId++)
		{
			lv_ucByteIdx = (uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltByteIdx;
			lv_ucBitOffset = (uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltBitOffset;

			if ((uint8_t)STD_TRUE == LIB_GET_UINT8_BIT(gv_stErrHdl.arFltData[lv_ucByteIdx], lv_ucBitOffset)) 
			{
				if ((uint8_t)ERRHDL_FLT_FLAG_MULTI != (uint8_t)(c_arErrHdlCfgArray[lv_ucErrHdlId].ucFltFlag & ERRHDL_FLT_FLAG_MULTI))
				{
					if ((uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel > (uint8_t)gv_stErrHdl.enErrorL)
					{
						gv_stErrHdl.enErrorL = c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel;
						gv_stErrHdl.ucCurrIdx = lv_ucErrHdlId;
					}
					else
					{
					}
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
							if ((uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel > (uint8_t)gv_stErrHdl.enErrorL)
							{
								gv_stErrHdl.enErrorL = c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel;
								gv_stErrHdl.ucCurrIdx = lv_ucErrHdlId;
							}
							else
							{
							}
						}
						else
						{
							if ((uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enTempFltLevel > (uint8_t)gv_stErrHdl.enErrorL)
							{
								gv_stErrHdl.enErrorL = c_arErrHdlCfgArray[lv_ucErrHdlId].enTempFltLevel;
								gv_stErrHdl.ucCurrIdx = lv_ucErrHdlId;
							}
							else
							{
							}
						}

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
					else
					{
						if ((uint8_t)c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel > (uint8_t)gv_stErrHdl.enErrorL)
						{
							gv_stErrHdl.enErrorL = c_arErrHdlCfgArray[lv_ucErrHdlId].enFltLevel;
							gv_stErrHdl.ucCurrIdx = lv_ucErrHdlId;
						}
						else
						{
						}
					}
				}
			}
			else
			{
				gv_stErrHdl.arRecoveryCnt[lv_ucErrHdlId] = 0u;
			}
		}

		if ((uint8_t)gv_stErrHdl.enFinalEeeorL != (uint8_t)gv_stErrHdl.enErrorL)
		{
			gv_stErrHdl.enFinalEeeorL = gv_stErrHdl.enErrorL;
			ERRHDL_JudgeChargeConditions();
		}
		else
		{
		}
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
