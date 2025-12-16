//****************************************************************************************
//*
//* File Name: STD_RlyCtrl.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_RLYCTRL_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _STD_RLYCTRL_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_RlyCtrl_Cfg.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define SELFCHECK_FAIL           (2U)   /*Selfcheck fail*/
#define SELFCHECK_SUCCESS        (1U)   /*Selfcheck success*/
#define SELFCHECK_PROCESS        (0U)   /*Selfcheck process*/

#define SELF_CHECK_IDLE_MODE     (0U)   /*Selfcheck idle mode*/
#define SELF_CHECK_CONTROL_MODE  (1U)   /*Selfcheck control mode*/
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

typedef enum
{
	RLYCTRL_STATE_UNKNOWN = 0U,                                        /*Ralay unkown status*/
	RLYCTRL_STATE_ON		= 1U,                                      /*Ralay status on*/
	RLYCTRL_STATE_OFF		= 2U,                                      /*Ralay status off*/
	RLYCTRL_STATE_ERR	   = 3U,                                       /*Ralay status error*/
	RLYCTRL_STATE_MAX,                                                 /*Ralay status max*/
} RLYCTRL_State_Type;

typedef enum
{
	RLYCTRL_SwitchOff,                  /*Relay control switching off*/
	RLYCTRL_SwitchOn                   	/*Relay control switching on*/
}RLYCTRL_Switch_EnumType;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void RLYCTRL_InitMemory(void);
extern void RLYCTRL_MainFunction(void);
extern uint8_t RLYCTRL_ReqRelaySwitchOn(SysConnector_Num_Enum ch);
extern uint8_t RLYCTRL_ReqRelaySwitchOff(SysConnector_Num_Enum ch);
extern uint8_t RLYCTRL_ForceRelaySwitchOff(SysConnector_Num_Enum ch);
extern uint8_t RLYCTRL_GetRelayStatus(SysConnector_Num_Enum ch);
#endif
/*EOF*/
