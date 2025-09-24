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

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUDM_TASK_PERIOD								                    (10U)
#define CLOUD_MESSAGE_BUFFER_MAX_LENGTH                                     (256U)
#define CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH                                 (MESSAGE_BUFFER_TYPE_DATA)
#define CLOUD_MESSAGE_TYPE_CTRL                                             (MESSAGE_BUFFER_TYPE_CTRL)
/* Cloud Client */
#define Cloud_MessageBuffer_SendMessage(pMsg, datalen, Type)                MessageBuffer_SendMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t)Type, pMsg, datalen, MESSAGE_BUFFER_ID_APP2, pdTICKS_TO_MS(100))
#define Cloud_MessageBuffer_ReceiveMessage(pMsgBuffer, pType, pRcvLen)      MessageBuffer_ReceiveMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t*)pType, pMsgBuffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH, pRcvLen, MESSAGE_BUFFER_ID_APP1, pdTICKS_TO_MS(100))
#define CLOUD_INFO(fmt, ...) 	                                            LOG_INFO(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_DEBUG(fmt, ...) 	                                            LOG_DEBUG(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
#define CLOUD_ERROR(fmt, ...) 	                                            LOG_ERROR(LOG_MODULE_CLOUDM, fmt, ##__VA_ARGS__)
/* Cloud Server */
#define CloudNet_MessageBuffer_SendMessage(pMsg, datalen, Type)             MessageBuffer_SendMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t)Type, pMsg, datalen, MESSAGE_BUFFER_ID_APP1, pdTICKS_TO_MS(100))
#define CloudNet_MessageBuffer_ReceiveMessage(pMsgBuffer, pType, pRcvLen)   MessageBuffer_ReceiveMessage(MessageBuffer_APP_And_NET, (MessageBuffer_type_t*)pType, pMsgBuffer, CLOUD_MESSAGE_BUFFER_MAX_LENGTH, pRcvLen, MESSAGE_BUFFER_ID_APP2, pdTICKS_TO_MS(100))
#define CLOUDNET_INFO(fmt, ...) 	                                        LOG_INFO(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_DEBUG(fmt, ...) 	                                        LOG_DEBUG(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
#define CLOUDNET_ERROR(fmt, ...) 	                                        LOG_ERROR(LOG_MODULE_CLOUDNETM, fmt, ##__VA_ARGS__)
/* freeRTOS */
#define CLOUDM_TASK_DELAY_MS(ms)                                            vTaskDelay(pdMS_TO_TICKS(ms))
#define CLOUDM_MALLOC(size)                                                 pvPortMalloc(size)
#define CLOUDM_FREE(ptr)                                                    vPortFree(ptr)

/******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CLOUD_MESSAGE_CTRL_TYPE_DEVICE_READY = 0x01,
    CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM = 0x02,
    CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM = 0x03,
    CLOUD_MESSAGE_CTRL_TYPE_LOGIN_ACK = 0x04,
    CLOUD_MESSAGE_CTRL_TYPE_HEARTBEAT_ACK = 0x05,
    CLOUD_MESSAGE_CTRL_TYPE_MAXIMUM
}cloud_message_type_ctrl_e;

typedef enum
{
    CLOUD_DEVICE_STATUS_INIT = 0x00,
    CLOUD_DEVICE_STATUS_READY = 0x01,
    CLOUD_DEVICE_STATUS_CONNECTED = 0x02,
    CLOUD_DEVICE_STATUS_DISCONNECTED = 0x03,
    CLOUD_DEVICE_STATUS_MAXIMUM
}cloud_device_status_e;
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
