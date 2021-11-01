/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file platform.c
 *
 */
#include <stdint.h>
#include <string.h>
#include "platform.h"
#include "memAPI.h"
#include "checksum.h"
#include "dprintf.h"
#ifdef HAVE_NVRAM
#include "nvram_api.h"
#ifndef HAVE_NVRAM_FILE
#include "rommgrapi.h"
#endif
#endif
#include "nvramTable.h"
#include "logger.h"
#include "platform_api.h"
#include "platform_types.h"
#include "platform_private.h"
#include "fwVersion.h"
#include "lassert.h"
#ifndef __linux__
#include "gpio_api.h"
#endif
#include "location_api.h"
#include "string_mgr_api.h"
#include "data_access_api.h"
#ifdef LSPTODO
#include "agConnectMgr.h"
#endif
#include "error_types.h"
#include "ioutils.h"
#ifdef HAVE_PAPER
#include "paper_size_api.h"
#include "paper_types_api.h"
#endif
#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
#include "printvars_api.h"
#endif

#ifdef HAVE_DBG_PRINTF
    #define DBG (DBG_LOUD | DBG_TERMINAL)
#endif

/** \brief The ASIC ID register address */
#define REG_ASIC_ID ( (uint32_t *)0xF8000000 )

const CONFIG_ENTRY configTable[] =
{
    { (error_type_t (*)(void *))platvars_set_prod_serialnum, NVOFF_PROD_SER_NUM, NVLEN_PROD_SER_NUM },
    { (error_type_t (*)(void *))platvars_config_table_set_usb_vid, NVOFF_USB_VENDORID, NVLEN_USB_VENDORID },
    { (error_type_t (*)(void *))platvars_config_table_set_usb_pid, NVOFF_USB_PRODUCTID, NVLEN_USB_PRODUCTID },
    { (error_type_t (*)(void *))platvars_config_table_set_usb_langdesc, NVOFF_USB_LANGDESC, NVLEN_USB_LANGDESC },
    { (error_type_t (*)(void *))platvars_set_formater_ser_num, NVOFF_FORMATTER_SER_NUM, NVLEN_FORMATTER_SER_NUM},
    { (error_type_t (*)(void *))platvars_set_usb_mfgstr, NVOFF_USB_MFGSTR, NVLEN_USB_MFGSTR },
    { (error_type_t (*)(void *))platvars_set_usb_prodstr, NVOFF_USB_PRODSTR, NVLEN_USB_PRODSTR },
    { (error_type_t (*)(void *))platvars_set_prtnamestr, NVOFF_PRINTER_NAMESTR, NVLEN_PRINTER_NAMESTR },
    { (error_type_t (*)(void *))platvars_set_mac_addr, NVOFF_MACADDR, NVLEN_MACADDR },
    { (error_type_t (*)(void *))platvars_set_cmdlangstr, NVOFF_CMD_LANG, NVLEN_CMD_LANG},
    { (error_type_t (*)(void *))platvars_set_default_lang, NVOFF_DEFAULT_LANG, NVLEN_DEFAULT_LANG },
};

#define NUM_CONFIG_TABLE_ENTRIES (sizeof(configTable)/sizeof(CONFIG_ENTRY))


//#define LEN_1284_STRING  255

#ifdef HAVE_PRINT_SUPPORT

#ifdef HAVE_PAPER

typedef struct country_code_to_media_size_map_s
{
    e_CountryCode code;
    mediasize_t size;
} country_code_to_media_size_map_t;

