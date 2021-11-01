/** @file  uaputl.c
 *  
 *  @brief Program to configure uAP parameters.
 *  
 *  Copyright (C) 2011-2013, Marvell International Ltd. All
 *  Rights Reserved
 */

#include "include.h"
#include "memAPI.h"
#include "os_net_device.h"
#include "os_wireless.h"
#include "wlanInterface.h"
#include "logger.h"
#include "wifi_intf.h"
#include "wifi_config.h"
#include "moal_uap.h"
#include "uap_api.h"
#include "wifi_intf_vars.h"

#define BASIC_RATE_SET_BIT      0x80
#define SECOND_CHANNEL_BELOW    0x30
#define SECOND_CHANNEL_ABOVE    0x10
#define CIPHER_NONE             0

    typedef enum _wmm_ac
    {
        AC_BE,
        AC_BK,
        AC_VI,
        AC_VO,
    } wmm_ac;


/**
 * @brief Create UAP Configuration
 *
 * @param config    Pointer to an allocated buffer with 
 *                  configuration
 *
 * @return OK or FAIL.
 */
error_type_t
uap_create_config(void **config)
{
    int i;
    t_u8* buffer;
    mlan_uap_bss_param * ptr;
    error_type_t retval = FAIL;

    buffer = (t_u8 *) MEM_MALLOC(sizeof(mlan_uap_bss_param));
    if (buffer) {
        memset(buffer, 0, sizeof(mlan_uap_bss_param));

        ptr = (mlan_uap_bss_param *) buffer;

        ptr->bcast_ssid_ctl = WIFI_UAP_BROADCAST_SSID_DEFAULT;
        ptr->dtim_period = WIFI_UAP_DTIM_PERIOD_DEFAULT;
        ptr->beacon_period = WIFI_UAP_BEACON_PERIOD_DEFAULT;
        ptr->tx_power_level = WIFI_UAP_TX_POWER_DEFAULT; 
        ptr->pkt_forward_ctl = WIFI_UAP_INTRA_BSS_TRAFFIC_ALLOWED;
        ptr->max_sta_count = WIFI_UAP_MAX_STA_COUNT_DEFAULT;
        if (WIFI_IFACE_ID_UAP_PRI0 == wifi_get_uap_active_config())
        {
            ptr->max_sta_count = WIFI_UAP_PRI0_MAX_STA_COUNT_DEFAULT;
        }    
        ptr->sta_ageout_timer = WIFI_UAP_STA_AGEOUT_TIMEOUT_DEFAULT;
        ptr->ps_sta_ageout_timer = WIFI_UAP_PS_STA_AGEOUT_TIMEOUT_DEFAULT;
        ptr->rts_threshold = WIFI_UAP_RTS_THRESHOLD_DEFAULT;
        ptr->frag_threshold = WIFI_UAP_FRAG_THRESHOLD_DEFAULT;
        ptr->retry_limit = WIFI_UAP_RETRY_LIMIT_DEFAULT;
        ptr->pairwise_update_timeout = WIFI_UAP_PAIRWISE_UPDATE_TIMEOUT_DEFAULT;
        ptr->pwk_retries = WIFI_UAP_PAIRWISE_HANDSHAKE_RETRY_DEFAULT;
        ptr->groupwise_update_timeout = WIFI_UAP_GROUPWISE_UPDATE_TIMEOUT_DEFAULT;
        ptr->gwk_retries = WIFI_UAP_GROUPWISE_HANDSHAKE_RETRY_DEFAULT;
        ptr->channel = WIFI_UAP_CHANNEL_DEFAULT;
        ptr->num_of_chan = WIFI_UAP_NUM_SCAN_CHANNEL_LIST_DEFAULT;
        {
            char array[] = WIFI_UAP_SCAN_CHANNEL_LIST_DEFAULT;
            for (i=0; i<ptr->num_of_chan; i++) {
                ptr->chan_list[i].chan_number = array[i];
                if (ptr->chan_list[i].chan_number > MAX_CHANNELS_BG) {
                    ptr->chan_list[i].band_config_type = BAND_CONFIG_5GHZ;
                }
            }
        }
                    
        ptr->ht_cap_info = WIFI_UAP_HT_CAP_INFO_DEFAULT;
        ptr->ampdu_param = WIFI_UAP_AMPDU_PARAM_DEFAULT;

        {
            char array[] = WIFI_UAP_SUPPORTED_MCS_SET_DEFAULT;
            memcpy(ptr->supported_mcs_set, array, sizeof(array));
        }

        ptr->protocol = WIFI_UAP_ENCRYPTION_PROTOCOL_DEFAULT;

        ptr->wpa_cfg.gk_rekey_time = WIFI_UAP_WPA_GROUP_REKEY_TIMEOUT_DEFAULT;

        ptr->wmm_para.ouitype[0] = 0x00;
        ptr->wmm_para.ouitype[1] = 0x50;
        ptr->wmm_para.ouitype[2] = 0xF2;
        ptr->wmm_para.ouitype[3] = 0x02;
        ptr->wmm_para.ouisubtype = 0x1;
        ptr->wmm_para.version = 0x1;
        ptr->wmm_para.qos_info = 0x80;

        ptr->wmm_para.ac_params[AC_BE].aci_aifsn.aifsn = 3;
        ptr->wmm_para.ac_params[AC_BE].aci_aifsn.acm = 0;
        ptr->wmm_para.ac_params[AC_BE].aci_aifsn.aci = 0;
        ptr->wmm_para.ac_params[AC_BE].ecw.ecw_min = 4;
        ptr->wmm_para.ac_params[AC_BE].ecw.ecw_max = 10;
        ptr->wmm_para.ac_params[AC_BE].tx_op_limit = 0;

        ptr->wmm_para.ac_params[AC_BK].aci_aifsn.aifsn = 7;
        ptr->wmm_para.ac_params[AC_BK].aci_aifsn.acm = 0;
        ptr->wmm_para.ac_params[AC_BK].aci_aifsn.aci = 1;
        ptr->wmm_para.ac_params[AC_BK].ecw.ecw_min = 4;
        ptr->wmm_para.ac_params[AC_BK].ecw.ecw_max = 10;
        ptr->wmm_para.ac_params[AC_BK].tx_op_limit = 0;
 
        ptr->wmm_para.ac_params[AC_VI].aci_aifsn.aifsn = 2;
        ptr->wmm_para.ac_params[AC_VI].aci_aifsn.acm = 0;
        ptr->wmm_para.ac_params[AC_VI].aci_aifsn.aci = 2;
        ptr->wmm_para.ac_params[AC_VI].ecw.ecw_min = 3;
        ptr->wmm_para.ac_params[AC_VI].ecw.ecw_max = 4;
        ptr->wmm_para.ac_params[AC_VI].tx_op_limit = 94;
 
        ptr->wmm_para.ac_params[AC_VO].aci_aifsn.aifsn = 2;
        ptr->wmm_para.ac_params[AC_VO].aci_aifsn.acm = 0;
        ptr->wmm_para.ac_params[AC_VO].aci_aifsn.aci = 3;
        ptr->wmm_para.ac_params[AC_VO].ecw.ecw_min = 2;
        ptr->wmm_para.ac_params[AC_VO].ecw.ecw_max = 3;
        ptr->wmm_para.ac_params[AC_VO].tx_op_limit = 47;

        *config = (void *) buffer;;

        retval = OK;
    }

    return retval;
}


