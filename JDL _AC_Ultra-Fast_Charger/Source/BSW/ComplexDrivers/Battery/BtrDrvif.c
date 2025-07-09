//****************************************************************************************
//*
//* File Name: BtrDrvif.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
 /* polyspace:begin<MISRA-C3:14.3:Not a defect:Other> NO fluence */
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "BtrDrvif_Cfg.h"
#include "STD_Os_Timer.h"
#include "STD_Fifo.h"
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
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static uint8_t c_enGptChannel = 0u;

/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence */


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
******************************************************************************/
/******************************************************************************
Name            : BTRDRV_GetAdcValue
Syntax          : void BTRDRV_GetAdcValue(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Get Adc Value From Mcal Level
Call By         :
|******************************************************************************/
static void BTRDRV_GetAdcValue(void)
{
    uint16_t lv_ulAdcValue = 0;
    lv_ulAdcValue = BtrDrvif_GetCpVolAdcValue(BTR_ADC1_12V_CH);
    (void)FIFO_InsertData(FIFO_CHAN_IN_12V_VOLT, (uint16_t)lv_ulAdcValue);
    lv_ulAdcValue = BtrDrvif_GetCpVolAdcValue(BTR_ADC1_5V_CH);
    (void)FIFO_InsertData(FIFO_CHAN_IN_5V_VOLT, (uint16_t)lv_ulAdcValue);
}

/*******************************************************************************
Name            : BTRDRV_StartAdcCollection
Syntax          : void BTRDRV_StartAdcCollection(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Start Adc Collection
Call By         :
|******************************************************************************/
void BTRDRV_StartAdcCollection(void)
{
    (void)OS_SoftTimerStart(&c_enGptChannel, BTRDRV_GetAdcValue, BTR_ADC_COLECTION_PERIOD, 1);
}
/*******************************************************************************
Name            : BTRDRV_StopAdcCollection
Syntax          : void BTRDRV_StopAdcCollection(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Stop Adc Collection
Call By         :
|******************************************************************************/
void BTRDRV_StopAdcCollection(void)
{
    OS_SoftTimerStop(c_enGptChannel);
}
