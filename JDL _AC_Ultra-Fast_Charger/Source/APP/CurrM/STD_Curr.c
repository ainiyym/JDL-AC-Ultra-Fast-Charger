//****************************************************************************************
//*
//* File Name: STD_Curr_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include "STD_Curr.h"
#include "STD_SysM.h"
#include "STD_Lib.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CURR_JOB_SUCESS											(0u) 		/*current module job success*/
#define CURR_JOB_FAILED											(1u)		/*current module job success*/
#define CURR_JOB_PENDING										(2u)		/*current module job success*/
#define CURR_PERCENT_MAX             							(100U)		/*current module job success*/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CURR_MODE_IDLE = 0u,			/*current module idle mode*/
    CURR_MODE_WAIT,					/*current module wait mode*/
    CURR_MODE_NORMAL,				/*current module normal mode*/
}CurrMode_Enum;

enum
{
	CURR_ONE_PHASE = 1u,			/*current module one phase enum type*/
	CURR_THR_PHASE = 3u,			/*current module three phase enum type*/
};

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

typedef struct
{
	uint8_t ucCurrEnCaliVal;		   /*Current enable scale quantification*/
	uint16_t usCurrTempVal;			   /*Channel Current value*/
	LibFilterStruct stLv1OverCurrFilt; /*Primary overcurrent fault state filtering*/
	LibFilterStruct stLv2OverCurrFilt; /*Secondary overcurrent fault status filtering*/
} CurrCh_Struct;

typedef struct
{
	CurrMode_Enum enMode;							  /*Current module mode status*/
	uint8_t ucEnStatus;								  /*Module enablement status*/
	uint8_t ucCurrOutputMode;						  /*Current module output mode type*/
	uint8_t ucAdjCpCurrEn;							  /*Enable to adjust CP current*/
	uint8_t ucReqSuspChargeStatus;					  /*Request to suspend the charging state*/
	uint16_t usReqCfgDfltCurrVal;					  /*Request set default current value*/
	uint16_t usDfltCurrVal;							  /*Rated current value*/
	uint16_t usAdjCpCurrWaitCnt;					  /*Adjust CP current wait count*/
	uint16_t usAdjCpCurrPrc;						  /*Adjust CP current precent*/
	uint16_t usAdjCpCurrVal;						  /*Adjust CP current value*/
	uint16_t usLv1OverCurrThrsh;					  /*Primary overcurrent fault threshold*/
	uint16_t usLv2OverCurrThrsh;					  /*Level 2 overcurrent fault threshold*/
	CurrCh_Struct stChanVartArray[CURR_CHAN_MAX_NUM]; /*Channel structure*/
} Curr_Struct;

/*******************************************************************************
|    Global NVM KAM Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static Curr_Struct gv_stCurr[SYS_CONNECTOR_NUM_MAX];

/****************************************************************************************
 	Global Variable Definition Section
*****************************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void CURR_ModeIdleHandle(SysConnector_Num_Enum ch);
static void CURR_ModeWaitHandle(SysConnector_Num_Enum ch);
static void CURR_ModeNormalHandle(SysConnector_Num_Enum ch);

static void CURR_AdjCpCurrHandle(SysConnector_Num_Enum ch);
static void CURR_RfrshDfltCurr(SysConnector_Num_Enum ch);
static void CURR_RfrshCurrCaliVal(SysConnector_Num_Enum ch);
static void CURR_RfrshFltCurrThrsh(SysConnector_Num_Enum ch);
static void CURR_ChargingErrJudgy(SysConnector_Num_Enum ch);
#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
static void CURR_Lv1OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan);
#endif
#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
static void CURR_Lv2OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan);
#endif
static void CURR_ErrCallBackHandle(SysConnector_Num_Enum ch);

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/*code*/

/*******************************************************************************
Name            : CURR_InitMemory
Syntax          : void CURR_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : CURR Module Initialize RAM
Call By         : STD_SysM
|******************************************************************************/
void CURR_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)&gv_stCurr, (0u), (uint16_t)(sizeof(gv_stCurr) / sizeof(uint8_t)));
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
#if CURR_MONITOR_MODE_TYPE == CURR_ONE_PHASE_MODE_TYPE
		gv_stCurr[ch].usDfltCurrVal = CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL;
		gv_stCurr[ch].usAdjCpCurrPrc = CURR_PERCENT_MAX;
		gv_stCurr[ch].usReqCfgDfltCurrVal = CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL;
