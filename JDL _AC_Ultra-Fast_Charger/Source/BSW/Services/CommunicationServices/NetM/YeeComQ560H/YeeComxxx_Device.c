#include "YeeComxxx_Device.h"
#include "YeeComxxx_Device_Cfg.h"
#include "FlashDB_AppM.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "Cloud_Cfg.h"

#define YEECOM_AT_CMD_SEND_BUF_SIZE             (YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN)

typedef enum
{
    YEECOM_STATE_LOADING,
    YEECOM_STATE_GETTING_CONST_INFO,
    YEECOM_STATE_CONFIRM_INFO,
    YEECOM_STATE_READY,
    YEECOM_STATE_RUNNING,
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

typedef struct
{
    uint8_t SimReadyStatus;                         /* device sim is valid:1 ,invalid:0 */
    uint16_t Rssi;                                  /* Signal Strength */
    char ICCID[YEECOM_ICCID_LENGTH + 1];            /* SIM ICCID */
    char IMEI[YEECOM_IMEI_LENGTH + 1];              /* device imei */
    char DTUID[YEECOM_DTUID_LENGTH + 1];            /* device DTUID */
    uint8_t GState[YEECOM_GSTATE_NUMBER];           /* The online status of the connected server: 0-disconnected, 1-connected */            
}YeeComxxx_DeviceInfo_struct;


static void YeeCom_SetDeviceInitFlag(bool flag);

static YeeComxxx_Struct gv_YeeComxxx;
static YeeComxxx_DeviceInfo_struct gv_YeeComxxx_device_info;
extern uint8_t YeeComxxx_Device_Init_Flag;

uint8_t YeeCom_AtCmd_Send(YeeCom_AT_Cmd_Get_Param_Type cmd_type, YeeCom_AT_Cmd cmd, const char *format, ...)
{
    uint8_t ret = 0;

    const char *str = NULL;
    at_recv_cb cb = NULL;
    const atcmd_config_t *atcmdconfig = NULL;
    YeeCom_AT_Cmd_ParameterCfg* atcmd_param = NULL;
    uint16_t atcmd_len = 0;
    uint16_t reply_timeout = 0;

    switch (cmd_type)
    {
        case YEECOM_AT_CMD_GET:
            for (YeeCom_AT_Cmd  id = YEECOM_AT_CMD_WORKING_MODE; id < YEECOM_AT_CMD_GET_PARAM_COUNT; id++)
            {
                if (YeeCom_At_Cmd_Get_Param[id].cmd == cmd)
                {
                    atcmd_param = (YeeCom_AT_Cmd_ParameterCfg *)&YeeCom_At_Cmd_Get_Param[id];
                    break;
                }
            }
            break;
        case YEECOM_AT_CMD_SET:
            for (YeeCom_AT_Cmd id = YEECOM_AT_CMD_WORKING_MODE; id < YEECOM_AT_CMD_SET_PARAM_COUNT; id++)
            {
                if (YeeCom_At_Cmd_Set_Param[id].cmd == cmd)
                {
                    atcmd_param = (YeeCom_AT_Cmd_ParameterCfg *)&YeeCom_At_Cmd_Set_Param[id];
                    break;
                }
            }
            break;
        default:
            ret = 1;
            break;
    }
    if (NULL == atcmd_param)
    {
        return 1;
    }

    str = atcmd_param->str;
    cb = atcmd_param->rcvCfg.recv_cb;
    reply_timeout = atcmd_param->rcvCfg.reply_timeout;
    atcmdconfig = &(atcmd_config_t){(char *)atcmd_param->rcvCfg.prefix,
                                    (char *)atcmd_param->rcvCfg.reply_success_postfix,
                                    (char *)atcmd_param->rcvCfg.reply_fail_postfix};
    if (xSemaphoreTake(gv_YeeComxxx.SendMutex, pdMS_TO_TICKS(reply_timeout)) == pdTRUE)
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
        YeeCom_Log("<%s> Send AT cmd:%s", __func__, gv_YeeComxxx.AtCmdSendBuf);
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

        str = "SNDPORT%d=";
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
            YeeCom_Log("<%s> result:%d data:", __func__, ret);
            YeeCom_Print_Hex(data, length);
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

void YeeCom_DeviceRestart(void)
{
    YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_RESTART, 0);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_DEVICE_RESTART, NULL);
}

