//******************************************************************************
//* File Name: AppTask_CorePrintTask.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: AppTask_CorePrintTask source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "string.h"
#include "stdio.h"
#include "AppTask_CorePrintTask.h"
#include "STD_LogService.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define PRINT_QUEUE_LENGTH 20
#define PRINT_ITEM_SIZE    128

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    char message[PRINT_ITEM_SIZE];
    uint16_t length;
} print_item_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static QueueHandle_t printQueue = NULL;

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
// SemaphoreHandle_t uart_tx_semaphore;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void CorePrint_TaskInit(void)
{
    printQueue = xQueueCreate(PRINT_QUEUE_LENGTH, sizeof(print_item_t));
	LogService_SetLogEnable();
}

uint8_t CorePrint_IsEmpty(void)
{
    return (printQueue == NULL || uxQueueMessagesWaiting(printQueue) == 0);
}

// General print function (supports task and interrupt calls)
int Core_printf(const char *format, ...)
{
    va_list arg;
    print_item_t item;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult;

    if (printQueue == NULL)
    {
        return 0;
    }

    va_start(arg, format);
    item.length = vsnprintf(item.message, sizeof(item.message), format, arg);
    va_end(arg);

    // Ensure string is properly terminated
    if (item.length >= sizeof(item.message))
    {
        item.length = sizeof(item.message) - 1;
        item.message[sizeof(item.message) - 1] = '\0';
    }

    // Check if called from interrupt context
    if (xPortIsInsideInterrupt())
    {
        // Called in interrupt - use FromISR version
        xResult = xQueueSendToBackFromISR(printQueue, &item, &xHigherPriorityTaskWoken);

        // If a higher priority task is woken, request context switch
        if (xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        // Called in task - use normal version
        xResult = xQueueSend(printQueue, &item, 0);
    }

    if (xResult != pdPASS)
    {
        // Queue full, discard print content
        return 0;
    }

    return item.length;
}

/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void CorePrint_Task(void *pvParameters)
{
    print_item_t item;
    Core_printf("CorePrint_Task create success.\r\n");
    while (1)
    {
        // Process all messages in the print queue
        while (xQueueReceive(printQueue, &item, 0) == pdPASS)
        {
            HAL_UART_Transmit(&huart2, (uint8_t *)item.message, item.length, 25);
        }
        vTaskDelay(pdMS_TO_TICKS(20)); // Adjust delay as needed
    }
}
/* EOL */
