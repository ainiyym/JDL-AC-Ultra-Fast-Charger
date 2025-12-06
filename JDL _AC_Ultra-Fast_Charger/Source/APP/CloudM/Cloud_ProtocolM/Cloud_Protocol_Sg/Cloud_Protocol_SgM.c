//******************************************************************************
//* File Name: Cloud_Protocol_SgM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_SgM.h"
#include "Cloud_Protocol_Mqtt_Cfg.h"
#include "Cloud_Protocol_Sg_Login.h"
#include "Cloud_Protocol_Sg_SynchronizeNetTime.h"
#include "Cloud_Protocol_Sg_ServiceCall.h"
#include "Cloud_Protocol_Sg_ReportResponse.h"
#include "Cloud_Protocol_Sg_PropertySetting.h"

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
static iotx_dev_meta_info_t Cloud_Protocol_Meta;
static iotx_sign_mqtt_t Cloud_Protocol_Sign;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static cloud_protocol_sg_message_type_e cloud_protocol_sg_detect_message_type(const char *payload);
static bool Cloud_Protocol_Mqtt_PayloadCallback(const char *payload);
static void Cloud_Protocol_Mqtt_ConnectCallback(bool connected);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Detect message type from payload
 */
static cloud_protocol_sg_message_type_e cloud_protocol_sg_detect_message_type(const char *payload)
{
    if (!payload)
    {
        return MESSAGE_TYPE_UNKNOWN;
    }

    cJSON *root = cJSON_Parse(payload);
    if (!root)
    {
        return MESSAGE_TYPE_UNKNOWN;
    }

    cloud_protocol_sg_message_type_e type = MESSAGE_TYPE_UNKNOWN;

    // check time sync fields
    if (cJSON_GetObjectItem(root, "deviceSendTime") &&
        cJSON_GetObjectItem(root, "serverSendTime") &&
        cJSON_GetObjectItem(root, "serverRecvTime"))
    {
        type = MESSAGE_TYPE_TIME_SYNC;
    }
    // check service call fields
    else if (cJSON_GetObjectItem(root, "method") &&
             cJSON_GetObjectItem(root, "id") &&
             cJSON_GetObjectItem(root, "params") &&
             cJSON_GetObjectItem(root, "version"))
    {
        type = MESSAGE_TYPE_SERVICE_CALL;
    }
    // check report response fields
    else if (cJSON_GetObjectItem(root, "id") &&
             cJSON_GetObjectItem(root, "code") &&
             cJSON_GetObjectItem(root, "data"))
    {
        type = MESSAGE_TYPE_REPORT_RESP;
    }
    // check property setting fields
    // nothing to do yet
    // check OTA info response fields
    else if (cJSON_GetObjectItem(root, "id") &&
             cJSON_GetObjectItem(root, "code") &&
             cJSON_GetObjectItem(root, "version") &&
             cJSON_GetObjectItem(root, "url") &&
             cJSON_GetObjectItem(root, "sign"))
    {
        type = MESSAGE_TYPE_OTA_INFO_RESP;
    }
    else
    {
        type = MESSAGE_TYPE_UNKNOWN;
    }

    cJSON_Delete(root);
    return type;
}

/**
 * @brief MQTT payloadCallback function - main entry
 */
static bool Cloud_Protocol_Mqtt_PayloadCallback(const char *payload)
{
    if (!payload)
    {
        CLOUD_ERROR("Received NULL payload\n");
        return false;
    }

    bool response = false;
    // CLOUD_DEBUG("Raw payload received: %s\n", payload);

    // Detection message type
    cloud_protocol_sg_message_type_e msg_type = cloud_protocol_sg_detect_message_type(payload);

    switch (msg_type)
    {
        case MESSAGE_TYPE_TIME_SYNC:
            return cloud_protocol_sysnchronize_net_time_handle_response(payload, strlen(payload));

        case MESSAGE_TYPE_SERVICE_CALL:
            return cloud_protocol_service_call(payload, strlen(payload));

        case MESSAGE_TYPE_REPORT_RESP:
            return cloud_protocol_report_response(payload, strlen(payload));

        case MESSAGE_TYPE_PROPERTY_SETTING:
            return cloud_protocol_property_setting_response(payload, strlen(payload));

        case MESSAGE_TYPE_OTA_INFO_RESP:
            // Handle OTA info response
            break;
        case MESSAGE_TYPE_UNKNOWN:
            CLOUD_WARN("<%s> Received unknown message type, payload: %.*s\n", __func__, (int)strlen(payload), payload);
            break;
        default:
            CLOUD_WARN("Unknown message type, payload: %s\n", payload);
            break;
    }
    return response;
}

