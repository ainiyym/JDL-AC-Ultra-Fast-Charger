//******************************************************************************
//* File Name: Meter.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module header file
/*******************************************************************************/
#if !defined (__METER_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_utils.h"

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
typedef struct
{
    bool error_flag;                  // error flag
    uint8_t recover_count;            // recovery count
    uint32_t error_count;             // error count
    MeterReadState_t read_state;      // read state for each meter
    MeterReadState_t next_read_state; // next read state
    uint32_t last_read_time;          // last read times
} MeterModule_t;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
void MeterModule_Init(void);
void MeterModule_Process(void);
void MeterModule_ErrHandleCallback(void);
void MeterModule_HandleResponse(uint8_t addr, uint8_t *data, uint8_t datalen);
#endif /* __METER_H */
/* EOL */
