//******************************************************************************
//* File Name: Cloud_Protocol_HeartbeatM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_HeartbeatM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_HeartbeatM.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct {
    uint32_t last_send_time;      // The time when the heartbeat was last sent
    uint32_t last_response_time;  // The time when the last heartbeat response was received
    uint8_t timeout_count;        // Heartbeat timeout count
    bool is_heartbeat_active;     // Is the heartbeat activated?
} cloud_protocol_heartbeat_manager_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_heartbeat_manager_t cloud_protocol_heartbeat_manager;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_Stop_Heartbeat(void);
static void Cloud_Protocol_Trigger_ReLogin(void);
static void Cloud_Protocol_Check_Heartbeat_Timeout(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
bool Cloud_Protocol_Get_HeartbeatIsNormal(void)
{
	return (0 == cloud_protocol_heartbeat_manager.timeout_count) && cloud_protocol_heartbeat_manager.is_heartbeat_active;
}

void Cloud_Protocol_Set_HeartbeatResponse(void)
{
	cloud_protocol_heartbeat_manager.timeout_count = 0;
	cloud_protocol_heartbeat_manager.last_response_time = CLOUD_GET_TIME_MS();
}

// Activate heartbeat
void Cloud_Protocol_Start_Heartbeat(void)
{
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};

    cloud_protocol_heartbeat_manager.is_heartbeat_active = true;
    cloud_protocol_heartbeat_manager.timeout_count = 0;
    cloud_protocol_heartbeat_manager.last_send_time = CLOUD_GET_TIME_MS();
	cloud_protocol_heartbeat_manager.last_response_time = cloud_protocol_heartbeat_manager.last_send_time;
    // Send the first heartbeat immediately
    Cloud_Protocol_CallSendFunc(0x03, msg_buffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH);
    CLOUD_INFO("<%s>\r\n", __func__);
}

// Stop heartbeat
static void Cloud_Protocol_Stop_Heartbeat(void)
{
    cloud_protocol_heartbeat_manager.is_heartbeat_active = false;
    CLOUD_INFO("<%s>\r\n", __func__);
}

// Trigger re-login function
static void Cloud_Protocol_Trigger_ReLogin(void)
{
    CLOUD_INFO("Triggering re-login due to heartbeat timeout\r\n");
    
    // Stop the heartbeat
    Cloud_Protocol_Stop_Heartbeat();
}

// Check the heart rate timeout function
static void Cloud_Protocol_Check_Heartbeat_Timeout(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();

    // If no heartbeat response is received within 30 seconds
    if ((current_time - cloud_protocol_heartbeat_manager.last_response_time) > CLOUD_PROTOCOL_HEARTBEAT_TIMEOUT_S)
    {
        cloud_protocol_heartbeat_manager.timeout_count++;

        CLOUD_WARN("Heartbeat response timeout, count: %d, last response: %lu, current: %lu\r\n",
                   cloud_protocol_heartbeat_manager.timeout_count, cloud_protocol_heartbeat_manager.last_response_time, current_time);

        // Three consecutive timeouts trigger a re-login
        if (cloud_protocol_heartbeat_manager.timeout_count >= 3)
        {
            CLOUD_DEBUG("Heartbeat timeout exceeded limit, triggering re-login\r\n");
            Cloud_Protocol_Trigger_ReLogin();
            cloud_protocol_heartbeat_manager.timeout_count = 0; // Reset count
        }
    }
    else
    {
        // Upon receiving a valid response, reset the timeout count
        if (cloud_protocol_heartbeat_manager.timeout_count > 0)
        {
            cloud_protocol_heartbeat_manager.timeout_count = 0;
            CLOUD_INFO("Heartbeat response received, reset timeout count\r\n");
        }
    }
}

void Cloud_Protocol_Heartbeat_Handler(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};

    // Check if a heartbeat needs to be sent
    if (cloud_protocol_heartbeat_manager.is_heartbeat_active &&
        (current_time - cloud_protocol_heartbeat_manager.last_send_time >= CLOUD_PROTOCOL_HEARTBEAT_INTERVAL_S))
    {
        Cloud_Protocol_CallSendFunc(0x03, msg_buffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH);
        cloud_protocol_heartbeat_manager.last_send_time = current_time;
        CLOUD_INFO("Heartbeat sent at time: %lu\r\n", current_time);
    }

    // Check for a heart rate response timeout
    Cloud_Protocol_Check_Heartbeat_Timeout();
}
/* EOL */
