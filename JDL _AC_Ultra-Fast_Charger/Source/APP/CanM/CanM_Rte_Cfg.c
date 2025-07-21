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

void CanM_Set_SECC_MSG1_Input(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_MsgM_U.SECC_MSG1_L1_Curr = CanM_Rte_GetL1CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L2_Curr = CanM_Rte_GetL2CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L3_Curr = CanM_Rte_GetL3CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L1_Vol = CanM_Rte_GetL1Volt(connector);
        CanM_MsgM_U.SECC_MSG1_L2_Vol = CanM_Rte_GetL2Volt(connector);
        CanM_MsgM_U.SECC_MSG1_L3_Vol = CanM_Rte_GetL3Volt(connector);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_MsgM_U.SECC_MSG1_L1_Curr1 = CanM_Rte_GetL1CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L2_Curr1 = CanM_Rte_GetL2CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L3_Curr1 = CanM_Rte_GetL3CUrr(connector);
        CanM_MsgM_U.SECC_MSG1_L1_Vol1 = CanM_Rte_GetL1Volt(connector);
        CanM_MsgM_U.SECC_MSG1_L2_Vol1 = CanM_Rte_GetL2Volt(connector);
        CanM_MsgM_U.SECC_MSG1_L3_Vol1 = CanM_Rte_GetL3Volt(connector);
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

uint64m_T CanM_Get_SECC_MSG1_Output(SysConnector_Num_Enum connector)
{
    uint64m_T defaultOutput = {0, 0}; // Default output in case of invalid connector

    if (connector == SYS_CONNECTOR1)
    {
        return CanM_MsgM_Y.SECC_MSG1_OutU64;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_MsgM_Y.SECC_MSG1_OutU64_1;
    }
#endif 
    else
    {
        /* Invalid connector, handle error if necessary */
        return defaultOutput; /* Default return value for invalid connector */
    }
}

void CanM_Set_SECC_MSG2_Input(SysConnector_Num_Enum connector, CanM_SECC_MSG2_Input_Struct Msg2Input)
{
    if (connector == SYS_CONNECTOR1)
    {
        CanM_MsgM_U.EVSE_ChargingEndReason = Msg2Input.ChargingEndReason;
        CanM_MsgM_U.EVSE_RequestVehicleMode = Msg2Input.ChargingStartMode;
        CanM_MsgM_U.EVSE_Relay_Status = Msg2Input.RelayStatus;
        CanM_MsgM_U.EVSE_SysStatus = Msg2Input.EvseSysStatus;
        CanM_MsgM_U.EVSE_EnergyCannotTransfer = Msg2Input.EnergyTransferUnable;
        CanM_MsgM_U.EVSE_OverCurrFault = Msg2Input.OverCurrFaultStatus;
        CanM_MsgM_U.EVSE_VoltageFault = Msg2Input.VoltFaultStatus;
        CanM_MsgM_U.EVSE_EmergeStopFault = Msg2Input.EmergeStopFaultStatus;
        CanM_MsgM_U.EVSE_RelayFault = Msg2Input.RelayFaultStatus;
        CanM_MsgM_U.EVSE_HardwareFault = Msg2Input.HardwareFaultStatus;
        CanM_MsgM_U.EVSE_OverTempFault = Msg2Input.OverTempFaultStatus;
        CanM_MsgM_U.EVSE_CpVolFault = Msg2Input.CpVolFaultStatus;
        CanM_MsgM_U.EVSE_ChargingParamMismatch = Msg2Input.ChargingParameterFaultStatus;
        CanM_MsgM_U.EVSE_SelfTestFault = Msg2Input.SelfTestFaultStatus;
        CanM_MsgM_U.EVSE_CanTimeout = Msg2Input.CanTimeOutFaultStatus;
        CanM_MsgM_U.EVSE_OtherFault = Msg2Input.OtherFaultStatus;
        CanM_MsgM_U.EVSE_RatedCurrent = Msg2Input.RatedCurr;
        CanM_MsgM_U.EVSE_RatedVoltage = Msg2Input.RatedVolt;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        CanM_MsgM_U.EVSE_ChargingEndReason1 = Msg2Input.ChargingEndReason;
        CanM_MsgM_U.EVSE_RequestVehicleMode1 = Msg2Input.ChargingStartMode;
        CanM_MsgM_U.EVSE_Relay_Status1 = Msg2Input.RelayStatus;
        CanM_MsgM_U.EVSE_SysStatus1 = Msg2Input.EvseSysStatus;
        CanM_MsgM_U.EVSE_EnergyCannotTransfer1 = Msg2Input.EnergyTransferUnable;
        CanM_MsgM_U.EVSE_OverCurrFault1 = Msg2Input.OverCurrFaultStatus;
        CanM_MsgM_U.EVSE_VoltageFault1 = Msg2Input.VoltFaultStatus;
        CanM_MsgM_U.EVSE_EmergeStopFault1 = Msg2Input.EmergeStopFaultStatus;
        CanM_MsgM_U.EVSE_RelayFault1 = Msg2Input.RelayFaultStatus;
        CanM_MsgM_U.EVSE_HardwareFault1 = Msg2Input.HardwareFaultStatus;
        CanM_MsgM_U.EVSE_OverTempFault1 = Msg2Input.OverTempFaultStatus;
        CanM_MsgM_U.EVSE_CpVolFault1 = Msg2Input.CpVolFaultStatus;
        CanM_MsgM_U.EVSE_ChargingParamMismatch1 = Msg2Input.ChargingParameterFaultStatus;
        CanM_MsgM_U.EVSE_SelfTestFault1 = Msg2Input.SelfTestFaultStatus;
        CanM_MsgM_U.EVSE_CanTimeout1 = Msg2Input.CanTimeOutFaultStatus;
        CanM_MsgM_U.EVSE_OtherFault1 = Msg2Input.OtherFaultStatus;
        CanM_MsgM_U.EVSE_RatedCurrent1 = Msg2Input.RatedCurr;
        CanM_MsgM_U.EVSE_RatedVoltage1 = Msg2Input.RatedVolt;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

uint64m_T CanM_Get_SECC_MSG2_Output(SysConnector_Num_Enum connector)
{
    uint64m_T defaultOutput = {0, 0}; // Default output in case of invalid connector

    if (connector == SYS_CONNECTOR1)
    {
        return CanM_MsgM_Y.SECC_MSG2_OutU64;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_MsgM_Y.SECC_MSG2_OutU64_1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return defaultOutput; /* Default return value for invalid connector */
    }
}

void CanM_Set_MCU_Data(SysConnector_Num_Enum connector, uint8_t *MCUData)
{
    if (connector == SYS_CONNECTOR1)
    {
        memcpy((uint8_t *)&CanM_MsgM_U.MCU_Status3_Data, &MCUData[0], sizeof(CanM_MsgM_U.MCU_Status3_Data));
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        memcpy((uint8_t *)&CanM_MsgM_U.MCU_Status3_Data1, &MCUData[0], sizeof(CanM_MsgM_U.MCU_Status3_Data1));
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
    }
}

uint8_t CanM_Get_MCU_State3ReqChargingEnableStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_MsgM_Y.MCU_State3ReqChargingEnable;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_MsgM_Y.MCU_State3ReqChargingEnable1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return 0; /* Default return value for invalid connector */
    }
}

uint8_t CanM_get_NCU_State3ValidStatus(SysConnector_Num_Enum connector)
{
    if (connector == SYS_CONNECTOR1)
    {
        return CanM_MsgM_Y.MCU_State3ValidStatus;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (connector == SYS_CONNECTOR2)
    {
        return CanM_MsgM_Y.MCU_State3ValidStatus1;
    }
#endif
    else
    {
        /* Invalid connector, handle error if necessary */
        return 0; /* Default return value for invalid connector */
    }
}
