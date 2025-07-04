//****************************************************************************************
//*
//* File Name: STD_Lib_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

#if!defined (_STD_LIB_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define _STD_LIB_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace<MISRA-C3:2.5:Not a defect:Justified> Normal. */
#define STD_LIB_DISABLE                 (0U)
#define STD_LIB_ENABLE                  (1U)

#define STD_LIBCFG_SETMEMORY            STD_LIB_ENABLE
#define STD_LIBCFG_STRLEN				STD_LIB_ENABLE
#define STD_LIBCFG_STRSTR				STD_LIB_ENABLE
#define STD_LIBCFG_ASCII_TO_INTEGER		STD_LIB_ENABLE
#define STD_LIBCFG_COMPARE              STD_LIB_ENABLE
#define STD_LIBCFG_COMPARER             STD_LIB_ENABLE
#define STD_LIBCFG_COMPARERAMTOROM		STD_LIB_ENABLE
#define STD_LIBCFG_COPY                 STD_LIB_ENABLE
#define STD_LIBCFG_COPYFROMROM			STD_LIB_ENABLE
#define STD_LIBCFG_COPYR                STD_LIB_ENABLE
#define STD_LIBCFG_XOR					STD_LIB_ENABLE
#define STD_LIBCFG_WORDTOBYTE			STD_LIB_ENABLE
#define STD_LIBCFG_CALCULATEAVERAGE     STD_LIB_ENABLE
#define STD_LIBCFG_READDATA             STD_LIB_ENABLE
#define STD_LIBCFG_WRITEDATA            STD_LIB_ENABLE
#define STD_LIBCFG_STATUSFILTER         STD_LIB_ENABLE
#define STD_LIBCFG_STATUSFILTERTYPE2	STD_LIB_ENABLE

/* Access macros for ROM data */
#define ACT_ROM_POST
#define ACT_ROM_STR(x)    (x)

#define ACT_ROM_BYT(x)    (*(x))
#define ACT_ROM_WRD(x)    (*(x))
#define ACT_ROM_DWD(x)    (*(x))

#define ACT_ROM_R_BYT(x)  (x)
#define ACT_ROM_R_WRD(x)  (x)
#define ACT_ROM_R_DWD(x)  (x)

#ifndef  NULL
#define NULL  ((void *) 0)
#endif
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
