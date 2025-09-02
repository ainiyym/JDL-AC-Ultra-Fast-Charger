#include "YeeComxxx.h"
#include "YeeComxxx_Callback.h"

typedef void (*YeeCom_at_recv_success_cb)(void *arg, char *buf, int buflen);
typedef void (*YeeCom_at_recv_fail_cb)(void *arg, char *buf, int buflen);

typedef enum
{
    YEECOM_STATE_LOADING,
} YeeComState_Enum;

typedef enum
{
    YEECOM_LOAD_STEP1,
} YeeComLoadStep_Enum;

typedef enum
{
	abcd
}YeeComNormalStep_Enum;

typedef struct
{
    const uint8_t *prefix;                     /* interested string. Must not be NULL. */
    const uint8_t *reply_success_postfix;      /* "Reply successfully" postfix */
    const uint8_t *reply_fail_postfix;         /* "Reply failed" postfix */
    YeeCom_at_recv_success_cb recv_success_cb; /* Callback function when receiving successful reply */
    YeeCom_at_recv_fail_cb recv_fail_cb;       /* Callback function when receiving failed reply */
} YeeCom_AT_Cmd_Get_Param_Rcv;

typedef struct
{
    YeeCom_AT_Cmd cmd;  /* cmd */
    const uint8_t *str; /* Instruction content */
    YeeCom_AT_Cmd_Get_Param_Rcv rcvCfg; /* Receive configuration */
} YeeCom_AT_Cmd_ParameterCfg;

typedef struct
{
    YeeCom_AT_Oob_enum cmd;                       /* cmd */
    const uint8_t *prefix;                   /* interested string. Must not be NULL. */
    const char *postfix;                     /* postfix string. Must not be NULL. */
    uint16_t preprocessing_data_len;         /* Length of preprocessing data */
    at_recv_cb cb;
    void *arg;
} YeeCom_AT_OobCmd_ParameterCfg;

/* AT oob command table */
const YeeCom_AT_OobCmd_ParameterCfg YeeCom_At_Cmd_OobParam[] =
{
    {YEECOM_AT_OOB_CMD_POWER_ON,            "+EIND:",       "\r\n",   YEECOM_OOB_CMD_POWER_ON_BUF_LEN,           YeeCom_At_OOB_Power_On_Callback, NULL},
    {YEECOM_AT_OOB_CMD_SIM_READY,           "+CSIM:",       "\r\n",   YEECOM_OOB_CMD_NET_READY_BUF_LEN,          YeeCom_At_OOB_Net_Ready_Callback, NULL},
    {YEECOM_AT_OOB_CMD_RESET,               "+SYSTEM:",     "\r\n",   YEECOM_OOB_CMD_RESET_BUF_LEN,              YeeCom_At_OOB_Net_Reset_Callback, NULL},
    {YEECOM_AT_OOB_CMD_DATA_PASSTHROUGH,    "RCVPORT",      "\r\n",   YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN,   YeeCom_At_OOB_Data_Passthrough_Callback, NULL},
};

/* AT set parameter command table */
const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Set_Param[] = 
{
    {YEECOM_AT_CMD_WORKING_MODE,                         "AT*SERVER%d=%d,%s,%s#\r\n",       {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_SERVERn_Callback,     YeeCom_At_Set_SERVERn_Callback}},
    {YEECOM_AT_CMD_GPRS_MODE,                            "AT*GPRSMODE=%d#\r\n",             {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_Parameter_Callback,   YeeCom_At_Set_Parameter_Callback}},
    {YEECOM_AT_CMD_CH_MODE,                              "AT*CHMODE=%d#\r\n",               {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_Parameter_Callback,   YeeCom_At_Set_Parameter_Callback}},
    {YEECOM_AT_CMD_DEBUG_MODE,                           "AT*DBGMODE=%d#\r\n",              {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_Parameter_Callback,   YeeCom_At_Set_Parameter_Callback}},
    {YEECOM_AT_CMD_USART_CFG,                            "AT*UART=%d,%d,%d,%d,%d#\r\n",     {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_Parameter_Callback,   YeeCom_At_Set_Parameter_Callback}},
    {YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME,            "AT*DFI=%d#\r\n",                  {NULL,    "OK\r\n",       "ERROR\r\n",    YeeCom_At_Set_Parameter_Callback,   YeeCom_At_Set_Parameter_Callback}}, /* "Unit: ms */
};

/* AT get parameter command table */
const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Get_Param[] = 
{
    {YEECOM_AT_CMD_WORKING_MODE,                    "AT*SERVER%d?\r\n",    {"+SERVER\r\n",   "OK\r\n", NULL, YeeCom_At_Get_SERVERnCallback,     NULL}},
    {YEECOM_AT_CMD_GPRS_MODE,                       "AT*GPRSMODE?\r\n",    {"+GPRSMODE\r\n", "OK\r\n", NULL, YeeCom_At_Get_GPRSMODECallback,    NULL}},
    {YEECOM_AT_CMD_CH_MODE,                         "AT*CHMODE?\r\n",      {"+CHMODE\r\n",   "OK\r\n", NULL, YeeCom_At_Get_CHMODECallback,      NULL}},
    {YEECOM_AT_CMD_DEBUG_MODE,                      "AT*DBGMODE?\r\n",     {"+DBGMODE\r\n",  "OK\r\n", NULL, YeeCom_At_Get_DBGMODECallback,     NULL}},
    {YEECOM_AT_CMD_USART_CFG,                       "AT*UART?\r\n",        {"+UART\r\n",     "OK\r\n", NULL, YeeCom_At_Get_UARTCallback,        NULL}},
    {YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME,       "AT*DFI?\r\n",         {"+DFI\r\n",      "OK\r\n", NULL, YeeCom_At_Get_DFICallback,         NULL}},
};

