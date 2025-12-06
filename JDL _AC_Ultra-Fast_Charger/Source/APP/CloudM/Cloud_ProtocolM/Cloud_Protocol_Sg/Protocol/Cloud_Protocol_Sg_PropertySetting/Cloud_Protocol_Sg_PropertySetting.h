//******************************************************************************
//* File Name: Cloud_Protocol_Sg_PropertySetting.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol set property module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_PROTERTYSETTING_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FILETEMPLATE___CLOUD_PROTOCOL_SG_PROTERTYSETTING_HH
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
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
extern bool cloud_protocol_property_setting_response(const char *payload, uint16_t payload_len);

#endif /* __CLOUD_PROTOCOL_SG_PROTERTYSETTING_H */
/* EOL */
