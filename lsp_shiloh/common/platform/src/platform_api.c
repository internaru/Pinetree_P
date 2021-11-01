/*
 *
 * ============================================================================
 * Copyright (c) 2007-2012   Marvell International Ltd. All Rights Reserved.
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
#include <posix_ostools.h>
#include <string.h>
#include "memAPI.h"
#include "checksum.h"
#include "dprintf.h"
#ifdef HAVE_NVRAM
#include "nvram_api.h"
#endif
#include "nvramTable.h"
#include "logger.h"
#include "platform.h"
#include "platform_api.h"
#include "platform_types.h"
#include "platform_private.h"
#include "fwVersion.h"
#include "lassert.h"
#include "location_api.h"
#include "string_mgr_api.h"
#include "data_access_api.h"
#include "agConnectMgr.h"
#include "error_types.h"
#include "ioutils.h"
#include "utils.h"
#ifdef HAVE_PAPER
#include "paper_size_api.h"
#include "paper_types_api.h"
#endif
#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
#include "printvars_api.h"
#endif
#ifdef HAVE_RTC
#include "rtc_api.h"
#endif

// Marvell reference platfrom personality override.  If the USBProdStr in NVRAM
// matches this string the system will use the personality defaults defined in platform.h.
#define MARVELL_REF_OVERRIDE_STR "MarvellReference"

static error_type_t plat_set_install_date(void);

static error_type_t updateNVMConfigTable(uint32_t Offset, uint32_t Len, char *data);

static pthread_t uptime_thread_id;
static mqd_t uptime_msgq;


#define UPTIME_STACK_SIZE POSIX_MIN_STACK_SIZE 
ALIGN8 unsigned char UptimeStack[ UPTIME_STACK_SIZE ];
 
#define UPDATE_DELAY 120        // 2 hours, don't use less or it will wear out nvram part.
void *Uptime( void *unused )
{
    uint16_t i;

    dbg_printf("%s: TID = %d\n", __func__, gettid());

    while(1) {
        for(i = 0; i < UPDATE_DELAY; i++) {                      /* every UPDATE_DELAY minutes */
            sleep( 60 );
            plat_vars.uptime_counter.mins += 1;
            while (plat_vars.uptime_counter.mins >= 60) {
                plat_vars.uptime_counter.hours++;
                plat_vars.uptime_counter.mins -= 60;
            }
        }
#ifdef HAVE_NVRAM
        nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
    }
    return 0;
}


void platvars_init()
{
    uint32_t h;
    uint16_t i;
    uint8_t m;
    error_type_t status;
#ifdef HAVE_RTC
    date_time_t time;
#endif
    char *version;

    plat_init_config_table_vars();

#ifdef HAVE_NVRAM
    nvram_variable_register(&plat_nvm_handle, PLAT_VARS_ID, PLAT_VARS_VERSION, sizeof(plat_vars), plat_vars_init_callback, NULL);
#endif
    platvars_inc_powerup_counter();
    platvars_get_powerup_counter(&h);
//    if(h == 1)
//        plat_set_install_date();
#ifdef HAVE_RTC
    plat_get_install_date( &time, &version );
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("Install date version %2d/%02d/%4d  %s\n", time.month + 1, time.day + 1, (time.year + 2000), version));
#else
    plat_get_install_date( &version );
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("Install version %s\n",version));
#endif
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("Powerup count = %d\n",h));
    platvars_get_uptime_counter(&h, &m);
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("Current uptime: %d hours, %d minutes\n", h, m));
    platvars_get_io_timeout(&i);
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("IOTimeout %d\n",i));

    status = platform_nvram_config_init();
    posix_create_thread(&uptime_thread_id, Uptime, 0, UptimeStack, UPTIME_STACK_SIZE, POSIX_THR_PRI_NORMAL);
}

