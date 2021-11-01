/*
 * WPA Supplicant - driver interaction with generic Linux Wireless Extensions
 * Copyright (c) 2003-2006, Jouni Malinen <jkmaline@cc.hut.fi>
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
 * This file implements a driver interface for the Linux Wireless Extensions.
 * When used with WE-18 or newer, this interface can be used as-is with number
 * of drivers. In addition to this, some of the common functions in this file
 * can be used by other driver interface implementations that use generic WE
 * ioctls, but require private ioctls for some of the functionality.
 */

#include "tx_api.h"
#include "agMessage.h"
#include "includes.h"

#define __func__ "driver_wext.c"

#include "wireless_copy.h"
#include "common.h"
#include "driver.h"
#include "l2_packet.h"
#include "eloop.h"
#include "wpa_supplicant.h"
#include "priv_netlink.h"
#include "driver_wext.h"
#include "wpa.h"

#include "raw_socket_api.h"
#include "wlanInterface.h"

#ifdef CONFIG_CLIENT_MLME
#include <netpacket/packet.h>
#include <hostapd_ioctl.h>
#include <net/d80211_common.h>
#include <net/d80211_shared.h>
#include "mlme.h"

#ifndef ETH_P_ALL
#define ETH_P_ALL 0x0003
#endif
#endif /* CONFIG_CLIENT_MLME */

#define WEXT_RECV_BUF_SIZE   8192

extern TX_QUEUE supplicantEventMsgQ;    // defined in eloop.c  -jrs 
extern SUPPLICANT_THREAD_STATUS supplicantThreadStatus;

struct wpa_driver_wext_data {
	void *ctx;
	int event_sock;
	int ioctl_sock;
	int mlme_sock;
	char ifname[IFNAMSIZ + 1];
	int ifindex;
	int ifindex2;
	u8 *assoc_req_ies;
	size_t assoc_req_ies_len;
	u8 *assoc_resp_ies;
	size_t assoc_resp_ies_len;
	struct wpa_driver_capa capa;
	int has_capability;
	int we_version_compiled;

	/* for set_auth_alg fallback */
	int use_crypt;
	int auth_alg_fallback;

	int operstate;

	char mlmedev[IFNAMSIZ + 1];
};

struct SCAN_SSID_IE{
	u8    SSID[33];
	ulong ssid_len;
	u8    BSSID[6];
	u16   Capability;
	u8    WPA_IE[40];
	int   wpa_ie_len;
    int   channel;
};

typedef u8  WLAN_802_11_MAC_ADDRESS[ETH_ALEN];
typedef u8  WLAN_802_11_KEY_RSC[8];

struct _WLAN_802_11_KEY {
	ulong Length;   /* Length of this structure */
	ulong KeyIndex;
	ulong KeyLength;  /* Length of key in bytes */
	WLAN_802_11_MAC_ADDRESS BSSID;
	WLAN_802_11_KEY_RSC KeyRSC;
	u8 KeyMaterial[0];
} __attribute__ ((packed));

typedef struct _WLAN_802_11_KEY WLAN_802_11_KEY, *PWLAN_802_11_KEY;


unsigned int if_nametoindex(const char * ifname);
int WlanShimCGetScanResult(struct SCAN_SSID_IE *scanResult, int maxAPs);
int WlanShimCSetScanWithSsid(char * ssid, int ssid_len);
int WlanShimCGetSsid(char *ssid);
int WlanShimCSetWpaIE(char * wpaie, int wpaieLen);
int WlanShimCSetKey(struct iw_encode_ext *param, int length);
int WlanShimCDeauth(void);


static int wpa_driver_wext_flush_pmkid(void *priv);
static int wpa_driver_wext_get_range(void *priv);

static int wpa_driver_wext_send_oper_ifla(struct wpa_driver_wext_data *drv,
					  int linkmode, int operstate)
{
#if 0
	struct {
		struct nlmsghdr hdr;
		struct ifinfomsg ifinfo;
		char opts[16];
	} req;
	struct rtattr *rta;
	static int nl_seq;
	ssize_t ret;

	req.hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
	req.hdr.nlmsg_type = RTM_SETLINK;
	req.hdr.nlmsg_flags = NLM_F_REQUEST;
	req.hdr.nlmsg_seq = ++nl_seq;
	req.hdr.nlmsg_pid = 0;

	req.ifinfo.ifi_family = AF_UNSPEC;
	req.ifinfo.ifi_type = 0;
	req.ifinfo.ifi_index = drv->ifindex;
	req.ifinfo.ifi_flags = 0;
	req.ifinfo.ifi_change = 0;

	if (linkmode != -1) {
		rta = (struct rtattr *)
			((char *) &req + NLMSG_ALIGN(req.hdr.nlmsg_len));
		rta->rta_type = IFLA_LINKMODE;
		rta->rta_len = RTA_LENGTH(sizeof(char));
		*((char *) RTA_DATA(rta)) = linkmode;
		req.hdr.nlmsg_len = NLMSG_ALIGN(req.hdr.nlmsg_len) +
			RTA_LENGTH(sizeof(char));
	}
	if (operstate != -1) {
		rta = (struct rtattr *)
			((char *) &req + NLMSG_ALIGN(req.hdr.nlmsg_len));
		rta->rta_type = IFLA_OPERSTATE;
		rta->rta_len = RTA_LENGTH(sizeof(char));
		*((char *) RTA_DATA(rta)) = operstate;
		req.hdr.nlmsg_len = NLMSG_ALIGN(req.hdr.nlmsg_len) +
			RTA_LENGTH(sizeof(char));
	}

	wpa_printf(MSG_DEBUG, "WEXT: Operstate: linkmode=%d, operstate=%d",
		   linkmode, operstate);

	ret = send(drv->event_sock,(char *) &req, req.hdr.nlmsg_len, 0);
	if (ret < 0) {
		wpa_printf(MSG_DEBUG, "WEXT: Sending operstate IFLA failed: "
			   "%s (assume operstate is not supported)",
			   strerror(errno));
	}

	return ret < 0 ? -1 : 0;
#else
    return 0;
#endif
}


static int wpa_driver_wext_set_auth_param(struct wpa_driver_wext_data *drv,
					  int idx, u32 value)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_set_auth_param");
    return 0;
}


/**
 * wpa_driver_wext_get_bssid - Get BSSID, SIOCGIWAP
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @bssid: Buffer for BSSID
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_get_bssid(void *priv, u8 *bssid)
{
	wifi_intf_get_var_bssid(WIFI_IFACE_ID_STATION, bssid, ETH_ALEN);
	return 0;
}


/**
 * wpa_driver_wext_set_bssid - Set BSSID, SIOCSIWAP
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @bssid: BSSID
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_bssid(void *priv, const u8 *bssid)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_set_bssid");
    return 0;
}


/**
 * wpa_driver_wext_get_ssid - Get SSID, SIOCGIWESSID
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @ssid: Buffer for the SSID; must be at least 32 bytes long
 * Returns: SSID length on success, -1 on failure
 */
