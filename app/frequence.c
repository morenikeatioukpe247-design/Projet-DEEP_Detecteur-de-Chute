/**
 * @file    frequence.c
 * @author  raafa
 * @date    Mar 25, 2026
 * @brief   Implémentation du module de gestion du capteur AD8232.
 *
 * Calcule le BPM par détection de fronts montants du signal ECG.
 * Un battement est détecté quand le signal passe au-dessus du seuil
 * peak_threshold. L'intervalle entre deux battements permet de
 * calculer la fréquence cardiaque.
 */

#include <frequence.h>
#include "stm32g4_adc.h"
#include "stm32g4_gpio.h"
#include "AD8232/stm32g4_ad8232.h"

static uint8_t  last_bpm = 0;

static uint32_t last_beat_time = 0;

static uint16_t peak_threshold = 2500;

static uint16_t last_raw = 0;

void freq_Init(void)
{
    AD8232_init();
    last_beat_time = HAL_GetTick();
}

uint16_t freq_GetRaw(void)
{
    return AD8232_getRaw();
}

bool freq_ElectrodesConnectees(void)
{
    return AD8232_electrodesConnectees();
}

bool freq_IsValid(void)
{
    return (last_bpm >= 40 && last_bpm <= 180);
}

uint8_t freq_GetBPM(void)
{
    uint32_t current_time = HAL_GetTick();
    uint16_t raw = freq_GetRaw();

    if (raw > peak_threshold && last_raw <= peak_threshold)
    {
        uint32_t interval = current_time - last_beat_time;

        if (interval > 350 && interval < 1500)
        {
            last_bpm = (uint8_t)(60000UL / interval);
            last_beat_time = current_time;
        }
    }

    last_raw = raw;

    if (raw > peak_threshold + 200)
        peak_threshold = raw - 300;

    return last_bpm;
}
