/*
 * stm32g4_ad8232.c
 *
 *  Created on: May 29, 2026
 *      Author: raafa
 */


#include "stm32g4_ad8232.h"
#include <stdio.h>

void AD8232_init(void)
{
    // LO+ sur PA3, LO- sur PA12
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_3,  GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_NO_AF);
    BSP_GPIO_pin_config(GPIOA, GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_NO_AF);
}

uint16_t AD8232_getRaw(void)
{
    return BSP_ADC_getValue(ADC_1); // PA0
}

bool AD8232_electrodesConnectees(void)
{
    // Affiche l'état des broches pour déboguer
    uint8_t lo_plus  = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
    uint8_t lo_minus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
    printf("LO+ = %d | LO- = %d\n", lo_plus, lo_minus);

    return (lo_plus == GPIO_PIN_RESET && lo_minus == GPIO_PIN_RESET);
}
