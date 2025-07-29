/*
 * File: CanM_MsgM.h
 *
 * Code generated for Simulink model 'CanM_MsgM'.
 *
 * Model version                  : 1.140
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Tue Jul 29 10:19:03 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CanM_MsgM_h_
#define RTW_HEADER_CanM_MsgM_h_
#ifndef CanM_MsgM_COMMON_INCLUDES_
#define CanM_MsgM_COMMON_INCLUDES_
#include <math.h>
#include "rtwtypes.h"
#include "can_message.h"
#endif                                 /* CanM_MsgM_COMMON_INCLUDES_ */

#include "CanM_MsgM_types.h"
#include <stddef.h>
#include <string.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals for system '<Root>/MCU_Status3_Unpack' */
typedef struct {
  CAN_MESSAGE_BUS BusCreator1;         /* '<S1>/Bus Creator1' */
  real_T CANUnpack_o1;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o2;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o3;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o4;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o5;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o6;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o7;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o8;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o9;                 /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o10;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o11;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o12;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o13;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o14;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o15;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o16;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o17;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o18;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o19;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o20;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o21;                /* '<S1>/CAN Unpack' */
  real_T CANUnpack_o22;                /* '<S1>/CAN Unpack' */
} B_MCU_Status3_Unpack_CanM_Msg_T;

/* Block states (default storage) for system '<Root>/MCU_Status3_Unpack' */
typedef struct {
  int_T CANUnpack_ModeSignalID;        /* '<S1>/CAN Unpack' */
  int_T CANUnpack_StatusPortID;        /* '<S1>/CAN Unpack' */
  uint8_T Delay_DSTATE;                /* '<S7>/Delay' */
  boolean_T UnitDelay_DSTATE;          /* '<S15>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_m;        /* '<S16>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_o;        /* '<S17>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_a;        /* '<S22>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_h;        /* '<S20>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_c;        /* '<S19>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_hf;       /* '<S21>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_mj;       /* '<S18>/Unit Delay' */
} DW_MCU_Status3_Unpack_CanM_Ms_T;

/* Block signals for system '<Root>/SECC_MSG1' */
typedef struct {
  CAN_MESSAGE_BUS SECC_MSG1Pack;       /* '<S3>/SECC_MSG1 Pack' */
  int16_T DataTypeConversion8;         /* '<S3>/Data Type Conversion8' */
  int16_T DataTypeConversion9;         /* '<S3>/Data Type Conversion9' */
  int16_T DataTypeConversion10;        /* '<S3>/Data Type Conversion10' */
  uint8_T Output;                      /* '<S74>/Output' */
} B_SECC_MSG1_CanM_MsgM_T;

/* Block states (default storage) for system '<Root>/SECC_MSG1' */
typedef struct {
  int_T SECC_MSG1Pack_ModeSignalID;    /* '<S3>/SECC_MSG1 Pack' */
  uint8_T Output_DSTATE;               /* '<S74>/Output' */
} DW_SECC_MSG1_CanM_MsgM_T;

