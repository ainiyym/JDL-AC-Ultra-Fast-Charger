//****************************************************************************************
//*
//* File Name: CpDuty_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#if !defined (_CP_DUTY_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _CP_DUTY_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "CpDrvIf.h"
#include "STD_LogService.h"
#include "Std_Types.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CPD_TASK_PERIOD					((uint16_t)(5U))							/*cp duty 5ms task period*/

#define CPD_ADJ_WAIT_TIME               ((uint8_t)20u / CPD_TASK_PERIOD)			/*adjust cp pwm wait time*/
#define CPD_DUTY_FILTER_MAX_CNT			(uint16_t)((uint16_t)20u / CPD_TASK_PERIOD) /*duty check filter max cnt*/
#define CPD_FREQ_FILTER_MAX_CNT			(uint16_t)((uint16_t)20u / CPD_TASK_PERIOD)	/*freq check filter max cnt*/
#define CPD_DIAG_PWM_FUN_EN				(STD_OFF)								    /*Diag pwm output function enable*/
#define CPD_DIAG_IN_FREQ_ERR_ABS_VALUE	((uint16_t)(50u))							/*Diag pwm in freq error abs value*/
#define CPD_DIAG_IN_DUTY_ERRABS_VALUE	((uint16_t)(10u))							/*Diag pwm in duty error abs value*/
#define CPD_DIAG_OUT_FREQ_ERR_ABS_VALUE	((uint16_t)(30u))							/*Diag pwm out freq error abs value*/
#define CPD_DIAG_OUT_DUTY_ERRABS_VALUE	((uint16_t)(5u))							/*Diag pwm out duty error abs value*/

#define CPD_DIODE_DETECT_FUN_EN			STD_OFF
#define CPD_DIODE_SELF_PROCESS 			(uint8_t)(0u)
#define	CPD_DIODE_SELF_SUCCESS			(uint8_t)(1u)
#define	CPD_DIODE_SELF_FAILED			(uint8_t)(2u)
#define CPD_DIODE_WAIT_TO_OUTPUT		((uint16_t)40u / CPD_TASK_PERIOD)
#define CPD_DIODE_WAIT_TO_STABLE		((uint16_t)40u / CPD_TASK_PERIOD)
#define CPD_SET_DIODE_DETECT_CNT		((uint16_t)40u / CPD_TASK_PERIOD)
#define CPD_GET_DIODE_GPIO()            Mcal_Gpio_ReadPinStatus(DIODE_Port, DIODE_Pin)

#define CPD_ERROR_RESET_MAX_CNT         (3U)                                        /*cp error reset max cnt*/
#define CPD_CFG_MAX_CURR_DFLT           (125U)									    /*cp max current config*/
#define CPD_SetPwmOutput				CPDRV_SetPwm							    /*set pwm output interface*/
#define CPD_GetPwmFreqDuty				CPDRV_GetDutyPeriod						    /*get pwm freq duty period interface*/
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

#endif
/*EOF*/