/**
 * @brief MQTT connect callback function
 */
static void Cloud_Protocol_Mqtt_ConnectCallback(bool connected)
{
	Cloud_Protocol_Sg_SynchronizeNetTime_SetNetworkConnectStatus(connected);
    Cloud_Protocol_EventPost_SetFwInfoNetConnectedFlag(connected);
    Cloud_Protocol_EventPost_SetVersionInfoNetConnectedFlag(connected);
}

/**
 * @brief Network time synchronization callback
 */
static void Cloud_Protocol_Mqtt_NetTimeCallback(bool success, int64_t time_offset, uint64_t round_trip_delay, uint64_t timestamp)
{
    if (!success)
    {
        CLOUD_WARN("<%s> NTP Time Sync Failed\n", __func__);
        return;
    }
    uint32_t time_in_seconds = timestamp / 1000;

    RTC_SecondsSetRtcDateTime(time_in_seconds);
    CLOUD_INFO("<%s> NTP Time Sync Successful:\n", __func__);
    CLOUD_INFO("Time Offset: %lld ms\n", time_offset);
    CLOUD_INFO("Round Trip Delay: %llu ms\n", round_trip_delay);
    CLOUD_INFO("Calculated Server Time(s): %u\n", time_in_seconds);

    static bool first_sync_done = false;
    if (!first_sync_done)
    {
        first_sync_done = true;
        Cloud_Protocol_EventPost_SetFwInfoRefreshFlag(true);
        Cloud_Protocol_EventPost_SetVersionInfoRefreshFlag(true);
    }
}

/**
 * @brief MQTT initialization function
 */
void Cloud_Protocol_Mqtt_init(void)
{
    uint64_t timestamp = CLOUDM_SG_PRODUCT_TIMESTAMP_VALUE; // Fixed timestamp

    memset(&Cloud_Protocol_Meta, 0, sizeof(iotx_dev_meta_info_t));
    memset(&Cloud_Protocol_Sign, 0, sizeof(iotx_sign_mqtt_t));

    strncpy(Cloud_Protocol_Meta.product_key, CLOUDM_SG_PRODUCT_KEY, IOTX_PRODUCT_KEY_LEN);
    strncpy(Cloud_Protocol_Meta.device_name, CLOUDM_SG_DEVICE_NAME, IOTX_DEVICE_NAME_LEN);
    strncpy(Cloud_Protocol_Meta.device_secret, CLOUDM_SG_DEVICE_SECRET, IOTX_DEVICE_SECRET_LEN);

    Cloud_Protocol_Mqtt_Sign(&Cloud_Protocol_Meta, timestamp, &Cloud_Protocol_Sign);
    Cloud_Protocol_Mqtt_ClientManagerInit(cloud_protocol_mqtt_passive_topic_configs, CLOUD_PROTOCOL_MQTT_PASSIVE_TOPIC_CONFIG_MAXIMUM\
                                           , &Cloud_Protocol_Sign, Cloud_Protocol_Mqtt_ConnectCallback, Cloud_Protocol_Mqtt_PayloadCallback);
    cloud_protocol_sysnchronize_net_time_init(CLOUDM_SG_PRODUCT_KEY, CLOUDM_SG_DEVICE_NAME, Cloud_Protocol_Mqtt_NetTimeCallback);
    Cloud_Protocol_EventPost_FwInfo_Init();
    Cloud_Protocol_EventPost_VersionInfo_Init();
}

/**
 * @brief MQTT main process function
 */
void Cloud_Protocol_Mqtt_MainProcess(void)
{
    /* Synchronize network time if needed */
    Cloud_Protocol_Sg_SynchronizeNetTime_Main();
    // MQTT client manager process
    Cloud_Protocol_Mqtt_ClientManagerProcess();
    /* event post */
    Cloud_Protocol_EventPost_FwInfoMainCtrl_Func();
    Cloud_Protocol_EventPost_VersionInfoMainCtrl_Func();
}
/* EOL */
