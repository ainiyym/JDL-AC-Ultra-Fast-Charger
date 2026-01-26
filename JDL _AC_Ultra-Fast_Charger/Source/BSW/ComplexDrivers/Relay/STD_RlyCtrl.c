//****************************************************************************************
//*
//* File Name: STD_RlyCtrl.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_RlyCtrl.h"
#include "STD_Fifo.h"
#include "STD_Lib.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucMode;					   /* Mode */
	uint8_t ucReqSwitchStatus;		   /* Request switching status */
	uint8_t ucCurrSwitchStatus;		   /* Current switching state */
	uint8_t ucSwitchStep;			   /* Switching steps */
	uint8_t ucControlType;			   /* Control type */
	LibFilterStruct stAuxiliaryFilter; /* Filter structure */
	uint8_t ucErrorStatus;			   /* Error status */
	uint16_t usWaitCnt;				   /* Wait conter */
} RLYCTRL_Struct;

typedef enum
{
	RLYCTRL_MODE_IDLE = 0U, /*Relay control idle mode*/
	RLYCTRL_MODE_SWITCH,	/*Relay control switching mode*/
} RLYCTRL_ModeType;

typedef enum
{
	RLYCTRL_Switch_One = 0U, /*Relay control switching step one*/
	RLYCTRL_Switch_Two,		 /*Relay control switching step two*/
	RLYCTRL_Switch_Three,	 /*Relay control switching step three*/
} RLYCTRL_SwitchStepType;

typedef enum
{
	RLYCTRL_STATUS_UNKNOWN = 0U, /*Ralay unkown status*/
	RLYCTRL_STATUS_OFF,			 /*Ralay status on*/
	RLYCTRL_STATUS_ON,			 /*Ralay status off*/
	RLYCTRL_CONGLUTINATION,		 /*Ralay conglutination*/
	RLYCTRL_OPENCIRCUIT			 /*Ralay opencircuit*/
} RLYCTRL_Status_Type;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static RLYCTRL_Struct gv_stRlyCtrl[SYS_CONNECTOR_NUM_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name              : RLYCTRL_InitMemory
Syntax            : void RLYCTRL_InitMemory(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Initialize Memory
Call By           : RELAYM_InitMemory
|******************************************************************************/
void RLYCTRL_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)(&gv_stRlyCtrl), 0u, (uint16_t)(sizeof(gv_stRlyCtrl) / sizeof(uint8_t))); /*PRQA S 0310*/
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus = 0xff;
	}
}

/*******************************************************************************
Name              : RLYCTRL_ReqRelaySwitchOn
Syntax            : uint8_t RLYCTRL_ReqRelaySwitchOn(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : request relay switch on
Call By           : RELAYM_ReqRelaySwitchOn
|******************************************************************************/
uint8_t RLYCTRL_ReqRelaySwitchOn(SysConnector_Num_Enum ch)
{
	uint8_t ret = STD_TRUE;

	if (ch < SYS_CONNECTOR_NUM_MAX)
	{
		gv_stRlyCtrl[ch].ucReqSwitchStatus = RLYCTRL_SwitchOn;
		RLYCTRL_DEBUG("ch:%d ucReqSwitch on\r\n", ch);
	}
	else
	{
		ret = STD_FALSE;
	}

	return ret;
}

/*******************************************************************************
Name              : RLYCTRL_ReqRelaySwitchOff
Syntax            : uint8_t  RLYCTRL_ReqRelaySwitchOff(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : request relay switch off
Call By           : RELAYM_ReqRelaySwitchOff
|******************************************************************************/
uint8_t  RLYCTRL_ReqRelaySwitchOff(SysConnector_Num_Enum ch)
{
	uint8_t ret = STD_TRUE;

	if (ch < SYS_CONNECTOR_NUM_MAX)
	{
		gv_stRlyCtrl[ch].ucReqSwitchStatus = RLYCTRL_SwitchOff;
		RLYCTRL_DEBUG("ch:%d ucReqSwitch off\r\n", ch);
	}
	else
	{
		ret = STD_FALSE;
	}

	return ret;
}

/*******************************************************************************
Name              : RLYCTRL_ForceRelaySwitchOff
Syntax            : uint8_t RLYCTRL_ForceRelaySwitchOff(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : force relay switch off
Call By           : RELAYM_ForceRelaySwitchOff
|******************************************************************************/
uint8_t RLYCTRL_ForceRelaySwitchOff(SysConnector_Num_Enum ch)
{
	uint8_t ret = STD_FALSE;

	ret = MOSDRV_ReqMosOff(ch);
	RLYCTRL_DEBUG("ForceRelaySwitchOff\r\n");

	return ret;
}
/*******************************************************************************
Name              : RLYCTRL_GetRelayStatus
Syntax            : uint8_t RLYCTRL_GetRelayStatus(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : uint8_t
Description       : Get Relay Status
Call By           : RELAYM_GetRelayStatus
|******************************************************************************/
uint8_t RLYCTRL_GetRelayStatus(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucret = RLYCTRL_STATE_UNKNOWN;

	if (STD_TRUE == gv_stRlyCtrl[ch].ucErrorStatus)
	{
		lv_ucret = RLYCTRL_STATE_ERR;
	}
	else
	{
		if ((uint8_t)RLYCTRL_SwitchOn == gv_stRlyCtrl[ch].ucCurrSwitchStatus)
		{
			lv_ucret = (uint8_t)RLYCTRL_STATE_ON;
		}
		else
		{
			lv_ucret = (uint8_t)RLYCTRL_STATE_OFF;
		}
	}

	return lv_ucret;
} 

