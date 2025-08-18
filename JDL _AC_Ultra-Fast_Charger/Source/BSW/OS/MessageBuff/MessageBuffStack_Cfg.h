#ifndef MESSAGEBUFFSTACK_CFG_H
#define MESSAGEBUFFSTACK_CFG_H
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

#define MESSAGE_BUFFER_SIZE 1024

#define MESSAGE_MALLOC(x)    pvPortMalloc(x)
#define MESSAGE_FREE(x)   vPortFree(x)

#endif /* MESSAGEBUFFSTACK_CFG_H */
