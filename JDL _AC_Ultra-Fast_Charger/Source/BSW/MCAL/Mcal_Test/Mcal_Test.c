/*
 * File: Mcal_Test.c
 * Description: MCAL Test Source File
 */

#include "Mcal_Test.h"
#include "SwitchM.h"
#include "STD_MosDrv.h"

void Mcal_Usart_Test(void)
{
    //uint8_t data[100] = {0};
    //uint32_t size = 0;

    // size = Mcal_Usart_AppReceiveData(LOG_SERVICE_USART_CH, data, 99);
    // if (size > 0)
    // {
    //     MCAL_DEBUG("B\r\n", 3);
    //     MCAL_DEBUG("%s", data);
    // }
}

void Mcal_CP_Test(void)
{
    static uint8_t step = 0;
    static uint8_t cnt = 0;
    //	  float duty = 0;
    //    uint32_t Frequency = 0;
    //   uint32_t CaptureValue[2] = {0};
    //    static float dutyLast  = 0;
    //    static uint32_t FrequencyLast = 0;

    switch (step)
    {
    case 0:
        /* code */
        SwitchM_SetCpVol12vMode(SYS_CONNECTOR1);
        // SwitchM_SetCpVol12vMode(SYS_CONNECTOR2);
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 1000, 200);
        Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP1);
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP2, 1000, 100);
        Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP2);
        MCAL_DEBUG("set connect(1)(2) 12V \r\n");
        step++;
        break;
    case 1:
        if (cnt++ == 10)
        {
            cnt = 0;
            step++;
        }
        break;
    case 2:
        /* code */
        SwitchM_SetCpVol4vMode(SYS_CONNECTOR1);
        // SwitchM_SetCpVol4vMode(SYS_CONNECTOR2);
        MCAL_DEBUG("set connect(1)(2) 4V\r\n");
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 1000, 1000);
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP2, 1000, 1000);
        step++;
        break;
    case 3:
        if (cnt++ == 10)
        {
            cnt = 0;
            step++;
        }
        break;
    case 4:
        /* code */
        // MCAL_DEBUG("set CP1 CP2 PWM OUT\r\n");
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 1000, 255);
        Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP1);
        Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP2, 1000, 100);
        Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP2);
        step++;
        break;
    case 5:
#if 0
            Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 2000, 60); // Set PWM output test channel to 50% duty cycle
            // Start PWM output test
            Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP);
            Mcal_GptDrrv_Icu_It_Start(MCAL_GPT_PWM_CAPTURE1_TEST); // Start PWM capture test channel
            Mcal_GptDrrv_Icu_It_Start(MCAL_GPT_PWM_CAPTURE_CP); // Start PWM capture test channel
#endif
        if (cnt++ == 10)
        {
            cnt = 0;
            Mcal_GptDrv_Pwm_Stop(MCAL_GPT_CH_PWM_OUT_CP1); 
            Mcal_GptDrv_Pwm_Stop(MCAL_GPT_CH_PWM_OUT_CP2);
            Mcal_Adc_ConvCollection_Enable(MCAL_ADC_CHANNEL_1);
            Mcal_Adc_ConvCollection_Enable(MCAL_ADC_CHANNEL_1);
            MCAL_DEBUG("stop CP1 CP2 PWM OUT\r\n");
            step++;
        }
        break;
    case 6:
        // 使能相关定时器功能
#if 0
            Mcal_GptDrrv_Icu_It_GetValue(MCAL_GPT_PWM_CAPTURE1_TEST, &CaptureValue[0]);
            if(CaptureValue[0] != 0)
            {
                MCAL_DEBUG("Capture0 Value: %lu\r\n", CaptureValue[0]);
            }
            Mcal_GptDrrv_Icu_It_GetValue(MCAL_GPT_PWM_CAPTURE_CP, &CaptureValue[1]);
            if(CaptureValue[1] != 0)
            {
                MCAL_DEBUG("Capture1 Value: %lu\r\n", CaptureValue[1]);
            }
           
            //获取频率和占空比
            Mcal_GptDrrv_Icu_It_GetFrequency(MCAL_GPT_PWM_CAPTURE1_TEST, &Frequency);
            Mcal_GptDrrv_Icu_It_GetDutyCycle(MCAL_GPT_PWM_CAPTURE1_TEST, &duty);
            if(Frequency != FrequencyLast)
            {
                FrequencyLast = Frequency;
                MCAL_DEBUG("Frequency: %lu Hz\r\n", Frequency);
            }

            if(duty != dutyLast)
            {
                dutyLast = duty;
                MCAL_DEBUG("Duty Cycle: %.2f%%\r\n", duty);
            }
