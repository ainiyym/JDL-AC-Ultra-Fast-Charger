//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrder.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Charging Order module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_ChargingOrder.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    bool initialized;                                                                            // Whether initialized
    cloud_protocol_charging_cloud_protocol_order_manager_t *order_info[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS]; // Order information
    cloud_protocol_transaction_id_config_t config;                                               // Configuration information
    cloud_protocol_energy_interface_t energy_iface;                                              // Electrical energy interface
} cloud_protocol_order_manager_t;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_order_manager_t cloud_protocol_order_manager = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool cloud_protocol_generate_transaction_id(uint8_t *transaction_id, uint8_t gun_no);
static bool cloud_protocol_calculate_rates(cloud_protocol_charging_cloud_protocol_order_manager_t *order, uint32_t delta_energy);
static bool cloud_protocol_save_order_to_tsdb(const cloud_protocol_charging_cloud_protocol_order_manager_t *order);
static cloud_protocol_charging_cloud_protocol_order_manager_t* cloud_protocol_load_order_from_tsdb(const char* transaction_id);
static bool cloud_protocol_load_sequence_from_storage(void);
static bool cloud_protocol_save_sequence_to_storage(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// Initialize the order module
bool cloud_protocol_charging_order_init(const cloud_protocol_transaction_id_config_t *config)
{
    if (cloud_protocol_order_manager.initialized)
    {
        return true;
    }
    memset(&cloud_protocol_order_manager, 0, sizeof(cloud_protocol_order_manager_t));
    if (config != NULL)
    {
        memcpy(&cloud_protocol_order_manager.config, config, sizeof(cloud_protocol_transaction_id_config_t));
    }
    else
    {
        memset(&cloud_protocol_order_manager.config, 0, sizeof(cloud_protocol_transaction_id_config_t));
    }
    // Load persistent sequence numbers

    cloud_protocol_load_sequence_from_storage();

    cloud_protocol_order_manager.initialized = true;
    CLOUD_INFO("<%s>initialized for %d guns\n", __FUNCTION__, CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS);

    return true;
}

// Set the electrical energy interface
bool cloud_protocol_charging_order_set_energy_interface(const cloud_protocol_energy_interface_t *interface)
{
    if (interface == NULL)
    {
        return false;
    }

    memcpy(&cloud_protocol_order_manager.energy_iface, interface, sizeof(cloud_protocol_energy_interface_t));
    return true;
}

// Get active orders
cloud_protocol_charging_cloud_protocol_order_manager_t *cloud_protocol_get_active_charging_order_(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return NULL;
    }

    return cloud_protocol_order_manager.order_info[gun_no - 1];
}

// Check if the gun is busy
bool cloud_protocol_charging_order_is_gun_busy(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_get_active_charging_order_(gun_no);
    return (order != NULL && order->status < CLOUD_PROTOCOL_ORDER_STATUS_FINISHED);
}

// get order status
cloud_protocol_order_status_t cloud_protocol_charging_order_get_status(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_get_active_charging_order_(gun_no);
    if (order == NULL)
    {
        return CLOUD_PROTOCOL_ORDER_STATUS_INIT;
    }
    
    return order->status;
}

// Stop all orders
bool cloud_protocol_charging_order_stop_all(cloud_protocol_StopCharging_Type_E reason)
{
    bool all_stopped = true;

    for (int i = 0; i < CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; i++)
    {
        cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[i];
        if (order != NULL && order->status < CLOUD_PROTOCOL_ORDER_STATUS_FINISHED)
        {
            if (!cloud_protocol_charging_order_finish(i + 1, reason))
            {
                all_stopped = false;
            }
        }
    }

    return all_stopped;
}

// Update all orders to TSDB
bool cloud_protocol_charging_order_update_tsdb_all(void)
{
    bool all_saved = true;

    for (int i = 0; i < CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; i++)
    {
        cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[i];
        if (order != NULL)
        {
            if (!cloud_protocol_save_order_to_tsdb(order))
            {
                all_saved = false;
            }
        }
    }

    return all_saved;
}

