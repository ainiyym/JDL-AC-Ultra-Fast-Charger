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
typedef enum
{
    ORDER_STATE_IDLE = 0,   // Idle state
    ORDER_STATE_CREATED,    // Created, waiting to start charging
    ORDER_STATE_CHARGING,   // Charging
    ORDER_STATE_STOPPING,   // Stopping (waiting for charging status update)
    ORDER_STATE_FINISHED,   // Order finished
    ORDER_STATE_UPLOADING,  // Uploading
    ORDER_STATE_RELEASE,    // Release order
    ORDER_STATE_ERROR       // Error state
} cloud_protocol_order_state_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    bool initialized;
    uint32_t last_update_time;        // last update time
    uint32_t last_upload_check_time;  // last upload check time
    uint32_t last_state_process_time; // last state process time
    uint32_t last_status_check_time;  // last status check time
    cloud_protocol_order_upload_cb_t upload_callback;
    cloud_protocol_network_status_cb_t network_status_callback;
    cloud_protocol_charging_status_cb_t charging_status_callback;
    cloud_protocol_auth_status_cb_t auth_status_callback;
    cloud_protocol_connector_status_cb_t connector_status_callback;
} cloud_protocol_order_manager_internal_t;

typedef struct
{
    bool initialized;                                                                                           // Whether initialized
    cloud_protocol_charging_cloud_protocol_order_manager_t *order_info[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS]; // Order information
    cloud_protocol_transaction_id_config_t config;                                                              // Configuration information
    cloud_protocol_energy_interface_t energy_iface;                                                             // Electrical energy interface
    cloud_protocol_order_state_t order_state[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS];                           // The order status of each gun
    cloud_protocol_order_charging_status_t charging_status[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS];             // last charging status
    cloud_protocol_order_auth_status_t auth_status[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS];                     // last auth status
    cloud_protocol_order_connector_status_t connector_status[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS];           // last connector status
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
static cloud_protocol_order_manager_internal_t order_mgr_internal = {0};
static cloud_protocol_order_manager_t cloud_protocol_order_manager = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void cloud_protocol_calibrate_offline_orders(cloud_protocol_charging_order_t *order);
static bool cloud_protocol_is_network_available(void);
static bool cloud_protocol_load_sequence_from_storage(void);
static bool cloud_protocol_save_sequence_to_storage(void);
static bool cloud_protocol_generate_transaction_id(uint8_t *transaction_id, uint8_t gun_no);
static bool cloud_protocol_calculate_rates(cloud_protocol_charging_order_t *order, uint32_t delta_energy);
static uint32_t cloud_protocol_calculate_total_amount(cloud_protocol_charging_order_t *order);
static bool cloud_protocol_update_order_meter_reading(uint8_t gun_no);
static bool cloud_protocol_create_order_internal(uint8_t gun_no, cloud_protocol_order_type_t type,
                                                 cloud_protocol_transaction_type_t trans_type,
                                                 const uint8_t *platform_transaction_id,
                                                 const char *vin, const uint8_t *card);
static bool cloud_protocol_start_order_internal(uint8_t gun_no);
static bool cloud_protocol_charging_order_update_internal(uint8_t gun_no);
static bool cloud_protocol_stop_order_internal(uint8_t gun_no);
static bool cloud_protocol_charging_order_finish_internal(uint8_t gun_no, cloud_protocol_StopCharging_Type_E reason);
static void cloud_protocol_release_order_internal(uint8_t gun_no);
static bool cloud_protocol_handle_state_idle(uint8_t gun_no);
static bool cloud_protocol_handle_state_created(uint8_t gun_no);
static bool cloud_protocol_handle_state_charging(uint8_t gun_no);
static bool cloud_protocol_handle_state_stopping(uint8_t gun_no);
static bool cloud_protocol_handle_state_finished(uint8_t gun_no);
static bool cloud_protocol_handle_state_uploading(uint8_t gun_no);
static void cloud_protocol_order_state_machine_process(uint8_t gun_no);
static void cloud_protocol_check_and_upload_offline_orders(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// initialize order manager
bool cloud_protocol_order_manager_init(cloud_protocol_order_upload_cb_t upload_cb,
                                       cloud_protocol_network_status_cb_t network_status_cb,
                                       cloud_protocol_charging_status_cb_t charging_status_cb,
                                       cloud_protocol_auth_status_cb_t auth_status_cb,
                                       cloud_protocol_connector_status_cb_t connector_status_cb)
{
    if (upload_cb == NULL || network_status_cb == NULL || charging_status_cb == NULL || auth_status_cb == NULL || connector_status_cb == NULL) {
        return false;
    }
    // callback assignments
    order_mgr_internal.upload_callback = upload_cb;
    order_mgr_internal.network_status_callback = network_status_cb;
    order_mgr_internal.charging_status_callback = charging_status_cb;
    order_mgr_internal.auth_status_callback = auth_status_cb;
    order_mgr_internal.connector_status_callback = connector_status_cb;
    // Initialize timestamps
    order_mgr_internal.last_update_time = CLOUD_GET_TIME_MS();
    order_mgr_internal.last_upload_check_time = CLOUD_GET_TIME_MS();
    order_mgr_internal.last_state_process_time = CLOUD_GET_TIME_MS();
    order_mgr_internal.last_status_check_time = CLOUD_GET_TIME_MS();
    // Initialize order manager
    order_mgr_internal.initialized = true;
    
    CLOUD_INFO("Order manager initialized successfully\n");
    return true;
}

// Initialize the order module
bool cloud_protocol_charging_order_init(const cloud_protocol_transaction_id_config_t *config, const cloud_protocol_energy_interface_t *interface)
{
    if (cloud_protocol_order_manager.initialized)
    {
        return true;
    }
    // reset order manager
    memset(&cloud_protocol_order_manager, 0, sizeof(cloud_protocol_order_manager_t));
    // Initialize the transaction ID configuration
    if (config != NULL)
    {
        memcpy(&cloud_protocol_order_manager.config, config, sizeof(cloud_protocol_transaction_id_config_t));
    }
    else
    {
        memset(&cloud_protocol_order_manager.config, 0, sizeof(cloud_protocol_transaction_id_config_t));
    }
    // initialize energy interface
    if (interface == NULL)
    {
        CLOUD_ERROR("Invalid energy interface\n");
        return false;
    }
    memcpy(&cloud_protocol_order_manager.energy_iface, interface, sizeof(cloud_protocol_energy_interface_t));

    // Initialize the order status of all guns to idle
    for (uint8_t i = 0; i < CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; i++)
    {
        cloud_protocol_order_manager.order_state[i] = ORDER_STATE_IDLE;
    }
    // Load persistent sequence numbers
    cloud_protocol_load_sequence_from_storage();

    cloud_protocol_order_manager.initialized = true;
    CLOUD_INFO("<%s>initialized for %d guns\n", __FUNCTION__, CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS);

    return true;
}

// Successful authorization when connector connected processing
bool cloud_protocol_order_creat(uint8_t gun_no,
                                cloud_protocol_order_type_t type,
                                cloud_protocol_transaction_type_t trans_type,
                                const uint8_t *platform_transaction_id,
                                const char *vin,
                                const uint8_t *card)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    // Check the current status. Only in an idle state can a new order be created
    if (cloud_protocol_order_manager.order_state[gun_no - 1] != ORDER_STATE_IDLE)
    {
        CLOUD_WARN("Gun %d is not in IDLE state, cannot create new order. Current state: %d\n",
                   gun_no, cloud_protocol_order_manager.order_state[gun_no - 1]);
        return false;
    }

    // Create order
    if (cloud_protocol_create_order_internal(gun_no, type, trans_type, platform_transaction_id, vin, card))
    {
        // Update status to CREATED
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_CREATED;

        CLOUD_INFO("Order created for gun %d, state: CREATED\n", gun_no);
        return true;
    }

    return false;
}

// Get active orders
cloud_protocol_charging_cloud_protocol_order_manager_t *cloud_protocol_get_active_charging_order(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return NULL;
    }

    return cloud_protocol_order_manager.order_info[gun_no - 1];
}

