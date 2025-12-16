//******************************************************************************
//* File Name: CloudNet_Mqtt_PublishM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: CloudNet Mqtt Publish module header file
/*******************************************************************************/
#if !defined (__CLOUDNET_MQTT_PUBLISHM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUDNET_MQTT_PUBLISHM_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Cloud_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* AT command callback types */
typedef bool (*cloud_net_at_send_callback_t)(const char *at_parameter, uint8_t context);

typedef struct
{
    uint8_t context;                                            /* User context */
    cloud_net_at_send_callback_t topic_send_cb;               /* Callback to send topic AT command */
    cloud_net_at_send_callback_t payload_send_cb;             /* Callback to send payload AT command */
} cloud_net_mqtt_at_callback_t;

/* MQTT publish queue item */
typedef struct cloud_net_mqtt_publish_item
{
    char *topic;                              /* Topic string */
    char *payload;                            /* Payload data */
    uint8_t qos;                              /* Quality of Service */
    uint64_t timestamp;                       /* Timestamp */
    struct cloud_net_mqtt_publish_item *next; /* Next message in queue */
} cloud_net_mqtt_publish_item_t;

/* Publish manager structure */
typedef struct
{
    cloud_net_mqtt_publish_item_t *head;        /* Queue head */
    cloud_net_mqtt_publish_item_t *tail;        /* Queue tail */
    cloud_net_mqtt_publish_item_t *current_msg; /* Currently processed message */
    uint16_t queue_size;                        /* Current queue size */
    uint16_t max_queue_size;                    /* Maximum queue size */
    char *last_topic;                           /* Topic of last message */
    volatile bool topic_at_sent;                /* Topic AT command sent */
    volatile bool waiting_topic_ok;             /* Waiting for topic AT response */
    volatile bool waiting_payload_ok;           /* Waiting for payload AT response */
    uint64_t topic_sent_time;                   /* Topic send timestamp */
    uint64_t payload_sent_time;                 /* Payload send timestamp */
    uint16_t retry_count;                       /* Retry counter */
    uint16_t max_retry_count;                   /* Maximum retry attempts */
} cloud_net_mqtt_publish_manager_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern bool CloudNetM_MqttPublishManagerInit(cloud_net_mqtt_at_callback_t config);
extern bool CloudNetM_MqttAddMessageToQueue(const char *topic, const char *payload, uint8_t qos);
extern void CloudNetM_GetCurrentPublishMessageTopic(char *topic_buffer, size_t buffer_size);
extern void CloudNetM_MqttHandleAtTopicResponse(const char *pub_topic);
extern void CloudNetM_MqttHandleATPayloadSendSuccess(void);
extern void CloudNetM_MqttPublishManagerProcess(void);
#endif /* __CLOUDNET_MQTT_PUBLISHM_H */
/* EOL */
