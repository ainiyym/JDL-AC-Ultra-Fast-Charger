
/******************************************************************************
 *                      Include Files
 ******************************************************************************/
#include "string.h"
#include "stdio.h"
#include "AppTask_MainTask.h"
#include "Mcal_Test.h"

/******************************************************************************
 *                      Macro Definitions
 ******************************************************************************/

/******************************************************************************
 *                      Constants
 ******************************************************************************/

/******************************************************************************
 *                      Type definitions
 ******************************************************************************/

/******************************************************************************
 *                      Variables
 ******************************************************************************/
EventGroupHandle_t UsartEvent_Handle = NULL;

/******************************************************************************
 *                      Function definitions
 ******************************************************************************/
/**
 * @brief 系统延时
 *
 * @param  msec-延时时间，单位毫秒
 */
void FreeRTOS_Sleep(int msec)
{
    TickType_t ticks = msec / portTICK_PERIOD_MS;

    /* Minimum delay = 1 tick */
    vTaskDelay(ticks ? ticks : 1);
}

/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void AppTask_MainTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(1);
    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {        
        APPTASK_MAINTASK_RUN_START();
        Scheduler_ISRCb();
        Mcal_SYSTICK_Counter_Increase();
        OS_Schedule();
        APPTASK_MAINTASK_RUN_END();
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

extern void Mcal_GpTime_AdcCollection_Start(void);
void OSTimerTask_MainTask(void *pvParameters)
{
    Mcal_GpTime_AdcCollection_Start();

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        APPTASK_MAINTASK_RUN_START();
        OS_SoftTimerSoftTimerINT_CB();
        OS_TimerTask();
        APPTASK_MAINTASK_RUN_END();
    }
}
/* EOF */
