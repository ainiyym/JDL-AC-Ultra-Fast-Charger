/*
 * Mcal_Can_Cfg.c
 * Configuration source file for CAN MCAL module
 */
#include "can.h"
#include "Mcal_Can_Cfg.h"
#include "Ccp_CanDrv.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Union Definition
|******************************************************************************/

/*******************************************************************************
|    Struct Definition
|******************************************************************************/

/*******************************************************************************
|    Variables Definition
|******************************************************************************/
Mcal_CanTxChannelCfg_t Mcal_CanTxChannelCfgTable[MCAL_CAN_TX_MAX_NUMBER] = 
{
    {MCAL_CAN1_TX_CH, &hcan1},
    {MCAL_CAN1_TX_CCP, &hcan1}
};

Mcal_CanFilterCfg_t Mcal_CanFilterCfgTable[MCAL_CAN_RX_MAX_NUMBER] =
    {
        [MCAL_CAN1_RX_CH] = {
            .CanChannel = MCAL_CAN1_RX_CH,                                  // CAN channel
            .CanHandle = &hcan1,                                             // CAN Handle
            .FilterConfig.FilterBank = 0,                                   // the filter bank number
            .FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK,               // Filter mode
            .FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT,              // Filter scale
            .FilterConfig.FilterIdHigh = MCAL_CAN1_FILTER_ID_HIGH,          // High-level ID
            .FilterConfig.FilterIdLow = MCAL_CAN1_FILTER_ID_LOW,            // Low-level ID
            .FilterConfig.FilterMaskIdHigh = MCAL_CAN1_FILTER_MASK_ID_HIGH, // Filter mask high-level ID
            .FilterConfig.FilterMaskIdLow = MCAL_CAN1_FILTER_MASK_ID_LOW,   // Filter mask low-level ID
            .FilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0,              // Filter FIFO assignment
            .FilterConfig.FilterActivation = ENABLE,                        // Filter activation
            .FilterConfig.SlaveStartFilterBank = 14                         // Slave start filter bank
        },                                                                  /* CAN test */
        [MCAL_CAN1_RX_CCP] = {
            .CanChannel = MCAL_CAN1_RX_CCP,                                     // CAN channel
            .CanHandle = &hcan2,                                                 // CAN Handle
            .FilterConfig.FilterBank = 1,                                       // the filter bank number
            .FilterConfig.FilterMode = CAN_FILTERMODE_IDLIST,                   // Filter mode
            .FilterConfig.FilterScale = CAN_FILTERSCALE_16BIT,                  // Filter scale
            .FilterConfig.FilterIdHigh = MCAL_CAN1_CCP_FILTER_ID_HIGH,          // High-level ID
            .FilterConfig.FilterIdLow = MCAL_CAN1_CCP_FILTER_ID_LOW,            // Low-level ID
            .FilterConfig.FilterMaskIdHigh = MCAL_CAN1_CCP_FILTER_MASK_ID_HIGH, // Filter mask high-level ID
            .FilterConfig.FilterMaskIdLow = MCAL_CAN1_CCP_FILTER_MASK_ID_LOW,   // Filter mask low-level ID
            .FilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1,                  // Filter FIFO assignment
            .FilterConfig.FilterActivation = ENABLE,                            // Filter activation
            .FilterConfig.SlaveStartFilterBank = 14                             // Slave start filter bank
        } /* CAN CCP cfg */
};

Mcal_CanCtrlCfg_t Mcal_CanCtrl = {0};
/*******************************************************************************
|    Constant Definition
|******************************************************************************/

/* CAN filter initialization */
void Mcal_Can_Filter_Init(void)
{
  uint8_t i = 0;
  for (i = 0; i < MCAL_CAN_RX_MAX_NUMBER; i++)
  {
    if (HAL_CAN_ConfigFilter(Mcal_CanFilterCfgTable[i].CanHandle, &Mcal_CanFilterCfgTable[i].FilterConfig) != HAL_OK)
    {
      /* Filter configuration Error */
      MCAL_ERROR("%s Err!\n\r", __FUNCTION__);
    }
  }
}

