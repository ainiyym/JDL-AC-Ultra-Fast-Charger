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
#include "infra_state.h"
#include "infra_md5.h"
#include "infra_sha1.h"
#include "Cloud_Protocol_Cfg.h"
#include "cJSON.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define DEV_SIGN_SOURCE_MAXLEN              (200)
#define SIGN_FMT_LEN                        50                                      /* "clientId%sdeviceName%sproductKey%stimestamp%s";*/
#define MAX_TOPIC_LENGTH                    256                                     /* max topic length */  
#define CLOUDM_SG_PRODUCT_TIMESTAMP_VALUE   2524608000000                           /* use fixed timestamp */
#define CLOUDM_SG_DEVICE_NAME               "971762846196581146388515"              /* Device Name */ 
#define CLOUDM_SG_PRODUCT_KEY               "a1D0siVHgRU"                           /* Product Key */ 
#define CLOUDM_SG_DEVICE_SECRET             "a7a202f77a28bcb47c264dacb3a66ff7"      /* Device Secret */

#define CLOUDM_SG_DM_READ_ONLY              "{\"id\":\"%d\",\"version\":\"%s\",\"params\":%.*s,\"method\":\"%s\"}"
#define MAX_TOPIC_COUNT 6
#define MAX_TOTAL_TOPIC_LENGTH 256

#define CLOUD_PROTOCOL_SG_SYSN_NET_TIME_TOPIC_MAX_LENGTH            128
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
    MESSAGE_TYPE_UNKNOWN = 0,      // Unknown type
    MESSAGE_TYPE_TIME_SYNC,        // Time synchronization response
    MESSAGE_TYPE_SERVICE_CALL,     // Service call
    MESSAGE_TYPE_REPORT_RESP,      // report response
    MESSAGE_TYPE_PROPERTY_SETTING, // Property setting
    MESSAGE_TYPE_OTA_INFO_RESP,    // Firmware info response,
    MESSAGE_TYPE_MAX
} cloud_protocol_sg_message_type_e;

typedef enum
{
	V2G_CMD_EVENT_FIREWARE_INFO = 0,
	V2G_CMD_EVENT_VEHICLEINFO,
	V2G_CMD_EVENT_PILEABNORMAL,
	V2G_CMD_EVENT_VEHICLEABNORMAL,
	V2G_CMD_EVENT_EXECUTIONSTATUS_CHANGE,
	V2G_CMD_EVENT_VER_INFO,
	V2G_CMD_EVENT_LOGQUERY_RESULT,
	V2G_CMD_EVENT_PILE_WORKSTATUS,
	V2G_CMD_EVENT_ASK_CONFIG,
	V2G_CMD_EVENT_DCPILE_CHANGE,
} v2g_cmd_event_enum;

typedef enum
{
	V2G_CMD_PROPERTY_BATTERY_STATUS= 0,
	V2G_CMD_PROPERTY_VEHICLE_STATUS,
} v2g_cmd_property_enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern int Cloud_Protocol_Sg_Build_Topic(const char *topic_template, char *topic_buffer, size_t buffer_size);
#endif /* __CLOUD_PROTOCOL_SG_CFG_H */
/* EOL */
