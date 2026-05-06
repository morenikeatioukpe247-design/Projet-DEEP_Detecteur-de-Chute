/*
 * capteurPouls.c
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */

#include "capteurPouls.h"
#include "stm32g4_adc.h"

static uint8_t last_bpm = 0;

void Pouls_Init(void)
{
    // ADC déjà initialisé par BSP (USE_ADC=1, USE_IN1=1)
}

uint16_t Pouls_GetRaw(void)
{
    return BSP_ADC_getValue(ADC_1); // PA0
}

bool Pouls_ElectrodesConnectees(void)
{
    // LO+ et LO- sur PA1 et PA2
    // Si l'un des deux est HIGH entraine que électrode déconnectée
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET &&
            HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET);
}

uint8_t Pouls_GetBPM(void)
{
    return last_bpm;
}
