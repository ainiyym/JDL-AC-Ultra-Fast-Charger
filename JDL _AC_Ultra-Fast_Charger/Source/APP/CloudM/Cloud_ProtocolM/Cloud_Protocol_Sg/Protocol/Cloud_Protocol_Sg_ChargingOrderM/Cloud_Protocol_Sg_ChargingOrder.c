//******************************************************************************
//* File Name: Cloud_Protocol_Sg_ChargingOrder.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Sg Charging Order module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_ChargingOrder.h"
#include "Cloud_Protocol_EventPost.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint32_t send_message_id; // Sent message ID
    uint32_t last_post_time;  // Last post time
    char order_id[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN]; /* order ID being uploaded */
    bool on_running;                             /* is uploading running flag */
} cloud_protocol_event_post_pile_work_status_ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_sg_order_manager_t cloud_protocol_sg_order_manager = {0};
static cloud_protocol_event_post_pile_work_status_ctrl_t cloud_protocol_event_post_pile_work_status_ctrl[CLOUD_PROTOCOL_SG_MAX_GUN_NUM] = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool Cloud_Protocol_Sg_Order_ValidateGunNo(uint8_t gun_no);
static void Cloud_Protocol_Sg_Order_Reset(cloud_protocol_sg_order_t *order);
static bool Cloud_Protocol_Sg_Order_HandleOfflineTimeout(uint8_t gun_no);
static void Cloud_Protocol_Sg_Order_StopAllActiveOrders(bool save_to_tsdb);
static uint8_t Cloud_Protocol_Sg_Order_GetActiveCount(uint8_t gun_no);
static void Cloud_Protocol_Sg_Order_UpdateEnergyByTime(cloud_protocol_sg_order_t *order, uint32_t energy_delta);
static void Cloud_Protocol_Sg_Order_UpdateEnergy(uint8_t gun_no, uint32_t energy_delta);
static bool Cloud_Protocol_Sg_Order_IsNetworkOnline(void);
static cloud_protocol_sg_order_t* Cloud_Protocol_Sg_Order_Get(uint8_t gun_no);
static bool Cloud_Protocol_Sg_Order_ExportToV2GStruct(cloud_protocol_sg_order_record_t *order, v2g_event_pile_workstatus *output);
static void Cloud_Protocol_Sg_Order_ConvertOrderToRecord(const cloud_protocol_sg_order_t *src, cloud_protocol_sg_order_record_t *dst, bool is_pending_upload);
static void Cloud_Protocol_EventPost_SetGun1PileWorkStatusMsgId(uint32_t msg_id);
static void Cloud_Protocol_EventPost_SetGun2PileWorkStatusMsgId(uint32_t msg_id);
static void Cloud_Protocol_Sg_Order_SetPileWorkStatusOrderId(uint8_t gun_no, char *order_id);
static bool Cloud_Protocol_Sg_Order_PostPileWorkstatus(v2g_event_pile_workstatus *data);
static bool Cloud_Protocol_Sg_Order_SaveToTSDB(const cloud_protocol_sg_order_t *order);
static bool Cloud_Protocol_Sg_Order_GetNextPendingRecord(uint8_t gun_no, v2g_event_pile_workstatus *output);
static bool Cloud_Protocol_Sg_Order_NotifyUploadComplete(uint8_t gun_no, const char *order_id, bool success);
static void Cloud_Protocol_Sg_Order_HandleOnlineRunningOrders(void);
static void Cloud_Protocol_Sg_Order_HandlePendingUploadOrders(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief initialize the order module
 */
bool Cloud_Protocol_Sg_Order_Init(void)
{
    memset(&cloud_protocol_sg_order_manager, 0, sizeof(cloud_protocol_sg_order_manager_t));
    memset(&cloud_protocol_event_post_pile_work_status_ctrl, 0, sizeof(cloud_protocol_event_post_pile_work_status_ctrl_t));
    /* set network status */
    cloud_protocol_sg_order_manager.network_online = false;
    cloud_protocol_sg_order_manager.is_uploading_pending = false;

    /* Initialize the order for two guns */
    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        cloud_protocol_sg_order_t *order = &cloud_protocol_sg_order_manager.orders[i];
        order->gun_no = i + 1; /* gun serial number starts from 1 */
        Cloud_Protocol_Sg_Order_Reset(order);
    }

    CLOUD_INFO("<%s> Order module initialized successfully\r\n", __func__);
    return true;
}

/**
 * @brief deinitialize the order module
 */
void Cloud_Protocol_Sg_Order_Deinit(void)
{
    CLOUD_INFO("<%s> Deinitializing order module...\r\n", __func__);

    /* save all active orders */
    Cloud_Protocol_Sg_Order_StopAllActiveOrders(true);

    /* clean all resources */
    memset(&cloud_protocol_sg_order_manager, 0, sizeof(cloud_protocol_sg_order_manager_t));
}

/**
 * @brief Verify the validity of the gun serial number
 */
