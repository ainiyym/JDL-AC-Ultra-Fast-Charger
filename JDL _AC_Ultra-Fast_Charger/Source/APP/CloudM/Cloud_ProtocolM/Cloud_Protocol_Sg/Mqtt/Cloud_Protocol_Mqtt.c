// ...existing code...
//*******************************************************************************
//* File Name: Cloud_Protocol_Mqtt.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: SG protocol mqtt module source file
//*******************************************************************************
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Mqtt.h"
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
/* MQTT protocol control structure */
typedef struct
{
    cloud_protocol_mqtt_state_e net_status; /* MQTT connection status */
    bool is_initialized;
    bool ip_connected;
    uint64_t last_connect_time;
} mqtt_client_ctrl_t;

/* Subscription request status */
typedef enum
{
    SUB_REQ_IDLE = 0,	   // idle
    SUB_REQ_SENT,		   // sent
    SUB_REQ_WAITING_ACK,   // waiting for acknowledgment
    SUB_REQ_ACTIVE		   // subscription active
} subscribe_request_state_e;

/* MQTT client manager */
typedef struct
{
    mqtt_client_ctrl_t ctrl; /* control state */

    /* topic configuration */
    const cloud_protocol_mqtt_topic_config_t *passive_topic_configs; /*  passive topic config array */
    uint16_t passive_topic_count;                                    /* number of passive topics */

    /* message queue management */
    cloud_protocol_mqtt_message_item_t *msg_queue_head; /* queue head */
    cloud_protocol_mqtt_message_item_t *msg_queue_tail; /* queue tail */
    uint16_t queue_size;                                /* current queue size */
    uint16_t max_queue_size;                            /* maximum queue size */

    /* subscription management */
    char *current_subscribe_topic;                          /* current subscribe topic */
    bool switch_default_topic_enabled;                      /* whether switch default subscribe topic */
    bool current_topic_is_polling;                          /* whether the current topic is polling */
    cloud_protocol_mqtt_message_item_t *last_processed_msg; /* last processed message (for iteration) */
    cloud_protocol_mqtt_subscribe_state_e sub_state;        /* subscription state */
    uint64_t last_subscribe_time;                           /* last subscribe time */
    uint32_t subscribe_switch_interval;                     /* subscribe switch interval (ms) */

    /* callbacks */
    cloud_protocol_mqtt_connect_cb connect_callback;
    cloud_protocol_mqtt_msg_cb message_callback;
} mqtt_client_manager_t;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static iotx_sign_mqtt_t cloud_protocol_mqtt_config;
static mqtt_client_manager_t cloud_protocol_mqtt_client;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_Mqtt_StartConnection(void);
static void Cloud_Protocol_Mqtt_StartReconnect(void);
static void Cloud_Protocol_Mqtt_RemoveMessageFromQueue(cloud_protocol_mqtt_message_item_t *msg);
static void Cloud_Protocol_Mqtt_DropOldestMessage(void);
static void Cloud_Protocol_Mqtt_FreePublishItem(cloud_protocol_mqtt_message_item_t *item);
static void Cloud_Protocol_Mqtt_SendConnectMsg(void);
static bool Cloud_Protocol_Mqtt_SendPublishMessage(cloud_protocol_mqtt_message_item_t *msg);
static bool Cloud_Protocol_Mqtt_SendPublishMsg(const cloud_protocol_mqtt_publish_t *publish);
static void Cloud_Protocol_Mqtt_SendSubscribePublishMsg(const cloud_protocol_mqtt_subscribe_topic_t *subscribe, const cloud_protocol_mqtt_publish_topic_t *publish);
static uint16_t Cloud_Protocol_Mqtt_PackString(uint8_t *buffer, const char *str, uint16_t str_len);
static bool Cloud_Protocol_Mqtt_ProcessSingleMessage(cloud_protocol_mqtt_message_item_t *msg);
static void Cloud_Protocol_Mqtt_PollingCurrentSubscribe(void);
static void Cloud_Protocol_Mqtt_ProcessMessageQueue(void);
static void Cloud_Protocol_Mqtt_SwitchDefaultSubscribe(void);
static void Cloud_Protocol_Mqtt_CheckTimeouts(void);
static void Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_message_item_t *msg);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Initialize MQTT client manager
 */