/**
 * @brief Delete UAP Configuration
 *
 * @param config    Pointer to a previously allocated buffer 
 *                  with configuration from
 *                  uap_create_config()
 *
 * @return OK or FAIL.
 */
error_type_t
uap_delete_config(void **config)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (config) {
        ptr = (void *) *config;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *config = NULL;
            retval = OK;
        }
    }
    return retval;
}

/**
 *  @brief Show all the BSS configuration in the buffer
 *
 *  @param buf     Pointer to BSS configuration buffer
 *  
 *  $return         N/A
 */
void
print_bss_config(mlan_uap_bss_param * buf)
{
    int i = 0;
    int flag = 0;

    if (!buf) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ERR:Empty BSS config!\n"));
        return;
    }

    /* Print AP MAC address */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("AP MAC address = "));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02X:%02X:%02X:%02X:%02X:%02X", 
             buf->mac_addr[0],buf->mac_addr[1],buf->mac_addr[2],
             buf->mac_addr[3],buf->mac_addr[4],buf->mac_addr[5]));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));

    /* Print SSID */
    if (buf->ssid.ssid_len) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("SSID = %s\n", buf->ssid.ssid));
    }

    /* Print broadcast SSID control */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("SSID broadcast = %s\n",
           (buf->bcast_ssid_ctl == 1) ? "enabled" : "disabled"));

    /* Print DTIM period */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("DTIM period = %d\n", buf->dtim_period));

    /* Print beacon period */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Beacon period = %d\n", buf->beacon_period));

    /* Print rates */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Basic Rates ="));
    for (i = 0; i < MAX_DATA_RATES; i++) {
        if (!buf->rates[i])
            break;
        if (buf->rates[i] > (BASIC_RATE_SET_BIT - 1)) {
            flag = flag ? : 1;
            DPRINTF((DBG_ERROR | DBG_OUTPUT), (" 0x%x", buf->rates[i]));
        }
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%s\nNon-Basic Rates =", flag ? "" : " ( none ) "));
    for (flag = 0, i = 0; i < MAX_DATA_RATES; i++) {
        if (!buf->rates[i])
            break;
        if (buf->rates[i] < BASIC_RATE_SET_BIT) {
            flag = flag ? : 1;
            DPRINTF((DBG_ERROR | DBG_OUTPUT), (" 0x%x", buf->rates[i]));
        }
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%s\n", flag ? "" : " ( none ) "));

    /* Print Tx data rate */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Tx data rate = "));
    if (buf->tx_data_rate == 0)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("auto\n"));
    }
    else 
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("0x%x\n", buf->tx_data_rate));
    }

    /* Print MCBC data rate */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("MCBC data rate = "));
    if (buf->mcbc_data_rate == 0)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("auto\n"));
    }
    else
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("0x%x\n", buf->mcbc_data_rate));
    }

    /* Print Tx power level */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Tx power = %d dBm\n", buf->tx_power_level));

    /* Print Tx antenna */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Tx antenna = %s\n", (buf->tx_antenna) ? "B" : "A"));

    /* Print Rx antenna */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Rx antenna = %s\n", (buf->rx_antenna) ? "B" : "A"));

    /* Print packet forward control */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Firmware = %s\n", (buf->pkt_forward_ctl == 0) ?
           "forwards all packets to the host" : "handles intra-BSS packets"));

    /* Print maximum STA count */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Max Station Number configured = %d\n", buf->max_sta_count));

    /* Print mgmt frame FWD control */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("MGMT frame Fwd Control = 0x%lx\n", buf->mgmt_ie_passthru_mask));

    /* Print MAC filter */
    if (buf->filter.filter_mode == 0) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Filter Mode = Filter table is disabled\n"));
    } else {
        if (buf->filter.filter_mode == 1) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT),
                ("Filter Mode = Allow MAC addresses specified in the allowed list\n"));
        } else if (buf->filter.filter_mode == 2) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT),
                ("Filter Mode = Block MAC addresses specified in the banned list\n"));
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Filter Mode = Unknown\n"));
        }
        if (buf->filter.mac_count <=16) {
            for (i = 0; i < buf->filter.mac_count; i++) {
                DPRINTF((DBG_ERROR | DBG_OUTPUT), ("MAC_%d = ", i));
                DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02X:%02X:%02X:%02X:%02X:%02X", 
                         buf->filter.mac_list[i][0],buf->filter.mac_list[i][1],buf->filter.mac_list[i][2],
                         buf->filter.mac_list[i][3],buf->filter.mac_list[i][4],buf->filter.mac_list[i][5]));
                DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
            }
        }
    }

    /* Print STA ageout timer */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("STA ageout timer = %ld\n", buf->sta_ageout_timer));

    /* Print PS STA ageout timer */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PS STA ageout timer = %ld\n", buf->ps_sta_ageout_timer));

    /* Print RTS threshold */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("RTS threshold = %d\n", buf->rts_threshold));

    /* Print Fragmentation threshold */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Fragmentation threshold = %d\n", buf->frag_threshold));

    /* Print retry limit */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Retry Limit = %d\n", buf->retry_limit));

    /* Print preamble type */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Preamble type = %s\n", (buf->preamble_type == 0) ?
           "auto" : ((buf->preamble_type == 1) ? "short" : "long")));

    /* Print channel */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Channel = %d\n", buf->channel));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Band = %s\n",
           (buf->band_cfg & BAND_CONFIG_5GHZ) ? "5GHz" : "2.4GHz"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Channel Select Mode = %s\n",
           (buf->band_cfg & BAND_CONFIG_ACS_MODE) ? "ACS" : "Manual"));
    buf->band_cfg &= 0x30;
    if (buf->band_cfg == 0)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("no secondary channel\n"));
    }
    else if (buf->band_cfg == SECOND_CHANNEL_ABOVE)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("secondary channel is above primary channel\n"));
    }
    else if (buf->band_cfg == SECOND_CHANNEL_BELOW)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("secondary channel is below primary channel\n"));
    }

    /* Print channel list */
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Channels List = "));
    for (i = 0; (unsigned int) i < buf->num_of_chan; i++) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n%d\t%sGHz", buf->chan_list[i].chan_number,
               (buf->chan_list[i].
                band_config_type & BAND_CONFIG_5GHZ) ? "5" : "2.4"));
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));

    /* Print auth mode */
    switch (buf->auth_mode) {
    case 0:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("AUTHMODE = Open authentication\n"));
        break;
    case 1:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("AUTHMODE = Shared key authentication\n"));
        break;
    case 255:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("AUTHMODE = Auto (open and shared key)\n"));
        break;
    default:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ERR: Invalid authmode=%d\n", buf->auth_mode));
        break;
    }

    /* Print protocol */
    switch (buf->protocol) {
    case 0:
    case PROTOCOL_NO_SECURITY:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PROTOCOL = No security\n"));
        break;
    case PROTOCOL_STATIC_WEP:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PROTOCOL = Static WEP\n"));
        break;
    case PROTOCOL_WPA:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PROTOCOL = WPA \n"));
        break;
    case PROTOCOL_WPA2:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PROTOCOL = WPA2 \n"));
        break;
    case PROTOCOL_WPA | PROTOCOL_WPA2:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PROTOCOL = WPA/WPA2 \n"));
        break;
    default:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Unknown PROTOCOL: 0x%x \n", buf->protocol));
        break;
    }

    /* Print key management */
    if (buf->key_mgmt == KEY_MGMT_PSK)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("KeyMgmt = PSK\n"));
    }
    else
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("KeyMgmt = NONE\n"));
    }

    /* Print WEP configurations */
    if (buf->wep_cfg.key0.length) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_0 = "));
        for (i = 0; i < buf->wep_cfg.key0.length; i++) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02x ", buf->wep_cfg.key0.key[i]));
        }
        if (buf->wep_cfg.key0.is_default)
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("<Default>\n"));
        }
        else
        {
             DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_0 = NONE\n"));
    }
    if (buf->wep_cfg.key1.length) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_1 = "));
        for (i = 0; i < buf->wep_cfg.key1.length; i++) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02x ", buf->wep_cfg.key1.key[i]));
        }
        if (buf->wep_cfg.key1.is_default)
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("<Default>\n"));
        }
        else 
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_1 = NONE\n"));
    }
    if (buf->wep_cfg.key2.length) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_2 = "));
        for (i = 0; i < buf->wep_cfg.key2.length; i++) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02x ", buf->wep_cfg.key2.key[i]));
        }
        if (buf->wep_cfg.key2.is_default)
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("<Default>\n"));
        }
        else
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_2 = NONE\n"));
    }
    if (buf->wep_cfg.key3.length) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_3 = "));
        for (i = 0; i < buf->wep_cfg.key3.length; i++) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%02x ", buf->wep_cfg.key3.key[i]));
        }
        if (buf->wep_cfg.key3.is_default)
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("<Default>\n"));
        }
        else
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WEP KEY_3 = NONE\n"));
    }

    /* Print WPA configurations */
    if (buf->protocol & PROTOCOL_WPA) {
        switch (buf->wpa_cfg.pairwise_cipher_wpa) {
        case CIPHER_TKIP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA = TKIP\n"));
            break;
        case CIPHER_AES_CCMP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA = AES CCMP\n"));
            break;
        case CIPHER_TKIP | CIPHER_AES_CCMP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA = TKIP + AES CCMP\n"));
            break;
        case CIPHER_NONE:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA =  None\n"));
            break;
        default:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Unknown PwkCipherWPA 0x%x\n",
                   buf->wpa_cfg.pairwise_cipher_wpa));
            break;
        }
    }
    if (buf->protocol & PROTOCOL_WPA2) {
        switch (buf->wpa_cfg.pairwise_cipher_wpa2) {
        case CIPHER_TKIP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA2 = TKIP\n"));
            break;
        case CIPHER_AES_CCMP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA2 = AES CCMP\n"));
            break;
        case CIPHER_TKIP | CIPHER_AES_CCMP:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA2 = TKIP + AES CCMP\n"));
            break;
        case CIPHER_NONE:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("PwkCipherWPA2 =  None\n"));
            break;
        default:
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Unknown PwkCipherWPA2 0x%x\n",
                   buf->wpa_cfg.pairwise_cipher_wpa2));
            break;
        }
    }
    switch (buf->wpa_cfg.group_cipher) {
    case CIPHER_TKIP:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("GroupCipher = TKIP\n"));
        break;
    case CIPHER_AES_CCMP:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("GroupCipher = AES CCMP\n"));
        break;
    case CIPHER_NONE:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("GroupCipher = None\n"));
        break;
    default:
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Unknown Group cipher 0x%x\n", buf->wpa_cfg.group_cipher));
        break;
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("RSN replay protection = %s\n",
           (buf->wpa_cfg.rsn_protection) ? "enabled" : "disabled"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Pairwise Handshake timeout = %ld\n", buf->pairwise_update_timeout));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Pairwise Handshake Retries = %ld\n", buf->pwk_retries));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Groupwise Handshake timeout = %ld\n",
           buf->groupwise_update_timeout));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Groupwise Handshake Retries = %ld\n", buf->gwk_retries));
    if (buf->wpa_cfg.length > 0) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WPA passphrase = "));
        for (i = 0; (unsigned int) i < buf->wpa_cfg.length; i++)
            DPRINTF((DBG_ERROR | DBG_OUTPUT), ("%c", buf->wpa_cfg.passphrase[i]));
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("WPA passphrase = None\n"));
    }
    if (buf->wpa_cfg.gk_rekey_time == 0)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Group re-key time = disabled\n"));
    }
    else
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Group re-key time = %ld second\n", buf->wpa_cfg.gk_rekey_time));
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("20/40 coex = %s\n",
           (buf->enable_2040coex) ? "enabled" : "disabled"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("wmm parameters:\n"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\tqos_info = 0x%x\n", buf->wmm_para.qos_info));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\tBE: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
           buf->wmm_para.ac_params[AC_BE].aci_aifsn.aifsn,
           buf->wmm_para.ac_params[AC_BE].ecw.ecw_max,
           buf->wmm_para.ac_params[AC_BE].ecw.ecw_min,
           buf->wmm_para.ac_params[AC_BE].tx_op_limit));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\tBK: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
           buf->wmm_para.ac_params[AC_BK].aci_aifsn.aifsn,
           buf->wmm_para.ac_params[AC_BK].ecw.ecw_max,
           buf->wmm_para.ac_params[AC_BK].ecw.ecw_min,
           buf->wmm_para.ac_params[AC_BK].tx_op_limit));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\tVI: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
           buf->wmm_para.ac_params[AC_VI].aci_aifsn.aifsn,
           buf->wmm_para.ac_params[AC_VI].ecw.ecw_max,
           buf->wmm_para.ac_params[AC_VI].ecw.ecw_min,
           buf->wmm_para.ac_params[AC_VI].tx_op_limit));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\tVO: AIFSN=%d, CW_MAX=%d CW_MIN=%d, TXOP=%d\n",
           buf->wmm_para.ac_params[AC_VO].aci_aifsn.aifsn,
           buf->wmm_para.ac_params[AC_VO].ecw.ecw_max,
           buf->wmm_para.ac_params[AC_VO].ecw.ecw_min,
           buf->wmm_para.ac_params[AC_VO].tx_op_limit));

    return;
}


