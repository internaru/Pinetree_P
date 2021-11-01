/** @file moal_main.h
  *
  * @brief This file contains wlan driver specific defines etc.
  * 
  * Copyright (C) 2008-2012, Marvell International Ltd.  
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#ifndef _MOAL_MAIN_H
#define _MOAL_MAIN_H

#include    "os_headers.h"
#include    "dprintf.h"
#include    "wlan_thread.h"
#include    "lassert.h"

/* warnfix for FS redefination if any? */
#ifdef FS
#undef FS
#endif


#include    "mlan.h"
#include 	"moal_shim.h"
#include	"moal_wext.h"
#include	"moal_priv.h"

/** Define BOOLEAN */
typedef t_u8 BOOLEAN;

/** Driver version */
extern const char driver_version[];

/** Private structure for MOAL */
typedef struct _moal_private moal_private;
/** Handle data structure for MOAL  */
typedef struct _moal_handle moal_handle;
/** firmware struct */
typedef struct _firmware firmware;

/** Hardware status codes */
typedef enum _MOAL_HARDWARE_STATUS
{
    HardwareStatusReady,
    HardwareStatusInitializing,
    HardwareStatusFwReady,
    HardwareStatusReset,
    HardwareStatusClosing,
    HardwareStatusNotReady
} MOAL_HARDWARE_STATUS;

/** moal_wait_option */
enum
{
    MOAL_NO_WAIT,
    MOAL_IOCTL_WAIT,
    MOAL_CMD_WAIT,
    MOAL_PROC_WAIT,
    MOAL_WSTATS_WAIT
};

/** moal_main_state */
enum
{
    MOAL_STATE_IDLE,
    MOAL_RECV_INT,
    MOAL_ENTER_WORK_QUEUE,
    MOAL_START_MAIN_PROCESS,
    MOAL_END_MAIN_PROCESS
};

/** HostCmd_Header */
typedef struct _HostCmd_Header
{
    /** Command */
    t_u16 command;
    /** Size */
    t_u16 size;
} HostCmd_Header;

#ifndef MIN
/** Find minimum */
#define MIN(a,b)		((a) < (b) ? (a) : (b))
#endif

#define IN6PTON_XDIGIT		0x00010000
#define IN6PTON_DIGIT		0x00020000
#define IN6PTON_COLON_MASK	0x00700000
#define IN6PTON_COLON_1		0x00100000      /* single : requested */
#define IN6PTON_COLON_2		0x00200000      /* second : requested */
#define IN6PTON_COLON_1_2	0x00400000      /* :: requested */
#define IN6PTON_DOT		0x00800000      /* . */
#define IN6PTON_DELIM		0x10000000
#define IN6PTON_NULL		0x20000000      /* first/tail */
#define IN6PTON_UNKNOWN		0x40000000

static inline int
xdigit2bin(char c, int delim)
{
    if (c == delim || c == '\0')
        return IN6PTON_DELIM;
    if (c == ':')
        return IN6PTON_COLON_MASK;
    if (c == '.')
        return IN6PTON_DOT;
    if (c >= '0' && c <= '9')
        return (IN6PTON_XDIGIT | IN6PTON_DIGIT | (c - '0'));
    if (c >= 'a' && c <= 'f')
        return (IN6PTON_XDIGIT | (c - 'a' + 10));
    if (c >= 'A' && c <= 'F')
        return (IN6PTON_XDIGIT | (c - 'A' + 10));
    if (delim == -1)
        return IN6PTON_DELIM;
    return IN6PTON_UNKNOWN;
}

