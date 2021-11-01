/** @file wps_main.c
 *  @brief This file contains WPS application program entry function
 *           and functions for initialization setting.
 *  
 *  Copyright (C) 2003-2013, Marvell International Ltd.
 *  All Rights Reserved
 */

#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "wps_msg.h"
#include "wps_eapol.h"
#include "wps_util.h"
#include "wps_def.h"
#include "wps_wlan.h"
#include "wps_os.h"
#include "wps_version.h"
#include "wireless_copy.h"
#include "wlanInterface.h"
#include "encrypt.h"
#include "wps_l2.h"

#include "nvram_api.h"
#include "tx_api.h"
#include "platform.h"
#include "platform_api.h"
#include "logger.h"

#include "sha1.h"
#include "crypto.h"
#include "wifi_intf.h"
#include "wifi_config.h"
#include "wlan_hostcmd.h"
#include "wlan_wfd.h"
#include "uuid.h"
#include "model_info.h"

void do_gettimeofday(struct timeval *time);
int WlanShimCDeauth(void);

/********************************************************
        Local Variables
********************************************************/

/********************************************************
        Global Variables
********************************************************/
/** Global pwps information */
PWPS_INFO gpwps_info;
/** wps global */
WPS_DATA wps_global;
/** IE buffer index */
short ie_index = -1;
/** IE buffer index */
short ap_assocresp_ie_index = -1;
short assocreq_ie_index = -1;
short probe_ie_index = -1;
/** Configuration file for initialization */
char init_cfg_file[100];

/** WFD Config file for initial download */
char wfd_cfg_file[100];
/** Autonomous GO flag */
int auto_go = 0;
/** Interface switch required or not */
int switch_intf = 0;

/** PF specific global variables */
int proto_ext_test = 0, tx_frag_test = 0;

/** WPS PIN/PBC menu show only once */
int wps_method_menu_shown = 0;

/** Run wps apps in non interactive mode */
int wps_non_interactive = 0;

static uint32_t wps_uid = 0;

/********************************************************
        Local Functions
********************************************************/

/** 
 *  @brief Process initialization configuration file
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_config_read(PWPS_INFO pwps_info, WPS_DATA * wps_s, int role)
{
    int ret = WPS_STATUS_SUCCESS;
    MESSAGE_ENROLLEE_REGISTRAR *device_info = NULL;
    int uuid_length;
    char *tmpstr;

    ENTER();

    if (role == WPS_REGISTRAR)
    {
        wps_s->bss_type = BSS_TYPE_UAP;
        device_info     = &(pwps_info->registrar);
    }
    else if (role == WPS_ENROLLEE)
    {
        device_info     = &(pwps_info->enrollee);
    }
    else if (role == WFD_ROLE)
    {
        auto_go = 1;
        wps_s->bss_type = BSS_TYPE_UAP;
        device_info     = &(pwps_info->registrar);
    }
    else
    {
        XASSERT("Unsupported WPS role requested" == 0, role);
        return WPS_STATUS_FAIL;
    }

    pwps_info->role = role;

    // UUID for enrollee and registar
    uuid_length = WPS_UUID_MAX_LEN;
    get_device_uuid_binary((char *)pwps_info->enrollee.wps_uuid, &uuid_length);
    pwps_info->enrollee.wps_uuid_length = uuid_length;
    memcpy(&pwps_info->registrar.wps_uuid, &pwps_info->enrollee.wps_uuid, uuid_length);
    pwps_info->registrar.wps_uuid_length = uuid_length;

    //device_info->version = WPS_VERSION;
    device_info->version = WIFIDIRECT_WPS_VERSION_DEFAULT;

    platvars_get_usb_mfgstr(&tmpstr);
    strncpy((char *)device_info->manufacture, tmpstr, 64);
    device_info->manufacture_length = strlen((char *)device_info->manufacture);

    get_model_name(&tmpstr);
    strncpy((char *)device_info->device_name, tmpstr, 32);
    device_info->device_name_length = strlen((char *)device_info->device_name);

    get_model_name(&tmpstr);
    strncpy((char *)device_info->model_name, tmpstr, 32);
    device_info->model_name_length = strlen((char *)device_info->model_name);

    get_model_number(&tmpstr);
    strncpy((char *)device_info->model_number, tmpstr, 32);
    device_info->model_number_length = strlen((char *)device_info->model_number);

    platvars_get_prod_serialnum(&tmpstr);
    strncpy((char *)device_info->serial_number, tmpstr, 32);
    device_info->serial_number_length = strlen((char *)device_info->serial_number);

    // Let them know we support PBC
    device_info->config_methods = WIFIDIRECT_WPS_SPEC_CONFIG_METHODS_DEFAULT;

    // 2.4 GhZ -- 802.11 b/g
    device_info->rf_bands = WIFIDIRECT_WPS_RFBAND_DEFAULT;

    pwps_info->wps_msg_max_retry = 3;
    pwps_info->wps_msg_timeout = 15000;

    device_info->os_version = 0x80010001;

    pwps_info->wps_non_advertise = WPS_CANCEL;

    // Identify ourselves
    {
        char array[] = WIFIDIRECT_WPS_PRIMARY_DEVICE_TYPE_DEFAULT;
        memcpy(device_info->primary_device_type, array, sizeof(array));
    }

    pwps_info->primary_dev_category = device_info->primary_device_type[1];
    pwps_info->primary_dev_subcategory = device_info->primary_device_type[7];

    LEAVE();
    return ret;
}

/** 
 *  @brief Allocate buffer for WPS_INFO structure
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_private_info_allocate(PWPS_INFO * pwps_info)
{
    PWPS_INFO temp;

    ENTER();

    if (!(temp = os_malloc(sizeof(WPS_INFO)))) {
        wps_printf(MSG_ERROR, "Allocate buffer for WPS_INFO failed!\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    os_memset(temp, 0, sizeof(WPS_INFO));
    *pwps_info = temp;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Reset PIN value
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_reset_pin(PWPS_INFO pwps_info)
{
    int i;
    ENTER();

    /* Set Default PIN to all '0' */
    for (i = 0; i < 8; i++)
        pwps_info->PIN[i] = 0x30;
		
	pwps_info->PINLen = 8;
			
    LEAVE();
}

