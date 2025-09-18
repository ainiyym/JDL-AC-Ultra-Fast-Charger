//******************************************************************************
//* File Name: MessageBuffer.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: MessageBuffer module header file
/*******************************************************************************/
#if !defined (__MESSAGEBUFFER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __MESSAGEBUFFER_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MESSAGE_BUFFER_APP1_2_NET_SIZE          (256)
#define MESSAGE_BUFFER_APP2_2_NET_MAX_SIZE      (MESSAGE_BUFFER_APP1_2_NET_SIZE * 4)
#define MESSAGE_BUFFER_ID_APP1                  (1)
#define MESSAGE_BUFFER_ID_APP2                  (2)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
******************************************************************************/
// message buffer data type
typedef enum
{
    MESSAGE_BUFFER_TYPE_DATA = 0x55, // data message
    MESSAGE_BUFFER_TYPE_CTRL = 0xCC // control message
} MessageBuffer_type_t;

// message header
typedef struct
{
    MessageBuffer_type_t type; // message type
    uint8_t source;            // source id
    uint8_t dest;              // dest id
    uint16_t length;           // data len
    uint32_t timestamp;        // timestamp
    uint16_t checksum;         // checksum
} MessageBuffer_header_t;

// message handler
typedef struct
{
    MessageBufferHandle_t app1_to_app2_buf; // APP1 → APP2
    MessageBufferHandle_t app2_to_app1_buf; // APP2 → APP1
    size_t max_message_size;                // max message size
} MessageBuffer_Comm_System_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
extern MessageBuffer_Comm_System_t* MessageBuffer_APP_And_NET; // APP ↔ NET, app1_to_app2_buf: APP → NET, app2_to_app1_buf: NET → APP

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern BaseType_t MessageBuffer_SendMessage(MessageBuffer_Comm_System_t *comm, MessageBuffer_type_t type, const uint8_t *data, uint16_t data_len, uint8_t dest, TickType_t timeout);
extern BaseType_t MessageBuffer_ReceiveMessage(MessageBuffer_Comm_System_t *comm, uint8_t *data_buf, size_t buf_size, uint16_t *received_len, uint8_t dest, TickType_t timeout);
extern void MessageBuffer_CreateInstance(void);
extern void MessageBuffer_Deinit(MessageBuffer_Comm_System_t *comm);
#endif /* __MESSAGEBUFFER_H */
/* EOL */
