#ifndef MessageBuffSTACK_H
#define MessageBuffSTACK_H

#include "MessageBuffStack_Cfg.h"

typedef enum
{
    MESSAGE_USART1_CH = 0U, /* 4G */
    MESSAGE_USART2_CH,      /* LOG */
    MESSAGE_USART4_CH,      /* Meter */
    MESSAGE_USART5_CH,      /* Fan speeder */
    MESSAGE_USART_MAX_NUMBER
} MessageBuffChannel_Enum_t;

typedef enum
{
    MESSAGE_BUFF_OK = 0,
    MESSAGE_BUFF_ERROR,
    MESSAGE_BUFF_FULL,
    MESSAGE_BUFF_EMPTY,
    MESSAGE_BUFF_TIMEOUT,
    MESSAGE_BUFF_RCV_NOT_ENOUGH
} MessageBuffStatus_Enum_t;

typedef struct {
    MessageBufferHandle_t handle;       // 消息缓冲区句柄
    uint8_t *Rcvbuffer;                 // rcv消息缓冲区
    uint16_t Rcvsize;                   // rcv消息长度
    uint8_t *Sendbuffer;                // Send消息缓冲区
    uint16_t Sendsize;                  // Send消息长度
} MessageBuffM_t;

extern MessageBuffM_t Message_Handle[MESSAGE_USART_MAX_NUMBER];

extern uint16_t MessageBuff_StackSendMessage(MessageBuffM_t *message, uint8_t ISR);
extern size_t MessageBuff_CheckMessage(MessageBuffM_t *message);
extern MessageBuffStatus_Enum_t MessageBuff_StackReceiveMessage(MessageBuffM_t *message, uint32_t* Rcvsize, uint8_t ISR);
extern void MessageBuff_StackInit(void);

#endif // MessageBuffSTACK_H