static bool Cloud_Protocol_Sg_Order_ValidateGunNo(uint8_t gun_no)
{
    if (gun_no < 1 || gun_no > CLOUD_PROTOCOL_SG_MAX_GUN_NUM)
    {
        CLOUD_ERROR("<%s> Invalid gun number: %d\r\n", __func__, gun_no);
        return false;
    }
    return true;
}

/**
 * @brief Reset the order to its initial state
 */
static void Cloud_Protocol_Sg_Order_Reset(cloud_protocol_sg_order_t *order)
{
    if (order == NULL)
        return;

    memset(order, 0, sizeof(cloud_protocol_sg_order_t));
    order->gun_no = 0;
    order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE;
    order->operation = CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE;

    order->is_offline_timed = false;
}

/**
 * @brief start an order
 */
bool Cloud_Protocol_Sg_Order_Start(uint8_t gun_no, const char *order_id, cloud_protocol_sg_order_op_t operation)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return false;
    }

    if (order_id == NULL || strlen(order_id) == 0)
    {
        CLOUD_ERROR("<%s> Order ID cannot be empty\r\n", __func__);
        return false;
    }

    /* check if the gun is idle */
    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return false;
    }

    if (order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE)
    {
        CLOUD_WARN("<%s> Gun %d is not idle (status: %d)\r\n", __func__, gun_no, order->status);
        return false;
    }

    CLOUD_INFO("<%s> Starting %s order on gun %d, ID: %s\r\n", __func__, operation == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE ? "charging" : "discharging", gun_no, order_id);

    /* reset the order */
    Cloud_Protocol_Sg_Order_Reset(order);

    order->gun_no = gun_no;
    strncpy(order->order_id, order_id, sizeof(order->order_id) - 1);
    order->order_id[sizeof(order->order_id) - 1] = '\0';

    order->operation = operation;
    order->start_timestamp = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* set status */
    if (operation == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
    {
        order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING;
    }
    else
    {
        order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING;
    }

    /* upload current order */
    // bool result = Cloud_Protocol_EventPost_PileWorkstatus_Post(gun_no);
    // if (result)
    // {
    //     cloud_protocol_event_post_pile_work_status_ctrl[gun_no - 1].last_post_time = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();
    // }
    // else
    // {
    //     Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS, 10 * 1000); // retry later
    // }

    CLOUD_INFO("<%s> Order started successfully on gun %d\r\n", __func__, gun_no);
    return true;
}

/**
 * @brief stop an order(normal stop)
 */
bool Cloud_Protocol_Sg_Order_Stop(uint8_t gun_no, char *order_id, char *reason)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return false;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        CLOUD_ERROR("<%s> Order not found on gun %d\r\n", __func__, gun_no);
        if (reason != NULL)
        {
            strncpy(reason, "Order not found", V2G_MAX_RESULT_LEN - 1);
            reason[V2G_MAX_RESULT_LEN - 1] = '\0';
        }
        return false;
    }

    if (order_id != NULL && strcmp(order->order_id, order_id) != 0)
    {
        CLOUD_ERROR("<%s> Order ID mismatch on gun %d (expected: %s, got: %s)\r\n", __func__, gun_no, order->order_id, order_id);
        if (reason != NULL)
        {
            strncpy(reason, "Order ID mismatch", V2G_MAX_RESULT_LEN - 1);
            reason[V2G_MAX_RESULT_LEN - 1] = '\0';
        }
        return false;
    }

    /* check if the order is in active state */
    if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE ||
        order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY)
    {
        CLOUD_WARN("<%s> Gun %d is not in active state (status: %d)\r\n", __func__, gun_no, order->status);
        if (reason != NULL)
        {
            strncpy(reason, "Order not active", V2G_MAX_RESULT_LEN - 1);
            reason[V2G_MAX_RESULT_LEN - 1] = '\0';
        }
        return false;
    }

    Cloud_Protocol_EventPost_DisableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS);
    CLOUD_INFO("<%s> Stopping order on gun %d, order ID: %s\r\n", __func__, gun_no, order->order_id);

    /* update the order end time */
    order->end_timestamp = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* set status to completed */
    order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY;
    order->is_offline_timed = false; 

    /* upload current order */
    bool result = Cloud_Protocol_EventPost_PileWorkstatus_Post(gun_no);

    if (!result)
    {
        Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS, 10); // retry later
    }

    /* reset auth */
    NETAUTH_SetReqCancelAuthStatus((SysConnector_Num_Enum)(gun_no - 1));
    if (reason != NULL)
    {
        strncpy(reason, "Order stopped successfully", V2G_MAX_RESULT_LEN - 1);
        reason[V2G_MAX_RESULT_LEN - 1] = '\0';
    }
    CLOUD_INFO("<%s> Order stopped successfully on gun %d\r\n", __func__, gun_no);
    return true;
}

/**
 * @brief Forced order suspension (for abnormal situations)
 */
