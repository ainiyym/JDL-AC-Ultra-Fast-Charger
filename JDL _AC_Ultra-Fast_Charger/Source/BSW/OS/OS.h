#ifndef OS_H
#define OS_H

#include "STD_SysM.h"
#include "Mcal_SysTick.h"
#include "stm32f1xx_hal.h"

typedef enum 
{
	OS_E_NO_ERROR = (uint8_t)(0u),
	OS_E_NULL_POINTER,
	OS_E_INVALID_PAR,	
	OS_E_PAR_OUT_OF_RANGE,
	OS_E_NO_FREE_SOFTIRMER,	
	OS_E_SOFTIRMER_STARTED,		
	OS_E_UNKNOWN,	
	OS_E_MAX_NUM
}OS_ErrorType_t;

// 调度表相关API声明
extern void OS_Init(void);
extern void OS_Schedule(void);

#endif // OS_H
