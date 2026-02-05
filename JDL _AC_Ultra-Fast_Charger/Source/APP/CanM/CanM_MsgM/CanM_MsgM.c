/*
 * File: CanM_MsgM.c
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

#include "CanM_MsgM.h"
#include "rtwtypes.h"
#include "CanM_MsgM_private.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <string.h>

/* Block signals (default storage) */
B_CanM_MsgM_T CanM_MsgM_B;

/* Block states (default storage) */
DW_CanM_MsgM_T CanM_MsgM_DW;

/* External inputs (root inport signals with default storage) */
ExtU_CanM_MsgM_T CanM_MsgM_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_CanM_MsgM_T CanM_MsgM_Y;

/* Real-time model */
static RT_MODEL_CanM_MsgM_T CanM_MsgM_M_;
RT_MODEL_CanM_MsgM_T *const CanM_MsgM_M = &CanM_MsgM_M_;

/*
 * Output and update for atomic system:
 *    '<S8>/Bit Shift'
 *    '<S8>/Bit Shift1'
 *    '<S8>/Bit Shift2'
 *    '<S8>/Bit Shift3'
 *    '<S8>/Bit Shift4'
 *    '<S8>/Bit Shift5'
 *    '<S8>/Bit Shift6'
 *    '<S28>/Bit Shift'
 *    '<S28>/Bit Shift1'
 *    '<S28>/Bit Shift2'
 *    ...
 */
uint64_T CanM_MsgM_BitShift(uint64_T rtu_u)
{
  /* MATLAB Function: '<S13>/bit_shift' */
  return rtu_u >> 8;
}

/*
 * System initialize for enable system:
 *    '<Root>/MCU_Status3_Unpack'
 *    '<Root>/MCU_Status3_Unpack1'
 */
void CanM_Ms_MCU_Status3_Unpack_Init(DW_MCU_Status3_Unpack_CanM_Ms_T *localDW)
{
  /* Start for S-Function (scanunpack): '<S1>/CAN Unpack' */

  /*-----------S-Function Block: <S1>/CAN Unpack -----------------*/

  /* SystemInitialize for MATLAB Function: '<S7>/MATLAB Function' */
  localDW->count_not_empty = false;
}

/*
 * Output and update for enable system:
 *    '<Root>/MCU_Status3_Unpack'
 *    '<Root>/MCU_Status3_Unpack1'
 */
