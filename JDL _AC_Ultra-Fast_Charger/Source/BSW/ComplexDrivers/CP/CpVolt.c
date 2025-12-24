//****************************************************************************************
//*
//* File Name: CpVolt.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
/* polyspace:begin<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "CpVolt.h"
#include "STD_Lib.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
	CPV_ERR_REC = 0u, /*CP error recover*/
	CPV_ERR_INPUT,	  /*CP error inout*/
	CPV_ERR_WAITING,  /*CP error waiting*/
};

typedef enum
{
	CPV_ERR_VOLT_12V = 0,  /* CP 12 voltage enum */
	CPV_ERR_VOLT_9V,	   /* CP 9 voltage enum */
	CPV_ERR_VOLT_6V,	   /* CP 6 voltage enum */
	CPV_ERR_VOLT_4V,	   /* CP 4 voltage enum */
	CPV_ERR_VOLT_3V,	   /* CP 3 voltage enum */
	CPV_ERR_VOLT_2V,	   /* CP 2 voltage enum */
	CPV_ERR_VOLT_MAX	   /* CP max numer of error voltage */
} CPVErrVolt_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucEnStatus;						/*enable cp module status*/
	uint8_t ucEvseThreeDot;					/*Evse ThreeDot Status*/
#if (STD_ON == CPV_BRK_LIN_FUN_EN)
	uint8_t ucBrkLinErr;			 		/*break line error*/
	uint8_t ucCpBreakLineFlag;		 		/*break line flag*/
	uint16_t usBrkLinRestoreWaitCnt; 		/*break line error recover wait cnt*/
	uint16_t usBrkLinWaitCnt;		 		/*break line input error wait cnt*/
#endif
	uint16_t usValidAdValue;				 /*cp vaild ad value*/
	uint16_t usVoltValue;					 /*cp vaild filter value*/
	uint16_t usAdData[CPV_ADC_FIFO_MAX_NUM]; /*cp cache ad value*/
	LibFilterStruct stCpVolt;				 /*cp voltge status filter*/
	LibFilterStruct stCpVoltError;			 /*cp error voltage error*/
#if (STD_ON == CPV_BRK_LIN_FUN_EN)
	LibFilterStruct stCpBreakLine; 			/*cp breakline error*/
#endif
} CpVolt_Struct;

typedef struct 
{
	uint8_t ucErrCnt;					/*cp voltage error cnt*/
	uint8_t ucRevocerErrCnt;			/*cp voltage recover err cnt*/
	uint8_t ucErrState;					/*cp voltage error status*/
}CpVoltErr_Struct;

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */
static CpVolt_Struct gv_stCpVolt[SYS_CONNECTOR_NUM_MAX];
static CpVoltErr_Struct gv_stVoltageErr[SYS_CONNECTOR_NUM_MAX][CPV_ERR_VOLT_MAX];
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint8_t CPV_AdcFillter(const uint16_t *plv_usData, uint16_t *lv_usValidAdValue);
static void CPV_ClearVoltErr(SysConnector_Num_Enum ch);
static void CPV_VoltErrTypeHandle(SysConnector_Num_Enum ch, CPVErrVolt_Enum enErrId, uint8_t lv_ucErrType);
static uint8_t CPV_GetErrStatus(SysConnector_Num_Enum ch);
static void CPV_CpVoltErrHandle(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg);
static void CPV_AdcCvrtToVolt(SysConnector_Num_Enum ch, uint16_t lv_usAdcValue);
#if (STD_ON == CPV_BRK_LIN_FUN_EN)
static void CPV_BreakLineHandle(SysConnector_Num_Enum ch);
#endif
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : CPV_InitMemory
Syntax          : void CPV_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPV Module Initialize RAM
Call By         :
|******************************************************************************/
void CPV_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)(&gv_stCpVolt), 0u, (uint16_t)(sizeof(gv_stCpVolt) / sizeof(uint8_t)));
}

/*******************************************************************************
Name            : CPV_Open
Syntax          : void CPV_Open(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPV Module Enable
Call By         :
|******************************************************************************/
void CPV_Open(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCpVolt[ch].ucEnStatus = STD_TRUE;
	}
	CPDRV_StartAdcCollection();
}

/*******************************************************************************
Name            : CPV_Close
Syntax          : void CPV_Close(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPV Module Disable
Call By         :
|******************************************************************************/
void CPV_Close(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stCpVolt[ch].ucEnStatus = STD_FALSE;
	}
	CPDRV_StopAdcCollection();
}

