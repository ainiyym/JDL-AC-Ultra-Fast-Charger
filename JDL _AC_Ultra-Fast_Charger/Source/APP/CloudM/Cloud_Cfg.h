//******************************************************************************
//* File Name: Cloud_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud module header file
/*******************************************************************************/
#if !defined (__CLOUD_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "STD_LogService.h"
#include "MessageBuffer.h"
#include "STD_Rtc.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUDM_TASK_PERIOD								                    (20U)
#define CLOUD_MESSAGE_BUFFER_MAX_LENGTH                                     (300U)
#define CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH                          (256U)
#define CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH                                 (MESSAGE_BUFFER_TYPE_DATA)
#define CLOUD_MESSAGE_TYPE_CTRL                                             (MESSAGE_BUFFER_TYPE_CTRL)
#define CLOUD_MESSAGE_TYPE_NOTIFY                                           (MESSAGE_BUFFER_TYPE_NOTIFY)
/* Cloud Client */
#define Cloud_MessageBuffer_SendMessage(pMsg, datalen, Type)                MessageBuffer_SendMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t)Type, pMsg, datalen, MESSAGE_BUFFER_ID_APP2, pdTICKS_TO_MS(100))
#define Cloud_MessageBuffer_ReceiveMessage(pMsgBuffer, pType, pRcvLen)      MessageBuffer_ReceiveMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t*)pType, pMsgBuffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH, pRcvLen, MESSAGE_BUFFER_ID_APP1, 0)
#define CLOUD_INFO(fmt, ...) 	                                            LOG_INFO(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_DEBUG(fmt, ...) 	                                            LOG_DEBUG(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_WARN(fmt, ...) 	                                            LOG_WARN(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_ERROR(fmt, ...) 	                                            LOG_ERROR(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_PRINT_HEX(hexArray, len)                                      LogService_Print_Hex_Array(LOG_MODULE_CLOUDM, (const uint8_t*)hexArray, (uint32_t)len, 1)
/* Cloud Server */
#define CloudNet_MessageBuffer_SendMessage(pMsg, datalen, Type)             MessageBuffer_SendMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t)Type, pMsg, datalen, MESSAGE_BUFFER_ID_APP1, pdTICKS_TO_MS(100))
#define CloudNet_MessageBuffer_ReceiveMessage(pMsgBuffer, pType, pRcvLen)   MessageBuffer_ReceiveMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t*)pType, pMsgBuffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH, pRcvLen, MESSAGE_BUFFER_ID_APP2, 0)
#define CLOUDNET_INFO(fmt, ...) 	                                        LOG_INFO(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_DEBUG(fmt, ...) 	                                        LOG_DEBUG(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_WARN(fmt, ...) 	                                        LOG_WARN(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_ERROR(fmt, ...) 	                                        LOG_ERROR(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_PRINT_HEX(hexArray, len)                                   LogService_Print_Hex_Array(LOG_MODULE_CLOUDNETM, (const uint8_t*)hexArray, (uint32_t)len, 1)
/* freeRTOS */
#define CLOUDM_TASK_DELAY_MS(ms)                                            vTaskDelay(pdMS_TO_TICKS(ms))
#define CLOUDM_MALLOC(size)                                                 pvPortMalloc(size)
#define CLOUDM_FREE(ptr)                                                    vPortFree(ptr)
    
/* Time */
#define CLOUD_TIME_1S_IN_MS                   (1000U)
#define CLOUD_TIME_1MIN_IN_MS                 (60U * CLOUD_TIME_1S_IN_MS)
#define CLOUD_TIME_1H_IN_MS                   (60U * CLOUD_TIME_1MIN_IN_MS)
#define CLOUD_TIME_1D_IN_MS                   (24U * CLOUD_TIME_1H_IN_MS)

/* Get current time during power on */
#define CLOUD_GET_TIME_MS() ({                            \
    static uint64_t base_time = 0;                          \
    uint64_t current_ticks = xTaskGetTickCount();           \
    uint64_t timestamp;                                     \
    if (base_time == 0)                                   \
    {                                                     \
        base_time = 1763822937000; /* 2025-09-01 12:00:00 */ \
    }                                                     \
    timestamp = base_time + current_ticks;                \
    timestamp; /* return the timestamp */                 \
})

#define CLOUD_GET_TIME_MS_PTR(timer)      \
    do                                    \
    {                                     \
        if (timer != NULL)                \
        {                                 \
            *timer = CLOUD_GET_TIME_MS(); \
        }                                 \
    } while (0)

