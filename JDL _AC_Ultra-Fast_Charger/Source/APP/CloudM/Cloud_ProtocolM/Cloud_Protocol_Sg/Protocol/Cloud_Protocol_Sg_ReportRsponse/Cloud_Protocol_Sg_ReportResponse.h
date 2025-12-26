//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ReportResponse.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol report response module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_REPORTRESPONSE_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_REPORTRESPONSE_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
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
typedef enum
{
    CLOUD_PROTOCOL_SG_REPORT_EVENT_VER_INFO,                   // version info event
    CLOUD_PROTOCOL_SG_REPORT_EVENT_FW_INFO,                    // firmware info event
    CLOUD_PROTOCOL_SG_REPORT_EVENT_PILE_WORKSTATUS,            // pile work status event
    CLOUD_PROTOCOL_SG_REPORT_EVENT_GUN_STATUS,                 // gun status event
    CLOUD_PROTOCOL_SG_REPORT_EVENT_OTHER,                      // other event
    CLOUD_PROTOCOL_SG_REPORT_EVENT_UNKNOWN                     // unknown event
} cloud_protocol_sg_report_event_type_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_PROTOCOL_SG_REPORTRESPONSE_H */
/* EOL */
