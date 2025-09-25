//****************************************************************************************
//*
//* File Name: Mcal_APP.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/* polyspace:begin<MISRA-C3:5.6:Not a defect:Justified> Normal */

#ifndef _MCAL_APP_H
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _MCAL_APP_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Mcal_App_Cfg.h"
#include "Mcal_Types.h"
#include "Mcal_Gpio_Cfg.h"
#include "Mcal_Dma_Cfg.h"
#include "Mcal_GpTime_Cfg.h"
#include "Mcal_MCUCore.h"
#include "Mcal_SysTick.h"
#include "Mcal_Usart_Cfg.h"
#include "Mcal_Can_Cfg.h"
#include "Mcal_iwdg_Cfg.h"
#include "Mcal_spi_Cfg.h"
#include "Mcal_Adc_Cfg.h"
#include "Mcal_Usart_Cfg.h"
#include "Mcal_Rtc_Cfg.h"
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
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void MCALAPP_InitMemory(void);
extern void MCALAPP_PeripheralInit(void);
extern void MCALAPP_PeripheralDeInit(void);

#endif
/* polyspace:end<MISRA-C3:5.6:Not a defect:Justified> Normal */
/*EOF*/
