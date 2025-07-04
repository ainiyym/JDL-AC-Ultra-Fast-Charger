#ifndef  MCAL_CORE_H
#define  MCAL_CORE_H
#include"Mcal_MCUCore_Cfg.h"
#include "stm32f1xx_hal.h"  // Include the HAL library for STM32F1xx
#include "STD_Logservice.h"

extern void Mcal_McuCheck_Rst(void);
extern void Mcal_MCU_SysRestart(void);
#endif