/** \brief A Table mapping location to paper size. */
static country_code_to_media_size_map_t country_code_to_media_size_map[] =
{
    {ARGENTINA, MEDIASIZE_LETTER},
    {AUSTRALIA, MEDIASIZE_A4},
    {AUSTRIA, MEDIASIZE_A4},
    {BELGIUM, MEDIASIZE_A4},
    {BRAZIL, MEDIASIZE_A4},
    {BULGARIA, MEDIASIZE_A4},
    {CANADA, MEDIASIZE_LETTER},
    {CHILE, MEDIASIZE_LETTER},
    {CHINA, MEDIASIZE_A4},
    {CROATIA, MEDIASIZE_A4},
    {CZECH_REPUBLIC, MEDIASIZE_A4},
    {DENMARK, MEDIASIZE_A4},
    {FINLAND, MEDIASIZE_A4},
    {FRANCE, MEDIASIZE_A4},
    {GERMANY, MEDIASIZE_A4},
    {GREECE, MEDIASIZE_A4},
    {HONG_KONG, MEDIASIZE_A4},
    {HUNGARY, MEDIASIZE_A4},
    {INDIA, MEDIASIZE_A4},
    {INDONESIA, MEDIASIZE_A4},
    {IRELAND, MEDIASIZE_A4},
    {ISRAEL, MEDIASIZE_A4},
    {ITALY, MEDIASIZE_A4},
    {JORDAN, MEDIASIZE_A4},
    {KOREA, MEDIASIZE_A4},
    {LEBANON, MEDIASIZE_A4},
    {LUXEMBURG, MEDIASIZE_A4},
    {MALAYSIA, MEDIASIZE_A4},
    {MEXICO, MEDIASIZE_LETTER},
    {NETHERLANDS, MEDIASIZE_A4},
    {NEW_ZEALAND, MEDIASIZE_A4},
    {NORTH_AFRICA, MEDIASIZE_A4},
    {NORWAY, MEDIASIZE_A4},
    {PAKISTAN, MEDIASIZE_A4},
    {PERU, MEDIASIZE_LETTER},
    {PHILIPPINES, MEDIASIZE_LETTER},
    {POLAND, MEDIASIZE_A4},
    {PORTUGAL, MEDIASIZE_A4},
    {ROMANIA, MEDIASIZE_A4},
    {RUSSIA, MEDIASIZE_A4},
    {SINGAPORE, MEDIASIZE_A4},
    {SLOVAK_REP, MEDIASIZE_A4},
    {SOUTH_AFRICA, MEDIASIZE_A4},
    {SPAIN, MEDIASIZE_A4},
    {SRI_LANKA, MEDIASIZE_A4},
    {SWEDEN, MEDIASIZE_A4},
    {SWITZERLAND, MEDIASIZE_A4},
    {TAIWAN, MEDIASIZE_A4},
    {THAILAND, MEDIASIZE_A4},
    {TURKEY, MEDIASIZE_A4},
    {UK, MEDIASIZE_A4},
    {USA, MEDIASIZE_LETTER},
    {VIETNAM, MEDIASIZE_A4},
    {JAPAN, MEDIASIZE_A4}       // BUGBUG: verify that this is correct -jrs
};
#define NUM_STRING_TO_MEDIA_SIZE_MAPS ( sizeof(country_code_to_media_size_map) / sizeof(country_code_to_media_size_map_t) )

#endif
#endif


platform_vars_t plat_vars;
CONFIG_TABLE_VARS plat_config_vars;

#ifdef HAVE_NVRAM
nvram_handle_t *plat_nvm_handle;
#ifdef HAVE_NVRAM_FILE
#else
static error_type_t CheckConfig(uint32_t *Buffer);
#endif // HAVE_NVRAM_FILE
#endif // HAVE_NVRAM


error_type_t plat_init_config_table_vars(void)
{
    plat_config_vars.prod1284String[0] = 0;
    plat_config_vars.ProductSerNum[0] = 0;
    plat_config_vars.DefaultLang = LANG_ENGLISH;
    plat_config_vars.USBVendorID = DEFAULT_VENDOR_ID;
    plat_config_vars.USBProductID = DEFAULT_PRODUCT_ID;
    plat_config_vars.USBLangDesc = DEFAULT_USB_LANG;
    plat_config_vars.FormatterSerNum[0] = 0;
    strcpy(plat_config_vars.Version, FIRMWARE_VERSION_STRING);
    strcpy(plat_config_vars.DevClass, DEFAULT_DEV_CLASS);
    strncpy(plat_config_vars.USBMfgStr, DEFAULT_MFG_STRING, NVLEN_USB_MFGSTR);
    strncpy(plat_config_vars.USBProdStr, DEFAULT_PROD_STRING, NVLEN_USB_PRODSTR);
    char prt_name[NVLEN_PRINTER_NAMESTR];
    getPrinterName(prt_name);
    strncpy(plat_config_vars.PrinterNameStr, prt_name, NVLEN_PRINTER_NAMESTR);
    strncpy(plat_config_vars.CmdLang, DEFAULT_CMD_LANG, NVLEN_CMD_LANG);
    memset(plat_config_vars.PrinterMACAddr, 0, NVLEN_MACADDR);   // zero out the address
    return OK;
}


