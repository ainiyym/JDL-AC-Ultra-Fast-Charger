//****************************************************************************************
//*
//* File Name: STD_Lib.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
/* polyspace:begin<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#if !defined (_STD_LIB_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_LIB_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "STD_Lib_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define LIB_SET_UINT32_BIT(Data, Offset)    ((Data) |= (uint32_t)((uint32_t)1u << (Offset)))
#define LIB_SET_UINT16_BIT(Data, Offset)    ((Data) |= (uint16_t)((uint16_t)1u << (Offset)))
#define LIB_SET_UINT8_BIT(Data, Offset)     ((Data) |= (uint8_t)((uint8_t) 1u << (Offset)))

#define LIB_RESET_UINT32_BIT(Data, Offset)  ((Data) &= (uint32_t)(~(uint32_t)((uint32_t)1u << (Offset))))
#define LIB_RESET_UINT16_BIT(Data, Offset)  ((Data) &= (uint16_t)(~(uint16_t)((uint16_t)1u << (Offset))))
#define LIB_RESET_UINT8_BIT(Data, Offset)   ((Data) &= (uint8_t )(~(uint8_t )((uint8_t )1u << (Offset))))

#define LIB_GET_UINT32_BIT(Data, Offset)    (((uint8_t)(((uint32_t) (Data)) >> (Offset))) & (uint8_t)0x01u)
#define LIB_GET_UINT16_BIT(Data, Offset)    (((uint8_t)(((uint16_t) (Data)) >> (Offset))) & (uint8_t)0x01u)
#define LIB_GET_UINT8_BIT(Data, Offset)     (((uint8_t)(((uint8_t ) (Data)) >> (Offset))) & (uint8_t)0x01u)

#define LIB_GET_BYTE_0(Data)                ((uint8_t) (Data))
#define LIB_GET_BYTE_1(Data)                ((uint8_t)((Data)>>(uint8_t)8u))
#define LIB_GET_BYTE_2(Data)                ((uint8_t)((Data)>>(uint8_t)16u))
#define LIB_GET_BYTE_3(Data)                ((uint8_t)((Data)>>(uint8_t)24u))

#define LIB_GET_BYTE_LIT_UINT16(Data)		((uint16_t)Data[0u] + ((uint16_t)Data[1u] << 8u))
#define LIB_GET_BYTE_BIG_UINT16(Data)		((uint16_t)Data[1u] + ((uint16_t)Data[0u] << 8u))

#define LIB_BIGLITTLEEND_SWAP_2_BYTES(X)    (X = (((X << 8) & 0xFF00)\
                                             | ((X >> 8) & 0x00FF) ) )

#define LIB_BIGLITTLEEND_SWAP_4_BYTES(X)    (X = (((X << 24) & 0xFF000000)\
								     	     | ((X << 8 ) & 0x00FF0000)\
								     	     | ((X >> 8 ) & 0x0000FF00)\
								      	     | ((X >> 24) & 0x000000FF)))

#define LIB_BIGLITTLEEND_SWAP_8_BYTES(X)    (X = (((X << 56) & 0xFF00000000000000)\
									         | ((X << 40) & 0x00FF000000000000)\
                                             | ((X << 24) & 0x0000FF0000000000)\
                                             | ((X << 8 ) & 0x000000FF00000000)\
                                             | ((X >> 8 ) & 0x00000000FF000000)\
                                             | ((X >> 24) & 0x0000000000FF0000)\
									         | ((X >> 40) & 0x000000000000FF00)\
                                             | ((X >> 56) & 0x00000000000000FF)))

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.4:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:5.7:Not a defect:Justified> NO fluence */
typedef struct Type_FilterStruct
{
    uint8_t ucStatus;
    uint8_t ucLastStatus;
    uint8_t ucValidStatus;
    uint16_t usFilterCount;
/* polyspace<MISRA-C3:5.6:Not a defect:Justified> NO fluence */
} LibFilterStruct;
/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#if (STD_LIB_ENABLE == STD_LIBCFG_SETMEMORY)
extern void LIB_SetMemory(uint8_t *pStr, uint8_t val, uint16_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_COMPARE)
extern uint8_t LIB_Compare(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_COMPARER)
extern uint8_t LIB_CompareR(const uint8_t *pSrc, const uint8_t *pDes, uint16_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_COPY)
extern void LIB_Copy(uint8_t *pDes, const uint8_t *pSrc, uint16_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_COPYR)
extern void LIB_CopyR(uint8_t *pDes, const uint8_t *pSrc, uint16_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_CALCULATEAVERAGE)
extern uint16_t LIB_CalculateAverage(const uint16_t *pData, uint8_t length);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_READDATA)
extern void LIB_ReadData(uint8_t *pDes, uint32_t pSrcAdd, uint32_t len);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_WRITEDATA)
extern void LIB_WriteData( uint8_t *pSrc, uint32_t pSrcAdd, uint32_t len );
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_STATUSFILTER)
extern uint8_t LIB_StatusFilter(LibFilterStruct *pFilterStatus, uint16_t filterMaxCnt);
#endif

#if (STD_LIB_ENABLE == STD_LIBCFG_STATUSFILTERTYPE2)
uint8_t LIB_StatusFilterType2( LibFilterStruct *pFilterStatus, uint16_t filterInMaxCnt ,uint16_t filterOutMaxCnt);
#endif

#endif
/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace:end<MISRA-C3:D4.9:Not a defect:Justified> Normal */
/*EOF*/