bool Cloud_Protocol_Mqtt_ClientManagerInit(const cloud_protocol_mqtt_topic_config_t *passive_topic_configs, uint16_t passive_topic_count,
                            iotx_sign_mqtt_t* mqtt_client,
                            cloud_protocol_mqtt_connect_cb connect_cb,
                            cloud_protocol_mqtt_msg_cb msg_cb)
{
    if (passive_topic_configs == NULL || passive_topic_count == 0)
    {
        return false;
    }

    memset(&cloud_protocol_mqtt_client, 0, sizeof(cloud_protocol_mqtt_client));
    memset(&cloud_protocol_mqtt_config, 0, sizeof(cloud_protocol_mqtt_config));

    cloud_protocol_mqtt_client.passive_topic_configs = passive_topic_configs;
    cloud_protocol_mqtt_client.passive_topic_count = passive_topic_count;
    cloud_protocol_mqtt_client.max_queue_size = CLOUD_PROTOCOL_MQTT_MAX_QUEUE_SIZE;
    cloud_protocol_mqtt_client.subscribe_switch_interval = CLOUD_PROTOCOL_MQTT_PROCESS_INTERVAL_MS; /* subscribe switch interval (ms) */
    cloud_protocol_mqtt_client.switch_default_topic_enabled = ENABLE;	/* enable polling subscribe */

    cloud_protocol_mqtt_client.connect_callback = connect_cb;
    cloud_protocol_mqtt_client.message_callback = msg_cb;

    cloud_protocol_mqtt_client.ctrl.net_status = CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED;

    cloud_protocol_mqtt_config = *mqtt_client;

    cloud_protocol_mqtt_client.ctrl.is_initialized = true;

    CLOUD_INFO("MQTT client manager initialized with %d topics\r\n", passive_topic_count);
    return true;
}

void Cloud_Protocol_Mqtt_GetClientConfig(iotx_sign_mqtt_t *mqtt_client)
{
    if (mqtt_client != NULL)
    {
        *mqtt_client = cloud_protocol_mqtt_config;
    }
}

/**
 * @brief Set device IP connection status
 */
void Cloud_Protocol_Mqtt_SetDeviceIPConnectionStatus(bool connected)
{
    cloud_protocol_mqtt_client.ctrl.ip_connected = connected;
    if (connected)
    {
        Cloud_Protocol_Mqtt_StartConnection();
    }
    else
    {
        // CLOUD_INFO("net ip break, disconnecting MQTT\r\n");
        Cloud_Protocol_Mqtt_HandleDisconnected();
    }
}

/**
 * @brief Handle received message (server request)
 * @param payload message payload
 */
void Cloud_Protocol_Mqtt_HandleReceivedMessage(const char *payload)
{
    if (payload == NULL)
    {
        return;
    }
    bool response_correct = true;

    // CLOUD_INFO("Received message: %s\r\n", payload);

    if (cloud_protocol_mqtt_client.message_callback == NULL)
    {
        return ;
    }
    cloud_protocol_sg_message_rcv_type_e type = CLOUD_PROTOCOL_SG_MESSAGE_RCV_TYPE_UNKNOWN;
    /* Call message callback */
    response_correct = cloud_protocol_mqtt_client.message_callback(payload, &type);

    // Handle active message receive failure
    if (!response_correct && cloud_protocol_mqtt_client.last_processed_msg != NULL && type != CLOUD_PROTOCOL_SG_MESSAGE_RCV_TYPE_PASSSTIVE)
    {
        Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_client.last_processed_msg);
        return;
    }
    else if (response_correct && type != CLOUD_PROTOCOL_SG_MESSAGE_RCV_TYPE_PASSSTIVE)
    {
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_ACTIVE;
        CLOUD_INFO("<%s> Message processed successfully\r\n", __func__);
    }
    else if (type == CLOUD_PROTOCOL_SG_MESSAGE_RCV_TYPE_PASSSTIVE)
    {
        CLOUD_INFO("<%s> passive message processed\r\n", __func__);
    }
    else
    {

    }

    /* Remove message from queue */
    if (type != CLOUD_PROTOCOL_SG_MESSAGE_RCV_TYPE_PASSSTIVE && cloud_protocol_mqtt_client.last_processed_msg != NULL)
    {
        Cloud_Protocol_Mqtt_RemoveMessageFromQueue(cloud_protocol_mqtt_client.last_processed_msg);
        cloud_protocol_mqtt_client.last_processed_msg = NULL;
        CLOUD_INFO("<%s>Message processed and removed from queue\r\n", __func__);
    }
}

/**
 * @brief Handle connection established event
 */
void Cloud_Protocol_Mqtt_HandleConnected(void)
{
    cloud_protocol_mqtt_client.ctrl.net_status = CLOUD_PROTOCOL_MQTT_STATE_CONNECTED;

    /* Call connect callback */
    if (cloud_protocol_mqtt_client.connect_callback != NULL)
    {
        cloud_protocol_mqtt_client.connect_callback(true);
    }

    cloud_protocol_mqtt_client.ctrl.last_connect_time = CLOUD_GET_TIME_MS();

    CLOUD_DEBUG("<%s> MQTT connected successfully, starting in polling mode\r\n", __func__);
}

