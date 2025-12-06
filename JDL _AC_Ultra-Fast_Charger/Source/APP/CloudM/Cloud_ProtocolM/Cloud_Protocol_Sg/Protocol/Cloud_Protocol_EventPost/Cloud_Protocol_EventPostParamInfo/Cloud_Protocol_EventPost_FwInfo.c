
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
    bool net_is_connected;	// network whether is connected
    bool fwinfo_is_refresh; // Indicates whether the parameter has been updated
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
static void Cloud_Protocol_EventPost_FwInfo_Post(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_EventPost_FwInfo_Init(void)
{
    char factory_code[17] = {0};
    uint16_t serial_num = 0;

    // all reset
    memset(&cloud_protocol_event_fireware_info, 0, sizeof(v2g_event_fireware_info));
    memset(&cloud_protocol_event_post_fireware_info_ctrl, 0, sizeof(cloud_protocol_event_post_fireware_info_ctrl_t));

    // set fwinfo
    strncpy(cloud_protocol_event_fireware_info.stakeModel, CLOUDM_SG_STAKE_MODEL, V2G_MAX_ICCID_LEN - 1);
    snprintf(cloud_protocol_event_fireware_info.modelNo, V2G_MAX_MODEL_ID_LEN, "%s", "");
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

void Cloud_Protocol_EventPost_SetFwInfoRefreshFlag(bool is_refresh)
{
    cloud_protocol_event_post_fireware_info_ctrl.fwinfo_is_refresh = is_refresh;
}

void Cloud_Protocol_EventPost_SetFwInfoNetConnectedFlag(bool is_connected)
{
    cloud_protocol_event_post_fireware_info_ctrl.net_is_connected = is_connected;
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
void Cloud_Protocol_EventPost_FwInfo_Set(cloud_protocol_event_fireware_enum type, void *value)
{
    if (value == NULL)
    {
        return;
    }

    switch (type)
    {
        // String type fields
        case CLOUD_PROTOCOL_EVENT_FW_SIM_NO:
            strncpy(cloud_protocol_event_fireware_info.simNo,
                    (const char *)value,
                    V2G_MAX_ICCID_LEN - 1);
            cloud_protocol_event_fireware_info.simNo[V2G_MAX_ICCID_LEN - 1] = '\0';
            CLOUD_INFO("Set SIM No: %s\r\n", cloud_protocol_event_fireware_info.simNo);
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MODEL_NO:
            strncpy(cloud_protocol_event_fireware_info.modelNo,
                    (const char *)value,
                    V2G_MAX_MODEL_ID_LEN - 1);
            cloud_protocol_event_fireware_info.modelNo[V2G_MAX_MODEL_ID_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_EVENT_FW_STAKE_MODEL:
            strncpy(cloud_protocol_event_fireware_info.stakeModel,
                    (const char *)value,
                    V2G_MAX_PILE_TYPE_LEN - 1);
            cloud_protocol_event_fireware_info.stakeModel[V2G_MAX_PILE_TYPE_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_EVENT_FW_DE_SN:
            strncpy(cloud_protocol_event_fireware_info.deSn,
                    (const char *)value,
                    V2G_MAX_DEV_SN_LEN - 1);
            cloud_protocol_event_fireware_info.deSn[V2G_MAX_DEV_SN_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_EVENT_FW_SIM_MAC:
            strncpy(cloud_protocol_event_fireware_info.simMac,
                    (const char *)value,
                    V2G_MAX_MAC_ADDR_LEN - 1);
            cloud_protocol_event_fireware_info.simMac[V2G_MAX_MAC_ADDR_LEN - 1] = '\0';
            break;

        case CLOUD_PROTOCOL_EVENT_FW_BT_MAC:
            strncpy(cloud_protocol_event_fireware_info.btMac,
                    (const char *)value,
                    V2G_MAX_MAC_ADDR_LEN - 1);
            cloud_protocol_event_fireware_info.btMac[V2G_MAX_MAC_ADDR_LEN - 1] = '\0';
            break;

        // Unsigned integer type fields
        case CLOUD_PROTOCOL_EVENT_FW_VENDOR_CODE:
            cloud_protocol_event_fireware_info.vendorCode = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_LONGITUDE:
            cloud_protocol_event_fireware_info.longitude = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_LATITUDE:
            cloud_protocol_event_fireware_info.latitude = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_HEIGHT:
            cloud_protocol_event_fireware_info.height = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_GRID_TYPE:
            cloud_protocol_event_fireware_info.gridType = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_OT_RATE:
            cloud_protocol_event_fireware_info.otRate = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_OT_MIN_VOL:
            cloud_protocol_event_fireware_info.otMinVol = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_OT_MAX_VOL:
            cloud_protocol_event_fireware_info.otMaxVol = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_OT_CUR:
            cloud_protocol_event_fireware_info.otCur = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_CT:
            cloud_protocol_event_fireware_info.CT = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MIN_CHARGING_CURRENT:
            cloud_protocol_event_fireware_info.minChargingCurrent = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MIN_CHARGING_POWER:
            cloud_protocol_event_fireware_info.minChargingPower = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MAX_DISCHARGE_VOLTAGE:
            cloud_protocol_event_fireware_info.maxDischargeVoltage = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MIN_DISCHARGE_VOLTAGE:
            cloud_protocol_event_fireware_info.minDischargeVoltage = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MAX_DISCHARGE_CURRENT:
            cloud_protocol_event_fireware_info.maxDischargeCurrent = *(unsigned int *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MIN_DISCHARGE_CURRENT:
            cloud_protocol_event_fireware_info.minDischargeCurrent = *(unsigned int *)value;
            break;

        // Unsigned char type fields
        case CLOUD_PROTOCOL_EVENT_FW_DE_TYPE:
            cloud_protocol_event_fireware_info.deType = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_CONNET_NUM:
            cloud_protocol_event_fireware_info.connetNum = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_MEA_TYPE:
            cloud_protocol_event_fireware_info.meaType = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_IS_GATE_LOCK:
            cloud_protocol_event_fireware_info.isGateLock = *(unsigned char *)value;
            break;

        case CLOUD_PROTOCOL_EVENT_FW_IS_GROUND_LOCK:
            cloud_protocol_event_fireware_info.isGroundLock = *(unsigned char *)value;
            break;

        // Array type fields (meter addresses)
        case CLOUD_PROTOCOL_EVENT_FW_IN_METER_ARRAY:
        {
            // value should point to a char[V2G_MAX_INPUT_METER_NUM][V2G_MAX_METER_ADDR_LEN] array
            memcpy(cloud_protocol_event_fireware_info.inMeter, value, V2G_MAX_INPUT_METER_NUM * V2G_MAX_METER_ADDR_LEN);
            break;
        }

        case CLOUD_PROTOCOL_EVENT_FW_OUT_METER_ARRAY:
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

static void Cloud_Protocol_EventPost_FwInfo_Post(void)
{
    cloud_protocol_event_post_req_t *cloud_protocol_event_post_req = NULL;
    cJSON *root = NULL;
    cloud_protocol_event_post_param_t param;
    uint64_t timestamp = 0;

    // create string arrays for inMeter and outMeter
    cloud_protocol_string_array_t in_meter_array = {0};
    cloud_protocol_string_array_t out_meter_array = {0};

    // initialize meter arrays
    char *in_meter_strings[V2G_MAX_INPUT_METER_NUM] = {0};
    char *out_meter_strings[V2G_MAX_PORT_NUM] = {0};

    // count valid meter addresses
    uint8_t in_meter_count = 0;
    uint8_t out_meter_count = 0;

    for (int i = 0; i < V2G_MAX_INPUT_METER_NUM; i++)
    {
        if (strlen(cloud_protocol_event_fireware_info.inMeter[i]) > 0)
        {
            in_meter_strings[i] = cloud_protocol_event_fireware_info.inMeter[i];
            in_meter_count++;
        }
    }

    for (int i = 0; i < V2G_MAX_PORT_NUM; i++)
    {
        if (strlen(cloud_protocol_event_fireware_info.outMeter[i]) > 0)
        {
            out_meter_strings[i] = cloud_protocol_event_fireware_info.outMeter[i];
            out_meter_count++;
        }
    }

    in_meter_array.strings = in_meter_strings;
    in_meter_array.count = in_meter_count;
    out_meter_array.strings = out_meter_strings;
    out_meter_array.count = out_meter_count;

    // create request
    cloud_protocol_event_post_req = Cloud_Protocol_EventPost_CreateRequest("firmwareEvt", Cloud_Protocol_EventPost_SetFwInfoMsgId);

    // build json header
    root = Cloud_Protocol_EventPost_BuildRequestJsonHeader(cloud_protocol_event_post_req);

    // add CLOUD_PROTOCOL_EVENT_FW_SIM_NO
    param.param_name = "simNo";
    param.param_value.string_value = cloud_protocol_event_fireware_info.simNo; // copy simNo string
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MODEL_NO
    param.param_name = "modelNo";
    param.param_value.string_value = cloud_protocol_event_fireware_info.modelNo; // copy modelNo string
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_STAKE_MODEL
    param.param_name = "stakeModel";
    param.param_value.string_value = cloud_protocol_event_fireware_info.stakeModel;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_VENDOR_CODE
    param.param_name = "vendorCode";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.vendorCode;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_DE_SN
    param.param_name = "deSn";
    param.param_value.string_value = cloud_protocol_event_fireware_info.deSn;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_DE_TYPE
    param.param_name = "deType";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.deType; // 转换为int32
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_CONNET_NUM
    param.param_name = "connetNum";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.connetNum; // 转换为int32
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_SIM_MAC
    param.param_name = "simMac";
    param.param_value.string_value = cloud_protocol_event_fireware_info.simMac;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_LONGITUDE
    param.param_name = "longitude";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.longitude;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_LATITUDE
    param.param_name = "latitude";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.latitude;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_HEIGHT
    param.param_name = "height";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.height;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_GRID_TYPE
    param.param_name = "gridType";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.gridType;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_BT_MAC
    param.param_name = "btMac";
    param.param_value.string_value = cloud_protocol_event_fireware_info.btMac;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MEA_TYPE
    param.param_name = "meaType";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.meaType;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_OT_RATE
    param.param_name = "otRate";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.otRate;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_OT_MIN_VOL
    param.param_name = "otMinVol";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.otMinVol;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_OT_MAX_VOL
    param.param_name = "otMaxVol";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.otMaxVol;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_OT_CUR
    param.param_name = "otCur";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.otCur;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

#if 0
    // add CLOUD_PROTOCOL_EVENT_FW_IN_METER_ARRAY
    if (in_meter_array.count > 0)
    {
        param.param_name = "inMeter";
        param.param_value.string_array_value = &in_meter_array;
        param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING_ARRAY;
        Cloud_Protocol_EventPost_AddParam(root, &param);
    }
    else
    {
        param.param_name = "inMeter";
        param.param_value.string_value = "[]"; // empty array
        param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
        Cloud_Protocol_EventPost_AddParam(root, &param);
    }

    // add CLOUD_PROTOCOL_EVENT_FW_OUT_METER_ARRAY
    if (out_meter_array.count > 0)
    {
        param.param_name = "outMeter";
        param.param_value.string_array_value = &out_meter_array;
        param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING_ARRAY;
        Cloud_Protocol_EventPost_AddParam(root, &param);
    }
    else
    {
        param.param_name = "outMeter";
        param.param_value.string_value = "[]"; // empty array
        param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_STRING;
        Cloud_Protocol_EventPost_AddParam(root, &param);
    }
#endif
    // add CLOUD_PROTOCOL_EVENT_FW_CT
    param.param_name = "CT";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.CT;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_IS_GATE_LOCK
    param.param_name = "isGateLock";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.isGateLock;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_IS_GROUND_LOCK
    param.param_name = "isGroundLock";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.isGroundLock;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MIN_CHARGING_CURRENT
    param.param_name = "minChargingCurrent";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.minChargingCurrent;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MIN_CHARGING_POWER
    param.param_name = "minChargingPower";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.minChargingPower;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MAX_DISCHARGE_VOLTAGE
    param.param_name = "maxDischargeVoltage";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.maxDischargeVoltage;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MIN_DISCHARGE_VOLTAGE
    param.param_name = "minDischargeVoltage";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.minDischargeVoltage;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MAX_DISCHARGE_CURRENT
    param.param_name = "maxDischargeCurrent";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.maxDischargeCurrent;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // add CLOUD_PROTOCOL_EVENT_FW_MIN_DISCHARGE_CURRENT
    param.param_name = "minDischargeCurrent";
    param.param_value.int32_value = (int32_t)cloud_protocol_event_fireware_info.minDischargeCurrent;
    param.param_type = CLOUD_PROTOCOL_VALUE_TYPE_INT32;
    Cloud_Protocol_EventPost_AddParam(root, &param);

    // print unformatted json string
    timestamp = (uint64_t)CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() * 1000; // convert to milliseconds
    Cloud_Protocol_EventPost_PrintUnformatted(root, timestamp, "firmwareEvt");
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

void Cloud_Protocol_EventPost_FwInfoMainCtrl_Func(void)
{
    if (!cloud_protocol_event_post_fireware_info_ctrl.net_is_connected)
    {
        return;
    }
    // Check if firmware info needs to be reported
    if (cloud_protocol_event_post_fireware_info_ctrl.fwinfo_is_refresh)
    {
        // Validate firmware info
        if (Cloud_Protocol_EventPost_FwInfo_Validate())
        {
            // Post firmware info event
            Cloud_Protocol_EventPost_FwInfo_Post();
            // Clear the refresh flag
            cloud_protocol_event_post_fireware_info_ctrl.fwinfo_is_refresh = false;
        }
    }
}
/* EOL */
