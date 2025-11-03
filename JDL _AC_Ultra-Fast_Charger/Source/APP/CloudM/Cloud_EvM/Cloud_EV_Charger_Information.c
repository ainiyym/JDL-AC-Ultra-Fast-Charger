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

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static Cloud_Ev_Charger_Constant_Info_T Cloud_Ev_Charger_Constant_Info;
static Cloud_Ev_Charger_Dynamic_Info_T Cloud_Ev_Charger_Dynamic_Info;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Ev_InfoInit(void)
{
    memset(&Cloud_Ev_Charger_Constant_Info, 0, sizeof(Cloud_Ev_Charger_Constant_Info_T));
    memset(&Cloud_Ev_Charger_Dynamic_Info, 0, sizeof(Cloud_Ev_Charger_Dynamic_Info_T));
}

/**
 * String to compressed BCD code
 * @param str Enter a numeric string
 * @param bcd Output the BCD buffer
 * @param bcd_size Buffer size
 * @return The number of BCD bytes successfully converted, and -1 returned if failed
 */
int Cloud_String_To_Packed_Bcd(const char *str, uint8_t *bcd, size_t bcd_size)
{
	if (str == NULL || bcd == NULL || bcd_size == 0)
	{
		return -1;
	}

	size_t len = strlen(str);
	if (len == 0)
	{
		return 0;
	}

	// Calculate the required number of BCD bytes (storing 2 digits per byte)
	size_t bcd_bytes_needed = (len + 1) / 2;
	if (bcd_bytes_needed > bcd_size)
	{
		return -1; // Buffer is insufficient
	}

	memset(bcd, 0, bcd_size);

	int bcd_index = 0;
	int shift_high = 1; // Start from a high position

	// Big-endian pattern: Processing from the beginning of the string backward (with the high bits first)
	for (size_t i = 0; i < len; i++)
	{
		uint8_t digit = str[i] - '0';

		if (shift_high)
		{
			bcd[bcd_index] = digit << 4; // The number is placed in the top four digits
			shift_high = 0;
		}
		else
		{
			bcd[bcd_index] |= digit; // The number is placed in the bottom four digits
			bcd_index++;
			shift_high = 1;
		}
	}

	return bcd_bytes_needed;
}

char *Cloud_Uint8_Array_To_Hex_String(const uint8_t *array, size_t len)
{
	if (array == NULL || len == 0)
	{
		return NULL;
	}

	// Each byte requires two characters to represent it, plus a string terminator
	char *result = (char *)pvPortMalloc(len * 2 + 1);
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t i = 0; i < len; i++)
	{
		// Convert the high 4 bits and low 4 bits of each byte to hexadecimal characters respectively
		sprintf(result + i * 2, "%02x", array[i]);
	}

	result[len * 2] = '\0'; // Add a string terminator

	CLOUD_INFO("array:");
	CLOUD_PRINT_HEX(array, len);
	return result;
}

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
            {
                if (strlen((const char *)value) >= CLOUD_EV_SN_LEN)
                {
                    return false;
                }
                char SN[CLOUD_EV_SN_LEN] = {0};
                int Bcdlength = 0;

                strncpy(SN, (const char *)value, CLOUD_EV_SN_LEN - 1);
                SN[CLOUD_EV_SN_LEN - 1] = '\0';
                Bcdlength = Cloud_String_To_Packed_Bcd(SN, &Cloud_Ev_Charger_Constant_Info.serial_number[0], CLOUD_PROTOCOL_SN_LENGTH);
                if (Bcdlength != CLOUD_PROTOCOL_SN_LENGTH)
                {
                    // Error handling
                    CLOUD_ERROR("%s: Invalid SN format\r\n", __func__);
                    return false;
                }
            }
            break;

        case CLOUD_CONST_FIRMWARE_VERSION:
            if (sizeof((const char *)value) >= CLOUD_EV_FIRMWARE_VERSION_LEN)
            {
                return false;
            }
            memcpy(Cloud_Ev_Charger_Constant_Info.firmware_version,
                   (const char *)value,
                   CLOUD_EV_FIRMWARE_VERSION_LEN);
            break;

        case CLOUD_CONST_HARDWARE_VERSION:
            if (sizeof((const char *)value) >= CLOUD_EV_HARDWARE_VERSION_LEN)
            {
                return false;
            }
            memcpy(Cloud_Ev_Charger_Constant_Info.hardware_version,
                   (const char *)value,
                   CLOUD_EV_HARDWARE_VERSION_LEN);
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

