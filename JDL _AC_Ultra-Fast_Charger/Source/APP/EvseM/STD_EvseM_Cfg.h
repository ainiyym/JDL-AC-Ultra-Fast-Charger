//****************************************************************************************
//*
//* File Name: STD_EvseM_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _EVSEM_CFG_H
#define _EVSEM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "STD_LogService.h"
#include "SwitchM.h"
#include "CanM_Rte.h"
#include "CpM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|***************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define EVSEM_RELAY_CHECKSELF_ENABLE  				    			STD_OFF
#define EVSEM_DIODE_CHECKSELF_ENABLE  				    			STD_OFF
#define EVSEM_RCD_CHECKSELF_ENABLE  				    			STD_OFF

#define EVSEM_TASK_PERIOD          								    (10u)	/* 10ms */
#define EVSEM_S2OFF_TIMEOUT_CNT									   	(uint16_t)(200u / EVSEM_TASK_PERIOD)				/* 0.2s   */
#define EVSEM_CP_FILTER_MAX_CNT									   	(uint16_t)(1500u / EVSEM_TASK_PERIOD)				/* 1.5s   */
#define EVSEM_S2OFF_DLY_CNT											(uint16_t)(1000u / EVSEM_TASK_PERIOD)				/* 1s   */
#define EVSEM_STATUS3_DLY_CNT  										(uint16_t)(6000u / EVSEM_TASK_PERIOD )			    /* 6s   */
#define EVSEM_SELFCHECK_DLY_CNT										(uint16_t)(900u / EVSEM_TASK_PERIOD)				/* 90ms */
#define EVSEM_STATUS2_DLY_CNT               			            (uint16_t)(5000u / EVSEM_TASK_PERIOD )	            /* 5s   */
#define EVSEM_SELFCHECK_TIMEOUT_CNT								    (uint16_t)(8000u / EVSEM_TASK_PERIOD )	            /* 8s   */

#define EVSEM_SET_CP_OUT_12V(ch)									SwitchM_SetCpVol12vMode(ch)					            /*set cp voltage 12v*/
#define EVSEM_SET_CP_OUT_4V(ch)										SwitchM_SetCpVol4vMode(ch)					            /*set cp voltage 4v*/
#define EVSEM_SET_CP_MOS_STATUS(ch, mode)						    SwitchM_SetCpMosStatus(ch, mode)				        /*set cp mos status*/

#define EVSEM_SET_CAN_START_COM(ch)									CanM_Rte_SetCarComStart(ch)
#define EVSEM_GET_CAN_END_COM(ch)									CanM_Rte_GetCarComEndStatus(ch)

#if (EVSEM_RELAY_CHECKSELF_ENABLE == STD_ON)                                                                    /*relay checkself enable*/
#define EVSEM_RLY_SELFCHECK_PROCESS								    (0u)                                        /*process of self-checking*/
#define EVSEM_RLY_SELFCHECK_SUCCESS								    (1u)                                        /*checkself sucessed*/
#define EVSEM_RLY_SELFCHECK_FAILED								    (2u)									    /*checkself failed*/
#define EVSEM_ReqRelaySelfCheck(ch)						 			RELAYM_ReqRelaySelfCheck(ch)        		/*request checkself*/
#define EVSEM_GetRelaySelfCheckStatus(ch)			    	        RELAYM_GetRelaySelfCheckStatus(ch)  		/*get checkself status*/
#endif
 
#if (EVSEM_RCD_CHECKSELF_ENABLE == STD_ON)
#define EVSEM_RCD_MONITOR_SUCCESS 						        MC00XXX_SELF_SUCCESS          	/*checkself sucessed*/
#define EVSEM_RCD_MONITOR_FAILED  						        MC00XXX_SELF_FAILED				/*checkself failed*/
#define EVSEM_RCD_MONITOR_PROCESS 						        MC00XXX_SELF_PROCESS            /*process of self-checking*/
#define EVSEM_ReqRCDSelfCheck(ch)						        RCDM_ReqRCDSelfCheck(ch)		/*request checkself*/
#define EVSEM_GetRCDSelfCheckStatus(ch)				            RCDM_GetSelfCheckResult(ch)		/*get checkself status*/
#endif