#endif
        if (cnt++ == 10)
        {
            cnt = 0;
            Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP1, 1000, 800); // Set cp1 level hight
            Mcal_Set_Pwm_Param(MCAL_GPT_CH_PWM_OUT_CP2, 1000, 650); // Set cp2 level hight
            Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP1);
            Mcal_GptDrv_Pwm_Start(MCAL_GPT_CH_PWM_OUT_CP2);
            SwitchM_SetCpVol12vMode(SYS_CONNECTOR1);
            MCAL_DEBUG("RESTART CP1 CP2 PWM OUT AND SET CP1 12v\r\n");
            step++;
        }
        break;
    case 7:
        if (cnt++ == 10)
        {
            cnt = 0;
            // SwitchM_SetCpVol12vMode(SYS_CONNECTOR2);
            SwitchM_SetCpVol4vMode(SYS_CONNECTOR1);
            MCAL_DEBUG("SET CP2 12v; cp1 4v\r\n");
            step++;
        }
        break;
    case 8:
        if (cnt++ == 10)
        {
            cnt = 0;
            // SwitchM_SetCpVol4vMode(SYS_CONNECTOR2);
            MCAL_DEBUG("SET CP2 4v\r\n");
            step++;
        }
        break;
    default:

        break;
    }
}

void Mcal_Can_Send_Test(void)
{
    static uint8_t step = 0;

    if (8 > step)
    {
        // Example data to send via CAN
        uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

        McalRetVal_t ret;

        // Send a CAN message
        ret = Mcal_Can_Send_Msg(MCAL_CAN_TX_TEST, data, sizeof(data));
        if (ret == MCAL_RET_SUCCESS)
        {
            MCAL_DEBUG("CAN message sent successfully. TxMailbox:%d \r\n", Mcal_Can_Get_TxMailbox(MCAL_CAN_TX_TEST));
        }
        else
        {
            MCAL_DEBUG("Failed to send CAN message. Error code: %d\r\n", ret);
        }
        step++;
    }
}

void Mcal_Can_Rcv_Test(void)
{
    uint8_t i = 0;
    uint8_t RcvBUff[256] = {0};
    uint8_t RcvLen = 0;

    RcvLen = Mcal_Can_Receive_Msg(MCAL_CAN1_RX_MCU_STATUS3, RcvBUff, sizeof(RcvBUff));
    if (RcvLen)
    {
        // Process received CAN message
        MCAL_DEBUG(" Rcv Len:%d Received CAN message: \r\n", RcvLen);
        for (i = 0; i < RcvLen; i++)
        {
            MCAL_DEBUG("0x%02X ", RcvBUff[i]);
            if (0 == (i + 1)%8)
            {
                MCAL_DEBUG("\r\n");
            }
        }
    }
}

void Mcal_SoftReset_test(void)
{
    static uint8_t cnt = 0;
    cnt++;
    if(3000/MCAL_TEST_PERIOD == cnt)
    {
        Mcal_MCU_SysRestart();
    }
}

McalRetVal_t Mcal_eep_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	McalRetVal_t ret;

	// ret = Mcal_I2cDrv_Write(MCAL_IIC_CHANNEL_0, 0xA0, WriteAddr, MCAL_I2C_MEMADD_SIZE_16BIT, pBuffer, NumToWrite);

	return ret;
}

McalRetVal_t Mcal_eep_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	McalRetVal_t ret;

	// ret = Mcal_I2cDrv_Read(MCAL_IIC_CHANNEL_0, 0XA0, ReadAddr, MCAL_I2C_MEMADD_SIZE_16BIT, pBuffer, NumToRead);

	return ret;
}

typedef enum
{
	EEP_STEP_1 = 0U,
	EEP_STEP_2,
	EEP_STEP_3,
	EEP_STEP_4
} EEP_Enum;