/** 
 *  @brief Set the method of Device Password ID (PIN or PBC)
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_set_device_password_id(PWPS_INFO pwps_info, char *pin)
{
    ENTER();

	pwps_info->wfd_pin_generated = WPS_CANCEL;
	
    if (pin == NULL)
    {
        // PUSH BUTTON
		wps_reset_pin(pwps_info);

        pwps_info->enrollee.device_password_id = DEVICE_PASSWORD_PUSH_BUTTON;
        pwps_info->registrar.device_password_id = DEVICE_PASSWORD_PUSH_BUTTON;
        pwps_info->enrollee.updated_device_password_id = DEVICE_PASSWORD_PUSH_BUTTON;
        pwps_info->registrar.updated_device_password_id = DEVICE_PASSWORD_PUSH_BUTTON;
        pwps_info->pbc_auto = WPS_SET;
    }
    else
    {
        // PIN

        pwps_info->enrollee.device_password_id  = DEVICE_PASSWORD_ID_PIN;
        pwps_info->registrar.device_password_id = DEVICE_PASSWORD_ID_PIN;
        pwps_info->enrollee.updated_device_password_id = DEVICE_PASSWORD_ID_PIN;
        pwps_info->registrar.updated_device_password_id = DEVICE_PASSWORD_ID_PIN;
        strcpy((char *)pwps_info->PIN, pin);
        pwps_info->PINLen = strlen(pin);

        if (pwps_info->role == WFD_ROLE)
        {
            if ((pwps_info->pin_generator != WPS_ENROLLEE)
                || (pwps_info->discovery_role != WPS_ENROLLEE)) {
                pwps_info->enrollee.updated_device_password_id = DEVICE_PASSWORD_REG_SPECIFIED;
            }
        }
    }

    pwps_info->pin_pbc_set = WPS_SET;

    LEAVE();
}

/** 
 *  @brief  Generate 16 Bytes Nonce
 *
 *  @param buf      A pointer to store nonce
 *  @return         None
 */
void
wps_generate_nonce_16B(u8 * buf)
{
    int i, randNum, seed;
    struct timeval now;

    ENTER();

    os_get_time(&now);
    seed = now.tv_sec | now.tv_usec;

    for (i = 0; i < 4; i++) {
        srand(seed);
        randNum = rand();
        memcpy(buf + i * 4, &randNum, sizeof(int));
        seed = seed * 1103515245 + 12345 * i;
    }

    LEAVE();
}


/** 
 *  @brief Generate UUID using local MAC address 
 *  
 *  @param mac_addr     A pointer to local MAC address
 *  @param wps_s        A pointer to UUID
 *  @return             None
 */
static void
wps_generate_uuid_using_mac_addr(const u8 * mac_addr, u8 * uuid)
{
    const u8 *addr[2];
    size_t len[2];
    u8 hash[SHA1_MAC_LEN];
    u8 nsid[16] = {
        0x52, 0x64, 0x80, 0xf8,
        0xc9, 0x9b,
        0x4b, 0xe5,
        0xa6, 0x55,
        0x58, 0xed, 0x5f, 0x5d, 0x60, 0x84
    };

    ENTER();
    addr[0] = nsid;
    len[0] = sizeof(nsid);
    addr[1] = mac_addr;
    len[1] = 6;
    sha1_vector(2, addr, len, hash);
    memcpy(uuid, hash, 16);

    /* Version: 5 = named-based version using SHA-1 */
    uuid[6] = (5 << 4) | (uuid[6] & 0x0f);

    /* Variant specified in RFC 4122 */
    uuid[8] = 0x80 | (uuid[8] & 0x3f);

    LEAVE();
}


