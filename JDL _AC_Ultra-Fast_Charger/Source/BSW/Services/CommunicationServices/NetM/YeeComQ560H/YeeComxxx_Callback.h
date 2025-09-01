#ifndef _YEECOM_XXX_CALLBACK_H_
#define _YEECOM_XXX_CALLBACK_H_
#include "YeeComxxx.h"
#include "stdint.h"
#include "string.h"
#include "STD_LogService.h"

#define YeeCom_Log(fmt, ...)                                            LOG_DEBUG(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define YeeCom_Err(fmt, ...)                                            LOG_ERROR(LOG_MODULE_AT, fmt, ##__VA_ARGS__)
#define YeeCom_Print_Hex(A, B)                                          LogService_Print_Hex_Array(LOG_MODULE_AT, A, B, 1)
/* oob cmd */
void YeeCom_At_OOB_Power_On_Callback(void *arg, char *buf, int buflen);
void YeeCom_At_OOB_Net_Ready_Callback(void *arg, char *buf, int buflen);
void YeeCom_At_OOB_Net_Reset_Callback(void *arg, char *buf, int buflen);
void YeeCom_At_OOB_Data_Passthrough_Callback(void *arg, char *buf, int buflen);

/* at set cmd */
void YeeCom_At_Set_SERVERn_Callback(void *arg, char *buf, int buflen);
void YeeCom_At_Set_Parameter_Callback(void *arg, char *buf, int buflen);

/* at get cmd */
void YeeCom_At_Get_SERVERnCallback(void *arg, char *buf, int buflen);
void YeeCom_At_Get_GPRSMODECallback(void *arg, char *buf, int buflen);
void YeeCom_At_Get_CHMODECallback(void *arg, char *buf, int buflen);
void YeeCom_At_Get_DBGMODECallback(void *arg, char *buf, int buflen);
void YeeCom_At_Get_UARTCallback(void *arg, char *buf, int buflen);
void YeeCom_At_Get_DFICallback(void *arg, char *buf, int buflen);
#endif
