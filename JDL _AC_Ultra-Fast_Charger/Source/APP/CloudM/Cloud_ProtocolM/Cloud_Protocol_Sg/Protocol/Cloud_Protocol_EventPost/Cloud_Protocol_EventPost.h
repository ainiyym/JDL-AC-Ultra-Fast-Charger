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
#include "Cloud_Protocol_EventPost_Cfg.h"
#include "Cloud_Protocol_EventPost_Task.h"

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
// Event post task structure
typedef struct
{
    cloud_protocol_event_post_type_t type; // event type
    uint32_t last_post_time;               // last post timestamp
    uint32_t interval;                     // post interval
    bool enabled;                          // enabled flag
    bool force_post;                       // force post flag
} cloud_protocol_event_post_task_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
// event post management functions
extern void Cloud_Protocol_EventPost_Init(void);
extern void Cloud_Protocol_EventPost_PeriodicTask(void);
extern void Cloud_Protocol_EventPost_UpdateConfig(const v2g_data_dev_config *config);
extern bool Cloud_Protocol_EventPost_TriggerEvent(cloud_protocol_event_post_type_t type);
extern void Cloud_Protocol_EventPost_ForceAllEvents(void);
// event post functions
// void Cloud_Protocol_EventPost_GunInfo(void);
// void Cloud_Protocol_EventPost_VehicleInfo(void);
// void Cloud_Protocol_EventPost_BatteryInfo(void);
// void Cloud_Protocol_EventPost_PileWarnings(void);
// void Cloud_Protocol_EventPost_VehicleWarnings(void);
// void Cloud_Protocol_EventPost_GrndLock(void);
// void Cloud_Protocol_EventPost_DoorLock(void);
// void Cloud_Protocol_EventPost_ChargeStatus(void);
// void Cloud_Protocol_EventPost_SystemStatus(void);
#endif /* __CLOUD_PROTOCOL_EVENTPOST_H */
/* EOL */
