//******************************************************************************
//* File Name: Cloud_EV_Charger_Information_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud EV Charger Information module header file
/*******************************************************************************/
#if !defined (__CLOUD_EV_CHARGER_INFORMATION_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_EV_CHARGER_INFORMATION_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "STD_SysM_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_EV_SN_LEN                (16U)
#define CLOUD_EV_MANUFACTURER_LEN      (32U)
#define CLOUD_EV_FIRMWARE_VERSION_LEN  (8U)
#define CLOUD_EV_HARDWARE_VERSION_LEN  (8U)

#define CLOUD_EV_MAX_CONNECTORS        (SYS_CONNECTOR_NUM_MAX) // Maximum number of connectors supported

#define CLOUD_EV_MAX_CHARGING_VOLTAGE   (1000U) // V
#define CLOUD_EV_MAX_CHARGING_CURRENT   (125U) // A
#define CLOUD_EV_MAX_TEMPERATURE        (150U)  // °C

#define CLOUD_EV_DEBUG(fmt, ...) 	                                    LOG_DEBUG(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_EV_ERR(fmt, ...) 	                                        LOG_ERROR(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_EVSE_STATUS_OFFLINE = 0, // Offline
    CLOUD_EVSE_STATUS_FAULTED = 1, // Faulted
    CLOUD_EVSE_STATUS_IDLE = 2,    // Idle
    CLOUD_EVSE_STATUS_CHARGING = 3 // Charging
} Cloud_Evse_StatusType_E;

typedef enum
{
    CLOUD_EV_CHARGER_CONNECTOR_NORMAL = 0, // Normal
    CLOUD_EV_CHARGER_CONNECTOR_ERROR = 1, // Error
} Cloud_Ev_Connector_StatusType_E;

typedef enum
{
    CLOUD_EV_CHARGER_CONNECTOR_INUSE = 0, // In Use
    CLOUD_EV_CHARGER_CONNECTOR_GO_BACK = 1, // Go Back
    CLOUD_EV_CHARGER_CONNECTOR_UNKNOWN = 2 // Unknown
} Cloud_Ev_ConnectorGoBack_StatusType_E;

typedef enum
{
    CLOUD_EV_TRANSACTION_ID_TYPE_APP = 0,
    CLOUD_EV_TRANSACTION_ID_TYPE_RFID = 1,
    CLOUD_EV_TRANSACTION_ID_TYPE_RFID_OFFLINE = 2,
    CLOUD_EV_TRANSACTION_ID_TYPE_VIN = 3
} Cloud_Ev_Transaction_IdentifierType_E;

