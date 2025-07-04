/*
 *                                 CCP
 * ccp.h
 *
 *  Created on: 2024-9-9
 *      Author: Lis
 */

#ifndef MBD_CCP_CCP_H_
#define MBD_CCP_CCP_H_

#include <stdbool.h>
#include <ccp_base_def.h>
// #include <ccp_can_drv.h>
#include <ccp_cfg.h>
#include "ex_fun.h"
// #include "type_def.h"
// #include "trm_2837xD_Can.h"
// #include "mos_queue.h"
// #include "eeprom_opt.h"

// #define CCP_CAL_PAGE

// #define CCP_CAN_PKG_W_ID  IDX_CCP_CAN_W
//--------------------------------------------------------------------------------------------------------------//
// #define IDX_CCP_R 0u
// #define IDX_CCP_W 1u
////---------------------------------------------------------------------------------------------------------------//
// #define MSG_ID_CCP_R  0x2E1
// #define MSG_ID_CCP_W  0x2E2
//---------------------------------------------------------------------------------------------------------------//

typedef enum {ECU_BITS_23,ECU_BITS_45}TECU_Bits;

//-------------------------TCCP_CRO---------------------------//
typedef union{
    uint64_t all;
    struct{
        uint64_t cmd:8;  //BIT:0~BIT:7;   [command]
        uint64_t ctr:8;  //BIT:8~BIT:15   [counter]
        uint64_t dt:48;  //BIT:16~BIT:63  [data]
    }bit;
    //-------------------------------//
    struct{
        uint64_t dt0:8;
        uint64_t dt1:8;
        uint64_t dt2:8;
        uint64_t dt3:8;
        uint64_t dt4:8;
        uint64_t dt5:8;
        uint64_t dt6:8;
        uint64_t dt7:8;
    }bytes;
}TCCP_CRO;

//--------------------TCCP_CRM/EVENT_DTO-----------------------//
typedef union{
    uint64_t all;
    struct{
        uint64_t pid:8;  //BIT:0~BIT:7;
        uint64_t err:8;  //BIT:8~BIT:15
        uint64_t ctr:8;  //BIT:16~BIT:23
        uint64_t dt:40;  //BIT:24~BIT:63
    }bit;
    //-------------------------------//
    struct{
        uint16_t dt0:8;
        uint16_t dt1:8;
        uint16_t dt2:8;
        uint16_t dt3:8;
        uint16_t dt4:8;
        uint16_t dt5:8;
        uint16_t dt6:8;
        uint16_t dt7:8;
    }bytes;
}TCCP_CRM_EVENT_DTO;

//-----------------------TCCP_DAQ_DTO--------------------------//
typedef union{
    uint64_t all;
    struct{
        uint64_t pid:8;  //BIT:0~BIT:7;
        uint64_t dt:56;  //BIT:8~BIT:63
    }bit;
    //-------------------------------//
    struct{
        uint16_t dt0:8;
        uint16_t dt1:8;
        uint16_t dt2:8;
        uint16_t dt3:8;
        uint16_t dt4:8;
        uint16_t dt5:8;
        uint16_t dt6:8;
        uint16_t dt7:8;
    }bytes;
}TCCP_DAQ_DTO;

//-----------------------TCCP_Data--------------------------//
//convert MSB to LSB
//----------------------------------------------------------//
typedef union{
    uint64_t all;
    struct{
        uint16_t dt7:8;
        uint16_t dt6:8;
        uint16_t dt5:8;
        uint16_t dt4:8;
        uint16_t dt3:8;
        uint16_t dt2:8;
        uint16_t dt1:8;
        uint16_t dt0:8;
    }bytes;
    struct{
        uint16_t dt4:8;
        uint16_t dt3:16;
        uint16_t dt2:16;
        uint16_t dt1:16;
        uint16_t dt0:8;
    }words;
}TCCP_Data;
//--------------------------------------------------------------//

typedef struct {
  uint8_t flag;                          /* DAQ Request flag */
  uint8_t channel;                       /* event channel of DAQ */
  uint8_t prepare;                       /* Prepare Flag */
} CCP_DAQ_strcut;

//--------------------------------------------------------------//
#define MAX_EVENT_NUM 30u
typedef struct{
  uint8_t  event_ch[MAX_EVENT_NUM];
  uint8_t  prescale[MAX_EVENT_NUM];
  uint32_t  counter[MAX_EVENT_NUM];
  uint8_t  ct;
}TCCP_DAQ_Channel;

//--------------------------------------------------------------------------------------------------------------//
extern void ccp_init(void);

static uint16_t ccp_Get_ECU_Addr(TCCP_CRO v,TECU_Bits bits);
extern void ccp_Read_CRO_Proc(uint64_t Data);

static void ccp_DTO_Transmit(void);        //response CRM or EVENT
//--------------------------------------------------------------------------------------------------------------//
static _Bool ccp_Test_Connect(TCCP_CRO v);
static void ccp_Set_MTA(TCCP_CRO v);

extern void ccp_DAQ_Task_1ms(void);
extern void ccp_DAQ_write_callback(void);
//--------------------------------------------------------------------------------------------------------------//
//                                   Interface
//--------------------------------------------------------------------------------------------------------------//
// typedef struct{
// void (*Read_CRO_Proc)(uint64_t tmp,TCCP_CRO *p);
// void (*Read_DTO_Proc)(uint64_t tmp,TCCP_CRM_EVENT_DTO *p);
// void (*Write_DTO_Proc)(uint64_t *tmp,TCCP_CRM_EVENT_DTO *p);
//}Tccp;

// extern Tccp ccp;
#endif /* MBD_CCP_CCP_H_ */
