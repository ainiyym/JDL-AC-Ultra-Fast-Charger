//****************************************************************************************
//*
//* File Name: STD_ErrorHandler_Cfg.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_ErrorHandler_Cfg.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define ERRHDL_TASKPERIOD					(10u)  /*10ms*/
#define ERRORH_FLT_WAIT_15MIN   			(uint32_t)(15*60*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_10S     			(uint32_t)(10*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_30S     			(uint32_t)(30*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_10MIN   			(uint32_t)(10*60*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_1MIN    			(uint32_t)(1*60*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_2MIN    			(uint32_t)(2*60*1000u/ERRHDL_TASKPERIOD)
#define ERRORH_FLT_WAIT_DEFAULT 			ERRORH_FLT_WAIT_2MIN

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Extern variables and functions declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
const ErrHdlCfg_Struct c_arErrHdlCfgArray[ERRHDL_ID_MAX_NUM] =
{
		/*              Id            Offset       Temporary level    Final level          ErrorFlag    																  Errorcnt  Error Recovery Time 							 */
		/*0*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_0,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*RCD selfcheck error*/
		/*1*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_1,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FIVE ,	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														3u,	ERRORH_FLT_WAIT_DEFAULT	  },/*RCD AC or DC error*/
		/*2*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_2,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														3u,	ERRORH_FLT_WAIT_1MIN   	  },/*PE error*/
		/*3*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_3,ERRHDL_FLT_L_NONE,	ERRHDL_FLT_L_NONE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_1MIN   	  },/*Reserve_03 error*/
		/*4*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Relay Short circuit before charging*/
		/*5*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_5,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Relay open circuit*/
		/*6*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Relay conglutination*/
		/*7*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT   },/*CP break line*/

		/*8*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_0,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_10S		  },/*CP Voltage error*/
		/*9*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_1,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR,	(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),									5u,	ERRORH_FLT_WAIT_10S		  },/*CP frequency error*/
		/*10*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_2,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*over temperature level 3 error*/
		/*11*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_3,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_10S		  },/*over temperature level 2 error*/
		/*12*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_TWO, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*over temperature level 1 error*/
		/*13*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_5,ERRORH_FLT_L_FOUR,	ERRORH_FLT_L_FOUR, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Low temperature*/
		/*14*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*The Relay power module is faulty*/
		/*15*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Fire zero line misphase fault*/

		/*16*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_0,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_10S		  },/*L1 phase over voltage level 1 error*/
		/*17*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_1,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														3u,	ERRORH_FLT_WAIT_10S		  },/*L1 phase over voltage level 2 error*/
		/*18*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_2,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_DEFAULT	  },/*L1 phase low voltage error*/
		/*19*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_3,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),									3u,	ERRORH_FLT_WAIT_10S		  },/*L1 phase over current level 1 error*/
		/*20*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(0u),																			0u,	ERRORH_FLT_WAIT_10S		  },/*L1 phase over current level 2 error*/
		/*21*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_5,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR,	(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),									5u,	ERRORH_FLT_WAIT_10S		  },/*grid frequency error*/
		/*22*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Metering chip error*/
		/*23*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*The 5V power module is faulty*/
        
		/*24*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_0,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_ONE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*CAN error*/
		/*25*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_1,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*Emergency stop error*/
		/*26*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_2,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_ONE, 	(uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*key stuck error*/
		/*27*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_3,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_10S		  },/*L2 or L3 phase over voltage level 1 error*/
		/*28*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  (uint8_t)(0u),																			1u,	ERRORH_FLT_WAIT_DEFAULT	  },/*L2 or L3 phase over voltage level 2 error*/
		/*29*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_5,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI),														5u,	ERRORH_FLT_WAIT_10S		  },/*L2 or L3 phase low voltage error*/
		/*30*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_6,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),									5u,	ERRORH_FLT_WAIT_10S		  },/*L2 or L3 phase over current level 1 error*/
		/*31*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_7,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR,  (uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),									3u,	ERRORH_FLT_WAIT_DEFAULT	  },/*L2 or L3 phase over current level 2 error*/
};

/*EOF*/
