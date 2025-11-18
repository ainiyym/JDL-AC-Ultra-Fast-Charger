// ...existing code...
//*******************************************************************************
//* File Name: CloudNet_Mqtt_PublishM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: CloudNet MQTT Publish module source file
//*******************************************************************************
/******************************************************************************
|    Other Header File Inclusion
******************************************************************************/
#include "CloudNet_Mqtt_PublishM.h"
#include "CloudNet_Cfg.h"

/******************************************************************************
|    Macro Definition
******************************************************************************/

/******************************************************************************
|    Enum Definition
******************************************************************************/

/******************************************************************************
|    Typedef Definition
******************************************************************************/

/******************************************************************************
|    Static local KAM variables Declaration
******************************************************************************/

/******************************************************************************
|    Static local variables Declaration
******************************************************************************/

/******************************************************************************
|    Global variables Declaration
******************************************************************************/
static cloud_net_mqtt_publish_manager_t cloud_net_mqtt_publish_manager = {0};
static cloud_net_at_send_callback_t cloud_net_mqtt_at_send_callback = NULL;
static cloud_net_at_response_handler_t cloud_net_mqtt_at_response_handler = NULL;
static void *cloud_net_mqtt_at_context = NULL;

/******************************************************************************
|    Table Const Definition
******************************************************************************/

/******************************************************************************
|    Static Local Functions Declaration
******************************************************************************/
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttCreatePublishItem(const char *topic, const char *payload, bool retain, uint8_t qos);
static void CloudNetM_MqttFreePublishItem(cloud_net_mqtt_publish_item_t *item);
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttPeekQueueHead(void);
static bool CloudNetM_MqttNeedSendTopicAT(const char *current_topic);
static bool CloudNetM_MqttSendTopicAT(const char *topic);
static bool CloudNetM_MqttSendPayloadAT(const char *payload);
static void CloudNetM_MqttHandleTimeout(void);

/******************************************************************************
|    Function Source Code
******************************************************************************/
/* Initialize the publish manager */
bool CloudNetM_MqttPublishManagerInit(cloud_net_at_send_callback_t send_cb, cloud_net_at_response_handler_t resp_handler, void *context)
{
    if (send_cb == NULL || resp_handler == NULL)
    {
        return FALSE;
    }

    memset(&cloud_net_mqtt_publish_manager, 0, sizeof(cloud_net_mqtt_publish_manager));

    cloud_net_mqtt_publish_manager.max_queue_size = CLOUDNET_MQTT_MAX_QUEUE_SIZE;
    cloud_net_mqtt_publish_manager.max_retry_count = CLOUDNET_MQTT_MAX_RETRY_COUNT;
    cloud_net_mqtt_at_send_callback = send_cb;
    cloud_net_mqtt_at_response_handler = resp_handler;
    cloud_net_mqtt_at_context = context; 

    CLOUDNET_DEBUG("Publish manager initialized\n");
    return TRUE;
}

/* Create a publish queue item */
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttCreatePublishItem(const char *topic, const char *payload, bool retain, uint8_t qos)
{
    if (topic == NULL || payload == NULL)
    {
        return NULL;
    }

    cloud_net_mqtt_publish_item_t *item = (cloud_net_mqtt_publish_item_t *)CLOUDM_MALLOC(sizeof(cloud_net_mqtt_publish_item_t));
    if (item == NULL)
    {
        return NULL;
    }

    item->topic = CloudNet_Strdup(topic);
    item->payload = CloudNet_Strdup(payload);
    item->retain = retain;
    item->qos = qos;
    item->timestamp = CLOUD_GET_TIME_MS(); // requires implementation of current time getter
    item->next = NULL;

    if (item->topic == NULL || item->payload == NULL)
    {
        CLOUDM_FREE(item->topic);
        CLOUDM_FREE(item->payload);
        CLOUDM_FREE(item);
        return NULL;
    }

    return item;
}

/* Free a publish queue item */
static void CloudNetM_MqttFreePublishItem(cloud_net_mqtt_publish_item_t *item)
{
    if (item != NULL)
    {
        CLOUDM_FREE(item->topic);
        CLOUDM_FREE(item->payload);
        CLOUDM_FREE(item);
    }
}

