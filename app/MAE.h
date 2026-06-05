/*
 * MAE.h
 *
 *  Created on: Mar 27, 2026
 *      Author: raafa
 */

#ifndef MAE_H_
#define MAE_H_

#define ADC_NIGHT_THRESHOLD   800 //CREPUSCULE THRESHOLD[500 - 1000]
// Seuil nuit
/*Typiquement, pour une photorésistance en ADC 12 bits (0-4095) :
	Plein jour : 3000;4000
	Crépuscule : 500;1000
	Nuit : 0;500
On peut choisir un seuil ADC_NIGHT_THRESHOLD = 800 pour déclencher l'état OFF.*/

typedef enum
{
    NORMAL = 0,
    SUSPICION_CHUTE,
    SUSPICION_POULS,
    ATTENTE_CONFIRMATION,
    ALERTE_ENVOYEE
} system_state_e;

typedef enum
{
    MODE_JOUR,
    MODE_NUIT
} system_mode_e;

typedef enum
{
    CAUSE_CHUTE,
    CAUSE_POULS
} alerte_cause_e;


extern system_mode_e current_mode;
void MPU_Init();

void system_state_machine(void);
#endif /* MAE_H_ */









