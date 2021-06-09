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

#include <string.h>
#include <stdbool.h>
#include "sysconfig.h"
#include "main.h"

#define DEFAULT_LANGUAGE "ENGLISH"
#define DEFAULT_COPYRIGHT "Copyright guilly@gmail.com"

typedef struct {
    uint16_t hw_id_0_low;
    char language[14];
    uint16_t hw_id_0_high;
    uint32_t mode;
    uint16_t hw_id_1_low;
    char copyright[35];
    uint16_t hw_id_1_high;
    char build_version[41];
    uint16_t hw_id_2_low;
    char build_status[14];
    uint16_t hw_id_2_high;
    char serial_number[14];
} flash_config_t;

/* this variable should be in flash */
flash_config_t __attribute__((section(".myflash"))) dev_config = { .language =
DEFAULT_LANGUAGE, .copyright = DEFAULT_COPYRIGHT, .hw_id_0_low =
UNINITIALISED_FLASH_VALUE, .hw_id_0_high = UNINITIALISED_FLASH_VALUE, .hw_id_1_low = UNINITIALISED_FLASH_VALUE, .hw_id_1_high =
UNINITIALISED_FLASH_VALUE, .hw_id_2_low =
UNINITIALISED_FLASH_VALUE, .hw_id_2_high =
UNINITIALISED_FLASH_VALUE, .mode = 0, .build_version = GIT_REVISION, .build_status = GIT_STATUS, };

/*
 * function that checks the stored hw id value in flash matches the current CPU
 */
SysDevice_StatusTypeDef check_hwid_in_flash(void)
{
    SysDevice_StatusTypeDef retval = SYSDEVICE_ERROR;
    uint32_t *current_device_id = (uint32_t *) UID_BASE;

    if ((dev_config.hw_id_0_low == (current_device_id[0] & UNINITIALISED_FLASH_VALUE))
            && (dev_config.hw_id_0_high == (current_device_id[0] >> 16))
            && (dev_config.hw_id_1_low == (current_device_id[1] & UNINITIALISED_FLASH_VALUE))
            && (dev_config.hw_id_1_high == (current_device_id[1] >> 16))
            && (dev_config.hw_id_2_low == (current_device_id[2] & UNINITIALISED_FLASH_VALUE))
            && (dev_config.hw_id_2_high == (current_device_id[2] >> 16))) {
        retval = SYSDEVICE_OK;
    }

    return (retval);
}

/*
 * function that checks the stored hw id value in flash has not been initialised
 */
bool hwid_in_flash_is_empty(void)
{
    bool retval = false;

    if ((dev_config.hw_id_0_low == UNINITIALISED_FLASH_VALUE) && (dev_config.hw_id_0_high == UNINITIALISED_FLASH_VALUE)
            && (dev_config.hw_id_1_low == UNINITIALISED_FLASH_VALUE) && (dev_config.hw_id_1_high == UNINITIALISED_FLASH_VALUE)
            && (dev_config.hw_id_2_low == UNINITIALISED_FLASH_VALUE) && (dev_config.hw_id_2_high == UNINITIALISED_FLASH_VALUE)) {
        retval = true;
    }
    return (retval);
}

/*
 * function that checks some default values are correct, to avoid
 * an empty config to unlock the device.
 */
bool hardcoded_flash_has_errors(void)
{
    bool retval = true;

    if ((memcmp(dev_config.build_version, GIT_REVISION, sizeof(GIT_REVISION)) == 0)
            && (memcmp(dev_config.copyright, DEFAULT_COPYRIGHT, sizeof(DEFAULT_COPYRIGHT)) == 0)
            && (memcmp(dev_config.language, DEFAULT_LANGUAGE, sizeof(DEFAULT_LANGUAGE)) == 0)) {
        retval = false;
    }

    return (retval);
}

void secure_device(void)
{
    uint32_t *current_device_id = (uint32_t *) UID_BASE;

    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_0_low,
            (current_device_id[0] & UNINITIALISED_FLASH_VALUE));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_0_high, (current_device_id[0] >> 16));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_1_low,
            (current_device_id[1] & UNINITIALISED_FLASH_VALUE));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_1_high, (current_device_id[1] >> 16));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_2_low,
            (current_device_id[2] & UNINITIALISED_FLASH_VALUE));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t) & dev_config.hw_id_2_high, (current_device_id[2] >> 16));
    HAL_FLASH_Lock();
}

SysDevice_StatusTypeDef check_hardware(void)
{
    SysDevice_StatusTypeDef retval = SYSDEVICE_ERROR;

    if (hardcoded_flash_has_errors() == true) {
        retval = SYSDEVICE_ERROR;
    } else if (hwid_in_flash_is_empty() == true) {
        //first time, store the value
        secure_device();
        retval = SYSDEVICE_OK;
    } else {
        retval = check_hwid_in_flash();
    }

    return (retval);
}

