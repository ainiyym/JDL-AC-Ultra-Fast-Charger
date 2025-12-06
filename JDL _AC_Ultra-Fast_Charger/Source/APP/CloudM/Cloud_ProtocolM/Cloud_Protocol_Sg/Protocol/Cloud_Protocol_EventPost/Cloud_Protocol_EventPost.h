//******************************************************************************
//* File Name: Cloud_Protocol_EventPost.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module event post header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_EVENTPOST_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_EVENTPOST_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"
#include "Cloud_Protocol_Sg_DataDef.h"
#include "Cloud_Protocol_Mqtt.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// string array type
typedef struct
{
    char **strings; // string array
    uint16_t count; // string count
} cloud_protocol_string_array_t;

// common parameter value union
typedef union
{
    char *string_value;
    int32_t int32_value;
    float float_value;
    bool bool_value;
    cloud_protocol_string_array_t *string_array_value;
    int32_t *int32_array_value;
    uint16_t array_count; // array element count
    double double_value;
} cloud_protocol_value_u;

// Event parameter structure
typedef struct
{
    char *param_name;                       // parameter name
    cloud_protocol_value_type_e param_type; // parameter type
    cloud_protocol_value_u param_value;     // parameter value
} cloud_protocol_event_post_param_t;

// Event post request structure
typedef struct
{
    char *id;         // message ID
    char *version;    // protocol version
    char *method;     // request method
} cloud_protocol_event_post_req_t;

// Event post response structure
typedef struct
{
    char *id;   // message ID
    int code;   // states code
    char *data; // response data
} cloud_protocol_event_post_resp_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
// create event post request object
extern cloud_protocol_event_post_req_t *Cloud_Protocol_EventPost_CreateRequest(char* identifier, cloud_protocol_mqtt_set_msg_id_cb set_msg_id_cb);
extern cJSON *Cloud_Protocol_EventPost_BuildRequestJsonHeader(const cloud_protocol_event_post_req_t *req);
extern bool Cloud_Protocol_EventPost_AddParam(cJSON *object, const cloud_protocol_event_post_param_t *param);
extern void Cloud_Protocol_EventPost_PrintUnformatted(cJSON *object, uint64_t timestamp, char* identifier);
// parse response object
extern cloud_protocol_event_post_resp_t *Cloud_Protocol_EventPost_ParseResponse(const char *json_str);
extern void Cloud_Protocol_EventPost_DestroyResponse(cloud_protocol_event_post_resp_t *resp);
extern bool Cloud_Protocol_EventPost_IsSuccess(const cloud_protocol_event_post_resp_t *resp);
extern const char *Cloud_Protocol_EventPost_GetCodeMessage(int code);    
#endif /* __CLOUD_PROTOCOL_EVENTPOST_H */
/* EOL */