bool Cloud_Protocol_Sg_Order_ForceStop(uint8_t gun_no, bool save_to_tsdb)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return false;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return false;
    }

    /* only active orders need to be force stopped */
    if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE || order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY)
    {
        return true;
    }

    CLOUD_WARN("<%s> Force stopping order on gun %d, order ID: %s, save: %d\r\n", __func__, gun_no, order->order_id, save_to_tsdb);

    /* update the order end time */
    order->end_timestamp = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* set status to completed */
    order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY;
    order->is_offline_timed = false;

    bool result = true;
    /* if save_to_tsdb is true, save to TSDB */
    if (save_to_tsdb)
    {
        result = Cloud_Protocol_Sg_Order_SaveToTSDB(order);
    }
    else
    {
        /* upload current order */
        result = Cloud_Protocol_EventPost_PileWorkstatus_Post(gun_no);
        if (!result)
        {
            Cloud_Protocol_EventPost_EnableTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS, 10); // retry later
        }
    }

    return result;
}

/**
 * @brief pause an order
 */
bool Cloud_Protocol_Sg_Order_Pause(uint8_t gun_no, const char *order_id)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return false;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL || (order_id != NULL && strcmp(order->order_id, order_id) != 0))
    {
        return false;
    }

    /* check if can be paused */
    if (order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING && order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING)
    {
        CLOUD_WARN("<%s> Gun %d cannot be paused (status: %d)\r\n", __func__, gun_no, order->status);
        return false;
    }

    CLOUD_INFO("<%s> Pausing order on gun %d\r\n", __func__, gun_no);

    order->status = CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED;

    cloud_protocol_event_post_pile_work_status_ctrl[gun_no - 1].last_post_time = CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP();

    return true;
}

/**
 * @brief resume an order
 */
bool Cloud_Protocol_Sg_Order_Resume(uint8_t gun_no)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return false;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return false;
    }

    /* check if can be resumed */
    if (order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED)
    {
        CLOUD_WARN("<%s> Gun %d is not paused (status: %d)\r\n", __func__, gun_no, order->status);
        return false;
    }

    CLOUD_INFO("<%s> Resuming order on gun %d\r\n", __func__, gun_no);

    /* resume previous status */
    cloud_protocol_sg_order_status_t prev_status = (order->operation == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE) ? CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING : CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING;

    order->status = prev_status;
    order->is_offline_timed = false;

    return true;
}

/**
 * @brief update order measurements(during the order cycle of the this charging/discharging)
 */
void Cloud_Protocol_Sg_Order_UpdateMeasurements(uint8_t gun_no, cloud_protocol_sg_order_measure_value_t measure_value)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return;
    }

    /* update measurements */
    order->measure_value = measure_value;
}

/**
 * @brief parse and update energy by time segments
 */
static void Cloud_Protocol_Sg_Order_UpdateEnergyByTime(cloud_protocol_sg_order_t *order, uint32_t energy_delta)
{
    if (order == NULL)
        return;

    /* get current hour and minute */
    uint8_t hour = Cloud_Protocol_Sg_Order_GetCurrentBeijingHour();
    uint8_t minute = Cloud_Protocol_Sg_Order_GetCurrentMinute();
    uint8_t seg_flag = Cloud_Protocol_Sg_GetCurrentSegFlag(hour, minute);

    /* Allocate electricity according to time periods */
    switch (seg_flag)
    {
        case CLOUD_PROTOCOL_SG_SEG_FLAG_PEAK: /* peak */
            order->energy.peak += energy_delta;
            break;
        case CLOUD_PROTOCOL_SG_SEG_FLAG_HIGH: /* hight */
            order->energy.hight += energy_delta;
            break;
        case CLOUD_PROTOCOL_SG_SEG_FLAG_NORMAL: /* normal */
            order->energy.normal += energy_delta;
            break;
        case CLOUD_PROTOCOL_SG_SEG_FLAG_VALLEY: /* valley */
            order->energy.valley += energy_delta;
            break;
        default: /* default normal */
            order->energy.normal += energy_delta;
            break;
    }
}

/**
 * @brief update order energy
 */
static void Cloud_Protocol_Sg_Order_UpdateEnergy(uint8_t gun_no, uint32_t energy_delta)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return;
    }

    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return;
    }

    /* update */
    order->energy.total += energy_delta;

    /* dispatch energy by time segments */
    Cloud_Protocol_Sg_Order_UpdateEnergyByTime(order, energy_delta);

    CLOUD_DEBUG("<%s> Gun %d energy updated: delta=%u, total=%u\r\n",  __func__, gun_no, energy_delta, order->energy.total);
}

/**
 * @brief set network status
 */
void Cloud_Protocol_Sg_Order_SetNetworkStatus(bool online)
{
    if (cloud_protocol_sg_order_manager.network_online == online)
    {
        return;
    }

    CLOUD_INFO("<%s> Network status changed: %s\r\n", __func__, online ? "online" : "offline");

    cloud_protocol_sg_order_manager.network_online = online;

    if (!online)
    {
        /* mark all active orders for offline timeout */
        for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
        {
            cloud_protocol_sg_order_t *order = &cloud_protocol_sg_order_manager.orders[i];

            if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING ||
                order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING ||
                order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED)
            {
                order->offline_start_time = Cloud_Protocol_Sg_Order_GetCurrentTime();
                order->is_offline_timed = true;
                CLOUD_WARN("<%s> Gun %d order %s marked for offline timeout\r\n", __func__, order->gun_no, order->order_id);
            }
        }
    }
    else
    {
        /* stop uploading pending */
        cloud_protocol_sg_order_manager.is_uploading_pending = false;
    }
}

