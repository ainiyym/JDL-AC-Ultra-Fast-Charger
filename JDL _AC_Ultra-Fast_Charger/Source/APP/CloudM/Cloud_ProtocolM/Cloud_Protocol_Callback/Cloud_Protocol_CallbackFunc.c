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
#include "Cloud_Protocol_CallbackFunc.h"
#include "YeeComxxx_Device_Cfg.h"
#include "Cloud_Ev_Charger_Information.h"

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

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * String to compressed BCD code
 * @param str Enter a numeric string
 * @param bcd Output the BCD buffer
 * @param bcd_size Buffer size
 * @return The number of BCD bytes successfully converted, and -1 returned if failed
 */
int string_to_packed_bcd(const char *str, uint8_t *bcd, size_t bcd_size)
{
	if (str == NULL || bcd == NULL || bcd_size == 0)
	{
		return -1;
	}

	size_t len = strlen(str);
	if (len == 0)
	{
		return 0;
	}

	// Calculate the required number of BCD bytes (storing 2 digits per byte)
	size_t bcd_bytes_needed = (len + 1) / 2;
	if (bcd_bytes_needed > bcd_size)
	{
		return -1; // Buffer is insufficient
	}

	memset(bcd, 0, bcd_size);

	int bcd_index = 0;
	int shift_high = 1; // Start from a high position

	// Big-endian pattern: Processing from the beginning of the string backward (with the high bits first)
	for (size_t i = 0; i < len; i++)
	{
		uint8_t digit = str[i] - '0';

		if (shift_high)
		{
			bcd[bcd_index] = digit << 4; // The number is placed in the top four digits
			shift_high = 0;
		}
		else
		{
			bcd[bcd_index] |= digit; // The number is placed in the bottom four digits
			bcd_index++;
			shift_high = 1;
		}
	}

	return bcd_bytes_needed;
}

char *uint8_array_to_hex_string(const uint8_t *array, size_t len)
{
	if (array == NULL || len == 0)
	{
		return NULL;
	}

	// Each byte requires two characters to represent it, plus a string terminator
	char *result = (char *)pvPortMalloc(len * 2 + 1);
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t i = 0; i < len; i++)
	{
		// Convert the high 4 bits and low 4 bits of each byte to hexadecimal characters respectively
		sprintf(result + i * 2, "%02x", array[i]);
	}

	result[len * 2] = '\0'; // Add a string terminator

	CLOUD_INFO("array:");
	CLOUD_PRINT_HEX(array, len);
	return result;
}

// Parse protocol frames
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x01_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	int Bcdlength = 0;
	// Handle frame type 0x01 (Pile Login Auth)
	uint8_t body[CLOUD_PROTOCOL_0x01_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	char SN[CLOUD_EV_SN_LEN] = {0};
	Cloud_Ev_Get_Constant_Info(CLOUD_CONST_SERIAL_NUMBER, SN, sizeof(SN));
	Bcdlength = string_to_packed_bcd(SN, &body[bodylen], CLOUD_PROTOCOL_SN_LENGTH);
	if (Bcdlength < 0)
	{
		// Error handling
		CLOUD_ERROR("%s: Invalid SN format\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
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
	char simCard[YEECOM_ICCID_LENGTH + 1] = {0};
	YeeCom_GetDeviceInfo(NULL, NULL, (char *)simCard, NULL);
	Bcdlength = string_to_packed_bcd(simCard, &body[bodylen], CLOUD_PROTOCOL_SIM_LENGTH);
	if (Bcdlength < 0)
	{
		// Error handling
		CLOUD_ERROR("%s: Invalid SIM Card format\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
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
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(0x01, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_SEND_LOGIN_FRAME;
	// Log the hex string of the message
	char *buff_hex = uint8_array_to_hex_string(&buff[1], frame_length);
	uint16_t buff_hex_len = strlen(buff_hex);
	memcpy(&buff[1], buff_hex, buff_hex_len);
	vPortFree(buff_hex);
	// Send the message
	Cloud_Protocol_SendMsg(buff, buff_hex_len + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);

	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x02_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x02 (Login Auth Ack)
	CLOUD_INFO("<%s> msg:%s\r\n", __func__, msg);
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x03_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x03 (Pile Heartbeat)
	int Bcdlength = 0;
	uint8_t body[CLOUD_PROTOCOL_0x03_BODY_LENGTH] = {0};
	uint16_t bodylen = 0;
	// Fill in the message body
	// SN
	char SN[CLOUD_EV_SN_LEN] = {0};
	Cloud_Ev_Get_Constant_Info(CLOUD_CONST_SERIAL_NUMBER, SN, sizeof(SN));
	Bcdlength = string_to_packed_bcd(SN, &body[bodylen], CLOUD_PROTOCOL_SN_LENGTH);
	if (Bcdlength < 0)
	{
		// Error handling
		CLOUD_ERROR("%s: Invalid SN format\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	bodylen += CLOUD_PROTOCOL_SN_LENGTH;
	// Connector ID
	body[bodylen] = 1; // Assuming single connector with ID 1
	bodylen += 1;
	// Connector Status
	Cloud_Ev_Get_Dynamic_Info(CLOUD_DYNAMIC_CONNECTOR_STATUS, &body[bodylen], sizeof(uint8_t));
	bodylen += 1;

	if (bodylen != CLOUD_PROTOCOL_0x03_BODY_LENGTH)
	{
		CLOUD_ERROR("%s: Message length mismatch, expected %d, got %d\r\n", __func__, CLOUD_PROTOCOL_0x03_BODY_LENGTH, bodylen);
		return CLOUD_PROTOCOL_SEND_ERROR_MESSAGE_LENGTH_MISMATCH;
	}
	// Prepare the full frame
	uint16_t frame_length = Cloud_Protocol_PrepareSendFrame(0x03, &buff[1], buffSize - 1, body, bodylen);
	if (frame_length == 0)
	{
		CLOUD_ERROR("%s: Failed to prepare send frame\r\n", __func__);
		return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM;
	}
	// buff[0] is reserved for message type
	buff[0] = CLOUD_MESSAGE_DATA_TYPE_SEND_HEARTBEAT_FRAME;
	// Send the message
	char *buff_hex = uint8_array_to_hex_string(&buff[1], frame_length);
	uint16_t buff_hex_len = strlen(buff_hex);
	memcpy(&buff[1], buff_hex, buff_hex_len);
	vPortFree(buff_hex);
	Cloud_Protocol_SendMsg(buff, buff_hex_len + 1, CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
	return CLOUD_PROTOCOL_SEND_SUCCESS;
}

void Cloud_Protocol_0x04_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x04 (Heartbeat Ack)
	CLOUD_INFO("<%s> msg:%s\r\n", __func__, msg);
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x05_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x05 (Billing Model Verification)
	// Add your processing logic here
}

void Cloud_Protocol_0x06_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x06 (Billing Model Verify Ack)
	// Add your processing logic here
}

Cloud_Protocol_Send_Status_T Cloud_Protocol_0x09_Callback(void *arg, uint8_t *buff, uint16_t buffSize)
{
	// Handle frame type 0x09 (Pile Billing Model Request)
	// Add your processing logic here
}

void Cloud_Protocol_0x0A_Callback(void *arg, uint8_t *msg, uint16_t bodylen)
{
	// Handle frame type 0x0A (Billing Model Request Ack)
	// Add your processing logic here
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

/* EOL */
