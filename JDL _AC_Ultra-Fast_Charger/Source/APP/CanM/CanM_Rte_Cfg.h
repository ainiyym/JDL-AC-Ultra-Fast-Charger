//****************************************************************************************
//*
//* File Name: CanM_Rte_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _CANM_RTE_CFG_H
#define _CANM_RTE_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_SysM_Cfg.h"
#include "CanM_EVSEM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CANM_RTE_SELFTEST_NUM                                       (3U) /* Number of self-test requests */
#define CANM_RTE_RELAY_CHECKSELF_ENABLE  					        STD_OFF
#define CANM_RTE_DIODE_CHECKSELF_ENABLE  					        STD_OFF
#define CANM_RTE_RCD_CHECKSELF_ENABLE  					            STD_OFF

#define CANM_RTE_REQ_SELFTEST_ENABLE                                (ENABLE_STATUS_ENUM_ENABLE)                 /* Enable status */
#define CANM_RTE_REQ_SELFTEST_DISABLE                               (ENABLE_STATUS_ENUM_DISABLE)                /* Disable status */
#define CANM_RTE_SELFTEST_SUCCESS                                   (CHARGING_BEFORE_TEST_STATUS_ENUM_SUCCESS)  /* Self-test success status */
#define CANM_RTE_SELFTEST_FAILURE                                   (CHARGING_BEFORE_TEST_STATUS_ENUM_FAIL)     /* Self-test failure status */
#if (CANM_RTE_RELAY_CHECKSELF_ENABLE == STD_ON)                                                                 /*relay checkself enable*/
#define CANM_RTE_RLY_SELFCHECK_PROCESS								(0u)                                        /*process of self-checking*/
#define CANM_RTE_RLY_SELFCHECK_SUCCESS								(1u)                                        /*checkself sucessed*/
#define CANM_RTE_RLY_SELFCHECK_FAILED								(2u)									    /*checkself failed*/
#define CANM_RTE_ReqRelaySelfCheck(ch)						 	    RELAYM_ReqRelaySelfCheck(ch)        		/*request checkself*/
#define CANM_RTE_GetRelaySelfCheckStatus(ch)			    	    RELAYM_GetRelaySelfCheckStatus(ch)  		/*get checkself status*/
#endif
 
#if (CANM_RTE_RCD_CHECKSELF_ENABLE == STD_ON)
#define CANM_RTE_RCD_MONITOR_SUCCESS 						        MC00XXX_SELF_SUCCESS          	            /*checkself sucessed*/
#define CANM_RTE_RCD_MONITOR_FAILED  						        MC00XXX_SELF_FAILED				            /*checkself failed*/
#define CANM_RTE_RCD_MONITOR_PROCESS 						        MC00XXX_SELF_PROCESS                        /*process of self-checking*/
#define CANM_RTE_ReqRCDSelfCheck(ch)						        RCDM_ReqRCDSelfCheck(ch)		            /*request checkself*/
#define CANM_RTE_GetRCDSelfCheckStatus(ch)				            RCDM_GetSelfCheckResult(ch)		            /*get checkself status*/
#endif

#if (CANM_RTE_DIODE_CHECKSELF_ENABLE == STD_ON)
#define CANM_RTE_ReqDiodeSelfCheck(ch)						        CPM_ReqCarDiodeDetect(ch)					/*request diode selfcheck*/
#define CANM_RTE_GetDiodeSelfCheckStatus(ch)			            CPM_CarDiodeDetectResult(ch)				/*get diode selfcheck result*/
#endif

#define CanM_Rte_GetChargeConditions()                              ERRHDL_GetChargeConditions()                /*get Charge Conditions*/

#define CanM_Rte_SetRelayOff(ch)							        RELAYM_ReqRelaySwitchOff(ch)			    /*request relay turn off*/
#define CanM_Rte_SetRelayOn(ch)							            RELAYM_ReqRelaySwitchOn(ch)				    /*request relay turn on*/
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
extern void CanM_Set_McuStatus3ValidStatus(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_EVSE_CanMode(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_MCU_State3ReqChargingEnable(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_MCU_State3ValidStatus(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_ErrHdl_ChargingStation(SysConnector_Num_Enum connector, uint8_t status);
extern void CanM_Set_AuthM_AuthStatus(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_BeforeChargingTestResult(SysConnector_Num_Enum connector, uint8_t result);
extern uint8_t CanM_GetChargingCanFaultStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_ReqSelfTestStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_ReqRelayOnStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_CanModeStatus(SysConnector_Num_Enum connector);
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
