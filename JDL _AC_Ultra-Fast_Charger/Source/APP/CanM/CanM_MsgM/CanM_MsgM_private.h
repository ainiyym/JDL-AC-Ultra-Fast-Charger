/*
 * File: CanM_MsgM_private.h
 *
 * Code generated for Simulink model 'CanM_MsgM'.
 *
 * Model version                  : 1.133
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Tue Jul 22 14:35:47 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CanM_MsgM_private_h_
#define RTW_HEADER_CanM_MsgM_private_h_
#include "rtwtypes.h"
#include "CanM_MsgM.h"
#include "CanM_MsgM_types.h"

extern uint64_T CanM_MsgM_BitShift(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift_e(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift1(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift2(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift3(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift4(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift5(uint64_T rtu_u);
extern uint64_T CanM_MsgM_BitShift6(uint64_T rtu_u);
extern void CanM_MsgM_SECC_MSG1(boolean_T rtu_Enable, uint16_T
  rtu_SECC_MSG1_L1_Curr, uint16_T rtu_SECC_MSG1_L1_Vol, uint16_T
  rtu_SECC_MSG1_L2_Curr, uint16_T rtu_SECC_MSG1_L2_Vol, uint16_T
  rtu_SECC_MSG1_L3_Curr, uint16_T rtu_SECC_MSG1_L3_Vol, uint64_T
  *rty_SECC_MSG1_OutU64, B_SECC_MSG1_CanM_MsgM_T *localB,
  DW_SECC_MSG1_CanM_MsgM_T *localDW);

#endif                                 /* RTW_HEADER_CanM_MsgM_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
