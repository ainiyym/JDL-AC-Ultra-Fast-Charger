//******************************************************************************
//* File Name: Cloud_Protocol_Sg_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "infra_state.h"
#include "infra_md5.h"
#include "infra_sha1.h"
#include "Cloud_Protocol_Cfg.h"
#include "cJSON.h"
#include "STD_SysM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define DEV_SIGN_SOURCE_MAXLEN              (200)
#define SIGN_FMT_LEN                        50                                      /* "clientId%sdeviceName%sproductKey%stimestamp%s";*/
#define MAX_TOPIC_LENGTH                    256                                     /* max topic length */
#define CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH 236                                     /* max sub topic length */
#define CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH 128                                     /* max pub topic length */
#define CLOUDM_SG_PRODUCT_TIMESTAMP_VALUE   2524608000000                           /* use fixed timestamp */
#define CLOUDM_SG_DEVICE_NAME               "971762846196581146388515"              /* Device Name */ 
#define CLOUDM_SG_PRODUCT_KEY               "a1D0siVHgRU"                           /* Product Key */ 
#define CLOUDM_SG_DEVICE_SECRET             "a7a202f77a28bcb47c264dacb3a66ff7"      /* Device Secret */
#define CLOUDM_SG_STAKE_MODEL			 	"JDL-AC600"                             /* Charger Model */
#define CLOUDM_SG_MANUFACTURER_CODE        	1420                                  	/* Manufacturer Code */
#define CLOUDM_SG_DEVICE_TYPE               "02"                                    /* Pile Type: 1-DC charger, 2-AC charger, 3-AC&DC integrated charger, 4-gateway */
#define CLOUDM_SG_PRODUCTION_DATE           "251223"                                /* Production Date: YYMMDD */
#define CLOUDM_SG_PILE_TYPE					11										/* 10: Single-phase AC, 11: Three-phase AC, 12: DC, 13: AC/DC integrated machine, 99: Others */
#define CLOUDM_SG_CONNECTOR_NUM				SYS_NUMBER_OF_CONNECTORS				/* connector num */
#define CLOUDM_SG_GRIDE_TYPE				10										/* not avaliable of longitude and latitude coordinates */
#define CLOUDM_SG_METERING_METHOD			11										/* 11: AC electric energy meter */
#define CLOUDM_SG_RATED_POWER				2962									/* kW, Precision:0.1 */
#define CLOUDM_SG_OT_MIN_VOL				1000									/* V, Precision:0.1 */
#define CLOUDM_SG_OT_MAX_VOL				4577									/* V, Precision:0.1 */
#define CLOUDM_SG_OT_MAX_CUR				4950									/* A, Precision:0.1 */
#define CLOUDM_SG_OT_MIN_CUR				60										/* A, Precision:0.1 */
#define CLOUDM_SG_OT_MIN_POWER				6000									/* W, Precision:1 */

#define CLOUDM_SG_DEVICE_REG_METHOD		    12										/* 12: Device registration by usart */
#define CLOUDM_SG_PILE_HW_VERSION           "V1.0.0"                                /* Hardware version */
#define CLOUDM_SG_PILE_FW_SOFT_VERSION      "V1.0.0"                                /* Software version */
#define CLOUDM_SG_SDK_VERSION               "V1.0.1"                                /* SDK version */

#define CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_PREFIX "thing.event."
#define CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_SUFFIX ".post"
#define CLOUD_PROTOCOL_SG_VERSION_DEFAULT "1.0"

#define CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX         CLOUDM_SG_CONNECTOR_NUM       /* connector num */
#define CLOUD_PROTOCOL_SG_INVALID_GUN_INDEX            0xFF                          /* invalid gun index */

#define CLOUD_PROTOCOL_SG_SYSN_NET_TIME_DEFAULT_RETRY_COUNT         3                           // Default retry attempts
#define CLOUD_PROTOCOL_SG_SYSN_NET_TIME_PERIOD_MS                   24 * 60 * 60 * 1000         // Default sync interval (24 hour)
#define CLOUD_PROTOCOL_SG_SYSN_NET_TIME_RETRY_BACKOFF_TIME_MS       30 * 1000                   // Default retry backoff time (30 seconds)
#define CLOUD_PROTOCOL_SG_SYSN_NET_TIME_MAX_CLOCK_ADJUSTMENT_MS     5000                        //Max 5 seconds adjustment
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_PROTOCOL_MQTT_SIGN_HMACMD5,
    CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1
} Cloud_Protocol_Mqtt_SignMethod;

