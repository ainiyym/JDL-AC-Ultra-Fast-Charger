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
    cloud_protocol_mqtt_state_e state;
    bool is_initialized;
    uint8_t reconnect_count;
    uint32_t last_connect_time;
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
    cloud_protocol_mqtt_topic_config_t *topic_configs; /* topic config array */
    uint16_t topic_count;			   /* number of topics */

    /* message queue */
    cloud_protocol_mqtt_message_item_t *msg_queue_head; /* queue head */
    cloud_protocol_mqtt_message_item_t *msg_queue_tail; /* queue tail */
    uint16_t queue_size;				 /* current queue size */
    uint16_t max_queue_size;			 /* maximum queue size */

    /* subscription management */
    char *current_subscribe_topic;	 /* current subscribe topic */
    cloud_protocol_mqtt_subscribe_state_e sub_state;		 /* subscription state */
    uint32_t last_subscribe_time;	 /* last subscribe time */
    uint32_t subscribe_switch_interval; /* subscribe switch interval (ms) */

    /* polling subscribe */
    bool polling_enabled;				  /* polling enabled */
    bool current_topic_polling;			  /* current topic is polling */
    uint16_t current_poll_index;		  /* current poll index */
    cloud_protocol_mqtt_message_item_t *last_processed_msg; /* last processed message (for iteration) */

    /* callbacks */
    void (*connect_callback)(bool connected);
    void (*message_callback)(const char *topic, const char *payload);
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
static cloud_protocol_mqtt_config_t cloud_protocol_mqtt_config;
static mqtt_client_manager_t cloud_protocol_mqtt_client;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_Mqtt_StartReconnect(void);
static void Cloud_Protocol_Mqtt_StartConnection(void);
static void Cloud_Protocol_Mqtt_RemoveMessageFromQueue(cloud_protocol_mqtt_message_item_t *msg);
static void Cloud_Protocol_Mqtt_FreePublishItem(cloud_protocol_mqtt_message_item_t *item);
static void Cloud_Protocol_Mqtt_SendConnectMsg(void);
static void Cloud_Protocol_Mqtt_SendPublishMsg(const cloud_protocol_mqtt_publish_t *publish);
static bool Cloud_Protocol_Mqtt_SendPublishMessage(cloud_protocol_mqtt_message_item_t *msg);
static void Cloud_Protocol_Mqtt_SendSubscribeMsg(const char *topic);
static void Cloud_Protocol_Mqtt_SendUnsubscribeMsg(const char *topic);
static void Cloud_Protocol_Mqtt_SendSubscribePublishMsg(const cloud_protocol_mqtt_subscribe_topic_t *subscribe, const cloud_protocol_mqtt_publish_topic_t *publish);
static void Cloud_Protocol_Mqtt_SendSetWillMsg(const char *topic, const char *message, uint8_t qos, bool retain);
static uint16_t Cloud_Protocol_Mqtt_PackString(uint8_t *buffer, uint16_t offset, const char *str);
static uint16_t Cloud_Protocol_Mqtt_PackJsonPayload(uint8_t *buffer, uint16_t offset, const char *json_payload);
static bool Cloud_Protocol_Mqtt_ProcessSingleMessage(cloud_protocol_mqtt_message_item_t *msg);
static void Cloud_Protocol_Mqtt_PollingCurrentSubscribe(void);
static void Cloud_Protocol_Mqtt_ProcessMessageQueue(void);
static void Cloud_Protocol_Mqtt_ProcessPollingSubscribe(void);
static void Cloud_Protocol_Mqtt_CheckTimeouts(void);
static void Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_message_item_t *msg);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Initialize MQTT client manager
 */
