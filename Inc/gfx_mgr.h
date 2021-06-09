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

#ifndef INC_GFX_MGR_H_
#define INC_GFX_MGR_H_

#include "lvgl.h"

void GFX_MGR_Init(void);

void GFX_MGR_task_handler(void);

void GFX_MGR_SetupClock(void);
void GFX_MGR_SetupScoreboard(void);
void GFX_MGR_SetupWelcome(void);
void GFX_MGR_ScoreAway(void);
void GFX_MGR_ScoreHome(void);
void GFX_MGR_ReduceScoreHome(void);
void GFX_MGR_ReduceScoreAway(void);
void GFX_MGR_ChangeColourAway(void);
void GFX_MGR_ChangeColourHome(void);
void GFX_MGR_FixHome(void);
void GFX_MGR_FixAway(void);
void GFX_MGR_ClockTick(void);

#endif /* INC_GFX_MGR_H_ */
