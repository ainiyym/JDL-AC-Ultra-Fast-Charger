//****************************************************************************************
//*
//* File Name: STD_Fifo.c
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
#include "STD_Fifo.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t  ucOkFlag;
	uint16_t usCfgLen;
	uint16_t usLenIdx;
	uint16_t usData[FIFO_BUFF_MAX_SIZE];
}Fifo_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static Fifo_Struct stFifoArray[FIFO_CHAN_MAX_NUM];
/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void FIFO_InitMemory(void)
{
	uint8_t lv_ucChanIdx;

	LIB_SetMemory( (uint8_t *)(&stFifoArray), 0u, (uint16_t)(sizeof(stFifoArray) / sizeof(uint8_t)) );

	for(lv_ucChanIdx = 0; lv_ucChanIdx < FIFO_CHAN_MAX_NUM; lv_ucChanIdx++)
	{
		if(stFifoCfgTable[lv_ucChanIdx].ucAllowNum <= FIFO_BUFF_MAX_SIZE)
		{
			stFifoArray[lv_ucChanIdx].usCfgLen = stFifoCfgTable[lv_ucChanIdx].ucAllowNum;
		}
		else
		{
			stFifoArray[lv_ucChanIdx].usCfgLen = FIFO_BUFF_MAX_SIZE;
		}
	}
}

uint8_t FIFO_GetData(uint8_t lv_ucChan,uint16_t* plv_usData)
{
	uint8_t lv_ucReVal = STD_FALSE;
	uint16_t lv_usLenIdx; 

	Fifo_Struct * pstFifoArray = &stFifoArray[lv_ucChan];

	if((lv_ucChan < FIFO_CHAN_MAX_NUM) && (plv_usData != NULL))
	{
		if(STD_TRUE == pstFifoArray->ucOkFlag)
		{
		    LIB_ENTER_EXCLUSIVE_AREA_0();
			for( lv_usLenIdx = (uint16_t)0u; lv_usLenIdx < pstFifoArray->usCfgLen; lv_usLenIdx++ )
			{
				plv_usData[lv_usLenIdx] = pstFifoArray->usData[lv_usLenIdx];
			}
			//pstFifoArray->ucOkFlag = STD_FALSE;
		    LIB_EXIT_EXCLUSIVE_AREA_0();
			lv_ucReVal = STD_TRUE;
		}
	}
	else
	{}

	return lv_ucReVal;
}

uint8_t FIFO_InsertData(uint8_t lv_ucChan,uint16_t lv_usInsertData)
{
	uint8_t lv_ucReVal = STD_FALSE;

	Fifo_Struct * pstFifoArray = &stFifoArray[lv_ucChan];

	if(1)//STD_STD_FALSE == pstFifoArray->ucOkFlag)
	{
		LIB_ENTER_EXCLUSIVE_AREA_0();
		pstFifoArray->usData[pstFifoArray->usLenIdx] = lv_usInsertData;

		if(pstFifoArray->usLenIdx >= (pstFifoArray->usCfgLen - (uint16_t)1u))
		{
			pstFifoArray->usLenIdx = 0u;
			pstFifoArray->ucOkFlag = STD_TRUE;
		}
		else
		{
			pstFifoArray->usLenIdx++;
		}
		LIB_EXIT_EXCLUSIVE_AREA_0();
		lv_ucReVal = STD_TRUE;
	}
	else
	{}

	return lv_ucReVal;
}

uint8_t FIFO_ClearData(uint8_t lv_ucChan)
{
	uint8_t lv_ucReVal = STD_FALSE;
	uint16_t lv_usLenIdx;

	Fifo_Struct * pstFifoArray = &stFifoArray[lv_ucChan];

	LIB_ENTER_EXCLUSIVE_AREA_0();
	for( lv_usLenIdx = (uint16_t)0u; lv_usLenIdx < pstFifoArray->usCfgLen; lv_usLenIdx++ )
	{
		pstFifoArray->usData[lv_usLenIdx] = 0u;;
	}
	pstFifoArray->ucOkFlag = STD_FALSE;
	pstFifoArray->usLenIdx = 0u;
	LIB_EXIT_EXCLUSIVE_AREA_0();
	lv_ucReVal = STD_TRUE;
	return lv_ucReVal;
}




/*EOF*/