bool cloud_protocol_call_upload_offline_order(cloud_protocol_charging_cloud_protocol_order_manager_t *order)
{
    if (order == NULL)
    {
        return false;
    }

    // Call the upload callback
    if (order_mgr_internal.upload_callback)
    {
        order_mgr_internal.upload_callback(order->active_orders.connector_id, order);
        return true;
    }
    else
    {
        CLOUD_ERROR("<%s>Upload callback not set\r\n", __func__);
    }
    return false;
}

static void cloud_protocol_calibrate_offline_orders(cloud_protocol_charging_order_t *order)
{
    /* 1.calibrate time */
    time_t current_accurate_time = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();

    time_t recorded_start_time = 0;
    CLOUD_PROTOCOL_CP56TIME2A_CONVERT_TO_TIMESTAMP(&order->start_time, &recorded_start_time);
    time_t recorded_end_time = 0;
    CLOUD_PROTOCOL_CP56TIME2A_CONVERT_TO_TIMESTAMP(&order->end_time, &recorded_end_time);
    time_t recorded_time_offset = recorded_end_time - recorded_start_time;

    // Calculate the time offset
    time_t accurate_start_time = current_accurate_time - recorded_time_offset - CLOUD_TIME_1D_IN_MS;
    time_t accurate_end_time = current_accurate_time - CLOUD_TIME_1D_IN_MS;

    // Calibrate all time fields
    CLOUD_PROTOCOL_TIMESTAMP_CONVERT_TO_CP56TIME2A(&accurate_start_time, &order->start_time);
    CLOUD_PROTOCOL_TIMESTAMP_CONVERT_TO_CP56TIME2A(&accurate_end_time, &order->end_time);
    order->transaction_time = order->start_time;

    /* 2.calibrate amount */
    cloud_protocol_update_order_meter_reading(order->connector_id);
    order->total_amount = cloud_protocol_calculate_total_amount(order);

    CLOUD_INFO("<%s> Calibrated offline order for gun %d: start_time=%u, end_time=%u, total_amount=%u\r\n",
               __FUNCTION__, order->connector_id, accurate_start_time, accurate_end_time, order->total_amount);
}