void YeeCom_SetDeviceInfo_sim(uint8_t sim_status)
{
    gv_YeeComxxx_device_info.SimReadyStatus = sim_status;
    YeeCom_Log("<%s> SIM Ready Status: %d\r\n", __func__, gv_YeeComxxx_device_info.SimReadyStatus);
}

void YeeCom_SetDeviceInfo_rssi(uint16_t rssi)
{
    gv_YeeComxxx_device_info.Rssi = rssi;
    YeeCom_Log("<%s> RSSI: %d\r\n", __func__, gv_YeeComxxx_device_info.Rssi);
}

void YeeCom_SetDeviceInfo_iccid(const char* iccid)
{
    if (iccid != NULL)
    {
        memcpy(&gv_YeeComxxx_device_info.ICCID, iccid, YEECOM_ICCID_LENGTH);
        gv_YeeComxxx_device_info.ICCID[YEECOM_ICCID_LENGTH] = '\0';
        YeeCom_Log("<%s> ICCID: %s\r\n", __func__, gv_YeeComxxx_device_info.ICCID);
    }
}

void YeeCom_SetDeviceInfo_imei(const char* imei)
{
    if (imei != NULL)
    {
        memcpy(&gv_YeeComxxx_device_info.IMEI, imei, YEECOM_IMEI_LENGTH);
        gv_YeeComxxx_device_info.IMEI[YEECOM_IMEI_LENGTH] = '\0';
        YeeCom_Log("<%s> IMEI: %s\r\n", __func__, gv_YeeComxxx_device_info.IMEI);
    }
}

void YeeCom_SetDeviceInfo_dtuid(const char* dtuid)
{
    if (dtuid != NULL)
    {
        memcpy(&gv_YeeComxxx_device_info.DTUID, dtuid, YEECOM_DTUID_LENGTH);
        gv_YeeComxxx_device_info.DTUID[YEECOM_DTUID_LENGTH] = '\0';
        YeeCom_Log("<%s> DTUID: %s\r\n", __func__, gv_YeeComxxx_device_info.DTUID);
    }
}

void YeeCom_SetDeviceInfo_gstate(uint8_t id, const uint8_t gstate)
{
    if (id < YEECOM_GSTATE_NUMBER)
    {
        gv_YeeComxxx_device_info.GState[id] = gstate;
        YeeCom_Log("<%s> GState[%d]: %d\r\n", __func__, id, gv_YeeComxxx_device_info.GState[id]);
    }
}

void YeeCom_GetDeviceInfo(uint8_t* sim_status, uint16_t* rssi, char* iccid, char* imei, uint8_t* gstate, uint8_t* gstate_num)
{
    if (sim_status != NULL)
    {
        *sim_status = gv_YeeComxxx_device_info.SimReadyStatus;
    }
    if (rssi != NULL)
    {
        *rssi = gv_YeeComxxx_device_info.Rssi;
    }
    if (iccid != NULL)
    {
        memcpy(iccid, &gv_YeeComxxx_device_info.ICCID, YEECOM_ICCID_LENGTH);
        iccid[YEECOM_ICCID_LENGTH] = '\0';
    }
    if (imei != NULL)
    {
        memcpy(imei, &gv_YeeComxxx_device_info.IMEI, YEECOM_IMEI_LENGTH);
        imei[YEECOM_IMEI_LENGTH] = '\0';
    }
    if (gstate != NULL)
    {
        memcpy(gstate, &gv_YeeComxxx_device_info.GState, YEECOM_GSTATE_NUMBER);
        if (gstate_num != NULL)
        {
            *gstate_num = YEECOM_GSTATE_NUMBER;
        }
    }
}

