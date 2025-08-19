//******************************************************************************
//* File Name: FanM.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module header file
/*******************************************************************************/
#if !defined (__FANM_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __FANM_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"

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
    FANM_BROADCAST_ADDR = 0x00,              /* Broadcast address */
    FANM_CURRENT_RUNNING_AD_VOL_ADDR = 0x01, /* Current running AD voltage address */
    FANM_WRITING_AD_VOL_ADDR = 0x02,         /* Writer AD voltage address */
    FANM_SLAVE_COMMUNICATION_ADDR = 0x03,    /* Slave communication address */
    FANM_BAUD_RATE_ADDR = 0x04,              /* Baud rate address */
} FanM_Register_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void FanM_Init(void);
extern void FanM_10msMainFunction(void);

#endif /* __FANM_H */
/* EOL */
