#ifndef MCAL_TEST_H
#define MCAL_TEST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Mcal_APP.h"
#include "MessageBuffer.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MCAL_TEST_PERIOD 1000

// 测试状态定义
typedef enum {
    TEST_STATE_IDLE = 0,
    TEST_STATE_INIT,
    TEST_STATE_SEND_NORMAL,
    TEST_STATE_RECEIVE_NORMAL,
    TEST_STATE_SEND_OVERSIZE,
    TEST_STATE_SEND_ZERO_LENGTH,
    TEST_STATE_RECEIVE_TIMEOUT,
    TEST_STATE_STRESS_TEST,
    TEST_STATE_COMPLETE,
    TEST_STATE_ERROR
} test_state_t;

// 测试结果定义
typedef enum {
    TEST_RESULT_PASS = 0,
    TEST_RESULT_FAIL,
    TEST_RESULT_IN_PROGRESS
} test_result_t;
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Mcal_Test_Run(void);
extern test_result_t run_comm_test(MessageBuffer_Comm_System_t *comm_system);
extern void Mcal_Can_Send_Test(void);
#endif // MCAL_TEST_H
