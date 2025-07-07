//****************************************************************************************
//*
//* File Name: SwitchM_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#ifndef _SWITCHM_CFG_H
#define _SWITCHM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Mcal_App.h"
#include "Std_Types.h"
#include "STD_LogService.h"
#include "STD_Os_Timer.h"
#include "STD_SysM_Cfg.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define SWITCHM_CP_4V                           (SYSM_CP_MODE_4V)
#define SWITCHM_CP_12V                          (SYSM_CP_MODE_12V)
#define SWITCHM_SET_SYSM_CP_MODE(ch, mode)		SYSM_SetCpVolMode(ch, mode)

#define SWITCHM_74HCT4851D_MULTIPLEX_ADC0       MCAL_ADC_CHANNEL_1
#define SWITCHM_74HCT4851D_MULTIPLEX_ADC1       MCAL_ADC_CHANNEL_2
#define SWITCHM_74HCT4851D_MULTIPLEX_CH_NUM     MCAL_ADC_CHANNEL_NUM_MAX

#define SWITCHM_74HCT4851D_SOFTTIMER_PERIOD     (54U)            /* 74HCT4851D softtimer period */
#define SWITCHM_DEBUG(fmt, ...) 			    LOG_DEBUG(LOG_MODULE_SWITCHM, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef Mcal_Adc_Channel_Num_e SwitchM_74HCT4851D_Multiplex_e;

typedef enum
{
    SWITCHM_74HCT4851D_CC1 = 0, /* YC_CC1(A0) */
    SWITCHM_74HCT4851D_CC2,     /* YC_CC2(A1) */
    SWITCHM_74HCT4851D_CP1,     /* YC_CP1(A2) */
    SWITCHM_74HCT4851D_CP2,     /* YC_CP2(A3) */
    SWITCHM_74HCT4851D_12V,     /* AD12V(A4) */
    SWITCHM_74HCT4851D_5V,      /* AD5V(A5) */
    SWITCHM_74HCT4851D_3V3,     /* AD3V3(A6) */
    SWITCHM_74HCT4851D_GND,     /* GND(A7) */
    SWITCHM_74HCT4851D_ADC1_MAX
} SwitchM_74HCT4851D_ADC1_e;

typedef enum
{
    SWITCHM_74HCT4851D_CONNECTOR1_FRONT_TEMP = 0, /* Tsmp1(A0) */
    SWITCHM_74HCT4851D_CONNECTOR1_REAR_TEMP,      /* Tsmp2(A1) */
    SWITCHM_74HCT4851D_OIL1_INLET_TEMP,           /* Tsmp3(A2) */
    SWITCHM_74HCT4851D_OIL1_OUTLET_TEMP,          /* Tsmp4(A3) */
    SWITCHM_74HCT4851D_CONNECTOR2_FRONT_TEMP,     /* Tsmp5(A4) */
    SWITCHM_74HCT4851D_CONNECTOR2_REAR_TEMP,      /* Tsmp6(A5) */
    SWITCHM_74HCT4851D_OIL2_INLET_TEMP,           /* Tsmp7(A6) */
    SWITCHM_74HCT4851D_OIL2_OUTLET_TEMP,          /* Tsmp8(A7) */
    SWITCHM_74HCT4851D_ADC2_MAX
} SwitchM_74HCT4851D_ADC2_e;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
