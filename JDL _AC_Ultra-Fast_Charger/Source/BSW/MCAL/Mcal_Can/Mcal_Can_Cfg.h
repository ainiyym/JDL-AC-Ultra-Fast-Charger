#ifndef MCAL_CAN_CFG_H
#define MCAL_CAN_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ================== Includes ================== */
#include "stm32f1xx_hal.h"  // Include the HAL library for STM32F1xx
#include "Mcal_Types.h"

/* ================== Macros ================== */
#define MCAL_CAN1_RX_CYC_LEN 256

/* CAN test cfg */
#define MCAL_CAN1_RX_ID 0x1800D8D0
#define MCAL_CAN1_TX_ID 0x1800D8D9

#define MCAL_CAN1_FILTER_ID_HIGH (((uint32_t)MCAL_CAN1_RX_ID << 3) & 0xFFFF0000) >> 16                  // High-level ID
#define MCAL_CAN1_FILTER_ID_LOW (((uint32_t)MCAL_CAN1_RX_ID << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF // Low-level ID
#define MCAL_CAN1_FILTER_MASK_ID_HIGH (0xffff)                                                          // Filter mask high-level ID
#define MCAL_CAN1_FILTER_MASK_ID_LOW ((0xfff0 << 3) | CAN_ID_EXT | CAN_RTR_DATA)                        // Filter mask low-level ID

/* CAN CCP cfg */
#define MCAL_CAN1_CCP_RX_ID 0x2E1
#define MCAL_CAN1_CCP_TX_ID 0x2E2

#define MCAL_CAN1_CCP_FILTER_ID_HIGH ((uint16_t)MCAL_CAN1_CCP_RX_ID << 5)                               // High-level ID
#define MCAL_CAN1_CCP_FILTER_ID_LOW ((uint16_t)MCAL_CAN1_CCP_TX_ID << 5)                                // Low-level ID
#define MCAL_CAN1_CCP_FILTER_MASK_ID_HIGH (0xffff)                                                 // Filter mask high-level ID
#define MCAL_CAN1_CCP_FILTER_MASK_ID_LOW (0xffff)                                                  // Filter mask low-level ID

/* ================== Enum ================== */
typedef enum
{
    MCAL_CAN1_RX_CH,       /* CAN1 test */
    MCAL_CAN1_RX_CCP,     /* CCP RX CH */
    MCAL_CAN_RX_MAX_NUMBER /* CAN NUM */
} Mcal_CanRxChannel_Enum_t;

typedef enum
{
    MCAL_CAN1_TX_CH,       /* CAN1 test */
    MCAL_CAN1_TX_CCP,     /* CCP TX CH */
    MCAL_CAN_TX_MAX_NUMBER /* CAN NUM */
} Mcal_CanTxChannel_Enum_t;

/* ================== Type Definitions ================== */
typedef struct
{
    Mcal_CanTxChannel_Enum_t CanChannel; /* Can Tx channal */
    CAN_HandleTypeDef *CanHandle;        /* Can Handle */
} Mcal_CanTxChannelCfg_t;

typedef struct
{
    Mcal_CanRxChannel_Enum_t CanChannel; /* Can Rx channal */
    CAN_HandleTypeDef *CanHandle;        /* Can Handle */
    CAN_FilterTypeDef FilterConfig;      /* Can Filter Configuration */
} Mcal_CanFilterCfg_t;

typedef struct
{
    uint8_t RcvCycBufID;                     /* Receive CycBuf ID */
    uint8_t RcvCycBuf[MCAL_CAN1_RX_CYC_LEN]; /* Receive the circular buffer */
    uint32_t RcvCycBufLen;                   /* Receive the length of the circular buffer */
} Mcal_Can_RcvBuf_Cfg_t;

typedef struct
{
    uint8_t sendData[8];                               /* Ready to send data */
    uint8_t rcvData[8];                                /* Received data */
    CAN_RxHeaderTypeDef RxHeader;                      /* Receive handle */
    Mcal_Can_RcvBuf_Cfg_t Buf[MCAL_CAN_RX_MAX_NUMBER]; /* Rcv cyc buf Configuration */
} Mcal_CanCtrlCfg_t;

/* ================== Extern Declarations ================== */

/* ================== Function Prototypes ================== */
void Mcal_Can_Init(void);
void Mcal_Can_Enable(void);
void Mcal_Can_Disable(void);
extern void Mcal_Can_Filter_Init(void);                                                                                                                 /* Filter configuration function */
extern void Mcal_Can_RxBufCfg_Init(void);                                                                                                               /* Can Rx buff configuration init func */
extern McalRetVal_t Mcal_Can_Send_Msg(Mcal_CanTxChannel_Enum_t Channel, CAN_TxHeaderTypeDef TxHeader, uint32_t *pTxMailbox, uint8_t *msg, uint8_t len); /* Send data function */
extern uint32_t Mcal_Can_Receive_Msg(Mcal_CanRxChannel_Enum_t Channel, uint8_t *data, uint32_t size);                                                   /* Receive data function */
#ifdef __cplusplus
}
#endif

#endif /* MCAL_CAN_CFG_H */
