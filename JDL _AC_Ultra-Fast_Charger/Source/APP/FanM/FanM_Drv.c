//******************************************************************************
//* File Name: FanM_Drv.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module driver file
/*******************************************************************************/
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FanM_Drv.h"
#include "Mcal_BigLittle_Endian.h"

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
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
bool FanM_Storage_Write(const FanM_HistoryRecord_t *record)
{
    
    return true;
}

bool FanM_Storage_GetUsedCount(uint32_t *count)
{
    return true;
}

bool FanM_Storage_Clear(void)
{

    return true;
}

/* 构建状态读取请求数据 */
void FanM_BuildStatusReadRequest(uint8_t *data, uint8_t *len)
{
    if (data == NULL || len == NULL) return;
    
    data[0] = FANM_REG_STATUS_START_ADDR >> 8;      /* 起始地址高字节 */
    data[1] = FANM_REG_STATUS_START_ADDR & 0xFF;    /* 起始地址低字节 */
    data[2] = FANM_REG_STATUS_COUNT >> 8;           /* 寄存器数量高字节 */
    data[3] = FANM_REG_STATUS_COUNT & 0xFF;         /* 寄存器数量低字节 */
    
    *len = 4;
}

/* 构建控制命令数据 */
void FanM_BuildControlCommand(uint8_t cmd, uint8_t fan_speed, 
                                uint8_t pump1_speed, uint8_t pump2_speed,
                                uint8_t *data, uint8_t *len)
{
    if (data == NULL || len == NULL) return;

    data[0] = FANM_REG_CONTROL_START_ADDR >> 8;   /* 起始地址高字节 */
    data[1] = FANM_REG_CONTROL_START_ADDR & 0xFF; /* 起始地址低字节 */
    data[2] = FANM_REG_CONTROL_COUNT >> 8;        /* 寄存器数量高字节 */
    data[3] = FANM_REG_CONTROL_COUNT & 0xFF;      /* 寄存器数量低字节 */
    data[4] = FANM_REG_CONTROL_BYTE_COUNT;        /* 字节数 */
    data[5] = cmd;                                /* 运行命令 */
    data[6] = fan_speed;                          /* 风扇速度设置 */
    data[7] = pump1_speed;                        /* 泵1速度设置 */
    data[8] = pump2_speed;                        /* 泵2速度设置 */

    /* 预留字节清零 */
    for (int i = 9; i < 15; i++) {
        data[i] = 0x00;
    }
    
    *len = 15;
}

McalRetVal_t FanM_SendControlCommand(uint8_t cmd)
{
	uint8_t control_data[15] = {0};
	uint8_t data_len;

	FanM_BuildControlCommand(cmd, 0, 0, 0, control_data, &data_len);
	McalRetVal_t ret = FANM_SEND_MODBUS(FANM_MODBUS_ADDR, FANM_MODBUS_CMD_WRITE_MULTIPLE_REGISTERS, control_data, data_len);

	return ret;
}

McalRetVal_t FanM_ManualControl(uint8_t fan_speed, uint8_t pump1_speed, uint8_t pump2_speed)
{
	uint8_t control_data[15] = {0};
	uint8_t data_len;

	FanM_BuildControlCommand(FANM_CMD_DEBUG_START, fan_speed, pump1_speed, pump2_speed, control_data, &data_len);

	McalRetVal_t ret = FANM_SEND_MODBUS(FANM_MODBUS_ADDR, FANM_MODBUS_CMD_WRITE_MULTIPLE_REGISTERS, control_data, data_len);

	return ret;
}

McalRetVal_t FanM_RebootDevice(void)
{
    return FanM_SendControlCommand(FANM_CMD_REBOOT);
}

/* EOL */
