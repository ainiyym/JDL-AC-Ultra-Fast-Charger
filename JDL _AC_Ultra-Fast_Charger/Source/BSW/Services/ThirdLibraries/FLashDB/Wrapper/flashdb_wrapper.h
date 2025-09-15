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
#include "fdb_def.h"
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
// 基本KV操作
fdb_wrapper_err_t fdb_wrapper_kv_set(fdb_kvdb_t db, const char *key, const char *value);
fdb_wrapper_err_t fdb_wrapper_kv_set_data(fdb_kvdb_t db, const char *key, const void *data, size_t size);
fdb_wrapper_err_t fdb_wrapper_kv_get(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len);
fdb_wrapper_err_t fdb_wrapper_kv_get_blob(fdb_kvdb_t db, const char *key, void *buffer, size_t buffer_size, size_t *actual_len);
fdb_wrapper_err_t fdb_wrapper_kv_del(fdb_kvdb_t db, const char *key);
bool fdb_wrapper_kv_exist(fdb_kvdb_t db, const char *key);
// 遍历接口
uint32_t fdb_wrapper_kv_count(fdb_kvdb_t db);


#ifdef __cplusplus
}
#endif
#endif /* __FLASHDB_WRAPPER_H */
/* EOL */
