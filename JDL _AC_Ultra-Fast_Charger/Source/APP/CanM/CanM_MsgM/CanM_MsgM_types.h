/*
 * File: CanM_MsgM_types.h
 *
 * Code generated for Simulink model 'CanM_MsgM'.
 *
 * Model version                  : 1.146
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Fri Jan 30 17:02:47 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CanM_MsgM_types_h_
#define RTW_HEADER_CanM_MsgM_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_CAN_MESSAGE_BUS_
#define DEFINED_TYPEDEF_FOR_CAN_MESSAGE_BUS_

typedef struct {
  uint8_T Extended;
  uint8_T Length;
  uint8_T Remote;
  uint8_T Error;
  uint32_T ID;
  real_T Timestamp;
  uint8_T Data[8];
} CAN_MESSAGE_BUS;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_CanM_MsgM_T RT_MODEL_CanM_MsgM_T;

#endif                                 /* RTW_HEADER_CanM_MsgM_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
