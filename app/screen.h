/**
 * @file    screen.h
 * @author  raafa
 * @date    Apr 29, 2026
 * @brief   Module de gestion de l'affichage sur ecran TFT ILI9341.
 *
 * Ce module gere l'affichage de l'etat du systeme, du BPM
 * et du statut des electrodes sur l'ecran TFT ILI9341.
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
