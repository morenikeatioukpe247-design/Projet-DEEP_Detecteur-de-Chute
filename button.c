/*
 * button.c
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */
#include "button.h"
#include "config.h"
#include "stm32g4_systick.h"
#include <stdio.h>
#include "stm32g4_gpio.h"

static void process_ms(void);

static volatile bool flag_10ms = false;
static bool initialized = false;
static GPIO_TypeDef* button_gpio;
static uint32_t button_pin;


bool read_button(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

void BUTTON_init(GPIO_TypeDef* gpio, uint32_t pin)
{
    button_gpio = gpio;
    button_pin = pin;

    // Configuration du GPIO en entrée + pull-up
    BSP_GPIO_pin_config(gpio, pin, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_NO_AF);

    // Appel de process_ms() toutes les 1 ms
    BSP_systick_add_callback_function(&process_ms);

    initialized = true;
}

static void process_ms(void)
{
    static uint32_t t50ms = 0;

    t50ms = (t50ms + 1) % 50;
    if (t50ms == 0)
        flag_10ms = true;
}

/**
 * Machine à états simplifiée :
 * - Un seul événement : BUTTON_EVENT_PRESSED
 * - Anti-rebond assuré par un cadencement à 10 ms
 */
button_event_t BUTTON_state_machine(void)
{
    typedef enum {
        INIT = 0,
        WAIT_PRESS,
        WAIT_RELEASE
    } state_e;

    static state_e state = INIT;
    button_event_t ret = BUTTON_EVENT_NONE;

    if (!initialized)
        return BUTTON_EVENT_NONE;

    if (flag_10ms) {
        flag_10ms = false;

        // Bouton câblé entre PB0 et GND → appui = 0
        bool current_button = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET);

        switch (state)
        {
            case INIT:
                state = WAIT_PRESS;
                break;

            case WAIT_PRESS:
                if (current_button) {
                    ret = BUTTON_EVENT_PRESSED;
                    printf("Bouton presse\n");
                    state = WAIT_RELEASE;
                }
                break;

            case WAIT_RELEASE:
                if (!current_button) {
                    printf("Bouton relache\n");

                    state = WAIT_PRESS;
                }
                break;

            default:
                state = INIT;
                break;
        }
    }

    return ret;
}