#elif CURR_MONITOR_MODE_TYPE == CURR_THR_PHASE_MODE_TYPE
		gv_stCurr[ch].usDfltCurrVal = CURR_THR_PHASE_DFLT_CURR_VAL;
		gv_stCurr[ch].usReqCfgDfltCurrVal = CURR_THR_PHASE_DFLT_CURR_VAL;
#endif
	}
}

/*******************************************************************************
Name            : CURR_Enable
Syntax          : void CURR_Enable(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Module Enable
Call By         : STD_SysM
|******************************************************************************/
void CURR_Enable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCurr[ch].ucEnStatus = STD_TRUE;
	}
}

/*******************************************************************************
Name            : CURR_Disable
Syntax          : void CURR_Disable(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : Module Disable
Call By         : STD_SysM
|******************************************************************************/
void CURR_Disable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCurr[ch].ucEnStatus = STD_FALSE;
	}
}

/*******************************************************************************
Name            : CURR_AdjDfltCurr
Syntax          : void void CURR_AdjRemoteCurr(SysConnector_Num_Enum ch, uint16_t lv_ucPercent, uint8_t lv_ucEn)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucPercent
Parameters(in)  : lv_ucEn
Parameters(out) : None
Return value    : None
Description     : adjust remote current
Call By         : App_Protocol
|******************************************************************************/
void CURR_AdjRemoteCurr(SysConnector_Num_Enum ch, uint16_t lv_ucPercent, uint8_t lv_ucEn)
{
	if (STD_TRUE == lv_ucEn)
	{
		if (lv_ucPercent <= CURR_PERCENT_MAX)
		{
			gv_stCurr[ch].usAdjCpCurrPrc = lv_ucPercent;
			gv_stCurr[ch].ucAdjCpCurrEn = STD_TRUE;
		}
		else
		{
		}
	}
	else
	{
		gv_stCurr[ch].usAdjCpCurrPrc = CURR_PERCENT_MAX;
		gv_stCurr[ch].ucAdjCpCurrEn = STD_FALSE;
	}
	CURR_DEBUG("connector: %d usAdjCpCurrPrc = %d usDfltCurrVal = %d\n\r", ch, gv_stCurr[ch].usAdjCpCurrPrc, gv_stCurr[ch].usDfltCurrVal);
}

