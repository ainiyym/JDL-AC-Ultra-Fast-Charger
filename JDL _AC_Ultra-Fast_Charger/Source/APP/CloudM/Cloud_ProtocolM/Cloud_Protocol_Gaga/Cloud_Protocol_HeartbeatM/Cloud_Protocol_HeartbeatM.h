//******************************************************************************
//* File Name: Cloud_Protocol_HeartbeatM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_HeartbeatM module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_HEARTBEATM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_HEARTBEATM_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "Cloud_Protocol_Cfg.h"
#include "Cloud_Protocol_Session.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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
extern bool Cloud_Protocol_Get_HeartbeatIsNormal(void);
extern void Cloud_Protocol_Heartbeat_Handler(void);
extern void Cloud_Protocol_Start_Heartbeat(void);
extern void Cloud_Protocol_Set_HeartbeatResponse(void);
#endif /* __CLOUD_PROTOCOL_HEARTBEATM_H */
/* EOL */
