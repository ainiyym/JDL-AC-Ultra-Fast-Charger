//******************************************************************************
//* File Name: Cloud_EV_Charger_Information_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud EV Charger Information module header file
/*******************************************************************************/
#if !defined (__CLOUD_EV_CHARGER_INFORMATION_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_EV_CHARGER_INFORMATION_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "Cloud_EV_Charger_Information_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum {
    CLOUD_CONST_MANUFACTURER = 0,
    CLOUD_CONST_SERIAL_NUMBER,
    CLOUD_CONST_FIRMWARE_VERSION,
    CLOUD_CONST_HARDWARE_VERSION,
    CLOUD_CONST_CONNECTOR_TYPE,
    CLOUD_CONST_NUMBER_OF_CONNECTORS,
    CLOUD_CONST_MAX_CHARGING_VOLTAGE,
    CLOUD_CONST_MAX_CHARGING_CURRENT,
    CLOUD_CONST_ALL_FIELDS  // Special values represent all fields
} Cloud_Constant_Field_E;

typedef enum {
    CLOUD_DYNAMIC_EVSE_STATUS = 0,
    CLOUD_DYNAMIC_GO_BACK_STATUS,
    CLOUD_DYNAMIC_CONNECTOR_STATUS,
    CLOUD_DYNAMIC_CURRENT_VOLTAGE,
    CLOUD_DYNAMIC_CURRENT_CURRENT,
    CLOUD_DYNAMIC_TEMPERATURE,
    CLOUD_DYNAMIC_TOTAL_ENERGY_TIME,
    CLOUD_DYNAMIC_TOTAL_ENERGY,
    CLOUD_DYNAMIC_FAULT_CODE,
    CLOUD_DYNAMIC_HARDWARE_FAULT_CODE,
    CLOUD_DYNAMIC_ALL_FIELDS  // Special values represent all fields
} Cloud_Dynamic_Field_E;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// Hardware fault bit definition
typedef struct
{
    uint8_t emergency_stop : 1;          // Bit1: The emergency stop button has malfunctioned
    uint8_t no_rectifier_module : 1;     // Bit2: No available rectifier module
    uint8_t outlet_temp_high : 1;        // Bit3: The temperature at the outlet is too high
    uint8_t ac_lightning_protection : 1; // Bit4: Ac lightning protection failure
    uint8_t dc20_comm_fault : 1;         // Bit5: The communication of the AC/DC module DC20 was interrupted
    uint8_t fc08_comm_fault : 1;         // Bit6: The insulation detection module FC08 has experienced communication interruption
    uint8_t meter_comm_fault : 1;        // Bit7: The communication of the electricity meter has been interrupted
    uint8_t card_reader_comm_fault : 1;  // Bit8: The communication of the card reader is interrupted
    uint8_t rc10_comm_fault : 1;         // Bit9: The communication of RC10 has been disrupted
    uint8_t fan_speed_control_fault : 1; // Bit10: The fan speed control board is faulty
    uint8_t dc_fuse_fault : 1;           // Bit11: Dc fuse failure
    uint8_t contactor_fault : 1;         // Bit12: High-voltage contactor failure
    uint8_t door_open : 1;               // Bit13: door open.
    uint8_t reserved : 3;                // Reserved
} Cloud_Ev_RealTimedData_Hardware_Fault_t;

typedef struct
{
    char manufacturer[CLOUD_EV_MANUFACTURER_LEN];                     // Manufacturer
    uint8_t serial_number[CLOUD_PROTOCOL_SN_LENGTH];                  // Serial Number
    uint8_t firmware_version[CLOUD_PROTOCOL_SOFTWARE_VERSION_LENGTH]; // Firmware Version
    char hardware_version[CLOUD_EV_HARDWARE_VERSION_LEN];             // Hardware Version
    uint8_t connector_type;                                           // Connector Type (e.g., Type 1, Type 2, GB_AC, GB_DC)
    uint8_t number_of_connectors;                                     // Number of Connectors
    uint16_t max_charging_voltage;                                    // Maximum Charging Voltage (V)
    uint16_t max_charging_current;                                    // Maximum Charging Current (A)
} Cloud_Ev_Charger_Constant_Info_T;

typedef struct
{
    uint16_t current_voltage[CLOUD_EV_MAX_CONNECTORS];                               // Current Power Output (V)
    uint16_t current_current[CLOUD_EV_MAX_CONNECTORS];                               // Current Current Output (A)
    uint16_t connector_temperature[CLOUD_EV_MAX_CONNECTORS];                         // Internal Temperature (°C)
    uint16_t total_energy_dispensed_time[CLOUD_EV_MAX_CONNECTORS];                   // Total Energy Dispensed Time (min)
    uint32_t total_energy_dispensed[CLOUD_EV_MAX_CONNECTORS];                        // Total Energy Dispensed (kWh)
    uint32_t fault_code;                                                             // Fault Code (if any)
    Cloud_Evse_StatusType_E evse_status[CLOUD_EV_MAX_CONNECTORS];                    // evse status
    Cloud_Ev_ConnectorGoBack_StatusType_E go_back_status[CLOUD_EV_MAX_CONNECTORS];   // Connector go back Status
    Cloud_Ev_Connector_StatusType_E connector_status[CLOUD_EV_MAX_CONNECTORS];       // Connector status
    Cloud_Ev_RealTimedData_Hardware_Fault_t hardware_fault[CLOUD_EV_MAX_CONNECTORS]; // Hardware Faults
} Cloud_Ev_Charger_Dynamic_Info_T;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
int Cloud_String_To_Packed_Bcd(const char *str, uint8_t *bcd, size_t bcd_size);
char *Cloud_Uint8_Array_To_Hex_String(const uint8_t *array, size_t len);
void Cloud_Ev_InfoInit(void);
bool Cloud_Ev_Set_Constant_Info(Cloud_Constant_Field_E field, const void *value);
bool Cloud_Ev_Set_Dynamic_Info(uint8_t connector_id, Cloud_Dynamic_Field_E field, const void *value);
bool Cloud_Ev_Get_Constant_Info(Cloud_Constant_Field_E field, void *value, size_t value_size);
bool Cloud_Ev_Get_Dynamic_Info(uint8_t connector_id, Cloud_Dynamic_Field_E field, void *value, size_t value_size);

#endif /* __CLOUD_EV_CHARGER_INFORMATION_H */
/* EOL */
