/*
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include "ATypes.h"
#include "tx_api.h"
#include "nvram_api.h"
#include "utils.h"
#include "wifi_intf.h"
#include "wifi_intf_api.h"
#include "wifi_intf_vars.h"
#include "wifi_config.h"
#include "data_access_api.h"
#include "logger.h"
#include "dprintf.h"
#include "debug.h"
#include "string.h"
#include "lassert.h"

#ifdef HAVE_HOTSPOT_SUPPORT
#include "net_api.h"
void net_config_change_notify(unsigned int if_index);
#endif // HAVE_HOTSPOT_SUPPORT

/* static members */
static bool g_wifi_hw_nvram_is_registered = false;
static bool g_wifi_intf_nvram_is_registered[WIFI_NUM_IFACES] = {false, false, false, false};

static TX_MUTEX g_wifi_intf_var_mutex[WIFI_NUM_IFACES];
static TX_MUTEX g_wifi_hw_var_mutex;
static TX_MUTEX g_wifi_intf_var_scan_mutex;
static TX_MUTEX g_wifi_intf_var_misc_mutex;
static nvram_handle_t *g_wifi_hw_nvram_hndl;
static nvram_handle_t *g_wifi_intf_nvram_hndl[WIFI_NUM_IFACES];

static wifi_hw_info_t   shadow_wifi_hw_info;
static wifi_intf_info_t shadow_wifi_iface_info[WIFI_NUM_IFACES];
static wifi_intf_scan_info_t shadow_wireless_scan_info;

typedef struct
{
    bool is_wireless_scanning;
} MISC_WIRELESS_VARS;

static MISC_WIRELESS_VARS shadow_misc_vars;

static error_type_t wifi_hw_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);

static error_type_t wifi_intf_nvram_init_cb_sta(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);
static error_type_t wifi_intf_nvram_init_cb_uap(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);
static error_type_t wifi_intf_nvram_init_cb_wfd(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);
static error_type_t wifi_intf_nvram_init_cb_uap_pri0(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused);


static nvram_init_func_ptr wifi_intf_init_cb_funcs[WIFI_NUM_IFACES] = {wifi_intf_nvram_init_cb_sta, wifi_intf_nvram_init_cb_uap, wifi_intf_nvram_init_cb_wfd, wifi_intf_nvram_init_cb_uap_pri0};
static void (*notify_iface_vars_updated[WIFI_NUM_IFACES])(void) = {notify_wifi_sta_vars_changed, notify_wifi_uap_vars_changed, notify_wifi_wfd_vars_changed, notify_wifi_uap_pri0_vars_changed};

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

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