/**
 * @brief export order data to V2G structure
 */
static bool Cloud_Protocol_Sg_Order_ExportToV2GStruct(cloud_protocol_sg_order_record_t *order, v2g_event_pile_workstatus *output)
{
    if (order == NULL || output == NULL)
    {
        return false;
    }

    /* clear output structure */
    memset(output, 0, sizeof(v2g_event_pile_workstatus));

    /* fill basic information */
    output->gunNo = order->gun_no;
    strncpy(output->applySheetNo, order->order_id, V2G_MAX_TRADE_LEN - 1);
    output->applySheetNo[V2G_MAX_TRADE_LEN - 1] = '\0';
    output->executionStatus = (unsigned char)order->status;

    /* fill cumulative charging/discharging time (minutes) */
    if (order->start_timestamp > 0)
    {
        uint32_t current_time = Cloud_Protocol_Sg_Order_GetCurrentTime();
        if (order->end_timestamp > 0)
        {
            output->cumulativeTime = (order->end_timestamp - order->start_timestamp) / 60;
        }
        else
        {
            output->cumulativeTime = (current_time - order->start_timestamp) / 60;
        }
    }

    /* set all dc parameter equal to 0 */
    memset(output->chargingEnergyValue, 0, sizeof(output->chargingEnergyValue));
    memset(output->disChargingEnergyValue, 0, sizeof(output->disChargingEnergyValue));
    memset(output->cumulativeCharge, 0, sizeof(output->cumulativeCharge));
    memset(output->cumulativeDischarge, 0, sizeof(output->cumulativeDischarge));

    /* set dc measurement values to 0 */
    output->voltage = 0;
    output->current = 0;
    output->electricPower = 0;

    /* copy ac measurement values */
    for (int i = 0; i < V2G_MAX_VOL_CUR_DATA_LEN; i++)
    {
        output->acVoltage[i] = order->measure_value.voltage[i];
        output->acCurrent[i] = order->measure_value.current[i];
    }
    for (int i = 0; i < V2G_MAX_POWER_DATA_LEN; i++)
    {
        output->acElectricPower[i] = order->measure_value.power[i];
    }

    /* fill ac charging/discharge energy data */
    if (order->operation == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
    {
        /* get total energy */
        cloud_protocol_sg_order_energy_t total_energy = Cloud_Protocol_Sg_Order_GetTotalEnergy(order->gun_no);
        for (int i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
        {
            output->acChargingEnergyValue[i] = ((uint32_t*)&total_energy)[i];
        }
        memset(output->acDisChargingEnergyValue, 0, sizeof(output->acDisChargingEnergyValue));
        /* set ac charge energy equal to charge energy */
        output->acCumulativeCharge[0] = order->energy.total;  /* total */
        output->acCumulativeCharge[1] = order->energy.hight;  /* peak */
        output->acCumulativeCharge[2] = order->energy.peak;   /* peak */
        output->acCumulativeCharge[3] = order->energy.normal; /* normal */
        output->acCumulativeCharge[4] = order->energy.valley; /* valley */
        /* set ac discharge energy equal to 0 */
        memset(output->acCumulativeDischarge, 0, sizeof(output->acCumulativeDischarge));
    }
    else
    {
        /* get total energy */
        cloud_protocol_sg_order_energy_t total_energy = Cloud_Protocol_Sg_Order_GetTotalEnergy(order->gun_no);
        for (int i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
        {
            output->acDisChargingEnergyValue[i] = ((uint32_t*)&total_energy)[i];
        }
        memset(output->acChargingEnergyValue, 0, sizeof(output->acChargingEnergyValue));

        /* discharge order: fill discharge energy, charging energy is 0 */
        output->acCumulativeDischarge[0] = order->energy.total;  /* total */
        output->acCumulativeDischarge[1] = order->energy.hight;  /* peak */
        output->acCumulativeDischarge[2] = order->energy.peak;   /* peak */
        output->acCumulativeDischarge[3] = order->energy.normal; /* normal */
        output->acCumulativeDischarge[4] = order->energy.valley; /* valley */
        /* set ac charge energy equal to 0  */
        memset(output->acCumulativeCharge, 0, sizeof(output->acCumulativeCharge));
    }

    return true;
}

/**
 * @brief convert order structure to record structure
 */
static void Cloud_Protocol_Sg_Order_ConvertOrderToRecord(const cloud_protocol_sg_order_t *src, cloud_protocol_sg_order_record_t *dst, bool is_pending_upload)
{
    if (src == NULL || dst == NULL)
    {
        return;
    }

    memset(dst, 0, sizeof(cloud_protocol_sg_order_record_t));
    dst->gun_no = src->gun_no;
    strncpy(dst->order_id, src->order_id, CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN - 1);
    dst->order_id[CLOUD_PROTOCOL_SG_MAX_ORDER_ID_LEN - 1] = '\0';
    dst->status = (uint8_t)src->status;       // cloud_protocol_sg_order_status_t -> uint8_t
    dst->operation = (uint8_t)src->operation; // cloud_protocol_sg_order_op_t -> uint8_t
    dst->start_timestamp = src->start_timestamp;
    dst->end_timestamp = src->end_timestamp;
    memcpy(&dst->energy, &src->energy, sizeof(cloud_protocol_sg_order_energy_t));
    memcpy(&dst->measure_value, &src->measure_value, sizeof(cloud_protocol_sg_order_measure_value_t));

    if (is_pending_upload)
    {
        dst->is_pending_upload = 1;
    }
    else
    {
        dst->is_pending_upload = 0;
    }
}

/**
 * @brief get next pending record for upload
 */
static bool Cloud_Protocol_Sg_Order_GetNextPendingRecord(uint8_t gun_no, v2g_event_pile_workstatus *output)
{
    if (output == NULL)
    {
        return false;
    }

    /* check if there are active orders, if so, do not upload */
    if (Cloud_Protocol_Sg_Order_GetActiveCount(gun_no) > 0)
    {
        return false;
    }

    /* check if is uploading */
    if (cloud_protocol_sg_order_manager.is_uploading_pending)
    {
        return false;
    }

    /* get next pending record */
    cloud_protocol_sg_order_record_t record = {0};
    Cloud_Protocol_Sg_Order_LoadNextPendingFromTSDB(gun_no, &record);

    if (memcmp(&record, &(cloud_protocol_sg_order_record_t){0}, sizeof(cloud_protocol_sg_order_record_t)) == 0)
    {
        return false;
    }

    /* export to V2G structure */
    bool result = Cloud_Protocol_Sg_Order_ExportToV2GStruct(&record, output);

    return result;
}

/**
 * @brief notify upload result (called after upload attempt)
 */
static bool Cloud_Protocol_Sg_Order_NotifyUploadComplete(uint8_t gun_no, const char *order_id, bool success)
{
    bool removed = false;

    if (order_id == NULL)
    {
        return false;
    }

    /* clear uploading flag */
    cloud_protocol_sg_order_manager.is_uploading_pending = false;

    if (success)
    {
        /* delete from TSDB */
        removed = Cloud_Protocol_Sg_Order_RemovePendingFromTSDB(gun_no, order_id);
    }
    else
    {
        /* retry later */
        CLOUD_WARN("<%s> Order %s upload failed, will retry later\r\n", __func__, order_id);
    }

    return removed;
}

/**
 * @brief save order to TSDB (mark as pending upload)
 */
static bool Cloud_Protocol_Sg_Order_SaveToTSDB(const cloud_protocol_sg_order_t *order)
{
    if (order == NULL)
    {
        return false;
    }

    /* switch to record */
    cloud_protocol_sg_order_record_t record = {0};
    Cloud_Protocol_Sg_Order_ConvertOrderToRecord(order, &record, true);

    /* use end timestamp as timestamp */
    fdb_time_t timestamp = (fdb_time_t)order->end_timestamp;

    /* save to TSDB */
    bool result = Cloud_Protocol_Sg_Order_SaveOrderToTsdb(&record, timestamp);

    if (result == true)
    {
        CLOUD_INFO("<%s> Order %s saved to TSDB (timestamp: %lu)\r\n", __func__, order->order_id, timestamp);
        return true;
    }
    else
    {
        CLOUD_ERROR("<%s> Failed to save order %s to TSDB: %d\r\n", __func__, order->order_id, result);
        return false;
    }
}

/**
 * @brief check and handle offline timeout for an order
 */
static bool Cloud_Protocol_Sg_Order_HandleOfflineTimeout(uint8_t gun_no)
{
    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL)
    {
        return false;
    }

    /* whether the order is offline timed */
    if (!order->is_offline_timed)
    {
        return false;
    }

    uint32_t current_time = Cloud_Protocol_Sg_Order_GetCurrentTime();
    uint32_t elapsed = current_time - order->offline_start_time;

    /* check if timeout */
    if (elapsed >= CLOUD_PROTOCOL_SG_OFFLINE_TIMEOUT)
    {
        CLOUD_WARN("<%s> Gun %d order %s offline timeout (%u s), forcing stop\r\n", __func__, gun_no, order->order_id, elapsed);

        /* force stop current order */
        Cloud_Protocol_Sg_Order_ForceStop(gun_no, true);
        return true;
    }

    return false;
}

/**
 * @brief Stop all activity orders (for abnormal situations)
 */
static void Cloud_Protocol_Sg_Order_StopAllActiveOrders(bool save_to_tsdb)
{
    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        cloud_protocol_sg_order_t *order = &cloud_protocol_sg_order_manager.orders[i];

        if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING ||
            order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING ||
            order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED)
        {
            Cloud_Protocol_Sg_Order_ForceStop(order->gun_no, save_to_tsdb);
        }
    }
}