static bool cloud_protocol_save_order_to_tsdb(const cloud_protocol_charging_cloud_protocol_order_manager_t *order)
{
    // TODO: 实现TSDB存储逻辑
    return true;
}

static cloud_protocol_charging_cloud_protocol_order_manager_t *cloud_protocol_load_order_from_tsdb(const char *transaction_id)
{
    // TODO: 实现TSDB加载逻辑
    return NULL;
}

static bool cloud_protocol_load_sequence_from_storage(void)
{
    CLOUD_PROTOCOL_READ_ORDER_SEQUENCE(&cloud_protocol_order_manager.config.sequence[0], sizeof(cloud_protocol_order_manager.config.sequence));
    CLOUD_INFO("<%s> sequence0:%d sequence1:%d \r\n", __func__, cloud_protocol_order_manager.config.sequence[0], cloud_protocol_order_manager.config.sequence[1]);
    return true;
}

static bool cloud_protocol_save_sequence_to_storage(void)
{
    CLOUD_PROTOCOL_SAVE_ORDER_SEQUENCE(&cloud_protocol_order_manager.config.sequence[0], sizeof(cloud_protocol_order_manager.config.sequence));
    CLOUD_INFO("<%s> sequence0:%d sequence1:%d \r\n", __func__, cloud_protocol_order_manager.config.sequence[0], cloud_protocol_order_manager.config.sequence[1]);
    return true;
}

