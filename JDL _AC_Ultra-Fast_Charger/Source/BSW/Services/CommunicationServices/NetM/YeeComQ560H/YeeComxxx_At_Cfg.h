#ifndef _YEECOM_XXX_AT_CFG_H_
#define _YEECOM_XXX_AT_CFG_H_

#include "YeeComxxx_At.h"
#include "YeeComxxx_Callback.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "at_parser.h"
#include "wrappers_os.h"

typedef enum
{
    YEECOM_AT_OOB_CMD_POWER_ON, /* OOB: power on */
    YEECOM_AT_OOB_CMD_SIM_READY, /* OOB: sim ready */
    YEECOM_AT_OOB_CMD_RESET, /* OOB: reset */
    YEECOM_AT_OOB_CMD_DATA_PASSTHROUGH, /* OOB: data passthrough */
} YeeCom_AT_Oob_enum;

typedef struct
{
    const char *prefix;                     /* interested string. Must not be NULL. */
    const char *reply_success_postfix;      /* "Reply successfully" postfix */
    const char *reply_fail_postfix;         /* "Reply failed" postfix */
    uint16_t reply_timeout;                 /* Reply timeout */
    at_recv_cb    recv_cb;                  /* Callback function when receiving successful reply */
} YeeCom_AT_Cmd_Get_Param_Rcv;

typedef struct
{
    YeeCom_AT_Cmd cmd;  /* cmd */
    const char *str; /* Instruction content */
    YeeCom_AT_Cmd_Get_Param_Rcv rcvCfg; /* Receive configuration */
} YeeCom_AT_Cmd_ParameterCfg;

typedef struct
{
    YeeCom_AT_Oob_enum cmd;                  /* cmd */
    const char *prefix;                      /* interested string. Must not be NULL. */
    const char *postfix;                     /* postfix string. Must not be NULL. */
    uint16_t preprocessing_data_len;         /* Length of preprocessing data */
    at_recv_cb cb;
    void *arg;
} YeeCom_AT_OobCmd_ParameterCfg;

#define YEECOM_AT_CMD_OOBPARAM_COUNT        (4)
#define YEECOM_AT_CMD_SET_PARAM_COUNT       (17)
#define YEECOM_AT_CMD_GET_PARAM_COUNT       (18)

extern const YeeCom_AT_OobCmd_ParameterCfg YeeCom_At_Cmd_OobParam[YEECOM_AT_CMD_OOBPARAM_COUNT];
extern const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_SET_PARAM_COUNT];
extern const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Get_Param[YEECOM_AT_CMD_GET_PARAM_COUNT];

#endif
