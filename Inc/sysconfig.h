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

#ifndef INC_SYSCONFIG_H_
#define INC_SYSCONFIG_H_

#include <stdint.h>

#define UNINITIALISED_FLASH_VALUE (0xffffu)

extern const uint32_t hw_id[];

typedef enum {
    SYSDEVICE_OK = 0x00U,
    SYSDEVICE_ERROR = 0x01U,
} SysDevice_StatusTypeDef;

void secure_device(void);
SysDevice_StatusTypeDef check_hardware(void);

#endif /* INC_SYSCONFIG_H_ */
