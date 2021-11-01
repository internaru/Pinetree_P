/** @file wps_def.h
 *  @brief This file contains definitions for WPS global information.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WPS_DEF_H_
#define _WPS_DEF_H_
#include <sys/types.h>
#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include "type_def.h"
//#include "common.h"
#include "trsocket.h"

#ifndef MIN
/** Macro to get minimun number */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
/** Macro to get maximun number */
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#endif

#ifndef NELEMENTS
/** Number of elements in aray x */
#define NELEMENTS(x)    (sizeof(x)/sizeof(x[0]))
#endif

/** Configuration file for initialization */
#define FILE_WPSINIT_CONFIG_NAME    "./config/wps_init_sta.conf"
/** Configuration file for multi-instance checking */
#define FILE_WPSRUN_CONFIG_NAME     "/tmp/wpsapp.pid"

/** Env variable to decide the run mode (Interactive/NonInteractive) */
#define WPS_NON_INTERACTIVE         "WPS_NON_INTERACTIVE"

#if 0  // Conflicts with other includes
/** Unsigned character, 1 byte */
#define u8     __u8
/** Unsigned character, 2 byte */
#define u16    __u16
/** Unsigned character, 4 byte */
#define u32    __u32
/** Unsigned character, 8 byte */
#define u64    __u64
/** Signed character, 1 byte */
#define s8     __s8
/** Signed character, 2 byte */
#define s16    __s16
/** Signed character, 4 byte */
#define s32    __s32
/** Signed character, 8 byte */
#define s64    __s64
#endif
/** Size: 1 Byte */
#define SIZE_1_BYTE         1
/** Size: 2 Byte */
#define SIZE_2_BYTES        2
/** Size: 4 Byte */
#define SIZE_4_BYTES        4
/** Size: 6 Byte */
#define SIZE_6_BYTES        6
/** Size: 8 Byte */
#define SIZE_8_BYTES        8
/** Size: 16 Byte */
#define SIZE_16_BYTES       16
/** Size: 20 Byte */
#define SIZE_20_BYTES       20
/** Size: 32 Byte */
#define SIZE_32_BYTES       32
/** Size: 64 Byte */
#define SIZE_64_BYTES       64
/** Size: 80 Byte */
#define SIZE_80_BYTES       80
/** Size: 128 Byte */
#define SIZE_128_BYTES      128
/** Size: 192 Byte */
#define SIZE_192_BYTES      192

#ifndef ETH_ALEN
/** Ethernet address length */
#define ETH_ALEN 6
#endif
/** IEEE 802.11 mode : infra */
#define IEEE80211_MODE_INFRA    0
/** IEEE 802.11 mode : IBSS */
#define IEEE80211_MODE_IBSS     1

/** IEEE 802.11 capability : ESS */
#define IEEE80211_CAP_ESS       0x0001
/** IEEE 802.11 capability : IBSS */
#define IEEE80211_CAP_IBSS      0x0002
/** IEEE 802.11 capability : PRIVACY */
#define IEEE80211_CAP_PRIVACY   0x0010

/** Maximum SSID length */
#define MAX_SSID_LEN         32
/** Max WPS_IE length */
#define MAX_WPS_IE_LEN       256
/** Max SSID WPA_IE length */
#define SSID_MAX_WPA_IE_LEN  40
/** Scan AP limit */
#define SCAN_AP_LIMIT        64

/** Return type: success */
#define WPS_STATUS_SUCCESS      (0)
/** Return type: failure */
#define WPS_STATUS_FAIL         (-1)

/** WPS Set */
#define WPS_SET                    1
/** WPS Cancel */
#define WPS_CANCEL                 0

/** Discovery phase : Start */
#define WPS_START_REG_DISCOVERY_PHASE   1
/** Discovery phase : End */
#define WPS_END_REG_DISCOVERY_PHASE     0

/** BSS type : STA */
#define BSS_TYPE_STA 0
/** BSS type : UAP */
#define BSS_TYPE_UAP 1

/** Skip scan and start adhoc network */
#define WPS_SKIP_SCAN_ADHOC_START       1
/** scan and join/start adhoc network */
#define WPS_SCAN_ADHOC_JOIN             0

