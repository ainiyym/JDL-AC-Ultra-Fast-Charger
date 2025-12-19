//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_VersionInfo.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module version info post source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_EventPost_VersionInfo.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint32_t send_message_id; // Sent message ID
} cloud_protocol_event_post_version_info_ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
v2g_event_ver_info cloud_protocol_event_version_info;
cloud_protocol_event_post_version_info_ctrl_t cloud_protocol_event_post_version_info_ctrl;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_EventPost_VersionInfo_Init(void)
{
	memset(&cloud_protocol_event_version_info, 0, sizeof(cloud_protocol_event_version_info));
	memset(&cloud_protocol_event_post_version_info_ctrl, 0, sizeof(cloud_protocol_event_post_version_info_ctrl));
	cloud_protocol_event_version_info.devRegMethod = CLOUDM_SG_DEVICE_REG_METHOD; // Default device registration method
	strncpy(cloud_protocol_event_version_info.pileSoftwareVer, CLOUDM_SG_PILE_FW_SOFT_VERSION, V2G_MAX_SOFTWAREVER_LEN - 1);
	strncpy(cloud_protocol_event_version_info.pileHardwareVer, CLOUDM_SG_PILE_HW_VERSION, V2G_MAX_HARDWAREVER_LEN - 1);
	strncpy(cloud_protocol_event_version_info.sdkVer, CLOUDM_SG_SDK_VERSION, V2G_MAX_SDKVER_LEN - 1);
}

void Cloud_Protocol_EventPost_SetVersionInfoMsgId(uint32_t msg_id)
{
	cloud_protocol_event_post_version_info_ctrl.send_message_id = msg_id;
}

void Cloud_Protocol_EventPost_VersionInfo_Post(void)
{
	cloud_protocol_event_post_req_t *cloud_protocol_event_post_req = NULL;
	cJSON *root = NULL;
	uint64_t timestamp = 0;

	// create request
	cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("verInfoEvt", Cloud_Protocol_EventPost_SetVersionInfoMsgId);

	// build json header
	root = Cloud_Protocol_EventPost_BuildRequestJsonHeader(cloud_protocol_event_post_req);
	Cloud_Protocol_EventPost_DestroyRequest((cloud_protocol_event_post_req_t *)cloud_protocol_event_post_req);

	// get params->value object
	cJSON *params_obj = cJSON_GetObjectItem(root, "params");
	if (params_obj == NULL)
	{
		CLOUD_WARN("<%s %d> params object not found\r\n", __func__, __LINE__);
		return;
	}

	cJSON *value_obj = cJSON_GetObjectItem(params_obj, "value");
	if (value_obj == NULL)
	{
		CLOUD_WARN("<%s %d> value object not found\r\n", __func__, __LINE__);
		return;
	}

	cJSON_AddNumberToObject(value_obj, "devRegMethod", cloud_protocol_event_version_info.devRegMethod);
	cJSON_AddStringToObject(value_obj, "pileSoftwareVer", cloud_protocol_event_version_info.pileSoftwareVer);
	cJSON_AddStringToObject(value_obj, "pileHardwareVer", cloud_protocol_event_version_info.pileHardwareVer);
	cJSON_AddStringToObject(value_obj, "sdkVer", cloud_protocol_event_version_info.sdkVer);

	// print unformatted json string
	timestamp = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // convert to milliseconds
	Cloud_Protocol_EventPost_PrintUnformatted(root, timestamp, "verInfoEvt");

	cJSON_Delete(root);
	root = NULL;
}

bool Cloud_Protocol_EventPost_VersionInfo_HandleResponse(uint32_t msg_id)
{
	if (msg_id != cloud_protocol_event_post_version_info_ctrl.send_message_id)
	{
		CLOUD_WARN("<%s>Version info response msg_id mismatch: received=%u, expected=%u\r\n", __func__, msg_id, cloud_protocol_event_post_version_info_ctrl.send_message_id);
		return false;
	}
	return true;
}

/* EOL */
