//******************************************************************************
//* File Name: Cloud_Protocol_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CONFIG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CONFIG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "Cloud_Cfg.h"
#include "Lib_Crc.h"
#include "Tcp.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUDM_PROTOCOL_HEADER_PREFIX			    (0x68U)
#define CLOUDM_PROTOCOL_MESSAGE_LEN_MAX				(200U)
#define CLOUDM_PROTOCOL_FRAME_OVERHEAD			    (8U) // Start(1)+Length(1)+Seq(2)+Enc(1)+Type(1)+CRC(2)
#define CLOUDM_PROTOCOL_FRAME_MAX_LEN			    (CLOUDM_PROTOCOL_MESSAGE_LEN_MAX + CLOUDM_PROTOCOL_FRAME_OVERHEAD)
#define CLOUDM_PROTOCOL_MESSAGE_Buffer_SIZE         (10)
#define CLOUDM_PROTOCOL_RETRY_MAX				    (3U)
#define CLOUDM_PROTOCOL_RETRY_INTERVAL_MS		    (2000U / CLOUDM_TASK_PERIOD) // 2 seconds
#define CLOUDM_PROTOCOL_RESPONSE_TIMEOUT_MS	        (5000U / CLOUDM_TASK_PERIOD) // 5 seconds
#define CLOUDM_PROTOCOL_MAX_RETRY_COUNT             (3U)

#define CloudM_Crc16(data, len)                     BigLittleEnd_Swap_2_Bytes(Lib_Crc16(data, len))
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_PROTOCOL_DATA_ENCRYPTION_NONE = 0,
    CLOUD_PROTOCOL_DATA_ENCRYPTION_3DES
}Cloud_Protocol_EncryptionType_E;

// Frame type definition
typedef enum
{
    CLOUD_PROTOCOL_FRAME_TYPE_UNKNOWN = 0x00,
    CLOUD_PROTOCOL_FRAME_TYPE_UPLINK = 0x01,   // Uplink data
    CLOUD_PROTOCOL_FRAME_TYPE_DOWNLINK = 0x02, // Downlink data
} Cloud_Protocol_Frame_Type_T;

// Parsing status enumeration
typedef enum
{
    CLOUD_PROTOCOL_PARSE_SUCCESS = 0,
    CLOUD_PROTOCOL_PARSE_ERROR_INVALID_START,
    CLOUD_PROTOCOL_PARSE_ERROR_INVALID_LENGTH,
    CLOUD_PROTOCOL_PARSE_ERROR_CRC_MISMATCH,
    CLOUD_PROTOCOL_PARSE_ERROR_BUFFER_TOO_SMALL,
    CLOUD_PROTOCOL_PARSE_ERROR_MEMORY_ALLOCATION
} Cloud_Protocol_Parse_Status_T;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef void (*Cloud_Protocol_Recv_Cb)(void *arg, uint8_t *msg, uint16_t msglen);
typedef void (*Cloud_Protocol_Send_Cb)(void *arg);

// Frame type information structure
typedef struct
{
    uint8_t frame_type;                   // Frame type code
    const char *frame_name;               // Frame type name
    Cloud_Protocol_Send_Cb send_func;     // Send callback function
    Cloud_Protocol_Recv_Cb recv_func;     // Receive callback function
    bool requires_response;               // Requires response
    uint8_t expected_response;            // Expected response frame type
} Cloud_Protocol_Frame_Type_Config_T;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Cloud_Protocol_InitCommunicationState(void);
extern Cloud_Protocol_Parse_Status_T Cloud_Protocol_ParseProtocolFrame(const uint8_t *buffer, uint16_t buffer_length);
extern uint16_t Cloud_Protocol_PrepareSendFrame(uint8_t frame_type,
                                                uint8_t *buffer,
                                                uint16_t buffer_size,
                                                const uint8_t *message_data,
                                                uint16_t message_length);
extern void Cloud_Protocol_CheckTimeoutRequests(void *arg);
#endif /* __CLOUD_PROTOCOL_CONFIG_H */
/* EOL */