/** Maximum event buffer size */
#define EVENT_MAX_BUF_SIZE            1024

/** Role enrollee after discovery */
#define IS_DISCOVERY_ENROLLEE(gpwps) \
             (((gpwps)->role == WPS_ADHOC_EXTENTION || \
               (gpwps)->role == WFD_ROLE) && \
               (gpwps)->discovery_role == WPS_ENROLLEE)
/** Role registrar after discovery */
#define IS_DISCOVERY_REGISTRAR(gpwps) \
             (((gpwps)->role == WPS_ADHOC_EXTENTION || \
               (gpwps)->role == WFD_ROLE) && \
               (gpwps)->discovery_role == WPS_REGISTRAR)

/** Max WFD_IE length */
#define MAX_WFD_IE_LEN                256
/** Maximum WFD devices in Find phase */
#define WFD_MAX_FIND_DEVICES          16
/** Maximum WFD device name length */
#define MAX_DEVICE_NAME_LEN           32
/** WPS Device Type Length */
#define WPS_DEVICE_TYPE_LEN           8

/** WPS input file location */
#define WPS_INPUT_FILE 				"/tmp/wps/wps_input"

/**
 * struct wpa_scan_result - Scan results
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 */
typedef struct driver_scan_result
{
    /** BSSID */
    u8 bssid[ETH_ALEN];
    /** SSID */
    u8 ssid[MAX_SSID_LEN];
    /** SSID length */
    size_t ssid_len;
    /** WPA IE */
    u8 wpa_ie[SSID_MAX_WPA_IE_LEN];
    /** WPA IE length */
    size_t wpa_ie_len;
    /** RSN Ie */
    u8 rsn_ie[SSID_MAX_WPA_IE_LEN];
    /** RSN IE length */
    size_t rsn_ie_len;
    /** WPS IE */
    u8 wps_ie[MAX_WPS_IE_LEN];
    /** WPS IE length */
    size_t wps_ie_len;
    /** WFD IE */
    u8 wfd_ie[MAX_WFD_IE_LEN];
    /** WFD IE length */
    size_t wfd_ie_len;
    /** Frequency of the channel in MHz (e.g., 2412 = channel 1) */
    int freq;
    /** Capability information field in host byte order */
    u16 caps;
    /** Signal quality */
    int qual;
    /** Noise level */
    int noise;
    /** Signal level */
    int level;
    /** Maximum supported rate */
    int maxrate;
} SCAN_RESULTS, *PSCAN_RESULTS;

typedef struct wps_ssid
{
    /** SSID */
    u8 ssid[MAX_SSID_LEN];
    /** SSID Length */
    size_t ssid_len;
    /** BSSID */
    u8 bssid[ETH_ALEN];

    /* 
     * mode - IEEE 802.11 operation mode (Infrastructure/IBSS)
     * 0 = infrastructure (Managed) mode, i.e., associate with an AP.
     * 1 = IBSS (ad-hoc, peer-to-peer)
     */
    /** IEEE 802.11 operation mode (Infrastructure/IBSS) */
    int mode;
} WPS_SSID, *PWPS_SSID;

typedef struct wps_l2_info
{
    /** Packet socket for EAPOL frames */
    int fd;
    /** Interface name */
    char ifname[17];
    /** Interface index */
    int ifindex;
    /** MAC Address */
    u8 my_mac_addr[6];
    /** callback handler */
    void (*rx_callback) (const u8 * src_addr, const u8 * buf, size_t len);
    /** Flag to decide whether to include layer 2 (Ethernet) header data buffers */
    int l2_hdr;
} WPS_L2_INFO;

/** Maximum no of channels in channel list */
#define MAX_CHANNELS 14

/** chan_list data structure */
typedef struct _channel_list
{
    /** Number of channel */
    u32 num_of_chan;
    /** Channel number*/
    u16 chan[MAX_CHANNELS];
} channel_list;

