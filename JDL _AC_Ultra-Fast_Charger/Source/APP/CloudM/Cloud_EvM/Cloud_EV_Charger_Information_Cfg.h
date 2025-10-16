//******************************************************************************
//* File Name: Cloud_EV_Charger_Information_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud EV Charger Information module header file
/*******************************************************************************/
#if !defined (__CLOUD_EV_CHARGER_INFORMATION_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_EV_CHARGER_INFORMATION_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "STD_SysM_Cfg.h"
#include "Cloud_Protocol_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_EV_SN_LEN                (16)
#define CLOUD_EV_MANUFACTURER_LEN      (32U)
#define CLOUD_EV_FIRMWARE_VERSION_LEN  (8U)
#define CLOUD_EV_HARDWARE_VERSION_LEN  (8U)

#define CLOUD_EV_MAX_CONNECTORS        (SYS_CONNECTOR_NUM_MAX) // Maximum number of connectors supported

#define CLOUD_EV_MAX_CHARGING_VOLTAGE   (1000U) // V
#define CLOUD_EV_MAX_CHARGING_CURRENT   (125U) // A
#define CLOUD_EV_MAX_TEMPERATURE        (150U)  // °C

#define CLOUD_EV_DEBUG(fmt, ...) 	                                    LOG_DEBUG(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_EV_ERR(fmt, ...) 	                                        LOG_ERROR(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_EVSE_STATUS_OFFLINE = 0, // Offline
    CLOUD_EVSE_STATUS_FAULTED = 1, // Faulted
    CLOUD_EVSE_STATUS_IDLE = 2,    // Idle
    CLOUD_EVSE_STATUS_CHARGING = 3 // Charging
} Cloud_Evse_StatusType_E;

typedef enum
{
    CLOUD_EV_CHARGER_CONNECTOR_NORMAL = 0, // Normal
    CLOUD_EV_CHARGER_CONNECTOR_ERROR = 1, // Error
} Cloud_Ev_Connector_StatusType_E;

typedef enum
{
    CLOUD_EV_CHARGER_CONNECTOR_INUSE = 0, // In Use
    CLOUD_EV_CHARGER_CONNECTOR_GO_BACK = 1, // Go Back
    CLOUD_EV_CHARGER_CONNECTOR_UNKNOWN = 2 // Unknown
} Cloud_Ev_ConnectorGoBack_StatusType_E;

typedef enum
{
    CLOUD_EV_TRANSACTION_ID_TYPE_APP = 0,
    CLOUD_EV_TRANSACTION_ID_TYPE_RFID = 1,
    CLOUD_EV_TRANSACTION_ID_TYPE_RFID_OFFLINE = 2,
    CLOUD_EV_TRANSACTION_ID_TYPE_VIN = 3
} Cloud_Ev_Transaction_IdentifierType_E;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_EV_CHARGER_INFORMATION_CFG_H */
/* EOL */