/*******************************************************************************
Name            : CURR_AdjCpCurrHandle
Syntax          : static void CURR_AdjCpCurrHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : adjust cp current handle function
Call By         : CURR_ModeNormalHandle
|******************************************************************************/
static void CURR_AdjCpCurrHandle(SysConnector_Num_Enum ch)
{
	uint16_t lv_usCpCurrPercent = 0u;
#if (STD_ON == CURR_DERATE_PERCENT_FUN_EN)
	uint16_t lv_uCpCurrValue = 0u;
#endif

	if (STD_TRUE == CURR_GetReqChargeStatus(ch))
	{
		if (STD_TRUE == gv_stCurr[ch].ucAdjCpCurrEn)
		{
			if (0u == gv_stCurr[ch].usAdjCpCurrWaitCnt)
			{
				gv_stCurr[ch].usAdjCpCurrVal = (uint16_t)(gv_stCurr[ch].usAdjCpCurrPrc * gv_stCurr[ch].usDfltCurrVal / CURR_PERCENT_MAX);
				CURR_DEBUG("connector: %d usAdjCpCurrVal = %d \n\r", ch, gv_stCurr[ch].usAdjCpCurrVal);
			}

			gv_stCurr[ch].usAdjCpCurrWaitCnt++;

			if (gv_stCurr[ch].usAdjCpCurrWaitCnt >= CURR_REMOTE_REQ_INTER_MAX_WAIT_NUM)
			{
				gv_stCurr[ch].usAdjCpCurrWaitCnt = 0u;
			}
		}
		else if (STD_FALSE == gv_stCurr[ch].ucAdjCpCurrEn)
		{
			gv_stCurr[ch].usAdjCpCurrPrc = CURR_PERCENT_MAX;
			gv_stCurr[ch].usAdjCpCurrVal = gv_stCurr[ch].usDfltCurrVal;
			gv_stCurr[ch].usAdjCpCurrWaitCnt = 0u;
		}
		else
		{
		}

		if (gv_stCurr[ch].usAdjCpCurrVal >= gv_stCurr[ch].usDfltCurrVal)
		{
			lv_usCpCurrPercent = CURR_PERCENT_MAX;
		}
		else if (gv_stCurr[ch].usAdjCpCurrVal < CURR_CFG_DFLT_CURR_MIN)
		{
			lv_usCpCurrPercent = CURR_PERCENT_MAX;
			gv_stCurr[ch].ucReqSuspChargeStatus = STD_TRUE;
		}
		else
		{
			lv_usCpCurrPercent = gv_stCurr[ch].usAdjCpCurrPrc;
		}
#if (STD_ON == CURR_DERATE_PERCENT_FUN_EN)
		if ((uint8_t)ERRHDL_CHARGE_DERATE == CURR_GetChargeConditions(SysConnector_Num_Enum ch))
		{
			gv_stCurr[ch].ucAdjCpCurrEn = STD_TRUE;
			lv_uCpCurrValue = (lv_usCpCurrPercent * CURR_DERATE_PERCENT * gv_stCurr[ch].usDfltCurrVal) / (CURR_PERCENT_MAX * CURR_PERCENT_MAX);

			if (lv_uCpCurrValue < CURR_CFG_DFLT_CURR_MIN)
			{
				lv_usCpCurrPercent = ((CURR_CFG_DFLT_CURR_MIN * CURR_PERCENT_MAX) / gv_stCurr[ch].usDfltCurrVal);
				lv_usCpCurrPercent = (float)((CURR_CFG_DFLT_CURR_MIN * CURR_PERCENT_MAX) / gv_stCurr[ch].usDfltCurrVal) > lv_usCpCurrPercent ? lv_usCpCurrPercent + 1u : lv_usCpCurrPercent;
			}
			else
			{
				lv_usCpCurrPercent = (lv_usCpCurrPercent * CURR_DERATE_PERCENT) / CURR_PERCENT_MAX;
			}
		}
#endif
		CURR_SetCpCurrVal(ch, lv_usCpCurrPercent, gv_stCurr[ch].ucAdjCpCurrEn);
	}
	else if (STD_FALSE == CURR_GetReqChargeStatus(ch))
	{
		gv_stCurr[ch].ucReqSuspChargeStatus = STD_FALSE;
		gv_stCurr[ch].usAdjCpCurrPrc = CURR_PERCENT_MAX;
		gv_stCurr[ch].ucAdjCpCurrEn = STD_FALSE;
		gv_stCurr[ch].usAdjCpCurrWaitCnt = 0u;
	}
	else
	{
	}
}

/*******************************************************************************
Name            : CURR_ChargingErrJudgy
Syntax          : static void CURR_ChargingErrJudgy(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : judge charging error status
Call By         : CURR_ModeNormalHandle
|******************************************************************************/
static void CURR_ChargingErrJudgy(SysConnector_Num_Enum ch)
{
	if (CURR_CHARGE_STATE == CURR_GetChargeStatus(ch))
	{
		CURR_GetAllCurrVailVal(ch);
		for (uint8_t lv_ucChan = 0u; lv_ucChan < CURR_CHAN_MAX_NUM; lv_ucChan++)
		{
			if (STD_TRUE == gv_stCurr[ch].stChanVartArray[lv_ucChan].ucCurrEnCaliVal)
			{
#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
				CURR_Lv1OverCurrHandle(ch, lv_ucChan);
#endif
#if (STD_ON == CURR_LV2_OVERCURR_CHECK_FUN_EN)
				CURR_Lv2OverCurrHandle(ch, lv_ucChan);
#endif
			}
			else
			{
			}
		}
	}
	else if (CURR_CHARGE_STATE != CURR_GetChargeStatus(ch))
	{
		for (uint8_t lv_ucChan = 0u; lv_ucChan < CURR_CHAN_MAX_NUM; lv_ucChan++)
		{
			if (STD_TRUE == gv_stCurr[ch].stChanVartArray[lv_ucChan].ucCurrEnCaliVal)
			{
				LIB_SetMemory((uint8_t *)&gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt, (0u), (uint16_t)(sizeof(gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt) / sizeof(uint8_t)));
				LIB_SetMemory((uint8_t *)&gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt, (0u), (uint16_t)(sizeof(gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt) / sizeof(uint8_t)));
				gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucValidStatus = 0xff;
				gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucValidStatus = 0xff;
			}
			else
			{
			}
		}
	}
	else
	{
	}
	CURR_ErrCallBackHandle(ch);
}

