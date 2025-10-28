//******************************************************************************
//* File Name: flashdb_wrapper.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FlashDB module header file
/*******************************************************************************/
#if !defined (__FLASHDB_WRAPPER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FLASHDB_WRAPPER_H
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "fdbM.h"
#include "flashdb.h"
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    FDB_WRAPPER_OK = 0,
    FDB_WRAPPER_INVALID_PARAM,
    FDB_WRAPPER_OUT_OF_MEMORY,
    FDB_WRAPPER_PEEK_ERROR,
    FDB_WRAPPER_SET_ERROR,
    FDB_WRAPPER_GET_ERROR,
    FDB_WRAPPER_DELETE_ERROR,
    FDB_WRAPPER_COUNT_ERROR,
} fdb_wrapper_err_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef bool (*fdb_wrapper_kv_iter_cb)(const char *key, void *value, size_t len, void *user_data);

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
// kvdb
fdb_wrapper_err_t fdb_wrapper_kv_set(fdb_kvdb_t db, const char *key, const char *value);
fdb_wrapper_err_t fdb_wrapper_kv_set_data(fdb_kvdb_t db, const char *key, const void *data, size_t size);
fdb_wrapper_err_t fdb_wrapper_kv_Peek(fdb_kvdb_t db, const char *key, size_t *actual_len);
fdb_wrapper_err_t fdb_wrapper_kv_get(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len);
fdb_wrapper_err_t fdb_wrapper_kv_get_blob(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len);
fdb_wrapper_err_t fdb_wrapper_kv_del(fdb_kvdb_t db, const char *key);
bool fdb_wrapper_kv_exist(fdb_kvdb_t db, const char *key);
uint32_t fdb_wrapper_kv_count(fdb_kvdb_t db);
// tsdb
fdb_wrapper_err_t fdb_wrapper_tsl_append(fdb_tsdb_t db, const void *data, size_t size);
fdb_wrapper_err_t fdb_wrapper_tsl_append_with_ts(fdb_tsdb_t db, const void *data, size_t size, fdb_time_t timestamp);
void fdb_wrapper_tsl_iter(fdb_tsdb_t db, fdb_tsl_cb cb, void *arg);
void fdb_wrapper_tsl_iter_reverse(fdb_tsdb_t db, fdb_tsl_cb cb, void *arg);
void fdb_wrapper_tsl_iter_by_time(fdb_tsdb_t db, fdb_time_t from, fdb_time_t to, fdb_tsl_cb cb, void *cb_arg);
fdb_wrapper_err_t fdb_wrapper_set_status(fdb_tsdb_t db, fdb_tsl_t tsl, fdb_tsl_status_t status);
void fdb_wrapper_set_clean(fdb_tsdb_t db);
#ifdef __cplusplus
}
#endif
#endif /* __FLASHDB_WRAPPER_H */
/* EOL */