void plat_check_default_personality_override()
{
    if(str_match(MARVELL_REF_OVERRIDE_STR, plat_config_vars.USBProdStr, strlen(MARVELL_REF_OVERRIDE_STR)))
    {
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("PLAT: Using default personality override\n"));
        plat_config_vars.USBVendorID = DEFAULT_VENDOR_ID;
        plat_config_vars.USBProductID = DEFAULT_PRODUCT_ID;
        plat_config_vars.USBLangDesc = DEFAULT_USB_LANG;
        strcpy(plat_config_vars.DevClass, DEFAULT_DEV_CLASS);
        strncpy(plat_config_vars.USBMfgStr, DEFAULT_MFG_STRING, NVLEN_USB_MFGSTR);
        char prt_name[NVLEN_PRINTER_NAMESTR];
        getPrinterName(prt_name);
        strncpy(plat_config_vars.USBProdStr, DEFAULT_PROD_STRING, NVLEN_USB_PRODSTR);
        strncpy(plat_config_vars.PrinterNameStr, prt_name, NVLEN_PRINTER_NAMESTR);
        strncpy(plat_config_vars.CmdLang, DEFAULT_CMD_LANG, NVLEN_CMD_LANG);
    }
}

void platvars_shutdown()
{
    uint32_t h;
    int result;

    platvars_get_powerup_counter(&h);
    if(h == 1)
        plat_set_install_date();

    result = pthread_cancel(uptime_thread_id);
    XASSERT(0 == result, thread_errno);

#ifdef HAVE_NVRAM
    nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
}


error_type_t platvars_get_uptime_counter(uint32_t *hours, uint8_t *mins)
{
    *hours = plat_vars.uptime_counter.hours;
    *mins = plat_vars.uptime_counter.mins;
    return OK;
}

error_type_t platvars_get_powerup_counter(uint32_t *counter)
{
    *counter = plat_vars.powerup_counter;
    return OK;
}
error_type_t platvars_inc_powerup_counter(void)
{
    plat_vars.powerup_counter++;
#ifdef HAVE_NVRAM
    nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
    return OK;
}

error_type_t platvars_get_devclass(char **Version)
{
    *Version = plat_config_vars.DevClass;
    return OK;
}
error_type_t platvars_get_fw_version(char **Version)
{
    *Version = plat_config_vars.Version;
    return OK;
}
error_type_t platvars_get_mac_addr(char **MacAddr)
{
    *MacAddr = plat_config_vars.PrinterMACAddr;
    return OK;
}

error_type_t platvars_set_mac_addr(char *MacAddr)
{
    memcpy(plat_config_vars.PrinterMACAddr, MacAddr, NVLEN_MACADDR);
    return OK;
}

error_type_t platvars_get_cmdlangstr(char **String)
{
    *String = plat_config_vars.CmdLang;
    return OK;
}

error_type_t platvars_set_cmdlangstr(char *String)
{
    memcpy(plat_config_vars.CmdLang, String, NVLEN_CMD_LANG);
    return OK;
}

error_type_t platvars_get_prtnamestr(char **String)
{
    *String = plat_config_vars.PrinterNameStr;
    return OK;
}

error_type_t platvars_set_prtnamestr(char *String)
{
    strncpy(plat_config_vars.PrinterNameStr, String, NVLEN_PRINTER_NAMESTR);
    return OK;
}

error_type_t platvars_get_usb_prodstr(char **String)
{
    *String = plat_config_vars.USBProdStr;
    return OK;
}

error_type_t platvars_set_usb_prodstr(char *String)
{
    strncpy(plat_config_vars.USBProdStr, String, NVLEN_USB_PRODSTR);
    return OK;
}

error_type_t platvars_get_usb_mfgstr( char **String)
{
    *String = plat_config_vars.USBMfgStr;
    return OK;
}

error_type_t platvars_set_usb_mfgstr(char *String)
{
    strncpy(plat_config_vars.USBMfgStr, String, NVLEN_USB_MFGSTR);
    return OK;
}

