//******************************************************************************
//*
//* File Name: STD_SysM.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-04 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#ifndef STD_SYSM_H
#define STD_SYSM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "STD_SysM_Cfg.h"
#include "AppTask_CorePrintTask.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define STD_SYSM_SYSSTATUS_SAFTY_ENTER		(0X5A5A5A5A)	/* 安全态进入标志位 */
#define STD_SYSM_SYSSTATUS_SAFTY_EXIT		(0XA5A5A5A5)	/* 安全态退出标志位 */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum {
	STD_SYSM_SYSSTATUS_CHARGING,        	/* 系统状态字索引：桩是否在充电 0：否；1：是 */
	STD_SYSM_SYSSTATUS_CAN,        			/* 系统状态字索引：桩是否在CAN通信模式 0：否；1：是 */
	STD_SYSM_SYSSTATUS_SAFTYMODE,		 	/* 系统状态字索引：桩是否处于安全态 0：否；1：是 */
	STD_SYSM_SYSSTATUS_APP_UPDATING,		/* 系统状态字索引：桩是否在升级过程中 0：否；1：是 */
	STD_SYSM_SYSSTATUS_LOW_VOLTAGE,			/* 系统状态字索引：桩是否在处于低压状态 0：否；1：是 */
	STD_SYSM_SYSSTATUS_STANDBY,			    /* 系统状态字索引：桩是否是待机状态，0：否；1：是 */
	STD_SYSM_SYSSTATUS_EMERGENCY_STOP,		/* 系统状态字索引：墙盒是否是急停状态，0：否；1：是 */
}STD_SysM_SysStatus_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void SYSM_InitZero(void);
extern void SYSM_InitOne(void);
extern void SYSM_InitTwo(void);
extern void SYSM_InitThree(void);
extern void SYSM_SleepShutdown(void);
extern void SYSM_WakeupRestart( void );
extern void SYSM_10msMainFunction(void);
extern void SYSM_RunningLedHandle(void);

extern void SYSM_ImmediatelyResetManage(void);
extern void SYSM_SetCpVolMode(SysConnector_Num_Enum ch, uint8_t mode);
extern uint8_t SYSM_GetCpVolMode(SysConnector_Num_Enum ch);
extern uint8_t SYSM_GetResetPrepareStatus(void);
extern void SYSM_SetSysStatusBit(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode);
extern uint8_t SYSM_GetSysStatusBit(SysConnector_Num_Enum ch, uint32_t SysStatusMask);
extern uint32_t SYSM_GetSysStatus(SysConnector_Num_Enum ch);
extern uint8_t SYSM_CheckSysStatus(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode);
#endif
/*EOF*/
