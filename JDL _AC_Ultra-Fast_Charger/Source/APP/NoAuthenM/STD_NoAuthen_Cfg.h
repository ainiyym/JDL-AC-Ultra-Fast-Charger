//****************************************************************************************
//*
//* File Name: STD_NoAuthen_Cfg.H
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_NOAUTHEN_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_NOAUTHEN_CFG_H

/*******************************************************************************
|    Other Header File Inclusion  
|******************************************************************************/
#include "Std_Types.h"
#include "STD_AuthM.h"
#include "CpM_Cfg.h"
#include "dummy.h" 

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define NOAUTHEN_TASK_PERIOD            (10U)	/*10ms*/

#define NOAUTHEN_NOAUTH_OPEN			(0x5A)	/*noauthen open status*/
#define NOAUTHEN_NOAUTH_CLOSE			(0xA5)	/*noauthen close status*/
#define NOAUTHEN_NOAUTH_DEFAULT			(NOAUTHEN_NOAUTH_OPEN)	/*default noauthen status*/

#define NOAUTHEN_GetSysPreStatus        SYSM_GetResetPrepareStatus	/*get system prepare status*/
#define NOAUTHEN_GetAllowAuthReqStatus  AUTHM_GetAllowResAuthReqStatus	/*get allow auth request status*/
#define NOAUTHEN_GetCurrAuthStatus	    AUTHM_GetCurrAuthStatus	/*get auth status*/
#define NOAUTHEN_GetCpVolStatus		    CPM_GetCpVoltStatus	/*get cp status*/

#define NOAUTHEN_CP_VOL_STATUS_12V       CPV_VOLT_TWF		    /*12V cp status*/
#define NOAUTHEN_CP_VOL_STATUS_9V        CPV_VOLT_NINE			/*9V cp status*/
#define NOAUTHEN_CP_VOL_STATUS_6V        CPV_VOLT_SIX			/*6V cp status*/
#define NOAUTHEN_CP_VOL_STATUS_4V        CPV_VOLT_FOUR			/*4V cp status*/

#define NOAUTHEN_TIMEAUTH_FILTER_CNT    ((uint32_t)100U/NOAUTHEN_TASK_PERIOD)	/*NoAuthen time auth filter count*/
#define NOAUTHEN_DEBUG(fmt, ...) 		LOG_DEBUG(LOG_MODULE_NOAUTH, fmt, ##__VA_ARGS__)

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

/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
