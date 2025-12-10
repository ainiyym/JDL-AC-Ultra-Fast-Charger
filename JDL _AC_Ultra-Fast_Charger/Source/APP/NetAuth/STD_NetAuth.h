//****************************************************************************************
//*
//* File Name: STD_NetAuth.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef _STD_NETAUTH_H
#define _STD_NETAUTH_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_NetAuth_Cfg.h"
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
|    Global Function Prototypes
|******************************************************************************/
extern void NETAUTH_Enable(SysConnector_Num_Enum ch);
extern void  NETAUTH_Disable(SysConnector_Num_Enum ch);
extern void  NETAUTH_InitMemory(void);
extern void  NETAUTH_MainFunction(void);
extern uint8_t NETAUTH_GetCancelAuthStatus(SysConnector_Num_Enum ch);
extern uint8_t NETAUTH_GetAuthStatus(SysConnector_Num_Enum ch);
extern void NETAUTH_SetReqAuthStatus(SysConnector_Num_Enum ch);
extern void NETAUTH_ResetAuthStatus(SysConnector_Num_Enum ch);
extern void NETAUTH_SetReqCancelAuthStatus(SysConnector_Num_Enum ch);
#endif /*_STD_NETAUTH_H */



















