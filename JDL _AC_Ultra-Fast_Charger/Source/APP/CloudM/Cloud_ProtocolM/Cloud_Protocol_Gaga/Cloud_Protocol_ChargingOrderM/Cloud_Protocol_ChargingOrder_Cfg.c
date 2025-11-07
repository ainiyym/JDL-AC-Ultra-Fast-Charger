//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrder_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol Charging Order module configuration file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_ChargingOrder_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool cloud_protocol_update_active_order_to_tsdb_cb(fdb_tsl_t tsl, void* arg);
static bool cloud_protocol_delete_active_order_in_tsdb_cb(fdb_tsl_t tsl, void* arg);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// Precision conversion function
uint32_t cloud_protocol_fixed_5dec(float value)
{
    return (uint32_t)(value * 100000.0f + 0.5f);
}

uint32_t cloud_protocol_fixed_4dec(float value)
{
    return (uint32_t)(value * 10000.0f + 0.5f);
}

uint64_t cloud_protocol_fixed_4dec_64(float value)
{
    return (uint64_t)(value * 10000.0f + 0.5f);
}

float cloud_protocol_fixed_5dec_to_float(uint32_t value)
{
    return value / 100000.0f;
}

float cloud_protocol_fixed_4dec_to_float(uint32_t value)
{
    return value / 10000.0f;
}

static bool cloud_protocol_update_active_order_to_tsdb_cb(fdb_tsl_t tsl, void* arg)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *tsl_order = (cloud_protocol_charging_cloud_protocol_order_manager_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    cloud_protocol_charging_cloud_protocol_order_manager_t *active_order = cloud_protocol_get_active_charging_order(tsl_order->active_orders.connector_id);
    if (active_order != NULL)
    {
        if (active_order->active_orders.transaction_id == tsl_order->active_orders.transaction_id)
        {
            // delete the tsl_order
            FlashDB_App_TSDB_Enum tsdb_id = tsl_order->active_orders.connector_id == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
            FlashDB_TS_Set_Record_Status(tsdb_id, tsl, FDB_TSL_DELETED);
            // store the updated order
            cloud_protocol_save_order_to_tsdb(tsl_order->active_orders.connector_id);
            CLOUD_INFO("<%s> gun:%d update order: ", __func__, tsl_order->active_orders.connector_id);
            CLOUD_PRINT_HEX(active_order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);
            // stop Iterate
            return true;
        }
    }
    return false;
}

static bool cloud_protocol_delete_active_order_in_tsdb_cb(fdb_tsl_t tsl, void* arg)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *tsl_order = (cloud_protocol_charging_cloud_protocol_order_manager_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    cloud_protocol_charging_cloud_protocol_order_manager_t *active_order = cloud_protocol_get_active_charging_order(tsl_order->active_orders.connector_id);
    if (active_order != NULL)
    {
        if (active_order->active_orders.transaction_id == tsl_order->active_orders.transaction_id)
        {
            // delete the tsl_order
            FlashDB_App_TSDB_Enum tsdb_id = tsl_order->active_orders.connector_id == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
            FlashDB_TS_Set_Record_Status(tsdb_id, tsl, FDB_TSL_DELETED);
            CLOUD_INFO("<%s> gun:%d delete order: ", __func__, tsl_order->active_orders.connector_id);
            CLOUD_PRINT_HEX(active_order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);
            // stop Iterate
            return true;
        }
    }
    return false;
}

static bool cloud_protocol_delete_local_order(fdb_tsl_t tsl, void* arg)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *tsl_order = (cloud_protocol_charging_cloud_protocol_order_manager_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    FlashDB_App_TSDB_Enum tsdb_id = tsl_order->active_orders.connector_id == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_TS_Set_Record_Status(tsdb_id, tsl, FDB_TSL_DELETED);
    CLOUD_INFO("<%s> gun:%d delete local order: ", __func__, tsl_order->active_orders.connector_id);
    CLOUD_PRINT_HEX(tsl_order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);

    return true;
}

static bool cloud_protocol_upload_offline_order_cb(fdb_tsl_t tsl, void* arg)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *tsl_order = (cloud_protocol_charging_cloud_protocol_order_manager_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    // got tsl order
    if (tsl_order->status != CLOUD_PROTOCOL_ORDER_STATUS_UPLOADED)
    {
        if (cloud_protocol_call_upload_offline_order(tsl_order))
        {
            // delete the local order
            cloud_protocol_delete_local_order(tsl, tsl_order);

            return true; // Stop the iteration and upload only one order at a time
        }
    }
    return false; // Continue to iterate on the next order
}

bool cloud_protocol_save_order_to_tsdb(uint8_t gun_no)
{
    if (gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *active_order = cloud_protocol_get_active_charging_order(gun_no);

    if (active_order == NULL)
    {
        return false;
    }
    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_ReturnType_t result = FlashDB_Append_Data(tsdb_id, active_order, sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t));

    CLOUD_INFO("<%s> gun:%d save result:%s\r\n", __func__, gun_no, result == FLASHDB_OK ? "SUCCESS" : "FAILURE");
    return (result == FLASHDB_OK);
}


bool cloud_protocol_update_active_order_to_tsdb(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }
    cloud_protocol_charging_cloud_protocol_order_manager_t order_temp = {0};
    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_TS_Iterate(tsdb_id, FLASHDB_ITERATOR_DIRECTION_BACKWARD, &order_temp,sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t), cloud_protocol_update_active_order_to_tsdb_cb);

    return true;
}

bool cloud_protocol_delete_active_order_in_tsdb(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }
    cloud_protocol_charging_cloud_protocol_order_manager_t order_temp = {0};
    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_TS_Iterate(tsdb_id, FLASHDB_ITERATOR_DIRECTION_BACKWARD, &order_temp,sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t), cloud_protocol_delete_active_order_in_tsdb_cb);

    return true;
}

bool cloud_protocol_process_offline_order_upload(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }
    cloud_protocol_charging_cloud_protocol_order_manager_t order_temp = {0};
    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_TS_Iterate(tsdb_id, FLASHDB_ITERATOR_DIRECTION_BACKWARD, &order_temp, sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t), cloud_protocol_upload_offline_order_cb);

    return true;
}