/**
 * @brief get the count of active orders
 */
static uint8_t Cloud_Protocol_Sg_Order_GetActiveCount(uint8_t gun_no)
{
    uint8_t count = 0;

    cloud_protocol_sg_order_t *order = &cloud_protocol_sg_order_manager.orders[gun_no - 1];

    if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_CHARGING ||
        order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_DISCHARGING ||
        order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_PAUSED)
    {
        count++;
    }

    return count;
}

/**
 * @brief check if operations are allowed (network online)
 */
static bool Cloud_Protocol_Sg_Order_IsNetworkOnline(void)
{
    return cloud_protocol_sg_order_manager.network_online;
}

/**
 * @brief get order by gun number
 */
static cloud_protocol_sg_order_t *Cloud_Protocol_Sg_Order_Get(uint8_t gun_no)
{
    if (!Cloud_Protocol_Sg_Order_ValidateGunNo(gun_no))
    {
        return NULL;
    }

    return &cloud_protocol_sg_order_manager.orders[gun_no - 1];
}

/*
 * @brief set message ID for pile work status event
 */
static void Cloud_Protocol_EventPost_SetGun1PileWorkStatusMsgId(uint32_t msg_id)
{
    cloud_protocol_event_post_pile_work_status_ctrl[0].send_message_id = msg_id;
    CLOUD_INFO("<%s> Gun 1 Pile Work Status Msg ID set to: %d\r\n", __func__, cloud_protocol_event_post_pile_work_status_ctrl[0].send_message_id);
}

