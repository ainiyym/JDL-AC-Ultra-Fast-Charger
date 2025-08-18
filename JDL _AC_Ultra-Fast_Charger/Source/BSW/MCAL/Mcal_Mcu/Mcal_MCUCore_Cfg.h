#ifndef  __MCAL_MCUCORE_CFG_H__
#define  __MCAL_MCUCORE_CFG_H__
#include "FreeRTOS.h"
#include "task.h"

#define MCAL_ENTER_CRITICAL_AREA() 	portENTER_CRITICAL()
#define MCAL_EXIT_CRITICAL_AREA() 	portEXIT_CRITICAL()

#define MCAL_ENTER_CRITICAL_AREA_ISR() 	taskENTER_CRITICAL_FROM_ISR()
#define MCAL_EXIT_CRITICAL_AREA_ISR(x) 	taskEXIT_CRITICAL_FROM_ISR(x)

#define MCAL_MALLOC(X)               pvPortMalloc(X)
#define MCAL_FREE(X)                 vPortFree(X)

#endif
