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
#include "STD_Lib.h"
#include "Std_Types.h"
#include "STD_NoAuthen.h"
#include "STD_ErrorHandler.h"
#include "STD_EvseM.h"
#include "CpM_Cfg.h"
#include "CanM_Rte.h"
#include "dummy.h" 

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define AUTHM_OPEN_RFID_EN          				STD_OFF											/*rfid Auth open enable state*/
#define AUTHM_OPEN_BTAPP_EN          				STD_OFF											/*BTapp Auth open enable state*/
#define AUTHM_OPEN_NETAPP_EN          				STD_OFF											/*Netapp Auth open enable state*/
#define AUTHM_OPEN_SINGLE_TIMING_EN					STD_OFF											/*singletiming Auth open enable state*/
#define AUTHM_OPEN_PERIOD_TIMING_EN					STD_OFF											/*periodtiming Auth open enable state*/
#define AUTHM_OPEN_NOAUTH_EN						STD_ON											/*Rfid Auth open enable state*/
#define AUTHM_OPEN_BT_FAST_EN         	 			STD_OFF											/*BTapp Auth open enable state*/

#define AUTHM_CLOSE_EMER_EN 						STD_OFF											/*emergencystop close enable state*/
#define AUTHM_CLOSE_RFID_EN							STD_OFF											/*rfid Auth close enable state*/
#define AUTHM_CLOSE_BTAPP_EN						STD_OFF											/*BTapp Auth close enable state*/
#define AUTHM_CLOSE_NETAPP_EN						STD_OFF											/*Netapp Auth close enable state*/
#define AUTHM_CLOSE_SINGLE_TIMING_EN				STD_OFF											/*singletiming close enable state*/
#define AUTHM_CLOSE_PERIOD_TIMING_EN				STD_OFF											/*periodtiming close enable state*/
#define AUTHM_CLOSE_BUTTON_EN						STD_OFF											/*Button close enable state*/

#define AUTHM_ERRHDL_CHARGE_ALLOW					ERRHDL_CHARGE_ALLOW								/*0U*/
#define AUTHM_ERRHDL_CHARGE_DERATE					ERRHDL_CHARGE_DERATE							/*1U*/
#define AUTHM_ERRHDL_CHARGE_SUSPENDED				ERRHDL_CHARGE_SUSPENDED							/*2U*/
#define AUTHM_ERRHDL_CHARGE_CANCEL					ERRHDL_CHARGE_CANCEL							/*3U*/
#define AUTHM_ERRHDL_CHARGE_SAFETY					ERRHDL_CHARGE_SAFETY							/*4U*/

#define AUTHM_TASK_PERIOD                			(10U)											/*10ms*/
#define AUTHM_NOT_PLUG_IN_TIMEOUT_CNT  				((uint32_t)2*60*1000u / AUTHM_TASK_PERIOD )		/*2 minutes*/

#define AUTHM_GetChargeConditions() 				ERRHDL_GetChargeConditions()					/*get charge conditions*/
#define AUTHM_GetResetPrepareStatus()				SYSM_GetResetPrepareStatus()					/*get system prepare status*/

#define AUTHM_EVSE_STATUS_3_DOT						(uint8_t)EVSEM_STATE_THREE_dot					/*6U*/

#define AUTHM_CP_VOL_STATUS_INVALID					CPV_VOLT_ZERO							        /*0U*/
#define AUTHM_CP_VOL_STATUS_12V						CPV_VOLT_TWF								    /*1U*/
#define AUTHM_CP_VOL_STATUS_9V						CPV_VOLT_NINE								    /*2U*/
#define AUTHM_CP_VOL_STATUS_6V						CPV_VOLT_SIX								    /*3U*/
#define AUTHM_CP_VOL_STATUS_4V                      CPV_VOLT_FOUR								    /*4U*/
#define AUTHM_CP_VOL_STATUS_3V						CPV_VOLT_THREE								    /*5U*/
#define AUTHM_CP_VOL_STATUS_2V						CPV_VOLT_TWEO                                   /*6U*/

