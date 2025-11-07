#ifndef _TCP_CFG_H_
#define _TCP_CFG_H_
#include "STD_LogService.h"
#include "YeeComxxx_Device.h"

typedef enum
{
    TCP_ID_PROTOCOL_GAGA = 0,   /* https://gaga.yuyangdev.cn */
    TCP_ID_PROTOCOL_SG, /* mqtts.heclouds.com */
    TCP_ID_MAXIMUM
}tcp_id_enum;

#define CLOUD_PROTOCOL_GAGA_IP                                           ("150.158.17.77")
#define CLOUD_PROTOCOL_GAGA_PORT                                         (2030U)

#define CLOUD_PROTOCOL_SG_IP                                             ("183.230.40.16")
#define CLOUD_PROTOCOL_SG_PORT                                           (1883U)

#define Tcp_At_Cmd_Send(cmd_type, cmd, format, ...)                  YeeCom_AtCmd_Send(cmd_type, cmd, format, ##__VA_ARGS__)
#define Tcp_Data_Passthrough(ch, data, len)                          YeeCom_At_DataPassthrougth(ch, data, len)

#define Tcp_Trace(fmt, ...)                                          LOG_TRACE(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define Tcp_Err(fmt, ...)                                            LOG_ERROR(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define Tcp_Print_Hex(A, B)                                          LogService_Print_Hex_Array(LOG_MODULE_AT, A, B, 1)

#endif
