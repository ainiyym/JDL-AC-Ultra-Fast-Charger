#ifndef MESSAGEBUFFSTACK_CFG_H
#define MESSAGEBUFFSTACK_CFG_H
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

#define STREAM_BUFFER_USART1_SIZE 512   /* 4G */
#define STREAM_BUFFER_USART2_SIZE 128   /* LOG */
#define STREAM_BUFFER_USART4_SIZE 128   /* Meter */
#define STREAM_BUFFER_USART5_SIZE 128   /* Fan speeder */

#define STREAM_BUFF_MALLOC(x)    pvPortMalloc(x)
#define STREAM_BUFF_FREE(x)   vPortFree(x)

#endif /* MESSAGEBUFFSTACK_CFG_H */
