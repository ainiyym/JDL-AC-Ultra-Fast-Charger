/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stm32f1xx_hal.h>
#include "Mcal_Spi_Cfg.h"
#include "Mcal_Gpio_Cfg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static SemaphoreHandle_t spi_semaphore;

typedef struct {
    SPI_TypeDef *spix;
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *cs_gpiox;
    uint16_t cs_gpio_pin;
} spi_user_data, *spi_user_data_t;

static void spi_lock(const sfud_spi *spi) {
    xSemaphoreTake(spi_semaphore, portMAX_DELAY);
}

static void spi_unlock(const sfud_spi *spi) {
    xSemaphoreGive(spi_semaphore);
}

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    spi_user_data_t spi_dev = (spi_user_data_t) spi->user_data;
    HAL_StatusTypeDef state = HAL_OK;

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }

    HAL_GPIO_WritePin(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin, GPIO_PIN_RESET);

    if (write_size) {
        state = HAL_SPI_Transmit(spi_dev->spi_handle, (uint8_t *)write_buf, write_size, 1000);
        while (HAL_SPI_GetState(spi_dev->spi_handle) != HAL_SPI_STATE_READY);
    }

    if (state != HAL_OK) {
        goto __exit;
    }

    if (read_size) {
        memset((uint8_t *)read_buf, 0xFF, read_size);
        state = HAL_SPI_Receive(spi_dev->spi_handle, read_buf, read_size, 1000);
        while (HAL_SPI_GetState(spi_dev->spi_handle) != HAL_SPI_STATE_READY);
    }

__exit:

    HAL_GPIO_WritePin(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin, GPIO_PIN_SET);

    return result;
}

/* about 1 millisecond delay */
static void retry_delay_1ms(void)
{
    vTaskDelay(pdMS_TO_TICKS(1));
}

extern SPI_HandleTypeDef hspi1;
static spi_user_data spi1 = { .spix = SPI1, .cs_gpiox = W25_CS_GPIO_Port, .cs_gpio_pin = W25_CS_Pin, .spi_handle = &hspi1};
sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;
    /* create SPI semaphore */
    if (spi_semaphore == NULL) {
        spi_semaphore = xSemaphoreCreateMutex();
        if (spi_semaphore == NULL) {
            SFUD_INFO("Error: Create SPI semaphore failed.\r\n");
            return SFUD_ERR_NOT_FOUND;
        }
    }

    if (!strcmp(flash->spi.name, "SPI1"))
    {
        flash->spi.wr = spi_write_read;
        flash->spi.lock = spi_lock;
        flash->spi.unlock = spi_unlock;
        flash->spi.user_data = &spi1;
        /* about 1 millisecond delay */
        flash->retry.delay = retry_delay_1ms;
        /* about 60 seconds timeout */
        flash->retry.times = 60 * 1000;
    }

    return result;
}
