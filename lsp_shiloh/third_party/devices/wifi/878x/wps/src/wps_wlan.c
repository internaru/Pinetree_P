/** @file wps_wlan.c
 *  @brief This file contains functions for WLAN driver control/command.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include    <unistd.h>
#include    <time.h>
#include    <ctype.h>
#include    <string.h>
#include    <stdlib.h>
#include    "common.h"
#include    "raw_socket_api.h"
#include    "lassert.h"
#include    "wps_def.h"
#include    "wlan_hostcmd.h"
#include    "wps_util.h"
#include    "wps_msg.h"
#include    "wps_wlan.h"
#include    "wireless_copy.h"
#include    "wlanInterface.h"
#include    "logger.h"
#include    "wlan_wfd.h"
#include 	"tx_api.h"

// ToDo - this is defined in os_wireless.h, but including that file causes
// compile errors.
#define SIOCDEVPRIVATE  0x89F0
#define SIOCIWFIRSTPRIV	0x8BE0
/** PRIVATE CMD ID */
#define WOAL_IOCTL                  (SIOCIWFIRSTPRIV)
/** Private command ID to set/get 2K bytes */
#define WOAL_SET_GET_2K_BYTES       (WOAL_IOCTL + 23)
/** Private command ID for setuserscan */
#define WOAL_SET_USER_SCAN          3

int wlan_get_scan_results(SCAN_RESULTS *results);
int WlanShimCGetSsid(char *ssid);
int WlanShimCDeauth(void);
int wps_wlan_set_scan(char *, int);

extern int WlanIoctl(int command, void * buf);

extern PWPS_INFO gpwps_info;

/** Chan-Freq mapping table */
typedef struct _CHANNEL_FREQ_ENTRY
{
    /** Channel Number */
    int Channel;
    /** Frequency of this Channel */
    int Freq;
} CHANNEL_FREQ_ENTRY;

/** Initial number of total private ioctl calls */
#define IW_INIT_PRIV_NUM    128
/** Maximum number of total private ioctl calls supported */
#define IW_MAX_PRIV_NUM     1024
/** MRV Event point off*/
#define MRV_EV_POINT_OFF (((char *) &(((struct iw_point *) NULL)->length)) - \
              (char *) NULL)

/** Convert character to integer */
#define CHAR2INT(x) (((x) >= 'A') ? ((x) - 'A' + 10) : ((x) - '0'))

#define MAX_WEP_KEY_LEN 27
#define FLUSH_PREV_RESULT 0
#define KEEP_PREV_RESULT 1

/********************************************************
        Local Variables
********************************************************/
/** Channels for 802.11b/g */
static CHANNEL_FREQ_ENTRY channel_freq_UN_BG[] = {
    {1, 2412},
    {2, 2417},
    {3, 2422},
    {4, 2427},
    {5, 2432},
    {6, 2437},
    {7, 2442},
    {8, 2447},
    {9, 2452},
    {10, 2457},
    {11, 2462},
    {12, 2467},
    {13, 2472},
    {14, 2484}
};

/** Channels for 802.11a/j */
static CHANNEL_FREQ_ENTRY channel_freq_UN_AJ[] = {
    {8, 5040},
    {12, 5060},
    {16, 5080},
    {34, 5170},
    {38, 5190},
    {42, 5210},
    {46, 5230},
    {36, 5180},
    {40, 5200},
    {44, 5220},
    {48, 5240},
    {52, 5260},
    {56, 5280},
    {60, 5300},
    {64, 5320},
    {100, 5500},
    {104, 5520},
    {108, 5540},
    {112, 5560},
    {116, 5580},
    {120, 5600},
    {124, 5620},
    {128, 5640},
    {132, 5660},
    {136, 5680},
    {140, 5700},
    {149, 5745},
    {153, 5765},
    {157, 5785},
    {161, 5805},
    {165, 5825},
/*  {240, 4920}, 
    {244, 4940}, 
    {248, 4960}, 
    {252, 4980}, 
channels for 11J JP 10M channel gap */
};

//static s32 ioctl_sock;
static s8 dev_name[IFNAMSIZ + 1];
//static struct iw_priv_args *Priv_args = NULL;
static int we_version_compiled = 0;

/********************************************************
        Global Variables
********************************************************/
/** Global pwps information */
extern PWPS_INFO gpwps_info;
/** wps global */
extern WPS_DATA wps_global;
/** IE index*/
extern short ie_index;
/** Probe Request IE index*/
extern short probe_ie_index;
/** WFD switch interface flag */
extern int switch_intf;

/** IE buffer index */
extern short ap_assocresp_ie_index;

/********************************************************
        Local Functions
********************************************************/
/** 
 *  @brief convert char to hex integer
 * 
 *  @param chr      char to convert
 *  @return         hex integer or 0
 */
int
hexval(s32 chr)
{
    if (chr >= '0' && chr <= '9')
        return chr - '0';
    if (chr >= 'A' && chr <= 'F')
        return chr - 'A' + 10;
    if (chr >= 'a' && chr <= 'f')
        return chr - 'a' + 10;

    return 0;
}

/** 
 *  @brief convert char to hex integer
 *
 *  @param chr      char
 *  @return         hex integer
 */
#if 0
static char
hexc2bin(char chr)
{
    if (chr >= '0' && chr <= '9')
        chr -= '0';
    else if (chr >= 'A' && chr <= 'F')
        chr -= ('A' - 10);
    else if (chr >= 'a' && chr <= 'f')
        chr -= ('a' - 10);

    return chr;
}
#endif

/** 
 *  @brief convert string to hex integer
 *
 *  @param s        A pointer string buffer
 *  @return         hex integer
 */
#if 0
u32
a2hex(char *s)
{
    u32 val = 0;
    if (!strncasecmp("0x", s, 2)) {
        s += 2;
    }

    while (*s && isxdigit(*s)) {
        val = (val << 4) + hexc2bin(*s++);
    }

    return val;
}
#endif

/* 
 *  @brief convert String to integer
 *  
 *  @param value    A pointer to string
 *  @return         integer
 */
#if 0
static u32
a2hex_or_atoi(s8 * value)
{
    if (value[0] == '0' && (value[1] == 'X' || value[1] == 'x'))
        return a2hex((char *) (value + 2));
    else
        return atoi((char *) value);
}
#endif

/** 
 *  @brief convert string to mac address
 *
 *  @param s                A pointer string buffer
 *  @param mac_addr         pointer to mac address array
 *  @return					none
 */
#if 0
void
a2_mac_addr(char *s, u8 * mac_addr)
{
    u32 val = 0, count = 0;
    if (!strncasecmp("0x", s, 2)) {
        s += 2;
    }

    while (*s && isxdigit(*s)) {
        val = (val << 4) + hexc2bin(*s++);
        val = (val << 4) + hexc2bin(*s++);
        mac_addr[count] = val;
        count++;
        val = 0;
    }

    return;
}
#endif

/** 
 *  @brief convert string to integer
 * 
 *  @param ptr      A pointer to data buffer
 *  @param chr      A pointer to return integer
 *  @return         A pointer to next data field
 */
s8 *
convert2hex(s8 * ptr, u8 * chr)
{
    u8 val;

    for (val = 0; *ptr && isxdigit(*ptr); ptr++) {
        val = (val * 16) + hexval(*ptr);
    }

    *chr = val;

    return ptr;
}

#ifdef DEBUG
/** 
 *  @brief convert frequency to channel
 *
 *  @param freq     frequency value
 *  @return         channel number
 */
static int
mapping_freq_to_chan(int freq)
{
    int i, table;
    int chan = 0;

    table = sizeof(channel_freq_UN_BG) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (freq == channel_freq_UN_BG[i].Freq) {
            chan = channel_freq_UN_BG[i].Channel;
            break;
        }
    }

    table = sizeof(channel_freq_UN_AJ) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (freq == channel_freq_UN_AJ[i].Freq) {
            chan = channel_freq_UN_AJ[i].Channel;
            break;
        }
    }

    return chan;
}
#endif

/** 
 *  @brief Get private info.
 *   
 *  @param ifname   A pointer to net name
 *  @return         The number of private ioctls if success, WPS_STATUS_FAIL if fail
 */