static void Cloud_Protocol_EventPost_SetGun2PileWorkStatusMsgId(uint32_t msg_id)
{
    cloud_protocol_event_post_pile_work_status_ctrl[1].send_message_id = msg_id;
    CLOUD_INFO("<%s> Gun 2 Pile Work Status Msg ID set to: %d\r\n", __func__, cloud_protocol_event_post_pile_work_status_ctrl[1].send_message_id);
}

/*
* @brief set order ID for pile work status event
*/
static void Cloud_Protocol_Sg_Order_SetPileWorkStatusOrderId(uint8_t gun_no, char *order_id)
{
    if (gun_no == 1)
    {
        strncpy(cloud_protocol_event_post_pile_work_status_ctrl[0].order_id, order_id, sizeof(cloud_protocol_event_post_pile_work_status_ctrl[0].order_id) - 1);
        cloud_protocol_event_post_pile_work_status_ctrl[0].order_id[sizeof(cloud_protocol_event_post_pile_work_status_ctrl[0].order_id) - 1] = '\0';
    }
    else if (gun_no == 2)
    {
        strncpy(cloud_protocol_event_post_pile_work_status_ctrl[1].order_id, order_id, sizeof(cloud_protocol_event_post_pile_work_status_ctrl[1].order_id) - 1);
        cloud_protocol_event_post_pile_work_status_ctrl[1].order_id[sizeof(cloud_protocol_event_post_pile_work_status_ctrl[1].order_id) - 1] = '\0';
    }
    else
    {
        CLOUD_WARN("<%s %d> invalid gunNo: %d\r\n", __func__, __LINE__, gun_no);
    }
}

/**
 * @brief post pile work status event to cloud
 */
