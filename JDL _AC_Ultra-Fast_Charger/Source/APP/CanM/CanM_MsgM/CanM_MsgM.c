/*
 * File: CanM_MsgM.c
 *
 * Code generated for Simulink model 'CanM_MsgM'.
 *
 * Model version                  : 1.129
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Mon Jul 21 08:32:47 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CanM_MsgM.h"
#include "multiword_types.h"
#include "rtwtypes.h"
#include "CanM_MsgM_private.h"
#include <math.h>
#include "rt_nonfinite.h"

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
void uMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T i;
  int32_T i1;
  int32_T nb;
  nb = (int32_T)(n2 >> 5);
  i = 0;
  if (nb < n1) {
    int32_T nc;
    uint32_T nr;
    nc = n + nb;
    if (nc > n1) {
      nc = n1;
    }

    nr = n2 - ((uint32_T)nb << 5);
    if (nr > 0U) {
      uint32_T u1i;
      uint32_T yi;
      u1i = u1[nb];
      for (i1 = nb + 1; i1 < nc; i1++) {
        yi = u1i >> nr;
        u1i = u1[i1];
        y[i] = u1i << (32U - nr) | yi;
        i++;
      }

      yi = u1i >> nr;
      if (nc < n1) {
        yi |= u1[nc] << (32U - nr);
      }

      y[i] = yi;
      i++;
    } else {
      for (i1 = nb; i1 < nc; i1++) {
        y[i] = u1[i1];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = 0U;
    i++;
  }
}

void uMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T y[],
                   int32_T n)
{
  int32_T i;
  int32_T nb;
  int32_T nc;
  uint32_T u1i;
  uint32_T ys;
  nb = (int32_T)(n2 >> 5);
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  nc = nb > n ? n : nb;
  u1i = 0U;
  for (i = 0; i < nc; i++) {
    y[i] = 0U;
  }

  if (nb < n) {
    uint32_T nl;
    nl = n2 - ((uint32_T)nb << 5);
    nb += n1;
    if (nb > n) {
      nb = n;
    }

    nb -= i;
    if (nl > 0U) {
      for (nc = 0; nc < nb; nc++) {
        uint32_T yi;
        yi = u1i >> (32U - nl);
        u1i = u1[nc];
        y[i] = u1i << nl | yi;
        i++;
      }

      if (i < n) {
        y[i] = u1i >> (32U - nl) | ys << nl;
        i++;
      }
    } else {
      for (nc = 0; nc < nb; nc++) {
        y[i] = u1[nc];
        i++;
      }
    }
  }

  while (i < n) {
    y[i] = ys;
    i++;
  }
}

void uLong2MultiWord(uint32_T u, uint32_T y[], int32_T n)
{
  int32_T i;
  y[0] = u;
  for (i = 1; i < n; i++) {
    y[i] = 0U;
  }
}

void MultiWordIor(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
                  int32_T n)
{
  int32_T i;
  for (i = 0; i < n; i++) {
    y[i] = u1[i] | u2[i];
  }
}

uint32_T MultiWord2uLong(const uint32_T u[])
{
  return u[0];
}

void MultiWordAnd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
                  int32_T n)
{
  int32_T i;
  for (i = 0; i < n; i++) {
    y[i] = u1[i] & u2[i];
  }
}

/*
 * Output and update for atomic system:
 *    '<S8>/Bit Shift'
 *    '<S8>/Bit Shift1'
 *    '<S8>/Bit Shift2'
 *    '<S8>/Bit Shift3'
 *    '<S8>/Bit Shift4'
 *    '<S8>/Bit Shift5'
 *    '<S8>/Bit Shift6'
 *    '<S25>/Bit Shift'
 *    '<S25>/Bit Shift1'
 *    '<S25>/Bit Shift2'
 *    ...
 */