/* Add a message to the publish queue */
bool CloudNetM_MqttAddMessageToQueue(const char *topic, const char *payload, bool retain, uint8_t qos)
{
    if (topic == NULL || payload == NULL)
    {
        return FALSE;
    }

    /* Check if queue is full */
    if (cloud_net_mqtt_publish_manager.queue_size >= cloud_net_mqtt_publish_manager.max_queue_size)
    {
        CLOUDNET_ERROR("Publish queue is full, cannot add new message\n");
        return FALSE;
    }

    /* Create new queue item */
    cloud_net_mqtt_publish_item_t *new_item = CloudNetM_MqttCreatePublishItem(topic, payload, retain, qos);
    if (new_item == NULL)
    {
        CLOUDNET_ERROR("Failed to create publish queue item\n");
        return FALSE;
    }

    /* Append to queue tail */
    if (cloud_net_mqtt_publish_manager.tail == NULL)
    {
        cloud_net_mqtt_publish_manager.head = new_item;
        cloud_net_mqtt_publish_manager.tail = new_item;
    }
    else
    {
        cloud_net_mqtt_publish_manager.tail->next = new_item;
        cloud_net_mqtt_publish_manager.tail = new_item;
    }

    cloud_net_mqtt_publish_manager.queue_size++;

    CLOUDNET_DEBUG("Message added to queue: topic=%s, queue size=%d\n", topic, cloud_net_mqtt_publish_manager.queue_size);
    return TRUE;
}

/* Peek at queue head (do not remove) */
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttPeekQueueHead(void)
{
    return cloud_net_mqtt_publish_manager.head;
}

/* Remove message at queue head */
static void CloudNetM_MqttRemoveQueueHead(void)
{
    if (cloud_net_mqtt_publish_manager.head == NULL)
    {
        return;
    }

    cloud_net_mqtt_publish_item_t *old_head = cloud_net_mqtt_publish_manager.head;
    cloud_net_mqtt_publish_manager.head = old_head->next;

    if (cloud_net_mqtt_publish_manager.head == NULL)
    {
        cloud_net_mqtt_publish_manager.tail = NULL;
    }

    CloudNetM_MqttFreePublishItem(old_head);
    cloud_net_mqtt_publish_manager.queue_size--;
}

/* Check whether topic AT command needs to be sent */
static bool CloudNetM_MqttNeedSendTopicAT(const char *current_topic)
{
    /* If last topic is NULL or different from current, send topic AT */
    if (cloud_net_mqtt_publish_manager.last_topic == NULL)
    {
        return TRUE;
    }

    if (strcmp(cloud_net_mqtt_publish_manager.last_topic, current_topic) != 0)
    {
        return TRUE;
    }

    /* Same topic, no need to resend */
    return FALSE;
}

/* Send topic AT command */
static bool CloudNetM_MqttSendTopicAT(const char *topic)
{
    if (cloud_net_mqtt_at_send_callback == NULL || topic == NULL)
    {
        return FALSE;
    }

    CLOUDNET_DEBUG("Sending topic AT: %s\n", topic);

    if (cloud_net_mqtt_at_send_callback(topic))
    {
        cloud_net_mqtt_publish_manager.topic_at_sent = TRUE;
        cloud_net_mqtt_publish_manager.waiting_topic_ok = TRUE;
        cloud_net_mqtt_publish_manager.topic_sent_time = CLOUD_GET_TIME_MS();
        cloud_net_mqtt_publish_manager.retry_count = 0;
        return TRUE;
    }

    CLOUDNET_DEBUG("Failed to send topic AT\n");
    return FALSE;
}

/* Send payload AT command */
static bool CloudNetM_MqttSendPayloadAT(const char *payload)
{
    if (cloud_net_mqtt_at_send_callback == NULL || payload == NULL)
    {
        return FALSE;
    }

    CLOUDNET_DEBUG("Sending payload AT: %s\n", payload);

    if (cloud_net_mqtt_at_send_callback(payload))
    {
        cloud_net_mqtt_publish_manager.waiting_payload_ok = TRUE;
        cloud_net_mqtt_publish_manager.payload_sent_time = CLOUD_GET_TIME_MS();
        return TRUE;
    }

    CLOUDNET_DEBUG("Failed to send payload AT\n");
    return FALSE;
}

