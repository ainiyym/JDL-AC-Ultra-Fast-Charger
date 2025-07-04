#ifndef CCP_CAN_DRV_H
#define CCP_CAN_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Mcal_Can_Cfg.h"
#include "Comm.h"
#include "STD_LogService.h"

#define CCP_DEBUG(fmt, ...) LOG_DEBUG(LOG_MODULE_CCP, fmt, ##__VA_ARGS__) /* log output */
#define CCP_INFO(fmt, ...)  LOG_INFO(LOG_MODULE_CCP, fmt, ##__VA_ARGS__)  /* log output */
#define CCP_WARN(fmt, ...)  LOG_WARN(LOG_MODULE_CCP, fmt, ##__VA_ARGS__)  /* log output */
#define CCP_ERROR(fmt, ...) LOG_ERROR(LOG_MODULE_CCP, fmt, ##__VA_ARGS__) /* log output */
#define CCP_CRITICAL(fmt, ...) LOG_CRITICAL(LOG_MODULE_CCP, fmt, ##__VA_ARGS__) /* log output */
#define CCP_PRINT_HEX(BUFF, LEN, R) LogService_Print_Hex_Array(LOG_MODULE_CCP, BUFF, LEN, R)  /* print hex array */

/* CAN 驱动初始化 */
extern void Ccp_CanDrv_Init(void);

/* 发送 CAN 消息 */
extern uint8_t Ccp_CanDrv_Transmit(const uint8_t *pData, uint8_t length);

extern void Ccp_CanDrv_ReceiveCallback(uint32_t id, const uint8_t *data, uint8_t len);
#ifdef __cplusplus
}
#endif

#endif /* CCP_CAN_DRV_H */