/**
 * @brief Get current network status
 * @return current network status
 */
cloud_protocol_mqtt_state_e Cloud_Protocol_Mqtt_GetNetworkStatus(void)
{
    return cloud_protocol_mqtt_client.ctrl.net_status;
}

/**
 * @brief Handle subscribe acknowledgment
 * @param subscribe_topic subscribed topic
 */
void Cloud_Protocol_Mqtt_HandleSubscribeAck(const char *subscribe_topic)
{
    if (subscribe_topic == NULL)
    {
        return;
    }

    /* Validate ACK belongs to current subscribe topic */
    if (cloud_protocol_mqtt_client.current_subscribe_topic == NULL || strcmp(cloud_protocol_mqtt_client.current_subscribe_topic, subscribe_topic) != 0)
    {
        CLOUD_WARN("Subscribe ACK for unexpected topic: %s, current: %s\r\n", subscribe_topic, cloud_protocol_mqtt_client.current_subscribe_topic);
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
        return;
    }
    else
    {
        CLOUD_INFO("<%s> <%s>Subscribe ACK success.\r\n", __func__, subscribe_topic);
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_ACTIVE;
        cloud_protocol_mqtt_client.current_topic_is_polling = true;
        cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
    }
}

/**
 * @brief Start connection
 */
static void Cloud_Protocol_Mqtt_StartConnection(void)
{
    if (!cloud_protocol_mqtt_client.ctrl.is_initialized ||
        !cloud_protocol_mqtt_client.ctrl.ip_connected)
    {
        return;
    }

    if (cloud_protocol_mqtt_client.ctrl.net_status == CLOUD_PROTOCOL_MQTT_STATE_CONNECTED ||
        cloud_protocol_mqtt_client.ctrl.net_status == CLOUD_PROTOCOL_MQTT_STATE_CONNECTING)
    {
        return;
    }

    CLOUD_INFO("<%s>Starting MQTT connection...\r\n", __func__);
    cloud_protocol_mqtt_client.ctrl.net_status = CLOUD_PROTOCOL_MQTT_STATE_CONNECTING;
    cloud_protocol_mqtt_client.ctrl.last_connect_time = CLOUD_GET_TIME_MS();

    Cloud_Protocol_Mqtt_SendConnectMsg();
}

/**
 * @brief Handle disconnection event
 */
void Cloud_Protocol_Mqtt_HandleDisconnected(void)
{
    cloud_protocol_mqtt_client.ctrl.net_status = CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED;

    if (0 == cloud_protocol_mqtt_client.ctrl.ip_connected)
    {
        return;
    }

    CLOUD_INFO("<%s> MQTT disconnected\r\n", __func__);

    /* Call connect callback */
    if (cloud_protocol_mqtt_client.connect_callback != NULL)
    {
        cloud_protocol_mqtt_client.connect_callback(false);
    }
}

/**
 * @brief Start reconnect attempts
 */
static void Cloud_Protocol_Mqtt_StartReconnect(void)
{
    uint64_t current_time = CLOUD_GET_TIME_MS();
    uint32_t reconnect_interval = CLOUD_PROTOCOL_MQTT_RECONNECT_INTERVAL_MS; /* reconnect interval */

    if (current_time - cloud_protocol_mqtt_client.ctrl.last_connect_time < reconnect_interval)
    {
        return;
    }
	Cloud_Protocol_Mqtt_StartConnection();
}

/**
 * @brief Add publish message to queue
 * @param topic publish topic
 * @param payload message payload
 * @param ack_type acknowledgment type
 * @param ack_topic ack topic (for messages requiring ack)
 * @return true on success, false on failure
 */
