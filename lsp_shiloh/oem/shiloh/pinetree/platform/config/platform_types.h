/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file platform_types.h
 *
 * \brief Types used for platform customization.  These are types shared between the
 * platform_api.c code and the platform customization code in the config directory.
 * These types should not be exported to or used by the main system.
 *
 */

#ifndef __PLATFORM_TYPES_H_
#define __PLATFORM_TYPES_H_

#include "ATypes.h"
#include "location_api.h"
#include "string_mgr_api.h"
#ifdef HAVE_NVRAM
#include "nvramTable.h"
#include "nvram_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct country_code_to_string_map_s
{
    string_mgr_code_t string_code;
    e_CountryCode code;
} country_code_to_string_map_t;

typedef struct _CONFIG_ENTRY
{
    error_type_t (*pStorageLoc)(void *);
    uint16_t offset;
    uint16_t len;
}CONFIG_ENTRY;

/**
 *
 * \brief A structure used to track how long the platform has been "up".
 *
 */
typedef struct uptime_s
{
    uint32_t    hours;
    uint8_t     mins;
} uptime_t;

/**
 *
 * \brief A structure containing the variables to write to non-volatile rom
 *
 * \warning Update the Version number whenever this structure changes
 *
 */

#define VERSION_STRLEN 30

#define PLAT_VARS_VERSION 6
typedef struct platform_vars_s
{
    uint32_t      powerup_counter;
    uptime_t      uptime_counter;
    uint16_t      IOTimeOut;
    e_CountryCode default_country_code;
    e_CountryCode current_country_code;
    #ifdef HAVE_RTC
    date_time_t     install_date;
    #endif
    char install_version[VERSION_STRLEN];
} platform_vars_t;


#define LEN_1284_STRING  255
typedef struct
{
    char prod1284String[LEN_1284_STRING] ALIGN16;
    char ProductSerNum[NVLEN_PROD_SER_NUM];
    string_mgr_language_t DefaultLang;
    uint16_t USBVendorID;
    uint16_t USBProductID;
    uint16_t USBLangDesc;
    char FormatterSerNum[NVLEN_FORMATTER_SER_NUM];
    char USBMfgStr[NVLEN_USB_MFGSTR];
    char USBProdStr[NVLEN_USB_PRODSTR];
    char PrinterNameStr[NVLEN_PRINTER_NAMESTR];
    char CmdLang[NVLEN_CMD_LANG];
    char PrinterMACAddr[NVLEN_MACADDR];
    char Version[VERSION_STRLEN];
    char DevClass[NVLEN_DEV_CLASS];
} CONFIG_TABLE_VARS;

extern platform_vars_t plat_vars;
extern CONFIG_TABLE_VARS plat_config_vars;
#ifdef HAVE_NVRAM
extern nvram_handle_t *plat_nvm_handle;
#endif

#ifdef __cplusplus
}
#endif

#endif //__PLATFORM_TYPES_H_
