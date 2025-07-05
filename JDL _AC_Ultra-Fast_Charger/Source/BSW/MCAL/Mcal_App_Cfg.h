//****************************************************************************************
//*
//* File Name: Mcal_APP_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#ifndef _MCAL_APP_CFG_H_
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _MCAL_APP_CFG_H_


/*******************************************************************************
|	 Other Header File Inclusion
|******************************************************************************/
#include "stm32f1xx_hal.h"
#include "Mcal_Types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "STD_LogService.h"
#include "STD_CycBuf.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MCAL_CYCBUF_RET_SUCCESS CYCBUF_RET_SUCCESS                       /*对队列操作成功 */
#define MCAL_CYCBUF_RET_NOT_OPEN CYCBUF_RET_NOT_OPEN                     /*没有申请队列句柄 */
#define MCAL_CYCBUF_RET_OPEN_ABORT CYCBUF_RET_OPEN_ABORT                 /*申请队列句柄异常 */
#define MCAL_CYCBUF_RET_BUF_OUT_OF_MAX CYCBUF_RET_BUF_OUT_OF_MAX         /*申请队列句柄个数到达 */
#define MCAL_CYCBUF_RET_ERR_PARAMETER CYCBUF_RET_ERR_PARAMETER           /*传入队列参数出错 */
#define MCAL_CYCBUF_RET_BUF_FULL CYCBUF_RET_BUF_FULL                     /*队列满 */
#define MCAL_CYCBUF_RET_BUF_EMPTY CYCBUF_RET_BUF_EMPTY                   /*队列空 */
#define MCAL_CYCBUF_RET_BUF_NOT_FULL CYCBUF_RET_BUF_NOT_FULL             /*队列非满 */
#define MCAL_CYCBUF_RET_BUF_NOT_EMPTY CYCBUF_RET_BUF_NOT_EMPTY           /*队列非空 */
#define MCAL_CYCBUF_RET_BUF_NO_ENOUGH_BUF CYCBUF_RET_BUF_NO_ENOUGH_BUF   /*队列缓存区不足 */
#define MCAL_CYCBUF_RET_BUF_NO_ENOUGH_DATA CYCBUF_RET_BUF_NO_ENOUGH_DATA /*队列缓存区没有足够的数据 */
#define MCAL_CYCBUF_RET_ERR_UNEXPECTED CYCBUF_RET_ERR_UNEXPECTED         /*其他错误 */

#define MCAL_CYCBUF_OPEN_CHAN(lv_ucChan, lv_pucDataAdd, lv_ulSize) CycBuf_OpenChan(lv_ucChan, lv_pucDataAdd, lv_ulSize)
#define MCAL_CYCBUF_CHECK_DATA(lv_ucChan, lv_pulLenBuf) CycBuf_CheckData(lv_ucChan, lv_pulLenBuf)
#define MCAL_CYCBUF_WRITE(lv_ucChan, lv_pucSrcData, lv_ulSize) CycBuf_WriteChan(lv_ucChan, lv_pucSrcData, lv_ulSize)
#define MCAL_CYCBUF_READ(lv_ucChan, lv_pucDesData, lv_ulSize) CycBuf_ReadChan(lv_ucChan, lv_pucDesData, lv_ulSize)
#define MCAL_CYCBUF_PREVIEW_READ(lv_ucChan, lv_pucReadBuf, lv_ulSize) CycBuf_PreviewReadChan(lv_ucChan, lv_pucReadBuf, lv_ulSize)
#define MCAL_CYCBUF_REMOVE(lv_ucChan, lv_ulRemoveSize) CycBuf_Remove(lv_ucChan, lv_ulRemoveSize)
#define MCAL_CYCBUF_IS_EMPTY(lv_ucChan) CycBuf_IsEmpty(lv_ucChan)
#define MCAL_CYCBUF_IS_FULL(lv_ucChan) CycBuf_IsFull(lv_ucChan)
#define MCAL_CYCBUF_CLOSE_CHAN(lv_ucChan) CycBuf_CloseChan(lv_ucChan)
#define MCAL_CYCBUF_RESET(lv_ucChan) CycBuf_Reset(lv_ucChan)

#define MCAL_DISABLE	(0u)
#define MCAL_ENABLE		(1u)

#define MCAL_DEBUG(fmt, ...) LOG_DEBUG(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__) /* log output */
#define MCAL_INFO(fmt, ...)  LOG_INFO(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__)  /* log output */
#define MCAL_WARN(fmt, ...)  LOG_WARN(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__)  /* log output */
#define MCAL_ERROR(fmt, ...) LOG_ERROR(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__) /* log output */
#define MCAL_CRITICAL(fmt, ...) LOG_CRITICAL(LOG_MODULE_MCAL, fmt, ##__VA_ARGS__) /* log output */
#define MCAL_PRINT_HEX(BUFF, LEN, R) LogService_Print_Hex_Array(LOG_MODULE_MCAL, BUFF, LEN, R)  /* print hex array */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|	 Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|	 Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|	 Global Function Prototypes
|******************************************************************************/
extern void Error_Handler(void);







#endif /* _MCAL_APP_CFG_H_ */

