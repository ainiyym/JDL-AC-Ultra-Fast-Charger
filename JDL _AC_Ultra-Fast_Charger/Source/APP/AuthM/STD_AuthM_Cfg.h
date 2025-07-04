//****************************************************************************************
//*
//* File Name: STD_AuthM_Cfg.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_AUTHM_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_AUTHM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_ErrorHandler.h"
#include "Std_Types.h"
#include "STD_EvseM.h"
#include "STD_CpM_Cfg.h"
#include "STD_Lib.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#define AUTHM_CORE_SINGLE  							(0U)		 									/*single core*/
#define AUTHM_CORE_MAJOR  							(1U)											/*major core*/
#define AUTHM_CORE_SALVE  							(2U)											/*slave core*/

#define AUTHM_CORE_TYPE								AUTHM_CORE_SINGLE								/*core type*/
            				
#define AUTHM_OPEN_RFID_EN          				STD_ON											/*rfid Auth open enable state*/
#define AUTHM_OPEN_BTAPP_EN          				STD_ON											/*BTapp Auth open enable state*/
#define AUTHM_OPEN_NETAPP_EN          				STD_ON											/*Netapp Auth open enable state*/
#define AUTHM_OPEN_ORDER_CONT_EN					STD_OFF											/*ordercontinue Auth open enable state*/
#define AUTHM_OPEN_SINGLE_TIMING_EN					STD_ON											/*singletiming Auth open enable state*/
#define AUTHM_OPEN_PERIOD_TIMING_EN					STD_ON											/*periodtiming Auth open enable state*/
#define AUTHM_OPEN_NOAUTH_EN						STD_ON											/*Rfid Auth open enable state*/
#define AUTHM_OPEN_BT_FAST_EN         	 			STD_OFF											/*BTapp Auth open enable state*/

#define AUTHM_CLOSE_EMER_EN 						STD_OFF											/*emergencystop close enable state*/
#define AUTHM_CLOSE_RFID_EN							STD_ON											/*rfid Auth close enable state*/
#define AUTHM_CLOSE_BTAPP_EN						STD_ON											/*BTapp Auth close enable state*/
#define AUTHM_CLOSE_NETAPP_EN						STD_ON											/*Netapp Auth close enable state*/
#define AUTHM_CLOSE_SINGLE_TIMING_EN				STD_ON											/*singletiming close enable state*/
#define AUTHM_CLOSE_PERIOD_TIMING_EN				STD_ON											/*periodtiming close nable state*/
#define AUTHM_CLOSE_BUTTON_EN						STD_OFF											/*Button close enable state*/

#define AUTHM_ERRHDL_CHARGE_ALLOW					ERRHDL_CHARGE_ALLOW								/*0U*/
#define AUTHM_ERRHDL_CHARGE_DERATE					ERRHDL_CHARGE_DERATE							/*1U*/
#define AUTHM_ERRHDL_CHARGE_SUSPENDED				ERRHDL_CHARGE_SUSPENDED							/*2U*/
#define AUTHM_ERRHDL_CHARGE_CANCEL					ERRHDL_CHARGE_CANCEL							/*3U*/
#define AUTHM_ERRHDL_CHARGE_SAFETY					ERRHDL_CHARGE_SAFETY							/*4U*/

#define AUTHM_TASK_PERIOD                			(10U)											/*10ms*/
#define AUTHM_NOT_PLUG_IN_TIMEOUT_CNT  				((uint32_t)300000u / AUTHM_TASK_PERIOD )			/*5 minutes*/

#define AUTHM_GetChargeConditions() 				ERRHDL_GetChargeConditions()					/*get charge conditions*/
#define AUTHM_GetResetPrepareStatus()				SYSM_GetResetPrepareStatus()					/*get system prepare status*/

#if(AUTHM_CORE_TYPE == AUTHM_CORE_MAJOR || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
#define AUTHM_EVSE_STATUS_3_DOT						(uint8_t)EVSEM_STATE_THREE_dot					/*6U*/

#define AUTHM_CP_VOL_STATUS_INVALID					CP_VOL_STATUS_INVALID							/*0U*/
#define AUTHM_CP_VOL_STATUS_12V						CP_VOL_STATUS_12V								/*1U*/
#define AUTHM_CP_VOL_STATUS_9V						CP_VOL_STATUS_9V								/*2U*/
#define AUTHM_CP_VOL_STATUS_6V						CP_VOL_STATUS_6V								/*3U*/

#define AUTHM_CLEAR_ERR_INFO						(1U)
#define AUTHM_RESET_EVSE_SELFCHECK_STATUS			(2U)

#define AUTHM_GetEvseStatus()			    		EVSEM_GetChargeStatus()							/*get evse status*/
#define AUTHM_GetCpStatus()	 						PLUG_GetCpVolStatus()							/*get cp status*/
#define AUTHM_GetEmerStopStatus()	       		 	EMSTOPM_GetKeyValue()							/*get emergencystop status*/


