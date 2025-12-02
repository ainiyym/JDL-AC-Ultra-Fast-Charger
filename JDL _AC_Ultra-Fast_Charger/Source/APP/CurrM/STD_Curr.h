//****************************************************************************************
//*
//* File Name: STD_Curr_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_CURR_H)
#define _STD_CURR_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Curr_Cfg.h"

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
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global KAM with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

extern void CURR_InitMemory(void);
extern void CURR_Enable(void);
extern void CURR_Disable(void);
extern void CURR_20msFunctionControl(void);
extern void CURR_AdjRemoteCurr(SysConnector_Num_Enum ch, uint16_t lv_ucPercent, uint8_t lv_ucEn);
extern uint8_t CURR_GetRemoteSuspendStatus(SysConnector_Num_Enum ch);
extern void CURR_SetDfltCurrVal(SysConnector_Num_Enum ch, uint16_t lv_ucCurrentVal);
extern uint8_t CURR_GetOutPutModeType(SysConnector_Num_Enum ch);
extern uint16_t CURR_GetL1Value(SysConnector_Num_Enum ch);
extern uint16_t CURR_GetL2Value(SysConnector_Num_Enum ch);
extern uint16_t CURR_GetL3Value(SysConnector_Num_Enum ch);
#endif
/*EOF*/



