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
    uint8_t ucMode;					/*Authorization mode*/
    uint8_t ucReqChargeStatus;		/*Request charging status*/
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
    uint8_t ucAuthoOpenSrc;			/*Authorization open source*/
    uint8_t ucAuthoCloseSrc;			/*Authorization close source*/
#endif
    uint32_t ulNotPlugInCnt;			/*Not plug input count*/

}AuthM_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static AuthM_Struct gv_stAuthM;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

static void AUTHM_EvseStatusManage(void);
static void AUTHM_ReqAuthStop(void);
static void AUTHM_UnauthorizedModeHandle(void);
static void AUTHM_AuthorizedModeHandle(void);
static void AUTHM_ReqChargeModeHandle(void);
static void AUTHM_ChargingModeHandle(void);

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

#if((AUTHM_CORE_TYPE == AUTHM_CORE_SALVE)  ||  (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
/*******************************************************************************
Name            : AUTHM_AppSetChargeStatus
Syntax          : void AUTHM_AppSetChargeStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                
Parameters(out) : None                
Return value    : void
Description     : APP Set Charge Status  
Call By         : UartM
|******************************************************************************/
void AUTHM_AppSetReqChargeStatus(void)
{
	gv_stAuthM.ucReqChargeStatus = STD_TRUE;
}

/*******************************************************************************
Name            : AUTHM_ChargingModeHandle
Syntax          : void AUTHM_ChargingModeHandle(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                   
Parameters(out) : None                
Return value    : void
Description     : APP Reset Charge Status
Call By         : UartM
|******************************************************************************/
void AUTHM_AppResetReqChargeStatus(void)
{
	gv_stAuthM.ucReqChargeStatus  = STD_FALSE;
}

/*******************************************************************************
Name            : AUTHM_GetChargeStatus
Syntax          : uint8_t AUTHM_GetChargeStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Charge Status
Call By         : UartM
|******************************************************************************/
uint8_t AUTHM_GetReqChargeStatus(void)
{
	return gv_stAuthM.ucReqChargeStatus;
}

#endif

#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
/*******************************************************************************
Name            : AUTHM_GetAuthOpenSource
Syntax          : uint8_t AUTHM_GetAuthOpenSource(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None               
Return value    : uint8_t
Description     : Get Auth Open Source
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAuthOpenSource(void)
{
	return gv_stAuthM.ucAuthoOpenSrc;
}

/*******************************************************************************
Name            : AUTHM_GetAuthCloseSource
Syntax          : uint8_t AUTHM_GetAuthCloseSource(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Auth Close Source
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAuthCloseSource(void)
{
	return gv_stAuthM.ucAuthoCloseSrc;
}

/*******************************************************************************
Name            : AUTHM_GetCurrAuthStatus
Syntax          : uint8_t AUTHM_GetCurrAuthStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Current Auth Status
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetCurrAuthStatus(void)
{
	uint8_t lv_ucAuthStatus = STD_FALSE;

	if((AUTHM_MODE_AUTHORIZED == gv_stAuthM.ucMode)\
			||(AUTHM_MODE_REQ_CHARGE == gv_stAuthM.ucMode)\
			||(AUTHM_MODE_CHARGING == gv_stAuthM.ucMode)\
			)
	{
		lv_ucAuthStatus = STD_TRUE;
	}
			
	return lv_ucAuthStatus;
}

/*******************************************************************************
Name            : AUTHM_GetAllowResAuthReqStatus
Syntax          : uint8_t AUTHM_GetAllowResAuthReqStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                    
Parameters(out) : None                
Return value    : uint8_t
Description     : Get Allow Response Auth Request Status
Call By         : 
|******************************************************************************/
uint8_t AUTHM_GetAllowResAuthReqStatus(void)
{
	uint8_t lv_ucRetStatus,lv_ucSafetyStatus,lv_EmerStatus,lv_FotaStatus;
	lv_ucRetStatus = STD_FALSE;
	lv_ucSafetyStatus = AUTHM_GetSafetyStatus();
#if(AUTHM_CLOSE_EMER_EN == STD_ON)
	lv_EmerStatus = AUTHM_GetEmerStopStatus();
#endif
	lv_FotaStatus = AUTHM_GetFotaStatus();

	if((AUTHM_SAFETY_FLAG_ENTER != lv_ucSafetyStatus)
#if(AUTHM_CLOSE_EMER_EN == STD_ON)
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
#endif

/*******************************************************************************
Name            : AUTHM_ReqAuthStop
Syntax          : static void AUTHM_ReqAuthStop(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                                 
Parameters(out) : None
Return value    : void
Description     : Request Auth Stop
Call By         : Auth
|******************************************************************************/
static void AUTHM_ReqAuthStop(void)
{
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
#if (AUTHM_CLOSE_RFID_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetRfidCancelAuthStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_RFID;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			AUTHM_DEBUG("RFID stop Auth\r\n");
		}
	}
	else
#endif

#if(AUTHM_CLOSE_BTAPP_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetBtAppCancelAuthStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_BT_APP;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			AUTHM_DEBUG("BTAPP stop Auth\r\n");	
		}
	}else
