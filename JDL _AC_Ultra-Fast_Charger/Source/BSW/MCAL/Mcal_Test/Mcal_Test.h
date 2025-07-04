#ifndef MCAL_TEST_H
#define MCAL_TEST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Mcal_APP.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MCAL_TEST_PERIOD 1000
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Mcal_Test_Run(void);
extern void Mcal_test_1ms(void);
extern void Mcal_Can_Send_Test(void);
#endif // MCAL_TEST_H
