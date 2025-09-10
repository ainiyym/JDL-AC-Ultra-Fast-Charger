#ifndef _TCP_CFG_H_
#define _TCP_CFG_H_
#include "STD_LogService.h"
#include "YeeComxxx_Device.h"

#define Tcp_At_Cmd_Send(cmd_type, cmd, format, ...)                  YeeCom_AtCmd_Send(cmd_type, cmd, format, ##__VA_ARGS__)
#define Tcp_Data_Passthrough(ch, data, len)                          YeeCom_At_DataPassthrougth(ch, data, len)

#define Tcp_Trace(fmt, ...)                                          LOG_TRACE(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define Tcp_Err(fmt, ...)                                            LOG_ERROR(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define Tcp_Print_Hex(A, B)                                          LogService_Print_Hex_Array(LOG_MODULE_AT, A, B, 1)

#endif
