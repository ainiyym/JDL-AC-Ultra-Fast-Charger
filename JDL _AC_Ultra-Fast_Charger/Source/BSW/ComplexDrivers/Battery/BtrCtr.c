//****************************************************************************************
//*
//* File Name: BtrCtr.c 
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Fifo.h"
#include "BtrCtr.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	BTRCTR_IDLE,            /*Module main function Idle working mode*/
	BTRCTR_NORMAL,		    /*Module main function NOMARL working mode*/
} BtrCtr_MainMode_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t ucEnStatus;                          /*Module enabled state*/
    BtrCtr_MainMode_Enum enMainStatus;           /* Module main function working mode*/
    uint16_t us12vAdData[BTRV_ADC_FIFO_MAX_NUM]; /*Btr cache ad value*/
    uint16_t us5vAdData[BTRV_ADC_FIFO_MAX_NUM];  /*Btr cache 5Vad value*/
    uint16_t us12vValidAdValue;                  /*Btr vaild ad value*/
    uint16_t us5vValidAdValue;                   /*Btr 5V vaild ad value*/
    LibFilterStruct st12VStatus;                 /*12V status filter*/
    LibFilterStruct st5VStatus;                  /*5V status filter*/
} BtrCtr_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static BtrCtr_Struct gv_stBtrCtr;
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void BTRCTR_12vCheck(void);
static void BTRCTR_5vCheck(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/

/*******************************************************************************
Name              : BTRCTR_InitMemory
Syntax            : void BTRCTR_InitMemory(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Initialize the variable
Call By           : BTRM_InitMemory
|******************************************************************************/
void BTRCTR_InitMemory(void)
{
    LIB_SetMemory((uint8_t *)(&gv_stBtrCtr), 0u, (uint16_t)(sizeof(gv_stBtrCtr) / sizeof(uint8_t))); /*PRQA S 0310*/
    gv_stBtrCtr.st12VStatus.ucValidStatus = BTRCTR_FILTER_INIT;
    gv_stBtrCtr.st5VStatus.ucValidStatus = BTRCTR_FILTER_INIT;
}

/*******************************************************************************
Name            : BTRCTR_Open
Syntax          : void BTRCTR_Open(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Enable BTRCTR Module
Call By         : EcuM_App Module
|******************************************************************************/
void BTRCTR_Open(void)
{
	gv_stBtrCtr.ucEnStatus = STD_TRUE;
}

/*******************************************************************************
Name            : BTRCTR_Close
Syntax          : void BTRCTR_Close(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : None                      :-
Parameters(out) : None                      :-
Return value    : None                      :-
Description     : Disable BTRCTR Module
Call By         : EcuM_App Module
|******************************************************************************/

void BTRCTR_Close(void)
{
	gv_stBtrCtr.ucEnStatus = STD_FALSE;
}

/*******************************************************************************
Name            : BTRV_AdcFillter
Syntax          : static uint8_t BTRV_AdcFillter(const uint16_t *plv_usData, uint16_t *lv_usValidAdValue)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : plv_usData                :-
Parameters(in)  : lv_usValidAdValue         :-
Parameters(out) : None                      :-
Return value    : lv_ucReVal                :-
Description     : Adc Fillter
Call By         :
|******************************************************************************/
static uint8_t BTRV_AdcFillter(const uint16_t *plv_usData, uint16_t *lv_usValidAdValue)
{
    uint8_t lv_ucReVal = STD_FALSE;
    uint16_t lv_usMin = 0, lv_usMax = 0, lv_usIdx = 0;
    static uint16_t lv_usValidIdx = 0;
    uint32_t lv_ulSum = 0;

    if (plv_usData != STD_NULL)
    {
        lv_usMin = plv_usData[0u];
        lv_usMax = plv_usData[0u];
        for (lv_usIdx = 0; lv_usIdx < BTRV_ADC_FIFO_MAX_NUM; lv_usIdx++)
        {
            if (lv_usMin == 0)
            {
                lv_usMin = plv_usData[lv_usIdx];
            }
            else if (lv_usMin > plv_usData[lv_usIdx])
            {
                lv_usMin = plv_usData[lv_usIdx];
            }
            else
            {
            }

            if (lv_usMax < plv_usData[lv_usIdx])
            {
                lv_usMax = plv_usData[lv_usIdx];
            }
            else
            {
            }
            lv_ulSum += plv_usData[lv_usIdx];
            lv_usValidIdx++;
        }

        if (lv_usValidIdx > 2u)
        {
            lv_ulSum -= lv_usMin;
            lv_ulSum -= lv_usMax;
            *lv_usValidAdValue = (uint16_t)(lv_ulSum / (lv_usValidIdx - 2u));
            lv_ucReVal = STD_TRUE;
            lv_usValidIdx = 0u;
        }
        else
        {
        }
    }
    else
    {
    }
    return lv_ucReVal;
}

/*******************************************************************************
Name              : BTRCTR_12vCheck
Syntax            : static void BTRCTR_12vCheck(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Check 12V Voltage
Call By           : EcuM_App Module
History
<No.>    <author>    <time>    <description>

|******************************************************************************/
static void BTRCTR_12vCheck(void)
{
	uint32_t lv_ui12vVol = 0u;

	if(STD_TRUE == gv_stBtrCtr.ucEnStatus)
	{
		if(STD_TRUE == FIFO_GetData(FIFO_CHAN_IN_12V_VOLT, gv_stBtrCtr.us12vAdData))
		{
            // BTRCTR_DEBUG("gv_stBtrCtr.us12vAdData:%d\n\r",gv_stBtrCtr.us12vAdData[0]);
			if(STD_TRUE == BTRV_AdcFillter(gv_stBtrCtr.us12vAdData, &gv_stBtrCtr.us12vValidAdValue))
			{
				lv_ui12vVol = (uint32_t)gv_stBtrCtr.us12vValidAdValue * BTRCTR_CALCULATION_12V / BTRCTR_ENLARGE;
				// BTRCTR_DEBUG("us12vValidAdValue:%d lv_ui12vVol:%d\n\r",gv_stBtrCtr.us12vValidAdValue, lv_ui12vVol);
				if((lv_ui12vVol >= BTRCTR_12V_IN_VALUE) && (lv_ui12vVol <= BTRCTR_12V_OUT_VALUE))
				{
					gv_stBtrCtr.st12VStatus.ucStatus = STD_TRUE;
				}
				else
				{
					gv_stBtrCtr.st12VStatus.ucStatus = STD_FALSE;
				}
				if(STD_TRUE == LIB_StatusFilter(&(gv_stBtrCtr.st12VStatus), BTRCTR_12V_WAIT_NUM))
				{
	                if(STD_FALSE == gv_stBtrCtr.st12VStatus.ucStatus)
					{
						BTRCTR_DEBUG("12V Power Fault!!\n\r");
						BTRCTR_VOL12V_ERRORCALLBACK(STD_TRUE);
					}
				}
			}
		}
	}
}

/*******************************************************************************
Name              : BTRCTR_5vCheck
Syntax            : static void BTRCTR_5vCheck(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Check 5V Voltage
Call By           : EcuM_App Module
History
<No.>    <author>    <time>    <description>

|******************************************************************************/
static void BTRCTR_5vCheck(void)
{
    uint32_t lv_ui5vVol = 0u;

    if (STD_TRUE == gv_stBtrCtr.ucEnStatus)
    {
        if (STD_TRUE == FIFO_GetData(FIFO_CHAN_IN_5V_VOLT, gv_stBtrCtr.us5vAdData))
        {
            // BTRCTR_DEBUG("gv_stBtrCtr.us5vAdData:%d\n\r", gv_stBtrCtr.us5vAdData[0]);
            if (STD_TRUE == BTRV_AdcFillter(gv_stBtrCtr.us5vAdData, &gv_stBtrCtr.us5vValidAdValue))
            {
                lv_ui5vVol = (uint32_t)gv_stBtrCtr.us5vValidAdValue * BTRCTR_CALCULATION_5V / BTRCTR_ENLARGE;
                // BTRCTR_DEBUG("us5vValidAdValue:%d lv_ui5vVol:%d\n\r",gv_stBtrCtr.us5vValidAdValue, lv_ui5vVol);
                if ((lv_ui5vVol >= BTRCTR_5V_IN_VALUE) && (lv_ui5vVol <= BTRCTR_5V_OUT_VALUE))
                {
                    gv_stBtrCtr.st5VStatus.ucStatus = STD_TRUE;
                }
                else
                {
                    gv_stBtrCtr.st5VStatus.ucStatus = STD_FALSE;
                }
                if (STD_TRUE == LIB_StatusFilter(&(gv_stBtrCtr.st5VStatus), BTRCTR_5V_WAIT_NUM))
                {
                    if (STD_FALSE == gv_stBtrCtr.st5VStatus.ucStatus)
                    {
                        BTRCTR_DEBUG("5V Power Fault!!\n\r");
                        BTRCTR_VOL5V_ERRORCALLBACK(STD_TRUE);
                    }
                }
            }
        }
    }
}

/*******************************************************************************
Name              : BTRCTR_MainFunction
Syntax            : void BTRCTR_MainFunction(void)
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      : None
Description       : Control Function
Call By           : EcuM_App Module
|******************************************************************************/

void BTRCTR_MainFunction(void)
{
    /* Check 12V Voltage */
    BTRCTR_12vCheck();

    /* Check 5V Voltage */
    BTRCTR_5vCheck();
}

/*******************************************************************************
Name              : BTRCTR_10msMainFunction
Syntax            : void BTRCTR_10msMainFunction( void )
Sync/Async        : Synchronous
Reentrancy        : None
Parameters(in)    : None
Parameters(out)   : None
Return value      :
Description       : MainFunction
Call By           : Task
|******************************************************************************/
void BTRCTR_10msMainFunction(void)
{
    switch (gv_stBtrCtr.enMainStatus)
    {
        case BTRCTR_IDLE:
        {
            if (STD_TRUE == SYSM_GetResetPrepareStatus() && (gv_stBtrCtr.ucEnStatus == TRUE))
            {
                gv_stBtrCtr.enMainStatus = BTRCTR_NORMAL;
            }
            break;
        }

        case BTRCTR_NORMAL:
        {
            if (gv_stBtrCtr.ucEnStatus == FALSE)
            {
                gv_stBtrCtr.enMainStatus = BTRCTR_IDLE;
                BTRCTR_InitMemory();
            }
            else
            {
                BTRCTR_MainFunction();
            }
            break;
        }

        default:
        {
            gv_stBtrCtr.enMainStatus = BTRCTR_IDLE;
            break;
        }
    }
}

/*EOF*/
