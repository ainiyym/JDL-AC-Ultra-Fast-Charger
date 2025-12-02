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
#include "STD_SysM.h"
#include "Cloud_EV_Charger_Information.h"
#include "Cloud_Protocol_ChargingOrder.h"
#include "Cloud_Protocol_EventPost_FwInfo.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef uint32_t (*fdb_get_ts_time_cb)(void);

typedef struct
{
    FlashDB_App_KvDB_Enum kv_id;
    fdb_kvdb_t kvdb;
    char* key;
    FlashDB_DataType_t type;
    void * def_value;
    size_t blob_size;
} FlashDB_AppKvDBDefaultCfg_t;

typedef struct
{
	FlashDB_App_TSDB_Enum tsdb_id; // TSDB ID
	fdb_tsdb_t tsdb;               // TSDB instance
	fdb_tsl_cb tsl_query_cb;       // TSL query callback function
	fdb_tsl_cb set_status_cb;      // TSL set status callback function
	fdb_get_ts_time_cb get_ts_time_cb;       // Get timestamp callback function
	uint32_t max_records;		   // Maximum record count
	size_t item_size;		   	   // Data item size
	uint16_t item_count;		   // Data item count
} FlashDB_AppTsdb_InstanceCfg_t;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static uint8_t flash_db_kv_4g_device_init_flag = 0;
static char flash_db_kv_gaga_sn[CLOUD_EV_SN_LEN] = "33030300000001";
static uint16_t flash_db_kv_gaga_order_sequence[SYS_CONNECTOR_NUM_MAX] = {1,1};
static uint32_t flash_db_kv_synced_tsdb_timestamp[FAL_PART_TABLE_TSDB_COUNT] = {0};
static char flash_db_kv_sg_billing_mode_number[V2G_MAX_MODEL_ID_LEN] = ""; // Default billing mode number code
static uint32_t flash_db_kv_sg_sn = 1; // Default SN code
/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool FlashDB_Order_Gun1_Query_Cb(fdb_tsl_t tsl, void *arg);
static bool FlashDB_Order_Gun2_Query_Cb(fdb_tsl_t tsl, void *arg);
static bool FlashDB_Order_Gun1_Set_Status_Cb(fdb_tsl_t tsl, void *arg);
static bool FlashDB_Order_Gun2_Set_Status_Cb(fdb_tsl_t tsl, void *arg);

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
static FlashDB_AppKvDBDefaultCfg_t FlashDB_AppKvDBDefaultCfgTable[] = 
{
	{FLASHDB_KV_M4G_DEVICE_INIT_FLAG, 	&kvdb, 		"flash_db_kv_4g_device_init_flag",		FLASHDB_TYPE_INT, 					(uint8_t*)&flash_db_kv_4g_device_init_flag,				sizeof(flash_db_kv_4g_device_init_flag)},
	{FLASHDB_KV_GAGA_SN, 				&kvdb, 		"flash_db_kv_gaga_sn", 					FLASHDB_TYPE_STRING,				(char*)flash_db_kv_gaga_sn,								0},
	{FLASHDB_KV_GAGA_ORDER_SEQUENCE, 	&kvdb, 		"flash_db_kv_gaga_order_sequence", 		FLASHDB_TYPE_BLOB, 					(uint16_t*)&flash_db_kv_gaga_order_sequence,			sizeof(flash_db_kv_gaga_order_sequence)},
	{FLASHDB_KV_SYNC_TSDB_TIMESTAMP, 	&kvdb, 		"flash_db_kv_synced_tsdb_timestamp", 	FLASHDB_TYPE_BLOB, 					(uint32_t*)&flash_db_kv_synced_tsdb_timestamp,			sizeof(flash_db_kv_synced_tsdb_timestamp)},
	{FLASHDB_KV_SG_BILLING_MODE_NO, 	&kvdb, 		"flash_db_kv_sg_billing_mode_number",	FLASHDB_TYPE_STRING,				(char*)flash_db_kv_sg_billing_mode_number,				0},
	{FLASHDB_KV_SG_SN, 					&kvdb, 		"flash_db_kv_sg_sn", 					FLASHDB_TYPE_INT, 					(uint32_t*)&flash_db_kv_sg_sn,							sizeof(flash_db_kv_sg_sn)}
};

