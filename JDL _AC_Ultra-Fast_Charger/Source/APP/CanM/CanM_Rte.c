//****************************************************************************************
//*
//* File Name: CanM_Rte.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include"CanM_Rte.h"
#include "CanM_Rte_Cfg.h"
#include "STD_ErrorHandler.h"
#include "Mcal_Can_Cfg.h"
#include "STD_SysM_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CANM_RTE_SELF_CHECK_NONE (0U)		    /*slefcheck state is none*/
#define CANM_RTE_SELF_CHECK_SUCCESSFUL (1U)     /*slefcheck state is successful*/
#define CANM_RTE_SELF_CHECK_FAILURE (2U)	    /*slefcheck state is failure*/
#define CANM_RTE_SELF_CHECK_BUSY (3U)           /*slefcheck state is busy*/

#define CANM_RTE_SELFCHECK_STEP0 (0u)           /*slefcheck init*/
#define CANM_RTE_SELFCHECK_STEP1 (1u)           /*slefcheck processing*/
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t ReqSelfTest;                                         /* Self-test request status */
	uint8_t ReqSelfTestStep[CANM_RTE_SELFTEST_NUM];				 /* SelfCheck Step */
	uint8_t ReqSelfTestResult;			                         /* Selfcheck result */
    CanM_SECC_MSG2_Input_Struct SECC_MSG2_Input;
} CanM_Rte_Struct;

/*******************************************************************************
|    Static local variables Declaration  
|******************************************************************************/
static CanM_Rte_Struct gv_stCanM_RteCtrl[SYS_CONNECTOR_NUM_MAX];

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void CanM_Rte_SelfTestHandler(SysConnector_Num_Enum ch);
static void CanM_Rte_SetReqChargingEnableStatus(SysConnector_Num_Enum ch);
static void CanM_Rte_InputFrameProcess(void);
static void CanM_Rte_OutputFrameProcess(void);
static void CanM_Rte_SET_SECC_MSG2_Input(SysConnector_Num_Enum connector, CanM_SECC_MSG2_Input_Struct *Output);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void CanM_Rte_Evse_Init(void)
{
    /* Initialize the RTE for CanM */
    /* This function can be called at the start of the application to set initial states */
    CanM_EVSEM_initialize();
    memset(gv_stCanM_RteCtrl, 0, sizeof(gv_stCanM_RteCtrl)); /* Initialize the structure to zero */
    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.RatedCurr = SYS_CONNECTOR_RATED_CURRENT + 2000;
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.RatedVolt = SYS_CONNECTOR_RATED_VOLTAGE;
    }
}

void CanM_Rte_Msg_Init(void)
{
    CanM_MsgM_initialize();
}

void CanM_Rte_SetCarComStart(SysConnector_Num_Enum ch)
{
    CanM_Set_EVSE_CanMode(ch, 1);   /* Set EVSE to CAN mode */
}

void CanM_Rte_Set_AuthM_AuthStatus(SysConnector_Num_Enum ch, uint8_t status)
{
    CanM_Set_AuthM_AuthStatus(ch, (boolean_T)status);
}

void CanM_Rte_Set_SECC_MSG2_InputFaultStatus(SysConnector_Num_Enum ch, CanM_Rte_SeccMsg2Fault_Enum FaultID, uint8_t status)
{
    switch (FaultID)
    {
    case SECC_MSG2_FAULT_ID_ENERGYT_CANNOT_TRANS:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.EnergyTransferUnable = status;
    case SECC_MSG2_FAULT_ID_OVER_CURR:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.OverCurrFaultStatus = status;
    case SECC_MSG2_FAULT_ID_VOLT:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.VoltFaultStatus = status;
    case SECC_MSG2_FAULT_ID_EMERGESTOP:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.EmergeStopFaultStatus = status;
    case SECC_MSG2_FAULT_ID_RELAY:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.RelayFaultStatus = status;
    case SECC_MSG2_FAULT_ID_HARDWARE:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.HardwareFaultStatus = status;
    case SECC_MSG2_FAULT_ID_OVERTEMP:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.OverTempFaultStatus = status;
    case SECC_MSG2_FAULT_ID_CP_VOL:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.CpVolFaultStatus = status;
    case SECC_MSG2_FAULT_ID_CHARGING_PARAMETER:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.ChargingParameterFaultStatus = status;
    case SECC_MSG2_FAULT_ID_SELFTEST_FAIL:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.SelfTestFaultStatus = status;
    case SECC_MSG2_FAULT_ID_CAN_TIME_OUT:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.CanTimeOutFaultStatus = status;
    case SECC_MSG2_FAULT_ID_OTHER:
        gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.OtherFaultStatus = status;
    }
}

void CanM_Rte_Set_SECC_MSG2_ChargingEndReason(SysConnector_Num_Enum ch, CanM_Rte_ChargingEndReason_Enum Reason)
{
    gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.ChargingEndReason = Reason;
}

