/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, AS2_Pin|Relay1ControlStop_Pin|Relay1ControlStart_Pin|CpCanSwitch1_Pin
                          |Switch1_4V_Pin|Relay2ControlStop_Pin|Relay2ControlStart_Pin|CpCanSwitch2_Pin
                          |Switch2_4V_Pin|AS0_Pin|AS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, W25_WP_Pin|Switch1_12V_Pin|Switch2_12V_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W25_HOLD_GPIO_Port, W25_HOLD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, W25_CS_Pin|Buzzer_Pin|ACE_Pin|TEST_E1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : AS2_Pin Relay1ControlStop_Pin Relay1ControlStart_Pin CpCanSwitch1_Pin
                           Switch1_4V_Pin Relay2ControlStop_Pin Relay2ControlStart_Pin CpCanSwitch2_Pin
                           Switch2_4V_Pin AS0_Pin AS1_Pin */
  GPIO_InitStruct.Pin = AS2_Pin|Relay1ControlStop_Pin|Relay1ControlStart_Pin|CpCanSwitch1_Pin
                          |Switch1_4V_Pin|Relay2ControlStop_Pin|Relay2ControlStart_Pin|CpCanSwitch2_Pin
                          |Switch2_4V_Pin|AS0_Pin|AS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : W25_WP_Pin W25_HOLD_Pin Switch1_12V_Pin Switch2_12V_Pin */
  GPIO_InitStruct.Pin = W25_WP_Pin|W25_HOLD_Pin|Switch1_12V_Pin|Switch2_12V_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : W25_CS_Pin Buzzer_Pin ACE_Pin */
  GPIO_InitStruct.Pin = W25_CS_Pin|Buzzer_Pin|ACE_Pin|TEST_E1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Contactor1DetectsContacts_Pin Contactor2DetectsContacts_Pin */
  GPIO_InitStruct.Pin = Contactor1DetectsContacts_Pin|Contactor2DetectsContacts_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
