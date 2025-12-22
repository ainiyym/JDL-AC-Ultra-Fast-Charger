/**
 * @file    stream_buffer.c
 * @brief   Implementation of data stream management functions.
 * @author  
 * @date    
 */
#include "streamBuffer.h"
#include "AppTask_CorePrintTask.h"
// 创建消息句柄
StreamBuffM_t Message_Handle[STREAM_USART_MAX_NUMBER];

uint16_t StreamBuff_SendMessage(StreamBuffM_t *message, uint8_t ISR)
{
    uint16_t message_sent_len = 0;

    if (message == NULL || message->handle == NULL || message->Sendbuffer == NULL)
    {
        // Core_printf("Invalid message Rcvbuffer handle or Sendbuffer.\r\n");
        return 0;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // 发送消息，并获取发送的消息长度
    if (ISR)
    {
        message_sent_len = xStreamBufferSendFromISR(
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
        message_sent_len = xStreamBufferSend(message->handle, message->Sendbuffer, message->Sendsize, pdMS_TO_TICKS(100));
    }

    return message_sent_len;
}

size_t StreamBuff_CheckMessage(StreamBuffM_t *message)
{
    return xStreamBufferBytesAvailable(message->handle);
}

StreamBuffStatus_Enum_t StreamBuff_ReceiveMessage(StreamBuffM_t *message, uint32_t* Rcvsize, uint8_t ISR)
{
    if (message == NULL || message->handle == NULL || message->Rcvbuffer == NULL)
    {
        Core_printf("Invalid message Rcvbuffer handle or Rcvbuffer.\r\n");
        return STREAM_BUFF_ERROR;
    }
    size_t xNextLength = StreamBuff_CheckMessage(message);
    if (xNextLength == 0)
    {
        // No message available
        *Rcvsize = 0;
        return STREAM_BUFF_EMPTY; 
    }

    if (ISR)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        *Rcvsize = xStreamBufferReceiveFromISR(message->handle, message->Rcvbuffer, message->Rcvsize, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        *Rcvsize = xStreamBufferReceive(message->handle, message->Rcvbuffer, message->Rcvsize, 0);
    }

    return STREAM_BUFF_OK;
}

void StreamBuff_StackInit(void)
{
    // 创建消息缓存
    Message_Handle[STREAM_USART1_CH].handle = xStreamBufferCreate(STREAM_BUFFER_USART1_SIZE, 1);
    if (Message_Handle[STREAM_USART1_CH].handle == NULL)
    {
        Core_printf("Failed to create StreamBuff[%d]. \r\n", STREAM_USART1_CH);
    }

    Message_Handle[STREAM_USART2_CH].handle = xStreamBufferCreate(STREAM_BUFFER_USART2_SIZE, 1);
    if (Message_Handle[STREAM_USART2_CH].handle == NULL)
    {
        Core_printf("Failed to create StreamBuff[%d]. \r\n", STREAM_USART2_CH);
    }

    Message_Handle[STREAM_USART4_CH].handle = xStreamBufferCreate(STREAM_BUFFER_USART4_SIZE, 1);
    if (Message_Handle[STREAM_USART4_CH].handle == NULL)
    {
        Core_printf("Failed to create StreamBuff[%d]. \r\n", STREAM_USART4_CH);
    }

    Message_Handle[STREAM_USART5_CH].handle = xStreamBufferCreate(STREAM_BUFFER_USART5_SIZE, 1);
    if (Message_Handle[STREAM_USART5_CH].handle == NULL)
    {
        Core_printf("Failed to create StreamBuff[%d]. \r\n", STREAM_USART5_CH);
    }
}
