/*
 * OS.c
 * Basic Software - OS Schedule Table
 */

#include "OS.h"
#include "ScheduleTable.h"
#include "STD_Os_Timer.h"

/* Function prototypes */

/* OS Initialization */
void OS_Init(void)
{
    SYSM_InitOne();

    SYSM_InitTwo();

    SYSM_InitThree();

    /* Initialize the scheduler */
    Scheduler_Init();
}

/* OS Schedule Table Handler */
void OS_Schedule(void)
{
    Scheduler_run(); // Call the scheduler to run tasks
}
