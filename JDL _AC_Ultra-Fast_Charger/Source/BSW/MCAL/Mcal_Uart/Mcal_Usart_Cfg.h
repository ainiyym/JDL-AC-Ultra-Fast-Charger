/*
 * Mcal_Usart_Cfg.h
 * DMA driver source file
 */

#if !defined (MCAL_USART_CFG_H_)
#define MCAL_USART_CFG_H_

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "stm32f1xx_hal.h"  // Include the HAL library for STM32F1xx
#include "usart.h"
#include "Mcal_MCUCore.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MCAL_USART1_CH_RCV_CYCBUF_LEN (1024U)
#define MCAL_USART2_CH_RCV_CYCBUF_LEN (1024U)
#define MCAL_USART4_CH_RCV_CYCBUF_LEN (256U)
#define MCAL_USART5_CH_RCV_CYCBUF_LEN (256U)
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef enum {
	MCAL_USART1_CH = 0U ,   	     /* 4G */
	MCAL_USART2_CH ,              /* LOG */
  MCAL_USART4_CH ,              /* Meter */
  MCAL_USART5_CH ,              /* Fan speeder */
	MCAL_USART_MAX_NUMBER
}McalUsartChannel_Enum_t;

typedef struct
{
  McalUsartChannel_Enum_t UsartNum; /* USART通道号 */
  UART_HandleTypeDef* UsartBase;    /* USART句柄 */
} McalUsart_NumMapUsart_t;

typedef struct
{
  uint8_t UsartNum; /* USART通道号 */
  uint32_t RcvBufLen; /* 接收缓冲区长度 */
}McalUsart_BufCfg_t;

typedef struct
{
  uint32_t baud_rate;
  uint32_t data_width;
  uint32_t parity;
  uint32_t stop_bits;
  uint32_t flow_control;
  uint32_t mode;
} McalUart_ReConfig_t;
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Union Definition
|******************************************************************************/

/*******************************************************************************
|    Struct Definition+
|******************************************************************************/
typedef struct
{
  uint8_t Send_Lock;
  uint16_t RcvIntSwapBufDataCnt;
  uint8_t* RcvIntSwapBuf; // 交换缓冲区,动态分配
  uint16_t RcvIntSwapBufSize;
  uint32_t SenLen;
} McalUsart_Ctrol_t;
/*******************************************************************************
|    Constant Definition
|******************************************************************************/

/*******************************************************************************
|    Extern variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Extern functions Declaration
|******************************************************************************/
extern void McalUsart_ReInit(McalUsartChannel_Enum_t UsartNum, McalUart_ReConfig_t *config);
extern void McalUsart_CycBuffCfgInit(void);
// USART initialization
extern void Mcal_Usart_Init(void);
// USART enable
extern void Mcal_Usart_IT_Enable(void);
// USART disable
extern void Mcal_Usart_Disable(void);
// USART receive data
extern uint32_t Mcal_Usart_AppCheckData(uint32_t USART);
extern uint32_t Mcal_Usart_AppReceiveData(uint32_t USART, uint8_t *data, uint32_t size);
// USART send data
extern McalRetVal_t Mcal_Usart_AppSendData(uint32_t USART, uint8_t *data, uint32_t size);
// USART send interrupt callback
extern void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
#endif
/*EOF*/
