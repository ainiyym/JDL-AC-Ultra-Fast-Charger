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

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void CanM_Rte_SetCarComStart(SysConnector_Num_Enum ch)
{
    CanM_Set_EVSE_CanMode(ch, 1);   /* Set EVSE to CAN mode */
}

void CanM_Rte_Set_MCU_State3ReqChargingEnableStatus(SysConnector_Num_Enum ch, uint8_t status)
{
    CanM_Set_MCU_State3ReqChargingEnable(ch, status);
}

void CanM_Rte_Set_MCU_State3ValidStatus(SysConnector_Num_Enum ch, uint8_t status)
{
    CanM_Set_MCU_State3ValidStatus(ch, (boolean_T)status);
}

void CanM_Rte_Set_AuthM_AuthStatus(SysConnector_Num_Enum ch, uint8_t status)
{
    CanM_Set_AuthM_AuthStatus(ch, (boolean_T)status);
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

void CanM_Rte_Init(void)
{
    /* Initialize the RTE for CanM */
    /* This function can be called at the start of the application to set initial states */
    CanM_EVSEM_initialize();
    memset(gv_stCanM_RteCtrl, 0, sizeof(gv_stCanM_RteCtrl)); /* Initialize the structure to zero */
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

static void CanM_Rte_CanFaultHandler(SysConnector_Num_Enum ch)
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
    if (CanM_Get_ReqRelayOnStatus(ch) == (uint8_t)ENABLE_STATUS_ENUM_ENABLE)
    {
        CanM_Rte_SetRelayOn(ch); /* Request to turn on the relay */
    }
    else
    {
        CanM_Rte_SetRelayOff(ch); /* Request to turn off the relay */
    }
}

void CanM_Rte_Main_Function(void)
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
        CanM_Rte_CanFaultHandler(ch);
        /* Set the error handler for the charging station */
        CanM_Set_ErrHdl_ChargingStation(ch, ChargeConditions);
        /* Handle relay control based on the request status */
        CanM_Rte_RelayControlHandler(ch);
    }
}