#define CLOUD_PROTOCOL_GET_CURRENT_DATE_TIME(pData)                  \
    do                                                               \
    {                                                                \
        RtcTimedate_Struct lv_stDateTime;                            \
        RTCIF_GetDateTime(&lv_stDateTime);                           \
        memcpy((pData), &lv_stDateTime, sizeof(RtcTimedate_Struct)); \
    } while (0) // get datetime: YYMMDDHHMMSS

#define CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() \
    ({                                         \
        uint32_t __timestamp = 0;              \
        RTC_GetRtcSeconds(&__timestamp);       \
        __timestamp;                           \
    }) // get timestamp: seconds since 1970-01-01 00:00:00

#define CLOUD_PROTOCOL_TIMESTAMP_CONVERT_TO_CP56TIME2A(pTimestamp, pCp56_data) \
    ({                                                                         \
        if ((pTimestamp) != NULL && (pCp56_data) != NULL)                      \
        {                                                                      \
            RtcTimedate_Struct lv_sttimeDate = {0};                            \
            RTC_ConvertSecondsToTimeDate((pTimestamp), &lv_sttimeDate);        \
            RTC_DatetimeToCp56time2a(&lv_sttimeDate, (uint8_t *)(pCp56_data)); \
        }                                                                      \
    }) // convert timestamp to CP56Time2a

#define CLOUD_PROTOCOL_CP56TIME2A_CONVERT_TO_TIMESTAMP(pCp56_data, pTimestamp) \
    ({                                                                         \
        if ((pCp56_data) != NULL && (pTimestamp) != NULL)                      \
        {                                                                      \
            RtcTimedate_Struct lv_sttimeDate = {0};                            \
            RTC_Cp56time2aToDatetime((uint8_t *)(pCp56_data), &lv_sttimeDate); \
            RTC_ConvertTimeDateToSeconds(&lv_sttimeDate, (pTimestamp));        \
        }                                                                      \
    }) // convert CP56Time2a to timestamp

#define CLOUD_PROTOCOL_GET_CP56TIME2A(pCp56_data)                               RTC_GetCP56Time2a((uint8_t *)(pCp56_data)) // get CP56Time2a(BCD)

/******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM = 0x01,
    CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM = 0x02,
    CLOUD_MESSAGE_CTRL_TYPE_SET_REGPKG_MODE = 0x03,
    CLOUD_MESSAGE_CTRL_TYPE_WAKE_UP_DTU = 0x04,
    CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG = 0x05,
    CLOUD_MESSAGE_CTRL_TYPE_MAXIMUM
}cloud_message_type_ctrl_e;

typedef enum
{
    CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL = 0x01,
    CLOUD_MESSAGE_DATA_TYPE_CLOUD_MQTT_PAYLOAD = 0x02,
    CLOUD_MESSAGE_DATA_TYPE_MAXIMUM
}cloud_message_type_data_e;

typedef enum
{
    CLOUD_MESSAGE_NOTIFY_TYPE_DEVICE_STATUS = 0x01,
    CLOUD_MESSAGE_NOTIFY_TYPE_NETWORK_STATUS = 0x02,
    CLOUD_MESSAGE_NOTIFY_TYPE_SIGNAL_STRENGTH = 0x03,
    CLOUD_MESSAGE_NOTIFY_TYPE_MAXIMUM
}cloud_message_type_notify_e;

typedef enum
{
    CLOUD_DEVICE_STATUS_INIT = 0x00,
    CLOUD_DEVICE_STATUS_READY = 0x01,
    CLOUD_DEVICE_SET_NET_OK = 0x02,
    CLOUD_DEVICE_STATUS_MAXIMUM
}cloud_device_status_e;

typedef enum
{
    CLOUD_PROTOCOL_GSTATE_OFFLINE = 0,
    CLOUD_PROTOCOL_GSTATE_ONLINE,
    CLOUD_PROTOCOL_GSTATE_UNREGISTERED,
    CLOUD_PROTOCOL_GSTATE_X,
}cloud_net_status;

typedef enum
{
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_CONNECT = 0x10,     // MQTT connect
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_DISCONNECT = 0x11,  // MQTT break connect
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_PUBLISH = 0x12,     // MQTT publish
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE = 0x13,   // MQTT subscribe
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_UNSUBSCRIBE = 0x14, // MQTT cancel subscribe
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE_PUBLISH = 0x15, // MQTT subscribe and publish
    CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SET_WILL = 0x16,    // Set up MQTT will messages
} cloud_protocol_mqtt_ctrl_type_e;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t MsgType;
    uint8_t MsgData[CLOUD_MESSAGE_BUFFER_MAX_LENGTH];
    uint16_t MsgLen;
} Cloud_Protocol_Msg_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_CFG_H */
/* EOL */
