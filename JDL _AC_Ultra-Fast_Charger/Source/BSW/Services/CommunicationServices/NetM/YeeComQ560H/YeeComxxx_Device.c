#include "YeeComxxx_Device.h"
#include "YeeComxxx_Device_Cfg.h"
#include "FlashDB_AppM.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#define YEECOM_AT_CMD_SEND_BUF_SIZE             (YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN)

typedef enum
{
    YEECOM_STATE_LOADING,
    YEECOM_STATE_READY,
    YEECOM_STATE_ERROR
} YeeComState_Enum;

typedef enum
{
    YEECOM_LOAD_STEP1,
    YEECOM_LOAD_STEP2,
    YEECOM_LOAD_STEP3
} YeeComLoadStep_Enum;

typedef struct
{
    volatile  uint16_t DeviceState;    /* Device state, Bit Control:YeeCom_Device_Status_t; 1: ready, 0: not ready */
    volatile  uint16_t ParameterState; /* Device parameter state, Bit Control:YeeCom_Device_Param_t; 1: OK, 0: ERR */
} YeeCom_AT_Device_Struct;

typedef struct
{
    YeeComState_Enum YeeComxxxState;                /* Manage state */
    YeeComLoadStep_Enum LoadStep;                   /* Load step */
    YeeCom_AT_Device_Struct Device;                 /* Device status and parameter state */
    YeeCom_AT_Cmd CurrentCmd;                       /* AT command */
    char AtCmdSendBuf[YEECOM_AT_CMD_SEND_BUF_SIZE]; /* AT command send buffer */
    uint8_t ErrorCnt;                               /* Error cnt */
    uint8_t ErrorState;                             /* Error state */
    uint16_t TimerCnt;                              /* Timer cnt */
    SemaphoreHandle_t SendMutex;                    /* SendMutex for thread safety */
} YeeComxxx_Struct;

static void YeeCom_SetDeviceInitFlag(bool flag);

static YeeComxxx_Struct gv_YeeComxxx;
extern uint8_t YeeComxxx_Device_Init_Flag;