static bool cloud_protocol_is_network_available(void)
{
    return order_mgr_internal.network_status_callback ? order_mgr_internal.network_status_callback() : false;
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
static bool cloud_protocol_calculate_rates(cloud_protocol_charging_order_t *order, uint32_t delta_energy)
{
    if (order == NULL)
    {
        return false;
    }

    // Obtain the current time to determine the rate period
    RtcTimedate_Struct *tm_info = NULL;
    CLOUD_PROTOCOL_GET_CURRENT_DATE_TIME(tm_info);
    int current_hour = tm_info->usHour;
    int current_minute = tm_info->usMinutes;

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
            rate_price = cloud_protocol_fixed_5dec_to_float(current_time_slot.peak_rate.electricity_rate) + cloud_protocol_fixed_5dec_to_float(current_time_slot.peak_rate.service_rate);
            order->peak.price = cloud_protocol_fixed_5dec(rate_price);
            order->peak.energy += delta_energy; // Update energy
            order->peak.amount += cloud_protocol_fixed_4dec(rate_price * cloud_protocol_fixed_4dec_to_float(delta_energy));
            rate_name = "Peak";
            break;
        case 1: // High
            rate_price = cloud_protocol_fixed_5dec_to_float(current_time_slot.high_rate.electricity_rate) + cloud_protocol_fixed_5dec_to_float(current_time_slot.high_rate.service_rate);
            order->high.price = cloud_protocol_fixed_5dec(rate_price);
            order->high.energy += delta_energy; // Update energy
            order->high.amount += cloud_protocol_fixed_4dec(rate_price * cloud_protocol_fixed_4dec_to_float(delta_energy));
            rate_name = "High";
            break;
        case 2: // Normal
            rate_price = cloud_protocol_fixed_5dec_to_float(current_time_slot.normal_rate.electricity_rate) + cloud_protocol_fixed_5dec_to_float(current_time_slot.normal_rate.service_rate);
            order->normal.price = cloud_protocol_fixed_5dec(rate_price);
            order->normal.energy += delta_energy; // Update energy
            order->normal.amount += cloud_protocol_fixed_4dec(rate_price * cloud_protocol_fixed_4dec_to_float(delta_energy));
            rate_name = "Normal";
            break;
        case 3: // Valley
            rate_price = cloud_protocol_fixed_5dec_to_float(current_time_slot.valley_rate.electricity_rate) + cloud_protocol_fixed_5dec_to_float(current_time_slot.valley_rate.service_rate);
            order->valley.price = cloud_protocol_fixed_5dec(rate_price);
            order->valley.energy += delta_energy; // Update energy
            order->valley.amount += cloud_protocol_fixed_4dec(rate_price * cloud_protocol_fixed_4dec_to_float(delta_energy));
            rate_name = "Valley";
            break;
        default: // Normal
            rate_price = cloud_protocol_fixed_5dec_to_float(current_time_slot.normal_rate.electricity_rate) + cloud_protocol_fixed_5dec_to_float(current_time_slot.normal_rate.service_rate);
            order->normal.price = cloud_protocol_fixed_5dec(rate_price);
            order->normal.energy += delta_energy; // Update energy
            order->normal.amount += cloud_protocol_fixed_4dec(rate_price * cloud_protocol_fixed_4dec_to_float(delta_energy));
            rate_name = "Default";
            break;
    }

    // Debugging information
    CLOUD_INFO("Gun %d: +%.4f kWh at %s rate (%.5f yuan/kWh)\r\n",
           order->connector_id, cloud_protocol_fixed_4dec_to_float(delta_energy), rate_name, rate_price);

    return true;
}