error_type_t platvars_get_formater_ser_num(char **String)
{
    *String = plat_config_vars.FormatterSerNum;
    return OK;
}

error_type_t platvars_set_formater_ser_num(char *String)
{
    strncpy(plat_config_vars.FormatterSerNum, String, NVLEN_FORMATTER_SER_NUM);
    return OK;
}

error_type_t platvars_get_usb_langdesc(uint16_t *Value)
{
    *Value = plat_config_vars.USBLangDesc;
    return OK;
}

error_type_t platvars_set_usb_langdesc(uint16_t *Value)
{
    plat_config_vars.USBLangDesc = *Value;
    return OK;
}

error_type_t platvars_config_table_set_usb_langdesc(uint16_t *value)
{
    error_type_t e_res;
    uint16_t usb_langdesc;

    /* Handle big and little endian systems */
    usb_langdesc = be16_to_cpu(*value);
    e_res = platvars_set_usb_langdesc( &usb_langdesc );
    return e_res;
}

error_type_t platvars_get_usb_pid(uint16_t *Value)
{
    *Value = plat_config_vars.USBProductID;
    return OK;
}

error_type_t platvars_set_usb_pid(uint16_t *Value)
{
    plat_config_vars.USBProductID = *Value;
    return OK;
}

error_type_t platvars_config_table_set_usb_pid(uint16_t *value)
{
    error_type_t e_res;
    uint16_t usb_pid;

    /* Handle big and little endian systems */
    usb_pid = be16_to_cpu(*value);
    e_res = platvars_set_usb_pid( &usb_pid );
    return e_res;
}

error_type_t platvars_get_1284str(char **String)
{
    *String = plat_config_vars.prod1284String;
    return OK;
}

error_type_t platvars_set_1284str(char *String)
{
    memcpy(plat_config_vars.prod1284String, String, LEN_1284_STRING);
    return OK;
}
error_type_t platvars_get_prod_serialnum(char **String)
{
    *String = plat_config_vars.ProductSerNum;
    return OK;
}

error_type_t platvars_set_prod_serialnum(char *String)
{
    strncpy(plat_config_vars.ProductSerNum, String, NVLEN_PROD_SER_NUM);
    return OK;
}

error_type_t platvars_get_default_lang( string_mgr_language_t * language )
{
    /* TODO REMOVE when I figure out how to set this thing permanently */
    plat_config_vars.DefaultLang = LANG_ENGLISH;

    *language = (char)plat_config_vars.DefaultLang;
    return OK;
}

error_type_t platvars_set_default_lang(string_mgr_language_t * language)
{
    plat_config_vars.DefaultLang = *( (char *)language );
    return OK;
}

error_type_t platvars_get_usb_vid(uint16_t *Value)
{
    *Value = plat_config_vars.USBVendorID;
    return OK;
}

error_type_t platvars_set_usb_vid(uint16_t *Value)
{
    plat_config_vars.USBVendorID = *Value;
    return OK;
}

error_type_t platvars_config_table_set_usb_vid(uint16_t *value)
{
    error_type_t e_res;
    uint16_t usb_vid;

    /* Handle big and little endian systems */
    usb_vid = be16_to_cpu(*value);
    e_res = platvars_set_usb_vid( &usb_vid );
    return e_res;
}

error_type_t platvars_get_io_timeout(uint16_t *timeout)
{
    *timeout = plat_vars.IOTimeOut;
    return OK;
}

error_type_t platvars_set_io_timeout(uint16_t *timeout)
{
    if((*timeout > MAX_IO_TIMEOUT) ||
       (*timeout < MIN_IO_TIMEOUT))
    {
        return FAIL;        // out of range, fail.
    }
    plat_vars.IOTimeOut = *timeout;
    
#ifdef HAVE_NVRAM
    nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
    return OK;
}