/** 
 *  @brief Process WPS parameter initialization 
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_parameter_initialization(PWPS_INFO pwps_info, WPS_DATA * wps_s, int role)
{
    int ret = WPS_STATUS_SUCCESS;
    //int flags;
	bss_config_t bss_config;
    ENTER();

    /* Initial variable used in state machine */
    pwps_info->register_completed = WPS_CANCEL;
    pwps_info->registration_fail = WPS_CANCEL;
    pwps_info->registration_in_progress = WPS_CANCEL;
    pwps_info->auto_enrollee_in_progress = WPS_CANCEL;

    wps_printf(DEBUG_INIT, "Set registration in progress %d\n",
               pwps_info->registration_in_progress);
    pwps_info->wps_valid_data = WPS_CANCEL;
    pwps_info->id = 1;
    pwps_info->eap_msg_sent = 0;
    pwps_info->last_recv_wps_msg = -1;
    pwps_info->wps_msg_max_retry = WPS_MSG_RETRY_LIMIT;
    pwps_info->restart_link_lost = WPS_CANCEL;
    pwps_info->read_ap_config_only = WPS_CANCEL;
    pwps_info->pin_pbc_set = WPS_CANCEL;
    pwps_info->input_state = WPS_INPUT_STATE_METHOD;
    pwps_info->wps_device_state = SC_NOT_CONFIGURED_STATE;
    pwps_info->config_load_by_oob = WPS_CANCEL;
    pwps_info->wps_ap_setup_locked = WPS_CANCEL;
    pwps_info->enrollee_in_authorized_mac = WPS_SET;

    if (wps_s->bss_type == BSS_TYPE_STA)
        pwps_info->frag_thres = EAP_FRAG_THRESHOLD_DEF;
    else
        pwps_info->frag_thres = EAP_FRAG_THRESHOLD_MAX;

    pwps_info->proto_ext_test = proto_ext_test;
    pwps_info->tx_frag_test = tx_frag_test;

    if (pwps_info->tx_frag_test) {
        /* TX Frag Test - override Default value */
        pwps_info->frag_thres = EAP_FRAG_THRESHOLD_PF;
    }

    /* Reset ssid here, to read from file or driver */
    memset(&wps_s->current_ssid, 0, sizeof(WPS_SSID));
    memcpy(pwps_info->ethx, wps_s->ifname, 4);

    memset(&pwps_info->enrollee.wps_uuid, 0, WPS_UUID_MAX_LEN);
    memset(&pwps_info->registrar.wps_uuid, 0, WPS_UUID_MAX_LEN);
    pwps_info->registrar.wps_uuid_length = WPS_UUID_MAX_LEN;
    pwps_info->enrollee.wps_uuid_length = WPS_UUID_MAX_LEN;

    /* Read Device info from config file & generate UUID */
    wps_update_device_info(pwps_info, wps_s, role);

#ifdef WIFI_WPA_WPA2_ENTERPRISE_SUPPORT
    pwps_info->registrar.auth_type_flag = AUTHENTICATION_TYPE_ALL;
    pwps_info->enrollee.auth_type_flag = AUTHENTICATION_TYPE_ALL;
#else
    pwps_info->registrar.auth_type_flag = AUTHENTICATION_TYPE_ALL & ~(AUTHENTICATION_TYPE_WPA | AUTHENTICATION_TYPE_WPA2);
    pwps_info->enrollee.auth_type_flag = AUTHENTICATION_TYPE_ALL & ~(AUTHENTICATION_TYPE_WPA | AUTHENTICATION_TYPE_WPA2);