static uint32_t cloud_protocol_calculate_total_amount(cloud_protocol_charging_order_t *order)
{
    if (order == NULL)
    {
        return 0;
    }

    uint32_t total_amount = 0;

    // Calculate total amount for each charging session
    total_amount += order->peak.amount;
    total_amount += order->high.amount;
    total_amount += order->normal.amount;
    total_amount += order->valley.amount;

    return total_amount;
}

static bool cloud_protocol_update_order_meter_reading(uint8_t gun_no)
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

    if (cloud_protocol_order_manager.energy_iface.get_meter_reading != NULL)
    {
        uint32_t current_reading = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);
        // Prevent the electricity meter reading from reverting
        if (current_reading >= order->last_meter_reading)
        {
            uint32_t delta_energy = current_reading - order->last_meter_reading;

            order->last_meter_reading = current_reading;
            // Calculate the rate
            cloud_protocol_calculate_rates(&order->active_orders, delta_energy);
            return true;
        }
        else
        {
            CLOUD_WARN("<%s>Meter reading reverted, gun_no: %d, last: %u, current: %u\r\n",
                       __FUNCTION__, gun_no, order->last_meter_reading, current_reading);
        }
    }
    else
    {
        CLOUD_ERROR("<%s>Failed to get meter reading, gun_no: %d\n", __FUNCTION__, gun_no);
    }

    return false;
}

