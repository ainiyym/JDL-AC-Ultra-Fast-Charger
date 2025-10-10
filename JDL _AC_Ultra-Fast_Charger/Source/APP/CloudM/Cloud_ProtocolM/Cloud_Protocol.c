//******************************************************************************
//* File Name: Cloud_Protocol.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol.h"
#include "Cloud_Protocol_Msg.h"
#include "tcp.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT          (3000U / CLOUDM_TASK_PERIOD)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
    CLOUD_PROTOCOL_STEP_INIT,
    CLOUD_PROTOCOL_STEP_SETUP_NETWORK,
    CLOUD_PROTOCOL_STEP_WAIT_NETWORK_ACK,
    CLOUD_PROTOCOL_STEP_SET_HEARTBEAT_INTERVAL,
    CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_INTERVAL_ACK,
    CLOUD_PROTOCOL_STEP_SET_REGPKG_MODE,
    CLOUD_PROTOCOL_STEP_WAIT_REGPKG_MODE_ACK,
    CLOUD_PROTOCOL_STEP_WAKE_UP_DTU,
    CLOUD_PROTOCOL_STEP_WAIT_WAKE_UP_DTU_ACK,
    CLOUD_PROTOCOL_STEP_RUNNING,
    CLOUD_PROTOCOL_STEP_MAX
} cloud_protocol_step_t;

enum
{
    CLOUD_PROTOCOL_RUNNING_STEP_INIT,
    CLOUD_PROTOCOL_RUNNING_STEP_PROCESS_DATA,
    CLOUD_PROTOCOL_RUNNING_STEP_MAX
} cloud_protocol_running_step_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct {
    uint32_t last_send_time;      // The time when the heartbeat was last sent
    uint32_t last_response_time;  // The time when the last heartbeat response was received
    uint8_t timeout_count;        // Heartbeat timeout count
    bool is_heartbeat_active;     // Is the heartbeat activated?
} cloud_protocol_heartbeat_manager_t;

typedef struct
{
    uint8_t step;
    uint8_t running_step;
    bool heartbeat_param_is_set;
    bool regpkg_param_is_set;
    bool dtu_is_wake_up;
    bool nettime_is_flash;
    uint16_t timer;
    cloud_device_status_e device_status;
    cloud_protocol_heartbeat_manager_t heartbeat_manager;
} cloud_protocol_ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_ctrl_t cloud_protocol_ctrl;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_NormalOperationProcess(void);
static void Cloud_Protocol_ResetLogIn(void);
static void Cloud_Protocol_Check_Heartbeat_Timeout(void);
static void Cloud_Protocol_Heartbeat_Handler(void);
static void Cloud_Protocol_Trigger_ReLogin(void);
static void Cloud_Protocol_Stop_Heartbeat(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_Init(void)
{
    memset(&cloud_protocol_ctrl, 0, sizeof(cloud_protocol_ctrl));
}

void Cloud_Protocol_AckDeviceStatus(cloud_device_status_e status)
{
    cloud_protocol_ctrl.device_status = status;
}

void Cloud_Protocol_AckHeartbeatParam(bool status)
{
    cloud_protocol_ctrl.heartbeat_param_is_set = status;
}
void Cloud_Protocol_AckRegpkgParam(bool status)
{
    cloud_protocol_ctrl.regpkg_param_is_set = status;
}

void Cloud_Protocol_AckWakeUpDTU(bool status)
{
    cloud_protocol_ctrl.dtu_is_wake_up = status;
}

void Cloud_Protocol_FlashNetTime(void)
{
    cloud_protocol_ctrl.nettime_is_flash = true;
}

static void Cloud_Protocol_ResetLogIn(void)
{
    cloud_protocol_ctrl.running_step = CLOUD_PROTOCOL_RUNNING_STEP_INIT;
    Cloud_Protocol_ResetLogInStatus();
}

// Activate heartbeat
void Cloud_Protocol_Start_Heartbeat(void)
{
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};

    cloud_protocol_ctrl.heartbeat_manager.is_heartbeat_active = true;
    cloud_protocol_ctrl.heartbeat_manager.timeout_count = 0;
    cloud_protocol_ctrl.heartbeat_manager.last_send_time = CLOUD_GET_TIME_MS();
    // Send the first heartbeat immediately
    Cloud_Protocol_CallSendFunc(0x03, msg_buffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH);
    // Initialize the last response time to the current time
    Cloud_Protocol_ReFlashHbTime(); 
    CLOUD_INFO("<%s>\r\n", __func__);
}

// Stop heartbeat
static void Cloud_Protocol_Stop_Heartbeat(void)
{
    cloud_protocol_ctrl.heartbeat_manager.is_heartbeat_active = false;
    CLOUD_INFO("<%s>\r\n", __func__);
}

