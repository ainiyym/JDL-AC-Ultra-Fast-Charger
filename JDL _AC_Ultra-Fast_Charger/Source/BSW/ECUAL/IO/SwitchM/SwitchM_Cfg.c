//****************************************************************************************
//*
//* File Name: SwitchM_Cfg.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-07 17:15:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include"SwitchM_Cfg.h"

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
static void SWITCHM_SetCp4V(SysConnector_Num_Enum ch);
static void SWITCHM_SetCp12V(SysConnector_Num_Enum ch);
static void SwitchM_SetInToCanMode(SysConnector_Num_Enum ch);
static void SwitchM_SetInToCcCpMode(SysConnector_Num_Enum ch);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
static void SWITCHM_SetCp4V(SysConnector_Num_Enum ch)
{
    /* 电路特性，必须先拉低12V */
    if (SYS_CONNECTOR1 == ch)
    {
        Mcal_Gpio_ResetPin(Switch1_12V_GPIO_Port, Switch1_12V_Pin);
        Mcal_Gpio_ResetPin(Switch1_4V_GPIO_Port, Switch1_4V_Pin);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == ch)
    {
        Mcal_Gpio_ResetPin(Switch2_12V_GPIO_Port, Switch2_12V_Pin);
        Mcal_Gpio_ResetPin(Switch2_4V_GPIO_Port, Switch2_4V_Pin);
    }
#endif
    else
    {
    }
}

static void SWITCHM_SetCp12V(SysConnector_Num_Enum ch)
{
    /* 电路特性，必须先拉高4V */
    if (SYS_CONNECTOR1 == ch)
    {
        Mcal_Gpio_SetPin(Switch1_4V_GPIO_Port, Switch1_4V_Pin);
        Mcal_Gpio_SetPin(Switch1_12V_GPIO_Port, Switch1_12V_Pin);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == ch)
    {
        Mcal_Gpio_SetPin(Switch2_4V_GPIO_Port, Switch2_4V_Pin);
        Mcal_Gpio_SetPin(Switch2_12V_GPIO_Port, Switch2_12V_Pin);
    }
#endif
    else
    {
    }
}

static void SwitchM_SetInToCanMode(SysConnector_Num_Enum ch)
{
    SWITCHM_DEBUG("ch:%d SetToCanMode\r\n",ch);
    if (SYS_CONNECTOR1 == ch)
    {
        Mcal_Gpio_SetPin(CpCanSwitch1_GPIO_Port, CpCanSwitch1_Pin);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == ch)
    {
        Mcal_Gpio_SetPin(CpCanSwitch2_GPIO_Port, CpCanSwitch2_Pin);
    }
#endif
    else
    {
    }
}

static void SwitchM_SetInToCcCpMode(SysConnector_Num_Enum ch)
{
    SWITCHM_DEBUG("ch:%d SetToCcCpMode\r\n",ch);
    if (SYS_CONNECTOR1 == ch)
    {
        Mcal_Gpio_ResetPin(CpCanSwitch1_GPIO_Port, CpCanSwitch1_Pin);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == ch)
    {
        Mcal_Gpio_ResetPin(CpCanSwitch2_GPIO_Port, CpCanSwitch2_Pin);
    }
#endif
    else
    {
    }
}

void SwitchM_SetCpMosStatus(SysConnector_Num_Enum ch, SwitchM_CcCpSwitchMode_Enum mode)
{
    if (mode == SWITCHM_CC_CP_MODE)
    {
        SwitchM_SetInToCcCpMode(ch);
    }
    else if (mode == SWITCHM_CAN_MODE)
    {
        SwitchM_SetInToCanMode(ch);
    }
    else
    {
        SWITCHM_ERROR("ch:%d Invalid mode\r\n", ch);
    }
}

void SwitchM_SetCpVol4vMode(SysConnector_Num_Enum ch)
{
    SWITCHM_DEBUG("ch:%d SetCpVol4v\r\n",ch);
    SWITCHM_SetCp4V(ch);
    SWITCHM_SET_SYSM_CP_MODE(ch, (uint8_t)SWITCHM_CP_4V);
}

void SwitchM_SetCpVol12vMode(SysConnector_Num_Enum ch)
{
    SWITCHM_DEBUG("ch:%d SetCpVol12v\r\n",ch);
    SWITCHM_SetCp12V(ch);
    SWITCHM_SET_SYSM_CP_MODE(ch, (uint8_t)SWITCHM_CP_12V);
}