/*******************************************************************************
Name            : CURR_GetSuspChargeStatus
Syntax          : uint8_t CURR_GetSuspChargeStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : gv_stCurr[ch].ucReqSuspChargeStatus
Description     : get remote suspend status
Call By         : STD_AuthM
|******************************************************************************/
uint8_t CURR_GetRemoteSuspendStatus(SysConnector_Num_Enum ch)
{
	 return gv_stCurr[ch].ucReqSuspChargeStatus;
}

/*******************************************************************************
Name            : CURR_SetDfltCurrVal
Syntax          : void CURR_SetDfltCurrVal(SysConnector_Num_Enum ch, uint8_t lv_ucCurrentVal)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : -
Call By         : -
|******************************************************************************/
void CURR_SetDfltCurrVal(SysConnector_Num_Enum ch, uint8_t lv_ucCurrentVal)
{
	gv_stCurr[ch].usReqCfgDfltCurrVal = (uint16_t)lv_ucCurrentVal;
	CURR_DEBUG("connector: %d gv_stCurr[ch].usReqCfgDfltCurrVal = %d\n\r",ch, gv_stCurr[ch].usReqCfgDfltCurrVal);
}

/*******************************************************************************
Name            : CURR_GetOutPutModeType
Syntax          : uint8_t CURR_GetOutPutModeType(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : gv_stCurr[ch].ucCurrOutputMode
Description     : get output mode type
Call By         : -
|******************************************************************************/
uint8_t CURR_GetOutPutModeType(SysConnector_Num_Enum ch)
{
	return gv_stCurr[ch].ucCurrOutputMode;
}

/*******************************************************************************
Name            : CURR_GetL1Value
Syntax          : uint8_t CURR_GetOutPutJobResult(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : gv_stCurr[ch].usCurrTempVal
Description     : get output job result
Call By         : -
|******************************************************************************/
uint16_t CURR_GetL1Value(SysConnector_Num_Enum ch)
{
	return gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].usCurrTempVal;
}

/*******************************************************************************
Name            : CURR_IdleHandle
Syntax          : static void CURR_IdleHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : current module idle handle function
Call By         : CURR_20msFunctionControl
|******************************************************************************/
static void CURR_ModeIdleHandle(SysConnector_Num_Enum ch)
{
    if((STD_TRUE == CURR_GetSysPrepareStatus())&&(STD_TRUE == gv_stCurr[ch].ucEnStatus)\
    		&&(STD_TRUE == CURR_GetMeterPrepareStatus(ch)))
    {
		gv_stCurr[ch].enMode = CURR_MODE_WAIT;
    }
    else
    {}
}

/*******************************************************************************
Name            : CURR_WaitHandle
Syntax          : static void CURR_WaitHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : current module wait handle function
Call By         : CURR_20msFunctionControl
|******************************************************************************/
static void CURR_ModeWaitHandle(SysConnector_Num_Enum ch)
{
#if CURR_MONITOR_MODE_TYPE == CURR_ONE_PHASE_MODE_TYPE
	gv_stCurr[ch].ucCurrOutputMode = CURR_ONE_PHASE;
	gv_stCurr[ch].enMode = CURR_MODE_NORMAL;
#elif CURR_MONITOR_MODE_TYPE == CURR_THR_PHASE_MODE_TYPE
	gv_stCurr[ch].ucCurrOutputMode = CURR_THR_PHASE;
	gv_stCurr[ch].enMode = CURR_MODE_NORMAL;
    gv_stCurr[ch].usDfltCurrVal = CURR_THR_PHASE_DFLT_CURR_VAL;
#endif
    CURR_RfrshCurrCaliVal(ch);
}

/*******************************************************************************
Name            : CURR_RfrshCurrCaliVal
Syntax          : static void CURR_RfrshCurrCaliVal(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : refresh current calibrate value
Call By         : CURR_ModeWaitHandle
|******************************************************************************/
static void CURR_RfrshCurrCaliVal(SysConnector_Num_Enum ch)
{
    if(CURR_ONE_PHASE == gv_stCurr[ch].ucCurrOutputMode)
    {
        gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].ucCurrEnCaliVal = STD_TRUE;
        gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].ucCurrEnCaliVal = STD_FALSE;
        gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].ucCurrEnCaliVal = STD_FALSE;

    }
    else if(CURR_THR_PHASE == gv_stCurr[ch].ucCurrOutputMode)
    {
        gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].ucCurrEnCaliVal = STD_TRUE;
        gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].ucCurrEnCaliVal = STD_TRUE;
        gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].ucCurrEnCaliVal = STD_TRUE;
    }
    else
    {}
}