void MCAL_TestIIC(void)
{
	uint8_t num = 8;
	uint8_t Buffer[8] = {0};
	uint16_t Addr = 0x00;
	static EEP_Enum Step = EEP_STEP_1;
    McalRetVal_t ret;

	switch(Step)
	{
		case EEP_STEP_1:
		{
      ret =  Mcal_eep_Read(Addr,Buffer,num);
			if(MCAL_RET_SUCCESS == ret)
			{
				Step = EEP_STEP_2;
				MCAL_DEBUG("init read data:\r\n");
				MCAL_PRINT_HEX(Buffer, 8, 1);
			}
		}
		break;

		case EEP_STEP_2:
        {
            uint8_t SrcData[8] = {0xff, 3, 3, 4, 5, 6, 7, 8};

            ret = Mcal_eep_Write(Addr, SrcData, num);
            if (MCAL_RET_SUCCESS == ret)
            {
                Step = EEP_STEP_3;
                MCAL_DEBUG("eep write data:\r\n");
                MCAL_PRINT_HEX(SrcData, 8, 1);
            }
            else
            {
                MCAL_DEBUG("eep write ret: %d:\r\n", ret);
            }
        }
        break;

        case EEP_STEP_3:
        {
            ret = Mcal_eep_Read(Addr, Buffer, num);
            if (MCAL_RET_SUCCESS == ret)
            {
                Step = EEP_STEP_4;
                MCAL_DEBUG("eep read data:\r\n");
                MCAL_PRINT_HEX(Buffer, 8, 1);
            }
            else
            {
                MCAL_DEBUG("eep read2 ret: %d:\r\n", ret);
            }
        }
        break;

		case EEP_STEP_4:
		{
            MCAL_DEBUG("%s\r\n",__FUNCTION__);
		}
		break;


		default:
		{
			Step = EEP_STEP_4;
		}
		break;
	}
}

/*FLASH常用命令*/
#define W25X_WriteEnable                0x06
#define W25X_WriteDisable               0x04
#define W25X_ReadStatusReg              0x05
#define W25X_WriteStatusReg             0x01
#define W25X_ReadData                   0x03
#define W25X_FastReadData               0x0B
#define W25X_FastReadDual               0x3B
#define W25X_PageProgram                0x02
#define W25X_BlockErase                 0xD8
#define W25X_SectorErase                0x20
#define W25X_ChipErase                  0xC7
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown           0xAB
#define W25X_DeviceID                   0xAB
#define W25X_ManufactDeviceID           0x90
#define W25X_JedecDeviceID              0x9F
/*其它*/
#define W25Qx_TIMEOUT_VALUE             1000
#define sFLASH_ID                       0XEF4017
#define Dummy_Byte                      0xFF

#define W25Q128FV_BULK_ERASE_MAX_TIME         250000
#define W25Q128FV_SECTOR_ERASE_MAX_TIME       3000
#define W25Q128FV_SUBSECTOR_ERASE_MAX_TIME    800

/* Flag Status Register */
#define W25Q128FV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q128FV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q128FV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */

#define W25Qx_OK                              ((uint8_t)0x00)
#define W25Qx_ERROR                           ((uint8_t)0x01)
#define W25Qx_BUSY                            ((uint8_t)0x02)
#define W25Qx_TIMEOUT				    	  ((uint8_t)0x03)

#define W25Qx_Enable()                  Mcal_Gpio_ResetPin(W25_CS_GPIO_Port, W25_CS_Pin)
#define W25Qx_Disable()                 Mcal_Gpio_SetPin(W25_CS_GPIO_Port, W25_CS_Pin)

static uint8_t Mcal_W25Qx_GetStatus(void)
{
    uint8_t cmd[] = {W25X_ReadStatusReg};
    uint8_t status;

    W25Qx_Enable();
    /* Send the read status command */
    Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 1, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    Mcal_SpiDrv_ReadData(MCAL_SPI1_W25Q64_CH, &status, 1, W25Qx_TIMEOUT_VALUE);
    W25Qx_Disable();

    /* Check the value of the register */
    if ((status & W25Q128FV_FSR_BUSY) != 0)
    {
        return W25Qx_BUSY;
    }
    else
    {
        return W25Qx_OK;
    }
}

static uint8_t Mcal_W25Qx_WriteEnable(void)
{
    uint8_t cmd[] = {W25X_WriteEnable};
    uint32_t tickstart = Mcal_SYSTICK_Get_Counter();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 1, W25Qx_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while (Mcal_W25Qx_GetStatus() == W25Qx_BUSY)
        ;
    {
        /* Check for the Timeout */
        if ((Mcal_SYSTICK_Get_Counter() - tickstart) > W25Qx_TIMEOUT_VALUE)
        {
            return W25Qx_TIMEOUT;
        }
    }

    return W25Qx_OK;
}

