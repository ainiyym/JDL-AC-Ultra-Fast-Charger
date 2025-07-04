//****************************************************************************************
//*
//* File Name: STD_CycBuf.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Lib.h"
#include "STD_CycBuf.h"
#include "STD_Interface.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

#define CYCBUF_NULL		(0u)				/* NULL */

#define CYCBUF_STATUS_INIT 		(0u)		/* Initial Status */
#define CYCBUF_STATUS_INUSE 	(1u)		/* Inuse Status */
#define CYCBUF_STATUS_BUSY 		(2u)		/* Busy Status */
#define CYCBUF_STATUS_FREE  	(3u)		/* Free Status */

/*******************************************************************************
|    Enum Definition
|******************************************************************************/


/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

typedef struct
{
	uint32_t ulBuffSize;
	uint32_t ulReadIdx;
	uint32_t ulWriteIdx;
	volatile uint8_t *ucData;
	uint8_t ucStatus;
} CycBuf_Struct; /* the channel setting config */

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

CycBuf_Struct gv_stCycBufArry[CYCBUF_CHAN_MAX_NUM];

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
Name            : CYCBUF_InitMemory
Syntax          : void CYCBUF_InitMemory(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none                               
Parameters(out) : none
Return value    : void
Description     : Cycbuffer init memory function 
Call By         : -
|******************************************************************************/
void CycBuf_InitMemory(void)
{
	LIB_SetMemory( (uint8_t *)(&gv_stCycBufArry), 0u, (uint16_t)(sizeof(gv_stCycBufArry) / sizeof(uint8_t)) );
}


/*******************************************************************************
Name            : CYCBUF_Init
Syntax          : void CYCBUF_Init(void)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : none                               
Parameters(out) : none
Return value    : void
Description     : Cycbuffer init config function 
Call By         : -
|******************************************************************************/
void CycBuf_Init(void)
{
	CYCBUF_ENTER_CRITICAL_AREA();

	for(uint8_t i = 0; i < CYCBUF_CHAN_MAX_NUM; i++)
	{
		gv_stCycBufArry[i].ucStatus 	= CYCBUF_STATUS_FREE;
		gv_stCycBufArry[i].ulBuffSize 	= 0u;
		gv_stCycBufArry[i].ucData 		= CYCBUF_NULL;
		gv_stCycBufArry[i].ulReadIdx 	= 0u;
		gv_stCycBufArry[i].ulWriteIdx 	= 0u;
	}

	CYCBUF_EXIT_CRITICAL_AREA();
}

/*******************************************************************************
Name            : CycBuf_OpenChan
Syntax          : uint8_t CYCBUF_OpenChan(uint8_t lv_ucChan, uint8_t * lv_pucDataAdd, uint32_t lv_usSize)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channal ID;
Parameters(in) 	: lv_pucDataAdd: the channel DATA address
Parameters(in)	: lv_ulSize: the channel buf size
Parameters(out) : none
Return value    : STD_TRCYCBUF_RET_OPEN_ABORTUE: channel open abort
Return value	: CYCBUF_RET_SUCCESS: channel open success					  
Description     : Cycbuffer open channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_OpenChan(uint8_t* lv_ucChan, uint8_t* lv_pucDataAdd, uint32_t lv_ulSize)
{
	uint8_t lv_ucReVal;
	uint8_t i;

	if((0 == lv_ulSize) || (CYCBUF_NULL == lv_pucDataAdd))
	{
		lv_ucReVal = CYCBUF_RET_OPEN_ABORT;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		for(i = 0; i < CYCBUF_CHAN_MAX_NUM; i++)
		{
			if(CYCBUF_STATUS_FREE == gv_stCycBufArry[i].ucStatus)
			{
				break;
			}
			else
			{;}
		}
		if(i >= CYCBUF_CHAN_MAX_NUM)
		{
			lv_ucReVal = CYCBUF_RET_BUF_OUT_OF_MAX;
		}
		else
		{
			gv_stCycBufArry[i].ucData 		= lv_pucDataAdd;
			gv_stCycBufArry[i].ulBuffSize 	= lv_ulSize;
			gv_stCycBufArry[i].ulReadIdx 	= 0;
			gv_stCycBufArry[i].ulWriteIdx 	= 0;
			gv_stCycBufArry[i].ucStatus 	= CYCBUF_STATUS_INUSE;

			*lv_ucChan = i;

			lv_ucReVal = CYCBUF_RET_SUCCESS;
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}

	return lv_ucReVal;
}

/*******************************************************************************
Name            : CycBuf_Close
Syntax          : uint8_t CycBuf_Close(uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID;                                  
Parameters(out) : none
Return value    : CYCBUF_RET_SUCCESS: channel Close success
Return value	: CYCBUF_RET_ERR_PARAMETER: the parameter error,channel close fail
Description     : Cycbuffer close channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_CloseChan(uint8_t lv_ucChan)
{
	uint8_t lv_ucReVal;

	CYCBUF_ENTER_CRITICAL_AREA();

	if (lv_ucChan < CYCBUF_CHAN_MAX_NUM)
	{
		gv_stCycBufArry[lv_ucChan].ucStatus 		= CYCBUF_STATUS_FREE;
		gv_stCycBufArry[lv_ucChan].ulBuffSize   	= 0u;
		gv_stCycBufArry[lv_ucChan].ucData 			= CYCBUF_NULL;
		gv_stCycBufArry[lv_ucChan].ulReadIdx  		= 0u;
		gv_stCycBufArry[lv_ucChan].ulWriteIdx 		= 0u;

		lv_ucReVal = CYCBUF_RET_SUCCESS;
	}
	else
	{
		lv_ucReVal = CYCBUF_RET_ERR_PARAMETER;
	}
	
	CYCBUF_EXIT_CRITICAL_AREA();	

	return lv_ucReVal;
}

/*******************************************************************************
Name            : CYCBUF_WriteChan
Syntax          : uint8_t CYCBUF_WriteChan(uint8_t lv_ucChan, uint8_t * lv_pucSrcData, uint32_t lv_usSize)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID
Parameters(in)  : lv_pucSrcData: the source data pointer
Parameters(in)  : lv_ulSize: the source data size      
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value	: CYCBUF_RET_BUF_FULL: the channel is full
Return value    : CYCBUF_RET_BUF_NO_ENOUGH_BUF: the size of channel if not enouth to write
Return value 	: CYCBUF_RET_SUCCESS: channel write success 
Description     : Cycbuffer Write data into the channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_WriteChan(uint8_t lv_ucChan, uint8_t * lv_pucSrcData, uint32_t lv_ulSize)
{
	uint8_t lv_ucReVal;
	uint32_t lv_ulDataIdx;
	uint32_t lv_ulFreeBuffSize;

	CycBuf_Struct *pstCycBuf = &gv_stCycBufArry[lv_ucChan];

	CYCBUF_ENTER_CRITICAL_AREA();

	/* Paramater check */
	if((0u != lv_ulSize) && (lv_ucChan < CYCBUF_CHAN_MAX_NUM) && (CYCBUF_NULL != lv_pucSrcData))
	{
		if(CYCBUF_STATUS_INUSE == pstCycBuf->ucStatus)
		{
			if(((pstCycBuf->ulWriteIdx + 1) % pstCycBuf->ulBuffSize) != pstCycBuf->ulReadIdx)
			{
				if(pstCycBuf->ulWriteIdx >= pstCycBuf->ulReadIdx)
				{
					lv_ulFreeBuffSize = pstCycBuf->ulBuffSize - (pstCycBuf->ulWriteIdx - pstCycBuf->ulReadIdx);
				}
				else
				{
					lv_ulFreeBuffSize = pstCycBuf->ulReadIdx - pstCycBuf->ulWriteIdx;
				}

				if(lv_ulSize <= lv_ulFreeBuffSize)
				{
					pstCycBuf->ucStatus = CYCBUF_STATUS_BUSY;
					for(lv_ulDataIdx = 0; lv_ulDataIdx < lv_ulSize; lv_ulDataIdx++)
					{
						pstCycBuf->ucData[pstCycBuf->ulWriteIdx] = lv_pucSrcData[lv_ulDataIdx];
						pstCycBuf->ulWriteIdx = (pstCycBuf->ulWriteIdx+ 1)% pstCycBuf->ulBuffSize;
					}
					pstCycBuf->ucStatus = CYCBUF_STATUS_INUSE;
					lv_ucReVal = CYCBUF_RET_SUCCESS;
				}
				else
				{
					lv_ucReVal = CYCBUF_RET_BUF_NO_ENOUGH_BUF;
				}
			}
			else
			{
				lv_ucReVal = CYCBUF_RET_BUF_FULL;
			}
		}
	}
	else
	{
		lv_ucReVal = CYCBUF_RET_ERR_PARAMETER;
	}

	CYCBUF_EXIT_CRITICAL_AREA();

	return lv_ucReVal;
}

