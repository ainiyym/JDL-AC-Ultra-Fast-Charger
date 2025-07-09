//****************************************************************************************
//*
//* File Name: BtrM.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "BTRM.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

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
Name              : BTRCTR_InitMemory
Syntax            : void BTRCTR_InitMemory(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Initialize the variable
Call By           : EcuM_App Module
History
<No.>    <author>    <time>    <description>
1
|******************************************************************************/
void BTRM_InitMemory(void)
{
	BTRCTR_InitMemory();
}

/*******************************************************************************
Name              : BTRM_Enable
Syntax            : void BTRM_Enable(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      :
Description       : Enable BTRCTR Module
Call By           : EcuM_App Module
History
<No.>    <author>    <time>    <description>
1
|******************************************************************************/
void BTRM_Enable(void)
{
	BTRCTR_Open();
	BTRDRV_StartAdcCollection();
}
/*******************************************************************************
Name              : BTRM_Disable
Syntax            : void BTRM_Disable(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      :
Description       : Disable BTRCTR Module
Call By           : EcuM_App Module
History
<No.>    <author>    <time>    <description>
1
|******************************************************************************/
void BTRM_Disable(void)
{
	BTRCTR_Close();
}
/*******************************************************************************
Name              : BTRM_10msMainFunction
Syntax            : void BTRCTR_10msMainFunction( void )
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      :
Description       :
Call By           :
History
<No.>    <author>    <time>    <description>
1
|******************************************************************************/
void BTRM_10msMainFunction(void)
{
	BTRCTR_10msMainFunction();
}
/*EOF*/
