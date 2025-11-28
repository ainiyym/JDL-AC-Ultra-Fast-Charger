/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <string.h>

#include "infra_types.h"
#include "wrappers.h"
#include "at_parser.h"

#define OOB_MAX 5
#define RECV_BUFFER_SIZE 512

typedef struct oob_s
{
    char *     prefix;
    char *     postfix;
    uint32_t   reallen;
    uint32_t   maxlen;
    at_recv_cb cb;
    void *     arg;
} oob_t;

enum
{
    AT_PARSER_INIT,
    AT_PARSER_IDLE,
    AT_PARSER_DATA_PROCESSING,
    AT_PARSER_DATA_OVERSIZE_PROCESSING
} at_parser_state;

/*
 * --> | slist | --> | slist | --> NULL
 *     ---------     ---------
 *     | smhr  |     | smpr  |
 *     ---------     ---------
 *     | rsp   |     | rsp   |
 *     ---------     ---------
 */
#if !AT_SINGLE_TASK
#include "infra_list.h"
typedef struct at_task_s
{
    slist_t       next;
    void *        smpr;
    char *        command;
    char *        rsp_prefix;
    char *        rsp_success_postfix;
    char *        rsp_fail_postfix;
    uint32_t      rsp_prefix_len;
    uint32_t      rsp_success_postfix_len;
    uint32_t      rsp_fail_postfix_len;
    at_recv_cb    rsp_cb;
} at_task_t;
#endif

/**
 * Parser structure for parsing AT commands
 */
typedef struct
{
    uart_dev_t *_pstuart;
    char        _parser_status;
    int         _timeout;
    char *      _default_recv_success_postfix;
    char *      _default_recv_fail_postfix;
    int         _recv_success_postfix_len;
    int         _recv_fail_postfix_len;
    oob_t       _oobs[OOB_MAX];
    char        _oob_processing;
    int         _oobs_num;
    char        _oob_recv_buf[RECV_BUFFER_SIZE];
    uint8_t     _oob_app_protocol_bit_num;
    char        _rsp[RECV_BUFFER_SIZE];
    uint16_t    _rsp_offset;
    void *      at_uart_recv_mutex;
    void *      at_uart_send_mutex;
    void *      task_mutex;
#if !AT_SINGLE_TASK
    slist_t     task_l;
#endif
} at_parser_t;

#define TASK_DEFAULT_WAIT_TIME (5000)

#ifndef AT_WORKER_STACK_SIZE
#define AT_WORKER_STACK_SIZE   1024
#endif

#ifndef AT_UART_TIMEOUT_MS
#define AT_UART_TIMEOUT_MS     1000
#endif

#ifndef AT_CMD_DATA_INTERVAL_MS
#define AT_CMD_DATA_INTERVAL_MS   0
#endif

#ifdef AT_DEBUG_MODE
#define atpsr_err(...)               do{HAL_Printf(__VA_ARGS__);}while(0)
#define atpsr_warning(...)           do{HAL_Printf(__VA_ARGS__);}while(0)
#define atpsr_info(...)              do{HAL_Printf(__VA_ARGS__);}while(0)
#define atpsr_debug(...)             do{HAL_Printf(__VA_ARGS__);}while(0)
#else
#define atpsr_err(...)
#define atpsr_warning(...)
#define atpsr_info(...)
#define atpsr_debug(...)
#endif
static uint8_t    inited = 0;

static uart_dev_t at_uart;

static at_parser_t at;

static char at_rx_buf[RECV_BUFFER_SIZE+1];

#if !AT_SINGLE_TASK
#if !defined (USE_USER_OS)
static void* at_worker(void *arg);
#endif
#endif

#ifndef PLATFORM_HAS_DYNMEM
#if !AT_SINGLE_TASK
static at_task_t g_at_task;
#endif
#endif


static void at_uart_configure(uart_dev_t *u)
{
    u->port                = AT_UART_PORT;
    u->config.baud_rate    = AT_UART_BAUDRATE;
    u->config.data_width   = AT_UART_DATA_WIDTH;
    u->config.parity       = AT_UART_PARITY;
    u->config.stop_bits    = AT_UART_STOP_BITS;
    u->config.flow_control = AT_UART_FLOW_CONTROL;
    u->config.mode         = AT_UART_MODE;
}