static inline int
in4_pton(const char *src, int srclen, u8 * dst, int delim, const char **end)
{
    const char *s;
    u8 *d;
    u8 dbuf[4];
    int ret = 0;
    int i;
    int w = 0;

    if (srclen < 0)
        srclen = strlen(src);
    s = src;
    d = dbuf;
    i = 0;
    while (1) {
        int c;
        c = xdigit2bin(srclen > 0 ? *s : '\0', delim);
        if (!
            (c &
             (IN6PTON_DIGIT | IN6PTON_DOT | IN6PTON_DELIM |
              IN6PTON_COLON_MASK))) {
            goto out;
        }
        if (c & (IN6PTON_DOT | IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
            if (w == 0)
                goto out;
            *d++ = w & 0xff;
            w = 0;
            i++;
            if (c & (IN6PTON_DELIM | IN6PTON_COLON_MASK)) {
                if (i != 4)
                    goto out;
                break;
            }
            goto cont;
        }
        w = (w * 10) + c;
        if ((w & 0xffff) > 255) {
            goto out;
        }
      cont:
        if (i >= 4)
            goto out;
        s++;
        srclen--;
    }
    ret = 1;
    memcpy(dst, dbuf, sizeof(dbuf));
  out:
    if (end)
        *end = s;
    return ret;
}

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__ __attribute__ ((packed))
#endif

/** Initialize semaphore */
#define MOAL_INIT_SEMAPHORE(x)    	OS_INIT_SEMAPHORE(x)
/** Initialize semaphore */
#define MOAL_INIT_SEMAPHORE_LOCKED(x) 	OS_INIT_MUTEX_LOCKED(x)
/** Acquire semaphore and with blocking */
#define MOAL_ACQ_SEMAPHORE_BLOCK(x)	OS_ACQ_SEMAPHORE_BLOCK(x)
/** Acquire semaphore without blocking */
#define MOAL_ACQ_SEMAPHORE_NOBLOCK(x)	down_trylock(x)
/** Release semaphore */
#define MOAL_REL_SEMAPHORE(x) 		OS_REL_SEMAPHORE(x)
/** Delete semaphore */
#define MOAL_FREE_SEMAPHORE(x)   	OS_FREE_LOCK(x)
/** Request FW timeout in second */
#define REQUEST_FW_TIMEOUT		30

/** Max loop count (* 100ms) for waiting device ready at init time */
#define MAX_WAIT_DEVICE_READY_COUNT	50

/** Default watchdog timeout */
#define MRVDRV_DEFAULT_WATCHDOG_TIMEOUT (5 * HZ)

#ifdef UAP_SUPPORT
/** Default watchdog timeout 
    Increase the value to avoid kernel Tx timeout message in case 
    station in PS mode or left.
    The default value of PS station ageout timer is 40 seconds. 
    Hence, the watchdog timer is set to a value higher than it.
*/
#define MRVDRV_DEFAULT_UAP_WATCHDOG_TIMEOUT (41 * HZ)
#endif

/** Threshold value of number of times the Tx timeout happened */
#define NUM_TX_TIMEOUT_THRESHOLD      5

/** 10 seconds */
#define MOAL_TIMER_10S                10000
/** 5 seconds */
#define MOAL_TIMER_5S                 5000
/** 1 second */
#define MOAL_TIMER_1S                 1000

/** Default value of re-assoc timer */
#define REASSOC_TIMER_DEFAULT         500

/** Netlink protocol number */
#define NETLINK_MARVELL     (MAX_LINKS - 1)
/** Netlink maximum payload size */
#define NL_MAX_PAYLOAD      1024
/** Netlink multicast group number */
#define NL_MULTICAST_GROUP  1

/** MAX Tx Pending count */
#define MAX_TX_PENDING    	100

/** LOW Tx Pending count */
#define LOW_TX_PENDING      80

/** Offset for subcommand */
#define SUBCMD_OFFSET       4

/** Offset for TOS field in the IP header */
#define IPTOS_OFFSET 5

/** Offset for DSCP in the tos field */
#define DSCP_OFFSET 2

/** wait_queue structure */
typedef struct _wait_queue
{
        /** Pointer to wait_queue_head */
    wait_queue_head_t *wait;
        /** Pointer to wait condition */
    t_u16 *condition;
        /** Start time */
    t_u32 start_time;
        /** Status from MLAN */
    mlan_status status;
} wait_queue, *pwait_queue;

/** Auto Rate */
#define AUTO_RATE 0xFF

#ifdef STA_SUPPORT
/** Driver mode STA bit */
#define DRV_MODE_STA       MBIT(0)
/** Maximum STA BSS */
#define MAX_STA_BSS        1
/** Default STA BSS */
#define DEF_STA_BSS        1
#endif
#ifdef UAP_SUPPORT
/** Driver mode uAP bit */
#define DRV_MODE_UAP       MBIT(1)
/** Maximum uAP BSS */
#define MAX_UAP_BSS        1
/** Default uAP BSS */
#define DEF_UAP_BSS        1
#endif
#if defined(WIFI_DIRECT_SUPPORT)
/** Driver mode WIFIDIRECT bit */
#define DRV_MODE_WIFIDIRECT       MBIT(2)
/** Maximum WIFIDIRECT BSS */
#define MAX_WIFIDIRECT_BSS        1
/** Default WIFIDIRECT BSS */
#define DEF_WIFIDIRECT_BSS        1
#endif /* WIFI_DIRECT_SUPPORT && V14_FEATURE */

typedef struct _moal_drv_mode
{
    /** driver mode */
    t_u16 drv_mode;
    /** total number of interfaces */
    t_u16 intf_num;
    /** attribute of bss */
    mlan_bss_attr *bss_attr;
    /** name of firmware image */
    char *fw_name;
} moal_drv_mode;

#ifdef PROC_DEBUG
/** Debug data */
struct debug_data
{
    /** Name */
    char name[32];
    /** Size */
    t_u32 size;
    /** Address */
    t_u32 addr;
};

/** Private debug data */
struct debug_data_priv
{
    /** moal_private handle */
    moal_private *priv;
    /** Debug items */
    struct debug_data *items;
    /** numbre of item */
    int num_of_items;
};
#endif

/** Maximum IP address buffer length */
#define IPADDR_MAX_BUF          20
/** IP address operation: Remove */
#define IPADDR_OP_REMOVE        0

/** Private structure for MOAL */
struct _moal_private
{
        /** Handle structure */
    moal_handle *phandle;
        /** Tx timeout count */
    t_u32 num_tx_timeout;
        /** BSS index */
    t_u8 bss_index;
        /** BSS type */
    t_u8 bss_type;
        /** BSS role */
    t_u8 bss_role;
        /** MAC address information */
    t_u8 current_addr[ETH_ALEN];
        /** Media connection status */
    BOOLEAN media_connected;
#ifdef UAP_SUPPORT
        /** uAP started or not */
    BOOLEAN bss_started;
#endif
        /** Net device pointer */
    struct net_device *netdev;
        /** Net device statistics structure */
    struct net_device_stats stats;

        /** IOCTL wait queue token */
    t_u16 ioctl_wait_q_woken;
        /** ioctl wait queue */
    wait_queue_head_t ioctl_wait_q __ATTRIB_ALIGN__;
        /** Cmd wait queue token */
    t_u16 cmd_wait_q_woken;
        /** CMD wait queue */
    wait_queue_head_t cmd_wait_q __ATTRIB_ALIGN__;
#ifdef CONFIG_PROC_FS
        /** Proc entry */
    struct proc_dir_entry *proc_entry;
        /** Proc entry name */
    t_s8 proc_entry_name[IFNAMSIZ];
        /** Proc wait queue token */
    t_u16 proc_wait_q_woken;
        /** PROC wait queue */
    wait_queue_head_t proc_wait_q __ATTRIB_ALIGN__;
#endif                          /* CONFIG_PROC_FS */
#ifdef STA_SUPPORT
        /** Nickname */
    t_u8 nick_name[16];
        /** AdHoc link sensed flag */
    BOOLEAN is_adhoc_link_sensed;
        /** Current WEP key index */
    t_u16 current_key_index;
#ifdef REASSOCIATION
    mlan_ssid_bssid prev_ssid_bssid;
        /** Re-association required */
    BOOLEAN reassoc_required;
        /** Flag of re-association on/off */
    BOOLEAN reassoc_on;
#endif                          /* REASSOCIATION */
        /** Report scan result */
    t_u8 report_scan_result;
        /** wpa_version */
    t_u8 wpa_version;
        /** key mgmt */
    t_u8 key_mgmt;
        /** rx_filter */
    t_u8 rx_filter;
#endif                          /* STA_SUPPORT */
        /** Rate index */
    t_u16 rate_index;
        /** IW statistics */
    struct iw_statistics w_stats;
        /** w_stats wait queue token */
    t_u16 w_stats_wait_q_woken;
        /** w_stats wait queue */
    wait_queue_head_t w_stats_wait_q __ATTRIB_ALIGN__;
    /** Pairwise Cipher used for WPA/WPA2 mode */
    t_u16 pairwise_cipher;
    /** Group Cipher */
    t_u16 group_cipher;
    /** Protocol stored during uap wext configuratoin */
    t_u16 uap_protocol;
    /** Key Mgmt whether PSK or 1x */
    t_u16 uap_key_mgmt;
    /** Beacon IE length from hostapd */
    t_u16 bcn_ie_len;
    /** Beacon IE buffer from hostapd */
    t_u8 bcn_ie_buf[MAX_IE_SIZE];

#ifdef PROC_DEBUG
    /** MLAN debug info */
    struct debug_data_priv items_priv;
#endif
    BOOLEAN is_h2d_active;
    BOOLEAN is_d2h_active;
};

struct _firmware
{
    t_void *data;
    t_u32 size;
};

/** Handle data structure for MOAL */
struct _moal_handle
{
        /** MLAN adapter structure */
    t_void *pmlan_adapter;
        /** Private pointer */
    moal_private *priv[MLAN_MAX_BSS_NUM];
        /** Priv number */
    t_u8 priv_num;
        /** Bss attr */
    moal_drv_mode drv_mode;
        /** set mac address flag */
    t_u8 set_mac_addr;
        /** MAC address */
    t_u8 mac_addr[ETH_ALEN];
#ifdef CONFIG_PROC_FS
        /** Proc top level directory entry */
    struct proc_dir_entry *proc_mwlan;
#endif
        /** Firmware download skip flag */
    t_u8 skip_fw_dnld;
        /** Firmware */
    firmware *firmware;
        /** Firmware request start time */
    struct timeval req_fw_time;
        /** Init config file */
    firmware *user_data;
        /** STATUS variables */
    MOAL_HARDWARE_STATUS hardware_status;
        /** POWER MANAGEMENT AND PnP SUPPORT */
    BOOLEAN surprise_removed;
        /** Firmware release number */
    t_u32 fw_release_number;
        /** Init wait queue token */
    t_u16 init_wait_q_woken;
        /** Init wait queue */
    wait_queue_head_t init_wait_q __ATTRIB_ALIGN__;
        /** Device suspend flag */
    BOOLEAN is_suspended;
        /** Suspend wait queue token */
    t_u16 suspend_wait_q_woken;
        /** Suspend wait queue */
    wait_queue_head_t suspend_wait_q __ATTRIB_ALIGN__;
        /** Host Sleep activated flag */
    t_u8 hs_activated;
        /** Host Sleep activated event wait queue token */
    t_u16 hs_activate_wait_q_woken;
        /** Host Sleep activated event wait queue */
    wait_queue_head_t hs_activate_wait_q __ATTRIB_ALIGN__;
        /** Card pointer */
    t_void *card;
        /** Rx pending in MLAN */
    atomic_t rx_pending;
        /** Tx packet pending count in mlan */
    atomic_t tx_pending;
        /** IOCTL pending count in mlan */
    atomic_t ioctl_pending;
        /** Malloc count */
    t_u32 malloc_count;
        /** lock count */
    t_u32 lock_count;
        /** mlan buffer alloc count */
    t_u32 mbufalloc_count;
        /** hs skip count */
    t_u32 hs_skip_count;
        /** hs force count */
    t_u32 hs_force_count;
        /** suspend_fail flag */
    BOOLEAN suspend_fail;
#ifdef REASSOCIATION
        /** Re-association thread */
    wlan_thread reassoc_thread;
        /** Re-association timer set flag */
    BOOLEAN is_reassoc_timer_set;
        /** Re-association timer */
    WLAN_DRV_TIMER reassoc_timer;
        /**  */
    SEMAPHORE reassoc_sem;
        /** Flag of re-association on/off */
    BOOLEAN reassoc_on;
#endif                          /* REASSOCIATION */
        /** Netlink kernel socket */
    struct sock *nl_sk;
        /** Netlink kernel socket number */
    t_u32 netlink_num;
    /** w_stats wait queue token */
    t_u16 meas_wait_q_woken;
    /** w_stats wait queue */
    wait_queue_head_t meas_wait_q __ATTRIB_ALIGN__;
    /** Measurement start jiffes */
    t_u32 meas_start_jiffies;
    /** CAC checking period flag */
    BOOLEAN cac_period;
    /** BSS START command delay executing flag */
    BOOLEAN delay_bss_start;
    /** SSID,BSSID parameter of delay executing */
    mlan_ssid_bssid delay_ssid_bssid;
    /** handle index - for multiple card supports */
    t_u8 handle_idx;
#ifdef STA_SUPPORT
        /** Scan pending on blocked flag */
    t_u8 scan_pending_on_block;
        /** Async scan semaphore */
    SEMAPHORE async_sem;

#endif
        /** main state */
    t_u8 main_state;
};

/** Max number of char in custom event - use multiple of them if needed */
#define IW_CUSTOM_MAX		256     /* In bytes */

/** Debug Macro definition*/
#if 0 // For Debug
#include "logger.h"
#define	PRINTM(level,msg...) DPRINTF((DBG_OUTPUT|DBG_ERROR), (msg))
#else
#define	PRINTM(level,msg...) do {} while (0)
#endif

/** Wait until a condition becomes true */
#define MASSERT(cond)                   \
do {                                    \
    if (!(cond)) {                      \
        DPRINTF((DBG_OUTPUT|DBG_ERROR), ("MASSERT: %s: %i\n", __FUNCTION__, __LINE__)); \
        ASSERT(0); \
    }                                   \
} while(0)

/** Log entry point for debugging */
#define	ENTER()			PRINTM(MENTRY, "Enter: %s\n", __FUNCTION__)
/** Log exit point for debugging */
#define	LEAVE()			PRINTM(MENTRY, "Leave: %s\n", __FUNCTION__)

#ifdef DEBUG_LEVEL1
#define DBG_DUMP_BUF_LEN 	64
#define MAX_DUMP_PER_LINE	16

static inline void
hexdump(char *prompt, t_u8 * buf, int len)
{
    int i;
    char dbgdumpbuf[DBG_DUMP_BUF_LEN];
    char *ptr = dbgdumpbuf;

    printk(KERN_DEBUG "%s:\n", prompt);
    for (i = 1; i <= len; i++) {
        ptr += snprintf(ptr, 4, "%02x ", *buf);
        buf++;
        if (i % MAX_DUMP_PER_LINE == 0) {
            *ptr = 0;
            printk(KERN_DEBUG "%s\n", dbgdumpbuf);
            ptr = dbgdumpbuf;
        }
    }
    if (len % MAX_DUMP_PER_LINE) {
        *ptr = 0;
        printk(KERN_DEBUG "%s\n", dbgdumpbuf);
    }
}

#define DBG_HEXDUMP_MERROR(x,y,z)    do {if (drvdbg & MERROR) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MCMD_D(x,y,z)    do {if (drvdbg & MCMD_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MDAT_D(x,y,z)    do {if (drvdbg & MDAT_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MIF_D(x,y,z)     do {if (drvdbg & MIF_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MEVT_D(x,y,z)    do {if (drvdbg & MEVT_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MFW_D(x,y,z)     do {if (drvdbg & MFW_D) hexdump(x,y,z);} while(0)
#define	DBG_HEXDUMP(level,x,y,z)    DBG_HEXDUMP_##level(x,y,z)

#else
/** Do nothing since debugging is not turned on */
#define DBG_HEXDUMP(level,x,y,z)    do {} while (0)
#endif

#ifdef DEBUG_LEVEL2
#define HEXDUMP(x,y,z)              do {if (drvdbg & MINFO) hexdump(x,y,z);} while(0)
#else
/** Do nothing since debugging is not turned on */
#define HEXDUMP(x,y,z)              do {} while (0)
#endif

#ifdef BIG_ENDIAN_SUPPORT
/** Convert from 16 bit little endian format to CPU format */
#define woal_le16_to_cpu(x) le16_to_cpu(x)
/** Convert from 32 bit little endian format to CPU format */
#define woal_le32_to_cpu(x) le32_to_cpu(x)
/** Convert from 64 bit little endian format to CPU format */
#define woal_le64_to_cpu(x) le64_to_cpu(x)
/** Convert to 16 bit little endian format from CPU format */
#define woal_cpu_to_le16(x) cpu_to_le16(x)
/** Convert to 32 bit little endian format from CPU format */
#define woal_cpu_to_le32(x) cpu_to_le32(x)
/** Convert to 64 bit little endian format from CPU format */
#define woal_cpu_to_le64(x) cpu_to_le64(x)
#else
/** Do nothing */
#define woal_le16_to_cpu(x) x
/** Do nothing */
#define woal_le32_to_cpu(x) x
/** Do nothing */
#define woal_le64_to_cpu(x) x
/** Do nothing */
#define woal_cpu_to_le16(x) x
/** Do nothing */
#define woal_cpu_to_le32(x) x
/** Do nothing */
#define woal_cpu_to_le64(x) x
#endif

/** 
 *  @brief This function returns first available priv
 *  based on the BSS role
 *  
 *  @param handle    A pointer to moal_handle
 *  @param bss_role  BSS role or MLAN_BSS_ROLE_ANY
 *
 *  @return          Pointer to moal_private
 */
static inline moal_private *
woal_get_priv(moal_handle * handle, mlan_bss_role bss_role)
{
    int i;

    for (i = 0; i < MIN(handle->priv_num, MLAN_MAX_BSS_NUM); i++) {
        if (handle->priv[i]) {
            if (bss_role == MLAN_BSS_ROLE_ANY ||
                GET_BSS_ROLE(handle->priv[i]) == bss_role)
                return (handle->priv[i]);
        }
    }
    return NULL;
}

/** Max line length allowed in init config file */
#define MAX_LINE_LEN        256
/** Max MAC address string length allowed */
#define MAX_MAC_ADDR_LEN    18
/** Max register type/offset/value etc. parameter length allowed */
#define MAX_PARAM_LEN       12

/** HostCmd_CMD_CFG_DATA for CAL data */
#define HostCmd_CMD_CFG_DATA 0x008f
/** HostCmd action set */
#define HostCmd_ACT_GEN_SET 0x0001
/** HostCmd CAL data header length */
#define CFG_DATA_HEADER_LEN	6

#if 0
typedef struct _HostCmd_DS_GEN
{
    t_u16 command;
    t_u16 size;
    t_u16 seq_num;
    t_u16 result;
} HostCmd_DS_GEN;
#endif

#if 0
typedef struct _HostCmd_DS_802_11_CFG_DATA
{
    /** Action */
    t_u16 action;
    /** Type */
    t_u16 type;
    /** Data length */
    t_u16 data_len;
    /** Data */
    t_u8 data[1];
} __ATTRIB_PACK__ HostCmd_DS_802_11_CFG_DATA;
#endif

/** moal_802_11_rates  */
typedef struct _moal_802_11_rates
{
        /** Num of rates */
    t_u8 num_of_rates;
        /** Rates */
    t_u8 rates[MLAN_SUPPORTED_RATES];
} moal_802_11_rates;

/** USB8782 card type */
#define CARD_TYPE_USB8782   0x01
/** USB8786 card type */
#define CARD_TYPE_USB8786   0x02

/**  Convert ASCII string to hex value */
int woal_ascii2hex(t_u8 * d, char *s, t_u32 dlen);
/**  Convert mac address from string to t_u8 buffer */
void woal_mac2u8(t_u8 * mac_addr, char *buf);
/**  Extract token from string */
char *woal_strsep(char **s, char delim, char esc);
/** Return int value of a given ASCII string */
mlan_status woal_atoi(int *data, char *a);
/** Return hex value of a given ASCII string */
int woal_atox(char *a);
/** Allocate buffer */
pmlan_buffer woal_alloc_mlan_buffer(moal_handle * handle, int size);
/** Allocate IOCTL request buffer */
pmlan_ioctl_req woal_alloc_mlan_ioctl_req(int size);
/** Free buffer */
void woal_free_mlan_buffer(moal_handle * handle, pmlan_buffer pmbuf);
/** Free handle */
void woal_free_moal_handle(moal_handle * handle);
/** Get private structure of a BSS by index */
moal_private *woal_bss_index_to_priv(moal_handle * handle, t_u8 bss_index);
/* Functions in interface module */
/** Add card */
moal_handle *woal_add_card(void *card);
/** Remove card */
mlan_status woal_remove_card(void *card);
/** broadcast event */
mlan_status woal_broadcast_event(moal_private * priv, t_u8 * payload,
                                 t_u32 len);
/** switch driver mode */
mlan_status woal_switch_drv_mode(moal_handle * handle, t_u32 mode);

/** Get version */
void woal_get_version(moal_handle * handle, char *version, int maxlen);
/** Get Driver Version */
int woal_get_driver_version(moal_private * priv, struct ifreq *req);
/** Get extended driver version */
int woal_get_driver_verext(moal_private * priv, struct ifreq *ireq);
/** Request open */
mlan_status woal_request_open(moal_private * priv);
/** Request close */
mlan_status woal_request_close(moal_private * priv);
/** Request MAC address setting */
mlan_status woal_request_set_mac_address(moal_private * priv);
/** Request multicast list setting */
void woal_request_set_multicast_list(moal_private * priv,
                                     struct net_device *dev);
/** Request IOCTL action */
mlan_status woal_request_ioctl(moal_private * priv, mlan_ioctl_req * req,
                               t_u8 wait_option);
mlan_status woal_request_soft_reset(moal_handle * handle);
#ifdef PROC_DEBUG
/** Get debug information */
mlan_status woal_get_debug_info(moal_private * priv, t_u8 wait_option,
                                mlan_debug_info * debug_info);
/** Set debug information */
mlan_status woal_set_debug_info(moal_private * priv, t_u8 wait_option,
                                mlan_debug_info * debug_info);
#endif
/** Disconnect */
mlan_status woal_disconnect(moal_private * priv, t_u8 wait_option, t_u8 * mac);
/** associate */
mlan_status woal_bss_start(moal_private * priv, t_u8 wait_option,
                           mlan_ssid_bssid * ssid_bssid);
/** Request firmware information */
mlan_status woal_request_get_fw_info(moal_private * priv, t_u8 wait_option,
                                     mlan_fw_info * fw_info);
/** Set/get Host Sleep parameters */
mlan_status woal_set_get_hs_params(moal_private * priv, t_u16 action,
                                   t_u8 wait_option, mlan_ds_hs_cfg * hscfg);
/** Cancel Host Sleep configuration */
mlan_status woal_cancel_hs(moal_private * priv, t_u8 wait_option);
/** Enable Host Sleep configuration */
int woal_enable_hs(moal_private * priv);
/** hs active timeout 2 second */
#define HS_ACTIVE_TIMEOUT  (2 * HZ)

/** Get BSS information */
mlan_status woal_get_bss_info(moal_private * priv, t_u8 wait_option,
                              mlan_bss_info * bss_info);
void woal_process_ioctl_resp(moal_private * priv, mlan_ioctl_req * req);
#ifdef STA_SUPPORT
void woal_send_iwevcustom_event(moal_private * priv, t_s8 * str);
void woal_send_disconnect_to_system(moal_private * priv);
void woal_send_mic_error_event(moal_private * priv, t_u32 event);
void woal_ioctl_get_bss_resp(moal_private * priv, mlan_ds_bss * bss);
void woal_ioctl_get_info_resp(moal_private * priv, mlan_ds_get_info * info);
/** Get mode */
t_u32 woal_get_mode(moal_private * priv, t_u8 wait_option);
/** Get signal information */
mlan_status woal_get_signal_info(moal_private * priv, t_u8 wait_option,
                                 mlan_ds_get_signal * signal);
/** Get statistics information */
mlan_status woal_get_stats_info(moal_private * priv, t_u8 wait_option,
                                mlan_ds_get_stats * stats);
/** Get data rates */
mlan_status woal_get_data_rates(moal_private * priv, t_u8 wait_option,
                                moal_802_11_rates * m_rates);
/** Get channel list */
mlan_status woal_get_channel_list(moal_private * priv, t_u8 wait_option,
                                  mlan_chan_list * chanlist);
/** Set/Get retry count */
mlan_status woal_set_get_retry(moal_private * priv, t_u32 action,
                               t_u8 wait_option, int *value);
/** Set/Get RTS threshold */
mlan_status woal_set_get_rts(moal_private * priv, t_u32 action,
                             t_u8 wait_option, int *value);
/** Set/Get fragment threshold */
mlan_status woal_set_get_frag(moal_private * priv, t_u32 action,
                              t_u8 wait_option, int *value);
/** Set/Get generic element */
mlan_status woal_set_get_gen_ie(moal_private * priv, t_u32 action, t_u8 * ie,
                                int *ie_len);
/** Set/Get TX power */
mlan_status woal_set_get_tx_power(moal_private * priv, t_u32 action,
                                  mlan_power_cfg_t * pwr);
/** Set/Get power IEEE management */
mlan_status woal_set_get_power_mgmt(moal_private * priv, t_u32 action,
                                    int *disabled, int type);
/** Get data rate */
mlan_status woal_set_get_data_rate(moal_private * priv, t_u8 action,
                                   mlan_rate_cfg_t * datarate);

/** Request a network scan */
mlan_status woal_request_scan(moal_private * priv, t_u8 wait_option,
                              mlan_802_11_ssid * req_ssid);
/** Set radio on/off */
int woal_set_radio(moal_private * priv, t_u8 option);
/** Set region code */
mlan_status woal_set_region_code(moal_private * priv, char *region);
/** Set authentication mode */
mlan_status woal_set_auth_mode(moal_private * priv, t_u8 wait_option,
                               t_u32 auth_mode);
/** Get authentication mode */
mlan_status woal_get_auth_mode(moal_private * priv, t_u8 wait_option,
                               t_u32 * auth_mode);
/** Set encryption mode */
mlan_status woal_set_encrypt_mode(moal_private * priv, t_u8 wait_option,
                                  t_u32 encrypt_mode);
/** Get encryption mode */
mlan_status woal_get_encrypt_mode(moal_private * priv, t_u8 wait_option,
                                  t_u32 * encrypt_mode);
/** Enable wep key */
mlan_status woal_enable_wep_key(moal_private * priv, t_u8 wait_option);
/** Set WPA enable */
mlan_status woal_set_wpa_enable(moal_private * priv, t_u8 wait_option,
                                t_u32 enable);
/** Get WPA state */
mlan_status woal_get_wpa_enable(moal_private * priv, t_u8 wait_option,
                                t_u32 * enable);
/** Find best network to connect */
mlan_status woal_find_best_network(moal_private * priv, t_u8 wait_option,
                                   mlan_ssid_bssid * ssid_bssid);
/** Set E-Supplicant mode */
mlan_status woal_set_ewpa_mode(moal_private * priv, t_u8 wait_option,
                               mlan_ssid_bssid * ssid_bssid);
/** Set Ad-Hoc channel */
mlan_status woal_change_adhoc_chan(moal_private * priv, int channel);

/** Get scan table */
mlan_status woal_get_scan_table(moal_private * priv, t_u8 wait_option,
                                mlan_scan_resp * scanresp);

#endif  /**STA_SUPPORT */

mlan_status woal_set_wapi_enable(moal_private * priv, t_u8 wait_option,
                                 t_u32 enable);

/** Initialize priv */
void woal_init_priv(moal_private * priv, t_u8 wait_option);
/** Reset interface(s) */
int woal_reset_intf(moal_private * priv, t_u8 wait_option, int all_intf);
/** common ioctl for uap, station */
int woal_custom_ie_ioctl(struct net_device *dev, struct ifreq *req);

int woal_get_bss_type(struct net_device *dev, struct ifreq *req);
int woal_host_command(moal_private * priv, struct iwreq *wrq);
#if defined(WIFI_DIRECT_SUPPORT)
#if defined(STA_SUPPORT) && defined(UAP_SUPPORT)
int woal_set_get_bss_role(moal_private * priv, struct iwreq *wrq);
#endif
#endif
#if defined(WIFI_DIRECT_SUPPORT) || defined(UAP_SUPPORT)
/** hostcmd ioctl for uap, wifidirect */
int woal_hostcmd_ioctl(struct net_device *dev, struct ifreq *req);
#endif

#ifdef CONFIG_PROC_FS
/** Initialize proc fs */
void woal_proc_init(moal_handle * handle);
/** Clean up proc fs */
void woal_proc_exit(moal_handle * handle);
/** Create proc entry */
void woal_create_proc_entry(moal_private * priv);
/** Remove proc entry */
void woal_proc_remove(moal_private * priv);
/** string to number */
int woal_string_to_number(char *s);
#endif

#ifdef PROC_DEBUG
/** Create debug proc fs */
void woal_debug_entry(moal_private * priv);
/** Remove debug proc fs */
void woal_debug_remove(moal_private * priv);
#endif /* PROC_DEBUG */

/** check pm info */
mlan_status woal_get_pm_info(moal_private * priv, mlan_ds_ps_info * pm_info);

/** Enter USB Suspend */
int woal_enter_usb_suspend(moal_handle * handle);
/** Exit from USB Suspend */
int woal_exit_usb_suspend(moal_handle * handle);

#ifdef REASSOCIATION
int woal_reassociation_thread(void *data);
void woal_reassoc_timer_func(void *context);
#endif /* REASSOCIATION */

//t_void woal_main_work_queue(struct work_struct *work);

int woal_hard_start_xmit(struct sk_buff *skb, struct net_device *dev);
moal_private *woal_add_interface(moal_handle * handle, t_u8 bss_num,
                                 t_u8 bss_type);
void woal_remove_interface(moal_handle * handle, t_u8 bss_index);
void woal_set_multicast_list(struct net_device *dev);
mlan_status woal_request_fw(moal_handle * handle);

int woal_11h_channel_check_ioctl(moal_private * priv);
void woal_cancel_cac_block(moal_private * priv);
//void woal_moal_debug_info(moal_handle * phandle, u8 flag);

#endif /* _MOAL_MAIN_H */