#endif
    pwps_info->registrar.encry_type_flag = ENCRYPTION_TYPE_ALL;
    pwps_info->enrollee.encry_type_flag = ENCRYPTION_TYPE_ALL;

    if (pwps_info->registrar.config_methods & CONFIG_METHOD_KEYPAD) {
        wps_printf(DEBUG_INIT, "Device is Rich UI device.\n");
        pwps_info->is_low_ui_device = WPS_CANCEL;
    } else {
        wps_printf(DEBUG_INIT, "Device is Low UI device.\n");
        pwps_info->is_low_ui_device = WPS_SET;
    }

    if (wps_s->bss_type == BSS_TYPE_UAP) {
        /* Get current uAP BSS configuration - for both Enrollee/Registrar
           modes */
        if (!wps_s->current_ssid.ssid_len) {
            ret = wps_load_uap_cred(wps_s, pwps_info);
            if (ret != WPS_STATUS_SUCCESS) {
        		wps_printf(MSG_ERROR, "BSS_TYPE_UAP Incorrect Network configuration. Quitting!\n");
                return ret;
            }
            if (pwps_info->role == WFD_ROLE) {
                /* For WFD, convert passpharase into PSK, and store it */
                /* PSK = ssid * passphrase using SHA1 */
                wlan_wfd_generate_psk(pwps_info);
            }
        }
    } else if (pwps_info->role == WPS_REGISTRAR) {
        /* Wireless STA Registrar */
        ret = wps_load_wsta_registrar_cred(wps_s, pwps_info);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "WPS_REGISTRAR Incorrect Network configuration. Quitting!\n");
            return ret;
        }
    }
    if ((wps_s->bss_type == BSS_TYPE_UAP) && (pwps_info->role == WPS_REGISTRAR)) {
        /* Get current BSS configuration */
        if (!wps_s->current_ssid.ssid_len)
            apcmd_get_bss_config(wps_s, &bss_config);
    }

    if (pwps_info->role == WPS_ENROLLEE) {
        wps_printf(MSG_INFO, "Role : WPS_ENROLLEE\n");
        pwps_info->state = WPS_STATE_A;

        /* Enrollee MAC Address */
        memcpy(pwps_info->enrollee.mac_address, wps_s->my_mac_addr, ETH_ALEN);
        wps_hexdump(MSG_MSGDUMP, "Enrollee MAC:", wps_s->my_mac_addr, sizeof(wps_s->my_mac_addr));

        /* Association State */
        pwps_info->enrollee.association_state = 0x01;

        /* Random Number */
        wps_generate_nonce_16B(pwps_info->enrollee.nonce);      /* Nonce */
        wps_generate_nonce_16B(pwps_info->enrollee.e_s1);       /* E-S1 */
        wps_generate_nonce_16B(pwps_info->enrollee.e_s2);       /* E-S1 */
        wps_generate_nonce_16B(pwps_info->enrollee.IV); /* IV */
    } else if (pwps_info->role == WPS_REGISTRAR) {
        wps_printf(MSG_INFO, "Role : WPS_REGISTRAR\n");

        /* check security mode */
        if ((AUTHENTICATION_TYPE_OPEN ==
             pwps_info->registrar.cred_data[0].auth_type) &&
            (ENCRYPTION_TYPE_NONE ==
             pwps_info->registrar.cred_data[0].encry_type)) {
            wps_printf(MSG_ERROR, "Unsecure AP Configuration!!\n");
        }
#if 0
        /* 
         * Keep inteface UP to receive the EAPOL from enrollee.
         */
        ret = wps_wlan_update_interface_flags(wps_s);
        if (ret != WPS_STATUS_SUCCESS)
            return ret;

        if (wps_s->bss_type == BSS_TYPE_UAP) {
            wps_print_uap_role_menu(pwps_info);
        } else {
            wps_print_registration_method_menu(pwps_info);
        }
#endif		
        /* Registrar MAC Address */
        memcpy(pwps_info->registrar.mac_address, wps_s->my_mac_addr, ETH_ALEN);

        /* Association State */
        pwps_info->registrar.association_state = 0x01;

        /* Random Number */
        wps_generate_nonce_16B(pwps_info->registrar.nonce);     /* Nonce */
        wps_generate_nonce_16B(pwps_info->registrar.r_s1);      /* R-S1 */
        wps_generate_nonce_16B(pwps_info->registrar.r_s2);      /* R-S2 */
        wps_generate_nonce_16B(pwps_info->registrar.IV);        /* IV */
    } else if (pwps_info->role == WFD_ROLE) {
        wps_printf(MSG_INFO, "Role : Wfd.\n");

        wps_print_registration_method_menu(pwps_info);

        if (!wps_s->current_ssid.ssid_len && wps_s->bss_type == BSS_TYPE_UAP) {
            if (auto_go)
                apcmd_get_bss_config(wps_s, &bss_config);
			
			load_cred_info(wps_s, pwps_info, &bss_config);
        }
#if 0
        /* 
         * Keep inteface UP to receive the EAPOL from enrollee.
         */
        if (wps_wlan_get_ifflags(&flags, wps_s->ifname) != 0 ||
            wps_wlan_set_ifflags(flags | IFF_UP, wps_s->ifname) != 0) {
            wps_printf(MSG_ERROR, "Could not set interface '%s' UP\n",
                       wps_s->ifname);
            return WPS_STATUS_FAIL;
        }
#endif
		if (auto_go)
			pwps_info->discovery_role = WPS_REGISTRAR;

        /* WFD data initiliazation */
        memset(&wps_s->wfd_data, 0, sizeof(WFD_DATA));
        wps_s->wfd_data.dev_index = -1;
        wps_s->wfd_data.dev_found = -1;

        memcpy(pwps_info->enrollee.mac_address, wps_s->my_mac_addr, ETH_ALEN);
        memcpy(pwps_info->registrar.mac_address, wps_s->my_mac_addr, ETH_ALEN);

        /* Association State */
        pwps_info->enrollee.association_state = 0x01;
        pwps_info->registrar.association_state = 0x01;

        /* Random Number */
        wps_generate_nonce_16B(pwps_info->enrollee.nonce);      /* Nonce */
        wps_generate_nonce_16B(pwps_info->enrollee.e_s1);       /* E-S1 */
        wps_generate_nonce_16B(pwps_info->enrollee.e_s2);       /* E-S1 */
        wps_generate_nonce_16B(pwps_info->enrollee.IV); /* IV */
        wps_generate_nonce_16B(pwps_info->registrar.nonce);     /* Nonce */
        wps_generate_nonce_16B(pwps_info->registrar.r_s1);      /* R-S1 */
        wps_generate_nonce_16B(pwps_info->registrar.r_s2);      /* R-S2 */
        wps_generate_nonce_16B(pwps_info->registrar.IV);        /* IV */
    }

    LEAVE();
    return ret;
}


/********************************************************
        Global Functions
********************************************************/

/** 
 *  @brief Get WPS_INFO structure
 *  
 *  @return             A pointer of global WPS_INFO structure
 */
PWPS_INFO
wps_get_private_info(void)
{
    return gpwps_info;
}

/** 
 *  @brief Get WPS settings from config file and generate UUID if not specified
 *  in config file 
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             A pointer of global WPS_INFO structure
 */
