#include "Mcal_Usart_Cfg.h"
#include "wrappers.h"

int32_t HAL_AT_Uart_Init(uart_dev_t *uart)
{
    if (uart == NULL)
    {
        return -1;
    }

    McalUsartChannel_Enum_t Port;
    McalUart_ReConfig_t Config;

    Port = (McalUsartChannel_Enum_t)uart->port;
    Config.baud_rate = uart->config.baud_rate;
    Config.data_width = uart->config.data_width;
    Config.stop_bits = uart->config.stop_bits;
    Config.parity = uart->config.parity;
    Config.flow_control = uart->config.flow_control;
    Config.mode = uart->config.mode;

    McalUsart_ReInit(Port, &Config);
    return 0;
}

int32_t HAL_AT_Uart_Deinit(uart_dev_t *uart)
{
    return 0;
}

int32_t HAL_AT_Uart_Send(uart_dev_t *uart, const void *data, uint32_t size, uint32_t timeout)
{
    if (uart == NULL || data == NULL || size == 0)
    {
        return -1;
    }

    McalUsartChannel_Enum_t Port = (McalUsartChannel_Enum_t)uart->port;
    if (Mcal_Usart_AppSendData(Port, (uint8_t*)data, size))
    {

        return -1;
    }

    return 0;
}

int32_t HAL_AT_Uart_Recv(uart_dev_t *uart, void *data, uint32_t expect_size, uint32_t *recv_size, uint32_t timeout)
{
    if (uart == NULL || data == NULL || expect_size == 0 || recv_size == NULL)
    {
        return -1;
    }

    McalUsartChannel_Enum_t Port = (McalUsartChannel_Enum_t)uart->port;

    *recv_size = Mcal_Usart_AppReceiveData(Port, data, expect_size);
    
    return 0;
}

int32_t HAL_AT_Uart_Recv_Check(uart_dev_t *uart, uint32_t *recv_size)
{
    if (uart == NULL || recv_size == NULL)
    {
        return -1;
    }

    McalUsartChannel_Enum_t Port = (McalUsartChannel_Enum_t)uart->port;

    *recv_size = Mcal_Usart_AppCheckData(Port);

    return 0;
}
