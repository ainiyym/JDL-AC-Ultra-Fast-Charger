//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrderInfomation.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Charging Order module header information file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CHARGING_ORDER_INFORMATION_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CHARGING_ORDER_INFORMATION_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_EV_Charger_Information.h"
#include "Cloud_Protocol_BillingModelM.h"
#include "Cloud_Cfg.h"

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
    CLOUD_PROTOCOL_ORDER_TYPE_ONLINE = 0x01,          // online
    CLOUD_PROTOCOL_ORDER_TYPE_OFFLINE = 0x02,         // offline
} cloud_protocol_order_type_t;

typedef enum
{
    CLOUD_PROTOCOL_ORDER_STATUS_INIT = 0,     // initialized
    CLOUD_PROTOCOL_ORDER_STATUS_CREATED = 1,  // created
    CLOUD_PROTOCOL_ORDER_STATUS_CHARGING = 2, // charging
    CLOUD_PROTOCOL_ORDER_STATUS_STOPPING = 3, // stopping
    CLOUD_PROTOCOL_ORDER_STATUS_FINISHED = 4, // finished
    CLOUD_PROTOCOL_ORDER_STATUS_UPLOADED = 5  // uploaded
} cloud_protocol_order_status_t;

typedef enum
{
    CLOUD_PROTOCOL_TRANSACTION_APP_START = 0x01,          // APP
    CLOUD_PROTOCOL_TRANSACTION_CARD_START = 0x02,         // RFID
    CLOUD_PROTOCOL_TRANSACTION_OFFLINE_CARD_START = 0x04, // RFID_OFFLINE
    CLOUD_PROTOCOL_TRANSACTION_VIN_START = 0x05           // VIN
} cloud_protocol_transaction_type_t;

typedef enum
{
    // ==================== Charge Completed (0x40-0x49) ====================
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_APP_REMOTE = 0x40,  // APP remote stop, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_SOC_100 = 0x41,     // SOC reaches 100%, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ENERGY_SET = 0x42,  // Energy meets set condition, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_AMOUNT_SET = 0x43,  // Amount meets set condition, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_TIME_SET = 0x44,    // Time meets set condition, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_MANUAL_STOP = 0x45, // Manual stop, stop charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_OTHER_1 = 0x46,     // Other method (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_OTHER_2 = 0x47,     // Other method (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_OTHER_3 = 0x48,     // Other method (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_OTHER_4 = 0x49,     // Other method (reserved)

    // ==================== Charge Start Failed (0x4A-0x69) ====================
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_SYSTEM = 0x4A,            // Start failed, charger control system fault
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_GUIDE = 0x4B,             // Start failed, guide disconnected
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BREAKER = 0x4C,           // Start failed, breaker tripped
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_METER_COMM = 0x4D,        // Start failed, meter communication interrupted
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BALANCE = 0x4E,           // Start failed, insufficient balance
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_MODULE = 0x4F,            // Start failed, charging module fault
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_ESTOP = 0x50,             // Start failed, emergency stop input
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_LIGHTNING = 0x51,         // Start failed, lightning protection abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BMS_READY = 0x52,         // Start failed, BMS not ready
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_TEMP = 0x53,              // Start failed, temperature abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BATTERY_REVERSE = 0x54,   // Start failed, battery reverse fault
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_ELOCK = 0x55,             // Start failed, electronic lock abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_CLOSE_FAIL = 0x56,        // Start failed, closing failed
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_INSULATION = 0x57,        // Start failed, insulation abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_RESERVED_1 = 0x58,        // Reserved
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BHM_TIMEOUT = 0x59,       // Start failed, BMS handshake message BHM timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BRM_TIMEOUT = 0x5A,       // Start failed, BMS/vehicle identification message BRM timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BCP_TIMEOUT = 0x5B,       // Start failed, battery charge parameter message BCP timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BRO_TIMEOUT = 0x5C,       // Start failed, BMS ready message BRO timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BCS_TIMEOUT = 0x5D,       // Start failed, battery charge status message BCS timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BCL_TIMEOUT = 0x5E,       // Start failed, battery charge request message BCL timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_BSM_TIMEOUT = 0x5F,       // Start failed, battery status message BSM timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_GB2015_VOLTAGE = 0x60,    // Start failed, GB2015 battery voltage not allowed in BHM phase
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_GB2015_VOLT_DIFF = 0x61,  // Start failed, GB2015 voltage difference >5% in BRO phase
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_GB2015_PRECHARGE = 0x62,  // Start failed, GB2015 charger precharge phase BRO_AA to BRO_00
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_HOST_CONFIG = 0x63,       // Start failed, host config message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_CHARGER_NOT_READY = 0x64, // Start failed, charger not ready
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_OTHER_1 = 0x65,           // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_OTHER_2 = 0x66,           // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_OTHER_3 = 0x67,           // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_OTHER_4 = 0x68,           // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_START_FAIL_OTHER_5 = 0x69,           // Other reason (reserved)

    // ==================== Charge Abnormal Stop (0x6A-0x8F) ====================
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_SYSTEM_LOCK = 0x6A,      // Abnormal stop, system locked
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_GUIDE_DISCONNECT = 0x6B, // Abnormal stop, guide disconnected
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BREAKER_JUMP = 0x6C,     // Abnormal stop, breaker tripped
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_METER_COMM = 0x6D,       // Abnormal stop, meter communication interrupted
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BALANCE = 0x6E,          // Abnormal stop, insufficient balance
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_AC_PROTECT = 0x6F,       // Abnormal stop, AC protection action
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_DC_PROTECT = 0x70,       // Abnormal stop, DC protection action
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_MODULE = 0x71,           // Abnormal stop, charging module fault
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_ESTOP = 0x72,            // Abnormal stop, emergency stop input
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_LIGHTNING = 0x73,        // Abnormal stop, lightning protection abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_TEMP = 0x74,             // Abnormal stop, temperature abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OUTPUT = 0x75,           // Abnormal stop, output abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_NO_CURRENT = 0x76,       // Abnormal stop, no charging current
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_ELOCK = 0x77,            // Abnormal stop, electronic lock abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_RESERVED_1 = 0x78,       // Reserved
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_TOTAL_VOLT = 0x79,       // Abnormal stop, total charging voltage abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_TOTAL_CURRENT = 0x7A,    // Abnormal stop, total charging current abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_CELL_VOLT = 0x7B,        // Abnormal stop, cell voltage abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BATTERY_TEMP = 0x7C,     // Abnormal stop, battery over temperature
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_MAX_CELL_VOLT = 0x7D,    // Abnormal stop, max cell voltage abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_MAX_BATTERY_TEMP = 0x7E, // Abnormal stop, max battery over temperature
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BMV_VOLT = 0x7F,         // Abnormal stop, BMV cell voltage abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BMT_TEMP = 0x80,         // Abnormal stop, BMT battery over temperature
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BATTERY_STATUS = 0x81,   // Abnormal stop, battery status abnormal
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_VEHICLE_FORBID = 0x82,   // Abnormal stop, vehicle forbids charging
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_POWER_OFF = 0x83,        // Abnormal stop, charger power off
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BCS_TIMEOUT = 0x84,      // Abnormal stop, battery charge status message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BCL_TIMEOUT = 0x85,      // Abnormal stop, battery charge request message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BSM_TIMEOUT = 0x86,      // Abnormal stop, battery status message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BMS_STOP_TIMEOUT = 0x87, // Abnormal stop, BMS stop charge message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_BMS_STAT_TIMEOUT = 0x88, // Abnormal stop, BMS charge statistics message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_CCS_TIMEOUT = 0x89,      // Abnormal stop, CCS message timeout
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_1 = 0x8A,          // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_2 = 0x8B,          // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_3 = 0x8C,          // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_4 = 0x8D,          // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_5 = 0x8E,          // Other reason (reserved)
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_ABNORMAL_OTHER_6 = 0x8F,          // Other reason (reserved)

    // ==================== Unknown Stop Reason (0x90) ====================
    CLOUD_PROTOCOL_STOP_CHARGING_TYPE_UNKNOWN = 0x90 // Unknown stop reason
} cloud_protocol_StopCharging_Type_E;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// Rate information structure
typedef struct
{
    uint32_t price;       // Unit price (accurate to five decimal places)
    uint32_t energy;      // Battery power (accurate to four decimal places)
    uint32_t loss_energy; // Measured loss power (accurate to four decimal places)
    uint32_t amount;      // Amount (accurate to four decimal places)
} cloud_protocol_rate_info_t;

