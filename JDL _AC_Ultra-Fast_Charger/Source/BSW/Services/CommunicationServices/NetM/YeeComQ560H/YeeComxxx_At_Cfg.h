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

#define YEECOM_AT_CMD_SET_PARAM_COUNT       (17)
#define YEECOM_AT_CMD_GET_PARAM_COUNT       (19)

#define MCAL_USART_CHANNEL_YEECOM         (MCAL_USART1_CH)
#define YEECOM_DEFAULT_DFI_TIME_SET       (100) /* ms */
#define YEECOM_BAUDRATE_115200            (115200)
#define YEECOM_DEFAULT_BAUDRATE           (9600)
#define YEECOM_ENABLE_BAUDRATE            (YEECOM_BAUDRATE_115200) /* Enable setting baudrate */
#define YEECOM_DEFAULT_DATA_BITS          (8) /* 8 bits */
#define YEECOM_DEFAULT_STOP_BITS          (1) /* UART_STOPBITS_1 */
#define YEECOM_DEFAULT_PARITY             (0) /* 0: none, 1: odd, 2: even */
#define YEECOM_DEFAULT_FLOW_CONTROL       (0) /* 0: none, 1: RTS/CTS */
#define YEECOM_USART_CHECK_TIMEOUT        (5000) /* ms */
#define YEECOM_RESET_CHECK_TIMEOUT        (45000) /* ms */
#define YEECOM_USART_CHECK_RETRY_COUNT    (3)

extern const YeeCom_AT_OobCmd_ParameterCfg YeeCom_At_Cmd_OobParam[YEECOM_AT_CMD_OOBPARAM_COUNT];
extern const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Set_Param[YEECOM_AT_CMD_SET_PARAM_COUNT];
extern const YeeCom_AT_Cmd_ParameterCfg YeeCom_At_Cmd_Get_Param[YEECOM_AT_CMD_GET_PARAM_COUNT];

extern void YeeCom_DeinitUsart(void);
extern void YeeCom_ReInitUsart(uint32_t baudrate);
#endif
