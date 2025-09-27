//****************************************************************************************
//*
//* File Name: main.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include "main.h"
#include "stdint.h"
#include "STD_SysM.h"
#include "FlashDB_AppM.h"
#include "AppTask_MainTask.h"
#include "AppTask_TaskInfo.h"
#include "AppTask_4gTask.h"
#include "AppTask_CorePrintTask.h"
#include "MessageBuffer.h"
#include "Tcp.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define INIT_TASK_STACK_SIZE (1u * 1024u / 4u)
#define MAIN_TASK_STACK_SIZE (7u * 1024u / 4u)
#define OS_TIMER_TASK_STACK_SIZE (1u * 1024u / 4u)
#define TASK_INFO_TASK_STACK_SIZE (1u * 1024u / 4u)
#define TASK_4G_TASK_STACK_SIZE (4u * 1024u / 4u)
#define TASK_AT_TASK_STACK_SIZE (2u * 1024u / 4u)
#define TASK_CORE_PRINT_TASK_STACK_SIZE (4 * 1024u / 4u)
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

TaskHandle_t init_task_handle = NULL;

/* os timer  task handle */
TaskHandle_t OsTimer_Task_Handle = NULL;

/* 4G  task handle */
TaskHandle_t M4g_Task_Handle = NULL;
TaskHandle_t AtTask_Handle = NULL;

#ifdef ENABLE_TASKINFO_TASK
TaskHandle_t TaskInfo_StaticTask = NULL;
#endif

TaskHandle_t CorePrintTaskHandle = NULL;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
static BaseType_t init_basic_services(void)
{
    // 初始化日志服务
	LogService_SetLogEnable();
    // uart_tx_semaphore = xSemaphoreCreateBinary();
    Core_printf("[Init] Log service initialized\n");
    return pdPASS;
}

static BaseType_t init_MainTasks(void)
{
    OS_Init();
    Core_printf("[Init] Main tasks initialized\n");
    return pdPASS;
}

static BaseType_t init_middleware(void)
{
    /* fdb Init*/
    FlashDB_AppM_Init();
    /* Stream buffer Init*/
    StreamBuff_StackInit();
    /* Message buffer Init*/
    MessageBuffer_CreateInstance();
    /* TCP Init */
    tcp_init();

    Core_printf("[Init] middleware initialized\n");
    return pdPASS;
}

static BaseType_t init_application_tasks(void)
{
    at_parser_init();
    YeeCom_Init();
    CloudNetM_Init();
    Core_printf("[Init] Application tasks initialized\n");
    return pdPASS;
}

static void init_task(void *argument)
{
    Core_printf("[Init] Task started\n");

    // 初始化步骤1: 基础服务
    Core_printf("[Init] Step 1: Basic services\n");
    if (init_basic_services() != pdPASS)
    {
        Core_printf("[Init] ERROR: Basic services failed\n");
        vTaskDelete(NULL);
        return;
    }

    // 初始化步骤2: Main Tasks
    Core_printf("[Init] Step 2: Main Tasks\n");
    if (init_MainTasks() != pdPASS)
    {
        Core_printf("[Init] ERROR: Main Tasks failed\n");
        vTaskDelete(NULL);
        return;
    }
    
    // 初始化步骤3: 中间件（如FlashDB）
    Core_printf("[Init] Step 3: Middleware\n");
    if (init_middleware() != pdPASS)
    {
        Core_printf("[Init] ERROR: Middleware failed\n");
        vTaskDelete(NULL);
        return;
    }

    if (init_application_tasks() != pdPASS)
    {
        Core_printf("[Init] ERROR: Application tasks failed\n");
        vTaskDelete(NULL);
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    // 初始化步骤4: 创建应用任务
    xTaskCreateStatic(
        AppTask_MainTask, "MainTask", MAIN_TASK_STACK_SIZE, NULL, TASK_START_PRIO_8, Main_Stack, &Main_StaticTask);
    vTaskDelay(pdMS_TO_TICKS(10));

    xTaskCreate(
        OSTimerTask_MainTask, "OSTimerTask", OS_TIMER_TASK_STACK_SIZE, (void *)NULL, TASK_START_PRIO_7, &OsTimer_Task_Handle);
    vTaskDelay(pdMS_TO_TICKS(10));

    xTaskCreate(
        M4gTask_MainTask, "M4gTask", TASK_4G_TASK_STACK_SIZE, NULL, TASK_START_PRIO_4, &M4g_Task_Handle);
    vTaskDelay(pdMS_TO_TICKS(10));

    xTaskCreate(
        AtTask_MainTask, "AtTask", TASK_AT_TASK_STACK_SIZE, NULL, TASK_START_PRIO_5, &AtTask_Handle);
    vTaskDelay(pdMS_TO_TICKS(10));

#ifdef ENABLE_TASKINFO_TASK
    xTaskCreate(
        AppTask_TaskInfo, "TaskInfo", TASK_INFO_TASK_STACK_SIZE, NULL, TASK_START_PRIO_1, &TaskInfo_StaticTask);
    vTaskDelay(pdMS_TO_TICKS(10));
#endif
    
    // 标记初始化完成
    Core_printf("[Init] All initialization complete\n");
 
    // 删除初始化任务
    Core_printf("[Init] Deleting initialization task\n");
    vTaskDelete(NULL);
}

int main(void) 
{
    SYSM_InitZero();
    CorePrint_TaskInit();
    /* Infinite loop */
    xTaskCreate(
        init_task, "InitTask", INIT_TASK_STACK_SIZE, NULL, TASK_START_PRIO_9, &init_task_handle);
    xTaskCreate(
        CorePrint_Task, "CorePrintTask", TASK_CORE_PRINT_TASK_STACK_SIZE, NULL, TASK_START_PRIO_3, &CorePrintTaskHandle);
    vTaskStartScheduler();
}
