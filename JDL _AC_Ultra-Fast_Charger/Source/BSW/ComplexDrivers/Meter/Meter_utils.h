//******************************************************************************
//* File Name: Meter_utils.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module utility functions header file
/*******************************************************************************/
#if !defined (__METER_UTILS_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_UTILS_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_def.h"
#include "Meter_config.h"
#include "Meter_interface.h"

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

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern float Meter_ParseFloat(uint8_t *data); // parse float from byte array
extern void Meter_ConvertRawData(MeterData_t *meter);
extern void Meter_CalculatePowerFactorData(MeterData_t *meter);
extern void Meter_CalculatePowerData(MeterData_t *meter);
extern void Meter_PrintMeterData(MeterData_t *meter);
#endif /* __METER_UTILS_H */
/* EOL */
