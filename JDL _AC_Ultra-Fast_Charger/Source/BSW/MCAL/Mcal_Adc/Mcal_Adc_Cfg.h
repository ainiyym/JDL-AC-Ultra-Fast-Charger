#ifndef MCAL_ADC_CFG_H
#define MCAL_ADC_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ================== Includes ================== */
#include "Mcal_App_Cfg.h"

/* ================== Macros ================== */
#define MCAL_ADC1_COLECTION_PERIOD   ((uint32_t)50u / OS_SOFTTIMER_COUNTING_UNIT) /* ADC1 Collection period */
#define MCAL_ADC2_COLECTION_PERIOD   ((uint32_t)1000u / OS_SOFTTIMER_COUNTING_UNIT) /* ADC2 Collection period */
/* ================== Type Definitions ================== */
typedef enum
{
    MCAL_ADC1_CP1_CH = 0, /* CP1 */
    MCAL_ADC1_CP2_CH,     /* CP2 */
    MCAL_ADC1_12V_CH,     /* 12V */
    MCAL_ADC1_5V_CH,      /* 5V */
    MCAL_ADC1_3V3_CH,     /* 3.3V */
    MCAL_ADC1_CHANNEL_MAX,
    MCAL_ADC2_CONNECTOR1_FRONT_TEMP_CH, /* CONNECTOR1 FRONT TEMP */
    MCAL_ADC2_CONNECTOR1_REAR_TEMP_CH,  /* CONNECTOR1 REAR TEMP */
    MCAL_ADC2_OIL1_INLET_TEMP_CH,       /* Oil1 inlet temp */
    MCAL_ADC2_OIL1_OUTLET_TEMP_CH,      /* Oil1 outlet temp */
    MCAL_ADC2_CONNECTOR2_FRONT_TEMP_CH, /* CONNECTOR2 FRONT TEMP */
    MCAL_ADC2_CONNECTOR2_REAR_TEMP_CH,  /* CONNECTOR2 REAR TEMP */
    MCAL_ADC2_OIL2_INLET_TEMP_CH,       /* Oil2 inlet temp */
    MCAL_ADC2_OIL2_OUTLET_TEMP_CH,      /* Oil2 outlet temp */
    MCAL_ADC2_TEMP_CHANNEL_MAX
} Mcal_Adc_Channel_e;

typedef enum
{
 MCAL_ADC_CHANNEL_1, /* hadc1 */
 MCAL_ADC_CHANNEL_2, /* hadc2 */
 MCAL_ADC_CHANNEL_NUM_MAX
}Mcal_Adc_Channel_Num_e;

typedef struct
{
    Mcal_Adc_Channel_Num_e Channel;
    ADC_HandleTypeDef *Handle;
} Mcal_Adc_ChCfg_t;

typedef struct
{
    Mcal_Adc_Channel_e Channel;
    ADC_HandleTypeDef *Handle;
    uint16_t* RcvBuff;
} Mcal_Adc_MapCfg_t;

typedef struct
{
    bool EnableStatus[MCAL_ADC_CHANNEL_NUM_MAX];
    uint8_t SoftGptChannel[MCAL_ADC_CHANNEL_NUM_MAX];
} Mcal_Adc_Ctrl_t;
/* ================== Configuration Structure ================== */


/* ================== Externs ================== */
extern void Mcal_Adc_Init(void);
extern void Mcal_Adc_EnableInjected(Mcal_Adc_Channel_e Channel);
extern void Mcal_Adc_DisableInjected(Mcal_Adc_Channel_e Channel);
extern void Mcal_Adc_SoftTimer_Enable(void);
extern void Mcal_Adc_ConvCollection_Enable(Mcal_Adc_Channel_Num_e ch);
extern void Mcal_Adc_ConvCollection_Disable(Mcal_Adc_Channel_Num_e ch);
extern uint16_t Mcal_AdcDrv_GetAdcValue(Mcal_Adc_Channel_e ch);
/* ================== Function Prototypes ================== */


#ifdef __cplusplus
}
#endif

#endif /* MCAL_ADC_CFG_H */
