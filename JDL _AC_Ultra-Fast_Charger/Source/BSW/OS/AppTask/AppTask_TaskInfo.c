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
static char TaskListinfo[TASK_LOG_PRINTF_SIZE * 8 + 300];

/******************************************************************************
 *                      Function definitions
 ******************************************************************************/
extern int Core_printf(const char *format, ...);
/**
 * @brief:任务栈信息调试
 */
static void AppPrintTaskInfo(void)
{
    uint32_t uii              = 0;
    uint32_t task_info_num    = 0;
    uint32_t task_info_remain = 0;

    Core_printf("FreeHeapSize:%d\r\n", xPortGetFreeHeapSize());
    Core_printf("Minimum HeapSize:%d\r\n", xPortGetMinimumEverFreeHeapSize());

    memset(TaskListinfo, 0, sizeof(TaskListinfo));
    vTaskList(TaskListinfo);
	
    task_info_num    = strlen(TaskListinfo) / TASK_LOG_PRINTF_SIZE;
    task_info_remain = strlen(TaskListinfo) % TASK_LOG_PRINTF_SIZE;

    Core_printf("g_task_info[%d:%d],task_info_num:%d,task_info_remain:%d\r\n", sizeof(TaskListinfo), strlen(TaskListinfo),
              task_info_num, task_info_remain);
	
    if (task_info_remain != 0)
    {
        task_info_num++;
    }

    Core_printf("tk_name   tk_state  tk_pri tk_freestack tk_num\r\n");

    for (uii = 0; uii < task_info_num; uii++)
    {
        Core_printf("%s", &TaskListinfo[uii * TASK_LOG_PRINTF_SIZE]);
    }
	
    Core_printf("task list info end\r\n");

    memset(TaskListinfo, 0, sizeof(TaskListinfo));
    vTaskGetRunTimeStats(TaskListinfo);
	
    task_info_num    = strlen(TaskListinfo) / TASK_LOG_PRINTF_SIZE;
    task_info_remain = strlen(TaskListinfo) % TASK_LOG_PRINTF_SIZE;
	
    Core_printf("g_task_info[%d:%d],task_info_num:%d,task_info_remain:%d\r\n", sizeof(TaskListinfo), strlen(TaskListinfo),
              task_info_num, task_info_remain);
	
    if (task_info_remain != 0)
    {
        task_info_num++;
    }

    Core_printf("task_name     running_time 	 task_cpu_used\r\n");
	
    for (uii = 0; uii < task_info_num; uii++)
    {
        Core_printf("%s", &TaskListinfo[uii * TASK_LOG_PRINTF_SIZE]);
    }

    Core_printf("task cpu info end\r\n");
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
    Core_printf("TaskInfo creat success \r\n");

    while (1)
    {
        AppPrintTaskInfo();
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
#endif  /* ENABLE_TASKINFO_TASK */

/* EOF */
