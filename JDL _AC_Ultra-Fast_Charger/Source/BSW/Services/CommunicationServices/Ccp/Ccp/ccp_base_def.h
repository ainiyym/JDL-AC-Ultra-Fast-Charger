/*
 * ccp_base_def.h
 *
 *  Created on: 2024-9-9
 *      Author: Lis
 */

#ifndef MBD_CCP_INC_CCP_BASE_DEF_H_
#define MBD_CCP_INC_CCP_BASE_DEF_H_

#include <ccp_cfg.h>

#define CCP_REF_PAGE_INDEX    (0x00u)
#define CCP_WORK_PAGE_INDEX   (0x01u)

#define CCP_REF_PAGE_ADDR     (0x01u)
#define CCP_WORK_PAGE_ADDR    (0x02u)
/*******************************************************************************
 *   Macro definition
 *******************************************************************************/
#define E_OK 0u
#define E_NOT_OK 1u

/* Declare CCP-protocol version */
#define CCP_VERSION_MAJOR              (0x02u)
#define CCP_VERSION_MINOR              (0x01u)

/* Resource Mask */
#define CCP_RM_ALL_LOCKED              (0x00u)
#define CCP_RM_CAL                     (0x01u)
#define CCP_RM_DAQ                     (0x02u)
#define CCP_RM_PGM                     (0x40u)

/* CCP Status */
#define CCP_DISCONNECTED               (0x00u)
#define CCP_CONNECTED                  (0x20u)
#define CCP_TMP_DISCONNECTED           (0x10u)
#define CCP_RESUME                     (0x04u)
#define CCP_STORE                      (0x40u)
#define CCP_RUN                        (0x80u)

/* Basic Commands */
#define CCP_CONNECT                    (0x01u)
#define CCP_SET_MTA                    (0x02u)
#define CCP_DNLOAD                     (0x03u)
#define CCP_UPLOAD                     (0x04u)
#define CCP_TEST                       (0x05u)
#define CCP_START_STOP                 (0x06u)
#define CCP_DISCONNECT                 (0x07u)
#define CCP_START_STOP_ALL             (0x08u)
#define CCP_SHORT_UPLOAD               (0x0Fu)
#define CCP_GET_DAQ_SIZE               (0x14u)
#define CCP_SET_DAQ_PTR                (0x15u)
#define CCP_WRITE_DAQ                  (0x16u)
#define CCP_EXCHANGE_ID                (0x17u)
#define CCP_GET_CCP_VERSION            (0x1Bu)
#define CCP_DNLOAD6                    (0x23u)

/* Optional Commands */
#define CCP_GET_CAL_PAGE               (0x09u)
#define CCP_SET_S_STATUS               (0x0Cu)
#define CCP_GET_S_STATUS               (0x0Du)
#define CCP_BUILD_CHKSUM               (0x0Eu)
#define CCP_CLEAR_MEMORY               (0x10u)
#define CCP_SET_CAL_PAGE               (0x11u)
#define CCP_GET_SEED                   (0x12u)
#define CCP_UNLOCK                     (0x13u)
#define CCP_PROGRAM                    (0x18u)
#define CCP_MOVE                       (0x19u)
#define CCP_DIAG_SERVICE               (0x20u)
#define CCP_ACTION_SERVICE             (0x21u)
#define CCP_PROGRAM6                   (0x22u)

/* Response ID */
#define CCP_RESPONSE_CODE              (0xFFu)

//---------------------------------------------//
//                 Error codes
//---------------------------------------------//
#define CCP_NO_ERROR                   (0x00u)
#define CCP_DAQ_OVERLOAD               (0x01u)
#define CCP_COMMAND_BUSY               (0x10u)
#define CCP_DAQ_BUSY                   (0x11u)
#define CCP_INTERNAL_TIMEOUT           (0x12u)
#define CCP_KEY_REQUEST                (0x18u)
#define CCP_SESSION_REQUEST            (0x19u)
#define CCP_COLD_START_REQ             (0x20u)
#define CCP_CAL_INIT_REQ               (0x21u)
#define CCP_DAQ_INIT_REQ               (0x22u)
#define CCP_CODE_UPDATE_REQ            (0x23u)
#define CCP_UNKNOWN_COMMAND            (0x30u)
#define CCP_COM_SYNTAX_ERROR           (0x31u)
#define CCP_OUT_OF_RANGE               (0x32u)
#define CCP_ACCESS_DENIED              (0x33u)
#define CCP_OVERLOAD                   (0x34u)
#define CCP_ACCESS_LOCKED              (0x35u)
#define CCP_RESOURCE_USELESS           (0x36u)

