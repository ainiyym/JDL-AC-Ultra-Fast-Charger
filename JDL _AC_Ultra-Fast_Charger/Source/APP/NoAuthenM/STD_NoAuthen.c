//****************************************************************************************
//*
//* File Name: STD_NoAuthen.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include "STD_NoAuthen.h"
#include "STD_Filter.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define NOAUTHEN_MODE_INIT			(0U)	/*init mode*/
#define NOAUTHEN_MODE_IDLE			(1U)	/*idle mod*/
#define NOAUTHEN_MODE_NOAUTHEN		(2U)	/*noauthen mode*/
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
	NOAUTHEN_TIMEAUTH_INVALID = 0,	/*NoAuthen time auth invalid*/
	NOAUTHEN_TIMEAUTH_NO_READY,		/*NoAuthen time auth no ready*/
	NOAUTHEN_TIMEAUTH_IS_READY,		/*NoAuthen time auth is ready*/
};
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucAuthStatus; 					/*Auth status*/
	uint8_t ucEnableStatus;					/*Enable status*/
	uint8_t ucMode;							/*Mode*/
	uint8_t ucFlag;							/*Flag*/
	Filter_Struct stNoAuthReadyStatus;		/*NoAuth Ready status*/
} NoAuthen_Struct;

/*******************************************************************************
|    Global NVM KAM Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static NoAuthen_Struct gv_stNoAuthen[SYS_CONNECTOR_NUM_MAX];

/****************************************************************************************
Global Variable Definition Section
*****************************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void NOAUTHEN_NoAuthenModeHandle(SysConnector_Num_Enum ch);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : NOAUTHEN_InitMemory
Syntax          : void NOAUTHEN_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  
Parameters(out) : None                  
Return value    : None
Description     : Initialize the NoAuth module
Call By         : SYSM_InitTwo
|******************************************************************************/
void NOAUTHEN_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)&gv_stNoAuthen, 0, sizeof(gv_stNoAuthen));
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		gv_stNoAuthen[ch].ucEnableStatus = NOAUTHEN_NOAUTH_DEFAULT;
	}
}

/*******************************************************************************
Name            : NOAUTHEN_GetEnableStatus
Syntax          : uint8_t NOAUTHEN_GetEnableStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  
Parameters(out) : None                  
Return value    : uint8_t
Description     : Get NoAuthen Enable Status
Call By         : Auth
|******************************************************************************/
uint8_t NOAUTHEN_GetEnableStatus(SysConnector_Num_Enum ch)	
{
	return gv_stNoAuthen[ch].ucEnableStatus;
}

/*******************************************************************************
Name            : NOAUTHEN_GetAuthStatus
Syntax          : uint8_t NOAUTHEN_GetAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  
Parameters(out) : None                  
Return value    : uint8_t
Description     : Get NoAuthen Auth Status
Call By         : Auth
|******************************************************************************/
uint8_t NOAUTHEN_GetAuthStatus(SysConnector_Num_Enum ch)	
{
	return gv_stNoAuthen[ch].ucAuthStatus;
}

/*******************************************************************************
Name            : NOAUTHEN_ResetAuthStatus
Syntax          : void NOAUTHEN_ResetAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None
Parameters(out) : None
Return value    : uint8_t
Description     : reset NoAuthen Auth Status
Call By         : Auth
|******************************************************************************/
void NOAUTHEN_ResetAuthStatus(SysConnector_Num_Enum ch)
{
	gv_stNoAuthen[ch].ucAuthStatus = STD_FALSE;
}

/*******************************************************************************
Name            : NOAUTHEN_ConfigEnable
Syntax          : void NOAUTHEN_ConfigEnable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  
Parameters(out) : None                 
Return value    : void
Description     : NoAuthen config enable
Call By         : diagnose
|******************************************************************************/
void NOAUTHEN_ConfigEnable(SysConnector_Num_Enum ch)
{
	gv_stNoAuthen[ch].ucEnableStatus = NOAUTHEN_NOAUTH_OPEN;
	NOAUTHEN_DEBUG("ch:%d NoAuthenEnable\r\n", ch);
}

/*******************************************************************************
Name            : NOAUTHEN_ConfigDisable
Syntax          : void NOAUTHEN_ConfigDisable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  
Parameters(out) : None                  
Return value    : void
Description     : NoAuthen config disable
Call By         : diagnose
|******************************************************************************/
void NOAUTHEN_ConfigDisable(SysConnector_Num_Enum ch)
{
	gv_stNoAuthen[ch].ucEnableStatus = NOAUTHEN_NOAUTH_CLOSE;
	NOAUTHEN_DEBUG("ch:%d NoAuthenDisable\r\n", ch);
}

