//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ConfigUpdate.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol Config Update module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_CONFIGUPDATE_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_CONFIGUPDATE_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_DataDef.h"
#include "Cloud_Protocol_Sg_Cfg.h"

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
extern void Cloud_Protocol_Sg_Config_Init(void);
extern bool Cloud_Protocol_Sg_ParseConfigUpdateParam(cJSON *params, const char *msg_id);
extern bool Cloud_Protocol_Sg_ParseQueryConfigParam(cJSON *params, const char *msg_id);
#endif /* __FILETEMPLATE_H */
/* EOL */
