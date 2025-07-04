//****************************************************************************************
//*
//* File Name: STD_CycBuf.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (_STD_CYCBUF_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_CYCBUF_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "STD_CycBuf_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

typedef enum
{
	CYCBUF_RET_SUCCESS = 0,		   /*对队列操作成功 */
	CYCBUF_RET_NOT_OPEN,		   /*没有申请队列句柄 */
	CYCBUF_RET_OPEN_ABORT,		   /*申请队列句柄异常 */
	CYCBUF_RET_BUF_OUT_OF_MAX,	   /*申请队列句柄个数到达 */
	CYCBUF_RET_ERR_PARAMETER,	   /*传入队列参数出错 */
	CYCBUF_RET_BUF_FULL,		   /*队列满 */
	CYCBUF_RET_BUF_EMPTY,		   /*队列空 */
	CYCBUF_RET_BUF_NOT_FULL,	   /*队列非满 */
	CYCBUF_RET_BUF_NOT_EMPTY,	   /*队列非空 */
	CYCBUF_RET_BUF_NO_ENOUGH_BUF,  /*队列缓存区不足 */
	CYCBUF_RET_BUF_NO_ENOUGH_DATA, /*队列缓存区没有足够的数据 */
	CYCBUF_RET_ERR_UNEXPECTED	   /*其他错误 */
} CycBuf_Ret_Value_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void CycBuf_InitMemory(void);
extern void CycBuf_Init(void);

extern uint8_t CycBuf_OpenChan(uint8_t* lv_ucChan, uint8_t* lv_pucDataAdd, uint32_t lv_ulSize);
extern uint8_t CycBuf_CloseChan(uint8_t lv_ucChan);

extern uint8_t CycBuf_WriteChan(uint8_t lv_ucChan, uint8_t * lv_pucSrcData, uint32_t lv_ulSize);
extern uint8_t CycBuf_PreviewReadChan(uint8_t lv_ucChan, uint8_t* lv_pucReadBuf, uint32_t lv_ulSize);
extern uint8_t CycBuf_ReadChan(uint8_t lv_ucChan, uint8_t * lv_pucDesData, uint32_t lv_ulSize);

extern uint8_t CycBuf_Remove(uint8_t lv_ucChan, uint32_t lv_ulRemoveSize);
extern uint8_t CycBuf_IsEmpty(uint8_t lv_ucChan);
extern uint8_t CycBuf_IsFull(uint8_t lv_ucChan);
extern uint8_t CycBuf_CheckData(uint8_t lv_ucChan, uint32_t* lv_pulLenBuf);
extern uint8_t CycBuf_Reset(uint8_t lv_ucChan);

#endif
/*EOF*/
