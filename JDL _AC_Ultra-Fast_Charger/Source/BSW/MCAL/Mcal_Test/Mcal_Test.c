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

#include "Mcal_Rtc_Cfg.h"

// 测试结果枚举
typedef enum {
    RTC_TEST_PASS,
    RTC_TEST_FAIL,
    RTC_TEST_ERROR
} RTC_TestResult_t;

// 打印日期时间信息
void print_datetime(const Mcal_RTC_DateTime_t* datetime)
{
    MCAL_INFO("Date: %04d-%02d-%02d, Time: %02d:%02d:%02d\r\n",
           datetime->Date.Year + 2000,  // 假设年份是2位数，需要加上2000
           datetime->Date.Month,
           datetime->Date.Date,
           datetime->Time.Hours,
           datetime->Time.Minutes,
           datetime->Time.Seconds);
}

// 比较两个日期时间是否相等
int compare_datetime(const Mcal_RTC_DateTime_t* dt1, const Mcal_RTC_DateTime_t* dt2)
{
    return (dt1->Date.Year == dt2->Date.Year &&
            dt1->Date.Month == dt2->Date.Month &&
            dt1->Date.Date == dt2->Date.Date &&
            dt1->Time.Hours == dt2->Time.Hours &&
            dt1->Time.Minutes == dt2->Time.Minutes &&
            dt1->Time.Seconds == dt2->Time.Seconds);
}

// 测试1: 基本的设置和获取功能
RTC_TestResult_t test_RTC_DateTime_SetGet(void)
{
    MCAL_INFO("=== 测试1: RTC日期时间设置获取测试 ===\r\n");
    
    Mcal_RTC_DateTime_t original_datetime, set_datetime, get_datetime;
    
    // 首先获取当前RTC时间
    Mcal_RTC_Get_DateTime(&original_datetime);
    MCAL_INFO("当前RTC时间: ");
    print_datetime(&original_datetime);
    
    // 设置一个特定的测试时间
    set_datetime.Date.Year = 23;    // 2023年
    set_datetime.Date.Month = 12;
    set_datetime.Date.Date = 31;
    set_datetime.Time.Hours = 23;
    set_datetime.Time.Minutes = 59;
    set_datetime.Time.Seconds = 30;
    
    MCAL_INFO("设置RTC时间为: ");
    print_datetime(&set_datetime);
    
    // 设置RTC时间
    Mcal_RTC_SetDateTime(set_datetime);
    HAL_Delay(100);  // 等待RTC更新
    
    // 获取RTC时间验证
    Mcal_RTC_Get_DateTime(&get_datetime);
    MCAL_INFO("获取的RTC时间: ");
    print_datetime(&get_datetime);
    
    // 比较设置和获取的时间
    if (compare_datetime(&set_datetime, &get_datetime)) {
        MCAL_INFO("? 设置获取测试通过\r\n");
        
        // 恢复原始时间
        Mcal_RTC_SetDateTime(original_datetime);
        return RTC_TEST_PASS;
    } else {
        MCAL_INFO("? 设置获取测试失败\r\n");
        return RTC_TEST_FAIL;
    }
}

