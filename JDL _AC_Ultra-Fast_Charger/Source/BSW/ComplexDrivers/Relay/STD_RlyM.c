//****************************************************************************************
//*
//* File Name: STD_RlyM.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_RlyM.h"
#include "STD_RlyCtrl.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name            : RELAYM_InitMemory
Syntax          : void RELAYM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                   :-
Parameters(in)  : None                   :-
Parameters(in)  : None                   :-
Parameters(out) : None                   :-
Return value    : None
Description     : Initialize Memory
Call By         : SYSM_InitThree
|******************************************************************************/
void RELAYM_InitMemory(void)
{
	RLYCTRL_InitMemory();
}
/*******************************************************************************
Name            : RELAYM_ForceRelaySwitchOff
Syntax          : void RELAYM_ForceRelaySwitchOff(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                   :-
Parameters(in)  : None                   :-
Parameters(in)  : None                   :-
Parameters(out) : None                   :-
Return value    : None
Description     : Force relay switch on
Call By         : RCDCTR_AcDcErrorCallBack
|******************************************************************************/
void RELAYM_ForceRelaySwitchOff(SysConnector_Num_Enum ch)
{
	RLYCTRL_ForceRelaySwitchOff(ch);
}
/*******************************************************************************
Name            : RELAYM_5msMainFunction
Syntax          : void RELAYM_5msMainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : Main function
Call By         : TASK( OS_Task5msA )
|******************************************************************************/
void RELAYM_5msMainFunction(void)
{
	RLYCTRL_MainFunction();
	// RLYCTRL_ShortSelfCheckMainFunction();
	// MOSDRV_SelfCheckMainFunction();
}
/*******************************************************************************
Name            : RELAYM_ReqRelaySwitchOn
Syntax          : void RELAYM_ReqRelaySwitchOn(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                   :-
Parameters(out) : None                   :-
Return value    : None
Description     : Request relay switch on
Call By         : EVSEM_StateTwoDotHandle
|******************************************************************************/
uint8_t RELAYM_ReqRelaySwitchOn(SysConnector_Num_Enum ch)
{
	return RLYCTRL_ReqRelaySwitchOn(ch);
}
/*******************************************************************************
Name            : RELAYM_ReqRelaySwitchOff
Syntax          : uint8_t RELAYM_ReqRelaySwitchOff(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : request relay switch off
Call By         : EVSEM_StateTwoDotHandle
|******************************************************************************/
uint8_t RELAYM_ReqRelaySwitchOff(SysConnector_Num_Enum ch)
{
	return RLYCTRL_ReqRelaySwitchOff(ch);
}
/*******************************************************************************
Name            : RELAYM_GetRelayStatus
Syntax          : uint8_t RELAYM_GetRelayStatus(SysConnector_Num_Enum ch)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : Get relay status
Call By         : Protocol_RelayValue
|******************************************************************************/
uint8_t RELAYM_GetRelayStatus(SysConnector_Num_Enum ch)
{
	return RLYCTRL_GetRelayStatus(ch);
}

/*******************************************************************************
Name            : RELAYM_ShortSelfCheckGetRelayStatus
Syntax          : uint8_t RELAYM_ShortSelfCheckGetRelayStatus(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : Short circuit selfcheck relay status
Call By         : EVSEM_RelaySelfCheck
|******************************************************************************/
uint8_t RELAYM_ShortSelfCheckGetRelayStatus(SysConnector_Num_Enum ch)
{
	return 0;
}

/*******************************************************************************
Name            : RELAYM_ShortSelfCheckReq
Syntax          : void RELAYM_ShortSelfCheckReq(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                  :-
Parameters(out) : None                  :-
Return value    : None
Description     : Short circuit selfcheck relay switch on
Call By         : EVSEM_RelaySelfCheck
|******************************************************************************/
void RELAYM_ShortSelfCheckReq(SysConnector_Num_Enum ch)
{
	// RLYCTRL_ShortSelfCheckReq(ch);
}
/*EOF*/


