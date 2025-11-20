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

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
cloud_protocol_mqtt_config_t cloud_sg_mqtt_default_config = 
{
	.client_id = CLOUD_PROTOCOL_SG_MQTT_CLIENT_ID,
	.username = CLOUD_PROTOCOL_SG_MQTT_CLIENT_USERNAME,
	.password = CLOUD_PROTOCOL_SG_MQTT_CLIENT_PASSWORD,
	.qos = CLOUD_PROTOCOL_MQTT_QOS_1,
	.keep_alive = 60,
	.clean_session = true,
	.will_qos = CLOUD_PROTOCOL_MQTT_QOS_0,
	.will_retain = false
};

// sg mqtt topic configuration table
cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_topic_configs[CLOUD_PROTOCOL_MQTT_TOPIC_CONFIG_COUNT] = {
    {
        .publish_topic = "/sys/device/service/get",
        .subscribe_topic = "/sys/device/service/get_reply",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .ack_timeout_ms = 5000,
        .max_retry_count = 3
    },
    {
        .publish_topic = "/sys/device/property/post", 
        .subscribe_topic = "/sys/device/property/post_reply",
        .ack_type = CLOUD_PROTOCOL_MQTT_NEED_ACK,
        .ack_timeout_ms = 3000,
        .max_retry_count = 2
    },
    {
        .publish_topic = "/sys/device/event/post",
        .subscribe_topic = "/sys/device/event/post_reply", 
        .ack_type = CLOUD_PROTOCOL_MQTT_MSG_NO_ACK,
        .ack_timeout_ms = 0,
        .max_retry_count = 0
    },
    {
        .publish_topic = "",
        .subscribe_topic = "/sys/device/service/set",
        .ack_type = CLOUD_PROTOCOL_MQTT_MSG_NO_ACK,
        .ack_timeout_ms = 0,
        .max_retry_count = 0
    }
};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
char *Cloud_Protocol_Strdup(const char *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    size_t len = strlen(s) + 1;
    char *new_str = (char *)CLOUDM_MALLOC(len);
    if (new_str != NULL)
    {
        memcpy(new_str, s, len);
    }
    return new_str;
}

void Cloud_Protocol_SetMqttConfig(const cloud_protocol_mqtt_config_t *config)
{
    if (config == NULL)
    {
        return;
    }
    // Copy the provided configuration to the default configuration
    memcpy(&cloud_sg_mqtt_default_config, config, sizeof(cloud_protocol_mqtt_config_t));
}

/* EOL */
