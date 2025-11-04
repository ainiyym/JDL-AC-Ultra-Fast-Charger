//******************************************************************************
//* File Name: Cloud_Protocol_CallbackFunc.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "YeeComxxx_Device_Cfg.h"
#include "Cloud_Ev_Charger_Information.h"
#include "Cloud_Protocol.h"
#include "STD_RTC.h"
#include "STD_SysM.h"

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
	bool is_initialized;
	uint8_t SN[CLOUD_PROTOCOL_SN_LENGTH];   // Charger Serial Number (BCD Format)
	uint8_t SIM[CLOUD_PROTOCOL_SIM_LENGTH]; // SIM ICCID Card(BCD Format)
} cloud_protocol_callback_func_commom_variable_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static cloud_protocol_callback_func_commom_variable_t cloud_protocol_callback_func_commom_variable = {0};

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_CallbackFunc_Init(void)
{
	// Initialize common variables
	int Bcdlength = 0;
	//SN
	if (!Cloud_Ev_Get_Constant_Info(CLOUD_CONST_SERIAL_NUMBER, &cloud_protocol_callback_func_commom_variable.SN[0], CLOUD_PROTOCOL_SN_LENGTH))
	{
		CLOUD_ERROR("%s: Failed to get SN from constant info\r\n", __func__);
		return;
	}
	//SIM ICCID Card
	char simCard[YEECOM_ICCID_LENGTH + 1] = {0};
	YeeCom_GetDeviceInfo(NULL, NULL, (char *)simCard, NULL, NULL, NULL);
	Bcdlength = Cloud_String_To_Packed_Bcd(simCard, &cloud_protocol_callback_func_commom_variable.SIM[0], CLOUD_PROTOCOL_SIM_LENGTH);
	if (Bcdlength != CLOUD_PROTOCOL_SIM_LENGTH)
	{
		// Error handling
		CLOUD_ERROR("%s: Invalid SIM Card format\r\n", __func__);
	}
	cloud_protocol_callback_func_commom_variable.is_initialized = true;
	CLOUD_INFO("<%s>init success\r\n", __func__);
}

// Parse protocol frames
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x01_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x01 (Pile Login Auth)
	uint8_t body[CLOUD_PROTOCOL_0x01_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	if (!cloud_protocol_callback_func_commom_variable.is_initialized)
	{
		CLOUD_ERROR("%s: Common variables not initialized\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// Pile Type
	Cloud_Ev_Get_Constant_Info(CLOUD_CONST_CONNECTOR_TYPE, &body[bodylen], sizeof(uint8_t));
	bodylen += 1;
	// Gun Count
	Cloud_Ev_Get_Constant_Info(CLOUD_CONST_NUMBER_OF_CONNECTORS, &body[bodylen], sizeof(uint8_t));
	bodylen += 1;
	// Protocol Version
	body[bodylen] = CLOUD_PROTOCOL_PROTOCOL_VERSION;
	bodylen += 1;
	// Software Version
	Cloud_Ev_Get_Constant_Info(CLOUD_CONST_FIRMWARE_VERSION, (void *)(body + bodylen), CLOUD_PROTOCOL_SOFTWARE_VERSION_LENGTH);
	bodylen += CLOUD_PROTOCOL_SOFTWARE_VERSION_LENGTH;
	// Network Type
	body[bodylen] = (uint8_t)CLOUD_PROTOCOL_NETWORK_SIM;
	bodylen += 1;
	// SIM Card
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SIM, CLOUD_PROTOCOL_SIM_LENGTH);
	bodylen += CLOUD_PROTOCOL_SIM_LENGTH;
	// Operator
	body[bodylen] = OPERATOR_CT;
	bodylen += 1;

	if (bodylen != CLOUD_PROTOCOL_0x01_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x01_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x01, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);

	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x02_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x02 (Login Auth Ack)
	CLOUD_INFO("<%s> msg:\r\n", __func__);
	CLOUD_PRINT_HEX(msg, bodylen);
	uint8_t auth_result = msg[7];
	if (auth_result == 0)
	{
		Cloud_Protocol_SetLogInStatus(CLOUD_PROTOCOL_AUTHENTICATION_SUCCESS);
		Cloud_Protocol_Start_Heartbeat();
		vTaskDelay(pdMS_TO_TICKS(200)); // Delay 200 milliseconds to ensure heartbeat starts first
		Cloud_Protocol_Start_BillingModelRequest();
		CLOUD_INFO("%s: Authentication successful, starting heartbeat and billing model request\r\n", __func__);
	}
	else
	{
		Cloud_Protocol_SetLogInStatus(CLOUD_PROTOCOL_AUTHENTICATION_FAILED);
		CLOUD_ERROR("%s: Authentication failed, error code: %d\r\n", __func__, auth_result);
	}
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x03_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x03 (Pile Heartbeat)
	uint8_t body[CLOUD_PROTOCOL_0x03_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	Cloud_Protocol_Send_Status_T send_status = CLOUD_PROTOCOL_SEND_SUCCESS;
	Cloud_Ev_Connector_StatusType_E connector_status = CLOUD_EV_CHARGER_CONNECTOR_NORMAL;

	// polling each connector
	for (uint8_t i = 0; i < CLOUD_EV_MAX_CONNECTORS; i++)
	{
		uint8_t current_connector_id = i + 1; // Connector IDs start from 1
		bodylen = 0;

		// Fill in the message body
		// SN
		memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
		bodylen += CLOUD_PROTOCOL_SN_LENGTH;

		// Connector ID
		body[bodylen] = current_connector_id;
		bodylen += 1;

		// Connector Status
		Cloud_Ev_Get_Dynamic_Info(i, CLOUD_DYNAMIC_CONNECTOR_STATUS, &connector_status, sizeof(connector_status));
		body[bodylen] = connector_status;
		bodylen += 1;

		if (bodylen != CLOUD_PROTOCOL_0x03_BODY_LENGTH)
		{
			CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x03_BODY_LENGTH, bodylen);
			send_status = CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
			continue;
		}

		// Prepare the full frame
		uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x03, &buff[1], buffSize - 1, body, bodylen);
		if (frame_length == 0)
		{
			CLOUD_ERROR("%s: Failed to prepare send frame for connector %d\r\n", __func__, current_connector_id);
			send_status = CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
			continue;
		}

		// buff[0] is reserved for message type
		buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;

		// Send the message
		Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);

		CLOUD_INFO("%s: Sent heartbeat for connector %d\r\n", __func__, current_connector_id);
	}

	return send_status;
}

