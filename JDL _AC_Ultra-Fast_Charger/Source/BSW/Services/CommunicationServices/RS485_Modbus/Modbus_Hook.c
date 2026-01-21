//******************************************************************************
//*
//* File Name: ModbusM_Hook.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Modbus_Hook.h"
#include "FanM.h"
#include "Meter.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Modbus_Hook_Rec01(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        break;
    default:
        break;    
    }
}

void Modbus_Hook_Rec02(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        break;
    default:
        break;
    }
}

void Modbus_Hook_Rec03(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    // MODBUS_INFO("%s <channel:%d> Rcv Modbus Frame: ", __func__, Channel);
	// MODBUS_PRINT_HEX(data, datalen);

    switch (Channel)
    {
        case MODBUS_CHANNEL_METER:
            /* code */
            MeterModule_HandleResponse(addr, data, datalen);
            break;
        case MODBUS_CHANNEL_FAN:
            /* code */
            break;
        default:
            break;
    }
}

void Modbus_Hook_Rec04(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        FanM_HandleModbusResponse(addr,MODBUS_CMD_READ_INPUT_REGISTERS,data,datalen);
        break;
    default:
        break;
    }
}

void Modbus_Hook_Rec05(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        break;
    default:
        break;
    }
}

void Modbus_Hook_Rec06(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        break;
    case MODBUS_CHANNEL_FAN:
        break;
    default:
        break;
    }
}

void Modbus_Hook_Rec0f(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        break;
    default:
        break;
    }
}

void Modbus_Hook_Rec10(ModbusChannel_t Channel, uint8_t addr, uint8_t *data, uint8_t datalen)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        FanM_HandleModbusResponse(addr,MODBUS_CMD_WRITE_MULTIPLE_REGISTERS,data,datalen);
        break;
    default:
        break;
    }
}

void Modbus_Hook_ErrHandle(ModbusChannel_t Channel, uint8_t addr, uint8_t cmd)
{
    switch (Channel)
    {
    case MODBUS_CHANNEL_METER:
        /* code */
        MeterModule_ErrHandleCallback();
        break;
    case MODBUS_CHANNEL_FAN:
        /* code */
        break;
    default:
        break;
    }
}