static bool Cloud_Protocol_Sg_Order_PostPileWorkstatus(v2g_event_pile_workstatus *data)
{
    cJSON *root = NULL;
    unsigned char i = 0;
    uint64_t timestamp = 0;

    cloud_protocol_event_post_req_t *cloud_protocol_event_post_req = NULL;

    // create request
    if (data->gunNo == 1)
    {
        cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("pileWorkStatusEvt", Cloud_Protocol_EventPost_SetGun1PileWorkStatusMsgId);
    }
    else if (data->gunNo == 2)
    {
        cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("pileWorkStatusEvt", Cloud_Protocol_EventPost_SetGun2PileWorkStatusMsgId);
    }
    else
    {
        CLOUD_WARN("<%s %d> invalid gunNo: %d\r\n", __func__, __LINE__, data->gunNo);
        return false;
    }

    // build json header
    root = Cloud_Protocol_EventPost_BuildRequestJsonHeader(cloud_protocol_event_post_req);
    Cloud_Protocol_EventPost_DestroyRequest((cloud_protocol_event_post_req_t *)cloud_protocol_event_post_req);

    // get params->value object
    cJSON *params_obj = cJSON_GetObjectItem(root, "params");
    if (params_obj == NULL)
    {
        CLOUD_WARN("<%s %d> params object not found\r\n", __func__, __LINE__);
        return false;
    }

    cJSON *value_obj = cJSON_GetObjectItem(params_obj, "value");
    if (value_obj == NULL)
    {
        CLOUD_WARN("<%s %d> value object not found\r\n", __func__, __LINE__);
        return false;
    }

    cJSON_AddNumberToObject(value_obj, "gunNo", data->gunNo);
    cJSON_AddStringToObject(value_obj, "applySheetNo", data->applySheetNo);
    cJSON_AddNumberToObject(value_obj, "executionStatus", data->executionStatus);
    
    cJSON_AddNumberToObject(value_obj, "voltage", data->voltage);
    cJSON_AddNumberToObject(value_obj, "current", data->current);
    cJSON_AddNumberToObject(value_obj, "electricPower", data->electricPower);
    cJSON_AddNumberToObject(value_obj, "cumulativeTime", data->cumulativeTime);
    
    cJSON *array_chargingEnergyValue = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "chargingEnergyValue", array_chargingEnergyValue);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_chargingEnergyValue, cJSON_CreateNumber(data->chargingEnergyValue[i]));
    }
    
    cJSON *array_disChargingEnergyValue = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "disChargingEnergyValue", array_disChargingEnergyValue);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_disChargingEnergyValue, cJSON_CreateNumber(data->disChargingEnergyValue[i]));
    }

    cJSON *array_cumulativeCharge = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "cumulativeCharge", array_cumulativeCharge);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_cumulativeCharge, cJSON_CreateNumber(data->cumulativeCharge[i]));
    }

    cJSON *array_cumulativeDischarge = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "cumulativeDischarge", array_cumulativeDischarge);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_cumulativeDischarge, cJSON_CreateNumber(data->cumulativeDischarge[i]));
    }

    cJSON *array_acChargingEnergyValue = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acChargingEnergyValue", array_acChargingEnergyValue);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acChargingEnergyValue, cJSON_CreateNumber(data->acChargingEnergyValue[i]));
    }

    cJSON *array_acDisChargingEnergyValue = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acDisChargingEnergyValue", array_acDisChargingEnergyValue);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acDisChargingEnergyValue, cJSON_CreateNumber(data->acDisChargingEnergyValue[i]));
    }

    cJSON *array_acVoltage = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acVoltage", array_acVoltage);
    for (i = 0; i < V2G_MAX_VOL_CUR_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acVoltage, cJSON_CreateNumber(data->acVoltage[i]));
    }

    cJSON *array_acCurrent = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acCurrent", array_acCurrent);
    for (i = 0; i < V2G_MAX_VOL_CUR_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acCurrent, cJSON_CreateNumber(data->acCurrent[i]));
    }

    cJSON *array_acElectricPower = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acElectricPower", array_acElectricPower);
    for (i = 0; i < V2G_MAX_POWER_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acElectricPower, cJSON_CreateNumber(data->acElectricPower[i]));
    }

    cJSON *array_acCumulativeCharge = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acCumulativeCharge", array_acCumulativeCharge);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acCumulativeCharge, cJSON_CreateNumber(data->acCumulativeCharge[i]));
    }

    cJSON *array_acCumulativeDischarge = cJSON_CreateArray();
    cJSON_AddItemToObject(value_obj, "acCumulativeDischarge", array_acCumulativeDischarge);
    for (i = 0; i < V2G_MAX_ENERGY_DATA_LEN; i++)
    {
        cJSON_AddItemToArray(array_acCumulativeDischarge, cJSON_CreateNumber(data->acCumulativeDischarge[i]));
    }

    // print unformatted json string
    timestamp = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // convert to milliseconds
    bool ret = Cloud_Protocol_EventPost_PrintUnformatted(root, timestamp, "pileWorkStatusEvt");
    if (!ret)
    {
        CLOUD_ERROR("<%s> Failed to print unformatted JSON for pile work status event\r\n", __func__);
    }
    else
    {
        cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(data->gunNo);
        if (order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY)
        {
            Cloud_Protocol_Sg_Order_Reset(order);
            CLOUD_DEBUG("<%s> Order on gun %d completed, reset order data\r\n", __func__, data->gunNo);
        }
    }

    cJSON_Delete(root);
    root = NULL;
    return ret;
}

/**
 * @brief post pile work status for active orders
 */
bool Cloud_Protocol_EventPost_PileWorkstatus_Post(uint8_t gun_no)
{
    cloud_protocol_sg_order_t *order = Cloud_Protocol_Sg_Order_Get(gun_no);
    if (order == NULL || order->status == CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE)
    {
        return false;
    }
    /* set measurement values */
    Dummy_GetMeterInfo(&order->measure_value);

    uint32_t energy_delta = Cloud_Protocol_Sg_Order_GetDeltaEnergyValue(gun_no);
    Cloud_Protocol_Sg_Order_UpdateEnergy(gun_no, energy_delta);

    v2g_event_pile_workstatus workstatus;
    memset(&workstatus, 0, sizeof(v2g_event_pile_workstatus));

    cloud_protocol_sg_order_record_t record;
    memset(&record, 0, sizeof(cloud_protocol_sg_order_record_t));

    /* convert to record */
    Cloud_Protocol_Sg_Order_ConvertOrderToRecord(order, &record, false);

    bool result = Cloud_Protocol_Sg_Order_ExportToV2GStruct(&record, &workstatus);
    if (!result)
    {
        CLOUD_ERROR("<%s> Failed to export order to V2G structure for gun %d\r\n", __func__, gun_no);
        return false;
    }

    /* post event */
    result = Cloud_Protocol_Sg_Order_PostPileWorkstatus(&workstatus);
    return result;
}

