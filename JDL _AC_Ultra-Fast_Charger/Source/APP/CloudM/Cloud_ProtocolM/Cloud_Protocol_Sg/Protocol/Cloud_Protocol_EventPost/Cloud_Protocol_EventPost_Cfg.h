//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_EVENTPOST_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_EVENTPOST_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Mqtt.h"
#include "Cloud_Protocol_Sg_Cfg.h"
#include "Cloud_Protocol_Sg_DataDef.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
// event parameter value type
typedef enum
{
   CLOUD_PROTOCOL_EVENT_POST_TYPE_GUN_INFO = 0,     // charging gun status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_INFO,     // vehicle status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_BATTERY_INFO,     // battery status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WARNINGS,    // device warning information
   CLOUD_PROTOCOL_EVENT_POST_TYPE_VEHICLE_WARNINGS, // vehicle warning information
   CLOUD_PROTOCOL_EVENT_POST_TYPE_GRND_LOCK,        // ground lock status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_DOOR_LOCK,        // door lock status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_FW_INFO,          // firmware information
   CLOUD_PROTOCOL_EVENT_POST_TYPE_VERSION_INFO,     // version information
   CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS,  // pile work status
   CLOUD_PROTOCOL_EVENT_POST_TYPE_MAX
} cloud_protocol_event_post_type_t;

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
extern void Cloud_Protocol_EventPost_DestroyRequest(cloud_protocol_event_post_req_t *req);
extern void Cloud_Protocol_EventPost_PrintUnformatted(cJSON *object, uint64_t timestamp, char* identifier);
// parse response object
extern cloud_protocol_event_post_resp_t *Cloud_Protocol_EventPost_ParseResponse(const char *json_str);

#endif /* __CLOUD_PROTOCOL_EVENTPOST_CFG_H */
/* EOL */
