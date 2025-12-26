//******************************************************************************
//* File Name: Cloud_Protocol_Sg_RemoteCharge.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Remote Charge module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_RemoteCharge.h"
#include "Cloud_Protocol_Sg_ChargingOrder.h"

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
static cloud_protocol_sg_remote_charge_t cloud_protocol_sg_remote_charge = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool Cloud_Protocol_Sg_RemoteStart_ParseTimesArray(cJSON *times_json, startcharge_times *times);
static uint8_t Cloud_Protocol_Sg_RemoteCharge_GetGunIndex(uint8_t gun_no);
static cJSON *Cloud_Protocol_Sg_RemoteStart_AckPack(const v2g_service_feedback_remoteStart *feedback);
static void Cloud_Protocol_Send_ChargeStartResponse(uint8_t gun_no, char* message_id, bool success);
static void Cloud_Protocol_Sg_RemoteStart_PrintV2GServiceJson(const v2g_service_remoteStart *pStart);
static cJSON *Cloud_Protocol_Sg_RemoteStop_AckPack(const v2g_service_feedback_remoteStop *feedback);
static void Cloud_Protocol_Sg_RemoteStop_PrintV2GServiceJson(const v2g_service_remoteStop *pStart);
static void Cloud_Protocol_Send_ChargeStopResponse(v2g_service_remoteStop *stop_param, char *message_id, bool success, char *reason);
static void Cloud_Protocol_Stop_ChargeProcess(uint8_t gun_no);
static void Cloud_Protocol_Sg_RemoteCharge_CheckTime(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Initialize the remote charge management module
 * @return Returns true on success, false on failure
 */
bool Cloud_Protocol_Sg_RemoteCharge_Init(void)
{
    memset(&cloud_protocol_sg_remote_charge, 0, sizeof(cloud_protocol_sg_remote_charge_t));
    return true;
}

/**
 * @brief deinitialize the remote charge management module
 * @param gun_no Gun number (1 or 2)
 * @return Returns true on success, false on failure
 */
bool Cloud_Protocol_Sg_RemoteCharge_Deinit(uint8_t gun_no)
{
    // Force stop ongoing charging/discharging
    uint8_t gun_index = Cloud_Protocol_Sg_RemoteCharge_GetGunIndex(gun_no);

    memset(&cloud_protocol_sg_remote_charge.charge_param[gun_index], 0, sizeof(v2g_service_remoteStart));
    CLOUD_INFO("<%s>Remote charge deinitialized for gun %d\r\n", __func__, gun_no);
    return true;
}

/**
 * @brief Obtain the index (0 or 1) of the gun
 * @param gun_no Gun number (1 or 2)
 * @return Gun index (0 or 1), returns CLOUD_PROTOCOL_SG_INVALID_GUN_INDEX if invalid
 */
static uint8_t Cloud_Protocol_Sg_RemoteCharge_GetGunIndex(uint8_t gun_no)
{
    if (gun_no >= 1 && gun_no <= CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX)
    {
        return gun_no - 1; // Gun number 1 corresponds to index 0, gun number 2 corresponds to index 1
    }
    return CLOUD_PROTOCOL_SG_INVALID_GUN_INDEX;
}

/**
 * @brief Obtain the gun number (1 or 2)
 * @param gun_index Gun index (0 or 1)
 * @return Gun number (1 or 2), returns 0 if invalid
 */
static uint8_t Cloud_Protocol_Sg_RemoteCharge_GetGunNo(uint8_t gun_index)
{
    if (gun_index < CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX)
    {
        return gun_index + 1; // Index 0 corresponds to gun number 1, index 1 corresponds to gun number 2
    }
    return 0;
}

/**
 * @brief parse the times array from remote start parameters
 * @param times_json JSON array of time slots
 * @param times Pointer to startcharge_times structure to store parsed data
 * @return Returns true on success, false on failure
 */
static bool Cloud_Protocol_Sg_RemoteStart_ParseTimesArray(cJSON *times_json, startcharge_times *times)
{
    int array_size = cJSON_GetArraySize(times_json);
    if (array_size <= 0)
    {
        CLOUD_ERROR("<%s>Times array is empty\r\n", __FUNCTION__);
        return false;
    }

    cJSON *first_time_item = cJSON_GetArrayItem(times_json, 0);
    if (!first_time_item || !cJSON_IsObject(first_time_item))
    {
        CLOUD_ERROR("<%s>Invalid first time item in array\r\n", __FUNCTION__);
        return false;
    }

    cJSON *item;

    // parse begin time
    item = cJSON_GetObjectItem(first_time_item, "beginTime");
    if (!item || !cJSON_IsString(item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'beginTime' in first time slot\r\n", __FUNCTION__);
        return false;
    }
    strncpy(times->beginTime, item->valuestring, V2G_MAX_TIMESTAMP_LEN - 1);
    times->beginTime[V2G_MAX_TIMESTAMP_LEN - 1] = '\0';

    // parse end time
    item = cJSON_GetObjectItem(first_time_item, "endTime");
    if (!item || !cJSON_IsString(item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'endTime' in first time slot\r\n", __FUNCTION__);
        return false;
    }
    strncpy(times->endTime, item->valuestring, V2G_MAX_TIMESTAMP_LEN - 1);
    times->endTime[V2G_MAX_TIMESTAMP_LEN - 1] = '\0';

    // parse direction
    item = cJSON_GetObjectItem(first_time_item, "direction");
    if (!item || !cJSON_IsNumber(item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'direction' in first time slot\r\n", __FUNCTION__);
        return false;
    }
    times->direction = (unsigned char)item->valueint;

    // validate direction value (1=discharge, 2=charge, 3=pause)
    if (times->direction < 1 || times->direction > 3)
    {
        CLOUD_ERROR("<%s>Invalid direction value %d (1=discharge,2=charge,3=pause)\r\n", __FUNCTION__, times->direction);
        return false;
    }

    // parse power
    item = cJSON_GetObjectItem(first_time_item, "power");
    if (!item || !cJSON_IsNumber(item))
    {
        CLOUD_ERROR("<%s>Missing or invalid 'power' in first time slot\r\n", __FUNCTION__);
        return false;
    }
    // parse power
    times->power = (unsigned int)(item->valuedouble);

    // validate power value
    if (times->power == 0)
    {
        CLOUD_ERROR("<%s>Invalid power value 0\r\n", __FUNCTION__);
        return false;
    }

    return true;
}

/*
 * @brief Pack the acknowledgment for remote start charge service
 * @param feedback Pointer to v2g_service_feedback_remoteStart structure containing feedback data
 * @return JSON object representing the acknowledgment
 */
static cJSON *Cloud_Protocol_Sg_RemoteStart_AckPack(const v2g_service_feedback_remoteStart *feedback)
{
    if (!feedback)
        return NULL;

    cJSON *root = cJSON_CreateObject();
    if (!root)
        return NULL;

    cJSON_AddNumberToObject(root, "gunNo", feedback->gunNo);
    cJSON_AddStringToObject(root, "applyNo", feedback->applyNo);
    cJSON_AddStringToObject(root, "VIN", feedback->VIN);
    cJSON_AddNumberToObject(root, "result", feedback->result);
    cJSON_AddStringToObject(root, "resultDes", feedback->resultDes);

    return root;
}

/**
 * @brief Send charge start response to cloud platform\
 * @param gun_no Gun number
 * @param message_id Message ID
 * @param success Whether the start was successful
 */
static void Cloud_Protocol_Send_ChargeStartResponse(uint8_t gun_no, char* message_id, bool success)
{
    uint8_t gun_index = Cloud_Protocol_Sg_RemoteCharge_GetGunIndex(gun_no);
    v2g_service_remoteStart *start_param = &cloud_protocol_sg_remote_charge.charge_param[gun_index];
    v2g_service_feedback_remoteStart feedback = {0};
    
    feedback.gunNo = gun_no;
    strncpy(feedback.applyNo, start_param->applyNo, V2G_MAX_TRADE_LEN - 1);
    feedback.applyNo[V2G_MAX_TRADE_LEN - 1] = '\0';
    strncpy(feedback.VIN, start_param->VIN, V2G_MAX_CAR_VIN_LEN - 1);
    feedback.VIN[V2G_MAX_CAR_VIN_LEN - 1] = '\0';

    if (success)
    {
        feedback.result = 0; // success
        strncpy(feedback.resultDes, "issue successful", V2G_MAX_RESULT_LEN - 1);
        feedback.resultDes[V2G_MAX_RESULT_LEN - 1] = '\0';
    }
    else
    {
        feedback.result = 1; // failure
        strncpy(feedback.resultDes, "issue failed", V2G_MAX_RESULT_LEN - 1);
        feedback.resultDes[V2G_MAX_RESULT_LEN - 1] = '\0';
    }
    cJSON *object = Cloud_Protocol_Sg_RemoteStart_AckPack(&feedback);
    if (!object)
    {
        CLOUD_ERROR("<%s>: Failed to create JSON object\r\n", __FUNCTION__);
        return;
    }

    cloud_protocol_service_call_response(message_id, "issueChargeOrDischargePlanSrv", CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS, object);

    cJSON_Delete(object);

    if (success)
    {
        CLOUD_INFO("<%s> gun_no=%d, result=success\r\n", __FUNCTION__, gun_no);
    }
    else
    {
        CLOUD_ERROR("<%s> gun_no=%d, result=failure\r\n", __FUNCTION__, gun_no);
    }
}

/**
 * @brief Parse remote start charge/discharge service parameters
 * @param params JSON object containing service parameters
 * @param msg_id Message ID
 * @return Returns true on success, false on failure
 */
bool Cloud_Protocol_Sg_ParseRemoteStartParam(cJSON *params, const char *msg_id)
{
    if (!params)
    {
        return false;
    }

    cJSON *root = params;
    if (!root)
    {
        CLOUD_ERROR("<%s>JSON parse failed\r\n", __FUNCTION__);
        return false;
    }

    v2g_service_remoteStart *remote_start = NULL;
    cJSON *item;
    uint8_t gun_index;
    bool parse_success = true;

    // Parse gun number
    item = cJSON_GetObjectItem(root, "gunNo");
    if (item && cJSON_IsNumber(item))
    {
        gun_index = Cloud_Protocol_Sg_RemoteCharge_GetGunIndex((unsigned char)item->valueint);
        if (gun_index != 0 && gun_index != 1)
        {
            CLOUD_ERROR("<%s>Invalid gun number: %d\r\n", __FUNCTION__, (unsigned char)item->valueint);
            return false;
        }
        remote_start = &cloud_protocol_sg_remote_charge.charge_param[gun_index];
        remote_start->gunNo = (unsigned char)item->valueint;
        cloud_protocol_sg_remote_charge.is_enabled[gun_index] = true;
    }

    // Parse application/order number
    item = cJSON_GetObjectItem(root, "applyNo");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_start->applyNo, item->valuestring, V2G_MAX_TRADE_LEN - 1);
        remote_start->applyNo[V2G_MAX_TRADE_LEN - 1] = '\0';
    }

    // Parse user ID
    item = cJSON_GetObjectItem(root, "userId");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_start->userId, item->valuestring, V2G_MAX_USERID_LEN - 1);
        remote_start->userId[V2G_MAX_USERID_LEN - 1] = '\0';
    }

    // Parse vehicle VIN
    item = cJSON_GetObjectItem(root, "VIN");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_start->VIN, item->valuestring, V2G_MAX_CAR_VIN_LEN - 1);
        remote_start->VIN[V2G_MAX_CAR_VIN_LEN - 1] = '\0';
    }

    // Parse decision type
    item = cJSON_GetObjectItem(root, "decisionType");
    if (item && cJSON_IsNumber(item))
    {
        remote_start->decisionType = (unsigned char)item->valueint;
    }

    // Parse decision time
    item = cJSON_GetObjectItem(root, "decisionTime");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_start->decisionTime, item->valuestring, V2G_MAX_TIMESTAMP_LEN - 1);
        remote_start->decisionTime[V2G_MAX_TIMESTAMP_LEN - 1] = '\0';
    }

    // Parse start/charge time unit
    item = cJSON_GetObjectItem(params, "times");
    if (item && cJSON_IsArray(item))
    {
        if (!Cloud_Protocol_Sg_RemoteStart_ParseTimesArray(item, &remote_start->times))
        {
            CLOUD_ERROR("<%s>Failed to parse times array\r\n", __FUNCTION__);
            parse_success = false;
            cloud_protocol_sg_remote_charge.is_enabled[gun_index] = false;
        }
    }

    if (remote_start->times.direction == 3)
    {
        parse_success = Cloud_Protocol_Sg_Order_Pause(remote_start->gunNo, remote_start->applyNo);
    }
    Cloud_Protocol_Sg_RemoteStart_PrintV2GServiceJson(remote_start);
    Cloud_Protocol_Send_ChargeStartResponse(remote_start->gunNo, (char *)msg_id, parse_success);

    return true;
}

