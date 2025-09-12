/******************************************************************************
 *                      Include Files
 ******************************************************************************/
#include "AppTask_TaskInfo.h"
#ifdef ENABLE_TASKINFO_TASK
#include "string.h"
#include "stdio.h"
#include "AppTask_MainTask.h"
#include "Mcal_Usart_Cfg.h"
/******************************************************************************
 *                      Macro Definitions
 ******************************************************************************/
#define TASK_LOG_PRINTF_SIZE (80)

/******************************************************************************
 *                      Constants
 ******************************************************************************/

/******************************************************************************
 *                      Type definitions
 ******************************************************************************/

/******************************************************************************
 *                      Variables
 ******************************************************************************/
/**
 * @brief:栈信息
 */
#define TASK_INFO_BUFFER_SIZE (TASK_LOG_PRINTF_SIZE * 7 + 300)

static char TaskListinfo[TASK_INFO_BUFFER_SIZE];
/******************************************************************************
 *                      Function definitions
 ******************************************************************************/
extern int Core_printf(const char *format, ...);
/**
 * @brief:任务栈信息调试
 */
static void AppPrintTaskInfo(void)
{
    uint32_t task_info_num = 0;
    size_t task_list_len = 0;

    // 打印堆信息
    Core_printf("FreeHeapSize: %d\r\n", xPortGetFreeHeapSize());
    Core_printf("Minimum HeapSize: %d\r\n", xPortGetMinimumEverFreeHeapSize());

    // 获取任务列表信息
    memset(TaskListinfo, 0, sizeof(TaskListinfo));
    vTaskList(TaskListinfo);

    task_list_len = strlen(TaskListinfo);
    task_info_num = (task_list_len + TASK_LOG_PRINTF_SIZE - 1) / TASK_LOG_PRINTF_SIZE; // 向上取整

    Core_printf("Task info buffer[%d:%d], chunks: %d\r\n",
                sizeof(TaskListinfo), task_list_len, task_info_num);

    // 打印任务列表头
    Core_printf("Task Name    State   Pri   FreeStack   Task#\r\n");

    // 分段打印任务信息
    for (uint32_t i = 0; i < task_info_num; i++)
    {
        const char *chunk_start = &TaskListinfo[i * TASK_LOG_PRINTF_SIZE];
        Core_printf("%.*s", TASK_LOG_PRINTF_SIZE, chunk_start);
    }

    Core_printf("Task list info end\r\n\r\n");

    // 获取任务运行时间统计
    memset(TaskListinfo, 0, sizeof(TaskListinfo));
    vTaskGetRunTimeStats(TaskListinfo);

    task_list_len = strlen(TaskListinfo);
    task_info_num = (task_list_len + TASK_LOG_PRINTF_SIZE - 1) / TASK_LOG_PRINTF_SIZE; // 向上取整

    Core_printf("Task stats buffer[%d:%d], chunks: %d\r\n",
                sizeof(TaskListinfo), task_list_len, task_info_num);

    // 打印运行时间统计头
    Core_printf("Task Name        Run Time        CPU Usage\r\n");

    // 分段打印运行时间统计
    for (uint32_t i = 0; i < task_info_num; i++)
    {
        const char *chunk_start = &TaskListinfo[i * TASK_LOG_PRINTF_SIZE];
        Core_printf("%.*s", TASK_LOG_PRINTF_SIZE, chunk_start);
    }

    Core_printf("Task CPU info end\r\n");
}

/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void AppTask_TaskInfo(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(10000);

    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        AppPrintTaskInfo();
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
#endif  /* ENABLE_TASKINFO_TASK */

/* EOF */
