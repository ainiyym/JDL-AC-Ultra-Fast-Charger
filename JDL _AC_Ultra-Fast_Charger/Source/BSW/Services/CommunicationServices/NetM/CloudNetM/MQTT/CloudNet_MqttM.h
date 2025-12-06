//******************************************************************************
//* File Name: CloudNet_MqttM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: CloudNet Mqtt module mannager header file
/*******************************************************************************/
#if !defined (__CLOUDNET_MQTTM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUDNET_MQTTM_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Mqtt_Cfg.h"
#include "CloudNet_Mqtt_PublishM.h"

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
extern void CloudNetM_MqttSetSignalStrength(uint16_t signal_strength);
extern void CloudNetM_MqttSetConnectionStatus(bool is_connected);
extern bool CloudNetM_MqttConnect(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttDisconnect(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttPublish(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttSubscribe(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttUnsubscribe(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttSubscribePublish(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern bool CloudNetM_MqttSetWill(uint8_t socket_id, uint8_t *payload, uint16_t length);
extern void CloudNet_MqttM_Init(void);
extern void CloudNet_MqttM_Main(void);
#endif /* __CLOUDNET_MQTTM_H */
/* EOL */