typedef enum
{
    // ==================== Charge Completed (0x40-0x49) ====================
    CLOUD_EV_STOP_CHARGING_TYPE_APP_REMOTE = 0x40,  // APP remote stop, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_SOC_100 = 0x41,     // SOC reaches 100%, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_ENERGY_SET = 0x42,  // Energy meets set condition, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_AMOUNT_SET = 0x43,  // Amount meets set condition, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_TIME_SET = 0x44,    // Time meets set condition, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_MANUAL_STOP = 0x45, // Manual stop, stop charging
    CLOUD_EV_STOP_CHARGING_TYPE_OTHER_1 = 0x46,     // Other method (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_OTHER_2 = 0x47,     // Other method (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_OTHER_3 = 0x48,     // Other method (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_OTHER_4 = 0x49,     // Other method (reserved)

    // ==================== Charge Start Failed (0x4A-0x69) ====================
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_SYSTEM = 0x4A,            // Start failed, charger control system fault
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_GUIDE = 0x4B,             // Start failed, guide disconnected
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BREAKER = 0x4C,           // Start failed, breaker tripped
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_METER_COMM = 0x4D,        // Start failed, meter communication interrupted
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BALANCE = 0x4E,           // Start failed, insufficient balance
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_MODULE = 0x4F,            // Start failed, charging module fault
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_ESTOP = 0x50,             // Start failed, emergency stop input
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_LIGHTNING = 0x51,         // Start failed, lightning protection abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BMS_READY = 0x52,         // Start failed, BMS not ready
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_TEMP = 0x53,              // Start failed, temperature abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BATTERY_REVERSE = 0x54,   // Start failed, battery reverse fault
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_ELOCK = 0x55,             // Start failed, electronic lock abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_CLOSE_FAIL = 0x56,        // Start failed, closing failed
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_INSULATION = 0x57,        // Start failed, insulation abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_RESERVED_1 = 0x58,        // Reserved
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BHM_TIMEOUT = 0x59,       // Start failed, BMS handshake message BHM timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BRM_TIMEOUT = 0x5A,       // Start failed, BMS/vehicle identification message BRM timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BCP_TIMEOUT = 0x5B,       // Start failed, battery charge parameter message BCP timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BRO_TIMEOUT = 0x5C,       // Start failed, BMS ready message BRO timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BCS_TIMEOUT = 0x5D,       // Start failed, battery charge status message BCS timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BCL_TIMEOUT = 0x5E,       // Start failed, battery charge request message BCL timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_BSM_TIMEOUT = 0x5F,       // Start failed, battery status message BSM timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_GB2015_VOLTAGE = 0x60,    // Start failed, GB2015 battery voltage not allowed in BHM phase
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_GB2015_VOLT_DIFF = 0x61,  // Start failed, GB2015 voltage difference >5% in BRO phase
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_GB2015_PRECHARGE = 0x62,  // Start failed, GB2015 charger precharge phase BRO_AA to BRO_00
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_HOST_CONFIG = 0x63,       // Start failed, host config message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_CHARGER_NOT_READY = 0x64, // Start failed, charger not ready
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_OTHER_1 = 0x65,           // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_OTHER_2 = 0x66,           // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_OTHER_3 = 0x67,           // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_OTHER_4 = 0x68,           // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_START_FAIL_OTHER_5 = 0x69,           // Other reason (reserved)

    // ==================== Charge Abnormal Stop (0x6A-0x8F) ====================
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_SYSTEM_LOCK = 0x6A,      // Abnormal stop, system locked
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_GUIDE_DISCONNECT = 0x6B, // Abnormal stop, guide disconnected
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BREAKER_JUMP = 0x6C,     // Abnormal stop, breaker tripped
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_METER_COMM = 0x6D,       // Abnormal stop, meter communication interrupted
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BALANCE = 0x6E,          // Abnormal stop, insufficient balance
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_AC_PROTECT = 0x6F,       // Abnormal stop, AC protection action
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_DC_PROTECT = 0x70,       // Abnormal stop, DC protection action
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_MODULE = 0x71,           // Abnormal stop, charging module fault
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_ESTOP = 0x72,            // Abnormal stop, emergency stop input
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_LIGHTNING = 0x73,        // Abnormal stop, lightning protection abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_TEMP = 0x74,             // Abnormal stop, temperature abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OUTPUT = 0x75,           // Abnormal stop, output abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_NO_CURRENT = 0x76,       // Abnormal stop, no charging current
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_ELOCK = 0x77,            // Abnormal stop, electronic lock abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_RESERVED_1 = 0x78,       // Reserved
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_TOTAL_VOLT = 0x79,       // Abnormal stop, total charging voltage abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_TOTAL_CURRENT = 0x7A,    // Abnormal stop, total charging current abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_CELL_VOLT = 0x7B,        // Abnormal stop, cell voltage abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BATTERY_TEMP = 0x7C,     // Abnormal stop, battery over temperature
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_MAX_CELL_VOLT = 0x7D,    // Abnormal stop, max cell voltage abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_MAX_BATTERY_TEMP = 0x7E, // Abnormal stop, max battery over temperature
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BMV_VOLT = 0x7F,         // Abnormal stop, BMV cell voltage abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BMT_TEMP = 0x80,         // Abnormal stop, BMT battery over temperature
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BATTERY_STATUS = 0x81,   // Abnormal stop, battery status abnormal
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_VEHICLE_FORBID = 0x82,   // Abnormal stop, vehicle forbids charging
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_POWER_OFF = 0x83,        // Abnormal stop, charger power off
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BCS_TIMEOUT = 0x84,      // Abnormal stop, battery charge status message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BCL_TIMEOUT = 0x85,      // Abnormal stop, battery charge request message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BSM_TIMEOUT = 0x86,      // Abnormal stop, battery status message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BMS_STOP_TIMEOUT = 0x87, // Abnormal stop, BMS stop charge message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_BMS_STAT_TIMEOUT = 0x88, // Abnormal stop, BMS charge statistics message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_CCS_TIMEOUT = 0x89,      // Abnormal stop, CCS message timeout
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_1 = 0x8A,          // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_2 = 0x8B,          // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_3 = 0x8C,          // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_4 = 0x8D,          // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_5 = 0x8E,          // Other reason (reserved)
    CLOUD_EV_STOP_CHARGING_TYPE_ABNORMAL_OTHER_6 = 0x8F,          // Other reason (reserved)

    // ==================== Unknown Stop Reason (0x90) ====================
    CLOUD_EV_STOP_CHARGING_TYPE_UNKNOWN = 0x90 // Unknown stop reason
} Cloud_Ev_StopCharging_Type_E;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_EV_CHARGER_INFORMATION_CFG_H */
/* EOL */
