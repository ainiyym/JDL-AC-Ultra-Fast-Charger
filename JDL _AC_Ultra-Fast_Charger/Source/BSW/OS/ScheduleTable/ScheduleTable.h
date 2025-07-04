#ifndef SCHEDULE_TABLE_H
#define SCHEDULE_TABLE_H

// 头文件包含
#include <stdint.h>
#include "Mcal_SysTick.h"
#include "Mcal_Test.h"
#include "Mcal_Usart_Cfg.h"
#include "STD_CycBuf.h"
#include "STD_Fifo.h"
#include "Mcal_iwdg_Cfg.h"
#include "STD_LogService.h"
#include "Comm.h"
#include "SwitchM.h"
#include "STD_EvseM.h"
#include "CpM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"

extern void Scheduler_ISRCb(void);
extern void Scheduler_Init(void);   
extern void Scheduler_run(void);
#endif // SCHEDULE_TABLE_H
