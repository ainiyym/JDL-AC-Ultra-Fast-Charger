//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ServiceCall.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol call service source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ServiceCall.h"

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
static const char *cloud_protocol_extract_service_identifier(const char *method);
static bool cloud_protocol_dispatch_service_call(cloud_protocol_sg_service_call_type_t service_type, cJSON *params, const char *msg_id);
static cloud_protocol_sg_service_call_type_t cloud_protocol_detect_service_call_type(const char *method);
static char *cloud_protocol_build_service_response_topic(const char *identifier);
static cJSON *cloud_protocol_service_call_create_response_json(const char *msg_id, int code, cJSON *data);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief extract service identifier from method string
 * @param method format："thing.service.issueChargeOrDischargePlanSrv"
 * @return the extracted identifier fails(success) or NULL(fails)
 */
static const char *cloud_protocol_extract_service_identifier(const char *method)
{
    if (!method)
    {
        return NULL;
    }

    // Find the last dot separator
    const char *last_dot = strrchr(method, '.');
    if (!last_dot)
    {
        CLOUD_ERROR("<%s>Invalid method format: %s\n", __FUNCTION__, method);
        return NULL;
    }

    // Return the identifier after the last dot
    return last_dot + 1;
}

/**
 * @brief Call the corresponding processing function according to the service type
 * @param service_type Service type
 * @param params Parameter JSON object
 * @param msg_id message ID
 * @return Returns true if processing is successful, false otherwise
 */
static bool cloud_protocol_dispatch_service_call(cloud_protocol_sg_service_call_type_t service_type, cJSON *params, const char *msg_id)
{
    bool result = false;

    switch (service_type)
    {
        case CLOUD_PROTOCOL_SG_SERVICE_CALL_START_CHARGE:
            result = Cloud_Protocol_Sg_ParseRemoteStartParam(params, msg_id);
            break;

        case CLOUD_PROTOCOL_SG_SERVICE_CALL_STOP_CHARGE:
            result = Cloud_Protocol_Sg_ParseRemoteStopParam(params, msg_id);
            break;

        case CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN:
        default:
            CLOUD_ERROR("<%s>Unknown or unsupported service type: %d\n", __FUNCTION__, service_type);
            break;
    }

    return result;
}

/**
 * @brief Detect the type of service call
 * @param method format："thing.service.{identifier}"
 * @return Detected service type
 */
static cloud_protocol_sg_service_call_type_t cloud_protocol_detect_service_call_type(const char *method)
{
    if (!method)
    {
        CLOUD_ERROR("<%s>Method string is NULL\r\n", __FUNCTION__);
        return CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN;
    }

    CLOUD_DEBUG("<%s>Detecting service call type from method: %s\r\n", __FUNCTION__, method);

    /* Detect remote start charge/discharge plan service */
    if (strstr(method, "issueChargeOrDischargePlanSrv") != NULL)
    {
        return CLOUD_PROTOCOL_SG_SERVICE_CALL_START_CHARGE;
    }

    /* Detect remote stop charge/discharge plan service */
    if (strstr(method, "stopChargeOrDischargeSrv") != NULL)
    {
        return CLOUD_PROTOCOL_SG_SERVICE_CALL_STOP_CHARGE;
    }

    /* Check if it is a service call mode */
    if (strstr(method, "thing.service.") != NULL)
    {
        CLOUD_WARN("<%s>Service call detected but type unknown: %s\r\n", __FUNCTION__, method);
        return CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN;
    }

    return CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN;
}

/**
 * @brief Handle service call
 * @param payload Service call payload
 * @param payload_len Payload length
 * @return NULL
 */