int wpa_driver_wext_get_ssid(void *priv, u8 *ssid)
{
    int len;

    len = WlanShimCGetSsid((char *)ssid);
#if 0 // RASBURY V12 may not have this issue.
    // Our code adds a NULL terminator.  We can't include it in the count or Supplicant gets confused.
    if (len > 0) len--;
#endif

    return len;
}


/**
 * wpa_driver_wext_set_ssid - Set SSID, SIOCSIWESSID
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @ssid: SSID
 * @ssid_len: Length of SSID (0..32)
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_ssid(void *priv, const u8 *ssid, size_t ssid_len)
{
    wpa_printf(MSG_WARNING, "Calling wext_set_ssid, which calls ASSOCIATE");

    return WlanAssoc((char *) ssid, (int) ssid_len, 0);
}


/**
 * wpa_driver_wext_set_freq - Set frequency/channel, SIOCSIWFREQ
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @freq: Frequency in MHz
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_freq(void *priv, int freq)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_set_freq");
    return 0;
}


static void
wpa_driver_wext_event_wireless_custom(void *ctx, char *custom)
{
	union wpa_event_data data;

	wpa_printf(MSG_MSGDUMP, "WEXT: Custom wireless event: '%s'",
		   custom);

	os_memset(&data, 0, sizeof(data));
	/* Host AP driver */
	if (os_strncmp(custom, "MLME-MICHAELMICFAILURE.indication", 33) == 0) {
		data.michael_mic_failure.unicast =
			os_strstr(custom, " unicast ") != NULL;
		/* TODO: parse parameters(?) */
		wpa_supplicant_event(ctx, EVENT_MICHAEL_MIC_FAILURE, &data);
	} else if (os_strncmp(custom, "ASSOCINFO(ReqIEs=", 17) == 0) {
		char *spos;
		int bytes;

		spos = custom + 17;

		bytes = strspn(spos, "0123456789abcdefABCDEF");
		if (!bytes || (bytes & 1))
			return;
		bytes /= 2;

		data.assoc_info.req_ies = os_malloc(bytes);
		if (data.assoc_info.req_ies == NULL)
			return;

		data.assoc_info.req_ies_len = bytes;
		hexstr2bin(spos, data.assoc_info.req_ies, bytes);

		spos += bytes * 2;

		data.assoc_info.resp_ies = NULL;
		data.assoc_info.resp_ies_len = 0;

		if (os_strncmp(spos, " RespIEs=", 9) == 0) {
			spos += 9;

			bytes = strspn(spos, "0123456789abcdefABCDEF");
			if (!bytes || (bytes & 1))
				goto done;
			bytes /= 2;

			data.assoc_info.resp_ies = os_malloc(bytes);
			if (data.assoc_info.resp_ies == NULL)
				goto done;

			data.assoc_info.resp_ies_len = bytes;
			hexstr2bin(spos, data.assoc_info.resp_ies, bytes);
		}

		wpa_supplicant_event(ctx, EVENT_ASSOCINFO, &data);

	done:
		os_free(data.assoc_info.resp_ies);
		os_free(data.assoc_info.req_ies);
#ifdef CONFIG_PEERKEY
	} else if (os_strncmp(custom, "STKSTART.request=", 17) == 0) {
		if (hwaddr_aton(custom + 17, data.stkstart.peer)) {
			wpa_printf(MSG_DEBUG, "WEXT: unrecognized "
				   "STKSTART.request '%s'", custom + 17);
			return;
		}
		wpa_supplicant_event(ctx, EVENT_STKSTART, &data);
#endif /* CONFIG_PEERKEY */
	}
}

#if 0
static int wpa_driver_wext_event_wireless_michaelmicfailure(
	void *ctx, const char *ev, size_t len)
{
	const struct iw_michaelmicfailure *mic;
	union wpa_event_data data;

	if (len < sizeof(*mic))
		return -1;

	mic = (const struct iw_michaelmicfailure *) ev;

	wpa_printf(MSG_DEBUG, "Michael MIC failure wireless event: "
		   "flags=0x%x src_addr=" MACSTR, mic->flags,
		   MAC2STR(mic->src_addr.sa_data));

	os_memset(&data, 0, sizeof(data));
	data.michael_mic_failure.unicast = !(mic->flags & IW_MICFAILURE_GROUP);

    if (ctx == NULL)
    {
        extern struct wpa_global *gWPAGlobalParams;
        ctx = gWPAGlobalParams->ifaces;
    }

	wpa_supplicant_event(ctx, EVENT_MICHAEL_MIC_FAILURE, &data);

	return 0;
}
#endif


#if 0
static int wpa_driver_wext_event_wireless_pmkidcand(
	struct wpa_driver_wext_data *drv, const char *ev, size_t len)
{
	const struct iw_pmkid_cand *cand;
	union wpa_event_data data;
	const u8 *addr;

	if (len < sizeof(*cand))
		return -1;

	cand = (const struct iw_pmkid_cand *) ev;
	addr = (const u8 *) cand->bssid.sa_data;

	wpa_printf(MSG_DEBUG, "PMKID candidate wireless event: "
		   "flags=0x%x index=%d bssid=" MACSTR, cand->flags,
		   cand->index, MAC2STR(addr));

	os_memset(&data, 0, sizeof(data));
	os_memcpy(data.pmkid_candidate.bssid, addr, ETH_ALEN);
	data.pmkid_candidate.index = cand->index;
	data.pmkid_candidate.preauth = cand->flags & IW_PMKID_CAND_PREAUTH;
	wpa_supplicant_event(drv->ctx, EVENT_PMKID_CANDIDATE, &data);

	return 0;
}
#endif


#if 0
static int wpa_driver_wext_event_wireless_assocreqie(
	struct wpa_driver_wext_data *drv, const char *ev, int len)
{
	if (len < 0)
		return -1;

	wpa_hexdump(MSG_DEBUG, "AssocReq IE wireless event", (const u8 *) ev,
		    len);
	os_free(drv->assoc_req_ies);
	drv->assoc_req_ies = os_malloc(len);
	if (drv->assoc_req_ies == NULL) {
		drv->assoc_req_ies_len = 0;
		return -1;
	}
	os_memcpy(drv->assoc_req_ies, ev, len);
	drv->assoc_req_ies_len = len;

	return 0;
}
#endif


#if 0
static int wpa_driver_wext_event_wireless_assocrespie(
	struct wpa_driver_wext_data *drv, const char *ev, int len)
{
	if (len < 0)
		return -1;

	wpa_hexdump(MSG_DEBUG, "AssocResp IE wireless event", (const u8 *) ev,
		    len);
	os_free(drv->assoc_resp_ies);
	drv->assoc_resp_ies = os_malloc(len);
	if (drv->assoc_resp_ies == NULL) {
		drv->assoc_resp_ies_len = 0;
		return -1;
	}
	os_memcpy(drv->assoc_resp_ies, ev, len);
	drv->assoc_resp_ies_len = len;

	return 0;
}
#endif


