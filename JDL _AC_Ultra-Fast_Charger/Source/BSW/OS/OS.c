/*
 * OS.c
 * Basic Software - OS Schedule Table
 */

#include "OS.h"
#include "ScheduleTable.h"
#include "STD_Os_Timer.h"

/* Function prototypes */
static void OS_Init(void);
static void OS_Schedule(void);

/* OS Initialization */
static void OS_Init(void)
{
    SYSM_InitZero();

    SYSM_InitOne();

    SYSM_InitTwo();

    SYSM_InitThree();

    /* Initialize the scheduler */
    Scheduler_Init();
}

/* OS Schedule Table Handler */
static void OS_Schedule(void)
{
    Scheduler_run(); // Call the scheduler to run tasks
}

/* Public API */
void OS_Start(void)
{
    OS_Init();
    while (1)
    {
        OS_Schedule();
        OS_TimerTask();
    }
}
