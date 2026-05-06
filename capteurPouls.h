/*
 * capteurPouls.h
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */


#ifndef CAPTEURPOULS_H
#define CAPTEURPOULS_H

#include "stm32g4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void     Pouls_Init(void);
uint16_t Pouls_GetRaw(void);
bool     Pouls_ElectrodesConnectees(void);
uint8_t  Pouls_GetBPM(void);

#endif /*CAPTEURPOULS_H*/

