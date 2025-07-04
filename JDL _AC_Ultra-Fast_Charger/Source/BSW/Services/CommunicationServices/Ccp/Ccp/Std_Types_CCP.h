/********************************************************************************************************************************
File Name	    : Std_Types_CCP.h
Author    		:
Created Date    : 2021.8.20
History         : Date           Version           Author            Description
                :
********************************************************************************************************************************/

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

/*******************************************************************************
*   Includes 
*******************************************************************************/
#include "Std_Types.h"
#include "stdint.h"
/*******************************************************************************
*   Macro 
*******************************************************************************/

/* This typedef has been addedf or OSEK compliance */     
#if 0                   
#ifndef STATUSTYPEDEFINED  
    #define STATUSTYPEDEFINED  
    #define E_OK    (0x00u) 
    /* OSEK compliance */ 
    typedef unsigned char StatusType; 
#endif 
 
#define E_NOT_OK  (0x01u) 
#endif
                                                
/*******************************************************************************
*   Typedef 
*******************************************************************************/                                                                   
typedef uint8_t Std_ReturnType;

typedef struct                                          
{
    uint16_t vendorID;                           
    uint16_t moduleID;
    uint8_t  instanceID;
    uint8_t  sw_major_version;                         
    uint8_t  sw_minor_version;
    uint8_t  sw_patch_version;
}Std_VersionInfoType;

#endif  /* _STD_TYPES_H_ */
