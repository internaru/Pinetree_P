/** @file wps_wlan.h
 *  @brief This file contains definition for WLAN driver control/command.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WPS_WLAN_H_
#define _WPS_WLAN_H_

/*
 *  ctype from older glib installations defines BIG_ENDIAN.  Check it 
 *   and undef it if necessary to correctly process the wlan header
 *   files
 */
#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN
#endif

/** WPS session on */
#define WPS_SESSION_ON              1
/** WPS session off */
#define WPS_SESSION_OFF             0

/** Keep all AP in scan result */
#define FILTER_NONE                 0
/** Keep PIN AP in scan result */
#define FILTER_PIN                  1
/** Keep PBC AP in scan result */
#define FILTER_PBC                  2

/** MLAN 802.11 MAC Address */
typedef u8 mlan_802_11_mac_addr[ETH_ALEN];

/** Event header */
typedef struct _eventheader
{
    /** Event ID */
    u32 event_id;
    /** Event data */
    u8 event_data[0];
} event_header;

/**
 *                 IEEE 802.11 MAC Message Data Structures                   
 *                                                                           
 * Each IEEE 802.11 MAC message includes a MAC header, a frame body (which   
 * can be empty), and a frame check sequence field. This section gives the   
 * structures that used for the MAC message headers and frame bodies that    
 * can exist in the three types of MAC messages - 1) Control messages,       
 * 2) Data messages, and 3) Management messages.                             
 */
#ifdef BIG_ENDIAN
typedef struct _IEEEtypes_FrameCtl_t
{
    /** Order */
    u8 order:1;
    /** Wep */
    u8 wep:1;
    /** More Data */
    u8 more_data:1;
    /** Power Mgmt */
    u8 pwr_mgmt:1;
    /** Retry */
    u8 retry:1;
    /** More Frag */
    u8 more_frag:1;
    /** From DS */
    u8 from_ds:1;
    /** To DS */
    u8 to_ds:1;
    /** Sub Type */
    u8 sub_type:4;
    /** Type */
    u8 type:2;
    /** Protocol Version */
    u8 protocol_version:2;
} IEEEtypes_FrameCtl_t;
#else
typedef struct _IEEEtypes_FrameCtl_t
{
    /** Protocol Version */
    u8 protocol_version:2;
    /** Type */
    u8 type:2;
    /** Sub Type */
    u8 sub_type:4;
    /** To DS */
    u8 to_ds:1;
    /** From DS */
    u8 from_ds:1;
    /** More Frag */
    u8 more_frag:1;
    /** Retry */
    u8 retry:1;
    /** Power Mgmt */
    u8 pwr_mgmt:1;
    /** More Data */
    u8 more_data:1;
    /** Wep */
    u8 wep:1;
    /** Order */
    u8 order:1;
} IEEEtypes_FrameCtl_t;
#endif

/** wlan_802_11_header */
typedef struct _wlan_802_11_header
{
    /** Frame Control */
    u16 frm_ctl;
    /** Duration ID */
    u16 duration_id;
    /** Address1 */
    mlan_802_11_mac_addr addr1;
    /** Address2 */
    mlan_802_11_mac_addr addr2;
    /** Address3 */
    mlan_802_11_mac_addr addr3;
    /** Sequence Control */
    u16 seq_ctl;
    /** Address4 */
    mlan_802_11_mac_addr addr4;
} __ATTRIB_PACK__ wlan_802_11_header;

/** MrvlIETypes_MgmtFrameSet_t */
typedef struct _MrvlIETypes_MgmtFrameSet_t
{
    /** Type */
    u16 type;
    /** Length */
    u16 len;
    /** Frame Control */
    IEEEtypes_FrameCtl_t frame_control;
    /** Frame Contents */
    u8 frame_contents[0];
} MrvlIETypes_MgmtFrameSet_t;

