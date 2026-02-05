//****************************************************************************************
//*
//* File Name: CpM.c
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

#include "CpM.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */


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
Name            : CPM_InitMemory
Syntax          : void CPM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP Module Initialize RAM
Call By         :
|******************************************************************************/
void CPM_InitMemory(void)
{
	CPD_InitMemory();
	CPV_InitMemory();
}

/*******************************************************************************
Name            : CPM_Enable
Syntax          : void CPM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Enable CP Module 
Call By         :
|******************************************************************************/
void CPM_Enable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		CPD_Open(ch);
		CPV_Open(ch);
	}
}

/*******************************************************************************
Name            : CPM_Disable
Syntax          : void CPM_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Disnable CP Module 
Call By         :
|******************************************************************************/
void CPM_Disable(void)
{
	for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
	{
		CPD_Close(ch);
		CPV_Close(ch);
	}
}

/*******************************************************************************
Name            : CPM_5msMainFunction
Syntax          : void CPM_5msMainFunction(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : CP Module Main Function
Call By         :
|******************************************************************************/
void CPM_5msMainFunction(void)
{
	if (STD_TRUE == CPM_GetSysReadyState())
	{
		for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
		{
			CPV_MainFunction(ch);
			CPD_MainFunction(ch);
		}
	}
	else
	{
	}
}

/* polyspace:end<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*EOF*/
