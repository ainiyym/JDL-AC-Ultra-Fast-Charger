//****************************************************************************************
//*
//* File Name: STD_Curr_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_CURR_CFG_H)
#define _STD_CURR_CFG_H


/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_LogService.h"
#include "STD_ErrorHandler.h"
#include "STD_SysM.h"
#include "CpM_Cfg.h"
#include "STD_AuthM.h"
#include "STD_EvseM.h"
#include "switchM.h"
#include "STD_Lib.h"
#include "Meter_data.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CURR_MONITOR_MODE_TYPE                 					 CURR_THR_PHASE_MODE_TYPE			/*current monitor mode type*/
#define CURR_DERATE_PERCENT_FUN_EN								 STD_OFF							/*derate percent function enable*/
#define CURR_LV1_OVERCURR_CHECK_FUN_EN							 STD_ON								/*level 1 over current check function enable*/
#define CURR_LV2_OVERCURR_CHECK_FUN_EN							 STD_ON								/*level 2 over current check function enable*/
#define CURR_ONE_PHASE_MODE_TYPE              					 (0u)								/*one phase mode type*/
#define CURR_THR_PHASE_MODE_TYPE             					 (1u)								/*three phase mode type*/

#define CURR_TASK_PERIOD										(20u)								/*current module task period*/
#define CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL           			(uint16_t)(63u)						/* when cp out 12v, one phase default current value */
#define CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL           			(uint16_t)(450u)					/* when cp out 4v, one phase default current value */
#define CURR_THR_PHASE_CP12V_DFLT_CURR_VAL         				(uint16_t)(63u)						/*three phase default current value*/
#define CURR_THR_PHASE_CP4V_DFLT_CURR_VAL           			(uint16_t)(450u)					/*three phase default current value*/

#define CURR_CFG_DFLT_CURR_MIN                  				(uint16_t)(6u)						/*config default current min value*/

#define CURR_LV2_IN_OVERCURR_WAIT_NUM							(uint16_t)(100u/CURR_TASK_PERIOD)	/*current module level 2 input overcurrent wait number*/
#define CURR_LV2_OUT_OVERCURR_WAIT_NUM							(uint16_t)(2000u/CURR_TASK_PERIOD)	/*current module level 2 output overcurrent wait number*/
#define CURR_LV1_IN_OVERCURR_WAIT_NUM							(uint16_t)(5000u/CURR_TASK_PERIOD)	/*current module level 1 input overcurrent wait number*/
#define CURR_LV1_OUT_OVERCURR_WAIT_NUM							(uint16_t)(2000u/CURR_TASK_PERIOD)	/*current module level 1 output overcurrent wait number*/
#define CURR_REMOTE_REQ_INTER_MAX_WAIT_NUM						(uint16_t)(5000u/CURR_TASK_PERIOD)	/*current module  remote request inter max wait number*/

#define CURR_CHARGE_STATE										(EVSEM_STATE_THREE_dot)				/*evse charge status*/
#define CURR_FLT_CURR_BASIS_VAL									(uint16_t)(20u)						/*refresh basis error current value*/
#define CURR_ENLARGE											(100u)								/*current module enlarge multiple*/
#define CURR_L_BASIS_LV1_VAL									(2u)								/*Lower than the reference current fault primary overcurrent coefficient value*/
#define CURR_L_BASIS_LV2_VAL									(4u)								/*Lower than the reference current fault secondary overcurrent coefficient value*/
#define CURR_H_BASIS_LV1_VAL									(1.1f)								/*Higher than the reference current fault primary overcurrent coefficient value*/
#define CURR_H_BASIS_LV2_VAL									(1.25f)								/*Higher than the reference current fault secondary overcurrent coefficient value*/

#define CURR_CP12V_MODE											(SWITCHM_CP_12V)					/*current module cp 12v mode*/
#define CURR_CP4V_MODE											(SWITCHM_CP_4V)						/*current module cp 4v mode*/

#define CURR_GetL1CurrVailVal(ch)								Meter_GetCurrL1(ch)					/*current module get METERM module L1 current value interface(0.01A)*/
#define CURR_GetL2CurrVailVal(ch)								Meter_GetCurrL2(ch)					/*current module get METERM module L2 current value interface(0.01A)*/
#define CURR_GetL3CurrVailVal(ch)								Meter_GetCurrL3(ch)					/*current module get METERM module L3 current value interface(0.01A)*/
#define CURR_GetAllCurrVailVal(ch)\
do{\
    gv_stCurr[ch].stChanVartArray[CURR_L1_CHAN_NUM].usCurrTempVal = CURR_GetL1CurrVailVal(ch);\
    gv_stCurr[ch].stChanVartArray[CURR_L2_CHAN_NUM].usCurrTempVal = CURR_GetL2CurrVailVal(ch);\
    gv_stCurr[ch].stChanVartArray[CURR_L3_CHAN_NUM].usCurrTempVal = CURR_GetL3CurrVailVal(ch);\
}while (0)

#define CURR_DEBUG(fmt, ...)	  							    LOG_DEBUG(LOG_MODULE_CURRM, fmt, ##__VA_ARGS__)

#define CURR_SetEepDfltCurrVal()\
do{\
}while(0)

#define CURR_GetCpVolMode(ch)										SYSM_GetCpVolMode(ch)					/*current module get SwitchM module cp voltage mode interface*/
#define CURR_GetChargeStatus(ch)									EVSEM_GetChargeStatus(ch)				/*current module get EVSEM module charge status interface*/
#define CURR_GetReqChargeStatus(ch)									AUTHM_GetReqChargeStatus(ch)			/*current module get AUTHM module request charge status interface*/
#define CURR_GetSysPrepareStatus()									SYSM_GetResetPrepareStatus()			/*current module get SYSM module reset Prepare status interface*/
#define CURR_GetMeterPrepareStatus(ch)								Meter_GetMeterReadyStatus(ch)			/*current module get METERM module ready  status interface*/
#define CURR_FaultStatusUpdata(lv_enFaultIdNum, lv_ucFaultStatus) 	ERRHDL_FaultStatusUpdata_CallBack(lv_enFaultIdNum, lv_ucFaultStatus) /*Fault Status Updata to errorHnadle module */
#define CURR_SetCpCurrVal(ch, lv_ucPercent, lv_ucEn)				CPM_AdjustCurrentValue(ch, lv_ucPercent, lv_ucEn)/*current module adjust CPM module cp current value interface*/
#define CURR_GetChargeConditions()									ERRHDL_GetChargeConditions()			/*current module get ERRHDL module charge conditions interface*/
#define CURR_DERATE_PERCENT											60										/*Derating charging percentage*/

#define CURR_ConfigCurrentValue(ch, lv_usCurrValue)				   	CPM_ConfigCurrentValue(ch, lv_usCurrValue)	/*set cp module config current value*/
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
	CURR_L1_CHAN_NUM = 0u,		/*L1 current channel number*/
	CURR_L2_CHAN_NUM,			/*L2 current channel number*/
	CURR_L3_CHAN_NUM,			/*L3 current channel number*/
	CURR_CHAN_MAX_NUM,			/*current channel max number*/
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


