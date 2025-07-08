//****************************************************************************************
//*
//* File Name: STD_MosDrv.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Lib.h"
#include "STD_MosDrv.h"
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
|    Static local KAM variables Declaration
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

///*******************************************************************************
//Name              : MOSDRV_SetAllConnectorOpen
//Syntax            : static void MOSDRV_SetConnectorOn(SysConnector_Num_Enum ch)
//Sync/Async        : Synchronous
//Reentrancy        : None
//Parameters(in)    : None
//Parameters(out)   : None
//Return value      : None
//Description       : Set contator open
//Call By           : MOSDRV_ReqMosOn
//|******************************************************************************/
static void MOSDRV_SetConnectorOn(SysConnector_Num_Enum ch)
{
	if (SYS_CONNECTOR1 == ch)
	{
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR1_ON, MOSDRV_HIGH);
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR1_OFF, MOSDRV_LOW);
	}
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
	else if (SYS_CONNECTOR2 == ch)
	{
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR2_ON, MOSDRV_HIGH);
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR2_OFF, MOSDRV_LOW);
	}
#endif
	else
	{
	}
}

///*******************************************************************************
//Name              : MOSDRV_SetConnectorOff
//Syntax            : void MOSDRV_SetConnectorOff(void)
//Sync/Async        : Synchronous
//Reentrancy        : None
//Parameters(in)    : None
//Parameters(out)   : None
//Return value      : None
//Description       : Set contactor off
//Call By           : MOSDRV_ReqMosOff
//|******************************************************************************/
static void MOSDRV_SetConnectorOff(SysConnector_Num_Enum ch)
{
	if (SYS_CONNECTOR1 == ch)
	{
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR1_ON, MOSDRV_LOW);
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR1_OFF, MOSDRV_HIGH);
	}
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
	else if (SYS_CONNECTOR2 == ch)
	{
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR2_ON, MOSDRV_LOW);
		MOSDRV_WriteGpioValue(MOS_DRV_CONNECTOR2_OFF, MOSDRV_HIGH);
	}
#endif
	else
	{
	}
}

/*******************************************************************************
Name              : MOSDRV_ReqMosOn
Syntax            : uint8_t MOSDRV_ReqMosOn(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : ch
Parameters(out)   : None
Return value      : ret
Description       : request mos on
Call By           : RLYCTRL_SwitchControl
|******************************************************************************/
uint8_t MOSDRV_ReqMosOn(SysConnector_Num_Enum ch)
{
	uint8_t ret = STD_TRUE;

	if (ch < SYS_CONNECTOR_NUM_MAX)
	{
		MOSDRV_SetConnectorOn(ch);
		MOSDRV_DEBUG("ucReqMos %d on\r\n", ch);
	}
	else
	{
		ret = STD_FALSE;
	}

	return ret;
}

/*******************************************************************************
Name              : MOSDRV_ReqMosOff
Syntax            : uint8_t MOSDRV_ReqMosOff(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : ch
Parameters(out)   : None
Return value      : ret
Description       : request mos off
Call By           : RLYCTRL_SwitchControl
|******************************************************************************/
uint8_t MOSDRV_ReqMosOff(SysConnector_Num_Enum ch)
{
	uint8_t ret = STD_TRUE;

	if (ch < SYS_CONNECTOR_NUM_MAX)
	{
		MOSDRV_SetConnectorOff(ch);
		MOSDRV_DEBUG("ucReqMos %d off\r\n", ch);
	}
	else
	{
		ret = STD_FALSE;
	}

	return ret;
}

/*******************************************************************************
Name              : MOSDRV_ReadContactorAuxiliaryStatus
Syntax            : uint8_t MOSDRV_ReadContactorAuxiliaryStatus(SysConnector_Num_Enum ch)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : ch
Parameters(out)   : None
Return value      : ret
Description       : READ Auxiliary contact status
Call By           : RLYCTRL_AuxlDetect
|******************************************************************************/
uint8_t MOSDRV_ReadContactorAuxiliaryStatus(SysConnector_Num_Enum ch)
{
	uint8_t status = MOSDRV_LOW;

	if (SYS_CONNECTOR1 == ch)
	{
		status = MOSDRV_ReadAuxiliaryGpioValue(MOS_DRV_CONTACT1_AUXILIARY);
	}
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
	else if (SYS_CONNECTOR2 == ch)
	{
		status = MOSDRV_ReadAuxiliaryGpioValue(MOS_DRV_CONTACT2_AUXILIARY);
	}
#endif
	else
	{
		MOSDRV_ERROR("Invalid connector number: %d\r\n", ch);
	}
	return status;
}
/*EOF*/
