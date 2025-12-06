//******************************************************************************
//* File Name: Cloud_Protocol_EventPost.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module event post source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_EventPost.h"

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
static char *cloud_protocol_event_post_build_method(const char *identifier);
static void Cloud_Protocol_EventPost_DestroyRequest(cloud_protocol_event_post_req_t *req);
static void Cloud_Protocol_EventPost_PostMessage(const char *json_str, const char* identifier);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
cloud_protocol_event_post_req_t *Cloud_Protocol_EventPost_CreateRequest(char* identifier, cloud_protocol_mqtt_set_msg_id_cb set_msg_id_cb)
{
	cloud_protocol_event_post_req_t *req = (cloud_protocol_event_post_req_t *)CLOUDM_MALLOC(sizeof(cloud_protocol_event_post_req_t));
	if (req == NULL)
	{
		return NULL;
	}

	// initialize request structure
	memset(req, 0, sizeof(cloud_protocol_event_post_req_t));

	// initialize version
	req->version = Cloud_Protocol_Strdup(CLOUD_PROTOCOL_VERSION_DEFAULT);
	if (req->version == NULL)
	{
		CLOUDM_FREE(req);
		return NULL;
	}

	// initialize id
	req->id = Cloud_Protocol_Strdup(Cloud_Protocol_GenerateMessageId());
	if (req->id == NULL)
	{
		CLOUDM_FREE(req->version);
		CLOUDM_FREE(req);
		return NULL;
	}

	if (set_msg_id_cb != NULL)
	{
		// set message ID using callback
		set_msg_id_cb(Cloud_Protocol_GetMessageId());
	}

	// initialize method
	req->method = cloud_protocol_event_post_build_method(identifier);

	return req;
}

// build method string
static char *cloud_protocol_event_post_build_method(const char *identifier)
{
	if (identifier == NULL)
	{
		return NULL;
	}

	size_t len = strlen(CLOUD_PROTOCOL_EVENT_POST_METHOD_PREFIX) +
				 strlen(identifier) +
				 strlen(CLOUD_PROTOCOL_EVENT_POST_METHOD_SUFFIX) + 1;

	char *method = (char *)CLOUDM_MALLOC(len);
	if (method != NULL)
	{
		snprintf(method, len, "%s%s%s",
				 CLOUD_PROTOCOL_EVENT_POST_METHOD_PREFIX,
				 identifier,
				 CLOUD_PROTOCOL_EVENT_POST_METHOD_SUFFIX);
	}

	return method;
}

// destroy request object
static void Cloud_Protocol_EventPost_DestroyRequest(cloud_protocol_event_post_req_t *req)
{
	if (req == NULL)
	{
		return;
	}

	if (req->id != NULL)
	{
		CLOUDM_FREE(req->id);
	}

	if (req->version != NULL)
	{
		CLOUDM_FREE(req->version);
	}

	if (req->method != NULL)
	{
		CLOUDM_FREE(req->method);
	}

	CLOUDM_FREE(req);
}

// build request JSON header
cJSON *Cloud_Protocol_EventPost_BuildRequestJsonHeader(const cloud_protocol_event_post_req_t *req)
{
	// build root object
	cJSON *root = cJSON_CreateObject();
	if (root == NULL)
	{
		return NULL;
	}

	// add first-level fields
	cJSON_AddStringToObject(root, "id", req->id);
	cJSON_AddStringToObject(root, "version", req->version);
	cJSON_AddStringToObject(root, "method", req->method);

	// create params object
	cJSON *params = cJSON_CreateObject();
	if (params == NULL)
	{
		cJSON_Delete(root);
		return NULL;
	}
	cJSON_AddItemToObject(root, "params", params);

	// create value object in params
	cJSON *value = cJSON_CreateObject();
	if (value == NULL)
	{
		cJSON_Delete(root);
		return NULL;
	}
	cJSON_AddItemToObject(params, "value", value);

	Cloud_Protocol_EventPost_DestroyRequest((cloud_protocol_event_post_req_t *)req);
	return root;
}

