/*
 * File: CanM_Evse_Cfg.h
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

#ifndef RTW_HEADER_CanM_Evse_Cfg_h_
#define RTW_HEADER_CanM_Evse_Cfg_h_
#include "rtwtypes.h"

typedef int32_T CAN_MODE_STATUS_ENUM;

/* enum CAN_MODE_STATUS_ENUM */
#define CAN_MODE_STATUS_ENUM_CAN_MODE_STATUS_INIT (0)            /* Default value */
#define CAN_MODE_STATUS_ENUM_ENTER     (1)
#define CAN_MODE_STATUS_ENUM_EXIT      (2)

typedef uint32_T CHARGING_BEFORE_TEST_STATUS_ENUM;

/* enum CHARGING_BEFORE_TEST_STATUS_ENUM */
#define CHARGING_BEFORE_TEST_STATUS_ENUM_CHARGING_BEFORE_TEST_STATUS_INIT (0U) /* Default value */
#define CHARGING_BEFORE_TEST_STATUS_ENUM_SUCCESS (1U)
#define CHARGING_BEFORE_TEST_STATUS_ENUM_FAIL (2U)

typedef uint32_T ENABLE_STATUS_ENUM;

/* enum ENABLE_STATUS_ENUM */
#define ENABLE_STATUS_ENUM_DISABLE     (0U)                      /* Default value */
#define ENABLE_STATUS_ENUM_ENABLE      (1U)
#endif                                 /* RTW_HEADER_CanM_Evse_Cfg_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