static int at_init_uart()
{
    at_uart_configure(&at_uart);
#if !defined (USE_USER_USART_INIT)
    if (HAL_AT_Uart_Init(&at_uart) != 0) {
        return -1;
    }
#endif
    at._pstuart = &at_uart;

    return 0;
}


static void at_set_timeout(int timeout)
{
    at._timeout = timeout;
}

static void at_set_recv_delimiter(const char *recv_success_postfix,
                                  const char *recv_fail_postfix)
{
    at._default_recv_success_postfix = (char *)recv_success_postfix;
    at._default_recv_fail_postfix    = (char *)recv_fail_postfix;
    at._recv_success_postfix_len     = strlen(recv_success_postfix);
    at._recv_fail_postfix_len        = strlen(recv_fail_postfix);
}

static int at_init_task_mutex()
{
    at.task_mutex = HAL_MutexCreate();
    if (NULL == at.task_mutex) {
        atpsr_err("Creating task mutex failed\r\n");
        return -1;
    }

    return 0;
}

static void at_deinit_task_mutex()
{
    if (at.task_mutex) {
        HAL_MutexDestroy(at.task_mutex);
    }
    return;
}

static int at_init_uart_recv_mutex()
{
    at.at_uart_recv_mutex = HAL_MutexCreate();
    if (NULL == at.at_uart_recv_mutex) {
        atpsr_err("Creating at_uart_recv_mutex failed\r\n");
        return -1;
    }

    return 0;
}

static void at_deinit_uart_recv_mutex()
{
    if (at.at_uart_recv_mutex) {
        HAL_MutexDestroy(at.at_uart_recv_mutex);
    }
    return;
}

static int at_worker_uart_send_mutex_init()
{
    at.at_uart_send_mutex = HAL_MutexCreate();
    if (NULL == at.at_uart_send_mutex) {
        atpsr_err("Creating at worker sem failed\r\n");
        return -1;
    }
    return 0;
}

#if !AT_SINGLE_TASK
#if !defined (USE_USER_USART_INIT)
static void at_worker_uart_send_mutex_deinit()
{
    if (at.at_uart_send_mutex) {
        HAL_MutexDestroy(at.at_uart_send_mutex);
    }
}
#endif
#endif

int at_parser_init(void)
{
    char *recv_success_postfix = AT_RECV_SUCCESS_POSTFIX;
    char *recv_fail_postfix = AT_RECV_FAIL_POSTFIX;
    int  timeout = AT_UART_TIMEOUT_MS;
#if !AT_SINGLE_TASK
#if !defined (USE_USER_OS)
    void *task;
    int stack_used;
    hal_os_thread_param_t task_parms = {0};
#endif
#endif

    if (inited == 1) {
        atpsr_info("have already inited ,it will init again\r\n");
        return -1;
    }

    memset(&at, 0, sizeof(at_parser_t));

    if (at_init_uart() != 0) {
        atpsr_err("at uart init fail \r\n");
        return -1;
    }

    memset(at._oobs, 0, sizeof(oob_t) * OOB_MAX);

    at_set_timeout(timeout);
    at_set_recv_delimiter(recv_success_postfix, recv_fail_postfix);

    if (at_init_uart_recv_mutex() != 0) {
        atpsr_err("at_uart_recv_mutex init fail \r\n");
        return -1;
    }

    if (at_init_task_mutex() != 0) {
        at_deinit_uart_recv_mutex();
        atpsr_err("at mutex init fail \r\n");
        return -1;
    }

    if (at_worker_uart_send_mutex_init() != 0) {
        at_deinit_uart_recv_mutex();
        at_deinit_task_mutex();
        atpsr_err("fail to creat at worker sem\r\n");
    }

#if AT_SINGLE_TASK
    inited = true;
#else
    slist_init(&at.task_l);
#if !defined (USE_USER_OS)
    task_parms.priority = os_thread_priority_normal;
    task_parms.stack_size = AT_WORKER_STACK_SIZE;
    task_parms.name = "at_worker";
    if (HAL_ThreadCreate(&task, at_worker, NULL, &task_parms, &stack_used) != 0) {
        at_deinit_uart_recv_mutex();
        at_deinit_task_mutex();
        at_worker_uart_send_mutex_deinit();
        atpsr_err("fail to creat at task\r\n");
        return -1;
    }
#endif
#endif

    return 0;
}