/** Event body : STA associated/ discovery request */
typedef struct _eventbuf_discovery_request
{
    /** Reserved */
    u8 reserved[2];
    /** MAC address of associated STA */
    u8 sta_mac[ETH_ALEN];
    /** Discovery request/response buffer */
    u8 discovery_payload[0];
} eventbuf_discovery_request, eventbuf_station_connect;

/** Maximum length of SSID */
#define MRVDRV_MAX_SSID_LENGTH          32

#ifdef SSID_LIST_LENGTH_20
#define MRVDRV_MAX_SSID_LIST_LENGTH 20
#else
#define MRVDRV_MAX_SSID_LIST_LENGTH 10
#endif

/** Maximum number of channels that can be sent in a setuserscan ioctl */
#define WLAN_IOCTL_USER_SCAN_CHAN_MAX  50

typedef struct _wlan_ioctl_user_scan_chan
{
    u8 chan_number;  /**< Channel Number to scan */
    u8 radio_type;   /**< Radio type: 'B/G' Band = 0, 'A' Band = 1 */
    u8 scan_type;    /**< Scan type: Active = 1, Passive = 2 */
    u8 reserved;    /**< Reserved */
    u32 scan_time;   /**< Scan duration in milliseconds; if 0 default used */
} __ATTRIB_PACK__ wlan_ioctl_user_scan_chan;

typedef struct _wlan_ioctl_user_scan_ssid
{
    char ssid[MRVDRV_MAX_SSID_LENGTH + 1];  /**< SSID */
    u8 max_len;                              /**< Maximum length of SSID */
} __ATTRIB_PACK__ wlan_ioctl_user_scan_ssid;

typedef struct _wlan_ioctl_user_scan_cfg
{

    /** Flag set to keep the previous scan table intact */
    u8 keep_previous_scan;      /* Do not erase the existing scan results */

    /** BSS mode to be sent in the firmware command */
    u8 bss_mode;

    /** Configure the number of probe requests for active chan scans */
    u8 num_probes;

    /** Reserved */
    u8 reserved;

    /** BSSID filter sent in the firmware command to limit the results */
    u8 specific_bssid[ETH_ALEN];
    /** SSID filter list used in the to limit the scan results */
    wlan_ioctl_user_scan_ssid ssid_list[MRVDRV_MAX_SSID_LIST_LENGTH];

    /** Variable number (fixed maximum) of channels to scan up */
    wlan_ioctl_user_scan_chan chan_list[WLAN_IOCTL_USER_SCAN_CHAN_MAX];

} __ATTRIB_PACK__ wlan_ioctl_user_scan_cfg;

/** Maximum MGMT IE index count */
#define MAX_MGMT_IE_INDEX                        12

/** WPA Passphrase length*/
#define WPA_PASSPHARE_LENGTH   64

/** Maximum number of MAC addresses for one-shot filter modifications */
#define MAX_MAC_ONESHOT_FILTER  16
/** Maximum SSID length */
#define MAX_SSID_LENGTH         32
/** Maximum channels */
#define MAX_CHANNELS    14
/** Standard DEAUTH code for AP deauth */
#define DEAUTH_REASON_PREV_AUTH_INVALID 0x0002

/** mlan_802_11_ssid data structure */
typedef struct _mlan_802_11_ssid
{
    /** SSID Length */
    u32 ssid_len;
    /** SSID information field */
    u8 ssid[MAX_SSID_LENGTH];
} mlan_802_11_ssid;

/** scan_chan_list data structure */
typedef struct _scan_chan_list
{
    /** Number of channel */
    u32 num_of_chan;
    /** Channel number*/
    u16 chan[MAX_CHANNELS];
} scan_chan_list;

/** mac_filter data structure */
typedef struct _mac_filter
{
    /** Mac filter mode */
    u16 filter_mode;
    /** Mac adress count */
    u16 mac_count;
    /** Mac address list */
    mlan_802_11_mac_addr mac_list[MAX_MAC_ONESHOT_FILTER];
} mac_filter;

