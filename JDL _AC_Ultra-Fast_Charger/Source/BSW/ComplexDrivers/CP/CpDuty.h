//****************************************************************************************
//*
//* File Name: CpDuty.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#if !defined (_CP_DUTY_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */
#define _CP_DUTY_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "CpDuty_Cfg.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void CPD_InitMemory(void);
extern void CPD_MainFunction(SysConnector_Num_Enum ch);
extern void CPD_Open(SysConnector_Num_Enum ch);
extern void CPD_Close(SysConnector_Num_Enum ch);
extern void CPD_OutputEnable(SysConnector_Num_Enum ch);
extern void CPD_OutputDisable(SysConnector_Num_Enum ch);
extern void CPD_ConfigCurrentValue(SysConnector_Num_Enum ch, uint16_t lv_usCurrValue);
extern void CPD_AdjustCurrentValue(SysConnector_Num_Enum ch, uint8_t lv_ucPercent, uint8_t lv_ucEn);
extern uint32_t CPD_GetCpDuty(SysConnector_Num_Enum ch);
extern uint32_t CPD_GetCpFreq(SysConnector_Num_Enum ch);
extern void CPD_OutputNegative12VEnable(SysConnector_Num_Enum ch);
extern void CPD_OutputNegative12VDisable(SysConnector_Num_Enum ch);
#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
extern void CPD_ReqCarDiodeDetect(void);
extern uint8_t CPD_CarDiodeDetectResult(void);
extern uint8_t CPD_DiodeDetectFlag(void);
#endif
#endif
/*EOF*/
