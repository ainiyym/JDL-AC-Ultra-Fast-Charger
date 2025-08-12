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
#include "Mcal_Usart_Cfg.h"
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
|    Global Variable with extern linkage
|******************************************************************************/
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
 /* 4G */
static uint8_t McalUsart_SendCycBuf[MCAL_USART1_CH_SEND_CYCBUF_LEN];
static uint8_t McalUsart_RcvCycBuf[MCAL_USART1_CH_RCV_CYCBUF_LEN];
static uint8_t McalUsart_SendBuf[MCAL_USART1_CH_SEND_BUF_LEN];
 /* LOG */
static uint8_t McalUsart_SendCycBuf2[MCAL_USART2_CH_SEND_CYCBUF_LEN];
static uint8_t McalUsart_RcvCycBuf2[MCAL_USART2_CH_RCV_CYCBUF_LEN];
static uint8_t McalUsart_SendBuf2[MCAL_USART2_CH_SEND_BUF_LEN];
/* Meter */
static uint8_t McalUsart_RcvCycBuf4[MCAL_USART4_CH_RCV_CYCBUF_LEN];
/* Fan speeder */
static uint8_t McalUsart_RcvCycBuf5[MCAL_USART5_CH_RCV_CYCBUF_LEN];

McalUsart_Ctrol_t McalUsart_Ctrl[MCAL_USART_MAX_NUMBER] = {0};

const McalUsart_NumMapUsart_t  McalUsart_NumMapUsart[MCAL_USART_MAX_NUMBER] =
    {
        {MCAL_USART1_CH, &huart1},
        {MCAL_USART2_CH, &huart2},
        {MCAL_USART4_CH, &huart4},
        {MCAL_USART5_CH, &huart5}
    };

McalUsart_BufCfg_t  const McalUsart_BufferCfg[MCAL_USART_MAX_NUMBER] =
{
    {
        .UsartNum = MCAL_USART1_CH,
        .SendCycBuf = McalUsart_SendCycBuf,
        .SendCycBufLen = MCAL_USART1_CH_SEND_CYCBUF_LEN,
        .RcvCycBuf = McalUsart_RcvCycBuf,
        .RcvCycBufLen = MCAL_USART1_CH_RCV_CYCBUF_LEN,
        .SendBuf = McalUsart_SendBuf,
        .SendBufLen = MCAL_USART1_CH_SEND_BUF_LEN
    },
    {
        .UsartNum = MCAL_USART2_CH,
        .SendCycBuf = McalUsart_SendCycBuf2,
        .SendCycBufLen = MCAL_USART2_CH_SEND_CYCBUF_LEN,
        .RcvCycBuf = McalUsart_RcvCycBuf2,
        .RcvCycBufLen = MCAL_USART2_CH_RCV_CYCBUF_LEN,
        .SendBuf = McalUsart_SendBuf2,
        .SendBufLen = MCAL_USART2_CH_SEND_BUF_LEN
    },
    {
        .UsartNum = MCAL_USART4_CH,
        .SendCycBuf = NULL,
        .SendCycBufLen = 0,
        .RcvCycBuf = McalUsart_RcvCycBuf4,
        .RcvCycBufLen = MCAL_USART4_CH_RCV_CYCBUF_LEN,
        .SendBuf = NULL,
        .SendBufLen = 0
    },
    {
        .UsartNum = MCAL_USART5_CH,
        .SendCycBuf = NULL,
        .SendCycBufLen = 0,
        .RcvCycBuf = McalUsart_RcvCycBuf5,
        .RcvCycBufLen = MCAL_USART5_CH_RCV_CYCBUF_LEN,
        .SendBuf = NULL,
        .SendBufLen = 0
    }
};
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
void McalUsart_CycBuffCfgInit(void)
{
    uint32_t CycBufRet;
    uint8_t i;

    /* 4G和LOG使用发送缓冲区 */
    for (i = MCAL_USART1_CH; i < MCAL_USART4_CH; i++)
    {
        McalUsart_Ctrl[i].SendBuf = McalUsart_BufferCfg[i].SendBuf;
        McalUsart_Ctrl[i].SendBufLen = McalUsart_BufferCfg[i].SendBufLen;
        /* 发送缓存申请 */
        CycBufRet = MCAL_CYCBUF_OPEN_CHAN(&McalUsart_Ctrl[i].SendCycBufID,
                                          McalUsart_BufferCfg[i].SendCycBuf,
                                          McalUsart_BufferCfg[i].SendCycBufLen);
        if (CycBufRet != MCAL_CYCBUF_RET_SUCCESS)
        {
            // Error_Handler();
        }
    }
    /* 所有通道均使用接收缓冲 */
    for (i = MCAL_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
    {
        /* 接收缓存申请 */
        CycBufRet = MCAL_CYCBUF_OPEN_CHAN(&McalUsart_Ctrl[i].RcvCycBufID,
                                          McalUsart_BufferCfg[i].RcvCycBuf,
                                          McalUsart_BufferCfg[i].RcvCycBufLen);
        if (CycBufRet != MCAL_CYCBUF_RET_SUCCESS)
        {
            // Error_Handler();
        }
    }
}


/* Initialize the USART peripheral */
void Mcal_Usart_Init(void)
{
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
}

void Mcal_Usart_Enable(void)
{
  uint8_t USART = MCAL_USART1_CH;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    __HAL_UART_ENABLE(McalUsart_NumMapUsart[USART].UsartBase);
    __HAL_UART_ENABLE_IT(McalUsart_NumMapUsart[USART].UsartBase, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(McalUsart_NumMapUsart[USART].UsartBase, UART_IT_TC);
  }
  for (USART = MCAL_USART1_CH; USART < MCAL_USART4_CH; USART++)
  {
    HAL_UARTEx_ReceiveToIdle_DMA(McalUsart_NumMapUsart[USART].UsartBase, McalUsart_Ctrl[USART].RcvIntSwapBuf[McalUsart_Ctrl[USART].RcvIntSwapBufIdx], McalUsart_BufferCfg[USART].RcvCycBufLen);
  }
  HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART4_CH].UsartBase, McalUsart_Ctrl[MCAL_USART4_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART4_CH].RcvIntSwapBufIdx], MCAL_USART_RCV_CYCBUF_MAX_LEN);
  HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART5_CH].UsartBase, McalUsart_Ctrl[MCAL_USART5_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART5_CH].RcvIntSwapBufIdx], MCAL_USART_RCV_CYCBUF_MAX_LEN);
}