void YeeCom_GetDeviceDTUID(char* dtuid)
{
    if (dtuid != NULL)
    {
        memcpy(dtuid, &gv_YeeComxxx_device_info.DTUID, YEECOM_DTUID_LENGTH);
        dtuid[YEECOM_DTUID_LENGTH] = '\0';
    }
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
    gv_YeeComxxx_device_info.SimReadyStatus = 0;
    memset(gv_YeeComxxx_device_info.GState, 0xff, YEECOM_GSTATE_NUMBER);
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
    at_at_register_set_app_protocol_bit_num_callback(YEECOM_AT_OOB_CMD_DATA_PASSTHROUGH);
}

static void YeeCom_SetDeviceInitFlag(bool flag)
{
    if (flag != YeeComxxx_Device_Init_Flag)
    {
        YeeComxxx_Device_Init_Flag = flag;
        FlashDB_WriteValue(FLASHDB_KV_M4G_DEVICE_INIT_FLAG, (uint8_t *)&YeeComxxx_Device_Init_Flag, sizeof(YeeComxxx_Device_Init_Flag));
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
    YeeCom_ClearTimeout();
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_WORKING_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_WORKING_MODE, NULL, 0, YEECOM_DEFAULT_NET_TYPE, YEECOM_DEFAULT_REMOTE_IP, YEECOM_DEFAULT_REMOTE_PORT);
    vTaskDelay(pdMS_TO_TICKS(50));
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_WORKING_MODE, NULL, 0);
}

static void YeeCom_SetDefaultCHMode(void)
{
    YeeCom_ResetDevice();
    YeeCom_ClearTimeout();
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_CH_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_CH_MODE, NULL, YEECOM_CENTRAL_MODE_MULTI_HOMED_CONNECTION_STANDALONE);
}

static void YeeCom_SetDefaultGPRSMode(void)
{
    YeeCom_ClearTimeout();
    YeeCom_ParameterTimeoutJudgy(YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_GPRS_MODE].rcvCfg.reply_timeout);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_GPRS_MODE, NULL, YEECOM_ONLINE_MODE_KEEP_ALIVE);
}

static void YeeCom_CfgParameter(void)
{
    while (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET_CMD) && gv_YeeComxxx_device_info.SimReadyStatus)
    {
        YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_CMD, 0);
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
    while (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET_CH) && YeeCom_GetDeviceParameters(YEECOM_DEVICE_PARAM_CH_MODE) && gv_YeeComxxx_device_info.SimReadyStatus)
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
        YeeCom_SetState(YEECOM_STATE_GETTING_CONST_INFO);
        YeeCom_Log("<%s> YeeComxxxState goto get const_info\r\n", __func__);
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
    uint8_t msg[2] = {0};
    if (YeeComxxx_Device_Init_Flag && gv_YeeComxxx_device_info.SimReadyStatus)
    {
        YeeCom_SetDeviceState(YEECOM_NET_READY, 1);
        msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_DEVICE_READY;
        msg[1] = (uint8_t)CLOUD_DEVICE_STATUS_READY;
        CloudNet_MessageBuffer_SendMessage((const uint8_t *)&msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
        YeeCom_Log("<%s> YeeComxxxState goto running..\r\n", __func__);
        YeeCom_SetState(YEECOM_STATE_RUNNING);
    }
}

