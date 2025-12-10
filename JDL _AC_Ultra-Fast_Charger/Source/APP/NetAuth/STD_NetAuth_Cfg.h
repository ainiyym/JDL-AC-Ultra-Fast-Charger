//****************************************************************************************
//*
//* File Name: STD_NetAuth_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef _STD_NETAUTH_CFG_H
#define _STD_NETAUTH_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "STD_AuthM.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/*
 * Get the system readiness status , TRUE-Means System is ready
 */
#define NETAUTH_GetSysPrepareStatus()		 SYSM_GetResetPrepareStatus()


/*
 * Get Auth Status, TRUE-Means Auth successfuly
 */
#define NETAUTH_GetCurrAuthStatus(ch)			 AUTHM_GetCurrAuthStatus(ch)

/*
 * Get whether the authorization module is currently allowed to respond to authorization requests
 * TRUE-Means allow
 */
#define NETAUTH_GetAuthAllowStatus(ch)         AUTHM_GetAllowResAuthReqStatus(ch)



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

#endif /*_STD_NETAUTH_CFG_H */



