#if 0
static void wpa_driver_wext_event_assoc_ies(struct wpa_driver_wext_data *drv)
{
	union wpa_event_data data;

	if (drv->assoc_req_ies == NULL && drv->assoc_resp_ies == NULL)
		return;

	os_memset(&data, 0, sizeof(data));
	if (drv->assoc_req_ies) {
		data.assoc_info.req_ies = drv->assoc_req_ies;
		drv->assoc_req_ies = NULL;
		data.assoc_info.req_ies_len = drv->assoc_req_ies_len;
	}
	if (drv->assoc_resp_ies) {
		data.assoc_info.resp_ies = drv->assoc_resp_ies;
		drv->assoc_resp_ies = NULL;
		data.assoc_info.resp_ies_len = drv->assoc_resp_ies_len;
	}

	wpa_supplicant_event(drv->ctx, EVENT_ASSOCINFO, &data);

	os_free(data.assoc_info.req_ies);
	os_free(data.assoc_info.resp_ies);
}
#endif


void wpa_driver_wext_event_wireless(struct wpa_driver_wext_data *drv,
					   void *ctx, char *data, int len)
{
    MESSAGE eventMsg;

    wpa_printf(MSG_DEBUG, "wpa_driver_wext_event_wireless Data @ %#x, len = %d, supStatus = %d\n", data, len, supplicantThreadStatus);

    if(supplicantThreadStatus < SUP_TERMINATING)    // only schedule events if the main thread is starting or running
    {
        eventMsg.msgType = MSG_WIRELESS_EVENT;
        eventMsg.param3 = os_malloc(len);
        eventMsg.param1 = len;

        memcpy(eventMsg.param3, data, len);

        wpa_printf(MSG_DEBUG, "wpa_driver_wext_event_wireless scheduling event data @ %#x", eventMsg.param3);

        tx_queue_send(&supplicantEventMsgQ, &eventMsg, TX_WAIT_FOREVER);    //-jrs changed from NO_WAIT 3/3/2005

        // signal any blocking socket layer selects to return
        raw_signal();
    }
}
/*
void wpa_driver_wext_event_wireless(struct wpa_driver_wext_data *drv,
					   void *ctx, char *data, int len)
{
	struct iw_event iwe_buf, *iwe = &iwe_buf;
	char *pos, *end, *custom, *buf;

	pos = data;
	end = data + len;

	while (pos + IW_EV_LCP_LEN <= end) {
		// Event data may be unaligned, so make a local, aligned copy
		// before processing. 
		os_memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
		wpa_printf(MSG_DEBUG, "Wireless event: cmd=0x%x len=%d",
			   iwe->cmd, iwe->len);
		if (iwe->len <= IW_EV_LCP_LEN)
			return;

		custom = pos + IW_EV_POINT_LEN;
		if (drv->we_version_compiled > 18 &&
		    (iwe->cmd == IWEVMICHAELMICFAILURE ||
		     iwe->cmd == IWEVCUSTOM ||
		     iwe->cmd == IWEVASSOCREQIE ||
		     iwe->cmd == IWEVASSOCRESPIE ||
		     iwe->cmd == IWEVPMKIDCAND)) {
			// WE-19 removed the pointer from struct iw_point 
			char *dpos = (char *) &iwe_buf.u.data.length;
			int dlen = dpos - (char *) &iwe_buf;
			os_memcpy(dpos, pos + IW_EV_LCP_LEN,
				  sizeof(struct iw_event) - dlen);
		} else {
			os_memcpy(&iwe_buf, pos, sizeof(struct iw_event));
			custom += IW_EV_POINT_OFF;
		}

		switch (iwe->cmd) {
		case SIOCGIWAP:
			wpa_printf(MSG_DEBUG, "Wireless event: new AP: "
				   MACSTR,
				   MAC2STR((u8 *) iwe->u.ap_addr.sa_data));
			if (os_memcmp(iwe->u.ap_addr.sa_data,
				      "\x00\x00\x00\x00\x00\x00", ETH_ALEN) ==
			    0 ||
			    os_memcmp(iwe->u.ap_addr.sa_data,
				      "\x44\x44\x44\x44\x44\x44", ETH_ALEN) ==
			    0) {
				os_free(drv->assoc_req_ies);
				drv->assoc_req_ies = NULL;
				os_free(drv->assoc_resp_ies);
				drv->assoc_resp_ies = NULL;
				wpa_supplicant_event(ctx, EVENT_DISASSOC,
						     NULL);
			
			} else {
				wpa_driver_wext_event_assoc_ies(drv);
				wpa_supplicant_event(ctx, EVENT_ASSOC, NULL);
			}
			break;
		case IWEVMICHAELMICFAILURE:
			wpa_driver_wext_event_wireless_michaelmicfailure(
				ctx, custom, iwe->u.data.length);
			break;
		case IWEVCUSTOM:
			if (custom + iwe->u.data.length > end)
				return;
			buf = os_malloc(iwe->u.data.length + 1);
			if (buf == NULL)
				return;
			os_memcpy(buf, custom, iwe->u.data.length);
			buf[iwe->u.data.length] = '\0';
			wpa_driver_wext_event_wireless_custom(ctx, buf);
			os_free(buf);
			break;
		case SIOCGIWSCAN:
			eloop_cancel_timeout(wpa_driver_wext_scan_timeout,
					     drv, ctx);
			wpa_supplicant_event(ctx, EVENT_SCAN_RESULTS, NULL);
			break;
		case IWEVASSOCREQIE:
			wpa_driver_wext_event_wireless_assocreqie(
				drv, custom, iwe->u.data.length);
			break;
		case IWEVASSOCRESPIE:
			wpa_driver_wext_event_wireless_assocrespie(
				drv, custom, iwe->u.data.length);
			break;
		case IWEVPMKIDCAND:
			wpa_driver_wext_event_wireless_pmkidcand(
				drv, custom, iwe->u.data.length);
			break;
		}

		pos += iwe->len;
	}
}
*/

#if 0
static void wpa_driver_wext_event_link(void *ctx, char *buf, size_t len,
				       int del)
{
	union wpa_event_data event;

	os_memset(&event, 0, sizeof(event));
	if (len > sizeof(event.interface_status.ifname))
		len = sizeof(event.interface_status.ifname) - 1;
	os_memcpy(event.interface_status.ifname, buf, len);
	event.interface_status.ievent = del ? EVENT_INTERFACE_REMOVED :
		EVENT_INTERFACE_ADDED;

	wpa_printf(MSG_DEBUG, "RTM_%sLINK, IFLA_IFNAME: Interface '%s' %s",
		   del ? "DEL" : "NEW",
		   event.interface_status.ifname,
		   del ? "removed" : "added");

	wpa_supplicant_event(ctx, EVENT_INTERFACE_STATUS, &event);
}