/*******************************************************************************
Name            : CycBuf_PreviewReadChan
Syntax          : uint8_t CycBuf_PreviewRead(uint8_t lv_ucChan, uint8_t* lv_pucReadBuf, uint32_t lv_ulReadSize)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID
Parameters(in)	: lv_pucReadBuf: the buffer which is to be preview read
Parameters(in)	: lv_ulSize: the size of the preview buffer                              
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value	: CYCBUF_RET_SUCCESS: preveiw success
Return value 	: CYCBUF_RET_BUF_NO_ENOUGH_DATA: the buf size if not enough for read
Description     : Cycbuffer preview read data from the channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_PreviewReadChan(uint8_t lv_ucChan, uint8_t *lv_pucReadBuf, uint32_t lv_ulSize)
{
	uint8_t lv_ucReval;
	uint32_t lv_ulSizeIdx;
	uint32_t lv_ulBufUsedSize;
	uint32_t lv_ulWriteIdxTemp;
	uint32_t lv_ulReadIdxTemp;

	if ((lv_ucChan >= CYCBUF_CHAN_MAX_NUM) || (CYCBUF_NULL == lv_pucReadBuf) || (0 == lv_ulSize))
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if (CYCBUF_STATUS_INUSE == gv_stCycBufArry[lv_ucChan].ucStatus)
		{
			gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_BUSY;
			lv_ulWriteIdxTemp = gv_stCycBufArry[lv_ucChan].ulWriteIdx;
			lv_ulReadIdxTemp = gv_stCycBufArry[lv_ucChan].ulReadIdx;

			if (lv_ulWriteIdxTemp >= lv_ulReadIdxTemp)
			{
				lv_ulBufUsedSize = lv_ulWriteIdxTemp - lv_ulReadIdxTemp;
			}
			else
			{
				lv_ulBufUsedSize = gv_stCycBufArry[lv_ucChan].ulBuffSize + lv_ulWriteIdxTemp - lv_ulReadIdxTemp;
			}
			if (lv_ulSize > lv_ulBufUsedSize)
			{
				lv_ucReval = CYCBUF_RET_BUF_NO_ENOUGH_DATA;
			}
			else
			{
				// gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_BUSY;

				for (lv_ulSizeIdx = 0; lv_ulSizeIdx < lv_ulSize; lv_ulSizeIdx++)
				{
					lv_pucReadBuf[lv_ulSizeIdx] = gv_stCycBufArry[lv_ucChan].ucData[lv_ulReadIdxTemp];
					lv_ulReadIdxTemp = (lv_ulReadIdxTemp+ 1) % gv_stCycBufArry[lv_ucChan].ulBuffSize;
				}

				// gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_INUSE;
				
				lv_ucReval = CYCBUF_RET_SUCCESS;
			}
			gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_INUSE;
		}
		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}

/*******************************************************************************
Name            : CycBuf_ReadChan
Syntax          : uint8_t CYCBUF_ReadChan(uint8_t lv_ucChan, uint8_t * lv_pucDesData, uint32_t lv_usSize)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID
Parameters(in)  : lv_pucDesData: the target data pointer
Parameters(in)	: lv_ulSize: the target data size                     
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value	: CYCBUF_RET_SUCCESS: channel read success
Description     : Cycbuffer read data from the channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_ReadChan(uint8_t lv_ucChan, uint8_t * lv_pucDesData, uint32_t lv_ulSize)
{
	uint8_t lv_ucReVal;
	uint32_t lv_ulDataIdx;
	uint32_t lv_ulReadBuffSize;
	uint32_t lv_ulWriteIdxTemp;
	uint32_t lv_ulReadIdxTemp;

	CycBuf_Struct *pstCycBuf = &gv_stCycBufArry[lv_ucChan];

	CYCBUF_ENTER_CRITICAL_AREA();

	/* Paramater check */
	if ((lv_ucChan < CYCBUF_CHAN_MAX_NUM) && (0u != lv_ulSize) && (CYCBUF_NULL != lv_pucDesData))
	{
		if (CYCBUF_STATUS_INUSE == pstCycBuf->ucStatus)
		{
			pstCycBuf->ucStatus = CYCBUF_STATUS_BUSY;
			lv_ulWriteIdxTemp = pstCycBuf->ulWriteIdx;
			lv_ulReadIdxTemp = pstCycBuf->ulReadIdx;

			if(((lv_ulReadIdxTemp+ 1) % pstCycBuf->ulBuffSize) != lv_ulWriteIdxTemp)
			{
				if(lv_ulReadIdxTemp >= lv_ulWriteIdxTemp)
				{
					lv_ulReadBuffSize = pstCycBuf->ulBuffSize - (lv_ulReadIdxTemp - lv_ulWriteIdxTemp);
				}
				else
				{
					lv_ulReadBuffSize = lv_ulWriteIdxTemp - lv_ulReadIdxTemp;
				}

				if (lv_ulSize <= lv_ulReadBuffSize)
				{
					// pstCycBuf->ucStatus = CYCBUF_STATUS_BUSY;

					for (lv_ulDataIdx = 0; lv_ulDataIdx < lv_ulReadBuffSize; lv_ulDataIdx++)
					{
						lv_pucDesData[lv_ulDataIdx] = pstCycBuf->ucData[pstCycBuf->ulReadIdx];
						pstCycBuf->ulReadIdx = (pstCycBuf->ulReadIdx + 1) % pstCycBuf->ulBuffSize;
					}

					// pstCycBuf->ucStatus = CYCBUF_STATUS_INUSE;

					lv_ucReVal = CYCBUF_RET_SUCCESS;
				}
			}
			pstCycBuf->ucStatus = CYCBUF_STATUS_INUSE;
		}
	}
	else
	{
		lv_ucReVal = CYCBUF_RET_ERR_PARAMETER;
	}

	CYCBUF_EXIT_CRITICAL_AREA();

	return lv_ucReVal;
}