static void Cloud_Protocol_Heartbeat_Handler(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};

    // Check if a heartbeat needs to be sent
    if (cloud_protocol_ctrl.heartbeat_manager.is_heartbeat_active &&
        (current_time - cloud_protocol_ctrl.heartbeat_manager.last_send_time >= CLOUD_PROTOCOL_HEARTBEAT_INTERVAL_S))
    {
        Cloud_Protocol_CallSendFunc(0x03, msg_buffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH);
        cloud_protocol_ctrl.heartbeat_manager.last_send_time = current_time;

        CLOUD_INFO("Heartbeat sent at time: %lu\r\n", current_time);
    }

    // Check for a heart rate response timeout
    Cloud_Protocol_Check_Heartbeat_Timeout();
}

// Check the heart rate timeout function
static void Cloud_Protocol_Check_Heartbeat_Timeout(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();
    time_t last_hb_time = Cloud_Protocol_GetHbTime();

    // If no heartbeat response is received within 30 seconds (3 heartbeat cycles)
    if ((current_time - last_hb_time) > CLOUD_PROTOCOL_HEARTBEAT_TIMEOUT_S)
    {
        cloud_protocol_ctrl.heartbeat_manager.timeout_count++;

        CLOUD_WARN("Heartbeat response timeout, count: %d, last response: %lu, current: %lu\r\n",
                   cloud_protocol_ctrl.heartbeat_manager.timeout_count, last_hb_time, current_time);

        // Three consecutive timeouts trigger a re-login
        if (cloud_protocol_ctrl.heartbeat_manager.timeout_count >= 3)
        {
            CLOUD_DEBUG("Heartbeat timeout exceeded limit, triggering re-login\r\n");
            Cloud_Protocol_Trigger_ReLogin();
            cloud_protocol_ctrl.heartbeat_manager.timeout_count = 0; // Reset count
        }
    }
    else
    {
        // Upon receiving a valid response, reset the timeout count
        if (cloud_protocol_ctrl.heartbeat_manager.timeout_count > 0)
        {
            cloud_protocol_ctrl.heartbeat_manager.timeout_count = 0;
            CLOUD_INFO("Heartbeat response received, reset timeout count\r\n");
        }
    }
}

// Trigger re-login function
static void Cloud_Protocol_Trigger_ReLogin(void)
{
    CLOUD_INFO("Triggering re-login due to heartbeat timeout\r\n");
    
    // Stop the heartbeat
    Cloud_Protocol_Stop_Heartbeat();

    // Reset login status
    Cloud_Protocol_ResetLogIn();
}

static void Cloud_Protocol_Clear_Timeout(void)
{
    cloud_protocol_ctrl.timer = 0;
}

static void Cloud_Protocol_Timeout_Handler(uint16_t timeout, uint8_t step)
{
    if (cloud_protocol_ctrl.timer < timeout)
    {
        cloud_protocol_ctrl.timer++;
    }
    else
    {
        cloud_protocol_ctrl.timer = 0;
        cloud_protocol_ctrl.step = step;
    }
}

static void Cloud_Protocol_ResetToInitPolling(void)
{
    if (CLOUD_DEVICE_STATUS_INIT == cloud_protocol_ctrl.device_status)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_INIT;
        cloud_protocol_ctrl.running_step = CLOUD_PROTOCOL_RUNNING_STEP_INIT;
        Cloud_Protocol_ResetLogInStatus();
    }
}

static void Cloud_Protocol_InitStepProcess(void)
{
    if (CLOUD_DEVICE_STATUS_READY == cloud_protocol_ctrl.device_status)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_SETUP_NETWORK;
    }
}

static void Cloud_Protocol_SetupNetworkProcess(void)
{
    uint8_t msg[2] = {0};
    msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM;
    msg[1] = (uint8_t)TCP_ID_PROTOCOL;
    Cloud_Protocol_SendMsg(msg, sizeof(msg), CLOUD_MESSAGE_TYPE_CTRL);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_NETWORK_ACK;
}

static void Cloud_Protocol_WaitNetworkAckProcess(void)
{
    // Waiting for network setup acknowledgment
    if (CLOUD_DEVICE_STATUS_CONNECTED == cloud_protocol_ctrl.device_status)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_SET_HEARTBEAT_INTERVAL;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SETUP_NETWORK);
    }
}

static void Cloud_Protocol_SetHeartbeatIntervalProcess(void)
{
    // Setup heartbeat parameters
    uint8_t msg[2] = {0};
    msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM;
    msg[1] = (uint8_t)CLOUD_4G_HEARTBEAT_INTERVAL_S;
    Cloud_Protocol_SendMsg(msg, sizeof(msg), CLOUD_MESSAGE_TYPE_CTRL);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_INTERVAL_ACK;
}

static void Cloud_Protocol_WaitHeartbeatAckProcess(void)
{
    // Waiting for heartbeat acknowledgment
    if (cloud_protocol_ctrl.heartbeat_param_is_set)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_SET_REGPKG_MODE;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SET_HEARTBEAT_INTERVAL);
    }
}