/*******************************************************************************
Name            : CURR_RfrshDfltCurr
Syntax          : static void CURR_RfrshDfltCurr(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : refresh current default value
Call By         : CURR_ModeWaitHandle
|******************************************************************************/
static void CURR_RfrshDfltCurr(SysConnector_Num_Enum ch)
{
	if (gv_stCurr[ch].usReqCfgDfltCurrVal != gv_stCurr[ch].usDfltCurrVal)
	{
		if (gv_stCurr[ch].usReqCfgDfltCurrVal < CURR_CFG_DFLT_CURR_MIN)
		{
			gv_stCurr[ch].usDfltCurrVal = CURR_CFG_DFLT_CURR_MIN;
		}
#if CURR_MONITOR_MODE_TYPE == CURR_ONE_PHASE_MODE_TYPE
		else if (gv_stCurr[ch].usReqCfgDfltCurrVal > CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL)
		{
			if (CURR_CP12V_MODE == CURR_GetCpVolMode(ch))
			{
				gv_stCurr[ch].usDfltCurrVal = CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL;
			}
			else
			{
				gv_stCurr[ch].usDfltCurrVal = CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL;
			}
		}
		else if (gv_stCurr[ch].usReqCfgDfltCurrVal > CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL)
		{
			if (CURR_CP12V_MODE == CURR_GetCpVolMode(ch))
			{
				gv_stCurr[ch].usDfltCurrVal = CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL;
			}
		}
#elif CURR_MONITOR_MODE_TYPE == CURR_THR_PHASE_MODE_TYPE
		else if (gv_stCurr[ch].usReqCfgDfltCurrVal > CURR_THR_PHASE_DFLT_CURR_VAL)
		{
			gv_stCurr[ch].usDfltCurrVal = CURR_THR_PHASE_DFLT_CURR_VAL;
		}
#endif
		else
		{
			gv_stCurr[ch].usDfltCurrVal = gv_stCurr[ch].usReqCfgDfltCurrVal;
		}
		gv_stCurr[ch].usReqCfgDfltCurrVal = gv_stCurr[ch].usDfltCurrVal;
		CURR_DEBUG("Connector:%d DfltCurrVal :%d\r\n", ch, gv_stCurr[ch].usDfltCurrVal);
		CURR_ConfigCurrentValue(ch, gv_stCurr[ch].usDfltCurrVal);
	}
	CURR_RfrshFltCurrThrsh(ch);
}

/*******************************************************************************
Name            : CURR_RfrshFltCurrThrsh
Syntax          : static void CURR_RfrshFltCurrThrsh(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : refresh current fault current threshold value
Call By         : CURR_ModeWaitHandle
|******************************************************************************/
static void CURR_RfrshFltCurrThrsh(SysConnector_Num_Enum ch)
{
	if (gv_stCurr[ch].usDfltCurrVal <= CURR_FLT_CURR_BASIS_VAL)
	{
		gv_stCurr[ch].usLv1OverCurrThrsh = (gv_stCurr[ch].usDfltCurrVal + CURR_L_BASIS_LV1_VAL) * CURR_ENLARGE;
		gv_stCurr[ch].usLv2OverCurrThrsh = (gv_stCurr[ch].usDfltCurrVal + CURR_L_BASIS_LV2_VAL) * CURR_ENLARGE;
	}
	else
	{
		gv_stCurr[ch].usLv1OverCurrThrsh = gv_stCurr[ch].usDfltCurrVal * CURR_ENLARGE * CURR_H_BASIS_LV1_VAL;
		gv_stCurr[ch].usLv2OverCurrThrsh = gv_stCurr[ch].usDfltCurrVal * CURR_ENLARGE * CURR_H_BASIS_LV2_VAL;
	}
}

