//****************************************************************************************
//*
//* File Name: CanM_Rte.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _CANM_RTE_H
#define _CANM_RTE_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_SysM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|****************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    LOWER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    UPPER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    Callback Notification
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    SECC_MSG2_FAULT_ID_ENERGYT_CANNOT_TRANS,
    SECC_MSG2_FAULT_ID_OVER_CURR,
    SECC_MSG2_FAULT_ID_VOLT,
    SECC_MSG2_FAULT_ID_EMERGESTOP,
    SECC_MSG2_FAULT_ID_RELAY,
    SECC_MSG2_FAULT_ID_HARDWARE,
    SECC_MSG2_FAULT_ID_OVERTEMP,
    SECC_MSG2_FAULT_ID_CP_VOL,
    SECC_MSG2_FAULT_ID_CHARGING_PARAMETER,
    SECC_MSG2_FAULT_ID_SELFTEST_FAIL,
    SECC_MSG2_FAULT_ID_CAN_TIME_OUT,
    SECC_MSG2_FAULT_ID_OTHER
}CanM_Rte_SeccMsg2Fault_Enum;

typedef enum
{
    CANM_RTE_CHARGING_END_REASON_NONE = 0,  /* No stoping charging */
    CANM_RTE_CHARGING_END_REASON_NORMAL,    /* Normal end of charging */
    CANM_RTE_CHARGING_END_REASON_MANUAL,    /* Manual stop of charging */
    CANM_RTE_CHARGING_END_REASON_EVSE_FAULT,/* Evse fault stop of charging */
    CANM_RTE_CHARGING_END_REASON_VEHICLE,   /* Vehicle stopped the charging */
    CANM_RTE_CHARGING_END_REASON_RESERVE    /* Reserve */
} CanM_Rte_ChargingEndReason_Enum;

typedef enum
{
    CANM_RTE_CHARGING_START_MODE_NONE = 0,  /* No start charging */
    CANM_RTE_CHARGING_START_MODE_G2V,       /* G2V charging start mode */
    CANM_RTE_CHARGING_START_MODE_V2G,       /* V2G charging start mode */
    CANM_RTE_CHARGING_START_MODE_RESERVE    /* Reserve */
} CanM_Rte_ChargingStartMode_Enum;

typedef enum
{
    CANM_RTE_RELAY_OFF = 0, /* Relay is off */
    CANM_RTE_RELAY_ON,      /* Relay is on */
    CANM_RTE_RELAY_ERROR,   /* Relay error */
    CANM_RTE_RELAY_RESERVE  /* reserve */
} CanM_Rte_RelayStatus_Enum;

typedef enum
{
    CANM_RTE_EVSE_SYS_STATUS_POWER_LOWER = 0,           /* lower power on */
    CANM_RTE_EVSE_SYS_STATUS_READY_POWER_HIGH,          /* Prepere high power on */
    CANM_RTE_EVSE_SYS_STATUS_CHARGING_NORMAL,           /* Charging normal mode */
    CANM_RTE_EVSE_SYS_STATUS_CHARGING_CONSTANT_VOLTAGE, /* Charging constant voltage mode */
    CANM_RTE_EVSE_SYS_STATUS_CHARGING_CONSTANT_CURRENT, /* Charging constant current mode */
    CANM_RTE_EVSE_SYS_STATUS_CHARGING_END,              /* Charging end */
    CANM_RTE_EVSE_SYS_STATUS_RESERVE                    /* reserve */
} CanM_Rte_EvseSysStatus_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void CanM_Rte_SetCarComStatus(SysConnector_Num_Enum ch, uint8_t Status);
extern void CanM_Rte_Set_AuthM_AuthStatus(SysConnector_Num_Enum ch, uint8_t status);
extern void CanM_Rte_Set_SECC_MSG2_InputFaultStatus(SysConnector_Num_Enum ch, CanM_Rte_SeccMsg2Fault_Enum FaultID, uint8_t status);
extern uint8_t CanM_Rte_GetCarComEndStatus(SysConnector_Num_Enum ch);
extern void CanM_Rte_Evse_Init(void);
extern void CanM_Rte_Msg_Init(void);
extern void CanM_Rte_EVSE_Main_Task(void);
extern void CanM_Rte_Msg_Main_Task(void);
extern void CanM_Rte_10ms_Task(void);
#endif
/*EOF*/ 