void CanM_Rte_Set_SECC_MSG2_ChargingStartMode(SysConnector_Num_Enum ch, CanM_Rte_ChargingStartMode_Enum Mode)
{
    gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.ChargingStartMode = Mode;
}

void CanM_Rte_Set_SECC_MSG2_RelayStatus(SysConnector_Num_Enum ch, CanM_Rte_RelayStatus_Enum Status)
{
    gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.RelayStatus = Status;
}

void CanM_Rte_Set_SECC_MSG2_EvseSysStatus(SysConnector_Num_Enum ch, CanM_Rte_EvseSysStatus_Enum Status)
{
    gv_stCanM_RteCtrl[ch].SECC_MSG2_Input.EvseSysStatus = Status;
}

uint8_t CanM_Rte_GetCarComEndStatus(SysConnector_Num_Enum ch)
{
    uint8_t CanModeStatus = (uint8_t)CAN_MODE_STATUS_ENUM_CAN_MODE_STATUS_INIT;

    CanModeStatus = CanM_Get_CanModeStatus(ch);

    if((uint8_t)CAN_MODE_STATUS_ENUM_EXIT == CanModeStatus)
    {
        return (uint8_t)STD_TRUE; /* Communication ended successfully */
    }

    return (uint8_t)STD_FALSE; /* Communication not ended */
}

static void CanM_Rte_SetReqChargingEnableStatus(SysConnector_Num_Enum ch)
{
    if (CanM_Get_McuState3HeartBeatStatus(ch))
    {
        CanM_Set_MCU_State3ReqChargingEnable(ch, CanM_Get_McuState3ReqChargingEnableStatus(ch));
    }
}