bool Cloud_Protocol_Mqtt_ClientManagerInit(cloud_protocol_mqtt_topic_config_t *topic_configs, uint16_t topic_count,
                            void (*connect_cb)(bool connected),
                            void (*msg_cb)(const char *topic, const char *payload))
{
    if (topic_configs == NULL || topic_count == 0)
    {
        return false;
    }

    memset(&cloud_protocol_mqtt_client, 0, sizeof(cloud_protocol_mqtt_client));
    memset(&cloud_protocol_mqtt_config, 0, sizeof(cloud_protocol_mqtt_config));

    cloud_protocol_mqtt_client.topic_configs = topic_configs;
    cloud_protocol_mqtt_client.topic_count = topic_count;
    cloud_protocol_mqtt_client.max_queue_size = CLOUD_PROTOCOL_MQTT_MAX_QUEUE_SIZE;
    cloud_protocol_mqtt_client.subscribe_switch_interval = CLOUD_PROTOCOL_MQTT_PROCESS_INTERVAL_MS; /* subscribe switch interval (ms) */
    cloud_protocol_mqtt_client.polling_enabled = ENABLE;	/* enable polling subscribe */

    cloud_protocol_mqtt_client.connect_callback = connect_cb;
    cloud_protocol_mqtt_client.message_callback = msg_cb;

    cloud_protocol_mqtt_client.ctrl.state = CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED;

    cloud_protocol_mqtt_config = cloud_sg_mqtt_default_config;

    if (cloud_protocol_mqtt_config.client_id == NULL)
    {
        return false;
    }
    cloud_protocol_mqtt_client.ctrl.is_initialized = true;

    CLOUD_INFO("MQTT client manager initialized with %d topics\r\n", topic_count);
    return true;
}

/**
 * @brief Handle received message (server request)
 * @param topic message topic
 * @param payload message payload
 */
void Cloud_Protocol_Mqtt_HandleReceivedMessage(const char *topic, const char *payload)
{
    if (topic == NULL || payload == NULL)
    {
        return;
    }

    CLOUD_INFO("Received message: %s -> %s\r\n", topic, payload);

    /* Call message callback */
    if (cloud_protocol_mqtt_client.message_callback != NULL)
    {
        cloud_protocol_mqtt_client.message_callback(topic, payload);
    }
}

/**
 * @brief Handle connection established event
 */
void Cloud_Protocol_Mqtt_HandleConnected(void)
{
    CLOUD_INFO("MQTT connected successfully\r\n");

    cloud_protocol_mqtt_client.ctrl.state = CLOUD_PROTOCOL_MQTT_STATE_CONNECTED;
    cloud_protocol_mqtt_client.ctrl.reconnect_count = 0;

    /* Call connect callback */
    if (cloud_protocol_mqtt_client.connect_callback != NULL)
    {
        cloud_protocol_mqtt_client.connect_callback(true);
    }

    /* Initialize polling state */
    cloud_protocol_mqtt_client.polling_enabled = ENABLE;
    cloud_protocol_mqtt_client.current_poll_index = 0;
    cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;

    CLOUD_DEBUG("<%s> Connected, starting in polling mode\r\n", __func__);
}

/**
 * @brief Handle disconnection event
 */
void Cloud_Protocol_Mqtt_HandleDisconnected(void)
{
    CLOUD_WARN("MQTT disconnected\r\n");

    cloud_protocol_mqtt_client.ctrl.state = CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED;
    cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
    cloud_protocol_mqtt_client.polling_enabled = DISABLE;

    /* Clean up current subscribe topic */
    if (cloud_protocol_mqtt_client.current_subscribe_topic != NULL)
    {
        CLOUDM_FREE(cloud_protocol_mqtt_client.current_subscribe_topic);
        cloud_protocol_mqtt_client.current_subscribe_topic = NULL;
    }

    /* Call connect callback */
    if (cloud_protocol_mqtt_client.connect_callback != NULL)
    {
        cloud_protocol_mqtt_client.connect_callback(false);
    }

    /* Start reconnect */
    Cloud_Protocol_Mqtt_StartReconnect();
}

/**
 * @brief Handle subscribe acknowledgment
 * @param subscribe_topic subscribed topic
 * @param publish_topic publish topic
 * @param success whether subscription succeeded
 */
void Cloud_Protocol_Mqtt_HandleSubscribeAck(const char *subscribe_topic, const char *publish_topic, bool success)
{
    if (subscribe_topic == NULL)
    {
        return;
    }

    /* Validate ACK belongs to current subscribe topic */
    if (cloud_protocol_mqtt_client.current_subscribe_topic == NULL || strcmp(cloud_protocol_mqtt_client.current_subscribe_topic, subscribe_topic) != 0)
    {
        CLOUD_WARN("Subscribe ACK for unexpected topic: %s, current: %s\r\n", subscribe_topic, cloud_protocol_mqtt_client.current_subscribe_topic);
        return;
    }

    if (success)
    {
        CLOUD_INFO("Subscribe ACK success: %s\r\n", subscribe_topic);
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_ACTIVE;
        cloud_protocol_mqtt_client.current_topic_polling = true;
        cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
    }
    else
    {
        CLOUD_ERROR("Subscribe ACK failed: %s\r\n", subscribe_topic);
        cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_IDLE;
    }
}

