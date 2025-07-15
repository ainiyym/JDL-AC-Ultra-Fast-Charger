//****************************************************************************************
//*
//* File Name: STD_ErrorHandler_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef _ERROR_HANDLER_CFG_H
#define _ERROR_HANDLER_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_LogService.h"
#include "STD_Lib.h"
#include "Std_Types.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define ERRHDL_ALL_PROTO_MAX_NUM					(6u)      /*Fault data size*/

#define ERRHDL_FLT_FLAG_MULTI						(0x01u)	  /*Multiple fault flag*/
#define ERRHDL_FLT_FLAG_SHOW_FIXED_TIME				(0x02u)   /*Led fixed time display flag*/
#define ERRHDL_FLT_FLAG_CLEAR						(0x04u)	  /*Manual clearing flag*/

#define ERRHDL_GetSysReadyState()				SYSM_GetResetPrepareStatus()	  /*get sys status*/

#define ERRHDL_DEBUG(fmt, ...) 						LOG_DEBUG(LOG_MODULE_ERRHDL, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
/*0*/	ERRHDL_ID_RCD_SELFCHECK = 0u,
/*1*/	ERRHDL_ID_RCD_AC_DC,			
/*2*/	ERRHDL_ID_PE,
/*3*/	ERRHDL_ID_RESERVE_03,
/*4*/	ERRHDL_ID_RELAY_SHORTCIRCUIT,
/*5*/	ERRHDL_ID_RELAY_OPENCIRCUIT,
/*6*/	ERRHDL_ID_RELAY_CONGLUTINATION,
/*7*/	ERRHDL_ID_CP_BREAKLINE,
/*8*/	ERRHDL_ID_CP_VOLT,
/*9*/	ERRHDL_ID_CP_PWM,
/*10*/	ERRHDL_ID_OVER_TEMP_L3,
/*11*/	ERRHDL_ID_OVER_TEMP_L2,
/*12*/	ERRHDL_ID_OVER_TEMP_L1,
/*13*/	ERRHDL_ID_LOW_TEMP,
/*14*/	ERRHDL_ID_POWER_RELAYPWR_FAULT,
/*15*/	ERRHDL_ID_Fire_ZERO_MISPHASE_FAULT,
/*16*/	ERRHDL_ID_L1P_OVER_VOLT_L1,
/*17*/	ERRHDL_ID_L1P_OVER_VOLT_L2,
/*18*/	ERRHDL_ID_L1P_LOW_VOLT,
/*19*/	ERRHDL_ID_L1P_OVER_CURR_L1,
/*20*/	ERRHDL_ID_L1P_OVER_CURR_L2,
/*21*/	ERRHDL_ID_GRID_FREQ,
/*22*/	ERRHDL_ID_METER_CHIP,
/*23*/	ERRHDL_ID_POWER_5V_FAULT,
/*24*/	ERRHDL_ID_CAN_FAULT,
/*25*/	ERRHDL_ID_EMERG_STOP,
/*26*/	ERRHDL_ID_KEY_STUCK,
/*27*/	ERRHDL_ID_L2L3P_OVER_VOLT_L1,
/*28*/	ERRHDL_ID_L2L3P_OVER_VOLT_L2,
/*29*/	ERRHDL_ID_L2L3P_LOW_VOLT,
/*30*/	ERRHDL_ID_L2L3P_OVER_CURR_L1,
/*31*/	ERRHDL_ID_L2L3P_OVER_CURR_L2,
/*32*/	ERRHDL_ID_MAX_NUM
} ErrHdlId_Enum;

typedef enum
{
    ERRHDL_BYTE_IDX0 = 0u,                                    	/*0*/
    ERRHDL_BYTE_IDX1,                                         	/*1*/
    ERRHDL_BYTE_IDX2,										  	/*2*/
    ERRHDL_BYTE_IDX3,										  	/*3*/
    ERRHDL_BYTE_MAX_NUM										  	/*NUM = 4*/
} ErrHdlByteIdx_Enum;

typedef enum
{
    ERRHDL_BIT_0 = 0u,											/*0*/
    ERRHDL_BIT_1,												/*1*/
    ERRHDL_BIT_2,												/*2*/
    ERRHDL_BIT_3,												/*3*/
    ERRHDL_BIT_4,												/*4*/
    ERRHDL_BIT_5,												/*5*/
    ERRHDL_BIT_6,												/*6*/
    ERRHDL_BIT_7												/*7*/
} ErrHdlBitOffset_Enum;

typedef enum
{
    ERRHDL_FLT_L_NONE = 0u,										/*0*/
	ERRORH_FLT_L_ONE,											/*1*/
	ERRORH_FLT_L_TWO,											/*2*/
	ERRORH_FLT_L_THREE,											/*3*/
	ERRORH_FLT_L_FOUR,											/*4*/
	ERRORH_FLT_L_FIVE											/*5*/
} ErrHdlLevel_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	ErrHdlByteIdx_Enum enFltByteIdx;          					/*Fault byte ID*/
	ErrHdlBitOffset_Enum enFltBitOffset;     				 	/*Fault byte Offset */
	ErrHdlLevel_Enum enTempFltLevel;          					/*Temporary fault level*/
	ErrHdlLevel_Enum enFltLevel;              					/*Fault level */
	uint8_t ucFltFlag;                          					/*Configuration flag*/
	uint8_t ucMultiFltNum;                      					/*Mult fault number */
	uint32_t ulRecoveryTime;                    					/*Fault Recovery Time */
}ErrHdlCfg_Struct;

/*******************************************************************************
|    Table Definition
|******************************************************************************/
extern const ErrHdlCfg_Struct c_arErrHdlCfgArray[ERRHDL_ID_MAX_NUM];

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