/**
 * @brief Print remote stop charge service parameters in JSON format for debugging
 * @param pStart Pointer to v2g_service_remoteStart structure containing start parameters
 */
static void Cloud_Protocol_Sg_RemoteStart_PrintV2GServiceJson(const v2g_service_remoteStart *pStart)
{
    if (pStart == NULL) return;
    
    CLOUD_INFO("<%s> {\r\n", __FUNCTION__);
    CLOUD_INFO("  \"gunNo\": %u,\r\n", (unsigned int)pStart->gunNo);
    CLOUD_INFO("  \"applyNo\": \"%s\",\r\n", pStart->applyNo);
    CLOUD_INFO("  \"userId\": \"%s\",\r\n", pStart->userId);
    CLOUD_INFO("  \"VIN\": \"%s\",\r\n", pStart->VIN);
    CLOUD_INFO("  \"decisionType\": %u,\r\n", (unsigned int)pStart->decisionType);
    CLOUD_INFO("  \"decisionTime\": \"%s\",\r\n", pStart->decisionTime);
    CLOUD_INFO("  \"times\": {\r\n");
    CLOUD_INFO("    \"beginTime\": \"%s\",\r\n", pStart->times.beginTime);
    CLOUD_INFO("    \"endTime\": \"%s\"\r\n", pStart->times.endTime);
    CLOUD_INFO("    \"direction\": %u,\r\n", (unsigned int)pStart->times.direction);
    CLOUD_INFO("    \"power\": %u\r\n", (unsigned int)pStart->times.power);
    CLOUD_INFO("  }\r\n");
    CLOUD_INFO("}\r\n");
}

