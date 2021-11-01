/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h> // memcpy
#include <stdio.h> // sprintf

#ifdef __linux__

#include "os_network.h"
#include "net_api.h" // NET_MAX_INTERFACES
#include "net_iface.h" // net_iface_enum_from_if_index()

#include "wifi_intf.h"
#include "wifi_intf_vars.h"

#include "nvram_api.h"
#include "data_access_api.h"
#include "net_wifi_api.h" // DEFAULT_WIFI_SSID

#else // ! __linux__

#include "ATypes.h"
#include "tx_api.h"
#include "nvram_api.h"
#include "wifi_intf.h"
#include "wifi_intf_vars.h"
#include "data_access_api.h"
#include "logger.h"
#include "dprintf.h"
#include "debug.h"
#include "string.h"
#include "lassert.h"

#endif

// local prototypes
static error_type_t wifi_intf_var_restore_shadow_defaults(unsigned int index, wifi_intf_var_id_t var_id);
unsigned int net_get_active_wifi_intf(void);

/* static members */
static bool g_wifi_intf_nvram_is_registered = false;

static OS_MUTEX g_wifi_intf_var_wireless_mutex;
static OS_MUTEX g_wifi_intf_var_scan_mutex;
static OS_MUTEX g_wifi_intf_var_misc_mutex;
static nvram_handle_t *g_wifi_intf_nvram_hndl[WIFI_NUM_IFACES];

static wifi_intf_wireless_info_t g_wifi_intf_perm_vars[WIFI_NUM_IFACES];
static wifi_intf_wireless_info_t* shadow_wireless_info = &g_wifi_intf_perm_vars[0]; // default 1st index
static wifi_intf_scan_info_t shadow_wireless_scan_info; // singleton

typedef struct
{
    bool is_wireless_sta_enabled;
    bool is_wireless_scanning;
} MISC_WIRELESS_VARS;

static MISC_WIRELESS_VARS shadow_misc_vars; // singleton

static error_type_t wifi_intf_nvram_init_cb(nvram_init_type_t, uint16_t, void*, uint32_t, void*);


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifdef __linux__

#define DBG_PRFX "wifi.var: "
//off #define DBG_VERBOSE_ON
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#include "net_debug.h"

#else
#define DBG_PRFX "WIFIVAR: "
#define DBG_ON
#define DBG_ERR_ON
//#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON

#ifdef DBG_RELEASE_ON
#define DBG_PRINTF_MACRO(...) dbg_printf(DBG_PRFX __VA_ARGS__)
#else
#define DBG_PRINTF_MACRO(...) DPRINTF((DBG_SOFT|DBG_OUTPUT), (DBG_PRFX __VA_ARGS__))
#endif
#ifdef DBG_ON
#define DBG_MSG(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif
#ifdef DBG_ERR_ON
#define DBG_ERR(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_ERR(...)
#endif
#ifdef DBG_VERBOSE_ON
#define DBG_VERBOSE(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_VERBOSE(...)
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#define DBG_CMD(...) cmd_printf(__VA_ARGS__) // omit module prefix for debug command output
#endif
#endif



