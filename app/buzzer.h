/*
 * buzzer.h
 *
 *  Created on: Apr 24, 2026
 *      Author: raafa
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "stm32g4xx_hal.h"
#include "stm32g4_gpio.h"


void BUZZER_init(void);
void BUZZER_On(void);
void BUZZER_Off(void);

#define BUZZER_PIN  GPIO_PIN_4
#define BUZZER_GPIO GPIOB

#endif /* BUZZER_H_ */