#if 0
static int
get_private_info(const s8 * ifname)
{
    /* This function sends the SIOCGIWPRIV command which is handled by the
       kernel. and gets the total number of private ioctl's available in the
       host driver. */
    struct iwreq iwr;
    int s, ret = WPS_STATUS_SUCCESS;
    struct iw_priv_args *pPriv = NULL;
    struct iw_priv_args *newPriv;
    int result = 0;
    size_t size = IW_INIT_PRIV_NUM;

    ENTER();

    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket[PF_INET,SOCK_DGRAM]");
        return WPS_STATUS_FAIL;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) ifname, IFNAMSIZ);

    do {
        /* (Re)allocate the buffer */
        newPriv = realloc(pPriv, size * sizeof(pPriv[0]));
        if (newPriv == NULL) {
            wps_printf(MSG_WARNING, "Error: Buffer allocation failed\n");
            ret = WPS_STATUS_FAIL;
            break;
        }
        pPriv = newPriv;
        iwr.u.data.pointer = (caddr_t) pPriv;
        iwr.u.data.length = size;
        iwr.u.data.flags = 0;

        if (ioctl(s, SIOCGIWPRIV, &iwr) < 0) {
            result = errno;
            ret = WPS_STATUS_FAIL;
            if (result == E2BIG) {
                /* We need a bigger buffer. Check if kernel gave us any hints. */
                if (iwr.u.data.length > size) {
                    /* Kernel provided required size */
                    size = iwr.u.data.length;
                } else {
                    /* No hint from kernel, double the buffer size */
                    size *= 2;
                }
            } else {
                /* ioctl error */
                perror("ioctl[SIOCGIWPRIV]");
                break;
            }
        } else {
            /* Success. Return the number of private ioctls */
            Priv_args = pPriv;
            ret = iwr.u.data.length;
            break;
        }
    } while (size <= IW_MAX_PRIV_NUM);

    if ((ret == WPS_STATUS_FAIL) && (pPriv))
        free(pPriv);

    close(s);

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief Get Sub command ioctl number
 *   
 *  @param i            command index
 *  @param priv_cnt     Total number of private ioctls available in driver
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return             WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
#if 0
static int
marvell_get_subioctl_no(s32 i, s32 priv_cnt, int *ioctl_val, int *subioctl_val)
{
    s32 j;

    ENTER();

    if (Priv_args[i].cmd >= SIOCDEVPRIVATE) {
        *ioctl_val = Priv_args[i].cmd;
        *subioctl_val = 0;
        LEAVE();
        return WPS_STATUS_SUCCESS;
    }

    j = -1;

    /* Find the matching *real* ioctl */

    while ((++j < priv_cnt)
           && ((Priv_args[j].name[0] != '\0') ||
               (Priv_args[j].set_args != Priv_args[i].set_args) ||
               (Priv_args[j].get_args != Priv_args[i].get_args))) {
    }

    /* If not found... */
    if (j == priv_cnt) {
        wps_printf(MSG_WARNING,
                   "%s: Invalid private ioctl definition for: 0x%x\n", dev_name,
                   Priv_args[i].cmd);
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    /* Save ioctl numbers */
    *ioctl_val = Priv_args[j].cmd;
    *subioctl_val = Priv_args[i].cmd;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}
#endif

/** 
 *  @brief Get ioctl number
 *   
 *  @param ifname       A pointer to net name
 *  @param priv_cmd     A pointer to priv command buffer
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
#if 0
static int
marvell_get_ioctl_no(const s8 * ifname,
                     const s8 * priv_cmd, int *ioctl_val, int *subioctl_val)
{
    s32 i;
    s32 priv_cnt;
    int ret = WPS_STATUS_FAIL;

    ENTER();

    priv_cnt = get_private_info(ifname);

    /* Are there any private ioctls? */
    if (priv_cnt <= 0 || priv_cnt > IW_MAX_PRIV_NUM) {
        /* Could skip this message ? */
        wps_printf(MSG_WARNING, "%-8.8s  no private ioctls.\n", ifname);
    } else {
        for (i = 0; i < priv_cnt; i++) {
            if (Priv_args[i].name[0] &&
                !strcmp((char *) Priv_args[i].name, (char *) priv_cmd)) {
                ret =
                    marvell_get_subioctl_no(i, priv_cnt, ioctl_val,
                                            subioctl_val);
                break;
            }
        }
    }

    if (Priv_args) {
        free(Priv_args);
        Priv_args = NULL;
    }

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief Retrieve the ioctl and sub-ioctl numbers for the given ioctl string
 *   
 *  @param ifname       Private IOCTL string name
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
#if 0
static int
get_priv_ioctl(char *ioctl_name, int *ioctl_val, int *subioctl_val)
{
    int retval;

    ENTER();

    retval = marvell_get_ioctl_no((s8 *) dev_name,
                                  (s8 *) ioctl_name, ioctl_val, subioctl_val);

    /* Debug print discovered IOCTL values */
    wps_printf(DEBUG_WLAN, "ioctl %s: %x, %x\n", ioctl_name, *ioctl_val,
               *subioctl_val);

    LEAVE();
    return retval;
}
#endif

#ifdef WIFI_DIRECT_SUPPORT
/** 
 *  @brief  get range 
 *   
 *  @return         WPS_STATUS_SUCCESS--success, otherwise --fail
 */
static int
get_range(void)
{
#if 0
    struct iw_range *range;
    struct iwreq iwr;
    size_t buflen;

    ENTER();

    buflen = sizeof(struct iw_range) + 500;
    range = malloc(buflen);
    if (range == NULL)
        return WPS_STATUS_FAIL;
    memset(range, 0, buflen);

    memset(&iwr, 0, sizeof(struct iwreq));
    iwr.u.data.pointer = (caddr_t) range;
    iwr.u.data.length = buflen;
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    if ((ioctl(ioctl_sock, SIOCGIWRANGE, &iwr)) < 0) {
        wps_printf(MSG_ERROR, "Get Range Results Failed\n");
        free(range);
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    we_version_compiled = range->we_version_compiled;
    wps_printf(DEBUG_INIT, "Driver build with Wireless Extension %d\n",
               range->we_version_compiled);
    wps_printf(DEBUG_INIT, "WPS Application build with Wireless Extension %d\n",
               WIRELESS_EXT);

    free(range);
#endif
    LEAVE();
    return WPS_STATUS_SUCCESS;
}
#endif

/* 
 *  @brief convert hex char to integer
 *  
 *  @param c        Hex char
 *  @return         Integer value or WPS_STATUS_FAIL
 */
#if 0
static int
hex2num(s8 c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return WPS_STATUS_FAIL;
}
#endif

/* 
 *  @brief convert integer to hex char
 *  
 *  @param c        Integer value or WPS_STATUS_FAIL
 *  @return         Hex char
 */
#if 0
int
num2hex(s8 c)
{
    if ((c + '0') >= '0' && (c + '0') <= '9')
        return c + '0';
    if ((c - 10 + 'a') >= 'a' && (c - 10 + 'a') <= 'f')
        return c - 10 + 'a';
    if ((c - 10 + 'A') >= 'A' && (c - 10 + 'A') <= 'F')
        return c - 10 + 'A';
    return WPS_STATUS_FAIL;
}
#endif

/* 
 *  @brief convert hex char to integer
 *  
 *  @param c        char
 *  @return         Integer value or WPS_STATUS_FAIL
 */
#if 0
static int
hex2byte(const s8 * hex)
{
    s32 a, b;
    a = hex2num(*hex++);
    if (a < 0)
        return WPS_STATUS_FAIL;
    b = hex2num(*hex++);
    if (b < 0)
        return WPS_STATUS_FAIL;
    return (a << 4) | b;
}
#endif

/* 
 *  @brief convert hex char to integer
 *  
 *  @param hex      A pointer to hex string
 *  @param buf      buffer to storage the data
 *  @param len      
 *  @return         WPS_STATUS_SUCCESS--success, otherwise --fail
 */
#if 0
int
hexstr2bin(const s8 * hex, u8 * buf, size_t len)
{
    s32 i, a;
    const s8 *ipos = hex;
    u8 *opos = buf;

    for (i = 0; i < len; i++) {
        a = hex2byte(ipos);
        if (a < 0)
            return WPS_STATUS_FAIL;
        *opos++ = a;
        ipos += 2;
    }
    return WPS_STATUS_SUCCESS;
}
#endif

/* 
 *  @brief convert binary data to Hex string
 *  
 *  @param bin      A pointer to binary data
 *  @param hex      A pointer to hex string
 *  @param len      
 *  @return         WPS_STATUS_SUCCESS--success, otherwise --fail
 */
#if 0
int
bin2hexstr(const u8 * bin, s8 * hex, size_t len)
{
    s32 i;

    for (i = 0; i < len; i++) {
        hex[2 * i] = num2hex(bin[i] >> 4);
        hex[(2 * i) + 1] = num2hex(bin[i] & 0x0f);
    }

    return WPS_STATUS_SUCCESS;
}
#endif

/** 
 *  @brief convert string to hex array
 *
 *  @param s                A pointer string buffer
 *  @param mac_addr         pointer to mac address array
 *  @return					none
 */
#if 0
int
char_str_to_hex_array(char *s, u8 * hex_array)
{
    u32 val = 0, count = 0;

    while (*s && isxdigit(*s)) {
        val = (val << 4) + hexc2bin(*s++);
        val = (val << 4) + hexc2bin(*s++);
        hex_array[count] = val;
        count++;
        val = 0;
    }

    return count;
}
#endif

#if WIRELESS_EXT > 14
/** 
 *  @brief parse custom info
 *
 *  @param results      A pointer to SCAN_RESULTS buffer
 *  @param data         A pointer to iw_point structure
 *  @param idx          AP index
 *  @return             NA
 */
#if 0
static void
parse_custom_info(SCAN_RESULTS * results, struct iw_point *data, s32 idx)
{
    ENTER();

    if (!data->pointer || !data->length) {
        wps_printf(MSG_ERROR, "iw_point: Invalid Pointer/Length\n");
        LEAVE();
        return;
    }

    if (!strncmp(data->pointer, "wpa_ie=", 7)) {
        char *spos;
        int bytes;
        spos = data->pointer + 7;
        bytes = data->length - 7;
        if (bytes & 1) {
            LEAVE();
            return;
        }
        bytes /= 2;
        if (bytes > SSID_MAX_WPA_IE_LEN) {
            wps_printf(MSG_INFO, "Too long WPA IE (%d)", bytes);
            LEAVE();
            return;
        }
        hexstr2bin((s8 *) spos, results[idx].wpa_ie, bytes);
        results[idx].wpa_ie_len = bytes;
        wps_printf(DEBUG_WLAN, "AP index %d", idx);
        wps_hexdump(DEBUG_WLAN, "WPA IE from driver :",
                    results[idx].wpa_ie, results[idx].wpa_ie_len);
    } else if (!strncmp(data->pointer, "rsn_ie=", 7)) {
        char *spos;
        int bytes;
        spos = data->pointer + 7;
        bytes = data->length - 7;
        if (bytes & 1) {
            LEAVE();
            return;
        }
        bytes /= 2;
        if (bytes > SSID_MAX_WPA_IE_LEN) {
            wps_printf(MSG_INFO, "Too long RSN IE (%d)", bytes);
            return;
        }
        hexstr2bin((s8 *) spos, results[idx].rsn_ie, bytes);
        results[idx].rsn_ie_len = bytes;
        wps_printf(DEBUG_WLAN, "AP index %d", idx);
        wps_hexdump(DEBUG_WLAN, "RSN IE from driver :",
                    results[idx].rsn_ie, results[idx].rsn_ie_len);
    } else if (!strncmp(data->pointer, "wps_ie=", 7)) {
        char *spos;
        int bytes;
        spos = data->pointer + 7;
        bytes = data->length - 7;
        if (bytes & 1) {
            LEAVE();
            return;
        }
        bytes /= 2;
        if (bytes > MAX_WPS_IE_LEN) {
            wps_printf(MSG_INFO, "Too long WPS IE (%d)", bytes);
            LEAVE();
            return;
        }
        hexstr2bin((s8 *) spos, results[idx].wps_ie, bytes);
        results[idx].wps_ie_len = bytes;
        wps_printf(DEBUG_WLAN, "AP index %d", idx);
        wps_hexdump(DEBUG_WLAN, "WPS IE from driver: ",
                    results[idx].wps_ie, results[idx].wps_ie_len);
    }
    LEAVE();
}
#endif
#endif

#if WIRELESS_EXT >= 18
/** 
 *  @brief parse generic ie
 *
 *  @param results      A pointer to SCAN_RESULTS buffer
 *  @param data         A pointer to iw_point structure
 *  @param idx          AP index
 *  @return             NA
 */
#if 0
static void
parse_generic_ie(SCAN_RESULTS * results, struct iw_point *data, s32 idx)
{
    int offset = 0;
    int buflen;
    unsigned char *buffer;
    unsigned char rsn_oui[3] = { 0x00, 0x0f, 0xac };
#define OUI_LEN 4
    u8 wpa_oui[OUI_LEN] = { 0x00, 0x50, 0xf2, 0x01 };   /* WPA */
    u8 wps_oui[OUI_LEN] = { 0x00, 0x50, 0xf2, 0x04 };   /* WPS */

    ENTER();

    if (!data->pointer || !data->length) {
        wps_printf(MSG_ERROR, "iw_point: Invalid Pointer/Length\n");
        LEAVE();
        return;
    }

    buffer = data->pointer;
    buflen = data->length;

    /* 
     * There might be lots of thing in generic IE.
     * We only process WPA/RSN IE we need
     */

    /* Loop on each IE, each IE is minimum 2 bytes */
    while (offset <= (buflen - 2)) {
        int ielen = buffer[offset + 1] + 2;

        /* Check IE type */
        switch (buffer[offset]) {
        case 0xdd:             /* WPA1 (and other) */
            if (memcmp(&buffer[offset + 2], wpa_oui, 4) == 0) {
                memcpy(results[idx].wpa_ie, buffer + offset, ielen);
                results[idx].wpa_ie_len = ielen;
                wps_printf(DEBUG_WLAN, "AP index %d", idx);
                wps_hexdump(DEBUG_WLAN, "WPA IE from driver :",
                            results[idx].wpa_ie, results[idx].wpa_ie_len);
            } else if (memcmp(&buffer[offset + 2], wps_oui, 4) == 0) {
                memcpy(results[idx].wps_ie, buffer + offset, ielen);
                results[idx].wps_ie_len = ielen;
                wps_printf(DEBUG_WLAN, "AP index %d", idx);
                wps_hexdump(DEBUG_WLAN, "WPS IE from driver :",
                            results[idx].wps_ie, results[idx].wps_ie_len);
            }
            break;
        case 0x30:             /* WPA2 , Len, 2 bytes of version and rsn_oui */
            if ((memcmp(&buffer[offset + 4], rsn_oui, 3) == 0)) {
                memcpy(results[idx].rsn_ie, buffer + offset, ielen);
                results[idx].rsn_ie_len = ielen;
                wps_printf(DEBUG_WLAN, "AP index %d", idx);
                wps_hexdump(DEBUG_WLAN, "RSN IE from driver :",
                            results[idx].rsn_ie, results[idx].rsn_ie_len);
            }
            break;
        default:
            break;
        }
        /* Skip over this IE to the next one in the list. */
        offset += buffer[offset + 1] + 2;
    }

    LEAVE();
}
#endif
#endif

/** 
 *  @brief          Get scan result from WLAN driver
 *
 *  @param results  A pointer to scan result buffer
 *  @return         Number of scan if success, WPS_STATUS_FAIL if fail
 */
#if 0
static int
wlan_get_scan_results(SCAN_RESULTS * results)
{
    struct iwreq iwr;
    struct iw_event iwe;
    struct iw_point iwp;
    int buflen = IW_SCAN_MAX_DATA;
    u8 *buffer = NULL;
    u8 *newbuf = NULL;
    u32 ap_index = 0;
    u32 apNum = 0;
    int new_index = 0;
    s32 len = 0;
    s32 length = 0;

    ENTER();

    memset(&iwr, 0, sizeof(struct iwreq));

  _read_result_:
    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
    newbuf = realloc(buffer, buflen);
    if (newbuf == NULL) {
        if (buffer)
            os_free(buffer);
        wps_printf(MSG_ERROR, "Memory allocation failed !\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    buffer = newbuf;

  _read_again_:
    /* Try to read the results */
    iwr.u.data.pointer = buffer;
    iwr.u.data.length = buflen;
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    if ((ioctl(ioctl_sock, SIOCGIWSCAN, &iwr)) < 0) {
        /* Check if buffer was too small */
        if (errno == E2BIG) {
            wps_printf(DEBUG_WLAN, "Buffer too small !");

            /* Check if the driver gave us any hints. */
            if (iwr.u.data.length > buflen)
                buflen = iwr.u.data.length;
            else
                buflen *= 2;

            /* Try again */
            goto _read_result_;
        } else if (errno == EAGAIN) {
            wps_printf(DEBUG_WLAN, "Reading Scan results again !");

            /* Wait for 100ms similar to WEXT. */
            usleep(100 * 1000);

            /* Try again */
            goto _read_again_;
        } else {
            wps_printf(MSG_ERROR, "Get scan results failed !\n");
            if (buffer)
                os_free(buffer);
            LEAVE();
            return WPS_STATUS_FAIL;
        }
    } else {
        /* We have the results, go to process them */
        wps_printf(DEBUG_WLAN, "Scan results ready, process them ... ");
    }

    length = iwr.u.data.length;

    while (len + IW_EV_LCP_LEN < length) {
        s32 MRV_EV_POINT_LEN;

        memcpy((s8 *) & iwe, buffer + len, sizeof(struct iw_event));

        if (we_version_compiled > 18)
            MRV_EV_POINT_LEN =
                IW_EV_LCP_LEN + sizeof(struct iw_point) - MRV_EV_POINT_OFF;
        else
            MRV_EV_POINT_LEN = IW_EV_LCP_LEN + sizeof(struct iw_point);

        if ((iwe.cmd == SIOCGIWESSID) || (iwe.cmd == SIOCGIWENCODE) ||
#if WIRELESS_EXT >= 18
            (iwe.cmd == IWEVGENIE) ||
#endif
            (iwe.cmd == IWEVCUSTOM)) {
            if (we_version_compiled > 18)
                memcpy((s8 *) & iwp,
                       buffer + len + IW_EV_LCP_LEN - MRV_EV_POINT_OFF,
                       sizeof(struct iw_point));
            else
                memcpy((s8 *) & iwp, buffer + len + IW_EV_LCP_LEN,
                       sizeof(struct iw_point));
            iwp.pointer = buffer + len + MRV_EV_POINT_LEN;
        }

        switch (iwe.cmd) {
        case SIOCGIWAP:
            if (new_index && ap_index < IW_MAX_AP - 1)
                ap_index++;
            memcpy(results[ap_index].bssid, iwe.u.ap_addr.sa_data, ETH_ALEN);
            new_index = 1;
            break;

        case SIOCGIWESSID:
            if ((iwp.pointer) && (iwp.length)) {
                memcpy(results[ap_index].ssid, (s8 *) iwp.pointer, iwp.length);
                results[ap_index].ssid_len = iwp.length;
            }
            break;

        case SIOCGIWENCODE:
            if (!(iwp.flags & IW_ENCODE_DISABLED)) {
                results[ap_index].caps |= IEEE80211_CAP_PRIVACY;
            }
            break;

        case SIOCGIWMODE:
            if (iwe.u.mode == IW_MODE_ADHOC)
                results[ap_index].caps |= IEEE80211_CAP_IBSS;
            else if (iwe.u.mode == IW_MODE_MASTER ||
                     iwe.u.mode == IW_MODE_INFRA)
                results[ap_index].caps |= IEEE80211_CAP_ESS;
            break;

        case SIOCGIWFREQ:      /* frequencey/channel */
            {
                int divi = 1000000, i;
                if (iwe.u.freq.e > 6) {
                    wps_printf(DEBUG_WLAN,
                               "Invalid freq in scan results (BSSID=" MACSTR
                               ": m=%d e=%d\n",
                               MAC2STR(results[ap_index].bssid), iwe.u.freq.m,
                               iwe.u.freq.e);
                    break;
                }
                for (i = 0; i < iwe.u.freq.e; i++)
                    divi /= 10;
                results[ap_index].freq = iwe.u.freq.m / divi;
            }
            break;

#if WIRELESS_EXT > 14
        case IWEVCUSTOM:
            /* iwp point to the start of custom info */
            parse_custom_info(results, &iwp, ap_index);
            break;
#endif

        case IWEVQUAL:         /* signal quality */
            results[ap_index].qual = iwe.u.qual.qual;
            results[ap_index].noise = iwe.u.qual.noise;
            results[ap_index].level = iwe.u.qual.level;
            break;

#if WIRELESS_EXT >= 18
        case IWEVGENIE:
            /* iwp point to the start of generic IE */
            parse_generic_ie(results, &iwp, ap_index);
            break;
#endif
        }

        len += iwe.len;
    }
    if (new_index)
        apNum = ap_index + 1;

    if (buffer)
        os_free(buffer);

    LEAVE();
    return apNum;
}
#endif

/********************************************************
        Global Functions
********************************************************/

/** 
 *  @brief Send scan ioctl command to WLAN driver
 *  @param rf_band  band for which scan is to be performed
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
#if 0
static int
wps_wlan_set_scan(int rf_band, u8 prev_results, char * ssid)
{
    struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;
    wlan_ioctl_user_scan_cfg scan_req;
    u8 chan_list_5G[] = { 36, 40, 44, 48, 52, 56, 60, 64,
        100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140,
        149, 153, 157, 161, 165
    };
    int ioctl_val, subioctl_val, i;
    u8 temp_rf_band = 0;

    ENTER();

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, "mlan0", strlen("mlan0"));

    memset(&scan_req, 0x00, sizeof(scan_req));

    if (ssid) {
        scan_req.ssid_list[0].max_len = MIN(strlen(ssid), MRVDRV_MAX_SSID_LENGTH);
        strncpy(scan_req.ssid_list[0].ssid, ssid, scan_req.ssid_list[0].max_len);
    }

    if (prev_results == KEEP_PREV_RESULT)
        scan_req.keep_previous_scan = prev_results;

    if (rf_band == RF_24_G) {
        /* Update Probe Request IE */
        temp_rf_band = gpwps_info->enrollee.rf_bands;
        gpwps_info->enrollee.rf_bands = RF_24_G;

        if (probe_ie_index != -1)
            wps_wlan_probe_request_ie_config(SET_WPS_STA_SESSION_ACTIVE_IE,
                                             &probe_ie_index);
        gpwps_info->enrollee.rf_bands = temp_rf_band;
        for (i = 0; i < 11; i++) {
            scan_req.chan_list[i].chan_number = i + 1;
            scan_req.chan_list[i].radio_type = 0;
        }
    } else if (rf_band == RF_50_G) {
        /* Update Probe Request IE */
        temp_rf_band = gpwps_info->enrollee.rf_bands;
        gpwps_info->enrollee.rf_bands = RF_50_G;

        if (probe_ie_index != -1)
            wps_wlan_probe_request_ie_config(SET_WPS_STA_SESSION_ACTIVE_IE,
                                             &probe_ie_index);
        gpwps_info->enrollee.rf_bands = temp_rf_band;
        for (i = 0; i < sizeof(chan_list_5G) / sizeof(u8); i++) {
            scan_req.chan_list[i].chan_number = chan_list_5G[i];
            scan_req.chan_list[i].radio_type = 1;
        }
    }

#if 0
    if (get_priv_ioctl("setuserscan",
                       &ioctl_val, &subioctl_val) == WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "setuserscan IOCTL not present\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
#else	
    ioctl_val = WOAL_SET_GET_2K_BYTES;
    subioctl_val = WOAL_SET_USER_SCAN;
#endif	

    iwr.u.data.pointer = (caddr_t) & scan_req;
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.length = sizeof(scan_req);

    if (WlanIoctl(ioctl_val, &iwr) < 0) {
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief Send user scan ioctl command to WLAN driver
 *  @param channel  channel for which scan is to be performed
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_wlan_set_user_scan(WFD_DATA * pwfd_data, int operating_channel,
                       int scan_on_chan_list)
{
    wlan_ioctl_user_scan_cfg scan_req;
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    channel_list scan_channel_list;
    int i = 0, j = 0, k = 0;

#define MAX_CHAN_BG_BAND 14

#if 0
    if (get_priv_ioctl("setuserscan",
                       &ioctl_val, &subioctl_val) == WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "setuserscan IOCTL not present\n");
        return WPS_STATUS_FAIL;
    }
#else
	ioctl_val = WOAL_SET_GET_2K_BYTES;
	subioctl_val = WOAL_SET_USER_SCAN;
#endif	
    memset(&scan_req, 0x00, sizeof(scan_req));
    memset(&scan_channel_list, 0x00, sizeof(channel_list));
    if (operating_channel) {
        wps_printf(MSG_INFO, "Scanning operating channel %d and wildcard ssid '%s'....\n",
               operating_channel,
               pwfd_data->find_results[wps_s->wfd_data.dev_index].go_ssid);
        scan_req.chan_list[0].chan_number = operating_channel;
        if (scan_req.chan_list[0].chan_number > MAX_CHAN_BG_BAND)
            scan_req.chan_list[0].radio_type = 1;
    } else if (scan_on_chan_list) {
        for (i = 0; i < pwfd_data->self_channel_list.num_of_chan; i++) {
            for (j = 0;
                 j <
                 pwfd_data->find_results[wps_s->wfd_data.dev_index].
                 peer_channel_list.num_of_chan; j++) {
                if (pwfd_data->self_channel_list.chan[i] ==
                    pwfd_data->find_results[wps_s->wfd_data.dev_index].
                    peer_channel_list.chan[j]) {
                    scan_channel_list.num_of_chan++;
                    scan_channel_list.chan[k++] =
                        pwfd_data->self_channel_list.chan[i];
                }
            }
        }
        wps_printf(MSG_INFO, "Scanning channels ");
        for (i = 0; i < scan_channel_list.num_of_chan; i++) {
            wps_printf(MSG_INFO, "%d ", scan_channel_list.chan[i]);
            scan_req.chan_list[i].chan_number = scan_channel_list.chan[i];
            if (scan_req.chan_list[i].chan_number > MAX_CHAN_BG_BAND)
                scan_req.chan_list[i].radio_type = 1;
        }
        wps_printf(MSG_INFO, "in the list and ssid '%s'....\n",
               pwfd_data->find_results[wps_s->wfd_data.dev_index].go_ssid);
    } else {
        return WPS_STATUS_FAIL;
    }
    if ((!memcmp
         (pwfd_data->find_results[wps_s->wfd_data.dev_index].go_ssid, "DIRECT-",
          7))
        && (sizeof(pwfd_data->find_results[wps_s->wfd_data.dev_index].go_ssid) >
            7)) {
        memcpy(&scan_req.ssid_list[0].ssid,
               (u8 *) & pwfd_data->find_results[wps_s->wfd_data.dev_index].
               go_ssid,
               sizeof(pwfd_data->find_results[wps_s->wfd_data.dev_index].
                      go_ssid));
        scan_req.ssid_list[0].max_len = 0;
    } else {
        strcpy(scan_req.ssid_list[0].ssid, "DIRECT-*");
        scan_req.ssid_list[0].max_len = 0xFF;
    }
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, "mlan0", strlen("mlan0"));

    iwr.u.data.pointer = (caddr_t) & scan_req;
    /* set flags to WOAL_SET_USER_SCAN i.e. 3 */
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.length = sizeof(scan_req);
    if (WlanIoctl(ioctl_val, &iwr) < 0) {
        return WPS_STATUS_FAIL;
    }
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Process scan results get from WLAN driver
 *
 *  @param wps_s       Pointer to WPS global structure
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_wlan_get_all_scan_results(WPS_DATA * wps_s)
{
    SCAN_RESULTS *results;
    int num;
    
    ENTER();

    results = os_malloc(SCAN_AP_LIMIT * sizeof(SCAN_RESULTS));
    if (results == NULL) {
        wps_printf(MSG_WARNING, "Failed to allocate memory for scan "
                   "results");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    memset(results, 0, SCAN_AP_LIMIT * sizeof(SCAN_RESULTS));

    num = wlan_get_scan_results(results);
    wps_printf(DEBUG_WLAN, "Scan results: %d", num);

    if (num < 0) {
        wps_printf(DEBUG_WLAN, "Failed to get scan results");
        os_free(results);
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    if (num == 0) {
        wps_s->num_scan_results = 0;
        wps_printf(DEBUG_WLAN, "Didn't find any APs");
        os_free(results);
        LEAVE();
        return WPS_STATUS_SUCCESS;
    }
    if (num > SCAN_AP_LIMIT) {
        wps_printf(DEBUG_WLAN, "Not enough room for all APs (%d < %d)",
                   num, SCAN_AP_LIMIT);
        num = SCAN_AP_LIMIT;
    }

    wps_s->num_scan_results = num;

    // Free previous results
    if (wps_s->scan_results)
        os_free(wps_s->scan_results);

    // Store results
    wps_s->scan_results = results;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}


/** 
 *  @brief Process scan results get from WLAN driver
 *
 *  @param wps_s       Pointer to WPS global structure
 *  @param filter      Filter AP in scan result
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_wlan_scan_results(WPS_DATA * wps_s, u16 filter, char *ssid)
{
    SCAN_RESULTS *results, *tmp;
    int num;
    u16 type;

    ENTER();

    results = os_malloc(SCAN_AP_LIMIT * sizeof(SCAN_RESULTS));
    if (results == NULL) {
        wps_printf(MSG_WARNING, "Failed to allocate memory for scan "
                   "results");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    memset(results, 0, SCAN_AP_LIMIT * sizeof(SCAN_RESULTS));

    num = wlan_get_scan_results(results);
    wps_printf(DEBUG_WLAN, "Scan results: %d", num);

    if (num < 0) {
        wps_printf(DEBUG_WLAN, "Failed to get scan results");
        os_free(results);
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    if (num == 0) {
        wps_s->num_scan_results = 0;
        wps_printf(DEBUG_WLAN, "Didn't find any APs");
        os_free(results);
        LEAVE();
        return WPS_STATUS_SUCCESS;
    }
    if (num > SCAN_AP_LIMIT) {
        wps_printf(DEBUG_WLAN, "Not enough room for all APs (%d < %d)",
                   num, SCAN_AP_LIMIT);
        num = SCAN_AP_LIMIT;
    }
    if ((filter == FILTER_PBC) || (filter == FILTER_PIN)) {
        int k, match = 0;

        tmp = os_malloc(num * sizeof(SCAN_RESULTS));
        if (tmp == NULL) {
            wps_printf(MSG_WARNING,
                       "Failed to allocate memory for scan results");
            LEAVE();
            return WPS_STATUS_FAIL;
        }

        for (k = 0; k < num; k++) {
            if (results[k].wps_ie_len > 0) {
                type =
                    wps_probe_response_device_password_id_parser(results[k].
                                                                 wps_ie,
                                                                 results[k].
                                                                 wps_ie_len);

                // Filter the SSIDs if they passed one in.
                if ((ssid == NULL) || ((memcmp(results[k].ssid, ssid, results[k].ssid_len) == 0) && 
                                        strlen(ssid) == results[k].ssid_len))
                {
                    if (((filter == FILTER_PBC) && (type == DEVICE_PASSWORD_PUSH_BUTTON)) ||
                        ((filter == FILTER_PIN) && (type == DEVICE_PASSWORD_ID_PIN)))
                    {
                        memcpy((void *) &tmp[match], (void *) &results[k],
                               sizeof(SCAN_RESULTS));
                        match++;
                    }
                }
            }
        }                       /* end of k loop */

        if (results)
            os_free(results);

        wps_s->num_scan_results = match;
        if (match > 0) {
            if (wps_s->scan_results)
                os_free(wps_s->scan_results);
            wps_s->scan_results = tmp;
        } else {
            if (tmp)
                os_free(tmp);
        }
    } /* PBC */
    else {
        int k, match = 0;

        /* Only save Infra mode AP */
        tmp = os_malloc(num * sizeof(SCAN_RESULTS));
        if (tmp == NULL) {
            wps_printf(MSG_WARNING,
                       "Failed to allocate memory for scan results");
            LEAVE();
            return WPS_STATUS_FAIL;
        }

        for (k = 0; k < num; k++)
        {
            // Filter the SSIDs if they passed one in.
            if ((ssid == NULL) || ((memcmp(results[k].ssid, ssid, results[k].ssid_len) == 0) && 
                                    strlen(ssid) == results[k].ssid_len))
            {
                if (results[k].caps & IEEE80211_CAP_ESS) {
                    memcpy((void *) &tmp[match], (void *) &results[k],
                           sizeof(SCAN_RESULTS));
                    match++;
                }
            }
        }                       /* end of k loop */

        if (results)
            os_free(results);

        wps_s->num_scan_results = match;
        if (match > 0) {
            if (wps_s->scan_results)
                os_free(wps_s->scan_results);
            wps_s->scan_results = tmp;
        } else {
            if (tmp)
                os_free(tmp);
        }
    }

    /* For WFD, check for matching device Id and send connect request */
    if (gpwps_info->role == WFD_ROLE) {
        wps_printf(DEBUG_WFD_DISCOVERY, "\nScan Result : %d entries.\n",
                   wps_s->num_scan_results);
        int i;
        for (i = 0; i < wps_s->num_scan_results; i++) {
            if (!
                (memcmp
                 (wps_s->wfd_data.find_results[wps_s->wfd_data.dev_index].
                  intended_address, wps_s->scan_results[i].bssid, ETH_ALEN))) {
                wps_s->wfd_data.dev_found = i;
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "\nSelected Device found at %d.\n", i);
                break;
            }
        }
		/* reset dev_found, if not found in the scan results */
		if (i == wps_s->num_scan_results) {
		    wps_s->wfd_data.dev_found = -1;
		}
    } else
    {
        int i, j;

        wps_printf(MSG_INFO, "\nScan Result : %d entry in Scan List\n",
               wps_s->num_scan_results);
        if (wps_s->num_scan_results == 0) {
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("\nNo device found with %s password.\n",
                   (gpwps_info->enrollee.device_password_id ==
                    DEVICE_PASSWORD_ID_PIN)
                   ? "PIN" : "PBC"));
        }
        if (wps_s->num_scan_results > 0) {
            updateWPSOutcome(WPS_STATE_CONNECTING);
            DPRINTF(DBG_SOFT|DBG_OUTPUT, 
                ("-----------------------------------------------------------------------\n"));
            DPRINTF(DBG_SOFT|DBG_OUTPUT, 
                ("  # |  Mode | Chan# | WPA | WPS |       BSSID       |     SSID         \n"));
            DPRINTF(DBG_SOFT|DBG_OUTPUT, 
                ("-----------------------------------------------------------------------\n"));

            for (i = 0; i < wps_s->num_scan_results; i++) {
                /* Index Number */
                DPRINTF(DBG_SOFT|DBG_OUTPUT, (" %02d |", i));

                /* Mode */
                if (wps_s->scan_results[i].caps & IEEE80211_CAP_IBSS)
                {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, (" Adhoc |"));
                }
                else
                {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, (" Infra |"));
                }

                /* Channel Number */
                DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  %03d  |",
                       mapping_freq_to_chan(wps_s->scan_results[i].freq)));

                /* WPA Enabled ? */
                if (wps_s->scan_results[i].wpa_ie_len != 0 ||
                    wps_s->scan_results[i].rsn_ie_len != 0) {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  Y  |"));
                } else {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  N  |"));
                }

                if (wps_s->scan_results[i].wps_ie_len != 0) {
                    type =
                        wps_probe_response_device_password_id_parser(wps_s->
                                                                     scan_results
                                                                     [i].wps_ie,
                                                                     wps_s->
                                                                     scan_results
                                                                     [i].
                                                                     wps_ie_len);

                    if (type == DEVICE_PASSWORD_PUSH_BUTTON)
                    {
                        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  Y* |"));
                    }
                    else if (type == DEVICE_PASSWORD_ID_PIN)
                    {
                        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  Y# |"));
                    }
                    else
                    {
                        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  Y  |"));
                    }
                } else {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("  N  |"));
                }

                /* BSSID */
                DPRINTF(DBG_SOFT|DBG_OUTPUT, (" %02x:%02x:%02x:%02x:%02x:%02x |",
                       wps_s->scan_results[i].bssid[0],
                       wps_s->scan_results[i].bssid[1],
                       wps_s->scan_results[i].bssid[2],
                       wps_s->scan_results[i].bssid[3],
                       wps_s->scan_results[i].bssid[4],
                       wps_s->scan_results[i].bssid[5]));

                /* SSID */
                DPRINTF(DBG_SOFT|DBG_OUTPUT, (" "));
                for (j = 0; j < wps_s->scan_results[i].ssid_len; j++) {
                    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("%c", wps_s->scan_results[i].ssid[j]));
                }
                DPRINTF(DBG_SOFT|DBG_OUTPUT, ("\n\n"));
            }
        }
    }

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

