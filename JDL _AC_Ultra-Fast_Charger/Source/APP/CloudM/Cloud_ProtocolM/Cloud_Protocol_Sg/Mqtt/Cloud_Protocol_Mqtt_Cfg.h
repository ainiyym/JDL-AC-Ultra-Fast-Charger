//******************************************************************************
//* File Name: Cloud_Protocol_Mqtt_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SGM_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SGM_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_MQTT_MAX_QUEUE_SIZE 				                    50		    // maximum queue size
#define CLOUD_PROTOCOL_MQTT_AT_RESPONSE_TIMEOUT_MS 		                    30 *1000 	// AT response timeout 30 seconds
#define CLOUD_PROTOCOL_MQTT_MSG_RESPONSE_TIMEOUT_MS                         30 *1000    // message response timeout 30 seconds
#define CLOUD_PROTOCOL_MQTT_PROCESS_INTERVAL_MS 		                    5000	    // processing interval 5 seconds
#define CLOUD_PROTOCOL_MQTT_RECONNECT_INTERVAL_MS 			                30000	    // reconnect interval 30 seconds

#define CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT                             1           // default retry attempts
#define CLOUD_PROTOCOL_MQTT_DEFAULT_PUBLISH_TOPIC			                "SG_Charger_001/pub/topic"	// default publish topic
#define CLOUD_PROTOCOL_MQTT_DEFAULT_SUBSCRIBE_TOPIC		                    "SG_Charger_001/sub/topic"	// default subscribe topic
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_REPORT_FW_VERSION = 0,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_REPORT_FW_INFO,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_REPORT_OTA_PROGRESS,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_GET_FW_INFO,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_SYNC_CLOUD_TIME,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_PROPERTY_POST,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_EVENT_POST,
    CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_CONFIG_MAXIMUM
} cloud_protocol_mqtt_active_topic_config_e; /* Proactively publish topic configuration */

typedef enum
{
    CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_DEVICE_PROPERTY = 0,
    CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_SERVICE_INVOCATION,
    CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_MAXIMUM
} cloud_protocol_mqtt_passive_topic_config_e; /* Subscribe platform configuration */

// MQTT QoS levels
typedef enum
{
    CLOUD_PROTOCOL_MQTT_QOS_0 = 0,
    CLOUD_PROTOCOL_MQTT_QOS_1,
    CLOUD_PROTOCOL_MQTT_QOS_2
} cloud_protocol_mqtt_qos_e;

// publish message type
typedef enum
{
    CLOUD_PROTOCOL_MQTT_NEED_ACK = 0, // Messages that require acknowledgment
    CLOUD_PROTOCOL_MQTT_MSG_NO_ACK    // Messages that do not require acknowledgment
} cloud_protocol_mqtt_msg_ack_type_e;

// Subscription status
typedef enum
{
    CLOUD_PROTOCOL_SUB_STATE_IDLE = 0,       // idle
    CLOUD_PROTOCOL_SUB_STATE_WAITING_ACK,    // waiting for acknowledgment
    CLOUD_PROTOCOL_SUB_STATE_ACTIVE,         // subscribed
    CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK // waiting for message acknowledgment
} cloud_protocol_mqtt_subscribe_state_e;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef void (*cloud_protocol_mqtt_connect_cb)(bool connected);
typedef bool (*cloud_protocol_mqtt_msg_cb)(const char *payload, cloud_protocol_sg_message_rcv_type_e *type);

// MQTT publishes parameters
typedef struct
{
    char *topic;          // Publish the theme
    char *payload;        // Publish payload (JSON format)
    uint16_t topic_len;   // topic length
    uint16_t payload_len; // payload length
} cloud_protocol_mqtt_publish_t;

// MQTT Subscribe parameters
typedef struct
{
    uint16_t topic_len; // topic length
    char *topic; // Subscribe to the topic
} cloud_protocol_mqtt_publish_topic_t; 

// MQTT Subscribe parameters
typedef struct
{
    uint16_t topic_len; // topic length
    char *topic; // Subscribe to the topic
} cloud_protocol_mqtt_subscribe_topic_t;

// topic configuration item
typedef struct
{
    char *name;               // Topic name
    bool is_enabled;          // Whether to use this topic
    char *subscribe_topic;    // Subscribe to the topic
    char *publish_topic;      // Publish the topic (for messages that require a response)
    cloud_protocol_mqtt_msg_ack_type_e ack_type;  // Message type
    uint8_t priority;          // Priority, the larger the number the higher the priority
    uint16_t ack_timeout_ms;   // Response timeout time
    uint8_t max_retry_count;   // Maximum retry count
} cloud_protocol_mqtt_topic_config_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/
extern const cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_passive_topic_configs[CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_MAXIMUM];
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern const cloud_protocol_mqtt_topic_config_t* Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(cloud_protocol_mqtt_active_topic_config_e Id);
extern const cloud_protocol_mqtt_topic_config_t* Cloud_Protocol_Mqtt_GetPassiveTopicConfigByEnum(cloud_protocol_mqtt_passive_topic_config_e Id);

#endif /* __CLOUD_PROTOCOL_SGM_CFG_H */
/* EOL */
