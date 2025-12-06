//****************************************************************************************
//*
//* File Name: Modbus_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (__MODBUS_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __MODBUS_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "STD_LogService.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MODBUS_DEBUG(fmt, ...) 	LOG_DEBUG(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__)
#define MODBUS_ERROR(fmt, ...) 	LOG_ERROR(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__)

#define MODBUS_BLOCKING_FRAME_RESEND_NUM  (3)  	/* Frame retransmission times */
#define MODBUS_RTU_MIN_SIZE	(4)

#define MODBUS_CH4_SND_BLOCK_FRAME_BUF_LEN   			(128 + 4)
#define MODBUS_CH4_RCV_PROC_FRAME_BUF_LEN   			(128)

#define MODBUS_CH5_SND_BLOCK_FRAME_BUF_LEN   			(128 + 4)
#define MODBUS_CH5_RCV_PROC_FRAME_BUF_LEN   			(128)

#define MODBUS_TASK_PERIOD								(10U)
#define MODBUS_RECEIVE_RESPONSE_TIMEOUT_TIME 			(300U / MODBUS_TASK_PERIOD)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum {
	MODBUS_CHANNEL_METER,
	MODBUS_CHANNEL_FAN,	
	MODBUS_CHANNEL_MAX_NUM
}ModbusChannel_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* 下面 Modbus_Config_t 结构体禁止配置修改！！！*/
typedef struct {
	uint8_t  USART_Channel;					/* 串口通道号，见Mcal_Usart_Cfg.h中定义 */
    uint8_t* SndBlockFrameBuf;
	uint16_t SndBlockFrameBufLen;
	uint8_t* RcvProcessFrameBuf;
	uint16_t RcvProcessFrameBufLen;
	uint16_t RcvBlockFrameOverTime;	 		/* 单位10ms */
}Modbus_Config_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/* EOL */
