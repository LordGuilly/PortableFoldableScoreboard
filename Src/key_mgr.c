/*
 Copyright (C) 2019  Sebastian Viviani
 Email: guilly@gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "gpio.h"
#include "gfx_mgr.h"

#define DEBOUNCE_PERIOD	(10)
#define NUM_KEYS	(2)

static GPIO_PinState mode_fix;

static bool is_key_pressed;

typedef struct {
    GPIO_PinState state;
    GPIO_TypeDef *port;
    uint16_t pin;
    int debounce_counter;
    void (*action_set_fn)(void);
    void (*action_reset_fn)(void);
} key_action_t;

static void home_key_action(void)
{
    if (mode_fix == GPIO_PIN_SET) {
        GFX_MGR_FixHome();
    } else {
        GFX_MGR_ScoreHome();
    }
}

static void away_key_action(void)
{
    if (mode_fix == GPIO_PIN_SET) {
        GFX_MGR_FixAway();
    } else {
        GFX_MGR_ScoreAway();
    }
}

/* keys are active low */
key_action_t key_table[NUM_KEYS] = {
        { GPIO_PIN_RESET, ACTION_HOME_GPIO_Port, ACTION_HOME_Pin, 0, NULL, &home_key_action },
        { GPIO_PIN_RESET, ACTION_AWAY_GPIO_Port, ACTION_AWAY_Pin, 0, NULL, &away_key_action },
};

bool KEY_MGR_is_any_key_pressed(void)
{
    return is_key_pressed;
}

void KEY_MGR_Init(void)
{
    int index = 0;

    for (index = 0; index < NUM_KEYS; index++) {
        key_table[index].state = HAL_GPIO_ReadPin(key_table[index].port, key_table[index].pin);
        key_table[index].debounce_counter = 0;
    }

    is_key_pressed = false;
}

void KEY_MGR_task_handler(void)
{

    int index = 0;
    GPIO_PinState new_state;

    mode_fix = HAL_GPIO_ReadPin(MODE_FIX_GPIO_Port, MODE_FIX_Pin);

    for (index = 0; index < NUM_KEYS; index++) {
        new_state = HAL_GPIO_ReadPin(key_table[index].port, key_table[index].pin);

        if (new_state != key_table[index].state) {
            key_table[index].debounce_counter += 1;
        } else {
            key_table[index].debounce_counter = 0;
        }

        if (key_table[index].debounce_counter == DEBOUNCE_PERIOD) {
            key_table[index].state = new_state;
            key_table[index].debounce_counter = 0;

            if (new_state == GPIO_PIN_SET) {
                is_key_pressed = false;
                if (key_table[index].action_set_fn != NULL) {
                    key_table[index].action_set_fn();
                }
            }

            if (new_state == GPIO_PIN_RESET) {
                is_key_pressed = true;
                if (key_table[index].action_reset_fn != NULL) {
                    key_table[index].action_reset_fn();
                }
            }
        }
    }
}
