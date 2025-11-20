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
#include "Cloud_Protocol_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_MQTT_MAX_QUEUE_SIZE 				                    50		// maximum queue size
#define CLOUD_PROTOCOL_MQTT_AT_RESPONSE_TIMEOUT_MS 		                    10000 	// AT response timeout 10 seconds
#define CLOUD_PROTOCOL_MQTT_MSG_RESPONSE_TIMEOUT_MS                         5000    // message response timeout 5 seconds
#define CLOUD_PROTOCOL_MQTT_PROCESS_INTERVAL_MS 		                    5000	// processing interval 5 seconds
#define CLOUD_PROTOCOL_MQTT_RECONNECT_INTERVAL_MS 			                5000	// reconnect interval 5 seconds

#define CLOUD_PROTOCOL_MQTT_RETRY_COUNT 				                    3		// maximum retry attempts
#define CLOUD_PROTOCOL_MQTT_TOPIC_CONFIG_COUNT                              4       // number of topic configurations
#define CLOUD_PROTOCOL_MQTT_DEFAULT_PUBLISH_TOPIC			                "SG_Charger_001/pub/topic"	// default publish topic
#define CLOUD_PROTOCOL_MQTT_DEFAULT_SUBSCRIBE_TOPIC		                    "SG_Charger_001/sub/topic"	// default subscribe topic
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
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
// MQTT configuration parameters
typedef struct
{
    char *client_id;     // Client ID
    char *username;      // User name
    char *password;      // Password
    uint8_t qos;         // Service quality
    uint16_t keep_alive; // Survival time
    bool clean_session;  // Clear the session
    char *will_topic;    // Will theme
    char *will_message;  // Will Message
    uint8_t will_qos;    // Will QoS
    bool will_retain;    // Reservation by will
} cloud_protocol_mqtt_config_t;

// MQTT publishes parameters
typedef struct
{
    char *topic;   // Publish the theme
    char *payload; // Publish payload (JSON format)
    bool retain;   // Reserve mark
} cloud_protocol_mqtt_publish_t;

// MQTT Subscribe parameters
typedef struct
{
    char *topic; // Subscribe to the topic
} cloud_protocol_mqtt_publish_topic_t; 

// MQTT Subscribe parameters
typedef struct
{
    char *topic; // Subscribe to the topic
} cloud_protocol_mqtt_subscribe_topic_t;

// topic configuration item
typedef struct
{
    char *subscribe_topic;    // Subscribe to the topic
    char *publish_topic;      // Publish the topic (for messages that require a response)
    cloud_protocol_mqtt_msg_ack_type_e ack_type;  // Message type
    uint16_t ack_timeout_ms;  // Response timeout time
    uint8_t max_retry_count;  // Maximum retry count
} cloud_protocol_mqtt_topic_config_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/
extern cloud_protocol_mqtt_config_t cloud_sg_mqtt_default_config;
extern cloud_protocol_mqtt_topic_config_t cloud_protocol_mqtt_topic_configs[CLOUD_PROTOCOL_MQTT_TOPIC_CONFIG_COUNT];

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern char *Cloud_Protocol_Strdup(const char *s);
extern void Cloud_Protocol_SetMqttConfig(const cloud_protocol_mqtt_config_t *config);

#endif /* __CLOUD_PROTOCOL_SGM_CFG_H */
/* EOL */
