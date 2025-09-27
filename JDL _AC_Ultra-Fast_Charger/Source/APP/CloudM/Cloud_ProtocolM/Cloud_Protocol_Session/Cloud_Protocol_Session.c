//******************************************************************************
//* File Name: Cloud_Protocol_Session.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module session layer file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Session.h"
#include "Cloud_Protocol_CallbackFunc.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// Protocol frame structure
typedef struct
{
    uint8_t start_byte;       // Start identifier (1 byte)
    uint8_t data_length;      // Data length (1 byte)
    uint16_t sequence_number; // Sequence number (2 bytes)
    uint8_t encryption_flag;  // Encryption flag (1 byte)
    uint8_t frame_type;       // Frame type (1 byte)
    uint8_t *message_body;    // Message body (N bytes)
    uint16_t crc;             // Frame check field (2 bytes)
} Cloud_Protocol_Frame_T;

// Send the record structure
typedef struct
{
    uint8_t frame_type;        // The type of the sent frame
    uint16_t sequence_number;  // Sequence number
    time_t send_time;          // Send timestamp
    uint8_t retry_count;       // Retry count
    bool awaiting_response;    // Whether waiting for a response
    uint8_t expected_response; // Expected response frame type
} Cloud_Protocol_Send_Record_T;

// Receive the record structure
typedef struct
{
    uint8_t frame_type;        // The type of the received frame
    uint16_t sequence_number;  // Sequence number
    time_t receive_time;       // Receive timestamp
    uint8_t related_send_type; // Related send frame type
} Cloud_Protocol_Receive_Record_T;

// Communication state machine
typedef struct
{
    Cloud_Protocol_Send_Record_T pending_requests[CLOUDM_PROTOCOL_MESSAGE_Buffer_SIZE]; // Pending request queue
    uint8_t request_count;                             // Current pending request count
    bool is_authenticated;                             // Whether authenticated
    time_t last_heartbeat_time;                        // Last heartbeat time
    uint16_t next_sequence_number;                     // Next sequence number
} Cloud_Protocol_Communication_State_T;

