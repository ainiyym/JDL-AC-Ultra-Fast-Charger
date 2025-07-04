//****************************************************************************************
//*
//* File Name: STD_Interface.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#if !defined (_STD_INTERFACE_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_INTERFACE_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
/* polyspace<MISRA-C3:1.1:Not a defect:Justified> Normal */
#include "Std_Types.h"

/* polyspace<MISRA-C3:1.1:Not a defect:Justified> Normal */
/*#include "interrupt_manager.h"*/
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* PRQA S 3458 L1 */ /* MD_MSR_19.4 *//* polyspace<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#define	LIB_ENTER_EXCLUSIVE_AREA_0()     /*SuspendAllInterrupts()*/
/* PRQA L:L1 */

/* PRQA S 3458 L1 */ /* MD_MSR_19.4 *//* polyspace<MISRA-C3:D4.9:Not a defect:Justified> Normal */
#define	LIB_EXIT_EXCLUSIVE_AREA_0()      /*ResumeAllInterrupts()*/
     
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

#endif
/*EOF*/
