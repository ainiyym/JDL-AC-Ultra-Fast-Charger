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
#define CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT          (300U / CLOUDM_TASK_PERIOD)  // e.g., 300 * 10ms = 3s timeout

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
    CLOUD_PROTOCOL_STEP_INIT,
    CLOUD_PROTOCOL_STEP_SETUP_NETWORK,
    CLOUD_PROTOCOL_STEP_WAIT_NETWORK_ACK,
    CLOUD_PROTOCOL_STEP_SETUP_HEARTBEAT,
    CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_ACK,
    CLOUD_PROTOCOL_STEP_LOGIN,
    CLOUD_PROTOCOL_STEP_MAX
} cloud_protocol_step_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t step;
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

void Cloud_Protocol_SetDeviceStatus(cloud_device_status_e status)
{
    cloud_protocol_ctrl.device_status = status;
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
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_SETUP_HEARTBEAT;
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SETUP_NETWORK);
    }
}

static void Cloud_Protocol_SetupHeartbeatProcess(void)
{
    // Setup heartbeat parameters
    cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_ACK;
}

static void Cloud_Protocol_WaitHeartbeatAckProcess(void)
{
    // Waiting for heartbeat acknowledgment
    if (CLOUD_DEVICE_STATUS_CONNECTED == cloud_protocol_ctrl.device_status)
    {
        cloud_protocol_ctrl.step = CLOUD_PROTOCOL_STEP_LOGIN;
    }
    else
    {
        Cloud_Protocol_Timeout_Handler(CLOUDM_PROTOCOL_INIT_POLLING_TIMEOUT, CLOUD_PROTOCOL_STEP_SETUP_HEARTBEAT);
    }
}

static void Cloud_Protocol_LogIn(void)
{

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
        case CLOUD_PROTOCOL_STEP_SETUP_HEARTBEAT:
        {
            Cloud_Protocol_SetupHeartbeatProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_WAIT_HEARTBEAT_ACK:
        {
            Cloud_Protocol_WaitHeartbeatAckProcess();
            break;
        }
        case CLOUD_PROTOCOL_STEP_LOGIN:
        {
            Cloud_Protocol_LogIn();
            break;
        }
        default:
            break;
    }
}

/* EOL */