/* Handle AT response timeouts and retry logic */
static void CloudNetM_MqttHandleTimeout(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();

    /* Check topic AT response timeout */
    if (cloud_net_mqtt_publish_manager.waiting_topic_ok)
    {
        if (current_time - cloud_net_mqtt_publish_manager.topic_sent_time > CLOUDNET_MQTT_AT_RESPONSE_TIMEOUT_MS)
        {
            CLOUDNET_DEBUG("Topic AT response timeout\n");
            cloud_net_mqtt_publish_manager.waiting_topic_ok = FALSE;
            cloud_net_mqtt_publish_manager.topic_at_sent = FALSE;

            /* Retry logic */
            if (cloud_net_mqtt_publish_manager.retry_count < cloud_net_mqtt_publish_manager.max_retry_count)
            {
                cloud_net_mqtt_publish_manager.retry_count++;
                CLOUDNET_DEBUG("Starting retry #%d\n", cloud_net_mqtt_publish_manager.retry_count);
                /* Reset current message to trigger resend */
                cloud_net_mqtt_publish_manager.current_msg = NULL;
            }
            else
            {
                CLOUDNET_DEBUG("Reached max retry count, dropping message\n");
                if (cloud_net_mqtt_publish_manager.current_msg != NULL)
                {
                    CloudNetM_MqttRemoveQueueHead();
                    cloud_net_mqtt_publish_manager.current_msg = NULL;
                }
            }
        }
    }

    /* Check payload AT response timeout */
    if (cloud_net_mqtt_publish_manager.waiting_payload_ok)
    {
        if (current_time - cloud_net_mqtt_publish_manager.payload_sent_time > CLOUDNET_MQTT_AT_RESPONSE_TIMEOUT_MS)
        {
            CLOUDNET_DEBUG("Payload AT response timeout\n");
            cloud_net_mqtt_publish_manager.waiting_payload_ok = FALSE;

            /* Retry logic */
            if (cloud_net_mqtt_publish_manager.retry_count < cloud_net_mqtt_publish_manager.max_retry_count)
            {
                cloud_net_mqtt_publish_manager.retry_count++;
                CLOUDNET_DEBUG("Starting retry #%d\n", cloud_net_mqtt_publish_manager.retry_count);
                /* Restart from topic AT for the current message */
                cloud_net_mqtt_publish_manager.topic_at_sent = FALSE;
                cloud_net_mqtt_publish_manager.waiting_topic_ok = FALSE;
            }
            else
            {
                CLOUDNET_DEBUG("Reached max retry count, dropping message\n");
                if (cloud_net_mqtt_publish_manager.current_msg != NULL)
                {
                    CloudNetM_MqttRemoveQueueHead();
                    cloud_net_mqtt_publish_manager.current_msg = NULL;
                }
            }
        }
    }
}

/* Periodic publish manager processing function */
void CloudNetM_MqttPublishManagerProcess(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();

    /* Throttle processing by interval */
    if (current_time - cloud_net_mqtt_publish_manager.last_process_time < CLOUDNET_MQTT_PROCESS_INTERVAL_MS)
    {
        return;
    }

    cloud_net_mqtt_publish_manager.last_process_time = current_time;

    /* Handle timeouts first */
    CloudNetM_MqttHandleTimeout();

    /* If waiting for AT responses, defer further processing */
    if (cloud_net_mqtt_publish_manager.waiting_topic_ok || cloud_net_mqtt_publish_manager.waiting_payload_ok)
    {
        return;
    }

    /* If no current message, peek queue head */
    if (cloud_net_mqtt_publish_manager.current_msg == NULL)
    {
        cloud_net_mqtt_publish_manager.current_msg = CloudNetM_MqttPeekQueueHead();
        if (cloud_net_mqtt_publish_manager.current_msg == NULL)
        {
            /* Queue empty */
            return;
        }

        CLOUDNET_DEBUG("Start processing message: topic=%s\n", cloud_net_mqtt_publish_manager.current_msg->topic);
        cloud_net_mqtt_publish_manager.retry_count = 0;
    }

    /* Process current message */
    cloud_net_mqtt_publish_item_t *current_msg = cloud_net_mqtt_publish_manager.current_msg;

    /* Stage 1: send topic AT if needed */
    if (!cloud_net_mqtt_publish_manager.topic_at_sent)
    {
        if (CloudNetM_MqttNeedSendTopicAT(current_msg->topic))
        {
            /* Need to send topic AT */
            if (CloudNetM_MqttSendTopicAT(current_msg->topic))
            {
                /* Save last topic */
                if (cloud_net_mqtt_publish_manager.last_topic != NULL)
                {
                    CLOUDM_FREE(cloud_net_mqtt_publish_manager.last_topic);
                }
                cloud_net_mqtt_publish_manager.last_topic = CloudNet_Strdup(current_msg->topic);
            }
            else
            {
                /* Failed to send topic AT: drop message */
                CLOUDNET_DEBUG("Failed to send topic AT, dropping message\n");
                CloudNetM_MqttRemoveQueueHead();
                cloud_net_mqtt_publish_manager.current_msg = NULL;
            }
        }
        else
        {
            /* Same topic: skip topic AT and send payload directly */
            CLOUDNET_DEBUG("Same topic, skip topic AT\n");
            cloud_net_mqtt_publish_manager.topic_at_sent = true;
            /* proceed to payload send */
            CloudNetM_MqttSendPayloadAT(current_msg->payload);
        }
    }
    else
    {
        /* Stage 2: send payload AT */
        CloudNetM_MqttSendPayloadAT(current_msg->payload);
    }
}

