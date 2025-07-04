//****************************************************************************************
//*
//* File Name: STD_Fifo_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#if!defined (_STD_FIFO_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_FIFO_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "CpM.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define FIFO_BUFF_MAX_SIZE		(18u)

#define FIFO_BUFF_SIZE_CP_VOLT	(CPV_ADC_FIFO_MAX_NUM)
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

enum
{
	FIFO_CHAN_CP1_VOLT = 0u,
	FIFO_CHAN_CP2_VOLT,
	FIFO_CHAN_IN_METER_VOLT,
	FIFO_CHAN_IN_METER_CURR,
	FIFO_CHAN_IN_ACDC_VOLT,
	FIFO_CHAN_IN_5V_VOLT,
	FIFO_CHAN_ENERGY_AVG_VOLT,
	FIFO_CHAN_ENERGY_AVG_CURR,
	FIFO_CHAN_RLY_OUT_VOLT,
	FIFO_CHAN_MAX_NUM,
};

typedef struct
{
    uint8_t ucAllowNum;
} FifoCfg_Struct;

/*******************************************************************************
|    Table Definition
|******************************************************************************/
extern const FifoCfg_Struct stFifoCfgTable[FIFO_CHAN_MAX_NUM];
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif
/*EOF*/
