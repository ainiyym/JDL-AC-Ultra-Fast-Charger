//******************************************************************************
//* File Name: Cloud_Protocol_Sg_SynchronizeNetTime.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module net time synchronization header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_SYNCHRONIZENETTIME_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_SYNCHRONIZENETTIME_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"
#include <inttypes.h>
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

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
typedef void (*cloud_protocol_sync_net_time_complete_callback_t)(bool success, int64_t time_offset, uint64_t round_trip_delay, uint64_t timestamp);

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern int cloud_protocol_sysnchronize_net_time_init(const char *product_key, const char *device_name, cloud_protocol_sync_net_time_complete_callback_t sync_callback);
extern bool cloud_protocol_sysnchronize_net_time_handle_response(const char *payload, uint16_t payload_len);
extern void Cloud_Protocol_Sg_SynchronizeNetTime_SetNetworkConnectStatus(bool connect);
extern void Cloud_Protocol_Sg_SynchronizeNetTime_Main(void);
#endif /* __CLOUD_PROTOCOL_SG_SYNCHRONIZENETTIME_H */
/* EOL */
