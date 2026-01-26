//****************************************************************************************
//*
//* File Name: STD_ErrorHandler.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_ErrorHandler_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define ERRHDL_CHARGE_ALLOW				(0u)
#define ERRHDL_CHARGE_DERATE			(1u)
#define ERRHDL_CHARGE_SUSPENDED			(2u)
#define ERRHDL_CHARGE_CANCEL			(3u)
#define ERRHDL_CHARGE_SAFETY			(4u)
/*******************************************************************************
|    LOWER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    UPPER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    Callback Notification
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void ERRHDL_InitMemory(void);
extern void ERRHDL_ClearAuthInfor(void);
extern void ERRHDL_10msMainFunction(void);
extern void ERRHDL_FaultStatusUpdata_CallBack(ErrHdlId_Enum lv_enFaultIdNum,uint8_t lv_ucFaultStatus);
extern ErrHdlLevel_Enum ERRHDL_GetErrorLevelStatus(void);
extern void ERRHDL_GetAllFaultData(uint8_t *lv_arrBuff);
extern uint8_t ERRHDL_GetChargeConditions(uint32_t gun_index);
extern void ERRHDL_Disable(void);
extern void ERRHDL_Enable(void);
#endif
/*EOF*/
