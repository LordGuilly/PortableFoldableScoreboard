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

#include "stdio.h"

#include "lvgl.h"
#include "gfx_drv.h"

#include "dma.h"
#include "tim.h"

uint16_t pwm_pulses_0[PWM_BURST_LENGTH] = { 0 };
uint16_t pwm_pulses_1[PWM_BURST_LENGTH] = { 0 };

int burst_counter;

volatile bool tx_in_progress;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
            HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3);
            HAL_TIM_PWM_Stop_IT(htim, TIM_CHANNEL_3);

            tx_in_progress = false;
        }
    }
}

void GFX_DRV_Update_BurstBuffer(uint16_t *buffer, lv_color_t *color_p)
{
    int index;
    /* TODO: can be improved! */
    uint32_t colour = color_p->ch.blue + (color_p->ch.red << 8) + (color_p->ch.green << 16);

    for (index = 0; index < PIXEL_BPP; index++) {
        buffer[index] = PULSE_WIDTH_BASE_0;
        if (colour & 0x800000) {
            buffer[index] += PULSE_WIDTH_ADD_1;
        }

        colour = colour << 1;
    }

}

static lv_color_t GFX_DRV_getPixelForBurst(lv_disp_buf_t *framebuffer, int index)
{
    lv_color_t retval = { 0 };
    lv_color_t *buffer = (lv_color_t *) framebuffer->buf1;
    int row = index / (NUMBER_OF_COLUMNS);
    int column = index % (NUMBER_OF_COLUMNS);

    if ((row % 2) == 0) {
        /* even row, keep it straight */
        retval = buffer[index];
    } else {
        retval = buffer[((row + 1) * NUMBER_OF_COLUMNS) - 1 - column];
    }
    return retval;
}

void GFX_DRV_Update_Display(lv_disp_buf_t *framebuffer)
{
    int rc;

    lv_color_t testp;

    tx_in_progress = false;
    burst_counter = 0;
    do {

        testp = GFX_DRV_getPixelForBurst(framebuffer, burst_counter);
        GFX_DRV_Update_BurstBuffer(pwm_pulses_0, &testp);

        while (tx_in_progress)
            ;
        burst_counter++;

        tx_in_progress = true;
        rc = HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t*) pwm_pulses_0, PWM_BURST_LENGTH);
        if (rc != HAL_OK) {
            printf("HAL_TIM_PWM_Start_DMA FAILED [%d]\r\n", rc);
        }

        testp = GFX_DRV_getPixelForBurst(framebuffer, burst_counter);
        GFX_DRV_Update_BurstBuffer(pwm_pulses_1, &testp);

        while (tx_in_progress)
            ;
        burst_counter++;

        tx_in_progress = true;
        rc = HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t*) pwm_pulses_1, PWM_BURST_LENGTH);
        if (rc != HAL_OK) {
            printf("HAL_TIM_PWM_Start_DMA FAILED [%d]\r\n", rc);
        }

    } while (burst_counter <= PWM_BURST_TOTAL);
}

