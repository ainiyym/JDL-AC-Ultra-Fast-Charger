//****************************************************************************************
//*
//* File Name: dummy.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _DUMMY_H
#define _DUMMY_H
#include "Mcal_App.h"


typedef enum
{
	SYS_CONNECTOR1, /* Connector 1 */
	SYS_CONNECTOR2, /* Connector 2 */
	SYS_CONNECTOR_NUM_MAX
} SysConnector_Num_Enum;

extern uint8_t Sys_GetSysReadyState(void);

#endif
