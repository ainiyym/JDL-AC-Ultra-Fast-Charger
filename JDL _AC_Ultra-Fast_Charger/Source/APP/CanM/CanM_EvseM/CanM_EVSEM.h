/*
 * File: CanM_EVSEM.h
 *
 * Code generated for Simulink model 'CanM_EVSEM'.
 *
 * Model version                  : 1.122
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Tue Jul 15 11:06:36 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CanM_EVSEM_h_
#define RTW_HEADER_CanM_EVSEM_h_
#ifndef CanM_EVSEM_COMMON_INCLUDES_
#define CanM_EVSEM_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* CanM_EVSEM_COMMON_INCLUDES_ */

#include "CanM_EVSEM_types.h"

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint8_T Evse_CanCtrlCnt1;            /* '<S2>/Evse_CanCtrl_Logic' */
  uint8_T is_active_c1_CanM_EVSEM;     /* '<S2>/Evse_CanCtrl_Logic' */
  uint8_T is_c1_CanM_EVSEM;            /* '<S2>/Evse_CanCtrl_Logic' */
  uint8_T Evse_CanCtrlCnt;             /* '<S1>/Evse_CanCtrl_Logic' */
  uint8_T is_active_c3_CanM_EVSEM;     /* '<S1>/Evse_CanCtrl_Logic' */
  uint8_T is_c3_CanM_EVSEM;            /* '<S1>/Evse_CanCtrl_Logic' */
} DW_CanM_EVSEM_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T EVSE_CanMode;              /* '<Root>/EVSE_CanMode' */
  boolean_T MCU_State3ReqChargingEnable;
                                      /* '<Root>/MCU_State3ReqChargingEnable' */
  boolean_T MCU_State3ValidStatus;     /* '<Root>/MCU_State3ValidStatus' */
  uint8_T ErrHdl_ChargingStation;      /* '<Root>/ErrHdl_ChargingStation' */
  boolean_T AuthM_AuthStatus;          /* '<Root>/AuthM_AuthStatus' */
  uint8_T BeforeChargingTestResult;    /* '<Root>/BeforeChargingTestResult' */
  boolean_T EVSE_CanMode1;             /* '<Root>/EVSE_CanMode1' */
  boolean_T MCU_State3ReqChargingEnable1;
                                     /* '<Root>/MCU_State3ReqChargingEnable1' */
  boolean_T MCU_State3ValidStatus1;    /* '<Root>/MCU_State3ValidStatus1' */
  uint8_T ErrHdl_ChargingStation1;     /* '<Root>/ErrHdl_ChargingStation1' */
  boolean_T AuthM_AuthStatus1;         /* '<Root>/AuthM_AuthStatus1' */
  uint8_T BeforeChargingTestResult1;   /* '<Root>/BeforeChargingTestResult1' */
} ExtU_CanM_EVSEM_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint8_T ChargingCanFault;            /* '<Root>/ChargingCanFault' */
  uint8_T ReqRelaySelfTest;            /* '<Root>/ReqRelaySelfTest' */
  uint8_T ReqRelayOnStatus;            /* '<Root>/ReqRelayOnStatus' */
  uint8_T CanModeStatus;               /* '<Root>/CanModeStatus' */
  uint8_T ChargingCanFault1;           /* '<Root>/ChargingCanFault1' */
  uint8_T ReqRelaySelfTest1;           /* '<Root>/ReqRelaySelfTest1' */
  uint8_T ReqRelayOnStatus1;           /* '<Root>/ReqRelayOnStatus1' */
  uint8_T CanModeStatus1;              /* '<Root>/CanModeStatus1' */
} ExtY_CanM_EVSEM_T;

/* Block states (default storage) */
extern DW_CanM_EVSEM_T CanM_EVSEM_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_CanM_EVSEM_T CanM_EVSEM_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_CanM_EVSEM_T CanM_EVSEM_Y;

/* Model entry point functions */
extern void CanM_EVSEM_initialize(void);
extern void CanM_EVSEM_step(void);
extern void CanM_EVSEM_terminate(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'CanM_EVSEM'
 * '<S1>'   : 'CanM_EVSEM/Enabled EVSE_CHARGING_CAN_MODE'
 * '<S2>'   : 'CanM_EVSEM/Enabled EVSE_CHARGING_CAN_MODE1'
 * '<S3>'   : 'CanM_EVSEM/Enabled EVSE_CHARGING_CAN_MODE/Evse_CanCtrl_Logic'
 * '<S4>'   : 'CanM_EVSEM/Enabled EVSE_CHARGING_CAN_MODE1/Evse_CanCtrl_Logic'
 */
#endif                                 /* RTW_HEADER_CanM_EVSEM_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