#define EVSEM_GetCpStatus(ch)								    CPM_GetCpVoltStatus(ch)                 /*get cp voltage status*/
#define EVSEM_StartCpOutput(ch)							        CPM_OutputEnable(ch)					/*cp output open*/
#define EVSEM_StopCpOutput(ch)							        CPM_OutputDisable(ch)					/*cp output close*/
#define EVSEM_CpOutputNegative12V(ch)				            CPM_OutputNegative12VEnable(ch)	 		/* cp output -12V */
#define EVSEM_CpStopOutputNegative12V(ch)                       CPM_OutputNegative12VDisable(ch)        /* cp stop output -12V */
#define EVSEM_SetRelayOff(ch)							        RELAYM_ReqRelaySwitchOff(ch)			/*request relay turn off*/
#define EVSEM_SetRelayOn(ch)							        RELAYM_ReqRelaySwitchOn(ch)				/*request relay turn on*/
#define EVSEM_GetRelayStatus(ch)						        RELAYM_GetRelayStatus(ch)				/*get relay status*/
#define EVSEM_GetSysReadyState()				                SYSM_GetResetPrepareStatus()              	/*get sys status*/
#define EVSEM_GetChargeConditions()				                ERRHDL_GetChargeConditions()			/*get Charge Conditions*/

#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
#define EVSEM_ReqDiodeSelfCheck(ch)						        CPM_ReqCarDiodeDetect(ch)					/*request diode selfcheck*/
#define EVSEM_GetDiodeSelfCheckStatus(ch)			            CPM_CarDiodeDetectResult(ch)				/*get diode selfcheck result*/
#endif

#define EVSEM_CP_NULL                     				        CPV_VOLT_ZERO							/*(0U)*/
#define EVSEM_CP_12V                     					    CPV_VOLT_TWF 							/*(1U)*/
#define EVSEM_CP_9V                      					    CPV_VOLT_NINE							/*(2U)*/
#define EVSEM_CP_6V                  							CPV_VOLT_SIX							/*(3U)*/
#define EVSEM_CP_4V                  							CPV_VOLT_FOUR							/*(4U)*/
#define EVSEM_CP_3V                  							CPV_VOLT_THREE						    /*(5U)*/
#define EVSEM_CP_2V                  							CPV_VOLT_TWEO							/*(6U)*/

#define EVSEM_RLYCTRL_STATE_ON                  	            RLYCTRL_STATE_ON					    /*(1U)*/

#if (EVSEM_DIODE_CHECKSELF_ENABLE == STD_ON)
#define EVSEM_DIODE_SELFCHECK_SUCCESS 					        CPD_DIODE_SELF_SUCCESS				/*diode selfcheck successful*/
#define EVSEM_DIODE_SELFCHECK_FAILED  					        CPD_DIODE_SELF_FAILED					/*diode selfcheck failed*/
#define EVSEM_DIODE_SELFCHECK_PROCESS 					        CPD_DIODE_SELF_PROCESS				/*diode selfcheck in progress*/
#endif

#define EVSEM_CHARGE_ALLOW										ERRHDL_CHARGE_ALLOW					    /*(0U)*/
#define EVSEM_CHARGE_DERATE										ERRHDL_CHARGE_DERATE					/*(1U)*/
#define EVSEM_CHARGE_SUSPENDED								    ERRHDL_CHARGE_SUSPENDED				    /*(2U)*/
#define EVSEM_CHARGE_CANCEL										ERRHDL_CHARGE_CANCEL					/*(3U)*/
#define EVSEM_CHARGE_SAFETY										ERRHDL_CHARGE_SAFETY					/*(4U)*/

#define EVSEM_DEBUG(fmt, ...) 							        LOG_DEBUG(LOG_MODULE_EVSEM, fmt, ##__VA_ARGS__)
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