static void Mcal_W25Qx_Read_ID(uint8_t *ID)
{
    uint8_t cmd[4] = {W25X_ManufactDeviceID, Dummy_Byte, Dummy_Byte, 0x00};

    W25Qx_Enable();
    /* Send the read ID command */
    Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 4, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    Mcal_SpiDrv_ReadData(MCAL_SPI1_W25Q64_CH, ID, 2, W25Qx_TIMEOUT_VALUE);
    W25Qx_Disable();
}

static uint8_t Mcal_W25Q64_ReadReg(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4];

    /* Configure the command */
    cmd[0] = W25X_ReadData;
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);

    W25Qx_Enable();
    /* Send the read ID command */
    Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 4, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    if (Mcal_SpiDrv_ReadData(MCAL_SPI1_W25Q64_CH, pData, Size, W25Qx_TIMEOUT_VALUE) != MCAL_RET_SUCCESS)
    {
        return W25Qx_ERROR;
    }
    W25Qx_Disable();

    return W25Qx_OK;
}

static uint8_t Mcal_W25Qx_Erase_Block(uint32_t Address)
{
	uint8_t cmd[4];
	uint32_t tickstart = Mcal_SYSTICK_Get_Counter();
	cmd[0] = W25X_SectorErase;
	cmd[1] = (uint8_t)(Address >> 16);
	cmd[2] = (uint8_t)(Address >> 8);
	cmd[3] = (uint8_t)(Address);
	
	/* Enable write operations */
    if(W25Qx_OK != Mcal_W25Qx_WriteEnable())
    {
        return W25Qx_ERROR;
    }
	
	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 4, W25Qx_TIMEOUT_VALUE);	
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();
	
	/* Wait the end of Flash writing */
	while(Mcal_W25Qx_GetStatus() == W25Qx_BUSY);
	
		/* Check for the Timeout */
    if((Mcal_SYSTICK_Get_Counter() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME)
    {        
			return W25Qx_TIMEOUT;
    }
	
	return W25Qx_OK;
}

static uint8_t W25Q64_WriteReg(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
    uint8_t cmd[4];
    uint32_t tickstart = Mcal_SYSTICK_Get_Counter();

    /* Configure the command */
    cmd[0] = W25X_PageProgram;
    cmd[1] = (uint8_t)(WriteAddr >> 16);
    cmd[2] = (uint8_t)(WriteAddr >> 8);
    cmd[3] = (uint8_t)(WriteAddr);

    /* Enable write operations */
    if(W25Qx_OK != Mcal_W25Qx_WriteEnable())
    {
        return W25Qx_ERROR;
    }

    W25Qx_Enable();
    /* Send the command */
    if (Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, cmd, 4, W25Qx_TIMEOUT_VALUE) != MCAL_RET_SUCCESS)
    {
        return W25Qx_ERROR;
    }

    /* Transmission of the data */
    if (Mcal_SpiDrv_SendData(MCAL_SPI1_W25Q64_CH, pData, Size, W25Qx_TIMEOUT_VALUE) != MCAL_RET_SUCCESS)
    {
        return W25Qx_ERROR;
    }
    W25Qx_Disable();
    /* Wait the end of Flash writing */
    while (Mcal_W25Qx_GetStatus() == W25Qx_BUSY);
    
    /* Check for the Timeout */
    if ((Mcal_SYSTICK_Get_Counter() - tickstart) > W25Qx_TIMEOUT_VALUE)
    {
        return W25Qx_TIMEOUT;
    }

    return W25Qx_OK;
}

void Mcal_Test_Spi(void)
{
    uint16_t DeviceID = 0;
    static uint8_t step = 0;
    uint8_t Buff[8] = {0};
    uint8_t WriteData[8] = {1,1,1,1,1,1,1,1};

    switch (step)
    {
        case 0:
            /* 获取 Flash Device ID */
            Mcal_W25Qx_Read_ID((uint8_t*)&DeviceID);
            MCAL_DEBUG("Manufacturer Device ID is 0x%X\r\n", DeviceID);
            step++;
            break;

        case 1:
            if(W25Qx_OK == Mcal_W25Q64_ReadReg(Buff, 0x00, 8))
            {
                MCAL_DEBUG("case1 sector 0 data:\r\n");
                MCAL_PRINT_HEX(Buff, 8, 1);
            }
            if (W25Qx_OK == Mcal_W25Qx_Erase_Block(0x00))
            {
                MCAL_DEBUG("SPI Erase Block ok\r\n");

                if(W25Qx_OK == W25Q64_WriteReg(WriteData, 0x00, 8))
                {
                    MCAL_DEBUG("Write sector 0 data:\r\n");
                    MCAL_PRINT_HEX(WriteData, 8, 1);
                }
            }
            step++;
            break;
        
        case 2:
            if(W25Qx_OK == Mcal_W25Q64_ReadReg(Buff, 0x00, 8))
            {
                MCAL_DEBUG("case2 sector 0 data:\r\n");
                MCAL_PRINT_HEX(Buff, 8, 1);
            }
            step++;
            break;

        default:
            break;
    }
}

