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
#include "gfx_drv.h"
#include "gfx_mgr.h"
#include "key_mgr.h"
#include "sysconfig.h"

void USER_CODE_Init(void)
{
    if (HAL_GPIO_ReadPin(BOOTLOADER_GPIO_Port, BOOTLOADER_Pin) == GPIO_PIN_SET) {
        enter_bootloader_mode();
    }

    if (check_hardware() == SYSDEVICE_ERROR) {
        /* check failure means it's a forged unit */
        while (1) {
            /* dead sequence */
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
            HAL_Delay(2000);
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
            HAL_Delay(2000);
        }
    } else {
        /* start light sequence */
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_Delay(800);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_Delay(800);
    }

    GFX_MGR_Init();
    KEY_MGR_Init();

#if SCORE_SUPPORTED
    GFX_MGR_SetupScoreboard();
#endif

#if CLOCK_SUPPORTED
    GFX_MGR_SetupClock();
#endif

    HAL_GPIO_WritePin(TIMMING_GPIO_Port, TIMMING_Pin, GPIO_PIN_RESET);

}

void USER_CODE_Loop(void)
{
#if CLOCK_SUPPORTED
    tick_counter = HAL_GetTick();
#endif
    while (1) {
        HAL_Delay(LV_DISP_DEF_REFR_PERIOD);

#if SCORE_SUPPORTED
        if (KEY_MGR_is_any_key_pressed() == true) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        } else {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        }
        KEY_MGR_task_handler();
#endif

#if CLOCK_SUPPORTED
        if (HAL_GetTick() > (tick_counter+1000))
        {
            GFX_MGR_ClockTick();
            HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
            tick_counter += 1000;
        }
#endif

        lv_tick_inc(LV_DISP_DEF_REFR_PERIOD);
        lv_task_handler();

        GFX_MGR_task_handler();
    }
}
