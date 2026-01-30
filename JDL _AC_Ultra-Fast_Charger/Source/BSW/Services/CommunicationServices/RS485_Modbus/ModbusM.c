//******************************************************************************
//*
//* File Name: ModbusM_.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "ModbusM.h"
#include "Modbus_Cfg.h"
#include "Modbus_Hook.h"
#include "Lib_Crc.h"
#include "stdint.h"
#include "string.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t USART_Channel;
	uint8_t state;			   // modbus host state
	uint8_t errTimes;		   // Count of failures
	uint8_t *txBuf;			   // Send buffer
	uint8_t *rxBuf;			   // Receive buffer
	uint16_t txLen;			   // Length of frame to be sent
	uint16_t rxCounter;		   // Receive counter
	McalTickCount_t rxTimeOut; // Receive timeout counter
} ModbusRtus_Ctrl_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static ModbusRtus_Ctrl_Struct ModbusRtu[MODBUS_CHANNEL_MAX_NUM] = {0};

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
extern const Modbus_Config_t ModbusUartConfigValue[MODBUS_CHANNEL_MAX_NUM]; 

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static McalRetVal_t ModbusM_SendCallFunc(ModbusChannel_t Channel);
static void ModbusM_ExecuteHandle(ModbusChannel_t Channel, uint8_t *pframe, uint16_t len);
static void ModbusM_ErrHandle(ModbusChannel_t Channel, uint8_t addr, uint8_t cmd);
static void ModbusM_Timeout_Handler(ModbusChannel_t Channel);
static void ModbusM_ClearTimeout_Handler(ModbusChannel_t Channel);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void ModbusM_Init(void)
{
	memset(ModbusRtu, 0U, sizeof(ModbusRtu));
	for (uint8_t i = 0; i < (uint8_t)MODBUS_CHANNEL_MAX_NUM; i++)
	{
		ModbusRtu[i].USART_Channel = ModbusUartConfigValue[i].USART_Channel;
		ModbusRtu[i].rxBuf = ModbusUartConfigValue[i].RcvProcessFrameBuf;
		ModbusRtu[i].txBuf = ModbusUartConfigValue[i].SndBlockFrameBuf;
	}
}

void ModbusM_Reset(void)
{
	ModbusM_Init();
}

