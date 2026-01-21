//******************************************************************************
//* File Name: FanM_Drv.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM_Drv module header file
/*******************************************************************************/
#if !defined (__FANM_DRV_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FANM_DRV_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "FanM_Cfg.h"

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
/* 历史数据存储接口 */
typedef struct
{
    uint8_t data[50];    /* 原始状态数据 */
    uint64_t timestamp;  /* 时间戳(毫秒) */
    uint8_t reserved[2]; /* 保留 */
} FanM_HistoryRecord_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
bool FanM_Storage_Write(const FanM_HistoryRecord_t *record);
bool FanM_Storage_GetUsedCount(uint32_t *count);
bool FanM_Storage_Clear(void);

void FanM_BuildStatusReadRequest(uint8_t *data, uint8_t *len);
void FanM_BuildControlCommand(uint8_t cmd, uint8_t fan_speed, 
                                uint8_t pump1_speed, uint8_t pump2_speed,
                                uint8_t *data, uint8_t *len);
McalRetVal_t FanM_SendControlCommand(uint8_t cmd);
McalRetVal_t FanM_ManualControl(uint8_t fan_speed, uint8_t pump1_speed, uint8_t pump2_speed);
McalRetVal_t FanM_RebootDevice(void);
#endif /* __FANM_DRV_H */
/* EOL */
