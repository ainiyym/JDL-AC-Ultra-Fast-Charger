//******************************************************************************
//* File Name: Cloud_Protocol_SgM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_SgM.h"
#include "Cloud_Protocol_Mqtt_Cfg.h"
#include "Cloud_Protocol_Sg_Login.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static iotx_dev_meta_info_t Cloud_Protocol_Meta;
static iotx_sign_mqtt_t Cloud_Protocol_Sign;

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_Mqtt_init(void)
{
    uint64_t timestamp = CLOUDM_SG_PRODUCT_TIMESTAMP_VALUE; // Fixed timestamp

    memset(&Cloud_Protocol_Meta, 0, sizeof(iotx_dev_meta_info_t));
    memset(&Cloud_Protocol_Sign, 0, sizeof(iotx_sign_mqtt_t));

    strncpy(Cloud_Protocol_Meta.product_key, CLOUDM_SG_PRODUCT_KEY, IOTX_PRODUCT_KEY_LEN);
    strncpy(Cloud_Protocol_Meta.device_name, CLOUDM_SG_DEVICE_NAME, IOTX_DEVICE_NAME_LEN);
    strncpy(Cloud_Protocol_Meta.device_secret, CLOUDM_SG_DEVICE_SECRET, IOTX_DEVICE_SECRET_LEN);

    Cloud_Protocol_Mqtt_Sign(&Cloud_Protocol_Meta, timestamp, &Cloud_Protocol_Sign);
    Cloud_Protocol_Mqtt_ClientManagerInit(cloud_protocol_mqtt_topic_configs, CLOUD_PROTOCOL_MQTT_TOPIC_CONFIG_COUNT, &Cloud_Protocol_Sign, NULL, NULL);
}

void Cloud_Protocol_Mqtt_MainProcess(void)
{
    // MQTT client manager process
    Cloud_Protocol_Mqtt_ClientManagerProcess();
}
/* EOL */
