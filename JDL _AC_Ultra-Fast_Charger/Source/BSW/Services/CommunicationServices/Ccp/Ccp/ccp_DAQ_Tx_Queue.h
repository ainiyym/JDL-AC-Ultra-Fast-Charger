/*
 * ccp_DAQ_Tx_Queue.h
 *
 *  Created on: 2024-9-19
 *      Author: Lis
 */

#ifndef MBD_CCP_INC_CCP_DAQ_TX_QUEUE_H_
#define MBD_CCP_INC_CCP_DAQ_TX_QUEUE_H_

#include <ccp.h>
//#include "type_def.h"
//--------------------------------------------------------------------------//
#define CCP_DAQ_QUEUE_MAX CCP_ODT_BUF_NUM
#define ccp_DAQ_BUF_LEN   8u
//--------------------------------------------------------------------------//
typedef struct {
      char buf[ccp_DAQ_BUF_LEN];
      _Bool  occupied;
}TCCP_DAQ_Queue;

typedef struct{
    int add;
    int run;
    int count;
}TCCP_DAQ_Queue_Idx;

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
extern void ccp_DAQ_tx_queue_init(void);
extern void ccp_DAQ_tx_queue_push(uint8_t *buf);
extern void ccp_DAQ_tx_queue_pop(void);
extern _Bool ccp_DAQ_tx_queue_Is_empty(void);

#endif /* MBD_CCP_INC_CCP_DAQ_TX_QUEUE_H_ */
