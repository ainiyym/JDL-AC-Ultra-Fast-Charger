//****************************************************************************************
//*
//* File Name: STD_Volt_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-19 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#if !defined (_STD_VOLT_CFG_H)
#define _STD_VOLT_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_LogService.h"
#include "Meter_data.h"
#include "STD_ErrorHandler.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define VOLT_TASK_PERIOD										(20u)							 /*Volt module task period*/
#define	VOLT_ENLARGE											(100u)							 /*enlarge factor*/
#define VOLT_INPUT_MODE_TYPE        							VOLT_THREE_PHASE_INPUT_MODE		 /*Input module type*/
#define VOLT_ONE_PHASE_INPUT_MODE      							(0u)							 /*voltage module one phase input mode*/
#define VOLT_THREE_PHASE_INPUT_MODE    							(1u)							 /*voltage module three phase input mode*/

#define VOLT_LV1_LOWVOLT_FUN_EN									STD_ON							 /*Level 1 lowvoltage function enable*/
#define VOLT_LV2_LOWVOLT_FUN_EN									STD_OFF							 /*Level 2 lowvoltage function enable*/
#define VOLT_LV1_OVERVOLT_FUN_EN								STD_ON							 /*Level 1 Overvoltage function enable*/
#define VOLT_LV2_OVERVOLT_FUN_EN								STD_OFF							 /*Level 2 Overvoltage function enable*/
#define VOLT_FAULT_PHASE_FUN_EN      							STD_OFF							 /*fire zero error phase function enable*/

#define VOLT_LV2_IN_OVERVOLT_VALUE								(uint16_t)(275u*VOLT_ENLARGE)		/*level2 input over voltage threshold value*/
#define VOLT_LV2_OUT_OVERVOLT_VALUE								(uint16_t)(259u*VOLT_ENLARGE)		/*level2 output over voltage threshold value */
#define VOLT_LV2_IN_OVERVOLT_WAIT_NUM							(uint16_t)(100u/VOLT_TASK_PERIOD) 	/*level2 input over voltage wait number*/
#define VOLT_LV2_OUT_OVERVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD)	/*level2 input over voltage wait number*/

#define VOLT_LV1_IN_OVERVOLT_VALUE								(uint16_t)(265u*VOLT_ENLARGE)		 /*level1 input over voltage threshold value*/
#define VOLT_LV1_OUT_OVERVOLT_VALUE								(uint16_t)(259u*VOLT_ENLARGE)		 /*level1 input over voltage threshold value*/
#define VOLT_LV1_IN_OVERVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD)	 /*level1 input over voltage wait number*/
#define VOLT_LV1_OUT_OVERVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD)	 /*level1 input over voltage wait number*/

#define VOLT_LV2_IN_LOWVOLT_VALUE								(uint16_t)(165u*VOLT_ENLARGE)		 /* input low voltage threshold value*/
#define VOLT_LV2_OUT_LOWVOLT_VALUE								(uint16_t)(176u*VOLT_ENLARGE)		 /* output low voltage threshold value*/
#define VOLT_LV2_IN_LOWVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD) /* input low voltage wait number*/
#define VOLT_LV2_OUT_LOWVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD) /* input low voltage wait number*/

#define VOLT_LV1_IN_LOWVOLT_VALUE								(uint16_t)(175u*VOLT_ENLARGE)		 /* input low voltage threshold value*/
#define VOLT_LV1_OUT_LOWVOLT_VALUE								(uint16_t)(192u*VOLT_ENLARGE)		 /* output low voltage threshold value*/
#define VOLT_LV1_IN_LOWVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD)  /* input low voltage wait number*/
#define VOLT_LV1_OUT_LOWVOLT_WAIT_NUM							(uint16_t)(2000u/VOLT_TASK_PERIOD) /* input low voltage wait number*/

#define VOLT_GetAllVoltVailVal(ch)\
do{\
    gv_stVolt[ch].stChanVartArray[VOLT_L1_CHAN_NUM].usVoltTempVal = Meter_GetVoltL1(ch);\
    gv_stVolt[ch].stChanVartArray[VOLT_L2_CHAN_NUM].usVoltTempVal = Meter_GetVoltL2(ch);\
    gv_stVolt[ch].stChanVartArray[VOLT_L3_CHAN_NUM].usVoltTempVal = Meter_GetVoltL3(ch);\
}while(0)
#define VOLT_GetSysPrepareStatus()									SYSM_GetResetPrepareStatus()	/*Obtain the system readiness interface*/
#define VOLT_GetMeterPrepareStatus(ch)								Meter_GetMeterReadyStatus(ch)		/*Obtain the Meter module prepare status interface*/

#define VOLT_SetVoltFaultStatus(lv_enFaultIdNum, lv_ucFaultStatus)	ERRHDL_FaultStatusUpdata_CallBack(lv_enFaultIdNum, lv_ucFaultStatus)	/*set voltage fault status*/	

#define VOLT_DEBUG(fmt, ...)	   									LOG_DEBUG(LOG_MODULE_VOLTM, fmt, ##__VA_ARGS__)/*Log printing interface*/
#define VOLT_ERROR(fmt, ...)	   									//LOG_ERROR(LOG_MODULE_VOLTM, fmt, ##__VA_ARGS__)/*Log printing interface*/
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
	VOLT_L1_CHAN_NUM = 0u,		/*L1 channel number*/
	VOLT_L2_CHAN_NUM,			/*L2 channel number*/
	VOLT_L3_CHAN_NUM,			/*L3 channel number*/
	VOLT_CHAN_MAX_NUM,			/*Channel max number*/
};
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global KAM with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/


