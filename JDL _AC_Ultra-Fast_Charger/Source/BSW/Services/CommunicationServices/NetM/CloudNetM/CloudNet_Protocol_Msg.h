//******************************************************************************
//* File Name: CloudNet_Protocol_Msg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol message module header file
/*******************************************************************************/
#if !defined (__CLOUDNET_PROTOCOL_MSG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUDNET_PROTOCOL_MSG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stdint.h"
#include "Cloud_Cfg.h"
#include "tcp.h"

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
extern void CloudNet_Protocol_Msg_Init(void);
extern void CloudNet_Protocol_RcvMsg_Process(void);
extern void CloudNet_Protocol_SendMsg(uint8_t *pMsg, uint16_t MsgLen, uint8_t MsgType);
#endif /* __CLOUDNET_PROTOCOL_MSG_H */
/* EOL */
