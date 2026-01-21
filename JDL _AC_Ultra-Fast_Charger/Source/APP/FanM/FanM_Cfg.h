//******************************************************************************
//* File Name: FanM_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module header file
/*******************************************************************************/
#if !defined (__FANM_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FANM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "STD_LogService.h"
#include "ModbusM.h"
#include "Sensor.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* 模块功能配置开关 */
#define FANM_READ_STATUS_ENABLED    1   /* 启用状态读取功能 */
#define FANM_CONTROL_ENABLED        1   /* 启用控制功能 */
#define FANM_HISTORY_STORE_ENABLED  0   /* 启用历史数据存储 */
#define FANM_FAULT_HANDLING_ENABLED 1   /* 启用故障处理功能 */
#define FANM_MODBUS_RESPONSE_HANDLER 1  /* 启用Modbus响应处理 */

/* 时间配置（单位：ms） */
#define FANM_TASK_PERIOD_MS         100                          /* 模块任务周期 */
#define FANM_READ_INTERVAL_MS       (1000 / FANM_TASK_PERIOD_MS) /* 状态读取间隔 */
#define FANM_CONTROL_INTERVAL_MS    (2000 / FANM_TASK_PERIOD_MS) /* 控制命令发送间隔 */
#define FANM_STORE_INTERVAL_MS      (10000 / FANM_TASK_PERIOD_MS)/* 历史数据存储间隔 */
#define FANM_CONTROL_BUSY_TIME      (5)                          /* 控制命令忙碌超时计数 */
/* Modbus配置 */
#define FANM_MODBUS_ADDR            0xA2 /* 设备地址 */
#define FANM_MODBUS_CHANNEL         MODBUS_CHANNEL_FAN /* 使用的Modbus通道 */

/* Modbus功能码定义 */
#define FANM_MODBUS_CMD_READ_INPUT_REGISTERS      MODBUS_CMD_READ_INPUT_REGISTERS
#define FANM_MODBUS_CMD_WRITE_MULTIPLE_REGISTERS  MODBUS_CMD_WRITE_MULTIPLE_REGISTERS

/* 寄存器地址定义 */
#define FANM_REG_STATUS_START_ADDR      0x0000  /* 状态寄存器起始地址 */
#define FANM_REG_STATUS_COUNT           0x0019  /* 状态寄存器数量(25个) */
#define FANM_REG_CONTROL_START_ADDR     0x0110  /* 控制寄存器起始地址 */
#define FANM_REG_CONTROL_COUNT          0x0005  /* 控制寄存器数量(5个) */
#define FANM_REG_CONTROL_BYTE_COUNT     0x0A    /* 控制数据字节数(10字节) */

/* 控制命令值 */
#define FANM_CMD_STOP                   0x00
#define FANM_CMD_AUTO_START             0x01
#define FANM_CMD_FULL_SPEED             0x02
#define FANM_CMD_DEBUG_START            0x03
#define FANM_CMD_REBOOT                 0xFE

/* 故障等级定义 */
#define FANM_FAULT_NONE                 0   /* 无故障 */
#define FANM_FAULT_URGENT_REPAIR        20  /* 尽快检修 */
#define FANM_FAULT_IMMEDIATE_STOP       30  /* 立即停机 */

/* 液位值定义 */
#define FANM_LIQUID_LEVEL_VERY_LOW          0   /* 极低 */
#define FANM_LIQUID_LEVEL_LOW               1   /* 较低 */
#define FANM_LIQUID_LEVEL_MEDIUM            2   /* 中等 */
#define FANM_LIQUID_LEVEL_HIGH              3   /* 最高 */
#define FANM_LIQUID_LEVEL_FAULT             0xFE /* 故障 */
#define FANM_LIQUID_LEVEL_NO_HARDWARE       0xFF /* 无硬件 */

/* 历史数据存储配置 */
#define FANM_HISTORY_MAX_RECORDS    60480 /* 7天记录数(10秒一条: 6*60*24*7) */
#define FANM_HISTORY_RECORD_SIZE    (50 + 8) /* 50字节数据 + 8字节时间戳 */

/* 特殊值定义 */
#define FANM_VALUE_NO_HARDWARE      0xFF  /* 无硬件 */
#define FANM_VALUE_SENSOR_FAULT     0xFE  /* 传感器故障 */
#define FANM_TEMP_OFFSET            50    /* 温度偏移值 */
#define FANM_PRESSURE_SCALE         10    /* 压力缩放因子 */
#define FANM_VOLTAGE_SCALE_DIV      5     /* 电压缩放除数 */

/* 故障位定义 */
#define FANM_FAULT_BIT_LIQUID_LEVEL_VERY_LOW   0x04  /* 第8字节bit2: 液位极低 */

#define FANM_SEND_MODBUS(addr, cmd, data, data_len)           ModbusM_Send(MODBUS_CHANNEL_FAN, addr, cmd, data, data_len)
#define FANM_GET_MODBUS_READY()                               ModbusM_GetReadyStatus(MODBUS_CHANNEL_FAN)
#define FANM_GET_CHARGER_STATUS()                             (SYSM_GetSysStatusBit(SYS_CONNECTOR1, STD_SYSM_SYSSTATUS_CHARGING) || SYSM_GetSysStatusBit(SYS_CONNECTOR2, STD_SYSM_SYSSTATUS_CHARGING))
#define FANM_DEBUG(fmt, ...) 	                              LOG_DEBUG(LOG_MODULE_RS485, fmt, ##__VA_ARGS__)
#define FANM_ERROR(fmt, ...) 	                              LOG_ERROR(LOG_MODULE_RS485, fmt, ##__VA_ARGS__)
#define FANM_PRINT_HEX(pdata,len)                             LogService_Print_Hex_Array(LOG_MODULE_RS485, pdata, len, 1)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
/* 运行模式定义 */
typedef enum {
    FANM_MODE_STANDBY = 0,          /* 待机 */
    FANM_MODE_AUTO_PRECOOL = 1,     /* 自动之预制冷 */
    FANM_MODE_AUTO_NORMAL = 2,      /* 自动之正常 */
    FANM_MODE_AUTO_FINISH = 3,      /* 自动之收尾 */
    FANM_MODE_FULL_SPEED = 10,      /* 全速 */
    FANM_MODE_DEBUG = 11            /* 普通调试 */
} FanM_RunMode_t;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __FANM_CFG_H */
/* EOL */