// generate order id
static bool cloud_protocol_generate_transaction_id(uint8_t *transaction_id, uint8_t gun_no)
{
    if (transaction_id == NULL || sizeof(transaction_id) < 17 || gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    // Offline orders are generated according to the rules: pile number (7) + gun number (1) + year, month, day, hour, minute and second (6) + auto-increment serial number (2)
    uint8_t date_str[6] = {0}; // YYMMDDHHMMSS

    CLOUD_PROTOCOL_GET_CURRENT_DATE_TIME(date_str);

    // fill in the order ID
    memcpy(transaction_id, cloud_protocol_order_manager.config.SN, 7); // Pile number (7 bytes)
    memcpy(transaction_id + 7, &gun_no, 1);               // Gun number (1 byte)
    memcpy(transaction_id + 8, date_str, 6);              // Date (6 bytes)
    // Increment sequence number (2 bytes, 0-65535)
    cloud_protocol_order_manager.config.sequence[gun_no - 1] =
        (cloud_protocol_order_manager.config.sequence[gun_no - 1] + 1) & 0xFFFF;

    // store sequence
    cloud_protocol_save_sequence_to_storage();
    memcpy(transaction_id + 14, &cloud_protocol_order_manager.config.sequence[gun_no - 1], 2); // Sequence (2 bytes)

    return true;
}

// Calculate the rate information
static bool cloud_protocol_calculate_rates(cloud_protocol_charging_cloud_protocol_order_manager_t *order, uint32_t delta_energy)
{
    if (order == NULL)
    {
        return false;
    }

    // Obtain the current time to determine the rate period
    uint32_t now_time = 0;
    now_time = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    struct tm *tm_info = localtime((const time_t *)&now_time);
    int current_hour = tm_info->tm_hour;
    int current_minute = tm_info->tm_min;

    // Obtain the current time slot (48 slots per day, each slot is 30 minutes)
    cloud_protocol_billing_time_slot_t current_time_slot = CLOUD_PROTOCOL_GET_BILLING_MODEL_TIME_SLOT_INFO();

    // Calculate which period you are currently in (one period every 30 minutes, a total of 48 periods)
    int time_slot_index = (current_hour * 2) + (current_minute >= 30 ? 1 : 0);
    if (time_slot_index >= 48)
        time_slot_index = 47;

    // Get the rate number for the current time slot
    uint8_t current_rate_no = current_time_slot.time_slot_rates[time_slot_index];

    // Determine the rate type based on the rate number
    float rate_price = 0.0f;
    const char *rate_name = "Unknown";

    switch (current_rate_no)
    {
        case 0: // Peak
            rate_price = fixed_5dec_to_float(current_time_slot.peak_rate.electricity_rate) + fixed_5dec_to_float(current_time_slot.peak_rate.service_rate);
            order->active_orders.peak.price = cloud_protocol_fixed_5dec(rate_price);
            order->active_orders.peak.energy += delta_energy; // Update energy
            order->active_orders.peak.amount += cloud_protocol_fixed_4dec(rate_price * fixed_4dec_to_float(delta_energy));
            rate_name = "Peak";
            break;
        case 1: // High
            rate_price = fixed_5dec_to_float(current_time_slot.high_rate.electricity_rate) + fixed_5dec_to_float(current_time_slot.high_rate.service_rate);
            order->active_orders.high.price = cloud_protocol_fixed_5dec(rate_price);
            order->active_orders.high.energy += delta_energy; // Update energy
            order->active_orders.high.amount += cloud_protocol_fixed_4dec(rate_price * fixed_4dec_to_float(delta_energy));
            rate_name = "High";
            break;
        case 2: // Normal
            rate_price = fixed_5dec_to_float(current_time_slot.normal_rate.electricity_rate) + fixed_5dec_to_float(current_time_slot.normal_rate.service_rate);
            order->active_orders.normal.price = cloud_protocol_fixed_5dec(rate_price);
            order->active_orders.normal.energy += delta_energy; // Update energy
            order->active_orders.normal.amount += cloud_protocol_fixed_4dec(rate_price * fixed_4dec_to_float(delta_energy));
            rate_name = "Normal";
            break;
        case 3: // Valley
            rate_price = fixed_5dec_to_float(current_time_slot.valley_rate.electricity_rate) + fixed_5dec_to_float(current_time_slot.valley_rate.service_rate);
            order->active_orders.valley.price = cloud_protocol_fixed_5dec(rate_price);
            order->active_orders.valley.energy += delta_energy; // Update energy
            order->active_orders.valley.amount += cloud_protocol_fixed_4dec(rate_price * fixed_4dec_to_float(delta_energy));
            rate_name = "Valley";
            break;
        default: // Normal
            rate_price = fixed_5dec_to_float(current_time_slot.normal_rate.electricity_rate) + fixed_5dec_to_float(current_time_slot.normal_rate.service_rate);
            order->active_orders.normal.price = cloud_protocol_fixed_5dec(rate_price);
            order->active_orders.normal.energy += delta_energy; // Update energy
            order->active_orders.normal.amount += cloud_protocol_fixed_4dec(rate_price * fixed_4dec_to_float(delta_energy));
            rate_name = "Default";
            break;
    }

    // Debugging information
    CLOUD_INFO("Gun %d: +%.4f kWh at %s rate (%.5f yuan/kWh)\r\n",
           order->active_orders.connector_id, fixed_4dec_to_float(delta_energy), rate_name, rate_price);

    return true;
}

// Create a new order
cloud_protocol_charging_cloud_protocol_order_manager_t *cloud_protocol_charging_order_create(uint8_t gun_no, cloud_protocol_order_type_t type,
                                        cloud_protocol_transaction_type_t trans_type,
                                        const uint8_t *platform_transaction_id,
                                        const char *vin, const uint8_t *card)
{
    if (!cloud_protocol_order_manager.initialized || gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return NULL;
    }

    // Check if there are any active orders for this gun
    if (cloud_protocol_order_manager.order_info[gun_no - 1] != NULL)
    {
        CLOUD_ERROR("<%s>Gun %d already has an active order\r\n", __FUNCTION__, gun_no);
        return NULL;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = (cloud_protocol_charging_cloud_protocol_order_manager_t *)CLOUDM_MALLOC(sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t));
    if (order == NULL)
    {
        CLOUD_ERROR("<%s>Memory allocation failed\r\n", __FUNCTION__);
        return NULL;
    }

    memset(order, 0, sizeof(cloud_protocol_charging_cloud_protocol_order_manager_t));

    // Generate the order number
    if (platform_transaction_id != NULL && sizeof(platform_transaction_id) == CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH)
    {
        memcpy(order->active_orders.transaction_id, platform_transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);
    }
    else if (!cloud_protocol_generate_transaction_id(order->active_orders.transaction_id, gun_no))
    {
        CLOUDM_FREE(order);
        CLOUD_ERROR("<%s>Failed to generate transaction ID\r\n", __FUNCTION__);
        return NULL;
    }

    // Set basic information
    order->order_type = type;
    order->status = CLOUD_PROTOCOL_ORDER_STATUS_INIT;

    // Set gun number and pile number
    order->active_orders.connector_id = gun_no;
    memcpy(order->active_orders.SN, cloud_protocol_order_manager.config.SN, 7);

    // Set transaction type
    order->active_orders.transaction_type = trans_type;

    // Set VIN
    if (vin != NULL)
    {
        strncpy(order->active_orders.vin, vin, sizeof(order->active_orders.vin));
    }
    else
    {
        memset(order->active_orders.vin, 0, sizeof(order->active_orders.vin));
    }

    // Set physical card number
    if (card != NULL)
    {
        memcpy(order->active_orders.physical_card, card, CLOUD_PROTOCOL_RFID_UID_LENGTH);
    }
    else
    {
        memset(order->active_orders.physical_card, 0, CLOUD_PROTOCOL_RFID_UID_LENGTH);
    }

    // Set the trading hours
    CLOUD_PROTOCOL_GET_CP56TIME2A(&order->active_orders.transaction_time);

    // Set timestamps
    if (CLOUD_PROTOCOL_ORDER_TYPE_ONLINE == type)
    {
        order->create_timestamp = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    }
    else
    {
        order->create_timestamp = CLOUD_GET_TIME_MS();
    }
    order->update_timestamp = order->create_timestamp;

    // Get initial meter reading
    if (cloud_protocol_order_manager.energy_iface.get_meter_reading != NULL)
    {
        order->last_meter_reading = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);
        order->active_orders.total_start = order->last_meter_reading;
    }

    // Save to the manager
    cloud_protocol_order_manager.order_info[gun_no - 1] = order;

    CLOUD_INFO("<%s> gun num:%d, type:%s, trans_type:0x%02X transaction id:%s, transaction id:", __FUNCTION__,
               gun_no, (type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE) ? "Online" : "Offline", trans_type);
    CLOUD_PRINT_HEX(order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);

    return order;
}

