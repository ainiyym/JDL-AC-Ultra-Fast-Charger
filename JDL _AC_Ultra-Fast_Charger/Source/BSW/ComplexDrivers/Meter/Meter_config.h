//******************************************************************************
//* File Name: Meter_config.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module configuration header file
/*******************************************************************************/
#if !defined (__METER_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "ModBusM.h"
#include "STD_LogService.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define METER_DEBUG(fmt, ...) 	LOG_DEBUG(LOG_MODULE_METER, fmt, ##__VA_ARGS__)
#define METER_INFO(fmt, ...) 	//LOG_INFO(LOG_MODULE_METER, fmt, ##__VA_ARGS__)
#define METER_WARN(fmt, ...) 	LOG_WARN(LOG_MODULE_METER, fmt, ##__VA_ARGS__)
#define METER_ERROR(fmt, ...) 	LOG_ERROR(LOG_MODULE_METER, fmt, ##__VA_ARGS__)

/************************** System configuration ***************************/
#define METER_TASK_PERIOD		            (5U)        // 电表任务周期(ms)
#define METER_COUNT                         2           // 电表数量
#define METER_MAX_ERROR_COUNT               3           // 重置通信前的最大错误次数
#define METER_RECOVERY_COUNT                (60)        // 电表通信恢复间隔次数
#define METER_COMM_TIMEOUT_TIME_MS          (1000)      // 电表通信超时时间(ms)
#define METER_PRINT_DATA_INTERVAL_MS        (10 * 1000 / METER_TASK_PERIOD)     // 电表数据打印间隔时间(ms)
#define METER_POWER_DOWN_VOLTAGE_THRESHOLD  SYSM_OUT_POWER_DOWN                 // 欠压断电阈值(V)
/******************** Default parameters of the electricity meter ********************/
#define DEFAULT_CT_VALUE        100     // 默认电流互感器倍率
#define DEFAULT_PT_VALUE        10      // 默认电压互感器倍率

/******************** Electric meter address ********************/
#define METER_1_ADDR            1       // 表1地址
#define METER_2_ADDR            2       // 表2地址

/******************** Read the cycle configuration(ms) ********************/
#define METER_INFO_READ_INTERVAL    200     // 电表数据读取周期
#define TIME_CHECK_INTERVAL         60000   // 时间检查周期(ms)

/******************** Time calibration configuration ********************/
#define TIME_CALIBRATION_THRESHOLD 60   // 时间误差阈值(秒)
#define TIME_CALIBRATION_HOUR     0     // 校准时间-小时
#define TIME_CALIBRATION_MINUTE   5     // 校准时间-分钟

/******************** Display configuration ********************/
#define DISPLAY_ABS_POWER_FACTOR 1      // 是否显示功率因数绝对值

/******************** Basic Parameter Register ********************/
#define REG_CT                  0x0006  // 电流互感器倍率
#define REG_PT                  0x0007  // 电压互感器倍率

/******************** Time register ********************/
#define REG_TIME_SECOND         0x002F  // 秒
#define REG_TIME_MINUTE         0x0030  // 分
#define REG_TIME_HOUR           0x0031  // 时
#define REG_TIME_DAY            0x0032  // 日
#define REG_TIME_MONTH          0x0033  // 月
#define REG_TIME_YEAR           0x0034  // 年

/******************** Voltage Current Register ********************/
#define REG_UAB                 0x2000  // 线电压AB相
#define REG_UBC                 0x2002  // 线电压BC相
#define REG_UCA                 0x2004  // 线电压CA相
#define REG_UA                  0x2006  // 相电压A相
#define REG_UB                  0x2008  // 相电压B相
#define REG_UC                  0x200A  // 相电压C相

#define REG_IA                  0x200C  // 电流A相
#define REG_IB                  0x200E  // 电流B相
#define REG_IC                  0x2010  // 电流C相

/******************** Power accumulation register ********************/
#define REG_PT_POWER            0x2012  // 合相有功功率
#define REG_PA_POWER            0x2014  // A相有功功率
#define REG_PB_POWER            0x2016  // B相有功功率
#define REG_PC_POWER            0x2018  // C相有功功率

/******************** Reactive Power Register ********************/
#define REG_QT_POWER            0x201A  // 合相无功功率
#define REG_QA_POWER            0x201C  // A相无功功率
#define REG_QB_POWER            0x201E  // B相无功功率
#define REG_QC_POWER            0x2020  // C相无功功率

/******************** Power factor register ********************/
#define REG_PFT                 0x202A  // 合相功率因数
#define REG_PFA                 0x202C  // A相功率因数
#define REG_PFB                 0x202E  // B相功率因数
#define REG_PFC                 0x2030  // C相功率因数

/******************** Frequency register ********************/
#define REG_FREQ                0x2044  // 频率

/******************** Electric energy storage device ********************/
// 正向有功电能
#define REG_IMP_EP              0x101E  // 正向总有功电能
#define REG_IMP_EPT1            0x1020  // 正向尖费率电能
#define REG_IMP_EPT2            0x1022  // 正向峰费率电能
#define REG_IMP_EPT3            0x1024  // 正向平费率电能
#define REG_IMP_EPT4            0x1026  // 正向谷费率电能

// 反向有功电能
#define REG_EXP_EP              0x1028  // 反向总有功电能
#define REG_EXP_EPT1            0x102A  // 反向尖费率电能
#define REG_EXP_EPT2            0x102C  // 反向峰费率电能
#define REG_EXP_EPT3            0x102E  // 反向平费率电能
#define REG_EXP_EPT4            0x1030  // 反向谷费率电能

/******************** Register count definition ********************/
#define MAX_REG_READ_COUNT      20      // 单次读取的最大寄存器数量
// Batch reading of configuration
#define VOLTAGE_REG_COUNT       12      // 6个浮点数
#define CURRENT_REG_COUNT       6       // 3个浮点数
#define POWER_REG_COUNT         16      // 8个浮点数（4有功+4无功）
#define POWER_FACTOR_REG_COUNT  8       // 4个浮点数
#define FREQUENCY_REG_COUNT     2       // 1个浮点数
#define CT_PT_REG_COUNT         2       // 2个寄存器
#define TIME_REG_COUNT          6       // 6个寄存器
#define ENERGY_FAST_REG_COUNT   6       // 3个浮点数
#define ENERGY_SLOW_REG_COUNT   10      // 5个浮点数

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

#endif /* __METER_CFG_H */
/* EOL */
