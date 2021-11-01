/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file platform_private.h
 *
 * \brief Private API between platform config layer and main platform_api.c
 * implementation.  This API should only be used internally in the platform
 * implementation.
 *
 */

#ifndef _PLATFORM_PRIVATE_H
#define _PLATFORM_PRIVATE_H

#include <stdint.h>

#include "error_types.h"
#include "platform.h"
#include "nvram_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************** Implemented in platform_api.c *******************/
error_type_t platvars_set_prod_serialnum(char *String);
error_type_t platvars_set_usb_vid(uint16_t *Value);
error_type_t platvars_set_usb_langdesc(uint16_t *Value);
error_type_t platvars_set_usb_pid(uint16_t *Value);
error_type_t platvars_set_usb_prodstr(char *String);
error_type_t platvars_set_usb_mfgstr(char *String);

/* We need an interim "set" function to go from reading a config table
 * to writing it into RAM in order to handle big and little endian systems */
error_type_t platvars_config_table_set_usb_vid(uint16_t *value);
error_type_t platvars_config_table_set_usb_pid(uint16_t *value);
error_type_t platvars_config_table_set_usb_langdesc(uint16_t *value);

error_type_t platvars_set_formater_ser_num(char *String);
error_type_t platvars_set_mac_addr(char *MacAddr);
error_type_t platvars_set_cmdlangstr(char *String);
error_type_t platvars_set_prtnamestr(char *String);
error_type_t platvars_set_default_lang(string_mgr_language_t * language);

/**
 *
 * \brief Check to see if the personality stored in system NVRAM should be
 * overridden in favor of the defaults defined for the system in platform.h.
 * This is primarily used for testing generic reconfigurable early development
 * boards where it may be useful to configure the board to take on the personality
 * of the downloaded code so that many "product" configurations can be tested
 * without having to re-burn the NVRAM between each test cycle.
 *
 * The routine checks to see if the USBProdStr string from NVRAM matches the
 * string "MarvellReference".  If so then all platform personality info such
 * as usb indentifiers reverts to the defaults defined in product.h
 *
 */
void plat_check_default_personality_override();

/**************** Implemented in platform.c *******************/
error_type_t plat_init_config_table_vars(void);
error_type_t platform_nvram_config_init(void);
error_type_t plat_vars_init_callback(INIT_TYPE InitType, uint16_t Version, void *InitLocation, uint32_t IntLocationSize, void *unused);


uint32_t plat_get_country_map_table(country_code_to_string_map_t** country_code_to_string_map);


#ifdef __cplusplus
}
#endif

#endif //_PLATFORM_PRIVATE_H
