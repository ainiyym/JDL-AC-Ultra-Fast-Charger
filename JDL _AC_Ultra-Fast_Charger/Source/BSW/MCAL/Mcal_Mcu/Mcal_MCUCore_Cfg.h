#ifndef  __MCAL_MCUCORE_CFG_H__
#define  __MCAL_MCUCORE_CFG_H__
#include "FreeRTOS.h"
#include "task.h"

#define MCAL_ENTER_CRITICAL_AREA() 	vPortEnterCritical()
#define MCAL_EXIT_CRITICAL_AREA() 	vPortExitCritical()

#endif