/*******************************************************************************
Name            : CPV_GetCpVoltStatus
Syntax          : uint8_t CPV_GetCpVoltStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get CP EVSE Status
Call By         :
|******************************************************************************/
uint8_t CPV_GetCpVoltStatus(SysConnector_Num_Enum ch)
{
	return gv_stCpVolt[ch].stCpVolt.ucValidStatus;
}

/*******************************************************************************
Name            : CPV_GetCpVoltage
Syntax          : uint16_t CPV_GetCpVoltage(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get CP Voltage
Call By         :
|******************************************************************************/
uint16_t CPV_GetCpVoltage(SysConnector_Num_Enum ch)
{
	return gv_stCpVolt[ch].usVoltValue;
}

/*******************************************************************************
Name            : CPV_MainFunction
Syntax          : void CPV_MainFunction(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CPV Main Function
Call By         :
|******************************************************************************/
void CPV_MainFunction(SysConnector_Num_Enum ch)
{
#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
	if ((STD_TRUE == gv_stCpVolt[ch].ucEnStatus) && (STD_FALSE == CPD_DiodeDetectFlag()))
#else
	if (STD_TRUE == gv_stCpVolt[ch].ucEnStatus)
#endif
	{
		if (STD_TRUE == FIFO_GetData(CPDRV_ConnectorCfgTable[ch].CpAdVolFifoCh, gv_stCpVolt[ch].usAdData))
		{
			// CP_DEBUG("CPV_MainFunction:ch:%d,usAdData[0]:%d,usAdData[1]:%d,usAdData[2]:%d,usAdData[3]:%d\r\n", ch,
			// 	gv_stCpVolt[ch].usAdData[0], gv_stCpVolt[ch].usAdData[1], gv_stCpVolt[ch].usAdData[2], gv_stCpVolt[ch].usAdData[3]);
			if (STD_TRUE == CPV_AdcFillter(gv_stCpVolt[ch].usAdData, &gv_stCpVolt[ch].usValidAdValue))
			{
				// CP_DEBUG("CPV_MainFunction:ch:%d,usValidAdValue:%d\r\n", ch, gv_stCpVolt[ch].usValidAdValue);
				(void)CPV_AdcCvrtToVolt(ch, gv_stCpVolt[ch].usValidAdValue);
				CPV_CpVoltErrHandle(ch, gv_stCpVolt[ch].usVoltValue);
#if (STD_ON == CPV_BRK_LIN_FUN_EN)
				CPV_BreakLineHandle(ch);
#endif
			}
			else
			{
			}
		} 
		else
		{
		}

		if (gv_stCpVolt[ch].stCpVoltError.ucValidStatus == STD_TRUE)
		{
			CpDrvif_SetFaultStatus(ERRHDL_ID_CP_VOLT, STD_TRUE);
		}
		else
		{
			CpDrvif_SetFaultStatus(ERRHDL_ID_CP_VOLT, STD_FALSE);
		}
	}
	else
	{
	}
}

/*******************************************************************************
Name            : CPV_AdcFillter
Syntax          : static uint8_t CPV_AdcFillter(const uint16_t *plv_usData, uint16_t * lv_usValidAdValue)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : plv_usData                :-
Parameters(in)  : lv_usValidAdValue         :-
Parameters(out) : None                      :-
Return value    : lv_ucReVal                :-
Description     : Adc Fillter
Call By         :
|******************************************************************************/
static uint8_t CPV_AdcFillter(const uint16_t *plv_usData, uint16_t *lv_usValidAdValue)
{
	uint16_t max1 = 0, max2 = 0;
	uint16_t min1 = plv_usData[0], min2 = plv_usData[0];
	uint32_t sum = 0;
	uint8_t validCount = 0;

	// 找出最大的两个元素
	for (uint8_t i = 0; i < CPV_ADC_FIFO_MAX_NUM; i++)
	{
		if (plv_usData[i] != 0)
		{
			if (plv_usData[i] > max1)
			{
				max2 = max1;
				max1 = plv_usData[i];
			}
			else if (plv_usData[i] > max2)
			{
				max2 = plv_usData[i];
			}

			// 找出最小的两个元素
			if (plv_usData[i] < min1)
			{
				min2 = min1;
				min1 = plv_usData[i];
			}
			else if (plv_usData[i] < min2)
			{
				min2 = plv_usData[i];
			}
			validCount++;
			sum += plv_usData[i];
		}
	}
	sum -= (max1 + max2 + min1 + min2);

	if (validCount > 4)
	{
		*lv_usValidAdValue = sum / (validCount - 4);

		return STD_TRUE;
	}

	return STD_FALSE;
}

