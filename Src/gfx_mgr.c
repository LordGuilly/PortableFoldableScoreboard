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

#define HOME_INITIAL_COLOUR_INDEX (0)
#define AWAY_INITIAL_COLOUR_INDEX (1)
#define GOAL_INITIAL_COLOUR_INDEX (2)
#define GOAL_DEFAULT_COLOUR_INDEX (0)

#define GOAL_BANNER_OFFSET	(5)
#define FONT_WIDTH_PIXELS (8)

#define COLOR_ARRAY_LENGTH	(sizeof(colour_array)/sizeof(lv_color_t))

static const char *goal_banner = "GOAL";
#define GOAL_BANNER_ANIMATION_TIME (3000)
#define GOAL_BANNER_X_START_POS (GOAL_BANNER_OFFSET+LV_HOR_RES_MAX)
#define GOAL_BANNER_X_END_POS	(-GOAL_BANNER_OFFSET - FONT_WIDTH_PIXELS*strlen(goal_banner))

/*A static or global variable to store the buffers*/
static lv_disp_buf_t disp_buf = { 0 };

static lv_disp_drv_t my_disp_drv;
static lv_disp_t * disp;

lv_style_t home_style;
lv_style_t away_style;
lv_style_t goal_style;

lv_obj_t * label_home_score;
lv_obj_t * label_away_score;
lv_obj_t * label_goal;

/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];

bool setup_stage;

lv_color_t colour_array[] = {
LV_COLOR_WHITE,
LV_COLOR_BLUE,
LV_COLOR_YELLOW,
LV_COLOR_ORANGE,
//LV_COLOR_GRAY,
        LV_COLOR_RED,
        LV_COLOR_GREEN,
        LV_COLOR_CYAN,
        LV_COLOR_MAGENTA, };

static int away_colour_index = AWAY_INITIAL_COLOUR_INDEX;
static int home_colour_index = HOME_INITIAL_COLOUR_INDEX;
static int goal_colour_index = GOAL_INITIAL_COLOUR_INDEX;

volatile static bool display_needs_refresh;

static void gfx_mgr_disp_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /* set the flag, so the task can refresh */
    display_needs_refresh = true;
    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void gfx_mgr_disp_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t * area)
{
    /* this rounder callback is used to force a full screen refresh */
    area->x1 = 0;
    area->x2 = LV_HOR_RES - 1;

    area->y1 = 0;
    area->y2 = LV_VER_RES - 1;
}

void GFX_MGR_Init(void)
{
    display_needs_refresh = false;

    lv_init();
    /*Initialize `disp_buf` with the buffer(s) */
    lv_disp_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * 10);

    lv_disp_drv_init(&my_disp_drv); /*Basic initialization*/
    my_disp_drv.buffer = &disp_buf; /*Set an initialized buffer*/
    my_disp_drv.flush_cb = &gfx_mgr_disp_flush_cb; /*Set a flush callback to draw to the display*/
    my_disp_drv.rounder_cb = &gfx_mgr_disp_rounder_cb;

    disp = lv_disp_drv_register(&my_disp_drv); /*Register the driver and save the created display objects*/

    /* force a display clean */
    GFX_DRV_Update_Display(lv_disp_get_buf(disp));
}

void GFX_MGR_task_handler(void)
{
    /* only refresh when needed */
    if (display_needs_refresh == true) {
        GFX_DRV_Update_Display(lv_disp_get_buf(disp));
        display_needs_refresh = false;
    }
}

void gfx_mgr_change_label(lv_obj_t * label, int delta)
{
    char auxi[2] = { 0 };
    char *value = lv_label_get_text(label);

    auxi[0] = *value;

    auxi[0] += delta;

    if ((auxi[0] > '9') || (auxi[0] < '0')) {
        auxi[0] = '0';
    }

    lv_label_set_text(label, auxi);
}

void gfx_mgr_set_y_cb(void *object, lv_anim_value_t value)
{
    lv_obj_set_y(object, value);
}

void gfx_mgr_set_x_cb(void *object, lv_anim_value_t value)
{
    lv_obj_set_x(object, value);
}

static void gfx_mgr_ScoreOutCb(lv_anim_t *animation)
{
    lv_anim_t a = { 0 };

    gfx_mgr_change_label(animation->var, 1);

    lv_obj_set_y(animation->var, -10);
    lv_anim_set_exec_cb(&a, animation->var, gfx_mgr_set_y_cb); /*Set the animator function and variable to animate*/
    lv_anim_set_time(&a, 1000, 0);
    lv_anim_set_values(&a, -10, 1); /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/

    lv_anim_create(&a);

}

static void gfx_mgr_CheckGoalColour(void)
{
    goal_colour_index = GOAL_DEFAULT_COLOUR_INDEX;
    while ((goal_colour_index == away_colour_index) || (goal_colour_index == home_colour_index)) {
        goal_colour_index++;

        if (goal_colour_index >= COLOR_ARRAY_LENGTH) {
            goal_colour_index = 0;
        }
    }

    goal_style.text.color = colour_array[goal_colour_index];
    lv_label_set_style(label_goal, LV_LABEL_STYLE_MAIN, &goal_style);
}

static void GFX_MGR_ScoreAnimation(lv_obj_t * label)
{
    lv_anim_t a = { 0 };
    lv_anim_set_exec_cb(&a, label, gfx_mgr_set_y_cb); /*Set the animator function and variable to animate*/
    lv_anim_set_time(&a, 800, 0);
    lv_anim_set_values(&a, 1, 10); /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/
    lv_anim_set_ready_cb(&a, gfx_mgr_ScoreOutCb); /*Set a callback to call then animation is ready. (Optional)*/

    lv_anim_create(&a);
}

