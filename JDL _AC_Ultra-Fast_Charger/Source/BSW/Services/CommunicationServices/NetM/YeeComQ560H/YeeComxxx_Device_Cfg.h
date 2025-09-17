#ifndef _YEECOM_XXX_DEVICE_CFG_H_
#define _YEECOM_XXX_DEVICE_CFG_H_

#include "YeeComxxx_At.h"
#include "stm32f1xx.h"

#define YEECOM_PERIOD_MS                        (50)
#define YEECOM_CFG_PARAMETER_MAX_ERROR          (3)
#define YEECOM_DEFAULT_REMOTE_IP                "47.94.253.148"
#define YEECOM_DEFAULT_REMOTE_PORT              (1883u)
#define YEECOM_DEFAULT_NET_TYPE                 (YEECOM_WORKING_MQTT)

#define YEECOM_ICCID_LENGTH                     (20U)
#define YEECOM_IMEI_LENGTH                      (15U)

#define YEECOM_PERIODIC_TASK_PERIOD             (60000 / YEECOM_PERIOD_MS)   // 60s

#define YEECOM_ATOMIC_SETH_BIT(REG, BIT)        ATOMIC_SETH_BIT(REG, BIT)
#define YEECOM_ATOMIC_CLR_BIT(REG, BIT)         ATOMIC_CLEARH_BIT(REG, BIT)

#endif
