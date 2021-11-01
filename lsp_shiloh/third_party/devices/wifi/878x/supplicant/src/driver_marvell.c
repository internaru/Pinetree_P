/*
 * WPA Supplicant - driver interaction with Linux Marvell drivers
 * 
 *  Copyright © 2006, Marvell International Ltd.
 *  
 *  All Rights Reserved
 *  
 * 1. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 2. Neither the name of Marvell nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND  CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR  
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>
#include <string.h>
//#include <sys/ioctl.h>
//#include <errno.h>
#include "includes.h"

#include "wireless_copy.h"
#include "common.h"
#include "driver.h"
#include "driver_wext.h"
#include "eloop.h"
#include "driver_hostap.h"
#include "wpa_supplicant.h"
#include "defs.h"

#include "raw_socket_api.h"

#define EAP_ALG_NONE    0x01
#define EAP_ALG_LEAP    0x02
#define EAP_ALG_TLS   0x04
#define EAP_ALG_TTLS    0x08
#define EAP_ALG_MD5   0x10

#ifndef AUTH_ALG_NETWORK_EAP
#define AUTH_ALG_NETWORK_EAP  0x08
#endif

typedef unsigned char uchar;
//typedef char * caddr_t;

typedef u8 WLAN_802_11_MAC_ADDRESS[ETH_ALEN];
typedef u8 WLAN_802_11_KEY_RSC[8];

struct wpa_driver_marvell_data {
	void  *wext; /* Private data for driver_wext */
	void  *ctx;
	char  ifname[IFNAMSIZ + 1];
	int sock;
	Boolean CounterMeasure;
	Boolean WpaEnabled;
};

struct _WLAN_802_11_KEY {
	ulong Length;   /* Length of this structure */
	ulong KeyIndex;
	ulong KeyLength;  /* Length of key in bytes */
	WLAN_802_11_MAC_ADDRESS BSSID;
	WLAN_802_11_KEY_RSC KeyRSC;
	u8 KeyMaterial[0];
} __attribute__ ((packed));

typedef struct _WLAN_802_11_KEY WLAN_802_11_KEY, *PWLAN_802_11_KEY;

struct _WLAN_802_11_SSID {
	ulong ssid_len;
	uchar ssid[32];
} __attribute__ ((packed));

typedef struct _WLAN_802_11_SSID WLAN_802_11_SSID, *PWLAN_802_11_SSID;


int WlanShimCSetWpaIE(char * wpaie, int wpaieLen);
int WlanShimCSetKey(struct iw_encode_ext *param, int length);
int WlanShimCDeauth(void);
void WlanSetCountermeasures(int enabled);


// #define IW_MAX_PRIV_DEF   128
// #define IW_PRIV_IOCTL_COUNT (SIOCIWLASTPRIV-SIOCIWFIRSTPRIV+1)

//static int Priv_count;
//static struct iw_priv_args Priv_args[IW_MAX_PRIV_DEF];