/**
 * @brief Start reconnect attempts
 */
static void Cloud_Protocol_Mqtt_StartReconnect(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();
    uint32_t reconnect_interval = CLOUD_PROTOCOL_MQTT_RECONNECT_INTERVAL_MS; /* reconnect interval */

    if (current_time - cloud_protocol_mqtt_client.ctrl.last_connect_time < reconnect_interval)
    {
        return;
    }

    if (cloud_protocol_mqtt_client.ctrl.reconnect_count < 10)
    {
        cloud_protocol_mqtt_client.ctrl.reconnect_count++;
        CLOUD_INFO("Attempting reconnect %d...\r\n", cloud_protocol_mqtt_client.ctrl.reconnect_count);

        Cloud_Protocol_Mqtt_StartConnection();
    }
    else
    {
        CLOUD_ERROR("Max reconnect attempts reached\r\n");
    }
}

/**
 * @brief Start connection
 */
static void Cloud_Protocol_Mqtt_StartConnection(void)
{
    if (!cloud_protocol_mqtt_client.ctrl.is_initialized)
    {
        return;
    }

    if (cloud_protocol_mqtt_client.ctrl.state == CLOUD_PROTOCOL_MQTT_STATE_CONNECTED ||
        cloud_protocol_mqtt_client.ctrl.state == CLOUD_PROTOCOL_MQTT_STATE_CONNECTING)
    {
        return;
    }

    CLOUD_INFO("Starting MQTT connection...\r\n");
    cloud_protocol_mqtt_client.ctrl.state = CLOUD_PROTOCOL_MQTT_STATE_CONNECTING;
    cloud_protocol_mqtt_client.ctrl.last_connect_time = CLOUD_GET_TIME_MS();

    Cloud_Protocol_Mqtt_SendConnectMsg();
}

/**
 * @brief Add publish message to queue
 * @param topic publish topic
 * @param payload message payload
 * @param retain retain flag
 * @param ack_type acknowledgment type
 * @param ack_topic ack topic (for messages requiring ack)
 * @return true on success, false on failure
 */
bool Cloud_Protocol_Mqtt_AddPublishMessage(const char *topic, const char *payload, bool retain, cloud_protocol_mqtt_msg_ack_type_e ack_type, const char *ack_topic)
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

    /* Create new message item */
    cloud_protocol_mqtt_message_item_t *new_msg = (cloud_protocol_mqtt_message_item_t *)CLOUDM_MALLOC(sizeof(cloud_protocol_mqtt_message_item_t));
    if (new_msg == NULL)
    {
        return false;
    }

    new_msg->publish.topic = Cloud_Protocol_Strdup(topic);
    new_msg->publish.payload = Cloud_Protocol_Strdup(payload);
    new_msg->publish.retain = retain;
    new_msg->ack_type = ack_type;
    new_msg->ack_topic = (ack_topic != NULL) ? Cloud_Protocol_Strdup(ack_topic) : NULL;
    new_msg->timestamp = CLOUD_GET_TIME_MS();
    new_msg->retry_count = 0;
    new_msg->next = NULL;

    /* Check memory allocation */
    if (new_msg->publish.topic == NULL || new_msg->publish.payload == NULL || (ack_topic != NULL && new_msg->ack_topic == NULL))
    {
        Cloud_Protocol_Mqtt_FreePublishItem(new_msg);
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

    CLOUD_INFO("Message added to queue: %s, ACK type: %d, queue size: %d\r\n", topic, ack_type, cloud_protocol_mqtt_client.queue_size);

    return true;
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
        .retain = msg->publish.retain};

    Cloud_Protocol_Mqtt_SendPublishMsg(&publish_msg);

    CLOUD_INFO("Message published: %s, ACK type: %d\r\n", publish_msg.topic, msg->ack_type);
    return true;
}

