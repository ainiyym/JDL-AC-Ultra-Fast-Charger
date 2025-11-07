//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrder_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol Charging Order module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_ChargingOrderInfomation.h"
#include "Cloud_Ev_Charger_Information.h"
#include "FreeRTOS.h"
#include "task.h"
#include "STD_Rtc.h"
#include "FlashDB_AppM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_ORDER_PROCESS_PERIOD_MS                                 (100U) // Order processing task period time in ms
#define CLOUD_PROTOCOL_ORDER_UPDATE_PERIOD_MS                                  (30 * 1000U) // Order update period time in ms
#define CLOUD_PROTOCOL_ORDER_OFFLINE_UPLOAD_CHECK_PERIOD_MS                    (60 * 1000U) // Offline order upload check period time in ms
#define CLOUD_PROTOCOL_ORDER_AUTH_CHECK_PERIOD_MS                              (5 * 60 * 1000U) // Order authorization check period time in ms

#define CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS                                  (CLOUD_EV_MAX_CONNECTORS) // Maximum number of guns supported by the order module
#define CLOUD_PROTOCOL_GET_BILLING_MODEL_TIME_SLOT_INFO()                       Cloud_Protocol_GetBillingModelTimeSlotInfo() // get billing model time slot info

#define CLOUD_PROTOCOL_READ_ORDER_SEQUENCE(buff, bufflen)                       FlashDB_ReadValue(FLASHDB_KV_ORDER_SEQUENCE, buff, bufflen, NULL) // read order sequence from storage
#define CLOUD_PROTOCOL_SAVE_ORDER_SEQUENCE(buff, bufflen)                       FlashDB_WriteValue(FLASHDB_KV_ORDER_SEQUENCE, buff, bufflen) // save order sequence to storage
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
// Enumeration of charging status
typedef enum
{
    CHARGING_STATUS_IDLE = 0,  // Idle
    CHARGING_STATUS_CONNECTED, // Connected
    CHARGING_STATUS_CHARGING,  // Charging
    CHARGING_STATUS_STOPPED,   // Stopped
    CHARGING_STATUS_ERROR      // Error
} cloud_protocol_order_charging_status_t;

// Authorization status enumeration
typedef enum
{
    AUTH_STATUS_UNAUTHORIZED = 0, // Unauthorized
    AUTH_STATUS_AUTHORIZED        // Authorized
} cloud_protocol_order_auth_status_t;

// Charging connector status enumeration
typedef enum
{
    CONNECTOR_STATUS_DISCONNECTED = 0, // Disconnected
    CONNECTOR_STATUS_CONNECTED         // Connected
} cloud_protocol_order_connector_status_t;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern uint32_t cloud_protocol_fixed_5dec(float value);
extern uint32_t cloud_protocol_fixed_4dec(float value);
extern uint64_t cloud_protocol_fixed_4dec_64(float value);
extern float cloud_protocol_fixed_5dec_to_float(uint32_t value);
extern float cloud_protocol_fixed_4dec_to_float(uint32_t value);

extern cloud_protocol_charging_cloud_protocol_order_manager_t *cloud_protocol_get_active_charging_order(uint8_t gun_no);
extern bool cloud_protocol_call_upload_offline_order(cloud_protocol_charging_cloud_protocol_order_manager_t *order);

extern bool cloud_protocol_save_order_to_tsdb(uint8_t gun_no);
extern bool cloud_protocol_update_active_order_to_tsdb(uint8_t gun_no);
extern bool cloud_protocol_delete_active_order_in_tsdb(uint8_t gun_no);
extern bool cloud_protocol_process_offline_order_upload(uint8_t gun_no);
#endif /* __CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H */
/* EOL */