static int at_sendto_lower(uart_dev_t *uart, void *data, uint32_t size,
                           uint32_t timeout, bool ackreq)
{
    int ret = -1;

    (void) ackreq;
    ret = HAL_AT_Uart_Send(uart, data, size, timeout);

    return ret;
}

static int at_recvfrom_lower(uart_dev_t *uart, void *data, uint32_t expect_size,
                             uint32_t *recv_size, uint32_t timeout)
{
    int ret = -1;

    ret = HAL_AT_Uart_Recv(uart, data, expect_size, recv_size, timeout);

    return ret;
}

static int at_recv_check_lower(uart_dev_t *uart, uint32_t *recv_size)
{
    int ret = -1;

    ret = HAL_AT_Uart_Recv_Check(uart, recv_size);

    return ret;
}

#if AT_SINGLE_TASK
int at_send_wait_reply(const char *cmd, int cmdlen, int _timeout,
                       at_recv_cb cb,const atcmd_config_t *atcmdconfig)
{
    if (at_send_no_reply(cmd, cmdlen) < 0) {
        return -1;
    }

    if (at_yield(cb, atcmdconfig, at._timeout) <  0) {
        return -1;
    }

    return 0;
}
#else
static int at_worker_task_add(at_task_t *tsk)
{
    if (NULL == tsk) {
        atpsr_err("invalid input %s \r\n", __func__);
        return -1;
    }

    HAL_MutexLock(at.task_mutex);
    slist_add_tail(&tsk->next, &at.task_l);
    HAL_MutexUnlock(at.task_mutex);

    return 0;
}

static int at_worker_task_del(at_task_t *tsk)
{
    if (NULL == tsk) {
        atpsr_err("invalid input %s \r\n", __func__);
        return -1;
    }

    HAL_MutexLock(at.task_mutex);
    slist_del(&tsk->next, &at.task_l);
    HAL_MutexUnlock(at.task_mutex);
    if (tsk->smpr) {
        HAL_SemaphoreDestroy(tsk->smpr);
    }
    if (tsk) {
#ifdef PLATFORM_HAS_DYNMEM
        HAL_Free(tsk);
#endif
    }

    return 0;
}

int at_send_wait_reply(const char *cmd, int cmdlen, int _timeout,
                       at_recv_cb cb,const atcmd_config_t *atcmdconfig)
{ 
    int ret = 0;
    at_task_t *tsk;

    if (inited == 0) {
        atpsr_err("at have not init yet\r\n");
        return -1;
    }

    if (NULL == cmd || cmdlen <= 0) {
        atpsr_err("%s invalid input \r\n", __FUNCTION__);
        return -1;
    }

    HAL_MutexLock(at.at_uart_send_mutex);
#ifdef PLATFORM_HAS_DYNMEM
    tsk = (at_task_t *)HAL_Malloc(sizeof(at_task_t));
#else
    tsk = &g_at_task;
#endif
    if (NULL == tsk) {
        atpsr_err("tsk buffer allocating failed");
        HAL_MutexUnlock(at.at_uart_send_mutex);
        return -1;
    }
    memset(tsk, 0, sizeof(at_task_t));

    tsk->smpr = HAL_SemaphoreCreate();
    if (NULL == tsk->smpr) {
        atpsr_err("failed to allocate semaphore");
        goto end;
    }

    if (atcmdconfig) {
        if (NULL != atcmdconfig->reply_prefix) {
            tsk->rsp_prefix     = atcmdconfig->reply_prefix;
            tsk->rsp_prefix_len = strlen(atcmdconfig->reply_prefix);
        }

        if (NULL != atcmdconfig->reply_success_postfix) {
            tsk->rsp_success_postfix     = atcmdconfig->reply_success_postfix;
            tsk->rsp_success_postfix_len = strlen(atcmdconfig->reply_success_postfix);
        }

        if (NULL != atcmdconfig->reply_fail_postfix) {
            tsk->rsp_fail_postfix     = atcmdconfig->reply_fail_postfix;
            tsk->rsp_fail_postfix_len = strlen(atcmdconfig->reply_fail_postfix);
        }
    }

    tsk->command = (char *)cmd;
    tsk->rsp_cb     = cb;

    at_worker_task_add(tsk);

    if ((ret = at_sendto_lower(at._pstuart, (void *)cmd, cmdlen,
                               at._timeout, true)) != 0) {
        atpsr_err("uart send command failed");
        goto end;
    }

    if ((ret = HAL_SemaphoreWait(tsk->smpr, _timeout)) != 0) 
    {
        atpsr_err("tsk->command:%s sem_wait failed\r\n", tsk->command);
        goto end;
    }

end:
    at_worker_task_del(tsk);
    HAL_MutexUnlock(at.at_uart_send_mutex);
    return ret;
}
#endif

