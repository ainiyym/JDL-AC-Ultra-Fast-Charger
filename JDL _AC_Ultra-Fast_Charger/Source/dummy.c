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

uint8_t Dummy_GetVoltL1(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for voltage L1
}

uint8_t Dummy_GetVoltL2(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for voltage L2
}

uint8_t Dummy_GetVoltL3(SysConnector_Num_Enum ch)
{
    return 0; // Dummy implementation, returning 0 for voltage L3
}

uint8_t Dummy_GetTemp(uint8_t tempSensorId)
{
    return 25; // Dummy implementation, returning a constant temperature value
}

char* Dummy_GetInMeterAddr(SysConnector_Num_Enum ch)
{
    if (SYS_CONNECTOR1 == ch)
    {
        return "1"; // Dummy implementation, returning 0 for internal meter address
    }
    else
    {
        return "2"; // Dummy implementation, returning 0 for internal meter address
    }

}

char* Dummy_GetOutMeterAddr(SysConnector_Num_Enum ch)
{
    if (SYS_CONNECTOR1 == ch)
    {
        return "3"; // Dummy implementation, returning 0 for external meter address
    }
    else
    {
        return "4"; // Dummy implementation, returning 0 for external meter address
    }
}

void Dummy_GetMeterInfo(cloud_protocol_sg_order_measure_value_t *meter_info)
{
    if (meter_info != NULL)
    {
        for (int i = 0; i < V2G_MAX_VOL_CUR_DATA_LEN; i++)
        {
            meter_info->voltage[i] = 220 + i; // Dummy voltage values
            meter_info->current[i] = 32 + i;  // Dummy current values
        }
        for (int j = 0; j < V2G_MAX_POWER_DATA_LEN; j++)
        {
            meter_info->power[j] = 7000 + j;     // Dummy power values
        }
    }
}
