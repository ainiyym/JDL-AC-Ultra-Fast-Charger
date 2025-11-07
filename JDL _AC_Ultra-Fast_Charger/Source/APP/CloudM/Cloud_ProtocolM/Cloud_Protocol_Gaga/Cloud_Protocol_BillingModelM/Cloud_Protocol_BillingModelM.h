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
#include "Mcal_BigLittle_Endian.h"

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
typedef struct
{
	uint32_t electricity_rate; // Electricity rate (accurate to five decimal places)
	uint32_t service_rate;	   // Service fee rate (accurate to five decimal places)
} cloud_protocol_billing_rate_t;

typedef struct
{
	cloud_protocol_billing_rate_t peak_rate;   // on-peak rate
	cloud_protocol_billing_rate_t high_rate;   // Peak rate
	cloud_protocol_billing_rate_t normal_rate; // Off-peak rate
	cloud_protocol_billing_rate_t valley_rate; // Valley rate
	uint8_t loss_ratio;						   // Loss-making ratio
	uint8_t time_slot_rates[48];			   // The rate numbers for 48 time periods
} cloud_protocol_billing_time_slot_t;

typedef struct
{
	// Add relevant fields for billing model management
	bool is_billing_model_updated;
	uint16_t billing_model_number;
    cloud_protocol_billing_time_slot_t time_slot_info;
} cloud_protocol_billing_model_manager_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Cloud_Protocol_Start_BillingModelRequest(void);
extern void Cloud_Protocol_UpdateBillingModelRequest(void);
extern void Cloud_Protocol_SetBillingModelFlag(void);
extern void Cloud_Protocol_UpdateBillingModelTimeSlotInfo(cloud_protocol_billing_time_slot_t info);
extern cloud_protocol_billing_model_manager_t Cloud_Protocol_GetBillingModelInfo(void);
extern cloud_protocol_billing_time_slot_t Cloud_Protocol_GetBillingModelTimeSlotInfo(void);
extern void Cloud_Protocol_SetBillingModelNumber(uint16_t number);
extern uint16_t Cloud_Protocol_GetBillingModelNumber(void);
#endif /* __FILETEMPLATE_H */
/* EOL */