#if 0
static int get_private_info(const char *ifname)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling mrvl_get_private");
    return 0;
}
#endif
/*
static char *priv_ioctl_names[] = {
	"extscan",
	"authalgs",
	"setwpaie",
	"deauth",
	"encryptionmode",
	"reasso-on",
	"reasso-off",
};

static int PRIV_IOCTL[sizeof(priv_ioctl_names)/sizeof(priv_ioctl_names[0])];
static int PRIV_SUBIOCTL[sizeof(priv_ioctl_names)/sizeof(priv_ioctl_names[0])];

#define IOCTL_WLANEXTSCAN		PRIV_IOCTL[0]
#define SUBIOCTL_WLANEXTSCAN		PRIV_SUBIOCTL[0]
#define IOCTL_SET_AP_AUTH_ALGS		PRIV_IOCTL[1]
#define SUBIOCTL_SET_AP_AUTH_ALGS	PRIV_SUBIOCTL[1]
#define IOCTL_SET_WPA_IE		PRIV_IOCTL[2]
#define SUBIOCTL_SET_WPA_IE		PRIV_SUBIOCTL[2]
#define IOCTL_DEAUTH			PRIV_IOCTL[3]
#define SUBIOCTL_DEAUTH			PRIV_SUBIOCTL[3]
#define IOCTL_ENCRYPTION_MODE		PRIV_IOCTL[4]
#define SUBIOCTL_ENCRYPTION_MODE	PRIV_SUBIOCTL[4]
#define IOCTL_REASSO_ON			PRIV_IOCTL[5]
#define SUBIOCTL_REASSO_ON		PRIV_SUBIOCTL[5]
#define IOCTL_REASSO_OFF		PRIV_IOCTL[6]
#define SUBIOCTL_REASSO_OFF		PRIV_SUBIOCTL[6]

int marvell_get_subioctl_no(int i, int *sub_cmd)
{
	int j;

	if (Priv_args[i].cmd >= SIOCDEVPRIVATE) {
		*sub_cmd = 0;
		return Priv_args[i].cmd;
	}
  	j = -1;

	// Find the matching *real* ioctl 
	while ((++j < Priv_count) && ((Priv_args[j].name[0] != '\0') ||
		(Priv_args[j].set_args != Priv_args[i].set_args) || 
		(Priv_args[j].get_args != Priv_args[i].get_args)));

  	// If not found... 
  	if (j == Priv_count) {
		wpa_printf(MSG_DEBUG,
			"%s: Invalid private ioctl definition for: 0x%x\n",
			__FUNCTION__, Priv_args[i].cmd);
		return -1;
	}

	// Save sub-ioctl number 
	*sub_cmd = Priv_args[i].cmd;
	// Return the real IOCTL number 
	return Priv_args[j].cmd;
}

static int marvell_get_ioctl_no(const char *ifname, const char *priv_cmd,
        int *sub_cmd)
{
	int i;

	// Are there any private ioctls? 
	if (Priv_count <= 0) {
	// Could skip this message ? 
	wpa_printf(MSG_DEBUG, "%-8.8s  no private ioctls.\n", "eth1");
	} else {
#ifdef MARVELL_DEBUG
		wpa_printf(MSG_DEBUG, "%-8.8s  Available private ioctl :\n", "eth1");
#endif  // MARVELL_DEBUG 
		for (i = 0; i < Priv_count; i++) {
			if (Priv_args[i].name[0] && !strcmp(Priv_args[i].name, priv_cmd)) {
				return marvell_get_subioctl_no(i, sub_cmd);
			}
		}
	}
	return -1;
}
*/
void marvell_init_ioctl_numbers(const char *ifname)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling mrvl_init_ioctl");
}

#if 0
static int marvell_ioctl(const char *dev, int cmdno, caddr_t param,
             int len, int show_err)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling mrvl_ioctl");
    return 0;
}
#endif

static int wpa_driver_marvell_set_wpa_ie(const char *ifname, 
           const char *wpa_ie, size_t wpa_ie_len)
{
    int ret;
    //-- API from Shim C layer.
    ret = WlanShimCSetWpaIE((char*)wpa_ie, wpa_ie_len);

    return ret;
}

/* marvell2param - function to call get/set ioctl's that use sub-commands */
#if 0
static int marvell2param(const char *ifname, int cmdno, int param, int value)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling mrvl2param");
    return 0;
}
#endif

int wpa_driver_marvell_set_wpa(void *priv, int enabled)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling supplicant function marvell_set_wpa that is needed for WPA2, but is not yet implemented");
    return 0;
}