void Mcal_Usart_Disable(void)
{
  uint8_t USART = MCAL_USART1_CH;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    __HAL_UART_DISABLE(McalUsart_NumMapUsart[USART].UsartBase);
    __HAL_UART_DISABLE_IT(McalUsart_NumMapUsart[USART].UsartBase, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(McalUsart_NumMapUsart[USART].UsartBase, UART_IT_TC);
  }
}

// 串口发送中断回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART1_CH].Send_Lock = 0;                                                 // Send completed, unlock
    McalUsart_Ctrl[MCAL_USART1_CH].SenLen = 0;                                                       // Clear the sending length
    memset(McalUsart_Ctrl[MCAL_USART1_CH].SendBuf, 0, McalUsart_Ctrl[MCAL_USART1_CH].SendBufLen); // Clear the sending buffer
  }
  else if (huart == &huart2)
  {
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
    McalUsart_Ctrl[MCAL_USART2_CH].Send_Lock = 0;                                                 // Send completed, unlock
    McalUsart_Ctrl[MCAL_USART2_CH].SenLen = 0;                                                       // Clear the sending length
    memset(McalUsart_Ctrl[MCAL_USART2_CH].SendBuf, 0, McalUsart_Ctrl[MCAL_USART2_CH].SendBufLen); // Clear the sending buffer
  }
  else if (huart == &huart4)
  {
    __HAL_UART_CLEAR_FLAG(&huart4, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
  }
  else if (huart == &huart5)
  {
    __HAL_UART_CLEAR_FLAG(&huart5, UART_FLAG_TC);                                                 // Clear the "Send Completed" flag
  }
  else
  {
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  for (uint32_t id = MCAL_USART4_CH; id < MCAL_USART_MAX_NUMBER; id++)
  {
    if (huart == McalUsart_NumMapUsart[id].UsartBase)
    {
      if (McalUsart_Ctrl[id].Rcv_Lock == 0)
      {
          MCAL_CYCBUF_WRITE(McalUsart_Ctrl[id].RcvCycBufID, McalUsart_Ctrl[id].RcvIntSwapBuf[McalUsart_Ctrl[id].RcvIntSwapBufIdx], Size);
      }
      McalUsart_Ctrl[id].RcvIntSwapBufIdx ^= 1; // 切换缓冲区索引
      HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[id].UsartBase, McalUsart_Ctrl[id].RcvIntSwapBuf[McalUsart_Ctrl[id].RcvIntSwapBufIdx], MCAL_USART_RCV_CYCBUF_MAX_LEN);
      break;
    }
  }
}

