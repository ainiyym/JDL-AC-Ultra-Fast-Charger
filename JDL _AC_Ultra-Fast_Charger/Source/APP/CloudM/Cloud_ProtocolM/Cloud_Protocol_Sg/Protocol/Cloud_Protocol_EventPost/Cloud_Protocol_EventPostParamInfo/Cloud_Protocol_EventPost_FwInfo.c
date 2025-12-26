
//******************************************************************************
//* File Name: Cloud_Protocol_EventPost_FwInfo.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module post fw info source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_EventPost_FwInfo.h"
#include "FlashDB_AppM.h"
#include "dummy.h"

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
} cloud_protocol_event_post_fireware_info_ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
v2g_event_fireware_info cloud_protocol_event_fireware_info;
cloud_protocol_event_post_fireware_info_ctrl_t cloud_protocol_event_post_fireware_info_ctrl;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Cloud_Protocol_EventPost_SetFwInfoMsgId(uint32_t msg_id);
static int Cloud_Protocol_EventPost_GenerateFactoryCode(uint16_t serial_num, char *output);
static bool Cloud_Protocol_EventPost_FwInfo_Validate(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_EventPost_FwInfo_Init(void)
{
    char factory_code[17] = {0};
    uint16_t serial_num = 0;
    char model_no[V2G_MAX_MODEL_ID_LEN] = {0};

    // all reset
    memset(&cloud_protocol_event_fireware_info, 0, sizeof(v2g_event_fireware_info));
    memset(&cloud_protocol_event_post_fireware_info_ctrl, 0, sizeof(cloud_protocol_event_post_fireware_info_ctrl_t));

    // set fwinfo
    strncpy(cloud_protocol_event_fireware_info.stakeModel, CLOUDM_SG_STAKE_MODEL, V2G_MAX_ICCID_LEN - 1);
    FlashDB_ReadValue(FLASHDB_KV_SG_BILLING_MODE_NO, model_no, V2G_MAX_MODEL_ID_LEN, NULL);
    snprintf(cloud_protocol_event_fireware_info.modelNo, V2G_MAX_MODEL_ID_LEN, "%s", model_no);
    cloud_protocol_event_fireware_info.vendorCode = CLOUDM_SG_MANUFACTURER_CODE;
    FlashDB_ReadValue(FLASHDB_KV_SG_SN, (uint16_t *)&serial_num, sizeof(serial_num), NULL);
    Cloud_Protocol_EventPost_GenerateFactoryCode(serial_num, factory_code);
    strncpy(cloud_protocol_event_fireware_info.deSn, factory_code, V2G_MAX_DEV_SN_LEN - 1);
    cloud_protocol_event_fireware_info.deType = (unsigned char)CLOUDM_SG_PILE_TYPE;
    cloud_protocol_event_fireware_info.connetNum = (unsigned char)CLOUDM_SG_CONNECTOR_NUM;
    snprintf(cloud_protocol_event_fireware_info.simMac, V2G_MAX_MAC_ADDR_LEN-1, "%s", "");
    cloud_protocol_event_fireware_info.longitude = 0;
    cloud_protocol_event_fireware_info.latitude = 0;
    cloud_protocol_event_fireware_info.height = 0;
    cloud_protocol_event_fireware_info.gridType = CLOUDM_SG_GRIDE_TYPE;
    snprintf(cloud_protocol_event_fireware_info.btMac, V2G_MAX_MAC_ADDR_LEN-1, "%s", "");
    cloud_protocol_event_fireware_info.meaType = (unsigned char)CLOUDM_SG_METERING_METHOD;
    cloud_protocol_event_fireware_info.otRate = CLOUDM_SG_RATED_POWER;
    cloud_protocol_event_fireware_info.otMinVol = CLOUDM_SG_OT_MIN_VOL;
    cloud_protocol_event_fireware_info.otMaxVol = CLOUDM_SG_OT_MAX_VOL;
    cloud_protocol_event_fireware_info.otCur = CLOUDM_SG_OT_MAX_CUR;

    strcpy(cloud_protocol_event_fireware_info.inMeter[SYS_CONNECTOR1], Dummy_GetInMeterAddr(SYS_CONNECTOR1));
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    strcpy(cloud_protocol_event_fireware_info.inMeter[SYS_CONNECTOR2], Dummy_GetInMeterAddr(SYS_CONNECTOR2));
    strcpy(cloud_protocol_event_fireware_info.outMeter[SYS_CONNECTOR2], Dummy_GetOutMeterAddr(SYS_CONNECTOR2));
#endif
    strcpy(cloud_protocol_event_fireware_info.outMeter[SYS_CONNECTOR1], Dummy_GetOutMeterAddr(SYS_CONNECTOR1));
    cloud_protocol_event_fireware_info.CT = 1; // The default value of the current transformer coefficient is 1
    cloud_protocol_event_fireware_info.isGateLock = 10;
    cloud_protocol_event_fireware_info.isGroundLock = 10;
    cloud_protocol_event_fireware_info.minChargingCurrent = CLOUDM_SG_OT_MIN_CUR;
    cloud_protocol_event_fireware_info.minChargingPower = CLOUDM_SG_OT_MIN_POWER;
    cloud_protocol_event_fireware_info.maxDischargeVoltage = CLOUDM_SG_OT_MAX_VOL;
    cloud_protocol_event_fireware_info.minDischargeVoltage = CLOUDM_SG_OT_MIN_VOL;
    cloud_protocol_event_fireware_info.maxDischargeCurrent = CLOUDM_SG_OT_MAX_CUR;
    cloud_protocol_event_fireware_info.minDischargeCurrent = CLOUDM_SG_OT_MIN_CUR;
}

static void Cloud_Protocol_EventPost_SetFwInfoMsgId(uint32_t msg_id)
{
    cloud_protocol_event_post_fireware_info_ctrl.send_message_id = msg_id;
}

// Generate factory code
static int Cloud_Protocol_EventPost_GenerateFactoryCode(uint16_t serial_num, char *output)
{
    if (output == NULL)
    {
        return -1;
    }

    if (serial_num > 0xffff)
    {
        CLOUD_ERROR("sn out of range (0-0xffff)\r\n");
        return -1;
    }

    char date_str[7] = {0};

    // 1. production date: YYMMDD
    strcpy(date_str, CLOUDM_SG_PRODUCTION_DATE);

    // 2. manufacturer code (4 digits)
    char manufacturer_str[5] = {0};
    snprintf(manufacturer_str, 5, "%04d", CLOUDM_SG_MANUFACTURER_CODE);

    // 3. device type (2 digits)
    char device_type_str[3] = {0};
    strncpy(device_type_str, CLOUDM_SG_DEVICE_TYPE, 2);

    // 4. serial number (4 digits)
    char serial_str[5] = {0};
    snprintf(serial_str, sizeof(serial_str), "%04d", serial_num);

    // 5. Combine all parts
    snprintf(output, 17, "%s%s%s%s", manufacturer_str, date_str, device_type_str, serial_str);

    return 0;
}

// Set the firmware information field
void Cloud_Protocol_EventPost_FwInfo_Set(cloud_protocol_sg_event_fireware_enum type, void *value)
{
    if (value == NULL)
    {
        return;
    }
    switch (type)
    {
        // String type fields
        case CLOUD_PROTOCOL_SG_EVENT_FW_SIM_NO:
            strncpy(cloud_protocol_event_fireware_info.simNo,
                    (const char *)value,
                    V2G_MAX_ICCID_LEN - 1);
            cloud_protocol_event_fireware_info.simNo[V2G_MAX_ICCID_LEN - 1] = '\0';
            CLOUD_INFO("Set SIM No: %s\r\n", cloud_protocol_event_fireware_info.simNo);
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MODEL_NO:
            strncpy(cloud_protocol_event_fireware_info.modelNo,
                    (const char *)value,
                    V2G_MAX_MODEL_ID_LEN - 1);
            cloud_protocol_event_fireware_info.modelNo[V2G_MAX_MODEL_ID_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_STAKE_MODEL:
            strncpy(cloud_protocol_event_fireware_info.stakeModel,
                    (const char *)value,
                    V2G_MAX_PILE_TYPE_LEN - 1);
            cloud_protocol_event_fireware_info.stakeModel[V2G_MAX_PILE_TYPE_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_DE_SN:
            strncpy(cloud_protocol_event_fireware_info.deSn,
                    (const char *)value,
                    V2G_MAX_DEV_SN_LEN - 1);
            cloud_protocol_event_fireware_info.deSn[V2G_MAX_DEV_SN_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_SIM_MAC:
            strncpy(cloud_protocol_event_fireware_info.simMac,
                    (const char *)value,
                    V2G_MAX_MAC_ADDR_LEN - 1);
            cloud_protocol_event_fireware_info.simMac[V2G_MAX_MAC_ADDR_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_BT_MAC:
            strncpy(cloud_protocol_event_fireware_info.btMac,
                    (const char *)value,
                    V2G_MAX_MAC_ADDR_LEN - 1);
            cloud_protocol_event_fireware_info.btMac[V2G_MAX_MAC_ADDR_LEN - 1] = '\0';
            break;

        // Unsigned integer type fields
        case CLOUD_PROTOCOL_SG_EVENT_FW_VENDOR_CODE:
            cloud_protocol_event_fireware_info.vendorCode = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_LONGITUDE:
            cloud_protocol_event_fireware_info.longitude = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_LATITUDE:
            cloud_protocol_event_fireware_info.latitude = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_HEIGHT:
            cloud_protocol_event_fireware_info.height = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_GRID_TYPE:
            cloud_protocol_event_fireware_info.gridType = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_OT_RATE:
            cloud_protocol_event_fireware_info.otRate = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_OT_MIN_VOL:
            cloud_protocol_event_fireware_info.otMinVol = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_OT_MAX_VOL:
            cloud_protocol_event_fireware_info.otMaxVol = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_OT_CUR:
            cloud_protocol_event_fireware_info.otCur = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_CT:
            cloud_protocol_event_fireware_info.CT = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MIN_CHARGING_CURRENT:
            cloud_protocol_event_fireware_info.minChargingCurrent = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MIN_CHARGING_POWER:
            cloud_protocol_event_fireware_info.minChargingPower = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MAX_DISCHARGE_VOLTAGE:
            cloud_protocol_event_fireware_info.maxDischargeVoltage = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MIN_DISCHARGE_VOLTAGE:
            cloud_protocol_event_fireware_info.minDischargeVoltage = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MAX_DISCHARGE_CURRENT:
            cloud_protocol_event_fireware_info.maxDischargeCurrent = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MIN_DISCHARGE_CURRENT:
            cloud_protocol_event_fireware_info.minDischargeCurrent = *(unsigned int *)value;
            break;

        // Unsigned char type fields
        case CLOUD_PROTOCOL_SG_EVENT_FW_DE_TYPE:
            cloud_protocol_event_fireware_info.deType = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_CONNET_NUM:
            cloud_protocol_event_fireware_info.connetNum = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_MEA_TYPE:
            cloud_protocol_event_fireware_info.meaType = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_IS_GATE_LOCK:
            cloud_protocol_event_fireware_info.isGateLock = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_SG_EVENT_FW_IS_GROUND_LOCK:
            cloud_protocol_event_fireware_info.isGroundLock = *(unsigned char *)value;
            break;

        // Array type fields (meter addresses)
        case CLOUD_PROTOCOL_SG_EVENT_FW_IN_METER_ARRAY:
        {
            // value should point to a char[V2G_MAX_INPUT_METER_NUM][V2G_MAX_METER_ADDR_LEN] array
            memcpy(cloud_protocol_event_fireware_info.inMeter, value, V2G_MAX_INPUT_METER_NUM * V2G_MAX_METER_ADDR_LEN);
            break;
        }

        case CLOUD_PROTOCOL_SG_EVENT_FW_OUT_METER_ARRAY:
        {
            // value should point to a char[V2G_MAX_PORT_NUM][V2G_MAX_METER_ADDR_LEN] array
            memcpy(cloud_protocol_event_fireware_info.outMeter, value, V2G_MAX_PORT_NUM * V2G_MAX_METER_ADDR_LEN);
            break;
        }

        default:
            // Unknown enum value, do not process
            break;
    }
}

// Validate if the firmware information is valid
static bool Cloud_Protocol_EventPost_FwInfo_Validate(void)
{
    // check required fields
    if (strlen(cloud_protocol_event_fireware_info.simNo) == 0)
    {
        return false;
    }

    if (strlen(cloud_protocol_event_fireware_info.deSn) == 0)
    {
        return false;
    }

    if (cloud_protocol_event_fireware_info.vendorCode == 0)
    {
        return false;
    }

    // check enum ranges
    if (cloud_protocol_event_fireware_info.deType > 99)
    {
        return false;
    }

    if (cloud_protocol_event_fireware_info.meaType < 10 || cloud_protocol_event_fireware_info.meaType > 13)
    {
        return false;
    }

    if (cloud_protocol_event_fireware_info.gridType < 10 || cloud_protocol_event_fireware_info.gridType > 14)
    {
        return false;
    }

    if (cloud_protocol_event_fireware_info.connetNum == 0)
    {
        return false;
    }

    return true;
}

bool Cloud_Protocol_EventPost_FwInfo_Post(void)
{
    cloud_protocol_event_post_req_t *cloud_protocol_event_post_req = NULL;
    uint64_t timestamp = 0;

    // create string arrays for inMeter and outMeter
    cJSON *root = NULL;
    cJSON *inMeterArray = NULL;
    cJSON *outMeterArray = NULL;
    unsigned char i = 0;

    // count valid meter addresses
    uint8_t in_meter_count = 0;
    uint8_t out_meter_count = 0;

    for (i = 0; i < V2G_MAX_INPUT_METER_NUM; i++)
    {
        if (strlen(cloud_protocol_event_fireware_info.inMeter[i]) > 0)
        {
            in_meter_count++;
        }
    }

    for (i = 0; i < V2G_MAX_PORT_NUM; i++)
    {
        if (strlen(cloud_protocol_event_fireware_info.outMeter[i]) > 0)
        {
            out_meter_count++;
        }
    }

    // create request
    cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("firmwareEvt", Cloud_Protocol_EventPost_SetFwInfoMsgId);

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
    // add fw info fields to value object
    cJSON_AddStringToObject(value_obj, "simNo", cloud_protocol_event_fireware_info.simNo);
    cJSON_AddStringToObject(value_obj, "modelNo", cloud_protocol_event_fireware_info.modelNo);
    cJSON_AddStringToObject(value_obj, "stakeModel", cloud_protocol_event_fireware_info.stakeModel);
    cJSON_AddStringToObject(value_obj, "deSn", cloud_protocol_event_fireware_info.deSn);

    cJSON_AddStringToObject(value_obj, "simMac", cloud_protocol_event_fireware_info.simMac);
    cJSON_AddStringToObject(value_obj, "btMac", cloud_protocol_event_fireware_info.btMac);

    cJSON_AddNumberToObject(value_obj, "vendorCode", cloud_protocol_event_fireware_info.vendorCode);
    cJSON_AddNumberToObject(value_obj, "deType", cloud_protocol_event_fireware_info.deType);
    cJSON_AddNumberToObject(value_obj, "connetNum", cloud_protocol_event_fireware_info.connetNum);

    cJSON_AddNumberToObject(value_obj, "longitude", cloud_protocol_event_fireware_info.longitude);
    cJSON_AddNumberToObject(value_obj, "latitude", cloud_protocol_event_fireware_info.latitude);
    cJSON_AddNumberToObject(value_obj, "height", cloud_protocol_event_fireware_info.height);

    cJSON_AddNumberToObject(value_obj, "gridType", cloud_protocol_event_fireware_info.gridType);
    cJSON_AddNumberToObject(value_obj, "meaType", cloud_protocol_event_fireware_info.meaType);
    cJSON_AddNumberToObject(value_obj, "otRate", cloud_protocol_event_fireware_info.otRate);

    cJSON_AddNumberToObject(value_obj, "otMaxVol", cloud_protocol_event_fireware_info.otMaxVol);
    cJSON_AddNumberToObject(value_obj, "otMinVol", cloud_protocol_event_fireware_info.otMinVol);
    cJSON_AddNumberToObject(value_obj, "otCur", cloud_protocol_event_fireware_info.otCur);

    cJSON_AddNumberToObject(value_obj, "CT", cloud_protocol_event_fireware_info.CT);
    cJSON_AddNumberToObject(value_obj, "isGateLock", cloud_protocol_event_fireware_info.isGateLock);
    cJSON_AddNumberToObject(value_obj, "isGroundLock", cloud_protocol_event_fireware_info.isGroundLock);

    cJSON_AddNumberToObject(value_obj, "minChargingCurrent", cloud_protocol_event_fireware_info.minChargingCurrent);
    cJSON_AddNumberToObject(value_obj, "minChargingPower", cloud_protocol_event_fireware_info.minChargingPower);
    cJSON_AddNumberToObject(value_obj, "maxDischargeVoltage", cloud_protocol_event_fireware_info.maxDischargeVoltage);

    cJSON_AddNumberToObject(value_obj, "minDischargeVoltage", cloud_protocol_event_fireware_info.minDischargeVoltage);
    cJSON_AddNumberToObject(value_obj, "maxDischargeCurrent", cloud_protocol_event_fireware_info.maxDischargeCurrent);
    cJSON_AddNumberToObject(value_obj, "minDischargeCurrent", cloud_protocol_event_fireware_info.minDischargeCurrent);

    cJSON_AddItemToObject(value_obj, "inMeter", inMeterArray = cJSON_CreateArray());
    for (i = 0; i < in_meter_count; i++)
    {
        cJSON *item = cJSON_CreateString(cloud_protocol_event_fireware_info.inMeter[i]);
        if (item != NULL)
        {
            cJSON_AddItemToArray(inMeterArray, item);
        }
    }

    cJSON_AddItemToObject(value_obj, "outMeter", outMeterArray = cJSON_CreateArray());
    for (i = 0; i < out_meter_count; i++)
    {
        cJSON *item = cJSON_CreateString(cloud_protocol_event_fireware_info.outMeter[i]);
        if (item != NULL)
        {
            cJSON_AddItemToArray(outMeterArray, item);
        }
    }  

    // print unformatted json string
    timestamp = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // convert to milliseconds

    bool ret = false;
    if (Cloud_Protocol_EventPost_FwInfo_Validate())
    {
        ret = Cloud_Protocol_EventPost_PrintUnformatted(root, timestamp, "firmwareEvt");
        if (ret == false)
        {
            CLOUD_WARN("<%s %d> Post fw info event failed\r\n", __func__, __LINE__);
        }
    }

    cJSON_Delete(root);
    root = NULL;

    return ret;
}

bool Cloud_Protocol_EventPost_FwInfo_HandleResponse(uint32_t msg_id)
{
    if (msg_id != cloud_protocol_event_post_fireware_info_ctrl.send_message_id)
    {
        CLOUD_WARN("<%s>FW info response msg_id mismatch: received=%u, expected=%u\r\n", __func__, msg_id, cloud_protocol_event_post_fireware_info_ctrl.send_message_id);
        return false;
    }

    return true;
}
/* EOL */
