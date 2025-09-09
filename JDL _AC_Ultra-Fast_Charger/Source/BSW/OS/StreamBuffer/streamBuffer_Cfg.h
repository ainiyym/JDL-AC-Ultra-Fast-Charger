#ifndef MESSAGEBUFFSTACK_CFG_H
#define MESSAGEBUFFSTACK_CFG_H
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

#define STREAM_BUFFER_SIZE 256

#define STREAM_BUFF_MALLOC(x)    pvPortMalloc(x)
#define STREAM_BUFF_FREE(x)   vPortFree(x)

#endif /* MESSAGEBUFFSTACK_CFG_H */