// Create a new order
static bool cloud_protocol_create_order_internal(uint8_t gun_no, cloud_protocol_order_type_t type,
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
    order->status = CLOUD_PROTOCOL_ORDER_STATUS_CREATED;

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

    // Set the start time
    if (order->order_type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE)
    {
        CLOUD_PROTOCOL_GET_CP56TIME2A(&order->active_orders.transaction_time);
    }
    else
    {
        uint32_t lv_timestamp = CLOUD_GET_TIME_MS();
        CLOUD_PROTOCOL_TIMESTAMP_CONVERT_TO_CP56TIME2A(&lv_timestamp, &order->active_orders.transaction_time);
    }
    order->active_orders.start_time = order->active_orders.transaction_time;
    
    // Set the start energy
    if (cloud_protocol_order_manager.energy_iface.get_meter_reading != NULL)
    {
        order->last_meter_reading = cloud_protocol_order_manager.energy_iface.get_meter_reading(gun_no);
        order->active_orders.total_start = order->last_meter_reading;
    }
    else
    {
        CLOUD_ERROR("<%s>Failed to get meter reading, gun_no: %d\n", __FUNCTION__, gun_no);
        return false;
    }

    // Set timestamps
    order->create_timestamp = CLOUD_GET_TIME_MS();

    // Save to the manager
    cloud_protocol_order_manager.order_info[gun_no - 1] = order;

    CLOUD_INFO("<%s> gun num:%d, type:%s, trans_type:0x%02X transaction id:%s, transaction id:", __FUNCTION__,
               gun_no, (type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE) ? "Online" : "Offline", trans_type);
    CLOUD_PRINT_HEX(order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);

    return true;
}

// Start the order
static bool cloud_protocol_start_order_internal(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return false;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order == NULL || order->status != CLOUD_PROTOCOL_ORDER_STATUS_CREATED)
    {
        CLOUD_ERROR("<%s>Invalid order state, gun_no: %d, status: %d\r\n", __FUNCTION__, gun_no, order ? order->status : -1);
        return false;
    }

    // Update the order status
    order->status = CLOUD_PROTOCOL_ORDER_STATUS_CHARGING;

    // Update the state machine status
    cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_CHARGING;

    CLOUD_INFO("<%s>Gun: %d\n", __FUNCTION__, gun_no);
    return true;
}

// Update active orders
static bool cloud_protocol_charging_order_update_internal(uint8_t gun_no)
{

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order == NULL || (order->status != CLOUD_PROTOCOL_ORDER_STATUS_CHARGING))
    {
        return false;
    }

    // Update energy data
    return cloud_protocol_update_order_meter_reading(gun_no); 
}

// stop the order
static bool cloud_protocol_stop_order_internal(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

    if (order == NULL)
    {
        return false;
    }

    order->status = CLOUD_PROTOCOL_ORDER_STATUS_STOPPING;

    // Update the state machine status
    cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_STOPPING;

    CLOUD_INFO("Order stopping internally for gun %d, state: STOPPING\n", gun_no);
    return true;
}

// finish the order
static bool cloud_protocol_charging_order_finish_internal(uint8_t gun_no, cloud_protocol_StopCharging_Type_E reason)
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
    }
    else
    {
        uint32_t lv_timestamp = CLOUD_GET_TIME_MS();
        CLOUD_PROTOCOL_TIMESTAMP_CONVERT_TO_CP56TIME2A(&lv_timestamp, &order->active_orders.end_time);
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

    // Final calculated rate for online orders when device is online
    if (order->order_type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE && cloud_protocol_is_network_available())
    {
        cloud_protocol_update_order_meter_reading(gun_no);
        order->active_orders.total_amount = cloud_protocol_calculate_total_amount(&order->active_orders);
    }

    order->active_orders.total_energy = (uint32_t)(order->active_orders.total_end - order->active_orders.total_start);

    order->status = CLOUD_PROTOCOL_ORDER_STATUS_FINISHED;

    CLOUD_INFO("<%s>gun_no:%d transaction id:\r\n", gun_no);
    CLOUD_PRINT_HEX(order->active_orders.transaction_id, CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH);


    // Update the state machine status
    cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_FINISHED;

    return true;
}

