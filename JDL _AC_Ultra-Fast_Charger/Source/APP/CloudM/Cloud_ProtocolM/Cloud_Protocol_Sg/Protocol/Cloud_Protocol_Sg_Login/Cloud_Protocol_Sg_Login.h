//******************************************************************************
//* File Name: Cloud_Protocol_Sg_Login.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module log in header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_LOGIN_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_LOGIN_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"
#include <inttypes.h>
#include "stdint.h"
#include "string.h"

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
int32_t Cloud_Protocol_Mqtt_Sign(iotx_dev_meta_info_t *meta, uint64_t timestamp, iotx_sign_mqtt_t *signout);

#endif /* __CLOUD_PROTOCOL_SG_LOGIN_H */
/* EOL */
