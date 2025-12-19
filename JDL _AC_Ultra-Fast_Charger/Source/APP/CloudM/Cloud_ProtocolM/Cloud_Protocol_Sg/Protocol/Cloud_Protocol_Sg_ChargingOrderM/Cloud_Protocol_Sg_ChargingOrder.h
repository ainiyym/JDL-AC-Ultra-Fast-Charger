//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ChargingOrder.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Sg Charging Order module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_SG_CHARGING_ORDER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_SG_CHARGING_ORDER_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ChargingOrder_Cfg.h"
#include "Cloud_Protocol_EventPost_Cfg.h"

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
/* Order Data Structure */
typedef struct
{
    uint8_t gun_no;                                        /* gun number */
    char order_id[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN];     /* order ID */
    cloud_protocol_sg_order_status_t status;               /* order status */
    cloud_protocol_sg_order_op_t operation;                /* charge/discharge operation type */
    uint32_t start_timestamp;                              /* start time (seconds) */
    uint32_t end_timestamp;                                /* end time (seconds) */
    uint32_t offline_start_time;                           /* offline start time (seconds, record when network is offline) */
    cloud_protocol_sg_order_energy_t energy;               /* energy data */
    cloud_protocol_sg_order_measure_value_t measure_value; /* measurement values */
    bool is_offline_timed : 1;                             /* already entered offline timing flag */
    bool reserved : 8;                                     /* reserved bits */
} cloud_protocol_sg_order_t;

/* The global structure of the order module */
typedef struct
{
    cloud_protocol_sg_order_t orders[CLOUD_PROTOCOL_SG_MAX_ORDERS]; /* current orders */
    bool network_online;                                            /* network online status */
    bool is_uploading_pending;                                      /* is uploading pending orders */
} cloud_protocol_sg_order_manager_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
/* order management */
extern bool Cloud_Protocol_Sg_Order_Init(void);
extern void Cloud_Protocol_Sg_Order_Deinit(void);
/* order management */
extern bool Cloud_Protocol_Sg_Order_Start(uint8_t gun_no, const char *order_id, cloud_protocol_sg_order_op_t operation, cloud_protocol_sg_order_measure_value_t measure_value);
extern bool Cloud_Protocol_Sg_Order_Stop(uint8_t gun_no);
extern bool Cloud_Protocol_Sg_Order_Pause(uint8_t gun_no);
extern bool Cloud_Protocol_Sg_Order_Resume(uint8_t gun_no);
extern bool Cloud_Protocol_Sg_Order_ForceStop(uint8_t gun_no, bool save_to_tsdb);
/* update order */
extern void Cloud_Protocol_Sg_Order_UpdateMeasurements(uint8_t gun_no, cloud_protocol_sg_order_measure_value_t measure_value);
/* network status handling */
extern void Cloud_Protocol_Sg_Order_SetNetworkStatus(bool online);
/* period task */
extern void Cloud_Protocol_Sg_Order_TimerTask(void);
/* power failure handling */
void Cloud_Protocol_Sg_Order_HandlePowerFailure(void);
/* event post response handling */
extern void Cloud_Protocol_Sg_Order_GetOrderOnRunningStatus(uint8_t* gun1, uint8_t* gun2);
extern void Cloud_Protocol_EventPost_PileWorkstatus_Post(uint8_t gun_no);
extern bool Cloud_Protocol_EventPost_PostPileWorkstatus_Response(uint32_t msg_id);
#endif /* __CLOUD_PROTOCOL_SG_CHARGING_ORDER_H */
/* EOL */
