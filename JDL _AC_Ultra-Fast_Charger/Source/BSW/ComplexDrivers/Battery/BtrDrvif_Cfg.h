//****************************************************************************************
//*
//* File Name: BtrDrvif_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#if !defined (_BTRDRVIF_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _BTRDRVIF_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "Std_Types.h"
#include "STD_SysM_Cfg.h"
#include "Mcal_Adc_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define BTR_ADC1_12V_CH                     ((Mcal_Adc_Channel_e)MCAL_ADC1_12V_CH)
#define BTR_ADC1_5V_CH                      ((Mcal_Adc_Channel_e)MCAL_ADC1_5V_CH)
#define BTR_ADC_COLECTION_PERIOD 			(SYSM_ADC_PERIOD_COLLECTION_TIME)		/*BTR ADC Collection period*/
#define BtrDrvif_GetCpVolAdcValue(ch)       Mcal_AdcDrv_GetAdcValue(ch)
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
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

extern void BTRDRV_StartAdcCollection(void);
extern void BTRDRV_StopAdcCollection(void);
#endif
/*EOF*/
