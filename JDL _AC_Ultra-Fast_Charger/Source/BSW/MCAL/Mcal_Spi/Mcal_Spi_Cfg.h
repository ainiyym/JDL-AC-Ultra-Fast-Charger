#ifndef MCAL_SPI_CFG_H
#define MCAL_SPI_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ===================[ Includes ]=========================================== */
#include "Mcal_app_Cfg.h"

/* ===================[ Macros ]============================================= */


/* ===================[ Type Definitions ]=================================== */
typedef enum {
	MCAL_SPI1_W25Q64_CH  ,   	        /* SPI1 */
  MCAL_SPI2_LED_CH  ,   	          /* SPI2 */
	MCAL_SPI_MAX_NUMBER
}McalSpiChannel_Enum_t;

typedef struct
{
  McalSpiChannel_Enum_t SpiNum; /* SPI通道号 */
  SPI_HandleTypeDef* SpiBase;    /* Spi句柄 */
} Mcal_Spi_ConfigType_t;

/* ===================[ External Data Declaration ]========================== */


/* ===================[ External Function Declaration ]====================== */
extern void Mcal_Spi_Init(void);
extern McalRetVal_t Mcal_SpiDrv_ReadData(McalSpiChannel_Enum_t Channel, uint8_t *RxData, uint16_t RcvDataLen, uint32_t TimeOut);
extern McalRetVal_t Mcal_SpiDrv_SendData(McalSpiChannel_Enum_t Channel, uint8_t *TxData, uint16_t DataLen, uint32_t TimeOut);
extern uint8_t Mcal_SpiDrv_SendByte(McalSpiChannel_Enum_t Channel, uint8_t Byte);
/* ===================[ Configuration Parameters ]=========================== */


#ifdef __cplusplus
}
#endif

#endif /* MCAL_SPI_CFG_H */