static void wpa_driver_wext_event_rtm_newlink(struct wpa_driver_wext_data *drv,
					      void *ctx, struct nlmsghdr *h,
					      size_t len)
{
	struct ifinfomsg *ifi;
	int attrlen, nlmsg_len, rta_len;
	struct rtattr * attr;

	if (len < sizeof(*ifi))
		return;

	ifi = NLMSG_DATA(h);

	if (drv->ifindex != ifi->ifi_index && drv->ifindex2 != ifi->ifi_index)
	{
		wpa_printf(MSG_DEBUG, "Ignore event for foreign ifindex %d",
			   ifi->ifi_index);
		return;
	}
#if 0
	wpa_printf(MSG_DEBUG, "RTM_NEWLINK: operstate=%d ifi_flags=0x%x "
		   "(%s%s%s%s)",
		   drv->operstate, ifi->ifi_flags,
		   (ifi->ifi_flags & IFF_UP) ? "[UP]" : "",
		   (ifi->ifi_flags & IFF_RUNNING) ? "[RUNNING]" : "",
		   (ifi->ifi_flags & IFF_LOWER_UP) ? "[LOWER_UP]" : "",
		   (ifi->ifi_flags & IFF_DORMANT) ? "[DORMANT" : "");
#endif
	/*
	 * Some drivers send the association event before the operup event--in
	 * this case, lifting operstate in wpa_driver_wext_set_operstate()
	 * fails. This will hit us when wpa_supplicant does not need to do
	 * IEEE 802.1X authentication
	 */
#if 0
    if (drv->operstate == 1 &&
	    (ifi->ifi_flags & (IFF_LOWER_UP | IFF_DORMANT)) == IFF_LOWER_UP &&
	    !(ifi->ifi_flags & IFF_RUNNING))
		wpa_driver_wext_send_oper_ifla(drv, -1, IF_OPER_UP);
#endif
	nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));

	attrlen = h->nlmsg_len - nlmsg_len;
	if (attrlen < 0)
		return;

	attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);

	rta_len = RTA_ALIGN(sizeof(struct rtattr));
	while (RTA_OK(attr, attrlen)) {
		if (attr->rta_type == IFLA_WIRELESS) {
			wpa_driver_wext_event_wireless(
				drv, ctx, ((char *) attr) + rta_len,
				attr->rta_len - rta_len);
		} else if (attr->rta_type == IFLA_IFNAME) {
			wpa_driver_wext_event_link(ctx,
						   ((char *) attr) + rta_len,
						   attr->rta_len - rta_len, 0);
		}
		attr = RTA_NEXT(attr, attrlen);
	}
}


static void wpa_driver_wext_event_rtm_dellink(struct wpa_driver_wext_data *drv,
					      void *ctx, struct nlmsghdr *h,
					      size_t len)
{
	struct ifinfomsg *ifi;
	int attrlen, nlmsg_len, rta_len;
	struct rtattr * attr;

	if (len < sizeof(*ifi))
		return;

	ifi = NLMSG_DATA(h);

	nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));

	attrlen = h->nlmsg_len - nlmsg_len;
	if (attrlen < 0)
		return;

	attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);

	rta_len = RTA_ALIGN(sizeof(struct rtattr));
	while (RTA_OK(attr, attrlen)) {
		if (attr->rta_type == IFLA_IFNAME) {
			wpa_driver_wext_event_link(ctx,
						   ((char *) attr) + rta_len,
						   attr->rta_len - rta_len, 1);
		}
		attr = RTA_NEXT(attr, attrlen);
	}
}


static void wpa_driver_wext_event_receive(int sock, void *eloop_ctx,
					  void *sock_ctx)
{
    char * buf;
	int left;
	struct nlmsghdr *h;
	int max_events = 10;

    buf = (char *)os_malloc(WEXT_RECV_BUF_SIZE);
    if (buf == NULL)
        return;

try_again:
    left = recv(sock, buf, WEXT_RECV_BUF_SIZE, 0);
	if (left < 0) {
        wpa_printf(MSG_DEBUG, "wpa_driver_wext_event_receive: recv error %d", left);
        os_free(buf);
		return;
	}

	h = (struct nlmsghdr *) buf;
	while (left >= (int) sizeof(*h)) {
		int len, plen;

		len = h->nlmsg_len;
		plen = len - sizeof(*h);
		if (len > left || plen < 0) {
			wpa_printf(MSG_DEBUG, "Malformed netlink message: "
				   "len=%d left=%d plen=%d",
				   len, left, plen);
			break;
		}

		switch (h->nlmsg_type) {
		case RTM_NEWLINK:
			wpa_driver_wext_event_rtm_newlink(eloop_ctx, sock_ctx,
							  h, plen);
			break;
		case RTM_DELLINK:
			wpa_driver_wext_event_rtm_dellink(eloop_ctx, sock_ctx,
							  h, plen);
			break;
		}

		len = NLMSG_ALIGN(len);
		left -= len;
		h = (struct nlmsghdr *) ((char *) h + len);
	}

	if (left > 0) {
		wpa_printf(MSG_DEBUG, "%d extra bytes in the end of netlink "
			   "message", left);
	}

	if (--max_events > 0) {
		/*
		 * Try to receive all events in one eloop call in order to
		 * limit race condition on cases where AssocInfo event, Assoc
		 * event, and EAPOL frames are received more or less at the
		 * same time. We want to process the event messages first
		 * before starting EAPOL processing.
		 */
		goto try_again;
	}
    os_free(buf);
}
#endif

static int wpa_driver_wext_get_ifflags_ifname(struct wpa_driver_wext_data *drv,
					      const char *ifname, int *flags)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling supplicant function get_ifflags that is needed for WPA2, but is not yet implemented");
    return 0;
}


/**
 * wpa_driver_wext_get_ifflags - Get interface flags (SIOCGIFFLAGS)
 * @drv: driver_wext private data
 * @flags: Pointer to returned flags value
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_get_ifflags(struct wpa_driver_wext_data *drv, int *flags)
{
	return wpa_driver_wext_get_ifflags_ifname(drv, drv->ifname, flags);
}


static int wpa_driver_wext_set_ifflags_ifname(struct wpa_driver_wext_data *drv,
					      const char *ifname, int flags)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling supplicant function set_ifflags that is needed for WPA2, but is not yet implemented");
    return 0;
}


/**
 * wpa_driver_wext_set_ifflags - Set interface flags (SIOCSIFFLAGS)
 * @drv: driver_wext private data
 * @flags: New value for flags
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_ifflags(struct wpa_driver_wext_data *drv, int flags)
{
	return wpa_driver_wext_set_ifflags_ifname(drv, drv->ifname, flags);
}


/**
 * wpa_driver_wext_init - Initialize WE driver interface
 * @ctx: context to be used when calling wpa_supplicant functions,
 * e.g., wpa_supplicant_event()
 * @ifname: interface name, e.g., wlan0
 * Returns: Pointer to private data, %NULL on failure
 */
void * wpa_driver_wext_init(void *ctx, const char *ifname)
{
	//int s;
    //int flags;
	//struct sockaddr_nl local;
	struct wpa_driver_wext_data *drv;

	drv = os_zalloc(sizeof(*drv));
	if (drv == NULL)
		return NULL;
	drv->ctx = ctx;
	os_strncpy(drv->ifname, ifname, sizeof(drv->ifname));
/*
	drv->ioctl_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (drv->ioctl_sock < 0) {
		perror("socket(PF_INET,SOCK_DGRAM)");
		os_free(drv);
		return NULL;
	}
*/
    drv->ioctl_sock = -1;
#if 0
    s = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (s < 0) {
		perror("socket(PF_NETLINK,SOCK_RAW,NETLINK_ROUTE)");
//		close(drv->ioctl_sock);
		os_free(drv);
		return NULL;
	}

    setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname));