/* Block signals (default storage) */
typedef struct {
  CAN_MESSAGE_BUS SECC_MSG2Pack;       /* '<S6>/SECC_MSG2 Pack' */
  CAN_MESSAGE_BUS SECC_MSG2Pack_d;     /* '<S5>/SECC_MSG2 Pack' */
  int16_T DataTypeConversion8;         /* '<S6>/Data Type Conversion8' */
  int16_T DataTypeConversion8_i;       /* '<S5>/Data Type Conversion8' */
  uint8_T Output;                      /* '<S125>/Output' */
  uint8_T Output_h;                    /* '<S108>/Output' */
  B_SECC_MSG1_CanM_MsgM_T SECC_MSG1_1; /* '<Root>/SECC_MSG1_1' */
  B_SECC_MSG1_CanM_MsgM_T SECC_MSG1;   /* '<Root>/SECC_MSG1' */
  B_MCU_Status3_Unpack_CanM_Msg_T MCU_Status3_Unpack1;/* '<Root>/MCU_Status3_Unpack1' */
  B_MCU_Status3_Unpack_CanM_Msg_T MCU_Status3_Unpack;/* '<Root>/MCU_Status3_Unpack' */
} B_CanM_MsgM_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int_T SECC_MSG2Pack_ModeSignalID;    /* '<S6>/SECC_MSG2 Pack' */
  int_T SECC_MSG2Pack_ModeSignalID_d;  /* '<S5>/SECC_MSG2 Pack' */
  uint8_T Output_DSTATE;               /* '<S125>/Output' */
  uint8_T Output_DSTATE_m;             /* '<S108>/Output' */
  DW_SECC_MSG1_CanM_MsgM_T SECC_MSG1_1;/* '<Root>/SECC_MSG1_1' */
  DW_SECC_MSG1_CanM_MsgM_T SECC_MSG1;  /* '<Root>/SECC_MSG1' */
  DW_MCU_Status3_Unpack_CanM_Ms_T MCU_Status3_Unpack1;/* '<Root>/MCU_Status3_Unpack1' */
  DW_MCU_Status3_Unpack_CanM_Ms_T MCU_Status3_Unpack;/* '<Root>/MCU_Status3_Unpack' */
} DW_CanM_MsgM_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  uint64_T MCU_Status3_Data;           /* '<Root>/MCU_Status3_Data' */
  boolean_T SECC_MSG_Enable;           /* '<Root>/SECC_MSG_Enable' */
  uint16_T SECC_MSG1_L1_Curr;          /* '<Root>/SECC_MSG1_L1_Curr' */
  uint16_T SECC_MSG1_L1_Vol;           /* '<Root>/SECC_MSG1_L1_Vol' */
  uint16_T SECC_MSG1_L2_Curr;          /* '<Root>/SECC_MSG1_L2_Curr' */
  uint16_T SECC_MSG1_L2_Vol;           /* '<Root>/SECC_MSG1_L2_Vol' */
  uint16_T SECC_MSG1_L3_Curr;          /* '<Root>/SECC_MSG1_L3_Curr' */
  uint16_T SECC_MSG1_L3_Vol;           /* '<Root>/SECC_MSG1_L3_Vol' */
  uint8_T EVSE_ChargingEndReason;      /* '<Root>/EVSE_ChargingEndReason' */
  uint8_T EVSE_OverCurrFault;          /* '<Root>/EVSE_OverCurrFault' */
  uint8_T EVSE_EmergeStopFault;        /* '<Root>/EVSE_EmergeStopFault' */
  uint8_T EVSE_HardwareFault;          /* '<Root>/EVSE_HardwareFault' */
  uint8_T EVSE_OtherFault;             /* '<Root>/EVSE_OtherFault' */
  uint8_T EVSE_OverTempFault;          /* '<Root>/EVSE_OverTempFault' */
  uint8_T EVSE_CpVolFault;             /* '<Root>/EVSE_CpVolFault' */
  uint8_T EVSE_ChargingParamMismatch;  /* '<Root>/EVSE_ChargingParamMismatch' */
  uint16_T EVSE_RatedCurrent;          /* '<Root>/EVSE_RatedCurrent' */
  uint16_T EVSE_RatedVoltage;          /* '<Root>/EVSE_RatedVoltage' */
  uint8_T EVSE_EnergyCannotTransfer;   /* '<Root>/EVSE_EnergyCannotTransfer' */
  uint8_T EVSE_SelfTestFault;          /* '<Root>/EVSE_SelfTestFault' */
  uint8_T EVSE_SysStatus;              /* '<Root>/EVSE_SysStatus' */
  uint8_T EVSE_CanTimeout;             /* '<Root>/EVSE_CanTimeout' */
  uint8_T EVSE_RequestVehicleMode;     /* '<Root>/EVSE_RequestVehicleMode' */
  uint8_T EVSE_VoltageFault;           /* '<Root>/EVSE_VoltageFault' */
  uint64_T MCU_Status3_Data1;          /* '<Root>/MCU_Status3_Data1' */
  boolean_T SECC_MSG_Enable1;          /* '<Root>/SECC_MSG_Enable1' */
  uint16_T SECC_MSG1_L1_Curr1;         /* '<Root>/SECC_MSG1_L1_Curr1' */
  uint16_T SECC_MSG1_L1_Vol1;          /* '<Root>/SECC_MSG1_L1_Vol1' */
  uint16_T SECC_MSG1_L2_Curr1;         /* '<Root>/SECC_MSG1_L2_Curr1' */
  uint16_T SECC_MSG1_L2_Vol1;          /* '<Root>/SECC_MSG1_L2_Vol1' */
  uint16_T SECC_MSG1_L3_Curr1;         /* '<Root>/SECC_MSG1_L3_Curr1' */
  uint16_T SECC_MSG1_L3_Vol1;          /* '<Root>/SECC_MSG1_L3_Vol1' */
  uint8_T EVSE_ChargingEndReason1;     /* '<Root>/EVSE_ChargingEndReason1' */
  uint8_T EVSE_OverCurrFault1;         /* '<Root>/EVSE_OverCurrFault1' */
  uint8_T EVSE_EmergeStopFault1;       /* '<Root>/EVSE_EmergeStopFault1' */
  uint8_T EVSE_RelayFault1;            /* '<Root>/EVSE_RelayFault1' */
  uint8_T EVSE_HardwareFault1;         /* '<Root>/EVSE_HardwareFault1' */
  uint8_T EVSE_OtherFault1;            /* '<Root>/EVSE_OtherFault1' */
  uint8_T EVSE_OverTempFault1;         /* '<Root>/EVSE_OverTempFault1' */
  uint8_T EVSE_CpVolFault1;            /* '<Root>/EVSE_CpVolFault1' */
  uint8_T EVSE_ChargingParamMismatch1;/* '<Root>/EVSE_ChargingParamMismatch1' */
  uint16_T EVSE_RatedCurrent1;         /* '<Root>/EVSE_RatedCurrent1' */
  uint16_T EVSE_RatedVoltage1;         /* '<Root>/EVSE_RatedVoltage1' */
  uint8_T EVSE_EnergyCannotTransfer1;  /* '<Root>/EVSE_EnergyCannotTransfer1' */
  uint8_T EVSE_SelfTestFault1;         /* '<Root>/EVSE_SelfTestFault1' */
  uint8_T EVSE_SysStatus1;             /* '<Root>/EVSE_SysStatus1' */
  uint8_T EVSE_CanTimeout1;            /* '<Root>/EVSE_CanTimeout1' */
  uint8_T EVSE_RequestVehicleMode1;    /* '<Root>/EVSE_RequestVehicleMode1' */
  uint8_T EVSE_VoltageFault1;          /* '<Root>/EVSE_VoltageFault1' */
  uint8_T EVSE_RelayFault;             /* '<Root>/EVSE_RelayFault' */
  uint8_T EVSE_Relay_Status;           /* '<Root>/EVSE_Relay_Status' */
  uint8_T EVSE_Relay_Status1;          /* '<Root>/EVSE_Relay_Status1' */
  boolean_T Mcu_Status3_Enable;        /* '<Root>/Mcu_Status3_Enable' */
  boolean_T Mcu_Status3_Enable1;       /* '<Root>/Mcu_Status3_Enable1' */
} ExtU_CanM_MsgM_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  boolean_T MCU_State3ReqChargingEnable;
                                      /* '<Root>/MCU_State3ReqChargingEnable' */
  boolean_T MCU_State3ValidStatus;     /* '<Root>/MCU_State3ValidStatus' */
  uint64_T SECC_MSG1_OutU64;           /* '<Root>/SECC_MSG1_OutU64' */
  uint64_T SECC_MSG2_OutU64;           /* '<Root>/SECC_MSG2_OutU64' */
  boolean_T MCU_State3ReqChargingEnable1;
                                     /* '<Root>/MCU_State3ReqChargingEnable1' */
  boolean_T MCU_State3ValidStatus1;    /* '<Root>/MCU_State3ValidStatus1' */
  uint64_T SECC_MSG1_OutU64_1;         /* '<Root>/SECC_MSG1_OutU64_1' */
  uint64_T SECC_MSG2_OutU64_1;         /* '<Root>/SECC_MSG2_OutU64_1' */
} ExtY_CanM_MsgM_T;

