#ifndef StreamBuffSTACK_H
#define StreamBuffSTACK_H

#include "streamBuffer_Cfg.h"

typedef enum
{
    STREAM_USART1_CH = 0U, /* 4G */
    STREAM_USART2_CH,      /* LOG */
    STREAM_USART4_CH,      /* Meter */
    STREAM_USART5_CH,      /* Fan speeder */
    STREAM_USART_MAX_NUMBER
} StreamBuffChannel_Enum_t;

typedef enum
{
    STREAM_BUFF_OK = 0,
    STREAM_BUFF_ERROR,
    STREAM_BUFF_FULL,
    STREAM_BUFF_EMPTY,
    STREAM_BUFF_TIMEOUT,
    STREAM_BUFF_RCV_NOT_ENOUGH
} StreamBuffStatus_Enum_t;

typedef struct {
    StreamBufferHandle_t  handle;       // 消息缓冲区句柄
    uint8_t *Rcvbuffer;                 // rcv消息缓冲区
    uint16_t Rcvsize;                   // rcv消息长度
    uint8_t *Sendbuffer;                // Send消息缓冲区
    uint16_t Sendsize;                  // Send消息长度
} StreamBuffM_t;

extern StreamBuffM_t Message_Handle[STREAM_USART_MAX_NUMBER];

extern uint16_t StreamBuff_SendMessage(StreamBuffM_t *message, uint8_t ISR);
extern size_t StreamBuff_CheckMessage(StreamBuffM_t *message);
extern StreamBuffStatus_Enum_t StreamBuff_ReceiveMessage(StreamBuffM_t *message, uint32_t* Rcvsize, uint8_t ISR);
extern void StreamBuff_StackInit(void);

#endif // StreamBuffSTACK_H
