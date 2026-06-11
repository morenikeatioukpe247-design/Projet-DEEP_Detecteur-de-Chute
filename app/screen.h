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

/**
 * @brief Initialise l'ecran TFT ILI9341 et efface le fond en noir.
 */
void Screen_Init(void);

/**
 * @brief  Met a jour l'affichage si necessaire.
 * @note   N'effectue le redessin que si le flag needs_update est leve.
 */
void Screen_Update(void);

/**
 * @brief  Definit l'etat du systeme a afficher.
 * @param  state : chaine de caracteres decrivant l'etat
 * @note   Leve le flag needs_update automatiquement.
 */
void Screen_SetState(const char* state);

/**
 * @brief  Definit la valeur BPM a afficher.
 * @param  bpm : frequence cardiaque en battements par minute
 * @note   Leve le flag needs_update automatiquement.
 */
void Screen_SetBPM(uint8_t bpm);

/**
 * @brief  Definit le statut de connexion des electrodes.
 * @param  connected : true si electrodes connectees, false sinon
 */
void Screen_ShowElectrodeStatus(bool connected);

#endif /* SCREEN_H_ */