static void Cloud_Protocol_Mqtt_SendConnectMsg(void)
{
    /* Calculate message length: control word (2) + client ID length (2+strlen) + username length (2+strlen) + password length (2+strlen) + qos (1) + keep alive (2) + clean session flag (1) */
    uint16_t msg_len = 2 + 2 + (cloud_protocol_mqtt_config.client_id ? strlen(cloud_protocol_mqtt_config.client_id) :0) + 
                       2 + (cloud_protocol_mqtt_config.username ? strlen(cloud_protocol_mqtt_config.username) : 0) +
                       2 + (cloud_protocol_mqtt_config.password ? strlen(cloud_protocol_mqtt_config.password) : 0) +
                       1 + 2 + 1;
    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* Control word */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_CONNECT;

    /* Client ID */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, cloud_protocol_mqtt_config.client_id);

    /* Username */
    if (cloud_protocol_mqtt_config.username)
    {
        offset = Cloud_Protocol_Mqtt_PackString(msg, offset, cloud_protocol_mqtt_config.username);
    }
    else
    {
        msg[offset++] = 0x00;
        msg[offset++] = 0x00;
    }

    /* Password */
    if (cloud_protocol_mqtt_config.password)
    {
        offset = Cloud_Protocol_Mqtt_PackString(msg, offset, cloud_protocol_mqtt_config.password);
    }
    else
    {
        msg[offset++] = 0x00;
        msg[offset++] = 0x00;
    }

    /* QoS */
    msg[offset++] = cloud_protocol_mqtt_config.qos;

    /* Keep alive */
    msg[offset++] = (uint8_t)((cloud_protocol_mqtt_config.keep_alive >> 8) & 0xFF);
    msg[offset++] = (uint8_t)(cloud_protocol_mqtt_config.keep_alive & 0xFF);

    /* Clean session flag */
    msg[offset++] = cloud_protocol_mqtt_config.clean_session ? 0x01 : 0x00;

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendPublishMsg(const cloud_protocol_mqtt_publish_t *publish)
{
    /* Calculate message length: control word + topic length + topic content + payload length + payload content + Retain */
    uint16_t topic_len = strlen(publish->topic);
    uint16_t payload_len = strlen(publish->payload);
    uint16_t msg_len = 2 + 2 + topic_len + 2 + payload_len + 1;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_PUBLISH;

    /* topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, publish->topic);

    /* JSON payload */
    offset = Cloud_Protocol_Mqtt_PackJsonPayload(msg, offset, publish->payload);

    /* Retain flag */
    msg[offset++] = publish->retain ? 0x01 : 0x00;

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendSubscribeMsg(const char *topic)
{
    /* Calculate message length: control cmd + topic length + topic content */
    uint16_t topic_len = strlen(topic);
    uint16_t msg_len = 2 + 2 + topic_len;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE;

    /* topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, topic);

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendUnsubscribeMsg(const char *topic)
{
    /* Calculate message length: control word + topic length + topic content */
    uint16_t topic_len = strlen(topic);
    uint16_t msg_len = 2 + 2 + topic_len;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_UNSUBSCRIBE;

    /* topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, topic);

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendSubscribePublishMsg(const cloud_protocol_mqtt_subscribe_topic_t *subscribe, const cloud_protocol_mqtt_publish_topic_t *publish)
{
    /* Calculate the message length: control cmd + subscribe topic length + subscribe topic + publish topic length + publish topic */
    uint16_t sub_topic_len = strlen(subscribe->topic);
    uint16_t pub_topic_len = strlen(publish->topic);
    uint16_t msg_len = 2 + 2 + sub_topic_len + 2 + pub_topic_len;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE_PUBLISH;

    /* SUB topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, subscribe->topic);

    /* PUB topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, publish->topic);

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static void Cloud_Protocol_Mqtt_SendSetWillMsg(const char *topic, const char *message, uint8_t qos, bool retain)
{
    /* Calculate the message length: control cmd + topic length + topic + message length + message + QoS + Retain */
    uint16_t topic_len = strlen(topic);
    uint16_t message_len = strlen(message);
    uint16_t msg_len = 2 + 2 + topic_len + 2 + message_len + 1 + 1;

    uint8_t msg[msg_len];
    uint16_t offset = 0;

    /* ctrl cmd */
    msg[offset++] = (uint8_t)CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
    msg[offset++] = (uint8_t)CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SET_WILL;

    /* topic */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, topic);

    /* message */
    offset = Cloud_Protocol_Mqtt_PackString(msg, offset, message);

    /* QoS */
    msg[offset++] = qos;

    /* Retain flag */
    msg[offset++] = retain ? 0x01 : 0x00;

    Cloud_Protocol_SendMsg(msg, offset, CLOUD_MESSAGE_TYPE_CTRL);
}

static uint16_t Cloud_Protocol_Mqtt_PackString(uint8_t *buffer, uint16_t offset, const char *str)
{
    uint16_t len = strlen(str);
    buffer[offset++] = (uint8_t)((len >> 8) & 0xFF);
    buffer[offset++] = (uint8_t)(len & 0xFF);
    memcpy(&buffer[offset], str, len);
    return offset + len;
}