bool Cloud_Protocol_Mqtt_AddPublishMessage(const char *topic, const char *payload, cloud_protocol_mqtt_msg_ack_type_e ack_type, const char *ack_topic)
{
    if (topic == NULL || payload == NULL)
    {
        return false;
    }

    if (ack_type == CLOUD_PROTOCOL_MQTT_NEED_ACK && ack_topic == NULL)
    {
        CLOUD_ERROR("<%s> ACK topic required for messages that need ACK\r\n", __func__);
        return false;
    }

    /* Check queue full */
    if (cloud_protocol_mqtt_client.queue_size >= cloud_protocol_mqtt_client.max_queue_size)
    {
        CLOUD_ERROR("<%s> Message queue is full\r\n", __func__);
        return false;
    }

    /* Calculate required memory size */
    size_t required_memory = 0;

    /* Structure size */
    required_memory += sizeof(cloud_protocol_mqtt_message_item_t);

    /* Topic string (including null terminator) */
    required_memory += strlen(topic) + 1;

    /* Payload string (including null terminator) */
    required_memory += strlen(payload) + 1;

    /* ACK topic string if needed */
    if (ack_topic != NULL)
    {
        required_memory += strlen(ack_topic) + 1;
    }

    // CLOUD_DEBUG("<%s> Required memory: %u bytes\r\n", __func__, required_memory);

    /* Check available heap memory - reject if less than 1KB available */
    size_t free_heap = CLOUDM_GET_FREE_HEAP_SIZE();
    // size_t min_heap = CLOUDM_GET_MINIMUM_HEAP_SIZE();

    // CLOUD_DEBUG("<%s> FreeHeap: %u, MinHeap: %u\r\n", __func__, free_heap, min_heap);

    /* Safety check: if free heap is less than 1KB or less than required memory */
    if (free_heap < required_memory + CLOUDM_MIN_HEAP_SIZE_THRESHOLD)
    {
        CLOUD_WARN("<%s> Insufficient heap memory. Free: %u, Required: %u\r\n",__func__, free_heap, required_memory);

        /* Try to free some memory by dropping oldest messages if queue not empty */
        if (cloud_protocol_mqtt_client.queue_size > 0  &&
            (cloud_protocol_mqtt_client.sub_state == CLOUD_PROTOCOL_SUB_STATE_IDLE ||
             cloud_protocol_mqtt_client.sub_state == CLOUD_PROTOCOL_SUB_STATE_ACTIVE)) 
        {
            CLOUD_WARN("<%s> Attempting to free memory by dropping oldest message\r\n", __func__);
            Cloud_Protocol_Mqtt_DropOldestMessage();

            /* Check again after freeing */
            free_heap = CLOUDM_GET_FREE_HEAP_SIZE();
            if (free_heap < CLOUDM_MIN_HEAP_SIZE_THRESHOLD || free_heap < required_memory)
            {
                CLOUD_ERROR("<%s> Still insufficient memory after cleanup. Rejecting message.\r\n", __func__);
            }
        }
        else
        {
            CLOUD_ERROR("<%s> Insufficient memory and sub_state is not idle or active. Rejecting message.\r\n", __func__);
        }
        return false;
    }

    /* Create new message item */
    cloud_protocol_mqtt_message_item_t *new_msg = (cloud_protocol_mqtt_message_item_t *)CLOUDM_MALLOC(sizeof(cloud_protocol_mqtt_message_item_t));
    if (new_msg == NULL)
    {
        CLOUD_ERROR("<%s> Failed to allocate memory for message structure\r\n", __func__);
        return false;
    }

    /* Initialize structure */
    memset(new_msg, 0, sizeof(cloud_protocol_mqtt_message_item_t));

    /* Allocate and copy strings */
    new_msg->publish.topic = Cloud_Protocol_Strdup(topic);
    new_msg->publish.payload = Cloud_Protocol_Strdup(payload);
    new_msg->ack_type = ack_type;
    new_msg->ack_topic = (ack_topic != NULL) ? Cloud_Protocol_Strdup(ack_topic) : NULL;
    new_msg->timestamp = CLOUD_GET_TIME_MS();
    new_msg->retry_count = 0;
    new_msg->next = NULL;

    /* Check memory allocation */
    if (new_msg->publish.topic == NULL || new_msg->publish.payload == NULL || (ack_topic != NULL && new_msg->ack_topic == NULL))
    {
        Cloud_Protocol_Mqtt_FreePublishItem(new_msg);
        CLOUD_ERROR("<%s> Memory allocation failed for new message\r\n", __func__);
        return false;
    }

    /* Append to queue tail */
    if (cloud_protocol_mqtt_client.msg_queue_tail == NULL)
    {
        cloud_protocol_mqtt_client.msg_queue_head = new_msg;
        cloud_protocol_mqtt_client.msg_queue_tail = new_msg;
    }
    else
    {
        cloud_protocol_mqtt_client.msg_queue_tail->next = new_msg;
        cloud_protocol_mqtt_client.msg_queue_tail = new_msg;
    }

    cloud_protocol_mqtt_client.queue_size++;
    free_heap = CLOUDM_GET_FREE_HEAP_SIZE();

    CLOUD_INFO("Message added to queue: %s, ACK type: %d, queue size: %d, free_heap: %u\r\n", topic, ack_type, cloud_protocol_mqtt_client.queue_size, free_heap);

    return true;
}

/**
 * @brief Drop oldest message from queue to free memory
 */