/*******************************************************************************
Name              : RLYCTRL_ModeSwitchHandle
Syntax            : static void RLYCTRL_ModeSwitchHandle(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : switch handle
Call By           : RLYCTRL_MainFunction
|******************************************************************************/
static void RLYCTRL_ModeSwitchHandle(SysConnector_Num_Enum ch)
{
	if(RLYCTRL_SWITCH_IDLE == gv_stRlyCtrl[ch].ucControlType)
	{
		gv_stRlyCtrl[ch].ucMode = RLYCTRL_MODE_IDLE;
		gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_One;
		RLYCTRL_DEBUG("ch:%d RELAYCTRL_MODE_IDLE \r\n",ch);
	}
	else
	{
	}
}

/*******************************************************************************
Name              : RLYCTRL_ModeIdleHandle
Syntax            : static void RLYCTRL_ModeIdleHandle(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Idle handle
Call By           : RLYCTRL_MainFunction
|******************************************************************************/
static void RLYCTRL_ModeIdleHandle(SysConnector_Num_Enum ch)
{
	if ((gv_stRlyCtrl[ch].ucReqSwitchStatus != gv_stRlyCtrl[ch].ucCurrSwitchStatus))
	{
		gv_stRlyCtrl[ch].ucMode = RLYCTRL_MODE_SWITCH;
		if (RLYCTRL_SwitchOff == gv_stRlyCtrl[ch].ucReqSwitchStatus)
		{
			gv_stRlyCtrl[ch].ucControlType = RLYCTRL_SWITCH_OFF;
			RLYCTRL_DEBUG("gv_stRlyCtrl[%d].ucControlType = RLYCTRL_SWITCH_Off\r\n",ch);
		}
		else
		{
			gv_stRlyCtrl[ch].ucControlType = RLYCTRL_SWITCH_ON;
			RLYCTRL_DEBUG("gv_stRlyCtrl[%d].ucControlType = RLYCTRL_SWITCH_On\r\n",ch);
		}
	}
	else
	{
	}
}