/*******************************************************************************
Name            : CURR_NormalHandle
Syntax          : static void CURR_NormalHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : current module normal handle function
Call By         : CURR_20msFunctionControl
|******************************************************************************/
static void CURR_ModeNormalHandle(SysConnector_Num_Enum ch)
{
	if (STD_FALSE == gv_stCurr[ch].ucEnStatus)
	{
		gv_stCurr[ch].enMode = CURR_MODE_IDLE;
		LIB_SetMemory((uint8_t *)&gv_stCurr[ch].stChanVartArray, (0u), (uint16_t)(sizeof(gv_stCurr[ch].stChanVartArray) / sizeof(uint8_t)));
	}
	else
	{
		CURR_RfrshCurrCaliVal(ch);
		CURR_AdjCpCurrHandle(ch);
		CURR_ChargingErrJudgy(ch);
	}
}

/*******************************************************************************
Name            : CURR_ErrCallBackHandle
Syntax          : static void CURR_ErrCallBackHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : current module error handle function
Call By         : CURR_ModeNormalHandle
|******************************************************************************/
static void CURR_ErrCallBackHandle(SysConnector_Num_Enum ch)
{
#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
	if (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].stLv1OverCurrFilt.ucValidStatus)
	{
		if (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].stLv1OverCurrFilt.ucStatus)
		{
			CURR_FaultStatusUpdata(ERRHDL_ID_L1P_OVER_CURR_L1, STD_TRUE);
			CURR_DEBUG("connector: %d L1CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].usCurrTempVal);
		}
		else
		{
			CURR_FaultStatusUpdata(ERRHDL_ID_L1P_OVER_CURR_L1, STD_FALSE);
		}
	}
#endif

#if (STD_ON == CURR_LV2_OVERCURR_CHECK_FUN_EN)

	if (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].stLv2OverCurrFilt.ucValidStatus)
	{
		if (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].stLv2OverCurrFilt.ucStatus)
		{
			CURR_FaultStatusUpdata(ERRHDL_ID_L1P_OVER_CURR_L2, STD_TRUE);
			CURR_DEBUG("connector: %d L1CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].usCurrTempVal);
		}
		else
		{
		}
	}
#endif

#if CURR_MONITOR_MODE_TYPE == CURR_THR_PHASE_MODE_TYPE

#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
	static uint8_t lv_ucLv1OverCurrStatus = STD_FALSE;

	if (((STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].stLv1OverCurrFilt.ucValidStatus) || (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].stLv1OverCurrFilt.ucValidStatus)) && (STD_FALSE == lv_ucLv1OverCurrStatus))
	{
		lv_ucLv1OverCurrStatus = STD_TRUE;
		CURR_FaultStatusUpdata(ERRHDL_ID_L2L3P_OVER_CURR_L1, lv_ucLv1OverCurrStatus);
		CURR_DEBUG("connector: %d L2L3Lv1OverCurrStatus : %d\r\n", ch, lv_ucLv1OverCurrStatus);
		CURR_DEBUG("connector: %d L2CurrVal : %d  L3CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].usCurrTempVal, gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].usCurrTempVal);
	}
	else if (lv_ucLv1OverCurrStatus != gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].stLv1OverCurrFilt.ucValidStatus)
	{
		lv_ucLv1OverCurrStatus = STD_FALSE;
		CURR_DEBUG("connector: %d L2L3Lv1OverCurrStatus : %d\r\n", ch, lv_ucLv1OverCurrStatus);
		CURR_DEBUG("connector: %d L2CurrVal : %d  L3CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].usCurrTempVal, gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].usCurrTempVal);
	}
	else
	{
	}
#endif

#if (STD_ON == CURR_LV2_OVERCURR_CHECK_FUN_EN)
	static uint8_t lv_ucLv2OverCurrStatus = STD_FALSE;
	if (((STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].stLv2OverCurrFilt.ucValidStatus) || (STD_TRUE == gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].stLv2OverCurrFilt.ucValidStatus)) && (STD_FALSE == lv_ucLv2OverCurrStatus))
	{
		lv_ucLv2OverCurrStatus = STD_TRUE;
		CURR_FaultStatusUpdata(ERRHDL_ID_L2L3P_OVER_CURR_L2, lv_ucLv2OverCurrStatus);
		CURR_DEBUG("connector: %d L2L3Lv2OverCurrStatus : %d\r\n", ch, lv_ucLv2OverCurrStatus);
		CURR_DEBUG("connector: %d L2CurrVal : %d  L3CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].usCurrTempVal, gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].usCurrTempVal);
	}
	else if (lv_ucLv2OverCurrStatus != gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].stLv2OverCurrFilt.ucValidStatus)
	{
		lv_ucLv2OverCurrStatus = STD_FALSE;
		CURR_DEBUG("connector: %d L2L3Lv2OverCurrStatus : %d\r\n", ch, lv_ucLv2OverCurrStatus);
		CURR_DEBUG("connector: %d L2CurrVal : %d  L3CurrVal : %d\r\n", ch, gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].usCurrTempVal, gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].usCurrTempVal);
	}
	else
	{
	}