/*
 * @brief Pack the acknowledgment for remote stop charge service
 * @param feedback Pointer to v2g_service_feedback_remoteStop structure containing feedback data
 * @return JSON object representing the acknowledgment
 */
static cJSON *Cloud_Protocol_Sg_RemoteStop_AckPack(const v2g_service_feedback_remoteStop *feedback)
{
    if (!feedback)
        return NULL;

    cJSON *root = cJSON_CreateObject();
    if (!root)
        return NULL;
    cJSON_AddNumberToObject(root, "result", feedback->result);
    cJSON_AddStringToObject(root, "resultDes", feedback->resultDes);
    cJSON_AddStringToObject(root, "applySheetNo", feedback->applySheetNo);

    return root;
}

static void Cloud_Protocol_Sg_RemoteStop_PrintV2GServiceJson(const v2g_service_remoteStop *pStart)
{
    if (pStart == NULL) return;
    
    CLOUD_INFO("<%s> {\r\n", __FUNCTION__);
    CLOUD_INFO("  \"gunNo\": %u,\r\n", (unsigned int)pStart->gunNo);
    CLOUD_INFO("  \"applySheetNo\": \"%s\",\r\n", pStart->applySheetNo);
    CLOUD_INFO("  \"VIN\": \"%s\"\r\n", pStart->VIN);
    CLOUD_INFO("}\r\n");
}