#if 0
void print_bss_config(mlan_uap_bss_param * uap)
{
    int i;

    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("UAP Config Settings\n"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
             uap->mac_addr[0],uap->mac_addr[1],uap->mac_addr[2],
             uap->mac_addr[3],uap->mac_addr[4],uap->mac_addr[5]));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ssid: %s\n", uap->ssid.ssid));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("bcast_ssid_ctl: %d\n", uap->bcast_ssid_ctl));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("radio_ctl: %d\n", uap->radio_ctl));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("dtim_period: %d\n", uap->dtim_period));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("beacon_period: %d\n", uap->beacon_period));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("rates: "));
    for (i=0; i<MAX_DATA_RATES; i++) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("0x%X,", uap->rates[i]));
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("tx_data_rate: %d\n", uap->tx_data_rate));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("mcbc_data_rate: %d\n", uap->mcbc_data_rate));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("tx_power_level: %d\n", uap->tx_power_level));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("tx_antenna: %d\n", uap->tx_antenna));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("rx_antenna: %d\n", uap->rx_antenna));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("pkt_forward_ctl: %d\n", uap->pkt_forward_ctl));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("max_sta_count: %d\n", uap->max_sta_count));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("sta_ageout_timer: %d\n", uap->sta_ageout_timer));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ps_sta_ageout_timer: %d\n", uap->ps_sta_ageout_timer));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("rts_threshold: %d\n", uap->rts_threshold));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("frag_threshold: %d\n", uap->frag_threshold));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("retry_limit: %d\n", uap->retry_limit));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("pairwise_update_timeout: %d\n", uap->pairwise_update_timeout));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("pwk_retries: %d\n", uap->pwk_retries));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("groupwise_update_timeout: %d\n", uap->groupwise_update_timeout));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("gwk_retries: %d\n", uap->gwk_retries));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("preamble_type: %d\n", uap->preamble_type));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("band_cfg: %d\n", uap->band_cfg));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("channel: %d\n", uap->channel));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("channel list: "));
    for (i=0; i<uap->num_of_chan; i++) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), (" %d ",uap->chan_list[i].chan_number));
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("(%d),",uap->chan_list[i].band_config_type));
    }
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("\n"));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("auth_mode: %d\n", uap->auth_mode));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("protocol: %d\n", uap->protocol));
    DPRINTF((DBG_ERROR | DBG_OUTPUT), ("key_mgmt: %d\n\n", uap->key_mgmt));
}
#endif