void plat_store_config_vals(UINT8* pNvramShadow)
{
    uint32_t index;
    for(index = 0; index < NUM_CONFIG_TABLE_ENTRIES; index++)
    {
        configTable[index].pStorageLoc((void *)(pNvramShadow + configTable[index].offset));
    }
}


#ifdef HAVE_NVRAM
error_type_t plat_vars_init_callback(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused)
{
    switch( InitType )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            plat_vars.powerup_counter = 0;
            plat_vars.uptime_counter.hours = 0;
            plat_vars.uptime_counter.mins = 0;
            plat_vars.IOTimeOut = DEFAULT_IO_TIMEOUT;
        #ifdef HAVE_RTC
            memset(&plat_vars.install_date, 0, sizeof(date_time_t));
        #endif
            strcpy(plat_vars.install_version, FIRMWARE_VERSION_STRING);

            ASSERT(sizeof(plat_vars) <= InitLocationSize);
            memcpy( InitLocation, &plat_vars, sizeof(plat_vars));
            break;

        default:
            ASSERT( NVRAM_OK == InitType );
            // read out current values
            memcpy(&plat_vars, InitLocation, sizeof(plat_vars));
            break;
    }
    return ( OK );
}
#endif
 

error_type_t plat_power_down(void)
{
#if 0
    gpio_handle_t * power_down_handle;
    error_type_t ret; 

    ret = gpio_open( &power_down_handle,
            GPIO_BANK_B,
            8,
            GPIO_DIRECTION_OUTPUT,
            GPIO_LOGIC_SETTING_ACTIVE_HIGH,
            GPIO_LOGIC_LEVEL_ASSERTED,
            GPIO_PULL_SETTING_FLOATING,
            "Power Control" );

    ASSERT( OK == ret );
    return ret;
#endif
	return OK;
}

static country_code_to_string_map_t plat_country_code_to_string_map[] =
{
    {STRING_CP_CTRY_USA,            USA},
    {STRING_CP_CTRY_FRANCE,         FRANCE},
    {STRING_CP_CTRY_GERMANY,        GERMANY}
};

#define NUM_STRING_TO_COUNTRY_MAPS ( sizeof(plat_country_code_to_string_map) / sizeof(country_code_to_string_map_t) )

uint32_t plat_get_country_map_table(country_code_to_string_map_t** country_code_to_string_map)
{
    *country_code_to_string_map = plat_country_code_to_string_map;
    return NUM_STRING_TO_COUNTRY_MAPS;
}

#define CONFIG_SECTOR     1
#define LCM_BUFFER_SIZE   512

#ifdef HAVE_NVRAM
#ifdef HAVE_NVRAM_FILE
#else
static error_type_t CheckConfig(uint32_t *Buffer);
#endif // HAVE_NVRAM_FILE
#endif // HAVE_NVRAM

/**
    \brief Initialize the NVRAM managment system

    \return error_type_t
    \retval 0 OK
    \retval -1 FAIL

**/
error_type_t platform_nvram_config_init(void)
{
    error_type_t status;
#ifdef HAVE_NVRAM
#ifdef HAVE_NVRAM_FILE
    status = FAIL;
#else
    int8_t    *tempMem = NULL;
    uint32_t   bytesRead = 0;

    status = FAIL;

    tempMem = RomMgrReadData(CONFIG_SECTOR, 0, &bytesRead);

    if (bytesRead && (tempMem != NULL))
    {
        status = CheckConfig((uint32_t*)tempMem);
    }

    if (status == OK)
    {
        // Store Configuration values from mem to permanent DRAM storage
        // locations for later use
        plat_store_config_vals((uint8_t *)tempMem);

    }
    else
    {
        DPRINTF(DBG,("NVRAM: Not using default Configuration Table\n"));
    }

    if (tempMem != NULL)
    {
        MEM_FREE_AND_NULL(tempMem);
    }
    status = FAIL;
#endif // HAVE_NVRAM_FILE
#endif // HAVE_NVRAM

    plat_check_default_personality_override();
    platform_init_1284_string();
    
    return status;
}


