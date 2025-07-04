/*
 * OS.c
 * Basic Software - OS Schedule Table
 */

#include "OS.h"
#include "ScheduleTable.h"
#include "STD_Os_Timer.h"

/* Function prototypes */
static void OS_Init(void);
static void OS_SysInitStep1(void);
static void Os_AppInit(void);
static void OS_AppEnableStatusCfg(void);
static void OS_Schedule(void);

/* OS Initialization */
static void OS_Init(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize the SysTick */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /* Set the SysTick priority */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /* Sys init step 1*/
    OS_SysInitStep1();

    /* Initialize application memory */
    Os_AppInit();

    /* Initialize the application memory */
    OS_AppEnableStatusCfg(); // Initialize the application system

    /* Initialize the scheduler */
    Scheduler_Init(); // Initialize the scheduler
}

static void SYSM_ShowUserInfo(void)
{
	LOG_TRACE(LOG_MODULE_SYSM, "(COM APP)Built@ " __DATE__" " __TIME__ "\n");
}

static void OS_SysInitStep1(void)
{
    /* STD_Library Init*/
    CycBuf_InitMemory();
    CycBuf_Init();

    FIFO_InitMemory();

    LogService_SetLogEnable(); // Enable logging service 
    
    McalUsart_CfgInit(); // Initialize USART configuration

    Mcal_Can_RxBufCfg_Init(); // Initialize CAN configuration

	/* Initialize the peripherals */
    MCALAPP_PeripheralInit();
	
    SYSM_ShowUserInfo(); //print user info

    Mcal_McuCheck_Rst(); //print reset source
}

/* APP Initialization */
static void OS_AppEnableStatusCfg(void)
{
    Mcal_Usart_Enable();
    Mcal_Can_Enable();
    SwitchM_SoftTimerStart74hct4851d_Enable();
    Mcal_GpTime_AdcCollection_Start();
    Mcal_Adc_SoftTimer_Enable();
    ERRHDL_Enable();
    CPM_Enable();
}

static void Os_AppInit(void)
{
    /* Initialize application memory */
    ERRHDL_InitMemory(); //  Initialize ERRHDL
    Comm_Init();         // Initialize communication module
    SwitchM_Init();      //  Initialize switch
    EVSEM_InitMemory();  // Initialize EVSE memory
    CPM_InitMemory();    //  Initialize CP
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
        // Optionally add sleep or wait for interrupt
    }
}