/**
 * @brief Send BSS Configuration
 *
 * @param dev_name      Device Name 
 * @param p_config      BSS Configuration
 * @param p_uap_config_ds   Datastore Configuration 
 *
 * @return OK or FAIL.
 */
error_type_t
send_bss_config(char * dev_name, void * p_config, void * p_config_ds)
{
    struct ifreq ifr;
    mlan_uap_bss_param * bss;
    mlan_uap_bss_param * p_uap_config;
    wifi_intf_info_t * p_uap_config_ds;
    apcmdbuf_bss_configure * cmd_buf;
    t_u8 *buf = NULL;
    t_u16 cmd_len;
    t_u16 buf_len = 0;
    int i;
    bool sta_enabled;
    char full_ssid[MLAN_MAX_SSID_LENGTH + 1];
    error_type_t retval = FAIL;

    if ((p_config) && (p_config_ds)) {
        p_uap_config = (mlan_uap_bss_param *) p_config;
        p_uap_config_ds = (wifi_intf_info_t *) p_config_ds;
        buf_len = sizeof(apcmdbuf_bss_configure) + sizeof(mlan_uap_bss_param);
        buf = (t_u8 *) MEM_MALLOC(buf_len);
        if (buf) {
            memset((char *) buf, 0, buf_len);

            cmd_len = sizeof(apcmdbuf_bss_configure);
            cmd_buf = (apcmdbuf_bss_configure *) buf;
            bss = (mlan_uap_bss_param *) (buf + cmd_len);

            // Get the WiFi MAC address
            WlanGetMacAddr(WIFI_IFACE_NAME_UAP, (char *)p_uap_config->mac_addr);

            minNSprintf(full_ssid, sizeof(p_uap_config->ssid.ssid), "%s%s", 
                        p_uap_config_ds->uap.SSIDPrefix, p_uap_config_ds->SSID);
            p_uap_config->ssid.ssid_len = strlen(full_ssid);
            memcpy(p_uap_config->ssid.ssid, full_ssid, p_uap_config->ssid.ssid_len);

            // Set channel
            //REVISIT: The current wireless modules operates using a single channel for the 
            //uAP and the wireless STA (uAP follows the STA). However, if we do decide to move 
            //to a wireless module capable of supporting dedicated channels for the uAP and the STA, 
            //we should modify the following lines of code accordingly.
            wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_STATION, &sta_enabled);
            if (sta_enabled) 
            {
                int channel = WlanGetChannel();

                if ((channel > 0) && (channel < 255))
                {
                    p_uap_config->channel =  (t_u8) channel;
                }
 
                //Modify the nvram var accordingly. 
                //NOTE: At this point, we are not sure if the STA connection has succeeded or not OR if the STA channel
                //would move to something else. 
                //We will have to update this when we know that the Association is complete on the STA side. 
                wifi_intf_set_var_channel(WIFI_IFACE_ID_UAP, (uint8_t)p_uap_config->channel);
                 
            } else {
                p_uap_config->channel =  (t_u8) p_uap_config_ds->Channel;
            }

            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("BSS: channel %d\n", p_uap_config->channel));

            for (i=0; i<MAX_DATA_RATES ; i++) {
                p_uap_config->rates[i] = 0x0;
            }

            // Set band
            if (p_uap_config->channel > MAX_CHANNELS_BG)
            {
                p_uap_config->band_cfg = BAND_CONFIG_5GHZ;
                p_uap_config->rates[0] = 0x8c;
                p_uap_config->rates[1] = 0x98;
                p_uap_config->rates[2] = 0xB0;
                p_uap_config->rates[3] = 0x12;
                p_uap_config->rates[4] = 0x24;
                p_uap_config->rates[5] = 0x48;
                p_uap_config->rates[6] = 0x60;
                p_uap_config->rates[7] = 0x6c;
            }
            else
            {
                p_uap_config->band_cfg = 0;
                p_uap_config->rates[0] = 0x82;
                p_uap_config->rates[1] = 0x84;
                p_uap_config->rates[2] = 0x8b;
                p_uap_config->rates[3] = 0x96;
                p_uap_config->rates[4] = 0x0c;
                p_uap_config->rates[5] = 0x12;
                p_uap_config->rates[6] = 0x18; 
                p_uap_config->rates[7] = 0x24;
                p_uap_config->rates[8] = 0x30;
                p_uap_config->rates[9] = 0x48;
                p_uap_config->rates[10] = 0x60;
                p_uap_config->rates[11] = 0x6c;
            }

            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("BSS: band_cfg %d\n", p_uap_config->band_cfg));

            // Configure Security Mode
            switch(p_uap_config_ds->SecurityMode)
            {
                case WPA_PSK:
                case WPA2_PSK:
                    p_uap_config->auth_mode = AUTH_OPEN;
                    p_uap_config->key_mgmt  = KEY_MGMT_PSK;
                    p_uap_config->wpa_cfg.length = strlen((char*)p_uap_config_ds->WPAPassPhrase);
                    memcpy(p_uap_config->wpa_cfg.passphrase, p_uap_config_ds->WPAPassPhrase, p_uap_config->wpa_cfg.length);
                    p_uap_config->wpa_cfg.rsn_protection = 1;     // Make configurable?
                    p_uap_config->wpa_cfg.gk_rekey_time  = WIFI_UAP_WPA_GROUP_REKEY_TIMEOUT_DEFAULT; // Make configurable?
                    switch(p_uap_config_ds->WPAEncryptionMode)
                    {
                        case WPA_TKIP:
                            p_uap_config->protocol = PROTOCOL_WPA;
                            p_uap_config->wpa_cfg.pairwise_cipher_wpa = CIPHER_TKIP;
                            p_uap_config->wpa_cfg.group_cipher    = CIPHER_TKIP;
                            break;

                        case WPA_AES:
                            p_uap_config->protocol = PROTOCOL_WPA2;
                            p_uap_config->wpa_cfg.pairwise_cipher_wpa2 = CIPHER_AES_CCMP;
                            p_uap_config->wpa_cfg.group_cipher    = CIPHER_AES_CCMP;
                            break;

                        case WPA_TKIP_AND_AES: // Intentionally fall through
                        default:
                            p_uap_config->protocol = PROTOCOL_WPA2_MIXED;
                            p_uap_config->wpa_cfg.pairwise_cipher_wpa = CIPHER_TKIP | CIPHER_AES_CCMP;
                            p_uap_config->wpa_cfg.pairwise_cipher_wpa2 = CIPHER_TKIP | CIPHER_AES_CCMP;
                            p_uap_config->wpa_cfg.group_cipher    = CIPHER_TKIP;
                            break;
                    }
                    break;

                case WEP:
                {
                    wep_key *uap_config_keys[4] = {&p_uap_config->wep_cfg.key0,
                                                   &p_uap_config->wep_cfg.key1,
                                                   &p_uap_config->wep_cfg.key2,
                                                   &p_uap_config->wep_cfg.key3};

                    // TODO - Sanity check on auth_mode?  Auto probably doesn't make sense for uAP.
                    p_uap_config->auth_mode = p_uap_config_ds->WEPAuthMode;
                    p_uap_config->key_mgmt  = KEY_MGMT_NONE;
                    p_uap_config->protocol  = PROTOCOL_STATIC_WEP;

                    for (i = 0; i < 4; i++)
                    {
                        uap_config_keys[i]->key_index = i;
                        if (i == p_uap_config_ds->WEPCurKey)
                        {
                            uap_config_keys[i]->is_default = 1;
                        }
                        else
                        {
                            uap_config_keys[i]->is_default = 0;
                        }

                        if ((p_uap_config_ds->WEPKeyLength == 5) || (p_uap_config_ds->WEPKeyLength == 13))
                        {
                            // Alphanumeric key - copy it straight across
                            uap_config_keys[i]->length = p_uap_config_ds->WEPKeyLength;
                            memcpy(uap_config_keys[i]->key, p_uap_config_ds->WEPKey[i], p_uap_config_ds->WEPKeyLength);
                        }
                        else
                        {
                            // Hex key written in ASCII form -- convert to raw before sending it down
                            uap_config_keys[i]->length = (p_uap_config_ds->WEPKeyLength + 1) / 2;
                            hex_str_to_raw((char *)p_uap_config_ds->WEPKey[i], (char *)uap_config_keys[i]->key, sizeof(uap_config_keys[i]->key));
                        }
                    }
                    break;
                }

                case OPEN_SYSTEM:   // Intentionally fall through
                default:
                    p_uap_config->auth_mode = AUTH_OPEN;
                    p_uap_config->protocol = PROTOCOL_NO_SECURITY;
                    p_uap_config->key_mgmt = KEY_MGMT_NONE;
                    break;
            }

            memset(&p_uap_config->filter, 0, sizeof(p_uap_config->filter));
            if (p_uap_config_ds->uap.MACFilterMode != MAC_FILTER_MODE_DISABLED)
            {
                p_uap_config->filter.filter_mode = p_uap_config_ds->uap.MACFilterMode;
                for (i = 0; i < MAX_FILTERED_ADDRS; i++)
                {
                    if (!buff_is_all_zero(p_uap_config_ds->uap.MACFilterList[i], MAC_ADDR_LENGTH))
                    {
                        memcpy(p_uap_config->filter.mac_list[p_uap_config->filter.mac_count], 
                               p_uap_config_ds->uap.MACFilterList[i], MAC_ADDR_LENGTH);
                        p_uap_config->filter.mac_count++;
                    }
                }
            }

            // AP Parameters
            memcpy(bss, p_uap_config, sizeof(mlan_uap_bss_param));

            cmd_buf->action = MLAN_ACT_SET;

            /* Initialize the ifr structure */
            memset(&ifr, 0, sizeof(ifr));
            strncpy(ifr.ifr_ifrn.ifrn_name, dev_name, strlen(dev_name));
            ifr.ifr_ifru.ifru_data = (void *) cmd_buf;

            // Send BSS config
            if (WlanIoctl(UAP_BSS_CONFIG, &ifr) == 0) { 
                retval = OK;
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ERR: send_bss_config failure for %s\n", dev_name));
            }

            MEM_FREE_AND_NULL(buf);
        } else {
            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ERR:Cannot allocate buffer for send_bss_config!\n"));
        }
    } else {
        DPRINTF((DBG_SOFT | DBG_OUTPUT),("ERR:send_bss_config NULL pointer(s)!\n"));
    }

    return retval;
}