// release the charging order
static void cloud_protocol_release_order_internal(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return;
    }

    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];
    if (order != NULL)
    {
        CLOUDM_FREE(order);
        cloud_protocol_order_manager.order_info[gun_no - 1] = NULL;
        CLOUD_INFO("<%s>Released order for gun %d\r\n", __FUNCTION__, gun_no);
    }
}

// Idle state processing
static bool cloud_protocol_handle_state_idle(uint8_t gun_no)
{
    // nothing to do in idle state
    return false;
}


// created state processing
static bool cloud_protocol_handle_state_created(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

    if (order == NULL)
    {
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
        return true;
    }

    uint32_t current_time = CLOUD_GET_TIME_MS();
    cloud_protocol_order_auth_status_t auth_status = cloud_protocol_order_manager.auth_status[gun_no - 1];
    cloud_protocol_order_charging_status_t charging_status = cloud_protocol_order_manager.charging_status[gun_no - 1];
    cloud_protocol_order_connector_status_t connector_status = cloud_protocol_order_manager.connector_status[gun_no - 1];

    // Check if the authorization has timed out (5 minutes)
    if (current_time - cloud_protocol_order_manager.order_info[gun_no - 1]->create_timestamp > CLOUD_PROTOCOL_ORDER_AUTH_CHECK_PERIOD_MS && connector_status == CONNECTOR_STATUS_CONNECTED)
    {
        CLOUD_WARN("Authorization timeout for gun %d, releasing order\n", gun_no);
        cloud_protocol_stop_order_internal(gun_no);
        return true;
    }

    // Check if the authorization status is still valid
    if (auth_status == AUTH_STATUS_UNAUTHORIZED && connector_status == CONNECTOR_STATUS_DISCONNECTED)
    {
        CLOUD_WARN("Authorization lost for gun %d, stopping order\n", gun_no);
        cloud_protocol_charging_order_finish_internal(gun_no, CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_HOST_CONFIG);
        return true;
    }

    // Check if charging has started
    if (charging_status == CHARGING_STATUS_CHARGING)
    {
        return cloud_protocol_start_order_internal(gun_no);
    }

    return false;
}

// Charging state processing
static bool cloud_protocol_handle_state_charging(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

    if (order == NULL)
    {
        // There are no orders, so it's back to an idle state
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
        CLOUD_WARN("<%s> no alive order.\r\n", __func__);
        return true;
    }

    cloud_protocol_order_auth_status_t auth_status = cloud_protocol_order_manager.auth_status[gun_no - 1];
    cloud_protocol_order_charging_status_t charging_status = cloud_protocol_order_manager.charging_status[gun_no - 1];
    cloud_protocol_order_connector_status_t connector_status = cloud_protocol_order_manager.connector_status[gun_no - 1];

    // Check stop charging condition
    if (auth_status != AUTH_STATUS_AUTHORIZED || connector_status != CONNECTOR_STATUS_CONNECTED)
    {
        CLOUD_WARN("<%s> gun %d, finishing order, <auth_status: %d, connector_status: %d>\n", __func__, gun_no, auth_status, connector_status);
        cloud_protocol_charging_order_finish_internal(gun_no, CLOUD_PROTOCOL_STOP_CHARGING_TYPE_MANUAL_STOP);
        return true;
    }

    // Check if the charging status has stopped
    if (charging_status != CHARGING_STATUS_CHARGING)
    {
        CLOUD_INFO("<%s> gun %d, stopping order\n", __func__, gun_no);
        cloud_protocol_stop_order_internal(gun_no);
        return true;
    }

    return false;
}

