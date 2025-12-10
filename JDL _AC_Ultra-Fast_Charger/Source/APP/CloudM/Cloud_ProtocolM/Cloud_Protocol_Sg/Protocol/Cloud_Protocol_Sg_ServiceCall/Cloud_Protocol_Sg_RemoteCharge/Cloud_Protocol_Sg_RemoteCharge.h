//******************************************************************************
//* File Name: Cloud_Protocol_Sg_RemoteCharge.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Remote Charge module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_REMOTECHARGE_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_REMOTECHARGE_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_DataDef.h"
#include "Cloud_Protocol_Sg_Cfg.h"
#include "STD_AuthM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
// Remote charge management structure
typedef struct
{
    bool is_authenticated[CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX];        // authentication status for each gun
    bool is_enabled[CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX];              // remote charge enable status for each gun
    v2g_service_remoteStart charge_param[CLOUD_PROTOCOL_SG_CHARGING_GUN_NUM_MAX]; // remote charge parameters for each gun
} cloud_protocol_sg_remote_charge_t;
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern bool Cloud_Protocol_Sg_RemoteCharge_Init(void);
extern bool Cloud_Protocol_Sg_RemoteCharge_Deinit(uint8_t gun_no);
extern bool Cloud_Protocol_Sg_ParseRemoteStartParam(cJSON *params, const char *msg_id);
extern bool Cloud_Protocol_Sg_ParseRemoteStopParam(cJSON *params, const char *msg_id);
extern void Cloud_Protocol_Sg_RemoteCharge_PeriodicTask(void);
#endif /* __CLOUD_PROTOCOL_SG_REMOTECHARGE_H */
/* EOL */
