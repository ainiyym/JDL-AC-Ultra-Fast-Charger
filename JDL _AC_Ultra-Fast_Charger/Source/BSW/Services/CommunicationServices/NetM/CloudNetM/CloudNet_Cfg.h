//******************************************************************************
//* File Name: CloudNet_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Net module configuration header file
/*******************************************************************************/
#if !defined (__CLOUDNET_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUDNET_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUDNET_MQTT_PROCESS_INTERVAL_MS 			10	  	// processing interval 10 ms
#define CLOUDNET_MQTT_AT_RESPONSE_TIMEOUT_MS 		5000 	// AT response timeout 5 seconds
#define CLOUDNET_MQTT_MAX_QUEUE_SIZE 				50		// maximum queue size
#define CLOUDNET_MQTT_MAX_RETRY_COUNT 				1		// maximum retry attempts

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
extern char *CloudNet_Strdup(const char *s);
#endif /* __CLOUDNET_CFG_H */
/* EOL */