/* Session Status */
#define CCP_DEFAULT_SESSION            (0x00u)
#define CCP_SESSION_CAL                (0x01u)
#define CCP_SESSION_DAQ                (0x02u)
#define CCP_SESSION_RUSUME             (0x04u)
#define CCP_SESSION_STORE              (0x40u)
#define CCP_SESSION_RUN                (0x80u)

/* For Disconnect */
#define CCP_END_SESSION                (0x01u)
#define CCP_TEM_END_SESSION            (0x00u)

/* For Exchange ID */
#define CCP_ASCII_TYPE                 (0x00u)

/* For Upload */
#define CCP_UPLOAD_MAX                 (0x05u)

/* For Get DAQ Size */
#define CCP_UNDEFINED_DAQ              (0x00u)
#define CCP_NO_FIRST_PID               (0x00u)

/* For Get_S_Status */
#define CCP_NO_ADDITIONAL_INFO         (0x00u)

/* For Start_Stop */
#define CCP_STOP_DAQ                   (0x00u)
#define CCP_START_DAQ                  (0x01u)
#define CCP_PREPARE_DAQ                (0x02u)

/* For Start_Stop_All */
#define CCP_STOP_DAQ_ALL               (0x00u)
#define CCP_START_DAQ_ALL              (0x01u)

/* For Program */
#define CCP_PROGRAM_MAX_LENGTH         (0x05u)
#define CCP_PROGRAM6_LENGTH            (0x06u)

/* For Seed&Key */
#define CCP_NEED_UNLOCK                (0x01)
#define CCP_NO_UNLOCK                  (0x00)
#define CCP_RIGHT_KEY                  (0x00)
#define CCP_WRONG_KEY                  (0x01)

/* For Checksum */
#define CCP_CHECKSUM_RESULT_BYTES      (0x02u)

/* Other Definition */
#define CCP_COMMAND_CODE               (CCP_RX_Data[0])
#define CCP_CTR                        (CCP_RX_Data[1])
#define CCP_CAN_LENGTH                 (8u)
#define CCP_MAX_ELEMENT                (7u)
#define CCP_READ_BUF_LENGTH            (5u)
#define CCP_DEFAULT_ZERO               (0x00u)
#define CCP_NO_PRESCALER               (1u)
#define CCP_DAQ_CHANNEL_DEFAULT        (0xFFu)

#define CCP_DEFAULT_LOWBYTE_FLG        (0xFFFFFFFF)

/*******************************************************************************
 *   Macro Function
 *******************************************************************************/
#ifdef CCP_MSB
    #define CCP_Get_First_Byte(x)          ((uint8_t)((x)>>8u))
    #define CCP_Get_Second_Byte(x)         ((uint8_t)((x)&(0x00FF)))
    #define CCP_Make_16Bits(x,y)           ((uint16_t)((((uint16_t)(x))<<8u)|((uint16_t)(y))))
    #define CCP_Get_1Byte(x)               ((uint8_t)((x)>>24u))
    #define CCP_Get_2Byte(x)               ((uint8_t)(((x)>>16u)&(0x00FF)))
    #define CCP_Get_3Byte(x)               ((uint8_t)(((x)>>8u)&(0x00FF)))
    #define CCP_Get_4Byte(x)               ((uint8_t)((x)&(0x00FF)))
#else
    #define CCP_Get_First_Byte(x)          ((uint8_t)((x)&(0x00FF)))
    #define CCP_Get_Second_Byte(x)         ((uint8_t)((x)>>8u))
    #define CCP_Make_16Bits(x,y)           ((uint16_t)((((uint16_t)(y))<<8u)|((uint16_t)(x))))
    #define CCP_Get_1Byte(x)               ((uint8_t)((x)&(0x00FF)))
    #define CCP_Get_2Byte(x)               ((uint8_t)(((x)>>8u)&(0x00FF)))
    #define CCP_Get_3Byte(x)               ((uint8_t)(((x)>>16u)&(0x00FF)))
    #define CCP_Get_4Byte(x)               ((uint8_t)((x)>>24u))
#endif


#endif /* MBD_CCP_INC_CCP_BASE_DEF_H_ */