void Mcal_Test_Adc(void)
{
    uint32_t Val = 0;

    Val = Mcal_AdcDrv_GetAdcValue(MCAL_ADC1_CP1_CH);
    MCAL_DEBUG("cp1 Collection Val :%d\r\n", Val);

    Val = Mcal_AdcDrv_GetAdcValue(MCAL_ADC1_CP2_CH);
    MCAL_DEBUG("cp2 Collection Val :%d\r\n", Val);
}

void Mcal_Gpio_Test(void)
{
    // uint8_t VolLevel = 0;
#if 0
    static uint8_t step = 1;
    static uint8_t cnt = 0;
    uint8_t RelayMos1OffStatus,RelayMos1OnStatus,RelayMos2OffStatus,RelayMos2OnStatus;

    switch (step)
    {
    case 1:
        if(cnt++ == 20)
        {
            MOSDRV_ReqMosOn(SYS_CONNECTOR1);
            // MOSDRV_ReqMosOn(SYS_CONNECTOR2);
            MCAL_DEBUG("%s SET CONNECTOR RELAY ON\r\n", __FUNCTION__);
            cnt = 0;
            step++;
        }
        break;
    case 2:
        if(cnt++ == 20)
        {
            MOSDRV_ReqMosOff(SYS_CONNECTOR1);
            // MOSDRV_ReqMosOff(SYS_CONNECTOR2);
            MCAL_DEBUG("%s SET CONNECTOR RELAY OFF\r\n", __FUNCTION__);
            cnt = 0;
            step++;
        }
    case 3:
        if(cnt++ == 20)
        {
            MOSDRV_ReqMosOn(SYS_CONNECTOR1);
            // MOSDRV_ReqMosOn(SYS_CONNECTOR2);
            MCAL_DEBUG("%s SET CONNECTOR RELAY ON\r\n", __FUNCTION__);
            cnt = 0;
            step++;
        }
    default:
        break;
    }
    RelayMos1OffStatus = MOSDRV_ReadGpioValue(MOS_DRV_CONNECTOR1_OFF);
    RelayMos1OnStatus = MOSDRV_ReadGpioValue(MOS_DRV_CONNECTOR1_ON);
    RelayMos2OffStatus = MOSDRV_ReadGpioValue(MOS_DRV_CONNECTOR2_OFF);
    RelayMos2OnStatus = MOSDRV_ReadGpioValue(MOS_DRV_CONNECTOR2_ON);
    // MCAL_DEBUG("%s Read Relay Status: Mos1 off:%d Mos1 on:%d Mos2 off:%d Mos2 on:%d\r\n",
               //__FUNCTION__, RelayMos1OffStatus, RelayMos1OnStatus, RelayMos2OffStatus, RelayMos2OnStatus);
#endif
    // VolLevel = MOSDRV_ReadAuxiliaryGpioValue(SYS_CONNECTOR1);
    
    // MCAL_DEBUG("%s Read Auxiliary Status: %d\r\n", __FUNCTION__, VolLevel);
}

void Mcal_test_1ms(void)
{
    // static uint8_t cnt = 0;
    // static uint8_t flag = 0;

    // if (0 == flag)
    // {
    //     SwitchM_Set74hct4851dEnable();
    //     Mcal_Adc_ConvCollection_Enable(MCAL_ADC_CHANNEL_1);
    //     Mcal_Adc_ConvCollection_Enable(MCAL_ADC_CHANNEL_2);
    //     flag = 1;
    // }
    // SwitchM_74hct4851dControlCallBack();
}

/* Run MCAL tests */
void Mcal_Test_Run(void)
{
    /* TODO: Add test code here */
    // Mcal_Usart_Test();
	// Mcal_CP_Test();
    // Mcal_Gpio_Test();
    Mcal_Can_Rcv_Test();
    // Mcal_SoftReset_test();
    // MCAL_TestIIC();
    // Mcal_Test_Spi();
    // Mcal_Test_Adc();
    // MCAL_DEBUG("%s\r\n", __FUNCTION__);
}
