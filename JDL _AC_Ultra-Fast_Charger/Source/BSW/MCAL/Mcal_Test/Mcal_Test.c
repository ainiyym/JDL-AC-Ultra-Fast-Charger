/*
 * File: Mcal_Test.c
 * Description: MCAL Test Source File
 */
#include "Mcal_Test.h"
#if 0
#include "SwitchM.h"
#include "STD_MosDrv.h"
#include "STD_EvseM.h"
#include "flashdb_wrapper.h"

void Mcal_Usart_Test(void)
{
    uint8_t data[100] = {0};
    uint32_t size = 0;

    size = Mcal_Usart_AppReceiveData(MCAL_USART1_CH, data, 99);
    if (size > 0)
    {
        Mcal_Usart_AppSendData(MCAL_USART1_CH, data, size);
    }
}
  
void Mcal_CP_Test(void)
{
    static uint8_t step = 0;
    static uint8_t cnt = 0;
    //	  float duty = 0;
    //    uint32_t Frequency = 0;
    //   uint32_t CaptureValue[MESSAGE_BUFFER_ID_APP2] = {0};
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
    static uint8_t cnt = 0;

    if (cnt < 10)
    {
        static uint8_t step = 0;
        // Example data to send via CAN
        static uint8_t data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        if (EVSEM_GetChargeStatus(1) == EVSEM_STATE_CAN_MODEL)
        {
            McalRetVal_t ret;

            data[7] = (uint8_t)(step << 4);
            if (16 == ++step)
            {
                step = 0;
                if (5 < ++cnt)
                {
                    data[4] = 0x00;
                }
                else
                {
                    data[4] = 0x03;
                }
            }
            // Send a CAN message
            ret = Mcal_Can_Send_Msg(MCAL_CAN1_TX_TEST, data, sizeof(data));
            if (ret != MCAL_RET_SUCCESS)
            {
                MCAL_DEBUG("Failed to send CAN1 message. Error code: %d\r\n", ret);
            }
            ret = Mcal_Can_Send_Msg(MCAL_CAN2_TX_TEST, data, sizeof(data));
            if (ret != MCAL_RET_SUCCESS)
            {
                MCAL_DEBUG("Failed to send CAN2 message. Error code: %d\r\n", ret);
            }
        }
        EVSEM_SET_CAN_START_COM(0); /* Set CAN communication start */
        EVSEM_SET_CAN_START_COM(1); /* Set CAN communication start */
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
            W25Q64_Read_ID((uint8_t*)&DeviceID);
            MCAL_DEBUG("Manufacturer Device ID is 0x%X\r\n", DeviceID);
            step++;
            break;

        case 1:
            if(W25Qx_OK == Mcal_W25Q64_ReadReg(Buff, 0x00, 8))
            {
                MCAL_DEBUG("case1 sector 0 data:\r\n");
                MCAL_PRINT_HEX(Buff, 8, 1);
            }
            if (W25Qx_OK == W25Q64_Erase_Block(0x00))
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

void Mcal_Test_FlashDB_Get(void)
{
    // Test getting a KV
    char buffer[64];
    size_t actual_len;
    fdb_wrapper_kv_get(&kvdb, "test_key", buffer, sizeof(buffer), &actual_len);
    MCAL_DEBUG("Get KV: %s\r\n", buffer);
}

void Mcal_Test_FlashDB_Set(void)
{
    // Test setting a KV
    const char *value = "Hello, FlashDB!!!!";
    fdb_wrapper_kv_set(&kvdb, "test_key", value);
    MCAL_DEBUG("Set KV: %s\r\n", value);
}

void Mcal_Test_FlashDB_Del(void)
{
    // Test deleting a KV
    fdb_wrapper_kv_del(&kvdb, "test_key");
}

static const uint8_t test_data_normal[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
static const uint8_t test_data_oversize[300] = {0}; // 300字节，超过256的限制

// 测试上下文结构
typedef struct {
    test_state_t current_state;
    test_state_t next_state;
    test_result_t overall_result;
    uint32_t test_counter;
    uint32_t pass_count;
    uint32_t fail_count;
    MessageBuffer_Comm_System_t *comm_system;
    TickType_t start_time;
    uint32_t timeout_ms;
} test_context_t;

// 初始化测试上下文
static void init_test_context(test_context_t *context, MessageBuffer_Comm_System_t *comm_system) {
    context->current_state = TEST_STATE_IDLE;
    context->next_state = TEST_STATE_INIT;
    context->overall_result = TEST_RESULT_IN_PROGRESS;
    context->test_counter = 0;
    context->pass_count = 0;
    context->fail_count = 0;
    context->comm_system = comm_system;
    context->start_time = xTaskGetTickCount();
    context->timeout_ms = 10000; // 10秒总超时
}

// 状态机处理函数 
static test_result_t process_test_state(test_context_t *context)
{
    BaseType_t result;
    MessageBuffer_type_t received_type;
    uint8_t receive_buffer[256];
    uint16_t received_length;

    switch (context->current_state)
    {
        case TEST_STATE_IDLE:
            MCAL_INFO("测试状态: IDLE -> INIT\r\n");
            context->current_state = TEST_STATE_INIT;
            break;

        case TEST_STATE_INIT:
            MCAL_INFO("测试状态: INIT - 初始化测试环境\r\n");
            MCAL_INFO("最大消息大小: %u 字节\r\n", context->comm_system->max_message_size);
            MCAL_INFO("缓冲区空间: APP1->APP2: %u, APP2->APP1: %u\r\n",
                    xMessageBufferSpacesAvailable(context->comm_system->app1_to_app2_buf),
                    xMessageBufferSpacesAvailable(context->comm_system->app2_to_app1_buf));
            context->next_state = TEST_STATE_SEND_NORMAL;
            context->current_state = TEST_STATE_SEND_NORMAL;
            break;

        // ...existing code...
        case TEST_STATE_SEND_NORMAL:
            MCAL_INFO("测试状态: SEND_NORMAL - 测试正常消息发送\r\n");
            result = MessageBuffer_SendMessage(context->comm_system, MESSAGE_BUFFER_TYPE_DATA,
                            test_data_normal, sizeof(test_data_normal),
                            MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(100));

            if (result == pdPASS)
            {
                MCAL_INFO("? 正常消息发送成功: 长度=%u\r\n", sizeof(test_data_normal));
                context->pass_count++;
                context->next_state = TEST_STATE_RECEIVE_NORMAL;
            }
            else
            {
                MCAL_INFO("? 正常消息发送失败\r\n");
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_RECEIVE_NORMAL:
            MCAL_INFO("测试状态: RECEIVE_NORMAL - 测试正常消息接收\r\n");
            result = MessageBuffer_ReceiveMessage(context->comm_system,
                                &received_type, receive_buffer, sizeof(receive_buffer),
                                &received_length, MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(500));

            if (result == pdPASS && received_type == MESSAGE_BUFFER_TYPE_DATA && received_length == sizeof(test_data_normal) &&
                memcmp(receive_buffer, test_data_normal, received_length) == 0)
            {
                MCAL_INFO("? 正常消息接收成功: 类型=%d, 长度=%u\r\n", MESSAGE_BUFFER_TYPE_DATA, received_length);
                context->pass_count++;
                context->next_state = TEST_STATE_SEND_OVERSIZE;
            }
            else
            {
                MCAL_INFO("? 正常消息接收失败: 结果=%d, 类型=%d, 长度=%u\r\n",
                        result, received_type, received_length);
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_SEND_OVERSIZE:
            MCAL_INFO("测试状态: SEND_OVERSIZE - 测试超长消息发送（应失败）\r\n");
            result = MessageBuffer_SendMessage(context->comm_system, MESSAGE_BUFFER_TYPE_DATA,
                            test_data_oversize, sizeof(test_data_oversize),
                            MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(100));

            if (result == pdFAIL)
            {
                MCAL_INFO("? 超长消息正确拒绝: 长度=%u > 最大=%u\r\n",
                        sizeof(test_data_oversize), context->comm_system->max_message_size);
                context->pass_count++;
                context->next_state = TEST_STATE_SEND_ZERO_LENGTH;
            }
            else
            {
                MCAL_INFO("? 超长消息未被正确拒绝\r\n");
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_SEND_ZERO_LENGTH:
            MCAL_INFO("测试状态: SEND_ZERO_LENGTH - 测试零长度消息\r\n");
            result = MessageBuffer_SendMessage(context->comm_system, MESSAGE_BUFFER_TYPE_CTRL,
                            NULL, 0, MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(100));

            if (result == pdPASS)
            {
                MCAL_INFO("? 零长度消息发送成功\r\n");
                context->pass_count++;

                // 验证零长度消息接收
                result = MessageBuffer_ReceiveMessage(context->comm_system,
                                    &received_type, receive_buffer, sizeof(receive_buffer),
                                    &received_length, MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(100));

                if (result == pdPASS && received_type == MESSAGE_BUFFER_TYPE_CTRL && received_length == 0)
                {
                    MCAL_INFO("? 零长度消息接收成功\r\n");
                    context->pass_count++;
                    context->next_state = TEST_STATE_RECEIVE_TIMEOUT;
                }
                else
                {
                    MCAL_INFO("? 零长度消息接收失败\r\n");
                    context->fail_count++;
                    context->next_state = TEST_STATE_ERROR;
                }
            }
            else
            {
                MCAL_INFO("? 零长度消息发送失败\r\n");
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_RECEIVE_TIMEOUT:
            MCAL_INFO("测试状态: RECEIVE_TIMEOUT - 测试接收超时\r\n");
            TickType_t start_time = xTaskGetTickCount();
            result = MessageBuffer_ReceiveMessage(context->comm_system, &received_type,
                                receive_buffer, sizeof(receive_buffer),
                                &received_length, MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(100));

            TickType_t elapsed_time = xTaskGetTickCount() - start_time;

            if (result == pdFAIL && elapsed_time >= pdMS_TO_TICKS(100))
            {
                MCAL_INFO("? 接收超时测试成功: 耗时=%lu ms\r\n",
                        pdTICKS_TO_MS(elapsed_time));
                context->pass_count++;
                context->next_state = TEST_STATE_STRESS_TEST;
            }
            else
            {
                MCAL_INFO("? 接收超时测试失败: 结果=%d, 耗时=%lu ms\r\n",
                        result, pdTICKS_TO_MS(elapsed_time));
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_STRESS_TEST:
            MCAL_INFO("测试状态: STRESS_TEST - 压力测试\r\n");
            static uint32_t stress_count = 0;
            uint8_t stress_data[32];

            // 生成测试数据
            for (int i = 0; i < sizeof(stress_data); i++)
            {
                stress_data[i] = (stress_count + i) & 0xFF;
            }

            result = MessageBuffer_SendMessage(context->comm_system, MESSAGE_BUFFER_TYPE_DATA,
                            stress_data, sizeof(stress_data), MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(50));

            if (result == pdPASS)
            {
                result = MessageBuffer_ReceiveMessage(context->comm_system, &received_type,
                                    receive_buffer, sizeof(receive_buffer),
                                    &received_length,MESSAGE_BUFFER_ID_APP2, pdMS_TO_TICKS(50));

                if (result == pdPASS && received_length == sizeof(stress_data) &&
                    memcmp(receive_buffer, stress_data, received_length) == 0)
                {
                    stress_count++;
                    if (stress_count >= 30)
                    {
                        MCAL_INFO("? 压力测试完成: %u 次循环\r\n", stress_count);
                        context->pass_count++;
                        context->next_state = TEST_STATE_COMPLETE;
                    }
                    else
                    {
                        MCAL_INFO("压力测试进度: %u/10\r\n", stress_count);
                        // 继续压力测试
                        vTaskDelay(pdMS_TO_TICKS(10));
                    }
                }
                else
                {
                    MCAL_INFO("? 压力测试接收失败\r\n");
                    context->fail_count++;
                    context->next_state = TEST_STATE_ERROR;
                }
            }
            else
            {
                MCAL_INFO("? 压力测试发送失败\r\n");
                context->fail_count++;
                context->next_state = TEST_STATE_ERROR;
            }
            context->current_state = context->next_state;
            break;

        case TEST_STATE_COMPLETE:
            MCAL_INFO("测试状态: COMPLETE - 所有测试完成\r\n");
            context->overall_result = (context->fail_count == 0) ? TEST_RESULT_PASS : TEST_RESULT_FAIL;
            break;

        case TEST_STATE_ERROR:
            MCAL_INFO("测试状态: ERROR - 测试过程中发生错误\r\n");
            context->overall_result = TEST_RESULT_FAIL;
            break;

        default:
            MCAL_INFO("未知测试状态: %d\r\n", context->current_state);
            context->overall_result = TEST_RESULT_FAIL;
            break;
    }

    context->test_counter++;
    return context->overall_result;
}

// 打印测试总结
void print_test_summary(const test_context_t *context)
{
    MCAL_INFO("测试总结:\r\n");
    MCAL_INFO("总测试数: %lu\r\n", context->test_counter);
    MCAL_INFO("通过数: %lu\r\n", context->pass_count);
    MCAL_INFO("失败数: %lu\r\n", context->fail_count);
    MCAL_INFO("通过率: %.1f%%\r\n",
              (context->pass_count + context->fail_count) > 0 ? (float)context->pass_count / (context->pass_count + context->fail_count) * 100.0f : 0.0f);
}

// 主测试函数
test_result_t run_comm_test(MessageBuffer_Comm_System_t *comm_system)
{
    test_context_t context;

    MCAL_INFO("开始通信接口测试...\r\n");
    MCAL_INFO("==========================================\r\n");

    init_test_context(&context, comm_system);

    // 状态机循环
    while (context.overall_result == TEST_RESULT_IN_PROGRESS)
    {
        test_result_t result = process_test_state(&context);

        // 检查超时
        TickType_t elapsed_time = xTaskGetTickCount() - context.start_time;
        if (pdTICKS_TO_MS(elapsed_time) > context.timeout_ms)
        {
            MCAL_INFO("测试超时: 已运行 %lu ms\r\n", pdTICKS_TO_MS(elapsed_time));
            context.overall_result = TEST_RESULT_FAIL;
            break;
        }

        // 短暂延迟，让出CPU
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // 打印测试总结
    print_test_summary(&context);

    MCAL_INFO("==========================================\r\n");
    MCAL_INFO("测试完成: %s\r\n",
              context.overall_result == TEST_RESULT_PASS ? "PASS" : "FAIL");

    return context.overall_result;
}
#endif

/* Run MCAL tests */
void Mcal_Test_Run(void)
{
    /* TODO: Add test code here */
    // Mcal_Test_StateMachine();
    // Mcal_Usart_Test();
	// Mcal_CP_Test();
    // Mcal_Gpio_Test();
    // Mcal_Can_Rcv_Test();
    // Mcal_SoftReset_test();
    // MCAL_TestIIC();
    // Mcal_Test_Spi();
    // Mcal_Test_Adc();
    // MCAL_DEBUG("%s\r\n", __FUNCTION__);
}
