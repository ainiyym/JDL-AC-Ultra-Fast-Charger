//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ChargingOrder_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Sg Cloud Protocol Charging Order module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_CHARGING_ORDER_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_CHARGING_ORDER_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_DataDef.h"
#include "Cloud_Protocol_Sg_Cfg.h"
#include "Cloud_Protocol_EventPost_Cfg.h"
#include "Cloud_Protocol_Sg_BillingMode.h"
#include "FlashDB_AppM.h"
#include "STD_SysM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* Maximum order quantity (two guns) */
#define CLOUD_PROTOCOL_SG_MAX_ORDERS                        SYS_CONNECTOR_NUM_MAX
#define CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN                  40
#define CLOUD_PROTOCOL_SG_OFFLINE_TIMEOUT                   300                     /* Offline timeout time (5 minutes =300 seconds) */
#define CLOUD_PROTOCOL_SG_ORDER_UPDATE_PERIOD               (60)                    /* Order update period in seconds(60') */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
/* Order Status enumeration */
typedef enum
{
    CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE = 0,        /* idle */
    CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING = 1, /* discharging */
    CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING = 2,    /* charging */
    CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED = 3,      /* paused */
    CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY = 4,     /* standby */
    CLOUD_PROTOCOL_SG_ORDER_STATUS_FAULT = 5,       /* fault */
} cloud_protocol_sg_order_status_t;

/* Order Operation Type enumeration */
typedef enum
{
    CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE = 0,    /* charge */
    CLOUD_PROTOCOL_SG_ORDER_OP_DISCHARGE = 1, /* discharge */
} cloud_protocol_sg_order_op_t;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* energy information (unit: 0.01kWh, 100=1kWh) */
typedef struct
{
    uint32_t total;  /* total energy */
    uint32_t peak;   /* peak energy */
    uint32_t hight;  /* hight energy */
    uint32_t normal; /* normal energy */
    uint32_t valley; /* valley energy */
} cloud_protocol_sg_order_energy_t;

typedef struct
{
    uint16_t voltage[V2G_MAX_VOL_CUR_DATA_LEN]; /* voltage(0.1V) */
    uint16_t current[V2G_MAX_VOL_CUR_DATA_LEN]; /* current(0.1A) */
    uint16_t power[V2G_MAX_POWER_DATA_LEN];     /* power(0.1kW) */
} cloud_protocol_sg_order_measure_value_t; 

typedef struct
{
    uint8_t gun_no;                                        /* Gun serial number (1-2) */
    char order_id[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN];     /* Order ID */
    uint8_t status;                                        /* Order status */
    uint8_t operation;                                     /* Charge/discharge operation type */
    uint32_t start_timestamp;                              /* start time (seconds) */
    uint32_t end_timestamp;                                /* end time (seconds) */
    cloud_protocol_sg_order_energy_t energy;               /* energy data */
    cloud_protocol_sg_order_measure_value_t measure_value; /* measurement values */
    uint8_t is_pending_upload : 1;                         /* pending upload flag */
    uint8_t reserved : 7;                                  /* reserved bits */
} cloud_protocol_sg_order_record_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern uint32_t Cloud_Protocol_Sg_Order_GetCurrentTime(void);
extern uint8_t Cloud_Protocol_Sg_Order_GetCurrentBeijingHour(void);
extern uint8_t Cloud_Protocol_Sg_Order_GetCurrentMinute(void);
extern cloud_protocol_sg_order_energy_t Cloud_Protocol_Sg_Order_GetTotalEnergy(uint8_t gun_no);\
extern uint32_t Cloud_Protocol_Sg_Order_GetDeltaEnergyValue(uint8_t gun_no);
extern bool Cloud_Protocol_Sg_Order_SaveOrderToTsdb(cloud_protocol_sg_order_record_t *order, fdb_time_t timestamp);
extern bool Cloud_Protocol_Sg_Order_LoadNextPendingFromTSDB(uint8_t gun_no, cloud_protocol_sg_order_record_t *record);
extern bool Cloud_Protocol_Sg_Order_RemovePendingFromTSDB(uint8_t gun_no, const char *order_id);
#endif /* __CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H */
/* EOL */
