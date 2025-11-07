//******************************************************************************
//* File Name: Cloud_Protocol_GagaM_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_GagaM module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_GAGAM_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_GAGAM_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Cfg.h"
#include "Cloud_Protocol_CallbackFunc.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_GAGA_PROTOCOL_ENABLE_STATUS               (1U)
#define CLOUD_GAGA_PROTOCOL_RESET_DEVICE_DELAY_TIME_S   (30 * 1000 / CLOUDM_TASK_PERIOD)

#define CLOUD_GAGA_PROTOCOL_CALL_SEND_FUNC              Cloud_Protocol_CallSendFunc
#define CLOUD_GAGA_PROTOCOL_RESTART_DEVICE              CLOUD_PROTOCOL_RESTART_DEVICE
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_PROTOCOL_GAGAM_CFG_H */
/* EOL */