/** wpa parameter */
typedef struct _wpa_param
{
    /** Pairwise cipher WPA */
    u8 pairwise_cipher_wpa;
    /** Pairwise cipher WPA2 */
    u8 pairwise_cipher_wpa2;
    /** Group cipher */
    u8 group_cipher;
    /** RSN replay protection */
    u8 rsn_protection;
    /** Passphrase length */
    u32 length;
    /** Passphrase */
    u8 passphrase[64];
    /** Group key rekey time */
    u32 gk_rekey_time;
} wpa_param;

/** wep key */
typedef struct _wep_key
{
    /** Key index 0-3 */
    u8 key_index;
    /** Is default */
    u8 is_default;
    /** Length */
    u16 length;
    /** Key data */
    u8 key[26];
} wep_key;

/** BSS config structure */
typedef struct _bss_config_t
{
    /** AP mac addr */
    mlan_802_11_mac_addr mac_addr;
    /** SSID */
    mlan_802_11_ssid ssid;
    /** Broadcast ssid control */
    u8 bcast_ssid_ctl;
    /** Radio control */
    u8 radio_ctl;
    /** dtim period */
    u8 dtim_period;
    /** beacon period */
    u16 beacon_period;
    /** rates */
    u8 rates[14];
    /** Tx data rate */
    u16 tx_data_rate;
    /** multicast/broadcast data rate */
    u16 mcbc_data_rate;
    /** Tx power level */
    u8 tx_power_level;
    /** Tx antenna */
    u8 tx_antenna;
    /** Rx anteena */
    u8 rx_antenna;
    /** packet forward control */
    u8 pkt_forward_ctl;
    /** max station count */
    u16 max_sta_count;
    /** mac filter */
    mac_filter filter;
    /** station ageout timer in the unit of 100ms  */
    u32 sta_ageout_timer;
    /** PS station ageout timer in the unit of 100ms  */
    u32 ps_sta_ageout_timer;
    /** RTS threshold */
    u16 rts_threshold;
    /** fragmentation threshold */
    u16 frag_threshold;
    /**  retry_limit */
    u16 retry_limit;
    /**  pairwise update timeout */
    u32 pairwise_update_timeout;
    /** pairwise handshake retries */
    u32 pwk_retries;
    /**  groupwise update timeout */
    u32 groupwise_update_timeout;
    /** groupwise handshake retries */
    u32 gwk_retries;
    /** preamble type */
    u8 preamble_type;
    /** band cfg */
    u8 band_cfg;
    /** channel */
    u8 channel;
    /** auth mode */
    u16 auth_mode;
    /** encryption protocol */
    u16 protocol;
    /** key managment type */
    u16 key_mgmt;
    /** wep param */
    wep_key wep_cfg[4];
    /** wpa param */
    wpa_param wpa_cfg;
    /** Mgmt IE passthru mask */
    u32 mgmt_ie_passthru_mask;
    /* 
     * 11n HT Cap  HTCap_t  ht_cap
     */
    /** HT Capabilities Info field */
    u16 ht_cap_info;
    /** A-MPDU Parameters field */
    u8 ampdu_param;
    /** Supported MCS Set field */
    u8 supported_mcs_set[16];
    /** HT Extended Capabilities field */
    u16 ht_ext_cap;
    /** Transmit Beamforming Capabilities field */
    u32 tx_bf_cap;
    /** Antenna Selection Capability field */
    u8 asel;
    /** Enable 2040 coex */
    u8 enable_2040coex;
    /** key management operation */
    u16 key_mgmt_operation;
    /** BSS status */
    u16 bss_status;
    /* pre shared key */
    u8 psk[32];
    /** scan channel list in ACS mode */
    scan_chan_list chan_list;

} bss_config_t;

/* Function Declaration */