/**
 * @brief Send charge stop response to cloud platform
 * @param stop_param Pointer to remote stop parameters
 * @param message_id Message ID
 * @param success Whether the stop was successful
 * @param reason Reason for failure (if any)
 */
static void Cloud_Protocol_Send_ChargeStopResponse(v2g_service_remoteStop *stop_param, char *message_id, bool success, char *reason)
{
    v2g_service_feedback_remoteStop feedback = {0};

    strncpy(feedback.applySheetNo, stop_param->applySheetNo, V2G_MAX_TRADE_LEN - 1);
    feedback.applySheetNo[V2G_MAX_TRADE_LEN - 1] = '\0';

    feedback.result = success;
    strncpy(feedback.resultDes, reason, V2G_MAX_RESULT_LEN - 1);
    feedback.resultDes[V2G_MAX_RESULT_LEN - 1] = '\0';

    cJSON *object = Cloud_Protocol_Sg_RemoteStop_AckPack(&feedback);
    if (!object)
    {
        CLOUD_ERROR("<%s>: Failed to create JSON object\r\n", __FUNCTION__);
        return;
    }

    cloud_protocol_service_call_response(message_id, "stopChargeOrDischargeSrv", CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS, object);

    cJSON_Delete(object);

    if (success)
    {
        Cloud_Protocol_Stop_ChargeProcess(stop_param->gunNo);
    }
}