#ifdef WPS_IE_NEEDED
/** 
 *  @brief Set WPS IE for foreground scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_wlan_set_fgscan_wps_ie(void)
{
    u8      *buf;
    int     wps_ie_size, buf_len;
    int     ret = WPS_STATUS_SUCCESS;
    //struct iwreq    iwr;
    //int     ioctl_val, subioctl_val;

    ENTER();

    buf = (u8 *)os_malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf) {
        memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        buf[0] = 221;          /* ElementID of WPS_IE */
        wps_ie_size = wps_sta_probe_request_prepare(&buf[2]);
        buf[1] = (u8)wps_ie_size;  /* IE len */
        buf_len = wps_ie_size + 2; /* Add IE_ID and IE_Len fileds */

        // wps_hexdump(DEBUG_WLAN, "WPS_IE",(const unsigned char *)buf, buf_len);
        extern void wps_set_gen_ie_helper(u8* buf, int buf_len);
        wps_set_gen_ie_helper(buf, buf_len);

        os_free(buf);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Reset WPS IE for foreground scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
#if 0
int
wps_wlan_reset_wps_ie(void)
{
    u8 buf[10];
    int buf_len;
    u8 wps_oui[4] = { 0x00, 0x50, 0xF2, 0x04 };
    int ret = WPS_STATUS_SUCCESS;
    struct iwreq iwr;
    int ioctl_val, subioctl_val;

    ENTER();

    buf[0] = 221;               /* ElementID of WPS_IE */
    memcpy(&buf[2], wps_oui, sizeof(wps_oui));
    buf[1] = sizeof(wps_oui);   /* IE len */
    buf_len = sizeof(wps_oui) + 2;      /* Add IE_ID and IE_Len fileds */

    /* 
     * Always try private ioctl "setgenie" first
     * If private ioctl failed and if driver build with WE version >= 18,
     * try standard ioctl "SIOCSIWGENIE".
     */
    if (get_priv_ioctl("setgenie", &ioctl_val, &subioctl_val) ==
        WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "ERROR : ioctl[setgenie] NOT SUPPORT !");
#if (WIRELESS_EXT >= 18)
        if (we_version_compiled < 18) {
            ret = WPS_STATUS_FAIL;
            goto _exit_;
        } else
            goto _we18_;
#else
        ret = WPS_STATUS_FAIL;
        goto _exit_;
#endif
    }

    /* 
     * Set up and execute the ioctl call
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = buf_len;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(ioctl_sock, ioctl_val, &iwr) < 0) {
        perror("ioctl[setgenie]");
#if (WIRELESS_EXT >= 18)
        if (we_version_compiled < 18) {
            ret = WPS_STATUS_FAIL;
            goto _exit_;
        }
#else
        ret = WPS_STATUS_FAIL;
        goto _exit_;
#endif
    } else {
        ret = WPS_STATUS_SUCCESS;
        goto _exit_;
    }

#if (WIRELESS_EXT >= 18)
  _we18_:
    /* 
     * If private ioctl failed and if driver build with WE version >= 18,
     * try standard ioctl "SIOCSIWGENIE".
     */
    if (we_version_compiled >= 18) {
        /* 
         * Driver WE version >= 18, use standard ioctl
         */
        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) buf;
        iwr.u.data.length = buf_len;

        if (ioctl(ioctl_sock, SIOCSIWGENIE, &iwr) < 0) {
            perror("ioctl[SIOCSIWGENIE]");
            ret = WPS_STATUS_FAIL;
            goto _exit_;
        } else {
            ret = WPS_STATUS_SUCCESS;
            goto _exit_;
        }
    }