// Stop status processing.
static bool cloud_protocol_handle_state_stopping(uint8_t gun_no)
{
    cloud_protocol_order_auth_status_t auth_status = cloud_protocol_order_manager.auth_status[gun_no - 1];
    cloud_protocol_order_charging_status_t charging_status = cloud_protocol_order_manager.charging_status[gun_no - 1];
    cloud_protocol_order_connector_status_t connector_status = cloud_protocol_order_manager.connector_status[gun_no - 1];

    if (CHARGING_STATUS_CHARGING == charging_status)
    {
        // Update the state machine status
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_CHARGING;
        CLOUD_INFO("<%s> gun %d revert charging.\n", __func__, gun_no);
        return true;
    }
    else if (auth_status == AUTH_STATUS_UNAUTHORIZED && connector_status == CONNECTOR_STATUS_DISCONNECTED)
    {
        CLOUD_WARN("<%s> gun %d stopping order\n", __func__, gun_no);
        cloud_protocol_charging_order_finish_internal(gun_no, CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_HOST_CONFIG);
        return true;
    }
    else
    {

    }

    return false;
}

// Finished state processing
static bool cloud_protocol_handle_state_finished(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

    if (order == NULL)
    {
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
        CLOUD_WARN("<%s> no alive order.\r\n", __func__);
        return true;
    }

    // Check network status
    if (cloud_protocol_is_network_available())
    {
        // Network is available, enter uploading state
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_UPLOADING;
        return true;
    }
    else
    {
        // Network is unavailable, save to TSDB
        if (cloud_protocol_save_order_to_tsdb(gun_no))
        {
            cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_RELEASE;
            return true;
        }
        else
        {
            CLOUD_ERROR("Failed to save order to TSDB for gun %d\n", gun_no);
            return false;
        }
    }
}

// Uploading state processing
static bool cloud_protocol_handle_state_uploading(uint8_t gun_no)
{
    cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

    if (order == NULL)
    {
        cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
        CLOUD_WARN("<%s> no alive order.\r\n", __func__);
        return true;
    }

    // Call upload callback function
    if (order_mgr_internal.upload_callback)
    {
        if (order->order_type == CLOUD_PROTOCOL_ORDER_TYPE_OFFLINE)
        {
            cloud_protocol_calibrate_offline_orders(&order->active_orders);
        }
        order_mgr_internal.upload_callback(gun_no, order);
    }

    // Mark as uploaded
    order->status = CLOUD_PROTOCOL_ORDER_STATUS_UPLOADED;
    cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_RELEASE;

    CLOUD_INFO("Order uploaded successfully for gun %d\n", gun_no);
    return true;
}

// Order state machine processing function
static void cloud_protocol_order_state_machine_process(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS)
    {
        return;
    }

    // get current status
    cloud_protocol_order_manager.auth_status[gun_no - 1] = order_mgr_internal.auth_status_callback(gun_no);
    cloud_protocol_order_manager.charging_status[gun_no - 1] = order_mgr_internal.charging_status_callback(gun_no);
    cloud_protocol_order_manager.connector_status[gun_no - 1] = order_mgr_internal.connector_status_callback(gun_no);

    cloud_protocol_order_state_t current_state = cloud_protocol_order_manager.order_state[gun_no - 1];
    bool state_changed = false;
    
    switch (current_state) 
    {
        case ORDER_STATE_IDLE:
            state_changed = cloud_protocol_handle_state_idle(gun_no);
            break;
            
        case ORDER_STATE_CREATED:
            state_changed = cloud_protocol_handle_state_created(gun_no);
            break;
            
        case ORDER_STATE_CHARGING:
            state_changed = cloud_protocol_handle_state_charging(gun_no);
            break;
            
        case ORDER_STATE_STOPPING:
            state_changed = cloud_protocol_handle_state_stopping(gun_no);
            break;
            
        case ORDER_STATE_FINISHED:
            state_changed = cloud_protocol_handle_state_finished(gun_no);
            break;
            
        case ORDER_STATE_UPLOADING:
            state_changed = cloud_protocol_handle_state_uploading(gun_no);
            break;

        case ORDER_STATE_RELEASE:
            cloud_protocol_release_order_internal(gun_no);
            cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
            state_changed = true;
            CLOUD_INFO("Order cleaned up for gun %d, state: IDLE\n", gun_no);
            break;
            
        case ORDER_STATE_ERROR:
            CLOUD_ERROR("Order in error state for gun %d\n", gun_no);
            break;
            
        default:
            break;
    }

    if (state_changed)
    {
        CLOUD_INFO("Gun %d state changed to: %d\n", gun_no, cloud_protocol_order_manager.order_state[gun_no - 1]);
    }
}

