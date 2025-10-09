//******************************************************************************
//* File Name: Cloud_Protocol.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Cfg.h"
#include "Cloud_Protocol_CallbackFunc.h"

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
extern void Cloud_Protocol_AckDeviceStatus(cloud_device_status_e status);
extern void Cloud_Protocol_AckHeartbeatParam(bool status);
extern void Cloud_Protocol_AckRegpkgParam(bool status);
extern void Cloud_Protocol_AckWakeUpDTU(bool status);
extern void Cloud_Protocol_Start_Heartbeat(void);
extern void Cloud_Protocol_Init(void);
extern void Cloud_Protocol_Main(void);
#endif /* __CLOUD_PROTOCOL_H */
/* EOL */
