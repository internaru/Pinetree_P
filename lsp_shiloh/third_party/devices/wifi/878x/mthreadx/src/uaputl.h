/** @file  uaputl.h
 *
 *  @brief Header file for uaputl application
 *
 *  Copyright (C) 2009-2012, Marvell International Ltd.
 *  All Rights Reserved
 */
/************************************************************************
Change log:
    03/01/08: Initial creation
************************************************************************/

#ifndef _UAP_H
#define _UAP_H

/** Character, 1 byte */
typedef char t_s8;
/** Unsigned character, 1 byte */
typedef unsigned char t_u8;

/** Short integer */
typedef signed short t_s16;
/** Unsigned short integer */
typedef unsigned short t_u16;

/** Long integer */
typedef signed long t_s32;
/** Unsigned long integer */
typedef unsigned long t_u32;

/** Long long integer */
typedef signed long long t_s64;
/** Unsigned long integer */
typedef unsigned long long t_u64;

#if 0
#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN_SUPPORT
#endif
#endif

/** 16 bits byte swap */
#define swap_byte_16(x) \
  ((t_u16)((((t_u16)(x) & 0x00ffU) << 8) | \
         (((t_u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swap_byte_32(x) \
  ((t_u32)((((t_u32)(x) & 0x000000ffUL) << 24) | \
         (((t_u32)(x) & 0x0000ff00UL) <<  8) | \
         (((t_u32)(x) & 0x00ff0000UL) >>  8) | \
         (((t_u32)(x) & 0xff000000UL) >> 24)))

/** 64 bits byte swap */
#define swap_byte_64(x) \
  ((t_u64)((t_u64)(((t_u64)(x) & 0x00000000000000ffULL) << 56) | \
         (t_u64)(((t_u64)(x) & 0x000000000000ff00ULL) << 40) | \
         (t_u64)(((t_u64)(x) & 0x0000000000ff0000ULL) << 24) | \
         (t_u64)(((t_u64)(x) & 0x00000000ff000000ULL) <<  8) | \
         (t_u64)(((t_u64)(x) & 0x000000ff00000000ULL) >>  8) | \
         (t_u64)(((t_u64)(x) & 0x0000ff0000000000ULL) >> 24) | \
         (t_u64)(((t_u64)(x) & 0x00ff000000000000ULL) >> 40) | \
         (t_u64)(((t_u64)(x) & 0xff00000000000000ULL) >> 56) ))

#ifdef BIG_ENDIAN_SUPPORT
/** Convert from 16 bit little endian format to CPU format */
#define uap_le16_to_cpu(x) swap_byte_16(x)
/** Convert from 32 bit little endian format to CPU format */
#define uap_le32_to_cpu(x) swap_byte_32(x)
/** Convert from 64 bit little endian format to CPU format */
#define uap_le64_to_cpu(x) swap_byte_64(x)
/** Convert to 16 bit little endian format from CPU format */
#define uap_cpu_to_le16(x) swap_byte_16(x)
/** Convert to 32 bit little endian format from CPU format */
#define uap_cpu_to_le32(x) swap_byte_32(x)
/** Convert to 64 bit little endian format from CPU format */
#define uap_cpu_to_le64(x) swap_byte_64(x)

/** Convert APCMD header to little endian format from CPU format */
#define endian_convert_request_header(x);               \
    {                                                   \
        (x)->cmd_code = uap_cpu_to_le16((x)->cmd_code);   \
        (x)->size = uap_cpu_to_le16((x)->size);         \
        (x)->seq_num = uap_cpu_to_le16((x)->seq_num);     \
        (x)->result = uap_cpu_to_le16((x)->result);     \
    }

/** Convert APCMD header from little endian format to CPU format */
#define endian_convert_response_header(x);              \
    {                                                   \
        (x)->cmd_code = uap_le16_to_cpu((x)->cmd_code);   \
        (x)->size = uap_le16_to_cpu((x)->size);         \
        (x)->seq_num = uap_le16_to_cpu((x)->seq_num);     \
        (x)->result = uap_le16_to_cpu((x)->result);     \
    }

/** Convert TLV header to little endian format from CPU format */
#define endian_convert_tlv_header_out(x);           \
    {                                               \
        (x)->tag = uap_cpu_to_le16((x)->tag);       \
        (x)->length = uap_cpu_to_le16((x)->length); \
    }

/** Convert TLV header from little endian format to CPU format */
#define endian_convert_tlv_header_in(x);            \
    {                                               \
        (x)->tag = uap_le16_to_cpu((x)->tag);       \
        (x)->length = uap_le16_to_cpu((x)->length); \
    }

#else /* BIG_ENDIAN_SUPPORT */
/** Do nothing */
#define uap_le16_to_cpu(x) x
/** Do nothing */
#define uap_le32_to_cpu(x) x
/** Do nothing */
#define uap_le64_to_cpu(x) x
/** Do nothing */
#define uap_cpu_to_le16(x) x
/** Do nothing */
#define uap_cpu_to_le32(x) x
/** Do nothing */
#define uap_cpu_to_le64(x) x

/** Do nothing */
#define endian_convert_request_header(x)
/** Do nothing */
#define endian_convert_response_header(x)
/** Do nothing */
#define endian_convert_tlv_header_out(x)
/** Do nothing */
#define endian_convert_tlv_header_in(x)
#endif /* BIG_ENDIAN_SUPPORT */

/** uAP application version string */
#define UAP_VERSION         "1.11"
/** Host Command ioctl number */
#define UAPHOSTCMD          (SIOCDEVPRIVATE + 1)
/** Private command ID to send ioctl */
#define UAP_IOCTL_CMD		(SIOCDEVPRIVATE + 2)

/** Private command id to start/stop/reset bss */
#define UAP_BSS_CTRL        (SIOCDEVPRIVATE + 4)
/** BSS START */
#define UAP_BSS_START               0
/** BSS STOP */
#define UAP_BSS_STOP                1
/** BSS RESET */
#define UAP_BSS_RESET               2

/** Private command ID to BSS config */
#define UAP_BSS_CONFIG      (SIOCDEVPRIVATE + 6)

/** deauth station */
#define	UAP_STA_DEAUTH	    (SIOCDEVPRIVATE + 7)

/** uap get station list */
#define UAP_GET_STA_LIST    (SIOCDEVPRIVATE + 11)

/** Private command ID to set/get custom IE buffer */
#define UAP_CUSTOM_IE       (SIOCDEVPRIVATE + 13)

/** Max IE index per BSS */
#define MAX_MGMT_IE_INDEX   10

/** Default device name */
#define DEFAULT_DEV_NAME    "uap0"

/** Success */
#define UAP_SUCCESS     1
/** Failure */
#define UAP_FAILURE     0
/** MAC BROADCAST */
#define UAP_RET_MAC_BROADCAST   0x1FF
/** MAC MULTICAST */
#define UAP_RET_MAC_MULTICAST   0x1FE

/** Command is successful */
#define CMD_SUCCESS     0
/** Command fails */
#define CMD_FAILURE     -1

/** BSS start error : Invalid parameters */
#define BSS_FAILURE_START_INVAL     2
/** BSS start error : BSS already started */
#define BSS_FAILURE_START_REDUNDANT 3

/** BSS stop error : BSS already stopped */
#define BSS_FAILURE_STOP_REDUNDANT  2
/** BSS stop error : No active BSS */
#define BSS_FAILURE_STOP_INVAL      3

/** Maximum line length for config file */
#define MAX_LINE_LENGTH         240
/** Maximum command length */
#define MAX_CMD_LENGTH          100
/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER       (5 * 1024)

/** Maximum number of clients supported by AP */
#define MAX_NUM_CLIENTS         16
/** Maximum number of MAC addresses for one-shot filter modifications */
#define MAX_MAC_ONESHOT_FILTER  16
/** Maximum SSID length */
#define MAX_SSID_LENGTH         32
/** Maximum SSID length */
#define MIN_SSID_LENGTH         1
/** Maximum WPA passphrase length */
#define MAX_WPA_PASSPHRASE_LENGTH   64
/** Minimum WPA passphrase length */
#define MIN_WPA_PASSPHRASE_LENGTH   8
/** Maximum data rates */
#define MAX_DATA_RATES          14
/** Maximum length of lines in configuration file */
#define MAX_CONFIG_LINE         240
/** MSB bit is set if its a basic rate */
#define BASIC_RATE_SET_BIT      0x80
/** Maximum group key timer */
#define MAX_GRP_TIMER           86400
/** Maximum Retry Limit */
#define MAX_RETRY_LIMIT         14
/** Maximum preamble type value */
#define MAX_PREAMBLE_TYPE       2

/** Maximum TX Power Limit */
#define MAX_TX_POWER    20
/** Minimum TX Power Limit */
#define MIN_TX_POWER    0

/** Maximum channels */
#define MAX_CHANNELS    14
/** Maximum RTS threshold */
#define MAX_RTS_THRESHOLD   2347

/** Maximum fragmentation threshold */
#define MAX_FRAG_THRESHOLD 2346
/** Minimum fragmentation threshold */
#define MIN_FRAG_THRESHOLD 256

/** Maximum stage out time */
#define MAX_STAGE_OUT_TIME  864000
/** Minimum stage out time */
#define MIN_STAGE_OUT_TIME  300

/** Maximum DTIM period */
#define MAX_DTIM_PERIOD 100

/** Maximum BEACON period */
#define MAX_BEACON_PERIOD 4000

/** Minimum BEACON period */
#define MIN_BEACON_PERIOD 50

/** Maximum IE buffer length */
#define MAX_IE_BUFFER_LEN 256

/** Maximum custom IE count */
#define MAX_CUSTOM_IE_COUNT 4

/** Maximum number of rates allowed at a time */
#define MAX_RATES               12

/** Default wait period in seconds */
#define DEFAULT_WAIT_TIME       3

#ifdef __GNUC__
/** Structure packing begins */
#define PACK_START
/** Structure packeing end */
#define PACK_END  __attribute__ ((packed))
#else
/** Structure packing begins */
#define PACK_START   __packed
/** Structure packeing end */
#define PACK_END
#endif

#ifndef ETH_ALEN
/** MAC address length */
#define ETH_ALEN    6
#endif

/** Action field value : get */
#define ACTION_GET  0
/** Action field value : set */
#define ACTION_SET  1
/**
 * Hex or Decimal to Integer
 * @param   num string to convert into decimal or hex
 */
#define A2HEXDECIMAL(num)  \
    (strncasecmp("0x", (num), 2)?(unsigned int) strtoll((num),NULL,0):a2hex((num)))\

/**
 * Check of decimal or hex string
 * @param   num string
 */
#define IS_HEX_OR_DIGIT(num) \
    (strncasecmp("0x", (num), 2)?ISDIGIT((num)):ishexstring((num)))\

/** Find minimum value */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

/** Valid Input Commands */
typedef enum
{
    RDEEPROM,
    SCANCHANNELS,
    TXPOWER,
    PROTOCOL,
    CHANNEL,
    RATE,
    BROADCASTSSID,
    RTSTHRESH,
    FRAGTHRESH,
    DTIMPERIOD,
    RADIOCONTROL,
    TXDATARATE,
    MCBCDATARATE,
    PKTFWD,
    STAAGEOUTTIMER,
    PSSTAAGEOUTTIMER,
    AUTHMODE,
    GROUPREKEYTIMER,
    MAXSTANUM,
    BEACONPERIOD,
    RETRYLIMIT,
    RSNREPLAYPROT,
    PREAMBLETYPE,
    EAPOL_PWK_HSK,
    EAPOL_GWK_HSK,
} valid_inputs;

/** Message verbosity level */
enum
{ MSG_NONE, MSG_DEBUG, MSG_ALL };

/** oids_table */
typedef struct
{
    /** oid type */
    t_u16 type;
    /** oid len */
    t_u16 len;
    /** oid name */
    char *name;
} oids_table;

/** 4 byte header to store buf len*/
#define BUF_HEADER_SIZE     4

/** AP CMD header */
#define APCMDHEADER     /** Buf Size */         \
                        t_u32 buf_size;         \
                        /** Command Code */     \
                        t_u16 cmd_code;         \
                        /** Size */             \
                        t_u16 size;             \
                        /** Sequence Number */  \
                        t_u16 seq_num;          \
                        /** Result */           \
                        t_s16 result

/** TLV header */
#define TLVHEADER       /** Tag */      \
                        t_u16 tag;      \
                        /** Length */   \
                        t_u16 length

/* TLV Definitions */

/** TLV buffer header*/
typedef PACK_START struct _tlvbuf_header
{
    /** Header type */
    t_u16 type;
    /** Header length */
    t_u16 len;
    /** Data */
    t_u8 data[0];
} PACK_END tlvbuf_header;

/** Band config ACS mode */
#define BAND_CONFIG_ACS_MODE    0x40
/** BITMAP for ACS mode */
#define BITMAP_ACS_MODE         0x01
#define CHANNEL_MODE_MASK       0x01

/** TLV buffer : Channel Config */
typedef PACK_START struct _tlvbuf_channel_config
{
    /** Header */
    TLVHEADER;
    /** Band Configuration
     *
     * [7-6] Channel Selection Mode; 00 manual, 01 ACS
     * [5-4] 00 - no secondary channel,
     *       01 -- secondary channel is above
     *       03 -- secondary channel is below
     * [3-2] Channel Width; 00 20 MHz
     * [1-0] Band Info; 00 2.4 GHz
     */
    t_u8 band_config_type;
    /** Channel number */
    t_u8 chan_number;
} PACK_END tlvbuf_channel_config;

/** Channel List Entry */
typedef PACK_START struct _channel_list
{
    /** Band Config */
    t_u8 band_config_type;
    /** Channel Number */
    t_u8 chan_number;
    /** Reserved */
    t_u8 reserved1;
    /** Reserved */
    t_u16 reserved2;
    /** Reserved */
    t_u16 reserved3;
} PACK_END channel_list;

/** TLV buffer : Channel List */
typedef PACK_START struct _tlvbuf_channel_list
{
    /** Header */
    TLVHEADER;
    /** Channel List */
    channel_list chan_list[0];
} PACK_END tlvbuf_channel_list;

/** TLV buffer : AP MAC address */
typedef PACK_START struct _tlvbuf_ap_mac_address
{
    /** Header */
    TLVHEADER;
    /** AP MAC address */
    t_u8 ap_mac_addr[ETH_ALEN];
} PACK_END tlvbuf_ap_mac_address;

/** TLV buffer : SSID */
typedef PACK_START struct _tlvbuf_ssid
{
    /** Header */
    TLVHEADER;
    /** SSID */
    t_u8 ssid[0];
} PACK_END tlvbuf_ssid;

/** TLV buffer : Beacon period */
typedef PACK_START struct _tlvbuf_beacon_period
{
    /** Header */
    TLVHEADER;
    /** Beacon period */
    t_u16 beacon_period_ms;
} PACK_END tlvbuf_beacon_period;

/** TLV buffer : DTIM period */
typedef PACK_START struct _tlvbuf_dtim_period
{
    /** Header */
    TLVHEADER;
    /** DTIM period */
    t_u8 dtim_period;
} PACK_END tlvbuf_dtim_period;

/** TLV buffer : Channel */
typedef PACK_START struct _tlvbuf_phyparamdsset
{
    /** Header */
    TLVHEADER;
    /** Channel */
    t_u8 channel;
} PACK_END tlvbuf_phyparamdsset;

/** TLV buffer : Operational rates */
typedef PACK_START struct _tlvbuf_rates
{
    /** Header */
    TLVHEADER;
    /** Operational rates */
    t_u8 operational_rates[0];
} PACK_END tlvbuf_rates;

/** TLV buffer : Tx power */
typedef PACK_START struct _tlvbuf_tx_power
{
    /** Header */
    TLVHEADER;
    /** Tx power in dBm */
    t_u8 tx_power_dbm;
} PACK_END tlvbuf_tx_power;

/** TLV buffer : SSID broadcast control */
typedef PACK_START struct _tlvbuf_bcast_ssid_ctl
{
    /** Header */
    TLVHEADER;
    /** SSID broadcast control flag */
    t_u8 bcast_ssid_ctl;
} PACK_END tlvbuf_bcast_ssid_ctl;

/** TLV buffer : RSN replay protection */
typedef PACK_START struct _tlvbuf_rsn_replay_prot
{
    /** Header */
    TLVHEADER;
    /** RSN replay protection control flag */
    t_u8 rsn_replay_prot;
} PACK_END tlvbuf_rsn_replay_prot;

/** TLV buffer : Preamble control */
typedef PACK_START struct _tlvbuf_preamble_ctl
{
    /** Header */
    TLVHEADER;
    /** Preamble type */
    t_u8 preamble_type;
} PACK_END tlvbuf_preamble_ctl;

/** TLV buffer : Antenna control */
typedef PACK_START struct _tlvbuf_antenna_ctl
{
    /** Header */
    TLVHEADER;
    /** Antenna type */
    t_u8 which_antenna;
    /** Antenna mode */
    t_u8 antenna_mode;
} PACK_END tlvbuf_antenna_ctl;

/** TLV buffer : RTS threshold */
typedef PACK_START struct _tlvbuf_rts_threshold
{
    /** Header */
    TLVHEADER;
    /** RTS threshold */
    t_u16 rts_threshold;
} PACK_END tlvbuf_rts_threshold;

/** TLV buffer : Radio control */
typedef PACK_START struct _tlvbuf_radio_ctl
{
    /** Header */
    TLVHEADER;
    /** Radio control flag */
    t_u8 radio_ctl;
} PACK_END tlvbuf_radio_ctl;

/** TLV buffer : Tx data rate */
typedef PACK_START struct _tlvbuf_tx_data_rate
{
    /** Header */
    TLVHEADER;
    /** Tx data rate */
    t_u16 tx_data_rate;
} PACK_END tlvbuf_tx_data_rate;

/** TLV buffer : MCBC Data Rate */
typedef PACK_START struct _tlvbuf_mcbc_data_rate
{
    /** Header */
    TLVHEADER;
    /** MCBC data rate */
    t_u16 mcbc_datarate;
} PACK_END tlvbuf_mcbc_data_rate;

/** TLV buffer : Packet forward control */
typedef PACK_START struct _tlvbuf_pkt_fwd_ctl
{
    /** Header */
    TLVHEADER;
    /** Packet forwarding control flag */
    t_u8 pkt_fwd_ctl;
} PACK_END tlvbuf_pkt_fwd_ctl;

/** TLV buffer : STA MAC address filtering control */
typedef PACK_START struct _tlvbuf_sta_mac_addr_filter
{
    /** Header */
    TLVHEADER;
    /** Filter mode */
    t_u8 filter_mode;
    /** Number of STA MACs */
    t_u8 count;
    /** STA MAC addresses buffer */
    t_u8 mac_address[0];
} PACK_END tlvbuf_sta_mac_addr_filter;

/** TLV buffer : STA ageout timer */
typedef PACK_START struct _tlvbuf_sta_ageout_timer
{
    /** Header */
    TLVHEADER;
    /** STA ageout timer in ms */
    t_u32 sta_ageout_timer_ms;
} PACK_END tlvbuf_sta_ageout_timer;

/** TLV buffer : PS STA ageout timer */
typedef PACK_START struct _tlvbuf_ps_sta_ageout_timer
{
    /** Header */
    TLVHEADER;
    /** PS STA ageout timer in ms */
    t_u32 ps_sta_ageout_timer_ms;
} PACK_END tlvbuf_ps_sta_ageout_timer;

/** TLV buffer : max station number */
typedef PACK_START struct _tlvbuf_max_sta_num
{
    /** Header */
    TLVHEADER;
    /** max station number */
    t_u16 max_sta_num;
} PACK_END tlvbuf_max_sta_num;

/** TLV buffer : retry limit */
typedef PACK_START struct _tlvbuf_retry_limit
{
    /** Header */
    TLVHEADER;
    /** Retry limit */
    t_u8 retry_limit;
} PACK_END tlvbuf_retry_limit;

/** TLV buffer : pairwise key handshake timeout */
typedef PACK_START struct _tlvbuf_eapol_pwk_hsk_timeout
{
    /** Header */
    TLVHEADER;
    /** pairwise update timeout */
    t_u32 pairwise_update_timeout;
} PACK_END tlvbuf_eapol_pwk_hsk_timeout;

/** TLV buffer : pairwise key handshake number of retries */
typedef PACK_START struct _tlvbuf_eapol_pwk_hsk_retries
{
    /** Header */
    TLVHEADER;
    /** pairwise key retries */
    t_u32 pwk_retries;
} PACK_END tlvbuf_eapol_pwk_hsk_retries;

/** TLV buffer : groupwise key handshake timeout */
typedef PACK_START struct _tlvbuf_eapol_gwk_hsk_timeout
{
    /** Header */
    TLVHEADER;
    /** groupwise update timeout */
    t_u32 groupwise_update_timeout;
} PACK_END tlvbuf_eapol_gwk_hsk_timeout;

/** TLV buffer : groupwise key handshake number of retries */
typedef PACK_START struct _tlvbuf_eapol_gwk_hsk_retries
{
    /** Header */
    TLVHEADER;
    /** groupwise key retries */
    t_u32 gwk_retries;
} PACK_END tlvbuf_eapol_gwk_hsk_retries;

/** custom IE */
typedef PACK_START struct _custom_ie
{
    /** IE Index */
    t_u16 ie_index;
    /** Mgmt Subtype Mask */
    t_u16 mgmt_subtype_mask;
    /** IE Length */
    t_u16 ie_length;
    /** IE buffer */
    t_u8 ie_buffer[0];
} PACK_END custom_ie;

/** TLV buffer : custom IE */
typedef PACK_START struct _tlvbuf_custom_ie
{
    /** Header */
    TLVHEADER;
    /** custom IE data */
    custom_ie ie_data[0];
} PACK_END tlvbuf_custom_ie;

/* Bitmap for protocol to use */
/** No security */
#define PROTOCOL_NO_SECURITY        1
/** Static WEP */
#define PROTOCOL_STATIC_WEP         2
/** WPA */
#define PROTOCOL_WPA                8
/** WPA2 */
#define PROTOCOL_WPA2               32
/** WP2 Mixed */
#define PROTOCOL_WPA2_MIXED         40

/* Bitmap for unicast/bcast cipher type */
/** None */
#define CIPHER_NONE                 0
/** WEP 40 */
#define CIPHER_WEP_40               1
/** WEP 104 */
#define CIPHER_WEP_104              2
/** TKIP */
#define CIPHER_TKIP                 4
/** AES CCMP */
#define CIPHER_AES_CCMP             8
/** Valid cipher bitmap */
#define CIPHER_BITMAP               0x0c

/** TLV buffer : Authentication Mode */
typedef PACK_START struct _tlvbuf_auth_mode
{
    /** Header */
    TLVHEADER;
    /** Authentication Mode */
    t_u8 auth_mode;
} PACK_END tlvbuf_auth_mode;

/** TLV buffer : Security Protocol */
typedef PACK_START struct _tlvbuf_protocol
{
    /** Header */
    TLVHEADER;
    /** Security protocol */
    t_u16 protocol;
} PACK_END tlvbuf_protocol;

/** TLV buffer : cipher */
typedef PACK_START struct _tlvbuf_cipher
{
    /** Header */
    TLVHEADER;
    /** Pairwise cipher */
    t_u8 pairwise_cipher;
    /** Group cipher */
    t_u8 group_cipher;
} PACK_END tlvbuf_cipher;

/** TLV buffer : Group re-key time */
typedef PACK_START struct _tlvbuf_group_rekey_timer
{
    /** Header */
    TLVHEADER;
    /** Group rekey time in seconds */
    t_u32 group_rekey_time_sec;
} PACK_END tlvbuf_group_rekey_timer;

/** Key_mgmt_psk */
#define KEY_MGMT_NONE   0x04
/** Key_mgmt_none */
#define KEY_MGMT_PSK    0x02

/** TLV buffer : KeyMgmt */
typedef PACK_START struct _tlvbuf_akmp
{
    /** Header */
    TLVHEADER;
    /** KeyMgmt */
    t_u16 key_mgmt;
} PACK_END tlvbuf_akmp;

/** TLV buffer : Single WEP key */
typedef PACK_START struct _tlvbuf_wep_key
{
    /** Header */
    TLVHEADER;
    /** Key index */
    t_u8 key_index;
    /** Default key flag */
    t_u8 is_default;
    /** Key */
    t_u8 key[0];
} PACK_END tlvbuf_wep_key;

/** TLV buffer : WPA passphrase */
typedef PACK_START struct _tlvbuf_wpa_passphrase
{
    /** Header */
    TLVHEADER;
    /** WPA passphrase */
    t_u8 passphrase[0];
} PACK_END tlvbuf_wpa_passphrase;

/** TLV buffer : Fragmentation threshold */
typedef PACK_START struct _tlvbuf_frag_threshold
{
    /** Header */
    TLVHEADER;
    /** Fragmentation threshold */
    t_u16 frag_threshold;
} PACK_END tlvbuf_frag_threshold;

/* APCMD definitions */
/** APCMD buffer */
typedef PACK_START struct _apcmdbuf
{
    /** Header */
    APCMDHEADER;
}
PACK_END apcmdbuf;

/** APCMD header length */
#define APCMDHEADERLEN  (sizeof(apcmdbuf))

/** APCMD buffer : sys_info request */
typedef PACK_START struct _apcmdbuf_sys_info_request
{
    /** Header */
    APCMDHEADER;
} PACK_END apcmdbuf_sys_info_request;

/** APCMD buffer : sys_info response */
typedef PACK_START struct _apcmdbuf_sys_info_response
{
    /** Header */
    APCMDHEADER;
    /** System information buffer */
    t_u8 sys_info[64];
} PACK_END apcmdbuf_sys_info_response;

/** APCMD buffer : sys_reset */
typedef PACK_START struct _apcmdbuf_sys_reset
{
    /** Header */
    APCMDHEADER;
} PACK_END apcmdbuf_sys_reset;

/** APCMD buffer : sys_configure */
typedef PACK_START struct _apcmdbuf_sys_configure
{
    /** Header */
    APCMDHEADER;
    /** Action : GET or SET */
    t_u16 action;
} PACK_END apcmdbuf_sys_configure;

/** APCMD buffer : SNMP MIB */
typedef PACK_START struct _apcmdbuf_snmp_mib
{
    /** Header */
    APCMDHEADER;
    /** Action : GET or SET */
    t_u16 action;
} PACK_END apcmdbuf_snmp_mib;
/** APCMD buffer : bss_start */
typedef PACK_START struct _apcmdbuf_bss_start
{
    /** Header */
    APCMDHEADER;
} PACK_END apcmdbuf_bss_start;

/** APCMD buffer : bss_stop */
typedef PACK_START struct _apcmdbuf_bss_stop
{
    /** Header */
    APCMDHEADER;
} PACK_END apcmdbuf_bss_stop;

/** APCMD buffer : sta_deauth */
typedef PACK_START struct _APCMDBUF_STA_DEAUTH
{
    /** Header */
    APCMDHEADER;
    /** STA MAC address to deauthenticate */
    t_u8 sta_mac_address[ETH_ALEN];
} PACK_END APCMDBUF_STA_DEAUTH;

/** Reg TYPE*/
enum reg_commands
{
    CMD_MAC = 1,
    CMD_BBP,
    CMD_RF
};

/** APCMD buffer: Regrdwr */
typedef PACK_START struct _apcmdbuf_reg_rdwr
{
   /** Header */
    APCMDHEADER;
   /** Read or Write */
    t_u16 action;
   /** Register offset */
    t_u16 offset;
   /** Value */
    t_u32 value;
} PACK_END apcmdbuf_reg_rdwr;

/** sub-band type */
typedef PACK_START struct _ieeetypes_subband_set
{
    t_u8 first_chan;        /**< First channel */
    t_u8 no_of_chan;        /**< Number of channels */
    t_u8 max_tx_pwr;        /**< Maximum Tx power */
} PACK_END ieeetypes_subband_set_t;

/** country code length  used for 802.11D */
#define COUNTRY_CODE_LEN    3

/** MAX domain SUB-BAND*/
#define MAX_SUB_BANDS 40

/** Max Multi Domain Entries for G */
#define MAX_MULTI_DOMAIN_CAPABILITY_ENTRY_G 1

/** Max Multi Domain Entries for A */
#define MAX_MULTI_DOMAIN_CAPABILITY_ENTRY_A 31

/** Country code and Sub-band */
typedef PACK_START struct domain_param
{
    /** Header */
    TLVHEADER;
    t_u8 country_code[COUNTRY_CODE_LEN];         /**< Country code */
    ieeetypes_subband_set_t subband[0];          /**< Set of subbands */
} PACK_END domain_param_t;

/** HostCmd_CFG_80211D */
typedef PACK_START struct _apcmdbuf_cfg_80211d
{
    /** Header */
    APCMDHEADER;
    /** Action */
    t_u16 action;               /* 0 = ACT_GET; 1 = ACT_SET; */
    /** Domain parameters */
    domain_param_t domain;
} PACK_END apcmdbuf_cfg_80211d;

/** HostCmd_MEM_ACCESS */
typedef PACK_START struct _apcmdbuf_mem_access
{
    /** Header */
    APCMDHEADER;
    /** Action */
    t_u16 action;               /* 0 = ACT_GET; 1 = ACT_SET; */
    /** Reserved field */
    t_u16 reserved;
    /** Address */
    t_u32 address;
    /** Value */
    t_u32 value;
} PACK_END apcmdbuf_mem_access;

/** HostCmd_EEPROM_ACCESS */
typedef PACK_START struct _apcmdbuf_eeprom_access
{
    /** Header */
    APCMDHEADER;
    /** Action */
    t_u16 action;               /* 0 = ACT_GET; */
    /** Offset field */
    t_u16 offset;               /* Multiples of 4 */
    /** Byte count */
    t_u16 byte_count;           /* Multiples of 4 */
    /** Value */
    t_u8 value[1];
} PACK_END apcmdbuf_eeprom_access;

/** MLAN 802.11 MAC Address */
typedef t_u8 mlan_802_11_mac_addr[ETH_ALEN];

/** mlan_802_11_ssid data structure */
typedef struct _mlan_802_11_ssid
{
    /** SSID Length */
    t_u32 ssid_len;
    /** SSID information field */
    t_u8 ssid[MAX_SSID_LENGTH];
} mlan_802_11_ssid;

/** scan_chan_list data structure */
typedef struct _scan_chan_list
{
    /** Number of channel */
    t_u32 num_of_chan;
    /** Channel number*/
    t_u16 chan[MAX_CHANNELS];
} scan_chan_list;

/** mac_filter data structure */
typedef struct _mac_filter
{
    /** Mac filter mode */
    t_u16 filter_mode;
    /** Mac adress count */
    t_u16 mac_count;
    /** Mac address list */
    mlan_802_11_mac_addr mac_list[MAX_MAC_ONESHOT_FILTER];
} mac_filter;

/** wpa parameter */
typedef struct _wpa_param
{
    /** Pairwise cipher */
    t_u8 pairwise_cipher;
    /** Group cipher */
    t_u8 group_cipher;
    /** RSN replay protection */
    t_u8 rsn_protection;
    /** Passphrase length */
    t_u32 length;
    /** Passphrase */
    t_u8 passphrase[64];
    /** Group key rekey time */
    t_u32 gk_rekey_time;
} wpa_param;

/** wep key */
typedef struct _wep_key
{
    /** Key index 0-3 */
    t_u8 key_index;
    /** Is default */
    t_u8 is_default;
    /** Length */
    t_u16 length;
    /** Key data */
    t_u8 key[26];
} wep_key;

/** wep param */
typedef struct _wep_param
{
    /** Key 0 */
    wep_key key0;
    /** Key 1 */
    wep_key key1;
    /** Key 2 */
    wep_key key2;
    /** Key 3 */
    wep_key key3;
} wep_param;

/** BSS config structure */
typedef struct _bss_config_t
{
    /** AP mac addr */
    mlan_802_11_mac_addr mac_addr;
    /** SSID */
    mlan_802_11_ssid ssid;
    /** Broadcast ssid control */
    t_u8 bcast_ssid_ctl;
    /** Radio control: on/off */
    t_u8 radio_ctl;
    /** dtim period */
    t_u8 dtim_period;
    /** beacon period */
    t_u16 beacon_period;
    /** rates */
    t_u8 rates[MAX_DATA_RATES];
    /** Tx data rate */
    t_u16 tx_data_rate;
    /** multicast/broadcast data rate */
    t_u16 mcbc_data_rate;
    /** Tx power level */
    t_u8 tx_power_level;
    /** Tx antenna */
    t_u8 tx_antenna;
    /** Rx anteena */
    t_u8 rx_antenna;
    /** packet forward control */
    t_u8 pkt_forward_ctl;
    /** max station count */
    t_u16 max_sta_count;
    /** mac filter */
    mac_filter filter;
    /** station ageout timer in the unit of 100ms  */
    t_u32 sta_ageout_timer;
    /** PS station ageout timer in the unit of 100ms  */
    t_u32 ps_sta_ageout_timer;
    /** RTS threshold */
    t_u16 rts_threshold;
    /** fragmentation threshold */
    t_u16 frag_threshold;
    /**  retry_limit */
    t_u16 retry_limit;
    /**  pairwise update timeout in milliseconds */
    t_u32 pairwise_update_timeout;
    /**  pairwise handshake retries */
    t_u32 pwk_retries;
    /**  groupwise update timeout in milliseconds */
    t_u32 groupwise_update_timeout;
    /**  groupwise handshake retries */
    t_u32 gwk_retries;
    /** preamble type */
    t_u8 preamble_type;
    /** band cfg */
    t_u8 band_cfg;
    /** channel */
    t_u8 channel;
    /** auth mode */
    t_u16 auth_mode;
    /** encryption protocol */
    t_u16 protocol;
    /** key managment type */
    t_u16 key_mgmt;
    /** wep param */
    wep_param wep_cfg;
    /** wpa param */
    wpa_param wpa_cfg;
    /** Mgmt IE passthru mask */
    t_u32 mgmt_ie_passthru_mask;
    /* 
     *  11n HT Cap 
     */
    /** HT Capabilities Info field */
    t_u16 ht_cap_info;
    /** A-MPDU Parameters field */
    t_u8 ampdu_param;
    /** Supported MCS Set field */
    t_u8 supported_mcs_set[16];
    /** HT Extended Capabilities field */
    t_u16 ht_ext_cap;
    /** Transmit Beamforming Capabilities field */
    t_u32 tx_bf_cap;
    /** Antenna Selection Capability field */
    t_u8 asel;
    /** Enable 20/40 coex */
    t_u8 enable_2040coex;
    /** key managment operation */
    t_u16 key_mgmt_operation;
    /** BSS status */
    t_u16 bss_status;
#ifdef WIFI_DIRECT_SUPPORT
    /* pre shared key */
    t_u8 psk[32];
#endif                          /* WIFI_DIRECT_SUPPORT */
    /** Number of channels in scan_channel_list */
    t_u32 num_of_chan;
    /** scan channel list in ACS mode */
    scan_chan_list chan_list[MAX_CHANNELS];
    /** Wmm parameters */
    WmmParameter_t wmm_para;
} bss_config_t;

/** station info */
typedef struct _sta_info
{
    /** STA MAC address */
    t_u8 mac_address[ETH_ALEN];
    /** Power mfg status */
    t_u8 power_mfg_status;
    /** RSSI */
    t_s8 rssi;
} sta_info;

/** sta_list structure */
typedef struct _sta_list
{
    /** station count */
    t_u16 sta_count;
    /** station list */
    sta_info info[MAX_NUM_CLIENTS];
} sta_list;

/** mlan_deauth_param */
typedef struct _deauth_param
{
    /** STA mac addr */
    t_u8 mac_addr[ETH_ALEN];
    /** deauth reason */
    t_u16 reason_code;
} deauth_param;

/** APCMD buffer : bss_configure */
typedef PACK_START struct _apcmdbuf_bss_configure
{
    /** Action : GET or SET */
    t_u32 action;
} PACK_END apcmdbuf_bss_configure;

/** Max EEPROM length */
#define MAX_EEPROM_LEN         20

/** Subcmd id for global flag */
#define DEBUG_SUBCOMMAND_GMODE          1
/** Subcmd id for Majorid mask */
#define DEBUG_SUBCOMMAND_MAJOREVTMASK   2
/** Subcmd id to trigger a scan */
#define DEBUG_SUBCOMMAND_CHANNEL_SCAN   3

/** Channel scan entry for each channel */
typedef PACK_START struct _channel_scan_entry_t
{
    /** Channel Number */
    t_u8 chan_num;
    /** Number of APs */
    t_u8 num_of_aps;
    /** CCA count */
    t_u32 cca_count;
    /** Duration */
    t_u32 duration;
    /** Channel weight */
    t_u32 channel_weight;
} PACK_END channel_scan_entry_t;

/** Channel scan entry */
typedef PACK_START struct _channel_scan_entry
{
    /** Number of channels */
    t_u8 num_channels;
    /** Channel scan entry */
    channel_scan_entry_t cst[0];
} PACK_END channel_scan_entry;

/** debug_config_t */
typedef PACK_START union
{
    /** used in all new debug commands */
    t_u32 value;
    /** Used in DEBUG_SUBCOMMAND_GMODE */
    t_u8 global_debug_mode;
    /** Used in DEBUG_SUBCOMMAND_MAJOREVTMASK */
    t_u32 debug_major_id_mask;
    /** Used in DEBUG_SUBCOMMAND_CHANNEL_SCAN */
    channel_scan_entry cs_entry;
} PACK_END debug_config_t;

/** HostCmd_SYS_DEBUG */
typedef PACK_START struct _apcmdbuf_sys_debug
{
    /** Header */
    APCMDHEADER;
    /** Action */
    t_u16 action;               /* 0 = ACT_GET; 1 = ACT_SET; */
    /** Sub command */
    t_u32 subcmd;
    /** debug parameter */
    debug_config_t debug_config;
} PACK_END apcmdbuf_sys_debug;

/** HostCmd_CFG_DATA */
typedef PACK_START struct _apcmdbuf_cfg_data
{
    /** Header */
    APCMDHEADER;
    /** Action */
    t_u16 action;
    /** Type */
    t_u16 type;
    /** Data length */
    t_u16 data_len;
    /** Data */
    t_u8 data[0];
} PACK_END apcmdbuf_cfg_data;

/** Maximum size of set/get configurations */
#define MAX_CFG_DATA_SIZE   2000        /* less than MRVDRV_SIZE_OF_CMD_BUFFER */

/** Host Command ID bit mask (bit 11:0) */
#define HostCmd_CMD_ID_MASK             0x0fff
/** APCMD response check */
#define APCMD_RESP_CHECK            0x8000

/* AP CMD IDs */
/** APCMD : sys_info */
#define APCMD_SYS_INFO              0x00ae
/** APCMD : sys_reset */
#define APCMD_SYS_RESET             0x00af
/** APCMD : sys_configure */
#define APCMD_SYS_CONFIGURE         0x00b0
/** APCMD : bss_start */
#define APCMD_BSS_START             0x00b1
/** APCMD : bss_stop */
#define APCMD_BSS_STOP              0x00b2
/** APCMD : sta_list */
#define APCMD_STA_LIST              0x00b3
/** APCMD : sta_deauth */
#define APCMD_STA_DEAUTH            0x00b5
/** SNMP MIB SET/GET */
#define HostCmd_SNMP_MIB            0x0016
/** Read/Write Mac register */
#define HostCmd_CMD_MAC_REG_ACCESS  0x0019
/** Read/Write BBP register */
#define HostCmd_CMD_BBP_REG_ACCESS  0x001a
/** Read/Write RF register */
#define HostCmd_CMD_RF_REG_ACCESS   0x001b
/** Host Command ID : EEPROM access */
#define HostCmd_EEPROM_ACCESS       0x0059
/** Host Command ID : Memory access */
#define HostCmd_CMD_MEM_ACCESS      0x0086
/** Host Command ID : 802.11D configuration */
#define HostCmd_CMD_802_11D_DOMAIN_INFO      0x005b
/** Host Command ID : Configuration data */
#define HostCmd_CMD_CFG_DATA        0x008f
/** Host Command ID:  SYS_DEBUG */
#define APCMD_SYS_DEBUG             0x00db

/** Oid for 802.11D enable/disable */
#define OID_80211D_ENABLE           0x0009

/* TLV IDs */
/** TLV : Base */
#define PROPRIETARY_TLV_BASE_ID         0x0100

/**TLV: Domain type */
#define TLV_TYPE_DOMAIN                 0x0007

/** TLV : SSID */
#define MRVL_SSID_TLV_ID                0x0000
/** TLV : Operational rates */
#define MRVL_RATES_TLV_ID               0x0001
/** TLV : Channel */
#define MRVL_PHYPARAMDSSET_TLV_ID       0x0003
/** TLV type : Scan Channels list */
#define MRVL_CHANNELLIST_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 1)
/** TLV type : Authentication type */
#define MRVL_AUTH_TLV_ID                (PROPRIETARY_TLV_BASE_ID + 31)
/** TLV Id : Channel Config */
#define MRVL_CHANNELCONFIG_TLV_ID       (PROPRIETARY_TLV_BASE_ID + 42)
/** TLV : AP MAC address */
#define MRVL_AP_MAC_ADDRESS_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 43)
/** TLV : Beacon period */
#define MRVL_BEACON_PERIOD_TLV_ID       (PROPRIETARY_TLV_BASE_ID + 44)
/** TLV : DTIM period */
#define MRVL_DTIM_PERIOD_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 45)
/** TLV : Tx power */
#define MRVL_TX_POWER_TLV_ID            (PROPRIETARY_TLV_BASE_ID + 47)
/** TLV : SSID broadcast control */
#define MRVL_BCAST_SSID_CTL_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 48)
/** TLV : Preamble control */
#define MRVL_PREAMBLE_CTL_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 49)
/** TLV : Antenna control */
#define MRVL_ANTENNA_CTL_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 50)
/** TLV : RTS threshold */
#define MRVL_RTS_THRESHOLD_TLV_ID       (PROPRIETARY_TLV_BASE_ID + 51)
/** TLV : Radio control */
#define MRVL_RADIO_CTL_TLV_ID           (PROPRIETARY_TLV_BASE_ID + 52)
/** TLV : Tx data rate */
#define MRVL_TX_DATA_RATE_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 53)
/** TLV : Packet forwarding control */
#define MRVL_PKT_FWD_CTL_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 54)
/** TLV : STA information */
#define MRVL_STA_INFO_TLV_ID            (PROPRIETARY_TLV_BASE_ID + 55)
/** TLV : STA MAC address filter */
#define MRVL_STA_MAC_ADDR_FILTER_TLV_ID (PROPRIETARY_TLV_BASE_ID + 56)
/** TLV : STA ageout timer */
#define MRVL_STA_AGEOUT_TIMER_TLV_ID    (PROPRIETARY_TLV_BASE_ID + 57)
/** TLV : WEP keys */
#define MRVL_WEP_KEY_TLV_ID             (PROPRIETARY_TLV_BASE_ID + 59)
/** TLV : WPA passphrase */
#define MRVL_WPA_PASSPHRASE_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 60)
/** TLV type : protocol TLV */
#define MRVL_PROTOCOL_TLV_ID            (PROPRIETARY_TLV_BASE_ID + 64)
/** TLV type : AKMP TLV */
#define MRVL_AKMP_TLV_ID                (PROPRIETARY_TLV_BASE_ID + 65)
/** TLV type : Cipher TLV */
#define MRVL_CIPHER_TLV_ID              (PROPRIETARY_TLV_BASE_ID + 66)
/** TLV : Fragment threshold */
#define MRVL_FRAG_THRESHOLD_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 70)
/** TLV : Group rekey timer */
#define MRVL_GRP_REKEY_TIME_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 71)
/**TLV: Max Station number */
#define MRVL_MAX_STA_CNT_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 85)
/**TLV: Retry limit */
#define MRVL_RETRY_LIMIT_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 93)
/**TLV: MCBC data rate */
#define MRVL_MCBC_DATA_RATE_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 98)
/**TLV: RSN replay protection */
#define MRVL_RSN_REPLAY_PROT_TLV_ID     (PROPRIETARY_TLV_BASE_ID + 100)
/** TLV :Pairwise Handshake Timeout */
#define MRVL_EAPOL_PWK_HSK_TIMEOUT_TLV_ID   (PROPRIETARY_TLV_BASE_ID + 117)
/** TLV :Pairwise Handshake Retries */
#define MRVL_EAPOL_PWK_HSK_RETRIES_TLV_ID   (PROPRIETARY_TLV_BASE_ID + 118)
/** TLV :Groupwise Handshake Timeout */
#define MRVL_EAPOL_GWK_HSK_TIMEOUT_TLV_ID   (PROPRIETARY_TLV_BASE_ID + 119)
/** TLV :Groupwise Handshake Retries */
#define MRVL_EAPOL_GWK_HSK_RETRIES_TLV_ID   (PROPRIETARY_TLV_BASE_ID + 120)
/** TLV: Management IE list */
#define MRVL_MGMT_IE_LIST_TLV_ID          (PROPRIETARY_TLV_BASE_ID + 105)
/** TLV : PS STA ageout timer */
#define MRVL_PS_STA_AGEOUT_TIMER_TLV_ID    (PROPRIETARY_TLV_BASE_ID + 123)

/** Function Prototype Declaration */
#endif /* _UAP_H */
