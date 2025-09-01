/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef _AT_PARSER_H_
#define _AT_PARSER_H_

#include "infra_config.h"
#include "Mcal_Usart_Cfg.h"

/* uart config */
#define AT_UART_PORT         MCAL_USART1_CH
#define AT_UART_LINUX_DEV    "/dev/ttyUSB0"
#define AT_UART_BAUDRATE     115200
#define AT_UART_DATA_WIDTH   UART_WORDLENGTH_8B
#define AT_UART_PARITY       UART_PARITY_NONE
#define AT_UART_STOP_BITS    UART_STOPBITS_1
#define AT_UART_FLOW_CONTROL UART_HWCONTROL_NONE
#define AT_UART_MODE         UART_MODE_TX_RX
#define AT_UART_TIMEOUT_MS   1000

/* Delimiter */
#define AT_RECV_SUCCESS_POSTFIX "OK\r\n"
#define AT_RECV_FAIL_POSTFIX    "ERROR\r\n"

#if defined(AT_TCP_HAL_SIM800)
#define AT_CMD_DATA_INTERVAL_MS 50
#endif

#ifdef PLATFORM_HAS_OS
#define AT_SINGLE_TASK    0
#else
#define AT_SINGLE_TASK    0
#endif

#ifndef bool
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct {
    char *reply_prefix;
    char *reply_success_postfix;
    char *reply_fail_postfix;
} atcmd_config_t;

typedef void (*at_recv_cb)(void *arg, char *buf, int buflen);

/**
 * initialization
 * Configuration (e.g. AT_UART_PORT, UART_BAUDRATE) can be found
 * in above macro
 */
int at_parser_init(void);

/**
 * at send (format: command + delimiter + data) and wait reply
 *
 * @param cmd at command sending buf. MUST not be NULL.
 * @param cmdlen at command length.
 * @param success_callback success callback function. MUST not be NULL.
 * @param fail_callback fail callback function. MUST not be NULL.
 * @param atcmdconfig AT cmd reply format config. Use default if NULL
 */
int at_send_wait_reply(const char *cmd, int cmdlen,
                       at_recv_cb success_callback, at_recv_cb fail_callback,
                       const atcmd_config_t *atcmdconfig);

/**
 * at send (format: data) and does not wait reply
 *
 * @param data sending buffer.
 * @param datalen sending length.
 */
int at_send_no_reply(const char *data, int datalen);


/**
 * at read for certain bytes of data
 *
 * @param outbuf output buffer.
 * @param readsize read size.
 */
uint32_t at_read(char *outbuf, uint32_t readsize);


/**
 * at register callback for recv
 *
 * @param prefix interested string. Must not be NULL.
 * @param postfix intersted postfix. NULL if postfix not provided.
 * @param ppcing_data_len buffer size for recv data, zero if postfix not provided
 * @param cb callback handle function. Must not be NULL.
 * @param arg callback handle function args. NULL if not used.
 */
int at_register_callback(const char *prefix, const char *postfix,
                         int ppcing_data_len, at_recv_cb cb, void *arg);


/**
 * at yield receive function. Only used in single task scenario
 *
 * @param replybuf reply buffer.
 * @param bufsize reply buffer size.
 * @param atcmdconfig AT cmd reply format config. Use default if NULL
 * @param timeout_ms receive timeout in millisecond
 */
int at_yield(char *replybuf, int bufsize, const atcmd_config_t *atcmdconfig,
             int timeout_ms);

#if defined (USE_USER_OS)
/**
 * at worker task
 * It is called when the user creates a task in user's os
 * in above macro
 */
void* at_worker(void *arg);
#endif

#endif


