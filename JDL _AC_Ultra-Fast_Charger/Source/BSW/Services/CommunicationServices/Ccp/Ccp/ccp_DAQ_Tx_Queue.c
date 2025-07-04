/*
 * ccp_DAQ_ccp_DAQ_tx_queue.c
 *                              CAN Tx Buffer of CCP DAQ
 *  Created on: 2024-9-19
 *      Author: Lis
 */
#include <ccp_DAQ_Tx_Queue.h>
#include "Comm.h"
//------------------------------------------------------------------//

#ifdef _FLASH
      #pragma CODE_SECTION(ccp_DAQ_tx_queue_push, ".TI.ramfunc");
      #pragma CODE_SECTION(ccp_DAQ_tx_queue_pop, ".TI.ramfunc");
      //#pragma DATA_SECTION(ccp_DAQ_tx_queue, "ramdata");
#endif
//------------------------------------------------------------------//

TCCP_DAQ_Queue     ccp_DAQ_tx_queue[CCP_DAQ_QUEUE_MAX];
TCCP_DAQ_Queue_Idx ccp_DAQ_queue_idx;

_Bool ccp_DAQ_queue_pop_enable = 0;
//------------------------------------------------------------------//
//                       sci_tx_init
//------------------------------------------------------------------//
void ccp_DAQ_tx_queue_init(void)
{
    int i=0;

    ccp_DAQ_queue_idx.add=0;
    ccp_DAQ_queue_idx.run=0;
    ccp_DAQ_queue_idx.count=0;

    for(i=0;i<CCP_DAQ_QUEUE_MAX;i++)
    {
        ccp_DAQ_tx_queue[i].occupied = 0;
    }
}

//------------------------------------------------------------------//
//                  void ccp_DAQ_tx_queue_push(uint8_t *buf)
//------------------------------------------------------------------//
void ccp_DAQ_tx_queue_push(uint8_t *buf)
{
    int i = 0;
    ccp_DAQ_queue_pop_enable = 0;
    
    if (ccp_DAQ_queue_idx.add < CCP_DAQ_QUEUE_MAX)
    {
        if (ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.add].occupied == 0)
        {
            for (i = 0; i < ccp_DAQ_BUF_LEN; i++)
            {
                ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.add].buf[i] = *buf;
                buf++;
            }
            ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.add].occupied = 1;

            ccp_DAQ_queue_idx.count++;
            ccp_DAQ_queue_idx.add++;

            if (ccp_DAQ_queue_idx.add >= CCP_DAQ_QUEUE_MAX)
            {
                ccp_DAQ_queue_idx.add = 0;
            }
        }
    }
    ccp_DAQ_queue_pop_enable = 1;
}

//------------------------------------------------------------------//
//              void ccp_DAQ_tx_queue_pop(TCCP_Can *p)
//------------------------------------------------------------------//
void ccp_DAQ_tx_queue_pop(void)
{
    TCCP_Data tmp_pkg;
    CCPCANMSGDATA CcpCanMsg_Data;

    if ((ccp_DAQ_queue_idx.count > 0) && (ccp_DAQ_queue_pop_enable == 1))
    {
        if (ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].occupied == 1)
        {
            // pop todo
            //-----------------------------------------//
            tmp_pkg.bytes.dt0 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[0];
            tmp_pkg.bytes.dt1 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[1];
            tmp_pkg.bytes.dt2 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[2];
            tmp_pkg.bytes.dt3 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[3];
            tmp_pkg.bytes.dt4 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[4];
            tmp_pkg.bytes.dt5 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[5];
            tmp_pkg.bytes.dt6 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[6];
            tmp_pkg.bytes.dt7 = ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].buf[7];

            CcpCanMsg_Data.Data.all = tmp_pkg.all;
            Comm_CcpCanMsgTx(CcpCanMsg_Data);

            //             (*p).Tx_buffer_u64=tmp_pkg.all;
            //             (*p).Write((*p).handle,(*p).objID[IDX_CCP_CAN_W]);

            //-----------------------------------------//
            ccp_DAQ_tx_queue[ccp_DAQ_queue_idx.run].occupied = 0;
            ccp_DAQ_queue_idx.count--;

            if (ccp_DAQ_queue_idx.run < CCP_DAQ_QUEUE_MAX)
            {
                ccp_DAQ_queue_idx.run++;
            }
            else
            {
                ccp_DAQ_queue_idx.run = 0;
            }
        }
        else
        {
        }

        if (ccp_DAQ_queue_idx.run >= CCP_DAQ_QUEUE_MAX)
        {
            ccp_DAQ_queue_idx.run = 0;
        }
        else
        {
        }
    }
    else
    {
    }
}

//------------------------------------------------------------------//
//                       bool ccp_DAQ_tx_queue_Is_empty(void)
//------------------------------------------------------------------//
_Bool ccp_DAQ_tx_queue_Is_empty(void)
{
  return (ccp_DAQ_queue_idx.count == 0);
}
