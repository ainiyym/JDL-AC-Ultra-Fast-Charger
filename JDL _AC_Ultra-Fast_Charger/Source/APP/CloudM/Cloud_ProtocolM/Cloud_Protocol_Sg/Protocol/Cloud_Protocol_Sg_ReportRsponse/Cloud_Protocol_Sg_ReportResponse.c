// ...existing code...
//*******************************************************************************
//* File Name: Cloud_Protocol_Sg_ReportResponse.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol report response source file
//*******************************************************************************
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ReportResponse.h"

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
    uint16_t code;    // response code
    uint32_t id;      // message ID
    char method[128]; // method name
    char message[64]; // message text
    char version[16]; // version string
} cloud_protocol_report_response_t;

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

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
static bool cloud_protocol_report_parse_report_response(const char *payload, cloud_protocol_report_response_t *response)
{
    if (!payload || !response)
    {
        CLOUD_ERROR("<%s> Invalid parameters\r\n", __func__);
        return false;
    }

    /* Clear the response structure */
    memset(response, 0, sizeof(cloud_protocol_report_response_t));

    /* Parse JSON using cJSON */
    cJSON *root = cJSON_Parse(payload);
    if (!root)
    {
        CLOUD_ERROR("<%s> Failed to parse JSON payload\r\n", __func__);
        return false;
    }

    bool parse_success = false;
    cJSON *code_item = cJSON_GetObjectItem(root, "code");
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON *method_item = cJSON_GetObjectItem(root, "method");
    cJSON *message_item = cJSON_GetObjectItem(root, "message");
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    
    /* Parse code */
    if (code_item && cJSON_IsNumber(code_item))
    {
        response->code = (uint16_t)code_item->valueint;
    }
    else
    {
        CLOUD_WARN("<%s> Missing or invalid 'code' field in report response\r\n", __func__);
        goto cleanup;
    }

    /* Parse id (may be string or number) */
    if (id_item)
    {
        if (cJSON_IsString(id_item))
        {
            response->id = (uint32_t)atoi(id_item->valuestring);
        }
        else if (cJSON_IsNumber(id_item))
        {
            response->id = (uint32_t)id_item->valueint;
        }
        else
        {
            CLOUD_WARN("<%s> Invalid 'id' field type in report response\r\n", __func__);
            goto cleanup;
        }
    }
    else
    {
        CLOUD_WARN("<%s> Missing 'id' field in report response\r\n", __func__);
        goto cleanup;
    }

    /* Parse method */
    if (method_item && cJSON_IsString(method_item))
    {
        strncpy(response->method, method_item->valuestring, sizeof(response->method) - 1);
        response->method[sizeof(response->method) - 1] = '\0';
    }
    else
    {
        CLOUD_WARN("<%s> Missing or invalid 'method' field in report response\r\n", __func__);
        /* If method field is missing, still continue processing code and id */
    }

    /* Parse message */
    if (message_item && cJSON_IsString(message_item))
    {
        strncpy(response->message, message_item->valuestring, sizeof(response->message) - 1);
        response->message[sizeof(response->message) - 1] = '\0';
    }

    /* Parse version */
    if (version_item && cJSON_IsString(version_item))
    {
        strncpy(response->version, version_item->valuestring, sizeof(response->version) - 1);
        response->version[sizeof(response->version) - 1] = '\0';
    }

    parse_success = true;

cleanup:
    cJSON_Delete(root);

    return parse_success;
}

static cloud_protocol_sg_report_event_type_t cloud_protocol_report_detect_report_event_type(const char *method)
{
    if (!method)
    {
        return CLOUD_PROTOCOL_SG_REPORT_EVENT_UNKNOWN;
    }

    /* Detect event type based on method name */
    if (strstr(method, "verInfoEvt") != NULL)
    {
        return CLOUD_PROTOCOL_SG_REPORT_EVENT_VER_INFO;
    }

    if (strstr(method, "firmwareEvt") != NULL)
    {
        return CLOUD_PROTOCOL_SG_REPORT_EVENT_FW_INFO;
    }

    if (strstr(method, "pileWorkStatusEvt") != NULL)
    {
        return CLOUD_PROTOCOL_SG_REPORT_EVENT_PILE_WORKSTATUS;
    }

    /* Check for other known event patterns (extendable) */
    if (strstr(method, "thing.event.") != NULL)
    {
        return CLOUD_PROTOCOL_SG_REPORT_EVENT_OTHER;
    }

    return CLOUD_PROTOCOL_SG_REPORT_EVENT_UNKNOWN;
}

static bool cloud_protocol_report_dispatch_report_event(const cloud_protocol_report_response_t *response)
{
    if (!response)
    {
        return false;
    }

    cloud_protocol_sg_report_event_type_t event_type = cloud_protocol_report_detect_report_event_type(response->method);

    switch (event_type)
    {
        case CLOUD_PROTOCOL_SG_REPORT_EVENT_VER_INFO:
            CLOUD_INFO("<%s> Dispatching version info event: id=%u\r\n", __func__, response->id);
            if (Cloud_Protocol_EventPost_VersionInfo_HandleResponse(response->id))
            {
                return true;
            }
            break;

        case CLOUD_PROTOCOL_SG_REPORT_EVENT_FW_INFO:
            CLOUD_INFO("<%s> Dispatching firmware info event: id=%u\r\n", __func__, response->id);
            if (Cloud_Protocol_EventPost_FwInfo_HandleResponse(response->id))
            {
                return true;
            }
            break;
        case CLOUD_PROTOCOL_SG_REPORT_EVENT_PILE_WORKSTATUS:
            CLOUD_INFO("<%s> Dispatching pile work status event: id=%u\r\n", __func__, response->id);
            if (Cloud_Protocol_EventPost_PostPileWorkstatus_Response(response->id))
            {
                return true;
            }
        case CLOUD_PROTOCOL_SG_REPORT_EVENT_OTHER:
            CLOUD_INFO("Other report event: method=%s, code=%u, id=%u\r\n", response->method, response->code, response->id);
            break;

        case CLOUD_PROTOCOL_SG_REPORT_EVENT_UNKNOWN:
        default:
            CLOUD_WARN("Unknown report event type: method=%s\r\n", response->method);
            return false;
    }
    return false;
}

/**
 * @brief Handle report response
 * @param payload report response payload
 * @param payload_len payload length
 * @return bool success or failure
 */
bool cloud_protocol_report_response(const char *payload, uint16_t payload_len)
{
    if (!payload || payload_len == 0)
    {
        CLOUD_ERROR("<%s> Invalid payload for report response\r\n", __func__);
        return false;
    }

    CLOUD_INFO("<%s> Processing report response: %.*s\r\n", __func__, payload_len < 256 ? payload_len : 256, payload);

    cloud_protocol_report_response_t response;

    /* Parse response */
    if (!cloud_protocol_report_parse_report_response(payload, &response))
    {
        CLOUD_ERROR("<%s> Failed to parse report response\r\n", __func__);
        return false;
    }

    if (response.code != CLOUD_PROTOCOL_SG_RESPONSE_SUCCESS)
    {
        CLOUD_WARN("<%s> Report response failed with code: %u, message: %s\r\n", __func__, response.code, response.message);
        return false;
    }

    /* Debug print of parsed result */
    CLOUD_DEBUG("<%s> Parsed report response: code=%u, id=%u, method=%s, message=%s\r\n", __func__, response.code, response.id, response.method, response.message);

    /* Dispatch event */
    bool response_success = cloud_protocol_report_dispatch_report_event(&response);

    return response_success;
}
/* EOL */