//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#ifdef DEBUG
#define WIFI_INTF_VAR_WIRELESS_LOCK() \
        { \
            unsigned int os_rcode = os_mutex_get(&g_wifi_intf_var_wireless_mutex, TX_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 
#else
#define WIFI_INTF_VAR_WIRELESS_LOCK() \
        { \
            os_mutex_get(&g_wifi_intf_var_wireless_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_WIRELESS_UNLOCK() \
        { \
            unsigned int os_rcode = os_mutex_put(&g_wifi_intf_var_wireless_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        }
#else
#define WIFI_INTF_VAR_WIRELESS_UNLOCK() \
        { \
            os_mutex_put(&g_wifi_intf_var_wireless_mutex); \
        }
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_SCAN_LOCK() \
        { \
            unsigned int os_rcode = os_mutex_get(&g_wifi_intf_var_scan_mutex, TX_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 
#else
#define WIFI_INTF_VAR_SCAN_LOCK() \
        { \
            os_mutex_get(&g_wifi_intf_var_scan_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_SCAN_UNLOCK() \
        { \
            unsigned int os_rcode = os_mutex_put(&g_wifi_intf_var_scan_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        }
#else
#define WIFI_INTF_VAR_SCAN_UNLOCK() \
        { \
            os_mutex_put(&g_wifi_intf_var_scan_mutex); \
        }
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_MISC_LOCK() \
        { \
            unsigned int os_rcode = os_mutex_get(&g_wifi_intf_var_misc_mutex, TX_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 
#else
#define WIFI_INTF_VAR_MISC_LOCK() \
        { \
            os_mutex_get(&g_wifi_intf_var_misc_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_MISC_UNLOCK() \
        { \
            unsigned int os_rcode = os_mutex_put(&g_wifi_intf_var_misc_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        }
#else
#define WIFI_INTF_VAR_MISC_UNLOCK() \
        { \
            os_mutex_put(&g_wifi_intf_var_misc_mutex); \
        }
#endif

#ifdef __linux__
static error_type_t wifi_intf_nvram_block_write(unsigned int index)
#else
static error_type_t wifi_intf_nvram_block_write(void)
#endif
{
    error_type_t retval = WIFI_INTF_OK;

    if(g_wifi_intf_nvram_is_registered)
    {
#ifdef __linux__
        retval = nvram_set_var(g_wifi_intf_nvram_hndl[index], &g_wifi_intf_perm_vars[index]);
#else
        retval = nvram_set_var(&g_wifi_intf_nvram_hndl, &shadow_wireless_info);
#endif
        if(retval < 0)
        {
            retval = WIFI_INTF_FAIL;
        }
    }

    return retval;
}

/** 
 *  Boot-time initialization
 * 
 *  This function is called at system startup following initialization of the NVRAM driver and
 *  partition manager.  It initializes the module datastore variables to either their default values
 *  or values previously stored in NVRAM.
 * 
 *  \param[in] init_type type of initialization being done
 **/ 
#ifdef __linux__
void wifi_intf_var_init(void)
#else
void wifi_intf_var_init(dataAccessInit_t init_type)
#endif
{
    uint32_t os_rcode;
    error_type_t retval;


#ifdef __linux__
    os_rcode = posix_mutex_init(&g_wifi_intf_var_wireless_mutex);
#else
    os_rcode = os_mutex_create(&g_wifi_intf_var_wireless_mutex, "wlanVar", TX_INHERIT);
#endif
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating wlan mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_wifi_intf_var_scan_mutex);
#else
    os_rcode = os_mutex_create(&g_wifi_intf_var_scan_mutex, "wlanScanVar", TX_INHERIT);
#endif
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating wlan scan mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

#ifdef __linux__
    os_rcode = posix_mutex_init(&g_wifi_intf_var_misc_mutex);
#else
    os_rcode = os_mutex_create(&g_wifi_intf_var_misc_mutex, "wlanMiscVar", TX_INHERIT);
#endif
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating wlan misc mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

    /*
     * Zero out the memory for the local static structures
     */
    memset((void*)shadow_wireless_info, 0, sizeof(shadow_wireless_info));
    memset((void*)&shadow_wireless_scan_info, 0, sizeof(shadow_wireless_scan_info));
    memset((void*)&shadow_misc_vars, 0, sizeof(shadow_misc_vars));

#ifdef __linux__

    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating mutex (os_rcode=0x%02x)\n", os_rcode);
        goto error;
    }

    char vars_id[5]; // 4 char NVRAM label (as defined in data_access_api.h) + NULL terminator
    char vars_id_prefix[4]; // 3 char prefix + NULL terminator 
    strncpy(vars_id_prefix, WIFI_IFACE_VAR_ID, 3);
    vars_id_prefix[3] = '\0';
    uint8_t iface_enum;
    const int iface_count = WIFI_NUM_IFACES;
    DBG_ERR("%x\n", &iface_enum);
    for( iface_enum = 0; iface_enum < iface_count; iface_enum++)
    {
        /*
         *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
         *  net_iface_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
         *  values.
         */
        if(iface_enum>63) break; // hard limit imposed by base64 encoding of index
        sprintf(vars_id, "%s%c", vars_id_prefix, net_base64_uint8_to_char(iface_enum));
        DBG_VERBOSE("registering NVRAM label %s (cb=%08x)\n", vars_id, wifi_intf_nvram_init_cb);
        retval = nvram_variable_register(
            g_wifi_intf_nvram_hndl+iface_enum, 
            vars_id,
            WIFI_INTF_VARS_VERSION, 
            sizeof(wifi_intf_wireless_info_t),
            wifi_intf_nvram_init_cb, (void *)(uint32_t)iface_enum);
        if(retval != OK)
        {
            // NVRAM is not available so just set defaults and operate on the local copies            
            wifi_intf_var_restore_shadow_defaults(iface_enum, WIFI_INTF_VAR_ALL);
            wifi_intf_var_restore_defaults( (uint8_t) iface_enum, WIFI_INTF_VAR_ALL);
        }
        else
        {
            g_wifi_intf_nvram_is_registered = true;
            nvram_init_type_t init_type = NVRAM_OK; // revisit: argument rather than local?
            switch(init_type)
            {
            case NVRAM_OK:
                // load values stored in nvram
                //revisit nvram_get_var_ptr(g_wifi_intf_nvram_hndl, shadow_wireless_info);
                break;

            case NVRAM_NEW:
            case NVRAM_ERROR:
            case NVRAM_VERSION_CHANGE:
            case NVRAM_FACTORY_DEFAULTS:
            case NVRAM_LANG_DEFAULTS:
            case NVRAM_USER_DEFAULTS:
                // reset all items to their default values
                WIFI_INTF_VAR_WIRELESS_LOCK();
                wifi_intf_var_restore_defaults((uint8_t)iface_enum, WIFI_INTF_VAR_ALL);
                wifi_intf_nvram_block_write(net_iface_enum_to_if_index(iface_enum));
                WIFI_INTF_VAR_WIRELESS_UNLOCK();
                break;

            default:
                // unexpected value
                XASSERT(0, init_type);
                break;
            }
        }
    }
    /*
     *  Ask for a discrete dynamic nvram block.  If it doesn't exist or is corrupted,
     *  wifi_intf_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
#else
    retval = nvram_variable_register(&g_wifi_intf_nvram_hndl, WIRELESS_VAR_ID, WIFI_INTF_VARS_VERSION, sizeof(wifi_intf_wireless_info_t), wifi_intf_nvram_init_cb, NULL);
    if(retval != OK)
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        wifi_intf_var_restore_defaults(WIFI_INTF_VAR_ALL);
    }
    else
    {
        g_wifi_intf_nvram_is_registered = true;
        switch(init_type)
        {
        case e_UseStoredValues:
            // load values stored in nvram
            nvram_get_var_ptr(g_wifi_intf_nvram_hndl, shadow_wireless_info);
            break;

        case e_DefaultUserSettableValues:
        case e_DefaultAllValues:
            // reset all items to their default values
            WIFI_INTF_VAR_WIRELESS_LOCK();
            wifi_intf_var_restore_defaults(WIFI_INTF_VAR_ALL);
            wifi_intf_nvram_block_write(index);
            WIFI_INTF_VAR_WIRELESS_UNLOCK();
            break;

        default:
            // unexpected value
            XASSERT(0, init_type);
            break;
        }
    }
#endif

error:
    return;
}

/** 
 *  Initialize or repair a discrete dynamic NVRAM block
 *  
 *  This function is called by the dynamic NVRAM driver during the call to nvram_variable_register()
 *  either to initialize a new block of data or to repair an existing block which has a bad CRC,
 *  version discrepancy, or change in size.
 *  
 *  \param[in] init_type type of handling needed:  INIT_TYPE enum value from nvram_api.h
 *  
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return error_type_t
 *  \retval OK
 *
 **/ 

error_type_t wifi_intf_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *user_data)
{
    DBG_ASSERT( NULL != init_loc );
    uint8_t local_idx = (uint8_t)(uint32_t)user_data; // given wifi intf (offset)

    XASSERT(local_idx < WIFI_NUM_IFACES, local_idx);

    if (init_loc && local_idx < WIFI_NUM_IFACES)
    {
        switch( init_type )
        {
            case NVRAM_FACTORY_DEFAULTS:
            case NVRAM_LANG_DEFAULTS:
            case NVRAM_USER_DEFAULTS:
            case NVRAM_NEW:                /**< New block */
            case NVRAM_ERROR:              /**< Block error */
            case NVRAM_VERSION_CHANGE:     /**< Version change */
                WIFI_INTF_VAR_WIRELESS_LOCK(); 
                wifi_intf_var_restore_shadow_defaults(local_idx, NET_WIFI_VAR_ALL);
                ASSERT(sizeof(wifi_intf_wireless_info_t) <= init_loc_size);
                memcpy(init_loc, g_wifi_intf_perm_vars+local_idx, sizeof(wifi_intf_wireless_info_t));
                WIFI_INTF_VAR_WIRELESS_UNLOCK();
                break;

            default:
                ASSERT( NVRAM_OK == init_type );
                // read out current values
                memcpy(g_wifi_intf_perm_vars+local_idx, init_loc, sizeof(wifi_intf_wireless_info_t));
                break;
        }
        return ( OK );
    }
    return FAIL;
}


/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param var_id variable to initialize to default value, or WIFI_INTF_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
error_type_t wifi_intf_var_restore_defaults(uint8_t index, wifi_intf_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    all_vars = (var_id == WIFI_INTF_VAR_ALL);

    WIFI_INTF_VAR_WIRELESS_LOCK();

    switch(var_id)
    {
    case WIFI_INTF_VAR_ALL:
        // clear all presets
        memset((void*)shadow_wireless_info, '\0', sizeof(wifi_intf_wireless_info_t));
        // fall through

    case WIFI_INTF_COMM_MODE:
        (*shadow_wireless_info).CommunicationMode = AD_HOC;
        if (!all_vars) break;
    case WIFI_INTF_SEC_MODE:
        (*shadow_wireless_info).SecurityMode = OPEN_SYSTEM;
        if (!all_vars) break;
    case WIFI_INTF_WEP_AUTH_MODE:
        (*shadow_wireless_info).WEPAuthMode = AUTH_OPEN;
        if (!all_vars) break;
    case WIFI_INTF_WPA_ENCR_MODE:
        (*shadow_wireless_info).WPAEncryptionMode = WPA_TKIP_AND_AES;
        if (!all_vars) break;
    case WIFI_INTF_INPUT_KEY:
        (*shadow_wireless_info).InputKey = HEXADECIMAL;
        if (!all_vars) break;
    case WIFI_INTF_CHANNEL:
        (*shadow_wireless_info).Channel = DEFAULT_WIFI_CHAN; 
        if (!all_vars) break;
    case WIFI_INTF_SSID:
        memset((*shadow_wireless_info).SSID, 0x0, MAX_SSID_NAME_LEN);
        strcpy((char*)(*shadow_wireless_info).SSID, DEFAULT_WIFI_SSID);
        if (!all_vars) break;
    case WIFI_INTF_PASSPHRASE:
        memset((*shadow_wireless_info).PassPhrase, 0x0, MAX_PASS_PHRASE);
        if (!all_vars) break;
    case WIFI_INTF_CURR_WEP_KEY:
        (*shadow_wireless_info).CurrWepKey = 0;
        if (!all_vars) break;
    case WIFI_INTF_WEP_KEYS:
        memset((*shadow_wireless_info).WEPKey, 0x0, (NUM_WEP_KEYS * WEP_KEY_LEN));
        if (!all_vars) break;
    case WIFI_INTF_WEP_KEY_LEN:
        (*shadow_wireless_info).WEPKeyLength = 0;
        if (!all_vars) break;
    case WIFI_INTF_WPAPWK:
        memset((*shadow_wireless_info).WPAPWK, 0x0, PW_KEY_LEN);
        if (!all_vars) break;
    case WIFI_INTF_WPAGRPK:
        memset((*shadow_wireless_info).WPAGRPK, 0x0, GW_KEY_LEN);
        if (!all_vars) break;
    case WIFI_INTF_IS_WIRELESS_ENABLED:
        (*shadow_wireless_info).IsEnabled = (uint8_t)false;
        (*shadow_wireless_info).IsWirelessHWEnabled = (uint8_t)false;
        if (!all_vars) break;
    case WIFI_INTF_RFTX_MODE:
        (*shadow_wireless_info).RFTxMode = RF_TX_MODE_NORMAL;
        if (!all_vars) break;
    case WIFI_INTF_TX_ANTENNA:
        (*shadow_wireless_info).TxAntenna.Type = ANT_TX;
        (*shadow_wireless_info).TxAntenna.Num = ANT_DIVERSITY;
        if (!all_vars) break;
    case WIFI_INTF_RX_ANTENNA:
        (*shadow_wireless_info).RxAntenna.Type = ANT_RX;
        (*shadow_wireless_info).RxAntenna.Num = ANT_DIVERSITY;
        if (!all_vars) break;
    case WIFI_INTF_BSSID_SET:
        (*shadow_wireless_info).BssidSet = false;
        if (!all_vars) break;
    case WIFI_INTF_BSSID:
        memset((*shadow_wireless_info).BSSID, 0x0, BSSID_LENGTH);
        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    wifi_intf_nvram_block_write(index);
    WIFI_INTF_VAR_WIRELESS_UNLOCK();

    return(retval);
}

/** 
 *  Restore one or all variables stored in the discrete dynamic NVRAM block to their default values
 * 
 *  This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 *  individual values when their default function is called.  
 *  
 *  \param if_index treck index for interface
 *  \param var_id variable to initialize to default value, or WIFI_INTF_VAR_ALL for all vars
 *  
 *  \return error_type_t
 *  \retval OK
 *  \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if write fails
 **/ 
static error_type_t wifi_intf_var_restore_shadow_defaults(unsigned int index, wifi_intf_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;
    
    DBG_VERBOSE("%s: index=%d)\n", __func__, index);
             

    all_vars = (var_id == WIFI_INTF_VAR_ALL);


    uint8_t iface_enum = index; // revisit = net_iface_enum_from_if_index(if_index);

    WIFI_INTF_VAR_WIRELESS_LOCK();

    switch(var_id)
    {
    case WIFI_INTF_VAR_ALL:
        // fall through

    case WIFI_INTF_ENABLED:        
            g_wifi_intf_perm_vars[iface_enum].IsEnabled = true; // factory default
            g_wifi_intf_perm_vars[iface_enum].IsWirelessHWEnabled = true; // factory default
        if (!all_vars) break;

    case WIFI_INTF_SSID:
        {
            char *default_ssid_name;
#ifdef __linux__
            default_ssid_name = (char*)DEFAULT_WIFI_SSID; // factory default
#else
            default_ssid_name = (char*)wifi_get_default_ssid_name();
            if ( default_ssid_name == NULL )
            {
                platvars_get_prtnamestr(&default_ssid_name);
            }            
#endif
            strncpy((char*)g_wifi_intf_perm_vars[iface_enum].SSID, default_ssid_name, MAX_SSID_NAME_LEN); // cast
            g_wifi_intf_perm_vars[iface_enum].SSID[MAX_SSID_NAME_LEN-1] = 0;
        }
        if (!all_vars) break;

#if 0 // tbd: revisit ssid- prefix, suffix (or both) usage
    case WIFI_INTF_SSID_PREFIX: // char ssid_prefix[16];
            strncpy(g_wifi_intf_perm_vars[iface_enum].ssid_prefix, NET_WLAN_LINK_NAME, strlen(NET_WLAN_LINK_NAME));
            g_wifi_intf_perm_vars[iface_enum].ssid_prefix[MAX_SSID_NAME_LEN-1] = '\0';
        if (!all_vars) break;
#endif

    case WIFI_INTF_BSSID:
            memset(g_wifi_intf_perm_vars[iface_enum].BSSID, 0x0, BSSID_LENGTH);
        if (!all_vars) break;


    // intf
    case WIFI_INTF_SEC_MODE: // uint8_t sec_mode;
            g_wifi_intf_perm_vars[iface_enum].SecurityMode = OPEN_SYSTEM; // NONE
        if (!all_vars) break;

    case WIFI_INTF_ENCRYP_MODE: // uint8_t encryp_mode;
            g_wifi_intf_perm_vars[iface_enum].WPAEncryptionMode = WPA_TKIP_AND_AES;
        if (!all_vars) break;

    case WIFI_INTF_CHANNEL: // uint8_t channel;
            g_wifi_intf_perm_vars[iface_enum].Channel = DEFAULT_WIFI_CHAN;
        if (!all_vars) break;

    case WIFI_INTF_COMM_MODE: // uint8_t comm_mode;
            g_wifi_intf_perm_vars[iface_enum].CommunicationMode = AD_HOC;
        if (!all_vars) break;

#if 0 // tbd
    case WIFI_INTF_FILTER_COUNT: // uint8_t filter_count;
            g_wifi_intf_perm_vars[iface_enum].filter_count = 0;
        if (!all_vars) break;
#endif

    // wpa
    case WIFI_INTF_WPA_ENABLED:
            g_wifi_intf_perm_vars[iface_enum].WPAEncryptionMode = false;
        if (!all_vars) break;

    case WIFI_INTF_WPA_PASSPHRASE:
            memset(g_wifi_intf_perm_vars[iface_enum].PassPhrase, 0, MAX_PASS_PHRASE);
            strncpy((char*)g_wifi_intf_perm_vars[iface_enum].PassPhrase, DEFAULT_WPA_PASSPHRASE, strlen(DEFAULT_WPA_PASSPHRASE));
            
        if (!all_vars) break;

    // wep

    case WIFI_INTF_WEP_CUR_KEY:
            g_wifi_intf_perm_vars[iface_enum].CurrWepKey = 0;
        if (!all_vars) break;

    case WIFI_INTF_WEP_KEY_NUM: // int wep_key_num; (redunant w/KEY_CURRENT?)
            g_wifi_intf_perm_vars[iface_enum].CurrWepKey = 0;
        if (!all_vars) break;


    case WIFI_INTF_WEP_AUTH_MODE: // int wep_auth_mode;
            g_wifi_intf_perm_vars[iface_enum].WEPAuthMode = AUTH_OPEN;
        if (!all_vars) break;

    case WIFI_INTF_WEP_KEY_LEN: // int wep_key_len;
            g_wifi_intf_perm_vars[iface_enum].WEPKeyLength = 0;
        if (!all_vars) break;

#if 0
    // wep keys as array?
    case WIFI_INTF_WEP_KEY_1: // uint8_t wep_key_1[4]; // literal size?
            memset(g_wifi_intf_perm_vars[iface_enum].wep_key[0], 0, WEP_KEY_LEN); // 0-fill
        if (!all_vars) break;

    case WIFI_INTF_WEP_KEY_2: // uint8_t wep_key_2[4]; // literal size?
            memset(g_wifi_intf_perm_vars[iface_enum].wep_key[1], 0, WEP_KEY_LEN); // 0-fill
        if (!all_vars) break;

    case WIFI_INTF_WEP_KEY_3: // uint8_t wep_key_3[4]; // literal size?
            memset(g_wifi_intf_perm_vars[iface_enum].wep_key[2], 0, WEP_KEY_LEN); // 0-fill
        if (!all_vars) break;

    case WIFI_INTF_WEP_KEY_4: // uint8_t wep_key_4[4]; // literal size?
            memset(g_wifi_intf_perm_vars[iface_enum].wep_key[3], 0, WEP_KEY_LEN); // 0-fill
        if (!all_vars) break;

    case WIFI_INTF_RSSI_RANGE: // int rssi_range;
            g_wifi_intf_perm_vars[iface_enum].rssi_range = 0;
        if (!all_vars) break;
#endif

    default:
        // do nothing
        break;
    }

    WIFI_INTF_VAR_WIRELESS_UNLOCK();

    return(retval);
}

/*
 * Getters and setters for the shadow_wireless_info structure
 * (of type wifi_intf_wireless_info_t)
 */

#ifdef __linux__
error_type_t wifi_intf_get_var_comm_mode(uint8_t index, wifi_intf_comm_mode_t * comm_mode)
#else
error_type_t wifi_intf_get_var_comm_mode(wifi_intf_comm_mode_t * comm_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *comm_mode = (*shadow_wireless_info).CommunicationMode;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_comm_mode(uint8_t index, const wifi_intf_comm_mode_t comm_mode)
#else
error_type_t wifi_intf_set_var_comm_mode(const wifi_intf_comm_mode_t * comm_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).CommunicationMode = comm_mode;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_sec_mode(uint8_t index, wifi_intf_security_mode_t * sec_mode)
#else
error_type_t wifi_intf_get_var_sec_mode(wifi_intf_security_mode_t * sec_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *sec_mode = (*shadow_wireless_info).SecurityMode;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_sec_mode(uint8_t index, const wifi_intf_security_mode_t sec_mode)
#else
error_type_t wifi_intf_set_var_sec_mode(const wifi_intf_security_mode_t * sec_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).SecurityMode = sec_mode;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_wep_auth_mode(uint8_t index, wifi_intf_wep_auth_mode_t * wep_auth_mode)
#else
error_type_t wifi_intf_get_var_wep_auth_mode(wifi_intf_wep_auth_mode_t * wep_auth_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *wep_auth_mode = (*shadow_wireless_info).WEPAuthMode;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_wep_auth_mode(uint8_t index, const wifi_intf_wep_auth_mode_t wep_auth_mode)
#else
error_type_t wifi_intf_set_var_wep_auth_mode(const wifi_intf_wep_auth_mode_t * wep_auth_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).WEPAuthMode = wep_auth_mode;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_wpa_encryp_mode(uint8_t index, wifi_intf_wpa_encryp_mode_t * encryp_mode)
#else
error_type_t wifi_intf_get_var_wpa_encryp_mode(wifi_intf_wpa_encryp_mode_t * encryp_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *encryp_mode = (*shadow_wireless_info).WPAEncryptionMode;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_wpa_encryp_mode(uint8_t index, const wifi_intf_wpa_encryp_mode_t encryp_mode)
#else
error_type_t wifi_intf_set_var_wpa_encryp_mode(const wifi_intf_wpa_encryp_mode_t * encryp_mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).WPAEncryptionMode = encryp_mode;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_input_key(uint8_t index, wifi_intf_input_key_t * input_key)
#else
error_type_t wifi_intf_get_var_input_key(wifi_intf_input_key_t * input_key)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *input_key = (*shadow_wireless_info).InputKey;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_input_key(uint8_t index, const wifi_intf_input_key_t * input_key)
#else
error_type_t wifi_intf_set_var_input_key(const wifi_intf_input_key_t * input_key)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).InputKey = *input_key;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_channel(uint8_t index, uint8_t * chan)
#else
error_type_t wifi_intf_get_var_channel(uint8_t * chan)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *chan = (*shadow_wireless_info).Channel;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_channel(uint8_t index, uint8_t chan)
#else
error_type_t wifi_intf_set_var_channel(uint8_t chan)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    // TODO REVISIT Should we do bounds checking based on region?
    (*shadow_wireless_info).Channel = chan;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_ssid(uint8_t index, uint8_t * ssid, uint8_t len)
#else
error_type_t wifi_intf_get_var_ssid(uint8_t * ssid, uint8_t len)
#endif
{
    int bytes_copied = MIN(len, MAX_SSID_NAME_LEN);
    WIFI_INTF_VAR_WIRELESS_LOCK();
    memcpy(ssid, shadow_wireless_info->SSID, bytes_copied);
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_ssid(uint8_t index, const uint8_t * ssid, uint8_t len)
#else
error_type_t wifi_intf_set_var_ssid(const uint8_t * ssid, uint8_t len)
#endif
{
    int bytes_copied = MIN(len, MAX_SSID_NAME_LEN) - 1;
    if (bytes_copied > 0)
    {
        WIFI_INTF_VAR_WIRELESS_LOCK();
        memcpy(shadow_wireless_info->SSID, ssid, bytes_copied);
        shadow_wireless_info->SSID[bytes_copied] = 0x0;
        wifi_intf_nvram_block_write(index);
        startApplyInfoTimer();
        WIFI_INTF_VAR_WIRELESS_UNLOCK();
    }
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_passphrase(uint8_t index_unused, uint8_t * passphrase, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    memcpy(passphrase, shadow_wireless_info->PassPhrase, MIN(len, MAX_PASS_PHRASE));
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_passphrase(uint8_t index, const uint8_t * passphrase, uint8_t len)
{
    int bytes_copied = MIN(len, MAX_PASS_PHRASE);
    WIFI_INTF_VAR_WIRELESS_LOCK();
    memcpy(shadow_wireless_info->PassPhrase, passphrase, bytes_copied);
    shadow_wireless_info->PassPhrase[bytes_copied] = 0x0;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_curr_wep_key(uint8_t * key)
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *key = (*shadow_wireless_info).CurrWepKey;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_curr_wep_key(uint8_t key)
{
    error_type_t err = WIFI_INTF_OK;
    unsigned int if_offset = net_get_active_wifi_intf(); // revisit = net_iface_enum_from_if_index(0);
    WIFI_INTF_VAR_WIRELESS_LOCK();
    if(key < NUM_WEP_KEYS)
        (*shadow_wireless_info).CurrWepKey = key;
    else
        err = WIFI_INTF_FAIL;
    wifi_intf_nvram_block_write(if_offset);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return err;
}

error_type_t wifi_intf_get_var_wep_key(uint8_t index, uint8_t key_index, uint8_t * key, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    if(index < NUM_WEP_KEYS)
    {
#ifdef __linux__
        memcpy(key, g_wifi_intf_perm_vars[index].WEPKey[key_index], MIN(len, WEP_KEY_LEN));
#else
        memcpy(key, (*shadow_wireless_info).WEPKey[key_index], MIN(len, WEP_KEY_LEN));
#endif
        WIFI_INTF_VAR_WIRELESS_UNLOCK();
        return WIFI_INTF_OK;
    }
    else
    {
        WIFI_INTF_VAR_WIRELESS_UNLOCK();
        return WIFI_INTF_FAIL;
    }
}

error_type_t wifi_intf_set_var_wep_key(uint8_t index, uint8_t key_index, const uint8_t * key, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    if(index < NUM_WEP_KEYS)
    {
#ifdef __linux__
        memcpy(g_wifi_intf_perm_vars[index].WEPKey[key_index], key, MIN(len, WEP_KEY_LEN));
#else
        memcpy((*shadow_wireless_info).WEPKey[key_index], key, MIN(len, WEP_KEY_LEN));
#endif
        wifi_intf_nvram_block_write(index);
        startApplyInfoTimer();
        WIFI_INTF_VAR_WIRELESS_UNLOCK();
        return WIFI_INTF_OK;
    }
    else
    {
        WIFI_INTF_VAR_WIRELESS_UNLOCK();
        return WIFI_INTF_FAIL;
    }
}

#ifdef __linux__
error_type_t wifi_intf_get_var_wep_key_len(uint8_t index, uint8_t * key_len)
#else
error_type_t wifi_intf_get_var_wep_key_len(uint8_t * key_len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    *key_len = (*shadow_wireless_info).WEPKeyLength;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_wep_key_len(uint8_t index, uint8_t key_len)
#else
error_type_t wifi_intf_set_var_wep_key_len(uint8_t key_len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    (*shadow_wireless_info).WEPKeyLength = key_len;
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_wpa_pair_key(uint8_t index, uint8_t* pair_key, uint8_t len)
#else
error_type_t wifi_intf_get_var_wpa_pair_key(uint8_t * pair_key, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(pair_key, &g_wifi_intf_perm_vars[index].WPAPWK, MIN(len, PW_KEY_LEN));
#else
    memcpy(pair_key, (*shadow_wireless_info).WPAPWK, MIN(len, PW_KEY_LEN));
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_wpa_pair_key(uint8_t index, const uint8_t * pair_key, uint8_t len)
#else
error_type_t wifi_intf_set_var_wpa_pair_key(const uint8_t * pair_key, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(&g_wifi_intf_perm_vars[index].WPAPWK, pair_key, MIN(len, PW_KEY_LEN));
#else
    memcpy((*shadow_wireless_info).WPAPWK, pair_key, MIN(len, PW_KEY_LEN));
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_wpa_group_key(uint8_t index, uint8_t * group_key, uint8_t len)
#else
error_type_t wifi_intf_get_var_wpa_group_key(uint8_t * group_key, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(group_key, &g_wifi_intf_perm_vars[index].WPAGRPK, MIN(len, GW_KEY_LEN));
#else
    memcpy(group_key, (*shadow_wireless_info).WPAGRPK, MIN(len, GW_KEY_LEN));
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_wpa_group_key(uint8_t index, const uint8_t * group_key, uint8_t len)
#else
error_type_t wifi_intf_set_var_wpa_group_key(const uint8_t * group_key, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(&g_wifi_intf_perm_vars[index].WPAGRPK, group_key, MIN(len, GW_KEY_LEN));
#else
    memcpy((*shadow_wireless_info).WPAGRPK, group_key, MIN(len, GW_KEY_LEN));
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_is_wireless_hw_enabled(bool * hw_enabled)
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
    // revisit more than 1 wireless hw device present?
    *hw_enabled = (*shadow_wireless_info).IsWirelessHWEnabled;
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_is_enabled(uint8_t index, bool *enabled)
{
    return wifi_intf_get_var_is_wireless_hw_enabled(enabled);
}

error_type_t wifi_intf_set_var_is_wireless_hw_enabled(bool hw_enabled)
{
    int index = 0; // WIFI_IFACE_ID_STATION
    WIFI_INTF_VAR_WIRELESS_LOCK();
    while (index<WIFI_NUM_IFACES)
    {
        g_wifi_intf_perm_vars[index].IsWirelessHWEnabled = hw_enabled;
        wifi_intf_nvram_block_write(index);
        index++;
    }
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_is_enabled(uint8_t index, bool enabled)
{
    return wifi_intf_set_var_is_wireless_hw_enabled(enabled);
}

#ifdef __linux__
error_type_t wifi_intf_get_var_rf_tx_mode(uint8_t index, wifi_intf_rf_tx_mode_t * mode)
#else
error_type_t wifi_intf_get_var_rf_tx_mode(wifi_intf_rf_tx_mode_t * mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    *mode = g_wifi_intf_perm_vars[index].RFTxMode;
#else
    *mode = (*shadow_wireless_info).RFTxMode;
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_rf_tx_mode(uint8_t index, const wifi_intf_rf_tx_mode_t * mode)
#else
error_type_t wifi_intf_set_var_rf_tx_mode(const wifi_intf_rf_tx_mode_t * mode)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    g_wifi_intf_perm_vars[index].RFTxMode = *mode;
#else
    (*shadow_wireless_info).RFTxMode = *mode;
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_tx_antenna(uint8_t index, wifi_intf_antenna_t * tx_antenna)
#else
error_type_t wifi_intf_get_var_tx_antenna(wifi_intf_antenna_t * tx_antenna)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    *tx_antenna = g_wifi_intf_perm_vars[index].TxAntenna;
#else
    *tx_antenna = (*shadow_wireless_info).TxAntenna;
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_tx_antenna(uint8_t index, const wifi_intf_antenna_t * tx_antenna)
#else
error_type_t wifi_intf_set_var_tx_antenna(const wifi_intf_antenna_t * tx_antenna)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    g_wifi_intf_perm_vars[index].TxAntenna = *tx_antenna;
#else
    (*shadow_wireless_info).TxAntenna = *tx_antenna;
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_rx_antenna(uint8_t index, wifi_intf_antenna_t * rx_antenna)
#else
error_type_t wifi_intf_get_var_rx_antenna(wifi_intf_antenna_t * rx_antenna)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    *rx_antenna = g_wifi_intf_perm_vars[index].RxAntenna;
#else
    *rx_antenna = (*shadow_wireless_info).RxAntenna;
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_rx_antenna(uint8_t index, const wifi_intf_antenna_t * rx_antenna)
#else
error_type_t wifi_intf_set_var_rx_antenna(const wifi_intf_antenna_t * rx_antenna)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    g_wifi_intf_perm_vars[index].RxAntenna = *rx_antenna;
#else
    (*shadow_wireless_info).RxAntenna = *rx_antenna;
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_region_code(uint8_t index, uint8_t * region_code)
#else
error_type_t wifi_intf_get_var_region_code(uint8_t * region_code)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    *region_code = g_wifi_intf_perm_vars[index].RegionCode;
#else
    *region_code = (*shadow_wireless_info).RegionCode;
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_region_code(uint8_t index, uint8_t region_code)
#else
error_type_t wifi_intf_set_var_region_code(uint8_t region_code)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    g_wifi_intf_perm_vars[index].RegionCode = region_code;
#else
    (*shadow_wireless_info).RegionCode = region_code;
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_bssid_set(uint8_t index, bool * bssid_set)
#else
error_type_t wifi_intf_get_var_bssid_set(bool * bssid_set)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    *bssid_set = g_wifi_intf_perm_vars[index].BssidSet;
#else
    *bssid_set = (*shadow_wireless_info).BssidSet;
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_bssid_set(uint8_t index, bool bssid_set)
#else
error_type_t wifi_intf_set_var_bssid_set(bool bssid_set)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    g_wifi_intf_perm_vars[index].BssidSet = bssid_set;
#else
    (*shadow_wireless_info).BssidSet = bssid_set;
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_bssid(uint8_t index, uint8_t * bssid, uint8_t len)
#else
error_type_t wifi_intf_get_var_bssid(uint8_t * bssid, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(bssid, g_wifi_intf_perm_vars[index].BSSID, MIN(len, BSSID_LENGTH));
#else
    memcpy(bssid, (*shadow_wireless_info).BSSID, MIN(len, BSSID_LENGTH));
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_bssid(uint8_t index, const uint8_t * bssid, uint8_t len)
#else
error_type_t wifi_intf_set_var_bssid(const uint8_t * bssid, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(&g_wifi_intf_perm_vars[index].BSSID, bssid, MIN(len, BSSID_LENGTH));
#else
    memcpy(&(*shadow_wireless_info).BSSID, bssid, MIN(len, BSSID_LENGTH));
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_get_var_mac_addr(uint8_t index, uint8_t * mac, uint8_t len)
#else
error_type_t wifi_intf_get_var_mac_addr(uint8_t * mac, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(mac, g_wifi_intf_perm_vars[index].MacAddr, MIN(MAC_ADDR_LENGTH,len));
#else
    memcpy(mac, (*shadow_wireless_info).MacAddr, MIN(MAC_ADDR_LENGTH,len));
#endif
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef __linux__
error_type_t wifi_intf_set_var_mac_addr(uint8_t index, const uint8_t * mac, uint8_t len)
#else
error_type_t wifi_intf_set_var_mac_addr(const uint8_t * mac, uint8_t len)
#endif
{
    WIFI_INTF_VAR_WIRELESS_LOCK();
#ifdef __linux__
    memcpy(g_wifi_intf_perm_vars[index].MacAddr, mac, MIN(MAC_ADDR_LENGTH,len));
#else
    memcpy((*shadow_wireless_info).MacAddr, mac, MIN(MAC_ADDR_LENGTH,len));
#endif
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

/*
 * Getters and setters for the scan info structure
 */

error_type_t wifi_intf_get_var_scan_info(wifi_intf_scan_info_t * info)
{
    WIFI_INTF_VAR_SCAN_LOCK();
    memcpy(info, &shadow_wireless_scan_info, sizeof(wifi_intf_scan_info_t));
    WIFI_INTF_VAR_SCAN_UNLOCK();
    return WIFI_INTF_OK;
}

// Private method -- this is in our private wifi_intf.h, but not our
// public wifi_intf_vars.h
error_type_t wifi_intf_set_var_scan_info(wifi_intf_scan_info_t * info)
{
    WIFI_INTF_VAR_SCAN_LOCK();
    memcpy(&shadow_wireless_scan_info, info, sizeof(wifi_intf_scan_info_t));
    WIFI_INTF_VAR_SCAN_UNLOCK();
    return WIFI_INTF_OK;
}

/*
 * Other API
 */
error_type_t wifi_intf_get_var_is_wireless_scanning(bool * is_scanning)
{
    WIFI_INTF_VAR_MISC_LOCK();
    *is_scanning = shadow_misc_vars.is_wireless_scanning;
    WIFI_INTF_VAR_MISC_UNLOCK();
    return WIFI_INTF_OK;
}

// Private method -- this is in our private wifi_intf.h, but not our
// public wifi_intf_vars.h
error_type_t wifi_intf_set_var_is_wireless_scanning(bool * is_scanning)
{
    WIFI_INTF_VAR_MISC_LOCK();
    shadow_misc_vars.is_wireless_scanning = *is_scanning;
    WIFI_INTF_VAR_MISC_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_is_enabled(uint8_t index, bool* enabled)
{
    // requires hw enabled
    bool hw_enabled = false;
    error_type_t res = wifi_hw_get_var_is_enabled(index, &hw_enabled);
    if (res == OK && hw_enabled)
    {
        //ram *enabled = shadow_misc_vars.is_wireless_sta_enabled;
        *enabled = g_wifi_intf_perm_vars[index].IsEnabled; // nvram
    }
    else
    {
        *enabled = false;
    }
    DBG_VERBOSE("%s(%d) %d\n", __func__, index, *enabled);
    return res;
}

error_type_t wifi_intf_set_var_is_enabled(uint8_t index, bool enabled)
{
    WIFI_INTF_VAR_MISC_LOCK();
    g_wifi_intf_perm_vars[index].IsEnabled = enabled; // nvram
    shadow_misc_vars.is_wireless_sta_enabled = enabled; // ram
    wifi_intf_nvram_block_write(index);
    startApplyInfoTimer();
    WIFI_INTF_VAR_MISC_UNLOCK();
    DBG_VERBOSE("%s(%d) -> %d\n", __func__, index, shadow_misc_vars.is_wireless_sta_enabled);

    net_link_t* wifi_sta_link = net_link_lookup_by_name(NET_STA_LINK_NAME);
    if (!enabled)
         net_link_event_handler_cb(NET_LINK_EVENT_DOWN, wifi_sta_link);
    else net_link_event_handler_cb(NET_LINK_EVENT_UP, wifi_sta_link);

    return OK;
}

// determine 'active' wifi interface w/exclusion rules
//
unsigned int net_get_active_wifi_intf(void)
{
    // revisit wifi-direct overrides (WIFI_IFACE_ID_WFD)
    // revisit uap vs station priority? (more than 1 concurrently active)
    if (wifi_uap_running()) return WIFI_IFACE_ID_UAP;
    if (wifi_sta_running()) return WIFI_IFACE_ID_STATION;
    return 0; // default to base table if no wireless active (revisit ambiguous WIFI_IFACE_ID_UAP)
}

// general stubs

#define tbd DBG_ERR("%s() - revisit impl\n", __func__); return OK

error_type_t wifi_intf_get_var_wpa_encr_mode(uint8_t index, wifi_intf_wpa_encryp_mode_t* mode) { tbd; }
error_type_t wifi_intf_set_uap_set_var_wpa_encr_mode(uint8_t index, wifi_intf_wpa_encryp_mode_t mode) { tbd; }

error_type_t wifi_intf_get_var_wep_cur_key(uint8_t index, uint8_t* key) { tbd; }
error_type_t wifi_intf_set_var_wep_cur_key(uint8_t index, uint8_t key) { tbd; }

error_type_t wifi_intf_get_var_intf_channel(uint8_t index, uint8_t* channel) { tbd; }
error_type_t wifi_intf_set_var_intf_channel(uint8_t index, uint8_t channel) { tbd; }

error_type_t wifi_intf_get_var_mac_filter_mode(uint8_t index, wifi_intf_mac_filter_mode_t* mode) { tbd; }
error_type_t wifi_intf_set_var_mac_filter_mode(uint8_t index, wifi_intf_mac_filter_mode_t mode) { tbd; }

error_type_t wifi_intf_get_var_mac_filter_count(uint8_t index, uint8_t* mode) { tbd; }
error_type_t wifi_intf_set_var_mac_filter_count(uint8_t index, uint8_t mode) { tbd; }

error_type_t wifi_intf_get_var_intf_comm_mode(uint8_t index, wifi_intf_comm_mode_t* mode) { tbd; }
error_type_t wifi_intf_set_var_intf_comm_mode(uint8_t index, wifi_intf_comm_mode_t mode) { tbd; }

error_type_t wifi_intf_get_var_hw_locale(unsigned int index, uint8_t* mode) { tbd; }
error_type_t wifi_intf_set_var_hw_locale(unsigned int index, uint8_t mode) { tbd; }

// dbm stubs
error_type_t wifi_intf_get_var_rssi_dbm(uint8_t index, int32_t* rssi) { tbd; }; // signed?
error_type_t wifi_intf_set_var_rssi_dbm(uint8_t index, int32_t rssi) { tbd; }; // signed?

int rssi_dbm_to_signal_strength(int rssi) { tbd; }; // signed (range 1-5)

#ifdef __cplusplus
}
#endif

//eof
