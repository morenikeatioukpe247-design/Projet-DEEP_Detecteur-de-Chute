/*
 * screen.h
 *
 *  Created on: Apr 29, 2026
 *      Author: raafa
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

void Screen_Init(void);
void Screen_Update(void);
void Screen_SetState(const char* state);
void Screen_SetBPM(uint8_t bpm);
void Screen_ShowElectrodeStatus(bool connected);

#endif /* SCREEN_H_ */