#if 0
    os_memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;
	if (bind(s, (struct sockaddr_ll*) &local, sizeof(local)) < 0) {
		perror("bind(netlink)");
		close(s);
//		close(drv->ioctl_sock);
		os_free(drv);
		return NULL;
	}
#endif

	eloop_register_read_sock(s, wpa_driver_wext_event_receive, drv, ctx);
	drv->event_sock = s;
#else
    drv->event_sock = -1;
#endif
	drv->mlme_sock = -1;

	/*
	 * Make sure that the driver does not have any obsolete PMKID entries.
	 */
	wpa_driver_wext_flush_pmkid(drv);

	if (wpa_driver_wext_set_mode(drv, 0) < 0) {
		printf("Could not configure driver to use managed mode\n");
	}
#if 0
	if (wpa_driver_wext_get_ifflags(drv, &flags) != 0 ||
	    wpa_driver_wext_set_ifflags(drv, flags | IFF_UP) != 0) {
		printf("Could not set interface '%s' UP\n", drv->ifname);
	}
#endif
	wpa_driver_wext_get_range(drv);

	drv->ifindex = if_nametoindex(drv->ifname);

	if (os_strncmp(ifname, "wlan", 4) == 0) {
		/*
		 * Host AP driver may use both wlan# and wifi# interface in
		 * wireless events. Since some of the versions included WE-18
		 * support, let's add the alternative ifindex also from
		 * driver_wext.c for the time being. This may be removed at
		 * some point once it is believed that old versions of the
		 * driver are not in use anymore.
		 */
		char ifname2[IFNAMSIZ + 1];
		os_strncpy(ifname2, ifname, sizeof(ifname2));
		os_memcpy(ifname2, "wifi", 4);
		wpa_driver_wext_alternative_ifindex(drv, ifname2);
	}

	wpa_driver_wext_send_oper_ifla(drv, 1, IF_OPER_DORMANT);

	return drv;
}


/**
 * wpa_driver_wext_deinit - Deinitialize WE driver interface
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 *
 * Shut down driver interface and processing of driver events. Free
 * private data buffer if one was allocated in wpa_driver_wext_init().
 */
void wpa_driver_wext_deinit(void *priv)
{
	struct wpa_driver_wext_data *drv = priv;
//	int flags;

	eloop_cancel_timeout(wpa_driver_wext_scan_timeout, drv, drv->ctx);

	/*
	 * Clear possibly configured driver parameters in order to make it
	 * easier to use the driver after wpa_supplicant has been terminated.
	 */
#if 0
    wpa_driver_wext_set_bssid(drv, (u8 *) "\x00\x00\x00\x00\x00\x00");
#else
    char test_addr[ETH_ALEN] = {0x00,0x00,0x00,0x00,0x00,0x00};
    wpa_driver_wext_set_bssid(drv, (u8 *) test_addr);
#endif
	wpa_driver_wext_send_oper_ifla(priv, 0, IF_OPER_UP);

    if (drv->event_sock >= 0)
        eloop_unregister_read_sock(drv->event_sock);
	if (drv->mlme_sock >= 0)
		eloop_unregister_read_sock(drv->mlme_sock);

//	This caused bug #4204 and #4092.
//	if (wpa_driver_wext_get_ifflags(drv, &flags) == 0)
//		(void) wpa_driver_wext_set_ifflags(drv, flags & ~IFF_UP);

#ifdef CONFIG_CLIENT_MLME
	if (drv->mlmedev[0] &&
	    wpa_driver_wext_get_ifflags_ifname(drv, drv->mlmedev, &flags) == 0)
		(void) wpa_driver_wext_set_ifflags_ifname(drv, drv->mlmedev,
							  flags & ~IFF_UP);
#endif /* CONFIG_CLIENT_MLME */

    if (drv->event_sock >= 0)
		raw_close_socket(drv->event_sock);
//	close(drv->ioctl_sock);
	if (drv->mlme_sock >= 0)
		close(drv->mlme_sock);
	os_free(drv->assoc_req_ies);
	os_free(drv->assoc_resp_ies);
	os_free(drv);
}


/**
 * wpa_driver_wext_scan_timeout - Scan timeout to report scan completion
 * @eloop_ctx: Unused
 * @timeout_ctx: ctx argument given to wpa_driver_wext_init()
 *
 * This function can be used as registered timeout when starting a scan to
 * generate a scan completed event if the driver does not report this.
 */
void wpa_driver_wext_scan_timeout(void *eloop_ctx, void *timeout_ctx)
{
	wpa_printf(MSG_DEBUG, "Scan timeout - try to get results");
	wpa_supplicant_event(timeout_ctx, EVENT_SCAN_RESULTS, NULL);
}


/**
 * wpa_driver_wext_scan - Request the driver to initiate scan
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @ssid: Specific SSID to scan for (ProbeReq) or %NULL to scan for
 *	all SSIDs (either active scan with broadcast SSID or passive
 *	scan
 * @ssid_len: Length of the SSID
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_scan(void *priv, const u8 *ssid, size_t ssid_len)
{
    int ret = 0;

    ret = WlanShimCSetScanWithSsid((char *) ssid, (int) ssid_len);
    /* Not all drivers generate "scan completed" wireless event, so try to
     * read results after a timeout. */
//    eloop_register_timeout(3, 0, wpa_driver_wext_scan_timeout, NULL, NULL);

    return ret;
}


/* Compare function for sorting scan results. Return >0 if @b is considered
 * better. */
#if 0
static int wpa_scan_result_compar(const void *a, const void *b)
{
	const struct wpa_scan_result *wa = a;
	const struct wpa_scan_result *wb = b;

	/* WPA/WPA2 support preferred */
	if ((wb->wpa_ie_len || wb->rsn_ie_len) &&
	    !(wa->wpa_ie_len || wa->rsn_ie_len))
		return 1;
	if (!(wb->wpa_ie_len || wb->rsn_ie_len) &&
	    (wa->wpa_ie_len || wa->rsn_ie_len))
		return -1;

	/* privacy support preferred */
	if ((wa->caps & IEEE80211_CAP_PRIVACY) == 0 &&
	    (wb->caps & IEEE80211_CAP_PRIVACY))
		return 1;
	if ((wa->caps & IEEE80211_CAP_PRIVACY) &&
	    (wb->caps & IEEE80211_CAP_PRIVACY) == 0)
		return -1;

	/* best/max rate preferred if signal level close enough XXX */
	if (wa->maxrate != wb->maxrate && abs(wb->level - wa->level) < 5)
		return wb->maxrate - wa->maxrate;

	/* use freq for channel preference */

	/* all things being equal, use signal level; if signal levels are
	 * identical, use quality values since some drivers may only report
	 * that value and leave the signal level zero */
	if (wb->level == wa->level)
		return wb->qual - wa->qual;
	return wb->level - wa->level;
}
#endif


