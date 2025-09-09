//****************************************************************************************
//*
//* File Name: main.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#include"stdint.h"
#include "STD_SysM.h"
#include "AppTask_MainTask.h"
#include "AppTask_TaskInfo.h"
#include "AppTask_4gTask.h"
#include "AppTask_CorePrintTask.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MAIN_TASK_STACK_SIZE (6u * 1024u / 4u)
#define OS_TIMER_TASK_STACK_SIZE (1u * 1024u / 4u)
#define TASK_INFO_TASK_STACK_SIZE (4u * 1024u / 4u)
#define TASK_4G_TASK_STACK_SIZE (4u * 1024u / 4u)
#define TASK_AT_TASK_STACK_SIZE (2u * 1024u / 4u)
#define TASK_CORE_PRINT_TASK_STACK_SIZE (1.5 * 1024u / 4u)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
 
/*******************************************************************************
|    Static local variables Declaration  

|******************************************************************************/
#if (configSUPPORT_STATIC_ALLOCATION == 1)
static StackType_t  Main_Stack[MAIN_TASK_STACK_SIZE];
static StaticTask_t Main_StaticTask;
#endif

/* os timer  task handle */
TaskHandle_t OsTimer_Task_Handle = NULL;

/* 4G  task handle */
TaskHandle_t M4g_Task_Handle = NULL;
TaskHandle_t AtTask_Handle = NULL;

#ifdef ENABLE_TASKINFO_TASK
TaskHandle_t TaskInfo_StaticTask = NULL;
#endif

static TaskHandle_t CorePrintTaskHandle = NULL;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

int main(void)
{
    SYSM_InitZero();
    CorePrint_TaskInit();
    /* Infinite loop */
    xTaskCreateStatic(
        AppTask_MainTask, "MainTask", MAIN_TASK_STACK_SIZE, NULL, TASK_START_PRIO_8, Main_Stack, &Main_StaticTask);
    xTaskCreate(
        OSTimerTask_MainTask, "OSTimerTask", OS_TIMER_TASK_STACK_SIZE, (void *)NULL, TASK_START_PRIO_7, &OsTimer_Task_Handle);
#ifdef ENABLE_TASKINFO_TASK
    xTaskCreate(
        AppTask_TaskInfo, "TaskInfo", TASK_INFO_TASK_STACK_SIZE, NULL, TASK_START_PRIO_1, &TaskInfo_StaticTask);
#endif
    xTaskCreate(
        CorePrint_Task, "CorePrintTask", TASK_CORE_PRINT_TASK_STACK_SIZE, NULL, TASK_START_PRIO_3, &CorePrintTaskHandle);
    xTaskCreate(
        M4gTask_MainTask, "M4gTask", TASK_4G_TASK_STACK_SIZE, NULL, TASK_START_PRIO_4, &M4g_Task_Handle);
    xTaskCreate(
        AtTask_MainTask, "AtTask", TASK_AT_TASK_STACK_SIZE, NULL, TASK_START_PRIO_5, &AtTask_Handle);
    vTaskStartScheduler();
}