/*******************************************************************************
Name            : CycBuf_Remove
Syntax          : uint8_t CycBuf_Remove(uint8_t lv_ucChan, uint32_t lv_ulRemoveSize)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID
Parameters(in)  : lv_ulRemoveSize: the size of the data which is wait for removed                 
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value 	: CYCBUF_RET_NOT_OPEN: channel not open
Return value 	: CYCBUF_RET_SUCCESS: the buf remove success
Return value 	: CYCBUF_RET_BUF_NO_ENOUGH_DATA: the length of buf is shorter than the removesize
Description     : Cycbuffer remove data from the channel function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_Remove(uint8_t lv_ucChan, uint32_t lv_ulRemoveSize)
{
	uint8_t lv_ucReval;
	uint32_t lv_ulLen;

	if((lv_ucChan >= CYCBUF_CHAN_MAX_NUM) || 0 == lv_ulRemoveSize)
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if(CYCBUF_STATUS_FREE == gv_stCycBufArry[lv_ucChan].ucStatus)
		{
			lv_ucReval = CYCBUF_RET_NOT_OPEN;
		}
		else
		{
			/*test valid data*/
			if (gv_stCycBufArry[lv_ucChan].ulWriteIdx >= gv_stCycBufArry[lv_ucChan].ulReadIdx)
			{
				lv_ulLen = gv_stCycBufArry[lv_ucChan].ulWriteIdx - gv_stCycBufArry[lv_ucChan].ulReadIdx;
			}
			else
			{
				lv_ulLen = gv_stCycBufArry[lv_ucChan].ulBuffSize + gv_stCycBufArry[lv_ucChan].ulWriteIdx - gv_stCycBufArry[lv_ucChan].ulReadIdx;
			}
			if(lv_ulLen >= lv_ulRemoveSize)
			{
				gv_stCycBufArry[lv_ucChan].ulReadIdx = (gv_stCycBufArry[lv_ucChan].ulReadIdx + lv_ulRemoveSize) % gv_stCycBufArry[lv_ucChan].ulBuffSize;
				lv_ucReval = CYCBUF_RET_SUCCESS;
			}
			else
			{
				lv_ucReval = CYCBUF_RET_BUF_NO_ENOUGH_DATA;
			}
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}