// SG protocol:mqtt message type enumeration
typedef enum
{
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_UNKNOWN = 0,      // Unknown type
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_TIME_SYNC,        // Time synchronization response
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_SERVICE_CALL,     // Service call
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_REPORT_RESP,      // report response
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_PROPERTY_SETTING, // Property setting
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_OTA_INFO_RESP,    // Firmware info response,
    CLOUD_PROTOCOL_SG_MESSAGE_TYPE_MAX
} cloud_protocol_sg_message_type_e;

typedef enum
{
	CLOUD_PROTOCOL_VALUE_TYPE_STRING = 0,	// string
	CLOUD_PROTOCOL_VALUE_TYPE_INT32,		// int32
	CLOUD_PROTOCOL_VALUE_TYPE_FLOAT,		// float
	CLOUD_PROTOCOL_VALUE_TYPE_BOOL,			// bool
	CLOUD_PROTOCOL_VALUE_TYPE_STRING_ARRAY, // string array
	CLOUD_PROTOCOL_VALUE_TYPE_INT32_ARRAY,	// int array
	CLOUD_PROTOCOL_VALUE_TYPE_DOUBLE,		// double
	CLOUD_PROTOCOL_VALUE_TYPE_NULL			// NULL
} cloud_protocol_value_type_e;

// charging and discharging directions
typedef enum
{
    CLOUD_PROTOCOL_SG_CHARGE_DIRECTION_UNKNOWN = 0,  // unknown
    CLOUD_PROTOCOL_SG_CHARGE_DIRECTION_CHARGE = 1,   // charge
    CLOUD_PROTOCOL_SG_CHARGE_DIRECTION_DISCHARGE = 2 // discharge
} cloud_protocol_sg_charge_direction_t;

// Error code definition
typedef enum
{
    CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS = 200,
    CLOUD_PROTOCOL_SG_RESPONSE_REQUEST_ERROR = 400,
    CLOUD_PROTOCOL_SG_RESPONSE_PARAMETER_ERROR = 460,
    CLOUD_PROTOCOL_SG_RESPONSE_TOO_MANY_REQUESTS = 429,
    CLOUD_PROTOCOL_SG_RESPONSE_CUSTOM_ERROR_BASE = 100000
} cloud_protocol_sg_response_code_t;

typedef enum
{
	CLOUD_PROTOCOL_SG_EVENT_FIREWARE_INFO = 0,
	CLOUD_PROTOCOL_SG_EVENT_VEHICLEINFO,
	CLOUD_PROTOCOL_SG_EVENT_PILEABNORMAL,
	CLOUD_PROTOCOL_SG_EVENT_VEHICLEABNORMAL,
	CLOUD_PROTOCOL_SG_EVENT_EXECUTIONSTATUS_CHANGE,
	CLOUD_PROTOCOL_SG_EVENT_VER_INFO,
	CLOUD_PROTOCOL_SG_EVENT_LOGQUERY_RESULT,
	CLOUD_PROTOCOL_SG_EVENT_PILE_WORKSTATUS,
	CLOUD_PROTOCOL_SG_EVENT_ASK_CONFIG,
	CLOUD_PROTOCOL_SG_EVENT_DCPILE_CHANGE,
} cloud_protocol_sg_event_enum;