void cloud_protocol_charging_order_release(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order != NULL && order->status == CLOUD_PROTOCOL_ORDER_STATUS_UPLOADED)
    {
        CLOUDM_FREE(order);
        cloud_protocol_order_manager.order_info[gun_no - 1] = NULL;
        CLOUD_INFO("<%s>Released order for gun %d\r\n", __FUNCTION__, gun_no);
    }
}

// Start the order
bool cloud_protocol_charging_order_start(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order == NULL || order->status != CLOUD_PROTOCOL_ORDER_STATUS_INIT)
    {
        CLOUD_ERROR("<%s>Invalid order state, gun_no: %d, status: %d\r\n", __FUNCTION__, gun_no, order ? order->status : -1);
        return false;
    }

    // Set the start time
    if (order->order_type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE)
    {
        CLOUD_PROTOCOL_GET_CP56TIME2A(&order->active_orders.start_time);
        order->update_timestamp = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    }
    else
    {
        uint32_t lv_timestamp = CLOUD_GET_TIME_MS();
        CLOUD_PROTOCOL_TIMESTAMP_TO_CP56TIME2A(&lv_timestamp, &order->active_orders.start_time);
        order->update_timestamp = CLOUD_GET_TIME_MS();
    }

    order->status = CLOUD_PROTOCOL_ORDER_STATUS_STARTED;

    // Set the start energy
    order->last_meter_reading = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);

    // save to TSDB
    cloud_protocol_save_order_to_tsdb(order);

    CLOUD_INFO("<%s>Gun: %d\n", __FUNCTION__, gun_no);
    return true;
}