/*******************************************************************************
Name            : CPV_ClearVoltErr
Syntax          : static void CPV_ClearVoltErr(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP clear error
Call By         :
|******************************************************************************/
static void CPV_ClearVoltErr(SysConnector_Num_Enum ch)
{
	for (uint8_t lv_ucIndex = 0u; lv_ucIndex < (uint8_t)CPV_ERR_VOLT_MAX; lv_ucIndex++)
	{
		LIB_SetMemory((uint8_t *)(&gv_stVoltageErr[ch][lv_ucIndex]), 0u, (uint16_t)(sizeof(gv_stVoltageErr[ch][lv_ucIndex]) / sizeof(uint8_t)));
	}
}

/*******************************************************************************
Name            : CPV_VoltErrTypeHandle
Syntax          : static void CPV_VoltErrTypeHandle(SysConnector_Num_Enum ch, CPVErrVolt_Enum enErrId, uint8_t lv_ucErrType)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : enErrId                   :-
Parameters(in)  : lv_ucErrType              :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP Error type handle
Call By         :
|******************************************************************************/
static void CPV_VoltErrTypeHandle(SysConnector_Num_Enum ch, CPVErrVolt_Enum enErrId, uint8_t lv_ucErrType)
{
	if (CPV_GET_CP_SWITCH_4V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		for (uint8_t lv_ucIndex = CPV_ERR_VOLT_4V; lv_ucIndex < (uint8_t)CPV_ERR_VOLT_MAX; lv_ucIndex++)
		{
			if (lv_ucErrType == (uint8_t)CPV_ERR_REC)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
				}
			}
			else if (lv_ucErrType == (uint8_t)CPV_ERR_INPUT)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrState = STD_FALSE;
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrState = STD_TRUE;
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0;
				}
			}
			else if (lv_ucErrType == (uint8_t)CPV_ERR_WAITING)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
			}
			else
			{
				// none
			}
		}
	}
	else if (CPV_GET_CP_SWITCH_12V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		for (uint8_t lv_ucIndex = CPV_ERR_VOLT_12V; lv_ucIndex < (uint8_t)CPV_ERR_VOLT_4V; lv_ucIndex++)
		{
			if (lv_ucErrType == (uint8_t)CPV_ERR_REC)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
				}
			}
			else if (lv_ucErrType == (uint8_t)CPV_ERR_INPUT)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrState = STD_FALSE;
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrState = STD_TRUE;
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0;
				}
			}
			else if (lv_ucErrType == (uint8_t)CPV_ERR_WAITING)
			{
				if (lv_ucIndex != (uint8_t)enErrId)
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucErrCnt = 0u;
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
				else
				{
					gv_stVoltageErr[ch][lv_ucIndex].ucRevocerErrCnt = 0u;
				}
			}
			else
			{
				// none
			}
		}
	}
	else
	{
		// none
	}
}

/*******************************************************************************
Name            : CPV_GetErrStatus
Syntax          : static uint8_t CPV_GetErrStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : lv_ucRtn                  :-
Description     : CP Err check
Call By         :
|******************************************************************************/
static uint8_t CPV_GetErrStatus(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucRtn = STD_FALSE;
	for (uint8_t lv_ucIndex = 0u; lv_ucIndex < (uint8_t)CPV_ERR_VOLT_MAX; lv_ucIndex++)
	{
		if (STD_TRUE == gv_stVoltageErr[ch][lv_ucIndex].ucErrState)
		{
			lv_ucRtn = STD_TRUE;
			// CP_DEBUG("ch:%d CPV_GetErrStatus:CPV_ERR_VOLT_%d usValidAdValue: %d\r\n", ch, lv_ucIndex, gv_stCpVolt[ch].usValidAdValue);
			break;
		}
	}
	return lv_ucRtn;
}