void get_bss_config(void)
{
    struct ifreq ifr;
    mlan_uap_bss_param * bss;
    apcmdbuf_bss_configure * cmd_buf;
    t_u8 *buf = NULL;
    t_u16 cmd_len;
    t_u16 buf_len = 0;

    buf_len = sizeof(apcmdbuf_bss_configure) + sizeof(mlan_uap_bss_param);
    buf = (t_u8 *) MEM_MALLOC(buf_len);
    if (buf) {
        memset((char *) buf, 0, buf_len);

        cmd_len = sizeof(apcmdbuf_bss_configure);
        cmd_buf = (apcmdbuf_bss_configure *) buf;
        bss = (mlan_uap_bss_param *) (buf + cmd_len);

        cmd_buf->action = MLAN_ACT_GET;

        /* Initialize the ifr structure */
        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_ifrn.ifrn_name, WIFI_IFACE_NAME_UAP, strlen(WIFI_IFACE_NAME_UAP));
        ifr.ifr_ifru.ifru_data = (void *) cmd_buf;

        if (WlanIoctl(UAP_BSS_CONFIG, &ifr) == 0)
        {
            print_bss_config((mlan_uap_bss_param *) bss);
        } else
        {
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("WLAN: FAILED to get uAP\n"));
        }

        MEM_FREE_AND_NULL(buf);
    }
}



