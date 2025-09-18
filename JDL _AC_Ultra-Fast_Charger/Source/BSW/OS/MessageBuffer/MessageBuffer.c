//******************************************************************************
//* File Name: MessageBuffer.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: MessageBuffer module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "MessageBuffer.h"
#include "MessageBuffer_Cfg.h"

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
MessageBuffer_Comm_System_t* MessageBuffer_APP_And_NET = NULL; // APP ↔ NET, app1_to_app2_buf: APP → NET, app2_to_app1_buf: NET → APP

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint16_t MessageBuffer_Checksum(const uint8_t *data, size_t len);
static MessageBuffer_Comm_System_t *MessageBuffer_Comm_Init(size_t buffer_size, size_t max_msg_size);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// calculate checksum
static uint16_t MessageBuffer_Checksum(const uint8_t *data, size_t len)
{
	uint16_t sum = 0;
	for (size_t i = 0; i < len; i++)
	{
		sum += data[i];
	}
	return sum;
}

// initialize communication system
static MessageBuffer_Comm_System_t *MessageBuffer_Comm_Init(size_t buffer_size, size_t max_msg_size)
{
	MessageBuffer_Comm_System_t *comm = pvPortMalloc(sizeof(MessageBuffer_Comm_System_t));
	if (comm == NULL)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Memory allocation failed\r\n", __func__, __LINE__);
		return NULL;
	}

	comm->max_message_size = max_msg_size;

	// Create MessageBuffer
	comm->app1_to_app2_buf = xMessageBufferCreate(buffer_size);
	comm->app2_to_app1_buf = xMessageBufferCreate(buffer_size);

	if (comm->app1_to_app2_buf == NULL || comm->app2_to_app1_buf == NULL)
	{
		if (comm->app1_to_app2_buf)
			vMessageBufferDelete(comm->app1_to_app2_buf);
		if (comm->app2_to_app1_buf)
			vMessageBufferDelete(comm->app2_to_app1_buf);
		vPortFree(comm);
		return NULL;
	}

	return comm;
}

//send message
BaseType_t MessageBuffer_SendMessage(MessageBuffer_Comm_System_t *comm, MessageBuffer_type_t type,
					 const uint8_t *data, uint16_t data_len,
					 uint8_t dest, TickType_t timeout)
{

	if (data_len > comm->max_message_size)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Message too large\r\n", __func__, __LINE__);
		return pdFAIL; // refuse large messages
	}

	MessageBufferHandle_t target_buf;
	uint8_t source;

	// ensure target and source are set correctly
	if (dest == MESSAGE_BUFFER_ID_APP2)
	{
		// app1 send to app2
		target_buf = comm->app1_to_app2_buf;
		source = MESSAGE_BUFFER_ID_APP1;
	}
	else if (dest == MESSAGE_BUFFER_ID_APP1)
	{
		// app2 send to app1
		target_buf = comm->app2_to_app1_buf;
		source = MESSAGE_BUFFER_ID_APP2;
	}
	else
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Invalid destination ID\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	// Calculate the total message size (header + data)
	size_t total_size = sizeof(MessageBuffer_header_t) + data_len;

	// Allocate temporary buffer
	uint8_t *message_buf = pvPortMalloc(total_size);
	if (message_buf == NULL)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Memory allocation failed\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	// Fill in the message header
	MessageBuffer_header_t *header = (MessageBuffer_header_t *)message_buf;
	header->type = type;
	header->source = source;
	header->dest = dest;
	header->length = data_len;
	header->timestamp = xTaskGetTickCount();
	header->checksum = MessageBuffer_Checksum(data, data_len);

	// copy data
	if (data_len > 0 && data != NULL)
	{
		memcpy(message_buf + sizeof(MessageBuffer_header_t), data, data_len);
	}

	// send to MessageBuffer
	size_t bytes_sent = xMessageBufferSend(target_buf, message_buf, total_size, timeout);

	// free temporary buffer
	vPortFree(message_buf);

	return (bytes_sent == total_size) ? pdPASS : pdFAIL;
}

// receive message
BaseType_t MessageBuffer_ReceiveMessage(MessageBuffer_Comm_System_t *comm,uint8_t *data_buf, size_t buf_size,
										uint16_t *received_len, uint8_t dest, TickType_t timeout)
{

	MessageBufferHandle_t source_buf;

	// ensure target and source are set correctly
	if (dest == MESSAGE_BUFFER_ID_APP2)
	{
		source_buf = comm->app1_to_app2_buf;
	}
	else if (dest == MESSAGE_BUFFER_ID_APP1)
	{
		source_buf = comm->app2_to_app1_buf;
	}
	else
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Invalid destination ID\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	// Read the message header to obtain the message length
	MessageBuffer_header_t header;
	size_t bytes_read = xMessageBufferReceive(source_buf, &header, sizeof(MessageBuffer_header_t), timeout);

	if (bytes_read != sizeof(MessageBuffer_header_t))
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Failed to read message header\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	// read message data
	if (header.length > 0)
	{
		if (header.length > comm->max_message_size)
		{
			MESSAGE_BUFFER_ERROR("<%s %d> Invalid message (possibly corrupted)\r\n", __func__, __LINE__);
			return pdFAIL; // Invalid message (possibly corrupted)
		}

		if (header.length > buf_size)
		{
			// buffer too small, skip this message
			uint8_t temp;
			for (uint16_t i = 0; i < header.length; i++)
			{
				xMessageBufferReceive(source_buf, &temp, 1, pdMS_TO_TICKS(100));
			}
			MESSAGE_BUFFER_ERROR("<%s %d> Buffer too small for message data\r\n", __func__, __LINE__);
			return pdFAIL;
		}

		bytes_read = xMessageBufferReceive(source_buf, data_buf, header.length, timeout);
		if (bytes_read != header.length)
		{
			MESSAGE_BUFFER_ERROR("<%s %d> Failed to read complete message data\r\n", __func__, __LINE__);
			return pdFAIL;
		}

		// verify checksum
		uint16_t calculated_cs = MessageBuffer_Checksum(data_buf, header.length);
		if (calculated_cs != header.checksum)
		{
			MESSAGE_BUFFER_ERROR("<%s %d> Checksum mismatch\r\n", __func__, __LINE__);
			return pdFAIL;
		}
	}

	// return message info
	if (received_len != NULL)
		*received_len = header.length;

	return pdPASS;
}

// deinitialize communication system
void MessageBuffer_Deinit(MessageBuffer_Comm_System_t *comm)
{
	if (comm != NULL)
	{
		if (comm->app1_to_app2_buf)
			vMessageBufferDelete(comm->app1_to_app2_buf);
		if (comm->app2_to_app1_buf)
			vMessageBufferDelete(comm->app2_to_app1_buf);
		vPortFree(comm);
	}
}

// create communication system instance
void MessageBuffer_CreateInstance(void)
{
	MessageBuffer_APP_And_NET = MessageBuffer_Comm_Init(MESSAGE_BUFFER_APP2_2_NET_MAX_SIZE, MESSAGE_BUFFER_APP1_2_NET_SIZE);
	if (NULL != MessageBuffer_APP_And_NET)
	{
		// Initialization successful
		MESSAGE_BUFFER_INFO("<%s %d> MessageBuffer_APP_And_NET created successfully\r\n", __func__, __LINE__);
	}
}
/* EOL */
