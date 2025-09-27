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
    CLOUD_PROTOCOL_SEND_LOG_IN_FRAME,
    CLOUD_PROTOCOL_WAIT_LOG_IN_FRAME_ACK,
    CLOUD_PROTOCOL_SEND_HEARTBEAT_FRAME,
    CLOUD_PROTOCOL_WAIT_HEARTBEAT_FRAME_ACK,
    CLOUD_PROTOCOL_STEP_RUNNING,
    CLOUD_PROTOCOL_STEP_MAX
} cloud_protocol_step_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t step;
    bool heartbeat_param_is_set;
    bool regpkg_param_is_set;
    bool login_frame_is_set;
    bool heartbeat_frame_is_set;
    uint16_t timer;
    cloud_device_status_e device_status;
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

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static cloud_protocol_ctrl_t cloud_protocol_ctrl;

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

void Cloud_Protocol_AckLoginFrame(bool status)
{
    cloud_protocol_ctrl.login_frame_is_set = status;
}

void Cloud_Protocol_AckHeartbeatFrame(bool status)
{
    cloud_protocol_ctrl.heartbeat_frame_is_set = status;
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
    msg[1] = (uint8_t)CLOUD_PROTOCOL_HEARTBEAT_INTERVAL_MS;
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
    msg[1] = YEECOM_REGPKG_ENABLE_HEX;
    Cloud_Protocol_SendMsg(msg, sizeof(msg), CLOUD_MESSAGE_TYPE_CTRL);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_REGPKG_MODE_ACK;
}

static void Cloud_Protocol_WaitRegpkgModeAckProcess(void)
{
    // Waiting for registration packet mode acknowledgment
    if (cloud_protocol_ctrl.regpkg_param_is_set)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_SEND_LOG_IN_FRAME;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SET_REGPKG_MODE);
    }
}

static void Cloud_Protocol_SendLogInFrame(void)
{
    uint8_t msg[256] = {0};
    // Prepare login frame data
    Cloud_Protocol_CallSendFunc(0x01, &msg[0], 256);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_WAIT_LOG_IN_FRAME_ACK;
}

static void Cloud_Protocol_WaitLoginFrameAckProcess(void)
{
    // Waiting for login acknowledgment
    if (cloud_protocol_ctrl.login_frame_is_set)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_SEND_HEARTBEAT_FRAME;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_SEND_LOG_IN_FRAME);
    }
}

static void Cloud_Protocol_SendHeartbeatFrame(void)
{
    uint8_t msg[256] = {0};
    // Prepare heartbeat frame data
    Cloud_Protocol_CallSendFunc(0x03, &msg[0], 256);
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_WAIT_HEARTBEAT_FRAME_ACK;
}

static void Cloud_Protocol_WaitHeartbeatFrameAckProcess(void)
{
    // Waiting for heartbeat acknowledgment
    if (cloud_protocol_ctrl.heartbeat_frame_is_set)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_RUNNING;
        Cloud_Protocol_Clear_Timeout();
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_SEND_HEARTBEAT_FRAME);
    }
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
        case CLOUD_PROTOCOL_SEND_LOG_IN_FRAME:
        {
            Cloud_Protocol_SendLogInFrame();
            break;
        }
        case CLOUD_PROTOCOL_WAIT_LOG_IN_FRAME_ACK:
        {
            Cloud_Protocol_WaitLoginFrameAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_SEND_HEARTBEAT_FRAME:
        {
            Cloud_Protocol_SendHeartbeatFrame();
            break;
        }
        case CLOUD_PROTOCOL_WAIT_HEARTBEAT_FRAME_ACK:
        {
            Cloud_Protocol_WaitHeartbeatFrameAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_RUNNING:
        {
            // Normal operation
            break;
        }
        default:
            break;
    }
}

/* EOL */
