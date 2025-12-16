//*******************************************************************************
//* File Name: Cloud_Protocol_Sg_BillingMode.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Billing Mode module source file
//*******************************************************************************
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_BillingMode.h"
#include "FlashDB_AppM.h"
#include "Cloud_Protocol_EventPost.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_SG_MAX_MODEL_DEVSEG      48      // Max number of time segments
#define CLOUD_PROTOCOL_SG_TIME_SEG_STR_LEN      5       // Time string length (4 + 1)
#define CLOUD_PROTOCOL_SG_FEE_SCALE_FACTOR      10000   // Fee scale factor (precision to 4 decimal places)
#define CLOUD_PROTOCOL_SG_MINUTES_PER_DAY       1440    // Minutes per day (24*60)
#define CLOUD_PROTOCOL_SG_TIME_NUM_MIN          1       // Minimum number of segments
#define CLOUD_PROTOCOL_SG_TIME_NUM_MAX          48      // Maximum number of segments

/* Time segment flag definitions */
#define CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK         1       // Peak segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH         2       // High segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL       3       // Normal segment
#define CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY       4       // Valley segment

/* Fee type indices */
#define CLOUD_PROTOCOL_SG_FEE_TYPE_PEAK         0       // Peak fee index
#define CLOUD_PROTOCOL_SG_FEE_TYPE_HIGH         1       // High fee index
#define CLOUD_PROTOCOL_SG_FEE_TYPE_NORMAL       2       // Normal fee index
#define CLOUD_PROTOCOL_SG_FEE_TYPE_VALLEY       3       // Valley fee index
#define CLOUD_PROTOCOL_SG_FEE_TYPE_COUNT        4       // Number of fee types

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_SUCCESS = 10, // Successful update
	CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_BUSY = 11,	// Charging and discharging in progress, update failed
	CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_FAILURE = 12	// Update failed
} cloud_protocol_sg_update_billing_mode_result_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint16_t start_minutes; // Start time (in minutes)
    uint16_t end_minutes;   // End time (in minutes)
    uint32_t seg_flag;      // Segment flag
    int index;              // Original index
} cloud_protocol_sg_billing_time_segment_Info;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static v2g_service_issue_feeModel cloud_protocol_sg_billing_mode = {0};
static cloud_protocol_sg_billing_time_segment_Info * cloud_protocol_sg_billing_time_segments = NULL;
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool Cloud_Protocol_Sg_ValidateTimeString(const char *time_str);
static uint16_t Cloud_Protocol_Sg_TimeStrToMinutes(const char *time_str);
static bool Cloud_Protocol_Sg_BuildTimeSegments(cloud_protocol_sg_billing_time_segment_Info *segments, int count);
static int Cloud_Protocol_Sg_CompareSegments(const void *a, const void *b);
static void Cloud_Protocol_Send_UpdateBillingModeResponse(char *message_id, cloud_protocol_sg_update_billing_mode_result_t result);
static int Cloud_Protocol_Sg_GetFeeTypeIndex(uint32_t seg_flag);
static bool Cloud_Protocol_Sg_IsAnyConnectorCharging(void);
static bool Cloud_Protocol_Sg_IsModelIdSame(const char *new_model_id);
static void Cloud_Protocol_Send_UpdateBillingTimeSegments(int count);
static void Cloud_Protocol_Sg_SaveBillingModeToFlash(const char *msg_id);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Validate time string format
 * @param time_str Time string ("hhmm")
 * @return true if valid, false otherwise
 */
static bool Cloud_Protocol_Sg_ValidateTimeString(const char *time_str)
{
    if (time_str == NULL || strlen(time_str) != 4)
    {
        CLOUD_ERROR("<%s> Time string invalid: NULL or length != 4\r\n", __func__);
        return false;
    }

    /* Check numeric characters */
    for (int i = 0; i < 4; i++)
    {
        if (time_str[i] < '0' || time_str[i] > '9')
        {
            CLOUD_ERROR("<%s> Time string contains non-digit: %s \r\n", __func__, time_str);
            return false;
        }
    }

    /* Parse hour and minute */
    char hour_str[3] = {0};
    char min_str[3] = {0};
    strncpy(hour_str, time_str, 2);
    strncpy(min_str, time_str + 2, 2);

    int hour = atoi(hour_str);
    int minute = atoi(min_str);

    /* Validate ranges */
    if (hour < 0 || hour > 23)
    {
        CLOUD_ERROR("<%s> Hour out of range: %d (time: %s)\r\n", __func__, hour, time_str);
        return false;
    }

    if (minute < 0 || minute > 59)
    {
        CLOUD_ERROR("<%s> Minute out of range: %d (time: %s)\r\n", __func__, minute, time_str);
        return false;
    }

    return true;
}

