//****************************************************************************************
//*
//* File Name: STD_Rtc_Lib.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_RTC_LIB_H)
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define _STD_RTC_LIB_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Rtc_Lib_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    LOWER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    UPPER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    Callback Notification
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/


/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint16_t usYear;      /*!< Year       */
    uint16_t usMonth;     /*!< Month      */
    uint16_t usDay;       /*!< Day        */
    uint16_t usHour;      /*!< Hour       */
    uint16_t usMinutes;   /*!< Minutes    */
    uint8_t ucSeconds;    /*!< Seconds    */
} RtcTimedate_Struct;

typedef struct
{
    uint16_t milliseconds;    // 16位毫秒 (0-59999)
    uint8_t minutes : 6;      // 6位分钟 (0-59)
    uint8_t res1 : 1;         // 保留位1
    uint8_t invalid : 1;      // IV标志 (0=有效, 1=无效)
    uint8_t hours : 5;        // 5位小时 (0-23)
    uint8_t res2 : 2;         // 保留位2
    uint8_t summer_time : 1;  // SU标志 (0=标准时间, 1=夏令时)
    uint8_t day_of_month : 5; // 5位日期 (1-31)
    uint8_t day_of_week : 3;  // 3位星期 (1-7)
    uint8_t month : 4;        // 4位月份 (1-12)
    uint8_t res3 : 4;         // 保留位3
    uint8_t year : 7;         // 7位年份 (0-99, 0=2000年)
    uint8_t res4 : 1;         // 保留位4
} CP56Time2a_t;
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern uint8_t RTC_IsTimeDateCorrectFormat(RtcTimedate_Struct * lv_sttimeDate);
extern void RTC_ConvertSecondsToTimeDate(uint32_t * lv_ulseconds, RtcTimedate_Struct * lv_sttimeDate);
extern void RTC_ConvertTimeDateToSeconds(RtcTimedate_Struct * lv_sttimeDate, uint32_t * lv_ulseconds);
extern void RTC_Cp56time2aToDatetime(const uint8_t *cp56_data, RtcTimedate_Struct *datetime);
void RTC_DatetimeToCp56time2a(const RtcTimedate_Struct *datetime, uint8_t *cp56_data);
#endif
/*EOF*/