void CanM_MsgM_BitShift(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S10>/bit_shift' */
  uMultiWordShr(&rtu_u.chunks[0U], 2, 8U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift'
 *    '<S4>/Bit Shift'
 *    '<S5>/Bit Shift'
 *    '<S6>/Bit Shift'
 */
void CanM_MsgM_BitShift_e(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S44>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 8U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift1'
 *    '<S4>/Bit Shift1'
 *    '<S5>/Bit Shift1'
 *    '<S6>/Bit Shift1'
 */
void CanM_MsgM_BitShift1(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S45>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 16U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift2'
 *    '<S4>/Bit Shift2'
 *    '<S5>/Bit Shift2'
 *    '<S6>/Bit Shift2'
 */
void CanM_MsgM_BitShift2(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S46>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 24U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift3'
 *    '<S4>/Bit Shift3'
 *    '<S5>/Bit Shift3'
 *    '<S6>/Bit Shift3'
 */
void CanM_MsgM_BitShift3(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S47>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 32U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift4'
 *    '<S4>/Bit Shift4'
 *    '<S5>/Bit Shift4'
 *    '<S6>/Bit Shift4'
 */
void CanM_MsgM_BitShift4(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S48>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 40U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift5'
 *    '<S4>/Bit Shift5'
 *    '<S5>/Bit Shift5'
 *    '<S6>/Bit Shift5'
 */
void CanM_MsgM_BitShift5(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S49>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 48U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for atomic system:
 *    '<S3>/Bit Shift6'
 *    '<S4>/Bit Shift6'
 *    '<S5>/Bit Shift6'
 *    '<S6>/Bit Shift6'
 */
void CanM_MsgM_BitShift6(uint64m_T rtu_u, uint64m_T *rty_y)
{
  /* MATLAB Function: '<S50>/bit_shift' */
  uMultiWordShl(&rtu_u.chunks[0U], 2, 56U, &rty_y->chunks[0U], 2);
}

/*
 * Output and update for enable system:
 *    '<Root>/SECC_MSG1'
 *    '<Root>/SECC_MSG1_1'
 */
void CanM_MsgM_SECC_MSG1(boolean_T rtu_Enable, uint16_T rtu_SECC_MSG1_L1_Curr,
  uint16_T rtu_SECC_MSG1_L1_Vol, uint16_T rtu_SECC_MSG1_L2_Curr, uint16_T
  rtu_SECC_MSG1_L2_Vol, uint16_T rtu_SECC_MSG1_L3_Curr, uint16_T
  rtu_SECC_MSG1_L3_Vol, uint64m_T *rty_SECC_MSG1_OutU64, B_SECC_MSG1_CanM_MsgM_T
  *localB, DW_SECC_MSG1_CanM_MsgM_T *localDW)
{
  uint64m_T rtb_y_a;
  uint64m_T rtb_y_an;
  uint64m_T rtb_y_fd;
  uint64m_T rtb_y_g;
  uint64m_T rtb_y_k;
  uint64m_T rtb_y_na;
  uint64m_T rtb_y_pt;
  uint64m_T tmp;
  uint64m_T tmp_0;
  uint64m_T tmp_1;
  uint64m_T tmp_2;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  uint64m_T tmp_5;

  /* Outputs for Enabled SubSystem: '<Root>/SECC_MSG1' incorporates:
   *  EnablePort: '<S3>/Enable'
   */
  if (rtu_Enable) {
    /* Sum: '<S41>/Subtract' incorporates:
     *  Constant: '<S41>/Constant1'
     */
    localB->Subtract = (int16_T)(rtu_SECC_MSG1_L2_Curr - 1000U);

    /* Sum: '<S42>/Subtract' incorporates:
     *  Constant: '<S42>/Constant1'
     */
    localB->Subtract_l = (int16_T)(rtu_SECC_MSG1_L1_Curr - 1000U);

    /* Sum: '<S43>/Subtract' incorporates:
     *  Constant: '<S43>/Constant1'
     */
    localB->Subtract_e = (int16_T)(rtu_SECC_MSG1_L3_Curr - 1000U);

    /* UnitDelay: '<S51>/Output' */
    localB->Output = localDW->Output_DSTATE;

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
          int32_T result = (int32_T) (localB->Subtract_l);

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
          int32_T result = (int32_T) (localB->Subtract);

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
       *  startBit                = 40
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
          int32_T result = (int32_T) (localB->Subtract_e);

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
              localB->SECC_MSG1Pack.Data[5] = localB->SECC_MSG1Pack.Data[5] |
                (uint8_T)((uint16_T)(packedValue & (uint16_T)0xFFU));
              localB->SECC_MSG1Pack.Data[6] = localB->SECC_MSG1Pack.Data[6] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x700U) >>
                8));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
       *  startBit                = 51
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
              localB->SECC_MSG1Pack.Data[6] = localB->SECC_MSG1Pack.Data[6] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x1FU) <<
                3));
              localB->SECC_MSG1Pack.Data[7] = localB->SECC_MSG1Pack.Data[7] |
                (uint8_T)((uint16_T)((uint16_T)(packedValue & (uint16_T)0x1E0U) >>
                5));
            }
          }
        }
      }
    }

    /* DataTypeConversion: '<S3>/Data Type Conversion1' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[1], &rtb_y_an.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift' */
    CanM_MsgM_BitShift_e(rtb_y_an, &rtb_y_fd);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift' */

    /* DataTypeConversion: '<S3>/Data Type Conversion2' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[2], &rtb_y_k.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift1' */
    CanM_MsgM_BitShift1(rtb_y_k, &rtb_y_an);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift1' */

    /* DataTypeConversion: '<S3>/Data Type Conversion3' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[3], &rtb_y_na.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift2' */
    CanM_MsgM_BitShift2(rtb_y_na, &rtb_y_k);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift2' */

    /* DataTypeConversion: '<S3>/Data Type Conversion4' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[4], &rtb_y_a.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift3' */
    CanM_MsgM_BitShift3(rtb_y_a, &rtb_y_na);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift3' */

    /* DataTypeConversion: '<S3>/Data Type Conversion5' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[5], &rtb_y_g.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift4' */
    CanM_MsgM_BitShift4(rtb_y_g, &rtb_y_a);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift4' */

    /* DataTypeConversion: '<S3>/Data Type Conversion6' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[6], &rtb_y_pt.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift5' */
    CanM_MsgM_BitShift5(rtb_y_pt, &rtb_y_g);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift5' */

    /* DataTypeConversion: '<S3>/Data Type Conversion7' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[7], &tmp.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S3>/Bit Shift6' */
    CanM_MsgM_BitShift6(tmp, &rtb_y_pt);

    /* End of Outputs for SubSystem: '<S3>/Bit Shift6' */

    /* DataTypeConversion: '<S3>/Data Type Conversion' */
    uLong2MultiWord(localB->SECC_MSG1Pack.Data[0], &tmp_5.chunks[0U], 2);

    /* S-Function (sfix_bitop): '<S3>/Bitwise OR' */
    MultiWordIor(&tmp_5.chunks[0U], &rtb_y_fd.chunks[0U], &tmp_4.chunks[0U], 2);
    MultiWordIor(&tmp_4.chunks[0U], &rtb_y_an.chunks[0U], &tmp_3.chunks[0U], 2);
    MultiWordIor(&tmp_3.chunks[0U], &rtb_y_k.chunks[0U], &tmp_2.chunks[0U], 2);
    MultiWordIor(&tmp_2.chunks[0U], &rtb_y_na.chunks[0U], &tmp_1.chunks[0U], 2);
    MultiWordIor(&tmp_1.chunks[0U], &rtb_y_a.chunks[0U],
                 &rty_SECC_MSG1_OutU64->chunks[0U], 2);
    MultiWordIor(&rty_SECC_MSG1_OutU64->chunks[0U], &rtb_y_g.chunks[0U],
                 &tmp_0.chunks[0U], 2);
    MultiWordIor(&tmp_0.chunks[0U], &rtb_y_pt.chunks[0U],
                 &rty_SECC_MSG1_OutU64->chunks[0U], 2);

    /* Switch: '<S60>/FixPt Switch' incorporates:
     *  Constant: '<S59>/FixPt Constant'
     *  Constant: '<S60>/Constant'
     *  Sum: '<S59>/FixPt Sum1'
     *  UnitDelay: '<S51>/Output'
     */
    if ((uint8_T)(localB->Output + 1) > 15) {
      localDW->Output_DSTATE = 0U;
    } else {
      localDW->Output_DSTATE = (uint8_T)(localB->Output + 1);
    }

    /* End of Switch: '<S60>/FixPt Switch' */
  }

  /* End of Outputs for SubSystem: '<Root>/SECC_MSG1' */
}

