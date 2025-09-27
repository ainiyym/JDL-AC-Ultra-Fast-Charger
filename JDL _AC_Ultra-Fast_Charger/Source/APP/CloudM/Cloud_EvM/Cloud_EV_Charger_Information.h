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
    CLOUD_DYNAMIC_CURRENT_POWER,
    CLOUD_DYNAMIC_CURRENT_CURRENT,
    CLOUD_DYNAMIC_TEMPERATURE,
    CLOUD_DYNAMIC_TOTAL_ENERGY,
    CLOUD_DYNAMIC_FAULT_CODE,
    CLOUD_DYNAMIC_ALL_FIELDS  // Special values represent all fields
} Cloud_Dynamic_Field_E;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    char manufacturer[CLOUD_EV_MANUFACTURER_LEN];         // Manufacturer
    char serial_number[CLOUD_EV_SN_LEN];                  // Serial Number
    char firmware_version[CLOUD_EV_FIRMWARE_VERSION_LEN]; // Firmware Version
    char hardware_version[CLOUD_EV_HARDWARE_VERSION_LEN]; // Hardware Version
    uint8_t connector_type;                               // Connector Type (e.g., Type 1, Type 2, GB_AC, GB_DC)
    uint8_t number_of_connectors;                         // Number of Connectors
    uint16_t max_charging_voltage;                          // Maximum Charging Power (kW)
    uint16_t max_charging_current;                        // Maximum Charging Current (A)
} Cloud_Ev_Charger_Constant_Info_T;

typedef struct
{
    Cloud_Evse_StatusType_E evse_status;                  // evse status
    Cloud_Ev_ConnectorGoBack_StatusType_E go_back_status; // Connector go back Status
    Cloud_Ev_Connector_StatusType_E connector_status;     // Connector status
    uint16_t current_power;                               // Current Power Output (kW)
    uint16_t current_current;                             // Current Current Output (A)
    uint16_t temperature;                                 // Internal Temperature (°C)
    uint32_t total_energy_dispensed;                      // Total Energy Dispensed (kWh)
    uint32_t fault_code;                                  // Fault Code (if any)
} Cloud_Ev_Charger_Dynamic_Info_T;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
bool Cloud_Ev_Set_Constant_Info(Cloud_Constant_Field_E field, const void *value);
bool Cloud_Ev_Set_Dynamic_Info(Cloud_Dynamic_Field_E field, const void *value);
bool Cloud_Ev_Get_Constant_Info(Cloud_Constant_Field_E field, void *value, size_t value_size);
bool Cloud_Ev_Get_Dynamic_Info(Cloud_Dynamic_Field_E field, void *value, size_t value_size);

#endif /* __CLOUD_EV_CHARGER_INFORMATION_H */
/* EOL */