#endif /* (WIRELESS_EXT >= 18) */

  _exit_:
    LEAVE();
    return ret;
}
#endif
#endif

/** 
 *  @brief  Get associated ESSID from WLAN driver (SIOCGIWESSID)
 *
 *  @param ssid       Buffer for the SSID, must be at least 32 bytes long
 *  @return           SSID length on success, WPS_STATUS_FAIL on failure
 */
int
wps_wlan_get_ssid(u8 * ssid)
{
    int len = 0;

    ENTER();

    len = WlanShimCGetSsid((char *)ssid);

    LEAVE();
    return len;
}

/** 
 *  @brief  Set ESSID to associate to WLAN driver (SIOCSIWESSID)
 *
 *  @param ssid       SSID
 *  @param ssid_len   Length of SSID (0..32)
 *  @param skip_scan  Skip scan during association
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_ssid(const u8 * ssid, size_t ssid_len, int skip_scan)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    if (ssid_len > 32) {
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    WlanAssoc((char *) ssid, (int) ssid_len, skip_scan);
    WlanClearEncryption();

    LEAVE();
    return ret;
}

/** 
 *  @brief  Set Auth Mode to WLAN driver (SIOCSIWAUTH)
 *
 *  @param auth_mode  Open or shared auth mode.
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
#if 0
int
wps_wlan_set_authentication(int auth_mode)
{
    struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);
    iwr.u.param.flags = IW_AUTH_80211_AUTH_ALG;

    if (auth_mode == AUTHENTICATION_TYPE_SHARED)
        iwr.u.param.value = IW_AUTH_ALG_SHARED_KEY;
    else
        iwr.u.param.value = IW_AUTH_ALG_OPEN_SYSTEM;

    if (ioctl(ioctl_sock, SIOCSIWAUTH, &iwr) < 0) {
        perror("ioctl[SIOCSIWAUTH]");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief  Get associated BSSID from WLAN driver (SIOCGIWAP)
 *
 *  @param bssid      Buffer for the BSSID
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_get_wap(u8 * bssid)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    wifi_intf_get_var_bssid(WIFI_IFACE_ID_STATION, bssid, ETH_ALEN);

    LEAVE();
    return ret;
}


/** 
 *  @brief  Set BSSID to associate to WLAN driver (SIOCSIWAP)
 *
 *  @param bssid      BSSID
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_wap(const u8 * bssid)
{
    int ret;
    ENTER();

    ret = WlanSetWAP((char *)bssid);

    LEAVE();
    return ret;
}


/** 
 *  @brief  Set wireless mode to WLAN driver (infra/adhoc) (SIOCSIWMODE)
 *
 *  @param mode       0 = infra/BSS (associate with an AP), 1 = adhoc/IBSS
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_mode(int mode)
{
    // struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;
    wifi_intf_comm_mode_t wlanMode;

    ENTER();
	
    // remap from the WPS code's values to our internal values
    if (mode == IEEE80211_MODE_INFRA)
        wlanMode = INFRASTRUCTURE;
    else
        wlanMode = AD_HOC;

    ret = WlanSetMode(wlanMode);

    LEAVE();
    return ret;
}

#if 0
/** 
 *  @brief  Set BSS type (STA/uAP)
 *
 *  @param bss_type   an integer 0: Sta, 1:uAP
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_bss_type(int bss_type)
{
    int ret;
	
	ENTER();
	
	ret = wlan_set_bss_type(WIFI_IFACE_NAME_WFD, bss_type);

    LEAVE();
    return ret;
}

/** 
 *  @brief  Get BSS type (STA/uAP)
 *
 *  @param bss_type   A pointer to an integer
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_get_bss_type(int *bss_type)
{
    int ret;
	
    ENTER();
	
	ret = wlan_get_bss_type(WIFI_IFACE_NAME_WFD, bss_type);

    LEAVE();
    return ret;
}
#endif

#if 1
/** uAP Protocol: Open */
#define UAP_PROTO_OPEN         1
/** uAP Protocol: WEP Static */
#define UAP_PROTO_WEP_STATIC   2
/** uAP Protocol: WPA */
#define UAP_PROTO_WPA          8
/** uAP Protocol: WPA2 */
#define UAP_PROTO_WPA2         32
/** uAP Protocol: WPA+WPA2 */
#define UAP_PROTO_MIXED        40

/** uAP Auth Mode: Open */
#define UAP_AUTH_MODE_OPEN     0
/** uAP Auth Mode: Shared Key */
#define UAP_AUTH_MODE_SHARED   1

/** uAP Cipher: None */
#define UAP_CIPH_NONE          0
/** uAP Cipher: TKIP */
#define UAP_CIPH_TKIP          4
/** uAP Cipher: AES */
#define UAP_CIPH_AES           8
/** uAP Cipher:  AES+TKIP*/
#define UAP_CIPH_AES_TKIP      12

