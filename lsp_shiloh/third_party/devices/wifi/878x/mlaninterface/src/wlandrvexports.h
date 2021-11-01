/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef _WLANDRVEXPORTS_H
#define _WLANDRVEXPORTS_H

#include "include.h"
#include "mlan_decl.h"
#include "mlan_join.h"
#include "mlan_fw.h"
#include "mlan_util.h"
#include "mlan_main.h"

int wlan_setwpaie_ioctl(mlan_private *priv, char* wpaie, int wpaie_len);
int wlan_wpaon_ioctl(mlan_private *priv);
int wlan_wpaoff_ioctl(mlan_private *priv);
int SupConfig(wpaSupConfig sConfig) ;
int wlan_get_scan_wpa(mlan_private *priv, struct iw_request_info *info, 
			struct iw_point *dwrq, struct SCAN_SSID_IE *scan_res);
int wlan_set_gen_ie_helper(mlan_private * priv, u8 * ie_data_ptr, u16 ie_len);

#endif //_WLANDRVEXPORTS_H
