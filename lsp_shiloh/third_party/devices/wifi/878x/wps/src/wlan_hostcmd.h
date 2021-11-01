/** @file wlan_hostcmd.h
 *  @brief This file contains definitions for Marvell WLAN driver host command.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WLAN_HOSTCMD_H_
#define _WLAN_HOSTCMD_H_

/*
 *  ctype from older glib installations defines BIG_ENDIAN.  Check it 
 *   and undef it if necessary to correctly process the wlan header
 *   files
 */
#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN
#endif

/** 16 bits byte swap */
#define swap_byte_16(x) \
((u16)((((u16)(x) & 0x00ffU) << 8) | \
         (((u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swap_byte_32(x) \
((u32)((((u32)(x) & 0x000000ffUL) << 24) | \
         (((u32)(x) & 0x0000ff00UL) <<  8) | \
         (((u32)(x) & 0x00ff0000UL) >>  8) | \
         (((u32)(x) & 0xff000000UL) >> 24)))

#ifdef BIG_ENDIAN
/** Convert from 16 bit little endian format to CPU format */
#define wlan_le16_to_cpu(x) swap_byte_16(x)
/** Convert from 32 bit little endian format to CPU format */
#define wlan_le32_to_cpu(x) swap_byte_32(x)
/** Convert to 16 bit little endian format from CPU format */
#define wlan_cpu_to_le16(x) swap_byte_16(x)
/** Convert to 32 bit little endian format from CPU format */
#define wlan_cpu_to_le32(x) swap_byte_32(x)
/** Convert WFDCMD header to little endian format from CPU format */
#define endian_convert_request_header(x);               \
    {                                                   \
        (x).cmd_code = wlan_cpu_to_le16((x).cmd_code);   \
        (x).size = wlan_cpu_to_le16((x).size);         \
        (x).seq_num = wlan_cpu_to_le16((x).seq_num);     \
        (x).result = wlan_cpu_to_le16((x).result);     \
    }

/** Convert WFDCMD header from little endian format to CPU format */
#define endian_convert_response_header(x);              \
    {                                                   \
        (x).cmd_code = wlan_le16_to_cpu((x).cmd_code);   \
        (x).size = wlan_le16_to_cpu((x).size);         \
        (x).seq_num = wlan_le16_to_cpu((x).seq_num);     \
        (x).result = wlan_le16_to_cpu((x).result);     \
    }

#else /* BIG_ENDIAN */
/** Do nothing */
#define wlan_le16_to_cpu(x) x
/** Do nothing */
#define wlan_le32_to_cpu(x) x
/** Do nothing */
#define wlan_cpu_to_le16(x) x
/** Do nothing */
#define wlan_cpu_to_le32(x) x
/** Do nothing */
#define endian_convert_request_header(x)
/** Do nothing */
#define endian_convert_response_header(x)

#endif /* BIG_ENDIAN */

#ifndef __ATTRIB_PACK__
/** Pack attribute */
#define __ATTRIB_PACK__  __attribute__((packed))
#endif
/** Command buffer size */
#define MRVDRV_SIZE_OF_CMD_BUFFER       (5 * 1024)

/** Get */
#define HostCmd_ACT_GEN_GET         0x0000
/** Set */
#define HostCmd_ACT_GEN_SET         0x0001

/** TLV Type : SSID */
#define TLV_TYPE_SSID               0x0000
/** TLV Type : Rates  */
#define TLV_TYPE_RATES              0x0001
/** TLV Type : PHY_FH */
#define TLV_TYPE_PHY_FH             0x0002
/** TLV Type : PHY_DS */
#define TLV_TYPE_PHY_DS             0x0003
/** TLV Type : CF */
#define TLV_TYPE_CF                 0x0004
/** TLV Type : IBSS */
#define TLV_TYPE_IBSS               0x0006

/** TLV type ID definition */
#define PROPRIETARY_TLV_BASE_ID     0x0100

/** TLV Type : Key material */
#define TLV_TYPE_KEY_MATERIAL       (PROPRIETARY_TLV_BASE_ID + 0)
/** TLV Type : Channel list */
#define TLV_TYPE_CHANLIST           (PROPRIETARY_TLV_BASE_ID + 1)
/** TLV Type : probes number */
#define TLV_TYPE_NUMPROBES          (PROPRIETARY_TLV_BASE_ID + 2)
/** TLV Type : RSSI Low */
#define TLV_TYPE_RSSI_LOW           (PROPRIETARY_TLV_BASE_ID + 4)
/** TLV Type : SNR Low */
#define TLV_TYPE_SNR_LOW            (PROPRIETARY_TLV_BASE_ID + 5)
/** TLV Type : Fail count */
#define TLV_TYPE_FAILCOUNT          (PROPRIETARY_TLV_BASE_ID + 6)
/** TLV Type : Beacon miss */
#define TLV_TYPE_BCNMISS            (PROPRIETARY_TLV_BASE_ID + 7)
/** TLV Type : LED GPIO */
#define TLV_TYPE_LED_GPIO           (PROPRIETARY_TLV_BASE_ID + 8)
/** TLV Type : LED Behaviour */
#define TLV_TYPE_LEDBEHAVIOR        (PROPRIETARY_TLV_BASE_ID + 9)
/** TLV Type : Pass through */
#define TLV_TYPE_PASSTHROUGH        (PROPRIETARY_TLV_BASE_ID + 10)
/** TLV Type : Power 2.4GHZ */
#define TLV_TYPE_POWER_TBL_2_4GHZ   (PROPRIETARY_TLV_BASE_ID + 12)
/** TLV Type : Power 5 GHZ */
#define TLV_TYPE_POWER_TBL_5GHZ     (PROPRIETARY_TLV_BASE_ID + 13)
/** TLV Type : Broadcast probe */
#define TLV_TYPE_BCASTPROBE         (PROPRIETARY_TLV_BASE_ID + 14)
/** TLV Type : SSID probe number */
#define TLV_TYPE_NUMSSID_PROBE      (PROPRIETARY_TLV_BASE_ID + 15)
/** TLV Type : WMM status */
#define TLV_TYPE_WMMQSTATUS         (PROPRIETARY_TLV_BASE_ID + 16)
/** TLV Type : Crypto data */
#define TLV_TYPE_CRYPTO_DATA        (PROPRIETARY_TLV_BASE_ID + 17)
/** TLV Type : Wild card SSID */
#define TLV_TYPE_WILDCARDSSID       (PROPRIETARY_TLV_BASE_ID + 18)
/** TLV Type : TSF Timestamp */
#define TLV_TYPE_TSFTIMESTAMP       (PROPRIETARY_TLV_BASE_ID + 19)

/* for WPS */

/** TLV Type :WPS selected registrar TLV */
#define TLV_TYPE_WPS_SELECTED_REGISTRAR_TLV    (PROPRIETARY_TLV_BASE_ID + 25)
/** TLV Type :WPS Enrollee TMO Tlv */
#define TLV_TYPE_WPS_ENROLLEE_TMO_TLV          (PROPRIETARY_TLV_BASE_ID + 26)
/** TLV Type :WPS Enrollee probe req TLV */
#define TLV_TYPE_WPS_ENROLLEE_PROBE_REQ_TLV    (PROPRIETARY_TLV_BASE_ID + 27)
/** TLV Type :WPS Registrar beacon TLV */
#define TLV_TYPE_WPS_REGISTRAR_BEACON_TLV      (PROPRIETARY_TLV_BASE_ID + 28)
/** TLV Type :WPS Registrar probe response Tlv */
#define TLV_TYPE_WPS_REGISTRAR_PROBE_RESP_TLV  (PROPRIETARY_TLV_BASE_ID + 29)
/** TLV type : STA Mac address */
#define TLV_TYPE_STA_MAC_ADDRESS               (PROPRIETARY_TLV_BASE_ID + 32)
/** TLV: Management Frame from FW */
#define MRVL_MGMT_FRAME_TLV_ID                 (PROPRIETARY_TLV_BASE_ID + 104)
/** TLV: Management IE list */
#define MRVL_MGMT_IE_LIST_TLV_ID               (PROPRIETARY_TLV_BASE_ID + 105)

/** length of Marvell TLV */
#define TLV_HEADER_SIZE     4

/** IEEE Type definitions  */
typedef enum _IEEEtypes_ElementId_e
{
    SSID = 0,
    SUPPORTED_RATES = 1,
    FH_PARAM_SET = 2,
    DS_PARAM_SET = 3,
    CF_PARAM_SET = 4,
    TIM,
    IBSS_PARAM_SET = 6,
    COUNTRY_INFO = 7,

    CHALLENGE_TEXT = 16,

    EXTENDED_SUPPORTED_RATES = 50,

    VENDOR_SPECIFIC_221 = 221,
    WMM_IE = VENDOR_SPECIFIC_221,
    WPA_IE = VENDOR_SPECIFIC_221,
    WPS_IE = VENDOR_SPECIFIC_221,

    RSN_IE = 48,
    WPA2_IE = 48,

    EXTRA_IE = 133,
} __ATTRIB_PACK__ IEEEtypes_ElementId_e;

/** TLV related data structures*/
/** MrvlIEtypesHeader_t */
typedef struct _MrvlIEtypesHeader
{
    /** Type */
    u16 Type;
    /** Length */
    u16 Len;
} __ATTRIB_PACK__ MrvlIEtypesHeader_t;

typedef struct _HostCmd_DS_GEN
{
    /** Command */
    u16 Command;
    /** Size */
    u16 Size;
    /** sequence number */
    u16 SeqNum;
    /** Result */
    u16 Result;
} __ATTRIB_PACK__ HostCmd_DS_GEN, *PHostCmd_DS_GEN;

/** Sizeof DS_GEN */
#define S_DS_GEN    sizeof(HostCmd_DS_GEN)

/** Private command ID to get BSS type */
#define GET_BSS_TYPE        (SIOCDEVPRIVATE + 15)

/** IEEEtypesHeader_t */
typedef struct _IEEEtypes_Header_t
{
    /** Type */
    u8 Type;
    /** Length */
    u8 Len;
} __ATTRIB_PACK__ IEEEtypes_Header_t;

/** MRVL command header buffer */
typedef struct _mrvl_cmd_head_buf
{
    /** Buf Size */
    u32 buf_size;
    /** Command Code */
    u16 cmd_code;
    /** Size */
    u16 size;
    /** Sequence Number */
    u16 seq_num;
    /** Result */
    s16 result;
} __ATTRIB_PACK__ mrvl_cmd_head_buf;

/** Private command ID to set/get custom IE buffer */
#define WLAN_CUSTOM_IE       (SIOCDEVPRIVATE + 13)

/** custom IE */
typedef struct _custom_ie
{
    /** IE Index */
    u16 ie_index;
        /** Mgmt Subtype Mask */
    u16 mgmt_subtype_mask;
        /** IE Length */
    u16 ie_length;
        /** IE buffer */
    u8 ie_buffer[0];
} __ATTRIB_PACK__ custom_ie;

/** TLV buffer : custom IE */
typedef struct _tlvbuf_custom_ie
{
    /** Tag */
    u16 tag;
    /** Length */
    u16 length;
        /** custom IE data */
    custom_ie ie_data[0];
} __ATTRIB_PACK__ tlvbuf_custom_ie;

 /** mlan_deauth_param */
typedef struct _deauth_param
{
     /** STA mac addr */
    u8 mac_addr[ETH_ALEN];
     /** deauth reason */
    u16 reason_code;
} deauth_param;

/** uAP Host Command ioctl number */
#define UAPHOSTCMD          (SIOCDEVPRIVATE + 1)
/** Private command ID to BSS config */
#define UAP_BSS_CONFIG      (SIOCDEVPRIVATE + 6)
/** Private command ID to AP Deauth */
#define UAP_BSS_STA_DEAUTH  (SIOCDEVPRIVATE + 7)

/** APCMD buffer : sys_configure */
typedef struct _apcmdbuf_bss_configure
{
        /** Action : GET or SET */
    u32 Action;
} apcmdbuf_bss_configure;

#endif /* _WLAN_HOSTCMD_H_ */
