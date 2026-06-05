/*
 * bluetooth.c
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */

#include "bluetooth.h"
#include "stm32g4_uart.h"

#include <stdio.h>
#include <string.h>

void BT_Init(void)
{
	BSP_UART_init(UART1_ID, 9600);
}

// Déclaration externe  du handle UART généré par CubeMX
void BT_SendAlert(AlerteType_t alerte, uint8_t bpm, ModeType_t mode)
{
    char buffer[128];

    const char* alerteStr;
    if (alerte == ALERTE_CHUTE)
        alerteStr = "CHUTE";
    else
        alerteStr = "AUCUNE REPONSE";

    const char* modeStr;
        if (mode == MODE_JOUR_BT)
            modeStr = "jour";
        else
            modeStr = "nuit";

    snprintf(buffer, sizeof(buffer),
             "ALERTE | %s | BPM=%d | MODE=%s\r\n",
             alerteStr, bpm, modeStr);

    BSP_UART_puts(UART1_ID, (uint8_t*)buffer, strlen(buffer));
}
