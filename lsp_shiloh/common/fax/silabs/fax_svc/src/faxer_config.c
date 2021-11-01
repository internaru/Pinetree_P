/* ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** This config lib can use the file system or nvram.  It may make sense to convert 
* to use the nvram lib in the future since that supports NAND and EEPROM. 
*/ 

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "memAPI.h"
#include "dprintf.h"
#include "faxer_api.h"
#include "nvram_api.h"

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

#define FAXER_CONF_VERSION_ID 0xFA010009

/* NOTE: be sure to increase this if using nvm and defaults or structure are modified */
#define FAXER_NVM_VER_ID 13 

void (*_init_storage)(void);
int (*_read_from_storage)(FAXER_SETTINGS*settings);
int (*_write_to_storage)(FAXER_SETTINGS*settings);

/** Initialize the settings.
 */
uint32_t _faxer_config_init_settings(FAXER_SETTINGS *pSettings, uint16_t country_id)
{
    return 0;
}

/*
 * Getter to obtain one faxer setting
 *
 * \param var Setting to read
 * \param buf Storage for the data
 *
 */
uint32_t faxer_config_get_setting(void *handle, FAXER_SETTING var, void *buf, int buf_size)
{
    int           ret        = 0;
    return ret;
}

/*
 *  Setter for changing faxer setting.
 *
 *  \param var  setting to be changed
 *  \param buf  buffer for string data
 *              or data for non-string data
 *
 *  \return 0 Success
 *          others failure
 *
 */
uint32_t faxer_config_set_setting(void *handle, FAXER_SETTING var, void *buf)
{
    int           ret        = 0;
    return ret;
}


uint32_t faxer_config_restore(void *handle)
{
        return 1;
}
