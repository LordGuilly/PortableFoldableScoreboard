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

#ifndef INC_KEY_MGR_H_
#define INC_KEY_MGR_H_

typedef void (*key_callback_t)(void);

void KEY_MGR_Init(void);
void KEY_MGR_task_handler(void);
bool KEY_MGR_is_any_key_pressed(void);

#endif /* INC_KEY_MGR_H_ */
