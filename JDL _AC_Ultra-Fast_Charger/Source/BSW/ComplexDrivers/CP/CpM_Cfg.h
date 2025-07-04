//****************************************************************************************
//*
//* File Name: CpM_Cfg.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (_CPM_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _CPM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "CpDuty.h"
#include "CpVolt.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CPM_GetSysReadyState            SYSM_GetResetPrepareStatus		/*get system ready state interface*/
#define CPM_OutputEnable				CPD_OutputEnable			/*pwm output enable interface*/
#define CPM_OutputDisable				CPD_OutputDisable			/*pwm output disable interface*/
#define CPM_ConfigCurrentValue			CPD_ConfigCurrentValue		/*config current value interface*/
#define CPM_AdjustCurrentValue			CPD_AdjustCurrentValue		/*adjust current value interface*/
#define CPM_GetCpDuty					CPD_GetCpDuty				/*get cp duty interface*/
#define CPM_GetCpFreq	            	CPD_GetCpFreq				/*get cp freq interface*/
#define CPM_GetCpVoltStatus				CPV_GetCpVoltStatus			/*get cp volt status interface*/
#define CPM_GetCpVoltage				CPV_GetCpVoltage			/*get cp voltage interface*/
#define CPM_OutputNegative12VEnable     CPD_OutputNegative12VEnable /* Cp output -12V */
#define CPM_OutputNegative12VDisable    CPD_OutputNegative12VDisable /* Cp stop output -12V */  

#if (STD_ON == CPD_DIODE_DETECT_FUN_EN)
#define CPM_ReqCarDiodeDetect			CPD_ReqCarDiodeDetect
#define	CPM_CarDiodeDetectResult		CPD_CarDiodeDetectResult
#endif
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
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/

