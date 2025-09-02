//******************************************************************************
//* File Name: AppTask_4gTask.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: AppTask_4gTask source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "AppTask_4gTask.h"
#include "AppTask_MainTask.h"
#include "at_parser.h"
#include "YeeComxxx.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void M4gTask_MainTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50);

	xLastWakeTime = xTaskGetTickCount();
    SYSM_printf("M4gTask creat success \r\n");
	while (1)
	{
		APPTASK_MAINTASK_RUN_START();

		APPTASK_MAINTASK_RUN_END();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
}

/**
 * @brief 启动任务
 *
 * @param pvParameters-函数入参
 *
 * @return None
 */
void AtTask_MainTask(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(33);

	xLastWakeTime = xTaskGetTickCount();
    SYSM_printf("AtTask creat success \r\n");
	YeeCom_Init();
	while (1)
	{
		APPTASK_MAINTASK_RUN_START();
		at_worker(NULL);
		APPTASK_MAINTASK_RUN_END();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
}
/* EOL */