//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#ifdef DEBUG
#define WIFI_HW_VAR_WIRELESS_LOCK() \
        { \
            UINT threadx_rcode = tx_mutex_get(&g_wifi_hw_var_mutex, TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        } 
#else
#define WIFI_HW_VAR_WIRELESS_LOCK() \
        { \
            tx_mutex_get(&g_wifi_hw_var_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_HW_VAR_WIRELESS_UNLOCK() \
        { \
            UINT threadx_rcode = tx_mutex_put(&g_wifi_hw_var_mutex); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        }
#else
#define WIFI_HW_VAR_WIRELESS_UNLOCK() \
        { \
            tx_mutex_put(&g_wifi_hw_var_mutex); \
        }
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_WIRELESS_LOCK(iface_id) \
        { \
            UINT threadx_rcode = tx_mutex_get(&g_wifi_intf_var_mutex[iface_id], TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        } 
#else
#define WIFI_INTF_VAR_WIRELESS_LOCK(iface_id) \
        { \
            tx_mutex_get(&g_wifi_intf_var_mutex[iface_id], TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id) \
        { \
            UINT threadx_rcode = tx_mutex_put(&g_wifi_intf_var_mutex[iface_id]); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        }
#else
#define WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id) \
        { \
            tx_mutex_put(&g_wifi_intf_var_mutex[iface_id]); \
        }
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_SCAN_LOCK() \
        { \
            UINT threadx_rcode = tx_mutex_get(&g_wifi_intf_var_scan_mutex, TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        } 
#else
#define WIFI_INTF_VAR_SCAN_LOCK() \
        { \
            tx_mutex_get(&g_wifi_intf_var_scan_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_SCAN_UNLOCK() \
        { \
            UINT threadx_rcode = tx_mutex_put(&g_wifi_intf_var_scan_mutex); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        }
#else
#define WIFI_INTF_VAR_SCAN_UNLOCK() \
        { \
            tx_mutex_put(&g_wifi_intf_var_scan_mutex); \
        }
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_MISC_LOCK() \
        { \
            UINT threadx_rcode = tx_mutex_get(&g_wifi_intf_var_misc_mutex, TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        } 
#else
#define WIFI_INTF_VAR_MISC_LOCK() \
        { \
            tx_mutex_get(&g_wifi_intf_var_misc_mutex, TX_WAIT_FOREVER); \
        } 
#endif

#ifdef DEBUG
#define WIFI_INTF_VAR_MISC_UNLOCK() \
        { \
            UINT threadx_rcode = tx_mutex_put(&g_wifi_intf_var_misc_mutex); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
        }
#else
#define WIFI_INTF_VAR_MISC_UNLOCK() \
        { \
            tx_mutex_put(&g_wifi_intf_var_misc_mutex); \
        }
#endif

static error_type_t wifi_intf_nvram_block_write(uint8_t iface_id)
{
    error_type_t retval = WIFI_INTF_FAIL;

    if((iface_id < WIFI_NUM_IFACES) && g_wifi_intf_nvram_is_registered[iface_id])
    {
        retval = nvram_set_var(g_wifi_intf_nvram_hndl[iface_id], &shadow_wifi_iface_info[iface_id]);
        if(retval == OK)
        {
            retval = WIFI_INTF_OK;
        }
    }

    return retval;
}

static error_type_t wifi_hw_nvram_block_write(void)
{
    error_type_t retval = WIFI_INTF_FAIL;

    if(g_wifi_hw_nvram_is_registered)
    {
        retval = nvram_set_var(g_wifi_hw_nvram_hndl, &shadow_wifi_hw_info);
        if(retval == OK)
        {
            retval = WIFI_INTF_OK;
        }
    }

    return retval;
}

int
generate_random_passphrase(uint8_t * block, int blockLen)
{
    int available = 0;
    uint8_t value;

    srand(tx_time_get());

    while (blockLen > (available + 1))
    {
        value = rand() % 0x100;

        if ((value > 0x20) && (value < 0x7F)) {
            block[available++] = value;
        }
    }
    //block[available] = NULL;

    return (0);
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
void wifi_intf_var_init(void)
{
    uint32_t threadx_rcode;
    error_type_t retval;
    int iface_id;

    threadx_rcode = tx_mutex_create(&g_wifi_hw_var_mutex, "wlanHWvar", TX_INHERIT);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating wlan hw var mutex (threadx_rcode=0x%02x)\n", threadx_rcode);
        goto error;
    }

    for(iface_id = 0; iface_id < WIFI_NUM_IFACES; iface_id++)
    {
        threadx_rcode = tx_mutex_create(&(g_wifi_intf_var_mutex[iface_id]), "wlanIfaceVar", TX_INHERIT);
        if(threadx_rcode != TX_SUCCESS)
        {
            DBG_ERR("error creating wlan iface[%d] var mutex (threadx_rcode=0x%02x)\n", iface_id, threadx_rcode);
            goto error;
        }
    }

    threadx_rcode = tx_mutex_create(&g_wifi_intf_var_scan_mutex, "wlanScanVar", TX_INHERIT);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating wlan scan mutex (threadx_rcode=0x%02x)\n", threadx_rcode);
        goto error;
    }

    threadx_rcode = tx_mutex_create(&g_wifi_intf_var_misc_mutex, "wlanMiscVar", TX_INHERIT);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating wlan misc mutex (threadx_rcode=0x%02x)\n", threadx_rcode);
        goto error;
    }

    /*
     * Zero out the memory for the local static structures
     */
    memset((void*)&shadow_wifi_hw_info, 0, sizeof(shadow_wifi_hw_info));
    memset((void*)shadow_wifi_iface_info, 0, sizeof(shadow_wifi_iface_info));
    memset((void*)&shadow_wireless_scan_info, 0, sizeof(shadow_wireless_scan_info));
    memset((void*)&shadow_misc_vars, 0, sizeof(shadow_misc_vars));

    /*
     *  Ask for a discrete dynamic nvram blocks.  If it doesn't exist or is corrupted,
     *  wifi_intf_nvram_init_cb() will be called from nvram_variable_register() to initialize the block
     *  values.
     */
    retval = nvram_variable_register(&g_wifi_hw_nvram_hndl, WIFI_HW_VAR_ID, WIFI_HW_VARS_VERSION, sizeof(wifi_hw_info_t), wifi_hw_nvram_init_cb, NULL);
    if(retval == OK)
    {
        g_wifi_hw_nvram_is_registered = true;
    }
    else
    {
        // NVRAM is not available so just set defaults and operate on the local copies       
        wifi_hw_var_restore_defaults(WIFI_HW_VAR_ALL);
    }

    for(iface_id = 0; iface_id < WIFI_NUM_IFACES; iface_id++)
    {
        char var_id[5];

        ASSERT(iface_id < 10);  // We don't have enough chars for 10 ifaces

        // The WIFI_IFACE_VAR_ID changed from "WIF" to "WIF "
        strncpy(var_id, WIFI_IFACE_VAR_ID, sizeof(WIFI_IFACE_VAR_ID));
        var_id[3] = NULL;
        minSprintf(var_id, "%s%d", var_id, iface_id);

        retval = nvram_variable_register(&g_wifi_intf_nvram_hndl[iface_id], var_id, WIFI_INTF_VARS_VERSION, sizeof(wifi_intf_info_t), wifi_intf_init_cb_funcs[iface_id], NULL);
        if(retval == OK)
        {
            g_wifi_intf_nvram_is_registered[iface_id] = true;
        }
        else
        {
            // NVRAM is not available so just set defaults and operate on the local copies       
            wifi_intf_var_restore_defaults(iface_id, WIFI_INTF_VAR_ALL);
        }
    }

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
 *  \param[in] init_type type of handling needed:  nvram_init_type_t enum value from nvram_api.h
 *  
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return error_type_t
 *  \retval OK
 *
 **/ 
static error_type_t wifi_hw_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            WIFI_HW_VAR_WIRELESS_LOCK();
            wifi_hw_var_restore_defaults(WIFI_HW_VAR_ALL);
            ASSERT(sizeof(wifi_hw_info_t) <= init_loc_size);
            memcpy(init_loc, &shadow_wifi_hw_info, sizeof(wifi_hw_info_t));
            WIFI_HW_VAR_WIRELESS_UNLOCK();
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&shadow_wifi_hw_info, init_loc, sizeof(wifi_hw_info_t));
            break;
    }
    return ( OK );
}

/** 
 *  Initialize or repair a discrete dynamic NVRAM block
 *  
 *  This function is called by the dynamic NVRAM driver during the call to nvram_variable_register()
 *  either to initialize a new block of data or to repair an existing block which has a bad CRC,
 *  version discrepancy, or change in size.
 *  
 *  \param[in] init_type type of handling needed:  nvram_init_type_t enum value from nvram_api.h
 *  
 *  \param[in,out] init_loc pointer to existing block data on entry, destination for new
 *  block data to be returned 
 *  
 *  \return error_type_t
 *  \retval OK
 *
 **/ 
static error_type_t wifi_intf_nvram_init_cb(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, uint8_t iface_id)
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
            WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
            wifi_intf_var_restore_defaults(iface_id, WIFI_INTF_VAR_ALL);
            ASSERT(sizeof(wifi_intf_info_t) <= init_loc_size);
            memcpy(init_loc, &shadow_wifi_iface_info[iface_id], sizeof(wifi_intf_info_t));
            WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&shadow_wifi_iface_info[iface_id], init_loc, sizeof(wifi_intf_info_t));
            break;
    }
    return ( OK );
}

static error_type_t wifi_intf_nvram_init_cb_sta(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    return wifi_intf_nvram_init_cb(init_type, version, init_loc, init_loc_size, WIFI_IFACE_ID_STATION);
}

static error_type_t wifi_intf_nvram_init_cb_uap(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    return wifi_intf_nvram_init_cb(init_type, version, init_loc, init_loc_size, WIFI_IFACE_ID_UAP);
}

static error_type_t wifi_intf_nvram_init_cb_wfd(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    return wifi_intf_nvram_init_cb(init_type, version, init_loc, init_loc_size, WIFI_IFACE_ID_WFD);
}

static error_type_t wifi_intf_nvram_init_cb_uap_pri0(nvram_init_type_t init_type, uint16_t version, void *init_loc, uint32_t init_loc_size, void *unused)
{
    return wifi_intf_nvram_init_cb(init_type, version, init_loc, init_loc_size, WIFI_IFACE_ID_UAP_PRI0);
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
error_type_t wifi_hw_var_restore_defaults(wifi_hw_var_id_t var_id)
{
    bool all_vars;
    error_type_t retval = OK;

    all_vars = (var_id == WIFI_HW_VAR_ALL);

    WIFI_HW_VAR_WIRELESS_LOCK();

    switch(var_id)
    {
    case WIFI_HW_VAR_ALL:
        // clear all presets
        memset((void*)&shadow_wifi_hw_info, '\0', sizeof(wifi_hw_info_t));
        // fall through

    case WIFI_HW_ENABLED:
        shadow_wifi_hw_info.enabled = (UINT8)TRUE;
        if (!all_vars) break;
    case WIFI_HW_RFTX_MODE:
        shadow_wifi_hw_info.RFTxMode = RF_TX_MODE_NORMAL;
        if (!all_vars) break;
    case WIFI_HW_TX_ANTENNA:
        shadow_wifi_hw_info.TxAntenna.Type = ANT_TX;
        shadow_wifi_hw_info.TxAntenna.Num = ANT_DIVERSITY;
        if (!all_vars) break;
    case WIFI_HW_RX_ANTENNA:
        shadow_wifi_hw_info.RxAntenna.Type = ANT_RX;
        shadow_wifi_hw_info.RxAntenna.Num = ANT_DIVERSITY;
        if (!all_vars) break;
    case WIFI_HW_REGIONCODE:
        shadow_wifi_hw_info.RegionCode = REG_US;
        if (!all_vars) break;
    case WIFI_HW_WWSCONFIG:
        shadow_wifi_hw_info.WwsConfig = 0; //Disable World Wide Safe (WWS) by default
        if (!all_vars) break;    

    #ifdef HAVE_HOTSPOT_SUPPORT
    case WIFI_HOTSPOT_ENABLED:
        shadow_wifi_hw_info.hotspot_enabled = FALSE;
        if(!all_vars) break;
    #endif // HAVE_HOTSPOT_SUPPORT

    default:
        // do nothing
        break;
    }

    wifi_hw_nvram_block_write();
    WIFI_HW_VAR_WIRELESS_UNLOCK();

    wifi_hw_var_oem_restore_defaults(var_id);

    return(retval);
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
error_type_t wifi_intf_var_restore_defaults(uint8_t iface_id, wifi_intf_var_id_t var_id)
{
    bool all_vars;
    uint8_t MacAddr[MAC_ADDR_LENGTH];

    error_type_t retval = WIFI_INTF_OK;

    if (iface_id >= WIFI_NUM_IFACES)
    {
        return WIFI_INTF_FAIL;
    }

    all_vars = (var_id == WIFI_INTF_VAR_ALL);

    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

#ifdef WIFI_DIRECT_SUPPORT
    if ((iface_id == WIFI_IFACE_ID_WFD) && all_vars)
        wifi_wfd_clear_active_ssid_flag();
#endif    
        
    switch(var_id)
    {
    case WIFI_INTF_VAR_ALL:
        // clear all presets
        // preserve the MAC address though 
        memcpy (MacAddr, shadow_wifi_iface_info[iface_id].MacAddr, MAC_ADDR_LENGTH);
        memset((void*)&shadow_wifi_iface_info[iface_id], '\0', sizeof(wifi_intf_info_t));
        memcpy (shadow_wifi_iface_info[iface_id].MacAddr, MacAddr, MAC_ADDR_LENGTH);
        // fall through

    case WIFI_INTF_ENABLED:
        shadow_wifi_iface_info[iface_id].enabled = (UINT8)FALSE;
        if (!all_vars) break;
    case WIFI_INTF_SSID:
        memset(shadow_wifi_iface_info[iface_id].SSID, 0x0, MAX_SSID_NAME_LEN);
        if (!all_vars) break;
    case WIFI_INTF_SEC_MODE:
        if (iface_id == WIFI_IFACE_ID_WFD)
            shadow_wifi_iface_info[iface_id].SecurityMode = WPA2_PSK;
        else
            shadow_wifi_iface_info[iface_id].SecurityMode = OPEN_SYSTEM;
        if (!all_vars) break;
    case WIFI_INTF_WPA_ENCR_MODE:
        if (iface_id == WIFI_IFACE_ID_WFD)
            shadow_wifi_iface_info[iface_id].WPAEncryptionMode = WPA_AES;
        else
            shadow_wifi_iface_info[iface_id].WPAEncryptionMode = WPA_TKIP_AND_AES;
        if (!all_vars) break;
    case WIFI_INTF_WPA_PASSPHRASE:
        if (iface_id == WIFI_IFACE_ID_WFD)
            wifi_wfd_gen_random_passphrase(shadow_wifi_iface_info[iface_id].WPAPassPhrase, MAX_PASS_PHRASE);
        else
            memset(shadow_wifi_iface_info[iface_id].WPAPassPhrase, 0x0, MAX_PASS_PHRASE);
        if (!all_vars) break;
    case WIFI_INTF_WEP_AUTH_MODE:
        shadow_wifi_iface_info[iface_id].WEPAuthMode = AUTH_OPEN;
        if (!all_vars) break;
    case WIFI_INTF_WEP_KEY_LEN:
        shadow_wifi_iface_info[iface_id].WEPKeyLength = 0;
        if (!all_vars) break;
    case WIFI_INTF_WEP_KEYS:
        memset(shadow_wifi_iface_info[iface_id].WEPKey, 0x0, (NUM_WEP_KEYS * WEP_KEY_LEN));
        if (!all_vars) break;
    case WIFI_INTF_WEP_CUR_KEY:
        // WEPCurKey and WFDBSSRole are a union since WFD can't use WEP
        if (iface_id == WIFI_IFACE_ID_WFD)
            shadow_wifi_iface_info[iface_id].WFDBSSRole = WIFIDIRECT_GO_ROLE;
        else
            shadow_wifi_iface_info[iface_id].WEPCurKey = 0;
        if (!all_vars) break;
    case WIFI_INTF_CHANNEL:
        shadow_wifi_iface_info[iface_id].Channel = DEFAULT_WIFI_CHAN; 
        if (!all_vars) break;

    // perm vars only available on STA
    case WIFI_INTF_COMM_MODE:
        if (iface_id == WIFI_IFACE_ID_STATION)
            shadow_wifi_iface_info[iface_id].sta.CommunicationMode = INFRASTRUCTURE;
        else if (!all_vars) retval = WIFI_INTF_UNAVAILABLE;
        if (!all_vars) break;

    // perm vars only available on UAP or WFD
    case WIFI_INTF_SSID_PREFIX:
        if (iface_id != WIFI_IFACE_ID_STATION)
            memset(shadow_wifi_iface_info[iface_id].uap.SSIDPrefix, 0x0, MAX_SSID_NAME_LEN);
        else if (!all_vars) retval = WIFI_INTF_UNAVAILABLE;
        if (!all_vars) break;
    case WIFI_INTF_MAC_FILTER_MODE:
        if (iface_id != WIFI_IFACE_ID_STATION)
            shadow_wifi_iface_info[iface_id].uap.MACFilterMode = MAC_FILTER_MODE_DISABLED;
        else if (!all_vars) retval = WIFI_INTF_UNAVAILABLE;
        if (!all_vars) break;
    case WIFI_INTF_MAC_FILTER_LIST:
        if (iface_id != WIFI_IFACE_ID_STATION)
            memset(shadow_wifi_iface_info[iface_id].uap.MACFilterList, 0x0, sizeof(shadow_wifi_iface_info[iface_id].uap.MACFilterList));
        else if (!all_vars) retval = WIFI_INTF_UNAVAILABLE;
        if (!all_vars) break;

    default:
        // do nothing
        break;
    }

    if (retval == WIFI_INTF_OK)
    {
        wifi_intf_nvram_block_write(iface_id);
        if (iface_id == WIFI_IFACE_ID_STATION)
            notify_wifi_sta_vars_changed();
        if (iface_id == WIFI_IFACE_ID_UAP)
            notify_wifi_uap_vars_changed();
        if (iface_id == WIFI_IFACE_ID_UAP_PRI0)
            notify_wifi_uap_pri0_vars_changed();
#ifdef WIFI_DIRECT_SUPPORT
        if (iface_id == WIFI_IFACE_ID_WFD)
            notify_wifi_wfd_vars_changed();
#endif
    }
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);

    wifi_intf_var_oem_restore_defaults(iface_id, var_id);

    return(retval);
}

/*
 * Getters and setters for the shadow_wireless_info structure
 * (of type wifi_intf_wireless_info_t)
 */

error_type_t wifi_hw_get_var_is_enabled(bool * hw_enabled)
{
    *hw_enabled = shadow_wifi_hw_info.enabled;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_is_enabled(bool hw_enabled)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.enabled = hw_enabled;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_rf_tx_mode(wifi_intf_rf_tx_mode_t * mode)
{
    *mode = shadow_wifi_hw_info.RFTxMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_rf_tx_mode(const wifi_intf_rf_tx_mode_t mode)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.RFTxMode = mode;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_tx_antenna(wifi_intf_antenna_t * tx_antenna)
{
    *tx_antenna = shadow_wifi_hw_info.TxAntenna;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_tx_antenna(wifi_intf_antenna_t tx_antenna)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.TxAntenna = tx_antenna;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_rx_antenna(wifi_intf_antenna_t * rx_antenna)
{
    *rx_antenna = shadow_wifi_hw_info.RxAntenna;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_rx_antenna(wifi_intf_antenna_t rx_antenna)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.RxAntenna = rx_antenna;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_region_code(uint8_t * region_code)
{
    *region_code = shadow_wifi_hw_info.RegionCode;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_region_code(uint8_t region_code)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.RegionCode = region_code;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_get_var_wws_config(uint8_t * wws_config)
{
    *wws_config = shadow_wifi_hw_info.WwsConfig;
    return WIFI_INTF_OK;
}

error_type_t wifi_hw_set_var_wws_config(uint8_t wws_config)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.WwsConfig = wws_config;
    wifi_hw_nvram_block_write();
    notify_wifi_hw_vars_changed();
    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}

#ifdef HAVE_HOTSPOT_SUPPORT
error_type_t wifi_get_var_hotspot_enabled(bool * enabled)
{
    *enabled = shadow_wifi_hw_info.hotspot_enabled;
    return WIFI_INTF_OK;
}

error_type_t wifi_set_var_hotspot_enabled(bool enabled)
{
    WIFI_HW_VAR_WIRELESS_LOCK();
    shadow_wifi_hw_info.hotspot_enabled = enabled;
    wifi_hw_nvram_block_write();

    // TODO - revisit: consider making 'nat enabled' a field on the interface
    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    net_config_change_notify(if_index);

    WIFI_HW_VAR_WIRELESS_UNLOCK();
    return WIFI_INTF_OK;
}
#endif // HAVE_HOTSPOT_SUPPORT



error_type_t wifi_intf_get_var_is_enabled(uint8_t iface_id, bool * enabled)
{
    *enabled = shadow_wifi_iface_info[iface_id].enabled;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_is_enabled(uint8_t iface_id, const bool enabled)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].enabled = enabled;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    if (iface_id == WIFI_IFACE_ID_STATION)
    {
        if (enabled)
            wifi_intf_sta_notify_event( WIFI_STA_ENABLE_EVENT, 0);
        else
            wifi_intf_sta_notify_event( WIFI_STA_DISABLE_EVENT, 0);
    }

    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    
    //Notify the OEM layer of change in iface vars
    wifi_oem_notify_vars_changed(iface_id);        
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_ssid(uint8_t iface_id, uint8_t * ssid, uint8_t len)
{
    memcpy(ssid, shadow_wifi_iface_info[iface_id].SSID, MIN(len, MAX_SSID_NAME_LEN));
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_ssid(uint8_t iface_id, const uint8_t * ssid, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(shadow_wifi_iface_info[iface_id].SSID, ssid, MIN(len, MAX_SSID_NAME_LEN));
    shadow_wifi_iface_info[iface_id].SSID[MIN(len,MAX_SSID_NAME_LEN-1)] = 0x0;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);

    //Notify the OEM layer of change in iface vars
    wifi_oem_notify_vars_changed(iface_id);
    
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_sec_mode(uint8_t iface_id, wifi_intf_security_mode_t * sec_mode)
{
    *sec_mode = shadow_wifi_iface_info[iface_id].SecurityMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_sec_mode(uint8_t iface_id, wifi_intf_security_mode_t sec_mode)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].SecurityMode = sec_mode;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_wpa_encryp_mode(uint8_t iface_id, wifi_intf_wpa_encryp_mode_t * encryp_mode)
{
    *encryp_mode = shadow_wifi_iface_info[iface_id].WPAEncryptionMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wpa_encryp_mode(uint8_t iface_id, wifi_intf_wpa_encryp_mode_t encryp_mode)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].WPAEncryptionMode = encryp_mode;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_wpa_passphrase(uint8_t iface_id, uint8_t * passphrase, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(passphrase, shadow_wifi_iface_info[iface_id].WPAPassPhrase, MIN(len, MAX_PASS_PHRASE + 1));
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wpa_passphrase(uint8_t iface_id, const uint8_t * passphrase, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(shadow_wifi_iface_info[iface_id].WPAPassPhrase, passphrase, MIN(len, MAX_PASS_PHRASE + 1));
    shadow_wifi_iface_info[iface_id].WPAPassPhrase[MIN(len,MAX_PASS_PHRASE)] = 0x0;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_wep_auth_mode(uint8_t iface_id, wifi_intf_wep_auth_mode_t * wep_auth_mode)
{
    *wep_auth_mode = shadow_wifi_iface_info[iface_id].WEPAuthMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wep_auth_mode(uint8_t iface_id, wifi_intf_wep_auth_mode_t wep_auth_mode)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].WEPAuthMode = wep_auth_mode;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_wep_key_len(uint8_t iface_id, uint8_t * key_len)
{
    *key_len = shadow_wifi_iface_info[iface_id].WEPKeyLength;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wep_key_len(uint8_t iface_id, uint8_t key_len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].WEPKeyLength = key_len;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_wep_key(uint8_t iface_id, uint8_t index, uint8_t * key, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    if(index < NUM_WEP_KEYS)
    {
        memcpy(key, shadow_wifi_iface_info[iface_id].WEPKey[index], MIN(len, WEP_KEY_LEN + 1));
        WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
        return WIFI_INTF_OK;
    }
    else
    {
        WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
        return WIFI_INTF_FAIL;
    }
}

error_type_t wifi_intf_set_var_wep_key(uint8_t iface_id, uint8_t index, const uint8_t * key, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    if(index < NUM_WEP_KEYS)
    {
        if (len > 0)
        {
            memcpy(shadow_wifi_iface_info[iface_id].WEPKey[index], key, MIN(len, WEP_KEY_LEN + 1));
        }
        else
        {
            // Erase the key
            memset(shadow_wifi_iface_info[iface_id].WEPKey[index], 0, sizeof(shadow_wifi_iface_info[iface_id].WEPKey[index]));
        }
        // Be nice to higher layer code and make sure this is always null terminated (printable)
        shadow_wifi_iface_info[iface_id].WEPKey[index][MIN(len, WEP_KEY_LEN)] = '\0';
        wifi_intf_nvram_block_write(iface_id);
        notify_iface_vars_updated[iface_id]();
        WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
        return WIFI_INTF_OK;
    }
    else
    {
        WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
        return WIFI_INTF_FAIL;
    }
}

error_type_t wifi_intf_get_var_wep_cur_key(uint8_t iface_id, uint8_t * key)
{
    *key = shadow_wifi_iface_info[iface_id].WEPCurKey;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wep_cur_key(uint8_t iface_id, uint8_t key)
{
    error_type_t err = WIFI_INTF_OK;
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    if(key < NUM_WEP_KEYS)
        shadow_wifi_iface_info[iface_id].WEPCurKey = key;
    else
        err = WIFI_INTF_FAIL;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return err;
}

#ifdef WIFI_DIRECT_SUPPORT
error_type_t wifi_intf_get_var_wfd_bss_role(uint8_t iface_id, uint8_t * role)
{
    *role = shadow_wifi_iface_info[iface_id].WFDBSSRole;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_wfd_bss_role(uint8_t iface_id, uint8_t role)
{
    error_type_t err = WIFI_INTF_OK;
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].WFDBSSRole = role;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return err;
}
#endif

error_type_t wifi_intf_get_var_channel(uint8_t iface_id, uint8_t * chan)
{
    *chan = shadow_wifi_iface_info[iface_id].Channel;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_channel(uint8_t iface_id, uint8_t chan)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    // TODO REVISIT Should we do bounds checking based on region?
    shadow_wifi_iface_info[iface_id].Channel = chan;
    wifi_intf_nvram_block_write(iface_id);
    //notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_mac_addr(uint8_t iface_id, uint8_t * mac, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(mac, shadow_wifi_iface_info[iface_id].MacAddr, MIN(MAC_ADDR_LENGTH,len));
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_mac_addr(uint8_t iface_id, const uint8_t * mac, uint8_t len)
{
    if (memcmp(shadow_wifi_iface_info[iface_id].MacAddr, mac, MIN(MAC_ADDR_LENGTH,len)) != 0)
    {
        WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
        memcpy(shadow_wifi_iface_info[iface_id].MacAddr, mac, MIN(MAC_ADDR_LENGTH,len));
        wifi_intf_nvram_block_write(iface_id);

        notify_iface_vars_updated[iface_id]();
        WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    }
    return WIFI_INTF_OK;
}

// perm vars only available on STA

error_type_t wifi_intf_get_var_comm_mode(uint8_t iface_id, wifi_intf_comm_mode_t * comm_mode)
{
    *comm_mode = shadow_wifi_iface_info[iface_id].sta.CommunicationMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_comm_mode(uint8_t iface_id, wifi_intf_comm_mode_t comm_mode)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].sta.CommunicationMode = comm_mode;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}


// perm vars only available on UAP
error_type_t wifi_intf_get_var_ssid_prefix(uint8_t iface_id, uint8_t * prefix, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(prefix, shadow_wifi_iface_info[iface_id].uap.SSIDPrefix, MIN(len, MAX_SSID_NAME_LEN));
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_ssid_prefix(uint8_t iface_id, const uint8_t * prefix, uint8_t len)
{
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    memcpy(shadow_wifi_iface_info[iface_id].uap.SSIDPrefix, prefix, MIN(len, MAX_SSID_NAME_LEN));
    shadow_wifi_iface_info[iface_id].SSID[MIN(len,MAX_SSID_NAME_LEN-1)] = 0x0;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_mac_filter_mode(uint8_t iface_id, wifi_intf_mac_filter_mode_t * filter_mode)
{
    *filter_mode = shadow_wifi_iface_info[iface_id].uap.MACFilterMode;
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_set_var_mac_filter_mode(uint8_t iface_id, wifi_intf_mac_filter_mode_t filter_mode)
{
    uint8_t count;
    wifi_intf_get_var_mac_filter_count(iface_id, &count);

    if (count == 0 && filter_mode != MAC_FILTER_MODE_DISABLED)
    {
        //Dont enable filtering if there aren't any MACs listed in the table
        //For instance, this would keep us from turning on whitelist that blocks everyone
        return WIFI_INTF_OK;
    }

    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);
    shadow_wifi_iface_info[iface_id].uap.MACFilterMode = filter_mode;
    wifi_intf_nvram_block_write(iface_id);
    notify_iface_vars_updated[iface_id]();
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_var_mac_filter_add(uint8_t iface_id, char *mac_as_str)
{
    uint8_t  raw_mac[MAC_ADDR_LENGTH];
    int      i;
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

    if (hex_str_to_raw(mac_as_str, (char *)raw_mac, MAC_ADDR_LENGTH) == MAC_ADDR_LENGTH)
    {
        // Check if this entry already exists in the filter list
        for (i = 0; i < MAX_FILTERED_ADDRS; i++)
        {
            if (!buff_is_all_zero(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH))
            {
                if (memcmp(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], raw_mac, MAC_ADDR_LENGTH) == 0)
                {
                    //Entry exists, do nothing, just leave
                    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
                    return WIFI_INTF_OK;
                }
            }
        }

        // Find a blank spot and add it
        for (i = 0; i < MAX_FILTERED_ADDRS; i++)
        {
            if (buff_is_all_zero(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH))
            {
                memcpy(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], raw_mac, MAC_ADDR_LENGTH);
                wifi_intf_nvram_block_write(iface_id);
                notify_iface_vars_updated[iface_id]();
                WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
                return WIFI_INTF_OK;
            }
        }
    }

    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_FAIL; //Add not successful!
}

error_type_t wifi_intf_var_mac_filter_remove(uint8_t iface_id, char *mac_as_str)
{
    uint8_t  raw_mac[MAC_ADDR_LENGTH];
    int      i;
    bool     mac_removed = false;
    
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

    if (hex_str_to_raw(mac_as_str, (char *)raw_mac, MAC_ADDR_LENGTH) == MAC_ADDR_LENGTH)
    {
        // Find the mac and remove it
        for (i = 0; i < MAX_FILTERED_ADDRS; i++)
        {
            if (memcmp(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], raw_mac, MAC_ADDR_LENGTH) == 0)
            {
                memset(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], 0, MAC_ADDR_LENGTH);
                mac_removed = true; // Keep looking in case there are multiple copies
            }
        }
    }

    if (mac_removed)
    {
        wifi_intf_nvram_block_write(iface_id);
        notify_iface_vars_updated[iface_id]();
    }
    
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;
}

error_type_t wifi_intf_get_var_mac_filter_count(uint8_t iface_id, uint8_t *filter_count)
{
    int i;
    uint8_t count;

    if (filter_count == NULL)
        return WIFI_INTF_FAIL;
   
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

    count = 0;

    for (i = 0; i < MAX_FILTERED_ADDRS; i++ )
    {
        if (!buff_is_all_zero(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH))
            count++;
    }
    
    *filter_count = count;

    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;    
}    

error_type_t wifi_intf_var_mac_filter_flush(uint8_t iface_id)
{
    int i;
    bool mac_removed = false;
       
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

    for (i = 0; i < MAX_FILTERED_ADDRS; i++ )
    {
        if (!buff_is_all_zero(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH))
        {
            memset(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], 0, MAC_ADDR_LENGTH);
            mac_removed = true; //to indicate that there has been a change made to the list
        }
    }
    
    if (mac_removed)
    {
        shadow_wifi_iface_info[iface_id].uap.MACFilterMode = MAC_FILTER_MODE_DISABLED;
        wifi_intf_nvram_block_write(iface_id);
        notify_iface_vars_updated[iface_id]();
    }
    
    WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
    return WIFI_INTF_OK;    
}

error_type_t wifi_intf_get_var_mac_filter_list(uint8_t iface_id, uint8_t (*filter_list)[MAC_ADDR_LENGTH])
{
    int i, j;
       
    WIFI_INTF_VAR_WIRELESS_LOCK(iface_id);

    j = 0;
    for (i = 0; i < MAX_FILTERED_ADDRS; i++ )
    {
        if (!buff_is_all_zero(shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH))
        {
            memcpy(filter_list[j], shadow_wifi_iface_info[iface_id].uap.MACFilterList[i], MAC_ADDR_LENGTH);
            j++;
        }
    }

	WIFI_INTF_VAR_WIRELESS_UNLOCK(iface_id);
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
    *is_scanning = shadow_misc_vars.is_wireless_scanning;
    return WIFI_INTF_OK;
}

// Private method -- this is in our private wifi_intf.h, but not our
// public wifi_intf_vars.h
error_type_t wifi_intf_set_var_is_wireless_scanning(bool is_scanning)
{
    WIFI_INTF_VAR_MISC_LOCK();
    shadow_misc_vars.is_wireless_scanning = is_scanning;
    WIFI_INTF_VAR_MISC_UNLOCK();
    return WIFI_INTF_OK;
}

