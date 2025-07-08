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
	{Relay1ControlStop_GPIO_Port, 	Relay1ControlStop_Pin, MOS_DRV_LOW},		/* MOS_DRV_CONNECTOR1_OFF */
	{Relay1ControlStart_GPIO_Port, 	Relay1ControlStart_Pin, MOS_DRV_LOW},		/* MOS_DRV_CONNECTOR1_ON */
	{Relay2ControlStop_GPIO_Port, 	Relay2ControlStop_Pin, MOS_DRV_LOW},		/* MOS_DRV_CONNECTOR2_OFF */
	{Relay2ControlStart_GPIO_Port, 	Relay2ControlStart_Pin, MOS_DRV_LOW}		/* MOS_DRV_CONNECTOR2_ON */
};

static const MosDrv_Struct gv_stMosDrvAuxiliaryContactStatusArry[MOS_DRV_CONTACTIOR_AUXILIARY_NUM_MAX] = {
	{Contactor1DetectsContacts_GPIO_Port, Contactor1DetectsContacts_Pin,MOS_DRV_LOW},
	{Contactor2DetectsContacts_GPIO_Port, Contactor2DetectsContacts_Pin,MOS_DRV_LOW}
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
Name            : MOSDRV_ReadAuxiliaryGpioValue
Syntax          : uint8_t MOSDRV_ReadAuxiliaryGpioValue(uint8_t lv_ucIndex)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucIndex             :-
Parameters(out) : None                   :-
Return value    : None
Description     : Read contactor Auxiliary circuit status
Call By         : -
|******************************************************************************/
uint8_t MOSDRV_ReadAuxiliaryGpioValue(uint8_t lv_ucIndex)
{
	uint8_t lv_ucRelust;

	if (gv_stMosDrvAuxiliaryContactStatusArry[lv_ucIndex].ucGpioLevelLower == Mcal_Gpio_ReadPinStatus(gv_stMosDrvAuxiliaryContactStatusArry[lv_ucIndex].ucGpioPort, gv_stMosDrvAuxiliaryContactStatusArry[lv_ucIndex].ucGpioPinIndx))
	{
		lv_ucRelust = MOSDRV_LOW;
	}
	else
	{
		lv_ucRelust = MOSDRV_HIGH;
	}

	return lv_ucRelust;
}

/*EOF*/
