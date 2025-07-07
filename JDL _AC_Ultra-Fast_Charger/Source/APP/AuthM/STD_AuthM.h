//****************************************************************************************
//*
//* File Name: STD_AuthM.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#if !defined (_STD_AUTHM_H)
#define _STD_AUTHM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_AuthM_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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
    AUTHM_OPEN_SRC_RFID = 0,          /* RFID auth */
    AUTHM_OPEN_SRC_BT_APP,        /* Bluetooth app auth */
    AUTHM_OPEN_SRC_NET_APP,       /* Network app auth */
    AUTHM_OPEN_SRC_SINGLE_TIMING, /* Single timing auth */
    AUTHM_OPEN_SRC_PERIOD_TIMING, /* Period timing auth */
    AUTHM_OPEN_SRC_NO_AUTHEN,     /* Noauth auth */
    AUTHM_OPEN_SRC_BT_FAST,       /* Bluetooth fast auth */
    AUTHM_OPEN_SRC_MAX
} AUTHM_OpenSrc_enum;

typedef enum
{
    AUTHM_CLOSE_SRC_RFID = 0,      /* RFID close */
    AUTHM_CLOSE_SRC_BT_APP,        /* Bluetooth app close */
    AUTHM_CLOSE_SRC_NET_APP,       /* Network app close */
    AUTHM_CLOSE_SRC_SINGLE_TIMING, /* Single timing close */
    AUTHM_CLOSE_SRC_PERIOD_TIMING, /* Period timing close */
    AUTHM_CLOSE_SRC_BUTTON,        /* Button close */
    AUTHM_CLOSE_SRC_DRAWGUN,       /* Drawgun close */
    AUTHM_CLOSE_SRC_NOPLUG_TIMEOUT,/* no connector close */
    AUTHM_CLOSE_SRC_EMERGENCY,     /* Emergency stop close */
    AUTHM_CLOSE_SRC_SERIOUS_FLT,   /* Serious fault close */
    AUTHM_CLOSE_SRC_MAX
} AUTHM_CloseSrc_enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void AUTHM_InitMemory(void);
extern uint8_t AUTHM_GetAuthOpenSource(SysConnector_Num_Enum ch);
extern uint8_t AUTHM_GetAuthCloseSource(SysConnector_Num_Enum ch);
extern uint8_t AUTHM_GetCurrAuthStatus(SysConnector_Num_Enum ch);
extern uint8_t AUTHM_GetAllowResAuthReqStatus(void);
extern void AUTHM_AppSetReqChargeStatus(SysConnector_Num_Enum ch);
extern void AUTHM_AppResetReqChargeStatus(SysConnector_Num_Enum ch);
extern uint8_t AUTHM_GetReqChargeStatus(SysConnector_Num_Enum ch);
extern void AUTHM_10msMainFunction(void);
#endif
/*EOF*/