/**
 * @brief Send BSS Control Ioctl
 *
 * @param dev_name  Device Name 
 * @param cmd       Command 
 *
 * @return OK or FAIL.
 */
error_type_t
send_bss_ctl_ioctl(char * dev_name, int cmd)
{
    struct ifreq ifr;
    t_u32 data;
    error_type_t retval = FAIL;

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, dev_name, strlen(dev_name));
    data = cmd;
    ifr.ifr_ifru.ifru_data = (void *) &data;

    /* Send BSS command */
    if (WlanIoctl(UAP_BSS_CTRL, &ifr) == 0) { 
        retval = OK;
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ERR: send_bss_ctl_ioctl failure\n"));
    }
    return retval;
}


/**
 * @brief BSS Configure
 *
 * @param dev_name      Device Name 
 * @param p_config_ds   Datastore Configuration 
 *
 * @return OK or FAIL.
 */
error_type_t
bss_configure(char * dev_name, void * p_config_ds)
{
    void *p_wfd_uap_config;
    error_type_t retval;

    // Create UAP config
    if ((retval = uap_create_config(&p_wfd_uap_config)) == OK) {
        // Configure UAP
        if ((retval = send_bss_config(dev_name, p_wfd_uap_config, p_config_ds)) != 0) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("bss_configure: FAILED to configure uAP for %s\n", dev_name));
        }
            
        // Delete UAP config
        uap_delete_config(&p_wfd_uap_config);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot create uap configuration bss_configure!\n"));
    }

    return retval;
}
