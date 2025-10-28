//****************************************************************************************
//*
//* File Name: STD_Rtc_Cfg.c 
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
#include "STD_Rtc_Cfg.h"
#include "Mcal_Rtc_Cfg.h"

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
/*! Initialization state of services */


/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
Mcal_RTC_DateTime_t gv_stSetDateTime;

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void  RTCIF_Init(Mcal_RTC_DateTime_t lv_stDateTime)
{
	gv_stSetDateTime = lv_stDateTime;
	MCAL_INFO("INIT RTC TIME: %04d-%02d-%02d %02d:%02d:%02d\n",
		gv_stSetDateTime.Date.Year + 2000, gv_stSetDateTime.Date.Month, gv_stSetDateTime.Date.Date,
		gv_stSetDateTime.Time.Hours, gv_stSetDateTime.Time.Minutes, gv_stSetDateTime.Time.Seconds);
}
/*******************************************************************************
Name            : RTC_SoftWareSetTime
Syntax          : void RTC_SoftWareSetTime(RtcTimedate_Struct *lv_stDateTime)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_stDateTime                :
Parameters(out) : lv_stDateTime                :
Return value    : None
Description     : Set software RTC data time
Call By         : RTC_SetRtcSeconds
|******************************************************************************/
void  RTCIF_SetDateTime(RtcTimedate_Struct *lv_stDateTime)
{
	gv_stSetDateTime.Date.Year = lv_stDateTime->usYear - 2000;
	gv_stSetDateTime.Date.Month = lv_stDateTime->usMonth;
	gv_stSetDateTime.Date.Date = lv_stDateTime->usDay;

	gv_stSetDateTime.Time.Hours =lv_stDateTime->usHour;
	gv_stSetDateTime.Time.Minutes = lv_stDateTime->usMinutes;
	gv_stSetDateTime.Time.Seconds =lv_stDateTime->ucSeconds;
	
	Mcal_RTC_SetDateTime(gv_stSetDateTime);
	MCAL_INFO("SET RTC TIME: %04d-%02d-%02d %02d:%02d:%02d\n",
		gv_stSetDateTime.Date.Year + 2000, gv_stSetDateTime.Date.Month, gv_stSetDateTime.Date.Date,
		gv_stSetDateTime.Time.Hours, gv_stSetDateTime.Time.Minutes, gv_stSetDateTime.Time.Seconds);
}
/*******************************************************************************
Name            : RTC_SoftWareGetTime
Syntax          : void RTC_SoftWareGetTime(RtcTimedate_Struct *lv_stDateTime)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_stDateTime                :
Parameters(out) : lv_stDateTime                :
Return value    : None
Description     : Get software RTC data time
Call By         : RTC_GetRtcSeconds
|******************************************************************************/
void RTCIF_GetDateTime(RtcTimedate_Struct *lv_stDateTime)
{
	Mcal_RTC_Get_DateTime(&gv_stSetDateTime);
	lv_stDateTime->usYear = gv_stSetDateTime.Date.Year;
	lv_stDateTime->usYear += 2000;
	lv_stDateTime->usMonth = gv_stSetDateTime.Date.Month;
	lv_stDateTime->usDay = gv_stSetDateTime.Date.Date;

	lv_stDateTime->usHour = gv_stSetDateTime.Time.Hours;
	lv_stDateTime->usMinutes = gv_stSetDateTime.Time.Minutes;
	lv_stDateTime->ucSeconds = gv_stSetDateTime.Time.Seconds;
}
/*******************************************************************************
Name            : RTC_SoftWareRestRtc
Syntax          : void RTC_SoftWareRestRtc(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  :               :
Parameters(out) :               :
Return value    : None
Description     : Reset RTC
Call By         : TM_CalibModeHandle
|******************************************************************************/
void RTCIF_RestRtc(void)
{

}
/*EOF*/
