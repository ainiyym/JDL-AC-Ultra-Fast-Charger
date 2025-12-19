//******************************************************************************
//* File Name: Cloud_Protocol_Sg_BillingMode.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Billing Mode module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_BILLINGMODE_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_BILLINGMODE_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_DataDef.h"
#include "Cloud_Protocol_Sg_Cfg.h"
#include "STD_SysM.h"
#include "string.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* Time segment flag definitions */
#define CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK         1       // Peak segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH         2       // High segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL       3       // Normal segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY       4       // Valley segment

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
extern bool Cloud_Protocol_Sg_ParseUpdateBillingModeParam(cJSON *params, const char *msg_id);
extern uint8_t Cloud_Protocol_Sg_GetCurrentSegFlag(uint8_t hour, uint8_t minute);
extern float Cloud_Protocol_Sg_GetCurrentChargeFee(uint8_t seg_flag);
extern float Cloud_Protocol_Sg_GetCurrentServiceFee(uint8_t seg_flag);
extern void Cloud_Protocol_Sg_PrintBillingMode(void);
extern v2g_service_issue_feeModel *Cloud_Protocol_Sg_GetBillingModePtr(void);
extern void Cloud_Protocol_Sg_SetBillingModePtr(v2g_service_issue_feeModel *billing_mode);
extern bool Cloud_Protocol_Sg_IsBillingModeInitialized(void);

#endif /* __CLOUD_PROTOCOL_SG_BILLINGMODE_H */
