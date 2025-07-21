/*
 * File: CanM_MsgM_private.h
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

#ifndef RTW_HEADER_CanM_MsgM_private_h_
#define RTW_HEADER_CanM_MsgM_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "CanM_MsgM.h"
#include "CanM_MsgM_types.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

extern void uMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
extern void uMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
extern void uLong2MultiWord(uint32_T u, uint32_T y[], int32_T n);
extern void MultiWordIor(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
extern uint32_T MultiWord2uLong(const uint32_T u[]);
extern void MultiWordAnd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
extern void CanM_MsgM_BitShift(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift_e(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift1(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift2(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift3(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift4(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift5(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_BitShift6(uint64m_T rtu_u, uint64m_T *rty_y);
extern void CanM_MsgM_SECC_MSG1(boolean_T rtu_Enable, uint16_T
  rtu_SECC_MSG1_L1_Curr, uint16_T rtu_SECC_MSG1_L1_Vol, uint16_T
  rtu_SECC_MSG1_L2_Curr, uint16_T rtu_SECC_MSG1_L2_Vol, uint16_T
  rtu_SECC_MSG1_L3_Curr, uint16_T rtu_SECC_MSG1_L3_Vol, uint64m_T
  *rty_SECC_MSG1_OutU64, B_SECC_MSG1_CanM_MsgM_T *localB,
  DW_SECC_MSG1_CanM_MsgM_T *localDW);

#endif                                 /* RTW_HEADER_CanM_MsgM_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