/** 
 *  @brief Set the BSS information into the registrar credential structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *  @param bss       BSS information
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int load_cred_info(WPS_DATA * wps_s, PWPS_INFO pwps_info,
                          bss_config_t * bss);
/** 
 *  @brief Checks if APs configuration is same as default OOB settings
 *  
 *  @param bss        A pointer to bss_config_t structure.
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_check_for_default_oob_settings(bss_config_t * bss);

/** 
 *  @brief Copy Credential data into BSS configuration
 *  @param bss       A pointer to the bss_config_t structure
 *  @param pCred     A pointer to the CREDENTIAL_DATA structure
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_cred_to_bss_config(bss_config_t * bss, PCREDENTIAL_DATA pCred,
                                  u8 load_by_oob);

/** 
 *  @brief Send DEAUTH to associated STA .
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @param sta_mac     STA MAC
 *
 *  @return     WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int apcmd_station_deauth(WPS_DATA * wps_s, u8 * sta_mac);

/** 
 *  @brief  Initial ioctl interface to WLAN driver
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_init(WPS_DATA * wps_s);

/** 
 *  @brief  Close ioctl interface to WLAN driver
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           None 
 */
extern void wps_wlan_deinit(WPS_DATA * wps_s);

#ifdef WPS_IE_NEEDED
/** 
 *  @brief Set WPS IE for foreground scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
extern int wps_wlan_set_fgscan_wps_ie(void);

/** 
 *  @brief Reset WPS IE for foreground scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
extern int wps_wlan_reset_wps_ie(void);
#endif

/** 
 *  @brief convert char to hex integer
 * 
 *  @param chr      char to convert
 *  @return         hex integer or 0
 */
extern int hexval(s32 chr);
/** 
 *  @brief convert string to mac address
 *
 *  @param s                A pointer string buffer
 *  @param mac_addr         pointer to mac address array
 *  @return					none
 */
extern void a2_mac_addr(char *s, u8 * mac_addr);

/* 
 *  @brief Process scan results get from WLAN driver
 *
 *  @param wps_s       Pointer to WPS global structure
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
extern int wps_wlan_get_all_scan_results(WPS_DATA * wps_s);

/* 
 *  @brief Process scan results get from WLAN driver
 *
 *  @param wps_s       Pointer to WPS global structure
 *  @param dev_pwd_id  PIN or PBC
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
extern int wps_wlan_scan_results(WPS_DATA * wps_s, u16 dev_pwd_id, char *ssid);

/* 
 *  @brief convert hex char to integer
 *  
 *  @param hex      A pointer to hex string
 *  @param buf      buffer to storage the data
 *  @param len      
 *  @return         WPS_STATUS_SUCCESS--success, otherwise --fail
 */
extern int hexstr2bin(const s8 * hex, u8 * buf, size_t len);

/* 
 *  @brief convert binary data to Hex string
 *  
 *  @param bin      A pointer to binary data
 *  @param hex      A pointer to hex string
 *  @param len      
 *  @return         WPS_STATUS_SUCCESS--success, otherwise --fail
 */

extern int bin2hexstr(const u8 * bin, s8 * hex, size_t len);
extern int char_str_to_hex_array(char *s, u8 * hex_array);
/** 
 *  @brief  Get associated ESSID from WLAN driver (SIOCGIWESSID)
 *
 *  @param ssid       Buffer for the SSID, must be at least 32 bytes long
 *  @return           SSID length on success, WPS_STATUS_FAIL on failure
 */
extern int wps_wlan_get_ssid(u8 * ssid);