/*******************************************************************************
Name            : CycBuf_IsEmpty
Syntax          : uint8_t CycBuf_IsEmpty(uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID                   
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value	: CYCBUF_RET_NOT_OPEN: the channel not open
Return value 	: CYCBUF_RET_BUF_EMPTY: the channel is empty
Return value 	: CYCBUF_RET_BUF_NOT_EMPTY: the channel is not empty
Description     : Judge the channel is empty or not function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_IsEmpty(uint8_t lv_ucChan)
{
	uint8_t lv_ucReval;

	if(lv_ucChan >= CYCBUF_CHAN_MAX_NUM)
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if(CYCBUF_STATUS_INUSE != gv_stCycBufArry[lv_ucChan].ucStatus)
		{
			lv_ucReval = CYCBUF_RET_NOT_OPEN;
		}
		else
		{
			if(gv_stCycBufArry[lv_ucChan].ulWriteIdx == gv_stCycBufArry[lv_ucChan].ulReadIdx)
			{
				lv_ucReval = CYCBUF_RET_BUF_EMPTY;
			}
			else
			{
				lv_ucReval = CYCBUF_RET_BUF_NOT_EMPTY;
			}
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}

/*******************************************************************************
Name            : CycBuf_IsFull
Syntax          : uint8_t CycBuf_IsFull(uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID                   
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value	: CYCBUF_RET_NOT_OPEN: channel not open
Return value	: CYCBUF_RET_BUF_FULL: channel is full
Return value 	: CYCBUF_RET_BUF_NOT_FULL: channel is not full
Description     : Judge the channel is full or not function 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_IsFull(uint8_t lv_ucChan)
{
	uint8_t lv_ucReval;

	if(lv_ucChan >= CYCBUF_CHAN_MAX_NUM)
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if(CYCBUF_STATUS_INUSE != gv_stCycBufArry[lv_ucReval].ucStatus)
		{
			lv_ucReval = CYCBUF_RET_NOT_OPEN;
		}
		else
		{
			if(gv_stCycBufArry[lv_ucChan].ulWriteIdx + 1 == gv_stCycBufArry[lv_ucChan].ulReadIdx)
			{
				lv_ucReval = CYCBUF_RET_BUF_FULL;
			}
			else
			{
				lv_ucReval = CYCBUF_RET_BUF_NOT_FULL;
			}
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}

/*******************************************************************************
Name            : CycBuf_CheckData
Syntax          : uint8_t CycBuf_CheckData(uint8_t lv_ucChan, uint32_t* lv_pulLenBuf)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID
Parameters(in) 	: lv_pulLenBuf: the length of the buffer which is to be checked
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the paramter error
Return value	: CYCBUF_RET_NOT_OPEN: channel not open
Return value	: CYCBUF_RET_SUCCESS: checkdata success
Description     : Check the length of channel unused area 
Call By         : -
|******************************************************************************/
uint8_t CycBuf_CheckData(uint8_t lv_ucChan, uint32_t *lv_pulLenBuf)
{
	uint8_t lv_ucReval = CYCBUF_RET_ERR_UNEXPECTED;
	uint32_t lv_ulWriteIdxTemp;
	uint32_t lv_ulReadIdxTemp;

	if (lv_ucChan >= CYCBUF_CHAN_MAX_NUM)
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	}
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if (CYCBUF_STATUS_INUSE != gv_stCycBufArry[lv_ucChan].ucStatus)
		{
			lv_ucReval = CYCBUF_RET_NOT_OPEN;
		}
		else
		{
			gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_BUSY;
			lv_ulWriteIdxTemp = gv_stCycBufArry[lv_ucChan].ulWriteIdx;
			lv_ulReadIdxTemp = gv_stCycBufArry[lv_ucChan].ulReadIdx;
			gv_stCycBufArry[lv_ucChan].ucStatus = CYCBUF_STATUS_INUSE;

			if (lv_ulWriteIdxTemp >= lv_ulReadIdxTemp)
			{
				*lv_pulLenBuf = lv_ulWriteIdxTemp - lv_ulReadIdxTemp;
			}
			else
			{
				*lv_pulLenBuf = gv_stCycBufArry[lv_ucChan].ulBuffSize + lv_ulWriteIdxTemp - lv_ulReadIdxTemp;
			}
			lv_ucReval = CYCBUF_RET_SUCCESS;
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}

