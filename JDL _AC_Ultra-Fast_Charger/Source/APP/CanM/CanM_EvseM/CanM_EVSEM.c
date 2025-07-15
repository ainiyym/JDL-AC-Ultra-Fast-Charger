/*
 * File: CanM_EVSEM.c
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

#include "CanM_EVSEM.h"
#include "rtwtypes.h"

/* Named constants for Chart: '<S1>/Evse_CanCtrl_Logic' */
#define CanM_EVSEM_IN_AuthMode         ((uint8_T)1U)
#define CanM_EVSEM_IN_ChargingMode     ((uint8_T)2U)
#define CanM_EVSEM_IN_ErrMode          ((uint8_T)3U)
#define CanM_EVSEM_IN_InitMdoe         ((uint8_T)4U)
#define CanM_EVSEM_IN_ReadyMdoe        ((uint8_T)5U)

/* Block states (default storage) */
DW_CanM_EVSEM_T CanM_EVSEM_DW;

/* External inputs (root inport signals with default storage) */
ExtU_CanM_EVSEM_T CanM_EVSEM_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_CanM_EVSEM_T CanM_EVSEM_Y;

/* Model step function */
void CanM_EVSEM_step(void)
{
  int32_T tmp;
  boolean_T guard1;

  /* Outputs for Enabled SubSystem: '<Root>/Enabled EVSE_CHARGING_CAN_MODE' incorporates:
   *  EnablePort: '<S1>/Enable'
   */
  /* Inport: '<Root>/EVSE_CanMode' */
  if (CanM_EVSEM_U.EVSE_CanMode) {
    /* Chart: '<S1>/Evse_CanCtrl_Logic' incorporates:
     *  Inport: '<Root>/AuthM_AuthStatus'
     *  Inport: '<Root>/BeforeChargingTestResult'
     *  Inport: '<Root>/ErrHdl_ChargingStation'
     *  Inport: '<Root>/MCU_State3ReqChargingEnable'
     *  Inport: '<Root>/MCU_State3ValidStatus'
     */
    if (CanM_EVSEM_DW.is_active_c3_CanM_EVSEM == 0U) {
      CanM_EVSEM_DW.is_active_c3_CanM_EVSEM = 1U;
      CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
      CanM_EVSEM_DW.Evse_CanCtrlCnt = 0U;
    } else {
      guard1 = false;
      switch (CanM_EVSEM_DW.is_c3_CanM_EVSEM) {
       case CanM_EVSEM_IN_AuthMode:
        if ((CanM_EVSEM_U.ErrHdl_ChargingStation < 2U) &&
            (CanM_EVSEM_U.BeforeChargingTestResult ==
             CHARGING_BEFORE_TEST_STATUS_ENUM_SUCCESS) &&
            CanM_EVSEM_U.MCU_State3ReqChargingEnable) {
          /* Outport: '<Root>/ReqRelaySelfTest' */
          CanM_EVSEM_Y.ReqRelaySelfTest = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ChargingMode;

          /* Outport: '<Root>/ReqRelayOnStatus' */
          CanM_EVSEM_Y.ReqRelayOnStatus = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;

          /* Outport: '<Root>/ChargingCanFault' */
          CanM_EVSEM_Y.ChargingCanFault = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        } else if ((!CanM_EVSEM_U.MCU_State3ValidStatus) ||
                   (CanM_EVSEM_U.BeforeChargingTestResult ==
                    CHARGING_BEFORE_TEST_STATUS_ENUM_FAIL) ||
                   (CanM_EVSEM_U.ErrHdl_ChargingStation >= 2U)) {
          /* Outport: '<Root>/ReqRelaySelfTest' */
          CanM_EVSEM_Y.ReqRelaySelfTest = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
        } else if (!CanM_EVSEM_U.AuthM_AuthStatus) {
          /* Outport: '<Root>/ReqRelaySelfTest' */
          CanM_EVSEM_Y.ReqRelaySelfTest = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ReadyMdoe;

          /* Outport: '<Root>/CanModeStatus' */
          CanM_EVSEM_Y.CanModeStatus = (uint8_T)CAN_MODE_STATUS_ENUM_ENTER;

          /* Outport: '<Root>/ChargingCanFault' */
          CanM_EVSEM_Y.ChargingCanFault = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        }
        break;

       case CanM_EVSEM_IN_ChargingMode:
        if (!CanM_EVSEM_U.MCU_State3ValidStatus) {
          /* Outport: '<Root>/ChargingCanFault' */
          CanM_EVSEM_Y.ChargingCanFault = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
          guard1 = true;
        } else if (CanM_EVSEM_U.ErrHdl_ChargingStation >= 2U) {
          guard1 = true;
        } else if ((!CanM_EVSEM_U.MCU_State3ReqChargingEnable) ||
                   (!CanM_EVSEM_U.AuthM_AuthStatus)) {
          /* Outport: '<Root>/ReqRelayOnStatus' */
          CanM_EVSEM_Y.ReqRelayOnStatus = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_AuthMode;

          /* Outport: '<Root>/ReqRelaySelfTest' */
          CanM_EVSEM_Y.ReqRelaySelfTest = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
        }
        break;

       case CanM_EVSEM_IN_ErrMode:
        /* Outport: '<Root>/CanModeStatus' */
        CanM_EVSEM_Y.CanModeStatus = (uint8_T)CAN_MODE_STATUS_ENUM_EXIT;
        CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
        CanM_EVSEM_DW.Evse_CanCtrlCnt = 0U;
        break;

       case CanM_EVSEM_IN_InitMdoe:
        if (CanM_EVSEM_U.MCU_State3ValidStatus) {
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ReadyMdoe;

          /* Outport: '<Root>/CanModeStatus' */
          CanM_EVSEM_Y.CanModeStatus = (uint8_T)CAN_MODE_STATUS_ENUM_ENTER;

          /* Outport: '<Root>/ChargingCanFault' */
          CanM_EVSEM_Y.ChargingCanFault = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        } else if (CanM_EVSEM_DW.Evse_CanCtrlCnt == 5) {
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
        } else {
          tmp = CanM_EVSEM_DW.Evse_CanCtrlCnt + 1;
          if (CanM_EVSEM_DW.Evse_CanCtrlCnt + 1 > 255) {
            tmp = 255;
          }

          CanM_EVSEM_DW.Evse_CanCtrlCnt = (uint8_T)tmp;
        }
        break;

       default:
        /* case IN_ReadyMdoe: */
        if (CanM_EVSEM_U.AuthM_AuthStatus) {
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_AuthMode;

          /* Outport: '<Root>/ReqRelaySelfTest' */
          CanM_EVSEM_Y.ReqRelaySelfTest = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
        } else if (!CanM_EVSEM_U.MCU_State3ValidStatus) {
          CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
          CanM_EVSEM_DW.Evse_CanCtrlCnt = 0U;
        }
        break;
      }

      if (guard1) {
        /* Outport: '<Root>/ReqRelayOnStatus' */
        CanM_EVSEM_Y.ReqRelayOnStatus = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        CanM_EVSEM_DW.is_c3_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
      }
    }

    /* End of Chart: '<S1>/Evse_CanCtrl_Logic' */
  }

  /* End of Inport: '<Root>/EVSE_CanMode' */
  /* End of Outputs for SubSystem: '<Root>/Enabled EVSE_CHARGING_CAN_MODE' */

  /* Outputs for Enabled SubSystem: '<Root>/Enabled EVSE_CHARGING_CAN_MODE1' incorporates:
   *  EnablePort: '<S2>/Enable'
   */
  /* Inport: '<Root>/EVSE_CanMode1' */
  if (CanM_EVSEM_U.EVSE_CanMode1) {
    /* Chart: '<S2>/Evse_CanCtrl_Logic' incorporates:
     *  Inport: '<Root>/AuthM_AuthStatus1'
     *  Inport: '<Root>/BeforeChargingTestResult1'
     *  Inport: '<Root>/ErrHdl_ChargingStation1'
     *  Inport: '<Root>/MCU_State3ReqChargingEnable1'
     *  Inport: '<Root>/MCU_State3ValidStatus1'
     */
    if (CanM_EVSEM_DW.is_active_c1_CanM_EVSEM == 0U) {
      CanM_EVSEM_DW.is_active_c1_CanM_EVSEM = 1U;
      CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
      CanM_EVSEM_DW.Evse_CanCtrlCnt1 = 0U;
    } else {
      guard1 = false;
      switch (CanM_EVSEM_DW.is_c1_CanM_EVSEM) {
       case CanM_EVSEM_IN_AuthMode:
        if ((CanM_EVSEM_U.ErrHdl_ChargingStation1 < 2U) &&
            (CanM_EVSEM_U.BeforeChargingTestResult1 ==
             CHARGING_BEFORE_TEST_STATUS_ENUM_SUCCESS) &&
            CanM_EVSEM_U.MCU_State3ReqChargingEnable1) {
          /* Outport: '<Root>/ReqRelaySelfTest1' */
          CanM_EVSEM_Y.ReqRelaySelfTest1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ChargingMode;

          /* Outport: '<Root>/ReqRelayOnStatus1' */
          CanM_EVSEM_Y.ReqRelayOnStatus1 = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;

          /* Outport: '<Root>/ChargingCanFault1' */
          CanM_EVSEM_Y.ChargingCanFault1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        } else if ((!CanM_EVSEM_U.MCU_State3ValidStatus1) ||
                   (CanM_EVSEM_U.BeforeChargingTestResult1 ==
                    CHARGING_BEFORE_TEST_STATUS_ENUM_FAIL) ||
                   (CanM_EVSEM_U.ErrHdl_ChargingStation1 >= 2U)) {
          /* Outport: '<Root>/ReqRelaySelfTest1' */
          CanM_EVSEM_Y.ReqRelaySelfTest1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
        } else if (!CanM_EVSEM_U.AuthM_AuthStatus1) {
          /* Outport: '<Root>/ReqRelaySelfTest1' */
          CanM_EVSEM_Y.ReqRelaySelfTest1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ReadyMdoe;

          /* Outport: '<Root>/CanModeStatus1' */
          CanM_EVSEM_Y.CanModeStatus1 = (uint8_T)CAN_MODE_STATUS_ENUM_ENTER;

          /* Outport: '<Root>/ChargingCanFault1' */
          CanM_EVSEM_Y.ChargingCanFault1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        }
        break;

       case CanM_EVSEM_IN_ChargingMode:
        if (!CanM_EVSEM_U.MCU_State3ValidStatus1) {
          /* Outport: '<Root>/ChargingCanFault1' */
          CanM_EVSEM_Y.ChargingCanFault1 = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
          guard1 = true;
        } else if (CanM_EVSEM_U.ErrHdl_ChargingStation1 >= 2U) {
          guard1 = true;
        } else if ((!CanM_EVSEM_U.MCU_State3ReqChargingEnable1) ||
                   (!CanM_EVSEM_U.AuthM_AuthStatus1)) {
          /* Outport: '<Root>/ReqRelayOnStatus1' */
          CanM_EVSEM_Y.ReqRelayOnStatus1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_AuthMode;

          /* Outport: '<Root>/ReqRelaySelfTest1' */
          CanM_EVSEM_Y.ReqRelaySelfTest1 = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
        }
        break;

       case CanM_EVSEM_IN_ErrMode:
        /* Outport: '<Root>/CanModeStatus1' */
        CanM_EVSEM_Y.CanModeStatus1 = (uint8_T)CAN_MODE_STATUS_ENUM_EXIT;
        CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
        CanM_EVSEM_DW.Evse_CanCtrlCnt1 = 0U;
        break;

       case CanM_EVSEM_IN_InitMdoe:
        if (CanM_EVSEM_U.MCU_State3ValidStatus1) {
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ReadyMdoe;

          /* Outport: '<Root>/CanModeStatus1' */
          CanM_EVSEM_Y.CanModeStatus1 = (uint8_T)CAN_MODE_STATUS_ENUM_ENTER;

          /* Outport: '<Root>/ChargingCanFault1' */
          CanM_EVSEM_Y.ChargingCanFault1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        } else if (CanM_EVSEM_DW.Evse_CanCtrlCnt1 == 5) {
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
        } else {
          tmp = CanM_EVSEM_DW.Evse_CanCtrlCnt1 + 1;
          if (CanM_EVSEM_DW.Evse_CanCtrlCnt1 + 1 > 255) {
            tmp = 255;
          }

          CanM_EVSEM_DW.Evse_CanCtrlCnt1 = (uint8_T)tmp;
        }
        break;

       default:
        /* case IN_ReadyMdoe: */
        if (CanM_EVSEM_U.AuthM_AuthStatus1) {
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_AuthMode;

          /* Outport: '<Root>/ReqRelaySelfTest1' */
          CanM_EVSEM_Y.ReqRelaySelfTest1 = (uint8_T)ENABLE_STATUS_ENUM_ENABLE;
        } else if (!CanM_EVSEM_U.MCU_State3ValidStatus1) {
          CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_InitMdoe;
          CanM_EVSEM_DW.Evse_CanCtrlCnt1 = 0U;
        }
        break;
      }

      if (guard1) {
        /* Outport: '<Root>/ReqRelayOnStatus1' */
        CanM_EVSEM_Y.ReqRelayOnStatus1 = (uint8_T)ENABLE_STATUS_ENUM_DISABLE;
        CanM_EVSEM_DW.is_c1_CanM_EVSEM = CanM_EVSEM_IN_ErrMode;
      }
    }

    /* End of Chart: '<S2>/Evse_CanCtrl_Logic' */
  }

  /* End of Inport: '<Root>/EVSE_CanMode1' */
  /* End of Outputs for SubSystem: '<Root>/Enabled EVSE_CHARGING_CAN_MODE1' */
}

/* Model initialize function */
void CanM_EVSEM_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void CanM_EVSEM_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
