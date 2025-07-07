//****************************************************************************************
//*
//* File Name: CpVolt_Cfg.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (_CP_VOLT_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _CP_VOLT_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "STD_EvseM.h"
#include "CpDrvIf.h"
#include "STD_Fifo.h"
#include "STD_LogService.h"  
#include "CpDuty.h"
#include "SwitchM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CPV_TASK_PERIOD                       		(uint16_t)(5U)					        /*cp module task period*/

#define CPV_VOLT_STATUS_FILTER_MAX_CNT              ((uint16_t)10u / CPV_TASK_PERIOD)	    /*cp voltage status filter max cnt*/
#define CPV_VOLT_ERROR_FILTER_MAX_CNT              	((uint16_t)10u / CPV_TASK_PERIOD)       /*cp voltage error filter max cnt*/
#define CPV_VOLT_BREAKLINE_FILTER_MAX_CNT           ((uint16_t)20u / CPV_TASK_PERIOD)       /*cp voltage breakline filter max cnt*/
#define CPV_VOLT_BRKLIN_RESTORE_MAX_CNT             ((uint16_t)10000u / CPV_TASK_PERIOD)    /*cp voltage brekline recover max cnt*/
#define CPV_VOLT_BRKLIN_NINE_RESTORE_MAX_CNT		((uint16_t)2000u / CPV_TASK_PERIOD)     /*cp breakline recover of push evse max cnt*/
#define CPV_VOLT_BRKLIN_FILTER_MAX_CNT  			((uint16_t)100u / CPV_TASK_PERIOD)      /*cp breakline input error filter max cnt*/

#define CPV_ADC_RESOLUTION							(4095u)							    /*cp ad resolution*/
#define CPV_12V_VOLT_ENLARGE_FACTOR 				(1600)							    /*12V cp volatge enlarge factor*/
#define CPV_4V_VOLT_ENLARGE_FACTOR 				    (CPV_12V_VOLT_ENLARGE_FACTOR)       /*4V cp volatge enlarge factor*/
#define CPV_12V_CVRT_COEFF							(((uint32_t)100 * (uint32_t)CPV_12V_VOLT_ENLARGE_FACTOR)/(uint32_t)CPV_ADC_RESOLUTION)/* The coefficient of AD collecting voltage to the true voltage(BASE 12V) */
#define CPV_4V_CVRT_COEFF							(((uint32_t)100 * (uint32_t)CPV_4V_VOLT_ENLARGE_FACTOR)/(uint32_t)CPV_ADC_RESOLUTION)/* The coefficient of AD collecting voltage to the true voltage(BASE 4V) */

#define CPV_BRK_LIN_FUN_EN							STD_OFF							    /*CP Break line function enable*/
#define CPV_VOLTAGE_6V_2V_ERR_FILLTE_COUNT 			((uint16_t)100u / CPV_TASK_PERIOD)  /*cp 6V error filter count*/
#define CPV_VOLTAGE_9V_3V_ERR_FILLTE_COUNT 			((uint16_t)500u / CPV_TASK_PERIOD)	/*cp 9V error filter count*/
#define CPV_VOLTAGE_12V_4V_ERR_FILLTE_COUNT 		((uint16_t)1000u / CPV_TASK_PERIOD) /*cp 12V error filter count*/
#define CPV_VOLTAGE_RECOVER_ERR_FILLTE_COUNT 		((uint16_t)1000u / CPV_TASK_PERIOD) /*cp recover error filter count*/

#define CPV_VOLTAGE_12P8  	 						(1280)							 /*cp 12p8 input error voltage*/
#define CPV_VOLTAGE_11P2 							(1120)							 /*cp 11p2 input error voltage*/
#define CPV_VOLTAGE_9P8  							(980)							 /*cp 9p8 input error voltage*/
#define CPV_VOLTAGE_8P2 							(820)							 /*cp 8p2 input error voltage*/
#define CPV_VOLTAGE_6P8  							(680)							 /*cp 6p8 input error voltage*/
#define CPV_VOLTAGE_5P2  							(520)							 /*cp 5p2 input error voltage*/

#define CPV_VOLTAGE_4P4                             (440)                            /*cp 4p4 input error voltage*/
#define CPV_VOLTAGE_3P6                             (360)                            /*cp 3p6 input error voltage*/
#define CPV_VOLTAGE_3P4                             (340)                            /*cp 3p4 input error voltage*/
#define CPV_VOLTAGE_2P6                             (260)                            /*cp 2p6 input error voltage*/
#define CPV_VOLTAGE_2P4                             (240)                            /*cp 3p4 input error voltage*/
#define CPV_VOLTAGE_1P6                             (160)                            /*cp 2p6 input error voltage*/

#define CPV_VOLTAGE_12P8_RECOVER 					(1280)							 /*cp 12P8 output error voltage*/
#define CPV_VOLTAGE_11P2_RECOVER 					(1120)							 /*cp 11P2 output error voltage*/
#define CPV_VOLTAGE_9P8_RECOVER  					(980)							 /*cp 9P8 output error voltage*/
#define CPV_VOLTAGE_8P2_RECOVER  					(820)							 /*cp 8P2 output error voltage*/
#define CPV_VOLTAGE_6P8_RECOVER  					(680)							 /*cp 6P8 output error voltage*/
#define CPV_VOLTAGE_5P2_RECOVER  					(520)							 /*cp 5P2 output error voltage*/
#define CPV_VOLTAGE_4P4_RECOVER  					(440)							 /*cp 4P8 output error voltage*/
#define CPV_VOLTAGE_3P6_RECOVER  					(360)							 /*cp 3P6 output error voltage*/
#define CPV_VOLTAGE_3P4_RECOVER  					(340)							 /*cp 3P4 output error voltage*/
#define CPV_VOLTAGE_2P6_RECOVER  					(260)							 /*cp 2P6 output error voltage*/
#define CPV_VOLTAGE_2P4_RECOVER  					(240)							 /*cp 2P4 output error voltage*/
#define CPV_VOLTAGE_1P6_RECOVER  					(160)							 /*cp 1P6 output error voltage*/

#define CPV_ADC_FIFO_MAX_NUM						(11)							 /*cp adc fifo max numer*/


#define CPV_GET_SWITCH_MODE(ch)                     SwitchM_GetCpVolMode(ch)            /* get cp switch mode */
#define CPV_GET_CP_SWITCH_12V_MODE                  SWITCHM_CP_12V                   /* get cp switch 12V mode */
#define CPV_GET_CP_SWITCH_4V_MODE                   SWITCHM_CP_4V                   /* get cp switch 4V mode */

#define CPV_GET_EVSE_ChargeStatus()                 EVSEM_GetChargeStatus()
#define CPV_EVSE_CHARING_STATUS                     EVSEM_STATE_THREE_dot
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
    CPV_VOLT_ZERO = 0U, /* cp error voltage status */
    CPV_VOLT_TWF,       /* cp 12V status */
    CPV_VOLT_NINE,      /* cp 9V status */
    CPV_VOLT_SIX,       /* cp 6V status */
    CPV_VOLT_FOUR,      /* cp 4V status */
    CPV_VOLT_THREE,     /* cp 3V status */
    CPV_VOLT_TWEO       /* cp 2V status */
}CPV_VOLT_ENUM;
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

