//******************************************************************************
//* File Name: FlashDB_AppM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-09-15 10:00:00
//* Author: JDLzhou
//* Description: FanM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FlashDB_AppM.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "string.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    FlashDB_App_KvDB_Enum kv_id;
    fdb_kvdb_t kvdb;
    char* key;
    FlashDB_DataType_t type;
    void * def_value;
    size_t blob_size;
} FlashDB_AppKvDBDefaultCfg_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
uint8_t YeeComxxx_Device_Init_Flag = 0;
char SN[16] = "33030300000001";

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
static FlashDB_AppKvDBDefaultCfg_t FlashDB_AppKvDBDefaultCfgTable[] = 
{
	{FLASHDB_KV_M4G_DEVICE_INIT_FLAG, 	&kvdb, 		"M4G_Device_Init_Flag",			FLASHDB_TYPE_INT, 					(uint8_t*)&YeeComxxx_Device_Init_Flag,					1},
	{FLASHDB_KV_SN, 					&kvdb, 		"SN", 							FLASHDB_TYPE_STRING,				(char*)SN,												0},
};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
int FlashDB_AppM_Init(void)
{
	int ret = 0;

	ret = fdb_init();
	if (ret != 0)
	{
		FLASHDB_TRACE("FlashDB init fail! ret=%d\r\n", ret);
		return -1;
	}
 
	vTaskDelay(pdMS_TO_TICKS(100));

	for (uint16_t i = 0; i < sizeof(FlashDB_AppKvDBDefaultCfgTable)/sizeof(FlashDB_AppKvDBDefaultCfgTable[0]); i++)
	{
		if (fdb_wrapper_kv_exist(FlashDB_AppKvDBDefaultCfgTable[i].kvdb, FlashDB_AppKvDBDefaultCfgTable[i].key))
		{
			FLASHDB_TRACE("FlashDB KVDB is exist key=%s\r\n", FlashDB_AppKvDBDefaultCfgTable[i].key);
		}
		else
		{
			if (FlashDB_AppKvDBDefaultCfgTable[i].blob_size)
			{
				ret = fdb_wrapper_kv_set_data(FlashDB_AppKvDBDefaultCfgTable[i].kvdb, FlashDB_AppKvDBDefaultCfgTable[i].key, FlashDB_AppKvDBDefaultCfgTable[i].def_value, FlashDB_AppKvDBDefaultCfgTable[i].blob_size);
			}
			else
			{
				ret = fdb_wrapper_kv_set(FlashDB_AppKvDBDefaultCfgTable[i].kvdb, FlashDB_AppKvDBDefaultCfgTable[i].key, FlashDB_AppKvDBDefaultCfgTable[i].def_value);
			}
			if (ret != FDB_WRAPPER_OK)
			{
				FLASHDB_TRACE("FlashDB KVDB set fail! key=%s, ret=%d\r\n", FlashDB_AppKvDBDefaultCfgTable[i].key, ret);
				return -1;
			}
			else
			{
				FLASHDB_TRACE("FlashDB KVDB set success! key=%s\r\n", FlashDB_AppKvDBDefaultCfgTable[i].key);
			}
		}
	}

	return 0;
}

/**
 * @brief Read the value of the specified Key from FlashDB
 * @param value Output parameter, used to store the value read
 * @param buff_size Buffer size
 * @param actual_len Actual length of the value retrieved
 * @return 0 on success, error code on failure
 */
FlashDB_ReturnType_t FlashDB_ReadValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size, size_t* actual_len)
{
	fdb_wrapper_err_t wrapper_ret = FDB_WRAPPER_OK;
	size_t len = 0;

	FlashDB_AppKvDBDefaultCfg_t *cfg = &FlashDB_AppKvDBDefaultCfgTable[kv_id];
	if (!cfg)
	{
		return FLASHDB_ERR_NOT_EXIST; // Configuration not found
	}

	char* key = cfg->key;

	if (!key || !value || buff_size == 0)
	{
		return FLASHDB_ERR_INVALID_PARAM; // Invalid parameter
	}

	switch (cfg->type)
	{
		case FLASHDB_TYPE_INT:
		{
			int32_t read_val = (int32_t)cfg->def_value;
			wrapper_ret = fdb_wrapper_kv_get_blob(cfg->kvdb, key, &read_val, sizeof(read_val), &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			*(int32_t *)value = read_val;
			break;
		} 
		case FLASHDB_TYPE_FLOAT:
		{
			float read_val = *(float *)cfg->def_value;
			wrapper_ret = fdb_wrapper_kv_get_blob(cfg->kvdb, key, &read_val, sizeof(read_val), &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			*(float *)value = read_val;
			break;
		}
		case FLASHDB_TYPE_STRING:
		{
			char read_val[256] = {0}; // Assume max string length is 255
			wrapper_ret = fdb_wrapper_kv_get(cfg->kvdb, key, read_val, sizeof(read_val), &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			if (len >= buff_size)
			{
				return FLASHDB_ERR_OUT_OF_MEMORY; // Provided buffer too small
			}
			strcpy((char *)value, read_val);
			break;
		}
		case FLASHDB_TYPE_BLOB:
		{
			wrapper_ret = fdb_wrapper_kv_get_blob(cfg->kvdb, key, value, buff_size, &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			break;
		}
		default:
			return FLASHDB_ERR_INVALID_PARAM; // Unsupported type
	}
	if (actual_len)
	{
		*actual_len = len;
	}

	return FLASHDB_OK;
}

/**
 * @brief Write the value of the specified Key to FlashDB
 * @param value Input parameter, value to be written
 * @param buff_size Buffer size
 * @return 0 on success, error code on failure
 */
FlashDB_ReturnType_t FlashDB_WriteValue(FlashDB_App_KvDB_Enum kv_id, void *value, size_t buff_size)
{
	fdb_wrapper_err_t wrapper_ret = FDB_WRAPPER_OK;

	FlashDB_AppKvDBDefaultCfg_t *cfg = &FlashDB_AppKvDBDefaultCfgTable[kv_id];
	if (!cfg)
	{
		return FLASHDB_ERR_NOT_EXIST; // Configuration not found
	}

	char *key = cfg->key;

	if (!key || !value || buff_size == 0)
	{
		return FLASHDB_ERR_INVALID_PARAM; // Invalid parameter
	}

	switch (cfg->type)
	{
		case FLASHDB_TYPE_INT:
		{
			wrapper_ret = fdb_wrapper_kv_set_data(cfg->kvdb, key, value, sizeof(int32_t));
			break;
		}
		case FLASHDB_TYPE_FLOAT:
		{
			wrapper_ret = fdb_wrapper_kv_set_data(cfg->kvdb, key, value, sizeof(float));
			break;
		}
		case FLASHDB_TYPE_STRING:
		{
			wrapper_ret = fdb_wrapper_kv_set(cfg->kvdb, key, value);
			break;
		}
		case FLASHDB_TYPE_BLOB:
		{
			wrapper_ret = fdb_wrapper_kv_set_data(cfg->kvdb, key, value, buff_size);
			break;
		}
		default:
			return FLASHDB_ERR_INVALID_PARAM; // Unsupported type
	}

	return (wrapper_ret == FDB_WRAPPER_OK) ? FLASHDB_OK : FLASHDB_ERR_SET_FAIL;
}
/* EOL */