void CanM_MsgM_MCU_Status3_Unpack(boolean_T rtu_Enable, uint64_T
  rtu_MCU_Status3_Data, boolean_T *rty_MCU_State3ValidStatus, boolean_T
  *rty_MCU_State3ReqChargingEnable, B_MCU_Status3_Unpack_CanM_Msg_T *localB,
  DW_MCU_Status3_Unpack_CanM_Ms_T *localDW)
{
  real_T tmp;
  uint64_T rtb_y_m;
  uint64_T rtb_y_o4;
  int32_T qY;
  uint8_T rtb_DataTypeConversion_o;

  /* Outputs for Enabled SubSystem: '<Root>/MCU_Status3_Unpack' incorporates:
   *  EnablePort: '<S1>/Enable'
   */
  if (rtu_Enable) {
    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift' */
    rtb_y_o4 = CanM_MsgM_BitShift(rtu_MCU_Status3_Data);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  DataTypeConversion: '<S8>/Data Type Conversion1'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND1'
     */
    localB->BusCreator1.Data[1] = (uint8_T)(rtb_y_o4 & 255ULL);

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift1' */
    rtb_y_m = CanM_MsgM_BitShift(rtb_y_o4);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift1' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  DataTypeConversion: '<S8>/Data Type Conversion2'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND2'
     */
    localB->BusCreator1.Data[2] = (uint8_T)(rtb_y_m & 255ULL);

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift2' */
    rtb_y_o4 = CanM_MsgM_BitShift(rtb_y_m);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift2' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  DataTypeConversion: '<S8>/Data Type Conversion3'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND3'
     */
    localB->BusCreator1.Data[3] = (uint8_T)(rtb_y_o4 & 255ULL);

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift3' */
    rtb_y_m = CanM_MsgM_BitShift(rtb_y_o4);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift3' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  DataTypeConversion: '<S8>/Data Type Conversion4'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND4'
     */
    localB->BusCreator1.Data[4] = (uint8_T)(rtb_y_m & 255ULL);

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift4' */
    rtb_y_o4 = CanM_MsgM_BitShift(rtb_y_m);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift4' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  DataTypeConversion: '<S8>/Data Type Conversion5'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND5'
     */
    localB->BusCreator1.Data[5] = (uint8_T)(rtb_y_o4 & 255ULL);

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift5' */
    rtb_y_m = CanM_MsgM_BitShift(rtb_y_o4);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift5' */

    /* Outputs for Atomic SubSystem: '<S8>/Bit Shift6' */
    rtb_y_o4 = CanM_MsgM_BitShift(rtb_y_m);

    /* End of Outputs for SubSystem: '<S8>/Bit Shift6' */

    /* BusCreator: '<S1>/Bus Creator1' incorporates:
     *  Constant: '<S1>/Constant26'
     *  Constant: '<S1>/Constant27'
     *  Constant: '<S1>/Constant28'
     *  Constant: '<S1>/Constant29'
     *  Constant: '<S1>/Constant3'
     *  Constant: '<S1>/Constant30'
     *  DataTypeConversion: '<S8>/Data Type Conversion'
     *  DataTypeConversion: '<S8>/Data Type Conversion6'
     *  DataTypeConversion: '<S8>/Data Type Conversion7'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND6'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND7'
     */
    localB->BusCreator1.Extended = 1U;
    localB->BusCreator1.Length = 8U;
    localB->BusCreator1.Remote = 0U;
    localB->BusCreator1.Error = 0U;
    localB->BusCreator1.ID = 419369199U;
    localB->BusCreator1.Timestamp = 0.0;
    localB->BusCreator1.Data[0] = (uint8_T)(rtu_MCU_Status3_Data & 255ULL);
    localB->BusCreator1.Data[6] = (uint8_T)(rtb_y_m & 255ULL);
    localB->BusCreator1.Data[7] = (uint8_T)(rtb_y_o4 & 255ULL);

    /* S-Function (scanunpack): '<S1>/CAN Unpack' */
    {
      /* S-Function (scanunpack): '<S1>/CAN Unpack' */
      if ((8 == localB->BusCreator1.Length) && (localB->BusCreator1.ID !=
           INVALID_CAN_ID) ) {
        if ((419369199 == localB->BusCreator1.ID) && (1U ==
             localB->BusCreator1.Extended) ) {
          {
            /* --------------- START Unpacking signal 0 ------------------
             *  startBit                = 60
             *  length                  = 4
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (localB->BusCreator1.Data[7]) & (uint8_T)(0xF0U)) >> 4);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real_T) (unpackedValue);
              }

              {
                real_T result = (real_T) outValue;
                localB->CANUnpack_o1 = result;
              }
            }

            /* --------------- START Unpacking signal 1 ------------------
             *  startBit                = 4
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 2 ------------------
             *  startBit                = 11
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 3 ------------------
             *  startBit                = 10
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 4 ------------------
             *  startBit                = 0
             *  length                  = 4
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 5 ------------------
             *  startBit                = 28
             *  length                  = 2
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 6 ------------------
             *  startBit                = 20
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 7 ------------------
             *  startBit                = 30
             *  length                  = 2
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 8 ------------------
             *  startBit                = 21
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            {
              real_T outValue = 0;

              {
                uint8_T unpackedValue = 0;

                {
                  uint8_T tempValue = (uint8_T) (0);

                  {
                    tempValue = tempValue | (uint8_T)((uint8_T)((uint8_T)
                      (localB->BusCreator1.Data[2]) & (uint8_T)(0x20U)) >> 5);
                  }

                  unpackedValue = tempValue;
                }

                outValue = (real_T) (unpackedValue);
              }

              {
                real_T result = (real_T) outValue;
                localB->CANUnpack_o9 = result;
              }
            }

            /* --------------- START Unpacking signal 9 ------------------
             *  startBit                = 17
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 10 ------------------
             *  startBit                = 12
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 11 ------------------
             *  startBit                = 7
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 12 ------------------
             *  startBit                = 16
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 13 ------------------
             *  startBit                = 6
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 14 ------------------
             *  startBit                = 13
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 15 ------------------
             *  startBit                = 14
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 16 ------------------
             *  startBit                = 19
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 17 ------------------
             *  startBit                = 15
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 18 ------------------
             *  startBit                = 8
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 19 ------------------
             *  startBit                = 5
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 20 ------------------
             *  startBit                = 18
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */

            /* --------------- START Unpacking signal 21 ------------------
             *  startBit                = 9
             *  length                  = 1
             *  desiredSignalByteLayout = LITTLEENDIAN
             *  dataType                = UNSIGNED
             *  factor                  = 1.0
             *  offset                  = 0.0
             * -----------------------------------------------------------------------*/
            /*
             * Signal is not connected or connected to terminator.
             * No unpacking code generated.
             */
          }
        }
      }
    }

    /* DataTypeConversion: '<S1>/Data Type Conversion' */
    tmp = floor(localB->CANUnpack_o1);
    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 256.0);
    }

    rtb_DataTypeConversion_o = (uint8_T)(tmp < 0.0 ? (int32_T)(uint8_T)-(int8_T)
      (uint8_T)-tmp : (int32_T)(uint8_T)tmp);

    /* End of DataTypeConversion: '<S1>/Data Type Conversion' */

    /* MATLAB Function: '<S7>/MATLAB Function' incorporates:
     *  Constant: '<S10>/Constant'
     *  Constant: '<S7>/Constant'
     *  Constant: '<S7>/Constant1'
     *  Constant: '<S9>/Constant'
     *  Logic: '<S7>/Logical Operator'
     *  Logic: '<S7>/Logical Operator1'
     *  RelationalOperator: '<S10>/Compare'
     *  RelationalOperator: '<S7>/Relational Operator'
     *  RelationalOperator: '<S9>/Compare'
     *  Sum: '<S7>/Add'
     *  Sum: '<S7>/Subtract'
     *  UnitDelay: '<S7>/Unit Delay'
     */
    if (!localDW->count_not_empty) {
      localDW->count = 0U;
      localDW->count_not_empty = true;
    }

    if ((((uint8_T)((uint32_T)rtb_DataTypeConversion_o -
                    localDW->UnitDelay_DSTATE) == 1) || ((uint8_T)((uint32_T)
           rtb_DataTypeConversion_o + localDW->UnitDelay_DSTATE) == 15)) &&
        (localDW->UnitDelay_DSTATE != rtb_DataTypeConversion_o)) {
      if (localDW->count < 5) {
        localDW->count++;
      }
    } else if (localDW->count > 0) {
      qY = (int32_T)(localDW->count - /*MW:OvSatOk*/ 1U);
      if (localDW->count - 1U > localDW->count) {
        qY = 0;
      }

      localDW->count = (uint8_T)qY;
    }

    /* RelationalOperator: '<S11>/Compare' incorporates:
     *  Constant: '<S11>/Constant'
     *  MATLAB Function: '<S7>/MATLAB Function'
     */
    *rty_MCU_State3ValidStatus = (localDW->count >= 3);

    /* DataTypeConversion: '<S1>/Data Type Conversion1' */
    *rty_MCU_State3ReqChargingEnable = (localB->CANUnpack_o9 != 0.0);

    /* Update for UnitDelay: '<S7>/Unit Delay' */
    localDW->UnitDelay_DSTATE = rtb_DataTypeConversion_o;
  }

  /* End of Outputs for SubSystem: '<Root>/MCU_Status3_Unpack' */
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift'
 *    '<S4>/Bit Shift'
 *    '<S5>/Bit Shift'
 *    '<S6>/Bit Shift'
 */