/** 
 *  @brief Set the BSS information into the registrar credential structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *  @param bss       BSS information
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
load_cred_info(WPS_DATA * wps_s, PWPS_INFO pwps_info, bss_config_t * bss)
{
    int i, ret = WPS_STATUS_SUCCESS;
    CREDENTIAL_DATA *reg_cred = NULL;
    MESSAGE_ENROLLEE_REGISTRAR *enr_reg = NULL;
    u8 cipher = 0;

    ENTER();

    if (!bss) {
        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("load_cred_info: BSS info is not present\n"));
        wps_printf(MSG_ERROR, "BSS info is not present\n");
        ret = WPS_STATUS_FAIL;
        return ret;
    }

    if ((wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) ||
        (wps_s->bss_type == BSS_TYPE_UAP && pwps_info->role == WPS_ENROLLEE)) {
        enr_reg = &pwps_info->enrollee;
    } else {
        enr_reg = &pwps_info->registrar;
    }

    reg_cred = &enr_reg->cred_data[0];

    memset(reg_cred, 0, sizeof(CREDENTIAL_DATA));
    if (bss->ssid.ssid_len) {
        memcpy(reg_cred->ssid, bss->ssid.ssid, bss->ssid.ssid_len);
        reg_cred->ssid_length = bss->ssid.ssid_len;
        memcpy(wps_s->current_ssid.ssid, bss->ssid.ssid, bss->ssid.ssid_len);
        wps_s->current_ssid.ssid_len = bss->ssid.ssid_len;
        wps_printf(DEBUG_INIT, "SSID '%s'", wps_s->current_ssid.ssid);
    }
    switch (bss->protocol) {
    case UAP_PROTO_OPEN:
        reg_cred->auth_type = AUTHENTICATION_TYPE_OPEN;
        reg_cred->encry_type = ENCRYPTION_TYPE_NONE;
        break;
    case UAP_PROTO_WEP_STATIC:
        reg_cred->encry_type = ENCRYPTION_TYPE_WEP;
        break;
    case UAP_PROTO_WPA:
        reg_cred->auth_type = AUTHENTICATION_TYPE_WPAPSK;
        break;
    case UAP_PROTO_WPA2:
        reg_cred->auth_type = AUTHENTICATION_TYPE_WPA2PSK;
        break;
    case UAP_PROTO_MIXED:
        reg_cred->auth_type = AUTHENTICATION_TYPE_WPA_MIXED;
        break;
    default:
        break;
    }

    wps_printf(MSG_ERROR, "bss->band_cfg %d, bss->channel %d\n", bss->band_cfg, bss->channel);

    if (bss->band_cfg & 0x01 || bss->channel > MAX_CHANNELS)
    {
        reg_cred->rf_bands = RF_50_G;
        wps_printf(MSG_ERROR, "reg_cred->rf_bands = RF_50_G\n");
    }

    if (reg_cred->encry_type == ENCRYPTION_TYPE_WEP) {
        /* For WEP */
        for (i = 0; i < 4; i++) {
            if (bss->wep_cfg[i].is_default) {
                /* Get the current default key settings */
                memcpy(reg_cred->network_key, bss->wep_cfg[i].key,
                       bss->wep_cfg[i].length);
                reg_cred->network_key_len = bss->wep_cfg[i].length;
                reg_cred->network_key_index = bss->wep_cfg[i].key_index + 1;
                break;
            }
        }
        wps_printf(DEBUG_INIT, "NETWORK KEY INDEX = %d",
                   reg_cred->network_key_index - 1);
        wps_hexdump(DEBUG_INIT, "NETWORK_KEY",
                    (const unsigned char *) reg_cred->network_key,
                    reg_cred->network_key_len);
        switch (bss->auth_mode) {
        case UAP_AUTH_MODE_OPEN:
            reg_cred->auth_type = AUTHENTICATION_TYPE_OPEN;
            break;
        case UAP_AUTH_MODE_SHARED:
            reg_cred->auth_type = AUTHENTICATION_TYPE_SHARED;
            break;
        default:
            break;
        }
    }
    wps_printf(DEBUG_INIT, "AUTHENTICATION TYPE = 0x%04x\n",
               enr_reg->cred_data[0].auth_type);
    if ((reg_cred->auth_type == AUTHENTICATION_TYPE_WPAPSK) ||
        (reg_cred->auth_type == AUTHENTICATION_TYPE_WPA2PSK) ||
        (reg_cred->auth_type == AUTHENTICATION_TYPE_WPA_MIXED) ||
        (reg_cred->auth_type == AUTHENTICATION_TYPE_ALL)) {
        /* For WPA/WPA2 */

        wps_printf(DEBUG_INIT, "\nOK\n%s:%d\n",__FILE__,__LINE__);
        if (reg_cred->auth_type == AUTHENTICATION_TYPE_WPAPSK)
            cipher = bss->wpa_cfg.pairwise_cipher_wpa;
        else
            cipher = bss->wpa_cfg.pairwise_cipher_wpa2;

        switch (cipher) {
        case UAP_CIPH_NONE:
            reg_cred->encry_type = ENCRYPTION_TYPE_NONE;
            break;
        case UAP_CIPH_TKIP:
            reg_cred->encry_type = ENCRYPTION_TYPE_TKIP;
            break;
        case UAP_CIPH_AES:
            reg_cred->encry_type = ENCRYPTION_TYPE_AES;
            break;
        case UAP_CIPH_AES_TKIP:
            reg_cred->encry_type = ENCRYPTION_TYPE_TKIP_AES_MIXED;
            break;
        default:
            break;
        }
        wps_printf(DEBUG_INIT, "\nOK\n%s:%d\n",__FILE__,__LINE__);
        memcpy(reg_cred->network_key, bss->wpa_cfg.passphrase,
               bss->wpa_cfg.length);
        wps_printf(DEBUG_INIT, "\nOK\n%s:%d\n",__FILE__,__LINE__);
        if (reg_cred->encry_type != ENCRYPTION_TYPE_WEP) {
            reg_cred->network_key_len = bss->wpa_cfg.length;
            reg_cred->network_key_index = 1;
        }
        wps_printf(DEBUG_INIT, "NETWORK_KEY '%s'",
                   enr_reg->cred_data[0].network_key);
    }
    wps_printf(DEBUG_INIT, "ENCRYPTION TYPE = 0x%04x",
               enr_reg->cred_data[0].encry_type);

    /* Both auth_type and encry_type can not be ZERO */
    if (!reg_cred->auth_type && !reg_cred->encry_type) {
        reg_cred->auth_type = AUTHENTICATION_TYPE_OPEN;
        wps_printf(MSG_INFO,
                   "Incorrect Auth encryption types, Using open security\n");
    }
    if (enr_reg->version >= WPS_VERSION_2DOT0) {
        /* Dont go ahead if current security configuration is prohibited */
        switch (reg_cred->encry_type) {

        case ENCRYPTION_TYPE_TKIP:
            wps_printf(MSG_ERROR, "WPA-TKIP is deprecated from WSC2.0.\n");
            ret = WPS_STATUS_FAIL;
            break;
        case ENCRYPTION_TYPE_WEP:
            wps_printf(MSG_ERROR, "WEP is deprecated from WSC2.0.\n");
            ret = WPS_STATUS_FAIL;
            break;
        default:
            break;
        }
    }

    LEAVE();
    return ret;
}

#ifdef UAP_SUPPORT
/** 
 *  @brief Change AP configuration as per registrar credential structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wlan_change_ap_configuration(WPS_DATA * wps_s, PWPS_INFO pwps_info)
{
    MESSAGE_ENROLLEE_REGISTRAR *enr_reg = NULL;
    bss_config_t bss;
    int ret = WPS_STATUS_SUCCESS;
    CREDENTIAL_DATA *pCred = NULL;

    ENTER();

    apcmd_stop_bss(wps_s);

    enr_reg = &pwps_info->registrar;
    pCred = &enr_reg->cred_data[0];
    wps_printf(MSG_ERROR, "ap_conf: pCred: %p", pCred);

    memset(&bss, 0, sizeof(bss_config_t));

    /* Read current params for default values */
    if ((ret = apcmd_get_bss_config(wps_s, &bss)) != WPS_STATUS_SUCCESS) {
        goto done;
    }

    wps_cred_to_bss_config(&bss, pCred, pwps_info->config_load_by_oob);

    wps_printf(DEBUG_WLAN, "====== new credentials ======\n");

    wps_printf(DEBUG_WLAN,
               "SSID:%s proto:0x%x pair_cip_wpa:0x%x pair_cip_wpa2:0x%x group_cip:0x%x\n",
               bss.ssid.ssid, bss.protocol, bss.wpa_cfg.pairwise_cipher_wpa,
               bss.wpa_cfg.pairwise_cipher_wpa2, bss.wpa_cfg.group_cipher);

    wps_hexdump(DEBUG_WLAN, "Net key(PSK)",
                (u8 *) bss.wpa_cfg.passphrase, bss.wpa_cfg.length);

    /* Set updated params */
    ret = apcmd_set_bss_config(wps_s, &bss);

    apcmd_start_bss(wps_s);

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief Reset AP configuration for Reversed Role or OOB.
 *
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS on success, WPS_STATUS_FAIL on fail
 */
int
wps_wlan_reset_ap_config(WPS_DATA * wps_s, PWPS_INFO pwps_info)
{
    int ret = WPS_STATUS_SUCCESS;
    bss_config_t bss;

    ENTER();
    memset(&bss, 0, sizeof(bss_config_t));

    /* Read current params for default values */
    if ((ret = apcmd_get_bss_config(wps_s, &bss)) != WPS_STATUS_SUCCESS) {
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    if (pwps_info->role == WPS_ENROLLEE
        && pwps_info->read_ap_config_only == WPS_CANCEL) {

        wps_printf(DEBUG_WLAN,
                   "Delay loading new AP config and restart by 200 milli-seconds!");
        tx_thread_sleep(20);
        /* Update the new AP's settings received from the Registrar */
        wlan_change_ap_configuration(wps_s, pwps_info);

    } else if (pwps_info->role == WPS_REGISTRAR
               && pwps_info->wps_device_state == SC_NOT_CONFIGURED_STATE) {

        pwps_info->config_load_by_oob = WPS_SET;
        wps_printf(DEBUG_WLAN,
                   "Delay loading new AP config and restart by 200 milli-seconds!");
        tx_thread_sleep(20);
        /* Update the new Randomly generated AP setting after OOB reset from
           the Registrar */
        wlan_change_ap_configuration(wps_s, pwps_info);
    } else {

        wps_printf(DEBUG_WLAN, "AP configuration unchanged!");

        /* For UAP mode */
        wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
        pwps_info->mode = IEEE80211_MODE_INFRA;

        /* 
         * Send APCMD_SYS_CONFIGURE command to set WPS IE
         */
        wps_wlan_ie_config(SET_WPS_IE, &ie_index);
        if (pwps_info->registrar.version >= WPS_VERSION_2DOT0 ||
            pwps_info->enrollee.version >= WPS_VERSION_2DOT0) {
            ret =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                   &ap_assocresp_ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
            }
        }

        pwps_info->input_state = WPS_INPUT_STATE_ROLE;
        pwps_info->pin_pbc_set = WPS_CANCEL;

        wps_print_uap_role_menu(pwps_info);

    }

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief Copy BSS configuration into PWPS_INFO  structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_load_uap_cred(WPS_DATA * wps_s, PWPS_INFO pwps_info)
{
    int ret;
    bss_config_t bss;

    ENTER();

    memset(&bss, 0, sizeof(bss_config_t));

    if ((ret = apcmd_get_bss_config(wps_s, &bss)) != WPS_STATUS_SUCCESS) {
        goto done;
    }

    /* set credentials */
    ret = load_cred_info(wps_s, pwps_info, &bss);

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief Converts a string to hex value
 *
 *  @param str      A pointer to the string
 *  @param raw      A pointer to the raw data buffer
 *  @return         Number of bytes read
 */
int
string2raw(char *str, unsigned char *raw)
{
    int len = (strlen(str) + 1) / 2;

    do {
        if (!isxdigit(*str)) {
            return -1;
        }
        *str = toupper(*str);
        *raw = CHAR2INT(*str) << 4;
        ++str;
        *str = toupper(*str);
        if (*str == '\0')
            break;
        *raw |= CHAR2INT(*str);
        ++raw;
    } while (*++str != '\0');
    return len;
}

/** 
 *  @brief Copy Credential data into BSS configuration
 *  @param bss       A pointer to the bss_config_t structure
 *  @param pCred     A pointer to the CREDENTIAL_DATA structure
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_cred_to_bss_config(bss_config_t * bss, PCREDENTIAL_DATA pCred,
                       u8 load_by_oob)
{
    /* Convert cred info to bss-config_t */
    int ret = WPS_STATUS_SUCCESS;
    unsigned char network_key[MAX_WEP_KEY_LEN];

    ENTER();

    /* Copy SSID */
    memset(bss->ssid.ssid, 0, MAX_SSID_LENGTH);
    bss->ssid.ssid_len = pCred->ssid_length;
    memcpy(bss->ssid.ssid, pCred->ssid, pCred->ssid_length);
    /* Enable SSID bcast ctrl */
    bss->bcast_ssid_ctl = 1;

    /* Copy Auth Type */
    switch (pCred->auth_type) {
    case AUTHENTICATION_TYPE_OPEN:
        bss->protocol = UAP_PROTO_OPEN;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    case AUTHENTICATION_TYPE_WPAPSK:
        bss->protocol = UAP_PROTO_WPA;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    case AUTHENTICATION_TYPE_SHARED:
        bss->protocol = UAP_PROTO_WEP_STATIC;
        bss->auth_mode = UAP_AUTH_MODE_SHARED;
        break;
    case AUTHENTICATION_TYPE_WPA:
        bss->protocol = UAP_PROTO_WPA;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    case AUTHENTICATION_TYPE_WPA2:
        bss->protocol = UAP_PROTO_WPA2;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    case AUTHENTICATION_TYPE_WPA2PSK:
        bss->protocol = UAP_PROTO_WPA2;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    case AUTHENTICATION_TYPE_WPA_MIXED:
        bss->protocol = UAP_PROTO_MIXED;
        bss->auth_mode = UAP_AUTH_MODE_OPEN;
        break;
    default:
        if (pCred->auth_type == AUTHENTICATION_TYPE_WPA_MIXED &&
            load_by_oob == WPS_SET) {
            bss->protocol = UAP_PROTO_MIXED;
            bss->auth_mode = UAP_AUTH_MODE_OPEN;
        } else {
            wps_printf(MSG_ERROR, " Unsupported AUTH type: 0x%x\n",
                       pCred->auth_type);
            ret = WPS_STATUS_FAIL;
        }
        break;
    }
    if (pCred->encry_type == ENCRYPTION_TYPE_WEP) {

        bss->protocol = UAP_PROTO_WEP_STATIC;
        bss->wep_cfg[pCred->network_key_index - 1].is_default = WPS_SET;

        if (pCred->network_key_len == 10 || pCred->network_key_len == 5) {
            bss->wep_cfg[pCred->network_key_index - 1].length = 5;
        } else if (pCred->network_key_len == 13 || pCred->network_key_len == 26) {
            bss->wep_cfg[pCred->network_key_index - 1].length = 13;
        }

        bss->wep_cfg[pCred->network_key_index - 1].key_index =
            pCred->network_key_index - 1;

        if (pCred->network_key_len == 26 || pCred->network_key_len == 10) {
            memset(network_key, 0, MAX_WEP_KEY_LEN);
            string2raw((char *) pCred->network_key, network_key);
        } else {
            memcpy(network_key, pCred->network_key, pCred->network_key_len);
        }

        memcpy(bss->wep_cfg[pCred->network_key_index - 1].key, network_key,
               pCred->network_key_len);
    } else {
        if (bss->protocol == UAP_PROTO_WPA) {
            bss->wpa_cfg.group_cipher = UAP_CIPH_TKIP;
            bss->wpa_cfg.pairwise_cipher_wpa = UAP_CIPH_TKIP;
        } else if (bss->protocol == UAP_PROTO_WPA2) {
            bss->wpa_cfg.group_cipher = UAP_CIPH_AES;
            bss->wpa_cfg.pairwise_cipher_wpa2 = UAP_CIPH_AES;
        } else if (bss->protocol == UAP_PROTO_MIXED) {
            bss->wpa_cfg.group_cipher = UAP_CIPH_TKIP;
            bss->wpa_cfg.pairwise_cipher_wpa2 = UAP_CIPH_AES;
            bss->wpa_cfg.pairwise_cipher_wpa = UAP_CIPH_TKIP;
        }

        /* Copy Network Key */
        bss->wpa_cfg.length = pCred->network_key_len;
        memcpy(bss->wpa_cfg.passphrase, pCred->network_key,
               pCred->network_key_len);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Copy Credential data into Enrollee/Registrar credential structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param pwps_info A pointer to the global PWPS_INFO structure
 *
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_load_wsta_registrar_cred(WPS_DATA * wps_s, PWPS_INFO pwps_info)
{
    MESSAGE_ENROLLEE_REGISTRAR *enr_reg = NULL;
    bss_config_t bss;
    int ret = WPS_STATUS_SUCCESS;
    CREDENTIAL_DATA *pCred = NULL;

    ENTER();
    enr_reg = &pwps_info->registrar;
    pCred = &enr_reg->cred_data[0];
    memset(&bss, 0, sizeof(bss_config_t));

    wps_cred_to_bss_config(&bss, pCred, pwps_info->config_load_by_oob);

    /* set credentials */
    ret = load_cred_info(wps_s, pwps_info, &bss);
    LEAVE();

    return ret;
}

/** 
 *  @brief Generate the PSK from passphrase and ssid generated.
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *
 *  @return   None 
 */
void
wlan_wfd_generate_psk(PWPS_INFO pwps_info)
{
    int i, len = 0;
    u8 temp[32];
    CREDENTIAL_DATA *reg_cred = NULL;

    ENTER();
    reg_cred = &pwps_info->registrar.cred_data[0];

    /* 
     * For WFD, we need to convert the passphrase to PSK.
     */
    pbkdf2_sha1((char *) reg_cred->network_key, (char *) reg_cred->ssid,
                reg_cred->ssid_length, 4096, temp, 32);
    for (i = 0; i < 32; ++i)
        len += minSprintf((char *) reg_cred->network_key + len, "%02x",
                       (temp[i] & 0xff));
    reg_cred->network_key_len = 64;

    wps_hexdump(DEBUG_WFD_DISCOVERY, "NETWORK KEY(PSK)",
                (u8 *) pwps_info->registrar.cred_data[0].network_key, 64);

    LEAVE();
}


/** 
 *  @brief Creates a get bss_config request and sends to the driver
 *
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param bss       A pointer to the bss_config_t structure
 * 
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
apcmd_get_bss_config(WPS_DATA * wps_s, bss_config_t * bss)
{
    apcmdbuf_bss_configure *cmd_buf = NULL;
    bss_config_t *bss_temp = NULL;
    u8 *buf = NULL;
    u16 cmd_len;
    u16 buf_len;
    int ret = WPS_STATUS_SUCCESS;
    struct iwreq iwr;

    ENTER();

    /* Alloc buf for command */
    buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;
    buf = (u8 *) os_malloc(buf_len);
    if (!buf) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate buffer from command!");
        return WPS_STATUS_FAIL;
    }
    memset((char *) buf, 0, buf_len);
    memset(bss, 0, sizeof(bss_config_t));

    /* Locate headers */
    cmd_len = sizeof(apcmdbuf_bss_configure);
    cmd_buf = (apcmdbuf_bss_configure *) buf;
    bss_temp = (bss_config_t *) (buf + cmd_len);
    ASSERT(sizeof(bss_config_t) + cmd_len <= buf_len);

    /* Fill the command buffer */
    cmd_buf->Action = ACTION_GET;

    /* Initialize the ifr structure */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_s->ifname, strlen(wps_s->ifname));
    iwr.u.data.pointer = cmd_buf;

    /* Send the command */
    if (WlanIoctl(UAP_BSS_CONFIG, &iwr) != 0) {
        wps_printf(MSG_ERROR, "ERR:UAP_BSS_CONFIG get is not supported by %s\n",
                   wps_s->ifname);
        ret = WPS_STATUS_FAIL;
    } else {
        memcpy(bss, bss_temp, sizeof(bss_config_t));
    }

    os_free(buf);
    LEAVE();
    return ret;
}

