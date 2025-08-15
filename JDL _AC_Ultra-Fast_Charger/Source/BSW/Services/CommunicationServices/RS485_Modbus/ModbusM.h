//****************************************************************************************
//*
//* File Name: Modbus_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (__MODBUS_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __MODBUS_H
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
typedef enum
{
	MODBUS_STATE_IDLE = 0X00,	/* Idle state */
	MODBUS_STATE_RX_PENDING,	/* The status of to be receive response frame */
	MODBUS_STATE_RX_CHECK,		/* The status of response frame check */
	MODBUS_STATE_EXEC,			/* Valid frame process */
	MODBUS_STATE_REC_ERR,		/* Error frame process */
	MODBUS_STATE_TIMES_ERR, 	/* Timeout processing */
} Modbus_MF_Status_Enum;

typedef enum
{
	MODBUS_CMD_READ_COIL_STATUS = 0x01,			/* Read Coil Status */
	MODBUS_CMD_READ_INPUT_STATUS = 0x02,		/* Read Input Status */
	MODBUS_CMD_READ_HOLDING_REGISTERS = 0x03,	/* Read Holding Registers */
	MODBUS_CMD_READ_INPUT_REGISTERS = 0x04,		/* Read Input Registers */
	MODBUS_CMD_WRITE_SINGLE_COIL = 0x05,		/* Write Single Coil */
	MODBUS_CMD_WRITE_SINGLE_REGISTER = 0x06,	/* Write Single Register */
	MODBUS_CMD_WRITE_MULTIPLE_COILS = 0x0F,		/* Write Multiple Coils */
	MODBUS_CMD_WRITE_MULTIPLE_REGISTERS = 0x10, /* Write Multiple Registers */
} Modbus_Cmd_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void ModbusM_Init(void);
extern void ModbusM_Reset(void);
extern McalRetVal_t ModbusM_Send(ModbusChannel_t Channel, uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t data_len);
extern void ModbusM_10msMainFunction(void);

#endif
/* EOL */
