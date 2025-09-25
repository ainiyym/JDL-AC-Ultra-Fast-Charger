//****************************************************************************************
//*
//* File Name: STD_Rtc_Lib.c
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
#include "Std_Types.h"
#include "STD_Rtc_Lib.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define RTC_SECONDS_IN_A_YEAR		(31536000UL)    /*Time seconds of one year*/
#define RTC_SECONDS_IN_A_DAY		(86400UL)       /*Time seconds of one day*/
#define RTC_SECONDS_IN_A_HOUR		(3600U)         /*Time seconds of one hour*/
#define RTC_SECONDS_IN_A_MIN		(60U)           /*Time seconds of one min*/
#define RTC_MINS_IN_A_HOUR			(60U)           /*Time mintues of one hour*/
#define RTC_HOURS_IN_A_DAY			(24U)           /*Time hours of one day*/
#define RTC_DAYS_IN_A_YEAR			(365U)          /*Time days of one year*/
#define RTC_DAYS_IN_A_LEAP_YEAR		(366U)          /*Time days in leapyear*/

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
/*!< Table of month length (in days) for the Un-leap-year*/
static const uint8_t gv_arUnLeapYearMonthArr[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};
/*!< Table of month length (in days) for the Leap-year*/
static const uint8_t gv_arLeapYearMonthArr[13u] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};
/*!< Number of days from begin of the non Leap-year*/
static const uint16_t gv_arMonthDaysArr[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/

uint8_t RTC_IsYearLeap(uint16_t lv_usyear)
{
	uint8_t lv_ucisYearLeap;
    if ((lv_usyear % 4U) > 0U)
    {
    	lv_ucisYearLeap = STD_FALSE;
    }
    else if ((lv_usyear % 100U) > 0U)
    {
    	lv_ucisYearLeap = STD_TRUE;
    }
    else if ((lv_usyear % 400U) > 0U)
    {
    	lv_ucisYearLeap = STD_FALSE;
    }
    else
    {
    	lv_ucisYearLeap = STD_TRUE;
    }
    /* Return the exit code */
    return lv_ucisYearLeap;
}

uint8_t RTC_IsTimeDateCorrectFormat(RtcTimedate_Struct * lv_sttimeDate)
{
    /* Declare local variables */
    uint8_t lv_ucreturnCode = STD_FALSE;
    const uint8_t * lv_ucpDays;

    if(lv_sttimeDate != NULL)
    {
		/* Set the days-in-month table for the corresponding year */
    	lv_ucpDays = RTC_IsYearLeap(lv_sttimeDate->usYear) ? (gv_arLeapYearMonthArr) : (gv_arUnLeapYearMonthArr);

		/* Check if the time and date are in the correct ranges */
		if ((lv_sttimeDate->usYear < RTC_YEAR_RANGE_START) || (lv_sttimeDate->usYear > RTC_YEAR_RANGE_END)
				||  (lv_sttimeDate->usMonth < 1U) || (lv_sttimeDate->usMonth > 12U)
				||  (lv_sttimeDate->usDay < 1U) || (lv_sttimeDate->usDay > lv_ucpDays[lv_sttimeDate->usMonth])
				||  (lv_sttimeDate->usHour >= RTC_HOURS_IN_A_DAY)
				||  (lv_sttimeDate->usMinutes >= RTC_MINS_IN_A_HOUR) || (lv_sttimeDate->ucSeconds >= RTC_SECONDS_IN_A_MIN))
		{
			lv_ucreturnCode = STD_FALSE;
		}
		else
		{
			lv_ucreturnCode = STD_TRUE;
		}
    }
    /* Return the exit code */
    return lv_ucreturnCode;
}

void RTC_ConvertSecondsToTimeDate(uint32_t * lv_ulseconds, RtcTimedate_Struct * lv_sttimeDate)
{
    /* Declare the variables needed */
    uint8_t  i;
    uint8_t  lv_ucyearLeap;
    uint32_t lv_ulallSeconds;
    uint32_t lv_ulnumberOfDays;
    uint32_t lv_ilremainSeconds;
    uint16_t lv_usdaysInYear;

    /* Because the starting year(1970) is not leap, set the daysInYear
     * variable with the number of the days in a normal year*/
    if((lv_ulseconds != NULL)&&(lv_sttimeDate != NULL))
    {
    	lv_ulallSeconds = *lv_ulseconds - RTC_SECONDS_1970_TO_NOW_YEAR;
		/* Set the year to the beginning of the range */
    	lv_sttimeDate->usYear = RTC_YEAR_RANGE_START;
		/* Get the number of days */
    	lv_ulnumberOfDays = lv_ulallSeconds / RTC_SECONDS_IN_A_DAY;
		/* Get the number of seconds remaining */
    	lv_ilremainSeconds = lv_ulallSeconds % RTC_SECONDS_IN_A_DAY;
		/* Get the current hour */
		lv_sttimeDate->usHour        = (uint16_t)(lv_ilremainSeconds / RTC_SECONDS_IN_A_HOUR);
		/* Get the remaining seconds */
		lv_ilremainSeconds           = lv_ilremainSeconds % RTC_SECONDS_IN_A_HOUR;
		/* Get the minutes */
		lv_sttimeDate->usMinutes     = (uint16_t)(lv_ilremainSeconds / RTC_SECONDS_IN_A_MIN);
		/* Get seconds */
		lv_sttimeDate->ucSeconds     = (uint8_t)(lv_ilremainSeconds % RTC_SECONDS_IN_A_MIN);

		if (!RTC_IsYearLeap(lv_sttimeDate->usYear))
		{
			/* Set the number of non leap year to the current year number of days.*/
			lv_usdaysInYear = RTC_DAYS_IN_A_YEAR;
		}
		else
		{
			/* Set the number of leap year to the current year number of days.*/
			lv_usdaysInYear = RTC_DAYS_IN_A_LEAP_YEAR;
		}
		/* Get the current year */
		while (lv_ulnumberOfDays >= lv_usdaysInYear)
		{
			/* Increment year if the number of days is greater than the ones in one year*/
			lv_sttimeDate->usYear++;
			/* Subtract the number of the days */
			lv_ulnumberOfDays -= lv_usdaysInYear;
			/* Check if the year is leap or unleap */
			if (!RTC_IsYearLeap(lv_sttimeDate->usYear))
			{
				/* Set the number of non leap year to the current year number of days.*/
				lv_usdaysInYear = RTC_DAYS_IN_A_YEAR;
			}
			else
			{
				/* Set the number of leap year to the current year number of days.*/
				lv_usdaysInYear = RTC_DAYS_IN_A_LEAP_YEAR;
			}
		}
		/* Add the current day */
		lv_ulnumberOfDays += 1U;
		/* Check if the current year is leap */
		lv_ucyearLeap = RTC_IsYearLeap(lv_sttimeDate->usYear);
		/* Get the month */
		for (i = 1U; i <= 12U; i++)
		{
			uint32_t daysInCurrentMonth = ((lv_ucyearLeap == STD_TRUE) ? (uint32_t)gv_arLeapYearMonthArr[i] : (uint32_t)gv_arUnLeapYearMonthArr[i]);
			if (lv_ulnumberOfDays <= daysInCurrentMonth)
			{
				lv_sttimeDate->usMonth = (uint16_t)i;
				break;
			}
			else
			{
				lv_ulnumberOfDays -= daysInCurrentMonth;
			}
		}
		/* Set the current day */
		lv_sttimeDate->usDay = (uint16_t)lv_ulnumberOfDays;
    }
}

void RTC_ConvertTimeDateToSeconds(RtcTimedate_Struct * lv_sttimeDate, uint32_t * lv_ulseconds)
{
    /* Declare local variables */
    uint16_t lv_usyear;

    if((lv_ulseconds != NULL)&&(lv_sttimeDate != NULL))
    {
		/* Convert years to seconds */
    	(*lv_ulseconds) = RTC_SECONDS_1970_TO_NOW_YEAR;
		(*lv_ulseconds) += ((uint32_t)RTC_SECONDS_IN_A_YEAR * ((uint32_t)lv_sttimeDate->usYear - RTC_YEAR_RANGE_START));

		/* Add the seconds from the leap years */
		for (lv_usyear = RTC_YEAR_RANGE_START; lv_usyear < lv_sttimeDate->usYear; lv_usyear++)
		{
			if (RTC_IsYearLeap(lv_usyear))
			{
				(*lv_ulseconds) += RTC_SECONDS_IN_A_DAY;
			}
		}

		/* If the current year is leap and 29th of February has passed, add another day to seconds passed.*/
		if ((RTC_IsYearLeap(lv_usyear)) && (lv_sttimeDate->usMonth > 2U))
		{
			(*lv_ulseconds) += RTC_SECONDS_IN_A_DAY;
		}

		/* Add the rest of the seconds from the current month */
		(*lv_ulseconds) += gv_arMonthDaysArr[lv_sttimeDate->usMonth] * RTC_SECONDS_IN_A_DAY;
		/* Add the rest of the seconds from the current day */
		(*lv_ulseconds) += (uint32_t)(((uint32_t)lv_sttimeDate->usDay - 1U) * (uint32_t)RTC_SECONDS_IN_A_DAY);
		/* Add the rest of the seconds from the current time */
		(*lv_ulseconds) += (uint32_t)(((uint32_t)lv_sttimeDate->usHour * RTC_SECONDS_IN_A_HOUR)   + \
								 ((uint32_t)lv_sttimeDate->usMinutes * RTC_SECONDS_IN_A_MIN) + \
								 (uint32_t)lv_sttimeDate->ucSeconds);
    }
}

// The CP56Time2a byte stream is converted to a date-time structure
void RTC_Cp56time2aToDatetime(const uint8_t *cp56_data, RtcTimedate_Struct *datetime)
{
	// Parse 7-byte data
	uint16_t milliseconds = (cp56_data[1] << 8) | cp56_data[0];
	uint8_t byte2 = cp56_data[2];
	uint8_t byte3 = cp56_data[3];
	uint8_t byte4 = cp56_data[4];
	uint8_t byte5 = cp56_data[5];
	uint8_t byte6 = cp56_data[6];

	datetime->ucSeconds = milliseconds / 1000; // The millisecond field contains second information

	datetime->usMinutes = byte2 & 0x3F; // The lower six digits represent minutes

	datetime->usHour = byte3 & 0x1F; // The last five digits represent hours

	datetime->usDay = byte4 & 0x1F; // The last five digits represent the date

	datetime->usMonth = byte5 & 0x0F; // The lower four digits represent the month

	datetime->usYear = (byte6 & 0x7F) + 2000; // The last seven digits represent the year, plus 2000
}

// The date-time structure is converted to a CP56Time2a byte stream
void RTC_DatetimeToCp56time2a(const RtcTimedate_Struct *datetime, uint8_t *cp56_data)
{
	// Clear the output buffer
	memset(cp56_data, 0, 7);

	// Calculate the millisecond field (seconds * 1000 + milliseconds)
	uint16_t milliseconds = datetime->ucSeconds * 1000;
	cp56_data[0] = milliseconds & 0xFF;		   // Millisecond low byte
	cp56_data[1] = (milliseconds >> 8) & 0xFF; // Millisecond high byte

	// Byte 2: Minute + IV flag(defaulting to 0)
	cp56_data[2] = (0 << 7) | (datetime->usMinutes & 0x3F);

	// Byte 3: Hour + SU flag(defaulting to 0)
	cp56_data[3] = (0 << 7) | (datetime->usHour & 0x1F);

	// Byte 4: Date + Day of the Week(defaulting to 0)
	cp56_data[4] = ((0 & 0x07) << 5) | (datetime->usDay & 0x1F);

	// Byte 5: Month
	cp56_data[5] = datetime->usMonth & 0x0F;

	// Byte 6: Year (last two digits)
	uint8_t year_short = datetime->usYear - 2000;
	cp56_data[6] = year_short & 0x7F;
}
/*EOF*/
