//****************************************************************************************
//*
//* File Name: ScheduleTable.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//*
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "ScheduleTable.h"
#include "Mcal_App.h"
#include "Mcal_Test.h"
#include "Comm.h"
#include "SwitchM.h"
#include "STD_EvseM.h"
#include "CpM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"
#include "STD_AuthM.h"
#include "STD_Curr.h"
#include "STD_Volt.h"
#include "BtrM.h"
#include "ModbusM.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// 调度任务结构体
typedef struct
{
    uint32_t tasktick;  // Task cycle, unit: ms
    void (*task)(void); // Task function pointer
} SchedTask;

// 调度任务数据结构体
typedef struct
{
    uint8_t running;   // Task status 1: Ready state 0: Not ready
    uint32_t timer;    // Task timer
} SchedTaskData;

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
// Task function prototype declaration
static void Task1ms(void);
static void Task5ms(void);
static void Task10ms(void);
static void Task20ms(void);
static void Task100ms(void);
static void Task1000ms(void);

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
// Scheduling table configuration
static SchedTask schedTable[] =
    {
        {1, Task1ms},      // 1ms Periodic task
        {5, Task5ms},      // 5ms Periodic task
        {10, Task10ms},    // 10ms Periodic task
        {20, Task20ms},    // 20ms Periodic task
        {100, Task100ms},  // 100ms Periodic task
        {1000, Task1000ms} // 1000ms Periodic task
};

#define TASKS_MAX (sizeof(schedTable) / sizeof(SchedTask)) // Number of tasks

SchedTaskData SchedulerTaskData[TASKS_MAX]; // Task array
/*******************************************************************************
|    static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|	 Global Function Prototypes
|******************************************************************************/
/******************************************************************************/

// Interrupt callback function
void Scheduler_ISRCb(void)
{
    uint8_t i = 0;

    for (i = 0; i < TASKS_MAX; i++)
    {
        if (SchedulerTaskData[i].timer > 0)
        {
            SchedulerTaskData[i].timer--; // Subtract 1 from the task timer
            if (SchedulerTaskData[i].timer == 0)
            {
                SchedulerTaskData[i].running = 1; // Set task to ready state
            }
        }
    }
}

// task initialization function
void Scheduler_Init(void)
{
    uint8_t i = 0;
    for (i = 0; i < TASKS_MAX; i++)
    {
        SchedulerTaskData[i].timer = schedTable[i].tasktick;
        SchedulerTaskData[i].running = 0; // The initialization task status is not ready
    }
}

// task running function
void Scheduler_run(void)
{
    uint8_t i = 0;

    for (i = 0; i < TASKS_MAX; i++)
    {
        if (SchedulerTaskData[i].running == 1)
        {
            SchedulerTaskData[i].timer = schedTable[i].tasktick; // Reset the initial value to the current task timer
            (schedTable[i].task)(); // Call the task function

            SchedulerTaskData[i].running = 0; // Reset the task status to 0
        }
    }
}

/*******************************************************************************
|    Static Local Functions Definition
*******************************************************************************/
static void Task1ms(void) 
{
    /* 1ms task code */
    Comm_GetCcpPrd();
    // Mcal_test_1ms();
}
	
static void Task5ms(void)
{
    /* 5ms task code */
    Mcal_USARTIf_Send_MainFunction(); // Call the main function for serial port transmission
    CPM_5msMainFunction();
    RELAYM_5msMainFunction();
}

static void Task10ms(void)
{
    SYSM_10msMainFunction();
    ModbusM_10msMainFunction();
    /* 10ms task code */
#if (MCAL_WDG_ENABLED)
    Mcal_Iwdg_Feedback();
#endif
    EVSEM_10msMainFunction(); // Call the EVSE manager's 10ms main function
    // ERRHDL_10msMainFunction();  // Call the error handler's 10ms main function
    AUTHM_10msMainFunction(); // Call the authorization manager's 10ms main function
    BTRM_10msMainFunction(); // Call the battery manager's 10ms main function
    CanM_Rte_10ms_Task();   // Call the can manager's 10ms main function
}

static void Task20ms(void)
{
    /* 20ms task code */
    CURR_20msFunctionControl();
    VOLT_20msFunctionControl();
}

static void Task100ms(void)
{
    /* 100ms task code */
    // Mcal_Can_Send_Test(); // Call the CAN send test function
    NOAUTHEN_100msFunction();
    CanM_Rte_Msg_Main_Task(); // Call canM task
}

static void Task1000ms(void)
{
    /* 1-second task code */
    SYSM_RunningLedHandle();
}
