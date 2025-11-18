//******************************************************************************
//* File Name: CloudNet_MqttM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: CloudNet Mqtt module mannager source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "CloudNet_MqttM.h"
#include "CloudNet_Cfg.h"

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
|    Global variables Declaration
|******************************************************************************/


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

bool CloudNetM_MqttConnect(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    cloud_protocol_mqtt_config_t config = {0};
    // Parse the payload and fill the config structure
    if (payload == NULL || length < CLOUD_PROTOCOL_MQTT_CONNECT_MINIMUM_LENGTH)
    {
        CLOUD_ERROR("Invalid parameters for MQTT connect parsing\r\n");
        return false;
    }
    uint16_t offset = 0;
    uint16_t client_id_len = 0;
    uint16_t username_len = 0;
    uint16_t password_len = 0;
    
    // parse ClientID
    client_id_len = (payload[offset] << 8) | payload[offset + 1];
    offset += 2;

    if (client_id_len > 0)
    {
        if (offset + client_id_len > length)
        {
            CLOUD_ERROR("ClientID length exceeds payload length\r\n");
            goto cleanup_memory;
        }

        config.client_id = CLOUDM_MALLOC(client_id_len + 1);
        if (config.client_id == NULL)
        {
            CLOUD_ERROR("Failed to allocate memory for ClientID\r\n");
            goto cleanup_memory;
        }
        memcpy(config.client_id, &payload[offset], client_id_len);
        config.client_id[client_id_len] = '\0';
        offset += client_id_len;
    }
    else
    {
        config.client_id = NULL;
    }

    // parse username
    username_len = (payload[offset] << 8) | payload[offset + 1];
    offset += 2;

    if (username_len > 0)
    {
        if (offset + username_len > length)
        {
            CLOUD_ERROR("Username length exceeds payload length\r\n");
            goto cleanup_memory;
        }

        config.username = CLOUDM_MALLOC(username_len + 1);
        if (config.username == NULL)
        {
            CLOUD_ERROR("Failed to allocate memory for username\r\n");
            goto cleanup_memory;
        }
        memcpy(config.username, &payload[offset], username_len);
        config.username[username_len] = '\0';
        offset += username_len;
    }
    else
    {
        config.username = NULL;
    }

    // parse password
    password_len = (payload[offset] << 8) | payload[offset + 1];
    offset += 2;

    if (password_len > 0)
    {
        if (offset + password_len > length)
        {
            CLOUD_ERROR("Password length exceeds payload length\r\n");
            goto cleanup_memory;
        }

        config.password = CLOUDM_MALLOC(password_len + 1);
        if (config.password == NULL)
        {
            CLOUD_ERROR("Failed to allocate memory for password\r\n");
            goto cleanup_memory;
        }
        memcpy(config.password, &payload[offset], password_len);
        config.password[password_len] = '\0';
        offset += password_len;
    }
    else
    {
        config.password = NULL;
    }

    // parse QoS
    if (offset >= length)
    {
        CLOUD_ERROR("Payload too short for QoS field\r\n");
        goto cleanup_memory;
    }
    config.qos = payload[offset++];

    // parse keep alive
    if (offset + 1 >= length)
    {
        CLOUD_ERROR("Payload too short for keep alive field\r\n");
        goto cleanup_memory;
    }
    config.keep_alive = (payload[offset] << 8) | payload[offset + 1];
    offset += 2;

    // Parse clean session
    if (offset >= length)
    {
        CLOUD_ERROR("Payload too short for clean session field\r\n");
        goto cleanup_memory;
    }
    config.clean_session = payload[offset];

    // CLOUD_DEBUG("MQTT Connect payload parsed successfully\r\n");
    // CLOUD_DEBUG("ClientID: %s\r\n", config.client_id ? config.client_id : "NULL");
    // CLOUD_DEBUG("Username: %s\r\n", config.username ? config.username : "NULL");
    // CLOUD_DEBUG("Password: %s\r\n", config.password ? "***" : "NULL");
    // CLOUD_DEBUG("QoS: %d, KeepAlive: %d, CleanSession: %d\r\n",
                // config.qos, config.keep_alive, config.clean_session);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_MQSET, NULL, socket_id,
                      config.client_id ? config.client_id : "",
                      config.username ? config.username : "",
                      config.password ? config.password : "");
    if (config.client_id)
    {
        CLOUDM_FREE(config.client_id);
        config.client_id = NULL;
    }
    if (config.username)
    {
        CLOUDM_FREE(config.username);
        config.username = NULL;
    }
    if (config.password)
    {
        CLOUDM_FREE(config.password);
        config.password = NULL;
    }

    return true;

