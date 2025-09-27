//******************************************************************************
//* File Name: CloudNet_Protocol_Msg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud module protocol message source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "CloudNet_Protocol_Msg.h"

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
static Cloud_Protocol_Msg_t CloudNet_ProtocolMsg;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void CloudNet_Protocol_Msg_Init(void)
{
    memset(&CloudNet_ProtocolMsg, 0u, sizeof(CloudNet_ProtocolMsg));
}

void CloudNet_Protocol_SendMsg(uint8_t *pMsg, uint16_t MsgLen, uint8_t MsgType)
{
    if ((pMsg != NULL) && (MsgLen <= CLOUD_MESSAGE_BUFFER_MAX_LENGTH))
    {
        CloudNet_MessageBuffer_SendMessage(pMsg, MsgLen, MsgType);
    }
    else
    {
        CLOUD_ERROR("CloudNet Protocol Send Message Error: Invalid parameters\r\n");
    }
}

void CloudNet_Protocol_RcvMsg_Process(void)
{
    uint8_t MsgRet;

    MsgRet = (uint8_t)CloudNet_MessageBuffer_ReceiveMessage(&CloudNet_ProtocolMsg.MsgData[0], &CloudNet_ProtocolMsg.MsgType, &CloudNet_ProtocolMsg.MsgLen);

    if (MsgRet)
    {
        CLOUDNET_DEBUG("%s, Length: %d data[0]=%d\r\n", __func__, CloudNet_ProtocolMsg.MsgLen, CloudNet_ProtocolMsg.MsgData[0]);
        switch (CloudNet_ProtocolMsg.MsgType)
        {
            case CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH:
                // Process data message
                switch (CloudNet_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_DATA_TYPE_SEND_LOGIN_FRAME:
                        YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_REGHEAD, NULL, (const char *)&CloudNet_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_DATA_TYPE_SEND_HEARTBEAT_FRAME:
                        YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_HBHEAD, NULL, (const char *)&CloudNet_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_DATA_TYPE_DATA_PASSTHROUGH:
  
                        break;
                    default:
                        CLOUDNET_ERROR("Cloud Protocol Unknown Data Command: %d\r\n", CloudNet_ProtocolMsg.MsgData[0]);
                        break;
                }
                break;

            case CLOUD_MESSAGE_TYPE_CTRL:
                // Process control message
                switch (CloudNet_ProtocolMsg.MsgData[0])
                {
                    case CLOUD_MESSAGE_CTRL_TYPE_DEVICE_READY:

                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM:
                        switch (CloudNet_ProtocolMsg.MsgData[1])
                        {
                            case TCP_ID_PROTOCOL:
                                // Set protocol TCP parameters
                                tcp_connect(TCP_ID_PROTOCOL);
                                break;
                            default:
                                CLOUDNET_ERROR("Cloud Protocol Unknown Network Parameter Command: %d\r\n", CloudNet_ProtocolMsg.MsgData[1]);
                                break;
                        }

                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM:
                        YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_HBTIME, NULL, (const char *)&CloudNet_ProtocolMsg.MsgData[1]);
                        break;
                    case CLOUD_MESSAGE_CTRL_TYPE_SET_REGPKG_MODE:
                        YeeCom_AtCmd_Send(YEECOM_AT_CMD_SET, YEECOM_AT_CMD_REGPKG, NULL, (const char *)&CloudNet_ProtocolMsg.MsgData[1]);
                        break;
                    default:
                        CLOUDNET_ERROR("Cloud Protocol Unknown Control Command: %d\r\n", CloudNet_ProtocolMsg.MsgData[0]);
                        break;
                }
                break;

            default:
                CLOUDNET_ERROR("Cloud Protocol Unknown Message Type: %d\r\n", CloudNet_ProtocolMsg.MsgType);
                break;
        }
    }
}
/* EOL */