// Update all active orders(online orders only)
bool cloud_protocol_charging_order_update_all(void)
{
    bool updated = false;

    for (int i = 0; i < CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; i++)
    {
        cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[i];
        if (order == NULL || (order->status != CLOUD_PROTOCOL_ORDER_STATUS_STARTED && order->status != CLOUD_PROTOCOL_ORDER_STATUS_CHARGING))
        {
            continue;
        }

        // Update status
        if (order->status == CLOUD_PROTOCOL_ORDER_STATUS_STARTED)
        {
            order->status = CLOUD_PROTOCOL_ORDER_STATUS_CHARGING;
        }

        // Update energy data
        uint8_t gun_no = i + 1;

        if (cloud_protocol_order_manager.energy_iface.get_meter_reading != NULL)
        {
            uint32_t current_reading = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);

            // Prevent the electricity meter reading from reverting
            if (current_reading >= order->last_meter_reading)
            {
                uint32_t delta_energy = current_reading - order->last_meter_reading;

                if (delta_energy > 0)
                {
                    order->last_meter_reading = current_reading;
                    // Calculate the rate
                    cloud_protocol_calculate_rates(order, delta_energy);
                    updated = true;
                }
            }
        }

        order->update_timestamp = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    }

    // Save to TSDB regularly
    static uint32_t save_counter = 0;
    // Save once every 60 valid updates
    if (updated && ++save_counter >= 60)
    { 
        cloud_protocol_charging_order_update_tsdb_all();
        save_counter = 0;
    }

    return updated;
}

// finish the order
bool cloud_protocol_charging_order_finish(uint8_t gun_no, cloud_protocol_StopCharging_Type_E reason)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order == NULL)
    {
        return false;
    }
    // set stop reason
    order->active_orders.stop_reason = reason;

    // set end time
    if (order->order_type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE)
    {
        CLOUD_PROTOCOL_GET_CP56TIME2A(&order->active_orders.end_time);
        order->update_timestamp = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    }
    else
    {
        uint32_t lv_timestamp = CLOUD_GET_TIME_MS();
        CLOUD_PROTOCOL_TIMESTAMP_TO_CP56TIME2A(&lv_timestamp, &order->active_orders.end_time);
        order->update_timestamp = CLOUD_GET_TIME_MS();
    }

    // Get final meter reading
    if (cloud_protocol_order_manager.energy_iface.get_meter_reading != NULL)
    {
        order->active_orders.total_end = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);
    }
    else
    {
        return false;
    }

    // Final calculated rate
    order->active_orders.total_energy = (uint32_t)(order->active_orders.total_end - order->active_orders.total_start);
    uint32_t delta_energy = order->active_orders.total_end - order->last_meter_reading;
    cloud_protocol_calculate_rates(order, delta_energy);

    order->status = CLOUD_PROTOCOL_ORDER_STATUS_FINISHED;

    // Save to TSDB
    cloud_protocol_save_order_to_tsdb(order);

    CLOUD_INFO("<%s>gun_no:%d transaction id:\r\n", gun_no);
    CLOUD_PRINT_HEX(order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);
    CLOUD_INFO("<%s>Total energy: %.4f kWh, Total amount: %.4f yuan\r\n",
               fixed_4dec_to_float(order->active_orders.total_energy),
               fixed_4dec_to_float(order->active_orders.total_amount));

    return true;
}
/* EOL */
