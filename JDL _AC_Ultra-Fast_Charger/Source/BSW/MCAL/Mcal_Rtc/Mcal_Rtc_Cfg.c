//******************************************************************************
//* File Name: Mcal_Rtc_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: RTC module source configuration  file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Mcal_Rtc_Cfg.h"
#include "STD_Rtc_Cfg.h"

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
void Mcal_RTC_InitTime(void)
{
    MX_RTC_Init();

    Mcal_RTC_DateTime_t data_time;

    HAL_RTC_GetTime(&hrtc, &data_time.Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &data_time.Date, RTC_FORMAT_BIN);

    RTCIF_Init(data_time);
}

// Set the RTC date and time
void Mcal_RTC_SetDateTime(Mcal_RTC_DateTime_t DateTime)
{
    // Set the time
    if (HAL_RTC_SetTime(&hrtc, &DateTime.Time, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    // Set the date
    if (HAL_RTC_SetDate(&hrtc, &DateTime.Date, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

// Get the RTC date and time
void Mcal_RTC_Get_DateTime(Mcal_RTC_DateTime_t *DateTime)
{
    // Get the time
    if (HAL_RTC_GetTime(&hrtc, &DateTime->Time, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    // Get the date
    if (HAL_RTC_GetDate(&hrtc, &DateTime->Date, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}
/* EOL */