// Firmware information field enumeration
typedef enum
{
    // String type fields
    CLOUD_PROTOCOL_SG_EVENT_FW_SIM_NO = 0,  // SIM card number
    CLOUD_PROTOCOL_SG_EVENT_FW_MODEL_NO,    // Metering/billing model number
    CLOUD_PROTOCOL_SG_EVENT_FW_STAKE_MODEL, // Charger model
    CLOUD_PROTOCOL_SG_EVENT_FW_DE_SN,       // Factory serial number
    CLOUD_PROTOCOL_SG_EVENT_FW_SIM_MAC,     // Network MAC address
    CLOUD_PROTOCOL_SG_EVENT_FW_BT_MAC,      // Bluetooth MAC address
    // Unsigned integer type fields
    CLOUD_PROTOCOL_SG_EVENT_FW_VENDOR_CODE,           // Manufacturer code
    CLOUD_PROTOCOL_SG_EVENT_FW_LONGITUDE,             // Longitude
    CLOUD_PROTOCOL_SG_EVENT_FW_LATITUDE,              // Latitude
    CLOUD_PROTOCOL_SG_EVENT_FW_HEIGHT,                // Altitude
    CLOUD_PROTOCOL_SG_EVENT_FW_GRID_TYPE,             // Coordinate type
    CLOUD_PROTOCOL_SG_EVENT_FW_OT_RATE,               // Rated power
    CLOUD_PROTOCOL_SG_EVENT_FW_OT_MIN_VOL,            // Minimum output voltage
    CLOUD_PROTOCOL_SG_EVENT_FW_OT_MAX_VOL,            // Maximum output voltage
    CLOUD_PROTOCOL_SG_EVENT_FW_OT_CUR,                // Maximum output current
    CLOUD_PROTOCOL_SG_EVENT_FW_CT,                    // Current transformer coefficient
    CLOUD_PROTOCOL_SG_EVENT_FW_MIN_CHARGING_CURRENT,  // Minimum charging current
    CLOUD_PROTOCOL_SG_EVENT_FW_MIN_CHARGING_POWER,    // Minimum charging power
    CLOUD_PROTOCOL_SG_EVENT_FW_MAX_DISCHARGE_VOLTAGE, // Maximum discharge voltage
    CLOUD_PROTOCOL_SG_EVENT_FW_MIN_DISCHARGE_VOLTAGE, // Minimum discharge voltage
    CLOUD_PROTOCOL_SG_EVENT_FW_MAX_DISCHARGE_CURRENT, // Maximum discharge current
    CLOUD_PROTOCOL_SG_EVENT_FW_MIN_DISCHARGE_CURRENT, // Minimum discharge current
    // Unsigned char type fields
    CLOUD_PROTOCOL_SG_EVENT_FW_DE_TYPE,        // Pile type
    CLOUD_PROTOCOL_SG_EVENT_FW_CONNET_NUM,     // Number of charging interfaces
    CLOUD_PROTOCOL_SG_EVENT_FW_MEA_TYPE,       // Metering method
    CLOUD_PROTOCOL_SG_EVENT_FW_IS_GATE_LOCK,   // Has smart gate lock
    CLOUD_PROTOCOL_SG_EVENT_FW_IS_GROUND_LOCK, // Has ground lock
    // Array type fields (meter addresses)
    CLOUD_PROTOCOL_SG_EVENT_FW_IN_METER_ARRAY,  // Array of AC input meter addresses
    CLOUD_PROTOCOL_SG_EVENT_FW_OUT_METER_ARRAY, // Array of billing meter addresses
    CLOUD_PROTOCOL_SG_EVENT_FW_TYPE_COUNT       // Number of enum values, used for bounds checking
} cloud_protocol_sg_event_fireware_enum;

typedef enum
{
	CLOUD_PROTOCOL_SG_PROPERTY_BATTERY_STATUS= 0,
	CLOUD_PROTOCOL_SG_PROPERTY_VEHICLE_STATUS,
} cloud_protocol_sg_property_enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern int Cloud_Protocol_Sg_Build_Topic(const char *topic_template,  const char* identifier, char *topic_buffer, size_t buffer_size);
extern char* Cloud_Protocol_GenerateMessageId(void);
extern uint32_t Cloud_Protocol_GetMessageId(void);
extern bool cloud_protocol_report_response(const char *payload, uint16_t payload_len);
extern bool cloud_protocol_service_call_response(const char *msg_id, const char *identifier, int response_code, const cJSON *data);
#endif /* __CLOUD_PROTOCOL_SG_CFG_H */
/* EOL */