/*******************************************************************************
Name            : CPV_12VCpVoltageFilter
Syntax          : static uint8_t CPV_12VCpVoltageFilter(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : lv_ucRtn                  :-
Description     : CP 12V filtering
Call By         :
|******************************************************************************/
static uint8_t CPV_12VCpVoltageFilter(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg)
{
	static uint8_t lv_ucRtnVolt[SYS_CONNECTOR_NUM_MAX] = {CPV_VOLT_ZERO};

	if (lv_usVoltAvrg > CPV_VOLTAGE_12P8)
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_12V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucErrCnt >= CPV_VOLTAGE_12V_4V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_12V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_12V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_11P2) && (lv_usVoltAvrg <= CPV_VOLTAGE_12P8))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_12V, (uint8_t)CPV_ERR_REC);
		lv_ucRtnVolt[ch] = CPV_VOLT_TWF;
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg >= CPV_VOLTAGE_11P2_RECOVER)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_12V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_12V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_TWF;
				}
			}
		}
	}
	else if ((lv_usVoltAvrg > CPV_VOLTAGE_9P8) && (lv_usVoltAvrg < CPV_VOLTAGE_11P2))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_9V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucErrCnt++;
			CP_DEBUG("ch :%d CPV_VoltErrTypeHandle CPV_ERR_VOLT_9V ucErrCnt:%d lv_usVoltAvrg:%d \r\n", ch, gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucErrCnt, lv_usVoltAvrg);
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucErrCnt >= CPV_VOLTAGE_9V_3V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_9V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_9V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_8P2) && (lv_usVoltAvrg <= CPV_VOLTAGE_9P8))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_9V, (uint8_t)CPV_ERR_REC);
		lv_ucRtnVolt[ch] = CPV_VOLT_NINE;
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg <= CPV_VOLTAGE_9P8_RECOVER)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_9V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_9V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_NINE;
				}
			}
		}
	}
	else if ((lv_usVoltAvrg > CPV_VOLTAGE_6P8) && (lv_usVoltAvrg < CPV_VOLTAGE_8P2))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_6V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrCnt++;
			CP_DEBUG("ch :%d CPV_VoltErrTypeHandle CPV_ERR_VOLT_6V ucErrCnt:%d lv_usVoltAvrg:%d\r\n", ch, gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrCnt, lv_usVoltAvrg);
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrCnt >= CPV_VOLTAGE_6V_2V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_6V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_6V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_5P2) && (lv_usVoltAvrg <= CPV_VOLTAGE_6P8))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_6V, (uint8_t)CPV_ERR_REC);
		lv_ucRtnVolt[ch] = CPV_VOLT_SIX;
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg <= CPV_VOLTAGE_6P8_RECOVER)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_6V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_SIX;
				}
			}
		}
	}
	else
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_6V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucErrCnt >= CPV_VOLTAGE_6V_2V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_6V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_6V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_6V].ucRevocerErrCnt = 0;
		}
	}

	return lv_ucRtnVolt[ch];
}

