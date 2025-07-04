//****************************************************************************************
//*
//* File Name: STD_Filter.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Lib.h"
#include "STD_Filter.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FILTER_MAXVALUE_32     ((uint32_t)0xFFFFFFFFu)
#define FILTER_MAXVALUE_16     ((uint16_t)0xFFFFu)
#define FILTER_MAXVALUE_08     ((uint8_t)0xFFu)
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


/*******************************************************************************
Name            : LIB_CalculateAverage
Syntax          : uint16_t LIB_CalculateAverage(const uint16_t *pData, uint8_t length)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pData                 :data pointer
Parameters(in)  : length                :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Use bubble sort method
Call By         : -
|******************************************************************************/
#if (FILT_ENABLE == FILT_CFG_CALCULATE_AVERAGE)
uint16_t FILTER_CalculateAverage(const uint16_t *pData, uint8_t length)
{
    uint8_t  i;
    uint32_t e_dw_sum = (uint32_t)0u;
    uint16_t e_w_average = (uint16_t)0u;

    if( ( length > (uint8_t)0u ) && ( length < FILTER_MAXVALUE_08 ) )
    {
        for( i = (uint8_t)0u; i < length; i++ )
        {
            e_dw_sum += pData[i];/*PRQA S 0492*/
        }

        e_w_average = (uint16_t)( e_dw_sum / length );
    }
    else
    {
    }

    return (e_w_average);
}
#endif

/*******************************************************************************
Name            : LIB_StatusFilter
Syntax          : uint16_t LIB_StatusFilter(LibFilterStruct *pFilterStatus, uint16_t filterMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterMaxCnt              :filter count
Parameters(out) : None                      :-
Return value    : None
Description     : data Filter
Call By         : -
|******************************************************************************/
#if (FILT_ENABLE == FILT_CFG_STATUS_FILT_T1)
uint8_t FILTER_StatusFiltT1( Filter_Struct *pFilterStatus, uint16_t filterMaxCnt )
{
    uint8_t  e_u_reValue = STD_FALSE;
    uint16_t e_w_filterMaxCnt = (uint16_t)0u;

    if(( filterMaxCnt > (uint16_t)0u ) && ( filterMaxCnt < FILTER_MAXVALUE_16 ))
    {
        e_w_filterMaxCnt = filterMaxCnt;
    }
    else
    {
        e_w_filterMaxCnt = FILTER_MAXVALUE_16;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucLastStatus )
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16_t)0u;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucValidStatus )
    {
        if( pFilterStatus->usFilterCount < e_w_filterMaxCnt )
        {
            pFilterStatus->usFilterCount++;
        }
        else
        {
            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
            e_u_reValue = STD_TRUE;
        }
    }
    else
    {
    }

    return (e_u_reValue);
}
#endif
/*******************************************************************************
Name            : LIB_StatusFilterType2
Syntax          : LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : *pFilterStatus            :data pointer
Parameters(in)  : filterInMaxCnt              :filter count
Parameters(in)  : filterOutMaxCnt           :filter count
Parameters(out) : None
Return value    : None
Description     : data Filter
Call By         : -
|******************************************************************************/
#if (FILT_ENABLE == FILT_CFG_STATUS_FILT_T2)
uint8_t FILTER_StatusFiltT2( Filter_Struct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt)
{
    uint8_t  e_u_reValue = STD_FALSE;
    uint16_t e_w_filterInMaxCnt = (uint16_t)0u;
    uint16_t e_w_filterOutMaxCnt = (uint16_t)0u;
    if(( filterInMaxCnt > (uint16_t)0u ) && ( filterInMaxCnt < FILTER_MAXVALUE_16))
    {
    	e_w_filterInMaxCnt = filterInMaxCnt;
    }
    else
    {
    	e_w_filterInMaxCnt = FILTER_MAXVALUE_16;
    }
    if(( filterOutMaxCnt > (uint16_t)0u ) && ( filterOutMaxCnt < FILTER_MAXVALUE_16))
    {
    	e_w_filterOutMaxCnt = filterOutMaxCnt;
    }
    else
    {
    	e_w_filterOutMaxCnt = FILTER_MAXVALUE_16;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucLastStatus )
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16_t)0u;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucValidStatus )
    {
    	if(pFilterStatus->ucValidStatus == STD_TRUE)
    	{
    	       if( pFilterStatus->usFilterCount < e_w_filterOutMaxCnt )
    	        {
    	            pFilterStatus->usFilterCount++;
    	        }
    	        else
    	        {
    	            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
    	            e_u_reValue = STD_TRUE;
    	        }
    	}
    	else if(pFilterStatus->ucValidStatus == STD_FALSE)
    	{
        	if( pFilterStatus->usFilterCount < e_w_filterInMaxCnt )
            {
                pFilterStatus->usFilterCount++;
            }
            else
            {
                pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
                e_u_reValue = STD_TRUE;
            }
    	}
    }
    else
    {
    }
    return (e_u_reValue);
}
#endif
/*EOF*/