/** 
 *  @brief  Set ESSID to associate to WLAN driver (SIOCSIWESSID)
 *
 *  @param ssid       SSID
 *  @param ssid_len   Length of SSID (0..32)
 *  @param skip_scan  Skip scan during association
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_ssid(const u8 * ssid, size_t ssid_len, int skip_scan);

/** 
 *  @brief  Set Auth Mode to WLAN driver (SIOCSIWAUTH)
 *
 *  @param auth_mode  Open or shared auth mode.
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_authentication(int auth_mode);

/** 
 *  @brief  Get associated BSSID from WLAN driver (SIOCGIWAP)
 *
 *  @param bssid      Buffer for the BSSID
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_get_wap(u8 * bssid);

/** 
 *  @brief  Set BSSID to associate to WLAN driver (SIOCSIWAP)
 *
 *  @param bssid      BSSID
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_wap(const u8 * bssid);

/** 
 *  @brief  Set wireless mode to WLAN driver (infra/adhoc) (SIOCSIWMODE)
 *
 *  @param mode       0 = infra/BSS (associate with an AP), 1 = adhoc/IBSS
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_mode(int mode);

/** 
 *  @brief Generate Random PSK
 *  
 *  @param dest      A pointer to the destination character array
 *  @param length    Destination string legth.
 *  @return          None.            
 *  */
void wps_wlan_generate_random_psk(char *dest, unsigned short len);
/** 
 *  @brief Resets AP configuration to default OOB settings
 *  
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param pwps_info  A pointer to the PWPS_INFO structure
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
*/

/** 
 *  @brief This function implements reset device state to OOB handling for
 *  1. WPS
 *  2. WPSE
 *  3. Customer specific execution. 
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param pwps_info  A pointer to the PWPS_INFO structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */

int wps_handle_reset_oob(PWPS_INFO pwps_info, WPS_DATA * wps_s);

int wps_wlan_reset_ap_config_to_OOB(WPS_DATA * wps_s, PWPS_INFO pwps_info);
 /** 
 *  @brief Creates new AP configuration after AP has been reset to OOB settings
 *  
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param pwps_info  A pointer to the PWPS_INFO structure
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_create_ap_config_after_OOB(WPS_DATA * wps_s, PWPS_INFO pwps_info);

/** 
 *  @brief Reset AP configuration for Reversed Role or OOB.
 *
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS on success, WPS_STATUS_FAIL on fail
 */
int wps_wlan_reset_ap_config(WPS_DATA * wps_s, PWPS_INFO pwps_info);

/** 
 *  @brief  Handle bss_start event to update WPS_INFO parameters and WPS IEs
 *  @param wps_s      Pointer to WPS global structure
 *  @param pwps_info  Pointer to WPS_INFO structure
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_load_ap_settings_after_bss_restart(WPS_DATA * wps_s,
                                           PWPS_INFO pwps_info);
/** 
 *  @brief Process and send adhoc registrar config command
 *  @param flag       Flag: Set/Get/Clear WPS IE
 *  @param ie_index   A pointer to a IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_uap_sys_config(int flag, short *ie_index);

/** 
 *  @brief Creates a get bss_config request and sends to the driver
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param bss        A pointer to the bss structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int apcmd_get_bss_config(WPS_DATA * wps_s, bss_config_t * bss);

/** 
 *  @brief Creates a set bss_config request and sends to the driver
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param bss        A pointer to the bss structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int apcmd_set_bss_config(WPS_DATA * wps_s, bss_config_t * bss);

/** 
 *  @brief  Get interface flags from WLAN driver (SIOCGIFFLAGS)
 *
 *  @param flags      Pointer to returned flags value
 *  @param ifname     Interface name 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_get_ifflags(int *flags, char *ifname);

/** 
 *  @brief  Set interface flags to WLAN driver (SIOCSIFFLAGS)
 *
 *  @param flags      New value for flags
 *  @param ifname     Interface name 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_ifflags(int flags, char *ifname);

/** 
 *  @brief UP interface either for bridge or wifi interface 
 *
 *  @param wps_s      Pointer to WPS_DATA structure 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_update_interface_flags(WPS_DATA * wps_s);

/** 
 *  @brief  Set private ioctl command "deauth" to WLAN driver
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_deauth(void);

/** 
 *  @brief Process and send ie config command for (re)assoc request. 
 *  @param flag   FLAG: Set/clear WPS IE
 *  @param index  A pointer to the IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_assoc_request_ie_config(int flag, short *ie_index);

/** 
 *  @brief  Wlan event parser for FW events
 *  @param context    Pointer to Context 
 *  @param wps_s      Pointer to WPS global structure
 *  @param evt_buffer Pointer to Event buffer
 *  @param evt_len    Event Length
 *
 *  @return           None 
 */
