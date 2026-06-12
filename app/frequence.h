/*
 * frequence.h
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */


#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "stm32g4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void     freq_Init(void);
uint16_t freq_GetRaw(void);
bool     freq_ElectrodesConnectees(void);
uint8_t  freq_GetBPM(void);
bool freq_IsValid(void);


#endif /*FREQUENCE_H*/

