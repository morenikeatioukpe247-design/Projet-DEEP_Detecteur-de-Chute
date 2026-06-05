/*
 * led.h
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */

#ifndef LED_H_
#define LED_H_
#include "stm32g4xx_hal.h"
#include "stm32g4_gpio.h"


#define LED_BLUE_PIN		GPIO_PIN_11
#define LED_BLUE_GPIO       GPIOA

void LED_init(void);
void LED_setJour(void);
void LED_setNuit(void);
void BSP_GPIO_pin_config(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed, uint32_t GPIO_Alternate);

void LED_BLINKY(uint16_t ms);
#endif /* LED_H_ */
