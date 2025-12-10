//****************************************************************************************
//*
//* File Name: STD_NetAuth.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_NetAuth.h"
#include "STD_Lib.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define  NETAUTH_MODE_IDLE        0x00u 	     /* Idle mode */
#define  NETAUTH_MODE_NORMAL      0x01u 	     /* Normal mode */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t ucMode;	              /* Manage mode */
	uint8_t ucEnStatus;             /* enable Status */
	uint8_t ucAuthStatus;           /* Net auth Status */
	uint8_t ucCancelAuthStatus;     /* Net cancle auth Status */
	uint8_t ucReqAuthStatus;        /* Net request auth Status */
	uint8_t ucReqCancelAuthStatus;  /* Net request cancel auth Status */
}NetAuthCtx_Struct;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static NetAuthCtx_Struct gv_stNetAuthCtx[SYS_CONNECTOR_NUM_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void NETAUTH_IdleModeHandle(SysConnector_Num_Enum ch);
static void NETAUTH_NormalModeHandle(SysConnector_Num_Enum ch);
static void NETAUTH_ResetToIdleMode(SysConnector_Num_Enum ch);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/*******************************************************************************
Name            : NETAUTH_Enable
Syntax          : void NETAUTH_Enable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Enable the Mode
Call By         : STD_SysM Or Diagnosis module
|******************************************************************************/
void NETAUTH_Enable(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucEnStatus = (uint8_t)STD_TRUE;
}

/*******************************************************************************
Name            : NETAUTH_Disable
Syntax          : void NETAUTH_Disable(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Enable the Mode
Call By         : STD_SysM Or Diagnosis module
|******************************************************************************/
void NETAUTH_Disable(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucEnStatus = (uint8_t)STD_FALSE;
}

/*******************************************************************************
Name            : NETAUTH_InitMemory
Syntax          : void RFIDAUTH_InitMemory(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Enable the Mode
Call By         : STD_SysM module
|******************************************************************************/
void NETAUTH_InitMemory(void)
{
	LIB_SetMemory((uint8_t *)(&gv_stNetAuthCtx), 0u, (uint16_t)(sizeof(gv_stNetAuthCtx)));
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		NETAUTH_Enable(ch);
	}
}
/*******************************************************************************
Name            : NETAUTH_GetAuthStatus
Syntax          : uint8_t NETAUTH_GetAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : STD_TRUE-Need Auth, STD_FALSE-Not need Auth
Description     : Get Auth Status
Call By         : STD_Auth Module
|******************************************************************************/
uint8_t NETAUTH_GetAuthStatus(SysConnector_Num_Enum ch)
{
	return gv_stNetAuthCtx[ch].ucAuthStatus;
}

/*******************************************************************************
Name            : NETAUTH_GetCancelAuthStatus
Syntax          : uint8_t NETAUTH_GetCancelAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : STD_TRUE-Need Cancel Auth, STD_FALSE-Not need cancel Auth
Description     : Get Cancel Auth Status
Call By         : STD_Auth Module
|******************************************************************************/
uint8_t NETAUTH_GetCancelAuthStatus(SysConnector_Num_Enum ch)
{
	return gv_stNetAuthCtx[ch].ucCancelAuthStatus;
}

/*******************************************************************************
Name            : NETAUTH_SetReqAuthStatus
Syntax          : void NETAUTH_SetReqAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Set Require Auth Status
Call By         :
|******************************************************************************/
void NETAUTH_SetReqAuthStatus(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucReqAuthStatus = STD_TRUE;
}

/*******************************************************************************
Name            : NETAUTH_ResetAuthStatus
Syntax          : void NETAUTH_ResetAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Reset auth status
Call By         : STD_AuthM
|******************************************************************************/
void NETAUTH_ResetAuthStatus(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucAuthStatus = STD_FALSE;
}

/*******************************************************************************
Name            : NETAUTH_SetReqCancelAuthStatus
Syntax          : void NETAUTH_SetReqCancelAuthStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Set Require Cancel Auth Status
Call By         :
|******************************************************************************/
void NETAUTH_SetReqCancelAuthStatus(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucReqCancelAuthStatus = STD_TRUE;
}

