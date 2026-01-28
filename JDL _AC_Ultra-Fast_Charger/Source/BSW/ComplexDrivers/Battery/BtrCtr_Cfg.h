//****************************************************************************************
//*
//* File Name: BtrCtr_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> NO fluence */
#if !defined (_BTRCTR_CFG_H)
/* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _BTRCTR_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_SysM.h"
#include "BtrDrvif_Cfg.h"
#include "dummy.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define BTRCTR_TASK_PERIOD          			 (10U)															/*10ms*/
#define BTRV_ADC_FIFO_MAX_NUM					 16						 	                                    /*btr adc fifo max numer*/
#define BTRCTR_FILTER_INIT						 0xfff						                                    /*filter initial value*/

#define BTRCTR_12V_WAIT_NUM  					 (uint16_t)( 100U / BTRCTR_TASK_PERIOD ) 						/* 12V Filter time */
#define BTRCTR_5V_WAIT_NUM  					 (uint16_t)( 100U / BTRCTR_TASK_PERIOD ) 						/* 5V Filter time */

#define BTRCTR_ENLARGE							 100 /* BTRCTR enlarge value */
#define BARCTR_VOL_CONVERT                  	 50U /* 12V/2.4V */
#define BARCTR_5VVOL_CONVERT                  	 20U /* 5V/2.5V */
#define BTRCTR_CALCULATION_12V                   ((uint16_t)(33U * BARCTR_VOL_CONVERT * BTRCTR_ENLARGE / 4095U))
#define BTRCTR_CALCULATION_5V                  	 ((uint16_t)(33U * BARCTR_5VVOL_CONVERT * BTRCTR_ENLARGE / 4095U))

#define BTRCTR_5V_OUT_VALUE						 (6.0F*BTRCTR_ENLARGE)  	/* 5V upper voltage threshold */
#define BTRCTR_5V_IN_VALUE						 (4.0F*BTRCTR_ENLARGE)		/* 5V lower voltage threshold */
#define BTRCTR_12V_OUT_VALUE					 (13.5F*BTRCTR_ENLARGE)	    /* 12V upper voltage threshold */
#define BTRCTR_12V_IN_VALUE						 (10.5F*BTRCTR_ENLARGE) 	/* 12V lower voltage threshold */

#define BTRCTR_VOL5V_ERRORCALLBACK(Flag)         ERRHDL_FaultStatusUpdata_CallBack(ERRHDL_ID_POWER_5V_FAULT,Flag)
#define BTRCTR_VOL12V_ERRORCALLBACK(Flag)		 ERRHDL_FaultStatusUpdata_CallBack(ERRHDL_ID_POWER_12V_FAULT,Flag)

#define BTRCTR_DEBUG(fmt, ...) 					  LOG_DEBUG(LOG_MODULE_BTRCTR, fmt, ##__VA_ARGS__)
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

#endif
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> NO fluence */
/*EOF*/
