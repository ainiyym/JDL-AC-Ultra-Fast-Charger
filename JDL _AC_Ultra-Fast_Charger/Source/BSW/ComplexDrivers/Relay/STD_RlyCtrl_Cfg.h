//****************************************************************************************
//*
//* File Name: STD_RlyCtrl_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_RLYCTRL_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _STD_RLYCTRL_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_MosDrv.h"
#include "Mcal_ADC_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define RLYCTRL_DEBUG(fmt, ...) 		LOG_DEBUG(LOG_MODULE_RLY, fmt, ##__VA_ARGS__)
#define RLYCTRL_Get12VResult() 			1

#define RLYCTRL_SWITCH_IDLE 			(0u)  				/* Relay control switch idle */
#define RLYCTRL_SWITCH_OFF 				(1u)  				/* Relay control switch OFF */
#define RLYCTRL_SWITCH_ON 				(2u) 				/* Relay control switch ON */

#define RLYCTRL_BTR_VOL_OK 				(1U) 				/* Relay Power supply voltage OK */
#define RLYCTRL_BTR_VOL_NG 				(2U)				/* Relay Power supply voltage NG */

#define RLYCTRL_DRV_LOW 				(0U)  				/* Relay drver low */
#define RLYCTRL_DRV_HIGH 				(1U) 				/* Relay drver low */

#define RLYCTRL_TASK_PERIOD 			(5U) 				/* 5ms */ /* Relay control task period */
#define RLYCTRL_WAITING_RELAY_PIOWER_UPPER_TIME (20U / RLYCTRL_TASK_PERIOD)	/* wating upper cnt */
/*******************************************************************************
|    LOWER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    UPPER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    Callback Notification
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct{
	uint8_t ucGpioPort;             /*GPIO PORT*/
	uint8_t ucGpioPinIndx;          /*GPIO PIN*/
	uint8_t ucGpioLevelLower;        /*GPIO LEVEL*/
} RlyCtrl_Struct;
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
//extern   const     RlyCtrl_Struct  gv_stRlyCtrlArry[RLYCTRL_AUXL_NUM_MAX];
/*******************************************************************************
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/
