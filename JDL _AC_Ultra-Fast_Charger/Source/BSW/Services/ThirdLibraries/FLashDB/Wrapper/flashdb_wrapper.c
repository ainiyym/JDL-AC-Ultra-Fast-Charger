//******************************************************************************
//* File Name: flashdb_wrapper.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FlashDB module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "flashdb_wrapper.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <string.h>
#include "STD_LogService.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FLASHDB_WRAPPER_LOG(...) LOG_DEBUG(LOG_MODULE_NVM, __VA_ARGS__)
#define FLASHDB_WRAPPER_ERROR(...) LOG_ERROR(LOG_MODULE_NVM, __VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * Set a string KV. If it value is NULL, delete it.
 * If not find it in flash, then create it.
 *
 * @param db database object
 * @param key KV name
 * @param value KV value
 * @return result
 */
fdb_wrapper_err_t fdb_wrapper_kv_set(fdb_kvdb_t db, const char *key, const char *value)
{
    if (!key)
    {
        return FDB_WRAPPER_INVALID_PARAM;
    }

    fdb_err_t err = fdb_kv_set(db, key, value);
    if (err != FDB_NO_ERR)
    {
        return FDB_WRAPPER_SET_ERROR;
    }

    return FDB_WRAPPER_OK;
} 

/**
 * Set a blob KV. If it blob value is NULL, delete it.
 * If not find it in flash, then create it.
 *
 * @param db database object
 * @param key KV name
 * @param blob blob object
 *
 * @return result
 */
fdb_wrapper_err_t fdb_wrapper_kv_set_data(fdb_kvdb_t db, const char *key, const void *data, size_t size)
{
    if (!key || !data || size == 0)
    {
        return FDB_WRAPPER_INVALID_PARAM;
    }
    fdb_err_t err;
    struct fdb_blob blob;

    err = fdb_kv_set_blob(db, key, fdb_blob_make(&blob, data, size));
    if (err != FDB_NO_ERR)
    {
        return FDB_WRAPPER_SET_ERROR;
    }

    return FDB_WRAPPER_OK;
}

/**
 * Get an KV value by key name.
 *
 * @note this function is NOT supported reentrant
 * @note this function is DEPRECATED
 *
 * @param db database object
 * @param key KV name
 * @param buffer buffer to store the value
 * @param buffer_size size of the buffer
 * @param actual_len actual length of the value retrieved
 * @return value
 */
fdb_wrapper_err_t fdb_wrapper_kv_get(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len)
{
    if (!key || !buffer || buffer_size == 0 || !actual_len)
    {
        return FDB_WRAPPER_INVALID_PARAM;
    }

    char *value = fdb_kv_get(db, key);
    if (value != NULL)
    {
        strncpy(buffer, value, buffer_size);
        *actual_len = strlen(value);
        return FDB_WRAPPER_OK;
    }
    else
    {
        *actual_len = 0;
        return FDB_WRAPPER_GET_ERROR;
    }
}

/**
 * Get a blob KV value by key name.
 *
 * @param db database object
 * @param key KV name
 * @param blob blob object
 * @param value_buf value buffer
 * @param buf_len buffer length
 *
 * @return the actually get size on successful
 */
fdb_wrapper_err_t fdb_wrapper_kv_get_blob(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len)
{
    if (!key || !buffer || buffer_size == 0 || !actual_len)
    {
        return FDB_WRAPPER_INVALID_PARAM;
    }
    struct fdb_blob blob;

    size_t len = fdb_kv_get_blob(db, key, fdb_blob_make(&blob, buffer, buffer_size));
    if (len > 0)
    {
        *actual_len = len;
        return FDB_WRAPPER_OK;
    }
    else
    {
        *actual_len = 0;
        return FDB_WRAPPER_GET_ERROR;
    }
}

/**
 * Delete an KV.
 *
 * @param db database object
 * @param key KV name
 *
 * @return result
 */
fdb_wrapper_err_t fdb_wrapper_kv_del(fdb_kvdb_t db, const char *key)
{
    if (!key)
    {
        return FDB_WRAPPER_INVALID_PARAM;
    }

    fdb_err_t err = fdb_kv_del(db, key);
    if (err != FDB_NO_ERR)
    {
        return FDB_WRAPPER_DELETE_ERROR;
    }

    return FDB_WRAPPER_OK;
}

bool fdb_wrapper_kv_exist(fdb_kvdb_t db, const char *key)
{
    if (!key)
    {
        return false;
    }

    struct fdb_blob blob;
    size_t len = fdb_kv_get_blob(db, key, fdb_blob_make(&blob, NULL, 0)); 

    return (len != 0);
}

// 遍历接口
uint32_t fdb_wrapper_kv_count(fdb_kvdb_t db)
{
    struct fdb_kv_iterator iterator;
    uint32_t count = 0;

    fdb_kv_iterator_init(db, &iterator);

    while (fdb_kv_iterate(db, &iterator))
    {
        count++;
    }
    return count;
}
/* EOL */
 