uint8_t YeeCom_AtCmd_Send(YeeCom_AT_Cmd_Get_Param_Type cmd_type, YeeCom_AT_Cmd cmd, const char *format, ...)
{
    uint8_t ret = 0;

    const char *str = NULL;
    at_recv_cb cb = NULL;
    const atcmd_config_t *atcmdconfig = NULL;
    uint16_t atcmd_len = 0;
    uint16_t reply_timeout = 0;

    switch (cmd_type)
    {
        case YEECOM_AT_CMD_GET:
            str = YeeCom_At_Cmd_Get_Param[cmd].str;
            cb = YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.recv_cb;
            reply_timeout = YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.reply_timeout;
            atcmdconfig = &(atcmd_config_t){(char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.prefix,
                                   (char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.reply_success_postfix,
                                   (char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.reply_fail_postfix};
            break;
        case YEECOM_AT_CMD_SET:
            str = YeeCom_At_Cmd_Set_Param[cmd].str;
            cb = YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.recv_cb;
            reply_timeout = YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.reply_timeout;
            atcmdconfig = &(atcmd_config_t){(char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.prefix,
                                   (char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.reply_success_postfix,
                                   (char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.reply_fail_postfix};
            break;
        default:
            ret = 1;
            break;
    }

    if (xSemaphoreTake(gv_YeeComxxx.SendMutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        memset(gv_YeeComxxx.AtCmdSendBuf, 0, YEECOM_AT_CMD_SEND_BUF_SIZE);

        va_list args;
        va_start(args, format);
        atcmd_len = vsnprintf((char *)gv_YeeComxxx.AtCmdSendBuf, YEECOM_AT_CMD_SEND_BUF_SIZE, (const char *)str, args);
        va_end(args);

        if (atcmd_len <= 0)
        {
            xSemaphoreGive(gv_YeeComxxx.SendMutex);
            return 2;
        }
        YeeCom_Log("<%s> Send AT cmd: %s", __func__, gv_YeeComxxx.AtCmdSendBuf);
        ret = at_send_wait_reply((const char *)gv_YeeComxxx.AtCmdSendBuf, (int)atcmd_len, reply_timeout, cb, atcmdconfig);
        xSemaphoreGive(gv_YeeComxxx.SendMutex);
    }
    else
    {
        YeeCom_Err("Error: %s Failed to take YeeCom mutex\r\n", __func__);
        ret = 3;
    }

    return ret;
}

uint8_t YeeCom_At_DataPassthrougth(uint8_t channel, const uint8_t *data, uint16_t length)
{
    uint8_t ret = 0;

    uint16_t atcmd_len = 0;
    const char *str = NULL;

    if (xSemaphoreTake(gv_YeeComxxx.SendMutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        memset(gv_YeeComxxx.AtCmdSendBuf, 0, YEECOM_AT_CMD_SEND_BUF_SIZE);

        str = "SNDPORT%d=\r\n";
        atcmd_len = snprintf((char *)gv_YeeComxxx.AtCmdSendBuf, YEECOM_AT_CMD_SEND_BUF_SIZE, str, channel);

        if (atcmd_len <= 0)
        {
            xSemaphoreGive(gv_YeeComxxx.SendMutex);
            return 2;
        }
        if (length < YEECOM_AT_CMD_SEND_BUF_SIZE - atcmd_len)
        {
            memcpy((char *)&gv_YeeComxxx.AtCmdSendBuf[atcmd_len], (const char *)data, length);
            ret = at_send_no_reply((const char *)gv_YeeComxxx.AtCmdSendBuf, (int)(atcmd_len + length));
        }
        else
        {
            ret = 2; // Data too large
        }
        xSemaphoreGive(gv_YeeComxxx.SendMutex);
    }
    else
    {
        YeeCom_Err("Error: %s Failed to take YeeCom mutex\r\n", __func__);
        ret = 3;
    }

    return ret;
}

void YeeCom_SetDeviceState(YeeCom_Device_Status_t id, uint8_t status)  
{
    if (0 == status)
    {
        YEECOM_ATOMIC_CLR_BIT(gv_YeeComxxx.Device.DeviceState, (1 << id));
    }
    else
    {
        YEECOM_ATOMIC_SETH_BIT(gv_YeeComxxx.Device.DeviceState, (1 << id));
    }
    // gv_YeeComxxx.Device.DeviceState &= ~(1 << id);
    // gv_YeeComxxx.Device.DeviceState |= (status << id);
}

void YeeCom_SetDeviceParameters(YeeCom_Device_Param_t id, uint8_t status)  
{
    if (0 == status)
    {
        YEECOM_ATOMIC_CLR_BIT(gv_YeeComxxx.Device.ParameterState, (1 << id));
    }
    else
    {
        YEECOM_ATOMIC_SETH_BIT(gv_YeeComxxx.Device.ParameterState, (1 << id));
    }
    // gv_YeeComxxx.Device.ParameterState &= ~(1 << id);
    // gv_YeeComxxx.Device.ParameterState |= (status << id);
}

uint8_t YeeCom_GetDeviceState(YeeCom_Device_Status_t id)
{
    return (gv_YeeComxxx.Device.DeviceState >> id) & 0x01;
}

static uint8_t YeeCom_GetDeviceParameters(YeeCom_Device_Param_t id)
{
    return (gv_YeeComxxx.Device.ParameterState >> id) & 0x01;
}

static void YeeCom_SetState(YeeComState_Enum state)
{
    gv_YeeComxxx.YeeComxxxState = state;
}

static void YeeCom_SetLoadStep(YeeComLoadStep_Enum state)
{
    gv_YeeComxxx.LoadStep = state;
}

static void YeeCom_ResetDevice(void)
{
    gv_YeeComxxx.Device.DeviceState = 0;
    gv_YeeComxxx.Device.ParameterState = 0;
}

static void YeeCom_OobRegister(void)
{
    uint8_t ret = 0;
    const YeeCom_AT_OobCmd_ParameterCfg* oob_cmd_param = NULL;

    for (uint8_t i = 0; i < YEECOM_AT_CMD_OOBPARAM_COUNT; i++)
    {
        oob_cmd_param = &YeeCom_At_Cmd_OobParam[i];
        ret = at_register_callback((char *)oob_cmd_param->prefix,
                                    (char *)oob_cmd_param->postfix,
                                    (int)oob_cmd_param->preprocessing_data_len,
                                    oob_cmd_param->cb,
                                    NULL);
        if (ret != 0)
        {
            YeeCom_Err("Error: Failed to register OOB command callback for prefix: %s\r\n", oob_cmd_param->prefix);
        }
        
    }
}

static void YeeCom_ParameterTimeoutJudgy(uint16_t reply_timeout)
{
    while (gv_YeeComxxx.TimerCnt > reply_timeout / YEECOM_PERIOD_MS)
    {
        gv_YeeComxxx.TimerCnt = 0;
        gv_YeeComxxx.ErrorCnt++;
        YeeCom_SetState(YEECOM_STATE_ERROR);
        YeeCom_Log("<%s> YeeComxxxState goto error..\r\n", __func__);
        break;
    }
    gv_YeeComxxx.TimerCnt++;
}

static void YeeCom_ClearTimeout(void)
{
    gv_YeeComxxx.TimerCnt = 0;
}

static void YeeCom_SetCmd(YeeCom_AT_Cmd cmd)
{
    gv_YeeComxxx.CurrentCmd = cmd;
}

static void YeeCom_DevicePowerON(void)
{
    if (YeeCom_GetDeviceState(YEECOM_POWER_ON))
    {
        // Power ON the device
        vTaskDelay(pdMS_TO_TICKS(8000));
        YeeCom_SetLoadStep(YEECOM_LOAD_STEP2);
    }
}

static void YeeCom_DeviceReset(void)
{ 
    YeeCom_ResetDevice();
    // Reset device parameters
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_DEVICE_RESET, NULL, (uint32_t)(123456));
    YeeCom_SetLoadStep(YEECOM_LOAD_STEP3);
}

static void YeeCom_SetDefaultCenterWorkingMode(void)
{
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_WORKING_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_WORKING_MODE, NULL, 0, YEECOM_DEFAULT_NET_TYPE, YEECOM_DEFAULT_REMOTE_IP, YEECOM_DEFAULT_REMOTE_PORT);
}

static void YeeCom_SetDefaultCHMode(void)
{
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_CH_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_CH_MODE, NULL, YEECOM_CENTRAL_MODE_MULTI_HOMED_CONNECTION_STANDALONE);
}

static void YeeCom_SetDefaultGPRSMode(void)
{
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_GPRS_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_GPRS_MODE, NULL, YEECOM_ONLINE_MODE_WAKE_ONLINE);
}

static void YeeCom_CfgParameter(void)
{
    while (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET_CMD) && YeeCom_GetDeviceState(YEECOM_SIM_READY))
    {
        YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_CMD, 0);
        YeeCom_SetDeviceState(YEECOM_SIM_READY, 0);
        YeeCom_SetCmd(YEECOM_AT_CMD_WORKING_MODE);
        // Configure device parameters
        YeeCom_ClearTimeout();
        YeeCom_SetDefaultCenterWorkingMode();

        YeeCom_Log("<%s> cmd: %d\r\n", __func__, gv_YeeComxxx.CurrentCmd);
        break;
    }
    while (YeeCom_GetDeviceParameters(YEECOM_DEVICE_PARAM_CFG_CENTER))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CFG_CENTER, 0);
        YeeCom_SetCmd(YEECOM_AT_CMD_CH_MODE);
        // Configure default channel mode
        YeeCom_ClearTimeout();
        YeeCom_SetDefaultCHMode();

        YeeCom_Log("<%s> cmd: %d\r\n", __func__, gv_YeeComxxx.CurrentCmd);
        break;
    }
    while (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET_CH) && YeeCom_GetDeviceParameters(YEECOM_DEVICE_PARAM_CH_MODE) && YeeCom_GetDeviceState(YEECOM_SIM_READY))
    {
        YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_CH, 0);
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CH_MODE, 0);
        YeeCom_SetCmd(YEECOM_AT_CMD_GPRS_MODE);
        // Configure default GPRS mode
        YeeCom_ClearTimeout();
        YeeCom_SetDefaultGPRSMode();

        YeeCom_Log("<%s> cmd: %d\r\n", __func__, gv_YeeComxxx.CurrentCmd);
        break;
    }
    while (YeeCom_GetDeviceParameters(YEECOM_DEVICE_PARAM_GPRS_MODE))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_GPRS_MODE, 0);
        // All parameters are configured successfully
        YeeCom_SetState(YEECOM_STATE_READY);
        YeeCom_SetDeviceState(YEECOM_DEVICE_RESET, 0);
        YeeCom_SetDeviceInitFlag(true);

        YeeCom_Log("<%s> YeeComxxxState goto ready..\r\n", __func__);
        YeeCom_ClearTimeout();
        break;
    }
}