// charging order structure
typedef struct
{
    uint8_t transaction_id[CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH]; // Transaction ID (BCD code)
    uint8_t SN[CLOUD_PROTOCOL_SN_LENGTH];                         // Pile number (BCD code)
    uint8_t connector_id;                                         // Connector ID (BCD code)
    CP56Time2a_t start_time;                                      // Start time
    CP56Time2a_t end_time;                                        // End time
    cloud_protocol_rate_info_t peak;                              // Peak
    cloud_protocol_rate_info_t high;                              // High
    cloud_protocol_rate_info_t normal;                            // Normal
    cloud_protocol_rate_info_t valley;                            // Valley
    uint64_t total_start;                                         // The total starting value of the electricity meter (accurate to four decimal places)
    uint64_t total_end;                                           // Total stop value of the electricity meter (accurate to four decimal places)
    uint32_t total_energy;                                        // total energy (accurate to four decimal places)
    uint32_t total_loss_energy;                                   // total loss energy (accurate to four decimal places)
    uint32_t total_amount;                                        // total amount (accurate to four decimal places)
    char vin[17];                                                 // Electric vehicle unique identifier (17 bytes ASCII)
    cloud_protocol_transaction_type_t transaction_type;           // Transaction identifier
    CP56Time2a_t transaction_time;                                // Transaction time
    cloud_protocol_StopCharging_Type_E stop_reason;               // Stop reason
    uint8_t physical_card[CLOUD_PROTOCOL_RFID_UID_LENGTH];        // Physical card number (8 bytes BIN code)
} cloud_protocol_charging_order_t;

typedef struct
{
    uint64_t create_timestamp;                       // creation timestamp(For internal computing only)
    uint32_t last_meter_reading;                   // last meter reading(The total electricity of the electricity meter)
    cloud_protocol_order_type_t order_type;        // transaction type
    cloud_protocol_order_status_t status;          // transaction status
    cloud_protocol_charging_order_t active_orders; // transaction information
} cloud_protocol_charging_cloud_protocol_order_manager_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_PROTOCOL_CHARGING_ORDER_INFORMATION_H */
/* EOL */