/**
 * wpa_driver_wext_get_scan_results - Fetch the latest scan results
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @results: Pointer to buffer for scan results
 * @max_size: Maximum number of entries (buffer size)
 * Returns: Number of scan result entries used on success, -1 on
 * failure
 *
 * If scan results include more than max_size BSSes, max_size will be
 * returned and the remaining entries will not be included in the
 * buffer.
 */
int wpa_driver_wext_get_scan_results(void *priv,
				     struct wpa_scan_result *results,
				     size_t max_size)
{
    struct SCAN_SSID_IE *scanVals;
    int i, numAPs;
    
//    wpa_printf(MSG_DEBUG, "SUP: wext driver -- Getting scan results (max_size: %d, results: %08x)", max_size, results);

    scanVals = os_zalloc(sizeof(struct SCAN_SSID_IE) * max_size);
    memset(results, 0, sizeof(struct wpa_scan_result) * max_size);

    numAPs = WlanShimCGetScanResult(scanVals, max_size);
    if (numAPs < 0)
    {
        wpa_printf(MSG_ERROR, "SUP: wext driver failed to scan APs");
    }
    else
    {
        for (i = 0; i < numAPs; i++)
        { 
            memcpy(results[i].bssid, scanVals[i].BSSID, 6);
            memcpy(results[i].ssid, scanVals[i].SSID, scanVals[i].ssid_len);
            results[i].ssid_len = scanVals[i].ssid_len;
            if (scanVals[i].WPA_IE[0] == RSN_INFO_ELEM)
            {
                memcpy(results[i].rsn_ie, scanVals[i].WPA_IE, scanVals[i].wpa_ie_len);
                results[i].rsn_ie_len = scanVals[i].wpa_ie_len;
            }
            else
            {
                memcpy(results[i].wpa_ie, scanVals[i].WPA_IE, scanVals[i].wpa_ie_len);
                results[i].wpa_ie_len = scanVals[i].wpa_ie_len;
            }
            results[i].caps = scanVals[i].Capability;
        }
    }

    os_free(scanVals);

//    wpa_printf(MSG_DEBUG, "SUP: wext driver -- Getting scan results (exit %d)", numAPs);
    return numAPs;    
}


static int wpa_driver_wext_get_range(void *priv)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling supplicant function wext_get_range that is needed for WPA2, but is not yet implemented");
    return 0;
}


static int wpa_driver_wext_set_wpa(void *priv, int enabled)
{
	struct wpa_driver_wext_data *drv = priv;
	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);

	return wpa_driver_wext_set_auth_param(drv, IW_AUTH_WPA_ENABLED,
					      enabled);
}


#if 0
static int wpa_driver_wext_set_key_ext(void *priv, wpa_alg alg,
				       const u8 *addr, int key_idx,
				       int set_tx, const u8 *seq,
				       size_t seq_len,
				       const u8 *key, size_t key_len)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_set_key_ext");
    return 0;
}
#endif


/**
 * wpa_driver_wext_set_key - Configure encryption key
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @priv: Private driver interface data
 * @alg: Encryption algorithm (%WPA_ALG_NONE, %WPA_ALG_WEP,
 *	%WPA_ALG_TKIP, %WPA_ALG_CCMP); %WPA_ALG_NONE clears the key.
 * @addr: Address of the peer STA or ff:ff:ff:ff:ff:ff for
 *	broadcast/default keys
 * @key_idx: key index (0..3), usually 0 for unicast keys
 * @set_tx: Configure this key as the default Tx key (only used when
 *	driver does not support separate unicast/individual key
 * @seq: Sequence number/packet number, seq_len octets, the next
 *	packet number to be used for in replay protection; configured
 *	for Rx keys (in most cases, this is only used with broadcast
 *	keys and set to zero for unicast keys)
 * @seq_len: Length of the seq, depends on the algorithm:
 *	TKIP: 6 octets, CCMP: 6 octets
 * @key: Key buffer; TKIP: 16-byte temporal key, 8-byte Tx Mic key,
 *	8-byte Rx Mic Key
 * @key_len: Length of the key buffer in octets (WEP: 5 or 13,
 *	TKIP: 32, CCMP: 16)
 * Returns: 0 on success, -1 on failure
 *
 * This function uses SIOCSIWENCODEEXT by default, but tries to use
 * SIOCSIWENCODE if the extended ioctl fails when configuring a WEP key.
 */
int wpa_driver_wext_set_key(void *priv, wpa_alg alg,
			    const u8 *addr, int key_idx,
			    int set_tx, const u8 *seq, size_t seq_len,
			    const u8 *key, size_t key_len)
{
    //WLAN_802_11_KEY *param;
    struct iw_encode_ext *param;
    u8 *buf;
    size_t blen;
    int ret = 0; //-jrs, flag = 0 ;
//-jrs    char *alg_name;

    /* Marvell driver does not support setting NULL keys.   */
    if (alg == WPA_ALG_NONE)
        if (!key_len) {
            return -1;
        }

    if (seq_len > 8)
        return -2;
    blen = sizeof(*param) + key_len;
    buf = (u8 *)os_malloc(blen);
    if (buf == NULL)
        return -1;
    memset(buf, 0, blen);
#if 0
    param = (WLAN_802_11_KEY *) buf;

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


static int wpa_driver_wext_set_countermeasures(void *priv,
					       int enabled)
{
	struct wpa_driver_wext_data *drv = priv;
	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
	return wpa_driver_wext_set_auth_param(drv,
					      IW_AUTH_TKIP_COUNTERMEASURES,
					      enabled);
}


static int wpa_driver_wext_set_drop_unencrypted(void *priv,
						int enabled)
{
	struct wpa_driver_wext_data *drv = priv;
	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
	drv->use_crypt = enabled;
	return wpa_driver_wext_set_auth_param(drv, IW_AUTH_DROP_UNENCRYPTED,
					      enabled);
}


#if 0
static int wpa_driver_wext_mlme(struct wpa_driver_wext_data *drv,
				const u8 *addr, int cmd, int reason_code)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_mlme");
    return 0;
}
#endif


static int wpa_driver_wext_deauthenticate(void *priv, const u8 *addr,
					  int reason_code)
{
    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    return WlanShimCDeauth();
}


static int wpa_driver_wext_disassociate(void *priv, const u8 *addr,
					int reason_code)
{
    wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);
    return WlanShimCDeauth();
}


static int wpa_driver_wext_set_gen_ie(void *priv, const u8 *ie,
				      size_t ie_len)
{
    return WlanShimCSetWpaIE((char *)ie, ie_len);
}


static int wpa_driver_wext_cipher2wext(int cipher)
{
	switch (cipher) {
	case CIPHER_NONE:
		return IW_AUTH_CIPHER_NONE;
	case CIPHER_WEP40:
		return IW_AUTH_CIPHER_WEP40;
	case CIPHER_TKIP:
		return IW_AUTH_CIPHER_TKIP;
	case CIPHER_CCMP:
		return IW_AUTH_CIPHER_CCMP;
	case CIPHER_WEP104:
		return IW_AUTH_CIPHER_WEP104;
	default:
		return 0;
	}

    return 0;
}