/** 
 *  @brief Creates a set bss_config request and sends to the driver
 *
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param bss       A pointer to the bss_config_t structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
apcmd_set_bss_config(WPS_DATA * wps_s, bss_config_t * bss)
{
    apcmdbuf_bss_configure *cmd_buf = NULL;
    bss_config_t *bss_temp = NULL;
    u8 *buf = NULL;
    u16 cmd_len;
    u16 buf_len;
    int ret = WPS_STATUS_SUCCESS;
    struct iwreq iwr;

    ENTER();

    /* Alloc buf for command */
    buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;
    buf = (u8 *) os_malloc(buf_len);
    if (!buf) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate buffer from command!");
        return WPS_STATUS_FAIL;
    }
    memset((char *) buf, 0, buf_len);

    /* Locate headers */
    cmd_len = sizeof(apcmdbuf_bss_configure);
    cmd_buf = (apcmdbuf_bss_configure *) buf;
    bss_temp = (bss_config_t *) (buf + cmd_len);
    memcpy(bss_temp, bss, sizeof(bss_config_t));
    ASSERT(sizeof(bss_config_t) + cmd_len <= buf_len);

    /* Fill the command buffer */
    cmd_buf->Action = ACTION_SET;

    /* Initialize the ifr structure */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_s->ifname, strlen(wps_s->ifname));
    iwr.u.data.pointer = cmd_buf;

    /* Send the command */
    if (WlanIoctl(UAP_BSS_CONFIG, &iwr) != 0) {
        wps_printf(MSG_ERROR, "ERR:UAP_BSS_CONFIG set is not supported by %s",
                   dev_name);
        ret = WPS_STATUS_FAIL;
    }

    os_free(buf);
    LEAVE();
    return ret;
}

#define UAP_BSS_CTRL                (SIOCDEVPRIVATE + 4)
/** 
 *  @brief Starts the existing AP BSS .
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
apcmd_start_bss(WPS_DATA * wps_s)
{
#if 0
    u32 data = 0;
    struct ifreq ifr;

    ENTER();

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, (char *) dev_name, IFNAMSIZ);
    ifr.ifr_data = (void *) &data;
    /* Send the command */
    if (ioctl(ioctl_sock, UAP_BSS_CTRL, &ifr)) {
        perror("");
        wps_printf(MSG_ERROR, "ERR:UAP_BSS_CTRL is not supported by %s",
                   dev_name);
        return WPS_STATUS_FAIL;
    }

    LEAVE();
    return WPS_STATUS_SUCCESS;
#else
    struct iwreq iwr;
    u32 data = 0;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_s->ifname, IFNAMSIZ);
    iwr.u.mode = (u32) &data;
    /* Send the command */
    if (WlanIoctl(UAP_BSS_CTRL, &iwr)) {
        wps_printf(MSG_WARNING, "ERR:UAP_BSS_CTRL is not supported by %s\n",
                   dev_name);
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return (ret);
#endif
}

/** 
 *  @brief Stops the existing AP BSS .
 *
 *  @param wps_s      A pointer to the WPS_DATA structure
 *
 *  @return     WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
apcmd_stop_bss(WPS_DATA * wps_s)
{
#if 0
    u32 data = 1;
    struct ifreq ifr;

    ENTER();

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, (char *) dev_name, IFNAMSIZ);
    ifr.ifr_data = (void *) &data;
    /* Send the command */
    if (ioctl(ioctl_sock, UAP_BSS_CTRL, &ifr)) {
        wps_printf(MSG_ERROR, "ERR:UAP_BSS_CTRL is not supported by %s",
                   dev_name);
        return WPS_STATUS_FAIL;
    }

    LEAVE();
    return WPS_STATUS_SUCCESS;
#else
    struct iwreq iwr;
    u32 data = 1;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_s->ifname, IFNAMSIZ);
    iwr.u.mode = (u32) &data;
    /* Send the command */
    if (WlanIoctl(UAP_BSS_CTRL, &iwr)) {
        wps_printf(MSG_WARNING, "ERR:UAP_BSS_CTRL is not supported by %s\n",
                   dev_name);
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return (ret);
#endif	
}
#endif

/** 
 *  @brief Send DEAUTH to associated STA .
 *
 *  @param      None
 *
 *  @return     WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
apcmd_station_deauth(WPS_DATA * wps_s, u8 * sta_mac)
{
    deauth_param param;
    //struct ifreq ifr;
    struct iwreq iwr;
    u8 wps_broadcast_mac[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    ENTER();
    memset(&param, 0, sizeof(deauth_param));

    if (memcmp(sta_mac, wps_broadcast_mac, ETH_ALEN) == 0) {
        wps_printf(MSG_ERROR, "ERR: Broadcast address specified for AP deauth");
        return WPS_STATUS_FAIL;
    } else if (sta_mac[0] & 0x01) {
        wps_printf(MSG_ERROR, "ERR: Multicast address specified for AP deauth");
        return WPS_STATUS_FAIL;
    }

    param.reason_code = DEAUTH_REASON_PREV_AUTH_INVALID;
    memcpy(param.mac_addr, sta_mac, ETH_ALEN);
#if 0
    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, (char *) dev_name, IFNAMSIZ - 1);
    ifr.ifr_ifru.ifru_data = (void *) &param;

    /* Perform ioctl */
    errno = 0;
    if (ioctl(ioctl_sock, UAP_BSS_STA_DEAUTH, &ifr)) {
        wps_printf(DEBUG_WLAN, "ERR:Sta Deauthentication failed " MACSTR,
                   MAC2STR(sta_mac));
    }
#else
    /* Initialize the iwr structure */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_s->ifname, IFNAMSIZ);
    iwr.u.mode = (u32) &param;

    /* Perform ioctl */
    if (WlanIoctl(UAP_BSS_STA_DEAUTH, &iwr)) {
        wps_printf(DEBUG_WLAN, "ERR:Sta Deauthentication failed " MACSTR,
                   MAC2STR(sta_mac));
    }
#endif

    wps_printf(DEBUG_WLAN, "Station deauth successful");

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief  Get interface flags from WLAN driver (SIOCGIFFLAGS)
 *
 *  @param flags      Pointer to returned flags value
 *  @param ifname     Interface name 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_get_ifflags(int *flags, char *ifname)
{
    return WPS_STATUS_SUCCESS;
#if 0
    struct ifreq ifr;

    ENTER();

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, (char *) ifname, IFNAMSIZ);
    if (ioctl(ioctl_sock, SIOCGIFFLAGS, (caddr_t) & ifr) < 0) {
        perror("ioctl[SIOCGIFFLAGS]");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    *flags = ifr.ifr_flags & 0xffff;

    LEAVE();
    return WPS_STATUS_SUCCESS;
#endif
}

/** 
 *  @brief  Set interface flags to WLAN driver (SIOCSIFFLAGS)
 *
 *  @param flags      New value for flags
 *  @param ifname     Interface name 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_ifflags(int flags, char *ifname)
{
#if 0
    struct ifreq ifr;

    ENTER();

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, (char *) ifname, IFNAMSIZ);
    ifr.ifr_flags = flags & 0xffff;
    if (ioctl(ioctl_sock, SIOCSIFFLAGS, (caddr_t) & ifr) < 0) {
        perror("SIOCSIFFLAGS");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    LEAVE();
#endif	
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief UP interface either for bridge or wifi interface 
 *
 *  @param wps_s      Pointer to WPS_DATA structure 
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_update_interface_flags(WPS_DATA * wps_s)
{
#if 0
    int flags;
    ENTER();
    /* 
     * uAP Enrollee : Keep inteface UP to receive the EAPOL.
     */
    /* Bridging is not enabled, UP wifi interface */
    wps_printf(DEBUG_INIT, "Setting interface '%s' UP\n", wps_s->ifname);
    if (wps_wlan_get_ifflags(&flags, wps_s->ifname) != 0 ||
        wps_wlan_set_ifflags(flags | IFF_UP, wps_s->ifname) != 0) {
        wps_printf(MSG_ERROR, "Could not set interface '%s' UP\n",
                   wps_s->ifname);
        return WPS_STATUS_FAIL;
    }
#endif
    LEAVE();
    return WPS_STATUS_SUCCESS;

}

/** 
 *  @brief  Set private ioctl command "deauth" to WLAN driver
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_deauth(void)
{
    //struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;
    //int ioctl_val, subioctl_val;

    ENTER();
#if 1
    WlanShimCDeauth();
#else
    if (get_priv_ioctl("deauth", &ioctl_val, &subioctl_val) == WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "ERROR : get_priv_ioctl FAIL !");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    wps_printf(DEBUG_WLAN, "ioctl_val %x, subioctl_val %d", ioctl_val,
               subioctl_val);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    iwr.u.data.pointer = NULL;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(ioctl_sock, ioctl_val, &iwr) < 0) {
        perror("ioctl[deauth]");
        ret = WPS_STATUS_FAIL;
    }
#endif
    LEAVE();
    return ret;
}

/** 
 *  @brief  Set private ioctl command "deauth" to WLAN driver
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_disassoc(void)
{
    //struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;
    //int ioctl_val, subioctl_val;

    ENTER();
#if 1
// Call to disassociate????
#else
    if (get_priv_ioctl("disassoc",
                       &ioctl_val, &subioctl_val) == WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "ERROR : get_priv_ioctl FAIL !");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    wps_printf(DEBUG_WLAN, "ioctl_val %x, subioctl_val %d", ioctl_val,
               subioctl_val);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    iwr.u.data.pointer = NULL;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(ioctl_sock, ioctl_val, &iwr) < 0) {
        perror("ioctl[deauth]");
        ret = WPS_STATUS_FAIL;
    }
#endif
    LEAVE();
    return ret;
}

/** 
 *  @brief  Wlan event parser for FW events
 *  @param context    Pointer to Context 
 *  @param wps_s      Pointer to WPS global structure
 *  @param evt_buffer Pointer to Event buffer
 *  @param evt_len    Event Length
 *
 *  @return           None 
 */
void
wps_wlan_event_parser(void *context, WPS_DATA * wps_s,
                      char *evt_buffer, int evt_len)
{
    event_header *event = NULL;
    PWPS_INFO pwps_info;

    ENTER();
    pwps_info = (PWPS_INFO) context;

    /* event parser */
    event = (event_header *) evt_buffer;

    switch (event->event_id) {
    case EV_ID_WFD_GENERIC:
        wfd_process_generic_event(event->event_data, evt_len
                                  - sizeof(event->event_id));
        break;

    default:
        break;
    }
    LEAVE();
}

