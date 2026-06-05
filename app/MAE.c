
/*
 * MAE.c
 *
 *  Created on: Mar 27, 2026
 *      Author: raafa
 */

#include "stm32g4_adc.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "config.h"

#include "MAE.h"
#include "button.h"
#include "led.h"
#include "bluetooth.h"
#include "buzzer.h"
#include "screen.h"
#include "capteurPouls.h"
#include "MPU6050/stm32g4_mpu6050.h"



static alerte_cause_e cause_alerte = CAUSE_CHUTE;
static MPU6050_t mpu_data;
static bool mpu_ok = false;

static system_state_e state = NORMAL;
static uint32_t timestamp = 0;
static bool alerte_envoyee = false;

static bool pouls_anormal(uint8_t bpm)
{
    if (bpm == 0) return false;  // pas encore de mesure valide
    return (bpm < 50 || bpm > 100);
}

void MPU_Init(void)
{
    MPU6050_Result_t res = MPU6050_Init(
        &mpu_data,
        NULL,
        0,
        MPU6050_Device_0,
        MPU6050_Accelerometer_8G,
        MPU6050_Gyroscope_2000s
    );

    if (res == MPU6050_Result_Ok)
    {
        mpu_ok = true;
        printf("MPU6050 init OK\n");
    }
    else
    {
        mpu_ok = false;
        printf("MPU6050 init ERREUR\n");
    }
}
static bool chute_detectee(void)
{
    if (!mpu_ok) return false;
    MPU6050_ReadAccelerometer(&mpu_data);
    int16_t az = mpu_data.Accelerometer_Z / 410;
    printf("AZ = %d\n", az);
    return (az < -12 || az > 12);
}


static bool immobilite_detectee(void)
{
    if (!mpu_ok) return false;
    MPU6050_ReadAccelerometer(&mpu_data);
    int16_t ax = mpu_data.Accelerometer_X / 410;
    int16_t ay = mpu_data.Accelerometer_Y / 410;
    int16_t az = mpu_data.Accelerometer_Z / 410;
    return (ax > -5 && ax < 5 && ay > -5 && ay < 5);
    (void)az;
}




void system_state_machine(void)
    {
        button_event_t evt = BUTTON_state_machine();
        uint32_t now = HAL_GetTick();

        uint8_t bpm = Pouls_GetBPM();
        bool electrodes_ok = Pouls_ElectrodesConnectees();


        // Mode jour/nuit
        uint16_t lumiere = BSP_ADC_getValue(ADC_2);
        if (lumiere < ADC_NIGHT_THRESHOLD)
        {
            LED_setNuit();
            printf("MODE NUIT\n");
        }
        else
        {
            LED_setJour();
            printf("MODE JOUR\n");
        }

        switch (state)
        {
        case NORMAL:
            BUZZER_Off();
            alerte_envoyee = false;
            Screen_SetState("NORMAL");

            if (chute_detectee())
            {
                timestamp = now;
                BUZZER_On();
                cause_alerte = CAUSE_CHUTE;
                Screen_SetState("CHUTE SUSPECTEE");
                state = SUSPICION_CHUTE;
            }
            else if (electrodes_ok && pouls_anormal(bpm))
            {
                timestamp = now;
                BUZZER_On();
                cause_alerte = CAUSE_POULS;
                Screen_SetState("POULS ANORMAL");
                state = SUSPICION_POULS;
            }
            break;

        case SUSPICION_POULS:

            Screen_SetState("POULS ANORMAL");

            if (evt == BUTTON_EVENT_PRESSED)
            {
                BUZZER_Off();
                state = NORMAL;
            }
            else if ((now - timestamp) > 5000)
            {
                timestamp = now;
                state = ATTENTE_CONFIRMATION;
            }
            break;

            case SUSPICION_CHUTE:
                Screen_SetState("CHUTE SUSPECTEE");

                if (evt == BUTTON_EVENT_PRESSED)
                {
                    BUZZER_Off();
                    state = NORMAL;
                }
                else if ((now - timestamp) > 5000)
                {
                    timestamp = now;
                    state = ATTENTE_CONFIRMATION;
                }
                break;

            case ATTENTE_CONFIRMATION:
                Screen_SetState("ATTENTE CONFIRM.");

                if (evt == BUTTON_EVENT_PRESSED)
                {
                    BUZZER_Off();
                    state = NORMAL;
                }
                else if ((now - timestamp) > 10000)
                {
                    if (!alerte_envoyee)
                    {
                        uint16_t lum = BSP_ADC_getValue(ADC_2);
                        ModeType_t mode = (lum < ADC_NIGHT_THRESHOLD) ? MODE_NUIT_BT : MODE_JOUR_BT;

                        // On choisit le type d'alerte selon la cause
                        AlerteType_t type = (cause_alerte == CAUSE_CHUTE) ? ALERTE_CHUTE : ALERTE_AUCUNE_REPONSE;

                        BT_SendAlert(type, bpm, mode);
                        alerte_envoyee = true;
                    }
                    Screen_SetState("ALERTE ENVOYEE");
                    state = ALERTE_ENVOYEE;
                }
                break;

            case ALERTE_ENVOYEE:
                Screen_SetState("ALERTE ENVOYEE");
                BUZZER_On();

                if (immobilite_detectee())
                {
                    BUZZER_Off();
                    state = NORMAL;
                }
                break;
        }

        Screen_SetBPM(Pouls_GetBPM());
        Screen_ShowElectrodeStatus(Pouls_ElectrodesConnectees());
        Screen_Update();
    }
