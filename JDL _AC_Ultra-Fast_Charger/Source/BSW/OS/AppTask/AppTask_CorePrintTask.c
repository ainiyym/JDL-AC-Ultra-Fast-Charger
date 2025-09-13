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
#include "Mcal_Gpio_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// 双缓冲结构
typedef struct
{
    print_item_t *buffer;
    uint16_t count;
    uint16_t total_length;
} print_batch_t;

// 内存池管理
typedef struct
{
    char *memory_pool;
    size_t pool_size;
    size_t used;
} print_memory_pool_t;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static QueueHandle_t printQueue = NULL;
static SemaphoreHandle_t printMutex = NULL;

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
static print_memory_pool_t memory_pool;

void CorePrint_TaskInit(void)
{
    // 创建互斥锁用于内存池管理
    printMutex = xSemaphoreCreateMutex();

    // 初始化内存池 (例如4KB)
    memory_pool.pool_size = 4096;
    memory_pool.memory_pool = pvPortMalloc(memory_pool.pool_size);
    memory_pool.used = 0;

    // 创建打印队列
    printQueue = xQueueCreate(PRINT_QUEUE_LENGTH, sizeof(print_item_t *));
}

// 从内存池分配内存
static char *print_malloc(size_t size)
{
    char *allocated = NULL;

    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE)
    {
        if (memory_pool.used + size <= memory_pool.pool_size)
        {
            allocated = memory_pool.memory_pool + memory_pool.used;
            memory_pool.used += size;
        }
        xSemaphoreGive(printMutex);
    }

    // 如果内存池不足，使用系统malloc
    if (allocated == NULL)
    {
        allocated = pvPortMalloc(size);
    }

    return allocated;
}

// 释放内存（如果是内存池分配的，实际上不单独释放）
static void print_free(char *ptr)
{
    // 如果指针不在内存池范围内，使用系统free
    if (ptr < memory_pool.memory_pool ||
        ptr >= memory_pool.memory_pool + memory_pool.pool_size)
    {
        vPortFree(ptr);
    }
    // 内存池分配的内存会在任务清理时统一处理
}

uint8_t CorePrint_IsEmpty(void)
{
    return (printQueue == NULL || uxQueueMessagesWaiting(printQueue) == 0);
}

// 清理函数，释放所有动态内存
void CorePrint_Cleanup(void)
{
    print_item_t *item;

    // 清空队列中的所有消息
    while (xQueueReceive(printQueue, &item, 0) == pdPASS)
    {
        print_free(item->message);
        vPortFree(item);
    }

    // 释放内存池
    if (memory_pool.memory_pool != NULL)
    {
        vPortFree(memory_pool.memory_pool);
        memory_pool.memory_pool = NULL;
        memory_pool.used = 0;
    }
}

uint16_t Core_Printf_AddItem(const char *message)
{
    size_t msg_len = strlen(message);
    print_item_t *item = pvPortMalloc(sizeof(print_item_t));

    if (item == NULL)
    {
        return 0;
    }

    item->message = print_malloc(msg_len + 1);
    if (item->message == NULL)
    {
        vPortFree(item);
        return 0;
    }

    strncpy(item->message, message, msg_len);
    item->message[msg_len] = '\0';
    item->length = msg_len;

    if (printQueue != NULL)
    {
        if (xQueueSend(printQueue, &item, 0) != pdPASS)
        {
            // Queue full, free memory
            print_free(item->message);
            vPortFree(item);
            Mcal_Gpio_SetPin(LED3_GPIO_Port, LED3_Pin);
            return 0;
        }
    }
    return item->length;
}

// 高性能打印函数
int Core_printf(const char *format, ...)
{
    va_list arg;
    print_item_t *item;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult;

    if (printQueue == NULL)
    {
        return 0;
    }

    // 分配项目结构体内存
    item = pvPortMalloc(sizeof(print_item_t));
    if (item == NULL)
    {
         Mcal_Gpio_SetPin(LED3_GPIO_Port, LED3_Pin);
        return 0;
    }

    // 分配消息内存
    item->message = print_malloc(PRINT_MAX_MESSAGE_SIZE);
    if (item->message == NULL)
    {
        vPortFree(item);
        return 0;
    }

    // 格式化消息
    va_start(arg, format);
    item->length = vsnprintf(item->message, PRINT_MAX_MESSAGE_SIZE, format, arg);
    va_end(arg);

    // 确保字符串正确终止
    if (item->length >= PRINT_MAX_MESSAGE_SIZE)
    {
        item->length = PRINT_MAX_MESSAGE_SIZE - 1;
        item->message[PRINT_MAX_MESSAGE_SIZE - 1] = '\0';
    }

    // 检查是否在中断上下文中调用
    if (xPortIsInsideInterrupt())
    {
        xResult = xQueueSendToBackFromISR(printQueue, &item, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken == pdTRUE)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
    else
    {
        xResult = xQueueSend(printQueue, &item, 0);
    }

    if (xResult != pdPASS)
    {
        // 队列满，释放内存
        print_free(item->message);
        vPortFree(item);
        Mcal_Gpio_SetPin(LED3_GPIO_Port, LED3_Pin);
        return 0;
    }
    
    return item->length;
}

// 高性能打印任务（使用批量处理和双缓冲）
void CorePrint_Task(void *pvParameters)
{
    print_item_t *received_items[PRINT_BATCH_SIZE];

    while (1)
    {
        UBaseType_t received_count = 0;

        // 接收一个批次的消息
        while (received_count < PRINT_BATCH_SIZE)
        {
            if (xQueueReceive(printQueue, &received_items[received_count], 0) != pdPASS)
            {
                break;  // 队列为空，退出循环
            }
            received_count++;
        }

        if (received_count > 0)
        {
            // 处理批次中的每条消息
            for (UBaseType_t i = 0; i < received_count; i++)
            {
                print_item_t *item = received_items[i];
                // HAL_UART_Transmit(&huart2, (uint8_t *)item->message, item->length, 200);
                HAL_UART_Transmit_DMA(&huart2, (uint8_t *)item->message, item->length);
                // 等待传输完成
                while (huart2.gState != HAL_UART_STATE_READY)
                {
                    vTaskDelay(1);
                }
                // 释放内存
                print_free(item->message);
                vPortFree(item);
            }
        }
        else
        {
            vTaskDelay(1);
        }
    }
}

//紧急直接打印函数（绕过队列）
int Core_Print_Immediate(const char *format, ...)
{
    va_list arg;
    char buffer[PRINT_MAX_MESSAGE_SIZE];
    int length;

    va_start(arg, format);
    length = vsnprintf(buffer, sizeof(buffer), format, arg);
    va_end(arg);

    if (length > 0)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)buffer, length, 25);
    }

    return length;
}
/* EOL */
