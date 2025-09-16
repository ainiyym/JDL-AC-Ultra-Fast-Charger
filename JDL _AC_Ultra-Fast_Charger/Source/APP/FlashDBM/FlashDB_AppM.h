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

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FLASHDB_TRACE(fmt, ...)   Core_printf("[FLASHDB_APP] " fmt, ##__VA_ARGS__)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
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
    FLASHDB_KV_SN = 0,
    FLASHDB_KV_M4G_DEVICE_INIT_FLAG
} FlashDB_App_KvDB_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

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
extern FlashDB_ReturnType_t FlashDB_ReadValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size, size_t* actual_len);
extern FlashDB_ReturnType_t FlashDB_WriteValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size);

#endif /* __FLASHDB_APP_M_H */
/* EOL */