#define YEECOM_AT_CMD_SEND_BUF_SIZE             (128u)

typedef struct
{
	YeeComState_Enum         YeeComxxxStatus;  /* Manage state */
	YeeComLoadStep_Enum      LoadStep;         /* Load step */
	YeeComNormalStep_Enum    NormalStep;	   /* Normal step */
    uint8_t                  DeviceStatus;      /* Device status, Bit Control:YeeCom_AT_Oob_enum; 1: ready, 0: not ready */
	uint8_t                  ErrorCnt;         /* Error cnt */
	uint8_t                  ErrorState;       /* Error state */
    uint8_t                  AtCmdSendBuf[YEECOM_AT_CMD_SEND_BUF_SIZE];/* AT command send buffer */
    uint16_t                 AtCmdSendLen;     /* AT command send buffer length */
}YeeComxxx_Struct;

static YeeComxxx_Struct gv_YeeComxxx;

static void YeeCom_SetState(YeeComState_Enum state)
{
    gv_YeeComxxx.YeeComxxxStatus = state;
}

void YeeCom_SetDeviceStatus(YeeCom_AT_Oob_enum id, uint8_t status)  
{
    gv_YeeComxxx.DeviceStatus &= ~(1 << id);
    gv_YeeComxxx.DeviceStatus |= (status << id);
}

static void YeeCom_OobRegister(void)
{
    uint8_t ret = 0;
    const YeeCom_AT_OobCmd_ParameterCfg* oob_cmd_param = NULL;

    for (uint8_t i = 0; i < sizeof(YeeCom_At_Cmd_OobParam) / sizeof(YeeCom_AT_OobCmd_ParameterCfg); i++)
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


uint8_t YeeCom_AtCmd_Send(YeeCom_AT_Cmd_Get_Param_Type cmd_type, YeeCom_AT_Cmd cmd, const char *format, ...)
{
    uint8_t ret = 0;

    const uint8_t *str = NULL;
    at_recv_cb success_callback = NULL;
    at_recv_cb fail_callback = NULL;
    const atcmd_config_t *atcmdconfig = NULL;

    switch (cmd_type)
    {
        case YEECOM_AT_CMD_GET:
            str = YeeCom_At_Cmd_Get_Param[cmd].str;
            success_callback = YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.recv_success_cb;
            fail_callback = YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.recv_fail_cb;
            atcmdconfig = &(atcmd_config_t){(char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.prefix,
                                   (char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.reply_success_postfix,
                                   (char *)YeeCom_At_Cmd_Get_Param[cmd].rcvCfg.reply_fail_postfix};
            break;
        case YEECOM_AT_CMD_SET:
            str = YeeCom_At_Cmd_Set_Param[cmd].str;
            success_callback = YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.recv_success_cb;
            fail_callback = YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.recv_fail_cb;
            atcmdconfig = &(atcmd_config_t){(char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.prefix,
                                   (char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.reply_success_postfix,
                                   (char *)YeeCom_At_Cmd_Set_Param[cmd].rcvCfg.reply_fail_postfix};
            break;
        default:
            ret = 1;
            break;
    }

    va_list args;
    va_start(args, format);
    gv_YeeComxxx.AtCmdSendLen = HAL_Snprintf((char *)gv_YeeComxxx.AtCmdSendBuf, YEECOM_AT_CMD_SEND_BUF_SIZE, (const char *)str, args);
    va_end(args);

    if (gv_YeeComxxx.AtCmdSendLen <= 0)
    {
        return 2;
    }
    ret = at_send_wait_reply((const char*)gv_YeeComxxx.AtCmdSendBuf, (int)gv_YeeComxxx.AtCmdSendLen, success_callback, fail_callback, atcmdconfig);

    return ret;
}

void YeeCom_LoadHandle(void)
{
    switch(gv_YeeComxxx.LoadStep)
    {
        case YEECOM_LOAD_STEP1:
        {

            break;
        }
        default:
            break;
    }
}

void YeeCom_MainFunc(void)
{
    switch (gv_YeeComxxx.YeeComxxxStatus)
    {
        case YEECOM_STATE_LOADING:
        {
            YeeCom_LoadHandle();
            break;
        }
        default:
            break;
    }
}

void YeeCom_Init(void)
{
	memset((uint8_t*)(&gv_YeeComxxx), 0u, (uint16_t)(sizeof(gv_YeeComxxx) / sizeof(uint8_t)));
    at_parser_init();
    YeeCom_OobRegister();
}
