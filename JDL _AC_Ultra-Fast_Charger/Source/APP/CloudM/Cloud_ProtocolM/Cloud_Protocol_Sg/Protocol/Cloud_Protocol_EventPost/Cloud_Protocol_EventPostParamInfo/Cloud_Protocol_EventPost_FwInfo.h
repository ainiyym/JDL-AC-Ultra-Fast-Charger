//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_FwInfo.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol post fw info module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_EVENTPOST_FWINFO_H__)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_EVENTPOST_FWINFO_H__
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
extern void Cloud_Protocol_EventPost_FwInfo_Init(void);
extern void Cloud_Protocol_EventPost_FwInfo_Set(cloud_protocol_sg_event_fireware_enum type, void *value);
extern void Cloud_Protocol_EventPost_FwInfoMainCtrl_Func(void);\
extern bool Cloud_Protocol_EventPost_FwInfo_HandleResponse(uint32_t msg_id);
extern bool Cloud_Protocol_EventPost_FwInfo_Post(void);
#endif /* __CLOUD_PROTOCOL_EVENTPOST_FWINFO_H__ */
/* EOL */
