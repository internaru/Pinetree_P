/*
 * WPA Supplicant / Configuration backend: empty starting point
 * Copyright (c) 2003-2005, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file implements dummy example of a configuration backend. None of the
 * functions are actually implemented so this can be used as a simple
 * compilation test or a starting point for a new configuration backend.
 */

#include "tx_api.h"
#include "logger.h"   //SS added to resolve the minVsprintf

#include "includes.h"

#include "common.h"
#include "config.h"
#include "base64.h"

#include "stddef.h"
#include "memAPI.h"
#include "lassert.h"
#include "platform.h"   
#include "wifi_intf_api.h"
#include "sha1.h"
#include "wlanInterface.h"

struct wpa_config * wpa_config_read(const char *name)
{
	struct wpa_config *config;
    struct wpa_ssid *ssid;
    int i;
    uint8_t passphrase_buf[MAX_PASS_PHRASE];

	config = wpa_config_alloc_empty(NULL, NULL);
	if (config == NULL)
		return NULL;

    wpa_printf(MSG_DEBUG, "SUP: Config wireless from datastore");

    ssid = wpa_config_add_network(config);
    if (ssid == NULL)
        return NULL;
    wpa_config_set_network_defaults(ssid);

    // Configure the supplicant from what he have stored in the datastore
    config->ap_scan = 1;
    config->eapol_version = 1;

    ssid->ssid = os_malloc(MAX_SSID_NAME_LEN);
    memset(ssid->ssid, 0, MAX_SSID_NAME_LEN);
    if (strcmp(name, WIFI_IFACE_NAME_WFD) == 0)
        wifi_intf_get_var_ssid(WIFI_IFACE_ID_WFD, ssid->ssid, MAX_SSID_NAME_LEN);
    else
        wifi_intf_get_var_ssid(WIFI_IFACE_ID_STATION, ssid->ssid, MAX_SSID_NAME_LEN);
    wpa_printf(MSG_DEBUG, "SUP: Configuring with SSID %s", ssid->ssid);
    ssid->ssid_len = strlen((char*)ssid->ssid);

    ssid->passphrase = os_malloc(MAX_PASS_PHRASE);
    memset(ssid->passphrase, 0, MAX_PASS_PHRASE);
    memset(passphrase_buf, 0, MAX_PASS_PHRASE);
    if (strcmp(name, WIFI_IFACE_NAME_WFD) == 0)
        wifi_intf_get_var_wpa_passphrase(WIFI_IFACE_ID_WFD, passphrase_buf, MAX_PASS_PHRASE);
    else
        wifi_intf_get_var_wpa_passphrase(WIFI_IFACE_ID_STATION, passphrase_buf, MAX_PASS_PHRASE);
    if (strlen((char*)passphrase_buf) < 64)
    {
        wpa_printf(MSG_DEBUG, "SUP: Configuring with Passphrase %s", passphrase_buf);
        strcpy((char*)ssid->passphrase, (char*)passphrase_buf);
        pbkdf2_sha1(ssid->passphrase, (char*)ssid->ssid, ssid->ssid_len, 4096, ssid->psk, PMK_LEN);
    }
    else
    {
        // We were given the PSK directly.  Convert it from ASCII to HEX bytes.
        for (i = 0; i < PMK_LEN; i++)
        {
            ssid->psk[i] = 0;
            if (passphrase_buf[2 * i] >= '0' && passphrase_buf[2 * i] <= '9')
                ssid->psk[i] = (UINT8)(passphrase_buf[2 * i] - '0');
            else if (passphrase_buf[2 * i] >= 'a' && passphrase_buf[2 * i] <= 'f')
                ssid->psk[i] = (UINT8)(passphrase_buf[2 * i] - 'a' + 10);
            else if (passphrase_buf[2 * i] >= 'A' && passphrase_buf[2 * i] <= 'F')
                ssid->psk[i] = (UINT8)(passphrase_buf[2 * i] - 'A' + 10);
            ssid->psk[i] <<= 4;
            if (passphrase_buf[2 * i + 1] >= '0' && passphrase_buf[2 * i + 1] <= '9')
                ssid->psk[i] += (UINT8)(passphrase_buf[2 * i + 1] - '0');
            else if (passphrase_buf[2 * i + 1] >= 'a' && passphrase_buf[2 * i + 1] <= 'f')
                ssid->psk[i] += (UINT8)(passphrase_buf[2 * i + 1] - 'a' + 10);
            else if (passphrase_buf[2 * i + 1] >= 'A' && passphrase_buf[2 * i + 1] <= 'F')
                ssid->psk[i] += (UINT8)(passphrase_buf[2 * i + 1] - 'A' + 10);
        }
        wpa_hexdump(MSG_DEBUG, "SUP: Configuring with PSK directly: ", ssid->psk, PMK_LEN);
    }

    ssid->scan_ssid = 1;
    ssid->psk_set = 1;
    for (i = 0; i < 4; i++)
    {
        //strcpy(ssid->wep_key[i], dsConfigInfo->WEPKey[i]);
        if (strcmp(name, WIFI_IFACE_NAME_WFD) == 0)
            wifi_intf_get_var_wep_key(WIFI_IFACE_ID_WFD, i, ssid->wep_key[i], sizeof(ssid->wep_key[i]));
        else
            wifi_intf_get_var_wep_key(WIFI_IFACE_ID_STATION, i, ssid->wep_key[i], sizeof(ssid->wep_key[i]));
    }

	return config;
}


int wpa_config_write(const char *name, struct wpa_config *config)
{
	struct wpa_ssid *ssid;
	struct wpa_config_blob *blob;

	wpa_printf(MSG_DEBUG, "Writing configuration file '%s'", name);

	/* TODO: write global config parameters */


	for (ssid = config->ssid; ssid; ssid = ssid->next) {
		/* TODO: write networks */
	}

	for (blob = config->blobs; blob; blob = blob->next) {
		/* TODO: write blobs */
	}

	return 0;
}