int
wps_update_device_info(PWPS_INFO pwps_info, WPS_DATA * wps_s, int role)
{

    int ret = WPS_STATUS_SUCCESS;
    u8 zero_uuid[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    ENTER();

    memset(pwps_info->registrar.wps_uuid, 0, WPS_UUID_MAX_LEN);
    memset(pwps_info->enrollee.wps_uuid, 0, WPS_UUID_MAX_LEN);

    ret = wps_config_read(pwps_info, wps_s, role);

    if (!(memcmp(pwps_info->enrollee.wps_uuid, zero_uuid, WPS_UUID_MAX_LEN)) ||
        (!memcmp(pwps_info->registrar.wps_uuid, zero_uuid, WPS_UUID_MAX_LEN))) {
        wps_printf(DEBUG_INIT,
                   "UUID not specified in config file. Generating UUID using local MAC address!");
        wps_generate_uuid_using_mac_addr(wps_s->my_mac_addr,
                                         pwps_info->registrar.wps_uuid);
        memcpy(&pwps_info->enrollee.wps_uuid, &pwps_info->registrar.wps_uuid,
               WPS_UUID_MAX_LEN);
        wps_hexdump(DEBUG_INIT, "pwps_info->registrar.wps_uuid",
                    pwps_info->registrar.wps_uuid, WPS_UUID_MAX_LEN);
    }

    return ret;
    LEAVE();
}

/** 
 *  @brief Process clear instance flag
 *  
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_clear_running_instance(void)
{
#if 0
    FILE *fp = NULL;
    int ret = WPS_STATUS_SUCCESS;
    char run_file[50];

    strcpy(run_file, FILE_WPSRUN_CONFIG_NAME);
    strcat(run_file, wps_global.ifname);

    fp = fopen(run_file, "w+");
    if (!fp) {
        wps_printf(MSG_ERROR, "ERROR - Fail to open file %s !\n", run_file);
        ret = WPS_STATUS_FAIL;
    } else {
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%d\n", 0);
        fclose(fp);
    }

    return ret;
#else
    return WPS_STATUS_SUCCESS;
#endif
}

/** 
 *  @brief WPS Initialization
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_init(WPS_DATA * wps_s, int role)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    wps_s->userAbort = WPS_CANCEL;

    ret = wps_private_info_allocate(&gpwps_info);

    ret = wps_parameter_initialization(gpwps_info, wps_s, role);

    LEAVE();
    return ret;
}

/** 
 *  @brief WPS uAP initialization
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_init_uap_common(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
    /* For UAP mode */
    wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
    pwps_info->mode = IEEE80211_MODE_INFRA;

    if (ie_index == -1) {
        wps_printf(DEBUG_INIT, "GET_WPS_IE\n");
        /* Get the free IE buffer index */
        ret = wps_wlan_ie_config(GET_WPS_IE, &ie_index);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "WPS Registrar failed to start\n");
            goto _exit_;
        }
    }

    /* 
     * Send APCMD_SYS_CONFIGURE command to set WPS IE
     */
    wps_printf(DEBUG_INIT, "SET_WPS_IE\n");
    ret = wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_IE, &ie_index);
    if (ret != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WPS Registrar failed to start\n");
        goto _exit_;
    }
    if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {

        if (ap_assocresp_ie_index == -1) {
            /* Get the free IE buffer index */
            ret = wps_wlan_ie_config(GET_WPS_IE, &ap_assocresp_ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                printf("WPS Registrar failed to start.\n");
                goto _exit_;
            }
        }

        ret =
            wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                               &ap_assocresp_ie_index);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
        }
    }

    LEAVE();
  _exit_:
    return ret;
}

