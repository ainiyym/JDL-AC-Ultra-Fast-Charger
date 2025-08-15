//****************************************************************************************
//*
//* File Name: Modbus_Hook.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (__MODBUS_HOOK_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __MODBUS_HOOK_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "Modbus_Cfg.h"

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
void Modbus_Hook_Rec01(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec02(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec03(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec04(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec05(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec06(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec0f(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_Rec10(ModbusChannel_t Channel, uint8_t addr,uint8_t *data,uint8_t datalen);
void Modbus_Hook_ErrHandle(ModbusChannel_t Channel, uint8_t addr,uint8_t cmd);

#endif
/* EOL */
