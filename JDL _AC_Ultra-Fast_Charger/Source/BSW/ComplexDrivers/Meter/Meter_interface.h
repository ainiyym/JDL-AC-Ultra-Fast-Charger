//******************************************************************************
//* File Name: Meter_interface.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module header file
/*******************************************************************************/
#if !defined (__METER_INTERFACE_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_INTERFACE_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_config.h"
#include "STD_Rtc.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define METER_GET_CURRENT_TIMESTAMP()    xTaskGetTickCount()  // get current timestamp in ms

#define Meter_SendModbusRequest(slave_addr, modbus_cmd, request_data, data_len) \
    ModbusM_Send(MODBUS_CHANNEL_METER, slave_addr, modbus_cmd, request_data, data_len) // send read request
#define Meter_GetModbusReady() \
    ModbusM_GetReadyStatus(MODBUS_CHANNEL_METER) // check Modbus channel ready status
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

#endif /* __METER_INTERFACE_H */
/* EOL */
