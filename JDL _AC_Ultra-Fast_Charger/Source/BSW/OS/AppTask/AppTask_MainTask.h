
#ifndef APPTASK_MAINTASK_H
#define APPTASK_MAINTASK_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 **                        MISRA-C Rules Violations
 ******************************************************************************/
/**
 * @page misra_violations MISRA-C:2004 violations
 *
 * @section
 */

/******************************************************************************
 **                        QAC Warnings
 ******************************************************************************/
/**
 * @page QAC Warnings
 *
 */

/******************************************************************************
 **                        Include Files
 ******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "Mcal_SysTick.h"
#include "OS.h"
#include "STD_OS_Timer.h"
#include "ScheduleTable.h"
#include "StreamBuffer.h"
#include "Mcal_Gpio_Cfg.h"

/******************************************************************************
 **                        File Version Check
 ******************************************************************************/

/******************************************************************************
 **                        Macro  Definitions
 ******************************************************************************/
#define APPTASK_MAINTASK_RUN_START()            //Mcal_Gpio_SetPin(TEST_E1_Port, TEST_E1_Pin)
#define APPTASK_MAINTASK_RUN_END()              //Mcal_Gpio_ResetPin(TEST_E1_Port, TEST_E1_Pin)

/******************************************************************************
 **                        Typedef  Definitions
 ******************************************************************************/
/**
 * @brief:任务优先级
 */
typedef enum
{
    TASK_START_PRIO_1 = 1,
    TASK_START_PRIO_2,
    TASK_START_PRIO_3,
    TASK_START_PRIO_4,
    TASK_START_PRIO_5,
    TASK_START_PRIO_6,
    TASK_START_PRIO_7,
    TASK_START_PRIO_8,
    TASK_START_PRIO_9,
    TASK_START_PRIO_10,
    TASK_START_PRIO_11,
    TASK_START_PRIO_12,
    TASK_START_PRIO_13,
    TASK_START_PRIO_14,
    TASK_START_PRIO_15,
    TASK_START_PRIO_16,
    TASK_START_PRIO_17,
    TASK_START_PRIO_18,
    TASK_START_PRIO_19,
    TASK_START_PRIO_20,
    TASK_START_PRIO_21,
    TASK_START_PRIO_22,
    TASK_START_PRIO_23,
    TASK_START_PRIO_24,
    TASK_START_PRIO_25,
    TASK_START_PRIO_26,
    TASK_START_PRIO_27,
    TASK_START_PRIO_28,
    TASK_START_PRIO_29,
    TASK_START_PRIO_30
} TASK_START_PRIO;
/******************************************************************************
 **                        Export Variables
 ******************************************************************************/

/******************************************************************************
 **                        Export Functions
 ******************************************************************************/
/**
 * @brief 系统延时
 *
 * @param  msec-延时时间，单位毫秒
 */
void FreeRTOS_Sleep(int msec);

/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void AppTask_MainTask(void *pvParameters);
void OSTimerTask_MainTask(void *pvParameters);
void UsartTask_MainTask(void *pvParameters);
#ifdef __cplusplus
}
#endif

#endif
/* EOF */
