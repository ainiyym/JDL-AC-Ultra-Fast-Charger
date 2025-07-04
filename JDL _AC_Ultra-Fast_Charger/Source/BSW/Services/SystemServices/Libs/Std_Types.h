/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2013 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Std_Types.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Provision of Standard Types
 *  
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *      
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Joachim Kalmbach              Jk            Vector Informatik
 *  Heike Honert                  visht         Vector Informatik
 *  Eugen Stripling               visseu        Vector Informatik
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2007-08-01  Jk                    Initial creation
 *  03.00.00  2007-10-10  Jk                    Changes for AUTOSAR 3.0
 *  03.00.01  2007-11-20  Jk                    Version defines for CFG management added
 *  03.01.00  2008-03-05  Jk                    Include order changed
 *  03.02.00  2008-03-18  Jk                    Version defines changed to specification version
 *  03.03.00  2008-06-03  Jk                    changed ModuleId Type in Std_VersionInfoType from uint8_t to uint16_t
 *  03.03.01  2009-11-11  visht  ESCAN00038201  Support the Standard Type E_PENDING
 *  03.04.00  2011-02-15  visht                 support ASR 4.0R1
 *  03.04.01  2011-02-28  visht  ESCAN00049579  incorrect BSW version scan information
 *  03.04.02  2012-08-23  visht                 version define corrected - replace PATCH by REVISION - (STD_TYPES_AR_RELEASE_REVISION_VERSION)
 *            2012-10-23  visseu                Add _VENDOR_ID and _MODULE_ID as specified in "AUTOSAR_TR_BSWModuleList.pdf" (R4.0 Rev 3)
 *  03.04.03  2013-02-13  visseu                No changes, only SW version corrected
 *  03.04.04  2013-05-29  visseu ESCAN00067740  Add general defines with prefix: STD
 *  03.04.05  2023-03-21  chenls                add STD_NULL,STD_OPT_NONE,STD_OPT_PROCESSING,STD_OPT_SUCCESS,STD_OPT_FAILURE definition
 *********************************************************************************************************************/

#ifndef STD_TYPES_H
# define STD_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

//# include "Compiler.h"
#include "stdint.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define STD_TYPES_VENDOR_ID               (30u)  /* SREQ00015345, SREQ00015361 */
# define STD_TYPES_MODULE_ID               (197u) /* SREQ00015345, SREQ00015361 */

/* ESCAN00067740 */
# define STD_VENDOR_ID                     STD_TYPES_VENDOR_ID
# define STD_MODULE_ID                     STD_TYPES_MODULE_ID

/* ##V_CFG_MANAGEMENT ##CQProject : CommonAsr__Common CQComponent : Impl_StdTypes */
# define COMMONASR__COMMON_IMPL_STDTYPES_VERSION 0x0304
# define COMMONASR__COMMON_IMPL_STDTYPES_RELEASE_VERSION 0x04

/* AUTOSAR Software Specification Version Information */
/* AUTOSAR release 4.0 R3 */
#  define STD_TYPES_AR_RELEASE_MAJOR_VERSION       (4u)
#  define STD_TYPES_AR_RELEASE_MINOR_VERSION       (0u)
#  define STD_TYPES_AR_RELEASE_REVISION_VERSION    (3u)

/* ESCAN00067740 */
#  define STD_AR_RELEASE_MAJOR_VERSION             STD_TYPES_AR_RELEASE_MAJOR_VERSION
#  define STD_AR_RELEASE_MINOR_VERSION             STD_TYPES_AR_RELEASE_MINOR_VERSION
#  define STD_AR_RELEASE_REVISION_VERSION          STD_TYPES_AR_RELEASE_REVISION_VERSION

/* Component Version Information */
# define STD_TYPES_SW_MAJOR_VERSION       (3u)
# define STD_TYPES_SW_MINOR_VERSION       (4u)
# define STD_TYPES_SW_PATCH_VERSION       (4u)

/* ESCAN00067740 */
# define STD_SW_MAJOR_VERSION             STD_TYPES_SW_MAJOR_VERSION
# define STD_SW_MINOR_VERSION             STD_TYPES_SW_MINOR_VERSION
# define STD_SW_PATCH_VERSION             STD_TYPES_SW_PATCH_VERSION

# define STD_HIGH     1u     /* Physical state 5V or 3.3V */
# define STD_LOW      0u     /* Physical state 0V */

# define STD_ACTIVE   1u     /* Logical state active */
# define STD_IDLE     0u     /* Logical state idle */

#ifndef STD_ON
#define STD_ON 		  1U	/* Used to describe a Function or Module enable */
#endif

#ifndef STD_OFF
#define STD_OFF       0U	/* Used to describe a Function or Module disable */
#endif

# define STD_TRUE     1u     /* Used to describe a valid State*/
# define STD_FALSE    0u     /* Used to describe a invalid State*/

# define STD_OPT_NONE        0u  /* Used to describe a operation not start*/
# define STD_OPT_PROCESSING  1u  /* Used to describe a operation not finish*/
# define STD_OPT_SUCCESS     2u  /* Used to describe a operation success*/
# define STD_OPT_FAILURE     3u  /* Used to describe a operation failure*/

# define STD_NULL    ((void *)0) /* Null pointer */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
typedef enum
{
    GLOBAL_E_OK = 0x0,                      /* 执行成功 */
    GLOBAL_E_ERROR,                         /* 执行错误，未指定原因 */
    GLOBAL_E_UNKNOWN,                       /* 执行错误，未知原因 */    
    GLOBAL_E_NOT_IMPLEMENT,                 /* 函数功能未实现 */
    GLOBAL_E_INVALID_PARAM,                 /* 无效参数 */
    GLOBAL_E_PARAM_OUT_OF_RANGE,            /* 参数超出有效范围 */
    GLOBAL_E_NULL_POINTER,                  /* 无效空指针 */
    GLOBAL_E_DIVIDE_ZERO,                   /* 除0错 */
    GLOBAL_E_WAIT_ASYNC_RESPONSE,           /* 等待异步响应 */
    GLOBAL_E_ASYNC_RESP_OVERTIME,           /* 异步响应超时 */
    GLOBAL_E_ASYNC_RESP_ERROR,              /* 异步响应错误 */
    GLOBAL_E_MAX = 0x3F,                    /* 最大全局错误号，0x40-0xFF为应用模块自定义返回值类型 */
}GlobalErrorType_t;

enum
{
    FALSE = 0,
    TRUE = !FALSE
};

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* This typedef has been added for OSEK compliance */
# ifndef STATUSTYPEDEFINED
#  define STATUSTYPEDEFINED
#  define E_OK      0u
//typedef unsigned char StatusType; /* OSEK compliance */
# endif

# define E_NOT_OK  1u

#  define E_PENDING 2u

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif  /* STD_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/
