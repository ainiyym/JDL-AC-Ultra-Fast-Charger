//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ConfigUpdate.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Config Update module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ConfigUpdate.h"
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
static v2g_data_dev_config cloud_protocol_sg_config_update = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_Send_ConfigUpdateParamResponse(char *message_id, bool success);
static void Cloud_Protocol_Send_QueryConfigParamResponse(char *message_id, bool success);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Initialize configuration with default values
 */
void Cloud_Protocol_Sg_Config_Init(void)
{
    cloud_protocol_sg_config_update.gunInfoFreq = CLOUD_PROTOCOL_SG_DEFAULT_GUN_INFO_FREQ;
    cloud_protocol_sg_config_update.vehicleInfoFreq = CLOUD_PROTOCOL_SG_DEFAULT_VEHICLE_INFO_FREQ;
    cloud_protocol_sg_config_update.batteryInfoFreq = CLOUD_PROTOCOL_SG_DEFAULT_BATTERY_INFO_FREQ;
    cloud_protocol_sg_config_update.pileWarnings = CLOUD_PROTOCOL_SG_DEFAULT_PILE_WARNINGS_FREQ;
    cloud_protocol_sg_config_update.vehicleWarnings = CLOUD_PROTOCOL_SG_DEFAULT_VEHICLE_WARNINGS_FREQ;
    cloud_protocol_sg_config_update.offlinChaLen = CLOUD_PROTOCOL_SG_DEFAULT_OFFLINE_CHARGE_LEN;
    cloud_protocol_sg_config_update.grndLock = CLOUD_PROTOCOL_SG_DEFAULT_GRND_LOCK_FREQ;
    cloud_protocol_sg_config_update.doorLock = CLOUD_PROTOCOL_SG_DEFAULT_DOOR_LOCK_FREQ;

    Cloud_Protocol_EventPost_Init();
    Cloud_Protocol_EventPost_UpdateConfig(&cloud_protocol_sg_config_update);
}

/*
    * @brief Parse configuration update parameters from JSON
    * @param params JSON object containing service parameters
    * @param msg_id Message ID
    * @return Returns true on success, false on failure
*/
bool Cloud_Protocol_Sg_ParseConfigUpdateParam(cJSON *params, const char *msg_id)
{
    if (!params)
    {
        CLOUD_ERROR("<%s> params is NULL\r\n", __FUNCTION__);
        return false;
    }
    cJSON *root = params;

    // copy current config to backup
    // v2g_data_dev_config backup_config = cloud_protocol_sg_config_update;

    cJSON *item_gunInfoFreq = cJSON_GetObjectItem(root, "gunInfoFreq");
    if (item_gunInfoFreq != NULL && cJSON_IsNumber(item_gunInfoFreq))
    {
        cloud_protocol_sg_config_update.gunInfoFreq = item_gunInfoFreq->valueint;
    }

    cJSON *item_vehicleInfoFreq = cJSON_GetObjectItem(root, "vehicleInfoFreq");
    if (item_vehicleInfoFreq != NULL && cJSON_IsNumber(item_vehicleInfoFreq))
    {
        cloud_protocol_sg_config_update.vehicleInfoFreq = item_vehicleInfoFreq->valueint;
    }

    cJSON *item_batteryInfoFreq = cJSON_GetObjectItem(root, "batteryInfoFreq");
    if (item_batteryInfoFreq != NULL && cJSON_IsNumber(item_batteryInfoFreq))
    {
        cloud_protocol_sg_config_update.batteryInfoFreq = item_batteryInfoFreq->valueint;
    }

    cJSON *item_pileWarnings = cJSON_GetObjectItem(root, "pileWarnings");
    if (item_pileWarnings != NULL && cJSON_IsNumber(item_pileWarnings))
    {
        cloud_protocol_sg_config_update.pileWarnings = item_pileWarnings->valueint;
    }

    cJSON *item_vehicleWarnings = cJSON_GetObjectItem(root, "vehicleWarnings");
    if (item_vehicleWarnings != NULL && cJSON_IsNumber(item_vehicleWarnings))
    {
        cloud_protocol_sg_config_update.vehicleWarnings = item_vehicleWarnings->valueint;
    }

    cJSON *item_offlinChaLen = cJSON_GetObjectItem(root, "offlinChaLen");
    if (item_offlinChaLen != NULL && cJSON_IsNumber(item_offlinChaLen))
    {
        cloud_protocol_sg_config_update.offlinChaLen = item_offlinChaLen->valueint;
    }

    cJSON *item_grndLock = cJSON_GetObjectItem(root, "grndLock");
    if (item_grndLock != NULL && cJSON_IsNumber(item_grndLock))
    {
        cloud_protocol_sg_config_update.grndLock = item_grndLock->valueint;
    }

    cJSON *item_doorLock = cJSON_GetObjectItem(root, "doorLock");
    if (item_doorLock != NULL && cJSON_IsNumber(item_doorLock))
    {
        cloud_protocol_sg_config_update.doorLock = item_doorLock->valueint;
    }

    cJSON *item_qrCode = cJSON_GetObjectItem(root, "qrCode");
    if (item_qrCode != NULL && cJSON_IsArray(item_qrCode))
    {
        cJSON *item_arrayData;
        int qrcode_len = 0;
        int qrcode_num = cJSON_GetArraySize(item_qrCode);
        qrcode_num = (qrcode_num <= V2G_MAX_PORT_NUM) ? qrcode_num : V2G_MAX_PORT_NUM;

        for (int i = 0; i < qrcode_num; i++)
        {
            item_arrayData = cJSON_GetArrayItem(item_qrCode, i);
            qrcode_len = strlen(item_arrayData->valuestring);
            if (qrcode_len < V2G_MAX_QRCODE_LEN)
            {
                memcpy(cloud_protocol_sg_config_update.qrCode[i], item_arrayData->valuestring, qrcode_len);
            }
            else
            {
                CLOUD_WARN("<%s>QRCode is too big!\r\n", __FUNCTION__);
            }
        }
    }

    // // save to storage
    // if (!Cloud_Protocol_Sg_Config_SaveToStorage())
    // {
    //     cloud_protocol_sg_config_update = backup_config; // restore from backup on failure
    //     CLOUD_WARN("<%s>Failed to save configuration to storage\r\n", __func__);
    // }

    Cloud_Protocol_EventPost_UpdateConfig(&cloud_protocol_sg_config_update);
    // send response
    Cloud_Protocol_Send_ConfigUpdateParamResponse((char *)msg_id, true);

    return true;
}

