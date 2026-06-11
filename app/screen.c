/**
 * @file    screen.c
 * @author  raafa
 * @date    Apr 29, 2026
 * @brief   Implementation du module d'affichage sur ecran TFT ILI9341.
 *
 * Gere l'affichage de l'etat de la machine a etats, du BPM
 * et du statut des electrodes. Le redessin n'est effectue que
 * si le flag needs_update est leve pour eviter les raffraichissements
 * inutiles.
 */

#include "screen.h"
#include <TFT_ili9341/stm32g4_ili9341.h>
#include "tft_ili9341/stm32g4_fonts.h"
#include "tft_ili9341/stm32g4_xpt2046.h"
#include <stdio.h>
#include <string.h>

/** @brief Etat courant du systeme a afficher */
static const char* current_state = "NORMAL";

/** @brief Valeur BPM courante a afficher */
static uint8_t current_bpm = 0;

/** @brief Statut de connexion des electrodes */
static bool electrode_connected = false;

/** @brief Flag indiquant si un redessin est necessaire */
static bool needs_update = true;

/**
 * @brief Initialise l'ecran TFT ILI9341 et efface le fond en noir.
 */
void Screen_Init(void)
{
    ILI9341_Init();
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    printf("Screen init OK\n");
}

/**
 * @brief  Definit l'etat du systeme a afficher.
 * @note   Leve le flag needs_update pour forcer le redessin.
 */
void Screen_SetState(const char* state)
{
    current_state = state;
    needs_update = true;
}

/**
 * @brief  Definit la valeur BPM a afficher.
 * @note   Leve le flag needs_update pour forcer le redessin.
 */
void Screen_SetBPM(uint8_t bpm)
{
    current_bpm = bpm;
    needs_update = true;
}

/**
 * @brief  Definit le statut de connexion des electrodes.
 */
void Screen_ShowElectrodeStatus(bool connected)
{
    electrode_connected = connected;
}

/**
 * @brief  Met a jour l'affichage si le flag needs_update est leve.
 * @note   Affiche l'etat systeme en couleur selon la criticite :
 *         vert = NORMAL, jaune = CHUTE SUSPECTEE / ATTENTE,
 *         orange = POULS ANORMAL, rouge = ALERTE ENVOYEE.
 */
void Screen_Update(void)
{
    if (!needs_update)
        return;

    needs_update = false;


    ILI9341_Puts(30, 20, "SYSTEME DE CHUTE", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    ILI9341_Puts(20, 70, "Etat :", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    uint16_t state_color = ILI9341_COLOR_GREEN;
    if (strcmp(current_state, "CHUTE SUSPECTEE") == 0)
        state_color = ILI9341_COLOR_YELLOW;
    else if (strcmp(current_state, "ATTENTE CONFIRM.") == 0)
        state_color = ILI9341_COLOR_YELLOW;
    else if (strcmp(current_state, "POULS ANORMAL") == 0)
        state_color = ILI9341_COLOR_ORANGE;
    else if (strcmp(current_state, "ALERTE ENVOYEE") == 0)
        state_color = ILI9341_COLOR_RED;

    ILI9341_Puts(120, 70, (char*)current_state, &Font_11x18, state_color, ILI9341_COLOR_BLACK);

    ILI9341_Puts(20, 120, "Pouls :", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    char buf[20];
    if (electrode_connected && current_bpm > 0)
    {
        sprintf(buf, "%d BPM", current_bpm);
        ILI9341_Puts(130, 120, buf, &Font_11x18, ILI9341_COLOR_MAGENTA, ILI9341_COLOR_BLACK);
    }
    else
    {
        ILI9341_Puts(130, 120, "-----", &Font_11x18, ILI9341_COLOR_GRAY, ILI9341_COLOR_BLACK);
    }

    if (!electrode_connected)
    {
        ILI9341_Puts(20, 200, "ELECTRODES DECONNECTEES !", &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
    }
}
