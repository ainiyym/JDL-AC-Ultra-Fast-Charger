//****************************************************************************************
//*
//* File Name: STD_Lib.c
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
#include "STD_Interface.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define LIB_MAXVALUE_32     ((uint32_t)0xFFFFFFFFu)
#define LIB_MAXVALUE_16     ((uint16_t)0xFFFFu)
#define LIB_MAXVALUE_08     ((uint8_t)0xFFu)
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
Name            : LIB_SetMemory
Syntax          : void LIB_SetMemory(uint8_t *pStr, uint8_t val, uint16_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : val                   :set value
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : set value to pStr
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_SETMEMORY)
void LIB_SetMemory(uint8_t *pStr, uint8_t val, uint16_t len)
{
    LIB_ENTER_EXCLUSIVE_AREA_0(); /*PRQA S 3138*/

    if ((len > (uint16_t)0u) && (len < LIB_MAXVALUE_16))
    {
        while (len != (uint16_t)0u)
        {
            *pStr = val;
            /* polyspace<MISRA-C3:17.8:Not a defect:Justified> Normal. */
            pStr++; /*PRQA S 0489*/
                    /* polyspace<MISRA-C3:17.8:Not a defect:Justified> Normal. */
            len--;
        }
    }

    LIB_EXIT_EXCLUSIVE_AREA_0(); /*PRQA S 3138*/
}
#endif

/*******************************************************************************
Name            : LIB_Compare
Syntax          : uint8_t LIB_Compare(uint8_t *pSrc, uint8_t *pDes, uint16_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Compare data from pDes to pSrc
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_COMPARE)
uint8_t LIB_Compare(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len)
{
    uint8_t e_u_result = (uint8_t)TRUE;
    uint16_t e_w_len;

    if ((len > (uint16_t)0u) && (len < LIB_MAXVALUE_16))
    {
        for (e_w_len = (uint16_t)0u; e_w_len < len; e_w_len++)
        {
            if (pSrc[e_w_len] != pDes[e_w_len]) /*PRQA S 0492*/
            {
                e_u_result = (uint8_t)FALSE;
                break;
            }
        }
    }

    return (e_u_result);
}
#endif

/*******************************************************************************
Name            : LIB_CompareR
Syntax          : uint8_t LIB_CompareR(uint8_t *pSrc, uint8_t *pDes, uint16_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                  :-
Return value    : None
Description     : Compare reverse data from pDes to pSrc
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_COMPARER)
uint8_t LIB_CompareR(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len)
{
    uint8_t  e_u_result = TRUE;
    uint16_t e_w_len;
    uint8_t  e_u_Rvalue = (uint8_t)0u;

    if( ( len > (uint16_t)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_len = (uint16_t)0u; e_w_len < len; e_w_len++ )
        {
            e_u_Rvalue = (uint8_t)( ~pDes[e_w_len] );/*PRQA S 0492*/

            if( pSrc[e_w_len] != e_u_Rvalue )/*PRQA S 0492*/
            {
                e_u_result = FALSE;
                break;
            }
        }
    }

    return e_u_result;
}
#endif

/*******************************************************************************
Name            : LIB_Copy
Syntax          : void LIB_Copy(uint8_t *pDes, uint8_t *pSrc, uint16_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrc to pDes
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_COPY)
void LIB_Copy(uint8_t *pDes, const uint8_t *pSrc, uint16_t len)
{
    uint16_t e_w_len;
    LIB_ENTER_EXCLUSIVE_AREA_0();/*PRQA S 3138*/

    if( ( len > (uint16_t)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_len = (uint16_t)0u; e_w_len < len; e_w_len++ )
        {
            pDes[e_w_len] = pSrc[e_w_len];/*PRQA S 0492*/
        }
    }

    LIB_EXIT_EXCLUSIVE_AREA_0();/*PRQA S 3138*/
}
#endif

