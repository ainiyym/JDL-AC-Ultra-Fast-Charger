//******************************************************************************
//* File Name: Cloud_Protocol_CallbackFunc.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_CallbackFunc.h"

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
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// Parse protocol frames
void Cloud_Protocol_0x01_Callback(void *arg)
{
	// Handle frame type 0x01 (Pile Login Auth)
	// Add your processing logic here
}

void Cloud_Protocol_0x02_Callback(void *arg, uint8_t *msg, uint16_t msglen)
{
	// Handle frame type 0x02 (Login Auth Ack)
	// Add your processing logic here
}

void Cloud_Protocol_0x03_Callback(void *arg)
{
	// Handle frame type 0x03 (Pile Heartbeat)
	// Add your processing logic here
}

void Cloud_Protocol_0x04_Callback(void *arg, uint8_t *msg, uint16_t msglen)
{
	// Handle frame type 0x04 (Heartbeat Ack)
	// Add your processing logic here
}

void Cloud_Protocol_0x05_Callback(void *arg)
{
	// Handle frame type 0x05 (Billing Model Verification)
	// Add your processing logic here
}

void Cloud_Protocol_0x06_Callback(void *arg, uint8_t *msg, uint16_t msglen)
{
	// Handle frame type 0x06 (Billing Model Verify Ack)
	// Add your processing logic here
}

void Cloud_Protocol_0x09_Callback(void *arg)
{
	// Handle frame type 0x09 (Pile Billing Model Request)
	// Add your processing logic here
}

void Cloud_Protocol_0x0A_Callback(void *arg, uint8_t *msg, uint16_t msglen)
{
	// Handle frame type 0x0A (Billing Model Request Ack)
	// Add your processing logic here
}

void Cloud_Protocol_0x12_Callback(void *arg, uint8_t *msg, uint16_t msglen)
{
	// Handle frame type 0x12 (Read Real-time Monitor Data)
	// Add your processing logic here
}

void Cloud_Protocol_0x13_Callback(void *arg)
{
	// Handle frame type 0x13 (Offline Monitor Data)
	// Add your processing logic here
}

/* EOL */