#endif

#if(AUTHM_CLOSE_NETAPP_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetNetAppCancelAuthStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_NET_APP;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			AUTHM_DEBUG("NETAPP stop Auth\r\n");
		}
	}
	else
#endif

#if (AUTHM_CLOSE_SINGLE_TIMING_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetSingleTimeCancelAuthStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_SINGLE_TIMING;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			AUTHM_DEBUG("ST stop Auth\r\n");
		}
	}
	else
#endif

#if (AUTHM_CLOSE_PERIOD_TIMING_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetPriodTimeCancelAuthStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_PERIOD_TIMING;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			AUTHM_DEBUG("PT stop Auth\r\n");
		}
	}
	else
#endif

#if(AUTHM_CLOSE_BUTTON_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetButtonStopStatus())
	{
		if (STD_TRUE == AUTHM_UartSendStopChargeCmd())
		{
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_BUTTON_STOP;
			AUTHM_DEBUG("Button stop Auth\r\n");
		}
	}else
#endif
#endif

#if(AUTHM_CLOSE_EMER_EN == STD_ON)
	if(STD_TRUE == AUTHM_GetEmerStopStatus())
	{
		gv_stAuthM.ucReqChargeStatus = STD_FALSE;
		gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
		gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_EMER_STOP;
#endif
		AUTHM_DEBUG("Emer stop Auth\r\n");
	}else
#endif

	if(AUTHM_GetChargeConditions() >= AUTHM_ERRHDL_CHARGE_CANCEL)
	{
		gv_stAuthM.ucReqChargeStatus = STD_FALSE;
		gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
		gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_SERIOUS_FLT;
#endif
		AUTHM_DEBUG("SERIOUS FLT stop Auth\r\n");
	}
	else
	{}
}

