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

#include "lvgl.h"
#include "gfx_drv.h"
#include "gfx_mgr.h"
#include "gfx_mgr_cfg.h"

#define CLOCK_DEFAULT_COLOUR_INDEX (5)
#define CLOCK_FINAL_COLOUR_INDEX (4)
#define CLOCK_INITIAL_VALUE_MINUTES (6)
#define CLOCK_FINAL_COLOUR_THRESHOLD (15)

#define BANNER_INITIAL_COLOUR_INDEX (2)
#define GOAL_BANNER_OFFSET	(5)
#define FONT_WIDTH_PIXELS (8)

static const char *countdown_banner = "9  8  7  6  5  4  3  2  1";
#define COUNTDOWN_BANNER_ANIMATION_TIME (10000)
#define COUNTDOWN_BANNER_X_START_POS (GOAL_BANNER_OFFSET+LV_HOR_RES_MAX)
#define COUNTDOWN_BANNER_X_END_POS	(-GOAL_BANNER_OFFSET - FONT_WIDTH_PIXELS*strlen(countdown_banner))

lv_style_t clock_style;
lv_style_t banner_style;

lv_obj_t * label_banner;
lv_obj_t * label_clock;

static int clock_value_seconds = (CLOCK_INITIAL_VALUE_MINUTES * 60);

static void gfx_mgr_CountdownCb(lv_anim_t *animation)
{
    char auxi[3] = { 0 };

    auxi[0] = '0' + CLOCK_INITIAL_VALUE_MINUTES;

    clock_style.text.color = colour_array[CLOCK_DEFAULT_COLOUR_INDEX];
    lv_label_set_style(label_clock, LV_LABEL_STYLE_MAIN, &clock_style);
    lv_label_set_text(label_clock, auxi);
    lv_obj_align(label_clock, NULL, LV_ALIGN_CENTER, 1, 0);
    setup_stage = false;
}

static void GFX_MGR_CountdownAnimation(lv_obj_t * label)
{
    lv_anim_t a = { 0 };

    setup_stage = true;

    lv_anim_set_exec_cb(&a, label_banner, gfx_mgr_set_x_cb); /*Set the animator function and variable to animate*/
    lv_anim_set_time(&a, COUNTDOWN_BANNER_ANIMATION_TIME, 0);
    lv_anim_set_values(&a, COUNTDOWN_BANNER_X_START_POS,
    COUNTDOWN_BANNER_X_END_POS); /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/
    lv_anim_set_ready_cb(&a, gfx_mgr_CountdownCb); /*Set a callback to call then animation is ready. (Optional)*/

    lv_anim_create(&a);
}

static void gfx_mgr_ClockOutCb(lv_anim_t *animation)
{
    lv_anim_t a = { 0 };

    if (clock_value_seconds > 0) {
        gfx_mgr_change_label(animation->var, -1);

        lv_obj_set_y(animation->var, -10);
        lv_anim_set_exec_cb(&a, animation->var, gfx_mgr_set_y_cb); /*Set the animator function and variable to animate*/
        lv_anim_set_time(&a, 1000, 0);
        lv_anim_set_values(&a, -10, 1); /*Set start and end values. E.g. 0, 150*/
        lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/

        lv_anim_create(&a);
    } else {
        /* restart the countdown, reset the clock */
        clock_value_seconds = (CLOCK_INITIAL_VALUE_MINUTES * 60);
        clock_style.text.color = colour_array[CLOCK_DEFAULT_COLOUR_INDEX];
        GFX_MGR_CountdownAnimation(NULL);
    }
}

static void GFX_MGR_ClockAnimation(lv_obj_t * label)
{
    lv_anim_t a = { 0 };
    lv_anim_set_exec_cb(&a, label, gfx_mgr_set_y_cb); /*Set the animator function and variable to animate*/
    lv_anim_set_time(&a, 800, 0);
    lv_anim_set_values(&a, 1, 10); /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/
    lv_anim_set_ready_cb(&a, gfx_mgr_ClockOutCb); /*Set a callback to call then animation is ready. (Optional)*/

    lv_anim_create(&a);
}

void GFX_MGR_ClockTick(void)
{
    if (setup_stage == false) {
        clock_value_seconds--;

        if (clock_value_seconds == CLOCK_FINAL_COLOUR_THRESHOLD) {
            clock_style.text.color = colour_array[CLOCK_FINAL_COLOUR_INDEX];
            lv_label_set_style(label_clock, LV_LABEL_STYLE_MAIN, &clock_style);
        }

        if (clock_value_seconds % 60 == 0) {
            GFX_MGR_ClockAnimation(label_clock);
        }
    }
}

void GFX_MGR_SetupClock(void)
{
    lv_obj_t * scr = lv_disp_get_scr_act(NULL); /*Get the current screen*/

    setup_stage = true;
    clock_value_seconds = (CLOCK_INITIAL_VALUE_MINUTES * 60);

    lv_style_scr.body.main_color = LV_COLOR_BLACK;
    lv_style_scr.body.grad_color = LV_COLOR_BLACK;

    lv_style_scr.text.line_space = 1;

    lv_style_copy(&clock_style, &lv_style_scr);
    clock_style.text.color = colour_array[CLOCK_DEFAULT_COLOUR_INDEX];
    clock_style.body.main_color = LV_COLOR_BLACK;
    clock_style.body.grad_color = LV_COLOR_BLACK;

    lv_style_copy(&banner_style, &lv_style_scr);
    banner_style.body.main_color = LV_COLOR_BLACK;
    banner_style.body.grad_color = LV_COLOR_BLACK;
    banner_style.text.color = colour_array[BANNER_INITIAL_COLOUR_INDEX];

    /* create the label, and set colour to black, it will be changed at the setup */
    label_clock = lv_label_create(scr, NULL);
    clock_style.text.color = LV_COLOR_BLACK;
    lv_label_set_style(label_clock, LV_LABEL_STYLE_MAIN, &clock_style);

    label_banner = lv_label_create(scr, NULL);
    lv_obj_set_size(label_banner, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_label_set_long_mode(label_banner, LV_LABEL_LONG_EXPAND);
    lv_label_set_text(label_banner, countdown_banner);
    lv_label_set_style(label_banner, LV_LABEL_STYLE_MAIN, &banner_style);
    lv_obj_set_x(label_banner, COUNTDOWN_BANNER_X_START_POS);
    lv_obj_set_y(label_banner, 1);

    GFX_MGR_CountdownAnimation(NULL);
}