/*******************************************************************************
Name              : RLYCTRL_SwitchControl
Syntax            : static void RLYCTRL_SwitchControl(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : control mode
Call By           : RLYCTRL_MainFunction
|******************************************************************************/
static void RLYCTRL_SwitchControl(SysConnector_Num_Enum ch)
{
	if (RLYCTRL_SWITCH_IDLE != gv_stRlyCtrl[ch].ucControlType)
	{
		switch (gv_stRlyCtrl[ch].ucSwitchStep)
		{
		case RLYCTRL_Switch_One:
		{
			if (RLYCTRL_SWITCH_ON == gv_stRlyCtrl[ch].ucControlType)
			{
				if (RLYCTRL_BTR_VOL_OK == RLYCTRL_Get12VResult())
				{
					gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_Two;
					RLYCTRL_DEBUG("ch:%d RELAYMOS_REQ MosON \r\n",ch);
				}
				else
				{
					if (gv_stRlyCtrl[ch].usWaitCnt++ > RLYCTRL_WAITING_RELAY_PIOWER_UPPER_TIME)
					{
						gv_stRlyCtrl[ch].usWaitCnt = 0;
						gv_stRlyCtrl[ch].ucReqSwitchStatus = RLYCTRL_SwitchOff;
						gv_stRlyCtrl[ch].ucControlType = RLYCTRL_SWITCH_IDLE;
					}
				}
			}
			else if (RLYCTRL_SWITCH_OFF == gv_stRlyCtrl[ch].ucControlType)
			{
				gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_Two;
				RLYCTRL_DEBUG("ch: %d RELAYMOS_REQ MosOFF \r\n",ch);
			}
			else
			{
			}
		}
		break;
		case RLYCTRL_Switch_Two:
		{
			if (RLYCTRL_SWITCH_OFF == gv_stRlyCtrl[ch].ucControlType)
			{
				MOSDRV_ReqMosOff(ch);
				gv_stRlyCtrl[ch].ucCurrSwitchStatus = RLYCTRL_SwitchOff;
				gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_Three;
				RLYCTRL_DEBUG("ch:%d MOS OFF \r\n",ch);
			}
			else if (RLYCTRL_SWITCH_ON == gv_stRlyCtrl[ch].ucControlType)
			{
				MOSDRV_ReqMosOn(ch);
				gv_stRlyCtrl[ch].ucCurrSwitchStatus = RLYCTRL_SwitchOn;
				gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_Three;
				RLYCTRL_DEBUG("ch:%d MOS ON\r\n",ch);
			}
			else
			{
			}
		}
		break;
		case RLYCTRL_Switch_Three:
		{
			static uint8_t cnt = 0;
			if (RLYCTRL_SWITCH_ON == gv_stRlyCtrl[ch].ucControlType)
			{
				if (cnt++ > 20u)
				{
					cnt = 0;
					MOSDRV_ResetMosOnStatus(ch);
					gv_stRlyCtrl[ch].ucControlType = RLYCTRL_SWITCH_IDLE;
					RLYCTRL_DEBUG("ch:%d ON GO TO IDLE \r\n",ch);
				}
			}
			else
			{
				MOSDRV_ResetMosOnStatus(ch);
				gv_stRlyCtrl[ch].ucControlType = RLYCTRL_SWITCH_IDLE;
				RLYCTRL_DEBUG("ch:%d OFF GO TO IDLE \r\n",ch);
			}
		}
		break;
		default:
			gv_stRlyCtrl[ch].ucSwitchStep = RLYCTRL_Switch_One;
		}
	}
	else
	{
	}
}
/*******************************************************************************
Name              : RLYCTRL_AuxlDetect
Syntax            : void RLYCTRL_AuxlDetect(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Detect Relay Auxl
Call By           : RLYCTRL_MainFunction
|******************************************************************************/
static void RLYCTRL_AuxlDetect(SysConnector_Num_Enum ch)
{
	gv_stRlyCtrl[ch].stAuxiliaryFilter.ucStatus = MOSDRV_ReadContactorAuxiliaryStatus(ch);

	if (RLYCTRL_SwitchOff == gv_stRlyCtrl[ch].ucReqSwitchStatus && RLYCTRL_SwitchOff == gv_stRlyCtrl[ch].ucCurrSwitchStatus)
	{
		(void)LIB_StatusFilter(&gv_stRlyCtrl[ch].stAuxiliaryFilter, RLYCTRL_AUXL_OFF_FILTER_NUM);

		if (0xff != gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus)
		{
			if (gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus == MOSDRV_HIGH)
			{
				if (ch == SYS_CONNECTOR1)
				{
					RLYCTRL_FAULT_CALLBACK(ERRHDL_ID_GUN1_RELAY_FAULT, STD_TRUE);
				}
				else if (ch == SYS_CONNECTOR2)
				{
					RLYCTRL_FAULT_CALLBACK(ERRHDL_ID_GUN2_RELAY_FAULT, STD_TRUE);
				}
				else
				{
				}
				gv_stRlyCtrl[ch].ucErrorStatus = STD_TRUE;
				RLYCTRL_ERR("ch:%d ERRHDL_ID_RELAY_CONGLUTINATION!!! \r\n",ch);
			}
			gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus = 0xff; /* Reset valid status */
		}
	}
	else if (RLYCTRL_SwitchOn == gv_stRlyCtrl[ch].ucReqSwitchStatus && RLYCTRL_SwitchOn == gv_stRlyCtrl[ch].ucCurrSwitchStatus)
	{
		(void)LIB_StatusFilter(&gv_stRlyCtrl[ch].stAuxiliaryFilter, RLYCTRL_AUXL_ON_FILTER_NUM);

		if (0xff != gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus)
		{
			if (gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus == MOSDRV_LOW)
			{
				if (ch == SYS_CONNECTOR1)
				{
					RLYCTRL_FAULT_CALLBACK(ERRHDL_ID_GUN1_RELAY_FAULT, STD_TRUE);
				}
				else if (ch == SYS_CONNECTOR2)
				{
					RLYCTRL_FAULT_CALLBACK(ERRHDL_ID_GUN2_RELAY_FAULT, STD_TRUE);
				}
				else
				{
				}
				gv_stRlyCtrl[ch].ucErrorStatus = STD_TRUE;
				RLYCTRL_ERR("ch:%d ERRHDL_ID_RELAY_OPENCIRCUIT!!! \r\n",ch);
			}
			gv_stRlyCtrl[ch].stAuxiliaryFilter.ucValidStatus = 0xff; /* Reset valid status */
		}
	}
	else
	{}
}
/*******************************************************************************
Name              : RLYCTRL_MainFunction
Syntax            : void RLYCTRL_MainFunction(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Relay control main function
Call By           : EcuM_App Module
|******************************************************************************/
void RLYCTRL_MainFunction(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		switch (gv_stRlyCtrl[ch].ucMode)
		{
		case RLYCTRL_MODE_IDLE:
		{
			RLYCTRL_ModeIdleHandle(ch);
		}
		break;
		case RLYCTRL_MODE_SWITCH:
		{
			RLYCTRL_SwitchControl(ch);
			RLYCTRL_ModeSwitchHandle(ch);
		}
		break;
		default:
		{
			gv_stRlyCtrl[ch].ucMode = RLYCTRL_MODE_IDLE;
		}
		break;
		}
		RLYCTRL_AuxlDetect(ch);
	}
}

/*EOF*/
