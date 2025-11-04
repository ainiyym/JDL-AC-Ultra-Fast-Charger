//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrder.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Charging Order module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CHARGING_ORDER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CHARGING_ORDER_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_ChargingOrder_Cfg.h"

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
typedef void (*cloud_protocol_order_upload_cb_t)(const cloud_protocol_charging_cloud_protocol_order_manager_t *order);
typedef bool (*cloud_protocol_network_status_cb_t)(void);
typedef cloud_protocol_order_charging_status_t (*cloud_protocol_charging_status_cb_t)(uint8_t gun_no);
typedef cloud_protocol_order_auth_status_t (*cloud_protocol_auth_status_cb_t)(uint8_t gun_no);
typedef cloud_protocol_order_connector_status_t (*cloud_protocol_connector_status_cb_t)(uint8_t gun_no);
typedef uint32_t (*cloud_protocol_get_meter_reading_cb_t)(uint8_t gun_no);

typedef struct
{
    uint8_t SN[CLOUD_PROTOCOL_SN_LENGTH];                      // pile number
    uint16_t sequence[CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS]; // Self-increasing serial number (independent for each gun)
} cloud_protocol_transaction_id_config_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern bool cloud_protocol_order_manager_init(cloud_protocol_order_upload_cb_t upload_cb,
                                              cloud_protocol_network_status_cb_t network_status_cb,
                                              cloud_protocol_charging_status_cb_t charging_status_cb,
                                              cloud_protocol_auth_status_cb_t auth_status_cb,
                                              cloud_protocol_connector_status_cb_t connector_status_cb,
                                              cloud_protocol_get_meter_reading_cb_t get_meter_reading_cb);
extern bool cloud_protocol_charging_order_init(const cloud_protocol_transaction_id_config_t *config);
extern bool cloud_protocol_order_creat(uint8_t gun_no, cloud_protocol_order_type_t type,
                                        cloud_protocol_transaction_type_t trans_type,
                                        const uint8_t *platform_transaction_id,
                                        const char *vin, const uint8_t *card);
extern void cloud_protocol_order_manager_process(void);
extern void cloud_protocol_order_handle_power_loss(void);
#endif /* __CLOUD_PROTOCOL_CHARGING_ORDER_H */
/* EOL */