/** 
 *  @brief  Send Custom_IE command to driver.
 *  @param buf Pointer to data buffer
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wlan_custom_ie_config(u8 * buf)
{
    struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    memset(&iwr, 0, sizeof(iwr));
    //strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);
    strncpy(iwr.ifr_name, "mlan0", IFNAMSIZ);

    iwr.u.data.pointer = (void *) buf;
    iwr.u.data.flags = 0;

    if (WlanIoctl(WLAN_CUSTOM_IE, &iwr)) {
        wps_printf(MSG_WARNING, "Failed to set/get/clear the IE buffer\n");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief  Send HostCmd command to driver for WFD.
 *  @param buf Pointer to data buffer
 *
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_wlan_hostcmd(u8 * buf)
{
#if 0
    struct ifreq ifr;
    int ret = WPS_STATUS_SUCCESS;

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, (char *) dev_name, IFNAMSIZ);
    ifr.ifr_ifru.ifru_data = (void *) buf;

    if (ioctl(ioctl_sock, WFDHOSTCMD, &ifr)) {
        perror("");
#else
    struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, "wfd0", IFNAMSIZ);

    iwr.u.mode = (u32) buf;

    /** Host Command ioctl number */
    #define WFDHOSTCMD          (SIOCDEVPRIVATE + 1)

    if (WlanIoctl(WFDHOSTCMD, &iwr)) {
#endif
        wps_printf(MSG_WARNING, "ERR:WFDHOSTCMD error.\n");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief  Update SET/CLEAR the group formation bit for GO.
 *  @param  set_clear  Set or clear the bit. 
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_update_group_formation_config(int set_clear)
{
    int i, ret = WPS_STATUS_SUCCESS;
    u8 *buf;
    short ie_index = 0;
    u16 len = 0, ie_len_wfd_org, ie_len_wfd, ie_len_wps;
    u8 *wfd_ptr;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;
    ENTER();

    if (0 == (buf = (u8*) os_malloc(MRVDRV_SIZE_OF_CMD_BUFFER)))
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("wps_wlan_update_group_formation_config: out of memory\n"));
        wps_printf(MSG_WARNING, "wps_wlan_update_group_formation_config: out of memory\n");
        ret = WPS_STATUS_FAIL;
    }
    else
    {
        memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        ret = wfd_ie_config((u16 *) & ie_index, 0, 0, buf);

        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("wps_wlan_update_group_formation_config: wfd_ie_config ret = 0x%X ie_index 0x%X\n",
                ret, ie_index));

    	if (ret == WPS_STATUS_SUCCESS && ie_index < (MAX_MGMT_IE_INDEX - 1)) {
            tlv = (tlvbuf_custom_ie *) buf;
    
            wps_hexdump(DEBUG_WLAN, "IE Buffer", (unsigned char *) buf,
                        tlv->length);
    
            if (tlv->tag == MRVL_MGMT_IE_LIST_TLV_ID) {
                ie_ptr = (custom_ie *) (tlv->ie_data);
                /* Goto appropriate Ie Index */
                for (i = 0; i < ie_index; i++) {
                    ie_ptr = (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                            ie_ptr->ie_length);
                }
                ie_len_wfd_org = ie_len_wfd = ie_ptr->ie_length;
                wfd_ptr = ie_ptr->ie_buffer;
                while (ie_len_wfd > WFD_IE_HEADER_LEN) {
                    memcpy(&len, wfd_ptr + 1, sizeof(u16));
                    len = wlan_le16_to_cpu(len);
                    /* check capability type */
                    if (*wfd_ptr == TLV_TYPE_WFD_CAPABILITY) {
                        tlvbuf_wfd_capability *wfd_tlv =
                            (tlvbuf_wfd_capability *) wfd_ptr;
                        /* set or clear the bit correctly */
                        if (set_clear)
                            wfd_tlv->group_capability |=
                                WFD_GROUP_FORMATION_OR_MASK;
                        else
                            wfd_tlv->group_capability &=
                                WFD_GROUP_FORMATION_AND_MASK;
                        break;
                    }
                    wfd_ptr += len + WFD_IE_HEADER_LEN;
                    ie_len_wfd -= len + WFD_IE_HEADER_LEN;
                }
    
                /* WPS length */
                ie_ptr =
                    (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                   ie_len_wfd_org);
                ie_len_wps = ie_ptr->ie_length;

                /* Update New IE now */
                ret =
                	wfd_ie_config((u16 *) & ie_index, ie_len_wfd_org, ie_len_wps,
                              	buf);
                if (ret != WPS_STATUS_SUCCESS) {
                	wps_printf(MSG_ERROR,
                           	"Failed to update group formation bit.\n");
                }
            }
        }
		os_free(buf);
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief Performs the ioctl operation to set mac address to kernel
 *
 *  @param  mac mac address to set
 *  @return WPS_STATUS_SUCCESS or WPS_STATUS_FAIL
 */
int
wlan_set_intended_mac_addr(u8 mac[ETH_ALEN])
{
#if 0 //REVISIT TODO
    struct ifreq ifr;

    ENTER();

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, (char *) dev_name, IFNAMSIZ);
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    memcpy(ifr.ifr_hwaddr.sa_data, mac, ETH_ALEN);

    /* Send the command */
    if (ioctl(ioctl_sock, SIOCSIFHWADDR, &ifr)) {
        printf("ERR:Set HW address.\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    LEAVE();
    wps_printf(DEBUG_WLAN, "HW address setting successful\n");
#endif
    return WPS_STATUS_SUCCESS;
}

/*
 * Bit 0 : Assoc Req
 * Bit 1 : Assoc Resp
 * Bit 2 : ReAssoc Req
 * Bit 3 : ReAssoc Resp
 * Bit 4 : Probe Req
 * Bit 5 : Probe Resp
 * Bit 8 : Beacon 
 */
/** Mask for Assoc request frame */
#define MGMT_MASK_ASSOC_REQ  0x01
/** Mask for ReAssoc request frame */
#define MGMT_MASK_REASSOC_REQ  0x04
/** Mask for Assoc response frame */
#define MGMT_MASK_ASSOC_RESP  0x02
/** Mask for ReAssoc response frame */
#define MGMT_MASK_REASSOC_RESP  0x08
/** Mask for probe request frame */
#define MGMT_MASK_PROBE_REQ  0x10
/** Mask for probe response frame */
#define MGMT_MASK_PROBE_RESP 0x20
/** Mask for beacon frame */
#define MGMT_MASK_BEACON 0x100
/** Mask to clear previous settings */
#define MGMT_MASK_CLEAR 0x000

/** 
 *  @brief Process and send ie config command 
 *  @param flag   FLAG: Set/clear WPS IE
 *  @param ie_index  A pointer to the IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_ie_config(int flag, short *pie_index)
{
    u8 *buf, *pos;
    IEEEtypes_Header_t *ptlv_header = NULL;
    struct iwreq iwr;
    int i, ret = WPS_STATUS_SUCCESS;
    u16 ie_len = 0, mgmt_subtype_mask = 0;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL, *ie_ptr2 = NULL;
    u8 action = 0x00, type = 0x00;

    ENTER();

    type = flag & 0x0f;
    action = flag & 0xf0;

    buf = (u8 *) os_malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
	if (buf == NULL)
		goto _exit_;
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    tlv = (tlvbuf_custom_ie *) buf;
    tlv->tag = MRVL_MGMT_IE_LIST_TLV_ID;

    /* Locate headers */
    ie_ptr = (custom_ie *) (tlv->ie_data);
    /* Set TLV fields */

    if (action == SET_WPS_IE) {
        /* Set WPS IE */
        pos = ie_ptr->ie_buffer;
        ptlv_header = (IEEEtypes_Header_t *) pos;
        pos += sizeof(IEEEtypes_Header_t);

        if (type == WPS_AP_SESSION_INACTIVE) {
            ie_len = wps_ap_beacon_prepare(WPS_END_REG_DISCOVERY_PHASE, pos);
            mgmt_subtype_mask = MGMT_MASK_BEACON;
        } else if (type == WPS_AP_SESSION_ACTIVE) {
            ie_len = wps_ap_beacon_prepare(WPS_START_REG_DISCOVERY_PHASE, pos);
            mgmt_subtype_mask = MGMT_MASK_BEACON;
        } else if (type == WPS_STA_SESSION_ACTIVE) {
            ie_len = wps_sta_assoc_request_prepare(pos);
            mgmt_subtype_mask = MGMT_MASK_ASSOC_REQ | MGMT_MASK_REASSOC_REQ;
        } else if (type == WPS_AP_SESSION_ACTIVE_AR) {
            ie_len =
                wps_ap_assoc_response_prepare(WPS_START_REG_DISCOVERY_PHASE,
                                              pos);
            mgmt_subtype_mask = MGMT_MASK_ASSOC_RESP | MGMT_MASK_REASSOC_RESP;
        } else if (type == WPS_AP_SESSION_INACTIVE_AR) {
            ie_len =
                wps_ap_assoc_response_prepare(WPS_END_REG_DISCOVERY_PHASE, pos);
            mgmt_subtype_mask = MGMT_MASK_ASSOC_RESP | MGMT_MASK_REASSOC_RESP;
        } else {
            wps_printf(MSG_ERROR, "ie_config: unknown SET type!\n");
            ret = WPS_STATUS_FAIL;
            goto _exit_;
        }

        ptlv_header->Type = WPS_IE;
        ptlv_header->Len = ie_len;
        ie_ptr->mgmt_subtype_mask = mgmt_subtype_mask;
        tlv->length = sizeof(custom_ie) + sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_length = sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_index = *pie_index;

        if (type == WPS_AP_SESSION_INACTIVE || type == WPS_AP_SESSION_ACTIVE) {
            ie_ptr = (custom_ie *) (((u8 *) ie_ptr) + sizeof(custom_ie) +
                                    sizeof(IEEEtypes_Header_t) + ie_len);
            pos = ie_ptr->ie_buffer;
            ptlv_header = (IEEEtypes_Header_t *) pos;
            pos += sizeof(IEEEtypes_Header_t);

            if (type == WPS_AP_SESSION_INACTIVE) {
                ie_len =
                    wps_ap_probe_response_prepare(WPS_END_REG_DISCOVERY_PHASE,
                                                  pos);
                mgmt_subtype_mask = MGMT_MASK_PROBE_RESP;
            } else if (type == WPS_AP_SESSION_ACTIVE) {
                ie_len =
                    wps_ap_probe_response_prepare(WPS_START_REG_DISCOVERY_PHASE,
                                                  pos);
                mgmt_subtype_mask = MGMT_MASK_PROBE_RESP;
            }

            ptlv_header->Type = WPS_IE;
            ptlv_header->Len = ie_len;
            ie_ptr->mgmt_subtype_mask = mgmt_subtype_mask;
            tlv->length +=
                sizeof(custom_ie) + sizeof(IEEEtypes_Header_t) + ie_len;
            ie_ptr->ie_length = sizeof(IEEEtypes_Header_t) + ie_len;
            ie_ptr->ie_index = *pie_index + 1;
        }

    } else if (action == CLEAR_WPS_IE) {
        if (*pie_index < 0) {
            wps_printf(DEBUG_WLAN, "Clear IE number should be valid\n");
            ret = WPS_STATUS_FAIL;
            goto _exit_;
        }
        /* Clear WPS IE */
        pos = ie_ptr->ie_buffer;
        ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
        ie_ptr->ie_length = 0;
        ie_ptr->ie_index = *pie_index;

        if ((type == CLEAR_WPS_IE) && (*pie_index < (MAX_MGMT_IE_INDEX - 1))) {
            ie_ptr = (custom_ie *) (((u8 *) ie_ptr) + sizeof(custom_ie));
            ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
            ie_ptr->ie_length = 0;
            ie_ptr->ie_index = *pie_index + 1;
            tlv->length = 2 * sizeof(custom_ie);
        } else
            tlv->length = sizeof(custom_ie);

        /* reinit IE index */
        *pie_index = -1;

    } else {
        /* Get WPS IE */
        tlv->length = 0;
    }

    wps_hexdump(DEBUG_WLAN, "MGMT_IE", (const unsigned char *) buf,
                tlv->length + TLV_HEADER_SIZE);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_global.ifname, IFNAMSIZ);
    iwr.u.data.pointer = (void *) buf;
    iwr.u.data.length = tlv->length + TLV_HEADER_SIZE;
    iwr.u.data.flags = 0;

    if (WlanIoctl(WLAN_CUSTOM_IE, &iwr)) {
        wps_printf(MSG_ERROR, "Failed to set/get/clear the IE buffer\n");
        ret = WPS_STATUS_FAIL;
        goto _exit_;
    }

    if (action == GET_WPS_IE) {
        /* Get the free IE buffer index number */
        tlv = (tlvbuf_custom_ie *) buf;
        *pie_index = -1;
        ie_ptr = (custom_ie *) (tlv->ie_data);
        for (i = 0; i < MAX_MGMT_IE_INDEX; i++) {
            if (ie_ptr->mgmt_subtype_mask == 0) {
                /* check if next one is also available */
                if (i < (MAX_MGMT_IE_INDEX - 1)) {
                    ie_ptr2 =
                        (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                       ie_ptr->ie_length);
                    if (ie_ptr2->mgmt_subtype_mask == 0) {
                        *pie_index = ie_ptr->ie_index;
                        break;
                    }
                }
            }
            if (i < (MAX_MGMT_IE_INDEX - 1))
                ie_ptr =
                    (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                   ie_ptr->ie_length);
        }
        if (*pie_index == -1 || *pie_index > (MAX_MGMT_IE_INDEX - 2)) {
            /* Two buffers are needed */
            wps_printf(MSG_ERROR, "No free IE buffer available\n");
            ret = WPS_STATUS_FAIL;
        }
    }
  _exit_:
    if (buf)
        os_free(buf);

    LEAVE();
    return ret;
}