bool Cloud_Ev_Set_Dynamic_Info(uint8_t connector_id , Cloud_Dynamic_Field_E field, const void *value)
{
    if (((value == NULL && field != CLOUD_DYNAMIC_ALL_FIELDS) || connector_id >= CLOUD_EV_MAX_CONNECTORS))
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
            Cloud_Ev_Charger_Dynamic_Info.evse_status[connector_id] = status;
            break;
        }

        case CLOUD_DYNAMIC_GO_BACK_STATUS:
        {
            Cloud_Ev_ConnectorGoBack_StatusType_E status = *(const Cloud_Ev_ConnectorGoBack_StatusType_E *)value;
            if (status > CLOUD_EV_CHARGER_CONNECTOR_UNKNOWN)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.go_back_status[connector_id] = status;
            break;
        }

        case CLOUD_DYNAMIC_CONNECTOR_STATUS:
        {
            Cloud_Ev_Connector_StatusType_E status = *(const Cloud_Ev_Connector_StatusType_E *)value;
            if (status > CLOUD_EV_CHARGER_CONNECTOR_ERROR)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.connector_status[connector_id] = status;
            break;
        }

        case CLOUD_DYNAMIC_CURRENT_VOLTAGE:
        {
            uint16_t power = *(const uint16_t *)value;
            if (power > Cloud_Ev_Charger_Constant_Info.max_charging_voltage)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.current_voltage[connector_id] = power;
            break;
        }

        case CLOUD_DYNAMIC_CURRENT_CURRENT:
        {
            uint16_t current = *(const uint16_t *)value;
            if (current > Cloud_Ev_Charger_Constant_Info.max_charging_current)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.current_current[connector_id] = current;
            break;
        }

        case CLOUD_DYNAMIC_TEMPERATURE:
        {
            uint16_t temp = *(const uint16_t *)value;
            if (temp > CLOUD_EV_MAX_TEMPERATURE)
                return false;
            Cloud_Ev_Charger_Dynamic_Info.connector_temperature[connector_id] = temp;
            break;
        }

        case CLOUD_DYNAMIC_TOTAL_ENERGY:
            Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed[connector_id] = *(const uint32_t *)value;
            break;

        case CLOUD_DYNAMIC_TOTAL_ENERGY_TIME:
            Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed_time[connector_id] = *(const uint16_t *)value;
            break;

        case CLOUD_DYNAMIC_FAULT_CODE:
            Cloud_Ev_Charger_Dynamic_Info.fault_code = *(const uint32_t *)value;
            break;
        case CLOUD_DYNAMIC_HARDWARE_FAULT_CODE:
            Cloud_Ev_Charger_Dynamic_Info.hardware_fault[connector_id] = *(const Cloud_Ev_RealTimedData_Hardware_Fault_t *)value;
            break;

        case CLOUD_DYNAMIC_ALL_FIELDS:
            if (value == NULL)
                return false;
            memcpy(&Cloud_Ev_Charger_Dynamic_Info, value, sizeof(Cloud_Ev_Charger_Dynamic_Info_T));
            break;
        case CLOUD_DYNAMIC_AUTH_STATUS:
            Cloud_Ev_Charger_Dynamic_Info.auth_status[connector_id] = *(const Cloud_Ev_Auth_StatusType_E *)value;
            break;
        case CLOUD_DYNAMIC_CHARGING_STATUS:
            Cloud_Ev_Charger_Dynamic_Info.charging_status[connector_id] = *(const Cloud_Ev_Charging_StatusType_E *)value;
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
            if (value_size < CLOUD_PROTOCOL_SN_LENGTH)
                return false;
            memcpy((char *)value, Cloud_Ev_Charger_Constant_Info.serial_number, CLOUD_PROTOCOL_SN_LENGTH);
            break;

        case CLOUD_CONST_FIRMWARE_VERSION:
            if (value_size < CLOUD_EV_FIRMWARE_VERSION_LEN)
                return false;
            memcpy((char *)value, Cloud_Ev_Charger_Constant_Info.firmware_version, CLOUD_EV_FIRMWARE_VERSION_LEN);
            break;

        case CLOUD_CONST_HARDWARE_VERSION:
            if (value_size < CLOUD_EV_HARDWARE_VERSION_LEN)
                return false;
            memcpy((char *)value, Cloud_Ev_Charger_Constant_Info.hardware_version, CLOUD_EV_HARDWARE_VERSION_LEN);
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

