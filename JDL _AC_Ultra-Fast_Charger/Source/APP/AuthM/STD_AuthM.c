//****************************************************************************************
//*
//* File Name: STD_AuthM.c 
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
#include "STD_AuthM.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#define AUTHM_MODE_IDLE					(0U)  	/*Idle mode*/
#define AUTHM_MODE_UNAUTHORIZED			(1U)	/*Unauthorized mode*/
#define AUTHM_MODE_AUTHORIZED			(2U)	/*Authorized mode*/
#define AUTHM_MODE_REQ_CHARGE			(3U)	/*Request charge mode*/
#define AUTHM_MODE_CHARGING				(4U)	/*Charging mode*/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucMode;			   /*Authorization mode*/
	bool ucReqChargeStatus; /*Request charge status*/
	uint8_t ucAuthoOpenSrc;	   /*Authorization open source*/
	uint8_t ucAuthoCloseSrc;   /*Authorization close source*/
	uint32_t ulNotPlugInCnt;   /*Not plug input count*/
} AuthM_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static AuthM_Struct gv_stAuthM[SYS_CONNECTOR_NUM_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

static void AUTHM_EvseStatusManage(SysConnector_Num_Enum ch);
static void AUTHM_ReqAuthStop(SysConnector_Num_Enum ch);
static void AUTHM_UnauthorizedModeHandle(SysConnector_Num_Enum ch);
static void AUTHM_AuthorizedModeHandle(SysConnector_Num_Enum ch);
static void AUTHM_ReqChargeModeHandle(SysConnector_Num_Enum ch);
static void AUTHM_ChargingModeHandle(SysConnector_Num_Enum ch);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : AUTHM_InitMemory
Syntax          : void AUTHM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                   
Parameters(out) : None                
Return value    : None
Description     : Memory initialization
Call By         : SYSM_InitTwo
|******************************************************************************/
void AUTHM_InitMemory(void)
{
	LIB_SetMemory( (uint8_t *)(&gv_stAuthM), 0u, (uint16_t)(sizeof(gv_stAuthM) / sizeof(uint8_t)));
}

/*******************************************************************************
Name            : AUTHM_AppSetChargeStatus
Syntax          : void AUTHM_AppSetChargeStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                
Parameters(out) : None                
Return value    : void
Description     : APP Set Charge Status  
Call By         : UartM
|******************************************************************************/
void AUTHM_AppSetReqChargeStatus(SysConnector_Num_Enum ch)
{
	gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
}

/*******************************************************************************
Name            : AUTHM_ChargingModeHandle
Syntax          : void AUTHM_ChargingModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                   
Parameters(out) : None                
Return value    : void
Description     : APP Reset Charge Status
Call By         : UartM
|******************************************************************************/
void AUTHM_AppResetReqChargeStatus(SysConnector_Num_Enum ch)
{
	gv_stAuthM[ch].ucReqChargeStatus  = STD_FALSE;
}

/*******************************************************************************
Name            : AUTHM_GetChargeStatus
Syntax          : uint8_t AUTHM_GetChargeStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Charge Status
Call By         : UartM
|******************************************************************************/
uint8_t AUTHM_GetReqChargeStatus(SysConnector_Num_Enum ch)
{
	return gv_stAuthM[ch].ucReqChargeStatus;
}

/*******************************************************************************
Name            : AUTHM_GetAuthOpenSource
Syntax          : uint8_t AUTHM_GetAuthOpenSource(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None               
Return value    : uint8_t
Description     : Get Auth Open Source
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAuthOpenSource(SysConnector_Num_Enum ch)
{
	return gv_stAuthM[ch].ucAuthoOpenSrc;
}

/*******************************************************************************
Name            : AUTHM_GetAuthCloseSource
Syntax          : uint8_t AUTHM_GetAuthCloseSource(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Auth Close Source
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAuthCloseSource(SysConnector_Num_Enum ch)
{
	return gv_stAuthM[ch].ucAuthoCloseSrc;
}

/*******************************************************************************
Name            : AUTHM_GetCurrAuthStatus
Syntax          : uint8_t AUTHM_GetCurrAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Current Auth Status
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetCurrAuthStatus(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucAuthStatus = STD_FALSE;

	if((AUTHM_MODE_AUTHORIZED == gv_stAuthM[ch].ucMode)\
			||(AUTHM_MODE_REQ_CHARGE == gv_stAuthM[ch].ucMode)\
			||(AUTHM_MODE_CHARGING == gv_stAuthM[ch].ucMode)\
			)
	{
		lv_ucAuthStatus = STD_TRUE;
	}
			
	return lv_ucAuthStatus;
}

/*******************************************************************************
Name            : AUTHM_GetAllowResAuthReqStatus
Syntax          : uint8_t AUTHM_GetAllowResAuthReqStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Allow Response Auth Request Status
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAllowResAuthReqStatus(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucRetStatus = STD_FALSE;
	uint8_t lv_ucSafetyStatus = AUTHM_GetSafetyStatus(ch);
#if (AUTHM_CLOSE_EMER_EN == STD_ON)
	uint8_t lv_EmerStatus = AUTHM_GetEmerStopStatus(ch);
#endif
	uint8_t lv_FotaStatus = AUTHM_GetFotaStatus(ch);

	if ((STD_FALSE == lv_ucSafetyStatus)
#if (AUTHM_CLOSE_EMER_EN == STD_ON)
		&& (STD_FALSE == lv_EmerStatus)
#endif
		&& (STD_FALSE == lv_FotaStatus))
	{
		lv_ucRetStatus = STD_TRUE;    
	}
	else
	{
	}

	return lv_ucRetStatus;
}

/*******************************************************************************
Name            : AUTHM_ReqAuthStop
Syntax          : static void AUTHM_ReqAuthStop(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                 
Parameters(out) : None
Return value    : void
Description     : Request Auth Stop
Call By         : Auth
|******************************************************************************/
static void AUTHM_ReqAuthStop(SysConnector_Num_Enum ch)
{
#if (AUTHM_CLOSE_RFID_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetRfidCancelAuthStatus(ch))
	{
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_RFID;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		AUTHM_DEBUG("ch:%d RFID stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_BTAPP_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetBtAppCancelAuthStatus(ch))
	{
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_BT_APP;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		AUTHM_DEBUG("ch:%d BTAPP stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_NETAPP_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetNetAppCancelAuthStatus(ch))
	{

		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_NET_APP;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		AUTHM_DEBUG("ch:%d NETAPP stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_SINGLE_TIMING_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetSingleTimeCancelAuthStatus(ch))
	{
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_SINGLE_TIMING;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		AUTHM_DEBUG("ch:%d ST stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_PERIOD_TIMING_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetPriodTimeCancelAuthStatus(ch))
	{

		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_PERIOD_TIMING;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		AUTHM_DEBUG("ch:%d PT stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_BUTTON_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetButtonStopStatus(ch))
	{

		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_BUTTON_STOP;
		AUTHM_DEBUG("ch:%d Button stop Auth\r\n", ch);
	}
	else
#endif

#if (AUTHM_CLOSE_EMER_EN == STD_ON)
	if (STD_TRUE == AUTHM_GetEmerStopStatus(ch))
	{
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_EMER_STOP;
		AUTHM_DEBUG("ch:%d Emer stop Auth\r\n", ch);
	}
	else
#endif
	if (AUTHM_GetChargeConditions() >= AUTHM_ERRHDL_CHARGE_CANCEL)
	{
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_SERIOUS_FLT;
		AUTHM_DEBUG("ch:%d SERIOUS FLT stop Auth\r\n", ch);
	}
	else
	{
	}
}

/*******************************************************************************
Name            : AUTHM_UnauthorizedModeHandle
Syntax          : static void AUTHM_UnauthorizedModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :              
Parameters(in)  : None
Parameters(out) : None
Return value    : void
Description     : Unauthorized Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_UnauthorizedModeHandle(SysConnector_Num_Enum ch)
{
	if(STD_TRUE == AUTHM_GetAllowResAuthReqStatus(ch))
	{
#if (AUTHM_OPEN_RFID_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetRfidAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_RFID;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetRfidAuthStatus(ch);
			AUTHM_DEBUG("ch:%d RFID Auth\r\n", ch);
		}
		else
#endif

#if (AUTHM_OPEN_BTAPP_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetBtAppAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_BT_APP;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetBtAppAuthStatus(ch);
			AUTHM_DEBUG("ch:%d BT Auth\r\n", ch);
		}
		else
#endif			

#if (AUTHM_OPEN_NETAPP_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetNetAppAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_NET_APP;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetNetAuthStatus(ch);
			AUTHM_DEBUG("ch:%d 5G Auth\r\n", ch);
		}
		else
#endif	

#if (AUTHM_OPEN_SINGLE_TIMING_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetSingleTimeAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_SINGLE_TIMING;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("ch:%d Single Timing Auth\r\n", ch);
		}
		else
#endif

#if (AUTHM_OPEN_PERIOD_TIMING_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetPriodTimeAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_PERIOD_TIMING;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("ch:%d Period Timing Auth\r\n", ch);
		}
		else
#endif

#if (AUTHM_OPEN_NOAUTH_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetNoAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_NO_AUTHEN;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetNoAuthStatus(ch);
			AUTHM_DEBUG("ch:%d NoAuth Auth\r\n", ch);
		}
		else
#endif

#if (AUTHM_OPEN_BT_FAST_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetBtFastAuthStatus(ch))
		{
			gv_stAuthM[ch].ucAuthoOpenSrc = AUTHM_OPEN_SRC_BT_FAST;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("ch:%d BTFAST Auth\r\n", ch);
		}
		else
#endif
		{}
	}
	else
	{}

	if(AUTHM_MODE_AUTHORIZED == gv_stAuthM[ch].ucMode)
	{
		gv_stAuthM[ch].ulNotPlugInCnt = 0u;
		AUTHM_ClearFltInfo();
		AUTHM_ResetEvseSelfCheckState(ch);
	}
}

/*******************************************************************************
Name            : AUTHM_AuthorizedModeHandle
Syntax          : static void AUTHM_AuthorizedModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :               
Parameters(in)  : None                 
Parameters(out) : None
Return value    : void
Description     : Authorized Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_AuthorizedModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucCpVolStatus;
	lv_ucCpVolStatus = AUTHM_GetCpStatus(ch);

	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus || AUTHM_CP_VOL_STATUS_4V == lv_ucCpVolStatus)
	{
		gv_stAuthM[ch].ulNotPlugInCnt++;
		if(gv_stAuthM[ch].ulNotPlugInCnt >= AUTHM_NOT_PLUG_IN_TIMEOUT_CNT)
		{
			gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_NOPLUG_TIMEOUT;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
			gv_stAuthM[ch].ulNotPlugInCnt = 0u;
			AUTHM_DEBUG("ch:%d Auth Wait Overtime\r\n", ch);
		}
	}
	else if ((AUTHM_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (AUTHM_CP_VOL_STATUS_6V == lv_ucCpVolStatus)\
			||(AUTHM_CP_VOL_STATUS_3V == lv_ucCpVolStatus)||(AUTHM_CP_VOL_STATUS_2V == lv_ucCpVolStatus))
	{
		if (STD_FALSE == gv_stAuthM[ch].ucReqChargeStatus)
		{
			gv_stAuthM[ch].ucReqChargeStatus = STD_TRUE;
			gv_stAuthM[ch].ucMode = AUTHM_MODE_REQ_CHARGE;
		}
		else
		{
			gv_stAuthM[ch].ucMode = AUTHM_MODE_REQ_CHARGE;
		}
	}
	else
	{}
	
	AUTHM_ReqAuthStop(ch);
}

/*******************************************************************************
Name            : AUTHM_ReqChargeModeHandle
Syntax          : static void AUTHM_ReqChargeModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none                               
Parameters(out) : None
Return value    : void
Description     : Request Charge Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_ReqChargeModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucEvseStatus;
	uint8_t lv_ucCpVolStatus;
	
	lv_ucCpVolStatus = AUTHM_GetCpStatus(ch);
	lv_ucEvseStatus = AUTHM_GetEvseStatus(ch);

	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus || AUTHM_CP_VOL_STATUS_4V == lv_ucCpVolStatus)
	{
		gv_stAuthM[ch].ucMode = AUTHM_MODE_AUTHORIZED;
		AUTHM_DEBUG("ch:%d ReqCharge goback Auth\r\n", ch);
	}
	else if((AUTHM_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (AUTHM_CP_VOL_STATUS_6V == lv_ucCpVolStatus)\
			||(AUTHM_CP_VOL_STATUS_3V == lv_ucCpVolStatus)||(AUTHM_CP_VOL_STATUS_2V == lv_ucCpVolStatus))
	{
		if(AUTHM_EVSE_STATUS_3_DOT == lv_ucEvseStatus)
		{
			gv_stAuthM[ch].ucMode = AUTHM_MODE_CHARGING;
			AUTHM_DEBUG("ch:%d Enter the charging\r\n", ch);
		}
	}
	else
	{}
		
	AUTHM_ReqAuthStop(ch);
}

/*******************************************************************************
Name            : AUTHM_ChargingModeHandle
Syntax          : static void AUTHM_ChargingModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                               
Parameters(out) : None
Return value    : void
Description     : Charging Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_ChargingModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucCpVolStatus;
	lv_ucCpVolStatus = AUTHM_GetCpStatus(ch);
	
	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus || AUTHM_CP_VOL_STATUS_4V == lv_ucCpVolStatus)
	{
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
		gv_stAuthM[ch].ucReqChargeStatus = STD_FALSE;	
		gv_stAuthM[ch].ucAuthoCloseSrc = AUTHM_CLOSE_SRC_DRAWGUN;
	}
	else
	{
		AUTHM_ReqAuthStop(ch);
	}
}

/*******************************************************************************
Name            : AUTHM_EvseStatusManage
Syntax          : static void AUTHM_EvseStatusManage(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                          
Parameters(out) : None
Return value    : void
Description     : Manage Evse Status
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_EvseStatusManage(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucChargeConditions;
	lv_ucChargeConditions = ERRHDL_GetChargeConditions();

	if(STD_TRUE == gv_stAuthM[ch].ucReqChargeStatus)
	{
		if (AUTHM_ERRHDL_CHARGE_SUSPENDED == lv_ucChargeConditions)
		{
			AUTHM_ReqEvseChargeOff(ch);
			AUTHM_SetStopChargeReason(ch, AUTHM_STOP_CHARGE_ERROR_SUSPEND);
		}
		else if (STD_TRUE == AUTHM_GetRemoteSuspendStatus())
		{
			AUTHM_ReqEvseChargeOff(ch);
			AUTHM_SetStopChargeReason(ch, AUTHM_STOP_CHARGE_REMOTE_SUSPEND);
		}
		else
		{
			AUTHM_ReqEvseChargeOn(ch);
		}
	}
	else
	{
		AUTHM_ReqEvseChargeOff(ch);
		gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
	}
}

static void AUTHM_AuthStatusManage(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucAuthStatus;

	lv_ucAuthStatus = AUTHM_GetCurrAuthStatus(ch);

	if (STD_TRUE == lv_ucAuthStatus)
	{
		AUTHM_SetAuthStatus(ch, STD_TRUE);
	}
	else
	{
		AUTHM_SetAuthStatus(ch, STD_FALSE);
	}
}

/*******************************************************************************
Name            : AUTHM_10msMainFunction
Syntax          : void AUTHM_10msMainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                             
Parameters(out) : None
Return value    : void
Description     : 10ms Main Function
Call By         : TASK( OS_Task10ms )
|******************************************************************************/
void AUTHM_10msMainFunction(void)
{
	SysConnector_Num_Enum ch;

	for (ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		switch (gv_stAuthM[ch].ucMode)
		{
			case AUTHM_MODE_IDLE:
			{
				if (STD_TRUE == AUTHM_GetResetPrepareStatus())
				{
					gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
				}
				else
				{
				}
				break;
			}
			case AUTHM_MODE_UNAUTHORIZED:
			{
				AUTHM_UnauthorizedModeHandle(ch);
				break;
			}
			case AUTHM_MODE_AUTHORIZED:
			{
				AUTHM_AuthorizedModeHandle(ch);
				break;
			}
			case AUTHM_MODE_REQ_CHARGE:
			{
				AUTHM_ReqChargeModeHandle(ch);
				break;
			}
			case AUTHM_MODE_CHARGING:
			{
				AUTHM_ChargingModeHandle(ch);
				break;
			}
			default:
			{
				gv_stAuthM[ch].ucMode = AUTHM_MODE_UNAUTHORIZED;
				break;
			}
		}

		AUTHM_EvseStatusManage(ch);
		AUTHM_AuthStatusManage(ch);
	}
}

/*EOF*/