/**
 * @brief Stop the charge/discharge process for a specific gun
 * @param gun_no Gun number (1 or 2)
 */
static void Cloud_Protocol_Stop_ChargeProcess(uint8_t gun_no)
{
    uint8_t gun_index = Cloud_Protocol_Sg_RemoteCharge_GetGunIndex(gun_no);
    if (gun_index != 0 && gun_index != 1)
    {
        CLOUD_ERROR("<%s> Invalid gun number: %d\r\n", __FUNCTION__, gun_no);
        return;
    }

    // Clear authentication status and parameters
    memset(&cloud_protocol_sg_remote_charge.charge_param[gun_index], 0, sizeof(v2g_service_remoteStart));
    CLOUD_INFO("<%s> Stopped charge/discharge process for gun %d\r\n", __FUNCTION__, gun_no);
}

/*
    * @brief Parse remote stop charge/discharge service parameters
    * @param params JSON object containing service parameters
    * @param msg_id Message ID
    * @return Returns true on success, false on failure
*/
bool Cloud_Protocol_Sg_ParseRemoteStopParam(cJSON *params, const char *msg_id)
{
    if (!params)
    {
        return false;
    }

    cJSON *root = params;
    if (!root)
    {
        CLOUD_ERROR("<%s>JSON parse failed\r\n", __FUNCTION__);
        return false;
    }

    v2g_service_remoteStop remote_stop = {0};
    cJSON *item;

    // Parse gun number
    item = cJSON_GetObjectItem(root, "gunNo");
    if (item && cJSON_IsNumber(item))
    {
        remote_stop.gunNo = (unsigned char)item->valueint;
    }

    // Parse vehicle VIN
    item = cJSON_GetObjectItem(root, "VIN");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_stop.VIN, item->valuestring, V2G_MAX_CAR_VIN_LEN - 1);
        remote_stop.VIN[V2G_MAX_CAR_VIN_LEN - 1] = '\0';
    }

    // Parse application/order number
    item = cJSON_GetObjectItem(root, "applySheetNo");
    if (item && cJSON_IsString(item))
    {
        strncpy(remote_stop.applySheetNo, item->valuestring, V2G_MAX_TRADE_LEN - 1);
        remote_stop.applySheetNo[V2G_MAX_TRADE_LEN - 1] = '\0';
    }
    char resultDes[V2G_MAX_RESULT_LEN] = {0};

    bool result = Cloud_Protocol_Sg_Order_Stop(remote_stop.gunNo, remote_stop.applySheetNo, resultDes);

    Cloud_Protocol_Sg_RemoteStop_PrintV2GServiceJson(&remote_stop);
    Cloud_Protocol_Send_ChargeStopResponse(&remote_stop, (char *)msg_id, result, resultDes);

    cloud_protocol_sg_remote_charge.is_enabled[remote_stop.gunNo - 1] = true;
    return true;
}

/**
 * @brief monitor charging status and stop orders if auth lost
 */
