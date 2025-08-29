/**
 * @file    MessageBuffStack.c
 * @brief   Implementation of message queue management functions.
 * @author  
 * @date    
 */
#include "MessageBuffStack.h"
// 创建消息句柄
MessageBuffM_t Message_Handle[MESSAGE_USART_MAX_NUMBER];

uint16_t MessageBuff_StackSendMessage(MessageBuffM_t *message, uint8_t ISR)
{
    uint16_t message_sent_len = 0;

    if (message == NULL || message->handle == NULL || message->Sendbuffer == NULL)
    {
        SYSM_printf("Invalid message Rcvbuffer handle or Sendbuffer.\r\n");
        return 0;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // 发送消息，并获取发送的消息长度
    if (ISR)
    {
        message_sent_len = xMessageBufferSendFromISR(
            message->handle,
            message->Sendbuffer,
            message->Sendsize,
            &xHigherPriorityTaskWoken
        );
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        // 在非ISR上下文中发送消息
        message_sent_len = xMessageBufferSend(message->handle, message->Sendbuffer, message->Sendsize, 0x100);
    }

    return message_sent_len;
}


MessageBuffStatus_Enum_t MessageBuff_StackReceiveMessage(MessageBuffM_t *message, uint32_t* Rcvsize, uint8_t ISR)
{
    if (message == NULL || message->handle == NULL || message->Rcvbuffer == NULL)
    {
        SYSM_printf("Invalid message Rcvbuffer handle or Rcvbuffer.\r\n");
        return MESSAGE_BUFF_ERROR;
    }
    size_t xNextLength = xMessageBufferNextLengthBytes(message->handle);
    if (xNextLength == 0)
    {
        // No message available
        *Rcvsize = 0;
        return MESSAGE_BUFF_EMPTY;
    }

    if (ISR)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        *Rcvsize = xMessageBufferReceiveFromISR(message->handle, message->Rcvbuffer, message->Rcvsize, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        *Rcvsize = xMessageBufferReceive(message->handle, message->Rcvbuffer, message->Rcvsize, 0);
    }

    return MESSAGE_BUFF_OK;
}

void MessageBuff_StackInit(void)
{
    // 创建消息缓冲区
    for (uint8_t i = 0; i < MESSAGE_USART_MAX_NUMBER; i++)
    {
        // 创建消息缓存
        Message_Handle[i].handle = xMessageBufferCreate(MESSAGE_BUFFER_SIZE);
        if (Message_Handle[i].handle == NULL)
        {
           SYSM_printf("Failed to create message Rcvbuffer. \r\n");
        }
    }
}