#endif

#endif
}

/*******************************************************************************
Name            : CURR_Lv1OverCurrHandle
Syntax          : static void CURR_Lv1OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(out) : None
Return value    : None
Description     : current module level 1 overcurrent handle
Call By         : CURR_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == CURR_LV1_OVERCURR_CHECK_FUN_EN)
static void CURR_Lv1OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan)
{
	if (gv_stCurr[ch].stChanVartArray[lv_ucChan].usCurrTempVal >= gv_stCurr[ch].usLv1OverCurrThrsh)
	{
		gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucStatus = STD_TRUE;
	}
	else if (gv_stCurr[ch].stChanVartArray[lv_ucChan].usCurrTempVal < gv_stCurr[ch].usLv1OverCurrThrsh - 100u)
	{
		gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if (STD_FALSE == gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucValidStatus)
		{
			gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucStatus = STD_FALSE;
		}
		else if (STD_TRUE == gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucValidStatus)
		{
			gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt.ucStatus = STD_TRUE;
		}
		else
		{
		}
	}
	(void)LIB_StatusFilterType2(&(gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv1OverCurrFilt), CURR_LV1_IN_OVERCURR_WAIT_NUM, CURR_LV1_OUT_OVERCURR_WAIT_NUM);
}
#endif
/*******************************************************************************
Name            : CURR_Lv2OverCurrHandle
Syntax          : CURR_Lv2OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan) void CURR_Lv2OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan
Parameters(out) : None
Return value    : None
Description     : current module level 2 overcurrent handle
Call By         : CURR_ModeNormalHandle
|******************************************************************************/
#if (STD_ON == CURR_LV2_OVERCURR_CHECK_FUN_EN)
static void CURR_Lv2OverCurrHandle(SysConnector_Num_Enum ch , uint8_t lv_ucChan)
{
	if (gv_stCurr[ch].stChanVartArray[lv_ucChan].usCurrTempVal >= gv_stCurr[ch].usLv2OverCurrThrsh)
	{
		gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucStatus = STD_TRUE;
	}
	else if (gv_stCurr[ch].stChanVartArray[lv_ucChan].usCurrTempVal < gv_stCurr[ch].usLv2OverCurrThrsh - 100u)
	{
		gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucStatus = STD_FALSE;
	}
	else
	{
		if (STD_FALSE == gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucValidStatus)
		{
			gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucStatus = STD_FALSE;
		}
		else if (STD_TRUE == gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucValidStatus)
		{
			gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt.ucStatus = STD_TRUE;
		}
		else
		{
		}
	}
	(void)LIB_StatusFilterType2(&(gv_stCurr[ch].stChanVartArray[lv_ucChan].stLv2OverCurrFilt), CURR_LV2_IN_OVERCURR_WAIT_NUM, CURR_LV2_OUT_OVERCURR_WAIT_NUM);
}

/*******************************************************************************
Name            : CURR_20msFunctionControl
Syntax          : void CURR_20msFunctionControl(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : None
Description     : current module 20ms function control
Call By         : Task
|******************************************************************************/
void CURR_20msFunctionControl(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		switch (gv_stCurr[ch].enMode)
		{
			case CURR_MODE_IDLE:
			{
				CURR_ModeIdleHandle(ch);
			}
			break;

			case CURR_MODE_WAIT:
			{
				CURR_ModeWaitHandle(ch);
			}
			break;

			case CURR_MODE_NORMAL:
			{
				CURR_ModeNormalHandle(ch);
			}
			break;

			default:
			{
				gv_stCurr[ch].enMode = CURR_MODE_IDLE;
			}
			break;
		}
		CURR_RfrshDfltCurr(ch);
	}
}
#endif
