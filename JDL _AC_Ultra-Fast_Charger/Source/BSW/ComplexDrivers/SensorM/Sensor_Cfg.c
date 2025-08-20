//****************************************************************************************
//*
//* File Name: STD_Sensor_Cfg.c
//* Project Name: JDL_CBCU
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Sensor_Cfg.h"

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
|    Global NVM KAM Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/
const Mcal_Adc_Channel_e SensorAdc2TempMap[SENSOR_ADC2_TEMP_NUM_MAXIMUM] = {MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH, MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH,
                                                                            MCAL_ADC2_OIL1_INLET_TEMP_CH, MCAL_ADC2_OIL1_OUTLET_TEMP_CH,
                                                                            MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH, MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH,
                                                                            MCAL_ADC2_OIL2_INLET_TEMP_CH, MCAL_ADC2_OIL2_OUTLET_TEMP_CH};

/* -40°C ～ 24℃ */
const Sensor_AdcTempCoeff_Struct SensorTempLowerCoeff = {
    .p1 = -0.0113f,
    .p2 = 0.9199f,
    .p3 = -27.0189f,
    .p4 = 331.3158f
};/* matlab fitting; resolution:1, offset: -40 */

 /* 24°C ～ 105℃ */
const Sensor_AdcTempCoeff_Struct SensorTempHigherCoeff = {
    .p1 = -0.0174f,
    .p2 = 0.7774f,
    .p3 = -13.7688f,
    .p4 = 168.8964f
};/* matlab fitting; resolution:1, offset: -40 */

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/****************************************************************************************
| 				            Global Variable Definition Section
|****************************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
  * @brief  Obtain the temperature value
  * @param[in]  x      adc VOL(Resolution 100)
  * @retval temp       offset -40℃
  * @example Sensor_CalculateTempLower(39.08);
  */
float Sensor_CalculateTempLower(float x)
{
    return ((SensorTempLowerCoeff.p1 * x + SensorTempLowerCoeff.p2) * x + SensorTempLowerCoeff.p3) * x + SensorTempLowerCoeff.p4;
}

/**
  * @brief  Obtain the temperature value
  * @param[in]  x      adc VOL(Resolution 100)
  * @retval temp       offset -40℃
  * @example Sensor_CalculateTempLower(14.59);
  */
float Sensor_CalculateTempHigher(float x)
{
    return ((SensorTempHigherCoeff.p1 * x + SensorTempHigherCoeff.p2) * x + SensorTempHigherCoeff.p3) * x + SensorTempHigherCoeff.p4;
}
/*EOF*/
