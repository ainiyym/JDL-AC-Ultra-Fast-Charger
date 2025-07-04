/*
 * STD_Os_Timer.c
 *
 *  Created on: 2025-6-16
 *      Author: JDLzhou
 */

#include "STD_Types.h"
#include "STD_Os_Timer.h"

static OS_SoftTimerCtrl_t OS_SoftTimerCtrl = {0};

OS_ErrorType_t OS_SoftTimerStart(uint8_t *Channel, OS_SoftTimerCBFunc CBFunc, uint32_t Period, uint8_t Mode)
{
	OS_ErrorType_t RetStatus = OS_E_NO_ERROR;
	uint8_t RetFlag = STD_FALSE;
	uint8_t Ch;
	uint32_t i;

	OS_SOFTTIMER_ENTER_CRITICAL_AREA();

	for (i = 0; i < OS_SOFTTIMER_MAX_CH_NUM; i++)
	{
		if (0 == (OS_SoftTimerCtrl.SoftTimerChannelStatus & (OS_SoftTimerChannelCtrlBits_t)(1 << i)))
		{
			break;
		}
	}

	if (OS_SOFTTIMER_MAX_CH_NUM == i)
	{
		RetStatus = OS_E_NO_FREE_SOFTIRMER;
		RetFlag = STD_TRUE;
	}
	else
	{
		Ch = i;
	}

	if (STD_FALSE == RetFlag)
	{
		if (NULL != CBFunc)
		{
			OS_SoftTimerCtrl.CB_Func[Ch] = CBFunc;
		}
		else
		{
			RetStatus = OS_E_INVALID_PAR;
			RetFlag = STD_TRUE;
		}
	}

	if (STD_FALSE == RetFlag)
	{
		if (0 != Period)
		{
			OS_SoftTimerCtrl.SoftTimerChannelPeriod[Ch] = Period;
			OS_SoftTimerCtrl.SoftTimerChannelCounter[Ch] = Period;
		}
		else
		{
			OS_SoftTimerCtrl.CB_Func[Ch] = NULL;
			RetStatus = OS_E_INVALID_PAR;
			RetFlag = STD_TRUE;
		}
	}

	if (STD_FALSE == RetFlag)
	{
		if (Mode)
		{
			OS_SoftTimerCtrl.SoftTimerChannelMode |= (OS_SoftTimerChannelCtrlBits_t)(1 << Ch);
		}
		else
		{
			OS_SoftTimerCtrl.SoftTimerChannelMode &= ~(OS_SoftTimerChannelCtrlBits_t)(1 << Ch);
		}

		OS_SoftTimerCtrl.SoftTimerChannelStatus |= (OS_SoftTimerChannelCtrlBits_t)(1 << Ch);
		*Channel = Ch;
	}

	OS_SOFTTIMER_EXIT_CRITICAL_AREA();

	return RetStatus;
}

void OS_SoftTimerStop(uint8_t Channel)
{
	if (Channel < OS_SOFTTIMER_MAX_CH_NUM)
	{
		OS_SOFTTIMER_ENTER_CRITICAL_AREA();
		OS_SoftTimerCtrl.SoftTimerChannelStatus &= ~(OS_SoftTimerChannelCtrlBits_t)(1 << Channel);
		OS_SoftTimerCtrl.CB_Func[Channel] = NULL;
		OS_SOFTTIMER_EXIT_CRITICAL_AREA();
	}
	else
	{
		;
	}
}

uint8_t OS_GetSoftTimerChannelStatus(uint8_t Channel)
{
	uint8_t RetStatus;

	OS_SOFTTIMER_ENTER_CRITICAL_AREA();

	if (Channel < OS_SOFTTIMER_MAX_CH_NUM)
	{
		if (OS_SoftTimerCtrl.SoftTimerChannelStatus & (OS_SoftTimerChannelCtrlBits_t)(1 << Channel))
		{
			RetStatus = STD_TRUE;
		}
		else
		{
			RetStatus = STD_FALSE;
		}
	}
	else
	{
		RetStatus = STD_FALSE;
	}

	OS_SOFTTIMER_EXIT_CRITICAL_AREA();

	return RetStatus;
}

void OS_SoftTimerSoftTimerINT_CB(void)
{
	for (uint32_t i = 0; i < OS_SOFTTIMER_MAX_CH_NUM; i++)
	{
		if (OS_SoftTimerCtrl.SoftTimerChannelStatus & (OS_SoftTimerChannelCtrlBits_t)(1 << i))
		{
			if (0 != OS_SoftTimerCtrl.SoftTimerChannelCounter[i])
			{
				OS_SoftTimerCtrl.SoftTimerChannelCounter[i]--;
			}
			else
			{
				if (NULL != OS_SoftTimerCtrl.CB_Func[i])
				{
					OS_SoftTimerCtrl.SoftTimerChannelTaskStatus[i] = 1;
				}
			}
		}
	}
}

void OS_TimerTask(void)
{
	uint8_t i = 0;

	for (i = 0; i < OS_SOFTTIMER_MAX_CH_NUM; i++)
	{
		if (OS_SoftTimerCtrl.SoftTimerChannelTaskStatus[i] == 1)
		{
			/* Reset the initial value to the current task timer */
			if (OS_SoftTimerCtrl.SoftTimerChannelMode & (OS_SoftTimerChannelCtrlBits_t)(1 << i))
			{
				OS_SoftTimerCtrl.SoftTimerChannelCounter[i] = OS_SoftTimerCtrl.SoftTimerChannelPeriod[i];
			}
			else
			{
				OS_SoftTimerCtrl.SoftTimerChannelStatus &= ~(OS_SoftTimerChannelCtrlBits_t)(1 << i);
				OS_SoftTimerCtrl.CB_Func[i] = NULL;
			}
			/* Call the task function */
			OS_SoftTimerCtrl.CB_Func[i]();
			/* Reset the task status to 0 */
			OS_SoftTimerCtrl.SoftTimerChannelTaskStatus[i] = 0;
		}
	}
}