#define	AUTHM_CHARGE_STOP   						(0U)	    									/*stop charging*/
#define	AUTHM_CHARGE_START 							(1U)											/*start charging*/
#define AUTHM_UartSendReqChargeCmd()       		    STD_TRUE										/*send request start charge command*/
#define AUTHM_UartSendStopChargeCmd()   		    STD_TRUE										/*send request stop charge command*/

#define AUTHM_SAFETY_FLAG_ENTER						SAFETY_FLAG_ENTER								/*0x5EU*/

#define AUTHM_GetFotaStatus()						SYSM_GetSysStatusBit((uint32_t)((uint8_t)1<<(uint8_t)STD_SYSM_SYSSTATUS_PWR_APP_UPDATING))			/*get fota status*/
#define AUTHM_ClearFltInfo()						ERRHDL_ClearAuthInfor();MC003E1_ClearFaultCnt()						/*clear fault Infor*/
#define AUTHM_ResetEvseSelfCheckState()				EVSEM_ResetStartSelfCheckStatus() 				/*reset evse selfcheck state*/
#define AUTHM_GetSafetyStatus()	  					SAFETY_GetSafetyStatus()						/*get safety status*/
#define AUTHM_GetNoAuthStatus()						NOAUTHEN_GetAuthStatus()						/*get NoAuth status*/
#define AUTHM_GetBtFastAuthStatus()  				BTAUTH_GetBtFastAuthStatus()					/*get BtFast Auth status*/
#define AUTHM_GetBtAppCancelAuthStatus()			BTAUTH_GetAppCancelAuthStatus()					/*get BtApp Cancel Auth status*/
#define AUTHM_GetBtAppAuthStatus()					BTAUTH_GetAppAuthStatus()						/*get BtApp Auth status*/
#define AUTHM_GetNetAppAuthStatus()					NETAUTH_GetAuthStatus()							/*get Net  Auth status*/
#define AUTHM_GetNetAppCancelAuthStatus()			NETAUTH_GetCancelAuthStatus()					/*get Net Cancel Auth status*/
#define AUTHM_GetOrderContAuthStatus()				ORDER_GetAuthStatus()							/*get order continue Auth status*/
#define AUTHM_GetSingleTimeAuthStatus()				TIMEAUTH_GetOnceTimeAuthStatus()				/*get single time Auth status*/
#define AUTHM_GetSingleTimeCancelAuthStatus()		TIMEAUTH_GetOnceTimeCancelAuthStatus()			/*get single time Cancel Auth status*/
#define AUTHM_GetPriodTimeAuthStatus()				TIMEAUTH_GetPeriodTimeAuthStatus()				/*get period time Auth status*/
#define AUTHM_GetPriodTimeCancelAuthStatus()		TIMEAUTH_GetPeriodTimeCancelAuthStatus()		/*get period time Cancel Auth status*/
#define AUTHM_GetRfidAuthStatus() 					RFIDAUTH_GetCardAuthStatus()					/*get rfid Auth status*/
#define AUTHM_GetRfidCancelAuthStatus() 			RFIDAUTH_GetCardCancelAuthStatus()			    /*get rfid Cancel Auth status*/
#define AUTHM_ResetBtAppAuthStatus()  				BTAUTH_ResetBtAppAuthStatus()					/*reset BtApp Auth status*/
#define AUTHM_ResetNoAuthStatus() 					NOAUTHEN_ResetAuthStatus()						/*reset NoAuthen Auth status*/
#define AUTHM_ResetRfidAuthStatus()  				RFIDAUTH_ResetCardAuthStatus()					/*reset rfid Auth status*/
#define AUTHM_ResetNetAuthStatus()  				NETAUTH_ResetAuthStatus()						/*reset net Auth status*/
#endif


#if((AUTHM_CORE_TYPE == AUTHM_CORE_SALVE) || (AUTHM_CORE_TYPE == AUTHM_CORE_SINGLE))
#define AUTHM_STOP_CHARGE_ERROR_SUSPEND				EVSEM_STOP_CHARGE_ERROR_SUSPEND					/*stop charge reason is error suspend*/
#define AUTHM_STOP_CHARGE_REMOTE_SUSPEND			EVSEM_STOP_CHARGE_REMOTE_SUSPEND				/*stop charge reason is remote suspend*/

#define AUTHM_ReqEvseChargeOff()					EVSEM_ReqChargeOff()							/*request evse charge off*/
#define AUTHM_ReqEvseChargeOn()						EVSEM_ReqChargeOn()								/*request evse charge on*/
#define AUTHM_SetStopChargeReason(e)				EVSEM_SetChargeStopReason(e)					/*set stop charge reason*/
#define AUTHM_GetEmerStopStatus()					EMSTOPM_GetKeyValue()							/*get emergencystop status*/
#define AUTHM_GetEvseStatus()						EVSEM_GetChargeStatus()							/*get evse status*/
#define AUTHM_GetRemoteSuspendStatus()				CURR_GetRemoteSuspendStatus()					/*get suspend charge status*/
#endif

#define AUTHM_DEBUG(fmt, ...) 						LOG_DEBUG(LOG_MODULE_AUTHM, fmt, ##__VA_ARGS__)


/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