extern void wps_wlan_event_parser(void *context, WPS_DATA * wps_s,
                                  char *evt_buffer, int evt_len);

/** 
 *  @brief Starts the existing AP BSS .
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int apcmd_start_bss(WPS_DATA * wps_s);

/** 
 *  @brief Stops the existing AP BSS .
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int apcmd_stop_bss(WPS_DATA * wps_s);

/** 
 *  @brief Loads the AP configuration into data structures
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param pwps_info  A pointer to the PWPS_INFO structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_load_uap_cred(WPS_DATA * wps_s, PWPS_INFO pwps_info);

/** 
 *  @brief Loads the STA configuration into data structures
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *  @param pwps_info  A pointer to the PWPS_INFO structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_load_wsta_registrar_cred(WPS_DATA * wps_s, PWPS_INFO pwps_info);

/**
 *  @brief  Validate checksum of PIN
 *
 *  @param PIN      PIN value
 *  @return         Validation result 1 - Success 0 - Failure
 */
int ValidateChecksum(unsigned long int PIN);

/*
 * @brief process WFD generic event data
 *
 * @param buffer   Pointer to received buffer
 * @param size     Length of the received event data
 * @return         N/A
 */
void wfd_process_generic_event(u8 * buffer, u16 size);

/** 
 *  @brief Creates a wfd_action_frame request and sends to the driver
 *
 *  Usage: "Usage : wfd_action_frame "
 *
 *  @param sel_index Index selected by user
 *  @param frame_sub_type  Frame Sub type for generic action frame
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wfdcmd_action_frame(int sel_index, int frame_sub_type);

/** 
 *  @brief  Update the password IE in the device configuration.
 *          First Get the config and update with password. 
 *  @param  None
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_update_password_ie_config(void);

/** 
 *  @brief  Remove the password IE in the device configuration.
 *          First Get the config and remove password if it exists. 
 *  @param  None
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_remove_password_ie_config(void);

/** 
 *  @brief Process and send ie config command 
 *  @param ie_index  A pointer to the IE buffer index
 *  @param data_len_wfd  Length of P2P data, 0 to get, else set.
 *  @param data_len_wps  Length of WPS data, 0 to get, else set.
 *  @param buf      Pointer to buffer to set.
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wfd_ie_config(u16 *, u16, u16, u8 *);

/** 
 *  @brief  Update SET/CLEAR the group formation bit for GO.
 *  @param  set_clear  Set or clear the bit. 
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_update_group_formation_config(int set_clear);

/** 
 *  @brief Performs the ioctl operation to send the command to
 *  the driver for various modes.
 *
 *  @param  mode Mode value to set
 *  @return WPS_STATUS_SUCCESS or WPS_STATUS_FAIL
 */
int wfd_wlan_update_mode(u16 mode);

/** 
 *  @brief Performs the ioctl operation to set mac address to kernel
 *
 *  @param  mac mac address to set
 *  @return WPS_STATUS_SUCCESS or WPS_STATUS_FAIL
 */
int wlan_set_intended_mac_addr(u8 mac[ETH_ALEN]);

/** 
 *  @brief  Send Custom_IE command to driver.
 *  @param buf Pointer to data buffer
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wlan_custom_ie_config(u8 * buf);

/** 
 *  @brief  Send HostCmd command to driver for WFD.
 *  @param buf Pointer to data buffer
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wfd_wlan_hostcmd(u8 * buf);

/** 
 *  @brief  Set BSS type (STA/uAP)
 *
 *  @param bss_type   an integer 0: Sta, 1:uAP
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_set_bss_type(int bss_type);

int wfd_update_persistent_record(PWPS_INFO pwps_info);

/** WPS IE Operation type */
/** Clear WPS IE */
#define CLEAR_WPS_IE   0x00
/** Set WPS IE */
#define SET_WPS_IE     0x10
/** Get WPS IE */
#define GET_WPS_IE     0x20

