//******************************************************************************
//* File Name: FlashDB_AppM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-09-15 10:00:00
//* Author: JDLzhou
//* Description: FlashDB APP module header file
/*******************************************************************************/
#if !defined (__FLASHDB_APP_M_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FLASHDB_APP_M_H
/*********************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "flashdb_wrapper.h"
#include "stdint.h"
#include "stddef.h"
#include "Cloud_Protocol_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FLASHDB_TEST_ENABLE       (0)  // FlashDB test function enable
#define FLASHDB_TRACE(fmt, ...)   Core_printf("[FLASHDB_APP] " fmt, ##__VA_ARGS__)
#define FLASHDB_PRINT_HEX(data, len) Core_Print_Hex(data, len)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum 
{
    FLASHDB_TYPE_INT,
    FLASHDB_TYPE_FLOAT,
    FLASHDB_TYPE_STRING,
    FLASHDB_TYPE_BLOB
} FlashDB_DataType_t;

typedef enum
{
    FLASHDB_OK = 0,
    FLASHDB_ERR_INVALID_PARAM = -1,
    FLASHDB_ERR_SET_FAIL = -2,
    FLASHDB_ERR_GET_FAIL = -3,
    FLASHDB_ERR_NOT_EXIST = -4,
    FLASHDB_ERR_READ_BLOB_FAIL = -5,
    FLASHDB_ERR_WRITE_BLOB_FAIL = -6,
    FLASHDB_ERR_DELETE_FAIL = -7,
    FLASHDB_ERR_OUT_OF_MEMORY = -8,
    FLASHDB_ERR_UNKNOWN = -100
} FlashDB_ReturnType_t;

typedef enum
{
    FLASHDB_KV_M4G_DEVICE_INIT_FLAG,
    FLASHDB_KV_GAGA_SN,
    FLASHDB_KV_GAGA_ORDER_SEQUENCE,
    FLASHDB_KV_SYNC_TSDB_TIMESTAMP,
    FLASHDB_KV_SG_SN,
    FLASHDB_KV_SG_BILLING_MODE_NO,
    FLASHDB_KV_SG_BILLING_MODE_MODEL
} FlashDB_App_KvDB_Enum;

typedef enum
{
    FLASHDB_TSDB_OFFLINE_ORDER_GUN1,
    FLASHDB_TSDB_OFFLINE_ORDER_GUN2
}FlashDB_App_TSDB_Enum;

typedef enum
{
    FLASHDB_ITERATOR_DIRECTION_FORWARD,
    FLASHDB_ITERATOR_DIRECTION_BACKWARD
} FlashDB_Iterator_Direction_t;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef bool (*user_processor_t)(fdb_tsl_t tsl, void* arg);
typedef struct
{
    void *record_data;
    size_t record_size;
    user_processor_t record_processor;
} tsdb_iter_context_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern int FlashDB_AppM_Init(void);
extern void FlashDB_Powerdown_Handler(void);
//KV
extern FlashDB_ReturnType_t FlashDB_ReadValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size, size_t* actual_len);
extern FlashDB_ReturnType_t FlashDB_WriteValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size);
//TSDB
extern FlashDB_ReturnType_t FlashDB_Append_Data(FlashDB_App_TSDB_Enum tsdb_id, const void *data, size_t size);
extern FlashDB_ReturnType_t FlashDB_Append_Data_With_Ts(FlashDB_App_TSDB_Enum tsdb_id, const void *data, size_t size, uint32_t timestamp);
extern FlashDB_ReturnType_t FlashDB_TS_Iterate(FlashDB_App_TSDB_Enum tsdb_id, FlashDB_Iterator_Direction_t direction, void *data, size_t size, user_processor_t record_processor_cb);
extern void FlashDB_TS_Set_Record_Status(FlashDB_App_TSDB_Enum tsdb_id, fdb_tsl_t tsl, fdb_tsl_status_t status);
extern uint32_t FlashDB_TS_GetTotalRecordsCounts(FlashDB_App_TSDB_Enum tsdb_id, fdb_tsl_status_t status);
#endif /* __FLASHDB_APP_M_H */
/* EOL */
