//******************************************************************************
//* File Name: Cloud_Protocol_BillingModelM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_BillingModelM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_BillingModelM.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
cloud_protocol_billing_model_manager_t cloud_protocol_billing_model_manager = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_SetBillingModelFlag(void)
{
    cloud_protocol_billing_model_manager.is_billing_model_updated = true;
}

void Cloud_Protocol_SetBillingModelNumber(uint16_t number)
{
    cloud_protocol_billing_model_manager.billing_model_number = number;
}

void Cloud_Protocol_UpdateBillingModelTimeSlotInfo(cloud_protocol_billing_time_slot_t info)
{
    BIGLITTLEEND_SWAP_4_BYTES(info.high_rate.electricity_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.high_rate.service_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.peak_rate.electricity_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.peak_rate.service_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.normal_rate.electricity_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.normal_rate.service_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.valley_rate.electricity_rate);
    BIGLITTLEEND_SWAP_4_BYTES(info.valley_rate.service_rate);
    
    cloud_protocol_billing_model_manager.time_slot_info = info;
}

uint16_t Cloud_Protocol_GetBillingModelNumber(void)
{
    return cloud_protocol_billing_model_manager.billing_model_number;
}

cloud_protocol_billing_time_slot_t Cloud_Protocol_GetBillingModelTimeSlotInfo(void)
{
	return cloud_protocol_billing_model_manager.time_slot_info;
}

void Cloud_Protocol_Start_BillingModelRequest(void)
{
    uint8_t msg_buffer[CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH] = {0};

    // Send the first billing model request immediately
    Cloud_Protocol_CallSendFunc(0x05, msg_buffer, CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH);
}

void Cloud_Protocol_UpdateBillingModelRequest(void)
{
    uint8_t msg_buffer[CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH] = {0};

    // Send the billing model update request
    Cloud_Protocol_CallSendFunc(0x09, msg_buffer, CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH);
}

/* EOL */