cleanup_memory:
    // Clear the allocated memory
    if (config.client_id)
    {
        CLOUDM_FREE(config.client_id);
        config.client_id = NULL;
    }
    if (config.username)
    {
        CLOUDM_FREE(config.username);
        config.username = NULL;
    }
    if (config.password)
    {
        CLOUDM_FREE(config.password);
        config.password = NULL;
    }
    return false;
}

bool CloudNetM_MqttDisconnect(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}

bool CloudNetM_MqttPublish(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    cloud_protocol_mqtt_publish_t publish = {0};

    // Parameter checking
    if (payload == NULL || length < CLOUD_PROTOCOL_MQTT_CONNECT_MINIMUM_LENGTH)
    {
        CLOUD_ERROR("Invalid parameters for MQTT connect parsing\r\n");
        return false;
    }

    // parse payload
    uint16_t offset = 0;

    // parse topic
    offset = CloudNet_Protocol_Mqtt_UnpackString(payload, offset, publish.topic);
    if (publish.topic == NULL)
    {
        CLOUD_ERROR("Topic parse failed\r\n");
        return false;
    }

    // parse payload
    offset = CloudNet_Protocol_Mqtt_UnpackJsonPayload(payload, offset, publish.payload);
    if (publish.payload == NULL)
    {
        CLOUD_ERROR("Payload parse failed\r\n");
        CLOUDM_FREE(publish.topic);
        return false;
    }

    // parse retain flag
    if (offset < length)
    {
        publish.retain = (payload[offset] == 0x01);
    }

    // Add to the push queue
    bool result = CloudNetM_MqttAddMessageToQueue(publish.topic, publish.payload, publish.retain, 1); // default QoS=1

    // Clean up temporary memory
    CLOUDM_FREE(publish.topic);
    CLOUDM_FREE(publish.payload);

    if (result)
    {
        CLOUD_INFO("MQTT publish message added to queue successfully\r\n");
    }
    else
    {
        CLOUD_ERROR("Failed to add MQTT publish message to queue\r\n");
    }

    return result;
}

bool CloudNetM_MqttSubscribe(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}

bool CloudNetM_MqttUnsubscribe(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}

bool CloudNetM_MqttSubscribePublish(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
	if (payload == NULL)
	{
		CLOUD_ERROR("<%s>Payload is NULL\r\n", __func__);
		return false;
	}

	if (length < CLOUD_PROTOCOL_MQTT_SUB_PUB_MINIMUM_LENGTH)
	{
		CLOUD_ERROR("<%s>Payload too short: %d < %d\r\n", __func__, length, CLOUD_PROTOCOL_MQTT_SUB_PUB_MINIMUM_LENGTH);
		return false;
	}

	// CLOUD_DEBUG("Processing MQTT subscribe-publish, socket: %d, length: %d\r\n", socket_id, length);

	uint16_t offset = 0;

	char *subscribe_topic = NULL;
	uint16_t prev_offset = offset;
	offset = CloudNet_Protocol_Mqtt_UnpackString(payload, offset, subscribe_topic);

	if (subscribe_topic == NULL)
	{
		CLOUD_ERROR("<%s>Failed to parse subscribe topic at offset: %d\r\n", __func__, prev_offset);
		return false;
	}

	// CLOUD_DEBUG("<%s>Subscribe topic parsed: %s\r\n", __func__, subscribe_topic);

	char *publish_topic = NULL;
	prev_offset = offset;
	offset = CloudNet_Protocol_Mqtt_UnpackString(payload, offset, publish_topic);

	if (publish_topic == NULL)
	{
		CLOUD_ERROR("Failed to parse publish topic at offset: %d\r\n", prev_offset);
		CLOUDM_FREE(subscribe_topic);
		return false;
	}

	// CLOUD_DEBUG("<%s>Publish topic parsed: %s\r\n", __func__, publish_topic);

	if (offset != length)
	{
		CLOUD_WARN("<%s>Payload length mismatch: parsed %d, expected %d\r\n", __func__, offset, length);
	}

	CLOUD_INFO("<%s>Subscribe topic: %s, Publish topic: %s\r\n", __func__, subscribe_topic, publish_topic);

	bool queue_result = YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_MQTOP, NULL,	socket_id, subscribe_topic, publish_topic);

	CLOUDM_FREE(subscribe_topic);
	CLOUDM_FREE(publish_topic);

	if (queue_result == 0)
	{
		CLOUD_INFO("<%s>Subscribe-publish message processed successfully, socket: %d\r\n", __func__, socket_id);
	}
	else
	{
		CLOUD_ERROR("<%s>Failed to process subscribe-publish message, socket: %d\r\n", __func__, socket_id);
	}
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_MQTOP, NULL, socket_id);

	return queue_result == 0 ? true : false;
}

bool CloudNetM_MqttSetWill(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}
/* EOL */
