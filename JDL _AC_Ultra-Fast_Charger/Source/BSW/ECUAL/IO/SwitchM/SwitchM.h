//****************************************************************************************
//*
//* File Name: SwitchM.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _SWITCHM_H
#define _SWITCHM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "SwitchM_Cfg.h"
#include "STD_SysM.h"
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
|    Global Function Prototypes
|******************************************************************************/
extern void SwitchM_Init(void);
extern void SwitchM_Set74hct4851dEnable(void);
extern void SwitchM_Set74hct4851dDisable(void);
extern uint8_t SwitchM_Get74hct4851dOutputStatusCallBack(SwitchM_74HCT4851D_Multiplex_e CH);
extern void SwitchM_74hct4851dControlCallBack(void);
extern void SwitchM_SoftTimerStart74hct4851d_Enable(void);
// extern void SwitchM_Set74hct4851dOutputA0(void);
// extern void SwitchM_Set74hct4851dOutputA1(void);
// extern void SwitchM_Set74hct4851dOutputA2(void);
// extern void SwitchM_Set74hct4851dOutputA3(void);
// extern void SwitchM_Set74hct4851dOutputA4(void);
// extern void SwitchM_Set74hct4851dOutputA5(void);
// extern void SwitchM_Set74hct4851dOutputA6(void);
// extern void SwitchM_Set74hct4851dOutputA7(void);
#endif
/*EOF*/
