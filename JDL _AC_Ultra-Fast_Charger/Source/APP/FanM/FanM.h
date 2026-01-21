//******************************************************************************
//* File Name: FanM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module header file
/*******************************************************************************/
#if !defined (__FANM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FANM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FanM_Drv.h"

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
/* 故障位定义 */
typedef struct
{
    union
    {
        uint8_t value;
        struct
        {
            uint8_t pump_low_temp_power_down : 1; /* 泵低温降功率 */
            uint8_t pump_overheat_stop : 1;       /* 泵过热停机 */
            uint8_t pump_overheat_power_down : 1; /* 泵过热降功率 */
            uint8_t pump_stall : 1;               /* 泵堵转 */
            uint8_t pump_idle : 1;                /* 泵空转 */
            uint8_t pump_program_error : 1;       /* 泵程序错误 */
            uint8_t pump_overload : 1;            /* 泵过载 */
            uint8_t pump_overcurrent : 1;         /* 泵过电流 */
        } bits;
    } pump_fault1;

    union
    {
        uint8_t value;
        struct
        {
            uint8_t pump_voltage_abnormal : 1; /* 泵电压异常 */
            uint8_t pump_low_temp_stop : 1;    /* 泵低温停止 */
            uint8_t pump_offline : 1;          /* 泵掉线 */
            uint8_t pump_low_pressure : 1;     /* 泵压力低 */
            uint8_t pump_other_fault : 1;      /* 泵其他故障 */
            uint8_t reserved : 3;              /* 预留 */
        } bits;
    } pump_fault2;
} FanM_PumpFault_t;

/* 状态数据结构 */
typedef struct
{
    /* 基本状态 */
    FanM_RunMode_t run_mode;     /* 运行模式 */
    uint8_t fault_severity;      /* 故障严重度 */
    uint8_t pump_fault_map;      /* 哪个泵有故障 */
    FanM_PumpFault_t pump_fault; /* 泵故障详情 */

    /* 传感器状态 */
    uint8_t sensor_fault1;       /* 传感器故障字节1 */
    uint8_t sensor_fault2;       /* 传感器故障字节2 */
    uint8_t temp_high_fault;     /* 温度过高故障 */
    uint8_t pressure_pipe_fault; /* 压力和管路故障 */
    uint8_t reserved_fault[4];   /* 预留故障字节 */

    /* 运行参数 */
    uint8_t liquid_level;      /* 液位值 */
    uint8_t flow_switch_state; /* 流量开关状态 */
    uint8_t fan_speed[4];      /* 风扇转速百分比 */
    uint8_t pump_speed[2];     /* 泵转速百分比 */

    /* 测量值 */
    float out_temp[2];    /* 出液温度(℃) */
    float return_temp[2]; /* 回液温度(℃) */
    float pressure[2];    /* 压力值(kPa) */
    float supply_voltage; /* 供电电压(V) */

    /* 传感器有效性标志 */
    bool out_temp_valid[2];
    bool return_temp_valid[2];
    bool pressure_valid[2];
    bool liquid_level_valid;
} FanM_Status_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
void FanM_Init(void);
void FanM_Deinit(void);
McalRetVal_t FanM_SetChargingState(bool charging);
const FanM_Status_t* FanM_GetCurrentStatus(void);
void FanM_Task_100ms(void);
void FanM_HandleModbusResponse(uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t data_len);
#endif /* __FANM_H */
/* EOL */
