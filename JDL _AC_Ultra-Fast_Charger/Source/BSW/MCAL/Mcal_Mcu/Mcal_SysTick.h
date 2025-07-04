#ifndef  __MCAL_SYSTICK_H__
#define  __MCAL_SYSTICK_H__
//****************************************************************************************
//*
#include "Mcal_Types.h"

extern void SystemClock_Config(void);
extern void Mcal_SYSTICK_Counter_Increase(void);
extern McalTickCount_t Mcal_SYSTICK_Get_Counter(void);
#endif