/* CAN Circular queue initialization */
void Mcal_Can_RxBufCfg_Init(void)
{
  uint8_t i = 0;
  uint32_t CycBufRet;

  for (i = 0; i < MCAL_CAN_RX_MAX_NUMBER; i++)
  {
    /* Receive Buff requests */
    CycBufRet = MCAL_CYCBUF_OPEN_CHAN(&Mcal_CanCtrl.Buf[i].RcvCycBufID,
                                      Mcal_CanCtrl.Buf[i].RcvCycBuf,
                                      MCAL_CAN1_RX_CYC_LEN);
    if (CycBufRet != MCAL_CYCBUF_RET_SUCCESS)
    {
      MCAL_ERROR("%s Err!\n\r", __FUNCTION__);
    }
  }
}

/** @brief  can data transmission interface function
 * @param  Channel Can Tx Channel.
 * @param  TxHeader Can Tx handle.
 * @param  pTxMailbox pointer to a variable where the function will return
 *         the TxMailbox used to store the Tx message.
 *         This parameter can be a value of @arg CAN_Tx_Mailboxes.
 * @param  msg Data address.
 * @param  len Data length.
 * @retval @McalRetVal_t
 */
McalRetVal_t Mcal_Can_Send_Msg(Mcal_CanTxChannel_Enum_t Channel, CAN_TxHeaderTypeDef TxHeader, uint32_t *pTxMailbox, uint8_t *msg, uint8_t len)
{
  McalRetVal_t ret = MCAL_RET_SUCCESS; /* ret init */
  uint8_t i = 0;

  if (len > 8 && Channel > MCAL_CAN1_TX_CCP) /* Check whether the data length exceeds 8 bytes */
  {
    return MCAL_RET_PARAMETE_ERROR;
  }

  if (Mcal_CanTxChannelCfgTable[Channel].CanHandle == NULL)
  {
    MCAL_ERROR("%s Err!\r\n", __func__);
    return MCAL_RET_UNEXPECTED_ERROR;
  }

  for (i = 0; i < len; i++)
  {
    Mcal_CanCtrl.sendData[i] = msg[i];
  }

  if (HAL_CAN_IsTxMessagePending(Mcal_CanTxChannelCfgTable[Channel].CanHandle, CAN_TX_MAILBOX0 | CAN_TX_MAILBOX1 | CAN_TX_MAILBOX2) != 0)
  {
    /* If the mailbox is not empty, return an error */
    MCAL_ERROR("%s Err: Tx Mailbox is not empty!\r\n", __func__);
    return MCAL_RET_FAILED;
  }

  if (HAL_CAN_AddTxMessage(Mcal_CanTxChannelCfgTable[Channel].CanHandle, &TxHeader, Mcal_CanCtrl.sendData, pTxMailbox) != HAL_OK) /* Sent */
  {
    /* Sending error */
    ret = MCAL_RET_FAILED;
  }

  while (HAL_CAN_GetTxMailboxesFreeLevel(Mcal_CanTxChannelCfgTable[Channel].CanHandle) != 3) /* Wait for the mailbox to be free */
  {
    if (HAL_CAN_IsTxMessagePending(Mcal_CanTxChannelCfgTable[Channel].CanHandle, CAN_TX_MAILBOX0 | CAN_TX_MAILBOX1 | CAN_TX_MAILBOX2) == 0)
    {
      break; /* If the mailbox is empty, exit the loop */
    }
  }

  return ret;
}

