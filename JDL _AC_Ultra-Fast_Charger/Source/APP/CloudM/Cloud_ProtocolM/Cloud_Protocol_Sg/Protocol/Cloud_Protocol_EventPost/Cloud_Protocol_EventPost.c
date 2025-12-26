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
static cloud_protocol_event_post_task_t cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static char *cloud_protocol_event_post_build_method(const char *identifier);
static bool Cloud_Protocol_EventPost_PostMessage(const char *json_str, const char* identifier);
static void Cloud_Protocol_EventPost_DestroyResponse(cloud_protocol_event_post_resp_t *resp);
static bool Cloud_Protocol_EventPost_ShouldPostEvent(cloud_protocol_event_post_task_t *task, uint32_t current_time);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// initialize event post module
void Cloud_Protocol_EventPost_Init(void)
{
	cloud_protocol_event_post_type_t type;
	for (type = CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_INFO; type < CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX; type++)
	{
		cloud_protocol_sg_event_tasks[type].type = type;
		cloud_protocol_sg_event_tasks[type].last_post_time = 0;
		cloud_protocol_sg_event_tasks[type].interval = 0;
		cloud_protocol_sg_event_tasks[type].enabled = false;
		cloud_protocol_sg_event_tasks[type].force_post = false;
	}
}

// create event post request object
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
	req->version = Cloud_Protocol_Strdup(CLOUD_PROTOCOL_SG_VERSION_DEFAULT);
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

	size_t len = strlen(CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_PREFIX) +
				 strlen(identifier) +
				 strlen(CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_SUFFIX) + 1;

	char *method = (char *)CLOUDM_MALLOC(len);
	if (method != NULL)
	{
		snprintf(method, len, "%s%s%s",
				 CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_PREFIX,
				 identifier,
				 CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_SUFFIX);
	}

	return method;
}

// destroy request object
void Cloud_Protocol_EventPost_DestroyRequest(cloud_protocol_event_post_req_t *req)
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
		CLOUD_WARN("<%s %d> Failed to create params object\r\n", __func__, __LINE__);
		cJSON_Delete(root);
		return NULL;
	}
	cJSON_AddItemToObject(root, "params", params);

	// create value object in params
	cJSON *value = cJSON_CreateObject();
	if (value == NULL)
	{
		CLOUD_WARN("<%s %d> Failed to create value object\r\n", __func__, __LINE__);
		cJSON_Delete(root);
		return NULL;
	}
	cJSON_AddItemToObject(params, "value", value);

	return root;
}

// print JSON string and post message
bool Cloud_Protocol_EventPost_PrintUnformatted(cJSON *object, uint64_t timestamp, char* identifier)
{
	if (object == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PrintUnformatted: object is NULL\r\n");
		return false;
	}

	// get params object 
	cJSON *params = cJSON_GetObjectItem(object, "params");
	if (params == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PrintUnformatted: params is NULL\r\n");
		return false;
	}

	// add time field
	cJSON_AddNumberToObject(params, "time", timestamp);
	// CLOUD_INFO("<%s>timestamp:%lld\r\n", __func__, timestamp);

	// print JSON string
	char *json_str = cJSON_PrintUnformatted(object);	

	bool result = Cloud_Protocol_EventPost_PostMessage(json_str, identifier);
	// free json_str
	if (json_str != NULL)
	CLOUDM_FREE(json_str);
	return result;
}

// post message to cloud
static bool Cloud_Protocol_EventPost_PostMessage(const char *json_str, const char *identifier)
{
	if (json_str == NULL)
	{
		CLOUD_ERROR("Cloud_Protocol_EventPost_PostMessage: json_str is NULL\r\n");
		return false;
	}
	char event_post_subscribe_topic[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH];  // subscribe topic
	char event_post_publish_topic[CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH]; // publish topic

	const cloud_protocol_mqtt_topic_config_t *topic = Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_EVENT_POST);
	Cloud_Protocol_Sg_Build_Topic(topic->subscribe_topic, NULL, event_post_subscribe_topic, CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH);
	Cloud_Protocol_Sg_Build_Topic(topic->publish_topic, identifier, event_post_publish_topic, CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH);

	// send message to cloud protocol module
	return Cloud_Protocol_Mqtt_AddPublishMessage(event_post_publish_topic, json_str, CLOUD_PROTOCOL_MQTT_NEED_ACK, event_post_subscribe_topic);
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
static void Cloud_Protocol_EventPost_DestroyResponse(cloud_protocol_event_post_resp_t *resp)
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

