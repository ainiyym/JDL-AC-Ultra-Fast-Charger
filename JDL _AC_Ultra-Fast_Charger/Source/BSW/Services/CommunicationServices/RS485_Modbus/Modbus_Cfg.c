//******************************************************************************
//*
//* File Name: Modbus_Cfg.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "MCAL_Usart_Cfg.h"
#include "Modbus_Cfg.h"

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
static uint8_t Modbus_CH4_SendBlockFrameBuf[MODBUS_CH4_SND_BLOCK_FRAME_BUF_LEN];
static uint8_t Modbus_CH4_RcvProcessFrameBuf[MODBUS_CH4_RCV_PROC_FRAME_BUF_LEN];

static uint8_t Modbus_CH5_SendBlockFrameBuf[MODBUS_CH5_SND_BLOCK_FRAME_BUF_LEN];
static uint8_t Modbus_CH5_RcvProcessFrameBuf[MODBUS_CH5_RCV_PROC_FRAME_BUF_LEN];   

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
const Modbus_Config_t ModbusUartConfigValue[MODBUS_CHANNEL_MAX_NUM] =
    {
        {MCAL_USART4_CH, Modbus_CH4_SendBlockFrameBuf, MODBUS_CH4_SND_BLOCK_FRAME_BUF_LEN, Modbus_CH4_RcvProcessFrameBuf, MODBUS_CH4_RCV_PROC_FRAME_BUF_LEN, MODBUS_RECEIVE_RESPONSE_TIMEOUT_TIME},
        {MCAL_USART5_CH, Modbus_CH5_SendBlockFrameBuf, MODBUS_CH5_SND_BLOCK_FRAME_BUF_LEN, Modbus_CH5_RcvProcessFrameBuf, MODBUS_CH5_RCV_PROC_FRAME_BUF_LEN, MODBUS_RECEIVE_RESPONSE_TIMEOUT_TIME},
};
/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/* EOL */