static uint16_t Cloud_Protocol_Mqtt_PackJsonPayload(uint8_t *buffer, uint16_t offset, const char *json_payload)
{
    uint16_t len = strlen(json_payload);
    buffer[offset++] = (uint8_t)((len >> 8) & 0xFF);
    buffer[offset++] = (uint8_t)(len & 0xFF);
    memcpy(&buffer[offset], json_payload, len);
    return offset + len;
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
        /* Remove message from queue */
        Cloud_Protocol_Mqtt_RemoveMessageFromQueue(msg);

        /* Update subscription state for messages that require ACK */
        if (msg->ack_type == CLOUD_PROTOCOL_MQTT_NEED_ACK)
        {
            cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK;
            cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
        }

        CLOUD_INFO("<%s> Message processed and removed: %s\r\n", __func__, msg->publish.topic);
        return true;
    }
    else
    {
        /* Send failed, handle retry */
        Cloud_Protocol_Mqtt_HandleMessageSendFail(msg);
        return false;
    }
}

/**
 * @brief Poll and process messages for the current subscribed topic
 * Process only one message per cycle; after iteration sets polling flags accordingly
 */
static void Cloud_Protocol_Mqtt_PollingCurrentSubscribe(void)
{
    if (cloud_protocol_mqtt_client.ctrl.state != CLOUD_PROTOCOL_MQTT_STATE_CONNECTED || cloud_protocol_mqtt_client.current_subscribe_topic == NULL || cloud_protocol_mqtt_client.sub_state != CLOUD_PROTOCOL_SUB_STATE_ACTIVE)
    {
        cloud_protocol_mqtt_client.current_topic_polling = false;
        return;
    }

    /* If no message queue, return */
    if (cloud_protocol_mqtt_client.msg_queue_head == NULL)
    {
        cloud_protocol_mqtt_client.current_topic_polling = false;
        cloud_protocol_mqtt_client.last_processed_msg = NULL;
        return;
    }

    cloud_protocol_mqtt_message_item_t *current = NULL;
    cloud_protocol_mqtt_message_item_t *prev = NULL;

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
        prev = cloud_protocol_mqtt_client.last_processed_msg;
    }

    /* Iterate queue and find messages matching the current subscribe topic */
    while (current != NULL)
    {
        /* Check if message requires ACK and publish topic matches current subscribe topic */
        if (current->ack_topic != NULL && cloud_protocol_mqtt_client.current_subscribe_topic != NULL &&
            strcmp(cloud_protocol_mqtt_client.current_subscribe_topic, current->ack_topic) == 0)
        {

            /* Found matching message, process it */
            bool processed = Cloud_Protocol_Mqtt_ProcessSingleMessage(current);

            if (processed)
            {
                /* Update last processed pointer */
                if (current->next != NULL)
                {
                    cloud_protocol_mqtt_client.last_processed_msg = current;
                }
                else
                {
                    cloud_protocol_mqtt_client.last_processed_msg = NULL; /* last one */
                }

                /* Set polling flag to indicate more messages may remain */
                cloud_protocol_mqtt_client.current_topic_polling = true;
                CLOUD_DEBUG("Processed one message for current topic, polling continues\r\n");
                return;
            }
            else
            {
                /* Processing failed, continue to next */
                CLOUD_DEBUG("Message processing failed, continue to next\r\n");
            }
        }

        prev = current;
        current = current->next;
    }

    /* No matching messages found or iteration complete */
    cloud_protocol_mqtt_client.current_topic_polling = false;
    cloud_protocol_mqtt_client.last_processed_msg = NULL;
    cloud_protocol_mqtt_client.polling_enabled = ENABLE;

    CLOUD_DEBUG("No more messages for current topic, switching back to polling mode\r\n");
}

/**
 * @brief Process message queue
 */
static void Cloud_Protocol_Mqtt_ProcessMessageQueue(void)
{
    if (cloud_protocol_mqtt_client.ctrl.state != CLOUD_PROTOCOL_MQTT_STATE_CONNECTED)
    {
        return;
    }

    /* Process current subscribe messages */
    Cloud_Protocol_Mqtt_PollingCurrentSubscribe();

    /* If current topic messages finished and not waiting for message ACK, resume polling */
    if (cloud_protocol_mqtt_client.current_topic_polling == false && cloud_protocol_mqtt_client.sub_state != CLOUD_PROTOCOL_SUB_STATE_WAITING_MSG_ACK)
    {
        cloud_protocol_mqtt_client.polling_enabled = ENABLE;
        CLOUD_DEBUG("Queue finished, enabling subscription polling\r\n");
    }
}

