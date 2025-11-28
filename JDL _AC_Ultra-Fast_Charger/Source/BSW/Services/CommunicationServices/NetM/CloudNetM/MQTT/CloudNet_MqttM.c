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
#include "CloudNet_Protocol_Msg.h"
#include "Cloud_Protocol_Mqtt.h"
#include "CloudNet_Cfg.h"
#include "Tcp_Cfg.h"

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
static bool CloudNetM_MqttSendAtTopic(const char *at_parameter, uint8_t context);
static bool CloudNetM_MqttSendAtPayload(const char *at_parameter, uint8_t context);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
static bool CloudNetM_MqttSendAtTopic(const char *at_parameter, uint8_t context)
{
    uint8_t ret = 0;

    ret = YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_PUBTOP, NULL, context, at_parameter);
    if (0 == ret)
    {
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_PUBTOP, NULL, context);
    }
    return ret == 0 ? true : false;
}

static bool CloudNetM_MqttSendAtPayload(const char *at_parameter, uint8_t context)
{
    uint8_t ret = 0;

    ret = YeeCom_At_DataPassthrougth(context, (const uint8_t *)at_parameter, (uint16_t)strlen(at_parameter) + 1);

    if (0 == ret)
    {
        CloudNetM_MqttHandleATPayloadSendSuccess();
    }
    return ret == 0 ? true : false;
}

bool CloudNetM_MqttConnect(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    // Parse the payload and fill the config structure
    if (payload == NULL)
    {
        CLOUDNET_ERROR("Invalid parameters for MQTT connect parsing\r\n");
        return false;
    }
    iotx_sign_mqtt_t config;
    Cloud_Protocol_Mqtt_GetClientConfig(&config);
    CLOUDNET_DEBUG("ClientID: %s\r\n", config.clientid);
    CLOUDNET_DEBUG("Username: %s\r\n", config.username);
    CLOUDNET_DEBUG("Password: %s\r\n", config.password);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_DTUID, NULL, config.clientid);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_MQSET, NULL, socket_id,
                      "{DTUID}",
                      config.username,
                      config.password);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_DTUID, NULL);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_MQSET, NULL, socket_id);
    // vTaskDelay(pdMS_TO_TICKS(100)); // Wait for 100 ms for the settings to take effect
    // YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_GSTATE, NULL);
    return true;
}

bool CloudNetM_MqttDisconnect(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}

