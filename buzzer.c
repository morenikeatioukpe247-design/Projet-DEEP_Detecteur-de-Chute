/*
 * buzzer.c
 *
 *  Created on: Apr 24, 2026
 *      Author: raafa
 */


#include "buzzer.h"

void BUZZER_init(void)
{
    BSP_GPIO_pin_config(BUZZER_GPIO, BUZZER_PIN,
                        GPIO_MODE_OUTPUT_PP,
                        GPIO_NOPULL,
                        GPIO_SPEED_FREQ_HIGH,
						GPIO_NO_AF);
}

void BUZZER_On(void)
{

    HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, GPIO_PIN_SET);
}

void BUZZER_Off(void)
{

    HAL_GPIO_WritePin(BUZZER_GPIO, BUZZER_PIN, GPIO_PIN_RESET);
}



