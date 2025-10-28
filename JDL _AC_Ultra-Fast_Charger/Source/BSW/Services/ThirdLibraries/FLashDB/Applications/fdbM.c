/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-31     armink       first version
 * 2025-09-11     zhousong     modify for STM32F103RCT6
 */

#include <stdio.h>
#include <board.h>
#include <stm32f1xx_hal.h>
#include <flashdb.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "STD_Rtc.h"

#define FDB_LOG_TAG "[main]"

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
static SemaphoreHandle_t xSemaphore = NULL;
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
        {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
struct fdb_kvdb kvdb = { 0 };
/* TSDB object */
struct fdb_tsdb tsdb_gun1 = { 0 };
struct fdb_tsdb tsdb_gun2 = { 0 };
/* ts_timestamp for simulated timestamp */
static uint32_t ts_timestamp[FAL_PART_TABLE_TSDB_COUNT] = {0};

extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
extern void tsdb_sample(fdb_tsdb_t tsdb);

static void lock(fdb_db_t db)
{
    if (xSemaphore != NULL) {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
    }
}
 
static void unlock(fdb_db_t db)
{
    if (xSemaphore != NULL) {
        xSemaphoreGive(xSemaphore);
    }
}

void tsdb_time_synced_init(uint32_t* timestamp)
{
    if (timestamp != NULL && sizeof(timestamp) == sizeof(ts_timestamp))
    {
        memcpy(ts_timestamp, timestamp, sizeof(ts_timestamp));
    }
}

uint32_t* powerdown_get_ts_timestamp(void)
{
    return ts_timestamp;
}

uint32_t get_ts0_time(void)
{
    /* Using the ts_timestamp instead of timestamp.
     * Please change this function to return RTC time.
     */
    return ++ts_timestamp[0];
}

uint32_t get_ts1_time(void)
{
    /* Using the ts_timestamp instead of timestamp.
     * Please change this function to return RTC time.
     */
    return ++ts_timestamp[1];
}

extern int spi_flash_init(void);
int fdb_init(void)
{
    spi_flash_init();

    xSemaphore = xSemaphoreCreateMutex();
    if (xSemaphore == NULL)
    {
        /* Mutex creation failed */
        return -1;
    }
    fdb_err_t result;

#ifdef FDB_USING_KVDB
    { /* KVDB Sample */
        struct fdb_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_kvdb_init(&kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        /* run basic KV samples */
        // kvdb_basic_sample(&kvdb);
        /* run string KV samples */
        // kvdb_type_string_sample(&kvdb);
        /* run blob KV samples */
        // kvdb_type_blob_sample(&kvdb);
    }
#endif /* FDB_USING_KVDB */ 

#ifdef FDB_USING_TSDB
    { /* TSDB Sample */
        /* set the lock and unlock function if you want */
        fdb_tsdb_control(&tsdb_gun1, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
        fdb_tsdb_control(&tsdb_gun1, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);
        fdb_tsdb_control(&tsdb_gun2, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
        fdb_tsdb_control(&tsdb_gun2, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);
        /* Time series database initialization
         *
         *       &tsdb: database object
         *       "log": database name
         * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         *    get_time: The get current timestamp function.
         *         128: maximum length of each log
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_tsdb_init(&tsdb_gun1, "order_gun1", "fdb_tsdb1", get_ts0_time, 256, NULL);
        result = fdb_tsdb_init(&tsdb_gun2, "order_gun2", "fdb_tsdb2", get_ts1_time, 256, NULL);
    }
#endif /* FDB_USING_TSDB */
    return 0;
}
