//****************************************************************************************
//*
//* File Name: STD_Sensor_Cfg.h
//* Project Name: JDL_CBCU
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef _SENSOR_CFG_H
#define _SENSOR_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Mcal_ADC_Cfg.h"
#include "Std_Types.h"
#include "SwitchM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define SENSOR_ADC_MAP_TEMP_OFFSET			(40u)	/* The offset of the temperature value corresponding to the AD value is -40 */
#define SENSOR_ADC_TEMP_SEGMENTATION_COEFF  (20.93f)/* The AD value corresponding to 24℃， Resolution:100 */

#define SENSOR_ADC_AVERAGE_BUFF_NUM 	    (8u)	/*temp ADC sample buffer number */

#define SENSOR_GetAdc2Value(ch)			    Mcal_AdcDrv_GetAdcValue(ch) /*get relay ADC1 value from ACDC channel*/
#define SENSOR_DEBUG(fmt, ...)	  			LOG_DEBUG(LOG_MODULE_SENSOR, fmt, ##__VA_ARGS__)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    SENSOR_CONNECTOR1_FRONT_TEMP_CH, /* CONNECTOR1 FRONT TEMP */
    SENSOR_CONNECTOR1_REAR_TEMP_CH,  /* CONNECTOR1 REAR TEMP */
    SENSOR_OIL1_INLET_TEMP_CH,       /* Oil1 inlet temp */
    SENSOR_OIL1_OUTLET_TEMP_CH,      /* Oil1 outlet temp */
    SENSOR_CONNECTOR2_FRONT_TEMP_CH, /* CONNECTOR2 FRONT TEMP */
    SENSOR_CONNECTOR2_REAR_TEMP_CH,  /* CONNECTOR2 REAR TEMP */
    SENSOR_OIL2_INLET_TEMP_CH,       /* Oil2 inlet temp */
    SENSOR_OIL2_OUTLET_TEMP_CH,      /* Oil2 outlet temp */
    SENSOR_ADC2_TEMP_NUM_MAXIMUM
}Sensor_Adc2TempChannel_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct {
    float p1;  // x³ Coefficient
    float p2;  // x² Coefficient
    float p3;  // x Coefficient
    float p4;  // const
} Sensor_AdcTempCoeff_Struct;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern float Sensor_CalculateTempLower(float x);
extern float Sensor_CalculateTempHigher(float x);
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/



