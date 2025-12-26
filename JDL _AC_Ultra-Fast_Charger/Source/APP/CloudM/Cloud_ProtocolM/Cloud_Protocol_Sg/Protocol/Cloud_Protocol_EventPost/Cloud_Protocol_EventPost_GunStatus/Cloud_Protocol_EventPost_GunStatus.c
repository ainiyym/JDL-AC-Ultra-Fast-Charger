//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_GunStatus.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module gun status check source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_EventPost_GunStatus.h"
#include "CpM_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_SG_GUN_STATUS_CONNECTED		 10
#define CLOUD_PROTOCOL_SG_GUN_STATUS_DISCONNECTED	 11

#define CLOUD_PROTOCOL_SG_GUN_IDLE_12V (CPV_VOLT_TWF)
#define CLOUD_PROTOCOL_SG_GUN_IDLE_4V (CPV_VOLT_FOUR)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	bool status_changed;	  // Status changed flag
	uint8_t gun_status;		  // Gun status: connected/disconnected
	uint32_t send_message_id; // Sent message ID
} cloud_protocol_event_post_gun_status_ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_event_post_gun_status_ctrl_t cloud_protocol_event_post_gun_status_ctrl[CLOUD_PROTOCOL_SG_MAX_GUN_NUM] = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
static void Cloud_Protocol_EventPost_SetGun1StatusMsgId(uint32_t msg_id)
{
    cloud_protocol_event_post_gun_status_ctrl[0].send_message_id = msg_id;
    CLOUD_INFO("<%s> Gun 1 Status Msg ID set to: %d\r\n", __func__, cloud_protocol_event_post_gun_status_ctrl[0].send_message_id);
}

static void Cloud_Protocol_EventPost_SetGun2StatusMsgId(uint32_t msg_id)
{
    cloud_protocol_event_post_gun_status_ctrl[1].send_message_id = msg_id;
    CLOUD_INFO("<%s> Gun 2 Status Msg ID set to: %d\r\n", __func__, cloud_protocol_event_post_gun_status_ctrl[1].send_message_id);
}

bool Cloud_Protocol_EventPost_GunStatus_Post(uint8_t gun_no)
{
	cloud_protocol_event_post_req_t *cloud_protocol_event_post_req = NULL;
	evs_event_pile_status_change data;
	cJSON *root = NULL;
	uint64_t timestamp = 0;

	// create request
	if (gun_no == 1)
    {
		cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("dcStChEvt", Cloud_Protocol_EventPost_SetGun1StatusMsgId);
	}
	else if (gun_no == 2)
	{
		cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("dcStChEvt", Cloud_Protocol_EventPost_SetGun2StatusMsgId);
	}
	else
	{
		CLOUD_WARN("<%s %d> Invalid gun number: %d\r\n", __func__, __LINE__, gun_no);
		return false;
	}

	// build json header
	root = Cloud_Protocol_EventPost_BuildRequestJsonHeader(cloud_protocol_event_post_req);
	Cloud_Protocol_EventPost_DestroyRequest((cloud_protocol_event_post_req_t *)cloud_protocol_event_post_req);

	// get params->value object
	cJSON *params_obj = cJSON_GetObjectItem(root, "params");
	if (params_obj == NULL)
	{
		CLOUD_WARN("<%s %d> params object not found\r\n", __func__, __LINE__);
		return false;
	}

	cJSON *value_obj = cJSON_GetObjectItem(params_obj, "value");
	if (value_obj == NULL)
	{
		CLOUD_WARN("<%s %d> value object not found\r\n", __func__, __LINE__);
		return false;
	}

	// add gun status fields
	uint64_t current_time = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // convert to milliseconds
	snprintf(data.yxOccurTime, V2G_MAX_TIMESTAMP_LEN, "%llu", (unsigned long long)current_time);
	data.gunNo = gun_no;
	data.connCheckStatus = cloud_protocol_event_post_gun_status_ctrl[data.gunNo - 1].gun_status;
    cJSON_AddNumberToObject(value_obj, "gunNo", data.gunNo);
    cJSON_AddStringToObject(value_obj, "yxOccurTime", data.yxOccurTime);
    cJSON_AddNumberToObject(value_obj, "connCheckStatus", data.connCheckStatus);

	// print unformatted json string
	timestamp = (uint64_t)current_time;
	bool ret = Cloud_Protocol_EventPost_PrintUnformatted(root, timestamp, "dcStChEvt");
	if (ret == false)
	{
		CLOUD_WARN("<%s %d> Post gun status event failed\r\n", __func__, __LINE__);
	}

	cJSON_Delete(root);
	root = NULL;
	return ret;
}

bool Cloud_Protocol_EventPost_GunStatus_HandleResponse(uint32_t msg_id)
{
    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        if (msg_id == cloud_protocol_event_post_gun_status_ctrl[i].send_message_id)
        {
			cloud_protocol_event_post_gun_status_ctrl[i].status_changed = false;
			CLOUD_INFO("<%s> Gun %d status event response received, status changed flag cleared\r\n", __func__, i + 1);
        	return true;
        }
    }
	return false;
}

void Cloud_Protocol_Sg_Order_GetGunStatusPostEnable(uint8_t *gun1, uint8_t *gun2)
{
	if (gun1 == NULL || gun2 == NULL)
	{
		return;
	}
	*gun1 = cloud_protocol_event_post_gun_status_ctrl[0].status_changed ? 1 : 0;
	*gun2 = cloud_protocol_event_post_gun_status_ctrl[1].status_changed ? 1 : 0;
}

void Cloud_Protocol_EventPost_GunStatusMonitoring(void)
{
	uint8_t cp_status = 0;
	static uint8_t last_cp_status[CLOUD_PROTOCOL_SG_MAX_GUN_NUM] = {0};

	for(SysConnector_Num_Enum gun_index = (SysConnector_Num_Enum)0; gun_index < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; gun_index++)
	{
		if (cloud_protocol_event_post_gun_status_ctrl[gun_index].status_changed)
		{
			continue; // Skip if status change is already pending
		}

		cp_status = CPM_GetCpVoltStatus(gun_index);
		if (cp_status != CLOUD_PROTOCOL_SG_GUN_IDLE_12V && cp_status != CLOUD_PROTOCOL_SG_GUN_IDLE_4V)
		{
			cloud_protocol_event_post_gun_status_ctrl[gun_index].gun_status = CLOUD_PROTOCOL_SG_GUN_STATUS_CONNECTED;
		}
		else
		{
			cloud_protocol_event_post_gun_status_ctrl[gun_index].gun_status = CLOUD_PROTOCOL_SG_GUN_STATUS_DISCONNECTED;
		}
		if ((uint8_t)cloud_protocol_event_post_gun_status_ctrl[gun_index].gun_status != last_cp_status[gun_index])
		{
			last_cp_status[gun_index] = (uint8_t)cloud_protocol_event_post_gun_status_ctrl[gun_index].gun_status;
			cloud_protocol_event_post_gun_status_ctrl[gun_index].status_changed = true;
			Cloud_Protocol_EventPost_ForceTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_STATUS);
		}
	}
}
/* EOL */