/*******************************************************************************
Name            : NOAUTHEN_NoAuthenModeHandle
Syntax          : void NOAUTHEN_NoAuthenModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : noauthen mode handle
Call By         : NOAUTHEN_100msFunction
|******************************************************************************/
static void NOAUTHEN_NoAuthenModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucAuthStatus, lv_ucCpVolStatus;

	lv_ucAuthStatus = NOAUTHEN_GetCurrAuthStatus(ch);
	lv_ucCpVolStatus = NOAUTHEN_GetCpVolStatus(ch);

	if ((STD_FALSE == lv_ucAuthStatus) &&
		(STD_FALSE == gv_stNoAuthen[ch].ucFlag) &&
		((NOAUTHEN_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (NOAUTHEN_CP_VOL_STATUS_6V == lv_ucCpVolStatus)\
		||(NOAUTHEN_CP_VOL_STATUS_3V == lv_ucCpVolStatus)||(NOAUTHEN_CP_VOL_STATUS_2V == lv_ucCpVolStatus)))

	{
		gv_stNoAuthen[ch].stNoAuthReadyStatus.ucStatus = NOAUTHEN_TIMEAUTH_IS_READY;
		//			NOAUTHEN_DEBUG("ch:%d NoAuthen Auth valid\r\n", ch);
	}
	else
	{
		gv_stNoAuthen[ch].stNoAuthReadyStatus.ucStatus = NOAUTHEN_TIMEAUTH_NO_READY;
		//			NOAUTHEN_DEBUG("ch:%d NoAuthen Auth invalid\r\n", ch);
	}
	/* Judgy noauthen effectiveness after confirm authen stata */
	if (STD_TRUE == FILTER_StatusFiltT1(&gv_stNoAuthen[ch].stNoAuthReadyStatus, NOAUTHEN_TIMEAUTH_FILTER_CNT))
	{
		if (gv_stNoAuthen[ch].stNoAuthReadyStatus.ucValidStatus == NOAUTHEN_TIMEAUTH_IS_READY)
		{
			gv_stNoAuthen[ch].ucAuthStatus = STD_TRUE;
			NOAUTHEN_DEBUG("ch:%d NoAuthen Auth \r\n", ch);
		}
		else
		{
			gv_stNoAuthen[ch].ucAuthStatus = STD_FALSE;
		}
	}
	/* Clear noauthen request after confirm authen stata */
	if (STD_TRUE == NOAUTHEN_GetCurrAuthStatus(ch))
	{
		gv_stNoAuthen[ch].ucFlag = STD_TRUE;
		gv_stNoAuthen[ch].ucAuthStatus = STD_FALSE;
	}
	/* Draw gun process */
	if ((NOAUTHEN_CP_VOL_STATUS_12V == lv_ucCpVolStatus) || (NOAUTHEN_CP_VOL_STATUS_4V == lv_ucCpVolStatus))
	{
		if (STD_TRUE == gv_stNoAuthen[ch].ucFlag)
		{
			gv_stNoAuthen[ch].ucMode = NOAUTHEN_MODE_IDLE;
			gv_stNoAuthen[ch].ucFlag = STD_FALSE;
			gv_stNoAuthen[ch].ucAuthStatus = STD_FALSE;
			NOAUTHEN_DEBUG("ch:%d NoAuthen Draw Gun\r\n", ch);
		}
	}
	/* Disable noauthen process */
	if (NOAUTHEN_NOAUTH_CLOSE == gv_stNoAuthen[ch].ucEnableStatus)
	{
		gv_stNoAuthen[ch].ucMode = NOAUTHEN_MODE_IDLE;
	}
}

/*******************************************************************************
Name            : NOAUTHEN_100msFunction
Syntax          : void NOAUTHEN_100msFunction(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : main function
Call By         : TASK( OS_Task100msA )
|******************************************************************************/
void NOAUTHEN_100msFunction(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		{
			uint8_t lv_ucCpVolStatus, lv_ucAllowAuthReqStatus;

			lv_ucCpVolStatus = NOAUTHEN_GetCpVolStatus(ch); 
			lv_ucAllowAuthReqStatus = NOAUTHEN_GetAllowAuthReqStatus(ch);

			switch (gv_stNoAuthen[ch].ucMode)
			{
				case NOAUTHEN_MODE_INIT:
				{
					if (STD_TRUE == NOAUTHEN_GetSysPreStatus()) 
					{
						gv_stNoAuthen[ch].ucMode = NOAUTHEN_MODE_IDLE;
						NOAUTHEN_DEBUG("ch:%d NOAUTHEN_MODE_INIT\r\n", ch);
					}
					break;
				}
				case NOAUTHEN_MODE_IDLE:
				{
					if (STD_TRUE == lv_ucAllowAuthReqStatus &&
						((NOAUTHEN_CP_VOL_STATUS_9V == lv_ucCpVolStatus) || (NOAUTHEN_CP_VOL_STATUS_6V == lv_ucCpVolStatus)\
						|| (NOAUTHEN_CP_VOL_STATUS_3V == lv_ucCpVolStatus) || (NOAUTHEN_CP_VOL_STATUS_2V == lv_ucCpVolStatus))\
						&&(NOAUTHEN_NOAUTH_OPEN == gv_stNoAuthen[ch].ucEnableStatus))
					{
						gv_stNoAuthen[ch].ucMode = NOAUTHEN_MODE_NOAUTHEN;
						NOAUTHEN_DEBUG("ch:%d NOAUTHEN_MODE_IDLE\r\n", ch);
					}
					break;
				}
				case NOAUTHEN_MODE_NOAUTHEN:
				{
					NOAUTHEN_NoAuthenModeHandle(ch);
					break;
				}
				default:
				{
					gv_stNoAuthen[ch].ucMode = NOAUTHEN_MODE_INIT;
					break;
				}
			}
		}
	}
}
	/*EOF*/