/**
 * @brief Convert time string to minutes (0-1439)
 * @param time_str Time string ("hhmm")
 * @return Minutes value
 */
static uint16_t Cloud_Protocol_Sg_TimeStrToMinutes(const char *time_str)
{
    if (!Cloud_Protocol_Sg_ValidateTimeString(time_str))
    {
        return 0;
    }

    char hour_str[3] = {0};
    char min_str[3] = {0};
    strncpy(hour_str, time_str, 2);
    strncpy(min_str, time_str + 2, 2);

    int hour = atoi(hour_str);
    int minute = atoi(min_str);

    return (hour * 60 + minute);
}

/**
 * @brief Build and sort time segment information
 * @param segments Array to fill with segment info
 * @param count Number of segments
 * @return true on success, false on failure
 */
static bool Cloud_Protocol_Sg_BuildTimeSegments(cloud_protocol_sg_billing_time_segment_Info *segments, int count)
{
    if (segments == NULL || count <= 0 || count > CLOUD_PROTOCOL_SG_MAX_MODEL_DEVSEG)
    {
        CLOUD_ERROR("<%s> Invalid parameters\r\n", __func__);
        return false;
    }

    /* Build segment info */
    for (int i = 0; i < count; i++)
    {
        segments[i].start_minutes = Cloud_Protocol_Sg_TimeStrToMinutes(cloud_protocol_sg_billing_mode.timeSeg[i]);
        segments[i].seg_flag = cloud_protocol_sg_billing_mode.segFlag[i];
        segments[i].index = i;
    }

    /* Sort by start time */
    qsort(segments, count, sizeof(cloud_protocol_sg_billing_time_segment_Info), Cloud_Protocol_Sg_CompareSegments);

    /* Check for duplicate start times */
    for (int i = 0; i < count - 1; i++)
    {
        if (segments[i].start_minutes == segments[i + 1].start_minutes)
        {
            CLOUD_ERROR("<%s> Duplicate time point found: %s\r\n", __func__, cloud_protocol_sg_billing_mode.timeSeg[segments[i].index]);
            return false;
        }
    }

    /* Set end times */
    for (int i = 0; i < count; i++)
    {
        if (i < count - 1)
        {
            segments[i].end_minutes = segments[i + 1].start_minutes;
        }
        else
        {
            /* The last segment's end time is the first segment start time + 24 hours */
            segments[i].end_minutes = segments[0].start_minutes + CLOUD_PROTOCOL_SG_MINUTES_PER_DAY;
        }
    }

    return true;
}

/**
 * @brief Segment comparison function for qsort
 */
static int Cloud_Protocol_Sg_CompareSegments(const void *a, const void *b)
{
    cloud_protocol_sg_billing_time_segment_Info *seg_a = (cloud_protocol_sg_billing_time_segment_Info *)a;
    cloud_protocol_sg_billing_time_segment_Info *seg_b = (cloud_protocol_sg_billing_time_segment_Info *)b;

    if (seg_a->start_minutes < seg_b->start_minutes)
        return -1;
    if (seg_a->start_minutes > seg_b->start_minutes)
        return 1;
    return 0;
}

/**
 * @brief Get fee type index from segment flag
 * @param seg_flag Segment flag
 * @return Fee type index (0-3), returns -1 on failure
 */
static int Cloud_Protocol_Sg_GetFeeTypeIndex(uint32_t seg_flag)
{
    switch (seg_flag)
    {
    case CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK:
        return CLOUD_PROTOCOL_SG_FEE_TYPE_PEAK;
    case CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH:
        return CLOUD_PROTOCOL_SG_FEE_TYPE_HIGH;
    case CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL:
        return CLOUD_PROTOCOL_SG_FEE_TYPE_NORMAL;
    case CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY:
        return CLOUD_PROTOCOL_SG_FEE_TYPE_VALLEY;
    default:
        return -1;
    }
}