void Cloud_Protocol_0x04_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x04 (Heartbeat Ack)
	CLOUD_INFO("<%s>\r\n", __func__);
	Cloud_Protocol_Set_HeartbeatResponse();
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x05_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x05 (Billing Model Verification)
	uint8_t body[CLOUD_PROTOCOL_0x05_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;

	// get sn
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// fill in billing model number
	uint16_t billing_model_number = Cloud_Protocol_GetBillingModelNumber();
	memcpy(&body[bodylen], &billing_model_number, sizeof(billing_model_number));
	bodylen += sizeof(billing_model_number);
	// judge message length
	if (bodylen != CLOUD_PROTOCOL_0x05_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x05_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x05, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x06_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x06 (Billing Model Verify Ack)
	uint16_t billing_model_number = 0;
	uint8_t result = msg[9];

	memcpy(&billing_model_number, &msg[7], sizeof(billing_model_number));
	CLOUD_INFO("%s:<result:%d> Received billing model number: %d\r\n", __func__, result, billing_model_number);
	Cloud_Protocol_SetBillingModelNumber(billing_model_number);
	if (result == 0)
	{
		Cloud_Protocol_SetBillingModelFlag();
	}
	else
	{
		Cloud_Protocol_UpdateBillingModelRequest();
	}
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x09_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x09 (Pile Billing Model Request)
	uint8_t body[CLOUD_PROTOCOL_0x09_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;

	// get sn
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// judge message length
	if (bodylen != CLOUD_PROTOCOL_0x09_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x09_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x09, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x0A_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x0A (Billing Model Request Ack)
	uint8_t SN[CLOUD_PROTOCOL_SN_LENGTH];
	uint16_t billing_mode_id = 0;
	cloud_protocol_billing_time_slot_t billing_time_slot_info = {0};

	memcpy(SN, &msg[7], CLOUD_PROTOCOL_SN_LENGTH);
	if (memcmp(SN, cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH) != 0)
	{
		CLOUD_ERROR("%s: SN mismatch\r\n", __func__);
		return;
	}
	memcpy(&billing_mode_id, &msg[7 + CLOUD_PROTOCOL_SN_LENGTH], sizeof(billing_mode_id));
	memcpy(&billing_time_slot_info, &msg[7 + CLOUD_PROTOCOL_SN_LENGTH + sizeof(billing_mode_id)], sizeof(billing_time_slot_info));
	Cloud_Protocol_SetBillingModelNumber(billing_mode_id);
	Cloud_Protocol_UpdateBillingModelTimeSlotInfo(billing_time_slot_info);
	Cloud_Protocol_SetBillingModelFlag();
	CLOUD_INFO("%s: Billing Model is refresh, Received billing model number: %d\r\n", __func__, billing_mode_id);
}

void Cloud_Protocol_0x12_Callback(void *arg, uint8_t *msg, uint16_t bodylen) 
{
	// Handle frame type 0x12 (Read Real-time Monitor Data)
	// Add your processing logic here
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x13_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x13 (Offline Monitor Data)
	// Add your processing logic here
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x55_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x55 (Time synchronization Settings Ack)
	// Add your processing logic here
	uint8_t body[CLOUD_PROTOCOL_0x55_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// Device CP56Time2a
	uint8_t cp56_time[7] = {0};
	RTC_GetCP56Time2a(cp56_time);
	memcpy(&body[bodylen], cp56_time, sizeof(cp56_time));
	bodylen += sizeof(cp56_time);
	if (bodylen != CLOUD_PROTOCOL_0x55_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x55_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x55, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x56_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x56 (Time synchronization Settings)
	// Add your processing logic here
	uint8_t time_sync_flag = RTC_CP56Time2aSetRtcDateTime(&msg[7]); // The time starts from the 8th byte
	if (time_sync_flag == 1)
	{
		CLOUD_INFO("%s: Time synchronization requested.\r\n", __func__);
		Cloud_Protocol_FlashNetTime();
	}
	else
	{
		CLOUD_ERROR("%s: Invalid data format.\r\n", __func__);
	}
}

void Cloud_Protocol_0x92_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x92 (Remote reset)
	uint8_t reset_type = msg[7];
	if (reset_type == 1)
	{
		CLOUD_INFO("%s: immediate reset requested.\r\n", __func__);
		SYSM_SetResetCmd(1);
	}
	else if (reset_type == 2)
	{
		CLOUD_INFO("%s: conditional reset requested.\r\n", __func__);
		SYSM_SetResetCmd(2);
	}
	else
	{
		CLOUD_ERROR("%s: Invalid reset type: %d\r\n", __func__, reset_type);
	}
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x91_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x91 (Remote reset Ack)
	uint8_t body[CLOUD_PROTOCOL_0x91_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// Result
	body[bodylen] = 1; // Assuming 1 means success
	bodylen += 1;
	if (bodylen != CLOUD_PROTOCOL_0x91_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x91_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x91, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x58_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x58 (Billing model Settings)
	uint8_t SN[CLOUD_PROTOCOL_SN_LENGTH] = {0};
	uint16_t billing_mode_id = 0;
	cloud_protocol_billing_time_slot_t billing_time_slot_info = {0};

	memcpy(SN, &msg[0], CLOUD_PROTOCOL_SN_LENGTH);
	if (memcmp(SN, cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH) != 0)
	{
		CLOUD_ERROR("%s: SN mismatch\r\n", __func__);
		return;
	}
	memcpy(&billing_mode_id, &msg[CLOUD_PROTOCOL_SN_LENGTH], sizeof(billing_mode_id));
	memcpy(&billing_time_slot_info, &msg[CLOUD_PROTOCOL_SN_LENGTH + sizeof(billing_mode_id)], sizeof(billing_time_slot_info));
	Cloud_Protocol_SetBillingModelNumber(billing_mode_id);
	Cloud_Protocol_UpdateBillingModelTimeSlotInfo(billing_time_slot_info);
	Cloud_Protocol_SetBillingModelFlag();
	CLOUD_INFO("%s: Billing Model is refresh, Received billing model number: %d\r\n", __func__, billing_mode_id);
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x57_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x57 (Billing model Settings Ack)
	uint8_t body[CLOUD_PROTOCOL_0x57_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	memcpy(&body[bodylen], cloud_protocol_callback_func_commom_variable.SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// Result
	body[bodylen] = 1; // Assuming 1 means success
	bodylen += 1;
	if (bodylen != CLOUD_PROTOCOL_0x57_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x57_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(arg, 0x57, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_Order_Upload_Callback(const cloud_protocol_charging_cloud_protocol_order_manager_t *order)
{
    if (order == NULL)
    {
        CLOUD_ERROR("%s: Invalid order parameter\r\n", __func__);
        return;
    }
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};
    const Cloud_Protocol_Frame_Type_Config_T *config = Cloud_Protocol_GetFrameConfig(0x3B);

    if (config == NULL || config->send_func == NULL)
    {
        CLOUD_ERROR("%s: Invalid frame type: %02x\r\n", __func__, 0x3B);
        return;
    }

    // Call the send function
    config->send_func(order, msg_buffer, sizeof(msg_buffer));
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x3B_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x3B (Transaction record)
	// Add your processing logic here
	cloud_protocol_charging_cloud_protocol_order_manager_t *order_manager = (cloud_protocol_charging_cloud_protocol_order_manager_t *)arg;
	cloud_protocol_charging_order_t *order = &order_manager->active_orders;

	uint8_t body[CLOUD_PROTOCOL_0x3B_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// 1. Transaction serial number (16-byte BCD code)
	memcpy(&body[bodylen], order->transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);
	bodylen += CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH;

	// 2. Stub number (7-byte BCD code, with zeros added if less than 7 bits)
	memcpy(&body[bodylen], order->SN, CLOUD_PROTOCOL_SN_LENGTH);
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;

	// 3. Gun number (1-byte BCD code)
	body[bodylen++] = order->connector_id;

	// 4. Start time (7-byte CP56Time2a format)
	memcpy(&body[bodylen], &order->start_time, sizeof(CP56Time2a_t));
	bodylen += sizeof(CP56Time2a_t);

	// 5. End time (7-byte CP56Time2a format)
	memcpy(&body[bodylen], &order->end_time, sizeof(CP56Time2a_t));
	bodylen += sizeof(CP56Time2a_t);

	// 6-9. Peak rate information (unit price 4B + electricity 4B + loss electricity 4B + amount 4B)
	memcpy(&body[bodylen], &order->peak, sizeof(cloud_protocol_rate_info_t));
	bodylen += sizeof(cloud_protocol_rate_info_t);

	// 10-13. High rate information
	memcpy(&body[bodylen], &order->high, sizeof(cloud_protocol_rate_info_t));
	bodylen += sizeof(cloud_protocol_rate_info_t);

	// 14-17. Normal rate information
	memcpy(&body[bodylen], &order->normal, sizeof(cloud_protocol_rate_info_t));
	bodylen += sizeof(cloud_protocol_rate_info_t);

	// 18-21. Valley rate information
	memcpy(&body[bodylen], &order->valley, sizeof(cloud_protocol_rate_info_t));
	bodylen += sizeof(cloud_protocol_rate_info_t);

	// 22. Total starting value of the electricity meter (5-byte BIN)
	memcpy(&body[bodylen], &order->total_start, 5);
	bodylen += 5;

	// 23. Total ending value of the electricity meter (5-byte BIN)
	memcpy(&body[bodylen], &order->total_end, 5);
	bodylen += 5;

	// 24. total charged electricity (4-byte BIN)
	memcpy(&body[bodylen], &order->total_energy, 4);
	bodylen += 4;

	// 25. total loss electricity (4-byte BIN)
	memcpy(&body[bodylen], &order->total_loss_energy, 4);
	bodylen += 4;

	// 26. total amount (4-byte BIN)
	memcpy(&body[bodylen], &order->total_amount, 4);
	bodylen += 4;

	// 27. VIN (17-byte ASCII)
	memcpy(&body[bodylen], order->vin, 17);
	bodylen += 17;

	// 28. transaction type (1-byte BIN)
	body[bodylen++] = (uint8_t)order->transaction_type;

	// 29. transaction date and time (7-byte CP56Time2a format)
	memcpy(&body[bodylen], &order->transaction_time, sizeof(CP56Time2a_t));
	bodylen += sizeof(CP56Time2a_t);

	// 30. Stop reason (1-byte BIN)
	body[bodylen++] = (uint8_t)order->stop_reason;

	// 31. physical card number (8-byte BIN)
	memcpy(&body[bodylen], order->physical_card, CLOUD_PROTOCOL_RFID_UID_LENGTH);
	bodylen += CLOUD_PROTOCOL_RFID_UID_LENGTH;

	if (bodylen != CLOUD_PROTOCOL_0x3B_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x3B_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(NULL, 0x3B, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
	// Send the message
	Cloud_Protocol_SendMsg(buff, frame_length + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);

	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x40_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x40 (Firmware Upgrade Command)
	// Add your processing logic here
}
/* EOL */
