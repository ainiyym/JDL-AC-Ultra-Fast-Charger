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
#include "StreamBuffer.h"
#include "Mcal_Usart_Cfg.h"
#include "STD_SysM.h"
#include "AppTask_CorePrintTask.h"
#include "Mcal_Gpio_Cfg.h"
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

McalUsart_Ctrol_t McalUsart_Ctrl[MCAL_USART_MAX_NUMBER] = {0};

const McalUsart_NumMapUsart_t McalUsart_NumMapUsart[MCAL_USART_MAX_NUMBER] =
    {
        {MCAL_USART1_CH, &huart1},
        {MCAL_USART2_CH, &huart2},
        {MCAL_USART4_CH, &huart4},
        {MCAL_USART5_CH, &huart5}
};

const StreamBuffChannel_Enum_t Mcal_UsartMapChannel[MCAL_USART_MAX_NUMBER] =
    {
        STREAM_USART1_CH, /* 4G */
        STREAM_USART2_CH, /* LOG */
        STREAM_USART4_CH, /* Meter */
        STREAM_USART5_CH  /* Fan speeder */
};

McalUsart_BufCfg_t  const McalUsart_BufferCfg[MCAL_USART_MAX_NUMBER] =
{
  {
    .UsartNum = MCAL_USART1_CH,
    .RcvBufLen = MCAL_USART1_CH_RCV_CYCBUF_LEN
  },
  {
    .UsartNum = MCAL_USART2_CH,
    .RcvBufLen = MCAL_USART2_CH_RCV_CYCBUF_LEN
  },
  {
    .UsartNum = MCAL_USART4_CH,
    .RcvBufLen = MCAL_USART4_CH_RCV_CYCBUF_LEN
  },
  {
    .UsartNum = MCAL_USART5_CH,
    .RcvBufLen = MCAL_USART5_CH_RCV_CYCBUF_LEN
  }
};