static void gfx_mgr_GoalOutCb(lv_anim_t *animation)
{
    GFX_MGR_ScoreAnimation(lv_anim_get_user_data(animation));
}

static void GFX_MGR_GoalAnimation(lv_obj_t * label)
{
    lv_anim_t a = { 0 };
    lv_anim_set_exec_cb(&a, label_goal, gfx_mgr_set_x_cb); /*Set the animator function and variable to animate*/
    lv_anim_set_time(&a, GOAL_BANNER_ANIMATION_TIME, 0);
    lv_anim_set_values(&a, GOAL_BANNER_X_START_POS, GOAL_BANNER_X_END_POS); /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_path_cb(&a, lv_anim_path_linear); /*Set path from `lv_anim_path_...` functions or a custom one.*/
    lv_anim_set_ready_cb(&a, gfx_mgr_GoalOutCb); /*Set a callback to call then animation is ready. (Optional)*/
    lv_anim_set_user_data(&a, label);

    lv_anim_create(&a);
}

void GFX_MGR_ScoreAway(void)
{
    GFX_MGR_GoalAnimation(label_away_score);
    setup_stage = false;
}

void GFX_MGR_ScoreHome(void)
{
    GFX_MGR_GoalAnimation(label_home_score);
    setup_stage = false;
}

void GFX_MGR_FixHome(void)
{
    if (setup_stage == true) {
        GFX_MGR_ChangeColourHome();
    } else {
        GFX_MGR_ReduceScoreHome();
    }

}

void GFX_MGR_FixAway(void)
{
    if (setup_stage == true) {
        GFX_MGR_ChangeColourAway();
    } else {
        GFX_MGR_ReduceScoreAway();
    }
}

void GFX_MGR_ReduceScoreHome(void)
{
    gfx_mgr_change_label(label_home_score, -1);
}

void GFX_MGR_ReduceScoreAway(void)
{
    gfx_mgr_change_label(label_away_score, -1);
}

void GFX_MGR_ChangeColourAway(void)
{

    away_colour_index++;

    if (away_colour_index >= COLOR_ARRAY_LENGTH) {
        away_colour_index = 0;
    }

    away_style.text.color = colour_array[away_colour_index];
    lv_label_set_style(label_away_score, LV_LABEL_STYLE_MAIN, &away_style);

    gfx_mgr_CheckGoalColour();

}

void GFX_MGR_ChangeColourHome(void)
{
    home_colour_index++;

    if (home_colour_index >= COLOR_ARRAY_LENGTH) {
        home_colour_index = 0;
    }

    home_style.text.color = colour_array[home_colour_index];
    lv_label_set_style(label_home_score, LV_LABEL_STYLE_MAIN, &home_style);

    gfx_mgr_CheckGoalColour();
}

void GFX_MGR_SetupScoreboard(void)
{
    lv_obj_t * scr = lv_disp_get_scr_act(NULL); /*Get the current screen*/
    setup_stage = true;

    lv_style_scr.body.main_color = LV_COLOR_BLACK;
    lv_style_scr.body.grad_color = LV_COLOR_BLACK;

    lv_style_scr.text.color = colour_array[goal_colour_index];
    lv_style_scr.text.line_space = 1;

    lv_style_copy(&home_style, &lv_style_scr);
    home_style.text.color = colour_array[home_colour_index];
    home_style.body.main_color = LV_COLOR_BLACK;
    home_style.body.grad_color = LV_COLOR_BLACK;

    lv_style_copy(&away_style, &lv_style_scr);
    away_style.body.main_color = LV_COLOR_BLACK;
    away_style.body.grad_color = LV_COLOR_BLACK;
    away_style.text.color = colour_array[away_colour_index];

    lv_style_copy(&goal_style, &lv_style_scr);
    goal_style.body.main_color = LV_COLOR_BLACK;
    goal_style.body.grad_color = LV_COLOR_BLACK;
    goal_style.text.color = colour_array[goal_colour_index];

    label_home_score = lv_label_create(scr, NULL);
    lv_label_set_style(label_home_score, LV_LABEL_STYLE_MAIN, &home_style);
    lv_label_set_text(label_home_score, "0");
    lv_obj_align(label_home_score, NULL, LV_ALIGN_IN_LEFT_MID, 0, 1);

    label_away_score = lv_label_create(scr, NULL);
    lv_label_set_text(label_away_score, "0");
    lv_label_set_style(label_away_score, LV_LABEL_STYLE_MAIN, &away_style);
    lv_obj_align(label_away_score, NULL, LV_ALIGN_IN_RIGHT_MID, 1, 1);

    label_goal = lv_label_create(scr, NULL);
    lv_obj_set_size(label_goal, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_label_set_long_mode(label_goal, LV_LABEL_LONG_EXPAND);
    lv_label_set_text(label_goal, goal_banner);
    lv_label_set_style(label_goal, LV_LABEL_STYLE_MAIN, &goal_style);
    lv_obj_set_x(label_goal, GOAL_BANNER_X_START_POS);
    lv_obj_set_y(label_goal, 1);

}

void GFX_MGR_SetupWelcome(void)
{
    lv_obj_t * scr = lv_disp_get_scr_act(NULL); /*Get the current screen*/

    label_goal = lv_label_create(scr, NULL);
    lv_obj_set_size(label_goal, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_label_set_long_mode(label_goal, LV_LABEL_LONG_EXPAND);
    lv_label_set_text(label_goal, "GOAL  GOAL  GOAL  GOAL  GOAL");
}
