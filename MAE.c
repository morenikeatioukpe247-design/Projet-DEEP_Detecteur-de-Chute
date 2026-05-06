/*
 * MAE.c
 *
 *  Created on: Mar 27, 2026
 *      Author: raafa
 */


#include "MAE.h"
#include "button.h"
#include "led.h"
#include "MPU6050.h"
#include "bluetooth.h"
#include "buzzer.h"
#include "capteurPouls.h"
#include <stdint.h>
#include <stdbool.h>

static system_state_e state = NORMAL;
static uint32_t timestamp = 0;
static bool alerte_envoyee = false;


void system_state_machine(void)
{
    button_event_t evt = BUTTON_state_machine();
    uint32_t now = HAL_GetTick();  // temps actuel en ms

    switch(state)
    {
        case NORMAL:
            LED_setJour();
            BUZZER_Off();
            alerte_envoyee = false;

            if (MPU_chute_detectee())
            {
                timestamp = now; //on note le moment de la chute
                BUZZER_On();
                state = SUSPICION_CHUTE;
            }
        break;

        case SUSPICION_CHUTE:
            if (evt == BUTTON_EVENT_PRESSED)
            {
            	BUZZER_Off();
            	state = NORMAL;
            }
            else if ((now - timestamp) > 5000)   // 5 secondes
            {
            	timestamp = now;  // on repart le timer pour l'état suivant
                state = ATTENTE_CONFIRMATION;
            }
        break;

        case ATTENTE_CONFIRMATION:
            if (evt == BUTTON_EVENT_PRESSED)
            {
            	BUZZER_Off();
                state = NORMAL;
            }
            else if ((now - timestamp)> 10000)   // 10 secondes
            {
            	if (!alerte_envoyee)
            	{
            	BT_SendAlert(ALERTE_CHUTE, /*Pouls_GetBPM(),*/0, MODE_JOUR_BT);
            	alerte_envoyee = true;  // on envoie UNE SEULE FOIS
            	}
            	state = ALERTE_ENVOYEE;
            }
        break;

        case ALERTE_ENVOYEE:
            BUZZER_On();

            if (!MPU_immobilite_detectee())
            {
            	BUZZER_Off();
                state = NORMAL;
            }
        break;
    }
}