/*******************************************************************************
|    Static local KAM variables Declaration 
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static Cloud_Protocol_Communication_State_T cloud_protocol_comm_state;
static Cloud_Protocol_Frame_T cloud_protocol_recv_frame;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
static const Cloud_Protocol_Frame_Type_Config_T CLOUD_PROTOCOL_FRAME_CONFIG_TABLE[] = 
{    
// FrameType    | Name                          | Send func                                   | recv func                                 | NeedAck       | ExpectedAck
    {0x01,      "Pile Login Auth",               Cloud_Protocol_0x01_Callback,                 NULL,                                       true,          0x02},
    {0x02,      "Login Auth Ack",                NULL,                                         Cloud_Protocol_0x02_Callback,               false,         0x00},
    {0x03,      "Pile Heartbeat",                Cloud_Protocol_0x03_Callback,                 NULL,                                       true,          0x04},
    {0x04,      "Heartbeat Ack",                 NULL,                                         Cloud_Protocol_0x04_Callback,               false,         0x00},
    {0x05,      "Billing Model Verify Req",      Cloud_Protocol_0x05_Callback,                 NULL,                                       true,          0x06},
    {0x06,      "Billing Model Verify Ack",      NULL,                                         Cloud_Protocol_0x06_Callback,               false,         0x00},
    {0x09,      "Pile Billing Model Req",        Cloud_Protocol_0x09_Callback,                 NULL,                                       true,          0x0A},
    {0x0A,      "Billing Model Req Ack",         NULL,                                         Cloud_Protocol_0x0A_Callback,               false,         0x00},
    {0x12,      "Read Real-time Monitor Data",   NULL,                                         Cloud_Protocol_0x12_Callback,               false,         0x13},
    {0x13,      "Offline Monitor Data",          Cloud_Protocol_0x13_Callback,                 NULL,                                       false,         0x00},
};
#define CLOUD_PROTOCOL_FRAME_CONFIG_COUNT (sizeof(CLOUD_PROTOCOL_FRAME_CONFIG_TABLE) / sizeof(Cloud_Protocol_Frame_Type_Config_T))
/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// Initialize the communication status
void Cloud_Protocol_InitCommunicationState(void)
{
    memset(&cloud_protocol_comm_state, 0, sizeof(Cloud_Protocol_Communication_State_T));
    cloud_protocol_comm_state.next_sequence_number = 0;
    cloud_protocol_comm_state.is_authenticated = false;

    memset(&cloud_protocol_recv_frame, 0, sizeof(Cloud_Protocol_Frame_T));
    cloud_protocol_recv_frame.message_body = NULL;
}

// Get frame type configuration by frame type code
static const Cloud_Protocol_Frame_Type_Config_T *Cloud_Protocol_GetFrameConfig(uint8_t frame_type)
{
    for (size_t i = 0; i < CLOUD_PROTOCOL_FRAME_CONFIG_COUNT; i++)
    {
        if (CLOUD_PROTOCOL_FRAME_CONFIG_TABLE[i].frame_type == frame_type)
        {
            return &CLOUD_PROTOCOL_FRAME_CONFIG_TABLE[i];
        }
    }
    return NULL; // The corresponding frame type configuration was not found
}

// Build protocol frames
static uint16_t Cloud_Protocol_Frame_Build(uint8_t *output_buffer,
                                    uint16_t buffer_size,
                                    uint16_t sequence_number,
                                    Cloud_Protocol_EncryptionType_E encryption,
                                    uint8_t frame_type,
                                    const uint8_t *message,
                                    uint16_t message_length)
{
    // Calculate the total frame length
    uint16_t total_length = CLOUDM_PROTOCOL_FRAME_OVERHEAD + message_length; // Header 8 bytes + message body

    if (buffer_size < total_length)
    {
        return 0; // Insufficient buffer size
    }

    if (message_length > CLOUDM_PROTOCOL_MESSAGE_LEN_MAX)
    {
        return 0; // Message too long
    }

    // Build header
    output_buffer[0] = CLOUDM_PROTOCOL_HEADER_PREFIX; // Start byte
    output_buffer[1] = 4 + message_length;            // Data length
    output_buffer[2] = sequence_number & 0xFF;        // Sequence number low byte
    output_buffer[3] = (sequence_number >> 8) & 0xFF; // Sequence number high byte
    output_buffer[4] = (uint8_t)encryption;           // Encryption flag
    output_buffer[5] = (uint8_t)frame_type;           // Frame type

    // Copy message body
    if (message_length > 0)
    {
        memcpy(&output_buffer[6], message, message_length);
    }

    // Calculate and add CRC
    uint16_t crc_data_length = 4 + message_length; // Sequence number(2)+encryption(1)+type(1)+message body
    uint16_t crc = CloudM_Crc16(&output_buffer[2], crc_data_length);

    // CRC low byte first
    uint16_t crc_offset = 6 + message_length;
    output_buffer[crc_offset] = crc & 0xFF;            // CRC low byte
    output_buffer[crc_offset + 1] = (crc >> 8) & 0xFF; // CRC high byte

    return total_length;
}

// Remove the responded requests
static void Cloud_Protocol_RemovePendingRequest(uint8_t element)
{
    // Remove this record (move the array elements)
    for (uint8_t j = element; j < cloud_protocol_comm_state.request_count - 1; j++)
    {
        cloud_protocol_comm_state.pending_requests[j] = cloud_protocol_comm_state.pending_requests[j + 1];
    }
    cloud_protocol_comm_state.request_count--;
}

static void Cloud_Protocol_HandleRequestSuccess(const Cloud_Protocol_Frame_T *frame)
{
    const Cloud_Protocol_Frame_Type_Config_T *config = Cloud_Protocol_GetFrameConfig(frame->frame_type);

    if (!config)
    {
        CLOUD_ERROR("Cloud Protocol Unknown Frame Type: 0x%02X\r\n", frame->frame_type);
        return;
    }
    CLOUD_INFO("Cloud Protocol Frame Received: %s (0x%02X), Sequence: %d, Length: %d\r\n",
               config->frame_name, frame->frame_type, frame->sequence_number, frame->data_length);
    // Process the received frame
    switch (frame->frame_type)
    {
        case 0x02: // Login Authentication response
            cloud_protocol_comm_state.is_authenticated = true;
            break;

        case 0x04: // Heartbeat packet response
            cloud_protocol_comm_state.last_heartbeat_time = time(NULL);
            break;

        case 0x06: // Billing model verification response
            break;

        case 0x0A: // Billing model request response
            break;

        case 0x13: // Offline monitoring data (response to 0x12)
            break;

        default:
            // Handle other frame types
            break;
    }
    // Call the corresponding receive callback function
    if (config->recv_func != NULL)
    {
        config->recv_func(NULL, frame->message_body, frame->data_length - 4); // Exclude sequence number(2)+encryption(1)+type(1)
    }
}

// Process the received frames
static void Cloud_Protocol_ProcessRcvFrame(const Cloud_Protocol_Frame_T *frame)
{
    // Search for matching pending requests
    for (uint8_t i = 0; i < cloud_protocol_comm_state.request_count; i++)
    {
        Cloud_Protocol_Send_Record_T *record = &cloud_protocol_comm_state.pending_requests[i];
        if (record->awaiting_response &&
            record->expected_response == frame->frame_type &&
            record->sequence_number == frame->sequence_number)
        {
            // Find the matching request and update the status
            record->awaiting_response = false;
            Cloud_Protocol_HandleRequestSuccess(frame);
            Cloud_Protocol_RemovePendingRequest(i);
            break;
        }
    }
}

// Check for timeout requests and retransmit them
void Cloud_Protocol_CheckTimeoutRequests(void *arg)
{
    time_t current_time = CLOUD_GET_TIME_MS();

    for (uint8_t i = 0; i < cloud_protocol_comm_state.request_count; i++)
    {
        Cloud_Protocol_Send_Record_T *record = &cloud_protocol_comm_state.pending_requests[i];

        if (record->awaiting_response &&
            (current_time - record->send_time) > CLOUDM_PROTOCOL_RESPONSE_TIMEOUT_MS)
        {

            if (record->retry_count < CLOUDM_PROTOCOL_MAX_RETRY_COUNT)
            {
                // Retransmission logic
                const Cloud_Protocol_Frame_Type_Config_T *config = Cloud_Protocol_GetFrameConfig(record->frame_type);
                if (config != NULL && config->send_func != NULL)
                {
                    // Call the send function
                    CLOUDNET_INFO("Retransmitted frame: %s (0x%02X), Sequence: %d, Retry count: %d\n",
                                  config->frame_name, record->frame_type, record->sequence_number, record->retry_count + 1);
                    uint8_t buff[CLOUDM_PROTOCOL_FRAME_MAX_LEN];
                    config->send_func(NULL, buff, sizeof(buff));
                }
                record->send_time = current_time;
                record->retry_count++;
            }
            else
            {
                // Discard and remove
                record->awaiting_response = false;
                Cloud_Protocol_RemovePendingRequest(i);
            }
        }
    }
}

// Prepare to send the frame
uint16_t Cloud_Protocol_PrepareSendFrame(uint8_t frame_type,
                                         uint8_t *buffer,
                                         uint16_t buffer_size,
                                         const uint8_t *message_data,
                                         uint16_t message_length)
{

    const Cloud_Protocol_Frame_Type_Config_T *config = Cloud_Protocol_GetFrameConfig(frame_type);
    if (config == NULL)
    {
        return 0; // invalid frame type
    }

    // Generate sequence number
    uint16_t sequence = cloud_protocol_comm_state.next_sequence_number++;

    // Build protocol frame (using the previously defined build_protocol_frame function)
    uint16_t frame_length = Cloud_Protocol_Frame_Build(buffer, buffer_size, sequence,
                                                       CLOUD_PROTOCOL_DATA_ENCRYPTION_NONE,
                                                       frame_type,
                                                       message_data,
                                                       message_length);

    if (frame_length > 0 && config->requires_response)
    {
        // Add to the response queue
        if (cloud_protocol_comm_state.request_count < CLOUDM_PROTOCOL_MESSAGE_Buffer_SIZE)
        {
            Cloud_Protocol_Send_Record_T *record = &cloud_protocol_comm_state.pending_requests[cloud_protocol_comm_state.request_count++];
            record->frame_type = frame_type;
            record->sequence_number = sequence;
            record->send_time = CLOUD_GET_TIME_MS();
            record->retry_count = 0;
            record->awaiting_response = true;
            record->expected_response = config->expected_response;
        }
    }

    return frame_length;
}

// get send function by frame type and call it
Cloud_Protocol_Send_Status_T Cloud_Protocol_CallSendFunc(uint8_t frame_type,
                                         uint8_t *buffer,
                                         uint16_t buffer_size)
{
    const Cloud_Protocol_Frame_Type_Config_T *config = Cloud_Protocol_GetFrameConfig(frame_type);
    if (config == NULL || config->send_func == NULL)
    {
        CLOUD_ERROR("%s: Invalid frame type: %d\r\n", __func__, frame_type);
        return CLOUD_PROTOCOL_SEND_ERROR_INVALID_PARAM; // invalid frame type
    }

    // Call the send function
    return config->send_func(NULL, buffer, buffer_size);
}

// Parse protocol frames
Cloud_Protocol_Parse_Status_T Cloud_Protocol_ParseProtocolFrame(const uint8_t *buffer, uint16_t buffer_length)
{
    // 1. Check minimum buffer length
    if (buffer_length < CLOUDM_PROTOCOL_FRAME_OVERHEAD)
    { 
        return CLOUD_PROTOCOL_PARSE_ERROR_BUFFER_TOO_SMALL;
    }

    // 2. Check start byte
    if (buffer[0] != CLOUDM_PROTOCOL_HEADER_PREFIX)
    {
        return CLOUD_PROTOCOL_PARSE_ERROR_INVALID_START;
    }

    // 3. Parse fixed header fields
    cloud_protocol_recv_frame.start_byte = buffer[0];
    cloud_protocol_recv_frame.data_length = buffer[1];
    cloud_protocol_recv_frame.sequence_number = (buffer[3] << 8) | buffer[2]; // Low byte first
    cloud_protocol_recv_frame.encryption_flag = buffer[4];
    cloud_protocol_recv_frame.frame_type = buffer[5];

    // 4. Check data length validity
    uint16_t expected_message_length = cloud_protocol_recv_frame.data_length - 4; // Subtract sequence number(2)+encryption(1)+type(1)
    uint16_t total_frame_length = CLOUDM_PROTOCOL_FRAME_OVERHEAD + expected_message_length; // Header 8 bytes + message body

    if (buffer_length < total_frame_length)
    {
        return CLOUD_PROTOCOL_PARSE_ERROR_BUFFER_TOO_SMALL;

    }

    if (expected_message_length > CLOUDM_PROTOCOL_MESSAGE_LEN_MAX)
    {
        return CLOUD_PROTOCOL_PARSE_ERROR_INVALID_LENGTH;
    }

    // 5. Extract the message body
    if (expected_message_length > 0)
    {
        cloud_protocol_recv_frame.message_body = (uint8_t *)CLOUDM_MALLOC(expected_message_length);
        if (cloud_protocol_recv_frame.message_body == NULL)
        {
            return CLOUD_PROTOCOL_PARSE_ERROR_MEMORY_ALLOCATION;
        }
        memcpy(cloud_protocol_recv_frame.message_body, &buffer[6], expected_message_length);
    }
    else
    {
        cloud_protocol_recv_frame.message_body = NULL;
    }

    // 6. Extract and verify the CRC
    uint16_t crc_offset = 6 + expected_message_length;
    cloud_protocol_recv_frame.crc = (buffer[crc_offset + 1] << 8) | buffer[crc_offset]; // The low byte comes first

    // 7. Calculate the CRC check range: from the sequence number field to the end of the message body
    uint16_t crc_data_length = 4 + expected_message_length; // Sequence number(2)+encryption(1)+type(1)+message body

    if (cloud_protocol_recv_frame.crc != CloudM_Crc16((uint8_t *)&buffer[2], crc_data_length))
    {
        if (cloud_protocol_recv_frame.message_body != NULL)
        {
            CLOUDM_FREE(cloud_protocol_recv_frame.message_body);
            cloud_protocol_recv_frame.message_body = NULL;
        }
        return CLOUD_PROTOCOL_PARSE_ERROR_CRC_MISMATCH;
    }

    // 8. Successfully parsed the frame
    Cloud_Protocol_ProcessRcvFrame(&cloud_protocol_recv_frame);

    // 9. Free the allocated message body memory
    if (cloud_protocol_recv_frame.message_body != NULL)
    {
        CLOUDM_FREE(cloud_protocol_recv_frame.message_body);
        cloud_protocol_recv_frame.message_body = NULL;
    }

    return CLOUD_PROTOCOL_PARSE_SUCCESS;
}

/* EOL */