/*******************************************************************************
Name            : CycBuf_Reset
Syntax          : uint8_t CycBuf_Reset(uint8_t lv_ucChan)
Sync/Async      : Synchronous
Reentrancy      :
Parameters(in)  : lv_ucChan: the channel ID          
Parameters(out) : none
Return value    : CYCBUF_RET_ERR_PARAMETER: the parameter error
Return value 	: CYCBUF_RET_NOT_OPEN: channel not open
Return value	: CYCBUF_RET_SUCCESS閿涳拷channel reset success
Description     : reset the cycbuffer channel
Call By         : -
|******************************************************************************/
uint8_t CycBuf_Reset(uint8_t lv_ucChan)
{
	uint8_t lv_ucReval;

	if(lv_ucChan >= CYCBUF_CHAN_MAX_NUM)
	{
		lv_ucReval = CYCBUF_RET_ERR_PARAMETER;
	} 
	else
	{
		CYCBUF_ENTER_CRITICAL_AREA();

		if(CYCBUF_STATUS_INUSE != gv_stCycBufArry[lv_ucChan].ucStatus)
		{
			lv_ucReval = CYCBUF_RET_NOT_OPEN;
		}
		else
		{
			gv_stCycBufArry[lv_ucChan].ulReadIdx 	= 0;
			gv_stCycBufArry[lv_ucChan].ulWriteIdx   = 0;

			lv_ucReval = CYCBUF_RET_SUCCESS;
		}

		CYCBUF_EXIT_CRITICAL_AREA();
	}
	return lv_ucReval;
}
/*EOF*/