static void cloud_protocol_check_and_upload_offline_orders(void)
{
    // Only upload offline orders when the network is connected and there are no active orders
    if (cloud_protocol_is_network_available())
    {
        for (uint8_t gun_no = 1; gun_no <= CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; gun_no++)
        {
            cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_get_active_charging_order(gun_no);
            if (order != NULL)
            {
                cloud_protocol_process_offline_order_upload(gun_no);
            }
        }
    }
}

// Order manager process function
void cloud_protocol_order_manager_process(void)
{
    if (!order_mgr_internal.initialized)
    {
        return;
    }

    uint32_t current_time = CLOUD_GET_TIME_MS();

    // Process the state machine every 100ms
    if (current_time - order_mgr_internal.last_state_process_time >= CLOUD_PROTOCOL_ORDER_PROCESS_PERIOD_MS)
    {
        for (uint8_t gun_no = 1; gun_no <= CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; gun_no++)
        {
            cloud_protocol_order_state_machine_process(gun_no);
        }
        order_mgr_internal.last_state_process_time = current_time;
    }

    // Update the electricity meter reading of the order every 30 seconds (only for online orders that are charging state when the device is online)
    if (current_time - order_mgr_internal.last_update_time >= CLOUD_PROTOCOL_ORDER_UPDATE_PERIOD_MS)
    {
        for (uint8_t gun_no = 1; gun_no <= CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; gun_no++)
        {
            if (cloud_protocol_order_manager.order_state[gun_no - 1] == ORDER_STATE_CHARGING &&\
                cloud_protocol_order_manager.order_info[gun_no - 1]->order_type == CLOUD_PROTOCOL_ORDER_TYPE_ONLINE\
                && cloud_protocol_is_network_available())
            {
                cloud_protocol_charging_order_update_internal(gun_no);
                CLOUD_INFO("<%s> gun:%d order update\r\n", __func__, gun_no);
            }
        }
        order_mgr_internal.last_update_time = current_time;
    }

    // Check every 60 seconds to see if offline orders need to be uploaded
    if (current_time - order_mgr_internal.last_upload_check_time >= CLOUD_PROTOCOL_ORDER_OFFLINE_UPLOAD_CHECK_PERIOD_MS)
    {
        cloud_protocol_check_and_upload_offline_orders();
        order_mgr_internal.last_upload_check_time = current_time;
    }
}

// Power failure handling function
void cloud_protocol_order_handle_power_loss(void)
{
    CLOUD_WARN("Power loss detected, saving current orders...\n");

    // Save all active orders to TSDB
    for (uint8_t gun_no = 1; gun_no <= CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS; gun_no++)
    {
        cloud_protocol_charging_cloud_protocol_order_manager_t *order = cloud_protocol_order_manager.order_info[gun_no - 1];

        if (order != NULL)
        {
            // Update final meter reading
            cloud_protocol_update_order_meter_reading(gun_no);

            // update stop reason
            order->active_orders.stop_reason = CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_POWER_OFF;

            // Save to TSDB
            if (cloud_protocol_save_order_to_tsdb(gun_no))
            {
                CLOUD_INFO("Order saved to TSDB due to power loss for gun %d\n", gun_no);
            }
            else
            {
                CLOUD_ERROR("Failed to save order to TSDB for gun %d\n", gun_no);
            }

            // Release the order memory
            cloud_protocol_release_order_internal(gun_no);
            cloud_protocol_order_manager.order_state[gun_no - 1] = ORDER_STATE_IDLE;
        }
    }

    CLOUD_INFO("Power loss handling completed\n");
}
/* EOL */