int at_send_no_reply(const char *data, int datalen)  
{
    int ret = 0;

    if (inited == 0) {
        atpsr_err("at have not init yet\r\n");
        return -1;
    }

    if (NULL == data || datalen <= 0) {
        atpsr_err("invalid input \r\n");
        return -1;
    }

    HAL_MutexLock(at.at_uart_send_mutex);
    if ((ret = at_sendto_lower(at._pstuart, (void *)data,
                               datalen, at._timeout, true)) != 0) {
        atpsr_err("uart send raw content (%s) failed", data);
        HAL_MutexUnlock(at.at_uart_send_mutex);
        return -1;
    }

    HAL_MutexUnlock(at.at_uart_send_mutex);

    return ret;
}

static int at_recv_check(uart_dev_t *uart, uint16_t *recv_size)
{
    int ret = 0;

    if (uart == NULL || recv_size == NULL)
    {
        return -1;
    }

    HAL_MutexLock(at.at_uart_recv_mutex);
    ret = at_recv_check_lower(uart, (uint32_t *)recv_size);
    HAL_MutexUnlock(at.at_uart_recv_mutex);

    return ret;
}

static int at_getc(char *c, int timeout_ms)
{
    int ret = 0;
    char data;
    uint32_t recv_size = 0;

    if (NULL == c)    
    {
        return -1;
    }

    if (inited == 0)
    {
        atpsr_err("at have not init yet\r\n");
        return -1;
    }

    HAL_MutexLock(at.at_uart_recv_mutex);
    ret = at_recvfrom_lower(at._pstuart, (void *)&data, 1, &recv_size, timeout_ms);
    HAL_MutexUnlock(at.at_uart_recv_mutex);

    if (ret != 0)
    {
#ifdef WORKAROUND_DEVELOPERBOARD_DMA_UART
        if (ret == 1)
        {
            HAL_UART_Deinit(at._pstuart);
            at_init_uart();
        }
#endif
        return -1;
    }

    if (recv_size == 1)
    {
        *c = data;
        return 0;
    }
    else
    {
        return -1;
    }
}

uint32_t at_read(char *outbuf, uint32_t readsize)
{
    int ret = 0;
    uint32_t recv_size;

    if (inited == 0)
    {
        atpsr_err("at have not init yet\r\n");
        return 0;
    }

    HAL_MutexLock(at.at_uart_recv_mutex);
    ret = at_recvfrom_lower(at._pstuart, (void *)outbuf, readsize, &recv_size, at._timeout);
    HAL_MutexUnlock(at.at_uart_recv_mutex);

    if (ret != 0)
    {
        recv_size = 0;
    }

    return recv_size;
}

void at_at_register_set_app_protocol_bit_num_callback(uint8_t bit_num)
{
    at._oob_app_protocol_bit_num = bit_num;
}