static int wpa_driver_marvell_set_key(void *priv, wpa_alg alg,
           const u8 *addr, int key_idx,
           int set_tx, const u8 * seq, size_t seq_len,
           const u8 * key, size_t key_len)
{
    //WLAN_802_11_KEY *param;
    struct iw_encode_ext *param;
    u8 *buf;
    size_t blen;
    int ret = 0, flag = 0 ;
    char *alg_name;

    /* Marvell driver does not support setting NULL keys.   */
    if (alg == WPA_ALG_NONE)
        if (!key_len) {
            return -1;
        }

    switch (alg) {
    case WPA_ALG_NONE:
        alg_name = "none";
        break;
    case WPA_ALG_WEP:
        alg_name = "WEP";
        flag = 1;   
        break;
    case WPA_ALG_TKIP:
        alg_name = "TKIP";
        break;
    case WPA_ALG_CCMP:
        alg_name = "CCMP";
        break;
    default:
        return -1;
    }

    wpa_printf(MSG_DEBUG, "%s: alg=%s key_idx=%d set_tx=%d seq_len=%d "
           "key_len=%d", __FUNCTION__, alg_name, key_idx, set_tx,
           seq_len, key_len);

    if (seq_len > 8)
        return -2;
    blen = sizeof(*param) + key_len;
    buf = (u8 *)os_malloc(blen);
    if (buf == NULL)
        return -1;
    memset(buf, 0, blen);
#if 0
    param = (WLAN_802_11_KEY *) buf;

    /* TODO: In theory, STA in client mode can use five keys; four default
     * keys for receiving (with keyidx 0..3) and one individual key for
     * both transmitting and receiving (keyidx 0) _unicast_ packets. Now,
     * keyidx 0 is reserved for this unicast use and default keys can only
     * use keyidx 1..3 (i.e., default key with keyidx 0 is not supported).
     * This should be fine for more or less all cases, but for completeness
     * sake, the driver could be enhanced to support the missing key. */

    // Param structure filled here
    memset((u8*)param->BSSID, 0xff, ETH_ALEN);

    if (addr) {
        if (!key_idx)
            key_idx = 0x40000000;
        param->KeyIndex = key_idx;
        param->KeyLength = key_len;
        memcpy((u8 *)param->BSSID, addr, ETH_ALEN);
        memcpy((u8 *)&param->KeyRSC, seq, seq_len);

        if (key)
            memcpy(param->KeyMaterial , key, key_len);
    }
#else
    param = (struct iw_encode_ext *) buf;
    param->key_len = key_len;
    param->ext_flags = key_idx + 1;
    if (key)
        memcpy(param->key , key, key_len);
    if (addr)
        memcpy(param->addr.sa_data, addr, ETH_ALEN);
#endif
    WlanShimCSetKey(param, blen);
    os_free(buf);
    return ret;
}

static int wpa_driver_marvell_set_countermeasures(void *priv, int enabled)
{
	//struct wpa_driver_marvell_data *drv = priv;

    WlanSetCountermeasures(enabled);

	wpa_printf(MSG_DEBUG, "%s: enabled=%d", __FUNCTION__, enabled);
	return 0;
}

static int wpa_driver_marvell_set_drop_unencrypted(void *priv, int enabled)
{
	wpa_printf(MSG_DEBUG, "%s: enabled=%d", __FUNCTION__, enabled);
	return 0;
}

#if 0
static int wpa_driver_marvell_reset(const char *ifname, int type)
{
	return 0;
}
#endif

#if 0
static int wpa_driver_marvell_mlme(void *priv, const u8 *addr, int cmd,
                 int reason_code)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    return 0;
}
#endif

static int wpa_driver_marvell_deauthenticate(void *priv, const u8 *addr,
               int reason_code)
{
	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    return WlanShimCDeauth();
//	return wpa_driver_marvell_mlme(priv, addr, MLME_STA_DEAUTH,
//              reason_code);
}

static int wpa_driver_marvell_disassociate(void *priv, const u8 *addr,
                 int reason_code)
{
	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
	return WlanShimCDeauth();
}

static int wpa_driver_marvell_associate(void *priv, 
      struct wpa_driver_associate_params *params)
{
	int ret = 0;
	int allow_unencrypted_eapol;
	struct wpa_driver_marvell_data *drv = priv;

	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    wpa_printf(1, "==>SUP: wpa_driver_marvell_associate\n");
/*
	if (IOCTL_REASSO_OFF > 0) {
		wpa_printf(MSG_DEBUG, "marvell: auto association off");
		marvell2param(drv->ifname, IOCTL_REASSO_OFF, 
			SUBIOCTL_REASSO_OFF, 0);
	}
*/
#ifdef MARVELL_DEBUG
	printf("pairwise_suite = %d, group_suite = %d, key_mgmt_suite =%d \n", 
		params->pairwise_suite, params->group_suite, 
		params->key_mgmt_suite);
#endif  /* MARVELL_DEBUG */

