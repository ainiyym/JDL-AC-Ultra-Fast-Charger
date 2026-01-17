//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ChargingOrder_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Sg Cloud Protocol Charging Order module configuration file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ChargingOrder_Cfg.h"
#include "Meter_data.h"

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
char cloud_protocol_sg_order_id_buffer[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN] = {0};

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
 * @brief get current timestamp in seconds
 */
uint32_t Cloud_Protocol_Sg_Order_GetCurrentTime(void)
{
    uint32_t timestamp = 0;

    RTC_GetRtcSeconds(&timestamp);

    return timestamp;
}

/**
 * @brief get current hour
 */
uint8_t Cloud_Protocol_Sg_Order_GetCurrentBeijingHour(void)
{
    uint32_t timestamp = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* calculate hour from timestamp */
    uint32_t seconds_in_day = timestamp % (24 * 3600);
    uint8_t hour = seconds_in_day / 3600;

    return hour + 8;
}

/**
 * @brief get current minute
 */
uint8_t Cloud_Protocol_Sg_Order_GetCurrentMinute(void)
{
    uint32_t timestamp = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* calculate minute from timestamp */
    uint32_t seconds_in_day = timestamp % (24 * 3600);
    uint32_t seconds_in_hour = seconds_in_day % 3600;
    uint8_t minute = seconds_in_hour / 60;

    return minute;
}

/**
 * @brief get total energy of an order
 */
cloud_protocol_sg_order_energy_t Cloud_Protocol_Sg_Order_GetTotalEnergy(uint8_t gun_no, cloud_protocol_sg_order_op_t op)
{
  cloud_protocol_sg_order_energy_t total_energy = {0};
  Meter_GetTotalEnergy(gun_no, &total_energy, op);
  return total_energy;
}

/**
 * @brief get delta energy value since last update
 */
uint32_t Cloud_Protocol_Sg_Order_GetDeltaEnergyValue(uint8_t gun_no, cloud_protocol_sg_order_op_t op)
{
    uint32_t delta_energy = Meter_GetDeltaEnergyValue(gun_no, op);

    return delta_energy;
}

/**
 * @brief callback function for loading TSDB records
 */
static bool Cloud_Protocol_Sg_Order_LoaderOrderCb(fdb_tsl_t tsl, void* arg)
{
    cloud_protocol_sg_order_record_t *tsl_order = (cloud_protocol_sg_order_record_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    // got tsl order
    if (tsl_order->is_pending_upload)
    {
        // found pending upload order
        return true; // Stop iteration
    }
    return false; // Continue to iterate on the next order
}

/**
 * @brief callback function for deleting TSDB records
 */
static bool Cloud_Protocol_Sg_Order_DeleteOrderCb(fdb_tsl_t tsl, void *arg)
{
    cloud_protocol_sg_order_record_t *tsl_order = (cloud_protocol_sg_order_record_t *)arg;
    if (tsl_order == NULL)
    {
        return false;
    }
    if (strncmp(tsl_order->order_id, cloud_protocol_sg_order_id_buffer, CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN) == 0)
    {
        // found the order to delete
        FlashDB_App_TSDB_Enum tsdb_id = tsl_order->gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
        FlashDB_TS_Set_Record_Status(tsdb_id, tsl, FDB_TSL_DELETED);
        return true; // Stop iteration
    }
    return false;
}

/**
 * @brief save order record to TSDB
 */
bool Cloud_Protocol_Sg_Order_SaveOrderToTsdb(cloud_protocol_sg_order_record_t *order, fdb_time_t timestamp)
{
    if (order == NULL)
    {
        return false;
    }

    FlashDB_App_TSDB_Enum tsdb_id = order->gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_ReturnType_t result = FlashDB_Append_Data_With_Ts(tsdb_id, order, sizeof(cloud_protocol_sg_order_record_t), (uint32_t)timestamp);

    CLOUD_INFO("<%s> gun:%d save result:%s\r\n", __func__, order->gun_no, result == FLASHDB_OK ? "SUCCESS" : "FAILURE");
    return (result == FLASHDB_OK);
}

/**
 * @brief load next pending upload record from TSDB
 */
bool Cloud_Protocol_Sg_Order_LoadNextPendingFromTSDB(uint8_t gun_no, cloud_protocol_sg_order_record_t *record)
{
    if (record == NULL)
    {
        return false;
    }
    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;
    FlashDB_TS_Iterate(tsdb_id, FLASHDB_ITERATOR_DIRECTION_BACKWARD, record, sizeof(cloud_protocol_sg_order_record_t), Cloud_Protocol_Sg_Order_LoaderOrderCb);

    return true;
}

/**
 * @brief delete pending upload record from TSDB
 */
bool Cloud_Protocol_Sg_Order_RemovePendingFromTSDB(uint8_t gun_no, const char *order_id)
{
    if (order_id == NULL)
    {
        return false;
    }
    strncpy(cloud_protocol_sg_order_id_buffer, order_id, CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN - 1);
    cloud_protocol_sg_order_id_buffer[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN - 1] = '\0';

    FlashDB_App_TSDB_Enum tsdb_id = gun_no == 1 ? FLASHDB_TSDB_OFFLINE_ORDER_GUN1 : FLASHDB_TSDB_OFFLINE_ORDER_GUN2;

    cloud_protocol_sg_order_record_t empty_record = {0};

    FlashDB_TS_Iterate(tsdb_id, FLASHDB_ITERATOR_DIRECTION_BACKWARD, &empty_record, sizeof(cloud_protocol_sg_order_record_t), Cloud_Protocol_Sg_Order_DeleteOrderCb);

    return true;
}