static FlashDB_AppTsdb_InstanceCfg_t FlashDB_AppTsdbInstanceCfgTable[] =
{
	{
		.tsdb_id = FLASHDB_TSDB_OFFLINE_ORDER_GUN1,
		.tsdb = &tsdb_gun1,
		.tsl_query_cb = FlashDB_Order_Gun1_Query_Cb,
		.set_status_cb = FlashDB_Order_Gun1_Set_Status_Cb,
		.get_ts_time_cb = get_ts0_time,
		.max_records = 100,
		.item_size = sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t),
		.item_count = 1
	},
	{
		.tsdb_id = FLASHDB_TSDB_OFFLINE_ORDER_GUN2,
		.tsdb = &tsdb_gun2,
		.tsl_query_cb = FlashDB_Order_Gun2_Query_Cb,
		.set_status_cb = FlashDB_Order_Gun2_Set_Status_Cb,
		.get_ts_time_cb = get_ts1_time,
		.max_records = 100,
		.item_size = sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t),
		.item_count = 1
	}
};

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Initialize FlashDB and set default values if keys do not exist
 * @return 0 on success, -1 on failure
 */
int FlashDB_AppM_Init(void)
{
	int ret = 0;
	size_t actual_len = 0;

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
#if (0 == FLASHDB_TEST_ENABLE)
			if (FlashDB_AppKvDBDefaultCfgTable[i].blob_size)
			{
				ret = fdb_wrapper_kv_get_blob(FlashDB_AppKvDBDefaultCfgTable[i].kvdb, FlashDB_AppKvDBDefaultCfgTable[i].key, FlashDB_AppKvDBDefaultCfgTable[i].def_value, FlashDB_AppKvDBDefaultCfgTable[i].blob_size, &actual_len);
				FLASHDB_TRACE("FlashDB KVDB is exist key=%s, VALUE=", FlashDB_AppKvDBDefaultCfgTable[i].key);
				FLASHDB_PRINT_HEX(FlashDB_AppKvDBDefaultCfgTable[i].def_value, actual_len);
			}
			else
			{
				ret = fdb_wrapper_kv_get(FlashDB_AppKvDBDefaultCfgTable[i].kvdb, FlashDB_AppKvDBDefaultCfgTable[i].key, FlashDB_AppKvDBDefaultCfgTable[i].def_value, strlen(FlashDB_AppKvDBDefaultCfgTable[i].def_value), &actual_len);
				FLASHDB_TRACE("FlashDB KVDB is exist key=%s\r\n, value=%s\r\n", FlashDB_AppKvDBDefaultCfgTable[i].key, (char*)FlashDB_AppKvDBDefaultCfgTable[i].def_value);
			}
#endif
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
	Cloud_Ev_Set_Constant_Info(CLOUD_CONST_SERIAL_NUMBER, flash_db_kv_gaga_sn);
#if (1 == FLASHDB_TEST_ENABLE)
	FlashDB_WriteValue(FLASHDB_KV_M4G_DEVICE_INIT_FLAG, (uint8_t*)&flash_db_kv_4g_device_init_flag, 1);
#endif
	tsdb_time_synced_init(&flash_db_kv_synced_tsdb_timestamp[0]);
	Cloud_Protocol_EventPost_FwInfo_Set(CLOUD_PROTOCOL_EVENT_FW_MODEL_NO, (void *)flash_db_kv_sg_billing_mode_number);

	return 0;
}

