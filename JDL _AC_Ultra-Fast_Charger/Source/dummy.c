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

uint8_t Dummy_GetTemp(uint8_t tempSensorId)
{
    return 25; // Dummy implementation, returning a constant temperature value
}

