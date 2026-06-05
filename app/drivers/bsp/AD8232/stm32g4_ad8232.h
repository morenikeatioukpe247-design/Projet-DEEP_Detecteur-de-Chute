/*
 * stm32g4_ad8232.h
 *
 *  Created on: May 29, 2026
 *      Author: raafa
 */

#ifndef BSP_AD8232_STM32G4_AD8232_H_
#define BSP_AD8232_STM32G4_AD8232_H_

#include "stm32g4xx_hal.h"
#include "stm32g4_adc.h"
#include "stm32g4_gpio.h"
#include <stdbool.h>
#include <stdint.h>

void     AD8232_init(void);
uint16_t AD8232_getRaw(void);
bool     AD8232_electrodesConnectees(void);



#endif /* BSP_AD8232_STM32G4_AD8232_H_ */
