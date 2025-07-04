/********************************************************************************************************************************
File Name	    : Ccp_Cfg.h
Author    		:
Created Date    : 2021.8.20
History         : Date           Version           Author            Description
                :
********************************************************************************************************************************/

#ifndef RTW_HEADER_Ccp_Cfg_h_
#define RTW_HEADER_Ccp_Cfg_h_

#ifndef _CCP_CFG_H_
#define _CCP_CFG_H_

/*******************************************************************************
 *    Includes
 *******************************************************************************/
#include<ComStack_Types.h>

/*******************************************************************************
 *   Configuration Macro
 *******************************************************************************/

/*******************************************************************************
 *   Node ID Config
 *******************************************************************************/
#define CCP_NODE_ID                    (0x55AAU)
#define CCP_BROADCAST_ID               (0xffffu)

/*******************************************************************************
 *   Seed&Key Config
 *******************************************************************************/
/* Seed&Key Enable */
//#define CCP_SEED_KEY

/*******************************************************************************
 *   DAQ Config

 *******************************************************************************/
#define CCP_EEPROM_OFFSET 0xD0000     //EEPROM OFFSET

#define CCP_CAL_PAGE

/* DAQ Enable */
#define CCP_DAQ_REQUEST

/*Max Number of DAQ*/
#define CCP_MAX_DAQ                    (5)

/*Max Number of ODT*/
#define CCP_MAX_ODT                    (30)

/*Max Number of ODT buffer for DAQ function*/
#define CCP_ODT_BUF_NUM                (30*5)

/* Max Number of DTO buffer for DTO transmit */
#define CCP_DTO_BUF_NUM                (5)

/*******************************************************************************
 *   Handle Config
 *******************************************************************************/
/*Handle Index between CanIf and CCP*/
#define CCP_CRO_PID                    (0x667)
#define CCP_UNDERLAYER                 (0u)

/*******************************************************************************
 *   Flash Config
 *******************************************************************************/
/* Flash Enable */
//#define CCP_FLASH                      (0)

/* Flash range for Upload & Short Upload Command */
#define CCP_FLASH_MAX_ADDR             (0x200000ul)
#define CCP_FLASH_MIN_ADDR             (0x1E0000ul)

/*******************************************************************************
 *   Checksum Config
 *******************************************************************************/
/* Checksum Enable */
//#define CCP_CHECKSUM

/* Checksum Block */
#define CCP_CHECKSUM_BLOCKSIZE         (256u)

/* Checksum calculation algorithm */
/*******************************************************************************
 *   Format  Config
 *******************************************************************************/
//#define CCP_MSB

/*******************************************************************************
 *   ECU Name Config
 *******************************************************************************/
#define CCP_ECU_NAME_LEGNTH            (3u)

/*******************************************************************************
 *    CCP_MTA Config
 *******************************************************************************/
/* Max number of CCP_MTA */
#define CCP_MTA_NUM                    (2u)

/*******************************************************************************
 *   Special Config
 *******************************************************************************/
/*Calculation Gloab_Address*/
#define CCP_Gloab_Address(a)           (a)

/*FAR Pointer*/
#define CCP_FAR

/* Config for special chips which there are 16bits data in an address unit */
/* Such as some chips of TI */
/*******************************************************************************
 *   hualin.deng added
 *******************************************************************************/
#define MB_IDType_ST                   0
#define CCP_CANID_TYPE                 MB_IDType_ST
#define CCP_DTO_ID                     (0x7E1)
#define CCP_CAN_RX_BUFNUM              1
#define CCP_CAN_TX_BUFNUM              0
#define CCP_CAN_MODULE                 1
#define Ccp_DisableInterrupt()         { IfxCpu_disableInterrupts(); }
#define Ccp_EnableInterrupt()          { IfxCpu_enableInterrupts();  }
#endif
#endif                                 /* RTW_HEADER_Ccp_Cfg_h_ */


