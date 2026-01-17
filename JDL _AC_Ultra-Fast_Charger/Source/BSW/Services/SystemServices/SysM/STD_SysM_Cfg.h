//******************************************************************************
//*
//* File Name: STD_SysM_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-04 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/
#if !defined (_STD_SYSM_CFG_H)
#define _STD_SYSM_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "STD_Types.h"
#include "STD_Lib.h"
#include <stdbool.h>
#include <stdint.h>
#include "STD_LogService.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
#define SYSM_CONNECTOR2_ENABLE              (STD_ON)

typedef enum
{
    SYSM_CP_MODE_4V,
    SYSM_CP_MODE_12V
}SysM_CpVol_Enum;

typedef enum
{
    SYS_CONNECTOR1, /* Connector 1 */
#if (SYSM_CONNECTOR2_ENABLE == STD_ON)
    SYS_CONNECTOR2, /* Connector 2 */
#endif
    SYS_CONNECTOR_NUM_MAX
} SysConnector_Num_Enum;

// pile type
typedef enum
{
    SYS_PILE_TYPE_DC = 0, // DC pile
    SYS_PILE_TYPE_AC = 1  // AC pile
} SYS_PileType_E;
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define SYSM_TASK_PERIOD                    (10U)
#define SYSM_RESET_PREPARE_MAX_CNT          ((uint32_t)30U / SYSM_TASK_PERIOD)
#define SYSM_REPORT_BASIC_INFO_CNT          (3000U / SYSM_TASK_PERIOD) /* 3s */
#define SYSN_RESET_DELAY_TIME_MS            (3000U / SYSM_TASK_PERIOD) /* Remote reset delay time */

#define SYSM_4HCT4851D_SOFTTIMER_PERIOD      (50U) /* 74HCT4851D软定时器周期，单位us */
#define SYSM_ADC_PERIOD_COLLECTION_TIME      (SYSM_4HCT4851D_SOFTTIMER_PERIOD * 8)  /* ADC周期采样周期，单位us */

/*Power Down*/
#define SYSM_PowerDownStatus()                 Meter_GetPowerDownStatus() /* Get power down status */
#define SYSM_GetPowerDownVoltValue()           Meter_GetPowerDownVoltValue() /* Get power down voltage value */
#define SYSM_DisableAllMode()\
do{\
}while(0)
#define SYSM_OUT_POWER_DOWN					     (100U)   /* Power down voltage threshold value 100V */
#define SYSM_POWERDOWN_FILTER_TIME				 (uint16_t)( 500 / SYSM_TASK_PERIOD )   /* 500ms */
#define SYSM_DELAY_1S							 (uint16_t)( 1000U / SYSM_TASK_PERIOD)  /* 1s */

//System parameters Macro Definition
#define SYS_CONNECTOR_RATED_CURRENT           (32U)    /* A */
#define SYS_CONNECTOR_RATED_VOLTAGE           (380U)   /* V */

// System information
#define SYS_MANUFACTURER                      "JDL"       // Manufacturer
#define SYS_SOFTWARE_VERSION                  "V1.0.0"    // Software version
#define SYS_HARDWARE_VERSION                  "V1.0.0"    // Hardware version
#define SYS_CONNECTOR_TYPE                    (SYS_PILE_TYPE_AC)      // Connector type
#define SYS_NUMBER_OF_CONNECTORS              (SYS_CONNECTOR_NUM_MAX) // Number of connectors

//Log Service Macro Definition
#define SYSM_DEBUG(fmt, ...) LOG_DEBUG(LOG_MODULE_SYSM, fmt, ##__VA_ARGS__) /* log output */
#define SYSM_INFO(fmt, ...)  LOG_INFO(LOG_MODULE_SYSM, fmt, ##__VA_ARGS__)  /* log output */
#define SYSM_WARN(fmt, ...)  LOG_WARN(LOG_MODULE_SYSM, fmt, ##__VA_ARGS__)  /* log output */
#define SYSM_ERROR(fmt, ...) LOG_ERROR(LOG_MODULE_SYSM, fmt, ##__VA_ARGS__) /* log output */
#define SYSM_CRITICAL(fmt, ...) LOG_CRITICAL(LOG_MODULE_SYSM, fmt, ##__VA_ARGS__) /* log output */
#define SYSM_PRINT_HEX(BUFF, LEN, R) LogService_Print_Hex_Array(LOG_MODULE_SYSM, BUFF, LEN, R)  /* print hex array */
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

/* polyspace:end<MISRA-C3:2.5:Not a defect:Justified> Normal. */

#endif
/*EOF*/