int at_register_callback(const char *prefix, const char *postfix,
                         int ppcing_data_len, at_recv_cb cb, void *arg)
{
    oob_t *oob = NULL;
    int    i   = 0;

    if (ppcing_data_len < 0 || ppcing_data_len > RECV_BUFFER_SIZE || NULL == prefix) {
        atpsr_err("%s invalid input \r\n", __func__);
        return -1;
    }

    if (NULL != postfix && 0 == ppcing_data_len) {
        atpsr_err("%s invalid postfix input \r\n", __func__);
        return -1;
    }

    if (at._oobs_num >= OOB_MAX) {
        atpsr_err("No place left in OOB.\r\n");
        return -1;
    }

    /*check oob exist*/
    for (i = 0; i < at._oobs_num; i++) {
        if (NULL != at._oobs[i].prefix &&
            strcmp(prefix, at._oobs[i].prefix) == 0) {
            atpsr_warning("oob prefix %s is already exist.\r\n", prefix);
            return -1;
        }
    }

    oob = &(at._oobs[at._oobs_num++]);

    oob->maxlen  = ppcing_data_len;
    oob->prefix  = (char *)prefix;
    oob->postfix = (char *)postfix;
    oob->cb      = cb;
    oob->arg     = arg;
    oob->reallen = 0;

    atpsr_debug("New oob registered (%s)\r\n", oob->prefix);

    return 0;
}

static void at_scan_for_callback(char c, char *buf, uint16_t *index)
{
    int k;
    oob_t *oob = NULL;
    int offset = *index;

    if (!buf || offset < 0)
    {
        return;
    }

    for (k = 0; k < at._oobs_num; k++)
    {
        oob = &(at._oobs[k]);
        if (oob->reallen > 0 ||
            (offset >= strlen(oob->prefix) &&
             memcmp(oob->prefix, buf + offset - strlen(oob->prefix),
                    strlen(oob->prefix)) == 0))
        {
            if (oob->postfix == NULL)
            {
                if (oob->reallen == 0)
                {
                    at._oob_processing = 1;
                    int len = strlen(oob->prefix) - 1;
                    len = len > 0 ? len : 0;
                    if (len > 0)
                    {
                        memset(at._oob_recv_buf, 0, oob->maxlen);
                        memcpy(at._oob_recv_buf, oob->prefix, len);
                        oob->reallen += len;
                    }
                }
                if (oob->reallen < oob->maxlen)
                {
                    at._oob_recv_buf[oob->reallen] = c;
                    oob->reallen++;
                }
            }
            else
            {
                if (oob->reallen == 0)
                {
                    atpsr_debug("AT! %s\r\n", oob->prefix);
                    at._oob_processing = 1;
                    int len = strlen(oob->prefix) - 1;
                    len = len > 0 ? len : 0;
                    if (len > 0)
                    {
                        memcpy(at._oob_recv_buf, oob->prefix, len);
                    }
                    oob->reallen += len;
                }

                if (oob->reallen < oob->maxlen)
                {
                    at._oob_recv_buf[oob->reallen] = c;
                    oob->reallen++;
                    if ((oob->reallen >=
                         strlen(oob->prefix) + strlen(oob->postfix)) &&
                        (strncmp(at._oob_recv_buf + oob->reallen -
                                     strlen(oob->postfix),
                                 oob->postfix,
                                 strlen(oob->postfix)) == 0))
                    {
                        /*recv postfix*/
                        oob->cb(oob->arg, at._oob_recv_buf, oob->reallen);
                        goto __init_rcv;
                    }
                }
                else 
                {
                    atpsr_err("invalid oob %s input , for oversize %s \r\n",
                              oob->prefix, at._oob_recv_buf);
__init_rcv:
                    offset -= oob->reallen;
                    memset(&buf[offset], 0, oob->reallen);
                    oob->reallen = 0;
                    at._oob_processing = 0;
                }
            }
            continue;
        }
    }

    *index = offset;
    return;
}

