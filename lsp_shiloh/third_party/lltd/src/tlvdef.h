/*
 * LICENSE NOTICE.
 *
 * Use of the Microsoft Windows Rally Development Kit is covered under
 * the Microsoft Windows Rally Development Kit License Agreement,
 * which is provided within the Microsoft Windows Rally Development
 * Kit or at http://www.microsoft.com/whdc/rally/rallykit.mspx. If you
 * want a license from Microsoft to use the software in the Microsoft
 * Windows Rally Development Kit, you must (1) complete the designated
 * "licensee" information in the Windows Rally Development Kit License
 * Agreement, and (2) sign and return the Agreement AS IS to Microsoft
 * at the address provided in the Agreement.
 */

/*
 * Copyright (c) Microsoft Corporation 2005.  All rights reserved.
 * This software is provided with NO WARRANTY.
 */

/* This is for a generic OpenWRT installation in an accesspoint/router */

/* pre-processor magic is used to turn this into a struct tlv_info_t
 * and the Tlvs_to_write data initialisers in tlv.c */

    /*      C-type,        name,         repeat, nmbr, access-type  inHello */
    TLVDEF( etheraddr_t,  hostid,              ,   1,  Access_dynamic, TRUE ) 
    TLVDEF( uint32_t,     net_flags,           ,   2,  Access_unset, TRUE )
    TLVDEF( uint32_t,     physical_medium,     ,   3,  Access_unset, TRUE )
#ifdef HAVE_WIRELESS
    TLVDEF( uint8_t,      wireless_mode,       ,   4,  Access_dynamic, TRUE )
    TLVDEF( etheraddr_t,  bssid,               ,   5,  Access_dynamic, TRUE )
    TLVDEF( uint8_t,      ssid,            [32],   6,  Access_dynamic, TRUE )
#endif
    TLVDEF( ipv4addr_t,   ipv4addr,            ,   7,  Access_dynamic, TRUE )
    TLVDEF( ipv6addr_t,   ipv6addr,            ,   8,  Access_invalid, FALSE )
    TLVDEF( uint16_t,     max_op_rate,         ,   9,  Access_invalid, FALSE )
    TLVDEF( uint32_t,     link_speed,          , 0xC,  Access_invalid, FALSE ) // 100bps increments
    TLVDEF( uint64_t,     tsc_ticks_per_sec,   , 0xA,  Access_invalid, FALSE )
    TLVDEF( icon_file_t,  icon_image,          , 0xE,  Access_unset, TRUE ) // (Windows .ico file always LTLV)
    TLVDEF( ucs2char_t,   machine_name,    [16], 0xF,  Access_dynamic, TRUE )
    TLVDEF( ucs2char_t,   support_info,    [32], 0x10, Access_invalid, FALSE )
    TLVDEF( ucs2char_t,   friendly_name,   [32], 0x11, Access_invalid, FALSE )
    TLVDEF( uint8_t,      upnp_uuid,       [16], 0x12, Access_dynamic, TRUE ) // 16 bytes long
    TLVDEF( ucs2char_t,   hw_id,          [200], 0x13, Access_invalid, FALSE ) // 400 bytes long, max
    TLVDEF( uint32_t,     qos_flags,           , 0x14, Access_invalid, FALSE )
    TLVDEF( uint8_t,      wl_physical_medium,  , 0x15, Access_invalid, FALSE )
    TLVDEF( assns_t,      accesspt_assns,      , 0x16, Access_invalid, FALSE ) // RLS: Large_TLV only
    TLVDEF( lg_icon_t,    jumbo_icon,          , 0x18, Access_unset, FALSE ) // RLS: Large_TLV only
    TLVDEF( uint16_t,     sees_max,            , 0x19, Access_unset, TRUE )
    TLVDEF( comptbl_t,    component_tbl,       , 0x1A, Access_invalid, FALSE ) // RLS: Large_TLV only