error_type_t platConfigMACAddr(char *MacAddr)
{
    error_type_t status = FAIL;
    platvars_set_mac_addr(MacAddr);
    status = updateNVMConfigTable(NVOFF_MACADDR, NVLEN_MACADDR, MacAddr);
    return status;
}
error_type_t platConfigPrtNameStr(char *String)
{
    error_type_t status = FAIL;
    platvars_set_prtnamestr(String);
    status = updateNVMConfigTable(NVOFF_PRINTER_NAMESTR, NVLEN_PRINTER_NAMESTR, String);
    return status;
}

error_type_t platConfigUSBProdStr(char *String)
{
    error_type_t status = FAIL;
    platvars_set_usb_prodstr(String);
    status = updateNVMConfigTable(NVOFF_USB_PRODSTR, NVLEN_USB_PRODSTR, String);
    return status;
}

error_type_t platConfigFormatSerNum(char *String)
{
    error_type_t status = FAIL;
    platvars_set_formater_ser_num(String);
    status = updateNVMConfigTable(NVOFF_FORMATTER_SER_NUM, NVLEN_FORMATTER_SER_NUM, String);
    return status;
}

error_type_t platConfigUSBPID(uint16_t *Value)
{
    error_type_t status = FAIL;
    uint16_t usb_pid;

    platvars_set_usb_pid(Value);
    /* Handle big and little endian systems */
    usb_pid = cpu_to_be16(*Value);

    status = updateNVMConfigTable(NVOFF_USB_PRODUCTID, NVLEN_USB_PRODUCTID, (char *)&usb_pid);
    return status;
}

error_type_t platConfigProdSerialNum(char *String)
{
    error_type_t status = FAIL;
    platvars_set_prod_serialnum(String);
    status = updateNVMConfigTable(NVOFF_PROD_SER_NUM, NVLEN_PROD_SER_NUM, String);
    return status;
}


static error_type_t updateNVMConfigTable(uint32_t Offset, uint32_t Len, char *data)
{
    error_type_t status;
#ifdef HAVE_NVRAM
    int8_t  *tempMem = NULL;
    uint32_t CheckSum = 0; //place holder for checksum
    uint32_t byteRead = 0;

    status = FAIL;

    tempMem = (int8_t *)RomMgrReadData(IM_CFG_SECTOR, 0, &byteRead);

    //get the update data
    if((byteRead == 0) || (tempMem == NULL))
    {
        return FAIL;
    }

    //modify configtable
    memcpy(tempMem + Offset, data, Len);

    //
    // calculate new checksum and update variables.
    //

    CheckSum = CalcChecksum((uint32_t *)(tempMem + NVLEN_STATIC_CHECKSUM),
                              byteRead - NVLEN_STATIC_CHECKSUM);

    CheckSum &= 0xffff;
    CheckSum = cpu_to_be32(CheckSum);

    //update check sum
    memcpy((tempMem + NVOFF_STATIC_CHECKSUM), ((int8_t *)&CheckSum + 2), NVLEN_STATIC_CHECKSUM);

    status = RomMgrWriteData(IM_CFG_SECTOR, 0, (int8_t *)tempMem, byteRead);

    MEM_FREE_AND_NULL(tempMem);
#else
    status = FAIL;
#endif

    return status;
}


error_type_t plat_get_current_country_code( e_CountryCode * code )
{
    ASSERT( NULL != code );
    /* TODO REMOVE when the country code is set to a default good value
     * temporary write to prevent ASSERTING */
    if(plat_vars.current_country_code != USA)
        plat_set_current_country_code( USA );
    *code = plat_vars.current_country_code;
    return OK;
}

error_type_t plat_get_default_country_code( e_CountryCode * code )
{
    ASSERT( NULL != code );
    /* TODO REMOVE when the country code is set to a default good value
     * temporary write to prevent ASSERTING */
    plat_set_default_country_code( USA );
    *code = plat_vars.default_country_code;
    return OK;
}

 
error_type_t plat_set_default_country_code( e_CountryCode code )
{
    error_type_t e_res;

    plat_vars.default_country_code = code;
#ifdef HAVE_NVRAM
    e_res = nvram_set_var(plat_nvm_handle, &plat_vars);
#endif
    ASSERT( OK == e_res );
    return OK;
}