/* AT response handler */
void CloudNetM_MqttHandleATResponse(const char *response)
{
    if (response == NULL)
    {
        return;
    }

    CLOUDNET_DEBUG("Received AT response: %s\n", response);

    /* Call user-registered response handler */
    if (cloud_net_mqtt_at_response_handler != NULL)
    {
        cloud_net_mqtt_at_response_handler(response, cloud_net_mqtt_at_context);
    }

    /* Check for OK response */
    if (strstr(response, "OK") != NULL)
    {
        if (cloud_net_mqtt_publish_manager.waiting_topic_ok)
        {
            CLOUDNET_DEBUG("Topic AT command succeeded\n");
            cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
            /* Topic AT succeeded: send payload AT */
            if (cloud_net_mqtt_publish_manager.current_msg != NULL)
            {
                CloudNetM_MqttSendPayloadAT(cloud_net_mqtt_publish_manager.current_msg->payload);
            }
        }
        else if (cloud_net_mqtt_publish_manager.waiting_payload_ok)
        {
            CLOUDNET_DEBUG("Payload AT command succeeded, message sent\n");
            cloud_net_mqtt_publish_manager.waiting_payload_ok = false;
            cloud_net_mqtt_publish_manager.topic_at_sent = false;

            /* Remove sent message from queue */
            if (cloud_net_mqtt_publish_manager.current_msg != NULL)
            {
                CloudNetM_MqttRemoveQueueHead();
                cloud_net_mqtt_publish_manager.current_msg = NULL;
            }
        }
    }
    else if (strstr(response, "ERROR") != NULL)
    {
        /* Handle error response */
        CLOUDNET_DEBUG("AT command failed\n");

        if (cloud_net_mqtt_publish_manager.waiting_topic_ok)
        {
            cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
            cloud_net_mqtt_publish_manager.topic_at_sent = false;
        }
        else if (cloud_net_mqtt_publish_manager.waiting_payload_ok)
        {
            cloud_net_mqtt_publish_manager.waiting_payload_ok = false;
        }

        /* Retry logic */
        if (cloud_net_mqtt_publish_manager.retry_count < cloud_net_mqtt_publish_manager.max_retry_count)
        {
            cloud_net_mqtt_publish_manager.retry_count++;
            CLOUDNET_DEBUG("Starting retry #%d\n", cloud_net_mqtt_publish_manager.retry_count);
            /* Reset state to resend next cycle */
            cloud_net_mqtt_publish_manager.topic_at_sent = false;
        }
        else
        {
            CLOUDNET_DEBUG("Reached max retry count, dropping message\n");
            if (cloud_net_mqtt_publish_manager.current_msg != NULL)
            {
                CloudNetM_MqttRemoveQueueHead();
                cloud_net_mqtt_publish_manager.current_msg = NULL;
            }
        }
    }
}
/* EOL */
// ...existing code...