/* Model step function */
void CanM_MsgM_step(void)
{
  uint64m_T rtb_DataTypeConversion;
  uint64m_T rtb_y;
  uint64m_T rtb_y_ay;
  uint64m_T rtb_y_f;
  uint64m_T rtb_y_j;
  uint64m_T rtb_y_ko;
  uint64m_T rtb_y_l;
  uint64m_T rtb_y_m;
  uint64m_T tmp_0;
  uint64m_T tmp_1;
  uint64m_T tmp_2;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  uint64m_T tmp_5;
  uint64m_T tmp_6;
  uint64m_T tmp_7;
  uint64m_T tmp_8;
  uint64m_T tmp_9;
  uint64m_T tmp_a;
  uint64m_T tmp_b;
  uint64m_T tmp_c;
  uint64m_T tmp_d;
  uint64m_T tmp_e;
  uint64m_T tmp_f;
  uint64m_T tmp_g;
  uint64m_T tmp_h;
  uint64m_T tmp_i;
  uint64m_T tmp_j;
  real_T tmp;
  uint8_T rtb_DataTypeConversion_j;
  uint8_T rtb_DataTypeConversion_o;

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift' */
  /* Inport: '<Root>/MCU_Status3_Data' */
  CanM_MsgM_BitShift(CanM_MsgM_U.MCU_Status3_Data, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND1' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_0.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion1'
   */
  CanM_MsgM_B.BusCreator1.Data[1] = (uint8_T)MultiWord2uLong(&tmp_0.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift1' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift1' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND2' */
  tmp_0 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &rtb_y.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion2'
   */
  CanM_MsgM_B.BusCreator1.Data[2] = (uint8_T)MultiWord2uLong(&rtb_y.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift2' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift2' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND3' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_0.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion3'
   */
  CanM_MsgM_B.BusCreator1.Data[3] = (uint8_T)MultiWord2uLong(&tmp_0.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift3' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift3' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND4' */
  tmp_0 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &rtb_y.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion4'
   */
  CanM_MsgM_B.BusCreator1.Data[4] = (uint8_T)MultiWord2uLong(&rtb_y.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift4' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift4' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND5' */
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_0.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion5'
   */
  CanM_MsgM_B.BusCreator1.Data[5] = (uint8_T)MultiWord2uLong(&tmp_0.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift5' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift5' */

  /* Outputs for Atomic SubSystem: '<S8>/Bit Shift6' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S8>/Bit Shift6' */

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND7' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &rtb_DataTypeConversion.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  Constant: '<S1>/Constant26'
   *  Constant: '<S1>/Constant27'
   *  Constant: '<S1>/Constant28'
   *  Constant: '<S1>/Constant29'
   *  Constant: '<S1>/Constant3'
   *  Constant: '<S1>/Constant30'
   */
  CanM_MsgM_B.BusCreator1.Extended = 1U;
  CanM_MsgM_B.BusCreator1.Length = 8U;
  CanM_MsgM_B.BusCreator1.Remote = 0U;
  CanM_MsgM_B.BusCreator1.Error = 0U;
  CanM_MsgM_B.BusCreator1.ID = 419406575U;
  CanM_MsgM_B.BusCreator1.Timestamp = 0.0;

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND' incorporates:
   *  Inport: '<Root>/MCU_Status3_Data'
   */
  tmp_0 = CanM_MsgM_U.MCU_Status3_Data;
  MultiWordAnd(&CanM_MsgM_U.MCU_Status3_Data.chunks[0U],
               &CanM_MsgM_ConstP.pooled12.chunks[0], &rtb_y.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion'
   */
  CanM_MsgM_B.BusCreator1.Data[0] = (uint8_T)MultiWord2uLong(&rtb_y.chunks[0U]);

  /* S-Function (sfix_bitop): '<S8>/Bitwise AND6' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_0.chunks[0U], 2);

  /* BusCreator: '<S1>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S8>/Data Type Conversion6'
   *  DataTypeConversion: '<S8>/Data Type Conversion7'
   */
  CanM_MsgM_B.BusCreator1.Data[6] = (uint8_T)MultiWord2uLong(&tmp_0.chunks[0U]);
  CanM_MsgM_B.BusCreator1.Data[7] = (uint8_T)MultiWord2uLong
    (&rtb_DataTypeConversion.chunks[0U]);

  /* S-Function (scanunpack): '<S1>/CAN Unpack' */
  {
    /* S-Function (scanunpack): '<S1>/CAN Unpack' */
    if ((8 == CanM_MsgM_B.BusCreator1.Length) && (CanM_MsgM_B.BusCreator1.ID !=
         INVALID_CAN_ID) ) {
      if ((419406575 == CanM_MsgM_B.BusCreator1.ID) && (1U ==
           CanM_MsgM_B.BusCreator1.Extended) ) {
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
                    (CanM_MsgM_B.BusCreator1.Data[7]) & (uint8_T)(0xF0U)) >> 4);
                }

                unpackedValue = tempValue;
              }

              outValue = (real_T) (unpackedValue);
            }

            {
              real_T result = (real_T) outValue;
              CanM_MsgM_B.CANUnpack_o1 = result;
            }
          }

          /* --------------- START Unpacking signal 1 ------------------
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

          /* --------------- START Unpacking signal 2 ------------------
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

          /* --------------- START Unpacking signal 3 ------------------
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
           *  startBit                = 33
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
           *  startBit                = 27
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
           *  startBit                = 35
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
           *  startBit                = 32
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
                  tempValue = tempValue | (uint8_T)((uint8_T)
                    (CanM_MsgM_B.BusCreator1.Data[4]) & (uint8_T)(0x1U));
                }

                unpackedValue = tempValue;
              }

              outValue = (real_T) (unpackedValue);
            }

            {
              real_T result = (real_T) outValue;
              CanM_MsgM_B.CANUnpack_o9 = result;
            }
          }

          /* --------------- START Unpacking signal 9 ------------------
           *  startBit                = 24
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

          /* --------------- START Unpacking signal 11 ------------------
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

          /* --------------- START Unpacking signal 12 ------------------
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

          /* --------------- START Unpacking signal 13 ------------------
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

          /* --------------- START Unpacking signal 14 ------------------
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

          /* --------------- START Unpacking signal 15 ------------------
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

          /* --------------- START Unpacking signal 16 ------------------
           *  startBit                = 26
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

          /* --------------- START Unpacking signal 18 ------------------
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

          /* --------------- START Unpacking signal 19 ------------------
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

          /* --------------- START Unpacking signal 20 ------------------
           *  startBit                = 25
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
        }
      }
    }
  }

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
   *  S-Function (sfix_bitop): '<S8>/Bitwise AND'
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
    /* Sum: '<S81>/Subtract' incorporates:
     *  Constant: '<S81>/Constant1'
     *  Inport: '<Root>/EVSE_RatedCurrent'
     */
    CanM_MsgM_B.Subtract_e = (int16_T)(CanM_MsgM_U.EVSE_RatedCurrent - 2000U);

    /* UnitDelay: '<S89>/Output' */
    CanM_MsgM_B.Output_h = CanM_MsgM_DW.Output_DSTATE_m;

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
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingEndReason);

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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[0] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0xFU));
            }
          }
        }
      }

      /* --------------- START Packing signal 2 ------------------
       *  startBit                = 20
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 3 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 4 ------------------
       *  startBit                = 26
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x3U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 5 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 7 ------------------
       *  startBit                = 17
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 8 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 9 ------------------
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
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 10 ------------------
       *  startBit                = 22
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 11 ------------------
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
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 12 ------------------
       *  startBit                = 42
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
          int32_T result = (int32_T) (CanM_MsgM_B.Subtract_e);

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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3FU) << 2));
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[6] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[6] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFC0U) >> 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 13 ------------------
       *  startBit                = 32
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[4] | (uint8_T)((uint16_T)
                (packedValue & (uint16_T)0xFFU));
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x300U) >> 8));
            }
          }
        }
      }

      /* --------------- START Packing signal 14 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[1] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 15 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 16 ------------------
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
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 17 ------------------
       *  startBit                = 28
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x7U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 18 ------------------
       *  startBit                = 23
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
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 19 ------------------
       *  startBit                = 24
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[3] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x3U));
            }
          }
        }
      }

      /* --------------- START Packing signal 20 ------------------
       *  startBit                = 21
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
              CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack_d.Data[2] | (uint8_T)((uint8_T)
                ((uint8_T)(packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }
    }

    /* DataTypeConversion: '<S5>/Data Type Conversion1' */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[1], &rtb_y.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift' */
    CanM_MsgM_BitShift_e(rtb_y, &rtb_DataTypeConversion);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift' */

    /* DataTypeConversion: '<S5>/Data Type Conversion2' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion1'
     *  S-Function (sfix_bitop): '<S8>/Bitwise AND'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[2], &rtb_y_ko.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift1' */
    CanM_MsgM_BitShift1(rtb_y_ko, &rtb_y);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift1' */

    /* DataTypeConversion: '<S5>/Data Type Conversion3' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion2'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[3], &rtb_y_l.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift2' */
    CanM_MsgM_BitShift2(rtb_y_l, &rtb_y_ko);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift2' */

    /* DataTypeConversion: '<S5>/Data Type Conversion4' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion3'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[4], &rtb_y_j.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift3' */
    CanM_MsgM_BitShift3(rtb_y_j, &rtb_y_l);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift3' */

    /* DataTypeConversion: '<S5>/Data Type Conversion5' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion4'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[5], &rtb_y_ay.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift4' */
    CanM_MsgM_BitShift4(rtb_y_ay, &rtb_y_j);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift4' */

    /* DataTypeConversion: '<S5>/Data Type Conversion6' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion5'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[6], &tmp_1.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift5' */
    CanM_MsgM_BitShift5(tmp_1, &rtb_y_ay);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift5' */

    /* DataTypeConversion: '<S5>/Data Type Conversion7' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion6'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[7], &tmp_2.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S5>/Bit Shift6' */
    CanM_MsgM_BitShift6(tmp_2, &rtb_y_f);

    /* End of Outputs for SubSystem: '<S5>/Bit Shift6' */

    /* DataTypeConversion: '<S5>/Data Type Conversion' incorporates:
     *  DataTypeConversion: '<S5>/Data Type Conversion7'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack_d.Data[0], &rtb_y_m.chunks[0U], 2);

    /* S-Function (sfix_bitop): '<S5>/Bitwise OR' */
    MultiWordIor(&rtb_y_m.chunks[0U], &rtb_DataTypeConversion.chunks[0U],
                 &tmp_7.chunks[0U], 2);
    MultiWordIor(&tmp_7.chunks[0U], &rtb_y.chunks[0U], &tmp_6.chunks[0U], 2);
    MultiWordIor(&tmp_6.chunks[0U], &rtb_y_ko.chunks[0U], &tmp_5.chunks[0U], 2);
    MultiWordIor(&tmp_5.chunks[0U], &rtb_y_l.chunks[0U], &tmp_4.chunks[0U], 2);
    MultiWordIor(&tmp_4.chunks[0U], &rtb_y_j.chunks[0U],
                 &CanM_MsgM_Y.SECC_MSG2_OutU64.chunks[0U], 2);
    MultiWordIor(&CanM_MsgM_Y.SECC_MSG2_OutU64.chunks[0U], &rtb_y_ay.chunks[0U],
                 &tmp_3.chunks[0U], 2);
    MultiWordIor(&tmp_3.chunks[0U], &rtb_y_f.chunks[0U],
                 &CanM_MsgM_Y.SECC_MSG2_OutU64.chunks[0U], 2);

    /* Switch: '<S98>/FixPt Switch' incorporates:
     *  Constant: '<S97>/FixPt Constant'
     *  Constant: '<S98>/Constant'
     *  Sum: '<S97>/FixPt Sum1'
     *  UnitDelay: '<S89>/Output'
     */
    if ((uint8_T)(CanM_MsgM_B.Output_h + 1) > 15) {
      CanM_MsgM_DW.Output_DSTATE_m = 0U;
    } else {
      CanM_MsgM_DW.Output_DSTATE_m = (uint8_T)(CanM_MsgM_B.Output_h + 1);
    }

    /* End of Switch: '<S98>/FixPt Switch' */
  }

  /* End of Inport: '<Root>/SECC_MSG_Enable' */
  /* End of Outputs for SubSystem: '<Root>/SECC_MSG2' */

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift' */
  /* Inport: '<Root>/MCU_Status3_Data1' */
  CanM_MsgM_BitShift(CanM_MsgM_U.MCU_Status3_Data1, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift' */

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND1' */
  tmp_4 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_3.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion1'
   */
  CanM_MsgM_B.BusCreator1_l.Data[1] = (uint8_T)MultiWord2uLong(&tmp_3.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift1' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift1' */

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND2' */
  tmp_5 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_4.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion2'
   */
  CanM_MsgM_B.BusCreator1_l.Data[2] = (uint8_T)MultiWord2uLong(&tmp_4.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift2' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift2' */

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND3' */
  tmp_6 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_5.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion3'
   */
  CanM_MsgM_B.BusCreator1_l.Data[3] = (uint8_T)MultiWord2uLong(&tmp_5.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift3' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift3' */

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND4' */
  tmp_7 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_6.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion4'
   */
  CanM_MsgM_B.BusCreator1_l.Data[4] = (uint8_T)MultiWord2uLong(&tmp_6.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift4' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift4' */

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND5' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_7.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion5'
   */
  CanM_MsgM_B.BusCreator1_l.Data[5] = (uint8_T)MultiWord2uLong(&tmp_7.chunks[0U]);

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift5' */
  CanM_MsgM_BitShift(rtb_y_m, &rtb_y_f);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift5' */

  /* Outputs for Atomic SubSystem: '<S25>/Bit Shift6' */
  CanM_MsgM_BitShift(rtb_y_f, &rtb_y_m);

  /* End of Outputs for SubSystem: '<S25>/Bit Shift6' */

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  Constant: '<S2>/Constant26'
   *  Constant: '<S2>/Constant27'
   *  Constant: '<S2>/Constant28'
   *  Constant: '<S2>/Constant29'
   *  Constant: '<S2>/Constant3'
   *  Constant: '<S2>/Constant30'
   */
  CanM_MsgM_B.BusCreator1_l.Extended = 1U;
  CanM_MsgM_B.BusCreator1_l.Length = 8U;
  CanM_MsgM_B.BusCreator1_l.Remote = 0U;
  CanM_MsgM_B.BusCreator1_l.Error = 0U;
  CanM_MsgM_B.BusCreator1_l.ID = 419406575U;
  CanM_MsgM_B.BusCreator1_l.Timestamp = 0.0;

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND' incorporates:
   *  Inport: '<Root>/MCU_Status3_Data1'
   */
  tmp_0 = CanM_MsgM_U.MCU_Status3_Data1;
  MultiWordAnd(&CanM_MsgM_U.MCU_Status3_Data1.chunks[0U],
               &CanM_MsgM_ConstP.pooled12.chunks[0], &rtb_y.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion'
   */
  CanM_MsgM_B.BusCreator1_l.Data[0] = (uint8_T)MultiWord2uLong(&rtb_y.chunks[0U]);

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND6' */
  rtb_y = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_f.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &tmp_0.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion6'
   */
  CanM_MsgM_B.BusCreator1_l.Data[6] = (uint8_T)MultiWord2uLong(&tmp_0.chunks[0U]);

  /* S-Function (sfix_bitop): '<S25>/Bitwise AND7' */
  tmp_0 = CanM_MsgM_ConstP.pooled12;
  MultiWordAnd(&rtb_y_m.chunks[0U], &CanM_MsgM_ConstP.pooled12.chunks[0],
               &rtb_y.chunks[0U], 2);

  /* BusCreator: '<S2>/Bus Creator1' incorporates:
   *  DataTypeConversion: '<S25>/Data Type Conversion7'
   */
  CanM_MsgM_B.BusCreator1_l.Data[7] = (uint8_T)MultiWord2uLong(&rtb_y.chunks[0U]);

  /* S-Function (scanunpack): '<S2>/CAN Unpack' */
  {
    /* S-Function (scanunpack): '<S2>/CAN Unpack' */
    if ((8 == CanM_MsgM_B.BusCreator1_l.Length) && (CanM_MsgM_B.BusCreator1_l.ID
         != INVALID_CAN_ID) ) {
      if ((419406575 == CanM_MsgM_B.BusCreator1_l.ID) && (1U ==
           CanM_MsgM_B.BusCreator1_l.Extended) ) {
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
                    (CanM_MsgM_B.BusCreator1_l.Data[7]) & (uint8_T)(0xF0U)) >> 4);
                }

                unpackedValue = tempValue;
              }

              outValue = (real_T) (unpackedValue);
            }

            {
              real_T result = (real_T) outValue;
              CanM_MsgM_B.CANUnpack_o1_o = result;
            }
          }

          /* --------------- START Unpacking signal 1 ------------------
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

          /* --------------- START Unpacking signal 2 ------------------
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

          /* --------------- START Unpacking signal 3 ------------------
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
           *  startBit                = 33
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
           *  startBit                = 27
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
           *  startBit                = 35
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
           *  startBit                = 32
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
                  tempValue = tempValue | (uint8_T)((uint8_T)
                    (CanM_MsgM_B.BusCreator1_l.Data[4]) & (uint8_T)(0x1U));
                }

                unpackedValue = tempValue;
              }

              outValue = (real_T) (unpackedValue);
            }

            {
              real_T result = (real_T) outValue;
              CanM_MsgM_B.CANUnpack_o9_e = result;
            }
          }

          /* --------------- START Unpacking signal 9 ------------------
           *  startBit                = 24
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

          /* --------------- START Unpacking signal 11 ------------------
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

          /* --------------- START Unpacking signal 12 ------------------
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

          /* --------------- START Unpacking signal 13 ------------------
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

          /* --------------- START Unpacking signal 14 ------------------
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

          /* --------------- START Unpacking signal 15 ------------------
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

          /* --------------- START Unpacking signal 16 ------------------
           *  startBit                = 26
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

          /* --------------- START Unpacking signal 18 ------------------
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

          /* --------------- START Unpacking signal 19 ------------------
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

          /* --------------- START Unpacking signal 20 ------------------
           *  startBit                = 25
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
        }
      }
    }
  }

  /* Outport: '<Root>/MCU_State3ReqChargingEnable1' incorporates:
   *  DataTypeConversion: '<S2>/Data Type Conversion1'
   */
  CanM_MsgM_Y.MCU_State3ReqChargingEnable1 = (CanM_MsgM_B.CANUnpack_o9_e != 0.0);

  /* DataTypeConversion: '<S2>/Data Type Conversion' */
  tmp = floor(CanM_MsgM_B.CANUnpack_o1_o);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 256.0);
  }

  if (tmp < 0.0) {
    rtb_DataTypeConversion_o = (uint8_T)-(int8_T)(uint8_T)-tmp;
  } else {
    rtb_DataTypeConversion_o = (uint8_T)tmp;
  }

  /* End of DataTypeConversion: '<S2>/Data Type Conversion' */

  /* Outport: '<Root>/MCU_State3ValidStatus1' incorporates:
   *  Constant: '<S24>/Constant'
   *  Constant: '<S26>/Constant'
   *  Delay: '<S24>/Delay'
   *  RelationalOperator: '<S26>/Compare'
   *  Sum: '<S24>/Add'
   *  Sum: '<S24>/Subtract'
   */
  CanM_MsgM_Y.MCU_State3ValidStatus1 = ((uint8_T)((uint8_T)
    (rtb_DataTypeConversion_o - CanM_MsgM_DW.Delay_DSTATE) - 1) <= 2);

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
   *  S-Function (sfix_bitop): '<S25>/Bitwise AND7'
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
    /* Sum: '<S99>/Subtract' incorporates:
     *  Constant: '<S99>/Constant1'
     *  Inport: '<Root>/EVSE_RatedCurrent1'
     */
    CanM_MsgM_B.Subtract = (int16_T)(CanM_MsgM_U.EVSE_RatedCurrent1 - 2000U);

    /* UnitDelay: '<S107>/Output' */
    CanM_MsgM_B.Output = CanM_MsgM_DW.Output_DSTATE;

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
          uint32_T result = (uint32_T) (CanM_MsgM_U.EVSE_ChargingEndReason1);

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
              CanM_MsgM_B.SECC_MSG2Pack.Data[0] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[0] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0xFU));
            }
          }
        }
      }

      /* --------------- START Packing signal 2 ------------------
       *  startBit                = 20
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 3 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 4 ------------------
       *  startBit                = 26
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[3] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x3U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 5 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 6 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 2));
            }
          }
        }
      }

      /* --------------- START Packing signal 7 ------------------
       *  startBit                = 17
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 8 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 1));
            }
          }
        }
      }

      /* --------------- START Packing signal 9 ------------------
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
                (packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 10 ------------------
       *  startBit                = 22
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 11 ------------------
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
                (packedValue & (uint8_T)0x1U) << 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 12 ------------------
       *  startBit                = 42
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
          int32_T result = (int32_T) (CanM_MsgM_B.Subtract);

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
              CanM_MsgM_B.SECC_MSG2Pack.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x3FU) << 2));
              CanM_MsgM_B.SECC_MSG2Pack.Data[6] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[6] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0xFC0U) >> 6));
            }
          }
        }
      }

      /* --------------- START Packing signal 13 ------------------
       *  startBit                = 32
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[4] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[4] | (uint8_T)((uint16_T)
                (packedValue & (uint16_T)0xFFU));
              CanM_MsgM_B.SECC_MSG2Pack.Data[5] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[5] | (uint8_T)((uint16_T)
                ((uint16_T)(packedValue & (uint16_T)0x300U) >> 8));
            }
          }
        }
      }

      /* --------------- START Packing signal 14 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[1] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 3));
            }
          }
        }
      }

      /* --------------- START Packing signal 15 ------------------
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U));
            }
          }
        }
      }

      /* --------------- START Packing signal 16 ------------------
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
                CanM_MsgM_B.SECC_MSG2Pack.Data[1] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }

      /* --------------- START Packing signal 17 ------------------
       *  startBit                = 28
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[3] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x7U) << 4));
            }
          }
        }
      }

      /* --------------- START Packing signal 18 ------------------
       *  startBit                = 23
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
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 7));
            }
          }
        }
      }

      /* --------------- START Packing signal 19 ------------------
       *  startBit                = 24
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[3] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[3] | (uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x3U));
            }
          }
        }
      }

      /* --------------- START Packing signal 20 ------------------
       *  startBit                = 21
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
              CanM_MsgM_B.SECC_MSG2Pack.Data[2] =
                CanM_MsgM_B.SECC_MSG2Pack.Data[2] | (uint8_T)((uint8_T)((uint8_T)
                (packedValue & (uint8_T)0x1U) << 5));
            }
          }
        }
      }
    }

    /* DataTypeConversion: '<S6>/Data Type Conversion1' */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[1], &tmp_0.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift' */
    CanM_MsgM_BitShift_e(tmp_0, &rtb_y_m);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift' */

    /* DataTypeConversion: '<S6>/Data Type Conversion2' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion1'
     *  S-Function (sfix_bitop): '<S25>/Bitwise AND7'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[2], &tmp_8.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift1' */
    CanM_MsgM_BitShift1(tmp_8, &rtb_y_f);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift1' */

    /* DataTypeConversion: '<S6>/Data Type Conversion3' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion2'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[3], &tmp_9.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift2' */
    CanM_MsgM_BitShift2(tmp_9, &rtb_y_ay);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift2' */

    /* DataTypeConversion: '<S6>/Data Type Conversion4' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion3'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[4], &tmp_a.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift3' */
    CanM_MsgM_BitShift3(tmp_a, &rtb_y_j);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift3' */

    /* DataTypeConversion: '<S6>/Data Type Conversion5' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion4'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[5], &tmp_b.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift4' */
    CanM_MsgM_BitShift4(tmp_b, &rtb_y_l);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift4' */

    /* DataTypeConversion: '<S6>/Data Type Conversion6' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion5'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[6], &tmp_c.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift5' */
    CanM_MsgM_BitShift5(tmp_c, &rtb_y_ko);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift5' */

    /* DataTypeConversion: '<S6>/Data Type Conversion7' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion6'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[7], &tmp_d.chunks[0U], 2);

    /* Outputs for Atomic SubSystem: '<S6>/Bit Shift6' */
    CanM_MsgM_BitShift6(tmp_d, &rtb_y);

    /* End of Outputs for SubSystem: '<S6>/Bit Shift6' */

    /* DataTypeConversion: '<S6>/Data Type Conversion' incorporates:
     *  DataTypeConversion: '<S6>/Data Type Conversion7'
     */
    uLong2MultiWord(CanM_MsgM_B.SECC_MSG2Pack.Data[0], &tmp_j.chunks[0U], 2);

    /* S-Function (sfix_bitop): '<S6>/Bitwise OR' */
    MultiWordIor(&tmp_j.chunks[0U], &rtb_y_m.chunks[0U], &tmp_i.chunks[0U], 2);
    MultiWordIor(&tmp_i.chunks[0U], &rtb_y_f.chunks[0U], &tmp_h.chunks[0U], 2);
    MultiWordIor(&tmp_h.chunks[0U], &rtb_y_ay.chunks[0U], &tmp_g.chunks[0U], 2);
    MultiWordIor(&tmp_g.chunks[0U], &rtb_y_j.chunks[0U], &tmp_f.chunks[0U], 2);
    MultiWordIor(&tmp_f.chunks[0U], &rtb_y_l.chunks[0U],
                 &CanM_MsgM_Y.SECC_MSG2_OutU64_1.chunks[0U], 2);
    MultiWordIor(&CanM_MsgM_Y.SECC_MSG2_OutU64_1.chunks[0U], &rtb_y_ko.chunks[0U],
                 &tmp_e.chunks[0U], 2);
    MultiWordIor(&tmp_e.chunks[0U], &rtb_y.chunks[0U],
                 &CanM_MsgM_Y.SECC_MSG2_OutU64_1.chunks[0U], 2);

    /* Switch: '<S116>/FixPt Switch' incorporates:
     *  Constant: '<S115>/FixPt Constant'
     *  Constant: '<S116>/Constant'
     *  Sum: '<S115>/FixPt Sum1'
     *  UnitDelay: '<S107>/Output'
     */
    if ((uint8_T)(CanM_MsgM_B.Output + 1) > 15) {
      CanM_MsgM_DW.Output_DSTATE = 0U;
    } else {
      CanM_MsgM_DW.Output_DSTATE = (uint8_T)(CanM_MsgM_B.Output + 1);
    }

    /* End of Switch: '<S116>/FixPt Switch' */
  }

  /* End of Inport: '<Root>/SECC_MSG_Enable1' */
  /* End of Outputs for SubSystem: '<Root>/SECC_MSG2_1' */

  /* Outport: '<Root>/MCU_State3ReqChargingEnable' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   */
  CanM_MsgM_Y.MCU_State3ReqChargingEnable = (CanM_MsgM_B.CANUnpack_o9 != 0.0);

  /* DataTypeConversion: '<S1>/Data Type Conversion' */
  tmp = floor(CanM_MsgM_B.CANUnpack_o1);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 256.0);
  }

  if (tmp < 0.0) {
    rtb_DataTypeConversion_j = (uint8_T)-(int8_T)(uint8_T)-tmp;
  } else {
    rtb_DataTypeConversion_j = (uint8_T)tmp;
  }

  /* End of DataTypeConversion: '<S1>/Data Type Conversion' */

  /* Outport: '<Root>/MCU_State3ValidStatus' incorporates:
   *  Constant: '<S7>/Constant'
   *  Constant: '<S9>/Constant'
   *  Delay: '<S7>/Delay'
   *  RelationalOperator: '<S9>/Compare'
   *  Sum: '<S7>/Add'
   *  Sum: '<S7>/Subtract'
   */
  CanM_MsgM_Y.MCU_State3ValidStatus = ((uint8_T)((uint8_T)
    (rtb_DataTypeConversion_j - CanM_MsgM_DW.Delay_DSTATE_a) - 1) <= 2);

  /* Update for Delay: '<S24>/Delay' */
  CanM_MsgM_DW.Delay_DSTATE = rtb_DataTypeConversion_o;

  /* Update for Delay: '<S7>/Delay' */
  CanM_MsgM_DW.Delay_DSTATE_a = rtb_DataTypeConversion_j;
}

/* Model initialize function */
void CanM_MsgM_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* Start for S-Function (scanunpack): '<S1>/CAN Unpack' */

  /*-----------S-Function Block: <S1>/CAN Unpack -----------------*/

  /* Start for S-Function (scanunpack): '<S2>/CAN Unpack' */

  /*-----------S-Function Block: <S2>/CAN Unpack -----------------*/

  /* SystemInitialize for Enabled SubSystem: '<Root>/SECC_MSG1' */

  /* SystemInitialize for Inport: '<Root>/SECC_MSG1_L1_Vol' incorporates:
   *  Inport: '<Root>/SECC_MSG1_L2_Vol'
   *  Inport: '<Root>/SECC_MSG1_L3_Vol'
   */

  /* End of SystemInitialize for SubSystem: '<Root>/SECC_MSG1' */

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