static int wpa_driver_wext_keymgmt2wext(int keymgmt)
{
	switch (keymgmt) {
	case KEY_MGMT_802_1X:
		return IW_AUTH_KEY_MGMT_802_1X;
	case KEY_MGMT_PSK:
		return IW_AUTH_KEY_MGMT_PSK;
	default:
		return 0;
	}

    return 0;
}


static int
wpa_driver_wext_auth_alg_fallback(struct wpa_driver_wext_data *drv,
				  struct wpa_driver_associate_params *params)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling wext_fallback");
    return 0;
}


static int
wpa_driver_wext_associate(void *priv,
			  struct wpa_driver_associate_params *params)
{
	struct wpa_driver_wext_data *drv = priv;
	int ret = 0;
	int allow_unencrypted_eapol;
	int value;

	wpa_printf(MSG_DEBUG, "%s", __FUNCTION__);

	/*
	 * If the driver did not support SIOCSIWAUTH, fallback to
	 * SIOCSIWENCODE here.
	 */
	if (drv->auth_alg_fallback &&
	    wpa_driver_wext_auth_alg_fallback(drv, params) < 0)
		ret = -1;

	if (!params->bssid &&
	    wpa_driver_wext_set_bssid(drv, NULL) < 0)
		ret = -1;

	if (wpa_driver_wext_set_mode(drv, params->mode) < 0)
		ret = -1;
	/* TODO: should consider getting wpa version and cipher/key_mgmt suites
	 * from configuration, not from here, where only the selected suite is
	 * available */
	if (wpa_driver_wext_set_gen_ie(drv, params->wpa_ie, params->wpa_ie_len)
	    < 0)
		ret = -1;
	if (params->wpa_ie == NULL || params->wpa_ie_len == 0)
		value = IW_AUTH_WPA_VERSION_DISABLED;
	else if (params->wpa_ie[0] == RSN_INFO_ELEM)
        value = IW_AUTH_WPA_VERSION_WPA2;
	else
		value = IW_AUTH_WPA_VERSION_WPA;
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_WPA_VERSION, value) < 0)
		ret = -1;
	value = wpa_driver_wext_cipher2wext(params->pairwise_suite);
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_CIPHER_PAIRWISE, value) < 0)
		ret = -1;
	value = wpa_driver_wext_cipher2wext(params->group_suite);
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_CIPHER_GROUP, value) < 0)
		ret = -1;
	value = wpa_driver_wext_keymgmt2wext(params->key_mgmt_suite);
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_KEY_MGMT, value) < 0)
		ret = -1;
	value = params->key_mgmt_suite != KEY_MGMT_NONE ||
		params->pairwise_suite != CIPHER_NONE ||
		params->group_suite != CIPHER_NONE ||
		params->wpa_ie_len;
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_PRIVACY_INVOKED, value) < 0)
		ret = -1;

	/* Allow unencrypted EAPOL messages even if pairwise keys are set when
	 * not using WPA. IEEE 802.1X specifies that these frames are not
	 * encrypted, but WPA encrypts them when pairwise keys are in use. */
	if (params->key_mgmt_suite == KEY_MGMT_802_1X ||
	    params->key_mgmt_suite == KEY_MGMT_PSK)
		allow_unencrypted_eapol = 0;
	else
		allow_unencrypted_eapol = 1;
	
	if (wpa_driver_wext_set_auth_param(drv,
					   IW_AUTH_RX_UNENCRYPTED_EAPOL,
					   allow_unencrypted_eapol) < 0)
		ret = -1;
	if (params->freq && wpa_driver_wext_set_freq(drv, params->freq) < 0)
		ret = -1;
	if (wpa_driver_wext_set_ssid(drv, params->ssid, params->ssid_len) < 0)
		ret = -1;
	if (params->bssid &&
	    wpa_driver_wext_set_bssid(drv, params->bssid) < 0)
		ret = -1;

	return ret;
}


static int wpa_driver_wext_set_auth_alg(void *priv, int auth_alg)
{
	struct wpa_driver_wext_data *drv = priv;
	int algs = 0, res;

	if (auth_alg & AUTH_ALG_OPEN_SYSTEM)
		algs |= IW_AUTH_ALG_OPEN_SYSTEM;
	if (auth_alg & AUTH_ALG_SHARED_KEY)
		algs |= IW_AUTH_ALG_SHARED_KEY;
	if (auth_alg & AUTH_ALG_LEAP)
		algs |= IW_AUTH_ALG_LEAP;
	if (algs == 0) {
		/* at least one algorithm should be set */
		algs = IW_AUTH_ALG_OPEN_SYSTEM;
	}

	res = wpa_driver_wext_set_auth_param(drv, IW_AUTH_80211_AUTH_ALG,
					     algs);
	drv->auth_alg_fallback = res == -2;
	return res;
}


/**
 * wpa_driver_wext_set_mode - Set wireless mode (infra/adhoc), SIOCSIWMODE
 * @priv: Pointer to private wext data from wpa_driver_wext_init()
 * @mode: 0 = infra/BSS (associate with an AP), 1 = adhoc/IBSS
 * Returns: 0 on success, -1 on failure
 */
int wpa_driver_wext_set_mode(void *priv, int mode)
{
    wifi_intf_comm_mode_t wlanMode;

    if (mode == 1) 
        wlanMode = AD_HOC;
    else           
        wlanMode = INFRASTRUCTURE;

    return WlanSetMode(wlanMode);
}


static int wpa_driver_wext_pmksa(struct wpa_driver_wext_data *drv,
				 u32 cmd, const u8 *bssid, const u8 *pmkid)
{
//    ASSERT("Presumed Unnecessary" == 0);
    wpa_printf(MSG_WARNING, "Calling supplicant function wext_pmksa that is needed for WPA2, but is not yet implemented");
    return 0;
}


#if 0
static int wpa_driver_wext_add_pmkid(void *priv, const u8 *bssid,
				     const u8 *pmkid)
{
	struct wpa_driver_wext_data *drv = priv;
	return wpa_driver_wext_pmksa(drv, IW_PMKSA_ADD, bssid, pmkid);
}
#endif


#if 0
static int wpa_driver_wext_remove_pmkid(void *priv, const u8 *bssid,
		 			const u8 *pmkid)
{
	struct wpa_driver_wext_data *drv = priv;
	return wpa_driver_wext_pmksa(drv, IW_PMKSA_REMOVE, bssid, pmkid);
}
#endif


static int wpa_driver_wext_flush_pmkid(void *priv)
{
	struct wpa_driver_wext_data *drv = priv;
	return wpa_driver_wext_pmksa(drv, IW_PMKSA_FLUSH, NULL, NULL);
}


static int wpa_driver_wext_get_capa(void *priv, struct wpa_driver_capa *capa)
{
	struct wpa_driver_wext_data *drv = priv;
	if (!drv->has_capability)
		return -1;
	os_memcpy(capa, &drv->capa, sizeof(*capa));
	return 0;
}