static void Cloud_Protocol_Mqtt_DropOldestMessage(void)
{
    if (cloud_protocol_mqtt_client.msg_queue_head == NULL)
    {
        return;
    }

    cloud_protocol_mqtt_message_item_t *oldest = cloud_protocol_mqtt_client.msg_queue_head;

    /* Update queue head */
    cloud_protocol_mqtt_client.msg_queue_head = oldest->next;

    /* If queue becomes empty, update tail */
    if (cloud_protocol_mqtt_client.msg_queue_head == NULL)
    {
        cloud_protocol_mqtt_client.msg_queue_tail = NULL;
    }

    /* Free the memory */
    Cloud_Protocol_Mqtt_FreePublishItem(oldest);

    cloud_protocol_mqtt_client.queue_size--;

    CLOUD_WARN("<%s> Dropped oldest message from queue. Queue size: %d\r\n", __func__, cloud_protocol_mqtt_client.queue_size);
}

/**
 * @brief Remove message from queue
 */
static void Cloud_Protocol_Mqtt_RemoveMessageFromQueue(cloud_protocol_mqtt_message_item_t *msg)
{
    if (msg == NULL || cloud_protocol_mqtt_client.msg_queue_head == NULL)
    {
        return;
    }

    cloud_protocol_mqtt_message_item_t *current = cloud_protocol_mqtt_client.msg_queue_head;
    cloud_protocol_mqtt_message_item_t *prev = NULL;

    /* Find the message to remove */
    while (current != NULL && current != msg)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        return; /* not found */
    }

    /* Remove from queue */
    if (prev == NULL)
    {
        cloud_protocol_mqtt_client.msg_queue_head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    if (current == cloud_protocol_mqtt_client.msg_queue_tail)
    {
        cloud_protocol_mqtt_client.msg_queue_tail = prev;
    }

    /* Update queue size */
    cloud_protocol_mqtt_client.queue_size--;

    /* Free memory */
    Cloud_Protocol_Mqtt_FreePublishItem(current);
}

/* Free a publish queue item */
static void Cloud_Protocol_Mqtt_FreePublishItem(cloud_protocol_mqtt_message_item_t *item)
{
    if (item != NULL)
    {
        CLOUDM_FREE(item->publish.topic);
        CLOUDM_FREE(item->publish.payload);
        CLOUDM_FREE(item->ack_topic);
        CLOUDM_FREE(item);
    }
}

/**
 * @brief Send publish message
 */
static bool Cloud_Protocol_Mqtt_SendPublishMessage(cloud_protocol_mqtt_message_item_t *msg)
{
    if (msg == NULL)
    {
        return false;
    }

    cloud_protocol_mqtt_publish_t publish_msg = {
        .topic = msg->publish.topic,
        .payload = msg->publish.payload,
        .topic_len = strlen(msg->publish.topic),
        .payload_len = strlen(msg->publish.payload)};

    CLOUD_INFO("<%s>: %s, ACK type: %d\r\n", __func__, publish_msg.topic, msg->ack_type);

    return Cloud_Protocol_Mqtt_SendPublishMsg(&publish_msg);
}

