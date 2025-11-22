//******************************************************************************
//* File Name: Cloud_Protocol_Sg_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "infra_state.h"
#include "infra_md5.h"
#include "infra_sha1.h"
#include "Cloud_Protocol_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define DEV_SIGN_SOURCE_MAXLEN              (200)
#define SIGN_FMT_LEN                        50                                      /* "clientId%sdeviceName%sproductKey%stimestamp%s";*/
#define CLOUDM_SG_PRODUCT_TIMESTAMP_VALUE   2524608000000                           /* use fixed timestamp */
#define CLOUDM_SG_DEVICE_NAME               "971762846196581146388515"              /* Device Name */ 
#define CLOUDM_SG_PRODUCT_KEY               "a1D0siVHgRU"                           /* Product Key */ 
#define CLOUDM_SG_DEVICE_SECRET             "a7a202f77a28bcb47c264dacb3a66ff7"      /* Device Secret */ 
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_PROTOCOL_MQTT_SIGN_HMACMD5,
    CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1
} Cloud_Protocol_Mqtt_SignMethod;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_PROTOCOL_SG_CFG_H */
/* EOL */