/*******************************************************************************
Name            : AUTHM_UnauthorizedModeHandle
Syntax          : static void AUTHM_UnauthorizedModeHandle(void)
Sync/Async      : Synchronous
Reentrancy      :              
Parameters(in)  : None
Parameters(out) : None
Return value    : void
Description     : Unauthorized Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_UnauthorizedModeHandle(void)
{
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
	if(STD_TRUE == AUTHM_GetAllowResAuthReqStatus())
	{
#if (AUTHM_OPEN_RFID_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetRfidAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_RFID;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetRfidAuthStatus();
			AUTHM_DEBUG("RFID Auth\r\n");
		}
		else
#endif

#if (AUTHM_OPEN_BTAPP_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetBtAppAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_BT_APP;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetBtAppAuthStatus();
			AUTHM_DEBUG("BT Auth\r\n");
		}
		else
#endif			

#if (AUTHM_OPEN_NETAPP_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetNetAppAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_NET_APP;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetNetAuthStatus();
			AUTHM_DEBUG("5G Auth\r\n");
		}
		else
#endif	

#if (AUTHM_OPEN_ORDER_CONT_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetOrderContAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_ORDER_CONT;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("Order Auth\r\n");
		}
		else
#endif

#if (AUTHM_OPEN_SINGLE_TIMING_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetSingleTimeAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_SINGLE_TIMING;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("Single Timing Auth\r\n");
		}
		else
#endif

#if (AUTHM_OPEN_PERIOD_TIMING_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetPriodTimeAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_PERIOD_TIMING;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("Period Timing Auth\r\n");
		}
		else
#endif

#if (AUTHM_OPEN_NOAUTH_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetNoAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_NO_AUTHEN;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_ResetNoAuthStatus();
			AUTHM_DEBUG("NoAuth Auth\r\n");
		}
		else
#endif

#if (AUTHM_OPEN_BT_FAST_EN == STD_ON)
		if(STD_TRUE == AUTHM_GetBtFastAuthStatus())
		{
			gv_stAuthM.ucAuthoOpenSrc = AUTHM_OPEN_SRC_BT_FAST;
			gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			AUTHM_DEBUG("BTFAST Auth\r\n");
		}
		else
#endif
		{}
	}
	else
	{}

	if(AUTHM_MODE_AUTHORIZED == gv_stAuthM.ucMode)
	{
		gv_stAuthM.ulNotPlugInCnt = 0u;
		AUTHM_ClearFltInfo();
		AUTHM_ResetEvseSelfCheckState();
	}
#endif	

#if(AUTHM_CORE_TYPE == AUTHM_CORE_SALVE)
	if(STD_TRUE == gv_stAuthM.ucReqChargeStatus)
	{
		gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
	}
#endif
}

/*******************************************************************************
Name            : AUTHM_AuthorizedModeHandle
Syntax          : static void AUTHM_AuthorizedModeHandle(void)
Sync/Async      : Synchronous
Reentrancy      :               
Parameters(in)  : None                 
Parameters(out) : None
Return value    : void
Description     : Authorized Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_AuthorizedModeHandle(void)
{
	uint8_t lv_ucCpVolStatus;
	lv_ucCpVolStatus = AUTHM_GetCpStatus();

	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus)
	{
		gv_stAuthM.ulNotPlugInCnt++;
		if(gv_stAuthM.ulNotPlugInCnt >= AUTHM_NOT_PLUG_IN_TIMEOUT_CNT)
		{
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
			gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_NOPLUG_TIMEOUT;
#endif
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			gv_stAuthM.ucReqChargeStatus = STD_FALSE;
			gv_stAuthM.ulNotPlugInCnt = 0u;
			AUTHM_DEBUG("Auth Wait Overtime\r\n");
		}
	}
	else if((AUTHM_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (AUTHM_CP_VOL_STATUS_6V == lv_ucCpVolStatus))
	{
		if(STD_FALSE == gv_stAuthM.ucReqChargeStatus)
		{
#if(AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR)

			if (STD_TRUE == AUTHM_UartSendReqChargeCmd())
			{
				gv_stAuthM.ucReqChargeStatus = STD_TRUE;
				gv_stAuthM.ucMode = AUTHM_MODE_REQ_CHARGE;
			}
#else
			gv_stAuthM.ucReqChargeStatus = STD_TRUE;
			gv_stAuthM.ucMode = AUTHM_MODE_REQ_CHARGE;
#endif
		}
		else
		{
			gv_stAuthM.ucMode = AUTHM_MODE_REQ_CHARGE;
		}
	}
	else
	{}
	
	AUTHM_ReqAuthStop();
}

/*******************************************************************************
Name            : AUTHM_ReqChargeModeHandle
Syntax          : static void AUTHM_ReqChargeModeHandle(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none                               
Parameters(out) : None
Return value    : void
Description     : Request Charge Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_ReqChargeModeHandle(void)
{
	uint8_t lv_ucEvseStatus;
	uint8_t lv_ucCpVolStatus;
	
	lv_ucCpVolStatus = AUTHM_GetCpStatus();
	lv_ucEvseStatus = AUTHM_GetEvseStatus();

	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus)
	{
		gv_stAuthM.ucMode = AUTHM_MODE_AUTHORIZED;
		AUTHM_DEBUG("ReqCharge goback Auth\r\n");
	}
	else if((AUTHM_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (AUTHM_CP_VOL_STATUS_6V == lv_ucCpVolStatus))
	{
		if(AUTHM_EVSE_STATUS_3_DOT == lv_ucEvseStatus)
		{
			gv_stAuthM.ucMode = AUTHM_MODE_CHARGING;
			AUTHM_DEBUG("Enter the charging\r\n");
		}
	}
	else
	{}
		
	AUTHM_ReqAuthStop();
}

/*******************************************************************************
Name            : AUTHM_ChargingModeHandle
Syntax          : static void AUTHM_ChargingModeHandle(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                               
Parameters(out) : None
Return value    : void
Description     : Charging Mode Handle
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_ChargingModeHandle(void)
{
	uint8_t lv_ucCpVolStatus;
	lv_ucCpVolStatus = AUTHM_GetCpStatus();
	
	if(AUTHM_CP_VOL_STATUS_12V == lv_ucCpVolStatus)
	{
		gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
		gv_stAuthM.ucReqChargeStatus = STD_FALSE;
#if((AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))		
		gv_stAuthM.ucAuthoCloseSrc = AUTHM_CLOSE_SRC_DRAWGUN;
#endif
	}
	else
	{
		AUTHM_ReqAuthStop();
	}
}

#if((AUTHM_CORE_TYPE == AUTHM_CORE_SALVE) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
/*******************************************************************************
Name            : AUTHM_EvseStatusManage
Syntax          : static void AUTHM_EvseStatusManage(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                          
Parameters(out) : None
Return value    : void
Description     : Manage Evse Status
Call By         : AUTHM_10msMainFunction
|******************************************************************************/
static void AUTHM_EvseStatusManage(void)
{
	uint8_t lv_ucChargeConditions;
	lv_ucChargeConditions= ERRHDL_GetChargeConditions();

	if(STD_TRUE == gv_stAuthM.ucReqChargeStatus)
	{
		if(AUTHM_ERRHDL_CHARGE_SUSPENDED == lv_ucChargeConditions)
		{
			AUTHM_ReqEvseChargeOff();
			AUTHM_SetStopChargeReason(AUTHM_STOP_CHARGE_ERROR_SUSPEND);
		}
		else if(STD_TRUE == AUTHM_GetRemoteSuspendStatus())
		{
			AUTHM_ReqEvseChargeOff();
			AUTHM_SetStopChargeReason(AUTHM_STOP_CHARGE_REMOTE_SUSPEND);
		}
		else
		{
			AUTHM_ReqEvseChargeOn();
		}
	}
	else
	{
		AUTHM_ReqEvseChargeOff();
		gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
	}
}
#endif

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
	switch(gv_stAuthM.ucMode)
	{
		case AUTHM_MODE_IDLE:
		{
		  	if(STD_TRUE == AUTHM_GetResetPrepareStatus())
		   	{
    			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
   		  	}
   			else
			{}
   			break;
		}
		case AUTHM_MODE_UNAUTHORIZED:
		{
			AUTHM_UnauthorizedModeHandle();
			break;
		}
		case AUTHM_MODE_AUTHORIZED:
		{
			AUTHM_AuthorizedModeHandle();
			break;
		}
		case AUTHM_MODE_REQ_CHARGE:
		{
			AUTHM_ReqChargeModeHandle();
			break;
		}
		case AUTHM_MODE_CHARGING:
		{
			AUTHM_ChargingModeHandle();
			break;
		}
		default:
		{
			gv_stAuthM.ucMode = AUTHM_MODE_UNAUTHORIZED;
			break;
		}
	}

#if((AUTHM_CORE_TYPE == AUTHM_CORE_SALVE) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
	AUTHM_EvseStatusManage();
#endif
}

/*EOF*/