#define AUTHM_GetEvseStatus(ch)			    		EVSEM_GetChargeStatus(ch)						/*get evse status*/
#define AUTHM_GetCpStatus(ch)	 					CPM_GetCpVoltStatus(ch)							/*get cp status*/
#define AUTHM_GetEmerStopStatus(ch)	       		 	SYSM_GetSysStatusBit(ch, (uint32_t)STD_SYSM_SYSSTATUS_EMERGENCY_STOP)           /*get emergencystop status*/
#define AUTHM_GetFotaStatus(ch)						SYSM_GetSysStatusBit(ch, (uint32_t)STD_SYSM_SYSSTATUS_APP_UPDATING)		        /*get fota status*/
#define AUTHM_GetSafetyStatus(ch)	  				SYSM_GetSysStatusBit(ch, (uint32_t)(STD_SYSM_SYSSTATUS_SAFTYMODE)) 	            /*get safety status*/
#define AUTHM_SetAuthStatus(ch, mode)               SYSM_SetSysStatusBit(ch, (uint32_t)STD_SYSM_SYSSTATUS_AUTHORIZATION, mode)      /*set auth status*/
#define AUTHM_ClearFltInfo()						ERRHDL_ClearAuthInfor()                         /*clear fault Infor*/
#define AUTHM_ResetEvseSelfCheckState(ch)			EVSEM_ResetStartSelfCheckStatus(ch) 			/*reset evse selfcheck state*/
#define AUTHM_GetNoAuthStatus(ch)					NOAUTHEN_GetAuthStatus(ch)						/*get NoAuth status*/
#define AUTHM_GetBtFastAuthStatus()  				                            					/*get BtFast Auth status*/
#define AUTHM_GetBtAppCancelAuthStatus()                                                            /*get BtApp Cancel Auth status*/
#define AUTHM_GetBtAppAuthStatus()                                                                  /*get BtApp Auth status*/
#define AUTHM_GetNetAppAuthStatus()                                                                 /*get Net  Auth status*/
#define AUTHM_GetNetAppCancelAuthStatus()                                                           /*get Net Cancel Auth status*/
#define AUTHM_GetOrderContAuthStatus()                                                              /*get order continue Auth status*/
#define AUTHM_GetSingleTimeAuthStatus()                                                             /*get single time Auth status*/
#define AUTHM_GetSingleTimeCancelAuthStatus()                                                       /*get single time Cancel Auth status*/
#define AUTHM_GetPriodTimeAuthStatus()                                                              /*get period time Auth status*/
#define AUTHM_GetPriodTimeCancelAuthStatus()                                                        /*get period time Cancel Auth status*/
#define AUTHM_GetRfidAuthStatus()                                                                   /*get rfid Auth status*/
#define AUTHM_GetRfidCancelAuthStatus()                                                             /*get rfid Cancel Auth status*/
#define AUTHM_ResetBtAppAuthStatus()                                                                /*reset BtApp Auth status*/
#define AUTHM_ResetNoAuthStatus(ch)                 NOAUTHEN_ResetAuthStatus(ch)                    /*reset NoAuthen Auth status*/
#define AUTHM_ResetRfidAuthStatus()                                                                 /*reset rfid Auth status*/
#define AUTHM_ResetNetAuthStatus()                                                                  /*reset net Auth status*/

#define AUTHM_STOP_CHARGE_ERROR_SUSPEND				EVSEM_STOP_CHARGE_ERROR_SUSPEND					/*stop charge reason is error suspend*/
#define AUTHM_STOP_CHARGE_REMOTE_SUSPEND			EVSEM_STOP_CHARGE_REMOTE_SUSPEND				/*stop charge reason is remote suspend*/

#define AUTHM_ReqEvseChargeOff(ch)					EVSEM_ReqChargeOff(ch);CanM_Rte_Set_AuthM_AuthStatus(ch,0)							/*request evse charge off*/
#define AUTHM_ReqEvseChargeOn(ch)					EVSEM_ReqChargeOn(ch);CanM_Rte_Set_AuthM_AuthStatus(ch,1)							/*request evse charge on*/
#define AUTHM_SetStopChargeReason(ch,e)				EVSEM_SetChargeStopReason(ch,e)					/*set stop charge reason*/
#define AUTHM_GetRemoteSuspendStatus()				Dummy_RemoteSuspendStatus()                     /*get suspend charge status*/

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