static void Cloud_Protocol_SetupRegpkgModeProcess(void)
{
    // Setup registration packet mode parameters
    uint8_t msg[2] = {0};
    msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_SET_REGPKG_MODE;
    msg[1] = YEECOM_REGPKG_DISABLE;
    Cloud_Protocol_SendMsg(msg, sizeof(msg), CLOUD_MESSAGE_TYPE_CTRL);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_REGPKG_MODE_ACK;
}

static void Cloud_Protocol_WaitRegpkgModeAckProcess(void)
{
    // Waiting for registration packet mode acknowledgment
    if (cloud_protocol_ctrl.regpkg_param_is_set)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAKE_UP_DTU;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SET_REGPKG_MODE);
    }
}

static void Cloud_Protocol_WakeUpDTUProcess(void)
{
    // Wake up DTU
    uint8_t msg[2] = {0};
    msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_WAKE_UP_DTU;
    msg[1] = 0; // No additional parameters
    Cloud_Protocol_SendMsg(msg, sizeof(msg), CLOUD_MESSAGE_TYPE_CTRL);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_WAKE_UP_DTU_ACK;
}

static void Cloud_Protocol_WaitWakeUpDTUAckProcess(void)
{
    if (cloud_protocol_ctrl.dtu_is_wake_up)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_RUNNING;
        CLOUD_DEBUG("%s: DTU is awake\r\n", __func__);
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_WAKE_UP_DTU);
    }
}

static void Cloud_Protocol_RunningStepProcess(void)
{
    uint8_t msg_buffer[CLOUD_MESSAGE_BUFFER_MAX_LENGTH] = {0};

    switch (cloud_protocol_ctrl.running_step)
    {
        case CLOUD_PROTOCOL_RUNNING_STEP_INIT:
            // send login authentication frame
            Cloud_Protocol_CallSendFunc(0x01, msg_buffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH);
            cloud_protocol_ctrl.running_step = CLOUD_PROTOCOL_RUNNING_STEP_PROCESS_DATA;
            break;
        case CLOUD_PROTOCOL_RUNNING_STEP_PROCESS_DATA:
            switch (Cloud_Protocol_GetLogInStatus())
            {
                case CLOUD_PROTOCOL_AUTHENTICATION_SUCCESS:
                    // normal operation
                    Cloud_Protocol_NormalOperationProcess();
                    Cloud_Protocol_Clear_Timeout();
                    break;
                case CLOUD_PROTOCOL_AUTHENTICATION_FAILED:
                    // Authentication failed, re-initiate authentication
                    Cloud_Protocol_Stop_Heartbeat();
                    Cloud_Protocol_ResetLogIn();
                    Cloud_Protocol_Clear_Timeout();
                    break;
                case CLOUD_PROTOCOL_AUTHENTICATION_INIT:
                    // 30 seconds without authentication response, reset device
                    if (cloud_protocol_ctrl.timer < CLOUD_RESET_DEVICE_DELAY_TIME_S)
                    {
                        cloud_protocol_ctrl.timer++;
                    }
                    else
                    {   
                        Cloud_Protocol_Clear_Timeout();
                        CLOUD_ERROR("Authentication timeout, restarting device\r\n");
                        CLOUD_PROTOCOL_RESTART_DEVICE();
                    }
                    break;
                default:
                    break;
            }
            break;
    }
}

static void Cloud_Protocol_NormalOperationProcess(void)
{
    // Placeholder for normal operation tasks
    Cloud_Protocol_Heartbeat_Handler();
}

void Cloud_Protocol_Main(void)
{
    Cloud_Protocol_ResetToInitPolling();
    switch (cloud_protocol_ctrl.step)
    {
        case CLOUD_PROTOCOL_STEP_INIT:
        {
			Cloud_Protocol_InitStepProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_SETUP_NETWORK:
        {
            Cloud_Protocol_SetupNetworkProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAIT_NETWORK_ACK:
        {
            Cloud_Protocol_WaitNetworkAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_SET_HEARTBEAT_INTERVAL:
        {
            Cloud_Protocol_SetHeartbeatIntervalProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_INTERVAL_ACK:
        {
            Cloud_Protocol_WaitHeartbeatAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_SET_REGPKG_MODE:
        {
            Cloud_Protocol_SetupRegpkgModeProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAIT_REGPKG_MODE_ACK:
        {
            Cloud_Protocol_WaitRegpkgModeAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAKE_UP_DTU:
        {
            Cloud_Protocol_WakeUpDTUProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAIT_WAKE_UP_DTU_ACK:
        {
            Cloud_Protocol_WaitWakeUpDTUAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_RUNNING:
        {
            // Normal operation
            Cloud_Protocol_RunningStepProcess();
            break;
        }
        default:
            break;
    }
}

/* EOL */
