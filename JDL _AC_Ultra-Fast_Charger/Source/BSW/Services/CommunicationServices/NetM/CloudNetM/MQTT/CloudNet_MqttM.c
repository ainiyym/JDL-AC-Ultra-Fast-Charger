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
        CLOUD_ERROR("Invalid parameters for MQTT connect parsing\r\n");
        return false;
    }
    iotx_sign_mqtt_t config;
    Cloud_Protocol_Mqtt_GetClientConfig(&config);
    CLOUD_DEBUG("ClientID: %s\r\n", config.clientid);
    CLOUD_DEBUG("Username: %s\r\n", config.username);
    CLOUD_DEBUG("Password: %s\r\n", config.password);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_DTUID, NULL, config.clientid);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_MQSET, NULL, socket_id,
                      "{DTUID}",
                      config.username,
                      config.password);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_DTUID, NULL);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_MQSET, NULL, socket_id);
    YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_GSTATE, NULL);

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
        YeeCom_AtCmd_Send(YEECOM_AT_CMD_GET, YEECOM_AT_CMD_MQTOP, NULL, socket_id);
	}
	else
	{
		CLOUD_ERROR("<%s>Failed to process subscribe-publish message, socket: %d\r\n", __func__, socket_id);
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
