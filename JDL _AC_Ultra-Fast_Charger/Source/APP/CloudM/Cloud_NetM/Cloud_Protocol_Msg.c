//******************************************************************************
//* File Name: Cloud_Protocol_Msg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud module protocol message source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Msg.h"
#include "Cloud_Protocol.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "Cloud_Protocol_Mqtt.h"
#include "Cloud_Protocol_EventPost_Config.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static Cloud_Protocol_Msg_t Cloud_ProtocolMsg;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_Protocol_Msg_Init(void)
{
    memset(&Cloud_ProtocolMsg, 0u, sizeof(Cloud_ProtocolMsg));
}

void Cloud_Protocol_SendMsg(uint8_t *pMsg, uint16_t MsgLen, uint8_t MsgType)
{
    if ((pMsg != NULL) && (MsgLen <= CLOUD_MESSAGE_BUFFER_MAX_LENGTH))
    {
        Cloud_MessageBuffer_SendMessage(pMsg, MsgLen, MsgType);
    }
    else
    {
        CLOUD_ERROR("Cloud Protocol Send Message Error: Invalid parameters\r\n");
    }
}

void Cloud_Protocol_RcvMsg_Process(void)
{
    uint8_t MsgRet;

    MsgRet = (uint8_t)Cloud_MessageBuffer_ReceiveMessage(&Cloud_ProtocolMsg.MsgData[0], &Cloud_ProtocolMsg.MsgType, &Cloud_ProtocolMsg.MsgLen);

    if (MsgRet)
    {
        // CLOUD_DEBUG("%s, Length: %d Msgtype:%02x data[0]=%02x\r\n", __func__, Cloud_ProtocolMsg.MsgLen, Cloud_ProtocolMsg.MsgType, Cloud_ProtocolMsg.MsgData[0]);
        switch (Cloud_ProtocolMsg.MsgType)
        {
            case CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH:
                switch (Cloud_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL:
                        // Process data message
                        Cloud_Protocol_ParseProtocolFrame((const uint8_t *)&Cloud_ProtocolMsg.MsgData[1], Cloud_ProtocolMsg.MsgLen - 1);
                        break;
                    case CLOUD_MESSAGE_DATA_TYPE_CLOUD_MQTT_PAYLOAD:
                        // Process MQTT payload message
                        Cloud_Protocol_Mqtt_HandleReceivedMessage((const char *)&Cloud_ProtocolMsg.MsgData[1]);
                        break;
                    default:
                        CLOUD_ERROR("Cloud Protocol Unknown Data Command: %d\r\n", Cloud_ProtocolMsg.MsgData[0]);
                        break;
                }
                break;

            case CLOUD_MESSAGE_TYPE_CTRL:
                // Process control message
                switch (Cloud_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM:

                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM:
                        Cloud_Protocol_AckHeartbeatParam(true);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_REGPKG_MODE:
                        Cloud_Protocol_AckRegpkgParam(true);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_WAKE_UP_DTU:
                        Cloud_Protocol_AckWakeUpDTU(true);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG:
                        switch (Cloud_ProtocolMsg.MsgData[1])
                        {
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_CONNECT:
                                // Handle MQTT connect acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Connect Acknowledged\r\n", __func__);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_DISCONNECT:
                                // Handle MQTT disconnect acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Disconnect Acknowledged\r\n", __func__);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_PUBLISH:
                                // Handle MQTT publish acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Publish Acknowledged\r\n", __func__);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE:
                                // Handle MQTT subscribe acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Subscribe Acknowledged\r\n", __func__);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_UNSUBSCRIBE:
                                // Handle MQTT unsubscribe acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Unsubscribe Acknowledged\r\n", __func__);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE_PUBLISH:
                                // Handle MQTT subscribe and publish acknowledgment
                                // CLOUD_DEBUG("%s: SG MQTT Subscribe and Publish Acknowledged\r\n", __func__);
                                Cloud_Protocol_Mqtt_HandleSubscribeAck((const char *)Cloud_ProtocolMsg.MsgData + 2);
                                break;
                            case CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SET_WILL:
                                // Handle MQTT set will acknowledgment
                                CLOUD_DEBUG("%s: SG MQTT Set Will Acknowledged\r\n", __func__);
                                break;
                            default:
                                CLOUD_ERROR("Cloud Protocol Unknown SG MQTT Command: %d\r\n", Cloud_ProtocolMsg.MsgData[1]);
                                break;
                        }
                        break;
                    default:
                        CLOUD_ERROR("Cloud Protocol Unknown Control Command: %d\r\n", Cloud_ProtocolMsg.MsgData[0]);
                        break;
                }
                break;

            case CLOUD_MESSAGE_TYPE_NOTIFY:
                // Process notify message
                switch (Cloud_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_NOTIFY_TYPE_DEVICE_STATUS:
                        Cloud_Protocol_NotifyDeviceStatus((cloud_device_status_e)Cloud_ProtocolMsg.MsgData[1]);
                        CLOUD_INFO("Device Status Notified: %d\r\n", Cloud_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_NOTIFY_TYPE_NETWORK_STATUS:
                        Cloud_Protocol_NotifyNetworkStatus((cloud_net_status)Cloud_ProtocolMsg.MsgData[1], (uint8_t)Cloud_ProtocolMsg.MsgData[2]);
                        break;
                    case CLOUD_MESSAGE_NOTIFY_TYPE_SIGNAL_STRENGTH:
                        Cloud_Protocol_NotifySignalStrength((int8_t)Cloud_ProtocolMsg.MsgData[1]);
                        break;

                    case CLOUD_MESSAGE_NOTIFY_TYPE_ICCID:
                        Cloud_Protocol_EventPost_FwInfo_Set(CLOUD_PROTOCOL_SG_EVENT_FW_SIM_NO, (void *)&Cloud_ProtocolMsg.MsgData[1]);
                        break;
                    default:
                        CLOUD_ERROR("Cloud Protocol Unknown Notify Command: %d\r\n", Cloud_ProtocolMsg.MsgData[0]);
                        break;
                }
                break;

            default:
                CLOUD_ERROR("Cloud Protocol Unknown Message Type: %d\r\n", Cloud_ProtocolMsg.MsgType);
                break;
        }
    }
}
/* EOL */