uint32_t Mcal_Can_Receive_Msg(Mcal_CanRxChannel_Enum_t Channel, uint8_t *data, uint32_t size)
{
  uint32_t RcvBuffLen = 0;
  uint32_t RetDataLen = 0;

  if (data == NULL || size == 0)
  {
    // Error: Invalid parameters
  }
  else
  {
    if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_CHECK_DATA(Mcal_CanCtrl.Buf[Channel].RcvCycBufID, &RcvBuffLen))
    {
      if (RcvBuffLen >= size)
      {
        if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_PREVIEW_READ(Mcal_CanCtrl.Buf[Channel].RcvCycBufID, data, size))
        {

          if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_READ(Mcal_CanCtrl.Buf[Channel].RcvCycBufID, data, size))
          {
            RetDataLen = size;
          }
        }
      }
      else
      {
        if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_PREVIEW_READ(Mcal_CanCtrl.Buf[Channel].RcvCycBufID, data, RcvBuffLen))
        {
          if (MCAL_CYCBUF_RET_SUCCESS == MCAL_CYCBUF_READ(Mcal_CanCtrl.Buf[Channel].RcvCycBufID, data, RcvBuffLen))
          {
            RetDataLen = RcvBuffLen;
          }
        }
      }
    }
  }

  return RetDataLen;
}

/* CAN receive interrupt function */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanNum)
{
    uint8_t chNum = 0;

    MCAL_DEBUG("%s \r\n", __func__);
    for (chNum = 0; chNum < MCAL_CAN_RX_MAX_NUMBER; chNum++)
    {
        if (CanNum == Mcal_CanFilterCfgTable[chNum].CanHandle && CAN_RX_FIFO0 == Mcal_CanFilterCfgTable[chNum].FilterConfig.FilterFIFOAssignment)
        {
            HAL_CAN_GetRxMessage(Mcal_CanFilterCfgTable[chNum].CanHandle, Mcal_CanFilterCfgTable[chNum].FilterConfig.FilterFIFOAssignment, &Mcal_CanCtrl.RxHeader, Mcal_CanCtrl.rcvData);
            if (Mcal_CanCtrl.RxHeader.DLC > 0)
            {
                MCAL_CYCBUF_WRITE(Mcal_CanCtrl.Buf[chNum].RcvCycBufID, Mcal_CanCtrl.rcvData, Mcal_CanCtrl.RxHeader.DLC);
            }
        }
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *CanNum)
{
    uint8_t chNum = 0;

    // MCAL_DEBUG("%s \r\n", __func__);
    for (chNum = 0; chNum < MCAL_CAN_RX_MAX_NUMBER; chNum++)
    {
        if (CanNum == Mcal_CanFilterCfgTable[chNum].CanHandle && CAN_RX_FIFO1 == Mcal_CanFilterCfgTable[chNum].FilterConfig.FilterFIFOAssignment)
        {
            HAL_CAN_GetRxMessage(Mcal_CanFilterCfgTable[chNum].CanHandle, Mcal_CanFilterCfgTable[chNum].FilterConfig.FilterFIFOAssignment, &Mcal_CanCtrl.RxHeader, Mcal_CanCtrl.rcvData);
            if (Mcal_CanCtrl.RxHeader.DLC > 0 && Mcal_CanCtrl.RxHeader.StdId == MCAL_CAN1_CCP_RX_ID)
            {
                Ccp_CanDrv_ReceiveCallback(Mcal_CanCtrl.RxHeader.StdId, Mcal_CanCtrl.rcvData, Mcal_CanCtrl.RxHeader.DLC);
            }
        }
    }
}

void Mcal_Can_Init(void)
{
    MX_CAN1_Init();
    MX_CAN2_Init();
    Mcal_Can_Filter_Init(); // Initialize the CAN filter
}

void Mcal_Can_Enable(void)
{
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        /* Start Error */
        MCAL_ERROR("%s Err!\n\r", __FUNCTION__);
    }

    /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        /* Start Error */
        MCAL_ERROR("%s Err 0!\n\r", __FUNCTION__);
    }

        /* Activate CAN RX notification */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
    {
        /* Start Error */
        MCAL_ERROR("%s Err 1!\n\r", __FUNCTION__);
    } 
}

void Mcal_Can_Disable(void)
{
    if (HAL_CAN_DeInit(&hcan1) != HAL_OK)
    {
        /* DeInit Error */
        MCAL_ERROR("%s Err!\n\r", __FUNCTION__);
    }
}
/* End of file */ 