/** 
 *  @brief WPS STA initialization
 *  
 *  @param wps_s        A pointer to global WPS structure
 * 
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_init_sta_common(WPS_DATA * wps_s)
{
    int ret = WPS_STATUS_SUCCESS;
    ENTER();
    /* Check if the WLAN device is in PS */
    wps_wlan_get_power_mode(&gpwps_info->ps_saved);

    if (gpwps_info->ps_saved == 1) {
        /* Exit Power save */
        wps_wlan_set_power_mode(0);
    }

    LEAVE();
    return ret;
}
/** 
 *  @brief Process WPS start up operations
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_start(WPS_DATA * wps_s, char *ssid, char *pin)
{
    int ret = WPS_STATUS_SUCCESS;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
	
    ENTER();

    if (wps_s->bss_type == BSS_TYPE_STA) {
        if ((ret = wps_init_sta_common(wps_s)) == WPS_STATUS_FAIL) {
            printf("\nWPS: STA initialization failure.\n");
            goto done;
        }
    }
  
      /* Select PIN/PBC */
    wps_set_device_password_id(gpwps_info, pin);

    if (gpwps_info->role == WPS_ENROLLEE) {
        // Clear out the encryption scheme currently in use so that we don't
        // start trying to associate with shared WEP, or something similar.
        WlanClearEncryption();

        // Run initial code the same way for PIN and PBC -- once AP is found they
        // will handle it differently.  gpwps_info lets the code know whether to
        // do PIN or PBC from this point.
        ret = wps_enrollee_start(gpwps_info, wps_s, ssid);
    } else if (gpwps_info->role == WPS_REGISTRAR) {
        /* Registrar Process */
        ret = wps_registrar_start(gpwps_info, wps_s);
    } else if (gpwps_info->role == WFD_ROLE) {
        if (auto_go) {
            if ((ret =
                 wps_init_uap_common(gpwps_info,
                                     wps_s)) == WPS_STATUS_FAIL) {
                wps_printf(MSG_INFO, "\nWFD: Auto-GO initialization failure.\n");
                goto done;
            }
        }
        wps_set_device_password_id(gpwps_info, pin);
        /* Update beacon IE, probe response IE for this device password */
        wps_wlan_update_password_ie_config();

        if (auto_go) {
            /* set up intended inteface address and start BSS */
            if (memcmp
                (gpwps_info->wfd_intended_addr, "\x00\x00\x00\x00\x00\x00",
                 ETH_ALEN)) {
                if (wlan_set_intended_mac_addr
                    (gpwps_info->wfd_intended_addr)) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to set mac address.\n");
                }
                memcpy(gpwps_info->registrar.mac_address,
                       gpwps_info->wfd_intended_addr, ETH_ALEN);
            }

            ret =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_ACTIVE_IE, &ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR,
                           "WPS Registrar failed to update IE's\n");
            }

            gpwps_info->wps_session_active = WPS_SET;
            if (apcmd_start_bss(wps_s) == WPS_STATUS_FAIL) {
                wps_printf(MSG_INFO,
                    "GO AP is already running, Interface address not set.\n");
            }
        }

        /* Discovery Process */
        /* 
         * Discovery is triggred by waiting for peer detected
         * events from FW and connecting to one of the
         * peers.
         */
        wps_printf(MSG_INFO, "\nWFD: Waiting for discovery events...\n");

        /* 
         * Scan phase before find phase
         */
        /** Start peer ageout timer if not auto_go */
        if (!auto_go) {
            pwfd_data->default_scan = 1;
            wps_wlan_scan(wps_s, ssid);
            pwfd_data->default_scan = 0;
            wfd_start_peer_ageout_timer(pwfd_data);
        }
    }
  done:
    LEAVE();
    return ret;
}

void
wps_print_uap_role_menu(PWPS_INFO pwps_info)
{

    ENTER();
#if 0
    /** print menu only if interactive mode */
    if (!wps_non_interactive) {
        printf("\n");
        printf("Select uAP role\n");
        printf("[1]. AP Enrollee\n");
        printf("[2]. AP Registrar\n");
        printf("Enter Selection : ");
        fflush(stdout);
    }
#endif	
    LEAVE();
}

/** 
 *  @brief Print WPS Registration method menu
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void
wps_print_registration_method_menu(WPS_INFO * pwps_info)
{
#if 0
    int i = 1;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    /* No need to print menu for uAP-Enrollee */
    if (pwps_info->role == WPS_ENROLLEE && wps_s->bss_type == BSS_TYPE_UAP)
        return;

    /* No need to print menu for STA-Registrar */
    if (pwps_info->role == WPS_REGISTRAR && wps_s->bss_type == BSS_TYPE_STA)
        return;

    ENTER();
    /** print menu only if interactive mode */
    if (!wps_non_interactive) {
        printf("\n");
        printf("Select Device Password ID Option\n");
        printf("[0]. PIN\n");
        if (!
            (pwps_info->role == WPS_ENROLLEE && wps_s->bss_type == BSS_TYPE_UAP)
            && !(pwps_info->role == WPS_REGISTRAR &&
                 wps_s->bss_type == BSS_TYPE_STA)) {
            printf("[1]. PBC\n");
            i++;
        }
        if (pwps_info->role == WPS_ENROLLEE && wps_s->bss_type != BSS_TYPE_UAP) {
            printf("[2]. PBC Auto\n");
            i++;
        }
        wps_method_menu_shown = 1;
        if (pwps_info->role == WPS_REGISTRAR && wps_s->bss_type == BSS_TYPE_UAP) {
            printf("You may directly input PIN value.\n");
        }
        printf("Enter Selection : ");
        fflush(stdout);
    }
#endif	
    LEAVE();
}

/** 
 *  @brief Process WPS stop operations
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_stop(WPS_DATA * wps_s)
{
    PWPS_INFO pwps_info;
    int ret = WPS_STATUS_SUCCESS;
    u8 bssid_get[ETH_ALEN];

    ENTER();

    pwps_info = wps_get_private_info();

    /* Disconnect if still connect to AP */
    memset(bssid_get, 0x00, ETH_ALEN);
    wps_wlan_get_wap(bssid_get);
    if ((memcmp(bssid_get, wps_s->current_ssid.bssid, ETH_ALEN) == 0)) {
    }

    if (wps_s->bss_type == BSS_TYPE_STA) {

        /* If the WLAN device was in PS then put it back to PS */
        if (gpwps_info->ps_saved == 1) {
            /* Enter Power save */
            wps_wlan_set_power_mode(1);
        }

    }
    LEAVE();
    return ret;
}

/** 
 *  @brief Process WPS free operations
 *  
 *  @return             None
 */
