//****************************************************************************************
//*
//* File Name: Mcal_Usart_Cfg.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
/* polyspace:begin<MISRA-C3:5.6:Not a defect:Justified> Normal */

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "MessageBuffStack.h"
#include "Mcal_Usart_Cfg.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void HAL_UART_IdleCallback(UART_HandleTypeDef *huart, uint16_t Size);
/*******************************************************************************
|    Global Variable with extern linkage
|******************************************************************************/
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
 /* 4G */
static uint8_t McalUsart_SendCycBuf[MCAL_USART1_CH_SEND_CYCBUF_LEN];
static uint8_t McalUsart_SendBuf[MCAL_USART1_CH_SEND_BUF_LEN];
 /* LOG */
static uint8_t McalUsart_SendCycBuf2[MCAL_USART2_CH_SEND_CYCBUF_LEN];
static uint8_t McalUsart_SendBuf2[MCAL_USART2_CH_SEND_BUF_LEN];

McalUsart_Ctrol_t McalUsart_Ctrl[MCAL_USART_MAX_NUMBER] = {0};

const McalUsart_NumMapUsart_t  McalUsart_NumMapUsart[MCAL_USART_MAX_NUMBER] =
    {
        {MCAL_USART1_CH, &huart1},
        {MCAL_USART2_CH, &huart2},
        {MCAL_USART4_CH, &huart4},
        {MCAL_USART5_CH, &huart5}
    };

const MessageBuffChannel_Enum_t Mcal_UsartMapChannel[MCAL_USART_MAX_NUMBER] = {
    MESSAGE_USART1_CH, /* 4G */
    MESSAGE_USART2_CH, /* LOG */
    MESSAGE_USART4_CH, /* Meter */
    MESSAGE_USART5_CH  /* Fan speeder */
};

McalUsart_BufCfg_t  const McalUsart_BufferCfg[MCAL_USART_MAX_NUMBER] =
{
    {
        .UsartNum = MCAL_USART1_CH,
        .SendCycBuf = McalUsart_SendCycBuf,
        .SendCycBufLen = MCAL_USART1_CH_SEND_CYCBUF_LEN,
        .SendBuf = McalUsart_SendBuf,
        .SendBufLen = MCAL_USART1_CH_SEND_BUF_LEN,
        .RcvBufLen = MCAL_USART1_CH_RCV_CYCBUF_LEN
    },
    {
        .UsartNum = MCAL_USART2_CH,
        .SendCycBuf = McalUsart_SendCycBuf2,
        .SendCycBufLen = MCAL_USART2_CH_SEND_CYCBUF_LEN,
        .SendBuf = McalUsart_SendBuf2,
        .SendBufLen = MCAL_USART2_CH_SEND_BUF_LEN,
        .RcvBufLen = MCAL_USART2_CH_RCV_CYCBUF_LEN
    },
    {
        .RcvBufLen = MCAL_USART4_CH_RCV_CYCBUF_LEN
    },
    {
        .RcvBufLen = MCAL_USART5_CH_RCV_CYCBUF_LEN
    }
};
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
void McalUsart_ReInit(McalUsartChannel_Enum_t UsartNum, McalUart_ReConfig_t *config)
{
  if (UsartNum < MCAL_USART_MAX_NUMBER && config != NULL)
  {
    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)&McalUsart_NumMapUsart[UsartNum].UsartBase;

    if (HAL_OK == HAL_UART_DeInit(huart))
    {
      huart->Init.BaudRate = config->baud_rate;
      huart->Init.WordLength = config->data_width;
      huart->Init.StopBits = config->stop_bits;
      huart->Init.Parity = config->parity;
      huart->Init.Mode = config->mode;
      huart->Init.HwFlowCtl = config->flow_control;
      if (HAL_OK != HAL_UART_Init(huart))
      {
        Error_Handler();
      }
    }
  }
}

