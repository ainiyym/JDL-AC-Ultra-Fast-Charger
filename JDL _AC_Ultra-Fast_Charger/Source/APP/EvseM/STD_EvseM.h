//****************************************************************************************
//*
//* File Name: STD_EvseM.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _EVSEM_H
#define _EVSEM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_EvseM_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define EVSEM_STATE_ZERO                   		(0U)		/*evse state is 0*/
#define EVSEM_STATE_ONE                     	(1U)		/*evse state is 1*/
#define EVSEM_STATE_ONE_dot                 	(2U)		/*evse state is 1'*/
#define EVSEM_STATE_TWO                     	(3U)		/*evse state is 2*/
#define EVSEM_STATE_TWO_dot                 	(4U)		/*evse state is 2'*/
#define EVSEM_STATE_THREE                   	(5U)		/*evse state is 3*/
#define EVSEM_STATE_THREE_dot               	(6U)		/*evse state is 3'*/
#define EVSEM_STATE_CAN_MODEL               	(7U)		/* switch can model */

#define EVSEM_STOP_CHARGE_NONE                  (0U)		/*stop charge reason is none*/
#define EVSEM_STOP_CHARGE_NORMAL                (1U)		/*stop charge reason is normal*/
#define EVSEM_STOP_CHARGE_ERROR_END             (2U)		/*stop charge reason is error*/
#define EVSEM_STOP_CHARGE_S2_OFF                (3U)		/*stop charge reason is S2 off*/
#define EVSEM_STOP_CHARGE_CP_OFF                (4U)		/*stop charge reason is CP off*/
#define EVSEM_STOP_CHARGE_ERROR_SUSPEND         (5U)		/*stop charge reason is error suspend*/
#define EVSEM_STOP_CHARGE_REMOTE_SUSPEND       	(6U)		/*stop charge reason is remote suspend*/

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

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void EVSEM_InitMemory(void);
extern void EVSEM_ResetStartSelfCheckStatus(SysConnector_Num_Enum ch);
extern void EVSEM_ReqChargeOn(SysConnector_Num_Enum ch);
extern void EVSEM_ReqChargeOff(SysConnector_Num_Enum ch);
extern void EVSEM_10msMainFunction(void);
extern uint8_t EVSEM_GetChargeStatus(SysConnector_Num_Enum ch);
extern uint8_t EVSEM_GetDiodeCheckResult(SysConnector_Num_Enum ch);
extern uint8_t EVSEM_GetChargeStopReason(SysConnector_Num_Enum ch);
extern void EVSEM_SetChargeStopReason(uint8_t lv_ucReson);
extern void EVSEM_EnterStateZero(SysConnector_Num_Enum ch);
#endif
/*EOF*/
