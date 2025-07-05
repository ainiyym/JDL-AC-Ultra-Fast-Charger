//****************************************************************************************
//*
//* File Name: SwitchM.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#include"SwitchM.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
    M74HCT4851D_A0 = 0u,
    M74HCT4851D_A1,
    M74HCT4851D_A2,
    M74HCT4851D_A3, 
    M74HCT4851D_A4,
    M74HCT4851D_A5,
    M74HCT4851D_A6,    
	M74HCT4851D_A7
};
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t CpVolMode[SYS_CONNECTOR_NUM_MAX];   /* cp based vol */
    uint8_t M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_CH_NUM];        /* 8-channel selector input status */
    uint8_t M74hct4851dIncrementCounter; /* 74hct4851d Mode switching auto-incrementing counter */
    uint8_t SoftGptChannel;
} SwitchM_Ctrl_t;

/*******************************************************************************
|    Static local variables Declaration  
|******************************************************************************/
static SwitchM_Ctrl_t SwitchMCtrl = {0u};
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void SWITCHM_SetCp4V(SysConnector_Num_Enum ch);
static void SWITCHM_SetCp12V(SysConnector_Num_Enum ch);
static void SwitchM_Set74hct4851dOutputA0(void);
static void SwitchM_Set74hct4851dOutputA1(void);
static void SwitchM_Set74hct4851dOutputA2(void);
static void SwitchM_Set74hct4851dOutputA3(void);
static void SwitchM_Set74hct4851dOutputA4(void);
static void SwitchM_Set74hct4851dOutputA5(void);
static void SwitchM_Set74hct4851dOutputA6(void);
static void SwitchM_Set74hct4851dOutputA7(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void SwitchM_Init(void)
{
    for (SysConnector_Num_Enum ch = SYS_CONNECTOR1; ch < SYS_CONNECTOR_NUM_MAX; ch++)
    {
        SwitchM_SetCpVol12vMode(ch);
    }
    SwitchMCtrl.M74hct4851dIncrementCounter = M74HCT4851D_A0;
}

uint8_t SwitchM_GetCpVolMode(SysConnector_Num_Enum ch)
{
    return (uint8_t)SwitchMCtrl.CpVolMode[ch];
}

static void SWITCHM_SetCp4V(SysConnector_Num_Enum ch)
{
    if (SYS_CONNECTOR1 == ch)
    {
        Mcal_Gpio_ResetPin(Switch1_4V_GPIO_Port, Switch1_4V_Pin);
        Mcal_Gpio_ResetPin(Switch1_12V_GPIO_Port, Switch1_12V_Pin);
    }
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    else if (SYS_CONNECTOR2 == ch)
    {
        Mcal_Gpio_ResetPin(Switch2_4V_GPIO_Port, Switch2_4V_Pin);
        Mcal_Gpio_ResetPin(Switch2_12V_GPIO_Port, Switch2_12V_Pin);
    }
#endif
    else
    {
    }
}

static void SWITCHM_SetCp12V(SysConnector_Num_Enum ch)
{
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

void SwitchM_SetCpVol4vMode(SysConnector_Num_Enum ch)
{
    // Switch to CAN mode code here
    SWITCHM_SetCp4V(ch);
    SwitchMCtrl.CpVolMode[ch] = (uint8_t)SWITCHM_CP_4V;
    SWITCHM_DEBUG("ch:%d SetCpVol4v\r\n",ch);
}

void SwitchM_SetCpVol12vMode(SysConnector_Num_Enum ch)
{
    // Switch to CP/CC mode code here
    SWITCHM_SetCp12V(ch);
    SwitchMCtrl.CpVolMode[ch] = (uint8_t)SWITCHM_CP_12V;
    SWITCHM_DEBUG("ch:%d SetCpVol12v\r\n",ch);
}

void SwitchM_Set74hct4851dEnable(void)
{
    Mcal_Gpio_ResetPin(ACE_GPIO_Port, ACE_Pin);
}

void SwitchM_Set74hct4851dDisable(void)
{
    Mcal_Gpio_SetPin(ACE_GPIO_Port, ACE_Pin);
}

static void SwitchM_Set74hct4851dOutputA0(void)
{
    Mcal_Gpio_ResetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_ResetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_ResetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_CC1;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_CONNECTOR1_FRONT_TEMP;
}

static void SwitchM_Set74hct4851dOutputA1(void)
{
    Mcal_Gpio_SetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_ResetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_ResetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_CC2;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_CONNECTOR1_REAR_TEMP;
}

static void SwitchM_Set74hct4851dOutputA2(void)
{
    Mcal_Gpio_ResetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_SetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_ResetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_CP1;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_OIL1_INLET_TEMP;
}

static void SwitchM_Set74hct4851dOutputA3(void)
{
    Mcal_Gpio_SetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_SetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_ResetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_CP2;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_OIL1_OUTLET_TEMP;    
}

static void SwitchM_Set74hct4851dOutputA4(void)
{
    Mcal_Gpio_ResetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_ResetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_SetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_12V;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_CONNECTOR2_FRONT_TEMP;
}

static void SwitchM_Set74hct4851dOutputA5(void)
{
    Mcal_Gpio_SetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_ResetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_SetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_5V;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_CONNECTOR2_REAR_TEMP;
}

static void SwitchM_Set74hct4851dOutputA6(void)
{
    Mcal_Gpio_ResetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_SetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_SetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_3V3;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_OIL2_INLET_TEMP;
}

static void SwitchM_Set74hct4851dOutputA7(void)
{
    Mcal_Gpio_SetPin(AS0_GPIO_Port, AS0_Pin);
    Mcal_Gpio_SetPin(AS1_GPIO_Port, AS1_Pin);
    Mcal_Gpio_SetPin(AS2_GPIO_Port, AS2_Pin);
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC0] = SWITCHM_74HCT4851D_GND;
    SwitchMCtrl.M74hct4851dWorkingStatus[SWITCHM_74HCT4851D_MULTIPLEX_ADC1] = SWITCHM_74HCT4851D_OIL2_OUTLET_TEMP;
}

uint8_t SwitchM_Get74hct4851dOutputStatusCallBack(SwitchM_74HCT4851D_Multiplex_e CH)
{
    return SwitchMCtrl.M74hct4851dWorkingStatus[CH];
}

void SwitchM_74hct4851dControlCallBack(void)
{
    switch (SwitchMCtrl.M74hct4851dIncrementCounter)
    {
    case M74HCT4851D_A0:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA0();
        break;

    case M74HCT4851D_A1:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA1();
        break;

    case M74HCT4851D_A2:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA2();
        break;

    case M74HCT4851D_A3:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA3();
        break;

    case M74HCT4851D_A4:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA4();
        break;

    case M74HCT4851D_A5:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA5();
        break;

    case M74HCT4851D_A6:
        SwitchMCtrl.M74hct4851dIncrementCounter++;
        SwitchM_Set74hct4851dOutputA6();
        break;

    case M74HCT4851D_A7:
        SwitchMCtrl.M74hct4851dIncrementCounter = M74HCT4851D_A0;
        SwitchM_Set74hct4851dOutputA7();
        break;

    default:

        break;
    }
}

void SwitchM_SoftTimerStart74hct4851d_Enable(void)
{
	(void)OS_SoftTimerStart(&SwitchMCtrl.SoftGptChannel, SwitchM_74hct4851dControlCallBack, SWITCHM_74HCT4851D_SOFTTIMER_PERIOD, 1);  
}
