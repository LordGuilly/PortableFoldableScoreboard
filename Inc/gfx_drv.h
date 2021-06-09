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

#ifndef INC_GFX_DRV_H_
#define INC_GFX_DRV_H_

#include "lvgl.h"

#define NUMBER_OF_COLUMNS (LV_HOR_RES_MAX)
#define NUMBER_OF_ROWS    (LV_VER_RES_MAX)

#define PIXEL_BPP		 	(24)
#define PIXEL_GUARD			(1)
#define NUMBER_OF_PIXELS	(NUMBER_OF_COLUMNS*NUMBER_OF_ROWS)
#define PWM_BURST_LENGTH 	(PIXEL_BPP+PIXEL_GUARD)
#define PWM_BURST_TOTAL 	(NUMBER_OF_PIXELS)

#define PULSE_WIDTH_BASE_0	(28)
#define PULSE_WIDTH_ADD_1	(32)

void GFX_DRV_Update_Display(lv_disp_buf_t *framebuffer);

#endif /* INC_GFX_DRV_H_ */