/*******************************************************************************
Name            : NETAUTH_ResetToIdleMode
Syntax          : void NETAUTH_ResetToIdleMode(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Reset to Idle mode and clear some flags
Call By         : NETAUTH_MainFunction\NETAUTH_NormalModeHandle
|******************************************************************************/
static void NETAUTH_ResetToIdleMode(SysConnector_Num_Enum ch)
{
	gv_stNetAuthCtx[ch].ucMode = NETAUTH_MODE_IDLE;
	gv_stNetAuthCtx[ch].ucAuthStatus = STD_FALSE;
	gv_stNetAuthCtx[ch].ucCancelAuthStatus = STD_FALSE;
}

/*******************************************************************************
Name            : NETAUTH_IdleModeHandle
Syntax          : void NETAUTH_IdleModeHandle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Idle mode handle
Call By         : NETAUTH_MainFunction
|******************************************************************************/
static void NETAUTH_IdleModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucSysReadyStatus = NETAUTH_GetSysPrepareStatus();
	uint8_t lv_ucAllowResAuthReqStatus = NETAUTH_GetAuthAllowStatus(ch);

	if (STD_TRUE == lv_ucSysReadyStatus \
		&& STD_TRUE == gv_stNetAuthCtx[ch].ucEnStatus \
		&& STD_TRUE == lv_ucAllowResAuthReqStatus)
	{
		gv_stNetAuthCtx[ch].ucMode = NETAUTH_MODE_NORMAL;
	}
}

/*******************************************************************************
Name            : NETAUTH_NormalMode_Handle
Syntax          : void NETAUTH_ConfigMode_Handle(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Normal mode handle
Call By         : NETAUTH_MainFunction
|******************************************************************************/
static void NETAUTH_NormalModeHandle(SysConnector_Num_Enum ch)
{
	uint8_t lv_ucAuthStatus = STD_FALSE;

	if (STD_TRUE != gv_stNetAuthCtx[ch].ucEnStatus)
	{
		NETAUTH_ResetToIdleMode(ch);
	}
	else
	{
		lv_ucAuthStatus = NETAUTH_GetCurrAuthStatus(ch);

		if (STD_TRUE != lv_ucAuthStatus)
		{
			gv_stNetAuthCtx[ch].ucReqCancelAuthStatus = STD_FALSE;

			gv_stNetAuthCtx[ch].ucCancelAuthStatus = STD_FALSE;

			if (STD_TRUE == gv_stNetAuthCtx[ch].ucReqAuthStatus)
			{
				gv_stNetAuthCtx[ch].ucAuthStatus = STD_TRUE;
				gv_stNetAuthCtx[ch].ucReqAuthStatus = STD_FALSE;
			}
		}
		else
		{
			gv_stNetAuthCtx[ch].ucReqAuthStatus = STD_FALSE;

			gv_stNetAuthCtx[ch].ucAuthStatus = STD_FALSE;

			if (STD_TRUE == gv_stNetAuthCtx[ch].ucReqCancelAuthStatus)
			{
				gv_stNetAuthCtx[ch].ucCancelAuthStatus = STD_TRUE;
				gv_stNetAuthCtx[ch].ucReqCancelAuthStatus = STD_FALSE;
			}
		}
	}
}

/*******************************************************************************
Name            : RFIDAUTH_MainFunction
Syntax          : void RFIDAUTH_MainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :  connector index
Parameters(out) : None
Return value    : None
Description     : Rfid auth mainfunction
Call By         : Task
|******************************************************************************/
void NETAUTH_MainFunction(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		switch (gv_stNetAuthCtx[ch].ucMode)
		{
		case NETAUTH_MODE_IDLE:
		{
			NETAUTH_IdleModeHandle(ch);
			break;
		}
		case NETAUTH_MODE_NORMAL:
		{
			NETAUTH_NormalModeHandle(ch);
			break;
		}
		default:
		{
			NETAUTH_ResetToIdleMode(ch);
			break;
		}
		}
	}
}
/*EOF*/



