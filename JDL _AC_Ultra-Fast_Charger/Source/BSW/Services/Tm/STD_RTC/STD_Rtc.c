//****************************************************************************************
//*
//* File Name: STD_Rtc.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/* polyspace:begin<MISRA-C3:11.4:Not a defect:Justified> Normal */
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Rtc.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */

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
static RtcTimedate_Struct gv_stDateTime;
/*! Initialization state of services */

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/*******************************************************************************
Name            : RTC_GetRtcSeconds
Syntax          : uint8_t RTC_GetRtcSeconds(uint32_t *lv_ulSenconds)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ulSenconds       :
Parameters(out) : None                :
Return value    : None
Description     : Get rtc seconds
Call By         : TM_CalibModeHandle,TM_NormalModeHandle
|******************************************************************************/
uint8_t RTC_GetRtcSeconds(uint32_t *lv_ulSenconds)
{
	uint8_t lv_ucRet = STD_FALSE;
	RTCIF_GetDateTime(&gv_stDateTime);
	if (STD_TRUE == RTC_IsTimeDateCorrectFormat(&gv_stDateTime))
	{
		RTC_ConvertTimeDateToSeconds(&gv_stDateTime, lv_ulSenconds);
		lv_ucRet = STD_TRUE;
	}
	else
	{
		lv_ucRet = STD_FALSE;
	}
	return lv_ucRet;
}

uint8_t RTC_GetCP56Time2a(uint8_t *cp56_data)
{
	uint8_t lv_ucRet = STD_FALSE;
	RTCIF_GetDateTime(&gv_stDateTime);
	if (STD_TRUE == RTC_IsTimeDateCorrectFormat(&gv_stDateTime))
	{
		RTC_DatetimeToCp56time2a(&gv_stDateTime, cp56_data);
		lv_ucRet = STD_TRUE;
	}
	else
	{
		lv_ucRet = STD_FALSE;
	}
	return lv_ucRet;
}
/*******************************************************************************
Name            : RTC_SetRtcDateTime
Syntax          : uint8_t RTC_SetRtcDateTime(uint32_t lv_ulSenconds)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ulSenconds       :
Parameters(out) : None                :
Return value    : None
Description     : Set rtc datatime
Call By         : TM_CalibModeHandle
|******************************************************************************/
uint8_t RTC_SecondsSetRtcDateTime(uint32_t lv_ulSenconds)
{
	uint8_t lv_ucRet = STD_FALSE;
	RTC_ConvertSecondsToTimeDate(&lv_ulSenconds, &gv_stDateTime);
	if (STD_TRUE == RTC_IsTimeDateCorrectFormat(&gv_stDateTime))
	{
		RTCIF_SetDateTime(&gv_stDateTime);
		lv_ucRet = STD_TRUE;
	}
	else
	{
		lv_ucRet = STD_FALSE;
	}
	return lv_ucRet;
}

uint8_t RTC_CP56Time2aSetRtcDateTime(uint8_t *cp56_data)
{
	uint8_t lv_ucRet = STD_FALSE;
	RTC_Cp56time2aToDatetime(cp56_data, &gv_stDateTime);
	if (STD_TRUE == RTC_IsTimeDateCorrectFormat(&gv_stDateTime))
	{
		RTCIF_SetDateTime(&gv_stDateTime);
		lv_ucRet = STD_TRUE;
	}
	else
	{
		lv_ucRet = STD_FALSE;
	}
	return lv_ucRet;
}
/*EOF*/