/** 
 *  @brief Process and send ie config command for (re)assoc request. 
 *  @param flag   FLAG: Set/clear WPS IE
 *  @param index  A pointer to the IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_assoc_request_ie_config(int flag, short *pie_index)
{
    u8 *buf, *pos;
    IEEEtypes_Header_t *ptlv_header = NULL;
    struct iwreq iwr;
    int i, ret = WPS_STATUS_SUCCESS;
    u16 ie_len = 0, mgmt_subtype_mask = 0;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;
    u8 action = 0x00, type = 0x00;

    ENTER();

    type = flag & 0x0f;
    action = flag & 0xf0;

    buf = (u8 *) os_malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
	if (buf == NULL)
		goto _exitp_;
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    tlv = (tlvbuf_custom_ie *) buf;
    tlv->tag = MRVL_MGMT_IE_LIST_TLV_ID;

    /* Locate headers */
    ie_ptr = (custom_ie *) (tlv->ie_data);
    /* Set TLV fields */

    if (action == SET_WPS_IE) {
        /* Set WPS IE */
        pos = ie_ptr->ie_buffer;
        ptlv_header = (IEEEtypes_Header_t *) pos;
        pos += sizeof(IEEEtypes_Header_t);
        ie_len = wps_sta_assoc_request_prepare(pos);
        mgmt_subtype_mask = MGMT_MASK_ASSOC_REQ | MGMT_MASK_REASSOC_REQ;

        ptlv_header->Type = WPS_IE;
        ptlv_header->Len = ie_len;
        ie_ptr->mgmt_subtype_mask = mgmt_subtype_mask;
        tlv->length += sizeof(custom_ie) + sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_length = sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_index = *pie_index;

    } else if (action == CLEAR_WPS_IE) {
        /* Clear WPS IE */
        if (*pie_index < 0) {
            wps_printf(DEBUG_WLAN, "Clear IE number should be valid\n");
            return WPS_STATUS_FAIL;
        }
        pos = ie_ptr->ie_buffer;
        ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
        ie_ptr->ie_length = 0;
        ie_ptr->ie_index = *pie_index;  /* index */
        tlv->length = sizeof(custom_ie);

        /* reinit IE index */
        *pie_index = -1;

    } else {
        /* Get WPS IE */
        tlv->length = 0;
    }

    wps_hexdump(DEBUG_WLAN, "MGMT_IE", (const unsigned char *) buf,
                tlv->length + TLV_HEADER_SIZE);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_global.ifname, IFNAMSIZ);
    iwr.u.data.pointer = (void *) buf;
    iwr.u.data.length = tlv->length + TLV_HEADER_SIZE;
    iwr.u.data.flags = 0;

    if (WlanIoctl(WLAN_CUSTOM_IE, &iwr)) {
        wps_printf(MSG_ERROR, "Failed to set/get/clear the IE buffer\n");
        ret = WPS_STATUS_FAIL;
        goto _exitp_;
    }

    if (flag == GET_WPS_IE) {
        /* Get the free IE buffer index number */
        tlv = (tlvbuf_custom_ie *) buf;
        *pie_index = -1;
        ie_ptr = (custom_ie *) (tlv->ie_data);
        for (i = 0; i < MAX_MGMT_IE_INDEX; i++) {
            if (ie_ptr->mgmt_subtype_mask == 0) {
                *pie_index = ie_ptr->ie_index;
                break;
            }
            if (i < (MAX_MGMT_IE_INDEX - 1))
                ie_ptr =
                    (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                   ie_ptr->ie_length);
        }
        if (*pie_index == -1 || *pie_index > (MAX_MGMT_IE_INDEX - 1)) {
            wps_printf(MSG_ERROR, "No free IE buffer available\n");
            ret = WPS_STATUS_FAIL;
        }
    }
  _exitp_:

    if (buf)
        os_free(buf);

    LEAVE();
    return ret;
}

/** 
 *  @brief Process and send ie config command for probe request. 
 *  @param flag   FLAG: Set/clear WPS IE
 *  @param index  A pointer to the IE buffer index
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_probe_request_ie_config(int flag, short *pie_index)
{
    u8 *buf, *pos;
    IEEEtypes_Header_t *ptlv_header = NULL;
    struct iwreq iwr;
    int i, ret = WPS_STATUS_SUCCESS;
    u16 ie_len = 0, mgmt_subtype_mask = 0;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;
    u8 action = 0x00, type = 0x00;

    ENTER();

    type = flag & 0x0f;
    action = flag & 0xf0;

    buf = (u8 *) os_malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
	if (buf == NULL)
		goto _exitp_;
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    tlv = (tlvbuf_custom_ie *) buf;
    tlv->tag = MRVL_MGMT_IE_LIST_TLV_ID;

    /* Locate headers */
    ie_ptr = (custom_ie *) (tlv->ie_data);
    /* Set TLV fields */

    if (action == SET_WPS_IE) {
        /* Set WPS IE */
        pos = ie_ptr->ie_buffer;
        ptlv_header = (IEEEtypes_Header_t *) pos;
        pos += sizeof(IEEEtypes_Header_t);
        ie_len = wps_sta_probe_request_prepare(pos);
        mgmt_subtype_mask = MGMT_MASK_PROBE_REQ;
        ptlv_header->Type = WPS_IE;
        ptlv_header->Len = ie_len;
        ie_ptr->mgmt_subtype_mask = mgmt_subtype_mask;
        tlv->length = sizeof(custom_ie) + sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_length = sizeof(IEEEtypes_Header_t) + ie_len;
        ie_ptr->ie_index = *pie_index;

        ie_ptr = (custom_ie *) (((u8 *) ie_ptr) + sizeof(custom_ie) +
                                sizeof(IEEEtypes_Header_t) + ie_len);
        pos = ie_ptr->ie_buffer;
        ptlv_header = (IEEEtypes_Header_t *) pos;
        pos += sizeof(IEEEtypes_Header_t);

    } else if (action == CLEAR_WPS_IE) {
        if (*pie_index < 0) {
            wps_printf(DEBUG_WLAN, "Clear IE number should be valid\n");
            return WPS_STATUS_FAIL;
        }
        /* Clear WPS IE */
        pos = ie_ptr->ie_buffer;
        ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
        ie_ptr->ie_length = 0;
        ie_ptr->ie_index = *pie_index;  /* index */
        tlv->length = sizeof(custom_ie);

        /* reinit IE index */
        *pie_index = -1;

    } else {
        /* Get WPS IE */
        tlv->length = 0;
    }

    wps_hexdump(DEBUG_WLAN, "MGMT_IE", (const unsigned char *) buf,
                tlv->length + TLV_HEADER_SIZE);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, wps_global.ifname, IFNAMSIZ);
    iwr.u.data.pointer = (void *) buf;
    iwr.u.data.length = tlv->length + TLV_HEADER_SIZE;
    iwr.u.data.flags = 0;

    if (WlanIoctl(WLAN_CUSTOM_IE, &iwr)) {
        wps_printf(MSG_ERROR, "Failed to set/get/clear the IE buffer\n");
        ret = WPS_STATUS_FAIL;
        goto _exitp_;
    }

    if (flag == GET_WPS_IE) {
        /* Get the free IE buffer index number */
        tlv = (tlvbuf_custom_ie *) buf;
        *pie_index = -1;
        ie_ptr = (custom_ie *) (tlv->ie_data);
        for (i = 0; i < MAX_MGMT_IE_INDEX; i++) {
            if (ie_ptr->mgmt_subtype_mask == 0) {
                *pie_index = ie_ptr->ie_index;
                break;
            }
            if (i < (MAX_MGMT_IE_INDEX - 1))
                ie_ptr =
                    (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                   ie_ptr->ie_length);
        }
        if (*pie_index == -1 || *pie_index > (MAX_MGMT_IE_INDEX - 1)) {
            wps_printf(MSG_ERROR, "No free IE buffer available\n");
            ret = WPS_STATUS_FAIL;
        }
    }
  _exitp_:

    if (buf)
        os_free(buf);

    LEAVE();
    return ret;
}

/*
 *  @brief  Set private ioctl command "wpssession" to WLAN driver
 *
 *  @param enable     0 - WPS Session Disable, 1 - WPS Session Enable
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
#if 0
int
wps_wlan_session_control(int enable)
{
    struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;
    u8 buf[4];
    int ioctl_val, subioctl_val;

    ENTER();

    if (get_priv_ioctl("wpssession",
                       &ioctl_val, &subioctl_val) == WPS_STATUS_FAIL) {
        wps_printf(MSG_ERROR, "ERROR : get_priv_ioctl FAIL !");
        LEAVE();
        return WPS_STATUS_FAIL;
    }
    wps_printf(DEBUG_WLAN, "ioctl_val %x, subioctl_val %d", ioctl_val,
               subioctl_val);

    memset(&iwr, 0, sizeof(iwr));
    memset(buf, 0, sizeof(buf));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    if (enable == WPS_SESSION_ON || enable == WPS_SESSION_OFF)
        buf[0] = enable;
    else
        return WPS_STATUS_FAIL;

    iwr.u.data.pointer = buf;
    iwr.u.data.length = sizeof(buf);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(ioctl_sock, ioctl_val, &iwr) < 0) {
        perror("ioctl[wpssession]");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief  Get power management mode of WLAN driver (SIOCGIWPOWER)
 *
 *  @param enable     Pointer of returned buffer
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_get_power_mode(u8 * enable)
{
    //struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
#if 0
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    iwr.u.power.flags = 0;

    if (ioctl(ioctl_sock, SIOCGIWPOWER, &iwr) < 0) {
        perror("ioctl[SIOCGIWPOWER]");
        ret = WPS_STATUS_FAIL;
    }

    if (iwr.u.power.disabled == 1)
        *enable = 0;
    else
        *enable = 1;

    wps_printf(DEBUG_WLAN, "Get PS Mode : %d", *enable);
#endif
    LEAVE();
    return ret;
}

/** 
 *  @brief  Set power management mode to WLAN driver (SIOCSIWPOWER)
 *
 *  @param enable     0 = Disable PS mode, 1 = Enable PS mode
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_set_power_mode(u8 enable)
{
    //struct iwreq iwr;
    int ret = WPS_STATUS_SUCCESS;

    ENTER();
#if 0
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    wps_printf(DEBUG_WLAN, "Set PS Mode : %d", enable);

    iwr.u.power.flags = 0;
    if (enable == 1)
        iwr.u.power.disabled = 0;
    else
        iwr.u.power.disabled = 1;

    if (ioctl(ioctl_sock, SIOCSIWPOWER, &iwr) < 0) {
        perror("ioctl[SIOCSIWPOWER]");
        ret = WPS_STATUS_FAIL;
    }
#endif
    LEAVE();
    return ret;
}

/** 
 *  @brief  Get compiled version of wireless extension of WLAN driver
 *
 *  @param version    version
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_get_WECompiledVersion(int *version)
{
    ENTER();

    *version = we_version_compiled;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief  Initial ioctl interface to WLAN driver
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_init(WPS_DATA * wps_s)
{
    //int flags;

    ENTER();

    strncpy((char *) dev_name, wps_s->ifname, IFNAMSIZ);

    wps_printf(DEBUG_WLAN, "wps_wlan_init: Initializing wlan interface %s\n", dev_name);
#if 0
    /* 
     * create a socket 
     */

    if ((ioctl_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        wps_printf(MSG_INFO, "wpsapp: Cannot open socket.\n");
        LEAVE();
        ASSERT("exit(-1);" ==0);
    }
#endif
#ifdef WIFI_DIRECT_SUPPORT
    /* Get BSS type */
    wlan_get_bss_type((char *) dev_name, &wps_s->bss_type);

    if (wps_s->bss_type == BSS_TYPE_STA) {
        if (get_range() < 0) {
            wps_printf(MSG_INFO, "wpsapp: Cannot get range.\n");
            LEAVE();
            ASSERT("exit(-1);");
        }
#if 0
        if (wps_wlan_get_ifflags(&flags) != 0 ||
            wps_wlan_set_ifflags(flags | IFF_UP) != 0) {
            wps_printf(MSG_WARNING, "Could not set interface '%s' UP\n",
                       wps_s->ifname);
        }
#endif
    }
#endif
    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief  Close ioctl interface to WLAN driver
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           None 
 */
void
wps_wlan_deinit(WPS_DATA * wps_s)
{
    ENTER();
    /* close existing socket */
    //close(ioctl_sock);

    LEAVE();
    return;
}

/** 
 *  @brief  Process scan operation
 *
 *  @param wps_s      Pointer to WPS global structure
 *  @return           None
 */
void
wps_wlan_scan(WPS_DATA * wps_s, char *ssid)
{
    int ret;
    int retry = 5;
#if 0
    WFD_DATA *pwfd_data = &(wps_s->wfd_data);
#endif
    ENTER();

#ifdef WPS_IE_NEEDED
    /* Append the WPS IE in Probe Request */
    wps_wlan_set_fgscan_wps_ie();
#endif

    /* Send the Scan IOCTL Command */
    do {
        if (ssid)
        {
            ret = wps_wlan_set_scan(ssid, (int) strlen(ssid));
        }
        else
        {
            ret = wps_wlan_set_scan(ssid, 0x0);
        }
        retry--;
    } while ((retry > 0) && (ret != WPS_STATUS_SUCCESS));

#ifdef WPS_IE_NEEDED
    /* Clear the WPS IE in Probe Request */
    wps_wlan_reset_wps_ie();
#endif

    LEAVE();
}

/** 
 *  @brief mapping RF band by frequency
 *
 *  @param freq     frequency value
 *  @return         channel number
 */
int
wps_wlan_freq_to_band(int freq)
{
    int i, table;

    table = sizeof(channel_freq_UN_BG) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (freq == channel_freq_UN_BG[i].Freq) {
            return RF_24_G;
        }
    }

    table = sizeof(channel_freq_UN_AJ) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (freq == channel_freq_UN_AJ[i].Freq) {
            return RF_50_G;
        }
    }

    return RF_24_G;
}

int
mapping_chan_to_freq(int chan)
{
    int i, table;
    int freq = 0;

    table = sizeof(channel_freq_UN_BG) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (chan == channel_freq_UN_BG[i].Channel) {
            freq = channel_freq_UN_BG[i].Freq;
            break;
        }
    }

    table = sizeof(channel_freq_UN_AJ) / sizeof(CHANNEL_FREQ_ENTRY);
    for (i = 0; i < table; i++) {
        if (chan == channel_freq_UN_AJ[i].Channel) {
            freq = channel_freq_UN_AJ[i].Freq;
            break;
        }
    }

    return freq;
}
