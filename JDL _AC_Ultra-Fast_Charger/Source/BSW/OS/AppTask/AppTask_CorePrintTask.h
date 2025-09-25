//******************************************************************************
//* File Name: AppTask_CorePrintTask.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: AppTask_4gTask header file
/*******************************************************************************/
#if !defined (APPTASK_CORE_PRINT_TASK_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define APPTASK_CORE_PRINT_TASK_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
// 配置参数
#define PRINT_QUEUE_LENGTH         64
#define PRINT_MAX_MESSAGE_SIZE     256
#define PRINT_BATCH_SIZE           4
#define PRINT_BUFFER_TIMEOUT_MS    20

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    char *message;
    uint16_t length;
} print_item_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void CorePrint_TaskInit(void);
extern void CorePrint_Task(void *pvParameters);
extern void AppPrint_Task(void *pvParameters);
extern int Core_printf(const char *format, ...);
extern uint8_t CorePrint_IsEmpty(void);
extern uint16_t Core_Printf_AddItem(const char *message);
extern int Core_Print_Immediate(const char *format, ...);
extern void Core_Print_Hex(const uint8_t *data, size_t length);
#endif /* APPTASK_4GTASK_H */
/* EOL */
