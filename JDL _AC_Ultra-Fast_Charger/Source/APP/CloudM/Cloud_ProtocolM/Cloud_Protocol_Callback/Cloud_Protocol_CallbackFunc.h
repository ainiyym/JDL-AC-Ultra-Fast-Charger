//******************************************************************************
//* File Name: Cloud_Protocol_CallbackFunc.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: clould protocol callbackfunc module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CALLBACK_FUNC_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CALLBACK_FUNC_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "Cloud_Protocol_Msg.h"
#include "Cloud_Protocol_Cfg.h"

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

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x01_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
void Cloud_Protocol_0x02_Callback(void *arg, uint8_t *msg, uint16_t msglen);
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x03_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
void Cloud_Protocol_0x04_Callback(void *arg, uint8_t *msg, uint16_t msglen);
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x05_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
void Cloud_Protocol_0x06_Callback(void *arg, uint8_t *msg, uint16_t msglen);
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x09_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
void Cloud_Protocol_0x0A_Callback(void *arg, uint8_t *msg, uint16_t msglen);
void Cloud_Protocol_0x12_Callback(void *arg, uint8_t *msg, uint16_t msglen);
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x13_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
Cloud_Protocol_Send_Status_T Cloud_Protocol_0x55_Callback(void *arg, uint8_t *buff, uint16_t buffSize);
void Cloud_Protocol_0x56_Callback(void *arg, uint8_t *msg, uint16_t msglen);
#endif /* __CLOUD_PROTOCOL_CALLBACK_FUNC_H */
/* EOL */