/**
 * @brief update event post configuration
 * @param config device configuration structure
 */
void Cloud_Protocol_EventPost_UpdateConfig(const v2g_data_dev_config *config)
{
	// update charging gun monitoring event report frequency (seconds)
	cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_INFO].interval = config->gunInfoFreq;
	CLOUD_DEBUG("Gun info report interval: %u seconds\r\n", config->gunInfoFreq);

	// update vehicle working status real-time monitoring attribute (seconds)
	cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_INFO].interval = config->vehicleInfoFreq;
	CLOUD_DEBUG("Vehicle info report interval: %u seconds\r\n", config->vehicleInfoFreq);

	// update vehicle battery working status real-time monitoring attribute (seconds)
	cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_BATTERY_INFO].interval = config->batteryInfoFreq;
	CLOUD_DEBUG("Battery info report interval: %u seconds\r\n", config->batteryInfoFreq);

	// update device warning information upload frequency (seconds)
	cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WARNINGS].interval = config->pileWarnings;
	CLOUD_DEBUG("Pile warnings report interval: %u seconds\r\n", config->pileWarnings);

	// update vehicle warning information upload frequency (seconds)
	cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_WARNINGS].interval = config->vehicleWarnings;
	CLOUD_DEBUG("Vehicle warnings report interval: %u seconds\r\n", config->vehicleWarnings);

	// update ground lock monitoring upload frequency (minutes to seconds)
	if (config->grndLock > 0)
	{
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_GRND_LOCK].interval = config->grndLock * 60;
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_GRND_LOCK].enabled = true;
		CLOUD_DEBUG("Ground lock report interval: %u minutes (%u seconds)\r\n", config->grndLock, config->grndLock * 60);
	}
	else
	{
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_GRND_LOCK].enabled = false;
		CLOUD_DEBUG("Ground lock reporting disabled (no ground lock)\r\n");
	}

	// update door lock monitoring upload frequency (minutes to seconds)
	if (config->doorLock > 0)
	{
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_DOOR_LOCK].interval = config->doorLock * 60;
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_DOOR_LOCK].enabled = true;
		CLOUD_DEBUG("Door lock report interval: %u minutes (%u seconds)\r\n",config->doorLock, config->doorLock * 60);
	}
	else
	{
		cloud_protocol_sg_event_tasks[CLOUD_PROTOCOL_EVENT_POST_TYPE_DOOR_LOCK].enabled = false;
		CLOUD_DEBUG("Door lock reporting disabled (no door lock)\r\n");
	}
}

#define CLOUDM_MIN_FREE_HEAP_SIZE	(1024 * 6) // 6KB
// check if event should be posted
static bool Cloud_Protocol_EventPost_ShouldPostEvent(cloud_protocol_event_post_task_t *task, uint32_t current_time)
{
	if (!task)
	{
		return false;
	}

    size_t free_heap = CLOUDM_GET_FREE_HEAP_SIZE();

	if (free_heap < CLOUDM_MIN_FREE_HEAP_SIZE)
	{
		// CLOUD_WARN("<%s> Low memory, cannot force post event type %d\r\n", __func__, task->type);
		return false;
	}

	// check if force post is set
	if (task->force_post)
	{
		task->force_post = false;
		return true;
	}

	// check if interval has elapsed
	if ((current_time >= task->last_post_time + task->interval) && (0 != task->interval) && task->enabled)
	{
		task->interval = 0;
		return true;
	}

	return false;
}

