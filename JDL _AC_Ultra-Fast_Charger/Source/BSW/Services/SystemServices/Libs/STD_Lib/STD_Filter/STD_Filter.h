//****************************************************************************************
//*
//* File Name: STD_Filter.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
/* polyspace:begin<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#if !defined (_STD_FILTER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_FILTER_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Filter_Cfg.h"

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
/* polyspace<MISRA-C3:2.4:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:5.7:Not a defect:Justified> NO fluence */
typedef struct 
{
    uint8_t ucStatus;
    uint8_t ucLastStatus;
    uint8_t ucValidStatus;
    uint16_t usFilterCount;
/* polyspace<MISRA-C3:5.6:Not a defect:Justified> NO fluence */
} Filter_Struct;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
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
Description     : Calculate Average
Call By         : -
|******************************************************************************/
#if (FILT_ENABLE == FILT_CFG_CALCULATE_AVERAGE)
extern uint16_t LIB_CalculateAverage(const uint16_t *pData, uint8_t length);
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
extern uint8_t FILTER_StatusFiltT1(Filter_Struct *pFilterStatus, uint16_t filterMaxCnt);
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
extern uint8_t FILTER_StatusFiltT2( Filter_Struct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt);
#endif

#endif
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace:end<MISRA-C3:D4.9:Not a defect:Justified> Normal */
/*EOF*/