/* Real-time Model Data Structure */
struct tag_RTM_CanM_MsgM_T {
  const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_CanM_MsgM_T CanM_MsgM_B;

/* Block states (default storage) */
extern DW_CanM_MsgM_T CanM_MsgM_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_CanM_MsgM_T CanM_MsgM_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_CanM_MsgM_T CanM_MsgM_Y;

/* Model entry point functions */
extern void CanM_MsgM_initialize(void);
extern void CanM_MsgM_step(void);
extern void CanM_MsgM_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CanM_MsgM_T *const CanM_MsgM_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S74>/Data Type Propagation' : Unused code path elimination
 * Block '<S82>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S83>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S91>/Data Type Propagation' : Unused code path elimination
 * Block '<S99>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S100>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S108>/Data Type Propagation' : Unused code path elimination
 * Block '<S116>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S117>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S125>/Data Type Propagation' : Unused code path elimination
 * Block '<S133>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S134>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S1>/u64' : Eliminate redundant data type conversion
 * Block '<S2>/u64' : Eliminate redundant data type conversion
 */

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
 * '<Root>' : 'CanM_MsgM'
 * '<S1>'   : 'CanM_MsgM/MCU_Status3_Unpack'
 * '<S2>'   : 'CanM_MsgM/MCU_Status3_Unpack1'
 * '<S3>'   : 'CanM_MsgM/SECC_MSG1'
 * '<S4>'   : 'CanM_MsgM/SECC_MSG1_1'
 * '<S5>'   : 'CanM_MsgM/SECC_MSG2'
 * '<S6>'   : 'CanM_MsgM/SECC_MSG2_1'
 * '<S7>'   : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy'
 * '<S8>'   : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8'
 * '<S9>'   : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Compare To Constant'
 * '<S10>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Compare To Constant1'
 * '<S11>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Compare To Constant2'
 * '<S12>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Compare To Constant3'
 * '<S13>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem1'
 * '<S14>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3'
 * '<S15>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem'
 * '<S16>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem2'
 * '<S17>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem4'
 * '<S18>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem'
 * '<S19>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem1'
 * '<S20>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem2'
 * '<S21>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem3'
 * '<S22>'  : 'CanM_MsgM/MCU_Status3_Unpack/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem4'
 * '<S23>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift'
 * '<S24>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift1'
 * '<S25>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift2'
 * '<S26>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift3'
 * '<S27>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift4'
 * '<S28>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift5'
 * '<S29>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift6'
 * '<S30>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift/bit_shift'
 * '<S31>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift1/bit_shift'
 * '<S32>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift2/bit_shift'
 * '<S33>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift3/bit_shift'
 * '<S34>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift4/bit_shift'
 * '<S35>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift5/bit_shift'
 * '<S36>'  : 'CanM_MsgM/MCU_Status3_Unpack/U64ToU8/Bit Shift6/bit_shift'
 * '<S37>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy'
 * '<S38>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8'
 * '<S39>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Compare To Constant'
 * '<S40>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Compare To Constant1'
 * '<S41>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Compare To Constant2'
 * '<S42>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Compare To Constant3'
 * '<S43>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem1'
 * '<S44>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3'
 * '<S45>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem'
 * '<S46>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem2'
 * '<S47>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem1/Subsystem4'
 * '<S48>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem'
 * '<S49>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem1'
 * '<S50>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem2'
 * '<S51>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem3'
 * '<S52>'  : 'CanM_MsgM/MCU_Status3_Unpack1/MCU_Status3_Valid_Judgy/Subsystem3/Subsystem4'
 * '<S53>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift'
 * '<S54>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift1'
 * '<S55>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift2'
 * '<S56>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift3'
 * '<S57>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift4'
 * '<S58>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift5'
 * '<S59>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift6'
 * '<S60>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift/bit_shift'
 * '<S61>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift1/bit_shift'
 * '<S62>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift2/bit_shift'
 * '<S63>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift3/bit_shift'
 * '<S64>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift4/bit_shift'
 * '<S65>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift5/bit_shift'
 * '<S66>'  : 'CanM_MsgM/MCU_Status3_Unpack1/U64ToU8/Bit Shift6/bit_shift'
 * '<S67>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift'
 * '<S68>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift1'
 * '<S69>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift2'
 * '<S70>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift3'
 * '<S71>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift4'
 * '<S72>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift5'
 * '<S73>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift6'
 * '<S74>'  : 'CanM_MsgM/SECC_MSG1/Counter Limited'
 * '<S75>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift/bit_shift'
 * '<S76>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift1/bit_shift'
 * '<S77>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift2/bit_shift'
 * '<S78>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift3/bit_shift'
 * '<S79>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift4/bit_shift'
 * '<S80>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift5/bit_shift'
 * '<S81>'  : 'CanM_MsgM/SECC_MSG1/Bit Shift6/bit_shift'
 * '<S82>'  : 'CanM_MsgM/SECC_MSG1/Counter Limited/Increment Real World'
 * '<S83>'  : 'CanM_MsgM/SECC_MSG1/Counter Limited/Wrap To Zero'
 * '<S84>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift'
 * '<S85>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift1'
 * '<S86>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift2'
 * '<S87>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift3'
 * '<S88>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift4'
 * '<S89>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift5'
 * '<S90>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift6'
 * '<S91>'  : 'CanM_MsgM/SECC_MSG1_1/Counter Limited'
 * '<S92>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift/bit_shift'
 * '<S93>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift1/bit_shift'
 * '<S94>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift2/bit_shift'
 * '<S95>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift3/bit_shift'
 * '<S96>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift4/bit_shift'
 * '<S97>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift5/bit_shift'
 * '<S98>'  : 'CanM_MsgM/SECC_MSG1_1/Bit Shift6/bit_shift'
 * '<S99>'  : 'CanM_MsgM/SECC_MSG1_1/Counter Limited/Increment Real World'
 * '<S100>' : 'CanM_MsgM/SECC_MSG1_1/Counter Limited/Wrap To Zero'
 * '<S101>' : 'CanM_MsgM/SECC_MSG2/Bit Shift'
 * '<S102>' : 'CanM_MsgM/SECC_MSG2/Bit Shift1'
 * '<S103>' : 'CanM_MsgM/SECC_MSG2/Bit Shift2'
 * '<S104>' : 'CanM_MsgM/SECC_MSG2/Bit Shift3'
 * '<S105>' : 'CanM_MsgM/SECC_MSG2/Bit Shift4'
 * '<S106>' : 'CanM_MsgM/SECC_MSG2/Bit Shift5'
 * '<S107>' : 'CanM_MsgM/SECC_MSG2/Bit Shift6'
 * '<S108>' : 'CanM_MsgM/SECC_MSG2/Counter Limited'
 * '<S109>' : 'CanM_MsgM/SECC_MSG2/Bit Shift/bit_shift'
 * '<S110>' : 'CanM_MsgM/SECC_MSG2/Bit Shift1/bit_shift'
 * '<S111>' : 'CanM_MsgM/SECC_MSG2/Bit Shift2/bit_shift'
 * '<S112>' : 'CanM_MsgM/SECC_MSG2/Bit Shift3/bit_shift'
 * '<S113>' : 'CanM_MsgM/SECC_MSG2/Bit Shift4/bit_shift'
 * '<S114>' : 'CanM_MsgM/SECC_MSG2/Bit Shift5/bit_shift'
 * '<S115>' : 'CanM_MsgM/SECC_MSG2/Bit Shift6/bit_shift'
 * '<S116>' : 'CanM_MsgM/SECC_MSG2/Counter Limited/Increment Real World'
 * '<S117>' : 'CanM_MsgM/SECC_MSG2/Counter Limited/Wrap To Zero'
 * '<S118>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift'
 * '<S119>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift1'
 * '<S120>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift2'
 * '<S121>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift3'
 * '<S122>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift4'
 * '<S123>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift5'
 * '<S124>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift6'
 * '<S125>' : 'CanM_MsgM/SECC_MSG2_1/Counter Limited'
 * '<S126>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift/bit_shift'
 * '<S127>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift1/bit_shift'
 * '<S128>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift2/bit_shift'
 * '<S129>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift3/bit_shift'
 * '<S130>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift4/bit_shift'
 * '<S131>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift5/bit_shift'
 * '<S132>' : 'CanM_MsgM/SECC_MSG2_1/Bit Shift6/bit_shift'
 * '<S133>' : 'CanM_MsgM/SECC_MSG2_1/Counter Limited/Increment Real World'
 * '<S134>' : 'CanM_MsgM/SECC_MSG2_1/Counter Limited/Wrap To Zero'
 */
#endif                                 /* RTW_HEADER_CanM_MsgM_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