	//set mode
	if (wpa_driver_wext_set_mode(drv->wext, params->mode) < 0)
		ret = -1;	

	/*
	* As in most cases the cipher is same for pairwise and group 
	* send only the group cipher, this is true for PSK, ENTERPRISE,
	* LEAP. This is a workaround, need to implement a better way.
	*/
/* 
	if (IOCTL_ENCRYPTION_MODE > 0) {
		marvell2param(drv->ifname, IOCTL_ENCRYPTION_MODE,
			SUBIOCTL_ENCRYPTION_MODE, params->group_suite);
	}
*/
	/* 
	* If the Key mgmt is NONE then disable WPA in the driver, and don't 
	* set the WPA_IE in the driver. 
	*/
	if (params->key_mgmt_suite == KEY_MGMT_802_1X_NO_WPA) {
		if (wpa_driver_marvell_set_wpa(priv, 0) < 0) {
//			fprintf(stderr, "Failed to disable WPA in the driver.\n");
			return -1;
		}
	} else {
		if (wpa_driver_marvell_set_wpa_ie(drv->ifname, (char *)params->wpa_ie, 
			params->wpa_ie_len) < 0)
			ret = -1;
	}

	/* Set SSID in Marvell drivers triggers association */
	if (wpa_driver_wext_set_ssid(drv->wext, params->ssid, 
		params->ssid_len) < 0)
		ret = -1;

	/*
	* Allow unencrypted EAPOL messages even if pairwise keys are set when
	* not using WPA. IEEE 802.1X specifies that these frames are not
	* encrypted, but WPA encrypts them when pairwise keys are in use.
	*/
	if (params->key_mgmt_suite == KEY_MGMT_802_1X ||
		params->key_mgmt_suite == KEY_MGMT_PSK)
		allow_unencrypted_eapol = 0;
	else
		allow_unencrypted_eapol = 1;

	return ret;
}

static int wpa_driver_marvell_scan(void *priv, const u8 * ssid,
              size_t ssid_len)
{
    //WLAN_802_11_SSID param;

    if (ssid) 
        wpa_printf(1, "wpa_driver_marvell_scan scanning for ssid %s\n", ssid);
    else
        wpa_printf(1, "wpa_driver_marvell_scan scanning for ssid << broadcast >>\n");

    return wpa_driver_wext_scan(priv, ssid, ssid_len);
}

static int wpa_driver_marvell_set_auth_alg(void *priv, int auth_alg)
{
	int algs = AUTH_ALG_OPEN_SYSTEM;
	int eap_algs = EAP_ALG_NONE;
	int ret = 0;
	//struct wpa_driver_marvell_data *drv = priv;

	if (auth_alg & AUTH_ALG_LEAP) {
		eap_algs = EAP_ALG_LEAP;
		/* For WPA should be disabled in the driver */
		if ((ret = wpa_driver_marvell_set_wpa(priv, 0)) < 0) {
//			fprintf(stderr,"Failed to disable WPA in the driver.\n");
			return ret;
		}
	}
  
	if (auth_alg & AUTH_ALG_OPEN_SYSTEM) {
		/* OPEN/OPEN+SHARED/OPEN+EAPLEAP/OPEN+SHARED+EAPLEAP */
		algs = AUTH_ALG_OPEN_SYSTEM;
	} else if (auth_alg & AUTH_ALG_SHARED_KEY) {
		/* SHARED/SHARED+EAPLEAP */
		algs = AUTH_ALG_SHARED_KEY;
	} else if (auth_alg & AUTH_ALG_LEAP) {
		/* NETWORKEAP+EAPLEAP */
		algs = AUTH_ALG_NETWORK_EAP;
	}

	wpa_printf(MSG_DEBUG, "Setting Auth Alg to 0x%X\n", algs);

//	if (IOCTL_SET_AP_AUTH_ALGS > 0)
//		ret = marvell2param(drv->ifname, IOCTL_SET_AP_AUTH_ALGS,
//			SUBIOCTL_SET_AP_AUTH_ALGS, algs);
	return ret;
}

