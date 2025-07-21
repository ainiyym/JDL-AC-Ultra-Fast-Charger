/*
 * Comm.c
 *
 * Author: Lixiguang
 *
 * 2025.4
 *
 * */

#include "Comm.h"

void Comm_Init(void)
{
    ccp_init();
}

/*******PWM*********************/
#ifdef _FLASH
#pragma CODE_SECTION(Comm_SetCcp, ".TI.ramfunc");
#endif
void Comm_SetCcp(CCPCANMSGDATA CcpCanMsg_Data)
{
    uint64_t Data = 0;
    uint16_t DataTmp[8] = {0};
    DataTmp[0] = CcpCanMsg_Data.Data.bit.Data0;
    DataTmp[1] = CcpCanMsg_Data.Data.bit.Data1;
    DataTmp[2] = CcpCanMsg_Data.Data.bit.Data2;
    DataTmp[3] = CcpCanMsg_Data.Data.bit.Data3;
    DataTmp[4] = CcpCanMsg_Data.Data.bit.Data4;
    DataTmp[5] = CcpCanMsg_Data.Data.bit.Data5;
    DataTmp[6] = CcpCanMsg_Data.Data.bit.Data6;
    DataTmp[7] = CcpCanMsg_Data.Data.bit.Data7;
//    Data = fun.array_ToUint64(&(CcpCanMsg_Data.Data.bit.Data0));
    Data = fun.array_ToUint64(DataTmp);
    ccp_Read_CRO_Proc(Data);
}


void Comm_GetCcp(void)
{
    ccp_DAQ_write_callback();
}

void Comm_CcpCanMsgTx(CCPCANMSGDATA CcpCanMsg_Data)
{
    CCPCANMSGDATA CcpCanMsg_Data01;
    uint16_t DataTmp[8] = {0};

    fun.uint64_ToArray(CcpCanMsg_Data.Data.all, DataTmp);
    CcpCanMsg_Data01.Data.bit.Data0 = DataTmp[0];
    CcpCanMsg_Data01.Data.bit.Data1 = DataTmp[1];
    CcpCanMsg_Data01.Data.bit.Data2 = DataTmp[2];
    CcpCanMsg_Data01.Data.bit.Data3 = DataTmp[3];
    CcpCanMsg_Data01.Data.bit.Data4 = DataTmp[4];
    CcpCanMsg_Data01.Data.bit.Data5 = DataTmp[5];
    CcpCanMsg_Data01.Data.bit.Data6 = DataTmp[6];
    CcpCanMsg_Data01.Data.bit.Data7 = DataTmp[7];

    Ccp_CanDrv_Transmit((uint8_t *)&CcpCanMsg_Data01.Data.all, sizeof(CcpCanMsg_Data01.Data.all));
}

#ifdef _FLASH
#pragma CODE_SECTION(Comm_GetCcpPrd, ".TI.ramfunc");
#endif
void Comm_GetCcpPrd(void)
{
    ccp_DAQ_Task_1ms();
    Comm_GetCcp();
}