static void YeeCom_LoadHandle(void)
{
    switch(gv_YeeComxxx.LoadStep)
    {
        case YEECOM_LOAD_STEP1:
        {
            YeeCom_DevicePowerON();
            break;
        }
        case YEECOM_LOAD_STEP2:
        {
            YeeCom_DeviceReset();
            break;
        }
        case YEECOM_LOAD_STEP3:
        {
            YeeCom_CfgParameter();
            break;
        }
        default:
            break;
    }
}

static void YeeCom_ErrorHandle(void)
{
    // Handle error state
    if (gv_YeeComxxx.ErrorCnt < YEECOM_CFG_PARAMETER_MAX_ERROR)
    {
        YeeCom_SetState(YEECOM_STATE_LOADING);
        YeeCom_SetLoadStep(YEECOM_LOAD_STEP3);
        switch (gv_YeeComxxx.CurrentCmd)
        {
            case YEECOM_AT_CMD_WORKING_MODE:
                YeeCom_SetDefaultCenterWorkingMode();
                break;
            case YEECOM_AT_CMD_GPRS_MODE:
                YeeCom_SetDefaultGPRSMode();
                break;
            case YEECOM_AT_CMD_CH_MODE:
                YeeCom_SetDefaultCHMode();
                break;
            default:
                break;
        }
    }
    else
    {
        YeeCom_SetState(YEECOM_STATE_LOADING);
        YeeCom_SetLoadStep(YEECOM_LOAD_STEP2);
        gv_YeeComxxx.ErrorCnt = 0;
        YeeCom_Log("<%s> YeeComxxx reset..\r\n", __func__);
    }
}