void FlashDB_Powerdown_Handler(void)
{
	memcpy(&flash_db_kv_synced_tsdb_timestamp[0], powerdown_get_ts_timestamp(), sizeof(flash_db_kv_synced_tsdb_timestamp));
	FlashDB_WriteValue(FLASHDB_KV_SYNC_TSDB_TIMESTAMP, &flash_db_kv_synced_tsdb_timestamp, sizeof(flash_db_kv_synced_tsdb_timestamp));
	FLASHDB_TRACE("<%s>TS Counts saved to FlashDB. TsCount[0]%d TsCount[1]%d\r\n", __func__, flash_db_kv_synced_tsdb_timestamp[0], flash_db_kv_synced_tsdb_timestamp[1]);
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
			wrapper_ret = fdb_wrapper_kv_get_blob(cfg->kvdb, key, value, buff_size, &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			break;
		}
		case FLASHDB_TYPE_FLOAT:
		{
			wrapper_ret = fdb_wrapper_kv_get_blob(cfg->kvdb, key, value, buff_size, &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			break;
		}
		case FLASHDB_TYPE_STRING:
		{
			wrapper_ret = fdb_wrapper_kv_get(cfg->kvdb, key, value, buff_size, &len);
			if (wrapper_ret != FDB_WRAPPER_OK)
			{
				return FLASHDB_ERR_GET_FAIL; // Read failed
			}
			if (len >= buff_size)
			{
				return FLASHDB_ERR_OUT_OF_MEMORY; // Provided buffer too small
			}
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
	if (NULL != actual_len)
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

/* TSDB */
static bool FlashDB_Order_Gun1_Query_Cb(fdb_tsl_t tsl, void *arg)
{
	struct fdb_blob blob;
	tsdb_iter_context_t *context = (tsdb_iter_context_t *)arg;
	fdb_tsdb_t db = &tsdb_gun1;

	if (!context->record_processor && context->record_data != NULL)
	{
		fdb_blob_read((fdb_db_t)db, fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, context->record_data, context->record_size)));
		if (context->record_size == sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t))
		{
			return context->record_processor(tsl, context->record_data);
		}
	}
	return false;
}
	
static bool FlashDB_Order_Gun2_Query_Cb(fdb_tsl_t tsl, void *arg)
{
	struct fdb_blob blob;
	tsdb_iter_context_t *context = (tsdb_iter_context_t *)arg;
	fdb_tsdb_t db = &tsdb_gun2;

	if (!context->record_processor && context->record_data != NULL)
	{
		fdb_blob_read((fdb_db_t)db, fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, context->record_data, context->record_size)));
		if (context->record_size == sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t))
		{
			return context->record_processor(tsl, context->record_data);
		}
	}
	return false;
}

static bool FlashDB_Order_Gun1_Set_Status_Cb(fdb_tsl_t tsl, void *arg)
{
	fdb_tsdb_t db = &tsdb_gun1;
	fdb_tsl_status_t status = *(fdb_tsl_status_t *)arg;

	fdb_wrapper_set_status(db, tsl, status);
	return false;
}

static bool FlashDB_Order_Gun2_Set_Status_Cb(fdb_tsl_t tsl, void *arg)
{
	fdb_tsdb_t db = &tsdb_gun2;
	fdb_tsl_status_t status = *(fdb_tsl_status_t *)arg;

	fdb_wrapper_set_status(db, tsl, status);
	return false;
}

FlashDB_ReturnType_t FlashDB_Append_Data(FlashDB_App_TSDB_Enum tsdb_id, const void *data, size_t size)
{
	uint32_t timestamp = 0;
	timestamp = FlashDB_AppTsdbInstanceCfgTable[tsdb_id].get_ts_time_cb();
	return FlashDB_Append_Data_With_Ts(tsdb_id, data, size, timestamp);
	// return FlashDB_Append_Data_With_Ts(tsdb_id, data, size, ++flash_db_kv_synced_tsdb_timestamp);
}

