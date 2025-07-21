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
//****************************************************************************************
#include "stdint.h"
#include "STD_SysM_Cfg.h"

extern uint8_t Dummy_RemoteSuspendStatus(void);
extern uint8_t Dummy_GetMeterReadyStatus(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetCurrL1(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetCurrL2(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetCurrL3(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetPowerDownStatus(void);
extern uint8_t Dummy_GetPowerDownVoltValue(void);
extern uint8_t Dummy_GetVoltL1(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetVoltL2(SysConnector_Num_Enum ch);
extern uint8_t Dummy_GetVoltL3(SysConnector_Num_Enum ch);
#endif