uint64_T CanM_MsgM_BitShift_e(uint64_T rtu_u)
{
  /* MATLAB Function: '<S47>/bit_shift' */
  return rtu_u << 8;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift1'
 *    '<S4>/Bit Shift1'
 *    '<S5>/Bit Shift1'
 *    '<S6>/Bit Shift1'
 */
uint64_T CanM_MsgM_BitShift1(uint64_T rtu_u)
{
  /* MATLAB Function: '<S48>/bit_shift' */
  return rtu_u << 16;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift2'
 *    '<S4>/Bit Shift2'
 *    '<S5>/Bit Shift2'
 *    '<S6>/Bit Shift2'
 */
uint64_T CanM_MsgM_BitShift2(uint64_T rtu_u)
{
  /* MATLAB Function: '<S49>/bit_shift' */
  return rtu_u << 24;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift3'
 *    '<S4>/Bit Shift3'
 *    '<S5>/Bit Shift3'
 *    '<S6>/Bit Shift3'
 */
uint64_T CanM_MsgM_BitShift3(uint64_T rtu_u)
{
  /* MATLAB Function: '<S50>/bit_shift' */
  return rtu_u << 32;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift4'
 *    '<S4>/Bit Shift4'
 *    '<S5>/Bit Shift4'
 *    '<S6>/Bit Shift4'
 */
uint64_T CanM_MsgM_BitShift4(uint64_T rtu_u)
{
  /* MATLAB Function: '<S51>/bit_shift' */
  return rtu_u << 40;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift5'
 *    '<S4>/Bit Shift5'
 *    '<S5>/Bit Shift5'
 *    '<S6>/Bit Shift5'
 */
uint64_T CanM_MsgM_BitShift5(uint64_T rtu_u)
{
  /* MATLAB Function: '<S52>/bit_shift' */
  return rtu_u << 48;
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift6'
 *    '<S4>/Bit Shift6'
 *    '<S5>/Bit Shift6'
 *    '<S6>/Bit Shift6'
 */
uint64_T CanM_MsgM_BitShift6(uint64_T rtu_u)
{
  /* MATLAB Function: '<S53>/bit_shift' */
  return rtu_u << 56;
}

/*
 * Output and update for enable system:
 *    '<Root>/SECC_MSG1'
 *    '<Root>/SECC_MSG1_1'
 */
void CanM_MsgM_SECC_MSG1(boolean_T rtu_Enable, uint16_T rtu_SECC_MSG1_L1_Curr,
  uint16_T rtu_SECC_MSG1_L1_Vol, uint16_T rtu_SECC_MSG1_L2_Curr, uint16_T
  rtu_SECC_MSG1_L2_Vol, uint16_T rtu_SECC_MSG1_L3_Curr, uint16_T
  rtu_SECC_MSG1_L3_Vol, uint64_T *rty_SECC_MSG1_OutU64, B_SECC_MSG1_CanM_MsgM_T *
  localB, DW_SECC_MSG1_CanM_MsgM_T *localDW)
{
  uint64_T rtb_y_b;
  uint64_T rtb_y_d0;
  uint64_T rtb_y_es;
  uint64_T rtb_y_g;
  uint64_T rtb_y_jt;
  uint64_T rtb_y_kg;
  uint64_T rtb_y_n;

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG1' incorporates:
   *  EnablePort: '<S3>/Enable'
   */
  if (rtu_Enable) {
    /* UnitDelay: '<S54>/Output' */
    localB->Output = localDW->Output_DSTATE;

    /* Switch: '<S63>/FixPt Switch' incorporates:
     *  Constant: '<S62>/FixPt Constant'
     *  Constant: '<S63>/Constant'
     *  Sum: '<S62>/FixPt Sum1'
     *  UnitDelay: '<S54>/Output'
     */
    if ((uint8_T)(localB->Output + 1U) > 15) {
      localDW->Output_DSTATE = 0U;
    } else {
      localDW->Output_DSTATE = (uint8_T)(localB->Output + 1U);
    }

    /* End of Switch: '<S63>/FixPt Switch' */

    /* DataTypeConversion: '<S3>/Data Type Conversion8' */
    localB->DataTypeConversion8 = (int16_T)rtu_SECC_MSG1_L1_Curr;

    /* DataTypeConversion: '<S3>/Data Type Conversion9' */
    localB->DataTypeConversion9 = (int16_T)rtu_SECC_MSG1_L2_Curr;

    /* DataTypeConversion: '<S3>/Data Type Conversion10' */
    localB->DataTypeConversion10 = (int16_T)rtu_SECC_MSG1_L3_Curr;

    /* S-Function (scanpack): '<S3>/SECC_MSG1 Pack' */
    /* S-Function (scanpack): '<S3>/SECC_MSG1 Pack' */
    localB->SECC_MSG1Pack.ID = 419364950U;
    localB->SECC_MSG1Pack.Length = 8U;
    localB->SECC_MSG1Pack.Extended = 1U;
    localB->SECC_MSG1Pack.Remote = 0;
    localB->SECC_MSG1Pack.Data[0] = 0;
    localB->SECC_MSG1Pack.Data[1] = 0;
    localB->SECC_MSG1Pack.Data[2] = 0;
    localB->SECC_MSG1Pack.Data[3] = 0;
    localB->SECC_MSG1Pack.Data[4] = 0;
    localB->SECC_MSG1Pack.Data[5] = 0;
    localB->SECC_MSG1Pack.Data[6] = 0;
    localB->SECC_MSG1Pack.Data[7] = 0;

    {
      /* --------------- START Packing signal 0 ------------------
       *  startBit                = 60
       *  length                  = 4
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (localB->Output);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(15)) {
            packedValue = (uint8_T) 15;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[7] = localB->SECC_MSG1Pack.Data[7] |
                (uint8_T)((uint8_T)((uint8_T)(packedValue & (uint8_T)0xFU) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 1 ------------------
       *  startBit                = 9
       *  length                  = 11
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = -1000.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        int32_T packingValue = 0;

        {
          int32_T result = (int32_T) (localB->DataTypeConversion8);

          /* no factor to apply */
          result = (int32_T) (result - -1000);
          packingValue = result;
        }

        if (packingValue < 0) {
          packingValue = 0;
        }

        {
          uint16_T packedValue;
          if (packingValue > (int32_T)(2047)) {
            packedValue = (uint16_T) 2047;
          } else if (packingValue < (int32_T)(0)) {
            packedValue = (uint16_T) 0;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[1] = localB->SECC_MSG1Pack.Data[1] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x7FU) <<
                1));
              localB->SECC_MSG1Pack.Data[2] = localB->SECC_MSG1Pack.Data[2] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x780U) >>
                7));
            }
          }
        }
      }

      /* --------------- START Packing signal 2 ------------------
       *  startBit                = 0
       *  length                  = 9
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (rtu_SECC_MSG1_L1_Vol);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint16_T packedValue;
          if (packingValue > (uint16_T)(511)) {
            packedValue = (uint16_T) 511;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[0] = localB->SECC_MSG1Pack.Data[0] |
                (uint8_T)((uint16_T)(packedValue & (uint16_T)0xFFU));
              localB->SECC_MSG1Pack.Data[1] = localB->SECC_MSG1Pack.Data[1] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x100U) >>
                8));
            }
          }
        }
      }

      /* --------------- START Packing signal 3 ------------------
       *  startBit                = 29
       *  length                  = 11
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = -1000.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        int32_T packingValue = 0;

        {
          int32_T result = (int32_T) (localB->DataTypeConversion9);

          /* no factor to apply */
          result = (int32_T) (result - -1000);
          packingValue = result;
        }

        if (packingValue < 0) {
          packingValue = 0;
        }

        {
          uint16_T packedValue;
          if (packingValue > (int32_T)(2047)) {
            packedValue = (uint16_T) 2047;
          } else if (packingValue < (int32_T)(0)) {
            packedValue = (uint16_T) 0;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[3] = localB->SECC_MSG1Pack.Data[3] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x7U) <<
                5));
              localB->SECC_MSG1Pack.Data[4] = localB->SECC_MSG1Pack.Data[4] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x7F8U) >>
                3));
            }
          }
        }
      }

      /* --------------- START Packing signal 4 ------------------
       *  startBit                = 20
       *  length                  = 9
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (rtu_SECC_MSG1_L2_Vol);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint16_T packedValue;
          if (packingValue > (uint16_T)(511)) {
            packedValue = (uint16_T) 511;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[2] = localB->SECC_MSG1Pack.Data[2] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0xFU) <<
                4));
              localB->SECC_MSG1Pack.Data[3] = localB->SECC_MSG1Pack.Data[3] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x1F0U) >>
                4));
            }
          }
        }
      }

      /* --------------- START Packing signal 5 ------------------
       *  startBit                = 49
       *  length                  = 11
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = -1000.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        int32_T packingValue = 0;

        {
          int32_T result = (int32_T) (localB->DataTypeConversion10);

          /* no factor to apply */
          result = (int32_T) (result - -1000);
          packingValue = result;
        }

        if (packingValue < 0) {
          packingValue = 0;
        }

        {
          uint16_T packedValue;
          if (packingValue > (int32_T)(2047)) {
            packedValue = (uint16_T) 2047;
          } else if (packingValue < (int32_T)(0)) {
            packedValue = (uint16_T) 0;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[6] = localB->SECC_MSG1Pack.Data[6] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x7FU) <<
                1));
              localB->SECC_MSG1Pack.Data[7] = localB->SECC_MSG1Pack.Data[7] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x780U) >>
                7));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
       *  startBit                = 40
       *  length                  = 9
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (rtu_SECC_MSG1_L3_Vol);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint16_T packedValue;
          if (packingValue > (uint16_T)(511)) {
            packedValue = (uint16_T) 511;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              localB->SECC_MSG1Pack.Data[5] = localB->SECC_MSG1Pack.Data[5] |
                (uint8_T)((uint16_T)(packedValue & (uint16_T)0xFFU));
              localB->SECC_MSG1Pack.Data[6] = localB->SECC_MSG1Pack.Data[6] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x100U) >>
                8));
            }
          }
        }
      }
    }

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift' */
    /* DataTypeConversion: '<S3>/Data Type Conversion1' */
    rtb_y_n = CanM_MsgM_BitShift_e((uint64_T)localB->SECC_MSG1Pack.Data[1]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift1' */
    /* DataTypeConversion: '<S3>/Data Type Conversion2' */
    rtb_y_es = CanM_MsgM_BitShift1((uint64_T)localB->SECC_MSG1Pack.Data[2]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift1' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift2' */
    /* DataTypeConversion: '<S3>/Data Type Conversion3' */
    rtb_y_jt = CanM_MsgM_BitShift2((uint64_T)localB->SECC_MSG1Pack.Data[3]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift2' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift3' */
    /* DataTypeConversion: '<S3>/Data Type Conversion4' */
    rtb_y_g = CanM_MsgM_BitShift3((uint64_T)localB->SECC_MSG1Pack.Data[4]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift3' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift4' */
    /* DataTypeConversion: '<S3>/Data Type Conversion5' */
    rtb_y_kg = CanM_MsgM_BitShift4((uint64_T)localB->SECC_MSG1Pack.Data[5]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift4' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift5' */
    /* DataTypeConversion: '<S3>/Data Type Conversion6' */
    rtb_y_d0 = CanM_MsgM_BitShift5((uint64_T)localB->SECC_MSG1Pack.Data[6]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift5' */

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift6' */
    /* DataTypeConversion: '<S3>/Data Type Conversion7' */
    rtb_y_b = CanM_MsgM_BitShift6((uint64_T)localB->SECC_MSG1Pack.Data[7]);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift6' */

    /* S-Function (sfix_bitop): '<S3>/Bitwise OR' incorporates:
     *  DataTypeConversion: '<S3>/Data Type Conversion'
     */
    *rty_SECC_MSG1_OutU64 = localB->SECC_MSG1Pack.Data[0] | rtb_y_n | rtb_y_es |
      rtb_y_jt | rtb_y_g | rtb_y_kg | rtb_y_d0 | rtb_y_b;
  }

  /* End of Outputs for SubSystem: '<Root>/SECC_MSG1' */
}

/* Model step function */
void CanM_MsgM_step(void)
{
  uint64_T rtb_DataTypeConversion;
  uint64_T rtb_y;
  uint64_T rtb_y_a;
  uint64_T rtb_y_f;
  uint64_T rtb_y_h;
  uint64_T rtb_y_k;
  uint64_T rtb_y_l;
  uint64_T rtb_y_o;

  /* Outputs for Enabled SubSystem: '<Root>/MCU_Status3_Unpack' */
  /* Inport: '<Root>/Mcu_Status3_Enable' incorporates:
   *  Inport: '<Root>/MCU_Status3_Data'
   *  Outport: '<Root>/MCU_State3ReqChargingEnable'
   *  Outport: '<Root>/MCU_State3ValidStatus'
   */
  CanM_MsgM_MCU_Status3_Unpack(CanM_MsgM_U.Mcu_Status3_Enable,
    CanM_MsgM_U.MCU_Status3_Data, &CanM_MsgM_Y.MCU_State3ValidStatus,
    &CanM_MsgM_Y.MCU_State3ReqChargingEnable, &CanM_MsgM_B.MCU_Status3_Unpack,
    &CanM_MsgM_DW.MCU_Status3_Unpack);

  /* End of Outputs for SubSystem: '<Root>/MCU_Status3_Unpack' */

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG1' */
  /* Inport: '<Root>/SECC_MSG_Enable' incorporates:
   *  DataTypeConversion: '<S5>/Data Type Conversion1'
   *  DataTypeConversion: '<S5>/Data Type Conversion2'
   *  DataTypeConversion: '<S5>/Data Type Conversion3'
   *  DataTypeConversion: '<S5>/Data Type Conversion4'
   *  DataTypeConversion: '<S5>/Data Type Conversion5'
   *  DataTypeConversion: '<S5>/Data Type Conversion6'
   *  DataTypeConversion: '<S5>/Data Type Conversion7'
   *  Inport: '<Root>/SECC_MSG1_L1_Curr'
   *  Inport: '<Root>/SECC_MSG1_L1_Vol'
   *  Inport: '<Root>/SECC_MSG1_L2_Curr'
   *  Inport: '<Root>/SECC_MSG1_L2_Vol'
   *  Inport: '<Root>/SECC_MSG1_L3_Curr'
   *  Inport: '<Root>/SECC_MSG1_L3_Vol'
   *  Outport: '<Root>/SECC_MSG1_OutU64'
   */
  CanM_MsgM_SECC_MSG1(CanM_MsgM_U.SECC_MSG_Enable, CanM_MsgM_U.SECC_MSG1_L1_Curr,
                      CanM_MsgM_U.SECC_MSG1_L1_Vol,
                      CanM_MsgM_U.SECC_MSG1_L2_Curr,
                      CanM_MsgM_U.SECC_MSG1_L2_Vol,
                      CanM_MsgM_U.SECC_MSG1_L3_Curr,
                      CanM_MsgM_U.SECC_MSG1_L3_Vol,
                      &CanM_MsgM_Y.SECC_MSG1_OutU64, &CanM_MsgM_B.SECC_MSG1,
                      &CanM_MsgM_DW.SECC_MSG1);

  /* End of Outputs for SubSystem: '<Root>/SECC_MSG1' */

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG2' incorporates:
   *  EnablePort: '<S5>/Enable'
   */
  if (CanM_MsgM_U.SECC_MSG_Enable) {
    /* UnitDelay: '<S88>/Output' */
    CanM_MsgM_B.Output_h = CanM_MsgM_DW.Output_DSTATE_m;

    /* DataTypeConversion: '<S5>/Data Type Conversion8' incorporates:
     *  Inport: '<Root>/EVSE_RatedCurrent'
     */
    CanM_MsgM_B.DataTypeConversion8_i = (int16_T)CanM_MsgM_U.EVSE_RatedCurrent;

    /* S-Function (scanpack): '<S5>/SECC_MSG2 Pack' incorporates:
     *  Constant: '<S5>/Constant'
     *  Inport: '<Root>/EVSE_CanTimeout'
     *  Inport: '<Root>/EVSE_ChargingEndReason'
     *  Inport: '<Root>/EVSE_ChargingParamMismatch'
     *  Inport: '<Root>/EVSE_CpVolFault'
     *  Inport: '<Root>/EVSE_EmergeStopFault'
     *  Inport: '<Root>/EVSE_EnergyCannotTransfer'
     *  Inport: '<Root>/EVSE_HardwareFault'
     *  Inport: '<Root>/EVSE_OtherFault'
     *  Inport: '<Root>/EVSE_OverCurrFault'
     *  Inport: '<Root>/EVSE_OverTempFault'
     *  Inport: '<Root>/EVSE_RatedVoltage'
     *  Inport: '<Root>/EVSE_RelayFault'
     *  Inport: '<Root>/EVSE_Relay_Status'
     *  Inport: '<Root>/EVSE_RequestVehicleMode'
     *  Inport: '<Root>/EVSE_SelfTestFault'
     *  Inport: '<Root>/EVSE_SysStatus'
     *  Inport: '<Root>/EVSE_VoltageFault'
     */
    /* S-Function (scanpack): '<S5>/SECC_MSG2 Pack' */
    CanM_MsgM_B.SECC_MSG2Pack_d.ID = 419365206U;
    CanM_MsgM_B.SECC_MSG2Pack_d.Length = 8U;
    CanM_MsgM_B.SECC_MSG2Pack_d.Extended = 1U;
    CanM_MsgM_B.SECC_MSG2Pack_d.Remote = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[6] = 0;
    CanM_MsgM_B.SECC_MSG2Pack_d.Data[7] = 0;

    {
      /* --------------- START Packing signal 0 ------------------
       *  startBit                = 60
       *  length                  = 4
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_B.Output_h);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(15)) {
            packedValue = (uint8_T) 15;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[7] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[7] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0xFU) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 1 ------------------
       *  startBit                = 0
       *  length                  = 3
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingEndReason);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(7)) {
            packedValue = (uint8_T) 7;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x7U));
            }
          }
        }
      }

      /* --------------- START Packing signal 2 ------------------
       *  startBit                = 13
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OverCurrFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 3 ------------------
       *  startBit                = 7
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_EmergeStopFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 4 ------------------
       *  startBit                = 19
       *  length                  = 2
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_Relay_Status);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(3)) {
            packedValue = (uint8_T) 3;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x3U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 5 ------------------
       *  startBit                = 3
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_HardwareFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
       *  startBit                = 5
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RelayFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 7 ------------------
       *  startBit                = 12
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OtherFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 8 ------------------
       *  startBit                = 4
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OverTempFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 9 ------------------
       *  startBit                = 10
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_CpVolFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 10 ------------------
       *  startBit                = 15
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingParamMismatch);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 11 ------------------
       *  startBit                = 9
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (((uint8_T)0U));

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 12 ------------------
       *  startBit                = 36
       *  length                  = 12
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = -2000.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        int32_T packingValue = 0;

        {
          int32_T result = (int32_T) (CanM_MsgM_B.DataTypeConversion8_i);

          /* no factor to apply */
          result = (int32_T) (result - -2000);
          packingValue = result;
        }

        if (packingValue < 0) {
          packingValue = 0;
        }

        {
          uint16_T packedValue;
          if (packingValue > (int32_T)(4095)) {
            packedValue = (uint16_T) 4095;
          } else if (packingValue < (int32_T)(0)) {
            packedValue = (uint16_T) 0;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFU) << 4));
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFF0U) >> 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 13 ------------------
       *  startBit                = 26
       *  length                  = 10
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RatedVoltage);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint16_T packedValue;
          if (packingValue > (uint16_T)(1023)) {
            packedValue = (uint16_T) 1023;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3FU) << 2));
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3C0U) >> 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 14 ------------------
       *  startBit                = 6
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_EnergyCannotTransfer);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 15 ------------------
       *  startBit                = 11
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (((uint8_T)0U));

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 16 ------------------
       *  startBit                = 8
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_SelfTestFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 17 ------------------
       *  startBit                = 21
       *  length                  = 3
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_SysStatus);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(7)) {
            packedValue = (uint8_T) 7;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x7U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 18 ------------------
       *  startBit                = 16
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_CanTimeout);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 19 ------------------
       *  startBit                = 17
       *  length                  = 2
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RequestVehicleMode);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(3)) {
            packedValue = (uint8_T) 3;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x3U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 20 ------------------
       *  startBit                = 14
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_VoltageFault);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }
    }

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift' */
    rtb_DataTypeConversion = CanM_MsgM_BitShift_e((uint64_T)
      CanM_MsgM_B.SECC_MSG2Pack_d.Data[1]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift1' */
    rtb_y = CanM_MsgM_BitShift1((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[2]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift1' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift2' */
    rtb_y_l = CanM_MsgM_BitShift2((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[3]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift2' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift3' */
    rtb_y_h = CanM_MsgM_BitShift3((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[4]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift3' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift4' */
    rtb_y_o = CanM_MsgM_BitShift4((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[5]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift4' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift5' */
    rtb_y_f = CanM_MsgM_BitShift5((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[6]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift5' */

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift6' */
    rtb_y_a = CanM_MsgM_BitShift6((uint64_T)CanM_MsgM_B.SECC_MSG2Pack_d.Data[7]);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift6' */

    /* DataTypeConversion: '<S5>/Data Type Conversion' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion1'
     *  DataTypeConversion: '<S5>/Data Type Conversion2'
     *  DataTypeConversion: '<S5>/Data Type Conversion3'
     *  DataTypeConversion: '<S5>/Data Type Conversion4'
     *  DataTypeConversion: '<S5>/Data Type Conversion5'
     *  DataTypeConversion: '<S5>/Data Type Conversion6'
     *  DataTypeConversion: '<S5>/Data Type Conversion7'
     */
    rtb_y_k = CanM_MsgM_B.SECC_MSG2Pack_d.Data[0];

    /* Outport: '<Root>/SECC_MSG2_OutU64' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion'
     *  S-Function (sfix_bitop): '<S5>/Bitwise OR'
     */
    CanM_MsgM_Y.SECC_MSG2_OutU64 = rtb_y_k | rtb_DataTypeConversion | rtb_y |
      rtb_y_l | rtb_y_h | rtb_y_o | rtb_y_f | rtb_y_a;

    /* Switch: '<S97>/FixPt Switch' incorporates:
     *  Constant: '<S96>/FixPt Constant'
     *  Constant: '<S97>/Constant'
     *  Sum: '<S96>/FixPt Sum1'
     *  UnitDelay: '<S88>/Output'
     */
    if ((uint8_T)(CanM_MsgM_B.Output_h + 1U) > 15) {
      CanM_MsgM_DW.Output_DSTATE_m = 0U;
    } else {
      CanM_MsgM_DW.Output_DSTATE_m = (uint8_T)(CanM_MsgM_B.Output_h + 1U);
    }

    /* End of Switch: '<S97>/FixPt Switch' */
  }

  /* End of Inport: '<Root>/SECC_MSG_Enable' */
  /* End of Outputs for SubSystem: '<Root>/SECC_MSG2' */

  /* Outputs for Enabled SubSystem: '<Root>/MCU_Status3_Unpack1' */
  /* Inport: '<Root>/Mcu_Status3_Enable1' incorporates:
   *  Inport: '<Root>/MCU_Status3_Data1'
   *  Outport: '<Root>/MCU_State3ReqChargingEnable1'
   *  Outport: '<Root>/MCU_State3ValidStatus1'
   */
  CanM_MsgM_MCU_Status3_Unpack(CanM_MsgM_U.Mcu_Status3_Enable1,
    CanM_MsgM_U.MCU_Status3_Data1, &CanM_MsgM_Y.MCU_State3ValidStatus1,
    &CanM_MsgM_Y.MCU_State3ReqChargingEnable1, &CanM_MsgM_B.MCU_Status3_Unpack1,
    &CanM_MsgM_DW.MCU_Status3_Unpack1);

  /* End of Outputs for SubSystem: '<Root>/MCU_Status3_Unpack1' */

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG1_1' */
  /* Inport: '<Root>/SECC_MSG_Enable1' incorporates:
   *  DataTypeConversion: '<S6>/Data Type Conversion1'
   *  DataTypeConversion: '<S6>/Data Type Conversion2'
   *  DataTypeConversion: '<S6>/Data Type Conversion3'
   *  DataTypeConversion: '<S6>/Data Type Conversion4'
   *  DataTypeConversion: '<S6>/Data Type Conversion5'
   *  DataTypeConversion: '<S6>/Data Type Conversion6'
   *  DataTypeConversion: '<S6>/Data Type Conversion7'
   *  Inport: '<Root>/SECC_MSG1_L1_Curr1'
   *  Inport: '<Root>/SECC_MSG1_L1_Vol1'
   *  Inport: '<Root>/SECC_MSG1_L2_Curr1'
   *  Inport: '<Root>/SECC_MSG1_L2_Vol1'
   *  Inport: '<Root>/SECC_MSG1_L3_Curr1'
   *  Inport: '<Root>/SECC_MSG1_L3_Vol1'
   *  Outport: '<Root>/SECC_MSG1_OutU64_1'
   */
  CanM_MsgM_SECC_MSG1(CanM_MsgM_U.SECC_MSG_Enable1,
                      CanM_MsgM_U.SECC_MSG1_L1_Curr1,
                      CanM_MsgM_U.SECC_MSG1_L1_Vol1,
                      CanM_MsgM_U.SECC_MSG1_L2_Curr1,
                      CanM_MsgM_U.SECC_MSG1_L2_Vol1,
                      CanM_MsgM_U.SECC_MSG1_L3_Curr1,
                      CanM_MsgM_U.SECC_MSG1_L3_Vol1,
                      &CanM_MsgM_Y.SECC_MSG1_OutU64_1, &CanM_MsgM_B.SECC_MSG1_1,
                      &CanM_MsgM_DW.SECC_MSG1_1);

  /* End of Outputs for SubSystem: '<Root>/SECC_MSG1_1' */

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG2_1' incorporates:
   *  EnablePort: '<S6>/Enable'
   */
  if (CanM_MsgM_U.SECC_MSG_Enable1) {
    /* UnitDelay: '<S105>/Output' */
    CanM_MsgM_B.Output = CanM_MsgM_DW.Output_DSTATE;

    /* DataTypeConversion: '<S6>/Data Type Conversion8' incorporates:
     *  Inport: '<Root>/EVSE_RatedCurrent1'
     */
    CanM_MsgM_B.DataTypeConversion8 = (int16_T)CanM_MsgM_U.EVSE_RatedCurrent1;

    /* S-Function (scanpack): '<S6>/SECC_MSG2 Pack' incorporates:
     *  Constant: '<S6>/Constant'
     *  Inport: '<Root>/EVSE_CanTimeout1'
     *  Inport: '<Root>/EVSE_ChargingEndReason1'
     *  Inport: '<Root>/EVSE_ChargingParamMismatch1'
     *  Inport: '<Root>/EVSE_CpVolFault1'
     *  Inport: '<Root>/EVSE_EmergeStopFault1'
     *  Inport: '<Root>/EVSE_EnergyCannotTransfer1'
     *  Inport: '<Root>/EVSE_HardwareFault1'
     *  Inport: '<Root>/EVSE_OtherFault1'
     *  Inport: '<Root>/EVSE_OverCurrFault1'
     *  Inport: '<Root>/EVSE_OverTempFault1'
     *  Inport: '<Root>/EVSE_RatedVoltage1'
     *  Inport: '<Root>/EVSE_RelayFault1'
     *  Inport: '<Root>/EVSE_Relay_Status1'
     *  Inport: '<Root>/EVSE_RequestVehicleMode1'
     *  Inport: '<Root>/EVSE_SelfTestFault1'
     *  Inport: '<Root>/EVSE_SysStatus1'
     *  Inport: '<Root>/EVSE_VoltageFault1'
     */
    /* S-Function (scanpack): '<S6>/SECC_MSG2 Pack' */
    CanM_MsgM_B.SECC_MSG2Pack.ID = 419365206U;
    CanM_MsgM_B.SECC_MSG2Pack.Length = 8U;
    CanM_MsgM_B.SECC_MSG2Pack.Extended = 1U;
    CanM_MsgM_B.SECC_MSG2Pack.Remote = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[0] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[1] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[2] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[3] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[4] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[5] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[6] = 0;
    CanM_MsgM_B.SECC_MSG2Pack.Data[7] = 0;

    {
      /* --------------- START Packing signal 0 ------------------
       *  startBit                = 60
       *  length                  = 4
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_B.Output);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(15)) {
            packedValue = (uint8_T) 15;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[7] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[7] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0xFU) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 1 ------------------
       *  startBit                = 0
       *  length                  = 3
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingEndReason1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(7)) {
            packedValue = (uint8_T) 7;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x7U));
            }
          }
        }
      }

      /* --------------- START Packing signal 2 ------------------
       *  startBit                = 13
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OverCurrFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 3 ------------------
       *  startBit                = 7
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_EmergeStopFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 4 ------------------
       *  startBit                = 19
       *  length                  = 2
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_Relay_Status1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(3)) {
            packedValue = (uint8_T) 3;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x3U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 5 ------------------
       *  startBit                = 3
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_HardwareFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
       *  startBit                = 5
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RelayFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 7 ------------------
       *  startBit                = 12
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OtherFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 8 ------------------
       *  startBit                = 4
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_OverTempFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 9 ------------------
       *  startBit                = 10
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_CpVolFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 10 ------------------
       *  startBit                = 15
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingParamMismatch1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 11 ------------------
       *  startBit                = 9
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (((uint8_T)0U));

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 12 ------------------
       *  startBit                = 36
       *  length                  = 12
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = -2000.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        int32_T packingValue = 0;

        {
          int32_T result = (int32_T) (CanM_MsgM_B.DataTypeConversion8);

          /* no factor to apply */
          result = (int32_T) (result - -2000);
          packingValue = result;
        }

        if (packingValue < 0) {
          packingValue = 0;
        }

        {
          uint16_T packedValue;
          if (packingValue > (int32_T)(4095)) {
            packedValue = (uint16_T) 4095;
          } else if (packingValue < (int32_T)(0)) {
            packedValue = (uint16_T) 0;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[4] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFU) << 4));
              CanM_MsgM_B.SECC_MSG2Pack.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFF0U) >> 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 13 ------------------
       *  startBit                = 26
       *  length                  = 10
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RatedVoltage1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint16_T packedValue;
          if (packingValue > (uint16_T)(1023)) {
            packedValue = (uint16_T) 1023;
          } else {
            packedValue = (uint16_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[3] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3FU) << 2));
              CanM_MsgM_B.SECC_MSG2Pack.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[4] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3C0U) >> 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 14 ------------------
       *  startBit                = 6
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_EnergyCannotTransfer1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 15 ------------------
       *  startBit                = 11
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (((uint8_T)0U));

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 16 ------------------
       *  startBit                = 8
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_SelfTestFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 17 ------------------
       *  startBit                = 21
       *  length                  = 3
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_SysStatus1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(7)) {
            packedValue = (uint8_T) 7;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x7U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 18 ------------------
       *  startBit                = 16
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_CanTimeout1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 19 ------------------
       *  startBit                = 17
       *  length                  = 2
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_RequestVehicleMode1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(3)) {
            packedValue = (uint8_T) 3;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x3U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 20 ------------------
       *  startBit                = 14
       *  length                  = 1
       *  desiredSignalByteLayout = LITTLEENDIAN
       *  dataType                = UNSIGNED
       *  factor                  = 1.0
       *  offset                  = 0.0
       *  minimum                 = 0.0
       *  maximum                 = 0.0
       * -----------------------------------------------------------------------*/
      {
        uint32_T packingValue = 0;

        {
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_VoltageFault1);

          /* no scaling required */
          packingValue = result;
        }

        {
          uint8_T packedValue;
          if (packingValue > (uint8_T)(1)) {
            packedValue = (uint8_T) 1;
          } else {
            packedValue = (uint8_T) (packingValue);
          }

          {
            {
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }
    }

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift' */
    rtb_y_k = CanM_MsgM_BitShift_e((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[1]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift1' */
    rtb_y_a = CanM_MsgM_BitShift1((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[2]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift1' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift2' */
    rtb_y_f = CanM_MsgM_BitShift2((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[3]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift2' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift3' */
    rtb_y_o = CanM_MsgM_BitShift3((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[4]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift3' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift4' */
    rtb_y_h = CanM_MsgM_BitShift4((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[5]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift4' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift5' */
    rtb_y_l = CanM_MsgM_BitShift5((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[6]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift5' */

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift6' */
    rtb_y = CanM_MsgM_BitShift6((uint64_T)CanM_MsgM_B.SECC_MSG2Pack.Data[7]);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift6' */

    /* Outport: '<Root>/SECC_MSG2_OutU64_1' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion'
     *  DataTypeConversion: '<S6>/Data Type Conversion1'
     *  DataTypeConversion: '<S6>/Data Type Conversion2'
     *  DataTypeConversion: '<S6>/Data Type Conversion3'
     *  DataTypeConversion: '<S6>/Data Type Conversion4'
     *  DataTypeConversion: '<S6>/Data Type Conversion5'
     *  DataTypeConversion: '<S6>/Data Type Conversion6'
     *  DataTypeConversion: '<S6>/Data Type Conversion7'
     *  S-Function (sfix_bitop): '<S6>/Bitwise OR'
     */
    CanM_MsgM_Y.SECC_MSG2_OutU64_1 = CanM_MsgM_B.SECC_MSG2Pack.Data[0] | rtb_y_k
      | rtb_y_a | rtb_y_f | rtb_y_o | rtb_y_h | rtb_y_l | rtb_y;

    /* Switch: '<S114>/FixPt Switch' incorporates:
     *  Constant: '<S113>/FixPt Constant'
     *  Constant: '<S114>/Constant'
     *  Sum: '<S113>/FixPt Sum1'
     *  UnitDelay: '<S105>/Output'
     */
    if ((uint8_T)(CanM_MsgM_B.Output + 1U) > 15) {
      CanM_MsgM_DW.Output_DSTATE = 0U;
    } else {
      CanM_MsgM_DW.Output_DSTATE = (uint8_T)(CanM_MsgM_B.Output + 1U);
    }

    /* End of Switch: '<S114>/FixPt Switch' */
  }

  /* End of Inport: '<Root>/SECC_MSG_Enable1' */
  /* End of Outputs for SubSystem: '<Root>/SECC_MSG2_1' */
}

/* Model initialize function */
void CanM_MsgM_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(CanM_MsgM_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &CanM_MsgM_B), 0,
                sizeof(B_CanM_MsgM_T));

  /* states (dwork) */
  (void) memset((void *)&CanM_MsgM_DW, 0,
                sizeof(DW_CanM_MsgM_T));

  /* SystemInitialize for Enabled SubSystem: '<Root>/MCU_Status3_Unpack' */
  CanM_Ms_MCU_Status3_Unpack_Init(&CanM_MsgM_DW.MCU_Status3_Unpack);

  /* End of SystemInitialize for SubSystem: '<Root>/MCU_Status3_Unpack' */

  /* SystemInitialize for Enabled SubSystem: '<Root>/SECC_MSG1' */

  /* SystemInitialize for Inport: '<Root>/SECC_MSG1_L1_Vol' incorporates:
   *  Inport: '<Root>/SECC_MSG1_L2_Vol'
   *  Inport: '<Root>/SECC_MSG1_L3_Vol'
   */

  /* End of SystemInitialize for SubSystem: '<Root>/SECC_MSG1' */

  /* SystemInitialize for Enabled SubSystem: '<Root>/MCU_Status3_Unpack1' */
  CanM_Ms_MCU_Status3_Unpack_Init(&CanM_MsgM_DW.MCU_Status3_Unpack1);

  /* End of SystemInitialize for SubSystem: '<Root>/MCU_Status3_Unpack1' */

  /* SystemInitialize for Enabled SubSystem: '<Root>/SECC_MSG1_1' */

  /* SystemInitialize for Inport: '<Root>/SECC_MSG1_L1_Vol1' incorporates:
   *  Inport: '<Root>/SECC_MSG1_L2_Vol1'
   *  Inport: '<Root>/SECC_MSG1_L3_Vol1'
   */

  /* End of SystemInitialize for SubSystem: '<Root>/SECC_MSG1_1' */
}

/* Model terminate function */
void CanM_MsgM_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
