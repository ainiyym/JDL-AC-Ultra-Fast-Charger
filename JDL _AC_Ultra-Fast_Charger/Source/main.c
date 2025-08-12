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

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MAIN_TASK_STACK_SIZE (5u * 1024u / 4u)
#define OS_TIMER_TASK_STACK_SIZE (1u * 1024u / 4u)
// #define ENABLE_TASKINFO_TASK
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

#ifdef ENABLE_TASKINFO_TASK
static StackType_t  TaskInfo_Stack[MAIN_TASK_STACK_SIZE];
static StaticTask_t TaskInfo_StaticTask;
#endif

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
    /* Infinite loop */
    xTaskCreateStatic(
        AppTask_MainTask, "MainTask", MAIN_TASK_STACK_SIZE, NULL, TASK_START_PRIO_3, Main_Stack, &Main_StaticTask);
    xTaskCreate(
        OSTimerTask_MainTask, "OSTimerTask", OS_TIMER_TASK_STACK_SIZE, (void *)NULL, TASK_START_PRIO_2, &OsTimer_Task_Handle);
#ifdef ENABLE_TASKINFO_TASK
    xTaskCreateStatic(
        AppTask_TaskInfo, "TaskInfo", MAIN_TASK_STACK_SIZE, NULL, TASK_START_PRIO_1, TaskInfo_Stack, &TaskInfo_StaticTask);
#endif
	  vTaskStartScheduler();
}
