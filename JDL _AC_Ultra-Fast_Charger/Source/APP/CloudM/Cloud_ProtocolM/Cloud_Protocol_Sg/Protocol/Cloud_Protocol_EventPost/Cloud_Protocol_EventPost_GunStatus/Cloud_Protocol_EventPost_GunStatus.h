//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_GunStatus.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module gun status check header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_EVENTPOST_GUNSTATUS_H__)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_EVENTPOST_GUNSTATUS_H__
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_EventPost_Cfg.h"

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

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern bool Cloud_Protocol_EventPost_GunStatus_Post(uint8_t gun_no);
extern bool Cloud_Protocol_EventPost_GunStatus_HandleResponse(uint32_t msg_id);
extern void Cloud_Protocol_Sg_Order_GetGunStatusPostEnable(uint8_t *gun1, uint8_t *gun2);
extern void Cloud_Protocol_EventPost_GunStatusMonitoring(void);
#endif /* __CLOUD_PROTOCOL_EVENTPOST_GUNSTATUS_H__ */
/* EOL */
