/*
 * bluetooth.h
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_


#include <stdint.h>
#include "stm32g4_uart.h"

typedef enum {
    ALERTE_CHUTE,
    ALERTE_AUCUNE_REPONSE
} AlerteType_t;

typedef enum {
    MODE_JOUR_BT,
    MODE_NUIT_BT
} ModeType_t;

void BT_Init(void);
void BT_SendAlert(AlerteType_t type, uint8_t bpm, ModeType_t mode);

#endif /* BLUETOOTH_H_ */