static void YeeCom_ReadyHandle(void)
{
    while (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET))
    {
        YeeCom_ResetDevice();
        YeeCom_Log("<%s> YeeComxxx reset..\r\n", __func__);
        return;
    }

    if (YeeComxxx_Device_Init_Flag && YeeCom_GetDeviceState(YEECOM_SIM_READY))
    {
        YeeCom_SetDeviceState(YEECOM_NET_READY, 1);
    }
    else
    {
        YeeCom_SetDeviceState(YEECOM_NET_READY, 0);
    }
}

static void YeeCom_SetDeviceInitFlag(bool flag)
{
    YeeComxxx_Device_Init_Flag = flag;
    FlashDB_WriteValue(FLASHDB_KV_M4G_DEVICE_INIT_FLAG, (uint8_t *)&YeeComxxx_Device_Init_Flag, sizeof(YeeComxxx_Device_Init_Flag));
}

void YeeCom_MainFunc(void)
{
    switch (gv_YeeComxxx.YeeComxxxState)
    {
        case YEECOM_STATE_LOADING:
        {
            YeeCom_LoadHandle();
            break;
        }
        case YEECOM_STATE_READY:
        {
            YeeCom_ReadyHandle();
            break;
        }
        case YEECOM_STATE_ERROR:
        {
            YeeCom_ErrorHandle();
            break;
        }
        default:
            break;
    }
}

void YeeCom_Init(void)
{
	memset((uint8_t*)(&gv_YeeComxxx), 0u, (uint16_t)(sizeof(gv_YeeComxxx) / sizeof(uint8_t)));
    gv_YeeComxxx.SendMutex = xSemaphoreCreateMutex();
    if (gv_YeeComxxx.SendMutex == NULL)
    {
        YeeCom_Err("Error: Failed to create YeeCom mutex\r\n");
        return;
    }
    YeeCom_OobRegister();
    FlashDB_ReadValue(FLASHDB_KV_M4G_DEVICE_INIT_FLAG, (uint8_t*)&YeeComxxx_Device_Init_Flag, sizeof(YeeComxxx_Device_Init_Flag), NULL);
    YeeCom_Log("<%s> YeeComxxx_Device_Init_Flag: %d\r\n", __func__, YeeComxxx_Device_Init_Flag);
    if (1 != YeeComxxx_Device_Init_Flag)
    {
        if (0 != YeeComxxx_Device_Init_Flag)
        {
            YeeCom_SetDeviceInitFlag(false);
        }
        YeeCom_SetState(YEECOM_STATE_LOADING);
    }
    else
    {
        YeeCom_SetState(YEECOM_STATE_READY);
    }
}