static void Mcal_Usart_IT_Enable(McalUsartChannel_Enum_t UsartNum);
static void Mcal_Usart_Disable(McalUsartChannel_Enum_t UsartNum);
/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
void McalUsart_CycBuffCfgInit(void)
{
  uint8_t i;

  /* Initialize receive buffer */
  for (i = MCAL_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
  {
    McalUsart_Ctrl[i].RcvIntSwapBufSize = McalUsart_BufferCfg[i].RcvBufLen;
    McalUsart_Ctrl[i].RcvIntSwapBuf = MCAL_MALLOC(McalUsart_Ctrl[i].RcvIntSwapBufSize);
  }

  for (i = STREAM_USART1_CH; i < MCAL_USART_MAX_NUMBER; i++)
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

void Mcal_Usart_DeInit(McalUsartChannel_Enum_t UsartNum)
{
  const McalUsart_NumMapUsart_t *pUsart = NULL;

  if (UsartNum < MCAL_USART_MAX_NUMBER)
  {
    pUsart = &McalUsart_NumMapUsart[UsartNum];
    HAL_UART_Abort(pUsart->UsartBase);
    Mcal_Usart_Disable(UsartNum);
    HAL_UART_DeInit(pUsart->UsartBase);
  }
}

void Mcal_Usart_ReInit(McalUsartChannel_Enum_t UsartNum, UART_InitTypeDef* Cfg)
{
  const McalUsart_NumMapUsart_t *pUsart = NULL;

  if (UsartNum < MCAL_USART_MAX_NUMBER)
  {
    pUsart = &McalUsart_NumMapUsart[UsartNum];
    pUsart->UsartBase->Init = *Cfg;
    HAL_UART_Init(pUsart->UsartBase);
    Mcal_Usart_IT_Enable(UsartNum);
  }
}

static void Mcal_Usart_IT_Enable(McalUsartChannel_Enum_t UsartNum)
{
  const McalUsart_NumMapUsart_t *pUsart = NULL;
  McalUsart_Ctrol_t *pUsartCtrl = NULL;

  if (UsartNum < MCAL_USART_MAX_NUMBER)
  {
    pUsart = &McalUsart_NumMapUsart[UsartNum];
    pUsartCtrl = &McalUsart_Ctrl[UsartNum];
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

void Mcal_Usart_IT_Enable_All(void)
{
  uint8_t USART = MCAL_USART1_CH;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    Mcal_Usart_IT_Enable((McalUsartChannel_Enum_t)USART);
  }
}

static void Mcal_Usart_Disable(McalUsartChannel_Enum_t UsartNum)
{
  const McalUsart_NumMapUsart_t *pUsart = NULL;

  if (UsartNum < MCAL_USART_MAX_NUMBER)
  {
    pUsart = &McalUsart_NumMapUsart[UsartNum];
    __HAL_UART_DISABLE(pUsart->UsartBase);
    __HAL_UART_DISABLE_IT(pUsart->UsartBase, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(pUsart->UsartBase, UART_IT_TC);
  }
}

void Mcal_Usart_Disable_All(void)
{
  uint8_t USART = MCAL_USART1_CH;

  for (USART = MCAL_USART1_CH; USART < MCAL_USART_MAX_NUMBER; USART++)
  {
    Mcal_Usart_Disable((McalUsartChannel_Enum_t)USART);
  }
}

// extern SemaphoreHandle_t uart_tx_semaphore;
// UART transmit complete interrupt callback function
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // // Release semaphore
    // xSemaphoreGiveFromISR(uart_tx_semaphore, &xHigherPriorityTaskWoken);

    // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  if (huart == &huart4)
  {
    McalUsart_Ctrl[MCAL_USART4_CH].Send_Lock = 0;
  }
  if (huart == &huart5)
  {
    McalUsart_Ctrl[MCAL_USART5_CH].Send_Lock = 0;
  }
  __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC); // Clear the "Send Completed" flag
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
    ActiveCtrl->RcvIntSwapBufDataCnt = MCAL_USART1_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart1_rx)); // Received count equals total buffer size minus remaining counter
    if (ActiveCtrl->RcvIntSwapBufDataCnt > 0)
    {
      // Move swap buffer data to receive ring buffer
      Message_Handle[STREAM_USART1_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
      message_sent_len = StreamBuff_SendMessage(&Message_Handle[STREAM_USART1_CH], 1);
      // Core_printf("Rcv%lld:\r\n", message_sent_len);
      if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
      {
        // Core_printf("StreamBuff_SendMessage failed for USART1, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
      }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize); // Restart DMA reception
  }
  else if (huart == &huart2)
  {
    HAL_UART_DMAStop(huart);
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART2_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = MCAL_USART2_CH_RCV_CYCBUF_LEN - (__HAL_DMA_GET_COUNTER(&hdma_usart2_rx)); // Received count equals total buffer size minus remaining counter
    if (ActiveCtrl->RcvIntSwapBufDataCnt > 0)
    {
      // Move swap buffer data to receive ring buffer
      Message_Handle[STREAM_USART2_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
      message_sent_len = StreamBuff_SendMessage(&Message_Handle[STREAM_USART2_CH], 1);
      if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
      {
        // Core_printf("StreamBuff_SendMessage failed for USART2, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
      }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize); // Restart DMA reception
  }
  else if (huart == &huart4)
  {
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART4_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = Size;
    Message_Handle[STREAM_USART4_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
    message_sent_len = StreamBuff_SendMessage(&Message_Handle[STREAM_USART4_CH], 1);
    if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
    {
      // Core_printf("StreamBuff_SendMessage failed for USART4, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
    }
    HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART4_CH].UsartBase, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize);
  }
  else if (huart == &huart5)
  {
    ActiveCtrl = &McalUsart_Ctrl[MCAL_USART5_CH];
    ActiveCtrl->RcvIntSwapBufDataCnt = Size;
    Message_Handle[STREAM_USART5_CH].Sendsize = ActiveCtrl->RcvIntSwapBufDataCnt;
    message_sent_len = StreamBuff_SendMessage(&Message_Handle[STREAM_USART5_CH], 1);
    if (message_sent_len != ActiveCtrl->RcvIntSwapBufDataCnt)
    {
      // Core_printf("StreamBuff_SendMessage failed for USART5, sent %d bytes, expected %d bytes\r\n", message_sent_len, ActiveCtrl->RcvIntSwapBufDataCnt);
    }
    HAL_UARTEx_ReceiveToIdle_IT(McalUsart_NumMapUsart[MCAL_USART5_CH].UsartBase, ActiveCtrl->RcvIntSwapBuf, ActiveCtrl->RcvIntSwapBufSize);
  }
  else
  {
  }
}