static void Cloud_Protocol_Mqtt_SendConnectMsg(void)
{
    /* Calculate message length: control word (2) */
    uint16_t msg_len = 2;
    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* Control word */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_CONNECT;

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static bool Cloud_Protocol_Mqtt_SendPublishMsg(const cloud_protocol_mqtt_publish_t *publish)
{
    /* Calculate message length */
    uint16_t msg_len = 2 + 2 + publish->topic_len + 2 + publish->payload_len;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_PUBLISH;

    /* topic */
    offset += Cloud_Protocol_Mqtt_PackString(&msg[offset], publish->topic, publish->topic_len);

    /* JSON payload */
    offset += Cloud_Protocol_Mqtt_PackString(&msg[offset], publish->payload, publish->payload_len);

    return Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendSubscribePublishMsg(const cloud_protocol_mqtt_subscribe_topic_t *subscribe, const cloud_protocol_mqtt_publish_topic_t *publish)
{
    /* Calculate the message length */
    uint16_t msg_len = 2 + 2 + subscribe->topic_len + 2 + publish->topic_len;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE_PUBLISH;

    /* SUB topic */
    offset += Cloud_Protocol_Mqtt_PackString(&msg[offset], subscribe->topic, subscribe->topic_len);

    /* PUB topic */
    offset += Cloud_Protocol_Mqtt_PackString(&msg[offset], publish->topic, publish->topic_len);

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);

    // CLOUD_INFO("<%s> msg:\r\n", __func__);
    // CLOUD_PRINT_HEX(&msg[2], offset - 2);
}

static uint16_t Cloud_Protocol_Mqtt_PackString(uint8_t *buffer, const char *str, uint16_t str_len)
{
    uint16_t offset = 0;

    buffer[offset] = (uint8_t)((str_len >> 8) & 0xFF);
    buffer[offset + 1] = (uint8_t)(str_len & 0xFF);
    offset += 2;
    if (str_len > 0)
    {
        memcpy(&buffer[offset], str, str_len);
        offset += str_len;
    }

    return offset;
}

/**
 * @brief Process a single message
 * @param msg message to process
 * @return true on success, false on failure
 */
static bool Cloud_Protocol_Mqtt_ProcessSingleMessage(cloud_protocol_mqtt_message_item_t *msg)
{
    if (msg == NULL)
    {
        return false;
    }

    /* Send message */
    bool sent = Cloud_Protocol_Mqtt_SendPublishMessage(msg);

    if (sent)
    {
        /* Update subscription state for messages that require ACK */
        if (msg->ack_type == CLOUD_PROTOCOL_MQTT_NEED_ACK)
        {
            cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK;
            cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
        }
        else
        {
            Cloud_Protocol_Mqtt_RemoveMessageFromQueue(msg);
        }
        CLOUD_INFO("<%s> Message sent successfully.\r\n", __func__);
        return true;
    }
    else
    {
        /* Send failed, wait for retry */
        CLOUD_ERROR("<%s> Failed to send message.\r\n", __func__);
        return false;
    }
}

/**
 * @brief Poll and process messages for the current subscribed topic
 * Process only one message per cycle; after iteration sets polling flags accordingly
 */
static void Cloud_Protocol_Mqtt_PollingCurrentSubscribe(void)
{
    /* If no message queue, return */
    if (cloud_protocol_mqtt_client.msg_queue_head == NULL)
    {
        cloud_protocol_mqtt_client.current_topic_is_polling = false;
        cloud_protocol_mqtt_client.last_processed_msg = NULL;
        return;
    }

    cloud_protocol_mqtt_message_item_t *current = NULL;

    /* Determine starting point for iteration */
    if (cloud_protocol_mqtt_client.last_processed_msg == NULL)
    {
        /* First iteration, start at queue head */
        current = cloud_protocol_mqtt_client.msg_queue_head;
    }
    else
    {
        /* Start from next of last processed */
        current = cloud_protocol_mqtt_client.last_processed_msg->next;
    }

    /* Iterate queue and find messages matching the current subscribe topic */
    while (current != NULL)
    {
        /* process active message */
        if (cloud_protocol_mqtt_client.current_subscribe_topic != NULL && cloud_protocol_mqtt_client.sub_state == CLOUD_PROTOCOL_SUB_STATE_ACTIVE)
        {
            /* Check if message requires ACK and publish topic matches current subscribe topic */
            if (current->ack_topic != NULL && strstr(cloud_protocol_mqtt_client.current_subscribe_topic, current->ack_topic) != NULL)
            {
                /* Found matching message, process it */
                bool processed = Cloud_Protocol_Mqtt_ProcessSingleMessage(current);

                if (processed)
                {
                    /* Update last processed pointer */
                    cloud_protocol_mqtt_client.last_processed_msg = current;

                    /* Set polling flag to indicate more messages may remain */
                    cloud_protocol_mqtt_client.current_topic_is_polling = true;
                    CLOUD_DEBUG("<%s>Processed one message for current topic, polling continues\r\n", __func__);
                    return;
                }
            }
        }
        /* process passive message */
        else if (current->ack_topic == NULL)
        {
            /* Message does not require ACK, direct processing */
            bool processed = Cloud_Protocol_Mqtt_ProcessSingleMessage(current);
            if (processed)
            {
                /* Set polling flag to indicate more messages may remain */
                cloud_protocol_mqtt_client.current_topic_is_polling = true;
                CLOUD_DEBUG("<%s>Processed one non-ACK message for current topic, polling continues\r\n", __func__);
                return;
            }
        }
        else
        {
            /* No match, continue to next */
        }
        current = current->next;
    }

    /* No matching messages found or iteration complete */
    cloud_protocol_mqtt_client.current_topic_is_polling = false;
    cloud_protocol_mqtt_client.last_processed_msg = NULL;

    // CLOUD_DEBUG("<%s>No more messages for current topic, stop current polling\r\n", __func__);
}

/**
 * @brief Process message queue
 */
static void Cloud_Protocol_Mqtt_ProcessMessageQueue(void)
{
    if (CLOUD_PROTOCOL_SUB_STATE_WAITING_ACK == cloud_protocol_mqtt_client.sub_state || 
        CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK == cloud_protocol_mqtt_client.sub_state)
    {
        return;
    }
    /* Process current subscribe messages */
    Cloud_Protocol_Mqtt_PollingCurrentSubscribe();

    /* If current topic messages finished and not waiting for message ACK, resume polling */
    if (cloud_protocol_mqtt_client.current_topic_is_polling == false)
    {
        cloud_protocol_mqtt_client.switch_default_topic_enabled = ENABLE;
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
        // CLOUD_DEBUG("<%s>Queue finished, enabling subscription polling\r\n", __func__);
    }
}

/**
 * @brief get passive topic configuration by name pattern
 */
static const cloud_protocol_mqtt_topic_config_t *Cloud_Protocol_Mqtt_GetPassiveTopicByEnum(cloud_protocol_mqtt_passive_topic_config_e topic_enum)
{
    return &cloud_protocol_mqtt_client.passive_topic_configs[topic_enum];
}

/**
 * @brief Build combined topic with passive topics
 * @param combined_topic output buffer for combined topic
 */
static void Cloud_Protocol_Mqtt_BuildPassiveTopic(char *combined_topic, size_t buffer_size)
{
    // Get passive topics
    const cloud_protocol_mqtt_topic_config_t *property_topic = Cloud_Protocol_Mqtt_GetPassiveTopicByEnum(CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_DEVICE_PROPERTY);
    const cloud_protocol_mqtt_topic_config_t *service_topic = Cloud_Protocol_Mqtt_GetPassiveTopicByEnum(CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_SERVICE_INVOCATION);
    char property_topic_buffer[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH] = {0};
    char service_topic_buffer[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH] = {0};

    if (property_topic)
    {
        Cloud_Protocol_Sg_Build_Topic(property_topic->subscribe_topic, NULL, property_topic_buffer, sizeof(property_topic_buffer));
        // CLOUD_INFO("Property topic: %s\r\n", property_topic_buffer);
    }

    if (!service_topic)
    {
        CLOUD_ERROR("Required passive topics not found\r\n");
        return;
    }
    else
    {
        Cloud_Protocol_Sg_Build_Topic(service_topic->subscribe_topic, NULL, service_topic_buffer, sizeof(service_topic_buffer));
        // CLOUD_INFO("Service topic: %s\r\n", service_topic_buffer);
    }

    // When there are no active messages: property set topic + service invoke topic (wildcard)
    if (property_topic == NULL)
    {
        snprintf(combined_topic, buffer_size, "%s", service_topic_buffer);
    }
    else
    {
        snprintf(combined_topic, buffer_size, "%s&%s", property_topic_buffer, service_topic_buffer);
    }
    // CLOUD_DEBUG("Combined topic (property + service): %s\r\n", combined_topic);
    
    return;
}

/**
 * @brief Build combined topic with active message topic
 * @param combined_topic output buffer for combined topic
 */
static void Cloud_Protocol_Mqtt_BuildActiveTopic(char *combined_topic, size_t buffer_size)
{
    // Get passive topics
    const cloud_protocol_mqtt_topic_config_t *service_topic = Cloud_Protocol_Mqtt_GetPassiveTopicByEnum(CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_SERVICE_INVOCATION);
    char service_topic_buffer[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH] = {0};

    if (!service_topic)
    {
        CLOUD_ERROR("Required passive topics not found\r\n");
        return;
    }
    else
    {
        Cloud_Protocol_Sg_Build_Topic(service_topic->subscribe_topic, NULL, service_topic_buffer, sizeof(service_topic_buffer));
        // CLOUD_INFO("Service topic: %s\r\n", service_topic_buffer);
    }

    // while having messages to send, subscribe to head message topics and service invoke topic
    if (NULL != cloud_protocol_mqtt_client.msg_queue_head->ack_topic)
    {
        snprintf(combined_topic, buffer_size, "%s&%s", cloud_protocol_mqtt_client.msg_queue_head->ack_topic, service_topic_buffer);
    }
    else
    {
        CLOUD_WARN("<%s> Active message has no ACK topic, using service topic only\r\n", __func__);
    }
    // CLOUD_DEBUG("Combined topic (active + service): %s\r\n", combined_topic);
}

/**
 * @brief Process polling subscribe with wildcard
 */
static void Cloud_Protocol_Mqtt_SwitchDefaultSubscribe(void)
{
    if (cloud_protocol_mqtt_client.switch_default_topic_enabled == DISABLE ||
        cloud_protocol_mqtt_client.sub_state != CLOUD_PROTOCOL_SUB_STATE_IDLE)
    {
        return;
    }

    // Build combined topic with wildcard
    char combined_topic[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH] = {0};
    uint8_t type = 0;
    if (cloud_protocol_mqtt_client.msg_queue_head != NULL && cloud_protocol_mqtt_client.msg_queue_head->ack_type == CLOUD_PROTOCOL_MQTT_NEED_ACK)
    {
        Cloud_Protocol_Mqtt_BuildActiveTopic(combined_topic, sizeof(combined_topic));
        type = 1;
    }
    else
    {
        Cloud_Protocol_Mqtt_BuildPassiveTopic(combined_topic, sizeof(combined_topic));
        type = 0;
    }

    if (strcmp(combined_topic, cloud_protocol_mqtt_client.current_subscribe_topic) == 0 || strlen(combined_topic) == 0)
    {
        // CLOUD_DEBUG("<%s> Combined topic unchanged, no need to switch subscription.\r\n", __func__);
        return;
    }

    CLOUD_DEBUG("<%s>%s Combined topic: %s\r\n", __func__, type == 1 ? "Active" : "Passive", combined_topic);

    cloud_protocol_mqtt_subscribe_topic_t subscribe_topic = {.topic = combined_topic,.topic_len = strlen(combined_topic)};
    cloud_protocol_mqtt_publish_topic_t publish_topic = {.topic = "", .topic_len = 0}; // No publish topic in polling subscribe

    Cloud_Protocol_Mqtt_SendSubscribePublishMsg(&subscribe_topic, &publish_topic);
    cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_WAITING_ACK;

    // Update subscribe state
    if (cloud_protocol_mqtt_client.current_subscribe_topic != NULL)
    {
        CLOUDM_FREE(cloud_protocol_mqtt_client.current_subscribe_topic);
    }
    cloud_protocol_mqtt_client.current_subscribe_topic = Cloud_Protocol_Strdup(combined_topic);
    cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
}

/**
 * @brief Check timeouts
 */
static void Cloud_Protocol_Mqtt_CheckTimeouts(void)
{
    uint64_t current_time = CLOUD_GET_TIME_MS();

    /* Check subscribe ACK timeout */
    if (cloud_protocol_mqtt_client.sub_state == CLOUD_PROTOCOL_SUB_STATE_WAITING_ACK)
    {
        if (current_time - cloud_protocol_mqtt_client.last_subscribe_time > CLOUD_PROTOCOL_MQTT_AT_RESPONSE_TIMEOUT_MS)
        {
            CLOUD_WARN("<%s> Subscribe ACK timeout\r\n", __func__);
            cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
        }
    }

    /* Check message ACK timeout */
    if (cloud_protocol_mqtt_client.sub_state == CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK)
    {
        if (current_time - cloud_protocol_mqtt_client.last_subscribe_time > CLOUD_PROTOCOL_MQTT_MSG_RESPONSE_TIMEOUT_MS)
        {
            // CLOUD_DEBUG("<%s> Message ACK wait timeout\r\n", __func__);
            if (cloud_protocol_mqtt_client.last_processed_msg != NULL)
            {
                CLOUD_WARN("<%s> Message ACK timeout for topic: %s\r\n", __func__, cloud_protocol_mqtt_client.last_processed_msg->publish.topic);
                if (NULL != cloud_protocol_mqtt_client.last_processed_msg)
                {
                    Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_client.last_processed_msg);
                    cloud_protocol_mqtt_client.last_processed_msg = NULL;
                }
                else
                {
                    cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
                }
            }
        }
    }
}

