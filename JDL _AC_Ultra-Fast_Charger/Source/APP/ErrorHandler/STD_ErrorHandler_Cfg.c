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
		/*              Id            Offset       Temporary level    Final level      	Fault attribution     		ErrorFlag    											Errorcnt  Error Recovery Time 							 */
		/*0*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_0,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_01 */
		/*1*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_1,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE ,	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_02 */
		/*2*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_2,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_1MIN   	  },/* PE error */
		/*3*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_3,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT   },/* Reserve_03 */
		/*4*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun1 relay fault */
		/*5*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_5,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun2 relay fault */
		/*6*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* 12V power supply fault */
		/*7*/	{ERRHDL_BYTE_IDX0, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FIVE,	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT   },/* 5V power supply fault */
		
		/*8*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_0,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S		  },/* gun1 CP Voltage error */
		/*9*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_1,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR,	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S		  },/* gun1 CP frequency error */
		/*10*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_2,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S	  	  },/* gun2 CP Voltage error */
		/*11*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_3,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S		  },/* gun2 CP frequency error */
		/*12*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun1 plug over temperature error*/
		/*13*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_5,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun2 plug over temperature error*/
		/*14*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_06 */
		/*15*/	{ERRHDL_BYTE_IDX1, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_07 */
		
		/*16*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_0,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S		  },/* gun1 meter communication error */
		/*17*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_1,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							5u,	ERRORH_FLT_WAIT_10S		  },/* gun2 meter communication error */
		/*18*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_2,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S	  	  },/* gun1 L1 phase over voltage error */
		/*19*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_3,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S		  },/* gun1 L2L3 over voltage error */
		/*20*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_4,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S		  },/* gun1 low voltage error */
		/*21*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_5,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR,	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S		  },/* gun2 L1 phase over voltage error */
		/*22*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_6,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S	  	  },/* gun2 L2L3 over voltage error */
		/*23*/	{ERRHDL_BYTE_IDX2, ERRHDL_BIT_7,ERRORH_FLT_L_THREE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_10S		  },/* gun2 low voltage error */
		
		/*24*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_0,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),		3u,	ERRORH_FLT_WAIT_10S	  	  },/* gun1 over current level 1 error */
		/*25*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_1,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,	ERRHDL_ATTIBUTION_GUN1,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun1 over current level 2 error */
		/*26*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_2,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI|ERRHDL_FLT_FLAG_CLEAR),		3u,	ERRORH_FLT_WAIT_10S	  	  },/* gun2 over current level 1 error */
		/*27*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_3,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_GUN2,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* gun2 over current level 2 error */
		/*28*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_08 */
		/*29*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_5,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_09 */
		/*30*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0A */
		/*31*/	{ERRHDL_BYTE_IDX3, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0B */
	
		/*32*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_0,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_DEFAULT	  },/* fan communication error */
		/*33*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_1,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* fan serious error */
		/*34*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_2,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0U),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* pump liquid level low error */
		/*35*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_3,ERRORH_FLT_L_TWO,	ERRORH_FLT_L_FOUR, 	ERRHDL_ATTIBUTION_NONE,		(uint8_t)(ERRHDL_FLT_FLAG_MULTI),							3u,	ERRORH_FLT_WAIT_DEFAULT	  },/* pump liquid temperature high error */
		/*36*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_4,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0C */
		/*37*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_5,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0D */
		/*38*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_6,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0E */
		/*39*/	{ERRHDL_BYTE_IDX4, ERRHDL_BIT_7,ERRHDL_FLT_L_NONE,	ERRORH_FLT_L_FOUR,  ERRHDL_ATTIBUTION_NONE,		(uint8_t)(0u),												1u,	ERRORH_FLT_WAIT_DEFAULT	  },/* Reserve_0F */
};

/*EOF*/