void McalUsart_CycBuffCfgInit(void)
{
  uint32_t CycBufRet;
  uint8_t i;

  /* 初始化接收缓冲 */
  for (i = MCAL_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
  {
    McalUsart_Ctrl[i].RcvIntSwapBufSize = McalUsart_BufferCfg[i].RcvBufLen;
    McalUsart_Ctrl[i].RcvIntSwapBuf = MCAL_MALLOC(McalUsart_Ctrl[i].RcvIntSwapBufSize);
  }

  /* 4G和LOG使用发送缓冲区 */
  for (i = MCAL_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
  {
    McalUsart_Ctrl[i].SendBuf = McalUsart_BufferCfg[i].SendBuf;
    McalUsart_Ctrl[i].SendBufLen = McalUsart_BufferCfg[i].SendBufLen;
    /* 发送缓存申请 */
    CycBufRet = MCAL_CYCBUF_OPEN_CHAN(&McalUsart_Ctrl[i].SendCycBufID,
                                      McalUsart_BufferCfg[i].SendCycBuf,
                                      McalUsart_BufferCfg[i].SendCycBufLen);
    if (CycBufRet != MCAL_CYCBUF_RET_SUCCESS)
    {
      SYSM_printf("MCAL_CYCBUF_OPEN_CHAN failed for USART%d\r\n", i);
    }
  }
    for (i = MESSAGE_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
    {
      Message_Handle[i].Sendbuffer = McalUsart_Ctrl[i].RcvIntSwapBuf;
    }
}

/* Initialize the USART peripheral */
void Mcal_Usart_Init(void)
{
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  for (uint8_t i = 0; i < MCAL_USART_MAX_NUMBER; i++)
  {
    __HAL_UART_ENABLE(McalUsart_NumMapUsart[i].UsartBase);
  }
}

void Mcal_Usart_IT_Enable(void)
{
  uint8_t USART = MCAL_USART1_CH;
  const McalUsart_NumMapUsart_t *pUsart = NULL;
  McalUsart_Ctrol_t *pUsartCtrl = NULL;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    pUsart = &McalUsart_NumMapUsart[USART];
    pUsartCtrl = &McalUsart_Ctrl[USART];
    __HAL_UART_ENABLE_IT(pUsart->UsartBase, UART_IT_TC);
    __HAL_UART_ENABLE_IT(pUsart->UsartBase, UART_IT_IDLE);
    if (pUsart->UsartBase == &huart1 || pUsart->UsartBase == &huart2)
    {
      HAL_UARTEx_ReceiveToIdle_DMA(pUsart->UsartBase, pUsartCtrl->RcvIntSwapBuf, pUsartCtrl->RcvIntSwapBufSize);
    }
    else if (pUsart->UsartBase == &huart4 || pUsart->UsartBase == &huart5)
    {
      HAL_UARTEx_ReceiveToIdle_IT(pUsart->UsartBase, pUsartCtrl->RcvIntSwapBuf, pUsartCtrl->RcvIntSwapBufSize);
    }
  }
}

void Mcal_Usart_Disable(void)
{
  uint8_t USART = MCAL_USART1_CH;
  const McalUsart_NumMapUsart_t *pUsart = NULL;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    pUsart = &McalUsart_NumMapUsart[USART];
    __HAL_UART_DISABLE(pUsart->UsartBase);
    __HAL_UART_DISABLE_IT(pUsart->UsartBase, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(pUsart->UsartBase, UART_IT_TC);
  }
}

// 串口发送中断回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART1_CH].SenLen = 0;                                                       // Clear the sending length
    memset(McalUsart_Ctrl[MCAL_USART1_CH].SendBuf, 0, McalUsart_Ctrl[MCAL_USART1_CH].SendBufLen); // Clear the sending buffer
  }
  else if (huart == &huart2)
  {
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART2_CH].SenLen = 0;                                                       // Clear the sending length
    memset(McalUsart_Ctrl[MCAL_USART2_CH].SendBuf, 0, McalUsart_Ctrl[MCAL_USART2_CH].SendBufLen); // Clear the sending buffer
  }
  else if (huart == &huart4)
  {
    __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART4_CH].Send_Lock = 0;
  }
  else if (huart == &huart5)
  {
    __HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART5_CH].Send_Lock = 0;
  }
  else
  {
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  HAL_UART_IdleCallback(huart, Size);
}

void HAL_UART_IdleCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  McalUsart_Ctrol_t *ActiveCtrl = NULL;
  uint16_t message_sent_len = 0;

    if (huart == &huart1)
    {
    HAL_UART_DMAStop(huart);
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART1_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = MCAL_USART1_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart1_rx)); // 接收个数等于接收缓冲区总大小减剩余计数
    if (ActiveCtrl->RcvIntSwapBufDataCnt > 0)
    {
      // 交换缓冲区数据搬移到接收环形缓冲区
      Message_Handle[MESSAGE_USART1_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
      message_sent_len =  MessageBuff_StackSendMessage(&Message_Handle[MESSAGE_USART1_CH], 1);
      if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
      {
        MCAL_ERROR("MessageBuff_StackSendMessage failed for USART1, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
      }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize); // 重新开始DMA接收
  }
  else if (huart == &huart2)
  {
    HAL_UART_DMAStop(huart);
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART2_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = MCAL_USART2_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart2_rx)); // 接收个数等于接收缓冲区总大小减剩余计数
    if (ActiveCtrl->RcvIntSwapBufDataCnt > 0)
    {
      // 交换缓冲区数据搬移到接收环形缓冲区
      Message_Handle[MESSAGE_USART2_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
      message_sent_len =  MessageBuff_StackSendMessage(&Message_Handle[MESSAGE_USART2_CH], 1);
      if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
      {
        MCAL_ERROR("MessageBuff_StackSendMessage failed for USART2, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
      }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize); // 重新开始DMA接收
  }
  else if (huart == &huart4)
  {
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART4_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = Size;
    Message_Handle[MESSAGE_USART4_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
    message_sent_len =  MessageBuff_StackSendMessage(&Message_Handle[MESSAGE_USART4_CH], 1);
    if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
    {
      MCAL_ERROR("MessageBuff_StackSendMessage failed for USART4, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
    }
    HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART4_CH].UsartBase, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize);
  }
  else if (huart == &huart5)
  {
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART5_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = Size;
    Message_Handle[MESSAGE_USART5_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
    message_sent_len =  MessageBuff_StackSendMessage(&Message_Handle[MESSAGE_USART5_CH], 1);
    if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
    {
      MCAL_ERROR("MessageBuff_StackSendMessage failed for USART5, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
    }
    HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART5_CH].UsartBase, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize);
  }
  else
  {
  }
}

