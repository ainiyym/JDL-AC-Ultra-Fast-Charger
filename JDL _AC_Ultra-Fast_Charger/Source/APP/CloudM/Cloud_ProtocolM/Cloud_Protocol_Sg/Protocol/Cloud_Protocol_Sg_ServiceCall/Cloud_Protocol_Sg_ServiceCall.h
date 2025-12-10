//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ServiceCall.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol call service module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_SERVICECALL_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_SERVICECALL_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"
#include "Cloud_Protocol_Mqtt.h"
#include "Cloud_Protocol_Sg_RemoteCharge.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_PROTOCOL_SG_SERVICE_CALL_START_CHARGE,                   // Charge Or Discharge Plan Service
    CLOUD_PROTOCOL_SG_SERVICE_CALL_STOP_CHARGE,                    // Remote Stop Charge Service
    CLOUD_PROTOCOL_SG_SERVICE_CALL_UNKNOWN                         // unknown event
} cloud_protocol_sg_service_call_type_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern bool cloud_protocol_service_call(const char *payload, uint16_t payload_len);

#endif /* __CLOUD_PROTOCOL_SG_SERVICECALL_H */
/* EOL */
