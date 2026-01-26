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
#include "CanM_MsgM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"
#include "STD_Curr.h"
#include "STD_Volt.h"
#include "STD_AuthM.h"
#include "SwitchM_Cfg.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CANM_RTE_SELFTEST_NUM                                       (3U) /* Number of self-test requests */
#define CANM_RTE_SINGLE_FRAME_LEN                                   (8U) /* CAN single-frame length */
#define CANM_RTE_RELAY_CHECKSELF_ENABLE  					        STD_OFF
#define CANM_RTE_DIODE_CHECKSELF_ENABLE  					        STD_OFF
#define CANM_RTE_RCD_CHECKSELF_ENABLE  					            STD_OFF

#define CANM_RTE_REQ_SELFTEST_ENABLE                                (ENABLE_STATUS_ENUM_ENABLE)                 /* Enable status */
#define CANM_RTE_REQ_SELFTEST_DISABLE                               (ENABLE_STATUS_ENUM_DISABLE)                /* Disable status */
#define CANM_RTE_SELFTEST_SUCCESS                                   (CHARGING_BEFORE_TEST_STATUS_ENUM_SUCCESS)  /* Self-test success status */
#define CANM_RTE_SELFTEST_FAILURE                                   (CHARGING_BEFORE_TEST_STATUS_ENUM_FAIL)     /* Self-test failure status */

#define CANM_RTE_RLYCTRL_STATE_MAX                                  (RLYCTRL_STATE_MAX)                         /* relay status Map max*/
#define CANM_RTE_CLOSE_SRC_MAX                                      (AUTHM_CLOSE_SRC_MAX)                       /* stop charging reason Map max*/

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

#define CanM_Rte_SetNoAuthStatus(ch)                                AUTHM_AppSetReqChargeStatus(ch)             /* Noauth req charging  */
#define CanM_Rte_CancelNoAuthStatus(ch)                             AUTHM_AppResetReqChargeStatus(ch)           /* Reset Noauth req charging status */
#define CanM_Rte_GetReqChargeStatus(ch)                             AUTHM_GetReqChargeStatus(ch)                /* get authm req charging status */
#define CanM_Rte_GetAuthCloseSource(ch)                             AUTHM_GetAuthCloseSource(ch)                /* get authm stop auth source */

#define CanM_Rte_GetChargeConditions(ch)                            ERRHDL_GetChargeConditions(ch)                /*get Charge Conditions*/

#define CanM_Rte_SetRelayOff(ch)							        RELAYM_ReqRelaySwitchOff(ch)			    /*request relay turn off*/
#define CanM_Rte_SetRelayOn(ch)							            RELAYM_ReqRelaySwitchOn(ch)				    /*request relay turn on*/
#define CanM_Rte_GetRelayStatus(ch)                                 RELAYM_GetRelayStatus(ch)                   /* get relay status */

#define CanM_Rte_GetL1CUrr(ch)                                      CURR_GetL1Value(ch)                         /* get L1 current value */
#define CanM_Rte_GetL2CUrr(ch)                                      CURR_GetL2Value(ch)                         /* get L2 current value */
#define CanM_Rte_GetL3CUrr(ch)                                      CURR_GetL3Value(ch)                         /* get L3 current value */
#define CanM_Rte_GetL1Volt(ch)                                      VOLT_GetL1VoltValue(ch)                     /* get L1 voltage value */
#define CanM_Rte_GetL2Volt(ch)                                      VOLT_GetL2VoltValue(ch)                     /* get L2 voltage value */
#define CanM_Rte_GetL3Volt(ch)                                      VOLT_GetL3VoltValue(ch)                     /* get L3 voltage value */

#define CanM_Rte_GetCanModeStatus(ch, SysStatusMask)                SYSM_GetSysStatusBit(ch, SysStatusMask)

#define CANM_DEBUG(fmt, ...) 						                LOG_DEBUG(LOG_MODULE_CAN, fmt, ##__VA_ARGS__)
#define CANM_ERR(fmt, ...) 						                    LOG_ERROR(LOG_MODULE_CAN, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t ChargingEndReason;            /* Charging end reason */
    uint8_t ChargingStartMode;            /* Charging start mode */
    uint8_t RelayStatus;                  /* Relay status */
    uint8_t EvseSysStatus;                /* EVSE system status */
    uint8_t EnergyTransferUnable;         /* Energy transfer unable status */
    uint8_t OverCurrFaultStatus;          /* Over current fault status */
    uint8_t VoltFaultStatus;              /* voltage fault status */
    uint8_t EmergeStopFaultStatus;        /* Low voltage fault status */
    uint8_t RelayFaultStatus;             /* Relay fault status */
    uint8_t HardwareFaultStatus;          /* Hardware fault status */
    uint8_t OverTempFaultStatus;          /* Charging CAN fault status */
    uint8_t CpVolFaultStatus;             /* CP voltage fault status */
    uint8_t ChargingParameterFaultStatus; /* Charging parameter fault status */
    uint8_t SelfTestFaultStatus;          /* Self-test fault status */
    uint8_t CanTimeOutFaultStatus;        /* CAN timeout fault status */
    uint8_t OtherFaultStatus;             /* Other fault status */
    uint16_t RatedCurr;                   /* Rated current */
    uint16_t RatedVolt;                   /* Rated voltage */
} CanM_SECC_MSG2_Input_Struct;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
/* CanM_EvseM */
extern void CanM_Set_EVSE_CanMode(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_MCU_State3ReqChargingEnable(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_EVSE_CanHeartBeatStatus(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_ErrHdl_ChargingStation(SysConnector_Num_Enum connector, uint8_t status);
extern void CanM_Set_AuthM_AuthStatus(SysConnector_Num_Enum connector, boolean_T status);
extern void CanM_Set_BeforeChargingTestResult(SysConnector_Num_Enum connector, uint8_t result);
extern uint8_t CanM_GetChargingCanFaultStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_ReqSelfTestStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_ReqRelayOnStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_CanModeStatus(SysConnector_Num_Enum connector);
/* CanM_MsgM */
extern void CanM_Set_MCU_MSG_Enable(SysConnector_Num_Enum connector, uint8_t Status);
extern uint8_t CanM_Set_MCU_Status3_Input(SysConnector_Num_Enum connector, uint8_t *MCUData, uint8_t DataLen);
extern void CanM_Set_SECC_MSG_Enable(SysConnector_Num_Enum connector, uint8_t Status);
extern void CanM_Set_SECC_MSG1_Input(SysConnector_Num_Enum connector);
extern void CanM_Set_SECC_MSG2_Input(SysConnector_Num_Enum connector, CanM_SECC_MSG2_Input_Struct Msg2Input);
uint8_t CanM_Get_SECC_MSG_Enable_Status(SysConnector_Num_Enum connector);
extern uint64_t CanM_Get_SECC_MSG1_Output(SysConnector_Num_Enum connector);
extern uint64_t CanM_Get_SECC_MSG2_Output(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_McuState3ReqChargingEnableStatus(SysConnector_Num_Enum connector);
extern uint8_t CanM_Get_McuState3HeartBeatStatus(SysConnector_Num_Enum connector);
#endif
/*EOF*/
