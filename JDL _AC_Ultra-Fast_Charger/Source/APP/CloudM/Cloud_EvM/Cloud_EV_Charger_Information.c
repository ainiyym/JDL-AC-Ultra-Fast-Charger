//******************************************************************************
//* File Name: Cloud_EV_Charger_Information.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud EV Charger Information module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_EV_Charger_Information.h"

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
|    Global variables Declaration
|******************************************************************************/
static Cloud_Ev_Charger_Constant_Info_T Cloud_Ev_Charger_Constant_Info = {0};
static Cloud_Ev_Charger_Dynamic_Info_T Cloud_Ev_Charger_Dynamic_Info = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
bool Cloud_Ev_Set_Constant_Info(Cloud_Constant_Field_E field, const void *value)
{
    if (value == NULL && field != CLOUD_CONST_ALL_FIELDS)
    {
        return false;
    }

    switch (field)
    {
        case CLOUD_CONST_MANUFACTURER:
            if (strlen((const char *)value) >= CLOUD_EV_MANUFACTURER_LEN)
            {
                return false;
            }
            strncpy(Cloud_Ev_Charger_Constant_Info.manufacturer,
                    (const char *)value,
                    CLOUD_EV_MANUFACTURER_LEN - 1);
            Cloud_Ev_Charger_Constant_Info.manufacturer[CLOUD_EV_MANUFACTURER_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_SERIAL_NUMBER:
            if (strlen((const char *)value) >= CLOUD_EV_SN_LEN)
            {
                return false;
            }
            strncpy(Cloud_Ev_Charger_Constant_Info.serial_number,
                    (const char *)value,
                    CLOUD_EV_SN_LEN - 1);
            Cloud_Ev_Charger_Constant_Info.serial_number[CLOUD_EV_SN_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_FIRMWARE_VERSION:
            if (strlen((const char *)value) >= CLOUD_EV_FIRMWARE_VERSION_LEN)
            {
                return false;
            }
            strncpy(Cloud_Ev_Charger_Constant_Info.firmware_version,
                    (const char *)value,
                    CLOUD_EV_FIRMWARE_VERSION_LEN - 1);
            Cloud_Ev_Charger_Constant_Info.firmware_version[CLOUD_EV_FIRMWARE_VERSION_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_HARDWARE_VERSION:
            if (strlen((const char *)value) >= CLOUD_EV_HARDWARE_VERSION_LEN)
            {
                return false;
            }
            strncpy(Cloud_Ev_Charger_Constant_Info.hardware_version,
                    (const char *)value,
                    CLOUD_EV_HARDWARE_VERSION_LEN - 1);
            Cloud_Ev_Charger_Constant_Info.hardware_version[CLOUD_EV_HARDWARE_VERSION_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_CONNECTOR_TYPE:
            Cloud_Ev_Charger_Constant_Info.connector_type = *(const uint8_t *)value;
            break;

        case CLOUD_CONST_NUMBER_OF_CONNECTORS:
        {
            uint8_t count = *(const uint8_t *)value;
            if (count == 0 || count > 10)
                return false;
            Cloud_Ev_Charger_Constant_Info.number_of_connectors = count;
            break;
        }

        case CLOUD_CONST_MAX_CHARGING_VOLTAGE:
        {
            uint16_t voltage = *(const uint16_t *)value;
            if (voltage > CLOUD_EV_MAX_CHARGING_VOLTAGE)
                return false;
            Cloud_Ev_Charger_Constant_Info.max_charging_voltage = voltage;
            break;
        }

        case CLOUD_CONST_MAX_CHARGING_CURRENT:
        {
            uint16_t current = *(const uint16_t *)value;
            if (current > CLOUD_EV_MAX_CHARGING_CURRENT)
                return false;
            Cloud_Ev_Charger_Constant_Info.max_charging_current = current;
            break;
        }

        case CLOUD_CONST_ALL_FIELDS:
            if (value == NULL)
                return false;
            memcpy(&Cloud_Ev_Charger_Constant_Info, value, sizeof(Cloud_Ev_Charger_Constant_Info_T));
            break;

        default:
            return false;
    }

    return true;
}

bool Cloud_Ev_Set_Dynamic_Info(Cloud_Dynamic_Field_E field, const void *value)
{
    if (value == NULL && field != CLOUD_DYNAMIC_ALL_FIELDS)
    {
        return false;
    }

    switch (field)
    {
        case CLOUD_DYNAMIC_EVSE_STATUS:
        {
            Cloud_Evse_StatusType_E status = *(const Cloud_Evse_StatusType_E *)value;
            if (status > CLOUD_EVSE_STATUS_CHARGING)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.evse_status = status;
            break;
        }

        case CLOUD_DYNAMIC_GO_BACK_STATUS:
        {
            Cloud_Ev_ConnectorGoBack_StatusType_E status = *(const Cloud_Ev_ConnectorGoBack_StatusType_E *)value;
            if (status > CLOUD_EV_CHARGER_CONNECTOR_UNKNOWN)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.go_back_status = status;
            break;
        }

        case CLOUD_DYNAMIC_CONNECTOR_STATUS:
        {
            Cloud_Ev_Connector_StatusType_E status = *(const Cloud_Ev_Connector_StatusType_E *)value;
            if (status > CLOUD_EV_CHARGER_CONNECTOR_ERROR)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.connector_status = status;
            break;
        }

        case CLOUD_DYNAMIC_CURRENT_POWER:
        {
            uint16_t power = *(const uint16_t *)value;
            if (power > Cloud_Ev_Charger_Constant_Info.max_charging_voltage)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.current_power = power;
            break;
        }

        case CLOUD_DYNAMIC_CURRENT_CURRENT:
        {
            uint16_t current = *(const uint16_t *)value;
            if (current > Cloud_Ev_Charger_Constant_Info.max_charging_current)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.current_current = current;
            break;
        }

        case CLOUD_DYNAMIC_TEMPERATURE:
        {
            uint16_t temp = *(const uint16_t *)value;
            if (temp > CLOUD_EV_MAX_TEMPERATURE)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.temperature = temp;
            break;
        }

        case CLOUD_DYNAMIC_TOTAL_ENERGY:
            Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed = *(const uint32_t *)value;
            break;

        case CLOUD_DYNAMIC_FAULT_CODE:
            Cloud_Ev_Charger_Dynamic_Info.fault_code = *(const uint32_t *)value;
            break;

        case CLOUD_DYNAMIC_ALL_FIELDS:
            if (value == NULL)
                return false;
            memcpy(&Cloud_Ev_Charger_Dynamic_Info, value, sizeof(Cloud_Ev_Charger_Dynamic_Info_T));
            break;

        default:
            return false; // Unknown field
        }

    return true;
}

bool Cloud_Ev_Get_Constant_Info(Cloud_Constant_Field_E field, void *value, size_t value_size)
{
    if (value == NULL)
    {
        return false;
    }

    switch (field)
    {
        case CLOUD_CONST_MANUFACTURER:
            if (value_size < CLOUD_EV_MANUFACTURER_LEN)
                return false;
            strncpy((char *)value, Cloud_Ev_Charger_Constant_Info.manufacturer, CLOUD_EV_MANUFACTURER_LEN - 1);
            ((char *)value)[CLOUD_EV_MANUFACTURER_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_SERIAL_NUMBER:
            if (value_size < CLOUD_EV_SN_LEN)
                return false;
            strncpy((char *)value, Cloud_Ev_Charger_Constant_Info.serial_number, CLOUD_EV_SN_LEN - 1);
            ((char *)value)[CLOUD_EV_SN_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_FIRMWARE_VERSION:
            if (value_size < CLOUD_EV_FIRMWARE_VERSION_LEN)
                return false;
            strncpy((char *)value, Cloud_Ev_Charger_Constant_Info.firmware_version, CLOUD_EV_FIRMWARE_VERSION_LEN - 1);
            ((char *)value)[CLOUD_EV_FIRMWARE_VERSION_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_HARDWARE_VERSION:
            if (value_size < CLOUD_EV_HARDWARE_VERSION_LEN)
                return false;
            strncpy((char *)value, Cloud_Ev_Charger_Constant_Info.hardware_version, CLOUD_EV_HARDWARE_VERSION_LEN - 1);
            ((char *)value)[CLOUD_EV_HARDWARE_VERSION_LEN - 1] = '\0';
            break;

        case CLOUD_CONST_CONNECTOR_TYPE:
            if (value_size < sizeof(uint8_t))
                return false;
            *(uint8_t *)value = Cloud_Ev_Charger_Constant_Info.connector_type;
            break;

        case CLOUD_CONST_NUMBER_OF_CONNECTORS:
            if (value_size < sizeof(uint8_t))
                return false;
            *(uint8_t *)value = Cloud_Ev_Charger_Constant_Info.number_of_connectors;
            break;

        case CLOUD_CONST_MAX_CHARGING_VOLTAGE:
            if (value_size < sizeof(uint16_t))
                return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Constant_Info.max_charging_voltage;
            break;

        case CLOUD_CONST_MAX_CHARGING_CURRENT:
            if (value_size < sizeof(uint16_t))
                return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Constant_Info.max_charging_current;
            break;

        case CLOUD_CONST_ALL_FIELDS:
            if (value_size < sizeof(Cloud_Ev_Charger_Constant_Info_T))
                return false;
            memcpy(value, &Cloud_Ev_Charger_Constant_Info, sizeof(Cloud_Ev_Charger_Constant_Info_T));
            break;

    default:
        return false; // Unknown field
    }

    return true;
}

bool Cloud_Ev_Get_Dynamic_Info(Cloud_Dynamic_Field_E field, void *value, size_t value_size)
{
    if (value == NULL) {
        return false;
    }
    
    switch (field) {
        case CLOUD_DYNAMIC_EVSE_STATUS:
            if (value_size < sizeof(Cloud_Evse_StatusType_E)) return false;
            *(Cloud_Evse_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.evse_status;
            break;
            
        case CLOUD_DYNAMIC_GO_BACK_STATUS:
            if (value_size < sizeof(Cloud_Ev_ConnectorGoBack_StatusType_E)) return false;
            *(Cloud_Ev_ConnectorGoBack_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.go_back_status;
            break;
            
        case CLOUD_DYNAMIC_CONNECTOR_STATUS:
            if (value_size < sizeof(Cloud_Ev_Connector_StatusType_E)) return false;
            *(Cloud_Ev_Connector_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.connector_status;
            break;
            
        case CLOUD_DYNAMIC_CURRENT_POWER:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.current_power;
            break;
            
        case CLOUD_DYNAMIC_CURRENT_CURRENT:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.current_current;
            break;
            
        case CLOUD_DYNAMIC_TEMPERATURE:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.temperature;
            break;
            
        case CLOUD_DYNAMIC_TOTAL_ENERGY:
            if (value_size < sizeof(uint32_t)) return false;
            *(uint32_t *)value = Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed;
            break;
            
        case CLOUD_DYNAMIC_FAULT_CODE:
            if (value_size < sizeof(uint32_t)) return false;
            *(uint32_t *)value = Cloud_Ev_Charger_Dynamic_Info.fault_code;
            break;
            
        case CLOUD_DYNAMIC_ALL_FIELDS:
            if (value_size < sizeof(Cloud_Ev_Charger_Dynamic_Info_T)) return false;
            memcpy(value, &Cloud_Ev_Charger_Dynamic_Info, sizeof(Cloud_Ev_Charger_Dynamic_Info_T));
            break;
            
        default:
            return false;  // Unknown field
    }
    
    return true;
}
/* EOL */
