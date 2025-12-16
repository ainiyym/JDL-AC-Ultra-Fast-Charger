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
static cloud_net_mqtt_at_callback_t cloud_net_mqtt_at_callbacks = {0};

/******************************************************************************
|    Table Const Definition
******************************************************************************/

/******************************************************************************
|    Static Local Functions Declaration
******************************************************************************/
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttCreatePublishItem(const char *topic, const char *payload, uint8_t qos);
static void CloudNetM_MqttFreePublishItem(cloud_net_mqtt_publish_item_t *item);
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttPeekQueueHead(void);
static bool CloudNetM_MqttNeedSendTopicAT(const char *current_topic);
static bool CloudNetM_MqttSendTopicAT(const char *topic);
static bool CloudNetM_MqttSendPayloadAT(const char *payload);
static void CloudNetM_MqttRetryTopicATHandler(void);
static void CloudNetM_MqttRetryPayloadHandler(void);
static void CloudNetM_MqttHandleTimeout(void);

/******************************************************************************
|    Function Source Code
******************************************************************************/
/* Initialize the publish manager */
bool CloudNetM_MqttPublishManagerInit(cloud_net_mqtt_at_callback_t config)
{
    memset(&cloud_net_mqtt_publish_manager, 0, sizeof(cloud_net_mqtt_publish_manager));

    cloud_net_mqtt_publish_manager.max_queue_size = CLOUDNET_MQTT_MAX_QUEUE_SIZE;
    cloud_net_mqtt_publish_manager.max_retry_count = CLOUDNET_MQTT_MAX_RETRY_COUNT;
    cloud_net_mqtt_at_callbacks = config;

    CLOUDNET_DEBUG("NetM mqtt publish manager initialized\n");
    return true;
}