int wpa_driver_wext_alternative_ifindex(struct wpa_driver_wext_data *drv,
					const char *ifname)
{
	if (ifname == NULL) {
		drv->ifindex2 = -1;
		return 0;
	}

	drv->ifindex2 = if_nametoindex(ifname);
	if (drv->ifindex2 <= 0)
		return -1;

	wpa_printf(MSG_DEBUG, "Added alternative ifindex %d (%s) for "
		   "wireless events", drv->ifindex2, ifname);

	return 0;
}


int wpa_driver_wext_set_operstate(void *priv, int state)
{
	struct wpa_driver_wext_data *drv = priv;

	wpa_printf(MSG_DEBUG, "%s: operstate %d->%d (%s)",
		   __func__, drv->operstate, state, state ? "UP" : "DORMANT");
	drv->operstate = state;
	return wpa_driver_wext_send_oper_ifla(
		drv, -1, state ? IF_OPER_UP : IF_OPER_DORMANT);
}


static int wpa_driver_wext_set_param(void *priv, const char *param)
{
#ifdef CONFIG_CLIENT_MLME
	struct wpa_driver_wext_data *drv = priv;
	const char *pos, *pos2;
	size_t len;

	if (param == NULL)
		return 0;

	wpa_printf(MSG_DEBUG, "%s: param='%s'", __func__, param);

	pos = os_strstr(param, "mlmedev=");
	if (pos) {
		pos += 8;
		pos2 = os_strchr(pos, ' ');
		if (pos2)
			len = pos2 - pos;
		else
			len = os_strlen(pos);
		if (len + 1 > sizeof(drv->mlmedev))
			return -1;
		os_memcpy(drv->mlmedev, pos, len);
		drv->mlmedev[len] = '\0';
		wpa_printf(MSG_DEBUG, "WEXT: Using user space MLME with "
			   "mlmedev='%s'", drv->mlmedev);
		drv->capa.flags |= WPA_DRIVER_FLAGS_USER_SPACE_MLME;

		drv->mlme_sock = wpa_driver_wext_open_mlme(drv);
		if (drv->mlme_sock < 0)
			return -1;
	}
#endif /* CONFIG_CLIENT_MLME */

	return 0;
}


int wpa_driver_wext_get_version(struct wpa_driver_wext_data *drv)
{
	return drv->we_version_compiled;
}

void do_wpa_driver_wext_event_wireless(void *ctx, char *data, int len)
{
    struct iw_event *iwe;
    char *pos, *end, *custom, *buf;

    pos = data;
    end = data + len;

    wpa_printf(MSG_DEBUG, "do_wpa_driver_wext_event_wireless processing event data @ %#x, context = %#x", data, ctx);

    while (pos + IW_EV_LCP_LEN <= end) {
        iwe = (struct iw_event *) pos;
        wpa_printf(MSG_DEBUG, "Wireless event: cmd=0x%x len=%d",
               iwe->cmd, iwe->len);
        if (iwe->len <= IW_EV_LCP_LEN)
            return;
        switch (iwe->cmd) {
        case SIOCGIWAP:
            wpa_printf(MSG_DEBUG, "Wireless event: new AP: "
                   MACSTR,
                   MAC2STR((u8 *) iwe->u.ap_addr.sa_data));
#if 0
            if (memcmp(iwe->u.ap_addr.sa_data,
                   "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0
                ||
                memcmp(iwe->u.ap_addr.sa_data,
                   "\x44\x44\x44\x44\x44\x44", ETH_ALEN) == 0)
#else
            char test_addr1[ETH_ALEN] = {0x00,0x00,0x00,0x00,0x00,0x00};
            char test_addr2[ETH_ALEN] = {0x44,0x44,0x44,0x44,0x44,0x44};

            if (memcmp(iwe->u.ap_addr.sa_data, test_addr1, ETH_ALEN) == 0
                ||
                memcmp(iwe->u.ap_addr.sa_data, test_addr2, ETH_ALEN) == 0)
#endif
            {
                wpa_supplicant_event(ctx, EVENT_DISASSOC,
                             NULL);
            }
            else
            {
                wpa_supplicant_event(ctx, EVENT_ASSOC, NULL);
            }
            break;
        case IWEVCUSTOM:
            custom = pos + IW_EV_POINT_LEN;
            if (custom + iwe->u.data.length > end)
                return;
            buf = (char*)os_malloc(iwe->u.data.length + 1);
            if (buf == NULL)
                return;
            memcpy(buf, custom, iwe->u.data.length);
            buf[iwe->u.data.length] = '\0';
            wpa_driver_wext_event_wireless_custom(ctx, buf);
            os_free(buf);
            break;
        case SIOCGIWSCAN:
            eloop_cancel_timeout(wpa_driver_wext_scan_timeout,
                         NULL, ctx);
            wpa_supplicant_event(ctx, EVENT_SCAN_RESULTS, NULL);
            break;
        case IWEVMICHAELMICFAILURE:
            wpa_supplicant_event(ctx, EVENT_MICHAEL_MIC_FAILURE, (union wpa_event_data *)&(iwe->u));
            break;
        default:
            wpa_printf(MSG_DEBUG, "Wireless event: Ignoring unexpected event");
            break;
        }

        pos += iwe->len;
    }
}

const struct wpa_driver_ops wpa_driver_wext_ops = {
	.name = "wext",
	.desc = "Linux wireless extensions (generic)",
	.get_bssid = wpa_driver_wext_get_bssid,
	.get_ssid = wpa_driver_wext_get_ssid,
	.set_wpa = wpa_driver_wext_set_wpa,
	.set_key = wpa_driver_wext_set_key,
	.set_countermeasures = wpa_driver_wext_set_countermeasures,
	.set_drop_unencrypted = wpa_driver_wext_set_drop_unencrypted,
	.scan = wpa_driver_wext_scan,
	.get_scan_results = wpa_driver_wext_get_scan_results,
	.deauthenticate = wpa_driver_wext_deauthenticate,
	.disassociate = wpa_driver_wext_disassociate,
	.associate = wpa_driver_wext_associate,
	.set_auth_alg = wpa_driver_wext_set_auth_alg,
	.init = wpa_driver_wext_init,
	.deinit = wpa_driver_wext_deinit,
	.set_param = wpa_driver_wext_set_param,
    // TODO: put back for WPA-2
//	.add_pmkid = wpa_driver_wext_add_pmkid,
//	.remove_pmkid = wpa_driver_wext_remove_pmkid,
//	.flush_pmkid = wpa_driver_wext_flush_pmkid,
	.get_capa = wpa_driver_wext_get_capa,
	.set_operstate = wpa_driver_wext_set_operstate,
#ifdef CONFIG_CLIENT_MLME
	.get_hw_feature_data = wpa_driver_wext_get_hw_feature_data,
	.set_channel = wpa_driver_wext_set_channel,
	.set_ssid = wpa_driver_wext_set_ssid,
	.set_bssid = wpa_driver_wext_set_bssid,
	.send_mlme = wpa_driver_wext_send_mlme,
	.mlme_add_sta = wpa_driver_wext_mlme_add_sta,
	.mlme_remove_sta = wpa_driver_wext_mlme_remove_sta,
#endif /* CONFIG_CLIENT_MLME */
};
