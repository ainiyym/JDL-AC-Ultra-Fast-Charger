#ifndef _YEECOM_XXX_DEVICE_H_
#define _YEECOM_XXX_DEVICE_H_
#include "YeeComxxx_At_Cfg.h"

typedef enum
{
    YEECOM_POWER_ON,
    YEECOM_DEVICE_RESET,
    YEECOM_DEVICE_RESET_POWER_ON,
    YEECOM_DEVICE_RESET_CMD,
    YEECOM_DEVICE_RESET_CH,
    YEECOM_NET_READY
} YeeCom_Device_Status_t;

typedef enum
{
    YEECOM_DEVICE_PARAM_RESET,
    YEECOM_DEVICE_PARAM_RESTART,
    YEECOM_DEVICE_PARAM_CFG_CENTER,
    YEECOM_DEVICE_PARAM_CH_MODE,
    YEECOM_DEVICE_PARAM_GPRS_MODE
} YeeCom_Device_Param_t;

extern void YeeCom_SetDeviceState(YeeCom_Device_Status_t id, uint8_t status);
extern uint8_t YeeCom_GetDeviceState(YeeCom_Device_Status_t id);
extern void YeeCom_SetDeviceParameters(YeeCom_Device_Param_t id, uint8_t status);
extern void YeeCom_DeviceRestart(void);
extern void YeeCom_SetDeviceInfo_sim(uint8_t sim_status);
extern void YeeCom_SetDeviceInfo_rssi(uint16_t rssi);
extern void YeeCom_SetDeviceInfo_iccid(const char* iccid);
extern void YeeCom_SetDeviceInfo_imei(const char* imei);
extern void YeeCom_SetDeviceInfo_gstate(uint8_t id, const uint8_t gstate);
extern void YeeCom_GetDeviceInfo(uint8_t* sim_status, uint16_t* rssi, char* iccid, char* imei, uint8_t* gstate, uint8_t* gstate_num);
extern void YeeCom_Init(void);
extern void YeeCom_MainFunc(void);
extern uint8_t YeeCom_AtCmd_Send(YeeCom_AT_Cmd_Get_Param_Type cmd_type, YeeCom_AT_Cmd cmd, const char *format, ...);
extern uint8_t YeeCom_At_DataPassthrougth(uint8_t channel, const uint8_t *data, uint16_t length);
#endif
