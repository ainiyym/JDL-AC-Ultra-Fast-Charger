#include "YeeComxxx_At_Cfg.h"

/* AT oob command table */
const YeeCom_AT_OobCmd_ParameterCfg YeeCom_At_Cmd_OobParam[YEECOM_AT_CMD_OOBPARAM_COUNT] =
{
    {YEECOM_AT_OOB_CMD_POWER_ON,            "+EIND:",               "\r\n",   YEECOM_OOB_CMD_POWER_ON_BUF_LEN,           YeeCom_At_OOB_Power_On_Callback, NULL},
    {YEECOM_AT_OOB_CMD_SIM_READY,           "+CSIM:",               "\r\n",   YEECOM_OOB_CMD_NET_READY_BUF_LEN,          YeeCom_At_OOB_Net_Ready_Callback, NULL},
    {YEECOM_AT_OOB_CMD_RESET,               "+SYSTEM",              "\r\n",   YEECOM_OOB_CMD_RESET_BUF_LEN,              YeeCom_At_OOB_Net_Reset_Callback, NULL},
    {YEECOM_AT_OOB_CMD_DATA_PASSTHROUGH,    "RCVPORT",              "\r\n",   YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN,   YeeCom_At_OOB_Data_Passthrough_Callback, NULL},
};

/* AT set parameter command table */
const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_SET_PARAM_COUNT] =
{
    { YEECOM_AT_CMD_WORKING_MODE,                  "AT*SERVER%d=%d,%s,%d#\r\n",         { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_SERVERn_Callback } },
    { YEECOM_AT_CMD_GPRS_MODE,                     "AT*GPRSMODE=%d#\r\n",               { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_GPRSMode_Callback } },
    { YEECOM_AT_CMD_CH_MODE,                       "AT*CHMODE=%d#\r\n",                 { NULL,           "OK\r\n",    "ERROR\r\n", 30000,         YeeCom_At_Set_CHMode_Callback } },
    { YEECOM_AT_CMD_HBTIME,                        "AT*HBTIME=%d#\r\n",                 { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_HBTime_Callback } },
    { YEECOM_AT_CMD_HBHEAD,                        "AT*HBHEAD=HEX%s#\r\n",              { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_HBHead_Callback } },
    { YEECOM_AT_CMD_REGPKG,                        "AT*REGPKG=%d#\r\n",                 { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_REGPKG_Callback } },
    { YEECOM_AT_CMD_REGHEAD,                       "AT*REGHEAD=%s#\r\n",                { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_REGHEAD_Callback } },
    { YEECOM_AT_CMD_DEBUG_MODE,                    "AT*DBGMODE=%d#\r\n",                { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_DebugMode_Callback } },
    { YEECOM_AT_CMD_USART_CFG,                     "AT*UART=%d,%d,%d,%d,%d#\r\n",       { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_USART_Callback } },
    { YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME,     "AT*DFI=%d#\r\n",                    { NULL,           "OK\r\n",    "ERROR\r\n", 5000,          YeeCom_At_Set_DFI_Callback } }, /* "Unit: ms" */
    { YEECOM_AT_CMD_DEVICE_RESET,                  "AT*RESET=%d#\r\n",                  { NULL,           "OK\r\n",    "ERROR\r\n", 30000,         YeeCom_At_Set_RESET_Callback } },
    { YEECOM_AT_CMD_DEVICE_RESTART,                "AT*RESTART#\r\n",                   { NULL,           "OK\r\n",    "ERROR\r\n", 30000,         YeeCom_At_Set_RESTART_Callback } },
    { YEECOM_AT_CMD_WAKEUP,                        "AT*WAKEUP#\r\n",                    { NULL,           "OK\r\n",    "ERROR\r\n", 30000,         YeeCom_At_Set_WAKEUP_Callback } },
};

/* AT get parameter command table */
const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Get_Param[YEECOM_AT_CMD_GET_PARAM_COUNT] =
{
    { YEECOM_AT_CMD_WORKING_MODE,                  "AT*SERVER%d?\r\n",                  { "+SERVER",       "OK",    NULL,       2000,             YeeCom_At_Get_SERVERnCallback } },
    { YEECOM_AT_CMD_GPRS_MODE,                     "AT*GPRSMODE?\r\n",                  { "+GPRSMODE",     "OK",    NULL,       2000,             YeeCom_At_Get_GPRSMODECallback } },
    { YEECOM_AT_CMD_CH_MODE,                       "AT*CHMODE?\r\n",                    { "+CHMODE",       "OK",    NULL,       2000,             YeeCom_At_Get_CHMODECallback } },
    { YEECOM_AT_CMD_HBTIME,                        "AT*HBTIME?\r\n",                    { "+HBTIME",       "OK",    NULL,       2000,             YeeCom_At_Get_HBTimeCallback } },
    { YEECOM_AT_CMD_HBHEAD,                        "AT*HBHEAD?\r\n",                    { "+HBHEAD",       "OK",    NULL,       2000,             YeeCom_At_Get_HBHeadCallback } },
    { YEECOM_AT_CMD_REGPKG,                        "AT*REGPKG?\r\n",                    { "+REGPKG",       "OK",    NULL,       2000,             YeeCom_At_Get_REGPKGCallback } },
    { YEECOM_AT_CMD_REGHEAD,                       "AT*REGHEAD?\r\n",                   { "+REGHEAD",      "OK",    NULL,       2000,             YeeCom_At_Get_REGHEADCallback } },
    { YEECOM_AT_CMD_DEBUG_MODE,                    "AT*DBGMODE?\r\n",                   { "+DBGMODE",      "OK",    NULL,       2000,             YeeCom_At_Get_DBGMODECallback } },
    { YEECOM_AT_CMD_USART_CFG,                     "AT*UART?\r\n",                      { "+UART",         "OK",    NULL,       2000,             YeeCom_At_Get_UARTCallback } },
    { YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME,     "AT*DFI?\r\n",                       { "+DFI",          "OK",    NULL,       2000,             YeeCom_At_Get_DFICallback } },
    { YEECOM_AT_CMD_ICCID,                         "AT*ICCID?\r\n",                     { "+ICCID",        "OK",    NULL,       3000,             YeeCom_At_Get_ICCIDCallback } },
    { YEECOM_AT_CMD_IMEI,                          "AT*IMEI?\r\n",                      { "+IMEI",         "OK",    NULL,       3000,             YeeCom_At_Get_IMEICallback } },
    { YEECOM_AT_CMD_RSSI,                          "AT*CSQ?\r\n",                       { "+CSQ",          "OK",    NULL,       2000,             YeeCom_At_Get_RSSICallback } },
};