// 串口DMA空闲中断回调函数
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
  // 当触发了串口接收空闲中断
  if (huart == &huart1)
  {
    /* 2.读取DMA */
    HAL_UART_DMAStop(huart); // 先停止DMA，暂停接收
    // 这里应注意数据接收不要大于 USART_DMA_RX_BUFFER_MAXIMUM
    McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufDataCnt = MCAL_USART1_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart1_rx)); // 接收个数等于接收缓冲区总大小减剩余计数

    /* 3.搬移数据进行其他处理 */
    if (McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufDataCnt > 0)
    {
      // 交换缓冲区数据搬移到接收环形缓冲区
      MCAL_CYCBUF_WRITE(McalUsart_Ctrl[MCAL_USART1_CH].RcvCycBufID, McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufIdx], McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufDataCnt);
      McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufIdx ^= 1; // 切换缓冲区索引
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART1_CH].RcvIntSwapBufIdx], MCAL_USART1_CH_RCV_CYCBUF_LEN); // 重新开始DMA接收
  }
  else if (huart == &huart2)
  {
    HAL_UART_DMAStop(huart); // 先停止DMA，暂停接收
    // 这里应注意数据接收不要大于 USART_DMA_RX_BUFFER_MAXIMUM
    McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufDataCnt = MCAL_USART2_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart2_rx)); // 接收个数等于接收缓冲区总大小减剩余计数

    /* 3.搬移数据进行其他处理 */
    if (McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufDataCnt > 0)
    {
      // 交换缓冲区数据搬移到接收环形缓冲区
      MCAL_CYCBUF_WRITE(McalUsart_Ctrl[MCAL_USART2_CH].RcvCycBufID, McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufIdx], McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufDataCnt);
      McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufIdx ^= 1; // 切换缓冲区索引
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBuf[McalUsart_Ctrl[MCAL_USART2_CH].RcvIntSwapBufIdx], MCAL_USART2_CH_RCV_CYCBUF_LEN); // 重新开始DMA接收
  }
  else
  {}
}

// 串口接收数据函数
uint32_t Mcal_Usart_AppReceiveData(uint32_t USART, uint8_t *data, uint32_t size)
{
  uint32_t RcvBuffLen = 0;
  uint32_t RetDataLen = 0;

  if (data == NULL || size == 0)
  {
    // Error: Invalid parameters
  }
  else
  {
    McalUsart_Ctrl[USART].Rcv_Lock = 1;
    if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_CHECK_DATA(McalUsart_Ctrl[USART].RcvCycBufID, &RcvBuffLen))
    {
      if (RcvBuffLen >= size)
      {
        if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_PREVIEW_READ(McalUsart_Ctrl[USART].RcvCycBufID, data, size))
        {

          if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_READ(McalUsart_Ctrl[USART].RcvCycBufID, data, size))
          {
            RetDataLen = size;
          }
        }
      }
      else if(0 < RcvBuffLen)
      {
        if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_PREVIEW_READ(McalUsart_Ctrl[USART].RcvCycBufID, data, RcvBuffLen))
        {
          if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_READ(McalUsart_Ctrl[USART].RcvCycBufID, data, RcvBuffLen))
          {
            RetDataLen = RcvBuffLen;
          }
        }
      }
      else
      {}
    }
    McalUsart_Ctrl[USART].Rcv_Lock = 0;
  }

  return RetDataLen;
}

// 串口发送数据函数
McalRetVal_t Mcal_Usart_AppSentData(uint32_t USART, uint8_t *data, uint32_t size)
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
      while (__HAL_UART_GET_FLAG(McalUsart_NumMapUsart[USART].UsartBase, UART_FLAG_TXE) != RESET)
      {
        if (HAL_OK != HAL_UART_Transmit_IT(McalUsart_NumMapUsart[USART].UsartBase, data, size))
        {
          ret = MCAL_RET_FAILED;
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

  if (McalUsart_Ctrl[i].Send_Lock == 1)
  {
    return;
  }

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
            McalUsart_Ctrl[i].Send_Lock = 1; // Lock send
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