FlashDB_ReturnType_t FlashDB_Append_Data_With_Ts(FlashDB_App_TSDB_Enum tsdb_id, const void *data, size_t size, uint32_t timestamp)
{
	if (data == NULL || size == 0 || tsdb_id >= sizeof(FlashDB_AppTsdbInstanceCfgTable)/sizeof(FlashDB_AppTsdbInstanceCfgTable[0]))
	{
		return FLASHDB_ERR_INVALID_PARAM;
	}
	FlashDB_AppTsdb_InstanceCfg_t *item_cfg = &FlashDB_AppTsdbInstanceCfgTable[tsdb_id];
	if (item_cfg == NULL)
	{
		return FLASHDB_ERR_NOT_EXIST; // TSDB or item not found
	}
	if (size > item_cfg->item_size)
	{
		return FLASHDB_ERR_OUT_OF_MEMORY; // Data size exceeds maximum
	}
	// FLASHDB_TRACE("<%s> timestamp=%u\r\n", __func__, timestamp);
	fdb_wrapper_err_t wrapper_ret = fdb_wrapper_tsl_append_with_ts(item_cfg->tsdb, data, size, timestamp);

	return (wrapper_ret == FDB_WRAPPER_OK) ? FLASHDB_OK : FLASHDB_ERR_SET_FAIL;
}

FlashDB_ReturnType_t FlashDB_TS_Iterate(FlashDB_App_TSDB_Enum tsdb_id, FlashDB_Iterator_Direction_t direction, void *data, size_t size, user_processor_t record_processor_cb)
{
	if (data == NULL || size == 0 || record_processor_cb == NULL)
	{
		return FLASHDB_ERR_INVALID_PARAM;
	}
	FlashDB_AppTsdb_InstanceCfg_t *item_cfg = &FlashDB_AppTsdbInstanceCfgTable[tsdb_id];
	if (item_cfg == NULL)
	{
		return FLASHDB_ERR_NOT_EXIST; // TSDB or item not found
	}
	if (size < item_cfg->item_size)
	{
		return FLASHDB_ERR_OUT_OF_MEMORY; // Provided buffer too small
	}

	tsdb_iter_context_t context = {
		.record_data = data,
		.record_size = size,
		.record_processor = record_processor_cb
	};

	switch (direction)
	{
		case FLASHDB_ITERATOR_DIRECTION_FORWARD:
			fdb_wrapper_tsl_iter(item_cfg->tsdb, item_cfg->tsl_query_cb, &context);
			break;
		case FLASHDB_ITERATOR_DIRECTION_BACKWARD:
			fdb_wrapper_tsl_iter_reverse(item_cfg->tsdb, item_cfg->tsl_query_cb, &context);
			break;
		default:
			return FLASHDB_ERR_INVALID_PARAM; // Invalid direction
	}
	return FLASHDB_OK;
}

void FlashDB_TS_Set_Record_Status(FlashDB_App_TSDB_Enum tsdb_id, fdb_tsl_t tsl, fdb_tsl_status_t status)
{
	FlashDB_AppTsdb_InstanceCfg_t *item_cfg = &FlashDB_AppTsdbInstanceCfgTable[tsdb_id];
	if (item_cfg == NULL || item_cfg->set_status_cb == NULL)
	{
		return; // TSDB not found
	}

	fdb_tsl_status_t status_copy = (fdb_tsl_status_t)status;
	(void)item_cfg->set_status_cb(tsl, &status_copy);
}

uint32_t FlashDB_TS_GetTotalRecordsCounts(FlashDB_App_TSDB_Enum tsdb_id, fdb_tsl_status_t status)
{
	fdb_tsdb_t tsdb = FlashDB_AppTsdbInstanceCfgTable[tsdb_id].tsdb;
	if (tsdb == NULL)
	{
		return 0;
	}

	return fdb_tsl_query_count(tsdb, 0, 0xffffffff-1, status);
}

/* EOL */