// 测试2: 边界值测试
RTC_TestResult_t test_RTC_Boundary_Values(void)
{
    MCAL_INFO("\n=== 测试2: RTC边界值测试 ===\r\n");
    
    Mcal_RTC_DateTime_t original_datetime, read_datetime;
    
    // 保存原始时间
    Mcal_RTC_Get_DateTime(&original_datetime);
    
    // 测试用例数组
    Mcal_RTC_DateTime_t test_cases[] = {
        // 最小日期时间
        {{RTC_WEEKDAY_SUNDAY, 1, 1, 0}, {0, 0, 0}},  // 2000-01-01 00:00:00
        // 最大日期时间（根据RTC支持的范围）
        {{RTC_WEEKDAY_SUNDAY, 12, 31, 99}, {23, 59, 59}}, // 2099-12-31 23:59:59
        // 闰年测试
        {{RTC_WEEKDAY_SUNDAY, 2, 29, 20}, {12, 0, 0}},  // 2020-02-29 12:00:00
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed_tests = 0;
    
    for (int i = 0; i < num_tests; i++) {
        MCAL_INFO("测试用例 %d: ", i + 1);
        print_datetime(&test_cases[i]);
        
        // 设置时间
        Mcal_RTC_SetDateTime(test_cases[i]);
        HAL_Delay(50);
        
        // 获取时间验证
        Mcal_RTC_Get_DateTime(&read_datetime);
        
        if (compare_datetime(&test_cases[i], &read_datetime)) {
            MCAL_INFO("? 通过\r\n");
            passed_tests++;
        } else {
            MCAL_INFO("? 失败 - 期望: ");
            print_datetime(&test_cases[i]);
            MCAL_INFO("       实际: ");
            print_datetime(&read_datetime);
        }
        
        HAL_Delay(100);
    }
    
    // 恢复原始时间
    Mcal_RTC_SetDateTime(original_datetime);
    
    if (passed_tests == num_tests) {
        MCAL_INFO("边界值测试: 全部通过 (%d/%d)\r\n", passed_tests, num_tests);
        return RTC_TEST_PASS;
    } else {
        MCAL_INFO("边界值测试: 部分失败 (%d/%d)\r\n", passed_tests, num_tests);
        return RTC_TEST_FAIL;
    }
}

// 测试3: 日期过渡测试（月底到月初）
RTC_TestResult_t test_RTC_Date_Transition(void)
{
    MCAL_INFO("\n=== 测试3: RTC日期过渡测试 ===\r\n");
    
    Mcal_RTC_DateTime_t original_datetime, test_datetime, read_datetime;
    
    // 保存原始时间
    Mcal_RTC_Get_DateTime(&original_datetime);
    
    // 测试月底到月初的过渡（例如1月31日到2月1日）
    test_datetime.Date.Year = 23;
    test_datetime.Date.Month = 1;  // 1月
    test_datetime.Date.Date = 31;
    test_datetime.Time.Hours = 23;
    test_datetime.Time.Minutes = 59;
    test_datetime.Time.Seconds = 50;
    
    MCAL_INFO("设置月底时间: ");
    print_datetime(&test_datetime);
    
    Mcal_RTC_SetDateTime(test_datetime);
    HAL_Delay(100);
    
    Mcal_RTC_Get_DateTime(&read_datetime);
    MCAL_INFO("读取的时间: ");
    print_datetime(&read_datetime);
    
    // 恢复原始时间
    Mcal_RTC_SetDateTime(original_datetime);
    
    if (compare_datetime(&test_datetime, &read_datetime)) {
        MCAL_INFO("? 日期过渡测试通过\r\n");
        return RTC_TEST_PASS;
    } else {
        MCAL_INFO("? 日期过渡测试失败\r\n");
        return RTC_TEST_FAIL;
    }
}

// 测试4: 时间过渡测试（59秒到下一分钟）
RTC_TestResult_t test_RTC_Time_Transition(void)
{
    MCAL_INFO("\n=== 测试4: RTC时间过渡测试 ===\r\n");
    
    Mcal_RTC_DateTime_t original_datetime, test_datetime, read_datetime;
    
    // 保存原始时间
    Mcal_RTC_Get_DateTime(&original_datetime);
    
    // 设置时间为59秒，测试分钟过渡
    test_datetime = original_datetime;
    test_datetime.Time.Seconds = 59;
    
    MCAL_INFO("设置59秒时间: ");
    print_datetime(&test_datetime);
    
    Mcal_RTC_SetDateTime(test_datetime);
    HAL_Delay(100);
    
    Mcal_RTC_Get_DateTime(&read_datetime);
    MCAL_INFO("读取的时间: ");
    print_datetime(&read_datetime);
    
    // 恢复原始时间
    Mcal_RTC_SetDateTime(original_datetime);
    
    if (compare_datetime(&test_datetime, &read_datetime)) {
        MCAL_INFO("? 时间过渡测试通过\r\n");
        return RTC_TEST_PASS;
    } else {
        MCAL_INFO("? 时间过渡测试失败\r\n");
        return RTC_TEST_FAIL;
    }
}

// 打印测试结果
void print_test_result(const char* test_name, RTC_TestResult_t result)
{
    const char* status_str[] = {"通过", "失败", "错误"};
    MCAL_INFO("测试 %s: %s\r\n", test_name, status_str[result]);
}

// 运行所有测试
void run_all_rtc_tests(void)
{
    MCAL_INFO("\n********** RTC测试程序开始 **********\r\n");
    
    RTC_TestResult_t results[4];
    int passed = 0, total = 0;
    
    // 执行测试
    results[0] = test_RTC_DateTime_SetGet();
    results[1] = test_RTC_Boundary_Values();
    results[2] = test_RTC_Date_Transition();
    results[3] = test_RTC_Time_Transition();
    
    // 统计结果
    MCAL_INFO("\n********** 测试结果汇总 **********\r\n");
    for (int i = 0; i < 4; i++) {
        if (results[i] == RTC_TEST_PASS) {
            passed++;
        }
        total++;
    }
    
    MCAL_INFO("总测试数: %d, 通过: %d, 失败: %d\r\n", 
           total, passed, total - passed);
    
    if (passed == total) {
        MCAL_INFO("? 所有测试通过！\r\n");
    } else {
        MCAL_INFO("? 有测试失败，请检查RTC硬件和驱动\r\n");
    }
    
    MCAL_INFO("********** RTC测试程序结束 **********\r\n");
}

void print_current_time(void)
{
    Mcal_RTC_DateTime_t read_datetime;
    Mcal_RTC_Get_DateTime(&read_datetime);
    MCAL_INFO("读取的时间: ");
    print_datetime(&read_datetime);
}

#include "STD_Rtc.h"

// 辅助函数：打印RTC时间
void print_rtc_time(const RtcTimedate_Struct *time)
{
    MCAL_INFO("RTC时间: %04d-%02d-%02d %02d:%02d:%02d\n",
           time->usYear, time->usMonth, time->usDay,
           time->usHour, time->usMinutes, time->ucSeconds);
}

// 辅助函数：打印CP56Time2a数据
void print_cp56_data(const uint8_t *cp56_data)
{
    MCAL_INFO("CP56Time2a: ");
    for(int i = 0; i < 7; i++) {
        MCAL_INFO("%02X \r\n", cp56_data[i]);
    }
}

// 测试1: RTC_GetRtcSeconds
void test_RTC_GetRtcSeconds(void)
{
    MCAL_INFO("=== 测试 RTC_GetRtcSeconds ===\n");
    
    uint32_t seconds = 0;
    uint8_t result = RTC_GetRtcSeconds(&seconds);
    
    MCAL_INFO("结果: %s\n", result ? "成功" : "失败");
    MCAL_INFO("获取的秒数: %lu\n", seconds);
    
    if(result == STD_TRUE)
    MCAL_INFO("测试通过!\n\n");
}

// 测试2: RTC_GetCP56Time2a
void test_RTC_GetCP56Time2a(void)
{
    MCAL_INFO("=== 测试 RTC_GetCP56Time2a ===\n");
    
    uint8_t cp56_data[7] = {0};
    uint8_t result = RTC_GetCP56Time2a(cp56_data);
    
    MCAL_INFO("结果: %s\n", result ? "成功" : "失败");
    print_cp56_data(cp56_data);
    
    if(result == STD_TRUE)
    MCAL_INFO("测试通过!\n\n");
}

// 测试3: RTC_SecondsSetRtcDateTime
void test_RTC_SecondsSetRtcDateTime(void)
{
    MCAL_INFO("=== 测试 RTC_SecondsSetRtcDateTime ===\n");
    
    uint32_t test_seconds = 1758706080; // 测试秒数
    uint8_t result = RTC_SecondsSetRtcDateTime(test_seconds);
    
    MCAL_INFO("设置秒数: %lu\n", test_seconds);
    MCAL_INFO("结果: %s\n", result ? "成功" : "失败");
    
    // 验证设置是否成功
    RtcTimedate_Struct current_time;
    RTCIF_GetDateTime(&current_time);
    print_rtc_time(&current_time);
    
    if(result == STD_TRUE)
    MCAL_INFO("测试通过!\n\n");
}

// 测试4: RTC_CP56Time2aSetRtcDateTime
void test_RTC_CP56Time2aSetRtcDateTime(void)
{
    MCAL_INFO("=== 测试 RTC_CP56Time2aSetRtcDateTime ===\n");
    
    // 测试CP56Time2a数据：2024年1月15日 14:30:25
    uint8_t test_cp56[7] = {0x98,0xB7,0x0E,0x11,0x10,0x03,0x14};
    
    MCAL_INFO("设置CP56Time2a数据: ");
    print_cp56_data(test_cp56);
    
    uint8_t result = RTC_CP56Time2aSetRtcDateTime(test_cp56);
    MCAL_INFO("结果: %s\n", result ? "成功" : "失败");
    
    // 验证设置是否成功
    RtcTimedate_Struct current_time;
    RTCIF_GetDateTime(&current_time);
    print_rtc_time(&current_time);
    
    if(result == STD_TRUE)
    MCAL_INFO("测试通过!\n\n");
}

// 测试5: 往返转换测试
void test_round_trip_conversion(void)
{
    MCAL_INFO("=== 往返转换测试 ===\n");
    
    // 测试数据
    uint32_t original_seconds = 1758706412;
    
    // 秒数 -> RTC -> 秒数
    MCAL_INFO("1. 秒数设置RTC测试:\n");
    RTC_SecondsSetRtcDateTime(original_seconds);
    
    uint32_t retrieved_seconds;
    RTC_GetRtcSeconds(&retrieved_seconds);
    
    MCAL_INFO("原始秒数: %lu, 读取秒数: %lu\n", original_seconds, retrieved_seconds);
    
    // CP56Time2a -> RTC -> CP56Time2a
    MCAL_INFO("2. CP56Time2a设置RTC测试:\n");
    uint8_t original_cp56[7] = {0x88, 0x13, 0x2A, 0x10, 0x1F, 0x0C, 0x18}; // 2024-12-31 16:42:05
    uint8_t retrieved_cp56[7] = {0};
    
    RTC_CP56Time2aSetRtcDateTime(original_cp56);
    RTC_GetCP56Time2a(retrieved_cp56);
    
    MCAL_INFO("原始CP56: ");
    print_cp56_data(original_cp56);
    MCAL_INFO("读取CP56: ");
    print_cp56_data(retrieved_cp56);
    
    if(memcmp(original_cp56, retrieved_cp56, 7) == 0)
    MCAL_INFO("往返转换测试通过!\n\n");
}

// 运行所有测试
void run_all_tests(void)
{
    MCAL_INFO("开始RTC模块测试...\n\n");
    
    // 初始化测试数据
    RtcTimedate_Struct init_time = {
        .usYear = 2024,
        .usMonth = 1,
        .usDay = 15,
        .usHour = 10,
        .usMinutes = 30,
        .ucSeconds = 0
    };
    RTCIF_SetDateTime(&init_time);
    
    MCAL_INFO("初始RTC时间: ");
    print_rtc_time(&init_time);
    MCAL_INFO("\n");
    
    // 执行测试
    test_RTC_GetRtcSeconds();
    test_RTC_GetCP56Time2a();
    test_RTC_SecondsSetRtcDateTime();
    test_RTC_CP56Time2aSetRtcDateTime();
    test_round_trip_conversion();
    
    MCAL_INFO("所有测试完成!\n");
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
