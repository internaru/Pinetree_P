/** @file wps_eapol.h
 *  @brief This file contains definition for EAPOL functions.
 *  
 *  Copyright (C) 2003-2010, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WPS_EAPOL_H_
#define _WPS_EAPOL_H_

#include "wps_def.h"

/** IEEE 802.1x header */
struct ieee802_1x_hdr
{
    /** Version */
    u8 version;
    /** Type */
    u8 type;
    /** Length */
    u16 length;
    /* followed by length octets of data */
} __attribute__ ((packed));

enum
{ IEEE802_1X_TYPE_EAP_PACKET = 0,
    IEEE802_1X_TYPE_EAPOL_START = 1,
    IEEE802_1X_TYPE_EAPOL_LOGOFF = 2,
    IEEE802_1X_TYPE_EAPOL_KEY = 3,
    IEEE802_1X_TYPE_EAPOL_ENCAPSULATED_ASF_ALERT = 4
};

void wps_rx_eapol(const u8 * src_addr, const u8 * buf, size_t len);
int wps_eapol_send(int type, u8 * buf, size_t len);
int wps_eapol_txStart(void);
int wps_eapol_txPacket(u8 * buf, size_t len);

#endif /* _WPS_EAPOL_H_ */