uint8_t ModbusM_GetReadyStatus(ModbusChannel_t Channel)
{
	if (ModbusRtu[Channel].state == MODBUS_STATE_IDLE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static McalRetVal_t ModbusM_SendCallFunc(ModbusChannel_t Channel)
{
	McalRetVal_t ret = MCAL_RET_SUCCESS;

	if (MCAL_RET_SUCCESS == Mcal_Usart_AppSendData(ModbusRtu[Channel].USART_Channel, ModbusRtu[Channel].txBuf, ModbusRtu[Channel].txLen))
	{
		ModbusRtu[Channel].state = MODBUS_STATE_RX_PENDING;
	}
	else
	{
		ret =  MCAL_RET_UNEXPECTED_ERROR;
	}

	return ret;
}

/**
 * 	@brief  The MODBUS master sends a command to the slave
 * 	@param	Channel: Serial port channel number
 * 	@param	addr: Slave address
 * 	@param 	cmd: Function code
 *	@param	data: Data to be sent
 *	@param	len: Length of data to be sent
 * 	@return	McalRetVal_t
 * 	@note	This function is non-blocking and returns immediately after being called
 */
McalRetVal_t ModbusM_Send(ModbusChannel_t Channel, uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t data_len)
{
	uint16_t crc = 0;

	if (ModbusRtu[Channel].state != MODBUS_STATE_IDLE)
		return MCAL_RET_USART_SEND_BUSY; // busy state
	if (data_len + 4 > ModbusUartConfigValue[Channel].SndBlockFrameBufLen)
		return MCAL_RET_USART_SEND_CYCBUF_OVERFLOW; // no free buff

	ModbusRtu[Channel].rxCounter = 0;
	ModbusRtu[Channel].txBuf[0] = addr;
	ModbusRtu[Channel].txBuf[1] = cmd;
	memcpy((ModbusRtu[Channel].txBuf + 2), data, data_len);
	ModbusRtu[Channel].txLen = data_len + 2; // data(n)+add(1)+cmd(1)
	crc = Lib_Crc16(ModbusRtu[Channel].txBuf, ModbusRtu[Channel].txLen);
	ModbusRtu[Channel].txBuf[ModbusRtu[Channel].txLen++] = (uint8_t)(crc & 0xff);
	ModbusRtu[Channel].txBuf[ModbusRtu[Channel].txLen++] = (uint8_t)(crc >> 8);

	// MODBUS_INFO("%s <channel:%d> Send Modbus Frame: ", __func__, Channel);
	// MODBUS_PRINT_HEX(ModbusRtu[Channel].txBuf, ModbusRtu[Channel].txLen);
	
	if (MCAL_RET_SUCCESS == ModbusM_SendCallFunc(Channel))
	{
		return MCAL_RET_SUCCESS;
	}
	else
	{
		return MCAL_RET_UNEXPECTED_ERROR;
	}
}

static void ModbusM_Timeout_Handler(ModbusChannel_t Channel)
{
	if (ModbusRtu[Channel].rxTimeOut++ > ModbusUartConfigValue[Channel].RcvBlockFrameOverTime)
	{
		ModbusRtu[Channel].state = MODBUS_STATE_REC_ERR;
		ModbusRtu[Channel].rxTimeOut = 0;
		// MODBUS_ERROR("%s <channel:%d> ERR!!!\r\n", __func__, Channel);
	}
}

static void ModbusM_ClearTimeout_Handler(ModbusChannel_t Channel)
{
	ModbusRtu[Channel].rxTimeOut = 0;
	ModbusRtu[Channel].state = MODBUS_STATE_RX_CHECK;
	// MODBUS_DEBUG("%s <channel:%d> go to MODBUS_STATE_RX_CHECK \r\n", __func__, Channel);
}

static void ModbusM_ExecuteHandle(ModbusChannel_t Channel, uint8_t *pframe, uint16_t len)
{
	uint8_t datalen = len - 2;
	switch (pframe[1]) // cmd
	{
	case MODBUS_CMD_READ_COIL_STATUS:
		Modbus_Hook_Rec01(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_READ_INPUT_STATUS:
		Modbus_Hook_Rec02(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_READ_HOLDING_REGISTERS:
		Modbus_Hook_Rec03(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_READ_INPUT_REGISTERS:
		Modbus_Hook_Rec04(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_WRITE_SINGLE_COIL:
		Modbus_Hook_Rec05(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_WRITE_SINGLE_REGISTER:
		Modbus_Hook_Rec06(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_WRITE_MULTIPLE_COILS:
		Modbus_Hook_Rec0f(Channel, pframe[0], (pframe + 2), datalen);
		break;
	case MODBUS_CMD_WRITE_MULTIPLE_REGISTERS:
		Modbus_Hook_Rec10(Channel, pframe[0], (pframe + 2), datalen);
		break;
	}
}

static void ModbusM_ErrHandle(ModbusChannel_t Channel, uint8_t addr, uint8_t cmd)
{
	Modbus_Hook_ErrHandle(Channel, addr, cmd);
}

static void ModbusM_MainCtrl(ModbusChannel_t Channel)
{
	uint16_t RcvLen = 0;

	switch (ModbusRtu[Channel].state)
	{
	// Wait for receiving data
	case MODBUS_STATE_RX_PENDING:
		RcvLen = Mcal_Usart_AppReceiveData(ModbusRtu[Channel].USART_Channel, &ModbusRtu[Channel].rxBuf[ModbusRtu[Channel].rxCounter], ModbusUartConfigValue[Channel].RcvProcessFrameBufLen);
		if (0 != RcvLen)
		{
			ModbusRtu[Channel].rxCounter += RcvLen;
			break;
		}

		if (0 == ModbusRtu[Channel].rxCounter)
		{
			ModbusM_Timeout_Handler(Channel);
		}
		else
		{
			ModbusM_ClearTimeout_Handler(Channel);
		}
		break;
	/* After receiving a frame of data, the verification process begins */
	case MODBUS_STATE_RX_CHECK:
		if ((ModbusRtu[Channel].rxCounter >= MODBUS_RTU_MIN_SIZE) &&
			((Lib_Crc16(ModbusRtu[Channel].rxBuf, ModbusRtu[Channel].rxCounter - 2) == (uint16_t)(((uint16_t)ModbusRtu[Channel].rxBuf[ModbusRtu[Channel].rxCounter - 2]) | ModbusRtu[Channel].rxBuf[ModbusRtu[Channel].rxCounter - 1] << 8))))
		{
			/* Valid frame processing */
			if ((ModbusRtu[Channel].txBuf[0] == ModbusRtu[Channel].rxBuf[0]) && (ModbusRtu[Channel].txBuf[1] == ModbusRtu[Channel].rxBuf[1])) // The address and function code of the sent frame data and the received frame data are the same
			{
				ModbusRtu[Channel].state = MODBUS_STATE_EXEC;
			}
			else
			{
				ModbusRtu[Channel].state = MODBUS_STATE_REC_ERR;
			}
		}
		else
		{
			ModbusRtu[Channel].state = MODBUS_STATE_REC_ERR;
		}
		break;
	/* receiving a error frame */
	case MODBUS_STATE_REC_ERR:
		ModbusRtu[Channel].errTimes++;
		if (ModbusRtu[Channel].errTimes >= MODBUS_BLOCKING_FRAME_RESEND_NUM)
		{
			ModbusRtu[Channel].state = MODBUS_STATE_TIMES_ERR;
			ModbusRtu[Channel].errTimes = 0;
		}
		else // Restart the transmission once again
		{
			ModbusRtu[Channel].rxCounter = 0;
			ModbusM_SendCallFunc(Channel);
		}
		break;
	/*Exceeded the maximum number of erroneous transmissions*/
	case MODBUS_STATE_TIMES_ERR:
		ModbusM_ErrHandle(Channel, ModbusRtu[Channel].txBuf[0], ModbusRtu[Channel].txBuf[1]);
		ModbusRtu[Channel].state = MODBUS_STATE_IDLE;
		ModbusRtu[Channel].rxCounter = 0;
		break;
	/* Make sure to receive the correct execution callback */
	case MODBUS_STATE_EXEC:
		ModbusM_ExecuteHandle(Channel, ModbusRtu[Channel].rxBuf, ModbusRtu[Channel].rxCounter);
		ModbusRtu[Channel].state = MODBUS_STATE_IDLE;
		ModbusRtu[Channel].rxCounter = 0;
		break;
	}
}

void ModbusM_10msMainFunction(void)
{
	for (uint8_t i = 0; i < (uint8_t)MODBUS_CHANNEL_MAX_NUM; i++)
	{
		ModbusM_MainCtrl((ModbusChannel_t)i);
	}
}
/* EOL */
