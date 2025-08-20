//****************************************************************************************
//*
//* File Name: SensorhM.c 
//* Project Name: JDL_CBCU
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Sensor.h"
#include "STD_Lib.h"
#include "STD_LogService.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t SampleCnt;											/*ADC sample count */
	uint8_t TempValue;											/*temperature value*/
	uint16_t ValidAdcValue;										/*valid ADC value*/
	uint16_t ArrayAdcValue[SENSOR_ADC_AVERAGE_BUFF_NUM]; /*temp ADC value array*/
} Sensor_TemperatureAdStruct;

typedef struct
{
	Sensor_TemperatureAdStruct TSensor[SENSOR_ADC2_TEMP_NUM_MAXIMUM];	/* Temperature */
}Sensor_CtrlStruct;

/*******************************************************************************
|    Global NVM KAM Declaration
|******************************************************************************/      

/*******************************************************************************
|    Table Definition
|******************************************************************************/
extern const Mcal_Adc_Channel_e SensorAdc2TempMap[SENSOR_ADC2_TEMP_NUM_MAXIMUM];

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static Sensor_CtrlStruct gv_stSensor;

/****************************************************************************************
|	Global Variable Definition Section
|****************************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint8_t SENSOR_AdcValueToTemperature(uint16_t AdValue);
static void SENSOR_AverageTempValueHandleFunc(Sensor_Adc2TempChannel_Enum Channel);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void SENSOR_InitMemory(void)
{			
	LIB_SetMemory((uint8_t*)&gv_stSensor, 0, sizeof(gv_stSensor));
}

uint8_t SENSOR_GetSensorAdcTempValue(Sensor_Adc2TempChannel_Enum ch)
{
	if (ch < SENSOR_ADC2_TEMP_NUM_MAXIMUM)
	{
		return gv_stSensor.TSensor[ch].TempValue;
	}
	else
	{
		return 0;
	}
}

static uint8_t SENSOR_AdcValueToTemperature(uint16_t AdValue)
{
	float TemperatureOut = 0.0f;
	float AdVolInput = 0.0f;

	AdVolInput = (float)AdValue / 100.0f;
	if(AdVolInput > SENSOR_ADC_TEMP_SEGMENTATION_COEFF)
	{
		TemperatureOut = Sensor_CalculateTempLower(AdVolInput);
	}
	else
	{
		TemperatureOut = Sensor_CalculateTempHigher(AdVolInput); 
	}

	return (uint8_t)(TemperatureOut + 0.5f); /* Round off */
}

static uint16_t SENSOR_AdcTrimmeMeanFilter(uint16_t *lv_usBuff, uint16_t lv_usLen)
{
	uint32_t lv_uiSum = 0;
	uint16_t lv_usAdcValue = 0;

	if (lv_usBuff != NULL && lv_usLen > 2)
	{
		for (uint8_t i = 0; i < lv_usLen - 1; i++)
		{
			for (uint8_t j = 0; j < lv_usLen - i - 1; j++)
			{
				if (lv_usBuff[j] > lv_usBuff[j + 1])
				{
					uint16_t t = 0;
					t = lv_usBuff[j + 1];
					lv_usBuff[j + 1] = lv_usBuff[j];
					lv_usBuff[j] = t;
				}
			}
		}
		for (uint8_t m = 1; m < lv_usLen - 1; m++)
		{
			lv_uiSum = lv_uiSum + lv_usBuff[m];
		}
		lv_usAdcValue = (uint16_t)(lv_uiSum / (lv_usLen - 2));
	}

	return lv_usAdcValue;
}

#if 0
static uint16_t SENSOR_MeanFilter(uint16_t *lv_usBuff, uint16_t lv_usLen)
{
	uint32_t lv_uiSum = 0;
	uint16_t lv_usAdcValue = 0;

	if (lv_usBuff != NULL)
	{
		for (uint8_t m = 0; m < lv_usLen; m++)
		{
			lv_uiSum = lv_uiSum + lv_usBuff[m];
		}
		lv_usAdcValue = (uint16_t)(lv_uiSum / lv_usLen);
	}

	return lv_usAdcValue;
}
#endif

static void SENSOR_AverageTempValueHandleFunc(Sensor_Adc2TempChannel_Enum Channel)
{
	if (Channel >= SENSOR_ADC2_TEMP_NUM_MAXIMUM)
	{
		return;
	}

	Sensor_TemperatureAdStruct* pTemp = &gv_stSensor.TSensor[Channel];
	if (pTemp->SampleCnt < SENSOR_ADC_AVERAGE_BUFF_NUM)
	{
		pTemp->ArrayAdcValue[pTemp->SampleCnt] = SENSOR_GetAdc2Value(SensorAdc2TempMap[Channel]);
		// pTemp->ArrayAdcValue[pTemp->SampleCnt] = 2000;
		pTemp->SampleCnt++;
	}
	else
	{
		pTemp->ValidAdcValue = SENSOR_AdcTrimmeMeanFilter(pTemp->ArrayAdcValue, SENSOR_ADC_AVERAGE_BUFF_NUM);
		pTemp->TempValue = SENSOR_AdcValueToTemperature(pTemp->ValidAdcValue);
		pTemp->SampleCnt = 0u;
	}
}

void SENSOR_5msMainFunction(void)
{
	for (Sensor_Adc2TempChannel_Enum ch = SENSOR_CONNECTOR1_FRONT_TEMP_CH; ch < SENSOR_ADC2_TEMP_NUM_MAXIMUM; ch++)
	{
		SENSOR_AverageTempValueHandleFunc(ch);
	}
	// SENSOR_DEBUG("DC:%d AC:%d W:%d\r\n", gv_stSensor.TSensor[SENSOR_DC_DOCK_TEMP].TempValue, gv_stSensor.TSensor[SENSOR_AC_DOCK_TEMP].TempValue, gv_stSensor.ASensor[SENSOR_MOTOR_ROTATION_ANGLE].AngleValue);
}
/*EOF*/
