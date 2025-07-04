/*
 * Comm.h
 *
 * Author: Lixiguang
 *
 * 2025.4
 *
 * */

#ifndef COMM_H
#define COMM_H

#include "Ccp.h"
#include "Mcal_App.h"
#include "ex_fun.h"
#include "Ccp_CanDrv.h"

struct CCPCAN_DATA_BITS
{
    uint16_t Data0 : 8;
    uint16_t Data1 : 8;
    uint16_t Data2 : 8;
    uint16_t Data3 : 8;
    uint16_t Data4 : 8;
    uint16_t Data5 : 8;
    uint16_t Data6 : 8;
    uint16_t Data7 : 8;
};
union CCPCAN_DATA_REG
{
    uint64_t all;
    struct CCPCAN_DATA_BITS bit;
};

typedef struct
{
    union CCPCAN_DATA_REG Data;
} CCPCANMSGDATA;

extern void Comm_Init(void);
extern void Comm_SetCcp(CCPCANMSGDATA CcpCanMsg_Data);
extern void Comm_GetCcp(void);
extern void Comm_CcpCanMsgTx(CCPCANMSGDATA CcpCanMsg_Data);
extern void Comm_GetCcpPrd(void);

#endif





