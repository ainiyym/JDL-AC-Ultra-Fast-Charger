//******************************************************************************
//* File Name: Cloud_Protocol_Mqtt.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: SG protocol mqtt module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_MQTT_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_MQTT_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Mqtt_Cfg.h"
#include "Cloud_Protocol_Msg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
// MQTT connection status
typedef enum
{
    CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED = 0,
    CLOUD_PROTOCOL_MQTT_STATE_CONNECTING,
    CLOUD_PROTOCOL_MQTT_STATE_CONNECTED,
    CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTING
} cloud_protocol_mqtt_state_e;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// message of publish item
typedef struct cloud_protocol_mqtt_message_item
{
    cloud_protocol_mqtt_publish_t publish;         // publish message
    cloud_protocol_mqtt_msg_ack_type_e ack_type;   // ack type
    char *ack_topic;                               // ack topic (only for messages that require acknowledgment)
    uint32_t timestamp;                            // timestamp
    uint8_t retry_count;                           // retry count
    struct cloud_protocol_mqtt_message_item *next; // next cloud_protocol_mqtt_message_item
} cloud_protocol_mqtt_message_item_t;

// topic state manager
typedef struct
{
    cloud_protocol_mqtt_topic_config_t config;      // topic config
    bool subscribed;                                // subscribed or not
    bool waiting_ack;                               // Waiting for subscription response
    bool publishing;                                // A message is being released.
    uint32_t last_subscribe_time;                   // Last subscription time
    cloud_protocol_mqtt_message_item_t *active_msg; // Current Active message
} cloud_protocol_mqtt_topic_state_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
bool Cloud_Protocol_Mqtt_ClientManagerInit(cloud_protocol_mqtt_topic_config_t *cloud_protocol_mqtt_topic_configs, uint16_t topic_count,
                            iotx_sign_mqtt_t *mqtt_client,
                            void (*connect_cb)(bool connected),
                            void (*msg_cb)(const char *payload));
void Cloud_Protocol_Mqtt_GetClientConfig(iotx_sign_mqtt_t *mqtt_client);
void Cloud_Protocol_Mqtt_SetDeviceIPConnectionStatus(bool connected);
void Cloud_Protocol_Mqtt_HandleConnected(void);
bool Cloud_Protocol_Mqtt_AddPublishMessage(const char *topic, const char *payload, bool retain, cloud_protocol_mqtt_msg_ack_type_e ack_type, const char *ack_topic);
void Cloud_Protocol_Mqtt_HandleSubscribeAck(const char *subscribe_topic);
void Cloud_Protocol_Mqtt_HandleReceivedMessage(const char *payload);
void Cloud_Protocol_Mqtt_ClientManagerProcess(void);
#endif /* __CLOUD_PROTOCOL_MQTT_H */
/* EOL */