static void Cloud_Protocol_Sg_MonitoringStopOrder(uint8_t gun_index)
{
    static uint8_t last_auth_status[CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX] = {0};
    uint8_t auth_status = AUTHM_GetCurrAuthStatus((SysConnector_Num_Enum)gun_index);
    uint8_t gun_no = Cloud_Protocol_Sg_RemoteCharge_GetGunNo(gun_index);
    // Check authentication status
    if (auth_status != last_auth_status[gun_index])
    {
        if (auth_status == false && last_auth_status[gun_index] == true)
        {
            CLOUD_INFO("<%s> Gun %d lost authentication, stop order\r\n", __func__, gun_index);
            char resultDes[V2G_MAX_RESULT_LEN] = {0};
            Cloud_Protocol_Sg_Order_Stop(gun_no, cloud_protocol_sg_remote_charge.charge_param[gun_index].applyNo, resultDes);
            cloud_protocol_sg_remote_charge.is_authenticated[gun_index] = false;
            memset(&cloud_protocol_sg_remote_charge.charge_param[gun_index], 0, sizeof(v2g_service_remoteStart));
        }
        last_auth_status[gun_index] = auth_status;
    }
}

/**
 * @brief Check for charge/discharge timeout and stop if necessary
 * @return NULL
 */
static void Cloud_Protocol_Sg_RemoteCharge_CheckTime(void)
{
    uint64_t current_time = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // Convert to milliseconds
    uint64_t start_time = 0, end_time = 0;

    // Check each gun for timeout
    for (int i = 0; i < CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX; i++)
    {
        Cloud_Protocol_Sg_MonitoringStopOrder(i);
        if (!cloud_protocol_sg_remote_charge.is_enabled[i])
        {
            continue; // Remote charge not enabled for this gun
        }

        v2g_service_remoteStart *charge_param = &cloud_protocol_sg_remote_charge.charge_param[i];
        start_time = strtoull(charge_param->times.beginTime, NULL, 10);
        end_time = strtoull(charge_param->times.endTime, NULL, 10);

        // Check start time
        if (!cloud_protocol_sg_remote_charge.is_authenticated[i])
        {
            if (end_time > start_time && current_time >= start_time)
            {
                cloud_protocol_sg_remote_charge.is_authenticated[i] = true;
                NETAUTH_SetReqAuthStatus((SysConnector_Num_Enum)i);
                cloud_protocol_sg_order_op_t operation = CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE;
                if (charge_param->times.direction == 1)
                {
                    operation = CLOUD_PROTOCOL_SG_ORDER_OP_DISCHARGE;
                    Cloud_Protocol_Sg_Order_Start(charge_param->gunNo, charge_param->applyNo, operation);
                }
                else if (charge_param->times.direction == 2)
                {
                    operation = CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE;
                    Cloud_Protocol_Sg_Order_Start(charge_param->gunNo, charge_param->applyNo, operation);
                }
                else
                {
                }
                CLOUD_INFO("<%s> Gun %d charge/discharge time started, authenticating\r\n", __func__, Cloud_Protocol_Sg_RemoteCharge_GetGunNo(i));
                continue;
            }
        }
        else if (cloud_protocol_sg_remote_charge.is_authenticated[i])
        {
            if (end_time < current_time)
            {
                cloud_protocol_sg_remote_charge.is_authenticated[i] = false;
                cloud_protocol_sg_remote_charge.is_enabled[i] = false;
                if (AUTHM_GetAuthOpenSource((SysConnector_Num_Enum)i) == AUTHM_OPEN_SRC_NET_APP)
                {
                    NETAUTH_SetReqCancelAuthStatus((SysConnector_Num_Enum)i);
                }  
                CLOUD_INFO("<%s> Gun %d charge/discharge time ended, stopping\r\n", __func__, Cloud_Protocol_Sg_RemoteCharge_GetGunNo(i));
                continue;
            }
        }
        else
        {
            // Not authenticated and not enabled, do nothing
            continue;
        }
    }
}

/**
 * @brief Periodic task to check for remote charge timeouts
 */
void Cloud_Protocol_Sg_RemoteCharge_PeriodicTask(void)
{
    Cloud_Protocol_Sg_RemoteCharge_CheckTime();
}
/* EOL */
