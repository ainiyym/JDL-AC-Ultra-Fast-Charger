//****************************************************************************************
//*
//* File Name: dummy.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#include "dummy.h"
uint8_t Dummy_RemoteSuspendStatus(void)
{
    return 0; // Dummy implementation
}

uint8_t Dummy_GetMeterReadyStatus(SysConnector_Num_Enum ch)
{
    return 1; // Dummy implementation, assuming meter is always ready
}

uint8_t Dummy_GetCurrL1(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for current L1
}

uint8_t Dummy_GetCurrL2(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for current L2
}

uint8_t Dummy_GetCurrL3(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for current L3
}

uint8_t Dummy_GetPowerDownStatus(void)
{
    return 0; // Dummy implementation, returning 0 for power down status
}

uint8_t Dummy_GetPowerDownVoltValue(void)
{
    return 0; // Dummy implementation, returning 0 for power down voltage value
}
