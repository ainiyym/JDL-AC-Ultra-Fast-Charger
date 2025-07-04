//****************************************************************************************
//*
//* File Name: STD_RlyM.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_RLYM_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Justified> NO fluence */
#define _STD_RLYM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "STD_RlyCtrl.h"
#include "dummy.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

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
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void  RELAYM_InitMemory( void );
extern void  RELAYM_ForceRelaySwitchOff(SysConnector_Num_Enum ch);
extern void  RELAYM_5msMainFunction( void );
extern uint8_t RELAYM_ReqRelaySwitchOn(SysConnector_Num_Enum ch);
extern uint8_t RELAYM_ReqRelaySwitchOff(SysConnector_Num_Enum ch);
extern uint8_t RELAYM_GetRelayStatus(SysConnector_Num_Enum ch);
extern void  RELAYM_ShortSelfCheckReq(SysConnector_Num_Enum ch);
extern uint8_t RELAYM_ShortSelfCheckGetRelayStatus(SysConnector_Num_Enum ch);
#endif
/*EOF*/

