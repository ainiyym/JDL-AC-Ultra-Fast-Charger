//******************************************************************************
//* File Name: Cloud_Protocol_GagaM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_GagaM module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_GAGAM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_GAGAM_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_HeartbeatM.h"
#include "Cloud_Protocol_BillingModelM.h"
#include "Cloud_Protocol_GagaM_Cfg.h"

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
extern void Cloud_GagaProtocol_Init(void);
extern void Cloud_GagaProtocol_DeviceResetChecking(void);
extern void Cloud_GagaProtocol_MainProcess(void);
#endif /* __CLOUD_PROTOCOL_GAGAM_H */
/* EOL */
