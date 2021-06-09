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

#ifndef INC_GFX_DRV_CFG_H_
#define INC_GFX_DRV_CFG_H_

extern lv_color_t colour_array[];
extern bool setup_stage;

void gfx_mgr_change_label(lv_obj_t * label, int delta);

void gfx_mgr_set_y_cb(void *object, lv_anim_value_t value);
void gfx_mgr_set_x_cb(void *object, lv_anim_value_t value);

#endif /* INC_GFX_DRV_CFG_H_ */