/*
 * @brief Parse billing mode update parameters from JSON
 * @param params JSON object containing service parameters
 * @param msg_id Message ID
 * @return true on success, false on failure
 */
bool Cloud_Protocol_Sg_ParseUpdateBillingModeParam(cJSON *params, const char *msg_id)
{
    cJSON *root = cJSON_Duplicate(params, true);
    if (root == NULL || !cJSON_IsObject(root))
    {
        CLOUD_ERROR("<%s> Failed to parse JSON parameters\r\n", __func__);
        return false;
    }

    v2g_service_issue_feeModel service_feeModel_data;
    int i = 0;
    memset(&service_feeModel_data, 0, sizeof(service_feeModel_data));
    cJSON *item_eletimeNum = cJSON_GetObjectItem(root, "timeNum");
    if (item_eletimeNum != NULL && cJSON_IsNumber(item_eletimeNum))
    {
        service_feeModel_data.timeNum = item_eletimeNum->valueint;
    }

    cJSON *item_timeSeg = cJSON_GetObjectItem(root, "timeSeg");
    cJSON *array_timeSeg = NULL;
    if (item_timeSeg != NULL && cJSON_IsArray(item_timeSeg))
    {
        for (i = 0; i < service_feeModel_data.timeNum; i++)
        {
            array_timeSeg = cJSON_GetArrayItem(item_timeSeg, i);
            memcpy(service_feeModel_data.timeSeg[i], array_timeSeg->valuestring, strlen(array_timeSeg->valuestring));
        }
    }

    cJSON *item_segFlag = cJSON_GetObjectItem(root, "segFlag");
    cJSON *arrary_segFlag = NULL;
    if (item_segFlag != NULL && cJSON_IsArray(item_segFlag))
    {

        for (i = 0; i < service_feeModel_data.timeNum; i++)
        {
            arrary_segFlag = cJSON_GetArrayItem(item_segFlag, i);
            service_feeModel_data.segFlag[i] = arrary_segFlag->valueint;
        }
    }

    cJSON *item_chargeFee = cJSON_GetObjectItem(root, "chargeFee");
    cJSON *arrary_chargeFee = NULL;
    if (item_chargeFee != NULL && cJSON_IsArray(item_chargeFee))
    {
        for (i = 0; i < 4; i++)
        {
            arrary_chargeFee = cJSON_GetArrayItem(item_chargeFee, i);
            service_feeModel_data.chargeFee[i] = arrary_chargeFee->valueint;
        }
    }

    cJSON *item_serviceFee = cJSON_GetObjectItem(root, "serviceFee");
    cJSON *arrary_serviceFee = NULL;
    if (item_serviceFee != NULL && cJSON_IsArray(item_serviceFee))
    {
        for (i = 0; i < 4; i++)
        {
            arrary_serviceFee = cJSON_GetArrayItem(item_serviceFee, i);
            service_feeModel_data.serviceFee[i] = arrary_serviceFee->valueint;
        }
    }

    cJSON_Delete(root);
    cloud_protocol_sg_update_billing_mode_result_t update_result = CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_SUCCESS;

    /* check if any connector is charging */
    if (Cloud_Protocol_Sg_IsAnyConnectorCharging())
    {
        update_result = CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_BUSY;
        CLOUD_WARN("<%s> Charging/discharging in progress, update rejected, msg_id: %s\r\n", __func__, msg_id);
    }

    /* check */
    if (update_result == CLOUD_PROTOCOL_SG_UPDATE_BILLING_MODE_SUCCESS)
    {
        /* check if data has actually changed to avoid unnecessary Flash writes */
        if (memcmp(&service_feeModel_data, &cloud_protocol_sg_billing_mode,
            sizeof(v2g_service_issue_feeModel)) != 0 ||
            !Cloud_Protocol_Sg_IsModelIdSame(msg_id))
        {
            Cloud_Protocol_Send_UpdateBillingTimeSegments(service_feeModel_data.timeNum);
            memcpy(&cloud_protocol_sg_billing_mode, &service_feeModel_data, sizeof(v2g_service_issue_feeModel));
            Cloud_Protocol_Sg_SaveBillingModeToFlash(msg_id);
        }
        else
        {
            CLOUD_DEBUG("<%s> Billing mode unchanged, skipping Flash write\r\n", __func__);
        }
    }

    /* send response */
    Cloud_Protocol_Send_UpdateBillingModeResponse((char *)msg_id, update_result);

    return true;
}

