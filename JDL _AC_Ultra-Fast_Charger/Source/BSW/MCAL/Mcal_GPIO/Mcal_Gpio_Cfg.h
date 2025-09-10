#ifndef MCAL_GPIOCFG_H
#define MCAL_GPIOCFG_H

#include "Mcal_Types.h"
#include "stm32f1xx_hal.h"

#define Relay1ControlStop_Pin GPIO_PIN_6
#define Relay1ControlStop_GPIO_Port GPIOE
#define W25_WP_Pin GPIO_PIN_4
#define W25_WP_GPIO_Port GPIOC
#define W25_HOLD_Pin GPIO_PIN_5
#define W25_HOLD_GPIO_Port GPIOC
#define W25_CS_Pin GPIO_PIN_0
#define W25_CS_GPIO_Port GPIOB
#define Relay1ControlStart_Pin GPIO_PIN_7
#define Relay1ControlStart_GPIO_Port GPIOE
#define CpCanSwitch1_Pin GPIO_PIN_8
#define CpCanSwitch1_GPIO_Port GPIOE
#define Switch1_4V_Pin GPIO_PIN_10
#define Switch1_4V_GPIO_Port GPIOE
#define Relay2ControlStop_Pin GPIO_PIN_11
#define Relay2ControlStop_GPIO_Port GPIOE
#define Relay2ControlStart_Pin GPIO_PIN_12
#define Relay2ControlStart_GPIO_Port GPIOE
#define CpCanSwitch2_Pin GPIO_PIN_13
#define CpCanSwitch2_GPIO_Port GPIOE
#define Switch2_4V_Pin GPIO_PIN_15
#define Switch2_4V_GPIO_Port GPIOE
#define Buzzer_Pin GPIO_PIN_14
#define Buzzer_GPIO_Port GPIOB
#define Contactor1DetectsContacts_Pin GPIO_PIN_14
#define Contactor1DetectsContacts_GPIO_Port GPIOD
#define Contactor2DetectsContacts_Pin GPIO_PIN_15
#define Contactor2DetectsContacts_GPIO_Port GPIOD
#define Switch1_12V_Pin GPIO_PIN_6
#define Switch1_12V_GPIO_Port GPIOC
#define Switch2_12V_Pin GPIO_PIN_7
#define Switch2_12V_GPIO_Port GPIOC
#define ACE_Pin GPIO_PIN_9
#define ACE_GPIO_Port GPIOB
#define AS0_Pin GPIO_PIN_0
#define AS0_GPIO_Port GPIOE
#define AS1_Pin GPIO_PIN_1
#define AS1_GPIO_Port GPIOE
#define AS2_Pin GPIO_PIN_2
#define AS2_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOC

#define TEST_E1_Port GPIOB
#define TEST_E1_Pin GPIO_PIN_1
typedef enum
{
    MCAL_GPIO_PIN_RESET = 0U,  /*!< Pin state is low */
    MCAL_GPIO_PIN_SET   = !MCAL_GPIO_PIN_RESET /*!< Pin state is high */
} Mcal_GPIO_PinState;

extern void Mcal_Gpio_Init(void);
extern void Mcal_Gpio_SetPin(GPIO_TypeDef  *GPIOx, uint32_t pin);
extern void Mcal_Gpio_ResetPin(GPIO_TypeDef  *GPIOx, uint32_t pin);
extern Mcal_GPIO_PinState Mcal_Gpio_ReadPinStatus(GPIO_TypeDef  *GPIOx, uint32_t pin);

#endif // MCAL_GPIOCFG_H
