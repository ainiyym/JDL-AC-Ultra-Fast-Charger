/*
 * STD_Os_Timer.h
 *
 *  Created on: 2025-6-16
 *      Author: JDLzhou
 */

#ifndef STD_OS_TIMER_H_
#define STD_OS_TIMER_H_

#include "Os.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/* 配置软定时器最大通道数 */
#define OS_SOFTTIMER_MAX_CH_NUM      (4u)
#define OS_SOFTTIMER_COUNTING_UNIT	 (25u)	/* 软定时器计数单位 :25us */

#define OS_SOFTTIMER_ENTER_CRITICAL_AREA()
#define OS_SOFTTIMER_EXIT_CRITICAL_AREA()

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

typedef unsigned int OS_SoftTimerChannelCtrlBits_t; /* 通道大于32时扩展备用 */
typedef void (*OS_SoftTimerCBFunc)(void);

typedef struct
{
	OS_SoftTimerChannelCtrlBits_t SoftTimerChannelStatus; /* 位控，位序对应Channel索引序。位值 1：表示启用；0：表示未启用。最大32个通道 */
	OS_SoftTimerChannelCtrlBits_t SoftTimerChannelMode;	  /* 位控，位序对应Channel索引序。位值 0：表示定时一次；1：表示循环定时。最大32个通道 */
	uint32_t SoftTimerChannelCounter[OS_SOFTTIMER_MAX_CH_NUM];
	uint32_t SoftTimerChannelPeriod[OS_SOFTTIMER_MAX_CH_NUM];
	bool SoftTimerChannelTaskStatus[OS_SOFTTIMER_MAX_CH_NUM];
	OS_SoftTimerCBFunc CB_Func[OS_SOFTTIMER_MAX_CH_NUM];
} OS_SoftTimerCtrl_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern OS_ErrorType_t OS_SoftTimerStart(uint8_t *Channel, OS_SoftTimerCBFunc CBFunc, uint32_t Period, uint8_t Mode);
extern void OS_SoftTimerStop(uint8_t Channel);
extern uint8_t OS_GetSoftTimerChannelStatus(uint8_t Channel);
extern void OS_SoftTimerSoftTimerINT_CB(void);
extern void OS_TimerTask(void);
#endif /* STD_OS_TIMER_H_ */