void plat_get_asic_id( uint32_t * id )
{
	*id = *REG_ASIC_ID;
}


#ifdef HAVE_NVRAM
#ifdef HAVE_NVRAM_FILE
#else
static error_type_t CheckConfig(uint32_t *Buffer)
{
    uint32_t TableSize=0;
    uint32_t CheckSum=0;

    //
    // The table size is the 254 and 255th bytes of the table.  Get that.
    //
    TableSize = (uint16_t)( be32_to_cpu(Buffer[(NVOFF_NAND_TBL_LEN>>2)]) & 0xffff );

    //
    //
    // Make sure our buffer size makes sense.
    //

    if( TableSize > (LCM_BUFFER_SIZE * 4))
        return FAIL;
    //
    // We now have the data, do the checksum and update variables.
    //
    CheckSum = CalcChecksum((uint32_t *)((uintptr_t)Buffer + NVLEN_STATIC_CHECKSUM),
                              TableSize - NVLEN_STATIC_CHECKSUM);
    //
    // See if the checksum is valid.
    //

    if( ( CheckSum & 0xffff ) == ( be32_to_cpu(Buffer[0]) >> 16 ) )
    {
        return OK;
    }
    return FAIL;
}
#endif // HAVE_NVRAM_FILE
#endif // HAVE_NVRAM

error_type_t plat_set_current_country_code( e_CountryCode code )
{
#ifdef HAVE_PRINT_SUPPORT

#ifdef HAVE_PAPER
    mediacnfg_t default_tray_size;
    uint32_t i;
    error_type_t e_res;

    for ( i = 0; i < NUM_STRING_TO_MEDIA_SIZE_MAPS; i++ )
    {
        if( country_code_to_media_size_map[i].code == code )
        {
            /* KAP not sure why we are setting paper size here
             * REVISIT TODO */
            e_res = printvar_get_trayconfig(&default_tray_size);
            ASSERT( OK == e_res );
            default_tray_size.DefmediaSize = country_code_to_media_size_map[i].size;
            e_res = printvar_set_trayconfig(&default_tray_size);
            ASSERT( OK == e_res );

            plat_vars.current_country_code = code;
#ifdef HAVE_NVRAM
            e_res = nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
            ASSERT( OK == e_res );

            return OK;
        }
    }
#endif
#endif
    return FAIL;
}
void getPrinterName(char *value)
{

    FILE *fp = NULL;
	char tmpStr[NVLEN_PRINTER_NAMESTR];
    if(NULL == value)
    {
        return;
    }
    fp = popen("/app/karas_pinetree_util.sh --get-product-string","r");
    if(NULL == fp)
    {
        snprintf(value,NVLEN_PRINTER_NAMESTR,"%s MFP",DEFAULT_MFG_STRING);
        return;
    }
    else
    {
        if(fgets(tmpStr,NVLEN_PRINTER_NAMESTR,fp) == NULL)
        {
            snprintf(value,NVLEN_PRINTER_NAMESTR,"%s MFP",DEFAULT_MFG_STRING);
        }
        else
        {
            *(tmpStr +(strlen(tmpStr)-1)) = 0;
            if(strcmp(tmpStr,"DEFAULT_PRODUCT_STRING") == 0)
            {
                snprintf(value,NVLEN_PRINTER_NAMESTR,"%s MFP",DEFAULT_MFG_STRING);
            }
			else
			{
			    snprintf(value,NVLEN_PRINTER_NAMESTR,"%s %s",DEFAULT_MFG_STRING,tmpStr);
			}
        }
        pclose(fp);
    }
}