/*
 * button.h
 *
 *  Created on: Mar 25, 2026
 *      Author: raafa
 */

#ifndef BUTTON_H
#define BUTTON_H
#include "stm32g4xx_hal.h"
#include "stm32g4_gpio.h"
#include <stdbool.h>

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_PRESSED
} button_event_t;

void BUTTON_init(GPIO_TypeDef* gpio, uint32_t pin);
button_event_t BUTTON_state_machine(void);

#endif