/** Probe request Enable OR Mask */
#define WPS_ORMASK_ENABLE_PROBE_REQ      0x0010
/** Probe request Disable AND Mask */
#define WPS_ANDMASK_DISABLE_PROBE_REQ    0xffef

/** WPS IE set type : session inactive */
#define WPS_AP_SESSION_INACTIVE     0x00
/** WPS IE set type : session active ap */
#define WPS_AP_SESSION_ACTIVE       0x01
/** WPS IE set type : session active sta */
#define WPS_STA_SESSION_ACTIVE      0x02
/** WPS IE set type : session inactive AP AssocResp */
#define WPS_AP_SESSION_ACTIVE_AR      0x03
/** WPS IE set type : session inactive AP ReAssocResp */
#define WPS_AP_SESSION_INACTIVE_AR      0x04

/** IE set type : wps + session active ap */
#define SET_WPS_AP_SESSION_ACTIVE_IE (SET_WPS_IE | WPS_AP_SESSION_ACTIVE)
/** IE set type : wps + session inactive ap */
#define SET_WPS_AP_SESSION_INACTIVE_IE (SET_WPS_IE | WPS_AP_SESSION_INACTIVE)
/** IE set type : wps + session active sta */
#define SET_WPS_STA_SESSION_ACTIVE_IE (SET_WPS_IE | WPS_STA_SESSION_ACTIVE)
/** IE set type : wps + session active ap */
#define SET_WPS_AP_SESSION_ACTIVE_AR_IE (SET_WPS_IE | WPS_AP_SESSION_ACTIVE_AR)
/** IE set type : wps + session active ap */
#define SET_WPS_AP_SESSION_INACTIVE_AR_IE (SET_WPS_IE | WPS_AP_SESSION_INACTIVE_AR)
/** IE clear type : wps + session active ap */
#define CLEAR_AR_WPS_IE                   (CLEAR_WPS_IE | WPS_AP_SESSION_ACTIVE_AR)

/** 
 *  @brief Process and send ie config command 
 *  @param flag   FLAG: Set/clear WPS IE
 *  @param index  A pointer to the IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_ie_config(int flag, short *ie_index);

/* offset of WPSE IE in mgmt frame event */
#define WPSE_IE_OFFSET 7

int wps_wlan_enrollee_ie_config(int flag);

/**WPS Probe Request IEEE frame contents offset: 
 * FrameCtl(2), Duration(2),
 * DestAddr(6),SrcAddr(6), BSSID(6), SeqCtl(2), WLANAddr4(6) */
#define WPS_PROBE_REQ_IE_OFFSET 30

/** 
 *  @brief  Process scan operation
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           None
 */
extern void wps_wlan_scan(WPS_DATA *, char *);

/** 
 *  @brief  Set private ioctl command "wpssession" to WLAN driver
 *
 *  @param enable     0 - WPS Session Disable, 1 - WPS Session Enable
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_session_control(int enable);

/** 
 *  @brief  Get power management mode of WLAN driver (SIOCGIWPOWER)
 *
 *  @param enable     Pointer of returned buffer
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_get_power_mode(u8 * enable);

/** 
 *  @brief  Set power management mode to WLAN driver (SIOCSIWPOWER)
 *
 *  @param enable     0 = Disable PS mode, 1 = Enable PS mode
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_set_power_mode(u8 enable);

/** 
 *  @brief  Get compiled version of wireless extension
 *
 *  @param version    version
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
extern int wps_wlan_get_WECompiledVersion(int *version);

/** 
 *  @brief mapping RF band by frequency
 *
 *  @param freq     frequency value
 *  @return         channel number
 */
extern int wps_wlan_freq_to_band(int freq);

#endif /* _WPS_WLAN_H_ */
