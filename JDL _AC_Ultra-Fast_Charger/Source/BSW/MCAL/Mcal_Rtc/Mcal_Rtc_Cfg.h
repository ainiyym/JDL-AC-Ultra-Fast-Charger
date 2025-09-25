//******************************************************************************
//* File Name: Mcal_Rtc_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Rtc module configuration header file
/*******************************************************************************/
#if !defined (_MCAL_RTC_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _MCAL_RTC_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "rtc.h"
#include "Mcal_App_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    RTC_DateTypeDef Date;
    RTC_TimeTypeDef Time;
}Mcal_RTC_DateTime_t;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Mcal_RTC_InitTime(void);
extern void Mcal_RTC_SetDateTime(Mcal_RTC_DateTime_t DateTime);
extern void Mcal_RTC_Get_DateTime(Mcal_RTC_DateTime_t *DateTime);
#endif /* _MCAL_RTC_CFG_H */
/* EOL */
