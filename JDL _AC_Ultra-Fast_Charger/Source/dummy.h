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
#include "Cloud_Protocol_Sg_ChargingOrder_Cfg.h"

extern uint8_t Dummy_RemoteSuspendStatus(void);
extern uint8_t Dummy_GetTemp(uint8_t tempSensorId);
#endif