static void YeeCom_RunningHandle(void)
{
    uint8_t msg[2] = {0};
    if (YeeCom_GetDeviceState(YEECOM_DEVICE_RESET))
    {
        YeeCom_SetDeviceState(YEECOM_NET_READY, 0);
        YeeCom_ResetDevice();
        msg[0] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_DEVICE_READY;
        msg[1] = (uint8_t)CLOUD_DEVICE_STATUS_INIT;
        CloudNet_MessageBuffer_SendMessage((const uint8_t *)&msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
        YeeCom_SetState(YEECOM_STATE_GETTING_CONST_INFO);
        YeeCom_Log("<%s> YeeComxxx reset..\r\n", __func__);
        return;
    }
}

static void YeeCom_GetDeviceConstInfoHandle(void)
{
    if (gv_YeeComxxx_device_info.SimReadyStatus)
    {
        // Get device constant information such as ICCID, IMEI, RSSI, etc.
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_ICCID, NULL);
        vTaskDelay(pdMS_TO_TICKS(50));
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_IMEI, NULL);
        YeeCom_SetDeviceState(YEECOM_DEVICE_RESET, 0);
        YeeCom_SetState(YEECOM_STATE_CONFIRM_INFO);
    }
}

static void YeeCom_ConfirmInfoHandle(void)
{
    if (strlen(gv_YeeComxxx_device_info.ICCID) > 0 && strlen(gv_YeeComxxx_device_info.IMEI) > 0)
    {
        YeeCom_Log("ICCID: %s IMEI: %s\r\n", gv_YeeComxxx_device_info.ICCID, gv_YeeComxxx_device_info.IMEI);
        YeeCom_SetDeviceInitFlag(true);
        YeeCom_SetState(YEECOM_STATE_READY);
        YeeCom_Log("<%s> YeeComxxxState goto ready..\r\n", __func__);
    }
    else
    {
        YeeCom_SetState(YEECOM_STATE_GETTING_CONST_INFO);
    }
}

static void YeeCom_PeriodicHandle(void)
{
    // Handle periodic tasks
    if(1 == YeeCom_GetDeviceState(YEECOM_NET_READY))
    {
        while (gv_YeeComxxx.TimerCnt++ > YEECOM_PERIODIC_TASK_PERIOD) // 60s
        {
            YeeCom_ClearTimeout();
            // Get RSSI periodically
            YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_RSSI, NULL);
            vTaskDelay(pdMS_TO_TICKS(50));
            YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_GSTATE, NULL);
            break;
        }
    }
    // judgy gstate
    while(gv_YeeComxxx.TimerCnt++ % YEECOM_PERIODIC_TASK_JUDGY_PERIOD == 0) // 10s
    {
        for (uint8_t i = 0; i < YEECOM_GSTATE_NUMBER; i++)
        {
            if (gv_YeeComxxx_device_info.GState[i] == 0)
            {
                YeeCom_DeviceRestart();
                YeeCom_Log("<%s> YeeComxxx reset..\r\n", __func__);
                break;
            }
        }
    }
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
        case YEECOM_STATE_GETTING_CONST_INFO:
        {
            YeeCom_GetDeviceConstInfoHandle();
            break;
        }
        case YEECOM_STATE_CONFIRM_INFO:
        {
            YeeCom_ConfirmInfoHandle();
            break;
        }
        case YEECOM_STATE_READY:
        {
            YeeCom_ReadyHandle();
            break;
        }
        case YEECOM_STATE_RUNNING:
        {
            YeeCom_RunningHandle();
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
    YeeCom_PeriodicHandle();
}

void YeeCom_Init(void)
{
    memset((uint8_t *)(&gv_YeeComxxx), 0u, (uint16_t)(sizeof(gv_YeeComxxx) / sizeof(uint8_t)));
    memset((uint8_t *)(&gv_YeeComxxx_device_info), 0u, (uint16_t)(sizeof(gv_YeeComxxx_device_info) / sizeof(uint8_t)));
    memset(gv_YeeComxxx_device_info.GState, 0xff, YEECOM_GSTATE_NUMBER);
    gv_YeeComxxx.SendMutex = xSemaphoreCreateMutex();
    if (gv_YeeComxxx.SendMutex == NULL)
    {
        YeeCom_Err("Error: Failed to create YeeCom mutex\r\n");
        return;
    }
    YeeCom_OobRegister();

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
        YeeCom_SetState(YEECOM_STATE_GETTING_CONST_INFO);
    }
}
