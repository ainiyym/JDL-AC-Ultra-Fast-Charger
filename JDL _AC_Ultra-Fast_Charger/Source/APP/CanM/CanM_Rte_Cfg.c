//****************************************************************************************
//*
//* File Name: CanM_Rte_Cfg.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include"CanM_Rte_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration  
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void CanM_Set_McuStatus3ValidStatus(SysConnector_Num_Enum connector, boolean_T status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.MCU_State3ValidStatus = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.MCU_State3ValidStatus1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

/********************************************************
* Function name CanM_Set_EVSE_CanMode
* Description       : Set EVSE into CanMode
* Parameter         ：
* @connector        :connector number
* @status           : 1 -- into CanMode, 0 -- exit CanMode
* Sync/Async        : Synchronous
* Return            ：none
* Call By           : CanM_Set_EVSE_CanMode
**********************************************************/
void CanM_Set_EVSE_CanMode(SysConnector_Num_Enum connector, boolean_T status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.EVSE_CanMode = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.EVSE_CanMode1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

void CanM_Set_MCU_State3ReqChargingEnable(SysConnector_Num_Enum connector, boolean_T status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.MCU_State3ReqChargingEnable = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.MCU_State3ReqChargingEnable1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

void CanM_Set_MCU_State3ValidStatus(SysConnector_Num_Enum connector, boolean_T status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.MCU_State3ValidStatus = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.MCU_State3ValidStatus1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

void CanM_Set_ErrHdl_ChargingStation(SysConnector_Num_Enum connector, uint8_t status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.ErrHdl_ChargingStation = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.ErrHdl_ChargingStation1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

void CanM_Set_AuthM_AuthStatus(SysConnector_Num_Enum connector, boolean_T status)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.AuthM_AuthStatus = status;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.AuthM_AuthStatus1 = status;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

void CanM_Set_BeforeChargingTestResult(SysConnector_Num_Enum connector, uint8_t result)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_EVSEM_U.BeforeChargingTestResult = result;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_EVSEM_U.BeforeChargingTestResult1 = result;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

uint8_T CanM_GetChargingCanFaultStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_EVSEM_Y.ChargingCanFault;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_EVSEM_Y.ChargingCanFault1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return 0; /* Default return value for invalid connector */
    }
}

uint8_t CanM_Get_ReqSelfTestStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_EVSEM_Y.ReqRelaySelfTest;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_EVSEM_Y.ReqRelaySelfTest1;
    }
#endif
    else
    {
       return 0; /* Invalid connector, handle error if necessary */
    }
}

uint8_t CanM_Get_ReqRelayOnStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_EVSEM_Y.ReqRelayOnStatus;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_EVSEM_Y.ReqRelayOnStatus1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return 0; /* Default return value for invalid connector */
    }
}

uint8_t CanM_Get_CanModeStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_EVSEM_Y.CanModeStatus;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_EVSEM_Y.CanModeStatus1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return 0; /* Default return value for invalid connector */
    }
}