/**
 * @brief handle pile work status response
 */
bool Cloud_Protocol_EventPost_PostPileWorkstatus_Response(uint32_t msg_id)
{
    int gun_index = -1;

    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        if (msg_id == cloud_protocol_event_post_pile_work_status_ctrl[i].send_message_id)
        {
            gun_index = i;
            break;
        }
    }

    if (gun_index == -1)
    {
        CLOUD_WARN("<%s> Message ID %u not found for any gun\r\n", __func__, msg_id);
        return false;
    }

    if (cloud_protocol_event_post_pile_work_status_ctrl[gun_index].order_id[0] != '\0')
    {
        Cloud_Protocol_Sg_Order_NotifyUploadComplete(gun_index + 1, cloud_protocol_event_post_pile_work_status_ctrl[gun_index].order_id, true);
        memset(cloud_protocol_event_post_pile_work_status_ctrl[gun_index].order_id, 0, sizeof(cloud_protocol_event_post_pile_work_status_ctrl[gun_index].order_id));
    }

    return true;
}

/*
    * @brief get current running status of orders
    1 - on running
*/
void Cloud_Protocol_Sg_Order_GetOrderOnRunningStatus(uint8_t* gun1, uint8_t* gun2)
{
  if (gun1 == NULL || gun2 == NULL)
  {
      return;
  }
    *gun1 = cloud_protocol_event_post_pile_work_status_ctrl[0].on_running ? 1 : 0;
    *gun2 = cloud_protocol_event_post_pile_work_status_ctrl[1].on_running ? 1 : 0;
}

/**
 * @brief handle online running orders (enable period event upload)
 */
static void Cloud_Protocol_Sg_Order_HandleOnlineRunningOrders(void)
{
    uint32_t current_time = Cloud_Protocol_Sg_Order_GetCurrentTime();

    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        cloud_protocol_sg_order_t *order = &cloud_protocol_sg_order_manager.orders[i];

        if (cloud_protocol_event_post_pile_work_status_ctrl[i].last_post_time + CLOUD_PROTOCOL_SG_ORDER_UPDATE_PERIOD > current_time)
        {
            continue;
        }

        if (order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_IDLE &&
            order->status != CLOUD_PROTOCOL_SG_ORDER_STATUS_STANDBY)
        {
            cloud_protocol_event_post_pile_work_status_ctrl[i].last_post_time = current_time;
            cloud_protocol_event_post_pile_work_status_ctrl[i].on_running = true;
            Cloud_Protocol_EventPost_ForceTriggerEvent(CLOUD_PROTOCOL_EVENT_POST_TYPE_PILE_WORKSTATUS);
            CLOUD_INFO("<%s> Gun %d order %s is running, triggering pile work status upload\r\n", __func__, order->gun_no, order->order_id);
        }
        else
        {
            cloud_protocol_event_post_pile_work_status_ctrl[i].on_running = false;
        }
    }
}

/*
 * @brief handle pending upload orders
 */
static void Cloud_Protocol_Sg_Order_HandlePendingUploadOrders(void)
{
    if (cloud_protocol_sg_order_manager.is_uploading_pending)
    {
        return;
    }

    v2g_event_pile_workstatus pending_record;

    for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
    {
        memset(&pending_record, 0, sizeof(v2g_event_pile_workstatus));
        if (Cloud_Protocol_Sg_Order_GetNextPendingRecord(i + 1, &pending_record))
        {
            cloud_protocol_sg_order_manager.is_uploading_pending = true;
            Cloud_Protocol_Sg_Order_SetPileWorkStatusOrderId(i + 1, pending_record.applySheetNo);
            Cloud_Protocol_Sg_Order_PostPileWorkstatus(&pending_record);
        }
    }
}

/**
 * @brief order timer task
 */
void Cloud_Protocol_Sg_Order_TimerTask(void)
{
    uint32_t current_time = Cloud_Protocol_Sg_Order_GetCurrentTime();

    /* check offline timeout (check every second) */
    if (!Cloud_Protocol_Sg_Order_IsNetworkOnline())
    {
        for (int i = 0; i < CLOUD_PROTOCOL_SG_MAX_GUN_NUM; i++)
        {
            Cloud_Protocol_Sg_Order_HandleOfflineTimeout(i + 1);
        }
        return;
    }
    Cloud_Protocol_Sg_Order_HandleOnlineRunningOrders();
    Cloud_Protocol_Sg_Order_HandlePendingUploadOrders();
}

/**
 * @brief handle power failure (save all active orders)
 */
void Cloud_Protocol_Sg_Order_HandlePowerFailure(void)
{
    /* emergency stop all active orders */
    Cloud_Protocol_Sg_Order_StopAllActiveOrders(true);

    CLOUD_WARN("<%s> Power failure handling completed\r\n", __func__);
}
/* EOL */