#if AT_SINGLE_TASK
int at_yield(at_recv_cb cb, const atcmd_config_t *atcmdconfig,
             int timeout_ms)
{
    int        offset                  = 0;
    int        ret                     = 0;
    int        rsp_prefix_len          = 0;
    int        rsp_success_postfix_len = 0;
    int        rsp_fail_postfix_len    = 0;
    int        at_reply_begin          = 0;
    int        at_reply_offset         = 0;
    char       c                       = 0;
    char      *buf                 = NULL;
    char      *rsp_prefix          = NULL;
    char      *rsp_success_postfix = NULL;
    char      *rsp_fail_postfix    = NULL;

    if (!inited) {
        atpsr_err("AT parser has not inited!\r\n");
        return -1;
    }

    buf = at_rx_buf;
    if (NULL == buf) {
        atpsr_err("AT worker fail to malloc ,task exist \r\n");
        return -1;
    }

    memset(buf, 0, RECV_BUFFER_SIZE);

    while (true) {
        /* read from uart and store buf */
        ret = at_getc(&c, timeout_ms);
        if (ret != 0) {
            atpsr_err("at yield timeout break loop");
            break;
        }

        if (offset + 1 >= RECV_BUFFER_SIZE) {
            atpsr_err("buffer full");
            break;
        }
        buf[offset++] = c;
        buf[offset]   = 0;

        at_scan_for_callback(c, buf, &offset);

        if (replybuf == NULL || bufsize <= 0) {
            /* if no task, continue recv */
            continue;
        }

        if (NULL != atcmdconfig && NULL != atcmdconfig->reply_prefix) {
            rsp_prefix     = atcmdconfig->reply_prefix;
            rsp_prefix_len = strlen(rsp_prefix);
        } else {

        }

        if (NULL != atcmdconfig && NULL != atcmdconfig->reply_success_postfix) {
            rsp_success_postfix     = atcmdconfig->reply_success_postfix;
            rsp_success_postfix_len = strlen(rsp_success_postfix);
        } else {
            rsp_success_postfix     = at._default_recv_success_postfix;
            rsp_success_postfix_len = at._recv_success_postfix_len;
        }

        if (NULL != atcmdconfig && NULL != atcmdconfig->reply_fail_postfix) {
            rsp_fail_postfix     = atcmdconfig->reply_fail_postfix;
            rsp_fail_postfix_len = strlen(rsp_fail_postfix);
        } else {
            rsp_fail_postfix     = at._default_recv_fail_postfix;
            rsp_fail_postfix_len = at._recv_fail_postfix_len;
        }

        if (offset >= rsp_prefix_len && at_reply_begin == 0 &&
            (strncmp(buf + offset - rsp_prefix_len, rsp_prefix,
                     rsp_prefix_len) == 0)) {
            at_reply_begin = 1;
        }

        if (at_reply_begin == 1) {
            if (at_reply_offset < bufsize) {
                replybuf[at_reply_offset] = c;
                at_reply_offset++;

                if ((at_reply_offset >= rsp_success_postfix_len &&
                     strncmp(
                       replybuf + at_reply_offset - rsp_success_postfix_len,
                       rsp_success_postfix, rsp_success_postfix_len) == 0) ||
                    (at_reply_offset >= rsp_fail_postfix_len &&
                     strncmp(replybuf + at_reply_offset - rsp_fail_postfix_len,
                             rsp_fail_postfix, rsp_fail_postfix_len) == 0)) {
                    return 0;
                }
            } else {
                memset(replybuf, 0, bufsize);
                strcpy(replybuf, rsp_fail_postfix);
                break;
            }
        }

    }

    return -1;
}
#else
static void at_work_init(void)
{
    inited = 1;
    memset(at_rx_buf, 0, RECV_BUFFER_SIZE);
    at._parser_status = AT_PARSER_IDLE;
}

static void at_work_idle(void)
{
    uint16_t check_size = 0;

    if (at_recv_check(at._pstuart, &check_size) != 0)
    {
        return;
    }

    if (check_size > 0)
    {
        at._parser_status = AT_PARSER_DATA_PROCESSING;
    }
}