/*******************************************************************************
Name            : LIB_CopyR
Syntax          : void LIB_CopyR(uint8_t * pDes, uint8_t * pSrc, uint16_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrc                  :source data pointer
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy reverse data from pSrc to pDes
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_COPYR)
void LIB_CopyR(uint8_t *pDes, const uint8_t *pSrc, uint16_t len)
{/* polyspace<MISRA-C3:D4.5:Not a defect:Justified> Normal */
    uint16_t e_w_Len;
    LIB_ENTER_EXCLUSIVE_AREA_0();/*PRQA S 3138*/

    if( ( len > (uint16_t)0u ) && ( len < LIB_MAXVALUE_16 ) )
    {
        for( e_w_Len = (uint16_t)0u; e_w_Len < len; e_w_Len++ )
        {
            pDes[e_w_Len] = (uint8_t)( ~pSrc[e_w_Len] );/*PRQA S 0492*/
        }
    }

    LIB_EXIT_EXCLUSIVE_AREA_0();/*PRQA S 3138*/
}
#endif

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
#if (STD_LIB_ENABLE == STD_LIBCFG_CALCULATEAVERAGE)
uint16_t LIB_CalculateAverage(const uint16_t *pData, uint8_t length)
{
    uint8_t  i;
    uint32_t e_dw_sum = (uint32_t)0u;
    uint16_t e_w_average = (uint16_t)0u;

    if( ( length > (uint8_t)0u ) && ( length < LIB_MAXVALUE_08 ) )
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
Name            : LIB_ReadData
Syntax          : void LIB_ReadData(uint8_t * pDes, uint32_t pSrcAdd, uint32_t len)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrcAdd               :source data address
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrcAdd to pDes
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_READDATA)
void LIB_ReadData(uint8_t *pDes, uint32_t pSrcAdd, uint32_t len)
{
    uint32_t e_dw_Len;
    /* polyspace<MISRA-C3:11.4:Not a defect:Justified> Normal */
    uint8_t *const pSrc = (uint8_t *)pSrcAdd; /*PRQA S 0306*/
    LIB_ENTER_EXCLUSIVE_AREA_0();         /*PRQA S 3138*/

    if ((len > (uint32_t)0u) && (len < LIB_MAXVALUE_32))
    {
        for (e_dw_Len = (uint32_t)0u; e_dw_Len < len; e_dw_Len++)
        {
            pDes[e_dw_Len] = pSrc[e_dw_Len]; /*PRQA S 0491,0492*/
        }
    }

    LIB_EXIT_EXCLUSIVE_AREA_0(); /*PRQA S 3138*/
}
#endif
/*******************************************************************************
Name            : LIB_WriteData
Syntax          : void LIB_WriteData( uint8_t *pSrc, uint32_t pDesAdd, uint32_t len )
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : pDes                  :destination data pointer
Parameters(in)  : pSrcAdd               :source data address
Parameters(in)  : len                   :data length
Parameters(out) : None                      :-
Return value    : None
Description     : Copy data from pSrc to pSrcAdd
Call By         : -
|******************************************************************************/
#if (STD_LIB_ENABLE == STD_LIBCFG_WRITEDATA)
/* polyspace<MISRA-C3:8.13:Not a defect:Justified> NO fluence */
/* polyspace<MISRA-C3:8.3:Not a defect:Justified> NO fluence */
void LIB_WriteData(uint8_t *pSrc, uint32_t pDesAdd, uint32_t len)
{
    uint32_t e_dw_Len;
    uint8_t *const pDes = (uint8_t *)pDesAdd; /*PRQA S 0306*/
    LIB_ENTER_EXCLUSIVE_AREA_0();         /*PRQA S 3138*/

    if ((len > (uint32_t)0u) && (len < LIB_MAXVALUE_32))
    {
        for (e_dw_Len = (uint32_t)0u; e_dw_Len < len; e_dw_Len++)
        {
            pDes[e_dw_Len] = pSrc[e_dw_Len]; /*PRQA S 0491,0492*/
        }
    }

    LIB_EXIT_EXCLUSIVE_AREA_0(); /*PRQA S 3138*/
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
#if (STD_LIB_ENABLE == STD_LIBCFG_STATUSFILTER)
uint8_t LIB_StatusFilter(LibFilterStruct *pFilterStatus, uint16_t filterMaxCnt)
{
    uint8_t e_u_reValue = FALSE;
    uint16_t e_w_filterMaxCnt = (uint16_t)0u;

    if ((filterMaxCnt > (uint16_t)0u) && (filterMaxCnt < LIB_MAXVALUE_16))
    {
        e_w_filterMaxCnt = filterMaxCnt;
    }
    else
    {
        e_w_filterMaxCnt = LIB_MAXVALUE_16;
    }

    if (pFilterStatus->ucStatus != pFilterStatus->ucLastStatus)
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16_t)0u;
    }

    if (pFilterStatus->ucStatus != pFilterStatus->ucValidStatus)
    {
        if (pFilterStatus->usFilterCount < e_w_filterMaxCnt)
        {
            pFilterStatus->usFilterCount++;
        }
        else
        {
            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
            e_u_reValue = TRUE;
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
#if (STD_LIB_ENABLE == STD_LIBCFG_STATUSFILTERTYPE2)
uint8_t LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt)
{
    uint8_t  e_u_reValue = FALSE;
    uint16_t e_w_filterInMaxCnt = (uint16_t)0u;
    uint16_t e_w_filterOutMaxCnt = (uint16_t)0u;
    if(( filterInMaxCnt > (uint16_t)0u ) && ( filterInMaxCnt < ((uint16_t)0xFFFFu) ))
    {
    	e_w_filterInMaxCnt = filterInMaxCnt;
    }
    else
    {
    	e_w_filterInMaxCnt = ((uint16_t)0xFFFFu);
    }
    if(( filterOutMaxCnt > (uint16_t)0u ) && ( filterOutMaxCnt < ((uint16_t)0xFFFFu) ))
    {
    	e_w_filterOutMaxCnt = filterOutMaxCnt;
    }
    else
    {
    	e_w_filterOutMaxCnt = ((uint16_t)0xFFFFu);
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucLastStatus )
    {
        pFilterStatus->ucLastStatus = pFilterStatus->ucStatus;
        pFilterStatus->usFilterCount = (uint16_t)0u;
    }

    if( pFilterStatus->ucStatus != pFilterStatus->ucValidStatus )
    {
    	if(pFilterStatus->ucStatus == FALSE)
    	{
    	       if( pFilterStatus->usFilterCount < e_w_filterOutMaxCnt )
    	        {
    	            pFilterStatus->usFilterCount++;
    	        }
    	        else
    	        {
    	            pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
    	            e_u_reValue = TRUE;
    	        }
    	}
    	else if(pFilterStatus->ucStatus == TRUE)
    	{
        	if( pFilterStatus->usFilterCount < e_w_filterInMaxCnt )
            {
                pFilterStatus->usFilterCount++;
            }
            else
            {
                pFilterStatus->ucValidStatus = pFilterStatus->ucStatus;
                e_u_reValue = TRUE;
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
