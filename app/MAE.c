/**
 * @file    MAE.c
 * @author  raafa
 * @date    Mar 27, 2026
 * @brief   Implementation de la machine a etats principale.
 *
 * Orchestre tous les modules du systeme : detection de chute via MPU6050,
 * detection d'anomalie cardiaque via AD8232, gestion du mode jour/nuit
 * via photoresistance, et envoi d'alertes via Bluetooth HC-05.
 */

#include "MAE.h"
#include "config.h"
#include "button.h"
#include "led.h"
#include "bluetooth.h"
#include "buzzer.h"
#include "screen.h"
#include "capteurPouls.h"
#include "MPU6050/stm32g4_mpu6050.h"
#include "stm32g4_adc.h"
#include "stm32g4_sys.h"
#include <stdio.h>


static MPU6050_t mpu_data;
static bool mpu_ok = false;
static system_state_e state = NORMAL;
static uint32_t timestamp = 0;
static bool alerte_envoyee = false;
static alerte_cause_e cause_alerte = CAUSE_CHUTE;

void MAE_Init(void)
{
    MPU6050_Result_t res = MPU6050_Init(
        &mpu_data,
        NULL, 0,
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
    Screen_SetState("NORMAL");

    BUTTON_init(GPIOB, GPIO_PIN_0);   
	BUZZER_init();
	LED_init();
	BT_Init();
	Pouls_Init();
	Screen_Init();
}


static bool chute_detectee(void)
{
    if (!mpu_ok) return false;

    MPU6050_ReadAccelerometer(&mpu_data);
    int16_t az = mpu_data.Accelerometer_Z / 410;
    printf("AZ = %d\n", az);
    return (az < -1);
}

static bool immobilite_detectee(void)
{
    if (!mpu_ok) return false;
    MPU6050_ReadAccelerometer(&mpu_data);
    int16_t ax = mpu_data.Accelerometer_X / 410;
    int16_t ay = mpu_data.Accelerometer_Y / 410;
    return (ax > -5 && ax < 5 && ay > -5 && ay < 5);
}

static bool pouls_anormal(uint8_t bpm)
{
    if (bpm == 0) return false;
    return (bpm < 50 || bpm > 100);
}

void system_state_machine(void)
{
    button_event_t evt   = BUTTON_state_machine();
    uint32_t now         = HAL_GetTick();
    uint8_t bpm          = Pouls_GetBPM();
    bool electrodes_ok   = Pouls_ElectrodesConnectees();


    uint16_t lumiere = BSP_ADC_getValue(ADC_2);
    printf("LUMIERE = %d\n", lumiere);
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

    //Debug et affichage
    Screen_SetBPM(bpm);
    Screen_ShowElectrodeStatus(electrodes_ok);
    Screen_Update();

    switch (state)
    {
        case NORMAL:
            BUZZER_Off();
            alerte_envoyee = false;

            if (chute_detectee())
            {
                timestamp = now;
                BUZZER_On();
                cause_alerte = CAUSE_CHUTE;
                Screen_SetState("CHUTE SUSPECTEE");
                state = SUSPICION_CHUTE;
                printf(">>> CHUTE DETECTEE -> SUSPICION_CHUTE\n");
            }
            else if (electrodes_ok && pouls_anormal(bpm))
            {
                timestamp = now;
                BUZZER_On();
                cause_alerte = CAUSE_POULS;
                state = SUSPICION_POULS;
            }
            break;

        case SUSPICION_CHUTE:
            Screen_SetState("CHUTE SUSPECTEE");

            if (evt == BUTTON_EVENT_PRESSED)
            {
                BUZZER_Off();
                Screen_SetState("NORMAL");
                printf("Bouton -> retour NORMAL\n");
                state = NORMAL;
            }
            else if ((now - timestamp) > 5000)
            {
                timestamp = now;
                printf(">>> 5s écoulées -> ATTENTE_CONFIRMATION\n");
                state = ATTENTE_CONFIRMATION;
            }
            break;

       case SUSPICION_POULS:
            if (evt == BUTTON_EVENT_PRESSED)
            {
                BUZZER_Off();
                Screen_SetState("NORMAL");
                state = NORMAL;
            }
            else if ((now - timestamp) > 5000)
            {
                timestamp = now;
                state = ATTENTE_CONFIRMATION;
            }
            break;

        case ATTENTE_CONFIRMATION:
            if (evt == BUTTON_EVENT_PRESSED)
            {
                BUZZER_Off();
                Screen_SetState("NORMAL");
                printf("Bouton -> retour NORMAL (annulation)\n");
                state = NORMAL;
            }
            else if ((now - timestamp) > 10000)
            {
                if (!alerte_envoyee)
                {
                    uint16_t lum = BSP_ADC_getValue(ADC_2);
                    ModeType_t mode = (lum < ADC_NIGHT_THRESHOLD) ? MODE_NUIT_BT : MODE_JOUR_BT;
                    AlerteType_t type = (cause_alerte == CAUSE_CHUTE) ? ALERTE_CHUTE : ALERTE_AUCUNE_REPONSE;
                    BT_SendAlert(type, bpm, mode);
                    alerte_envoyee = true;
                    printf(">>> ALERTE ENVOYEE via Bluetooth\n");
                }
                state = ALERTE_ENVOYEE;
            }
            break;

        case ALERTE_ENVOYEE:
            BUZZER_On();
            Screen_SetState("ALERTE ENVOYEE");

            if (evt == BUTTON_EVENT_PRESSED || (!immobilite_detectee()))
            {
                BUZZER_Off();
                Screen_SetState("NORMAL");
                printf("Retour NORMAL (fin alerte)\n");
                state = NORMAL;
            }
            break;
    }

}