bool cloud_protocol_service_call(const char *payload, uint16_t payload_len)
{
    if (!payload || payload_len == 0)
    {
        CLOUD_ERROR("<%s>Invalid payload or length\r\n", __FUNCTION__);
        return false;
    }

    cJSON *root = cJSON_Parse(payload);
    if (!root)
    {
        CLOUD_ERROR("<%s>Failed to parse JSON payload\r\n", __FUNCTION__);
        return false;
    }

    // get fileds
    cJSON *method_item = cJSON_GetObjectItem(root, "method");
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON *params_item = cJSON_GetObjectItem(root, "params");

    // verify required fields
    if (!method_item || !cJSON_IsString(method_item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'method' field\r\n", __FUNCTION__);
        cJSON_Delete(root);
        return false;
    }

    if (!id_item || !cJSON_IsString(id_item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'id' field\r\n", __FUNCTION__);
        cJSON_Delete(root);
        return false;
    }

    const char *method = method_item->valuestring;
    const char *msg_id = id_item->valuestring;

    CLOUD_DEBUG("<%s> method=%s, id=%s\r\n", __FUNCTION__, method, msg_id);

    // extract service identifier from method
    const char *identifier = cloud_protocol_extract_service_identifier(method);
    if (!identifier)
    {
        CLOUD_ERROR("<%s>Failed to extract service identifier from method: %s\r\n", __FUNCTION__, method);
        cJSON_Delete(root);
        return false;
    }

    CLOUD_DEBUG("<%s> >Service identifier: %s\n", __FUNCTION__, identifier);

    // search for corresponding handler
    cloud_protocol_sg_service_call_type_t service_type = cloud_protocol_detect_service_call_type(method);

    if (service_type == CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN)
    {
        CLOUD_WARN("<%s>Unknown service call type for method: %s\r\n", __FUNCTION__, method);
        cJSON_Delete(root);
        return false;
    }

    CLOUD_DEBUG("<%s>Detected service type: %d\r\n", __FUNCTION__, service_type);

    // call dispatch function
    bool result = cloud_protocol_dispatch_service_call(service_type, params_item, msg_id);

    // Based on the processing result, send a response
    if (result)
    {
        CLOUD_DEBUG("<%s>Service type %d handled successfully\r\n", __FUNCTION__, service_type);
    }
    else
    {
        CLOUD_ERROR("<%s>Service type %d handling failed\r\n", __FUNCTION__, service_type);
    }

    cJSON_Delete(root);
    return result;
}

/**
 * @brief build service response topic
 * @param identifier service identifier
 * @return topic string
 */
static char *cloud_protocol_build_service_response_topic(const char *identifier)
{
    if (!identifier)
    {
        CLOUD_ERROR("<%s>: identifier is NULL\r\n", __func__);
        return NULL;
    }

    char service_call_publish_topic[CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH] = {0};

    // get topic config
    const cloud_protocol_mqtt_topic_config_t *topic = Cloud_Protocol_Mqtt_GetPassiveTopicConfigByEnum(CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_SERVICE_INVOCATION);

    if (!topic || !topic->publish_topic)
    {
        CLOUD_ERROR("<%s>: Failed to get service invocation topic\r\n", __func__);
        return NULL;
    }

    // build topic
    Cloud_Protocol_Sg_Build_Topic(topic->publish_topic, identifier, service_call_publish_topic, CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH);

    // duplicate string for return
    char *result = Cloud_Protocol_Strdup(service_call_publish_topic);
    if (!result)
    {
        CLOUD_ERROR("<%s>: Failed to allocate memory for topic\n", __func__);
    }

    return result;
}

/**
 * @brief create response JSON object
 * @param msg_id message ID
 * @param code response code
 * @param data data JSON object
 * @return response JSON object
 */
static cJSON *cloud_protocol_service_call_create_response_json(const char *msg_id, int code, cJSON *data)
{
    if (!msg_id)
    {
        CLOUD_ERROR("<%s>: msg_id is NULL\n", __func__);
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root)
    {
        CLOUD_ERROR("<%s>: Failed to create JSON object\n", __func__);
        return NULL;
    }

    // add id field
    cJSON_AddStringToObject(root, "id", msg_id);

    // add code field
    cJSON_AddNumberToObject(root, "code", code);

    // add data field (ensure it is an object)
    if (data && cJSON_IsObject(data))
    {
        cJSON_AddItemToObject(root, "data", cJSON_Duplicate(data, true));
    }
    else
    {
        // If no data or data is not an object, create an empty object
        cJSON *empty_data = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "data", empty_data);
    }

    return root;
}

/**
 * @brief Send service call response
 * @param msg_id message ID
 * @param identifier service identifier
 * @param response_code response code
 * @param data data JSON object
 */
bool cloud_protocol_service_call_response(const char *msg_id, const char *identifier, int response_code, const cJSON *data)
{
    bool result = false;

    if (!msg_id || !identifier)
    {
        CLOUD_ERROR("<%s>: msg_id or identifier is NULL\r\n", __func__);
        return false;
    }

    // build response topic
    char *response_topic = cloud_protocol_build_service_response_topic(identifier);
    if (!response_topic)
    {
        CLOUD_ERROR("<%s>: Failed to build response topic\r\n", __func__);
        return false;
    }

    // create response JSON object
    cJSON *response_json = cloud_protocol_service_call_create_response_json(msg_id, response_code, (cJSON *)data);
    if (!response_json)
    {
        CLOUD_ERROR("<%s>: Failed to create response JSON object\r\n", __func__);
        CLOUDM_FREE(response_topic);
        return false;
    }

    // print JSON string
    char *json_str = cJSON_PrintUnformatted(response_json);
    if (!json_str)
    {
        CLOUD_ERROR("<%s>: Failed to print JSON string\r\n", __func__);
        cJSON_Delete(response_json);
        CLOUDM_FREE(response_topic);
        return false;
    }

    // send message to cloud protocol module
    result = Cloud_Protocol_Mqtt_AddPublishMessage(response_topic, json_str, CLOUD_PROTOCOL_MQTT_MSG_NO_ACK, NULL);

    // free resources
    cJSON_Delete(response_json);
    CLOUDM_FREE(json_str);
    CLOUDM_FREE(response_topic);

    return result;
}
/* EOL */
