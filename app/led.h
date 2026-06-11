/**
 * @file    led.h
 * @author  raafa
 * @date    Mar 25, 2026
 * @brief   Module de gestion de la LED indicatrice de mode jour/nuit.
 *
 * Ce module gere la LED bleue connectee sur PA11 qui indique
 * le mode jour (eteinte) ou nuit (allumee).
 */

#ifndef LED_H_
#define LED_H_

#include "stm32g4xx_hal.h"
#include "stm32g4_gpio.h"

#define LED_BLUE_PIN    GPIO_PIN_11

#define LED_BLUE_GPIO   GPIOA

void LED_init(void);

void LED_setJour(void);

void LED_setNuit(void);

void LED_BLINKY(uint16_t ms);

#endif /* LED_H_ */
