//******************************************************************************
//*
//* File Name: FanM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FanM.h"
#include "FanM_Drv.h"
#include "FanM_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	FANM_STATE_IDLE,		 	/* init mode*/
	FANM_STATE_OPERATE,			/* operate mode */
	FANM_STATE_SEND,			/* send mode */
	FANM_STATE_PENDING,			/* waiting mode */
	FANM_STATE_ERROR			/* error mode */
} FanM_StateType;

typedef enum
{
	FANM_CTRL_PENDING,			/* Fan gear position Pending */
	FANM_CTRL_ACCELERATE,		/* Fan gear acceleration */
	FANM_CTRL_DECELERATE,		/* Fan gear deceleration */
} FanM_CtrlCmdType;

typedef enum
{
	FANM_INIT,
	FANM_OK,
	FANM_ERR,
} FanM_RetrySentResultType;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef uint8_t (*pModbusCmdSendFunc)(void);

typedef struct
{
	uint8_t State;					/* main machine control state */
	uint8_t SubState;				/* sub machine control state */
	uint8_t FanCmd;					/* Fan control command, refer to FanM_CtrlCmdType */
	uint8_t RetryCount;				/* Retry counter */
	uint16_t WriteVol;				/* communication value of write to modbus slave */
	uint16_t CurrentRunningVol;		/* communication value of read from modbus slave */
	uint16_t Timeout;				/* Timeout counter */
	pModbusCmdSendFunc CallFunc;	/* Function pointer for Modbus command sending */
} FanM_Ctrl_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static FanM_Ctrl_Struct FanM_Ctrl;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void FanM_OileTempMonitoring(void);
static void FanM_ReadRunningAdVolPolling(void);
static void FanM_SubStateRevertPolling(void);
static void FanM_IdleStateHandle(void);
static void FanM_OperateStateHandle(void);
static void FanM_SendStateHandle(void);
static void FanM_PendingStateHandle(void);
static void FanM_ErrorStateHandle(void);
static uint8_t FanM_ReadRegister01(void);
static uint8_t FanM_WriteRegister02(void);
static FanM_RetrySentResultType FanM_RetrySendFunc(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void FanM_Init(void)
{
	FanMDrv_Init();
	memset(&FanM_Ctrl, 0, sizeof(FanM_Ctrl));
}

static void FanM_OileTempMonitoring(void)
{
	uint8_t Temp = FANM_GET_OILE_TEMP(0);

	if (Temp > FANM_SPEED_TEMP_THRESHOLD_UPPER)
	{
		FanM_Ctrl.FanCmd = FANM_CTRL_ACCELERATE;
	}
	else if (Temp < FANM_SPEED_TEMP_THRESHOLD_LOWER)
	{
		FanM_Ctrl.FanCmd = FANM_CTRL_DECELERATE;
	}
	else
	{
		FanM_Ctrl.FanCmd = FANM_CTRL_PENDING;
	}
}

static void FanM_ReadRunningAdVolPolling(void)
{
	if (FANM_GET_MODBUS_READY())
	{
		FanM_Ctrl.CurrentRunningVol = FanMDrv_GetCurrRunningVol();
	}
}

static void FanM_SubStateRevertPolling(void)
{
	static uint8_t lastState = FANM_STATE_IDLE;

	if (lastState != FanM_Ctrl.State)
	{
		lastState = FanM_Ctrl.State;
		FanM_Ctrl.SubState = FANM_STATE_IDLE;
		FanM_Ctrl.Timeout = 0U;
	}
}

static uint8_t FanM_ReadRegister01(void)
{
	FANM_DEBUG("%s time:%d \r\n", __func__, FanM_Ctrl.RetryCount);
	return (uint8_t)FanMDrv_SetAdVolCmd(MODBUS_CMD_READ_HOLDING_REGISTERS, FANM_BROADCAST_ADDR, FANM_CURRENT_RUNNING_AD_VOL_ADDR);
}

static FanM_RetrySentResultType FanM_RetrySendFunc(void)
{
	FanM_RetrySentResultType result = FANM_INIT;

	if ((++FanM_Ctrl.Timeout) > (FanM_Ctrl.RetryCount * FANM_RETRY_DELAY_COUNT_THRESHOLD))
	{
		FanM_Ctrl.Timeout = 0;
		if ((0 == FanM_Ctrl.CallFunc()) && (FanM_Ctrl.RetryCount++ < FANM_RETRY_COUNT))
		{
			FanM_Ctrl.RetryCount = 0;
			result = FANM_OK;
		}
		else if (FanM_Ctrl.RetryCount >= FANM_RETRY_COUNT)
		{
			FanM_Ctrl.RetryCount = 0;
			result = FANM_ERR;
		}
		else
		{
		}
	}
	return result;
}

static void FanM_IdleStateHandle(void)
{
	FanM_RetrySentResultType result = FANM_INIT;

	switch (FanM_Ctrl.SubState)
	{
		case FANM_STATE_IDLE:
			if (FANM_GET_MODBUS_READY())
			{
				FanM_Ctrl.SubState = FANM_STATE_OPERATE;
			}
			break;
		case FANM_STATE_OPERATE:
			FanM_Ctrl.CallFunc = &FanM_ReadRegister01;
			// FanM_Ctrl.SubState = FANM_STATE_SEND;
			// break;
		case FANM_STATE_SEND:
			result = FanM_RetrySendFunc();
			if (FANM_OK == result)
			{
				FanM_Ctrl.SubState = FANM_STATE_PENDING;
			}
			else if(FANM_ERR == result)
			{
				FanM_Ctrl.SubState = FANM_STATE_ERROR;
				FANM_ERROR("FANM_STATE_OPERATE Error!\r\n");
			}
			else
			{

			}
			break;
		case FANM_STATE_PENDING:
			if (FANM_GET_MODBUS_READY())
			{
				FanM_Ctrl.State = FANM_STATE_OPERATE;
			}
			else
			{
				if (FanM_Ctrl.Timeout++ > FANM_TIMEOUT_THRESHOLD)
				{
					FanM_Ctrl.Timeout = 0U;
					FanM_Ctrl.SubState = FANM_STATE_ERROR;
				}
			}
			break;
		case FANM_STATE_ERROR:
			FanM_Ctrl.State = FANM_STATE_ERROR;
			break;
		default:
			break;
	}
}

static uint8_t FanM_WriteRegister02(void)
{
	FANM_DEBUG("%s time:%d \r\n", __func__, FanM_Ctrl.RetryCount);
	return 	FanMDrv_SetAdVolCmd(MODBUS_CMD_WRITE_SINGLE_REGISTER, FANM_WRITING_AD_VOL_ADDR, FanM_Ctrl.WriteVol);
}

static void FanM_OperateStateHandle(void)
{
	switch (FanM_Ctrl.FanCmd)
	{
		case FANM_CTRL_ACCELERATE:
			if (FanM_Ctrl.CurrentRunningVol < FANM_AD_VOL_MINIMUM)
			{
				FanM_Ctrl.WriteVol = FANM_AD_VOL_MINIMUM;
			}
			else
			{
				FanM_Ctrl.WriteVol = FanM_Ctrl.CurrentRunningVol + FANM_ACCELERATE_STEP;
			}
			break;
		case FANM_CTRL_DECELERATE:
			if (FanM_Ctrl.CurrentRunningVol - FANM_DECELERATE_STEP > FANM_AD_VOL_MINIMUM)
			{
				FanM_Ctrl.WriteVol = FanM_Ctrl.CurrentRunningVol - FANM_DECELERATE_STEP;
			}
			else
			{
				FanM_Ctrl.WriteVol = FANM_AD_VOL_MINIMUM;
			}
			break;
		case FANM_CTRL_PENDING:
			break;
	}
	FanM_Ctrl.State = FANM_STATE_SEND;
}

static void FanM_SendStateHandle(void)
{
	FanM_RetrySentResultType result = FANM_INIT;

	FanM_Ctrl.CallFunc = &FanM_WriteRegister02;
	result = FanM_RetrySendFunc();
	if (FANM_OK == result)
	{
		FanM_Ctrl.State = FANM_STATE_PENDING;
	}
	else
	{
		FanM_Ctrl.State = FANM_STATE_ERROR;
		FANM_ERROR("%s Error!\r\n", __func__);
	}
}

static void FanM_PendingStateHandle(void)
{
	if (FANM_GET_MODBUS_READY())
	{
		FanM_Ctrl.State = FANM_STATE_IDLE;
	}
	else
	{
		if (FanM_Ctrl.Timeout++ > FANM_TIMEOUT_THRESHOLD)
		{
			FanM_Ctrl.Timeout = 0U;
			FanM_Ctrl.State = FANM_STATE_ERROR;
			FANM_ERROR("%s Error!\r\n", __func__);
		}
	}
}

static void FanM_ErrorStateHandle(void)
{
	/*
		If necessary, it is necessary to diagnose whether the baud rate is correct and whether the current communication address is correct 
	 */
}

void FanM_10msMainFunction(void)
{
	FanM_OileTempMonitoring();
	FanM_ReadRunningAdVolPolling();
	FanM_SubStateRevertPolling();
	switch (FanM_Ctrl.State)
	{
		case FANM_STATE_IDLE:
			FanM_IdleStateHandle();
			break;
		case FANM_STATE_OPERATE:
			FanM_OperateStateHandle();
			break;
		case FANM_STATE_SEND:
			FanM_SendStateHandle();
			break;
		case FANM_STATE_PENDING:
			FanM_PendingStateHandle();
			break;
		case FANM_STATE_ERROR:
			FanM_ErrorStateHandle();
			break;
		default:
			break;
	}
}

/* EOL */
