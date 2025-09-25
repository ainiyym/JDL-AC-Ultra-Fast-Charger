//****************************************************************************************
//*
//* File Name: STD_Rtc_Cfg.h 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_RTC_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_RTC_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Rtc_Lib.h"
#include "Mcal_Rtc_Cfg.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/


/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void RTCIF_Init(Mcal_RTC_DateTime_t lv_stDateTime);
extern void RTCIF_GetDateTime(RtcTimedate_Struct *lv_stDateTime);
extern void RTCIF_SetDateTime(RtcTimedate_Struct *lv_stDateTime);
extern void RTCIF_RestRtc(void);
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