/*******************************************************************************
Name            : CPV_4VCpVoltageFilter
Syntax          : static uint8_t CPV_4VCpVoltageFilter(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : lv_ucRtn                  :-
Description     : CP 4V filtering
Call By         :
|******************************************************************************/
static uint8_t CPV_4VCpVoltageFilter(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg)
{
	static uint8_t lv_ucRtnVolt[SYS_CONNECTOR_NUM_MAX] = {CPV_VOLT_ZERO};

	if (lv_usVoltAvrg > CPV_VOLTAGE_4P4)
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_4V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucErrCnt >= CPV_VOLTAGE_12V_4V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_4V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_4V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_3P6) && (lv_usVoltAvrg <= CPV_VOLTAGE_4P4))
	{
		lv_ucRtnVolt[ch] = CPV_VOLT_FOUR;
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_4V, (uint8_t)CPV_ERR_REC);
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg <= CPV_VOLTAGE_4P4_RECOVER)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_4V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_4V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_FOUR;
				}
			}
		}
	}
	else if ((lv_usVoltAvrg > CPV_VOLTAGE_3P4) && (lv_usVoltAvrg < CPV_VOLTAGE_3P6))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_3V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucErrCnt >= CPV_VOLTAGE_12V_4V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_3V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_3V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_2P6) && (lv_usVoltAvrg <= CPV_VOLTAGE_3P4))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_3V, (uint8_t)CPV_ERR_REC);
		lv_ucRtnVolt[ch] = CPV_VOLT_THREE;
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg <= CPV_VOLTAGE_3P4)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_3V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_3V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_THREE;
				}
			}
		}
	}
	else if (lv_usVoltAvrg > CPV_VOLTAGE_2P4 && lv_usVoltAvrg < CPV_VOLTAGE_2P6)
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_2V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrCnt >= CPV_VOLTAGE_6V_2V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_2V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_2V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucRevocerErrCnt = 0;
		}
	}
	else if ((lv_usVoltAvrg >= CPV_VOLTAGE_1P6) && (lv_usVoltAvrg <= CPV_VOLTAGE_2P4))
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_2V, (uint8_t)CPV_ERR_REC);
		lv_ucRtnVolt[ch] = CPV_VOLT_TWEO;
		if (CPV_GetErrStatus(ch) == STD_TRUE)
		{
			lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			if (lv_usVoltAvrg <= CPV_VOLTAGE_2P4)
			{
				gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucRevocerErrCnt++;
				if (gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucRevocerErrCnt >= CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT)
				{
					CPV_ClearVoltErr(ch);
					CP_DEBUG("ch:%d CP CLEAR CPV_ERR_VOLT_2V\r\n", ch);
					lv_ucRtnVolt[ch] = CPV_VOLT_TWEO;
				}
			}
		}
		else
		{
		}
	}
	else
	{
		CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_2V, (uint8_t)CPV_ERR_WAITING);
		if (gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrState == STD_FALSE)
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrCnt++;
			if (gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucErrCnt >= CPV_VOLTAGE_6V_2V_ERR_FILLTE_COUNT)
			{
				CP_DEBUG("ch :%d CP ERROR CPV_ERR_VOLT_2V\r\n", ch);
				CPV_VoltErrTypeHandle(ch, CPV_ERR_VOLT_2V, (uint8_t)CPV_ERR_INPUT);
				lv_ucRtnVolt[ch] = CPV_VOLT_ZERO;
			}
		}
		else
		{
			gv_stVoltageErr[ch][CPV_ERR_VOLT_2V].ucRevocerErrCnt = 0;
		}
	}

	return lv_ucRtnVolt[ch];
}
/*******************************************************************************
Name            : CPV_CpVoltErrHandle
Syntax          : static void CPV_CpVoltErrHandle(float lv_usVoltAvrg)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_usVoltAvrg
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP voltage status filter
Call By         :
|******************************************************************************/
static void CPV_CpVoltErrHandle(SysConnector_Num_Enum ch, uint16_t lv_usVoltAvrg)
{
	static uint8_t lv_ucRtnVolt[SYS_CONNECTOR_NUM_MAX] = {CPV_VOLT_ZERO};

	if (CPV_GET_CP_SWITCH_12V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		lv_ucRtnVolt[ch] = CPV_12VCpVoltageFilter(ch, lv_usVoltAvrg);
	}
	else if (CPV_GET_CP_SWITCH_4V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		lv_ucRtnVolt[ch] = CPV_4VCpVoltageFilter(ch, lv_usVoltAvrg);
	}
	gv_stCpVolt[ch].stCpVolt.ucStatus = lv_ucRtnVolt[ch];

	if (TRUE == LIB_StatusFilter(&gv_stCpVolt[ch].stCpVolt, CPV_VOLT_STATUS_FILTER_MAX_CNT))
	{
		CP_DEBUG("ch:%d Cp OUT %d\r\n", ch, gv_stCpVolt[ch].stCpVolt.ucValidStatus);
	}

	if (CPV_VOLT_ZERO == gv_stCpVolt[ch].stCpVolt.ucValidStatus)
	{
		gv_stCpVolt[ch].stCpVoltError.ucStatus = STD_TRUE;
	}
	else
	{
		gv_stCpVolt[ch].stCpVoltError.ucStatus = STD_FALSE;
	}

	LIB_StatusFilter(&gv_stCpVolt[ch].stCpVoltError, CPV_VOLT_ERROR_FILTER_MAX_CNT);

}

/*******************************************************************************
Name            : CPV_AdcCvrtToVolt
Syntax          : static uint16_t CPV_AdcCvrtToVolt(SysConnector_Num_Enum ch, uint16_t lv_usAdcValue)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Adc Cvrt To Volt
Call By         :
|******************************************************************************/
static void CPV_AdcCvrtToVolt(SysConnector_Num_Enum ch, uint16_t lv_usAdcValue)
{
	if (CPV_GET_CP_SWITCH_4V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		gv_stCpVolt[ch].usVoltValue = (uint16_t)((uint32_t)lv_usAdcValue * CPV_12V_CVRT_COEFF / 100u);
	}
	else if (CPV_GET_CP_SWITCH_12V_MODE == CPV_GET_SWITCH_MODE(ch))
	{
		gv_stCpVolt[ch].usVoltValue = (uint16_t)((uint32_t)lv_usAdcValue * CPV_4V_CVRT_COEFF / 100u);
	}
	else
	{}
	// CP_DEBUG("ch:%d,usAdcValue:%d,usVoltValue:%d\r\n", ch, lv_usAdcValue, gv_stCpVolt[ch].usVoltValue);
	// CP_DEBUG("usVoltValue:%d\r\n", gv_stCpVolt[0].usVoltValue);
}