bool Cloud_Protocol_EventPost_AddParam(cJSON *object, const cloud_protocol_event_post_param_t *param)
{
	if (object == NULL || param == NULL || param->param_name == NULL)
	{
		return false;
	}

	// get params->value object
	cJSON *params_obj = cJSON_GetObjectItem(object, "params");
	if (params_obj == NULL)
	{
		return false;
	}

	cJSON *value_obj = cJSON_GetObjectItem(params_obj, "value");
	if (value_obj == NULL)
	{
		return false;
	}

	// add parameter based on type
	switch (param->param_type)
	{
		case CLOUD_PROTOCOL_VALUE_TYPE_STRING:
			cJSON_AddStringToObject(value_obj, param->param_name, param->param_value.string_value);
			break;

		case CLOUD_PROTOCOL_VALUE_TYPE_INT32:
			cJSON_AddNumberToObject(value_obj, param->param_name, param->param_value.int32_value);
			break;

		case CLOUD_PROTOCOL_VALUE_TYPE_FLOAT:
			cJSON_AddNumberToObject(value_obj, param->param_name, (double)param->param_value.float_value);
			break;

		case CLOUD_PROTOCOL_VALUE_TYPE_BOOL:
			cJSON_AddBoolToObject(value_obj, param->param_name, param->param_value.bool_value);
			break;

		case CLOUD_PROTOCOL_VALUE_TYPE_STRING_ARRAY:
		{
			cJSON *array = cJSON_CreateArray();
			if (array == NULL)
			{
				return false;
			}

			cloud_protocol_string_array_t *string_array = param->param_value.string_array_value;
			for (uint16_t i = 0; i < string_array->count; i++)
			{
				cJSON *item = cJSON_CreateString(string_array->strings[i]);
				if (item != NULL)
				{
					cJSON_AddItemToArray(array, item);
				}
			}

			cJSON_AddItemToObject(value_obj, param->param_name, array);
			break;
		}

		case CLOUD_PROTOCOL_VALUE_TYPE_DOUBLE:
			cJSON_AddNumberToObject(value_obj, param->param_name, param->param_value.double_value);
			break;

		case CLOUD_PROTOCOL_VALUE_TYPE_NULL:
			cJSON_AddNullToObject(value_obj, param->param_name);
			break;

		default:
			// not support
			return false;
	}

	return true;
}

void Cloud_Protocol_EventPost_PrintUnformatted(cJSON *object, uint64_t timestamp, char* identifier)
{
	if (object == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PrintUnformatted: object is NULL\r\n");
		return;
	}

	// get params object
	cJSON *params = cJSON_GetObjectItem(object, "params");
	if (params == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PrintUnformatted: params is NULL\r\n");
		return;
	}

	// add time field
	cJSON_AddNumberToObject(params, "time", timestamp);
	// CLOUD_INFO("<%s>timestamp:%lld\r\n", __func__, timestamp);

	// print JSON string
	char *json_str = cJSON_PrintUnformatted(object);	

	// free static variable
	cJSON_Delete(object);
	object = NULL;

	Cloud_Protocol_EventPost_PostMessage(json_str, identifier);
	// post function will free json_str
}