bool CloudNetM_MqttPublish(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    cloud_protocol_mqtt_publish_t publish = {0};

    // Parameter checking
    if (payload == NULL)
    {
        CLOUDNET_ERROR("Invalid parameters for MQTT connect parsing\r\n");
        return false;
    }

    // parse payload
    uint16_t offset = 0;

    // parse topic
    publish.topic_len = payload[offset] << 8 | payload[offset + 1];
    offset += 2;
    publish.topic = (char *)CLOUDM_MALLOC(publish.topic_len + 1);
    if (publish.topic == NULL)
    {
        CLOUDNET_ERROR("Memory allocation failed for topic\r\n");
        return false;
    }
    memcpy(publish.topic, &payload[offset], publish.topic_len);
    publish.topic[publish.topic_len] = '\0';
    offset += publish.topic_len;
    if (publish.topic == NULL)
    {
        CLOUDNET_ERROR("Topic parse failed\r\n");
        return false;
    }

    // parse payload
    publish.payload_len = payload[offset] << 8 | payload[offset + 1];
    offset += 2;
    publish.payload = (char *)CLOUDM_MALLOC(publish.payload_len + 1);
    if (publish.payload == NULL)
    {
        CLOUDNET_ERROR("Memory allocation failed for payload\r\n");
        CLOUDM_FREE(publish.topic);
        return false;
    }
    memcpy(publish.payload, &payload[offset], publish.payload_len);
    publish.payload[publish.payload_len] = '\0';
    offset += publish.payload_len;
    if (publish.payload == NULL)
    {
        CLOUDNET_ERROR("Payload parse failed\r\n");
        CLOUDM_FREE(publish.topic);
        return false;
    }

    // Add to the push queue
    bool result = CloudNetM_MqttAddMessageToQueue(publish.topic, publish.payload, 1); // default QoS=1

    // Clean up temporary memory
    CLOUDM_FREE(publish.topic);
    CLOUDM_FREE(publish.payload);

    if (result)
    {
        CLOUDNET_INFO("MQTT publish message added to queue successfully\r\n");
    }
    else
    {
        CLOUDNET_ERROR("Failed to add MQTT publish message to queue\r\n");
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
    cloud_protocol_mqtt_publish_topic_t publish_topic = {0};
    cloud_protocol_mqtt_subscribe_topic_t subscribe_topic = {0};
	if (payload == NULL)
	{
		CLOUDNET_ERROR("<%s>Payload is NULL\r\n", __func__);
		return false;
	}

	if (length < CLOUD_PROTOCOL_MQTT_SUB_PUB_MINIMUM_LENGTH)
	{
		CLOUDNET_ERROR("<%s>Payload too short: %d < %d\r\n", __func__, length, CLOUD_PROTOCOL_MQTT_SUB_PUB_MINIMUM_LENGTH);
		return false;
	}

    CLOUDNET_INFO("<%s>, msg:\r\n", __func__);
    CLOUDNET_PRINT_HEX(payload, length);

	uint16_t offset = 0;
    subscribe_topic.topic_len = payload[offset] << 8 | payload[offset + 1];
    offset += 2;
    subscribe_topic.topic = (char *)CLOUDM_MALLOC(subscribe_topic.topic_len + 1);
    if (subscribe_topic.topic == NULL)
    {
        CLOUDNET_ERROR("<%s>Memory allocation failed for subscribe topic\r\n");
        return false;
    }
    memcpy(subscribe_topic.topic, &payload[offset], subscribe_topic.topic_len);
    subscribe_topic.topic[subscribe_topic.topic_len] = '\0';
    offset += subscribe_topic.topic_len;

	if (subscribe_topic.topic == NULL)
	{
		CLOUDNET_ERROR("<%s>Failed to parse subscribe topic\r\n");
		return false;
	}
 
	// CLOUDNET_DEBUG("<%s>Subscribe topic parsed: %s\r\n", __func__, subscribe_topic);

    publish_topic.topic_len = payload[offset] << 8 | payload[offset + 1];
    offset += 2;
    publish_topic.topic = (char *)CLOUDM_MALLOC(publish_topic.topic_len + 1);
    if (publish_topic.topic == NULL)
    {
        CLOUDNET_ERROR("<%s>Memory allocation failed for publish topic\r\n");
        CLOUDM_FREE(subscribe_topic.topic);
        return false;
    }
    memcpy(publish_topic.topic, &payload[offset], publish_topic.topic_len);
    publish_topic.topic[publish_topic.topic_len] = '\0';
    offset += publish_topic.topic_len;

	// CLOUDNET_DEBUG("<%s>Publish topic parsed: %s\r\n", __func__, publish_topic);

	if (offset != length)
	{
		CLOUDNET_WARN("<%s>Payload length mismatch: parsed %d, expected %d\r\n", __func__, offset, length);
	}

	CLOUDNET_INFO("<%s>Subscribe topic: %s, Publish topic: %s\r\n", __func__, subscribe_topic.topic, publish_topic.topic);

	bool queue_result = YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_MQTOP, NULL,	socket_id, subscribe_topic.topic, publish_topic.topic);

	CLOUDM_FREE(subscribe_topic.topic);
	CLOUDM_FREE(publish_topic.topic);

	if (queue_result == 0)
	{
		// CLOUDNET_INFO("<%s>Subscribe-publish message processed successfully, socket: %d\r\n", __func__, socket_id);
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_MQTOP, NULL, socket_id);
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_GSTATE, NULL);
	}
	else
	{
		CLOUDNET_ERROR("<%s>Failed to process subscribe-publish message, socket: %d\r\n", __func__, socket_id);
	}

	return queue_result == 0 ? true : false;
}

bool CloudNetM_MqttSetWill(uint8_t socket_id, uint8_t *payload, uint16_t length)
{
    return false;
}

void CloudNet_MqttM_Init(void)
{
    cloud_net_mqtt_at_callback_t mqtt_at_callback;

    mqtt_at_callback.context = (uint8_t)TCP_ID_PROTOCOL_SG;
    mqtt_at_callback.topic_send_cb = CloudNetM_MqttSendAtTopic;
    mqtt_at_callback.payload_send_cb = CloudNetM_MqttSendAtPayload;

    CloudNetM_MqttPublishManagerInit(mqtt_at_callback);
}

void CloudNet_MqttM_Main(void)
{
    CloudNetM_MqttPublishManagerProcess();
}
/* EOL */