bool Cloud_Ev_Get_Dynamic_Info(uint8_t connector_id, Cloud_Dynamic_Field_E field, void *value, size_t value_size)
{
    if (value == NULL || connector_id >= CLOUD_EV_MAX_CONNECTORS) {
        return false;
    }
    
    switch (field) {
        case CLOUD_DYNAMIC_EVSE_STATUS:
            if (value_size < sizeof(Cloud_Evse_StatusType_E)) return false;
            *(Cloud_Evse_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.evse_status[connector_id];
            break;
            
        case CLOUD_DYNAMIC_GO_BACK_STATUS:
            if (value_size < sizeof(Cloud_Ev_ConnectorGoBack_StatusType_E)) return false;
            *(Cloud_Ev_ConnectorGoBack_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.go_back_status[connector_id];
            break;
            
        case CLOUD_DYNAMIC_CONNECTOR_STATUS:
            if (value_size < sizeof(Cloud_Ev_Connector_StatusType_E)) return false;
            *(Cloud_Ev_Connector_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.connector_status[connector_id];
            break;

        case CLOUD_DYNAMIC_CURRENT_VOLTAGE:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.current_voltage[connector_id];
            break;
            
        case CLOUD_DYNAMIC_CURRENT_CURRENT:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.current_current[connector_id];
            break;
            
        case CLOUD_DYNAMIC_TEMPERATURE:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.connector_temperature[connector_id];
            break;
            
        case CLOUD_DYNAMIC_TOTAL_ENERGY:
            if (value_size < sizeof(uint32_t)) return false;
            *(uint32_t *)value = Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed[connector_id];
            break;
            
        case CLOUD_DYNAMIC_TOTAL_ENERGY_TIME:
            if (value_size < sizeof(uint16_t)) return false;
            *(uint16_t *)value = Cloud_Ev_Charger_Dynamic_Info.total_energy_dispensed_time[connector_id];
            break;

        case CLOUD_DYNAMIC_FAULT_CODE:
            if (value_size < sizeof(uint32_t)) return false;
            *(uint32_t *)value = Cloud_Ev_Charger_Dynamic_Info.fault_code;
            break;
            
        case CLOUD_DYNAMIC_HARDWARE_FAULT_CODE:
            if (value_size < sizeof(Cloud_Ev_RealTimedData_Hardware_Fault_t)) return false;
            *(Cloud_Ev_RealTimedData_Hardware_Fault_t *)value = Cloud_Ev_Charger_Dynamic_Info.hardware_fault[connector_id];
            break;

        case CLOUD_DYNAMIC_AUTH_STATUS:
            if (value_size < sizeof(Cloud_Ev_Auth_StatusType_E)) return false;
            *(Cloud_Ev_Auth_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.auth_status[connector_id];
            break;

        case CLOUD_DYNAMIC_CHARGING_STATUS:
            if (value_size < sizeof(Cloud_Ev_Charging_StatusType_E)) return false;
            *(Cloud_Ev_Charging_StatusType_E *)value = Cloud_Ev_Charger_Dynamic_Info.charging_status[connector_id];
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
