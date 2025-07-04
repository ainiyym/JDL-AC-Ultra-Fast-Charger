//****************************************************************************************
//*
//* File Name: STD_MosDrv_Cfg.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_MosDrv_Cfg.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

//#define MOSDRV_DEBUG(fmt, ...) 	LOG_DEBUG(LOG_MODULE_RLY, fmt, ##__VA_ARGS__)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
static const MosDrv_Struct gv_stMosDrvArry[MOS_DRV_CONTACTIOR_SWITCH_NUM_MAX] = {
	{GPIOE, GPIO_PIN_6, MOS_DRV_LOW},	/* MOS_DRV_CONNECTOR1_OFF */
	{GPIOE, GPIO_PIN_7, MOS_DRV_LOW},	/* MOS_DRV_CONNECTOR1_ON */
	{GPIOE, GPIO_PIN_11, MOS_DRV_LOW},	/* MOS_DRV_CONNECTOR2_OFF */
	{GPIOE, GPIO_PIN_12, MOS_DRV_LOW}	/* MOS_DRV_CONNECTOR2_ON */
};

static const MosDrv_Struct gv_stMosDrvShortSelfCheckArry[MOS_SHORT_SELFCHECK_NUM_MAX] = {
	/*{MCAL_PORT_B, MCAL_PIN_9,MOS_DRV_HIGH}*/
};
/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
 /*******************************************************************************
 Name            : MOSDRV_WriteGpioValue
 Syntax          : void MOSDRV_WriteGpioValue(uint8_t lv_ucIndex,uint8_t lv_ucLevel)
 Sync/Async      : Synchronous
 Reentrancy      :
 Parameters(in)  : lv_ucIndex ,lv_ucLevel       :-
 Parameters(in)  : None                         :-
 Parameters(out) : None                         :-
 Return value    : None
 Description     : Write GPIO
 Call By         : MOSDRV_SetAllConnectorOpen MOSDRV_SetConnectorOff MOSDRV_SelfCheckMainFonction
 |******************************************************************************/
void MOSDRV_WriteGpioValue(uint8_t lv_ucIndex,uint8_t lv_ucLevel)
{
	if (MOSDRV_HIGH == lv_ucLevel)
	{
		Mcal_Gpio_SetPin(gv_stMosDrvArry[lv_ucIndex].ucGpioPort,gv_stMosDrvArry[lv_ucIndex].ucGpioPinIndx);
	}
	else
	{
		Mcal_Gpio_ResetPin(gv_stMosDrvArry[lv_ucIndex].ucGpioPort,gv_stMosDrvArry[lv_ucIndex].ucGpioPinIndx);
	}
}
/*******************************************************************************
Name            : MOSDRV_ReadGpioValue
Syntax          : uint8_t MOSDRV_ReadGpioValue(uint8_t lv_ucIndex)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucIndex             :-
Parameters(out) : None                   :-
Return value    : None
Description     : Read GPIO
Call By         : MOSDRV_SelfCheckMainFonction
|******************************************************************************/
uint8_t MOSDRV_ReadGpioValue(uint8_t lv_ucIndex)
{
	 uint8_t lv_ucRelust;
	 if(gv_stMosDrvArry[lv_ucIndex].ucGpioLevelLower == Mcal_Gpio_ReadPinStatus(gv_stMosDrvArry[lv_ucIndex].ucGpioPort,gv_stMosDrvArry[lv_ucIndex].ucGpioPinIndx))
	 {
		  lv_ucRelust = MOSDRV_LOW;
	 }
	 else
	 {
		  lv_ucRelust = MOSDRV_HIGH;
	 }
	 return lv_ucRelust;
}
/*******************************************************************************
Name            : MOSDRV_ShortSelfCheckWriteGpioValue
Syntax          : void MOSDRV_ShortSelfCheckWriteGpioValue(uint8_t lv_ucIndex,uint8_t lv_ucLevel)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucIndex ,lv_ucLevel       :-
Parameters(in)  :                              :-
Parameters(out) : None                         :-
Return value    : None
Description     : Write gpio Short circuit self check
Call By         : MOSDRV_ShortSelfCheckSetAllOn MOSDRV_ShortSelfCheckSetAllOff
|******************************************************************************/
void MOSDRV_ShortSelfCheckWriteGpioValue(uint8_t lv_ucIndex, uint8_t lv_ucLevel)
{
	if (MOSDRV_HIGH == lv_ucLevel)
	{
		Mcal_Gpio_SetPin(gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPort, gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPinIndx);
	}
	else
	{
		Mcal_Gpio_ResetPin(gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPort, gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPinIndx);
	}
}
/*******************************************************************************
Name            : MOSDRV_ShortSelfCheckReadGpioValue
Syntax          : uint8_t MOSDRV_ShortSelfCheckReadGpioValue(uint8_t lv_ucIndex)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucIndex             :-
Parameters(out) : None                   :-
Return value    : None
Description     : Read gpio Short circuit self check
Call By         : -
|******************************************************************************/
uint8_t MOSDRV_ShortSelfCheckReadGpioValue(uint8_t lv_ucIndex)
{
	uint8_t lv_ucRelust;

	if (gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioLevelLower == Mcal_Gpio_ReadPinStatus(gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPort, gv_stMosDrvShortSelfCheckArry[lv_ucIndex].ucGpioPinIndx))
	{
		lv_ucRelust = MOSDRV_LOW;
	}
	else
	{
	}
	lv_ucRelust = MOSDRV_HIGH;

	return lv_ucRelust;
}

/*EOF*/
