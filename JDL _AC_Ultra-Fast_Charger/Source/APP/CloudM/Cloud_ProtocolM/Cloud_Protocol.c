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
#include "Cloud_Protocol_GagaM.h"
#include "Cloud_Protocol_SgM.h"

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

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t step;
    bool heartbeat_param_is_set;
    bool regpkg_param_is_set;
    bool dtu_is_wake_up;
    bool nettime_is_flash;
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
static cloud_protocol_ctrl_t cloud_protocol_ctrl;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_Init(void)
{
    memset(&cloud_protocol_ctrl, 0, sizeof(cloud_protocol_ctrl));
#if (CLOUD_PROTOCOL_SG_PROTOCOL_ENABLE == 1)
    Cloud_Protocol_Mqtt_init();
#endif
#if (CLOUD_PROTOCOL_DEFAULT_TCP_ID == TCP_ID_PROTOCOL_GAGA)
    Cloud_GagaProtocol_Init();
#endif
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
#if (CLOUD_PROTOCOL_SG_PROTOCOL_ENABLE == 1)
    if (status)
    {
        Cloud_Protocol_Mqtt_SetDeviceOnlineStatus(true);
    }
#endif
}

void Cloud_Protocol_FlashNetTime(void)
{
    cloud_protocol_ctrl.nettime_is_flash = true;
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
       Cloud_GagaProtocol_DeviceResetChecking();
#if (CLOUD_PROTOCOL_SG_PROTOCOL_ENABLE == 1)
       Cloud_Protocol_Mqtt_SetDeviceOnlineStatus(false);
#endif
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
#if (CLOUD_PROTOCOL_DEFAULT_TCP_ID == TCP_ID_PROTOCOL_SG)
    msg[1] = (uint8_t)TCP_ID_PROTOCOL_SG;
#elif (CLOUD_PROTOCOL_DEFAULT_TCP_ID == TCP_ID_PROTOCOL_GAGA)
    msg[1] = (uint8_t)TCP_ID_PROTOCOL_GAGA;
#else

#endif
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
    msg[1] = 0; // No need additional parameters
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
#if (CLOUD_PROTOCOL_SG_PROTOCOL_ENABLE == 1)
    Cloud_Protocol_Mqtt_MainProcess();
#endif
#if (CLOUD_PROTOCOL_GAGA_PROTOCOL_ENABLE == 1)
    Cloud_GagaProtocol_MainProcess();
#endif
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
