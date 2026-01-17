//******************************************************************************
//* File Name: Meter_def.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module define header file
/*******************************************************************************/
#if !defined (__METER_DEF_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_DEF_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "math.h"

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
typedef enum
{
    METER_READ_IDLE = 0,
    METER_READ_VOLTAGE,
    METER_READ_CURRENT,
    METER_READ_POWER,
    METER_READ_POWER_FACTOR,
    METER_READ_FREQUENCY,
    METER_READ_ENERGY,
    METER_READ_CT_PT,
    METER_READ_TIME,
    METER_READ_ERROR
} MeterReadState_t;

typedef enum
{
    METER_READ_VOLTAGE_IDX = 0,
    METER_READ_CURRENT_IDX,
    METER_READ_POWER_IDX,
    METER_READ_POWER_FACTOR_IDX,
    METER_READ_FREQUENCY_IDX,
    METER_READ_ENERGY_IDX,
    METER_READ_CT_PT_IDX,
    METER_READ_TIME_IDX,
    METER_READ_MAX_IDX
}MeterReadIndex_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
// Definition of Power Factor Symbol
typedef enum
{
    PF_INDUCTIVE = 1,   // Inductive (positive)
    PF_CAPACITIVE = -1, // Capacitive (negative)
    PF_UNKNOWN = 0      // Unknown
} PowerFactorType_t;

// Electric meter measurement data structure
typedef struct
{
    /************ Original register value ************/
    // Voltage data (raw, unit: 0.1V)
    float raw_Uab;
    float raw_Ubc;
    float raw_Uca;
    float raw_Ua;
    float raw_Ub;
    float raw_Uc;

    // Current data (raw, unit: 0.001A)
    float raw_Ia;
    float raw_Ib;
    float raw_Ic;

    // Active power (raw, unit: 0.1W)
    float raw_Pt;
    float raw_Pa;
    float raw_Pb;
    float raw_Pc;

    // Reactive power (raw, unit: 0.1var)
    float raw_Qt;
    float raw_Qa;
    float raw_Qb;
    float raw_Qc;

    // Power factor (raw, unit: 0.001)
    float raw_PFt;
    float raw_PFa;
    float raw_PFb;
    float raw_PFc;

    // Frequency (raw, unit: 0.01Hz)
    float raw_Freq;

    /************ Ratio coefficient ************/
    uint16_t CT_ratio; // Current transformer ratio
    uint16_t PT_ratio; // Voltage transformer ratio

    /************ Calculated actual values ************/
    // Voltage (V)
    float Uab, Ubc, Uca; // Line voltages
    float Ua, Ub, Uc;    // Phase voltages

    // Current (A)
    float Ia, Ib, Ic;

    // Active power (W)
    float Pt, Pa, Pb, Pc;

    // Reactive power (var)
    float Qt, Qa, Qb, Qc;

    // Apparent power (VA) - calculated
    float St, Sa, Sb, Sc;

    // Power factor
    float PFt, PFa, PFb, PFc;
    PowerFactorType_t PFt_type, PFa_type, PFb_type, PFc_type;

    // Frequency (Hz)
    float Freq;

    // Data validity flag
    uint8_t data_valid;
} MeterMeasureData_t;

// Meter energy data structure
typedef struct
{
    // Forward active energy (raw)
    float raw_ImpEp;
    float raw_ImpEpT1;
    float raw_ImpEpT2;
    float raw_ImpEpT3;
    float raw_ImpEpT4;
    // Reverse active energy (raw)
    float raw_ExpEp;
    float raw_ExpEpT1;
    float raw_ExpEpT2;
    float raw_ExpEpT3;
    float raw_ExpEpT4;
    // Forward active energy (kWh)
    float ImpEp;   // total energy
    float ImpEpT1; // tier1
    float ImpEpT2; // tier2
    float ImpEpT3; // tier3
    float ImpEpT4; // tier4

    // Reverse active energy (kWh)
    float ExpEp;   // total energy
    float ExpEpT1; // tier1
    float ExpEpT2; // tier2
    float ExpEpT3; // tier3
    float ExpEpT4; // tier4

    // Energy increment statistics
    float energy_increment_impEp; // forward energy increment
    float energy_increment_ExpEp; // reverse energy increment
    float last_ImpEp;             // previous forward total energy
    float last_ExpEp;             // previous reverse total energy

    uint32_t timestamp;
    uint8_t data_valid;
} MeterEnergyData_t;

// Meter time data structure
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t need_calibration; // calibration required flag
    int32_t time_diff;        // time difference (seconds)
    uint8_t weekday;          // weekday (0-6, 0=Sunday)
} MeterTime_t;

// 电表完整数据结构
typedef struct
{
    // 数据部分
    MeterMeasureData_t measure;
    MeterEnergyData_t energy;
    MeterTime_t time;

    // Device information
    uint8_t meter_id;    // meter ID: 0 or 1
    uint8_t slave_addr;  // Modbus address
    char meter_name[16]; // meter name

    // statistics
    float total_energy_positive; // accumulated forward energy
    float total_energy_negative; // accumulated reverse energy
    float max_power;             // max power record
    uint32_t max_power_time;     // time of max power
} MeterData_t;

#endif /* __METER_DEF_H */
/* EOL */
