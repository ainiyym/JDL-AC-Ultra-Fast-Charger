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

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

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
#endif /* APPTASK_4GTASK_H */
/* EOL */
