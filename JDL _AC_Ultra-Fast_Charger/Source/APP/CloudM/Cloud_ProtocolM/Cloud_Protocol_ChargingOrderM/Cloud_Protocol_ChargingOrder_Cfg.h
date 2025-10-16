//******************************************************************************
//* File Name: Cloud_Protocol_ChargingOrder_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol Charging Order module header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "STD_Rtc.h"
#include "FlashDB_AppM.h"
#include "Cloud_EV_Charger_Information.h"
#include "Cloud_Protocol_BillingModelM.h"
#include "Cloud_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_PROTOCOL_CHARGING_ORDER_MAX_GUNS                                  (CLOUD_EV_MAX_CONNECTORS) // Maximum number of guns supported by the order module

#define CLOUD_PROTOCOL_GET_CURRENT_DATE_TIME(pData)                  \
    do                                                               \
    {                                                                \
        RtcTimedate_Struct lv_stDateTime;                            \
        RTCIF_GetDateTime(&lv_stDateTime);                           \
        memcpy((pData), &lv_stDateTime, sizeof(RtcTimedate_Struct)); \
    } while (0) // get datetime: YYMMDDHHMMSS

#define CLOUD_PROTOCOL_GET_CURRENT_TIMESTAMP() \
    ({                                         \
        uint32_t __timestamp = 0;              \
        RTC_GetRtcSeconds(&__timestamp);       \
        __timestamp;                           \
    }) // get timestamp: seconds since 1970-01-01 00:00:00

#define CLOUD_PROTOCOL_TIMESTAMP_TO_CP56TIME2A(pTimestamp, pCp56_data)         \
    ({                                                                         \
        if ((pTimestamp) != NULL && (pCp56_data) != NULL)                      \
        {                                                                      \
            RtcTimedate_Struct lv_sttimeDate = {0};                            \
            RTC_ConvertSecondsToTimeDate((pTimestamp), &lv_sttimeDate);        \
            RTC_DatetimeToCp56time2a(&lv_sttimeDate, (uint8_t *)(pCp56_data)); \
        }                                                                      \
    }) // convert timestamp to CP56Time2a

#define CLOUD_PROTOCOL_GET_CP56TIME2A(pCp56_data)                               RTC_GetCP56Time2a((uint8_t *)(pCp56_data)) // get CP56Time2a(BCD)
#define CLOUD_PROTOCOL_GET_BILLING_MODEL_TIME_SLOT_INFO()                       Cloud_Protocol_GetBillingModelTimeSlotInfo() // get billing model time slot info
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern uint32_t cloud_protocol_fixed_5dec(float value);
extern uint32_t cloud_protocol_fixed_4dec(float value);
extern uint64_t cloud_protocol_fixed_4dec_64(float value);
extern float fixed_5dec_to_float(uint32_t value);
extern float fixed_4dec_to_float(uint32_t value);

#endif /* __CLOUD_PROTOCOL_CHARGING_ORDER_CFG_H */
/* EOL */