void
wps_deinit(int role)
{
    ENTER();

    if (gpwps_info == NULL) {
        // disassociate
        WlanShimCDeauth();

        LEAVE();
        return;
    }

    os_free(gpwps_info);
    gpwps_info = NULL;

    if (role == WPS_ENROLLEE)
    {
    	// disassociate
    	WlanShimCDeauth();
    }

    LEAVE();
}

/** 
 *  @brief Process user abort function
 *  
 *  @param fd           File descriptor for reading
 *  @param context      A pointer to user private information 
 *  @return             None
 */
void
wps_user_abort_handler(int fd, void *context)
{
    PWPS_INFO pwps_info;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
#if 0
    char inp[3];
#endif

    ENTER();

    pwps_info = (PWPS_INFO) context;

#if 0
    fgets(inp, 3, stdin);
    fflush(stdin);
    if (strlen(inp) != 0) {
        if (inp[0] == 'q' || inp[0] == 'Q') {
#endif
            wps_printf(MSG_INFO, "\nUser Abort.");
            wps_printf(MSG_INFO, "WPS Registration Protocol Not Completed !\n");

            wps_s->userAbort = WPS_SET;
            pwps_info->registration_fail = WPS_SET;

            /* Cancel WPS Registration Timer */
            wps_printf(MSG_INFO, "Cancelling registration timer!\n");
            wps_cancel_timer(wps_registration_time_handler, pwps_info);

            /* Cancel packet Tx timer */
            if (pwps_info->set_timer == WPS_SET) {
                wps_cancel_timer(wps_txTimer_handler, pwps_info);
                pwps_info->set_timer = WPS_CANCEL;
                pwps_info->wps_msg_resent_count = 0;
            }

            /* Unregister user abort handler */
            wps_unregister_rdsock_handler(STDIN_FILENO);

            /* Shutdown the main processing loop */
            wps_main_loop_shutdown();
#if 0
        }
    }
#endif

    updateWPSOutcome(WPS_STATE_CANCELLED);

    LEAVE();
}



/** 
 *  @brief WPS Registrar Shutdown function
 *  
 *  @param              None
 *  @return             None
 */
void
wps_registrar_shutdown()
{
    ENTER();

    /* Clear WPS IE */
    if (ie_index != -1) {
        wps_wlan_ie_config(CLEAR_WPS_IE, &ie_index);
        ie_index = -1;
    }
    /* shutdown the main processing loop */
    wps_main_loop_shutdown();

    /* Unregister user abort handler */
    wps_unregister_rdsock_handler(STDIN_FILENO);

    LEAVE();
}

/** 
 *  @brief C main entry function
 *  @param role             Role - Registrar or Enrollee
 *  @param iface            Interface
 *  @param pin       		pin
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_main(int role, char *iface, char *ssid, char *pin)
{
    int exitcode, ret;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    PWPS_INFO pwps_info;
    bss_config_t bss;

    wifi_intf_disallow_pwr_mgr(&wps_uid);

    exitcode = WPS_STATUS_SUCCESS;

    updateWPSOutcome(WPS_STATE_PROCESSING);

    strncpy(wps_s->ifname, iface, 64 + 1);
    wps_printf(MSG_INFO, "Initializing interface '%s'\n", wps_s->ifname);

    /* 
     * 1. Initialize L2 packet interface for receiving EAP packet.
     * 2. Get L2 MAC address and store to application global structure
     * 3. Create socket for wlan driver ioctl
     */
    if (wps_loop_init(wps_s, role) != 0) {
        exitcode = WPS_STATUS_FAIL;
        wps_printf(MSG_ERROR, "ERROR - Fail to initialize layer 2 socket !\n");
        goto exit_main;
    }

    if (exitcode == WPS_STATUS_SUCCESS)
        exitcode = wps_set_signal_handler();

    memset(&bss, 0, sizeof(bss_config_t));
    if (wps_s->bss_type == BSS_TYPE_UAP) {
        /* Read current params for default values */
        if ((ret = apcmd_get_bss_config(wps_s, &bss)) != WPS_STATUS_SUCCESS) {
            goto exit_main;
        }
        if (bss.bcast_ssid_ctl == WPS_CANCEL) {
            wps_printf(MSG_ERROR, 
                    "Network is configured as hidden/SSID broadcast disabled.\n");
            wps_printf(MSG_ERROR,
			        "WPS registration will not continue!!\n\n");
            goto exit_main;
        }
        if (bss.filter.filter_mode != WPS_CANCEL) {
            wps_printf(MSG_ERROR, 
                    "Mac Address filtering enabled. WPS registration will not continue!!\n\n");
            goto exit_main;
        }
    }

    /* 
     * 1. Allocate memory for WPS state machine
     * 2. Read the config file
     * 3. Initialize parameter needed in state machine
     */
    if (wps_init(wps_s, role) != WPS_STATUS_SUCCESS) {
        exitcode = WPS_STATUS_FAIL;
        wps_printf(MSG_ERROR, "ERROR - Failed to initialize WPS !\n");
        goto exit_main;
    }

    /* Get the free IE buffer index */
    exitcode = wps_wlan_ie_config(GET_WPS_IE, &ie_index);
    if (exitcode != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WPS IE failed to get\n");
        goto exit_main;
    }
    /* 
     * Send APCMD_SYS_CONFIGURE command to set WPS IE
     */
    wps_wlan_ie_config(SET_WPS_IE, &ie_index);

    pwps_info = wps_get_private_info();

    if (pwps_info->role == WFD_ROLE) {
#if 0
        if (strlen(wfd_cfg_file)) {
            wps_printf(MSG_INFO, "WFD Config file '%s'", wfd_cfg_file);
            if (wfdcmd_config_download(wfd_cfg_file) != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR,
                           "ERROR - Fail to download WFD configuration.!\n");
                goto exit;
            }
        }
