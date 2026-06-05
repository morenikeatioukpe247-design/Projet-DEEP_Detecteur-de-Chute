/*
 * led.c
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */
//Indicateur de mode jour et nuit

#include "led.h"

void LED_init(void)
{
    BSP_GPIO_pin_config(LED_BLUE_GPIO, LED_BLUE_PIN,
                        GPIO_MODE_OUTPUT_PP,
                        GPIO_NOPULL,
                        GPIO_SPEED_FREQ_HIGH,
						GPIO_NO_AF);
}

void LED_setJour(void)
{
    // JOUR = LED éteinte
    HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_RESET);
}

void LED_setNuit(void)
{
    // NUIT = LED allumée
    HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_SET);
}


void LED_BLINKY(uint16_t ms)
{
	while(1)
	{
    HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_RESET);
    HAL_Delay(ms);
    HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_SET);
	}
}