/* Create a publish queue item */
static cloud_net_mqtt_publish_item_t *CloudNetM_MqttCreatePublishItem(const char *topic, const char *payload, uint8_t qos)
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
bool CloudNetM_MqttAddMessageToQueue(const char *topic, const char *payload, uint8_t qos)
{
    if (topic == NULL || payload == NULL)
    {
        return false;
    }

    /* Check if queue is full */
    if (cloud_net_mqtt_publish_manager.queue_size >= cloud_net_mqtt_publish_manager.max_queue_size)
    {
        CLOUDNET_ERROR("Publish queue is full, cannot add new message\n");
        return false;
    }

    /* Create new queue item */
    cloud_net_mqtt_publish_item_t *new_item = CloudNetM_MqttCreatePublishItem(topic, payload, qos);
    if (new_item == NULL)
    {
        CLOUDNET_ERROR("Failed to create publish queue item\n");
        return false;
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
    return true;
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
    cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
    cloud_net_mqtt_publish_manager.waiting_payload_ok = false;
}

/* Check whether topic AT command needs to be sent */
static bool CloudNetM_MqttNeedSendTopicAT(const char *current_topic)
{
    /* If last topic is NULL or different from current, send topic AT */
    if (cloud_net_mqtt_publish_manager.last_topic == NULL)
    {
        return true;
    }

    if (strcmp(cloud_net_mqtt_publish_manager.last_topic, current_topic) != 0)
    {
        return true;
    }

    /* Same topic, no need to resend */
    return false;
}

/* Send topic AT command */
static bool CloudNetM_MqttSendTopicAT(const char *topic)
{
    if (cloud_net_mqtt_at_callbacks.topic_send_cb == NULL || topic == NULL)
    {
        return false;
    }

    CLOUDNET_DEBUG("<%s> current_time:%lld\r\n", __func__, CLOUD_GET_TIME_MS());

taskENTER_CRITICAL();
    cloud_net_mqtt_publish_manager.waiting_topic_ok = true;
taskEXIT_CRITICAL();
    cloud_net_mqtt_publish_manager.topic_sent_time = CLOUD_GET_TIME_MS();

    if (cloud_net_mqtt_at_callbacks.topic_send_cb(topic, cloud_net_mqtt_at_callbacks.context))
    {
        return true;
    }

    CLOUDNET_DEBUG("Failed to send topic AT\n");
    return false;
}

/* Send payload AT command */
static bool CloudNetM_MqttSendPayloadAT(const char *payload)
{
    if (cloud_net_mqtt_at_callbacks.payload_send_cb == NULL || payload == NULL)
    {
        return false;
    }

    // CLOUDNET_DEBUG("Sending payload AT: %s\n", payload);

taskENTER_CRITICAL();
        cloud_net_mqtt_publish_manager.waiting_payload_ok = true;
        cloud_net_mqtt_publish_manager.payload_sent_time = CLOUD_GET_TIME_MS();
taskEXIT_CRITICAL();
    if (cloud_net_mqtt_at_callbacks.payload_send_cb(payload, cloud_net_mqtt_at_callbacks.context))
    {
        return true;
    }

    CLOUDNET_DEBUG("Failed to send payload AT\n");
    return false;
}

static void CloudNetM_MqttRetryTopicATHandler(void)
{
    cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
    cloud_net_mqtt_publish_manager.topic_at_sent = false;

    /* Retry logic */
    if (cloud_net_mqtt_publish_manager.retry_count < cloud_net_mqtt_publish_manager.max_retry_count)
    {
        cloud_net_mqtt_publish_manager.retry_count++;
        if (cloud_net_mqtt_publish_manager.last_topic != NULL)
        {
            CLOUDM_FREE(cloud_net_mqtt_publish_manager.last_topic);
            cloud_net_mqtt_publish_manager.last_topic = NULL;
        }
        CLOUDNET_DEBUG("<topic:%s>Starting retry #%d\r\n", cloud_net_mqtt_publish_manager.current_msg->topic, cloud_net_mqtt_publish_manager.retry_count);
    }
    else
    {
        CLOUDNET_ERROR("<topic:%s>Reached max retry count, dropping message\r\n", cloud_net_mqtt_publish_manager.current_msg->topic);
        if (cloud_net_mqtt_publish_manager.current_msg != NULL)
        {
            CloudNetM_MqttRemoveQueueHead();
            cloud_net_mqtt_publish_manager.current_msg = NULL;
        }
    }
}

static void CloudNetM_MqttRetryPayloadHandler(void)
{
    cloud_net_mqtt_publish_manager.waiting_payload_ok = false;

    /* Retry logic */
    if (cloud_net_mqtt_publish_manager.retry_count < cloud_net_mqtt_publish_manager.max_retry_count)
    {
        cloud_net_mqtt_publish_manager.retry_count++;
        CLOUDNET_DEBUG("<%s>Mag:%d Starting retry #%d\n", __func__, cloud_net_mqtt_publish_manager.queue_size, cloud_net_mqtt_publish_manager.retry_count);
        /* Restart from topic AT for the current message */
        cloud_net_mqtt_publish_manager.topic_at_sent = false;
        cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
        if (cloud_net_mqtt_publish_manager.last_topic != NULL)
        {
            CLOUDM_FREE(cloud_net_mqtt_publish_manager.last_topic);
            cloud_net_mqtt_publish_manager.last_topic = NULL;
        }
    }
    else
    {
        CLOUDNET_DEBUG("<%s>Mag:%d Reached max retry count, dropping message\n", __func__, cloud_net_mqtt_publish_manager.queue_size);
        if (cloud_net_mqtt_publish_manager.current_msg != NULL)
        {
            CloudNetM_MqttRemoveQueueHead();
            cloud_net_mqtt_publish_manager.current_msg = NULL;
        }
    }
}

/* Handle AT response timeouts and retry logic */
static void CloudNetM_MqttHandleTimeout(void)
{
    uint64_t current_time = CLOUD_GET_TIME_MS();

    /* Check topic AT response timeout */
    if (cloud_net_mqtt_publish_manager.waiting_topic_ok)
    {
        if (current_time - cloud_net_mqtt_publish_manager.topic_sent_time > CLOUDNET_MQTT_AT_RESPONSE_TIMEOUT_MS)
        {
            CLOUDNET_ERROR("<%s> TopicAT err\r\n", __func__);
            CloudNetM_MqttRetryTopicATHandler();
        }
    }

    /* Check payload AT response timeout */
    if (cloud_net_mqtt_publish_manager.waiting_payload_ok)
    {
        if (current_time - cloud_net_mqtt_publish_manager.payload_sent_time > CLOUDNET_MQTT_AT_RESPONSE_TIMEOUT_MS)
        {
            CLOUDNET_ERROR("<%s>  PayloadAT err\r\n", __func__);
            CloudNetM_MqttRetryPayloadHandler();
        }
    }
}

void CloudNetM_MqttHandleAtTopicResponse(const char *pub_topic)
{
    if (pub_topic == NULL)
    {
        return;
    }

    /* Check for OK response */
    if (strstr(pub_topic, cloud_net_mqtt_publish_manager.current_msg->topic) != NULL)
    {
taskENTER_CRITICAL();
        cloud_net_mqtt_publish_manager.waiting_topic_ok = false;
        cloud_net_mqtt_publish_manager.topic_at_sent = true;
taskEXIT_CRITICAL();
        CLOUDNET_DEBUG("<%s>ack succeeded, waiting_topic_ok = %d current_time:%lld\r\n", __func__, cloud_net_mqtt_publish_manager.waiting_topic_ok, CLOUD_GET_TIME_MS());
    }
}

void CloudNetM_GetCurrentPublishMessageTopic(char *topic_buffer, size_t buffer_size)
{
    if (cloud_net_mqtt_publish_manager.current_msg != NULL && topic_buffer != NULL && buffer_size > 0)
    {
        strncpy(topic_buffer, cloud_net_mqtt_publish_manager.current_msg->topic, buffer_size - 1);
        topic_buffer[buffer_size - 1] = '\0'; // Ensure null-termination
    }
    else if (topic_buffer != NULL && buffer_size > 0)
    {
        topic_buffer[0] = '\0'; // Empty string if no current message
    }
    else
    {
        CLOUDNET_ERROR("<%s> Invalid parameters\r\n", __func__);
    }
}

void CloudNetM_MqttHandleATPayloadSendSuccess(void)
{
taskENTER_CRITICAL();
    cloud_net_mqtt_publish_manager.waiting_payload_ok = false;
    cloud_net_mqtt_publish_manager.topic_at_sent = false;

    /* Remove sent message from queue */
    if (cloud_net_mqtt_publish_manager.current_msg != NULL)
    {
        CloudNetM_MqttRemoveQueueHead();
        cloud_net_mqtt_publish_manager.current_msg = NULL;
    }
taskEXIT_CRITICAL();
}

/* Periodic publish manager processing function */
void CloudNetM_MqttPublishManagerProcess(void)
{
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
        }
    }
    else
    {
        /* Stage 2: send payload AT */
        if (cloud_net_mqtt_publish_manager.current_msg->payload != NULL)
        {
            CloudNetM_MqttSendPayloadAT(cloud_net_mqtt_publish_manager.current_msg->payload);
        }
    }
}
/* EOL */