/**
 * @brief Send configuration update response to cloud platform
 * @param config_update_param Pointer to configuration update parameters
 * @param message_id Message ID
 * @param success Whether the update was successful
 */
static void Cloud_Protocol_Send_ConfigUpdateParamResponse(char *message_id, bool success)
{
    cJSON *response_data = cJSON_CreateObject();
    if (!response_data)
    {
        CLOUD_ERROR("<%s> Failed to create JSON object\r\n", __FUNCTION__);
        return;
    }

    int result_code = success ? 10 : 11; // 10: success, 11: failure
    cJSON_AddNumberToObject(response_data, "resCode", result_code);

    // send response
    cloud_protocol_service_call_response(message_id, "confUpdateSrv", CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS, response_data);

    // clean up
    cJSON_Delete(response_data);

    CLOUD_DEBUG("<%s> Configuration update response sent: success=%d, resCode=%d\n", __FUNCTION__, success, result_code);
}

/*
    * @brief query configuration parameters from JSON
    * @param params JSON object containing service parameters
    * @param msg_id Message ID
    * @return Returns true on success, false on failure
*/
bool Cloud_Protocol_Sg_ParseQueryConfigParam(cJSON *params, const char *msg_id)
{
    Cloud_Protocol_Send_QueryConfigParamResponse((char *)msg_id, true);
    return true;
}

/**
 * @brief Send query configuration response to cloud platform
 * @param message_id Message ID
 * @param success Whether the query was successful
 */
static void Cloud_Protocol_Send_QueryConfigParamResponse(char *message_id, bool success)
{
    cJSON *response_data = cJSON_CreateObject();
    cJSON *qrCodeArray;
    if (!response_data)
    {
        CLOUD_ERROR("<%s> Failed to create JSON object\r\n", __FUNCTION__);
        return;
    }
    cJSON_AddNumberToObject(response_data, "gunInfoFreq", cloud_protocol_sg_config_update.gunInfoFreq);
    cJSON_AddNumberToObject(response_data, "vehicleInfoFreq", cloud_protocol_sg_config_update.vehicleInfoFreq);
    cJSON_AddNumberToObject(response_data, "batteryInfoFreq", cloud_protocol_sg_config_update.batteryInfoFreq);
    cJSON_AddNumberToObject(response_data, "pileWarnings", cloud_protocol_sg_config_update.pileWarnings);
    cJSON_AddNumberToObject(response_data, "vehicleWarnings", cloud_protocol_sg_config_update.vehicleWarnings);
    cJSON_AddNumberToObject(response_data, "offlinChaLen", cloud_protocol_sg_config_update.offlinChaLen);
    cJSON_AddNumberToObject(response_data, "grndLock", cloud_protocol_sg_config_update.grndLock);
    cJSON_AddNumberToObject(response_data, "doorLock", cloud_protocol_sg_config_update.doorLock);

    cJSON_AddItemToObject(response_data, "qrCode", qrCodeArray = cJSON_CreateArray());
    for (int i = 0; i < V2G_MAX_PORT_NUM; i++)
    {
        cJSON_AddItemToArray(qrCodeArray, cJSON_CreateString(cloud_protocol_sg_config_update.qrCode[i]));
    }

   // send response
    cloud_protocol_service_call_response(message_id, "getConfSrv", CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS, response_data);

    // clean up
    cJSON_Delete(response_data);

    CLOUD_DEBUG("<%s> query config response sent: success=%d\n", __FUNCTION__, success);
}

/* EOL */