int wpa_driver_marvell_get_ssid(void *priv, u8 *ssid)
{
	struct wpa_driver_marvell_data *drv = priv;
	return wpa_driver_wext_get_ssid(drv->wext, ssid);
}

static int wpa_driver_marvell_get_bssid(void *priv, u8 *bssid)
{
	struct wpa_driver_marvell_data *drv = priv;
	return wpa_driver_wext_get_bssid(drv->wext, bssid);
}

static int wpa_driver_marvell_get_scan_results(void *priv,
                struct wpa_scan_result *results,
                size_t max_size)
{
    int ret;
    struct wpa_driver_marvell_data *drv = priv;
//    wpa_printf(MSG_DEBUG, "SUP: mrvl driver -- Getting scan results", ret);
    ret = wpa_driver_wext_get_scan_results(drv->wext, results, max_size);
//    wpa_printf(MSG_DEBUG, "SUP: mrvl driver -- Getting scan results (exit %d)", ret);
    return ret;
}

static void *wpa_driver_marvell_init(void *ctx, const char *ifname)
{
	struct wpa_driver_marvell_data *drv;

//	marvell_init_ioctl_numbers(ifname);

	drv = os_malloc(sizeof(*drv));
	if (drv == NULL)
		return NULL;
	memset(drv, 0, sizeof(*drv));
	drv->wext = wpa_driver_wext_init(ctx, ifname);
	if (drv->wext == NULL) {
		os_free(drv);
		return NULL;
	}

	drv->ctx = ctx;
	strncpy(drv->ifname, ifname, sizeof(drv->ifname));
/*
    drv->sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (drv->sock < 0) {
		perror("socket");
		wpa_driver_wext_deinit(drv->wext);
		os_free(drv);
		return NULL;
	}
*/
    drv->sock = -1;

    return drv;
}

static void wpa_driver_marvell_deinit(void *priv)
{
	struct wpa_driver_marvell_data *drv = priv;

	if (drv->WpaEnabled == TRUE) {
		if (wpa_driver_marvell_set_wpa(priv, 0) < 0) {
//			fprintf(stderr, "WPA ON ioctl failed\n");
		}
	}

	wpa_driver_wext_deinit(drv->wext);
//	close(drv->sock);
	os_free(drv);
}

struct wpa_driver_ops wpa_driver_marvell_ops = {
	.name = "marvell",
	.desc = "Marvell Semiconductor Inc.",
	.get_bssid = wpa_driver_marvell_get_bssid,
	.get_ssid = wpa_driver_marvell_get_ssid,
	.set_wpa = wpa_driver_marvell_set_wpa,
	.set_key = wpa_driver_marvell_set_key,
	.set_countermeasures = wpa_driver_marvell_set_countermeasures,
	.set_drop_unencrypted = wpa_driver_marvell_set_drop_unencrypted,
	.scan = wpa_driver_marvell_scan,
	.get_scan_results = wpa_driver_marvell_get_scan_results,
	.deauthenticate = wpa_driver_marvell_deauthenticate,
	.disassociate = wpa_driver_marvell_disassociate,
	.associate = wpa_driver_marvell_associate,
	.set_auth_alg = wpa_driver_marvell_set_auth_alg,
	.init = wpa_driver_marvell_init,
	.deinit = wpa_driver_marvell_deinit,
};

#ifdef MARVELL_DEBUG
void DUMPHEX(char *prompt, u8 * buf, int len)
{
	int i;

	printf("%s: ", prompt);
	for (i = 0; i < len; i++)
		printf("%02x ", *buf++);
	printf("\n");
}
#endif  /* MARVELL_DEBUG */