/** Wfd Device Information to display */
typedef struct wfd_device_results
{
    /** Group capability */
    u8 group_capability;
    /** Device Identifier ; Interface address */
    u8 device_id[ETH_ALEN];
    /** Device address in deviceInfo */
    u8 device_address[ETH_ALEN];
    /** WFD GO SSID */
    u8 go_ssid[MAX_SSID_LEN + 1];
    /** Intended address from Peer */
    u8 intended_address[ETH_ALEN];
    /** WPS Device Name  */
    u8 wps_device_name[MAX_DEVICE_NAME_LEN + 1];
    /** is device valid  */
    u8 is_valid;
    /** Operating channel */
    int op_channel;
    /** Listen channel */
    int listen_channel;
    /** Peer channel list */
    channel_list peer_channel_list;
    /** Last event timestamp */
    struct timeval timestamp;
} WFD_DEVICE_RESULTS, *PWFD_DEVICE_RESULTS;

typedef struct wfd_data
{
        /** Find Results */
    WFD_DEVICE_RESULTS find_results[WFD_MAX_FIND_DEVICES];
        /** number of find results */
    int num_find_results;
        /** current Device index */
    int dev_index;
    /** Self channel list */
    channel_list self_channel_list;
        /** Device found or not */
    int dev_found;
        /** Default scan */
    int default_scan;
} WFD_DATA, *PWFD_DATA;

typedef struct wps_data
{
    /** Layer 2 Info */
    WPS_L2_INFO *l2;
    /** event Info */
    int evt_fd;
    /** IO vector */
    void *evt_iov;
    /** event netlink header */
    void *evt_nl_head;
    /** event messsage header */
    void *evt_msg_head;
    /** event destination address */
    void *evt_dest_addr;
    /** WFD data pointer */
    WFD_DATA wfd_data;
    /** MAC Address */
    u8 my_mac_addr[ETH_ALEN];
    /** Interface name */
    char ifname[100];
    /** WPS non interactive FD */
    int non_int_fd;
    /** WPS non interactive input file */
    int non_int_input_file;
    /** BSS type */
    int bss_type;
    /** BSSID */
    u8 bssid[ETH_ALEN];
    /** WPS SSID */
    WPS_SSID current_ssid;
    /** Scan results */
    SCAN_RESULTS *scan_results;
    /** number of scan results */
    int num_scan_results;
    /** User abort flag */
    u8 userAbort;
    /** number of EAPOL packets received after the previous association event */
    int eapol_received;
    /** Flag letting G know whether or not to record state
     *  to the datastore */
    BOOL logStateToDataStore;
} WPS_DATA, *PWPS_DATA;

/** Action SET */
#define ACTION_GET                      (0)
/** Action GET */
#define ACTION_SET                      (1)

/** Maximum Persistent peers possible in a group */
#define WFD_MAX_PERSISTENT_PEERS    (2)
/** PSK max len */
#define WFD_PSK_LEN_MAX             (64)

/** User Persistent record format  */
typedef struct
{
    /* Index of record */
    s8 index;
    /* Device role */
    u8 dev_role;
    /* BSSID */
    u8 bssid[ETH_ALEN];
    /* Group device ID */
    u8 groupdevid[ETH_ALEN];
    /* SSID length */
    u8 ssid_len;
    /* SSID */
    u8 ssid[MAX_SSID_LEN];
    /* PSK length */
    u8 psk_len;
    /* PSK */
    u8 psk[WFD_PSK_LEN_MAX];
    /* Number of Peers */
    u8 num_peers;
    /* List of Peers */
    u8 peers[WFD_MAX_PERSISTENT_PEERS][ETH_ALEN];
} wfd_persistent_record;

/** Read/Write Persistent group record in FW */
int wfd_cfg_cmd_persistent_group_record(int action,
                                        wfd_persistent_record * prec);

/** Create supplicant config file */
int wps_create_supp_file(u8 * file_name, u8 * ssid, u32 ssid_len,
                         u8 * psk, u32 psk_len);

void wfd_cfg_cmd_provisioning_params(u16 action,
                                     u16 config_method, u16 dev_password);
void wfd_cfg_cmd_wps_params(u16 action, char *wps_param);

#endif /* _WPS_DEF_H_ */