uint32_t Mcal_Usart_AppCheckData(uint32_t USART)
{
  const StreamBuffChannel_Enum_t *channel = &Mcal_UsartMapChannel[USART];

  return (uint32_t)StreamBuff_CheckMessage(&Message_Handle[*channel]);
}

// UART receive data function
uint32_t Mcal_Usart_AppReceiveData(uint32_t USART, uint8_t *data, uint32_t size)
{
  uint32_t RetDataLen = 0;
  const StreamBuffChannel_Enum_t *channel = &Mcal_UsartMapChannel[USART];

  if (data == NULL || size == 0)
  {
    // Error: Invalid parameters
  }
  else
  {
    Message_Handle[*channel].Rcvbuffer = data;
    Message_Handle[*channel].Rcvsize = size;
    if (STREAM_BUFF_OK != StreamBuff_ReceiveMessage(&Message_Handle[*channel], &RetDataLen, 0))
    {
      RetDataLen = 0;
    }
  }

  return RetDataLen;
}

// UART send data function
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
      HAL_UART_Transmit_DMA(McalUsart_NumMapUsart[USART].UsartBase, data, size);
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
      else
      {
        ret = MCAL_RET_USART_SEND_BUSY; // Error: USART is busy sending data
      }
    }
  }

  return ret;
}

#if 1
void assert_failed(const char *file, int line)
{
  // 禁用中断，防止进一步破坏系统状态
  __disable_irq();

  // 固定错误信息
  HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n!!! ASSERT FAILED !!!\r\n", 25, HAL_MAX_DELAY);

  // 打印文件名
  HAL_UART_Transmit(&huart2, (uint8_t *)"File: ", 6, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t *)file, strlen(file), HAL_MAX_DELAY);

  // 打印行号
  HAL_UART_Transmit(&huart2, (uint8_t *)"\r\nLine: ", 8, HAL_MAX_DELAY);

  // 转换行号为字符串并发送
  char line_str[10];
  int idx = 0;
  int n = line;

  // 处理行号为0的情况
  if (n == 0)
  {
    line_str[idx++] = '0';
  }
  else
  {
    // 提取每一位数字
    while (n > 0)
    {
      line_str[idx++] = '0' + (n % 10);
      n /= 10;
    }
  }
  line_str[idx] = '\0';

  // 反转字符串（因为上面是反向存储的）
  for (int i = 0; i < idx / 2; i++)
  {
    char temp = line_str[i];
    line_str[i] = line_str[idx - 1 - i];
    line_str[idx - 1 - i] = temp;
  }

  // 发送行号
  HAL_UART_Transmit(&huart2, (uint8_t *)line_str, idx, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t *)"\r\nSystem Halted!\r\n", 18, HAL_MAX_DELAY);

  // 进入死循环
  while (1)
  {
    Mcal_Gpio_SetPin(LED3_GPIO_Port, LED3_Pin);
    for (volatile int i = 0; i < 1000000; i++)
      ;
  }
}
#else
void assert_failed(const char *file, int line)
{}
#endif
  /*EOF*/