static void Cloud_Protocol_EventPost_PostMessage(const char *json_str, const char *identifier)
{
	if (json_str == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PostMessage: json_str is NULL\r\n");
		return;
	}
	char event_post_subscribe_topic[CLOUD_PROTOCOL_SG_SYSN_NET_TIME_TOPIC_MAX_LENGTH];  // subscribe topic
	char event_post_publish_topic[CLOUD_PROTOCOL_SG_SYSN_NET_TIME_TOPIC_MAX_LENGTH]; // publish topic

	const cloud_protocol_mqtt_topic_config_t *topic = Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_EVENT_POST);
	Cloud_Protocol_Sg_Build_Topic(topic->subscribe_topic, NULL, event_post_subscribe_topic, CLOUD_PROTOCOL_SG_SYSN_NET_TIME_TOPIC_MAX_LENGTH);
	Cloud_Protocol_Sg_Build_Topic(topic->publish_topic, identifier, event_post_publish_topic, CLOUD_PROTOCOL_SG_SYSN_NET_TIME_TOPIC_MAX_LENGTH);

	// send message to cloud protocol module
	Cloud_Protocol_Mqtt_AddPublishMessage(event_post_publish_topic, json_str, CLOUD_PROTOCOL_MQTT_NEED_ACK, event_post_subscribe_topic);
}

// parse JSON
cloud_protocol_event_post_resp_t *Cloud_Protocol_EventPost_ParseResponse(const char *json_str)
{
	if (json_str == NULL)
	{
		return NULL;
	}

	cJSON *root = cJSON_Parse(json_str);
	if (root == NULL)
	{
		return NULL;
	}

	cloud_protocol_event_post_resp_t *resp = (cloud_protocol_event_post_resp_t *)CLOUDM_MALLOC(sizeof(cloud_protocol_event_post_resp_t));
	if (resp == NULL)
	{
		cJSON_Delete(root);
		return NULL;
	}

	memset(resp, 0, sizeof(cloud_protocol_event_post_resp_t));

	// parse fields
	cJSON *id_item = cJSON_GetObjectItem(root, "id");
	cJSON *code_item = cJSON_GetObjectItem(root, "code");
	cJSON *data_item = cJSON_GetObjectItem(root, "data");

	if (id_item != NULL && cJSON_IsString(id_item))
	{
		resp->id = Cloud_Protocol_Strdup(id_item->valuestring);
	}

	if (code_item != NULL && cJSON_IsNumber(code_item))
	{
		resp->code = code_item->valueint;
	}

	if (data_item != NULL && cJSON_IsString(data_item))
	{
		resp->data = Cloud_Protocol_Strdup(data_item->valuestring);
	}

	cJSON_Delete(root);

	// param check
	if (resp->id == NULL)
	{
		Cloud_Protocol_EventPost_DestroyResponse(resp);
		return NULL;
	}

	return resp;
}

// destroy response object
void Cloud_Protocol_EventPost_DestroyResponse(cloud_protocol_event_post_resp_t *resp)
{
	if (resp == NULL)
	{
		return;
	}

	if (resp->id != NULL)
	{
		CLOUDM_FREE(resp->id);
	}

	if (resp->data != NULL)
	{
		CLOUDM_FREE(resp->data);
	}

	CLOUDM_FREE(resp);
}

// check if response indicates success
bool Cloud_Protocol_EventPost_IsSuccess(const cloud_protocol_event_post_resp_t *resp)
{
	return (resp != NULL && resp->code == CLOUD_PROTOCOL_RESPONSE_SUCCESS);
}

// get error code message
const char *Cloud_Protocol_EventPost_GetCodeMessage(int code)
{
	switch (code)
	{
	case CLOUD_PROTOCOL_RESPONSE_SUCCESS:
		return "success";
	case CLOUD_PROTOCOL_RESPONSE_REQUEST_ERROR:
		return "request error";
	case CLOUD_PROTOCOL_RESPONSE_PARAMETER_ERROR:
		return "request parameter error";
	case CLOUD_PROTOCOL_RESPONSE_TOO_MANY_REQUESTS:
		return "too many requests";
	default:
		if (code >= CLOUD_PROTOCOL_RESPONSE_CUSTOM_ERROR_BASE &&
			code <= CLOUD_PROTOCOL_RESPONSE_CUSTOM_ERROR_BASE + 10000)
		{
			return "custom error";
		}
		return "unknown error";
	}
}
/* EOL */
