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
typedef struct
{
	uint8_t buffer[MESSAGE_BUFFER_SIZE];
} MessageBufferSlot_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
MessageBuffer_Comm_System_t* message_buffer_app_2_net = NULL; // APP ↔ NET, app1_to_app2_buf: APP → NET, app2_to_app1_buf: NET → APP
static MessageBufferSlot_t message_buffer_pool[MESSAGE_BUFFER_MESSAGE_BUFFER_POOL_SIZE] = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint16_t MessageBuffer_Checksum(const uint8_t *data, size_t len);
static MessageBuffer_Comm_System_t *MessageBuffer_Comm_Init(size_t buffer_size, size_t max_msg_size);
static void MessageBuffer_Comm_Destroy(MessageBuffer_Comm_System_t *comm);

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

	memset(comm, 0, sizeof(MessageBuffer_Comm_System_t));

	// Create MessageBuffer
	comm->app1_to_app2_buf = xMessageBufferCreate(buffer_size);
	comm->app2_to_app1_buf = xMessageBufferCreate(buffer_size);

	if (comm->app1_to_app2_buf == NULL || comm->app2_to_app1_buf == NULL)
	{
		MessageBuffer_Comm_Destroy(comm);
		return NULL;
	}

	comm->max_message_size = max_msg_size;
	comm->send_mutex = xSemaphoreCreateMutex();

	if (!comm->send_mutex)
	{
		MessageBuffer_Comm_Destroy(comm);
		return NULL;
	}

	return comm;
}

// destroy communication system
static void MessageBuffer_Comm_Destroy(MessageBuffer_Comm_System_t *comm)
{
	if (!comm)
	{
		return;
	}

	// delete MessageBuffers
	if (comm->app1_to_app2_buf)
	{
		vMessageBufferDelete(comm->app1_to_app2_buf);
	}

	if (comm->app2_to_app1_buf)
	{
		vMessageBufferDelete(comm->app2_to_app1_buf);
	}

	// delete mutex
	if (comm->send_mutex)
	{
		vSemaphoreDelete(comm->send_mutex);
	}

	// free comm structure
	vPortFree(comm);
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

	if (xSemaphoreTake(comm->send_mutex, timeout) != pdPASS)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Failed to acquire send mutex\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	MessageBufferHandle_t target_buf;
	uint8_t source;
	size_t bytes_sent = pdFAIL;
	uint8_t *message_buf = NULL;
	size_t total_size = 0;
	size_t free_heap = 0;
	MessageBuffer_header_t *header = NULL;

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
		goto free_buffer;
	}

	// Calculate the total message size (header + data)
	total_size = sizeof(MessageBuffer_header_t) + data_len;
	free_heap = xPortGetFreeHeapSize();

	if (total_size > free_heap + 1024)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Insufficient heap memory for message\r\n", __func__, __LINE__);
		goto free_buffer;
	}
	// Allocate temporary buffer
	message_buf = pvPortMalloc(total_size);
	if (message_buf == NULL)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Memory allocation failed\r\n", __func__, __LINE__);
		goto free_buffer;
	}

	// Fill in the message header
	header = (MessageBuffer_header_t *)message_buf;
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
	bytes_sent = xMessageBufferSend(target_buf, message_buf, total_size, timeout);

free_buffer:
	// free temporary buffer
	if (message_buf != NULL)
	{
		vPortFree(message_buf);
	}
	xSemaphoreGive(comm->send_mutex);

	return (bytes_sent == total_size) ? pdPASS : pdFAIL;
}

// receive message
BaseType_t MessageBuffer_ReceiveMessage(MessageBuffer_Comm_System_t *comm, MessageBuffer_type_t *type,
										uint8_t **data_ptr , uint16_t *received_len, uint8_t dest, TickType_t timeout)
{
	MessageBufferHandle_t source_buf;
	uint8_t *message_buf;

	// 1. Verify the target ID and obtain the source buffer
	switch (dest)
	{
		case MESSAGE_BUFFER_ID_APP2:
			source_buf = comm->app1_to_app2_buf;
			message_buf = message_buffer_pool[0].buffer;
			break;
		case MESSAGE_BUFFER_ID_APP1:
			source_buf = comm->app2_to_app1_buf;
			message_buf = message_buffer_pool[1].buffer;
			break;
		default:
			MESSAGE_BUFFER_ERROR("<%s %d> Invalid destination ID\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	//2. Read message body
	memset(message_buf, 0, MESSAGE_BUFFER_SIZE);
	size_t bytes_read = xMessageBufferReceive(source_buf, message_buf, MESSAGE_BUFFER_SIZE, timeout);
	if (bytes_read == 0)
	{
		return pdFAIL;
	}

	//3. check Read message header
	MessageBuffer_header_t* header = NULL;
	header = (MessageBuffer_header_t *)message_buf;

	// 4. Verify message type
	if (header->type != MESSAGE_BUFFER_TYPE_DATA && header->type != MESSAGE_BUFFER_TYPE_CTRL &&
		header->type != MESSAGE_BUFFER_TYPE_NOTIFY)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Invalid message type: %d\r\n", __func__, __LINE__, header->type);
		return pdFAIL;
	}

	// 5. Verify message length
	if (header->length > comm->max_message_size)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Invalid message length: %u\r\n", __func__, __LINE__, header->length);
		return pdFAIL;
	}

	// 6. Verify the checksum of the message body
	uint16_t calculated_cs = MessageBuffer_Checksum(message_buf + sizeof(MessageBuffer_header_t), header->length);
	if (calculated_cs != header->checksum)
	{
		MESSAGE_BUFFER_ERROR("<%s %d> Checksum mismatch\r\n", __func__, __LINE__);
		return pdFAIL;
	}

	// 7. Copy message body to user buffer
    if (data_ptr != NULL)
    {
        *data_ptr = message_buf + sizeof(MessageBuffer_header_t);
    }

	// 8. Return message information
	if (type)
		*type = header->type;
	if (received_len)
		*received_len = header->length;

	return pdPASS;
}

// deinitialize communication system
void MessageBuffer_Deinit(MessageBuffer_Comm_System_t *comm)
{
	if (comm != NULL)
	{
		MessageBuffer_Comm_Destroy(comm);
	}
}

// create communication system instance
void MessageBuffer_CreateInstance(void)
{
	message_buffer_app_2_net = MessageBuffer_Comm_Init(MESSAGE_BUFFER_APP2_2_NET_MAX_SIZE, MESSAGE_BUFFER_SIZE);
	if (NULL != message_buffer_app_2_net)
	{
		// Initialization successful
		MESSAGE_BUFFER_INFO("<%s %d> message_buffer_app_2_net created successfully\r\n", __func__, __LINE__);
	}
}
/* EOL */