/**
 * @brief Handle message send failure
 */
static void Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_message_item_t *msg)
{
    if (msg == NULL)
    {
        return;
    }

    /* config for retry parameters */
    uint8_t max_retry = CLOUD_PROTOCOL_MQTT_DEFAULT_RETRY_COUNT; /* default max retry */

    if (msg->retry_count < max_retry)
    {
        msg->retry_count++;
        Cloud_Protocol_Mqtt_ProcessSingleMessage(msg);
        CLOUD_WARN("Message send failed, will retry %d/%d: %s\r\n", msg->retry_count, max_retry, msg->publish.topic);
    }
    else
    {
        /* Remove from queue */
        CLOUD_ERROR("Message send failed after %d retries, discarding: %s\r\n", max_retry, msg->publish.topic);
        Cloud_Protocol_Mqtt_RemoveMessageFromQueue(msg);
    }
}

/**
 * @brief Periodic processing function
 */
void Cloud_Protocol_Mqtt_ClientManagerProcess(void)
{
    /* Handle connection state */
    if (!cloud_protocol_mqtt_client.ctrl.ip_connected)
    {
        return;
    }
    else if (cloud_protocol_mqtt_client.ctrl.net_status == CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED)
    {
        Cloud_Protocol_Mqtt_StartReconnect();
    }
    else if (cloud_protocol_mqtt_client.ctrl.net_status == CLOUD_PROTOCOL_MQTT_STATE_CONNECTED)
    {
        /* Process default subscribe */
        Cloud_Protocol_Mqtt_SwitchDefaultSubscribe();
        /* Process message queue */
        Cloud_Protocol_Mqtt_ProcessMessageQueue();
        /* Check timeouts */
        Cloud_Protocol_Mqtt_CheckTimeouts();
    }
    else
    {
        // connecting state, do nothing
    }
}
/* EOL */
// ...existing code...