static void at_work_data_oversize_processing(uint16_t* offset, char *prefix, char *success_postfix, char *fail_postfix)
{
    char *buf = NULL;
    int memcpy_size = 0;
    int rsp_prefix_len = 0;
    int rsp_success_postfix_len = 0;
    int rsp_fail_postfix_len = 0;

    buf = at_rx_buf;
    if (prefix != NULL)
        rsp_prefix_len = strlen(prefix);
    if (success_postfix != NULL)
        rsp_success_postfix_len = strlen(success_postfix);
    if (fail_postfix != NULL)
        rsp_fail_postfix_len = strlen(fail_postfix);

    if (*offset > (RECV_BUFFER_SIZE - 2))
    {
        memcpy_size = rsp_prefix_len > rsp_success_postfix_len
                          ? rsp_prefix_len
                          : rsp_success_postfix_len;
        memcpy_size = memcpy_size > rsp_fail_postfix_len
                          ? memcpy_size
                          : rsp_fail_postfix_len;
        memmove(buf, buf + *offset - memcpy_size, memcpy_size);
        *offset = memcpy_size;
    }
    at._parser_status = AT_PARSER_IDLE;
}

static void at_cleanup_response(char* buf, uint16_t* offset, char* rsp, uint16_t* rsp_offset)
{
    memset(buf, 0, *offset);
    *offset = 0;
    memset(rsp, 0, sizeof(rsp));
    *rsp_offset = 0;
}

static void at_work_cmd_data_processing(char c, uint16_t* index, char *prefix, char *success_postfix, char *fail_postfix)
{
    int at_task_empty = 0;
    static int at_task_response_begin = 0;
    int prefix_len = 0;
    int success_postfix_len = 0;
    int fail_postfix_len = 0;
    at_task_t *tsk = NULL;
    char *buf = NULL;
    at_recv_cb rsp_cb = NULL;
    uint16_t offset = *index;

    buf = at_rx_buf;
    HAL_MutexLock(at.task_mutex);
    at_task_empty = slist_empty(&at.task_l);

    if (!at_task_empty)
    {
        tsk = slist_first_entry(&at.task_l, at_task_t, next);
        if (NULL != tsk->rsp_prefix && 0 != tsk->rsp_prefix_len)
        {
            prefix = tsk->rsp_prefix;
            prefix_len = tsk->rsp_prefix_len;
        }
        else
        {
            prefix = NULL;
            prefix_len = 0;
        }

        if (NULL != tsk->rsp_success_postfix &&
            0 != tsk->rsp_success_postfix_len)
        {
            success_postfix = tsk->rsp_success_postfix;
            success_postfix_len = tsk->rsp_success_postfix_len;
        }
        else
        {
            success_postfix = at._default_recv_success_postfix;
            success_postfix_len = at._recv_success_postfix_len;
        }

        if (NULL != tsk->rsp_fail_postfix && 0 != tsk->rsp_fail_postfix_len)
        {
            fail_postfix = tsk->rsp_fail_postfix;
            fail_postfix_len = tsk->rsp_fail_postfix_len;
        }
        else
        {
            fail_postfix = at._default_recv_fail_postfix;
            fail_postfix_len = at._recv_fail_postfix_len;
        }

        if (NULL != tsk->rsp_cb)
        {
            rsp_cb = tsk->rsp_cb;
        }
        else
        {
            rsp_cb = NULL;
        }
    }
    HAL_MutexUnlock(at.task_mutex);

    /* if no task, continue recv */
    if (at_task_empty || tsk == NULL)
    {
        // atpsr_debug("No task in queue.\r\n");
        return;
    }

    if (NULL != prefix)
    {
        if (offset >= prefix_len && at_task_response_begin == 0 &&
            (strncmp(buf + offset - prefix_len, prefix,
                     prefix_len) == 0))
        {
            strcpy(at._rsp, prefix);
            at._rsp_offset += prefix_len;
            at_task_response_begin = 1;
            atpsr_debug("<%s> Found prefix: %.*s\r\n", __func__, prefix_len, buf + offset - prefix_len);
            return;
        }
    }
    else if (at._oob_processing == 0 && at_task_response_begin == 0)
    {
        at_task_response_begin = 1;
        at._rsp_offset = 0;
        atpsr_debug("<%s> NOT Found OOB data\r\n", __func__);
    }
    else
    {
        
    }

    if (at_task_response_begin == 1)
    {
        if (at._rsp_offset < sizeof(at._rsp) - 2)
        {
            at._rsp[at._rsp_offset++] = c;
            at._rsp[at._rsp_offset] = 0;

            int response_complete = 0;
            if (tsk->rsp_success_postfix != NULL &&
                at._rsp_offset >= success_postfix_len &&
                strcmp(at._rsp + at._rsp_offset - success_postfix_len,
                       success_postfix) == 0)
            {
                response_complete = 1;
            }
            else if (tsk->rsp_fail_postfix != NULL &&
                     at._rsp_offset >= fail_postfix_len &&
                     strcmp(at._rsp + at._rsp_offset - fail_postfix_len,
                            fail_postfix) == 0)
            {
                response_complete = 1;
            }

            if (response_complete == 1)
            {
                HAL_MutexLock(at.task_mutex);
                if (rsp_cb != NULL)
                {
                    rsp_cb(tsk->command, at._rsp, at._rsp_offset);
                }
                HAL_SemaphorePost(tsk->smpr);
                HAL_MutexUnlock(at.task_mutex);

                at_task_response_begin = 0;
                at_cleanup_response(buf, &offset, at._rsp, &at._rsp_offset);
                atpsr_debug("<%s> Response complete\r\n", __func__);
            }
        }
        else
        {
            HAL_MutexLock(at.task_mutex);
            if (rsp_cb != NULL)
            {
                rsp_cb(tsk->command, at._rsp, at._rsp_offset);
            }
            HAL_SemaphorePost(tsk->smpr);
            HAL_MutexUnlock(at.task_mutex);

            at_task_response_begin = 0;
            at_cleanup_response(buf, &offset, at._rsp, &at._rsp_offset);
            atpsr_err("Response oversize, break\r\n");
        }
    }
    *index = offset;
}