uint32_t Mcal_Usart_AppCheckData(uint32_t USART)
{
  const MessageBuffChannel_Enum_t *channel = &Mcal_UsartMapChannel[USART];

  return (uint32_t)MessageBuff_CheckMessage(&Message_Handle[*channel]);
}

// 串口接收数据函数
uint32_t Mcal_Usart_AppReceiveData(uint32_t USART, uint8_t *data, uint32_t size)
{
  uint32_t RetDataLen = 0;
  const MessageBuffChannel_Enum_t *channel = &Mcal_UsartMapChannel[USART];

  if (data == NULL || size == 0)
  {
    // Error: Invalid parameters
  }
  else
  {
    Message_Handle[*channel].Rcvbuffer = data;
    Message_Handle[*channel].Rcvsize = size;
    if (MESSAGE_BUFF_OK != MessageBuff_StackReceiveMessage(&Message_Handle[*channel], &RetDataLen, 0))
    {
       RetDataLen = 0;
    }
  }

  return RetDataLen;
}

// 串口发送数据函数
McalRetVal_t Mcal_Usart_AppSendData(uint32_t USART, uint8_t *data, uint32_t size)
{
  McalRetVal_t ret = MCAL_RET_SUCCESS;

  if (data == NULL || size == 0)
  {
    ret = MCAL_RET_PARAMETE_ERROR; // Error: Invalid parameters
  }
  else
  {
    if (MCAL_USART1_CH == USART || MCAL_USART2_CH == USART)
    {

      if (size > McalUsart_Ctrl[USART].SendBufLen)
      {
        ret = MCAL_RET_USART_SEND_CYCBUF_OVERFLOW; // Error: Data size exceeds buffer SenLength
      }
      else
      {
        if (MCAL_CYCBUF_RET_SUCCESS != MCAL_CYCBUF_WRITE(McalUsart_Ctrl[USART].SendCycBufID, data, size))
        {
          ret = MCAL_RET_FAILED; // Error: Failed to write data to buffer
        }
      }
    }
    else
    {
      if (1 != McalUsart_Ctrl[USART].Send_Lock)
      {
        if (HAL_UART_GetState(McalUsart_NumMapUsart[USART].UsartBase) != HAL_UART_STATE_BUSY_TX)
        {
          MCAL_ENTER_CRITICAL_AREA();
          McalUsart_Ctrl[USART].Send_Lock = 1;
          MCAL_EXIT_CRITICAL_AREA();
          while (1 == __HAL_UART_GET_FLAG(McalUsart_NumMapUsart[USART].UsartBase, UART_FLAG_TXE))
          {
            HAL_UART_Transmit_IT(McalUsart_NumMapUsart[USART].UsartBase, data, size);
          }
        }
      }
    }
  }

  return ret;
}

// 串口发送处理函数(串口1和串口2)
void Mcal_USARTIf_Send_MainFunction(void)
{
  uint8_t i = 0;

  for (i = MCAL_USART1_CH; i < MCAL_USART4_CH; i++)
  {
    if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_CHECK_DATA(McalUsart_Ctrl[i].SendCycBufID, &McalUsart_Ctrl[i].SenLen))
    {
      if (McalUsart_Ctrl[i].SenLen > 0)
      {
        if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_PREVIEW_READ(McalUsart_Ctrl[i].SendCycBufID, McalUsart_Ctrl[i].SendBuf, McalUsart_Ctrl[i].SenLen))
        {
          if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_READ(McalUsart_Ctrl[i].SendCycBufID, McalUsart_Ctrl[i].SendBuf, McalUsart_Ctrl[i].SenLen))
          {
            HAL_UART_Transmit_DMA(McalUsart_NumMapUsart[i].UsartBase, McalUsart_Ctrl[i].SendBuf, McalUsart_Ctrl[i].SenLen);
            // HAL_UART_Transmit_IT(McalUsart_NumMapUsart[i].UsartBase, McalUsart_Ctrl[i].SendBuf, McalUsart_Ctrl[i].SenLen);
          }
        }
      }
      else
      {
        McalUsart_Ctrl[i].SenLen = 0;
      }
    }
    else
    {
      McalUsart_Ctrl[i].SenLen = 0;
    }
  }
}
  /*EOF*/