#endif
        if (wps_s->bss_type == BSS_TYPE_STA) {
            if (wfdcmd_start_find_phase() != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR, "ERROR - Fail to initialize WFD!\n");
                goto exit_main;
            }
        } else if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (auto_go) {
                if (wfdcmd_start_group_owner() != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_ERROR,
                               "ERROR - Fail to initialize WFD GO!\n");
                    goto exit_main;
                }
            } else {
                if (wfdcmd_start_find_phase() != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_ERROR, "ERROR - Fail to initialize WFD!\n");
                    goto exit_main;
                }
            }
        }

        /* 
         * Initialize netlink socket interface for receiving events.
         */
        if (wps_event_init(wps_s) != 0) {
            exitcode = WPS_STATUS_FAIL;
            wps_printf(MSG_ERROR,
                       "ERROR - Fail to initialize event socket !\n");
            goto exit_main;
        }
    }
#if 0
    // Register user abort handler
    wps_register_rdsock_handler(STDIN_FILENO, wps_user_abort_handler,
                                pwps_info);
#endif 

    /* In case of WPSE Auto mode; this menu is already shown. So no need to
       show again. */
    if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {

        wps_set_device_password_id(gpwps_info, pin);
    }

    /* 
     * 1. Scan and associate to AP if role is Enrollee
     * 2. Wait for WPS connection if role is Registrar
     */
    if ((exitcode = wps_start(wps_s, ssid, pin)) == WPS_STATUS_SUCCESS) {

        /* Enable main loop procedure */
        wps_main_loop_enable();

        /* Main loop for socket read and timeout function */
        wps_main_loop_proc();

        if (pwps_info->wps_valid_data != WPS_SET)
            exitcode = WPS_STATUS_FAIL;
        else if (pwps_info->role == WPS_REGISTRAR && wps_s->userAbort != WPS_SET) {
            if (pwps_info->set_timer == WPS_SET) {
                wps_cancel_timer(wps_txTimer_handler, pwps_info);
                pwps_info->set_timer = WPS_CANCEL;
                pwps_info->wps_msg_resent_count = 0;
            }

            /* Update Beacon, probe response IE ; Remove password */
            wps_wlan_ie_config(SET_WPS_IE, &ie_index);
        }
    }
    else
    {
        if (exitcode == WPS_SET)
        {
            exitcode = WPS_STATUS_SUCCESS;
        }
    }

    if (wps_s->bss_type == BSS_TYPE_STA) {
        /* Disconnect with AP if needed */
        wps_stop(wps_s);
    } else if (wps_s->bss_type == BSS_TYPE_UAP) {
        /* Clear WPS IE */
        if (ie_index != -1)
            wps_wlan_ie_config(CLEAR_WPS_IE, &ie_index);
        /* Configure session start IEs for Assoc response */
        if ((pwps_info->role == WPS_REGISTRAR &&
             pwps_info->registrar.version >= WPS_VERSION_2DOT0) ||
            (pwps_info->role == WPS_ENROLLEE &&
             pwps_info->enrollee.version >= WPS_VERSION_2DOT0)) {
            if (ap_assocresp_ie_index != -1) {
                ret =
                    wps_wlan_ie_config(CLEAR_AR_WPS_IE, &ap_assocresp_ie_index);
                if (ret != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
                }
            }
        }
    }
    /* free functions */
    if (pwps_info->role == WFD_ROLE) {
        /* Clear WPS IE */
        if (ie_index != -1)
            wps_wlan_ie_config(CLEAR_WPS_IE, &ie_index);
    }

    /* For unconditionally, the function has checks for NULL, 0 */
	wps_event_deinit(wps_s);
    wps_deinit(role);

 exit_main:
    wps_loop_deinit(wps_s);
    wps_intf_deinit(wps_s);
    wps_clear_running_instance();

    wifi_intf_allow_pwr_mgr(&wps_uid);

    return exitcode;
}


/** 
 *  @brief Update Information Element to indicate WPS
 *  
 *  @param              None
 *  @return             WPS_STATUS_SUCCESS, WPS_STATUS_FAIL
 */
int
wps_update_ie(char * iface)
{
    int ret = WPS_STATUS_SUCCESS;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    strncpy(wps_global.ifname, iface, IFNAMSIZ);
    wps_init(wps_s, WPS_REGISTRAR);
    ret = wps_init_uap_common(gpwps_info, wps_s);
    wps_deinit(WPS_REGISTRAR);

    return (ret);
}



