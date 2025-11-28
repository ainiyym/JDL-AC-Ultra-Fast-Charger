//******************************************************************************
//* File Name: Cloud_Protocol_Mqtt_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Mqtt_Cfg.h"

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
const static cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_active_topic_configs[CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_CONFIG_MAXIMUM] ;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
// sg mqtt active topic configuration table(Publish driver subscription)
const static cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_active_topic_configs[CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_CONFIG_MAXIMUM] = {
    {
        .name = "Report FW Version",
        .is_enabled = false,
        .publish_topic = "/ota/device/inform/{productKey}/{deviceName}",
        .subscribe_topic = NULL,
        .ack_type = CLOUD_PROTOCOL_MQTT_MSG_NO_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Report FW Info",
        .is_enabled = false,
        .publish_topic = "/ota/device/upgrade/{productKey}/{deviceName}", 
        .subscribe_topic = NULL,
        .ack_type = CLOUD_PROTOCOL_MQTT_MSG_NO_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Report OTA Progress",
        .is_enabled = false,
        .publish_topic = "/ota/device/progress/{productKey}/{deviceName}",
        .subscribe_topic = NULL, 
        .ack_type = CLOUD_PROTOCOL_MQTT_MSG_NO_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Get FW Info",
        .is_enabled = false,
        .publish_topic = "/sys/{productKey}/{deviceName}/thing/ota/firmware/get",
        .subscribe_topic = "/sys/device/service/set",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "NTP Request",
        .is_enabled = true,
        .publish_topic = "/ext/ntp/{productKey}/{deviceName}/request",
        .subscribe_topic = "/ext/ntp/{productKey}/{deviceName}/response",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Property Report",
        .is_enabled = true,
        .publish_topic = "/sys/{productKey}/{deviceName}/thing/event/property/post",
        .subscribe_topic = "/sys/{productKey}/{deviceName}/thing/event/property/post_reply",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Event Report",
        .is_enabled = true,
        .publish_topic = "/sys/{productKey}/{deviceName}/thing/model/up_raw",
        .subscribe_topic = "/sys/{productKey}/{deviceName}/thing/model/up_raw_reply",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    }
};

// sg mqtt passive topic configuration table(Subscribe platform publish)
const cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_passive_topic_configs[CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_MAXIMUM] = {
    {
        .name = "Property Set",
        .is_enabled = false,
        .publish_topic = "/sys/{productKey}/{deviceName}/thing/service/property/set_reply",
        .subscribe_topic = "/sys/{productKey}/{deviceName}/thing/service/property/set",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    },
    {
        .name = "Service Invoke",
        .is_enabled = true,
        .publish_topic = "/sys/{productKey}/{deviceName}/thing/service/{identifier}_reply",
        .subscribe_topic = "/sys/{productKey}/{deviceName}/thing/service/{identifier}",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .priority = 0,
        .ack_timeout_ms = 0,
        .max_retry_count = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT
    }
};
/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
const cloud_protocol_mqtt_topic_config_t* Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(cloud_protocol_mqtt_active_topic_config_e Id)
{
    if (Id >= CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_CONFIG_MAXIMUM)
    {
        return NULL;
    }
    return &cloud_protocol_mqtt_active_topic_configs[Id];
}
/* EOL */