static void CanM_Rte_SelfTestHandler(SysConnector_Num_Enum ch)
{
    /* Iterate through each connector to handle self-test requests */
    gv_stCanM_RteCtrl[ch].ReqSelfTest = CanM_Get_ReqSelfTestStatus(ch);
#if (CANM_RTE_DIODE_CHECKSELF_ENABLE == STD_ON)
    if ((uint8_t)CANM_RTE_SELF_CHECK_NONE == gv_stCanM_RteCtrl[ch].ReqSelfTestResult &&
        gv_stCanM_RteCtrl.ReqSelfTest == (uint8_t)CANM_RTE_REQ_SELFTEST_ENABLE &&
        gv_stCanM_RteCtrl[ch].ReqSelfTestStep[0] == (uint8_t)CANM_RTE_SELFCHECK_STEP0)
    {
        CANM_RTE_ReqDiodeSelfCheck(ch);
        gv_stCanM_RteCtrl[ch].ReqSelfTestStep[0] = (uint8_t)CANM_RTE_SELFCHECK_STEP1;
    }
    else if ((uint8_t)CANM_RTE_SELFCHECK_STEP1 == gv_stCanM_RteCtrl[ch].ReqSelfTestStep[0])
    {
        gv_stCanM_RteCtrl[ch].ReqSelfTestResult = CANM_RTE_GetDiodeSelfCheckStatus(ch);
    }
    else
    {

    }
#else
    gv_stCanM_RteCtrl[ch].ReqSelfTestResult = (uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL;
#if (CANM_RTE_RCD_CHECKSELF_ENABLE == STD_ON)
    if ((uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL == gv_stCanM_RteCtrl[ch].ReqSelfTestResult &&
        (uint8_t)CANM_RTE_REQ_SELFTEST_ENABLE == gv_stCanM_RteCtrl.ReqSelfTest &&
        (uint8_t)CANM_RTE_SELFCHECK_STEP0 == gv_stCanM_RteCtrl[ch].ReqSelfTestStep[1])
    {
        CANM_RTE_ReqRCDSelfCheck(ch);
        gv_stCanM_RteCtrl[ch].ReqSelfTestResult = CANM_RTE_SELF_CHECK_BUSY; /* Set to self-checking process */
        gv_stCanM_RteCtrl[ch].ReqSelfTestStep[1] = (uint8_t)CANM_RTE_SELFCHECK_STEP1;
    }
    else if ((uint8_t)CANM_RTE_SELFCHECK_STEP1 == gv_stCanM_RteCtrl[ch].ReqSelfTestStep[1])
    {
        gv_stCanM_RteCtrl[ch].ReqSelfTestResult = CANM_RTE_GetRCDSelfCheckStatus(ch);
    }
    else
    {
    }
#else
    gv_stCanM_RteCtrl[ch].ReqSelfTestResult = (uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL;
#if (CANM_RTE_RELAY_CHECKSELF_ENABLE == STD_ON)
    if ((uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL == gv_stCanM_RteCtrl[ch].ReqSelfTestResult &&
        (uint8_t)CANM_RTE_REQ_SELFTEST_ENABLE == gv_stCanM_RteCtrl.ReqSelfTest &&
        (uint8_t)CANM_RTE_SELFCHECK_STEP0 == gv_stCanM_RteCtrl[ch].ReqSelfTestStep[2])
    {
        CANM_RTE_ReqRelaySelfCheck(ch);
        gv_stCanM_RteCtrl[ch].ReqSelfTestResult = CANM_RTE_SELF_CHECK_BUSY; /* Set to self-checking process */
        gv_stCanM_RteCtrl[ch].ReqSelfTestStep[2] = (uint8_t)CANM_RTE_SELFCHECK_STEP1;
    }
    else if ((uint8_t)CANM_RTE_SELFCHECK_STEP1 == gv_stCanM_RteCtrl[ch].ReqSelfTestStep[2])
    {
        gv_stCanM_RteCtrl[ch].ReqSelfTestResult = CANM_RTE_GetRelaySelfCheckStatus(ch);
    }
    else
    {
    }
#else
    gv_stCanM_RteCtrl[ch].ReqSelfTestResult = (uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL;
#endif
#endif
#endif
    /* Check if the self-test is successful */
    if ((uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL == gv_stCanM_RteCtrl[ch].ReqSelfTestResult || (uint8_t)CANM_RTE_SELF_CHECK_FAILURE == gv_stCanM_RteCtrl[ch].ReqSelfTestResult)
        {
            if ((uint8_t)CANM_RTE_SELF_CHECK_SUCCESSFUL == gv_stCanM_RteCtrl[ch].ReqSelfTestResult)
            {
                CanM_Set_BeforeChargingTestResult(ch, CANM_RTE_SELFTEST_SUCCESS);
            }
            else
            {
                CanM_Set_BeforeChargingTestResult(ch, CANM_RTE_SELFTEST_FAILURE);
            }
            gv_stCanM_RteCtrl[ch].ReqSelfTestResult = (uint8_t)CANM_RTE_SELF_CHECK_NONE;                             /* Reset self-test result */
            memset(gv_stCanM_RteCtrl[ch].ReqSelfTestStep, (uint8_t)CANM_RTE_SELFCHECK_STEP0, CANM_RTE_SELFTEST_NUM); /* Reset self-test steps */
        }
}

static void CanM_Rte_EvseChargingFaultHandler(SysConnector_Num_Enum ch)
{
    if (CanM_GetChargingCanFaultStatus(ch) == 1)
    {
        /* Handle charging CAN fault */
    }
    else
    {
    }
}

static void CanM_Rte_RelayControlHandler(SysConnector_Num_Enum ch)
{
    static uint8_t relayOnStatus[SYS_CONNECTOR_NUM_MAX] = {ENABLE_STATUS_ENUM_DISABLE};

    if (relayOnStatus[ch] != CanM_Get_ReqRelayOnStatus(ch)) /* Get the request relay on status */
    {
        relayOnStatus[ch] = CanM_Get_ReqRelayOnStatus(ch);
        if (relayOnStatus[ch] == (uint8_t)ENABLE_STATUS_ENUM_ENABLE)
        {
            CanM_Rte_SetRelayOn(ch); /* Request to turn on the relay */
        }
        else
        {
            CanM_Rte_SetRelayOff(ch); /* Request to turn off the relay */
        }
    }
}

static void CanM_Rte_InputFrameProcess(void)
{
    uint8_t DataBuff[CANM_RTE_SINGLE_FRAME_LEN] = {0};
    uint8_t RcvLen = 0;

    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        switch (ch)
        {
        case SYS_CONNECTOR1:
            RcvLen = Mcal_Can_Receive_Msg(MCAL_CAN1_RX_MCU_STATUS3, DataBuff, CANM_RTE_SINGLE_FRAME_LEN);
            break;

#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
        case SYS_CONNECTOR2:
            RcvLen = Mcal_Can_Receive_Msg(MCAL_CAN2_RX_MCU_STATUS3, DataBuff, CANM_RTE_SINGLE_FRAME_LEN);
            break;
#endif
        default:
            break;
        }

        if (CANM_RTE_SINGLE_FRAME_LEN == RcvLen)
        {
            (void)CanM_Set_MCU_Status3_Input(ch, DataBuff, CANM_RTE_SINGLE_FRAME_LEN);
        }
    }
}

static void CanM_Rte_OutputFrameProcess(void)
{
    uint64_t Msg1Data = 0;
    uint64_t Msg2Data = 0;

    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        if (CanM_Get_McuState3HeartBeatStatus(ch))
        {
            Msg1Data = CanM_Get_SECC_MSG1_Output(ch);
            Msg2Data = CanM_Get_SECC_MSG2_Output(ch);
            switch (ch)
            {
            case SYS_CONNECTOR1:
                Mcal_Can_Send_Msg(MCAL_CAN1_TX_SECC_MSG1, (uint8_t *)&Msg1Data, CANM_RTE_SINGLE_FRAME_LEN);
                Mcal_Can_Send_Msg(MCAL_CAN1_TX_SECC_MSG2, (uint8_t *)&Msg2Data, CANM_RTE_SINGLE_FRAME_LEN);
                break;

#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
            case SYS_CONNECTOR2:
                Mcal_Can_Send_Msg(MCAL_CAN2_TX_SECC_MSG1, (uint8_t *)&Msg1Data, CANM_RTE_SINGLE_FRAME_LEN);
                Mcal_Can_Send_Msg(MCAL_CAN2_TX_SECC_MSG2, (uint8_t *)&Msg2Data, CANM_RTE_SINGLE_FRAME_LEN);
                break;
#endif

            default:
                break;
            }
        }
    }
}

static void CanM_Rte_SET_SECC_MSG2_Input(SysConnector_Num_Enum connector, CanM_SECC_MSG2_Input_Struct *Output)
{
    if (SYS_CONNECTOR1 == connector)
    {
        Output->CanTimeOutFaultStatus = 0;
        Output->ChargingEndReason = 0;
        Output->CanTimeOutFaultStatus = 0;
        Output->ChargingStartMode = 0;
        Output->CpVolFaultStatus = 0;
        Output->EmergeStopFaultStatus = 0;
        Output->EnergyTransferUnable = 0;
        Output->EvseSysStatus = 0;
        Output->HardwareFaultStatus = 0;
        Output->CanTimeOutFaultStatus = 0;
        Output->OverCurrFaultStatus = 0;
        Output->OverTempFaultStatus = 0;
        Output->RatedCurr = SYS_CONNECTOR_RATED_CURRENT + 2000; /* factor:1; offset:2000 */
        Output->RatedVolt = SYS_CONNECTOR_RATED_VOLTAGE;
        Output->RelayFaultStatus = 0;
        Output->RelayStatus = 0;
        Output->SelfTestFaultStatus = 0;
        Output->VoltFaultStatus = 0;
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == connector)
    {
        Output->CanTimeOutFaultStatus = 0;
        Output->ChargingEndReason = 0;
        Output->CanTimeOutFaultStatus = 0;
        Output->ChargingStartMode = 0;
        Output->CpVolFaultStatus = 0;
        Output->EmergeStopFaultStatus = 0;
        Output->EnergyTransferUnable = 0;
        Output->EvseSysStatus = 0;
        Output->HardwareFaultStatus = 0;
        Output->CanTimeOutFaultStatus = 0;
        Output->OverCurrFaultStatus = 0;
        Output->OverTempFaultStatus = 0;
        Output->RatedCurr = SYS_CONNECTOR_RATED_CURRENT + 2000; /* factor:1; offset:2000 */
        Output->RatedVolt = SYS_CONNECTOR_RATED_VOLTAGE;
        Output->RelayFaultStatus = 0;
        Output->RelayStatus = 0;
        Output->SelfTestFaultStatus = 0;
        Output->VoltFaultStatus = 0;
    }
#endif
    else
    {
    }
}

void CanM_Rte_EVSE_Main_Task(void)
{
    /* Main function for CanM RTE, can be used to call periodic tasks or handle events */
    /* Check if the system is ready for charging */
    uint8_t ChargeConditions = CanM_Rte_GetChargeConditions();

    /* Call the step function of CanM_EVSEM to process the EVSE CAN control logic */
    CanM_EVSEM_step();

    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        /* Handle the self-test for each connector */
        CanM_Rte_SelfTestHandler(ch);
        /* Handle charging CAN fault */
        CanM_Rte_EvseChargingFaultHandler(ch);
        /* Set the error handler for the charging station */
        CanM_Set_ErrHdl_ChargingStation(ch, ChargeConditions);
        /* Handle relay control based on the request status */
        CanM_Rte_RelayControlHandler(ch);
    }
}

void CanM_Rte_Msg_Main_Task(void)
{
    CanM_Rte_InputFrameProcess();
    CanM_MsgM_step();
    CanM_Rte_OutputFrameProcess();
}

void CanM_Rte_10ms_Task(void)
{
    CanM_SECC_MSG2_Input_Struct MSG2_Input = {0};

    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        CanM_Set_EVSE_CanHeartBeatStatus(ch, (boolean_T)CanM_Get_McuState3HeartBeatStatus(ch));
        CanM_Rte_SET_SECC_MSG2_Input(ch, &MSG2_Input);
        CanM_Rte_SetReqChargingEnableStatus(ch);
        CanM_Set_SECC_MSG1_Input(ch);
        CanM_Set_SECC_MSG2_Input(ch, MSG2_Input);
    }
}