// event post periodic task
void Cloud_Protocol_EventPost_PeriodicTask(void)
{
	if (Cloud_Protocol_Mqtt_GetNetworkStatus() != CLOUD_PROTOCOL_MQTT_STATE_CONNECTED)
	{
		return;
	}

	uint32_t current_time = (uint32_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
	uint8_t gun1 = 0;
	uint8_t gun2 = 0;
	bool result = false;

	for (int i = 0; i < CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX; i++)
	{
		cloud_protocol_event_post_task_t *task = &cloud_protocol_sg_event_tasks[i];

		if (Cloud_Protocol_EventPost_ShouldPostEvent(task, current_time))
		{ 
			// update last post time
			task->last_post_time = current_time;

			// call corresponding event post function
			switch (task->type)
			{
				case CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_INFO:
					// Cloud_Protocol_EventPost_GunInfo();
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_INFO:
					// Cloud_Protocol_EventPost_VehicleInfo();
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_BATTERY_INFO:
					// Cloud_Protocol_EventPost_BatteryInfo();
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WARNINGS:
					// Cloud_Protocol_EventPost_PileWarnings();
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_WARNINGS:
					// Cloud_Protocol_EventPost_VehicleWarnings();
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_GRND_LOCK:
					if (task->enabled)
					{
						// Cloud_Protocol_EventPost_GrndLock();
					}
					return;

				case CLOUD_PROTOCOL_EVENT_POST_TYPE_DOOR_LOCK:
					if (task->enabled)
					{
						// Cloud_Protocol_EventPost_DoorLock();
					}
					return;
				case CLOUD_PROTOCOL_EVENT_POST_TYPE_FW_INFO:
					result = Cloud_Protocol_EventPost_FwInfo_Post();
					if (!result)
					 {
						Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_FW_INFO, 20); // retry later
						CLOUD_WARN("<%s> FW info event post failed, 20' will retry later\r\n", __func__);
					 }
					return;
				case CLOUD_PROTOCOL_EVENT_POST_TYPE_VERSION_INFO:
					result = Cloud_Protocol_EventPost_VersionInfo_Post();
					if (!result)
					 {
						Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_VERSION_INFO, 20); // retry later
						CLOUD_WARN("<%s> Version info event post failed, 20' will retry later\r\n", __func__);
					 }
					return;
				case CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS:
					Cloud_Protocol_Sg_Order_GetOrderOnRunningStatus(&gun1, &gun2);
					if (gun1)
					{
						result = Cloud_Protocol_EventPost_PileWorkstatus_Post(1);
						if (!result)
						{
							Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS, 10); // retry later
							CLOUD_WARN("<%s> Gun %d work status event post failed, 10' will retry later\r\n", __func__, gun1);
						}
					}
					if (gun2)
					{
						result = Cloud_Protocol_EventPost_PileWorkstatus_Post(2);
						if (!result)
						{
							Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS, 10); // retry later
							CLOUD_WARN("<%s> Gun %d work status event post failed, 10' will retry later\r\n", __func__, gun2);
						}
					}
					return;
				case CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_STATUS:
					Cloud_Protocol_Sg_Order_GetGunStatusPostEnable(&gun1, &gun2);
					if (gun1)
					{
						result = Cloud_Protocol_EventPost_GunStatus_Post(1);
						if (!result)
						{
							Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_STATUS, 10); // retry later
							CLOUD_WARN("<%s> Gun %d status event post failed, 10' will retry later\r\n", __func__, gun1);
						}
					}
					if (gun2)
					{
						result = Cloud_Protocol_EventPost_GunStatus_Post(2);
						if (!result)
						{
							Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_STATUS, 10); // retry later
							CLOUD_WARN("<%s> Gun %d status event post failed, 10' will retry later\r\n", __func__, gun2);
						}
					}
				default:
					return;
			}
		}
	}
}

//  enable period trigger event post
bool Cloud_Protocol_EventPost_EnableTriggerEvent(cloud_protocol_event_post_type_t type, uint16_t interval)
{
	if (type >= CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX)
	{
		return false;
	}
	cloud_protocol_sg_event_tasks[type].enabled = true;
	cloud_protocol_sg_event_tasks[type].last_post_time = (uint32_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
	if (0 != interval)
	{
		cloud_protocol_sg_event_tasks[type].interval = interval;
	}
	CLOUD_INFO("<%s> Enabled event type %d with interval %u s\r\n", __func__, type, interval);
	return true;
}

// disable period trigger event post
bool Cloud_Protocol_EventPost_DisableTriggerEvent(cloud_protocol_event_post_type_t type)
{
	if (type >= CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX)
	{
		return false;
	}
	cloud_protocol_sg_event_tasks[type].enabled = false;
	return true;
}

// immediate trigger event post
bool Cloud_Protocol_EventPost_ForceTriggerEvent(cloud_protocol_event_post_type_t type)
{
	if (type >= CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX)
	{
		return false;
	}
	cloud_protocol_sg_event_tasks[type].force_post = true;
	return true;
}

// force immediate post of all events
void Cloud_Protocol_EventPost_ForceAllEvents(void)
{
	for (int i = 0; i < CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX; i++)
	{

		cloud_protocol_sg_event_tasks[i].force_post = true;
	}
}
/* EOL */
