/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010,2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file platform_api.h
 *
 * \brief API for the platform variables
 *
 */

#ifndef PLATFORMAPI_H
#define PLATFORMAPI_H

#include <stdint.h>
#ifdef HAVE_LOCATION
#include "location_api.h"
#endif
#include "error_types.h"
#ifdef HAVE_STRING_MGR
#include "string_mgr_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

error_type_t platvars_get_mac_addr(char **MacAddr);
error_type_t platvars_get_cmdlangstr(char **String);
error_type_t platvars_get_prtnamestr(char **String);
error_type_t platvars_get_usb_prodstr(char **String);
error_type_t platvars_get_usb_mfgstr( char **String);
error_type_t platvars_get_formater_ser_num(char **String);
error_type_t platvars_get_usb_langdesc(uint16_t *Value);
error_type_t platvars_get_usb_pid(uint16_t *Value);
error_type_t platvars_get_1284str(char **String);
error_type_t platvars_set_1284str(char *String);
error_type_t platvars_get_prod_serialnum(char **String);
#ifdef HAVE_STRING_MGR
error_type_t platvars_get_default_lang( string_mgr_language_t * language );
#endif
error_type_t platvars_get_usb_vid(uint16_t *Value);
error_type_t platvars_get_model_name(char **String);
error_type_t platvars_get_model_number(char **String);

/**
 *
 * \brief Returns IO device's timeout in seconds.  This value applies
 * to all IOs going through the connections manager.
 *
 */
error_type_t platvars_get_io_timeout(uint16_t *timeout);

/**
 *
 * \brief Sets the IO device's timeout in seconds.  This value applies
 * to all IOs going through the conection manager.
 *
 */
error_type_t platvars_set_io_timeout(uint16_t *timeout);

error_type_t platvars_get_fw_version(char **Version);
error_type_t platvars_get_devclass(char **Version);
error_type_t platvars_get_powerup_counter(uint32_t *counter);
error_type_t platvars_inc_powerup_counter(void);
error_type_t platvars_get_uptime_counter(uint32_t *hours, uint8_t *mins);

error_type_t platConfigMACAddr(char *MacAddr);
error_type_t platConfigPrtNameStr(char *String);
error_type_t platConfigUSBProdStr(char *String);
error_type_t platConfigFormatSerNum(char *String);
error_type_t platConfigUSBPID(uint16_t *Value);
error_type_t platConfigProdSerialNum(char *String);
error_type_t platConfigProdSKU(char *String);
error_type_t platConfigNVflags(uint8_t *value );

error_type_t plat_power_down(void);

#ifdef HAVE_LOCATION
error_type_t plat_get_current_country_code( e_CountryCode * code );
error_type_t plat_get_default_country_code( e_CountryCode * code );
error_type_t plat_set_current_country_code( e_CountryCode code );
error_type_t plat_set_default_country_code( e_CountryCode code );
#endif


/**
 *
 * \brief Request the ASIC id
 *
 * \param id The destination address for the ASIC ID value
 *
 */
void plat_get_asic_id( uint32_t * id );

#ifdef HAVE_RTC
#include "rtc_api.h"
error_type_t plat_get_install_date( date_time_t *time, char **version );
#else
error_type_t plat_get_install_date( char **version );
#endif

void platvars_init( void );
void platvars_shutdown( void );

void platform_init_1284_string(void);


/* 
   The following API's should only be called during manufacturing
   as a general rule - DO NOT USE in any other condition
   */
error_type_t platvars_set_formater_ser_num(char *String);
error_type_t platvars_set_mac_addr(char *MacAddr);
error_type_t platvars_set_prtnamestr(char *String);
error_type_t platvars_set_prod_serialnum(char *String);
error_type_t platvars_set_usb_prodstr(char *String);

#ifdef __cplusplus
}
#endif
#endif
