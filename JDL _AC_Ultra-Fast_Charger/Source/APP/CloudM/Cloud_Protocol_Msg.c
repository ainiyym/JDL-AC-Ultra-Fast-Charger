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

static void Cloud_Protocol_RcvMsg_Process(void)
{
    uint8_t MsgRet;

    MsgRet = (uint8_t)Cloud_MessageBuffer_ReceiveMessage(&Cloud_ProtocolMsg.MsgData[0], &Cloud_ProtocolMsg.MsgType, &Cloud_ProtocolMsg.MsgLen);

    if (MsgRet)
    {
        switch (Cloud_ProtocolMsg.MsgType)
        {
            case CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH:
                CLOUD_DEBUG("Cloud Protocol Data Message Received, Length: %d\r\n", Cloud_ProtocolMsg.MsgLen);
                // Process data message
                Cloud_Protocol_ParseProtocolFrame((const uint8_t *)&Cloud_ProtocolMsg.MsgData[0], Cloud_ProtocolMsg.MsgLen);
                break;

            case CLOUD_MESSAGE_TYPE_CTRL:
                // Process control message
                CLOUD_DEBUG("Cloud Protocol Control Message Received, Length: %d data[0]=%d\r\n", Cloud_ProtocolMsg.MsgLen, Cloud_ProtocolMsg.MsgData[0]);
                switch (Cloud_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_CTRL_TYPE_DEVICE_READY:
                        Cloud_Protocol_SetDeviceStatus((cloud_device_status_e)Cloud_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM:
                        Cloud_Protocol_SetDeviceStatus((cloud_device_status_e)Cloud_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM:
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_LOGIN_ACK:
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_HEARTBEAT_ACK:
                        break;
                    default:
                        CLOUD_ERROR("Cloud Protocol Unknown Control Command: %d\r\n", Cloud_ProtocolMsg.MsgData[0]);
                        break;
                }
                CLOUD_DEBUG("<%s> Cloud Protocol Device Status Set to: %d\r\n", __func__, Cloud_ProtocolMsg.MsgData[1]);
                break;

            default:
                CLOUD_ERROR("Cloud Protocol Unknown Message Type: %d\r\n", Cloud_ProtocolMsg.MsgType);
                break;
        }
    }
}
/* EOL */
