//******************************************************************************
//* File Name: Cloud_Protocol_BillingModelM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_BillingModelM module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_BILLING_MODEL_M_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_BILLING_MODEL_M_H
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
extern void Cloud_Protocol_Start_BillingModelRequest(void);
extern void Cloud_Protocol_UpdateBillingModelRequest(void);
extern void Cloud_Protocol_FlashBillingModel(void);
extern void Cloud_Protocol_SetBillingModelNumber(uint16_t number);
extern uint16_t Cloud_Protocol_GetBillingModelNumber(void);
#endif /* __FILETEMPLATE_H */
/* EOL */