/*******************************************************************************
Name            : CPV_BreakLineHandle
Syntax          : static void CPV_BreakLineHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP break line handle
Call By         :
|******************************************************************************/
#if (STD_ON == CPV_BRK_LIN_FUN_EN)
static void CPV_BreakLineHandle(SysConnector_Num_Enum ch)
{
	static uint16_t lv_usBrkLnRecCnt = 0u;
	static uint8_t lv_ucEvseThreeDot = STD_FALSE;

	if (STD_TRUE == lv_ucEvseThreeDot)
	{
		if (CPV_VOLT_TWF == gv_stCpVolt[ch].stCpVolt.ucValidStatus)
		{
			gv_stCpVolt[ch].ucCpBreakLineFlag = STD_TRUE;
		}
		else
		{
		}
	}
	else
	{
	}

	if ((CPV_VOLT_TWF == gv_stCpVolt[ch].stCpVolt.ucValidStatus) && (STD_TRUE == gv_stCpVolt[ch].ucCpBreakLineFlag))
	{
		if (gv_stCpVolt[ch].usBrkLinWaitCnt < CPV_VOLT_BRKLIN_FILTER_MAX_CNT)
		{
			gv_stCpVolt[ch].usBrkLinWaitCnt++;
		}
		else
		{
			gv_stCpVolt[ch].ucBrkLinErr = STD_TRUE;
			gv_stCpVolt[ch].usBrkLinWaitCnt = 0u;
			gv_stCpVolt[ch].ucCpBreakLineFlag = STD_FALSE;
		}
	}
	else
	{
		gv_stCpVolt[ch].usBrkLinWaitCnt = 0u;
		gv_stCpVolt[ch].ucCpBreakLineFlag = STD_FALSE;
	}

	if (STD_TRUE == gv_stCpVolt[ch].ucBrkLinErr)
	{
		gv_stCpVolt[ch].usBrkLinRestoreWaitCnt++;
		if (gv_stCpVolt[ch].usBrkLinRestoreWaitCnt >= CPV_VOLT_BRKLIN_RESTORE_MAX_CNT)
		{
			gv_stCpVolt[ch].usBrkLinRestoreWaitCnt = 0;
			gv_stCpVolt[ch].ucBrkLinErr = STD_FALSE;
		}
		else
		{
		}

		if ((CPV_VOLT_NINE == gv_stCpVolt[ch].stCpVolt.ucValidStatus) || (CPV_VOLT_SIX == gv_stCpVolt[ch].stCpVolt.ucValidStatus))
		{
			lv_usBrkLnRecCnt++;
			if (lv_usBrkLnRecCnt >= CPV_VOLT_BRKLIN_NINE_RESTORE_MAX_CNT)
			{
				lv_usBrkLnRecCnt = 0u;
				gv_stCpVolt[ch].ucBrkLinErr = STD_FALSE;
			}
		}
		else
		{
			lv_usBrkLnRecCnt = 0u;
		}
	}
	else
	{
		lv_usBrkLnRecCnt = 0u;
		gv_stCpVolt[ch].usBrkLinRestoreWaitCnt = 0;
	}

	gv_stCpVolt[ch].stCpBreakLine.ucStatus = gv_stCpVolt[ch].ucBrkLinErr;

	if (STD_TRUE == LIB_StatusFilter(&gv_stCpVolt[ch].stCpBreakLine, CPV_VOLT_BREAKLINE_FILTER_MAX_CNT))
	{
		if (STD_TRUE == gv_stCpVolt[ch].stCpBreakLine.ucValidStatus)
		{
			CpDrvif_SetFaultStatus(ERRHDL_ID_CP_BREAKLINE, STD_TRUE);
		}
		else
		{
			CpDrvif_SetFaultStatus(ERRHDL_ID_CP_BREAKLINE, STD_FALSE);
		}
	}
	else
	{
	}

	if (CPV_EVSE_CHARING_STATUS == CPV_GET_EVSE_ChargeStatus())
	{
		lv_ucEvseThreeDot = STD_TRUE;
	}
	else
	{
		lv_ucEvseThreeDot = STD_FALSE;
	}
}
#endif
/* polyspace:end<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*EOF*/
