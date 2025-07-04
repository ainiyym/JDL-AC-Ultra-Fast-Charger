/*
 * Mcal_Spi_Cfg.c
 * SPI Configuration Source File
 */
#include "Spi.h"
#include "Mcal_Spi_Cfg.h"

/* SPI configuration structure(s) */
const Mcal_Spi_ConfigType_t Mcal_Spi_Config[MCAL_SPI_MAX_NUMBER] = 
{
    {
        .SpiNum = MCAL_SPI1_W25Q64_CH,
        .SpiBase = &hspi1
    },
    {
        .SpiNum = MCAL_SPI2_LED_CH,
        .SpiBase = &hspi2
    }
};

void Mcal_Spi_Init(void)
{
  MX_SPI1_Init();
  MX_SPI2_Init();
}

McalRetVal_t Mcal_SpiDrv_ReadData(McalSpiChannel_Enum_t Channel, uint8_t *RxData, uint16_t RcvDataLen, uint32_t TimeOut)
{
	HAL_StatusTypeDef TransStatus = HAL_OK;
	McalRetVal_t  Ret = MCAL_RET_SUCCESS;

   if((Channel < MCAL_SPI_MAX_NUMBER) && (0 != RcvDataLen)&& (0!= TimeOut))
   {
		TransStatus = HAL_SPI_Receive_IT(Mcal_Spi_Config[Channel].SpiBase, RxData, RcvDataLen);
        if (HAL_OK != TransStatus)
        {
            Ret = MCAL_RET_FAILED;
        }
   }
   else
   {
	   Ret = MCAL_RET_PARAMETE_ERROR;
   }

   return Ret;
}

McalRetVal_t Mcal_SpiDrv_SendData(McalSpiChannel_Enum_t Channel, uint8_t *TxData, uint16_t DataLen, uint32_t TimeOut)
{
    HAL_StatusTypeDef TransStatus = HAL_OK;
    McalRetVal_t Ret = MCAL_RET_SUCCESS;

    if (Channel == MCAL_SPI2_LED_CH && NULL != TxData)
    {
        TransStatus = HAL_SPI_Transmit_DMA(Mcal_Spi_Config[Channel].SpiBase, TxData, DataLen);
        if (HAL_OK != TransStatus)
        {
            Ret = MCAL_RET_FAILED;
        }
    }
    else if ((Channel < MCAL_SPI_MAX_NUMBER) && (0 != DataLen) && (NULL != TxData))
    {
        TransStatus = HAL_SPI_Transmit_IT(Mcal_Spi_Config[Channel].SpiBase, TxData, DataLen);
        if (HAL_OK != TransStatus)
        {
            Ret = MCAL_RET_FAILED;
        }
    }
    else
    {
        Ret = MCAL_RET_PARAMETE_ERROR;
    }

    return Ret;
}
/* End of file */