static void at_work_data_processing(uint16_t* offset, char *prefix, char *success_postfix, char *fail_postfix)
{
    int ret = 0;
    char c = 0;
    char *buf = NULL;

    buf = at_rx_buf;

    ret = at_getc(&c, at._timeout);
    if (ret != 0)
    {
        oob_t *oob = &at._oobs[at._oob_app_protocol_bit_num];
        if (oob->reallen > 0)
        {
            oob->cb(oob->arg, at._oob_recv_buf, oob->reallen);
            at._oob_processing = 0;
            memset(at._oob_recv_buf, 0, oob->maxlen);
            *offset -= oob->reallen;
            memset(&buf[*offset], 0, oob->reallen);
            oob->reallen = 0;
        }
        at._parser_status = AT_PARSER_IDLE;
        return;
    }
    if (*offset >= RECV_BUFFER_SIZE)
    {
        atpsr_err("Fatal error, no one is handling AT uart");
        at._parser_status = AT_PARSER_DATA_OVERSIZE_PROCESSING;
        return;
    }
    buf[(*offset)++] = c;
    buf[*offset] = 0;

    at_scan_for_callback(c, buf, offset);
    at_work_cmd_data_processing(c, offset, prefix, success_postfix, fail_postfix);
}

void *at_worker(void *arg)
{
    static uint16_t offset = 0;
    static char *rsp_prefix = NULL;
    static char *rsp_success_postfix = NULL;
    static char *rsp_fail_postfix = NULL;

    switch (at._parser_status)
    {
    case AT_PARSER_INIT:
        at_work_init();
        break;
    case AT_PARSER_IDLE:
        at_work_idle();
        break;
    case AT_PARSER_DATA_PROCESSING:
        at_work_data_processing(&offset, rsp_prefix, rsp_success_postfix, rsp_fail_postfix);
        break;
    case AT_PARSER_DATA_OVERSIZE_PROCESSING:
        at_work_data_oversize_processing(&offset, rsp_prefix, rsp_success_postfix, rsp_fail_postfix);
        break;
    default:
        at_parser_state = AT_PARSER_INIT; 
        break;
    }
		return NULL;
}
#endif
