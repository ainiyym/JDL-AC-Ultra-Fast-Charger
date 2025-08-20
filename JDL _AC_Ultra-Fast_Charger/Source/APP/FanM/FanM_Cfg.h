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
#include "dummy.h"
#include "Sensor.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FANM_TASK_PERIOD								      (10U)
#define FANM_SPEED_TEMP_THRESHOLD_UPPER                       (40U)
#define FANM_SPEED_TEMP_THRESHOLD_LOWER                       (30U)
#define FANM_AD_VOL_MINIMUM                                   (860U)
#define FANM_ACCELERATE_STEP                                  (500U)
#define FANM_DECELERATE_STEP                                  (200U)
#define FANM_TIMEOUT_THRESHOLD                                (30 * 1000 / FANM_TASK_PERIOD)    /* 30s */
#define FANM_PERIOD_COMN_THRESHOLD                            (120 * 1000 / FANM_TASK_PERIOD)    /* 120s */
#define FANM_RETRY_COUNT                                      (3) /* 3 times */
#define FANM_RETRY_DELAY_COUNT_THRESHOLD                      (5 * 1000 / FANM_TASK_PERIOD) /* 5s */

#define FANM_GET_OILE_TEMP()                                  SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_OUTLET_TEMP_CH) > SENSOR_GetSensorAdcTempValue(SENSOR_OIL2_OUTLET_TEMP_CH) \
                                                            ? SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_OUTLET_TEMP_CH) : SENSOR_GetSensorAdcTempValue(SENSOR_OIL2_OUTLET_TEMP_CH)
#define FANM_SEND_MODBUS(addr, cmd, data, data_len)           ModbusM_Send(MODBUS_CHANNEL_FAN, addr, cmd, data, data_len)
#define FANM_GET_MODBUS_READY()                               ModbusM_GetReadyStatus(MODBUS_CHANNEL_FAN)
#define FANM_DEBUG(fmt, ...) 	                              LOG_DEBUG(LOG_MODULE_RS485, fmt, ##__VA_ARGS__)
#define FANM_ERROR(fmt, ...) 	                              LOG_ERROR(LOG_MODULE_RS485, fmt, ##__VA_ARGS__)
#define FANM_PRINT_HEX(pdata,len)                             LogService_Print_Hex_Array(LOG_MODULE_RS485, pdata, len, 1)
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

#endif /* __FANM_CFG_H */
/* EOL */
