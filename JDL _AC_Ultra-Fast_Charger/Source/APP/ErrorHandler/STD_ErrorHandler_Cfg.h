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

#define ERRHDL_GUN1                                 SYS_CONNECTOR1   /* gun1 */
#define ERRHDL_GUN2                                 SYS_CONNECTOR2   /* gun2 */
#define ERRHDL_GUN_MAX_NUM                          SYS_CONNECTOR_NUM_MAX  /* max gun num */
#define ERRHDL_GetSysReadyState()				    SYSM_GetResetPrepareStatus()	  /*get sys status*/

#define ERRHDL_DEBUG(fmt, ...) 						LOG_DEBUG(LOG_MODULE_ERRHDL, fmt, ##__VA_ARGS__)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
/*0*/	ERRHDL_ID_RESERVE_01 = 0u,
/*1*/	ERRHDL_ID_RESERVE_02,			
/*2*/	ERRHDL_ID_PE,
/*3*/	ERRHDL_ID_RESERVE_03,
/*4*/	ERRHDL_ID_GUN1_RELAY_FAULT,
/*5*/	ERRHDL_ID_GUN2_RELAY_FAULT,
/*6*/	ERRHDL_ID_POWER_12V_FAULT,
/*7*/	ERRHDL_ID_POWER_5V_FAULT,
/*8*/	ERRHDL_ID_GUN1_CP_VOLT,
/*9*/	ERRHDL_ID_GUN1_CP_PWM,
/*10*/	ERRHDL_ID_GUN2_CP_VOLT,
/*11*/	ERRHDL_ID_GUN2_CP_PWM,
/*12*/	ERRHDL_ID_GUN1_CONNECTOR_TEMP_HIGHT,
/*13*/	ERRHDL_ID_GUN2_CONNECTOR_TEMP_HIGHT,
/*14*/	ERRHDL_ID_RESERVE_06,
/*15*/	ERRHDL_ID_RESERVE_07,
/*16*/	ERRHDL_ID_GUN1_METER_COM_FAIL,
/*17*/	ERRHDL_ID_GUN2_METER_COM_FAIL,
/*18*/	ERRHDL_ID_GUN1_L1_PHASE_OVER_VOLT,
/*19*/	ERRHDL_ID_GUN1_L2L3_PHASE_OVER_VOLT,
/*20*/	ERRHDL_ID_GUN1_LOW_VOLT,
/*21*/	ERRHDL_ID_GUN2_L1_PHASE_OVER_VOLT,
/*22*/	ERRHDL_ID_GUN2_L2L3_PHASE_OVER_VOLT,
/*23*/	ERRHDL_ID_GUN2_LOW_VOLT,
/*24*/	ERRHDL_ID_GUN1_OVER_CURR_L1,
/*25*/	ERRHDL_ID_GUN1_OVER_CURR_L2,
/*26*/	ERRHDL_ID_GUN2_OVER_CURR_L1,
/*27*/	ERRHDL_ID_GUN2_OVER_CURR_L2,
/*28*/	ERRHDL_ID_RESERVE_08,
/*29*/	ERRHDL_ID_RESERVE_09,
/*30*/	ERRHDL_ID_RESERVE_0A,
/*31*/	ERRHDL_ID_RESERVE_0B,
/*32*/	ERRHDL_ID_FANM_COM_FAULT,
/*33*/	ERRHDL_ID_FANM_SERIOUS_FAULT,
/*34*/	ERRHDL_ID_PUMP_LIQUID_LEVEL_LOW,
/*35*/	ERRHDL_ID_PUMP_LIQUID_TEMP_HIGHT,
/*36*/  ERRHDL_ID_RESERVE_0C,
/*37*/	ERRHDL_ID_RESERVE_0D,
/*38*/	ERRHDL_ID_RESERVE_0E,
/*39*/	ERRHDL_ID_RESERVE_0F,
/*40*/	ERRHDL_ID_MAX_NUM
} ErrHdlId_Enum;

typedef enum
{
    ERRHDL_BYTE_IDX0 = 0u,                                    	/*0*/
    ERRHDL_BYTE_IDX1,                                         	/*1*/
    ERRHDL_BYTE_IDX2,										  	/*2*/
    ERRHDL_BYTE_IDX3,										  	/*3*/
    ERRHDL_BYTE_IDX4,										  	/*4*/
    ERRHDL_BYTE_MAX_NUM										  	/*NUM = 5*/
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

typedef enum
{
    ERRHDL_ATTIBUTION_NONE = 0u,
    ERRHDL_ATTIBUTION_GUN1, /* gun1 */
    ERRHDL_ATTIBUTION_GUN2, /* gun2 */
    ERRHDL_ATTIBUTION_MAX
} ErrHdlAttibution_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    ErrHdlByteIdx_Enum enFltByteIdx;          /*Fault byte ID*/
    ErrHdlBitOffset_Enum enFltBitOffset;      /*Fault byte Offset */
    ErrHdlLevel_Enum enTempFltLevel;          /*Temporary fault level*/
    ErrHdlLevel_Enum enFltLevel;              /*Fault level */
    ErrHdlAttibution_Enum enFaultAttribution; /*Gun type */
    uint8_t ucFltFlag;                        /*Configuration flag*/
    uint8_t ucMultiFltNum;                    /*Mult fault number */
    uint32_t ulRecoveryTime;                  /*Fault Recovery Time */
} ErrHdlCfg_Struct;

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