/**
 * @brief check if any connector is currently charging
 * @return true if any connector is charging, false otherwise
 */
static bool Cloud_Protocol_Sg_IsAnyConnectorCharging(void)
{
    for (SysConnector_Num_Enum sys_connector_num = SYS_CONNECTOR1; sys_connector_num < SYS_CONNECTOR_NUM_MAX; sys_connector_num++)
    {
        if (SYSM_GetSysStatusBit(sys_connector_num, STD_SYSM_SYSSTATUS_CHARGING))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief check if model ID is the same
 * @param new_model_id New model ID
 * @return true if same, false otherwise
 */
static bool Cloud_Protocol_Sg_IsModelIdSame(const char *new_model_id)
{
    char stored_model_id[V2G_MAX_MODEL_ID_LEN] = {0};
    FlashDB_ReadValue(FLASHDB_KV_SG_BILLING_MODE_NO, stored_model_id, sizeof(stored_model_id), NULL);

    return (strcmp(stored_model_id, new_model_id) == 0);
}

/**
 * @brief send update billing mode response
 * @param message_id Message ID
 * @param result Update result
 */
static void Cloud_Protocol_Send_UpdateBillingTimeSegments(int count)
{
    if (cloud_protocol_sg_billing_time_segments != NULL)
    {
        CLOUDM_FREE(cloud_protocol_sg_billing_time_segments);
        cloud_protocol_sg_billing_time_segments = NULL;
        cloud_protocol_sg_billing_time_segments = CLOUDM_MALLOC(sizeof(cloud_protocol_sg_billing_time_segment_Info) * count);
        if (NULL != cloud_protocol_sg_billing_time_segments)
        {
            if (!Cloud_Protocol_Sg_BuildTimeSegments(cloud_protocol_sg_billing_time_segments, count))
            {
                CLOUD_ERROR("<%s> Failed to build time segments for event\r\n", __func__);
            }
        }
        CLOUD_INFO("<%s> Billing time segments updated for event\r\n", __func__);
    }
}

/**
 * @brief store billing mode to Flash
 * @param msg_id Model ID
 * @return void
 */
static void Cloud_Protocol_Sg_SaveBillingModeToFlash(const char *msg_id)
{
    static uint64_t last_save_time = 0;

     /* Throttle Flash writes to at most once every 30 seconds */
    if (0 == last_save_time || ((CLOUD_GET_TIME_MS() - last_save_time) >= 30000))
    {
        last_save_time = CLOUD_GET_TIME_MS();
    }
    else
    {
        return;
    }

    if (msg_id != NULL)
    {
        char fw_model_no[V2G_MAX_MODEL_ID_LEN] = {0};
        snprintf(fw_model_no, sizeof(fw_model_no), "%s", msg_id);

        /* write model ID */
        FlashDB_WriteValue(FLASHDB_KV_SG_BILLING_MODE_NO, (void *)fw_model_no, V2G_MAX_MODEL_ID_LEN);

        /* trigger event notification */
        Cloud_Protocol_EventPost_FwInfo_Set(CLOUD_PROTOCOL_SG_EVENT_FW_MODEL_NO, (void *)msg_id);
        Cloud_Protocol_EventPost_TriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_FW_INFO);
    }

    /* write billing mode data */
    FlashDB_WriteValue(FLASHDB_KV_SG_BILLING_MODE_MODEL, &cloud_protocol_sg_billing_mode, sizeof(cloud_protocol_sg_billing_mode));

    CLOUD_INFO("<%s> Billing mode saved to Flash\r\n", __func__);
}

/**
 * @brief Get current segment flag by current time (supports unordered segments and cross-day)
 * @param hour Hour (0-23)
 * @param minute Minute (0-59)
 * @return Segment flag (1-4), returns 0 if no matching segment found
 */
uint8_t Cloud_Protocol_Sg_GetCurrentSegFlag(uint8_t hour, uint8_t minute)
{
    if (cloud_protocol_sg_billing_mode.timeNum == 0)
    {
        CLOUD_WARN("<%s> Billing mode not initialized\r\n", __func__);
        return 0;
    }

    /* Convert current time to minutes */
    uint16_t current_minutes = hour * 60 + minute;

    /* Build time segments */
    cloud_protocol_sg_billing_time_segment_Info segments[CLOUD_PROTOCOL_SG_MAX_MODEL_DEVSEG];
    if (!Cloud_Protocol_Sg_BuildTimeSegments(segments, cloud_protocol_sg_billing_mode.timeNum))
    {
        CLOUD_ERROR("<%s> Failed to build time segments\r\n", __func__);
        return 0;
    }

    /* Find segment containing current time */
    for (int i = 0; i < cloud_protocol_sg_billing_mode.timeNum; i++)
    {
        /* Compute minutes relative to this segment (handle cross-day) */
        uint16_t check_minutes = current_minutes;

        /* If current time is less than start time, it may belong to a cross-day segment */
        if (check_minutes < segments[i].start_minutes)
        {
            check_minutes += CLOUD_PROTOCOL_SG_MINUTES_PER_DAY;
        }

        /* Check if within the segment */
        if (check_minutes >= segments[i].start_minutes && check_minutes < segments[i].end_minutes)
        {
            CLOUD_DEBUG("<%s> Time %02d:%02d found in segment %d (flag: %u)\r\n", __func__, hour, minute, i, segments[i].seg_flag);
            return segments[i].seg_flag;
        }
    }

    CLOUD_WARN("<%s> No matching time segment found for %02d:%02d\r\n", __func__, hour, minute);
    return 0;
}

/**
 * @brief Get current charge fee for the given segment flag (actual value)
 * @param seg_flag Segment flag
 * @return Charge fee (actual value, e.g., 0.1), returns 0 on failure
 */
float Cloud_Protocol_Sg_GetCurrentChargeFee(uint8_t seg_flag)
{
    int fee_type_index = Cloud_Protocol_Sg_GetFeeTypeIndex(seg_flag);
    if (fee_type_index < 0)
    {
        CLOUD_ERROR("<%s> Invalid seg flag: %u\r\n", __func__, seg_flag);
        return 0.0f;
    }

    /* Convert stored integer to actual value (1000 -> 0.1) */
    return (float)cloud_protocol_sg_billing_mode.chargeFee[fee_type_index] / CLOUD_PROTOCOL_SG_FEE_SCALE_FACTOR;
}

/**
 * @brief Get current service fee for the given segment flag (actual value)
 * @param seg_flag Segment flag
 * @return Service fee (actual value, e.g., 0.1), returns 0 on failure
 */
float Cloud_Protocol_Sg_GetCurrentServiceFee(uint8_t seg_flag)
{
    int fee_type_index = Cloud_Protocol_Sg_GetFeeTypeIndex(seg_flag);
    if (fee_type_index < 0)
    {
        CLOUD_ERROR("<%s> Invalid seg flag: %u\r\n", __func__, seg_flag);
        return 0.0f;
    }

    /* Convert stored integer to actual value (1000 -> 0.1) */
    return (float)cloud_protocol_sg_billing_mode.serviceFee[fee_type_index] / CLOUD_PROTOCOL_SG_FEE_SCALE_FACTOR;
}

/**
 * @brief Print current billing model information
 */
void Cloud_Protocol_Sg_PrintBillingMode(void)
{
    CLOUD_INFO("=== Current Billing Mode ===\r\n");
    CLOUD_INFO("Time segments count: %d\r\n", cloud_protocol_sg_billing_mode.timeNum);

    for (int i = 0; i < cloud_protocol_sg_billing_mode.timeNum; i++)
    {
        CLOUD_INFO("Segment[%d]: Time=%s, Flag=%u\r\n", i,
                   cloud_protocol_sg_billing_mode.timeSeg[i],
                   cloud_protocol_sg_billing_mode.segFlag[i]);
    }

    CLOUD_INFO("Charge Fees (raw): Peak=%u, High=%u, Normal=%u, Valley=%u \r\n",
               cloud_protocol_sg_billing_mode.chargeFee[CLOUD_PROTOCOL_SG_FEE_TYPE_PEAK],
               cloud_protocol_sg_billing_mode.chargeFee[CLOUD_PROTOCOL_SG_FEE_TYPE_HIGH],
               cloud_protocol_sg_billing_mode.chargeFee[CLOUD_PROTOCOL_SG_FEE_TYPE_NORMAL],
               cloud_protocol_sg_billing_mode.chargeFee[CLOUD_PROTOCOL_SG_FEE_TYPE_VALLEY]);

    CLOUD_INFO("Charge Fees (actual): Peak=%.4f, High=%.4f, Normal=%.4f, Valley=%.4f \r\n",
               Cloud_Protocol_Sg_GetCurrentChargeFee(CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK),
               Cloud_Protocol_Sg_GetCurrentChargeFee(CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH),
               Cloud_Protocol_Sg_GetCurrentChargeFee(CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL),
               Cloud_Protocol_Sg_GetCurrentChargeFee(CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY));

    CLOUD_INFO("Service Fees (raw): Peak=%u, High=%u, Normal=%u, Valley=%u \r\n",
               cloud_protocol_sg_billing_mode.serviceFee[CLOUD_PROTOCOL_SG_FEE_TYPE_PEAK],
               cloud_protocol_sg_billing_mode.serviceFee[CLOUD_PROTOCOL_SG_FEE_TYPE_HIGH],
               cloud_protocol_sg_billing_mode.serviceFee[CLOUD_PROTOCOL_SG_FEE_TYPE_NORMAL],
               cloud_protocol_sg_billing_mode.serviceFee[CLOUD_PROTOCOL_SG_FEE_TYPE_VALLEY]);

    CLOUD_INFO("Service Fees (actual): Peak=%.4f, High=%.4f, Normal=%.4f, Valley=%.4f \r\n",
               Cloud_Protocol_Sg_GetCurrentServiceFee(CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK),
               Cloud_Protocol_Sg_GetCurrentServiceFee(CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH),
               Cloud_Protocol_Sg_GetCurrentServiceFee(CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL),
               Cloud_Protocol_Sg_GetCurrentServiceFee(CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY));
    CLOUD_INFO("============================\r\n");
}

/**
 * @brief Get pointer to billing model data
 * @return Pointer to billing model data
 */
v2g_service_issue_feeModel *Cloud_Protocol_Sg_GetBillingModePtr(void)
{
    return &cloud_protocol_sg_billing_mode;
}

/**
 * @brief Set billing model data from external source
 * @param billing_mode Pointer to billing model data
 */
void Cloud_Protocol_Sg_SetBillingModePtr(v2g_service_issue_feeModel *billing_mode)
{
	if (billing_mode != NULL)
	{
		memcpy(&cloud_protocol_sg_billing_mode, billing_mode, sizeof(cloud_protocol_sg_billing_mode));
	}
}

/**
 * @brief Check whether billing model is initialized
 * @return true if initialized, false otherwise
 */
bool Cloud_Protocol_Sg_IsBillingModeInitialized(void)
{
    return (cloud_protocol_sg_billing_mode.timeNum > 0);
}

/**
 * @brief Send update billing mode response to cloud platform
 * @param message_id Message ID
 * @param result Update result
 */
static void Cloud_Protocol_Send_UpdateBillingModeResponse(char *message_id, cloud_protocol_sg_update_billing_mode_result_t result)
{
    cJSON *response_data = cJSON_CreateObject();
    if (!response_data)
    {
        CLOUD_ERROR("<%s> Failed to create JSON object\r\n", __FUNCTION__);
        return;
    }
    cJSON_AddNumberToObject(response_data, "res", result);

   // send response
    cloud_protocol_service_call_response(message_id, "issueElectricPriceModelSrv", CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS, response_data);

    // clean up
    cJSON_Delete(response_data);

    CLOUD_DEBUG("<%s> issue electric price model response sent: result=%d\n", __FUNCTION__, result);
}
/* EOL */