error_type_t plat_set_install_date(void)
{
    error_type_t e_res = OK;
    char *Version;

#ifdef HAVE_RTC
    e_res = rtc_get_time( &plat_vars.install_date );
#endif

    platvars_get_fw_version(&Version);
    memcpy( plat_vars.install_version, Version, VERSION_STRLEN );

#ifdef HAVE_NVRAM
    nvram_set_var(plat_nvm_handle, &plat_vars);
#endif

    return e_res;
}

#ifdef HAVE_RTC
error_type_t plat_get_install_date( date_time_t *time, char **version )
{
    memcpy( time, &plat_vars.install_date, sizeof(date_time_t) );

    *version = plat_vars.install_version;

    return OK;
}
#else
error_type_t plat_get_install_date( char **version )
{
    *version = plat_vars.install_version;
    return OK;
}
#endif


/**
 * \brief A function used to get any country's string
 * based on the country code
 *
 * \param code A valid e_CountryCode enum value
 *
 * \param string a pointer to the string manager's static string representing 
 * the language code
 *
 *
 * \return OK upon success, FAIL otherwise
 */
error_type_t plat_get_country_string( e_CountryCode code, char ** string )
{
    uint32_t i;
    error_type_t e_res;
    e_res = FAIL;
    country_code_to_string_map_t* country_code_to_string_map;
    uint32_t num_string_to_country_maps;

    ASSERT( NULL != string );

    num_string_to_country_maps = plat_get_country_map_table(&country_code_to_string_map);

    for (i = 0; i < num_string_to_country_maps; i++)
    {
        if ( country_code_to_string_map[i].code == code )
        {
            *string = (char *)string_mgr_get_string( country_code_to_string_map[i].string_code, string_mgr_loc_english );
            ASSERT( NULL != *string );
            e_res = OK;
                
            break;
        }
    }

    return e_res;
}

/**
 *
 * \brief Initialize the IEEE-1284 printer communications string
 */
void platform_init_1284_string(void)
{
    char *Lang;
    char Comma[]=",";
    uint32_t i,m;
    char *usbMfgStr, *usbProdStr, *prtNameStr, *new1284Str, *Version, *DevClass;
    char * pjl_language_string;

    while((Lang = MEM_MALLOC(256)) == NULL);
    while((new1284Str = MEM_MALLOC(LEN_1284_STRING)) == NULL);
    Lang[0] = '\0';     // init the string
    m = GetNumLanguages();
    for(i = 0; i < m; i++)
    {
        pjl_language_string = GetPJLLanguageString(i);

        if ( NULL != pjl_language_string )
        {
            strcat( Lang, pjl_language_string );
        }

        if( i < ( m - 1 ) )
        {
            strcat(Lang, Comma);
        }
    }
    platvars_get_usb_mfgstr(&usbMfgStr);
    platvars_get_usb_prodstr(&usbProdStr);
    platvars_get_prtnamestr(&prtNameStr);
    platvars_get_fw_version(&Version);
    platvars_get_devclass(&DevClass);
    // use USB vals to build 1284 string
    minSprintf(new1284Str, TEMPLATE_1284_STRING, usbMfgStr, usbProdStr, Lang, DevClass, prtNameStr, Version);
    // add length to 1284 string; + 2 accounts for the length bytes themselves
    *(uint16_t *)&new1284Str[0] = cpu_to_be16(strlen(&new1284Str[2]) + 2);
    platvars_set_1284str(new1284Str);      // save the thing
    
    MEM_FREE_AND_NULL(new1284Str);
    MEM_FREE_AND_NULL(Lang);
}
