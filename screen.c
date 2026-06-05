/*
 * screen.c
 *
 *  Created on: Apr 29, 2026
 *      Author: raafa
 */

#include "screen.h"
#include <TFT_ili9341/stm32g4_ili9341.h>
#include "tft_ili9341/stm32g4_fonts.h"
#include "tft_ili9341/stm32g4_xpt2046.h"
#include <stdio.h>
#include <string.h>

static const char* current_state = "NORMAL";
static uint8_t current_bpm = 0;
static bool electrode_connected = false;
static bool needs_update = true;

void Screen_Init(void)
{
    ILI9341_Init();
    ILI9341_Fill(ILI9341_COLOR_BLACK);     // Fond noir
    printf("Screen init OK\n");
}

void Screen_SetState(const char* state)
{
	current_state = state;
	needs_update = true;
}

void Screen_SetBPM(uint8_t bpm)
{
    current_bpm = bpm;
    needs_update = true;
}

void Screen_ShowElectrodeStatus(bool connected)
{
    electrode_connected = connected;
}

void Screen_Update(void)
{
    if (!needs_update)
        return;

    needs_update = false;

    ILI9341_Fill(ILI9341_COLOR_BLACK);

    // Titre
    ILI9341_Puts(30, 20, "SYSTEME DE CHUTE", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    // État du système
    ILI9341_Puts(20, 70, "Etat :", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    uint16_t state_color = ILI9341_COLOR_GREEN;
    if (strcmp(current_state, "CHUTE SUSPECTEE") == 0)
        state_color = ILI9341_COLOR_YELLOW;
    else if (strcmp(current_state, "ATTENTE CONFIRM.") == 0)
        state_color = ILI9341_COLOR_YELLOW;
    else if (strcmp(current_state, "ALERTE ENVOYEE") == 0)   // ← correspond exactement à MAE.c
        state_color = ILI9341_COLOR_RED;
    else if (strcmp(current_state, "POULS ANORMAL") == 0)
        state_color = ILI9341_COLOR_ORANGE;

    ILI9341_Puts(120, 70, (char*)current_state, &Font_11x18, state_color, ILI9341_COLOR_BLACK);

    // Pouls
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

    // Message électrodes
    if (!electrode_connected)
    {
        ILI9341_Puts(20, 200, "ELECTRODES DECONNECTEES !", &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
    }
}


