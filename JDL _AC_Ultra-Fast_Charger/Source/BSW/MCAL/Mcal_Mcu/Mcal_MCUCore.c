/*
 * Mcal_MCUCore.c
 * MCU Core Module Source File
 */

#include "Mcal_MCUCore.h"

/* Private macros -----------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Public functions ---------------------------------------------------------*/
void Mcal_MCUCore_Init(void)
{
    /* Initialize MCU core related hardware and settings */
}

void Mcal_MCUCore_DeInit(void)
{
    /* De-initialize MCU core related hardware and settings */
}

void Mcal_McuCheck_Rst(void)
{
    LOG_TRACE(LOG_MODULE_MCAL, " CSR = %x\r\n", RCC->CSR);
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET) // NRST Pin reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "PIN reset \r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET) // Power on reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "POR/PDR reset \r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET) //  Software reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "Software reset  \r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) // Independent watchdog reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "Independent watchdog reset \r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET) // Window watchdog reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "Window watchdog reset \r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) != RESET) // Low-power reset
    {
        LOG_TRACE(LOG_MODULE_MCAL, "(Low-power reset \r\n");
    }
    __HAL_RCC_CLEAR_RESET_FLAGS(); // Clear reset flag
}

// the system is reset, all circuits will be reset
void Mcal_MCU_SysRestart(void)
{
    __set_FAULTMASK(1);	 // disable all interrupts
    HAL_NVIC_SystemReset();
}
/* Add more MCU core related functions below */

#define CRITICAL_PRIO 0x40
 
void vPortEnterCritical(void) {

}

void vPortExitCritical(void) {

}