/**
 * @brief Process polling subscribe
 */
static void Cloud_Protocol_Mqtt_ProcessPollingSubscribe(void)
{
    if (!cloud_protocol_mqtt_client.polling_enabled || cloud_protocol_mqtt_client.ctrl.state != CLOUD_PROTOCOL_MQTT_STATE_CONNECTED)
    {
        return;
    }

    uint32_t current_time = CLOUD_GET_TIME_MS();

    /* Check switch interval */
    if (current_time - cloud_protocol_mqtt_client.last_subscribe_time < cloud_protocol_mqtt_client.subscribe_switch_interval)
    {
        return;
    }

    /* Select next topic to subscribe */
    if (cloud_protocol_mqtt_client.topic_count == 0)
    {
        return;
    }

    /* Advance to next topic */
    cloud_protocol_mqtt_client.current_poll_index = (cloud_protocol_mqtt_client.current_poll_index + 1) % cloud_protocol_mqtt_client.topic_count;
    cloud_protocol_mqtt_topic_config_t *next_topic = &cloud_protocol_mqtt_client.topic_configs[cloud_protocol_mqtt_client.current_poll_index];

    /* Send subscribe request */
    CLOUD_DEBUG("Polling subscribe: %s\r\n", next_topic->subscribe_topic);

    cloud_protocol_mqtt_subscribe_topic_t subscribe_topic = {.topic = next_topic->subscribe_topic};
    cloud_protocol_mqtt_publish_topic_t publish_topic = {.topic = next_topic->publish_topic};

    Cloud_Protocol_Mqtt_SendSubscribePublishMsg(&subscribe_topic, &publish_topic);

    /* Update subscribe state */
    if (cloud_protocol_mqtt_client.current_subscribe_topic != NULL)
    {
        CLOUDM_FREE(cloud_protocol_mqtt_client.current_subscribe_topic);
    }
    cloud_protocol_mqtt_client.current_subscribe_topic = Cloud_Protocol_Strdup(next_topic->subscribe_topic);
    cloud_protocol_mqtt_client.sub_state = CLOUD_PROTOCOL_SUB_STATE_WAITING_ACK;
    cloud_protocol_mqtt_client.last_subscribe_time = CLOUD_GET_TIME_MS();
}

/**
 * @brief Check timeouts
 */
static void Cloud_Protocol_Mqtt_CheckTimeouts(void)
{
    uint32_t current_time = CLOUD_GET_TIME_MS();

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
            CLOUD_DEBUG("<%s> Message ACK wait timeout\r\n", __func__);
            Cloud_Protocol_Mqtt_HandleMessageSendFail(cloud_protocol_mqtt_client.last_processed_msg);
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

    /* Find topic config for retry parameters */
    uint8_t max_retry = CLOUD_PROTOCOL_MQTT_RETRY_COUNT;
    for (uint16_t i = 0; i < cloud_protocol_mqtt_client.topic_count; i++)
    {
        if (strcmp(cloud_protocol_mqtt_client.topic_configs[i].publish_topic, msg->publish.topic) == 0)
        {
            max_retry = cloud_protocol_mqtt_client.topic_configs[i].max_retry_count;
            break;
        }
    }

    if (msg->retry_count < max_retry)
    {
        msg->retry_count++;
        CLOUD_WARN("Message send failed, will retry %d/%d: %s\r\n", msg->retry_count, max_retry, msg->publish.topic);
    }
    else
    {
        /* Remove from queue */
        Cloud_Protocol_Mqtt_RemoveMessageFromQueue(msg);
        CLOUD_ERROR("Message send failed after %d retries, discarding: %s\r\n", max_retry, msg->publish.topic);
    }
}

/**
 * @brief Periodic processing function
 */
void Cloud_Protocol_Mqtt_ClientManagerProcess(void)
{
    /* Handle connection state */
    if (cloud_protocol_mqtt_client.ctrl.state == CLOUD_PROTOCOL_MQTT_STATE_DISCONNECTED)
    {
        Cloud_Protocol_Mqtt_StartReconnect();
    }
    /* Process message queue */
    Cloud_Protocol_Mqtt_ProcessMessageQueue();
    
    /* Process polling subscribe */
    Cloud_Protocol_Mqtt_ProcessPollingSubscribe();

    /* Check timeouts */
    Cloud_Protocol_Mqtt_CheckTimeouts();
}
/* EOL */
// ...existing code...
