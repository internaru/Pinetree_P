/****************************************************************************** 
 *
 *
 * ============================================================================
 * Copyright (c) 2004-2013 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *	Wireless configuration and status manager
 */

/**************************************************************************************
 * Wireless system interface detailed description
 *
 * The wireless protocol stack operates independently from the IP networking layer.
 * It interfaces with the network layer through a MAC level interface implemented in
 * the wirelessDrvr.c module.  At the same time the stack can be directed to perform
 * various non-network related actions such as scanning for access points in range via
 * the interface exported in wlanInterface.h
 *
 **************************************************************************************
 * Boot Time Initialization:
 * The stack's boot time configuration is stored in the datastore.  At boot time the
 * WirelessInit routine is called by the system initialization code.  If the stack is
 * marked enabled in the datastore then the WlanStartup routine is called to start the
 * protocol stack, upload the FW to the wireless HW module, apply the initial wireless
 * settings and start the associated wireless tasks such as WPS and WPA.
 *
 * If the stack is disabled at boot time then the IO level shutdown routine is called
 * to guarantee that the wireless HW module is being held in reset to minimize power
 * consumption.
 *
 **************************************************************************************
 * Network Interface
 *
 * The interface to the TCPIP protocol stack is implemented in the WirelessDrvr.c file.
 * This essentially provides a MAC level interface driver to the IP stack.  Enabling
 * and disabling the MAC interface is controlled by iptask.c.  IPTask polls the link
 * status through the WirelessDrvr interface.  Link status is returned as disconnected
 * when the wireless stack is disabled, when the wireless stack is not associated to an
 * access point or adhoc network and when the wireless stack is in the process of
 * authenticating.  Once the wireless stack is completely associated and authenticated
 * it will return valid link status.
 *
 * When IPTask detects that link status is good it will bring up the MAC level interface.
 * This is done differently at system init time vs later associations.  If an association
 * happens at system init time the TCPIP interface is brought up right a way.  If
 * association is lost the TCPIP interface remains enabled and IP task starts polling to
 * see if link status has been recovered.  Once the link is re-established IPTask disables
 * and then re-enables the interface.  This complicates the startup/shutdown of the
 * wireless stack as described below.
 *
 * Once the interface is operational it operates in a very simple TX/RX mode.  Tx packets
 * are passed to the wireless stack for transmission.  Once the packet has been sent
 * it is acknowledged back to the TCPIP stack through the driver interface.  One issue to
 * note is that the wireless MAC driver queues packets internally and will pass only one
 * packet at a time to the wireless protocol stack.  Only when that packet has been sent
 * and acknowledged will another packet be passed to the protocol stack.  This was done
 * to simplify the interface and debugging, but it is a potential performance impact and
 * it also has the side-effect of defeating the WMM queuing in the protocol stack.  I 
 * believe that WMM will not operate correctly unless we start queueing multiple sends
 * in the wireless stack.
 *
 * In the RX path the wireless protocol stack requests and RX buffer from the TCPIP stack
 * via the wireless MAC driver interface.  When a packet is received it is stored into
 * this buffer and passed back to the TCPIP stack through the wireless MAC driver interface.
 *
 **************************************************************************************
 * Runtime Startup/Shutdown
 * 
 * The base wireless protocol stack may be enabled or disabled at any time by a variety
 * of methods such as the control panel, web interface, debug serial interface, etc.
 * All of these methods start or stop the stack by calling the WlanStartup and 
 * WlanShutdown functions.  Starting and stopping the stack has somewhat complex
 * requirements that are described here.
 *
 * Startup:
 * When the stack is started the low/level I/O driver is initiaized first.  Once this is
 * complete the higher layer protocol stack is started.  The higher level protocol stack
 * registers with the I/O driver and then checks to see if the wireless module has
 * operations FW or if it is waiting for a FW upload.  If the wireless module is waiting
 * for FW the protocol stack uploads the wireless module FW.  This is a two stage process.
 * First a small bootloader called the "helper" is uploaded.  Then the main FW is uploaded.
 * Once the wireless module is running its FW the protocol stack completes its
 * initialization including starting several control threads and returns.
 * 
 * At this point the wireless MAC driver is notified that it can open an interface to the
 * protocol stack, the current wireless settings are applied, and the associated wireless
 * tasks such as WPS and WPA are started.  These are independent threads.
 *
 * Shutdown:
 * When the stack is shutdown the process proceeds from high layer to low layer.  First
 * the stack is marked as disabled so that IPTask will detect that link status has been
 * lost in its polling.  Then the MAC driver is notified that the wireless stack is being
 * shut down.  At this point it drops its reference to the wireless stack interface setting
 * it to NULL.  This causes the wireless MAC driver to return failure to any further
 * requests from the TCPIP stack.  This is implementationally complex and was done because 
 * IPTask does not bring down the MAC interface when link status is lost.  I believe
 * that it would be better if IPTask brought down the interface when link status is lost,
 * but I do not understand all the implications of making that change and at this point in
 * the program it is too high risk to make that change.
 *
 * Once the MAC level interface has been notified of the shutdown then the shutdown routine
 * proceeds to notify the associated wireless task (WPS, WPA, etc) threads to exit.  
 * This is done in * a synchronous fashion.  The routine waits for each thread to be 
 * confirmed to be halted before shutting down the next thread.
 * 
 * The routine then shuts down the high level wireless protocol stack.  The protocol stack
 * must free all TX and RX buffers that it currently has ownership of back to the TCPIP
 * stack by notifying the MAC driver of completion.  It then halts all internal threads
 * and closes its interface to the low-level I/O driver.  Finally the low-level I/O driver
 * is shut down.  As a part of this shutdown process it sets the HW to hold the wireless
 * module in reset to minimize it's power consumption.
 *
 *
 **************************************************************************************
 */


#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <tx_api.h>
#include <SysInit.h>
#include <dprintf.h>
#include <logger.h>

#include <agMessage.h>

#include "os_headers.h"

#undef DEBUG_LEVEL1 // turn off defines that conflict with main system defines in this file
#include "include.h"
#include "mlan_decl.h"
#include "mlan_join.h"
#include "mlan_fw.h"
#include "mlan_util.h"
#include "mlan_main.h"

#include "os_timers.h"
#include "mlan_wmm.h"
#include "os_net_device.h"
#include "wlandrvexports.h"

#undef ASSERT // get rid of the assert definition in wlan_defs.h and use the one in lassert
#include "lassert.h"

#include "wlanInterface.h"
#include "wps_msg.h"
#include "mlan_ieee.h"
#include "wifi_intf.h"
#include "wifi_intf_vars.h"
#include "wifi_intf_config.h"
#include "wps_events.h"
#include "wifi_config.h"
#include "wifidirect_api.h"
#include "uap_api.h"
#include "moal_uap.h"
#include "wfd_api.h"
#include "uuid.h"
#include "platform_api.h"
#include "delay_api.h"
 
#ifdef HAVE_WIFI_CERT_TEST 
#include "sigma_interface.h"
#endif

#if 0
#include "wpa.h"
#include "config_ssid.h"
#else

#define WPA_KEY_MGMT_IEEE8021X  0x1
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA   0x3

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	int num_pmkid;
	const u8 *pmkid;
	int mgmt_group_cipher;
};

extern int wpa_parse_wpa_ie(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);
#endif
//#include "pwr_mgr_API.h"

#include <debug.h>
#ifdef HAVE_CMD
   #include "cmd_proc_api.h"
#endif

void startWPSThread(void);
void stopWPSThread(void);

void WlanWirelessSetRadioPresent(void);
void WlanWirelessHWActive(bool UsingWireless);

int mapping_chan_to_freq(int chan);

#define PASSWORD_ID_NOT_PRESENT           0xFFFF
#define DEVICE_PASSWORD_ID_PIN            0x0000
#define DEVICE_PASSWORD_PUSH_BUTTON       0x0004
extern UINT16 wps_probe_response_device_password_id_parser(uint8_t *, size_t);

#define WCDBG (DBG_LOUD | DBG_OUTPUT)
void wireless_register_cmds(void);
BOOL WlanIsWirelessInstalled(void);
void wConfigApplyStaSettings(void);
void wConfigApplyUapSettings(void);
void wConfigApplyWfdSettings(void);

#define WCONFIG_STACK_SIZE	8 * 2048
#define WTIMER_STACK_SIZE	8 * 2048
#define STARTSTOP_STACK_SIZE 10 * 2048
#define SCAN_MAX_DATA		2048
#define WLANIDLE_ON 		1
#define WLANIDLE_OFF 		0
#define NUM_WCONFIG_MSGS	 16 
#define NUM_WTIMER_MSGS	    8 
			
// Time between rejoin attempts when we lost link status in ad hoc mode (8 seconds for now)
#define AD_HOC_REJOIN_TIME 100 * 8

// Time to notify network we are going down
#define NOTIFY_NETWORK_TIME 100 * 2


static TX_MUTEX wlan_scan_mutex;
static TX_MUTEX wlan_start_stop_mutex;

ALIGN8 static char wconfigStack[WCONFIG_STACK_SIZE];
ALIGN8 static char wtimerStack[WTIMER_STACK_SIZE];
ALIGN8 static char wlanStartStopStack[STARTSTOP_STACK_SIZE];

#ifdef USE_EXTERNAL_INTERFACE
/* pointer to system interface func table; used for run-time linking
 */
SYSTEM_INTERFACE* g_pSystemInterface = (SYSTEM_INTERFACE*)SYSTEM_INTERFACE_ADDRESS;
#endif

TX_TIMER wcfgStaInfoTimer;
TX_TIMER wcfgUapInfoTimer;
TX_TIMER wcfgWfdInfoTimer;
TX_TIMER wcfgAdHocRejoinTimer;

delay_msec_timer_t  *wifi_sta_badwepkey_timer = NULL; //added this to detect a possible bad WEP key
void wifi_sta_badwepkey_timer_expired(ULONG unused);

static TX_THREAD wlanStartStopThread;

TX_THREAD wconfigThread;
TX_QUEUE wconfigMsgQ;
UINT32 wconfigMsgQBuf[NUM_WCONFIG_MSGS * sizeof(MESSAGE)];

TX_THREAD wtimerThread;
TX_QUEUE wtimerMsgQ;
UINT32 wtimerMsgQBuf[NUM_WTIMER_MSGS * sizeof(MESSAGE)];

//-jrsWIRELESSINFORMATION curr_pWinfo;
static struct net_device *g_wlanDev = NULL;
static mlan_private *g_pmlanPriv = NULL;

typedef struct _WIRELESS_SETTINGS
{
    wifi_intf_comm_mode_t mode;     ///< infrastructure or adhoc
}WIRELESS_SETTINGS;

static WIRELESS_SETTINGS g_curWirelessSettings;     ///< current wireless settings
static BOOL g_wirelessInitialized = FALSE;          ///< true if the wireless subsystem has been initialized
                                                    ///< and the main wireless thread has been started

// When the wired link is enabled, we're disabled.  Per the requirements of the
// netDriver, we assume we're disabled until they explicitly tell us otherwise.
static BOOL gWifiStaDisabledByWiredLink = TRUE;

// When the STA is using AdHoc, the uAP must be shut down.  They don't coexist.
static BOOL gWifiUapDisabledByAdHocSta  = FALSE;

#ifdef WIFI_DIRECT_SUPPORT
//Make sure that the Active SSID set for WFD GO happens only once
static bool gWfdActiveSsidSet = FALSE;
#endif

//Counter to detect bad WEP key in shared authentication mode
int g_wifi_sta_badwepkey_retry_counter = 0;

//This points to the iface id of the currently active uAP configuration, 
uint8_t g_wifi_uap_current_active_config = WIFI_IFACE_ID_UAP;

//Current scan configuration
static bool gWifiScanConfig = SCAN_ALL;

//Custom IE array
static wifi_intf_customie_info_t *g_customie_info[MAX_NUM_CUSTOM_IE];


typedef enum
{
    WLAN_OFF = 0,
    WLAN_SHUTTING_DOWN,
    WLAN_STARTING_UP,
    WLAN_ON,
} WLAN_POWER_STATE;

static WLAN_POWER_STATE g_wlan_power_state = WLAN_OFF;

typedef enum
{
    NONE = 0,
    POWER_DOWN_AFTER_MAC_INIT,
    POWER_DOWN_AFTER_SCAN,
    RUN_WPS_AFTER_POWER_ON,
} POWERUP_EVENTS;

static POWERUP_EVENTS g_after_power_up_event = NONE;
static MESSAGE gWPSMsg;
static MESSAGE gScanMsg;
static TX_EVENT_FLAGS_GROUP wlan_startup_shutdown_flags;
static bool gUsingWireless;

#define WLAN_STARTUP_FLAG  0x1
#define WLAN_SHUTDOWN_FLAG  0x2
#define WLAN_STARTUP_SHUTDOWN_MASK  (WLAN_STARTUP_FLAG | WLAN_SHUTDOWN_FLAG)


// forward function declarations 
int WlanSetScan();
int WlanSetEssid(CHAR* ssid, int len, int skip_scan);
int wConfigInit(void);
ATSTATUS WlanStartupInternal(void);
ATSTATUS WlanShutdownInternal(void);
void wifi_update_channel_number(void);

void WirelessInitMACAddress(void);

    // TODO - Find a logical header for these to live in
    int32_t net_wlan_uap_notify_link_up();
    int32_t net_wlan_uap_notify_link_down();

bool shared_uap_link_is_up[WIFI_NUM_IFACES] = {false, false, false, false};
void netdrvr_uap_shared_notify_link_down(int iface_id)
{
    int  i;
    bool one_iface_on = false;

    shared_uap_link_is_up[iface_id] = false;

    for (i = 0; i < WIFI_NUM_IFACES; i++)
    {
        if (shared_uap_link_is_up[i])
        {
            one_iface_on = true;
        }
    }
    if (!one_iface_on)
    {
        net_wlan_uap_notify_link_down();        
        wifi_intf_uap_set_state(WLAN_INACTIVE, NULL, 0, NULL, NULL);
    }
}

void netdrvr_uap_shared_notify_link_up(int iface_id)
{
    int  i;
    bool one_iface_on = false;

    shared_uap_link_is_up[iface_id] = true;

    for (i = 0; i < WIFI_NUM_IFACES; i++)
    {
        if (shared_uap_link_is_up[i])
        {
            one_iface_on = true;
        }
    }
    if (one_iface_on)
    {
        net_wlan_uap_notify_link_up();
    }
}


// =======================================================
// === NEEDS TO BE CLEANED UP ============================
// =======================================================
static void fillInWirelessIfaceInfo(int iface_id, wifi_intf_info_t * pWinfo)
{
    int key_idx;

    wifi_intf_get_var_is_enabled(iface_id, &pWinfo->enabled);
    wifi_intf_get_var_ssid(iface_id, pWinfo->SSID, sizeof(pWinfo->SSID));
    wifi_intf_get_var_sec_mode(iface_id, &pWinfo->SecurityMode);
    wifi_intf_get_var_wpa_encryp_mode(iface_id, &pWinfo->WPAEncryptionMode);
    wifi_intf_get_var_wpa_passphrase(iface_id, pWinfo->WPAPassPhrase, sizeof(pWinfo->WPAPassPhrase));
    wifi_intf_get_var_wep_auth_mode(iface_id, &pWinfo->WEPAuthMode);
    wifi_intf_get_var_wep_key_len(iface_id, &pWinfo->WEPKeyLength);
    for(key_idx = 0; key_idx < NUM_WEP_KEYS; key_idx++)
        wifi_intf_get_var_wep_key(iface_id, key_idx, pWinfo->WEPKey[key_idx], WEP_KEY_LEN);
    wifi_intf_get_var_wep_cur_key(iface_id, &pWinfo->WEPCurKey);
    wifi_intf_get_var_channel(iface_id, &pWinfo->Channel);

    switch(iface_id)
    {
        case WIFI_IFACE_ID_STATION:
        {
            wifi_intf_get_var_comm_mode(iface_id, &pWinfo->sta.CommunicationMode);
            break;
        }

        case WIFI_IFACE_ID_WFD: // Intentionally fall through
        case WIFI_IFACE_ID_UAP:
        case WIFI_IFACE_ID_UAP_PRI0:
        {
            wifi_intf_get_var_ssid_prefix(iface_id, pWinfo->uap.SSIDPrefix, sizeof(pWinfo->uap.SSIDPrefix));
            wifi_intf_get_var_mac_filter_mode(iface_id, &pWinfo->uap.MACFilterMode);
            wifi_intf_get_var_mac_filter_list(iface_id, pWinfo->uap.MACFilterList);
            break;
        }

        default:
            break;
    }
}
// =======================================================
// =======================================================


/**
 * Check all the wifi enable fields to determine whether we
 * should power the radio or not.  Turn the radio on or off as
 * appropriate.
 * 
 * returns TRUE if the radio is powered on, FALSE if it's off
 * 
 */
static void EnsureCorrectRadioPowerState(BOOL *power_is_now_on)
{
    bool  hw_enabled;
    bool  sta_enabled;
    bool  uap_enabled;
    bool  wfd_enabled;
    bool  uap_pri0_enabled;
    uint8_t uap_active_config;
    bool scan_active = false;

    wifi_hw_get_var_is_enabled(&hw_enabled);
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_STATION, &sta_enabled);
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP, &uap_enabled);
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_WFD, &wfd_enabled);
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP_PRI0, &uap_pri0_enabled);
    uap_active_config = wifi_get_uap_active_config();

    if (WlanIsWirelessInstalled())
    {
        // Disable the hardware if:
        // 1) Someone turned off the master HW enable
        // 2) The STA, UAP, WFD interfaces and UAP priority 0 interfaces are disabled. 
        // AND: we aren't in an exception case (running scan or WPS from powered off state)
        if ((!hw_enabled ||
             (!(wfd_enabled) && (!(uap_enabled && (uap_active_config == WIFI_IFACE_ID_UAP))) && 
              (!(sta_enabled) || (gWifiStaDisabledByWiredLink)) && 
              (!(uap_pri0_enabled && (uap_active_config == WIFI_IFACE_ID_UAP_PRI0))))) &&
            ((g_after_power_up_event == NONE) && !wifi_intf_get_var_is_wps_running())
           )
        {
            WlanShutdown();
            if (power_is_now_on != NULL)
            {
                *power_is_now_on = FALSE;
            }
        }
        else
        {
            if (g_after_power_up_event == POWER_DOWN_AFTER_SCAN)
                scan_active = true;    

            if (wifi_oem_allow_power_up(scan_active, wifi_intf_get_var_is_wps_running()))
            {
                WlanStartup();
                if (power_is_now_on != NULL)
                {
                    *power_is_now_on = TRUE;
                }
            }
        }
    }
    else
    {
        if (power_is_now_on != NULL)
        {
            *power_is_now_on = FALSE;
        }
    }
}

#ifdef REASSOCIATION
/*---------------------------------------------------------------------------
 * This API is used to turn on or off reassociation
 */
static void reassociation(BOOL turn_reassociation_on)
{
    struct iwreq rwrq;
    int reassoc_mode;

    if (turn_reassociation_on) 
        reassoc_mode = 1;
    else
        reassoc_mode = 0;

    // Turn reassociation mode on/off
    rwrq.u.data.length = 1;
    rwrq.u.data.pointer = &reassoc_mode;
    rwrq.u.data.flags = WOAL_SET_GET_REASSOC;
    g_wlanDev->do_ioctl(g_wlanDev,(struct ifreq *) &rwrq, WOAL_SETONEINT_GETONEINT);
}
#endif /* REASSOCIATION */

/*---------------------------------------------------------------------------
 * This API is used to clean any outstanding packets that have already been 
 * handed to the module .
 */
void WlanWmmCleanupQueues(struct net_device * dev)
{
    // wlan_wmm_cleanup_queues() is already being called in wlan_clean_txrx() for V12
    // This may not be needed for V12
#if 0
    if (dev)
    {
        mlan_private* priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];
        wlan_wmm_cleanup_queues(priv);
    }
#endif
}


/*---------------------------------------------------------------------------
 * This API is used to associate with the AP that has the specified ssid.
 */
int WlanAssoc(char * ssid, int ssid_len, int skip_scan)
{
    return WlanSetEssid(ssid, ssid_len, skip_scan);
}

/*---------------------------------------------------------------------------
 * This API is used to get the scan result 
 * for SUPPLICANT
 */ 
int WlanShimCGetScanResult(struct SCAN_SSID_IE *scanResult, int maxAPs)
{
	struct iw_point dwrq;

    dwrq.length = maxAPs;

	if(wlan_get_scan_wpa(g_pmlanPriv, NULL, &dwrq, scanResult) != 0){
		DPRINTF(WCDBG, (" ERROR SHIMC: GET SCAN RESULTS \n"));
		return -1;
	}
    return dwrq.length;
}

/*---------------------------------------------------------------------------
 * This API  is used send the WPA_IE 
 */
int WlanShimCSetWpaIE(char * wpaie, int wpaieLen)
{
    if( wlan_setwpaie_ioctl(g_pmlanPriv, wpaie, wpaieLen))
    {
        DPRINTF(WCDBG, (" ERROR SHIMC: SET WPAIE \n"));
        return -1;
    }
    return 0;
}
/*---------------------------------------------------------------------------
 * This API is used get the ssid of the current associated AP.
 */ 
int WlanShimCGetSsid(char *ssid)
{
	struct iw_point dwrq;

    dwrq.length = MLAN_MAX_SSID_LENGTH;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCGIWESSID)](g_wlanDev,
                                                                           NULL,
                                                                           (union iwreq_data *)&dwrq,
                                                                           ssid) != 0)
    {
		DPRINTF(WCDBG, ("ERROR SHIMC: GET ESSID \n"));
		return -1;
	}
	return dwrq.length;
}

/*---------------------------------------------------------------------------
 * This API is used get the bssid of the current associated AP.
 */ 
int WlanGetBssid(char *extra)
{
    if (WlanIsWirelessHWActive() && g_pmlanPriv->media_connected == MTRUE) {
        memcpy(extra, g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address, ETH_ALEN);
		DPRINTF(WCDBG, ("Current BSSID = %02x:%02x:%02x:%02x:%02x:%02x\n",
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[0],
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[1],
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[2],
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[3],
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[4],
				g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address[5]));

	} else {
		DPRINTF(WCDBG, (" WlanMediaState not Connected\n"));
		memset(extra, 0, ETH_ALEN);
	}

	return 0;
}

/*---------------------------------------------------------------------------
 * This API is used to de - authenticate from associated AP.
 */
int WlanShimCDeauth(void)
{
    struct iwreq dwrq;

#ifdef REASSOCIATION
    reassociation(FALSE);
    if (((moal_private *)(g_wlanDev->priv))->phandle->is_reassoc_timer_set == MTRUE)
    {
        CancelTimer(&((moal_private *)(g_wlanDev->priv))->phandle->reassoc_timer);
        ((moal_private *)(g_wlanDev->priv))->phandle->is_reassoc_timer_set = MFALSE;
    }
#endif /* REASSOCIATION */

    // Only bother doing a deauth if you're actually connected
    if ((g_pmlanPriv->media_connected == MTRUE) && 
        (g_pmlanPriv->bss_mode == MLAN_BSS_MODE_INFRA))
    {
        DPRINTF(WCDBG, ("Calling deauth\n"));
        dwrq.u.data.length = 0;
        dwrq.u.data.flags = WOAL_DEAUTH;
        g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SETADDR_GETNONE);
    }
    else
    {
        DPRINTF(WCDBG, ("Didn't deauth because state: %d, Mode: %d\n", g_pmlanPriv->media_connected, g_pmlanPriv->bss_mode));
    }
	return 0;
}

/*---------------------------------------------------------------------------
 * This API Is used to set the Key after EAPOL HANDSHAKE
 */

int WlanShimCSetKey(struct iw_encode_ext *param, int length)
{
	struct iw_point dwrq;

	dwrq.pointer = param;
	dwrq.length = length;
	dwrq.flags = param->ext_flags;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWENCODEEXT)](g_wlanDev,
                                                                            NULL,
                                                                            (union iwreq_data *)&dwrq,
                                                                            (char*)param))
    {
		DPRINTF(WCDBG, ("ERROR SHIMC: SET ENCODE 1 \n"));
		return -1;
	}
	return 0;
}


/*---------------------------------------------------------------------------
 * This API  is used to scan the AP with specificed ssid
 */
int WlanShimCSetScanWithSsid(char * ssid, int ssid_len)
{
    int ret = 0;

    if (!WlanIsWirelessHWActive())
    {
        return -1;
    }

    tx_mutex_get(&wlan_scan_mutex, TX_WAIT_FOREVER);

    if (ssid != NULL)
    {
    	/*SCAN WITH SPECIFIC SSID 
    	 */
        struct iw_scan_req req;
        struct iw_point dwrq;

        req.essid_len = MIN(ssid_len, IW_ESSID_MAX_SIZE);
        ASSERT(req.essid_len <= IW_ESSID_MAX_SIZE);
        strncpy((char *) req.essid, ssid, req.essid_len);
        
        dwrq.flags = IW_SCAN_THIS_ESSID;
        dwrq.length = sizeof(struct iw_scan_req);

        if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWSCAN)](g_wlanDev,
                                                                              NULL,
                                                                              (union iwreq_data *)&dwrq,
                                                                              (char *)&req) != 0)
        {
    		DPRINTF(WCDBG, ("ERROR SHIMC: SPECIFIC SCAN \n"));
            ret = -1;
    	}
    }
    else
    {
        // Generic scan -- no specific ssid
        ret = WlanSetScan();
    }
    tx_mutex_put(&wlan_scan_mutex);
   
	return (ret);
}

/*---------------------------------------------------------------------------
 * This API is used to set MAC address 
 */
#if 0
int WlanSetMacAddrFromStaticNVRAM(void)
{
    unsigned char *Addr;
    struct sockaddr	addr;

    Addr = (unsigned char *)DSLockVar(e_MACAddress);

    DPRINTF(WCDBG, ("WlanSetMacAddr :Setting Mac Addr to "
                    "%02X %02X %02X %02X %02X %02X\n",
                    Addr[0], Addr[1], Addr[2],
                    Addr[3], Addr[4], Addr[5]));

    memcpy(addr.sa_data, Addr, ETH_ALEN);
    DSUnlockVar(e_MACAddress);

    if ((g_wlanDev->set_mac_address(g_wlanDev, &addr)) != 0)
    {
        DPRINTF(WCDBG, ("ERROR WlanSetMacAddr: SET MAC ADDRESS \n"));
        return -1;
    }

	return 0;
}
#endif


/*---------------------------------------------------------------------------
 * This API is used to get the MAC address
 */
BOOL WlanGetMacAddr(char *iface, char* Addr)
{
#ifdef WIFI_DIRECT_SUPPORT
    uint8_t wfd_role;

    wifi_intf_get_var_wfd_bss_role(WIFI_IFACE_ID_WFD, &wfd_role);
#endif

    if (!WlanIsWirelessHWActive())
    {
        return FALSE;
    }
    else
    {
        memcpy(Addr, g_pmlanPriv->curr_addr, ETH_ALEN);

#ifdef WIFI_DIRECT_SUPPORT
        //Generate the virtual MAC address only for the following cases:
        // 1. If this is the WFD interface and we are set as the group owner
        // 2. If this is the uAP interface.
        // For all other cases, use the regular wireless MAC address.  
            
        if (((strcmp(iface, WIFI_IFACE_NAME_WFD) == 0) && (wfd_role == WIFIDIRECT_GO_ROLE)) ||
            (strcmp(iface, WIFI_IFACE_NAME_UAP) == 0))
        {
            wifi_config_generate_virtual_MAC_addr(Addr);                
        }
#else
        //Generate the virtual MAC address only for the following cases:
        // 1. If this is the uAP interface.
        // For all other cases, use the regular wireless MAC address.  

        if (strcmp(iface, WIFI_IFACE_NAME_UAP) == 0)
        {
            wifi_config_generate_virtual_MAC_addr(Addr);                
        }
#endif
    }
    
	return TRUE;
}

/*--------------------------------------------------------------------------
 * This API is used to set the ESSID
 */
int WlanSetEssid(CHAR* ssid, int ssid_len, int skip_scan)
{
    struct iw_point dwrq;
    int retCode;
    int channel;
    
    memset(&dwrq, 0, sizeof(dwrq));

    ASSERT(ssid != NULL);
	DPRINTF(WCDBG, ("WlanSetEssid = %s skip %d\n", ssid, skip_scan));
    ASSERT(ssid[0] != 0);

    if (skip_scan)
    {
        dwrq.flags = 0xFFFF;
    }
    else
    {
        if(g_curWirelessSettings.mode == AD_HOC)
        {
            // In adhoc mode a flags value of 1 indicates that wlan_set_essid should
            // scan for the adhoc network.  A value > 1 indicates an index in the current
            // scan results that we should try to connect to.
            // Since we generate unique-ish SSIDs based on MAC, there isn't much
            // chance of printers connecting to eachother.  We need to be able to scan and
            // join for the re-join tests in WiFi certification.
            #ifdef REASSOCIATION
            reassociation(FALSE);
            #endif /* REASSOCIATION */
            dwrq.flags = 1;
        }
        else
        {
            // in infrastructure mode a flags of 1 indicates that we should scan for the
            // essid and try to connect to it.
            dwrq.flags = 1;
        }
    }

    dwrq.length = ssid_len;

    
#if 0
    int i;
    for (i=0; i<10; i++) 
    {
        if ((retCode = g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWESSID)](g_wlanDev, 
                                                                                          NULL, 
                                                                                          (union iwreq_data *)&dwrq, 
                                                                                          ssid)) == 0)
            break;
    }
    if (retCode != 0)
    { 
        DPRINTF(WCDBG, ("WlanSetEssid ERROR failed to set essid (%d)\n", retCode));
        return -1;
    }

#else
    if ((retCode = g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWESSID)](g_wlanDev, 
                                                                                      NULL, 
                                                                                      (union iwreq_data *)&dwrq, 
                                                                                      ssid)) != 0)
    { 
		DPRINTF(WCDBG, ("WlanSetEssid ERROR failed to set essid (%d)\n", retCode));
		return -1;
	}
#endif

	DPRINTF(WCDBG, ("WlanSetEssid %s !!!!\n", (g_curWirelessSettings.mode == AD_HOC) ? "ADHOC STARTED" : "ASSOC STARTED"));

    channel = WlanGetChannel();
    DPRINTF(WCDBG, ("WlanSetEssid: STA channel set to %d\n", channel));

    if (gWifiScanConfig  == SCAN_2_4GHZ_ONLY)
    {
        if (channel > MAX_CHANNELS_BG)
        {
            DPRINTF(WCDBG, ("WlanSetEssid ERROR Scan restriction in place!\n"));
		    return -1;            
        }    
    }
    
    //Update the NVRAM var for channel number for all wireless interfaces
    wifi_update_channel_number();
        
	return 0;
}
/*--------------------------------------------------------------------------
 * This API is used to set the WAP when you need the AP with a specific MAC address
 */
int WlanSetWAP(const CHAR* bssid)
{
    struct sockaddr awrq;
    int retCode;

    memset(&awrq, 0, sizeof(awrq));

    ASSERT(bssid != NULL);
	DPRINTF(WCDBG, ("WlanSetWAP = %02x:%02x:%02x:%02x:%02x:%02x\n", 
                    (int)bssid[0], (int)bssid[1], (int)bssid[2],
                    (int)bssid[3], (int)bssid[4], (int)bssid[5]));

    awrq.sa_family = ARPHRD_ETHER;
    memcpy(awrq.sa_data, bssid, BSSID_LENGTH);
    awrq.sa_len = BSSID_LENGTH;

    WlanSetMode(INFRASTRUCTURE);

    if ((retCode = g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWAP)](g_wlanDev,
                                                                                   NULL,
                                                                                   (union iwreq_data *)&awrq,
                                                                                   NULL)) != 0)
    {
		DPRINTF(WCDBG, ("WlanSetWAP ERROR failed to set bssid (%d)\n", retCode));
		return -1;
	}

#ifdef REASSOCIATION
    reassociation(TRUE);
#endif /* REASSOCIATION */

	return 0;
}


/** 
 * \brief Verify that our encryption settings match those of the
 * specified AP.
 * 
 * \param targetAP Driver's scan descriptor for the AP that you
 * want to connect to.
 * 
 * In many cases, you can still associate even if you have the
 * wrong encryption settings.  (If you're set for an open
 * network, for example, a WPA AP will let you associate because
 * it expects you to start the 4-way handshake.)  To keep our
 * blue light from going solid (indicating a successful
 * connection), we double-check that our encryption settings are
 * compatible with those of the AP before we even try to
 * associate.  This won't help you if you have the right
 * settings, but the wrong key/passphrase.  It will, however,
 * keep you from appearing connected when you don't even have
 * the right security mode set.
 * 
 * \return 0 when compatible, -1 if incompatible
 **/
int WlanAssocEncryptionCheck(BSSDescriptor_t *targetAP)
{
    BOOL compatible = FALSE;
    wifi_intf_security_mode_t securityMode;

    if (wifi_intf_get_var_is_wps_running())
    {
        // WPS needs to associate with APs before it sets the correct encryption mode.
        compatible = TRUE;
    }
    else
    {
        wifi_intf_get_var_sec_mode(WIFI_IFACE_ID_STATION, &securityMode);

        if ((targetAP->prsn_ie) &&
            (targetAP->prsn_ie->ieee_hdr.element_id == RSN_IE))
        {
            if (securityMode == WPA_PSK || securityMode == WPA2_PSK)
            {
                compatible = TRUE;
            }
        }
        else if ((targetAP->pwpa_ie) &&
                 (targetAP->pwpa_ie->vend_hdr.element_id == WPA_IE))
        {
            if (securityMode == WPA_PSK || securityMode == WPA2_PSK)
            {
                compatible = TRUE;
            }
        }
        else if (targetAP->privacy)
        {
            if (securityMode == WEP)
            {
                compatible = TRUE;
            }
        }
        else
        {
            if (securityMode == OPEN_SYSTEM)
            {
                compatible = TRUE;
            }
        }
    }

    if (compatible)
    {
        return 0;
    }
    DPRINTF((WCDBG), ("WLAN: AP (%02x:%02x:%02x:%02x:%02x:%02x) has incompatible settings (ignoring)\n", 
                      targetAP->mac_address[0], targetAP->mac_address[1], targetAP->mac_address[2],
                      targetAP->mac_address[3], targetAP->mac_address[4], targetAP->mac_address[5]));
    return -1;
}


extern struct net_device *wlan_get_net_uap_dev();
extern struct net_device *wlan_get_net_wfd_dev();

int WlanIoctl(int command, void * buf)
{
    int ret = 0;
    net_device *net_dev = NULL;

    if (strcmp(((struct ifreq *) buf)->ifr_name, WIFI_IFACE_NAME_STATION) == 0)
        net_dev = wlan_get_net_dev();
    else if (strcmp(((struct ifreq *) buf)->ifr_name, WIFI_IFACE_NAME_UAP) == 0)
        net_dev = wlan_get_net_uap_dev();
#ifdef WIFI_DIRECT_SUPPORT
    else if (strcmp(((struct ifreq *) buf)->ifr_name, WIFI_IFACE_NAME_WFD) == 0)
        net_dev = wlan_get_net_wfd_dev();
#endif

    if (net_dev != NULL)
    {
        if (net_dev->do_ioctl(net_dev, (struct ifreq *) buf, command) != 0)
        {
            DPRINTF(WCDBG,("Failed WlanIoctl command 0x%X for %s\n", command, ((struct ifreq *) buf)->ifr_name));
            ret = -1;
         }
    }
    else
    {
        DPRINTF(WCDBG,("Failed to get device WlanIoctl\n"));
        ret = -1;
    }

    return (ret);
}


/*---------------------------------------------------------------------------
 * This API used to set mode
 */
int WlanSetMode(wifi_intf_comm_mode_t wlanMode)
{
	UINT32 mode;
    struct iwreq dwrq;

	DPRINTF(WCDBG, ("WlanSetMode: %s\n", (wlanMode == AD_HOC) ? "ADHOC" : "INFRASTRUCTURE"));

    switch(wlanMode)
    {
        case AD_HOC:
            mode = IW_MODE_ADHOC;
            dwrq.u.data.length = 0;
            dwrq.u.data.flags = WOAL_DEAUTH;
            if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SETADDR_GETNONE) != 0)
            {
                DPRINTF(WCDBG,("WlanSetMode: set mode Ad-Hoc Stop failed\n"));
            }
            break;
        case INFRASTRUCTURE:
            mode = IW_MODE_INFRA;
            break;
        default:
            DPRINTF(WCDBG, ("WlanSetMode: Unknown Mode requested: %#x\n", wlanMode));
            ASSERT(0);
            break;
    }

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWMODE)](g_wlanDev, 
                                                                          NULL, 
                                                                          (union iwreq_data *)&mode, 
                                                                          NULL) != 0)
    {
		DPRINTF(WCDBG, ("WlanSetMode: ERROR: set mode failed.\n"));
		return -1;
	}

    // store the current mode
    g_curWirelessSettings.mode = mode;

	return 0;
}

/*---------------------------------------------------------------------------
 * This API is used set the wlan driver to Idle State
 */

int wlan_set_idleState(int type)
{
    wpaSupConfig sConfig;
    int ret =0;

    DPRINTF(WCDBG, ("SHIMC: Set WlanIdleState Function\n"));
    if( type == WLANIDLE_ON )
    {
        //if(wlanidle_on(g_pmlanPriv))
        //RASBURY - TBD What is the equivalent???
        ASSERT(0);
        {
            DPRINTF(WCDBG, (" ERROR SHIMC: WLANIDLE ON \n"));
            return -1;
        }
        /* send wpa disable
         */
        sConfig.WPA_ON = 0;
        ret = SupConfig(sConfig); 
        if(ret == 0)
        {
            if( wlan_wpaoff_ioctl(g_pmlanPriv))
            {
                DPRINTF(WCDBG, (" ERROR SHIMC: SET WPA-OFF-IOCTL\n"));
                return -1;
            }
        }
    }
    else if(type == WLANIDLE_OFF)
    {
        wConfigApplyStaSettings();
    }
    return 0;
}
/*---------------------------------------------------------------------------*/

int WlanSetChannel(wifi_intf_info_t *pWinfo)
{
	struct iw_freq fwrq = {0};

	DPRINTF(WCDBG, ("!! SHIMC: SETCHANNEL  FUNCTION !... \n"));
		
	if(pWinfo->Channel == 0 ){
		DPRINTF(WCDBG, ("SHIMC: DEFAULT CHANNEL 11\n"));
		pWinfo->Channel = 11;
	}

	fwrq.m = pWinfo->Channel;
	fwrq.e = 0;
	fwrq.i = 0;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWFREQ)](g_wlanDev, 
                                                                          NULL, 
                                                                          (union iwreq_data *)&fwrq, 
                                                                          NULL) != 0)
    {
		DPRINTF(WCDBG, (" ERROR SHIMC: SET CHANNEL \n"));
		return -1;
	}

	DPRINTF(WCDBG, ("SHIMC: SetChannel to =%d\n", pWinfo->Channel));
	return 0;
}

int WlanGetChannel(void)
{
	struct iw_freq fwrq = {0};
    int channel = 11;

	DPRINTF(WCDBG, ("!! SHIMC: GETCHANNEL  FUNCTION !... \n"));

	fwrq.m = 0;
	fwrq.e = 0;
	fwrq.i = 0;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCGIWFREQ)](g_wlanDev, 
                                                                          NULL, 
                                                                          (union iwreq_data *)&fwrq, 
                                                                          NULL) != 0)
    {
		DPRINTF(WCDBG, (" ERROR SHIMC: GET CHANNEL \n"));
		return -1;
	}

    channel = (int) fwrq.i;

    DPRINTF(WCDBG, ("SHIMC: GetChannel to = %d\n", channel));
	return channel;
}


/*---------------------------------------------------------------------------*/
int WlanSetScan()
{
    struct iw_point dwrq;

    dwrq.flags = IW_SCAN_DEFAULT;
    dwrq.length = 0;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWSCAN)](g_wlanDev,
                                                                          NULL,
                                                                          (union iwreq_data *)&dwrq,
                                                                          NULL) != 0)
    {
        DPRINTF(WCDBG, ("ERROR SHIMC :SET SCAN \n"));
        return -1;
    }

	return 0;
}

/*---------------------------------------------------------------------------*/
int ProcessSetSupp(wifi_intf_info_t *pWinfo, bool enable_allowed)
{
    wpaSupConfig sConfig;
    
    strcpy((char*)sConfig.SSID,(char*)pWinfo->SSID);
    sConfig.WPAEncryptionMode = pWinfo->WPAEncryptionMode;
    strcpy((char*)sConfig.PassPhrase, (char*)pWinfo->WPAPassPhrase);

    if ((enable_allowed) && (pWinfo->enabled) && (strlen((char*)(pWinfo->SSID)) > 0) &&
        (pWinfo->sta.CommunicationMode == INFRASTRUCTURE) &&
        ((pWinfo->SecurityMode == WPA_PSK) || (pWinfo->SecurityMode == WPA2_PSK)))
    {
        sConfig.WPA_ON = 1;
        sConfig.ifname = WIFI_IFACE_NAME_STATION;
        SupConfig(sConfig); 

        if( wlan_wpaon_ioctl(g_pmlanPriv))
        {
            DPRINTF(WCDBG, ("ERROR SHIMC: SET WPA-ON-IOCTL\n"));
            return -1;
        }
    }
    else
    {
        sConfig.WPA_ON = 0;
        SupConfig(sConfig); 

        if( wlan_wpaoff_ioctl(g_pmlanPriv))
        {
            DPRINTF(WCDBG, ("ERROR SHIMC: SET WPA-OFF-IOCTL\n"));
            return -1;
        }
    }
    
    return 0;
}

void WlanSetCountermeasures(int enabled)
{
    if (enabled)
    {
        // Start countermeasures -- disable WPA
        wlan_wpaoff_ioctl(g_pmlanPriv);
    }
    else
    {
        // Stop countermeasures -- re-enable WPA
        wlan_wpaon_ioctl(g_pmlanPriv);
    }
}


/*---------------------------------------------------------------------------*/
int WlanSetEncryption(wifi_intf_info_t *pWinfo)
{
    struct iw_point dwrq;
    int i;
    int key = 0; 

    dwrq.length=0; 
    dwrq.flags = 0;

    if((pWinfo->SecurityMode == WPA_PSK) || (pWinfo->SecurityMode == WPA2_PSK))
    {
        //CHECK IF WEP IS ENABLED ,THEN DISABLE THE KEYS
        dwrq.flags  |= IW_ENCODE_DISABLED;
        dwrq.flags  |= IW_ENCODE_NOKEY;

        if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWENCODE)](g_wlanDev,
                                                                                NULL,
                                                                                (union iwreq_data *)&dwrq,
                                                                                NULL))
        {
            DPRINTF(WCDBG, (" ERROR SHIMC: SET ENCODE 2 \n"));
            return -1;
        }
        ProcessSetSupp(pWinfo, true);
    }
    else
    {
        mlan_ds_encrypt_key * WEPkey;

        // Disable WPA
        ProcessSetSupp(pWinfo, FALSE);

        DPRINTF(WCDBG, (" SHIMC: SETENCRYPTION FUNCTION \n"));

        if(pWinfo->SecurityMode == WEP)
        {
            DPRINTF(WCDBG, ("SHIMC: FIRST TIME WEP \n"));

            key = pWinfo->WEPCurKey + 1; 

            for(i = 0; i < 4; i++){ //set all the wep keys 
                DPRINTF(WCDBG, ("WEP keys[%d] -- %s\n", i, pWinfo->WEPKey[i]));
                if(pWinfo->WEPAuthMode == AUTH_AUTO)
                {
                    dwrq.flags = IW_ENCODE_ENABLED | IW_ENCODE_AUTO | (i + 1);
                } 
                else if(pWinfo->WEPAuthMode == AUTH_SHARED_KEY)
                {
                    dwrq.flags = IW_ENCODE_ENABLED | IW_ENCODE_RESTRICTED | (i + 1);
                }
                else
                {
                    dwrq.flags = IW_ENCODE_ENABLED | IW_ENCODE_OPEN | (i + 1) ;
                }
                
                WEPkey = ( mlan_ds_encrypt_key *) MEM_MALLOC(sizeof(mlan_ds_encrypt_key) + pWinfo->WEPKeyLength);
                ASSERT(WEPkey);
                memset(WEPkey, 0, sizeof(mlan_ds_encrypt_key));
                WEPkey->key_index = i;

                if ((pWinfo->WEPKeyLength == 5) || (pWinfo->WEPKeyLength == 13))
                {
                    // Alphanumeric key - copy it straight across
                    WEPkey->key_len = pWinfo->WEPKeyLength;
                    memcpy(WEPkey->key_material, pWinfo->WEPKey[i], pWinfo->WEPKeyLength);
                }
                else
                {
                    // Hex key written in ASCII form -- convert to raw before sending it down
                    WEPkey->key_len = (pWinfo->WEPKeyLength + 1) / 2;
                    hex_str_to_raw((char *)pWinfo->WEPKey[i], (char *)WEPkey->key_material, sizeof(WEPkey->key_material));
                }

                dwrq.length = WEPkey->key_len;
                if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWENCODE)](g_wlanDev,
                                                                                        NULL,
                                                                                        (union iwreq_data *)&dwrq,
                                                                                        (char*)WEPkey->key_material))
                {
                    DPRINTF(WCDBG, ("ERROR SHIMC: SET ENCODE 3\n"));
                    MEM_FREE_AND_NULL(WEPkey);
                    return -1;
                }
                MEM_FREE_AND_NULL(WEPkey);
            }

            DPRINTF(WCDBG, (" SHIMC :WEP ENABLED \n"));
            if(pWinfo->WEPAuthMode == AUTH_AUTO)
            {
                DPRINTF(WCDBG, (" AUTO AUTHENTICATION ENABLED \n"));
                dwrq.flags = IW_ENCODE_AUTO | key;
            }
            else if(pWinfo->WEPAuthMode == AUTH_SHARED_KEY)
            {
                DPRINTF(WCDBG, (" SHARED AUTHENTICATION ENABLED \n"));
                dwrq.flags = IW_ENCODE_RESTRICTED | key;
            }
            else
            {
                DPRINTF(WCDBG, (" OPEN AUTHENTICATION ENABLED \n"));
                dwrq.flags = IW_ENCODE_OPEN | key;
            }
        }        
        else  //if (pWinfo->SecurityMode == OPEN_SYSTEM)
        {
            DPRINTF(WCDBG, (" SHIMC: ENCRYPTION DISABLED\n"));
            dwrq.length = 0;
            dwrq.flags = IW_ENCODE_DISABLED | IW_ENCODE_NOKEY;
        }

        DPRINTF(WCDBG, ("SHIMC: SECOND TIME WEP \n"));
        dwrq.length = 0;    // Length 0 tells it to just enable/disable the key -- rather than set it

        WEPkey = ( mlan_ds_encrypt_key *) MEM_MALLOC(sizeof(mlan_ds_encrypt_key) + pWinfo->WEPKeyLength);
        ASSERT(WEPkey);
        memset(WEPkey, 0, sizeof(mlan_ds_encrypt_key));
        WEPkey->key_index = pWinfo->WEPCurKey;

        if ((pWinfo->WEPKeyLength == 5) || (pWinfo->WEPKeyLength == 13))
        {
            // Alphanumeric key - copy it straight across
            WEPkey->key_len = pWinfo->WEPKeyLength;
            memcpy(WEPkey->key_material, pWinfo->WEPKey[pWinfo->WEPCurKey], pWinfo->WEPKeyLength);
        }
        else
        {
            // Hex key written in ASCII form -- convert to raw before sending it down
            WEPkey->key_len = (pWinfo->WEPKeyLength + 1) / 2;
            hex_str_to_raw((char *)pWinfo->WEPKey[pWinfo->WEPCurKey], (char *)WEPkey->key_material, sizeof(WEPkey->key_material));
        }

        if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWENCODE)](g_wlanDev,
                                                                                NULL,
                                                                                (union iwreq_data *)&dwrq,
                                                                                (char*)WEPkey->key_material))
        {
            DPRINTF(WCDBG, (" ERROR SHIMC: SET ENCODE 4\n"));
            MEM_FREE_AND_NULL(WEPkey);
            return -1;
        }
        MEM_FREE_AND_NULL(WEPkey);
    }
    return 0;
}

int WlanClearEncryption(void)
{
	struct iw_point dwrq;

    DPRINTF(WCDBG, (" SHIMC: ENCRYPTION DISABLED\n"));
    dwrq.length = 0; 
    dwrq.flags  = IW_ENCODE_DISABLED | IW_ENCODE_NOKEY;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWENCODE)](g_wlanDev,
                                                                            NULL,
                                                                            (union iwreq_data *)&dwrq,
                                                                            NULL))
    {

        DPRINTF(WCDBG, (" ERROR SHIMC: SET ENCODE 5\n"));
        return -1;
    }
    
    if (g_pmlanPriv->sec_info.wpa_enabled == TRUE)
    {
        if( wlan_wpaoff_ioctl(g_pmlanPriv))
        {
            DPRINTF(WCDBG, ("ERROR SHIMC: WPA OFF \n")); 
            return -1;
        }
    }

	return 0;
}

#if 1
/*---------------------------------------------------------------------------*/
BOOL WlanSetInfraBandBG(void)
{
    BOOL ret = TRUE;
    struct iwreq dwrq;
    int data[3];

    // The parameters:
	//	[l]: Infrastructure band
	//	     bit 0: B
	//	     bit 1: G
	//	     bit 3: N
	//	[m]: Ad-hoc start band
	//	     bit 0: B
	//	     bit 1: G
	//	[n]: Ad-hoc start channel 

    DPRINTF(WCDBG,("WlanSetInfraBandBG\n"));

    // Read Band Info
    dwrq.u.data.length = 0;
    dwrq.u.data.pointer = &data;
    dwrq.u.data.flags = WOAL_BAND_CFG;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_SIXTEEN_INT) != 0)
    {
        DPRINTF(WCDBG, ("FAILED to read BAND CFG\n"));
        ret = FALSE;
    }
    else
    {
        // Check to see if bands already set
        if (data[0] !=  0x3)
        {
            data[0] = 0x3; // B G

            dwrq.u.data.length = 1; // Only set the Infrastructure band
            dwrq.u.data.pointer = &data;
            dwrq.u.data.flags = WOAL_BAND_CFG;

            if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_SIXTEEN_INT) != 0)
            {
                DPRINTF(WCDBG, ("FAILED to set BAND to B and G\n"));
                ret = FALSE;
            }
        }
    }

    return (ret);
}

/*---------------------------------------------------------------------------*/
BOOL WlanSetInfraBandBGN(void)
{
    BOOL ret = TRUE;
    struct iwreq dwrq;
    int data[3];

    // The parameters:
	//	[l]: Infrastructure band
	//	     bit 0: B
	//	     bit 1: G
	//	     bit 3: N
	//	[m]: Ad-hoc start band
	//	     bit 0: B
	//	     bit 1: G
	//	[n]: Ad-hoc start channel 

    DPRINTF(WCDBG,("WlanSetInfraBandBGN\n"));

    // Read Band Info
    dwrq.u.data.length = 0;
    dwrq.u.data.pointer = &data;
    dwrq.u.data.flags = WOAL_BAND_CFG;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_SIXTEEN_INT) != 0)
    {
        DPRINTF(WCDBG, ("FAILED to read BAND CFG\n"));
        ret = FALSE;
    }
    else
    {
        // Check to see if bands already set
        if (data[0] !=  0xB)
        {
            data[0] = 0xB; // B G N
        
            dwrq.u.data.length = 1; // Only set the Infrastructure band
            dwrq.u.data.pointer = &data;
            dwrq.u.data.flags = WOAL_BAND_CFG;
        
            if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_SIXTEEN_INT) != 0)
            {
                DPRINTF(WCDBG, ("FAILED to set BAND to B, G and N\n"));
                ret = FALSE;
            }
        }
    }

    return (ret);
}
#endif

/*---------------------------------------------------------------------------*/
int WlanSetAntenna(wifi_intf_antenna_t * tx_antenna, wifi_intf_antenna_t * rx_antenna)
{
    struct iwreq dwrq;
    int data;

	DPRINTF(WCDBG,
		("SHIMC: Setting Tx Antenna Diversity to %#x\n",
	 					 tx_antenna->Num));

    data = RF_ANTENNA_AUTO;

    dwrq.u.data.length = 1;
    dwrq.u.data.pointer = &data;
    dwrq.u.data.flags = WOAL_SET_GET_TX_RX_ANT;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_SIXTEEN_INT) != 0)
    {
        DPRINTF(WCDBG, ("FAILED to set TX DIVERSITY\n"));
    }
	return 0;
}

#if 0
void SetRxAntennaI(int mode)
{
    SetRxAntenna(g_wlanDev->priv, mode);
}
void SetTxAntennaI(int mode)
{
    SetTxAntenna(g_wlanDev->priv, mode);
}
void GetRxAntennaI()
{
    char buf[10];
    GetRxAntenna(g_wlanDev->priv, buf);
    DPRINTF(WCDBG, ("RX antenna: %s\n", buf));
}
void GetTxAntennaI()
{
    char buf[10];
    GetTxAntenna(g_wlanDev->priv, buf);
    DPRINTF(WCDBG, ("TX antenna: %s\n", buf));
}
#endif


/*---------------------------------------------------------------------------*/
int WlanARPFilter(void * buf, int len)
{
    struct iwreq dwrq;

    DPRINTF(WCDBG,
        ("WlanARPFilter:\n"));

    // String examples
    // "0" Remove IP address
    // "1;192.168.0.5" Set IP address for ARP filter
    // "3;192.168.0.6" Set IP address for ARP filter and auto ARP response

    dwrq.u.data.length = (uint16_t) len;
    dwrq.u.data.pointer = buf;
    dwrq.u.data.flags = WOAL_IP_ADDRESS;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SET_GET_256_CHAR) != 0)
    {
        DPRINTF(WCDBG, ("FAILED WOAL_ARP_FILTER \n"));
    }
    return 0;
}


/*---------------------------------------------------------------------------*/
int WlanUSBSuspend(void * remote_wakeup_callback)
{
#if 0
    struct iwreq dwrq;

	DPRINTF(WCDBG,
		("SHIMC: WlanUSBSuspend\n"));

    dwrq.u.data.length = 0;
    dwrq.u.data.pointer = NULL;
    dwrq.u.data.flags = WOAL_USB_SUSPEND;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SETNONE_GETNONE) != 0)
    {
        DPRINTF(WCDBG, ("FAILED WOAL_USB_SUSPEND \n"));
    }
	return 0;
#else
    return (wlan_usb_suspend(remote_wakeup_callback));
#endif
}


/*---------------------------------------------------------------------------*/
int WlanUSBResume(void)
{
#if 0
    struct iwreq dwrq;

	DPRINTF(WCDBG,
		("SHIMC: WlanUSBResume\n"));

    dwrq.u.data.length = 0;
    dwrq.u.data.pointer = NULL;
    dwrq.u.data.flags = WOAL_USB_RESUME;

    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&dwrq, WOAL_SETNONE_GETNONE) != 0)
    {
        DPRINTF(WCDBG, ("FAILED WOAL_USB_RESUME\n"));
    }
	return 0;
#else
    wlan_usb_resume();

	return 0;
#endif
}

/*---------------------------------------------------------------------------*/
int WlanSetUSBPort(int port)
{
    wlan_set_usb_port(port);

    return 0;
}

/*---------------------------------------------------------------------------*/
int WlanSetRate(wifi_intf_info_t *pWinfo)
{
	struct iw_param dwrq;
	
	/* THIS Value has to be defined in the DataStore
	 */
	//dwrq->value = pWinfo->value; 

	//if(wlan_set_rate(g_wlanDev, NULL, &dwrq, NULL))
    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWRATE)](g_wlanDev, 
                                                                          NULL, 
                                                                          (union iwreq_data *)&dwrq, 
                                                                          NULL))
    {
		DPRINTF(WCDBG, ("\n Failed to Set the Rate\n"));
		return -1;
	}

	return 0;
}
/*---------------------------------------------------------------------------*/
int WlanGetRate(void)
{
	struct iw_param dwrq;

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCGIWRATE)](g_wlanDev, 
                                                                          NULL, 
                                                                          (union iwreq_data *)&dwrq, 
                                                                          NULL))
    {
		DPRINTF(WCDBG, ("\n Failed to Get the Rate\n"));
		return -1;
	}

	return dwrq.value;
}
/*---------------------------------------------------------------------------*/
int WlanSetRadio(BOOL radioOn)
{
	char value = 0;
	
	//RADIO_ON = 0;
	//RADIO_OFF = 1;
    if(!radioOn)
        value = 1;
	
    if(woal_set_radio(g_wlanDev->priv, value) != 0)    
    {
		DPRINTF(WCDBG, ("\n Failed to Set the Radio\n"));
		return -1;
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
/* Please update this to the correct values in dbm */
unsigned int	Region_TPowerTable[][2]=
				{ 
				 {REG_US ,14},
				 {REG_EUR,14},
				 {REG_CAN,14},
				 {REG_ESP,14},
				 {REG_FRA,14},
				 {REG_JPN,14},
				};

int WlanSetRegion(uint8_t region_code)
{
	unsigned short  i;
	struct iw_param dwrq;
	int max_tx_power;
	int r_code;
    struct iwreq rwrq;
	
	DPRINTF(WCDBG, ("Process SET REGION  = %d \n", region_code));

	for( i =0;i<sizeof(Region_TPowerTable)/
			sizeof(Region_TPowerTable[0]);i++){
		if(region_code == Region_TPowerTable[i][0])
			break;
	}

	if(i == sizeof(Region_TPowerTable)/sizeof(Region_TPowerTable[0])){
		i=0;
	}
	r_code = Region_TPowerTable[i][0];

	DPRINTF(WCDBG, (" REGION CODE value = %d  %d \n"
						, i, region_code));

    /* Set the region in the driver
	 */
    rwrq.u.data.flags = WOAL_SET_GET_REGIONCODE;
    rwrq.u.data.length = 1;
    rwrq.u.data.pointer = &r_code;
    if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&rwrq, WOAL_SETONEINT_GETONEINT) != 0)
    {
		DPRINTF(WCDBG, (" FAILED TO SET REGION \n"));
		return -1;
	}

	/* Set the maximum power level depending on region */
	max_tx_power = Region_TPowerTable[i][1];

	dwrq.value = max_tx_power;
	dwrq.fixed = 1;
	dwrq.disabled = 0;
	dwrq.flags = 0; /* IW_TXPOW_DBM; */

	DPRINTF(WCDBG, (" REGION Power Level = %d dbm\n", max_tx_power));

    if (g_wlanDev->wireless_handlers->standard[IW_IOCTL_IDX(SIOCSIWTXPOW)](g_wlanDev, 
                                                                           NULL, 
                                                                           (union iwreq_data *)&dwrq, 
                                                                           NULL) != 0)
    {
		DPRINTF(WCDBG, (" FAILED TO SET TXPOWER \n"));
		return -1;
	}

	return 0;
}

/*---------------------------------------------------------------------------*/
int WlanGetWwsCfg(uint8_t *wws_config)
{
    int ret = 0;
    if (WlanIsWirelessHWActive())
    {
        struct iwreq rwrq;
        uint8_t curr_wws_config;

        DPRINTF(WCDBG, ("ACQUIRING WORLD WIDE SAFE CONFIG \n"));
   
        rwrq.u.data.flags = WOAL_SET_GET_WWS_CFG;
        rwrq.u.data.length = 0;
        rwrq.u.data.pointer = &curr_wws_config;
        if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&rwrq, WOAL_SETONEINT_GETONEINT) != 0)
        {
            DPRINTF(WCDBG, (" FAILED TO GET WWS CONFIG \n"));
            ret = -1;
        } 

        *wws_config = curr_wws_config;
        DPRINTF(WCDBG, ("WORLD WIDE SAFE CONFIG: %x \n", *wws_config));
    }
    else
        ret = -1;    
    
    return ret;
}

/*---------------------------------------------------------------------------*/
int WlanSetWwsCfg(uint8_t wws_config)
{
    int ret = 0;
    if (WlanIsWirelessHWActive())
    {
        struct iwreq rwrq;
        uint8_t curr_wws_config;

        curr_wws_config = (wws_config != 0); 
        DPRINTF(WCDBG, ("SETTING WORLD WIDE SAFE CONFIG to %x\n", curr_wws_config));
       
        rwrq.u.data.flags = WOAL_SET_GET_WWS_CFG;
        rwrq.u.data.length = 1;
        rwrq.u.data.pointer = &curr_wws_config;
        if (g_wlanDev->do_ioctl(g_wlanDev, (struct ifreq *)&rwrq, WOAL_SETONEINT_GETONEINT) != 0)
        {
            DPRINTF(WCDBG, (" FAILED TO SET WWS CONFIG \n"));
            ret = -1;
        }
    }
    else 
       ret = -1;    

    return ret;
}

/*---------------------------------------------------------------------------*/
/* Get Scan Results ..............*/
int  WlanGetScan()
{
    struct iwreq iwr;
    t_u8 *scan_rsp_buf = NULL;

    unsigned int scan_start;
    int idx, ret = 0, i, j;

    t_u8 *pcurrent;
    t_u8 *pnext;
    IEEEtypes_ElementId_e *pelement_id;
    t_u8 *pelement_len;
    int ssid_idx;
    char ssid[33];
    int ssid_len = 0;
    bool isBlank = true;

    IEEEtypes_CapInfo_t cap_info;
    t_u16 tmp_cap;
    t_u8 tsf[8];
    t_u16 beacon_interval;

    IEEEtypes_VendorSpecific_t *pwpa_ie = NULL;
    const t_u8 wpa_oui[4] = { 0x00, 0x50, 0xf2, 0x01 };

    IEEEtypes_WmmParameter_t *pwmm_ie = NULL;
    const t_u8 wmm_oui[4] = { 0x00, 0x50, 0xf2, 0x02 };
    IEEEtypes_VendorSpecific_t *pwps_ie = NULL;
    IEEEtypes_VendorSpecific_t *pwps_ie_save = NULL;
    const t_u8 wps_oui[4] = { 0x00, 0x50, 0xf2, 0x04 };
    IEEEtypes_Generic_t *prsn_ie = NULL;
    char wmm_cap;
    char wps_cap;
    char dot11k_cap;
    char dot11r_cap;
    char priv_cap;
    char ht_cap;

    int displayed_info;

    wlan_ioctl_get_scan_table_info *prsp_info;

    wlan_get_scan_table_fixed fixed_fields;
    t_u32 fixed_field_length;
    t_u32 bss_info_length;
    wifi_intf_scan_info_t * scan_info;
    int dsIndex, dBm;

    struct wpa_ie_data data; //to filter out WPA/WPA2 enterprise networks

    DPRINTF(WCDBG, ("SHIMC: GETSCAN FUNCTION  \n"));

    memset(&cap_info, 0x00, sizeof(cap_info));
    scan_rsp_buf = (t_u8 *) MEM_MALLOC(SCAN_RESP_BUF_SIZE);
    if (scan_rsp_buf == NULL) {
        DPRINTF(WCDBG,("Error: allocate memory for scan_rsp_buf failed\n"));
        return -ENOMEM;
    }
    prsp_info = (wlan_ioctl_get_scan_table_info *) scan_rsp_buf;

    scan_info = (wifi_intf_scan_info_t *)MEM_MALLOC(sizeof(wifi_intf_scan_info_t));
    if (scan_info == NULL) {
        DPRINTF(WCDBG,("Error: allocate memory for scan_info failed\n"));
        return -ENOMEM;
    } 
  	memset (scan_info, '\0', sizeof(wifi_intf_scan_info_t)); 

    displayed_info = FALSE;
    scan_start = 1;

    DPRINTF(WCDBG,("---------------------------------------"));
    DPRINTF(WCDBG,("---------------------------------------\n"));
    DPRINTF(WCDBG,("# | ch  | dBm | ss  |       bssid       |   cap    |   SSID \n"));
    DPRINTF(WCDBG,("---------------------------------------"));
    DPRINTF(WCDBG,("---------------------------------------\n"));

    dsIndex = 0;

    do 
    {
        prsp_info->scan_number = scan_start;

        /* 
         * Set up and execute the ioctl call
         */
        //strncpy(iwr.ifr_name, dev_name, IFNAMSIZ - 1);
        iwr.u.data.pointer = (caddr_t) prsp_info;
        iwr.u.data.length = SCAN_RESP_BUF_SIZE;
        iwr.u.data.flags = WOAL_GET_SCAN_TABLE;
        if (g_wlanDev->do_ioctl(g_wlanDev,(struct ifreq *) &iwr, WOAL_SET_GET_2K_BYTES) < 0)
        {
            goto done;            
        }    

        pcurrent = 0;
        pnext = prsp_info->scan_table_entry_buf;

        for (idx = 0; (unsigned int) idx < prsp_info->scan_number; idx++) {

            /* 
             * Set pcurrent to pnext in case pad bytes are at the end
             *   of the last IE we processed.
             */
            pcurrent = pnext;

            memcpy((t_u8 *) & fixed_field_length,
                   (t_u8 *) pcurrent, sizeof(fixed_field_length));
            pcurrent += sizeof(fixed_field_length);

            memcpy((t_u8 *) & bss_info_length,
                   (t_u8 *) pcurrent, sizeof(bss_info_length));
            pcurrent += sizeof(bss_info_length);

            memcpy((t_u8 *) & fixed_fields,
                   (t_u8 *) pcurrent, sizeof(fixed_fields));
            pcurrent += fixed_field_length;

            /* Set next to be the start of the next scan entry */
            pnext = pcurrent + bss_info_length;
                                   
            if (bss_info_length >=
                (sizeof(tsf) + sizeof(beacon_interval) + sizeof(cap_info))) {
                /* time stamp is 8 byte long */
                memcpy(tsf, pcurrent, sizeof(tsf));
                pcurrent += sizeof(tsf);
                bss_info_length -= sizeof(tsf);

                /* beacon interval is 2 byte long */
                memcpy(&beacon_interval, pcurrent, sizeof(beacon_interval));
                /* endian convert needed here */
                //beacon_interval = le16_to_cpu(beacon_interval);
                pcurrent += sizeof(beacon_interval);
                bss_info_length -= sizeof(beacon_interval);

                /* capability information is 2 byte long */
                memcpy(&tmp_cap, pcurrent, sizeof(tmp_cap));
                /* endian convert needed here */
                //tmp_cap = le16_to_cpu(tmp_cap);
                memcpy(&cap_info, &tmp_cap, sizeof(cap_info));
                pcurrent += sizeof(cap_info);
                bss_info_length -= sizeof(cap_info);
            }


            wmm_cap = ' ';      /* M (WMM), C (WMM-Call Admission Control) */
            wps_cap = ' ';      /* "S" */
            dot11k_cap = ' ';   /* "K" */
            dot11r_cap = ' ';   /* "R" */
            ht_cap = ' ';       /* "N" */

            /* "P" for Privacy (WEP) since "W" is WPA, and "2" is RSN/WPA2 */
            priv_cap = cap_info.privacy ? 'P' : ' ';

            memset(ssid, 0, MRVDRV_MAX_SSID_LENGTH + 1);
            ssid_len = 0;
            while (bss_info_length >= 2) {
                pelement_id = (IEEEtypes_ElementId_e *) pcurrent;
                pelement_len = pcurrent + 1;
                pcurrent += 2;

                switch (*pelement_id) {

                case SSID:
                    if (*pelement_len &&
                        *pelement_len <= MRVDRV_MAX_SSID_LENGTH) {
                        memcpy(ssid, pcurrent, *pelement_len);
                        ssid_len = *pelement_len;
                    }
                    break;

                case WPA_IE:
                    pwpa_ie = (IEEEtypes_VendorSpecific_t *) pelement_id;
                    if ((memcmp
                         (pwpa_ie->vend_hdr.oui, wpa_oui,
                          sizeof(pwpa_ie->vend_hdr.oui)) == 0)
                        && (pwpa_ie->vend_hdr.oui_type == wpa_oui[3])) {
                        /* WPA IE found, 'W' for WPA */
                        priv_cap = 'W';

                        memset(&data, 0x0, sizeof(data));
                        wpa_parse_wpa_ie((const u8 *)pwpa_ie, 
                                         (size_t) (*(pwpa_ie)).vend_hdr.len + 2, 
                                         &data);

                        if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X)
                            priv_cap = 'X'; //wpa_enterprise
                        
                    } else {
                        pwmm_ie = (IEEEtypes_WmmParameter_t *) pelement_id;
                        if ((memcmp(pwmm_ie->vend_hdr.oui,
                                    wmm_oui,
                                    sizeof(pwmm_ie->vend_hdr.oui)) == 0)
                            && (pwmm_ie->vend_hdr.oui_type == wmm_oui[3])) {
                            /* Check the subtype: 1 == parameter, 0 == info */
                            if ((pwmm_ie->vend_hdr.oui_subtype == 1)
                                && pwmm_ie->ac_params[WMM_AC_VO].aci_aifsn.acm) {
                                /* Call admission on VO; 'C' for CAC */
                                wmm_cap = 'C';
                            } else {
                                /* No CAC; 'M' for uh, WMM */
                                wmm_cap = 'M';
                            }
                        } else {
                            pwps_ie =
                                (IEEEtypes_VendorSpecific_t *) pelement_id;
                            if ((memcmp
                                 (pwps_ie->vend_hdr.oui, wps_oui,
                                  sizeof(pwps_ie->vend_hdr.oui)) == 0)
                                && (pwps_ie->vend_hdr.oui_type == wps_oui[3])) {
                                wps_cap = 'S';
                                if (pwps_ie_save == NULL)
                                    pwps_ie_save = pwps_ie;
                            }
                        }
                    }
                    break;

                case RSN_IE:
                    /* RSN IE found; '2' for WPA2 (RSN) */
                    prsn_ie = (IEEEtypes_Generic_t *) pelement_id;
                    priv_cap = '2';
                    memset(&data, 0x0, sizeof(data));
                    wpa_parse_wpa_ie((const u8 *)prsn_ie, 
                                         (size_t) (*(prsn_ie)).ieee_hdr.len + 2, 
                                         &data);

                    if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X)
                        priv_cap = '3'; //wpa2_enterprise
                        
                    break;
                case HT_CAPABILITY:
                    ht_cap = 'N';
                    break;

                default:
                    break;
                }

                pcurrent += *pelement_len;
                bss_info_length -= (2 + *pelement_len);
            }

            isBlank = true; 
            //Check for blank ssids
            for (i = 0; i < ssid_len; i++)
            {
                if (ssid[i] != ' ' && ssid[i] != '\0')
                {
                    isBlank = false;
                    break;
                }
            }

            // Only add scan results we actually want in the list of things to connect to.
            // Filter our hidden (blank) SSIDs and other ad hoc devices.
            
            if (!isBlank && !cap_info.ibss)
            {
                //Store the communication mode in data store
                scan_info->networks[dsIndex].communicationMode = INFRASTRUCTURE;

                //Store security
                if (priv_cap == 'P') {
                    scan_info->networks[dsIndex].securityMode = WEP;
                } else if (priv_cap == 'W') {
                    scan_info->networks[dsIndex].securityMode = WPA_PSK;                    
                } else if (priv_cap == 'X') {
                    scan_info->networks[dsIndex].securityMode = WPA_ENTERPRISE;
                } else if (priv_cap == '2') {
                    scan_info->networks[dsIndex].securityMode = WPA2_PSK;
                } else if (priv_cap == '3') {
                    scan_info->networks[dsIndex].securityMode = WPA2_ENTERPRISE;
                } else {
                    scan_info->networks[dsIndex].securityMode = OPEN_SYSTEM;                    
                }

                //WPS Support
                if (wps_cap == 'S'){
                    UINT16 configMethods = 0x0000;
                    UINT16 passwordID;
                    int status;

                    // All WPS capable devices are required to support PIN mode
                    scan_info->networks[dsIndex].wifiOptions = WPS_PIN;

                    status = wps_probe_response_selected_registrar_config_method_parser((u8 *)pwps_ie_save, pwps_ie_save->vend_hdr.len, &configMethods);
                    if (status == WPS_STATUS_SUCCESS) {
                        if (configMethods != 0xFFFF) {
                            if (configMethods & 0x0080) { // CONFIG_METHOD_PUSHBUTTON == 0x0080 in wps_msg.h
                                scan_info->networks[dsIndex].wifiOptions |= WPS_PUSH_BUTTON;
                            }
                        } else {
                            // They didn't specify which methods they support (it's optional).
                            // Assume they support both methods so we don't keep people from being able to try.
                            scan_info->networks[dsIndex].wifiOptions |= (WPS_PIN | WPS_PUSH_BUTTON);
                        }
                    } 
                      
                    passwordID = wps_probe_response_device_password_id_parser((u8 *)pwps_ie_save, pwps_ie_save->vend_hdr.len);
                    if (passwordID != PASSWORD_ID_NOT_PRESENT) {
                        if (passwordID == DEVICE_PASSWORD_PUSH_BUTTON) {
                            scan_info->networks[dsIndex].wifiOptions |= (WPS_PUSH_BUTTON | WPS_PBC_ACTIVE);
                        }
                    }
                }
                pwps_ie_save = NULL;

                dBm = 0 - fixed_fields.rssi;
                if (dBm < -100) dBm = -100;
                if (dBm > 0)    dBm = 0; 
                scan_info->networks[dsIndex].dBm = (SINT8)dBm;
                scan_info->networks[dsIndex].signalStrength = rssi_dbm_to_signal_strength(dBm);
                scan_info->networks[dsIndex].channel = (uint8_t)fixed_fields.channel;
                        
                DPRINTF(WCDBG,("%02u| %03d | %03d | %03d | %02x:%02x:%02x:%02x:%02x:%02x |",
                   scan_start + idx,
                   fixed_fields.channel,
                   dBm,
                   scan_info->networks[dsIndex].signalStrength,
                   fixed_fields.bssid[0],
                   fixed_fields.bssid[1],
                   fixed_fields.bssid[2],
                   fixed_fields.bssid[3],
                   fixed_fields.bssid[4], fixed_fields.bssid[5]));

                displayed_info = TRUE;
            
                /* "A" for Adhoc "I" for Infrastructure, "D" for DFS (Spectrum
                   Mgmt) */
                DPRINTF(WCDBG,(" %c%c%c%c%c%c%c%c | ", cap_info.ibss ? 'A' : 'I', priv_cap, /* P 
                                                                                   (WEP), 
                                                                                   W 
                                                                                   (WPA), 
                                                                                   2 
                                                                                   (WPA2) 
                                                                                 */
                                   cap_info.spectrum_mgmt ? 'D' : ' ', wmm_cap, /* M (WMM), C
                                                                   (WMM-Call
                                                                   Admission
                                                                   Control) */
                   dot11k_cap,  /* K */
                   dot11r_cap,  /* R */
                   wps_cap,     /* S */
                   ht_cap));     /* N */

            
               /* Print out the ssid or the hex values if non-printable */
               for (ssid_idx = 0; ssid_idx < ssid_len; ssid_idx++) {
                   if (isprint(ssid[ssid_idx])) {
                       DPRINTF(WCDBG,("%c", ssid[ssid_idx]));
                   } else {
                       DPRINTF(WCDBG,("\\%02x", ssid[ssid_idx]));
                   }                
               }

               // Store the SSID in data store
               memcpy(scan_info->networks[dsIndex].SSIDNames, ssid, ssid_len);
               scan_info->networks[dsIndex].SSIDNames[ssid_len] = '\0';

               //Store the BSSID
               memcpy(scan_info->networks[dsIndex].BSSID, fixed_fields.bssid, BSSID_LENGTH);

               DPRINTF(WCDBG,("\n"));

               dsIndex++;
            }                       
        }

        scan_start += prsp_info->scan_number;

    } while (prsp_info->scan_number);

    scan_info->NumSSID = dsIndex;
    scan_info->minSignalStrength = 1;
    scan_info->maxSignalStrength = 5;
    
    if (displayed_info == TRUE) {
        {
            DPRINTF(WCDBG,("\n\n"
                   "Capability Legend (Not all may be supported)\n"
                   "-----------------\n"
                   " I [ Infrastructure ]\n"
                   " A [ Ad-hoc ]\n"
                   " P [ WEP ]\n"
                   " W [ WPA IE ]\n"
                   " X [ WPA Enterprise ]\n"
                   " 2 [ WPA2/RSN IE ]\n"
                   " 3 [ WPA2 Enterprise ]\n"
                   " M [ WMM IE ]\n"
                   " C [ Call Admission Control - WMM IE, VO ACM set ]\n"
                   " D [ Spectrum Management - DFS (11h) ]\n"
                   " K [ 11k ]\n"
                   " R [ 11r ]\n" " S [ WPS ]\n" " N [ HT (11n) ]\n" "\n\n"));
        }
    } else {
        DPRINTF(WCDBG,("< No Scan Results >\n"));
    }

    // Be nice and sort the results for everyone
    for (i = 0; i < scan_info->NumSSID; i++)
    {
        for (j = i + 1; j < scan_info->NumSSID; j++)
        {
            if (scan_info->networks[i].dBm < scan_info->networks[j].dBm)
            {
                wifi_intf_network_info_t temp;
                temp = scan_info->networks[i];
                scan_info->networks[i] = scan_info->networks[j];
                scan_info->networks[j] = temp;
            }
        }
        scan_info->networks[ i ].rank = i + 1;
    }

	/* WRITE TO DATASTORE;
	 */
    wifi_intf_set_var_scan_info(scan_info);
    wifi_intf_set_var_is_wireless_scanning(FALSE);

    if (g_after_power_up_event == POWER_DOWN_AFTER_SCAN)
    {
        g_after_power_up_event = NONE;
        WlanShutdown();
    }

  done:
    if (scan_rsp_buf)
        MEM_FREE_AND_NULL(scan_rsp_buf);
    if (scan_info)
        MEM_FREE_AND_NULL(scan_info);
    return ret; 
}

void WlanWirelessHWInit(void)
{
    uint32_t counter = 0;

    // Turning on the hardware to allow us to get the MAC address and
    // store scan information in the datastore.
    WlanStartup();

    // Give time for module to turn on
    counter = 0;
    while ( ( !WlanIsWirelessHWActive() ) &&
            ( counter < 3000 ) )
    {
        counter += 5;
        tx_thread_sleep( 5 );
    }

    if (WlanIsWirelessHWActive())
    {
        tx_mutex_get(&wlan_scan_mutex, TX_WAIT_FOREVER);
    
        // Request a scan
        //WlanShimCSetScanWithSsid(NULL, 0);
        WlanSetUserScanCfg(NULL, 0);
        
        // Store the results in the DataStore
        WlanGetScan();
    
        tx_mutex_put(&wlan_scan_mutex);
    }
}

static void get_printer_model_number_str(char *p_model_number, int max_len)
{
    char* pStrVal;
    int i,j,k;

    platvars_get_prtnamestr(&pStrVal);

    // Search for Model Number
    for (i=0; i<strlen(pStrVal); i++)
    {
        // Find string with numbers
        if ((pStrVal[i] >= '0') && (pStrVal[i] <= '9'))
        {
            // Find start of string
            for (j=i; j>0; j--)
            {
                if (pStrVal[j] == ' ')
                {
                    j++;
                    break;
                }
            }

            // Find length of string
            for (k=i; k<strlen(pStrVal); k++)
            {
                // Exclude trailing lower case letters
                if ((pStrVal[k] == ' ') ||
                    ((pStrVal[k] >= 'a') && (pStrVal[k] <= 'z')))
                    break;
            }

            if (k>j)
            {
                memset( p_model_number, 0x00, MIN(k - j + 1, max_len) );
                memcpy( p_model_number, pStrVal + j, MIN(k - j, max_len - 1) );
            }
            break;
        }
    }
}

/** 
 * \brief Sets the default SSID if none has been set yet.
 * 
 * This function checks to see if a valid SSID is stored.  If
 * not, it creates and stores the default SSID 
 * ("<model #>-<6 digits of MAC>" where the model #
 * excludes the trailing lower-case letters.
 * 
 * \return 1 if the SSID was reset to default, 0 otherwise
 * 
 * */
int wirelessEnsureValidStaSSID()
{
    int retVal = 0;
    uint8_t ssid_buf[MAX_SSID_NAME_LEN];
    char macOnBoard[6];
    wifi_intf_comm_mode_t comm_mode;

    wifi_intf_get_var_ssid(WIFI_IFACE_ID_STATION, ssid_buf, MAX_SSID_NAME_LEN);
    wifi_intf_get_var_comm_mode(WIFI_IFACE_ID_STATION, &comm_mode);

    if ((strlen((char*)ssid_buf) == 0) && (comm_mode == AD_HOC))
    {
        strcpy((char *)ssid_buf, "");
        get_printer_model_number_str((char *)ssid_buf, MAX_SSID_NAME_LEN);

        WlanGetMacAddr(WIFI_IFACE_NAME_STATION, macOnBoard);
        minSprintf((char *)ssid_buf + strlen((char *)ssid_buf), "-%02X%02X%02X", macOnBoard[3], macOnBoard[4], macOnBoard[5]);

        // Save the new value
        wifi_intf_set_var_ssid(WIFI_IFACE_ID_STATION, ssid_buf, MAX_SSID_NAME_LEN);

        // Let caller know we reset the SSID
        retVal = 1;
    }
    else
    {
        // Let caller know that nothing happened
        retVal = 0;
    }
    return retVal;
}

/** 
 * \brief Sets the default SSID if none has been set yet.
 * 
 * This function checks to see if a valid SSID is stored.  If
 * not, it creates and stores the default SSID 
 * 
 * \return 1 if the SSID was reset to default, 0 otherwise
 * 
 * */
int wirelessEnsureValidUapSSID()
{
    return wifi_oem_update_uap_ssid();    
}

/** 
 * \brief Sets the default SSID if none has been set yet.
 * 
 * This function checks to see if a valid SSID is stored.  If
 * not, it creates and stores the default SSID 
 * 
 * \return 1 if the SSID was reset to default, 0 otherwise
 * 
 * */
int wirelessEnsureValidWfdSSID()
{
    int ssid_was_modified = 0;
    uint8_t ssid_data[MAX_SSID_NAME_LEN];

    wifi_intf_get_var_ssid_prefix(WIFI_IFACE_ID_WFD, ssid_data, sizeof(ssid_data));
    if (strlen((char *)ssid_data) == 0)
    {
        strcpy((char *)ssid_data, WIFIDIRECT_CONFIG_GROUP_ID_GROUP_SSID_DEFAULT);

        wifi_intf_set_var_ssid_prefix(WIFI_IFACE_ID_WFD, ssid_data, strlen((char *)ssid_data));
        ssid_was_modified = 1;
    }

    wifi_intf_get_var_ssid(WIFI_IFACE_ID_WFD, ssid_data, sizeof(ssid_data));
    if (strlen((char *)ssid_data) == 0)
    {
        strcpy((char *)ssid_data, " ");

        wifi_intf_set_var_ssid(WIFI_IFACE_ID_WFD, ssid_data, strlen((char *)ssid_data));
        ssid_was_modified = 1;
    }

    return ssid_was_modified;
}

/*---------------------------------------------------------------------------*/
void wcfgInfoTimerFunc(ULONG iface_id)
{
	MESSAGE nxtMsg;
	nxtMsg.msgType = MSG_WIRELESS_UPDATE;
    nxtMsg.param1  = iface_id;

	DPRINTF(WCDBG, ("Enter: wconfig Timer function %d\n", iface_id));

    tx_queue_send(&wconfigMsgQ, &nxtMsg, TX_NO_WAIT);
}

/*---------------------------------------------------------------------------*/
void wcfgAdHocRejoinTimerFunc(ULONG unused)
{
	MESSAGE nxtMsg;
	nxtMsg.msgType = MSG_WIRELESS_ATTEMPT_REJOIN;

    if (WlanIsWirelessHWActive())
    {
        tx_queue_send(&wconfigMsgQ, &nxtMsg, TX_NO_WAIT);
    }
}

/*---------------------------------------------------------------------------*/
void wifi_sta_badwepkey_timer_expired(ULONG unused)
{
    if (wifi_sta_badwepkey_timer == NULL)
        return;

    DPRINTF(WCDBG, ("wifi_sta_badwepkey_timer_expired: Timed Out! Setting Wifi State to Link Error!\n"));

    //Set Link Status to Link Error
    wifi_intf_sta_set_state(WLAN_BAD_KEY_ERROR);
}

/*---------------------------------------------------------------------------*/
int wconfigMsgProc( MESSAGE *msg )
{
    BOOL power_is_on;

    if(msg->msgType == MSG_THREAD_EXIT)
    {
        return 1;
    }

    EnsureCorrectRadioPowerState(&power_is_on);

    if(!power_is_on)
    {
        DPRINTF(WCDBG, ("Exit: wconfig Update function - HW disabled\n"));

        if (msg->msgType == MSG_WIRELESS_START_SCAN)
        {
            bool isScanning = FALSE;
            wifi_intf_get_var_is_wireless_scanning(&isScanning);
        }

        return 0;
    }

    if(msg->msgType == MSG_WIRELESS_UPDATE)
    {
        if (msg->param1 == WIFI_IFACE_ID_STATION)
        {
            wifi_intf_comm_mode_t sta_comm_mode;
            wifi_intf_get_var_comm_mode(WIFI_IFACE_ID_STATION, &sta_comm_mode);

            // When they change STA settings we need to see if they are going
            // into or out of ad hoc mode and reconfigure the uAP accordingly
            if ((gWifiUapDisabledByAdHocSta) && (sta_comm_mode == INFRASTRUCTURE))
            {
                // uAP used to be disabled, but doesn't need to be any more.
                // First configure the STA (to get it out of ad hoc) then allow
                // the uAP to start.
                gWifiUapDisabledByAdHocSta = FALSE;
                wConfigApplyStaSettings();
                wConfigApplyUapSettings();
            }
            else if ((!gWifiUapDisabledByAdHocSta) && (sta_comm_mode == AD_HOC))
            {
                // STA is going into ad hoc mode.  First take down the uAP, then
                // let the STA configure itself as ad hoc
                gWifiUapDisabledByAdHocSta = TRUE;
                wConfigApplyUapSettings();
                wConfigApplyStaSettings();
            }
            else
            {
                wConfigApplyStaSettings();
            }
        }
        else if (msg->param1 == WIFI_IFACE_ID_UAP || msg->param1 == WIFI_IFACE_ID_UAP_PRI0 )
        {
            //wConfigApplyWfdSettings();
            wConfigApplyUapSettings();
        }
        else if (msg->param1 == WIFI_IFACE_ID_WFD)
        {
            //wConfigApplyUapSettings();
            wConfigApplyWfdSettings();
        }
    }
    else if (msg->msgType == MSG_WIRELESS_START_SCAN)
    {
        unsigned char *ssid;
        DPRINTF(WCDBG, ("wconfigMsgProc starting requested scan\n"));

        tx_mutex_get(&wlan_scan_mutex, TX_WAIT_FOREVER);
        ssid = (unsigned char *)msg->param1;
        //WlanShimCSetScanWithSsid((char*)ssid, (int)msg->param2);

        WlanSetUserScanCfg((char*)ssid, (int)msg->param2);
        
        // Store the results in the DataStore
        WlanGetScan();

        if (ssid)
            MEM_FREE_AND_NULL(ssid);

        tx_mutex_put(&wlan_scan_mutex);
    }
    else if (msg->msgType == MSG_WIRELESS_ATTEMPT_REJOIN)
    {
        if (!WlanIsConnected())
        {
            mlan_adapter *adapter = g_pmlanPriv->adapter;
            BOOL bAdHocNetworkAvailable = FALSE;
            int i;

            // Scan to see if the network came back
            tx_mutex_get(&wlan_scan_mutex, TX_WAIT_FOREVER);

            uint8_t ssid_buf[MAX_SSID_NAME_LEN];
            wifi_intf_get_var_ssid(WIFI_IFACE_ID_STATION, ssid_buf, MAX_SSID_NAME_LEN);

            //WlanShimCSetScanWithSsid((char*)ssid_buf, strlen((char*)ssid_buf));
            WlanSetUserScanCfg((char*)ssid_buf, strlen((char*)ssid_buf)); 

            for (i = 0; i < adapter->num_in_scan_table; i++)
            {
                //if ((adapter->pscan_table[i].InfrastructureMode == Wlan802_11IBSS) &&
                if ((g_pmlanPriv->bss_mode == MLAN_BSS_MODE_IBSS) &&
                    (strncmp((char*)ssid_buf, (char*)adapter->pscan_table[i].ssid.ssid, adapter->pscan_table[i].ssid.ssid_len) == 0))
                {
                    bAdHocNetworkAvailable = TRUE;
                }
            }

            tx_mutex_put(&wlan_scan_mutex);

            if (bAdHocNetworkAvailable)
            {
                // Re-apply the settings (it will join)
                wConfigApplyStaSettings();
            }
            else
            {
                // Set up the rejoin timer to check again later
                tx_timer_deactivate(&wcfgAdHocRejoinTimer);
                tx_timer_change(&wcfgAdHocRejoinTimer, AD_HOC_REJOIN_TIME, 0);
                tx_timer_activate(&wcfgAdHocRejoinTimer);
            }
        }
    }
    else
    {
        DPRINTF(WCDBG, ("WARNING: wconfigMsgProc received unexpected msg: %#x\n", msg->msgType));
    }
    return 0;
}


/*---------------------------------------------------------------------------*/
void wtimerFunc(void *function_context)
{
    tx_queue_send(&wtimerMsgQ, function_context, TX_NO_WAIT);
}


/*---------------------------------------------------------------------------*/
void wtimerThreadFunc(ULONG unused)
{
    MESSAGE wtimerMsg;
    void (*wtimer_func)(void*);

    while(TRUE)
    {
        /* Wait on a message
         */
        if(TX_SUCCESS == tx_queue_receive(&wtimerMsgQ, &wtimerMsg, TX_WAIT_FOREVER))
        {
            // Check for exit message
            if(wtimerMsg.msgType == MSG_THREAD_EXIT)
                return;

            /* Call original timer function on this thread */
            wtimer_func = (void *) wtimerMsg.param1;
            wtimer_func((void *) wtimerMsg.param2);
        }
    }
}

/*---------------------------------------------------------------------------*/
void wconfigThreadFunc()
{
    MESSAGE wconfigMsg;
    wifi_intf_comm_mode_t comm_mode;

    DPRINTF(WCDBG, ("WLAN: ==> wconfigThreadFunc\n"));

    // Power up, grab the MAC address, etc (need to do this before network
    // loads, which is why it's at init level 2)
    WlanWirelessHWInit();

    // Wait for the rest of the system to come up before we start 
    // integrating to other components and running WiFi
    SysWaitForInit();
 
    // Make it possible to configure wifi via commands
    wireless_register_cmds();

    // We need to keep track of whether the uAP should be disabled by the STA
    // in ad hoc mode.  Initialize it here before we enable/disable
    wifi_intf_get_var_comm_mode(WIFI_IFACE_ID_STATION, &comm_mode);
    if (comm_mode == AD_HOC)
    {
        gWifiUapDisabledByAdHocSta = TRUE;
    }
    else
    {
        gWifiUapDisabledByAdHocSta = FALSE;
    }

    EnsureCorrectRadioPowerState(NULL);

    while(TRUE)
    {
        /* Wait on a message
         */
        if(TX_SUCCESS == tx_queue_receive(&wconfigMsgQ,
                                          &wconfigMsg,TX_WAIT_FOREVER))
        {
            /* Process the message
             */
            if(wconfigMsgProc(&wconfigMsg))
            {
                // if the message processor receives an indication that the thread
                // should exit it will return true (1).
                break;
            }
        }
    }

    //DSNotifyUnregister(e_WirelessInfo, WIRELESSCONFIGID);

    DPRINTF(WCDBG, ("WLAN: <== wconfigThreadFunc\n"));
}

void wifi_change_uap_active_config(uint8_t iface_id)
{
    if (iface_id == WIFI_IFACE_ID_UAP_PRI0)
    {
        DPRINTF(WCDBG, ("WLAN: Changing uAP active config to UAP Pri0\n"));
    }
    else if (iface_id == WIFI_IFACE_ID_UAP)
    {
        DPRINTF(WCDBG, ("WLAN: Changing uAP active config to UAP\n"));
    }    
    g_wifi_uap_current_active_config = iface_id;
}

uint8_t wifi_get_uap_active_config()
{
    return g_wifi_uap_current_active_config;
}

void wifi_set_scan_config(uint8_t scan_config)
{
    gWifiScanConfig = scan_config;
}    

uint8_t wifi_get_scan_config(void)
{
    return gWifiScanConfig;
}

void wifi_oem_update(uint8_t iface_id)
{    
    MESSAGE nxtMsg;
    nxtMsg.msgType = MSG_WIRELESS_UPDATE;
    nxtMsg.param1  = iface_id;

    tx_queue_send(&wconfigMsgQ, &nxtMsg, TX_NO_WAIT);    
}

//Enable the bad WEP key timer : used to detect the bad WEP key in open/auto authentication mode
void wifi_enable_bad_wepkey_timer(void)
{
   wifi_intf_security_mode_t securityMode;
   wifi_intf_wep_auth_mode_t wep_auth_mode;

   wifi_intf_get_var_sec_mode(WIFI_IFACE_ID_STATION, &securityMode);
   wifi_intf_get_var_wep_auth_mode(WIFI_IFACE_ID_STATION, &wep_auth_mode);

   if (securityMode == WEP && ((wep_auth_mode == AUTH_OPEN) || (wep_auth_mode == AUTH_AUTO)))  {
       DPRINTF(WCDBG, ("Checking for bad WEP key\n"));
       wifi_sta_badwepkey_timer =  delay_msec_timer_non_blocking(1000 * WIFI_STA_BAD_WEP_KEY_TIMER_CALLBACK_TIME, 
                    (void *)wifi_sta_badwepkey_timer_expired, NULL, false);
   }
   
}

//Disable the bad WEP key timer : used to detect the bad WEP key in open/auto authentication mode
void wifi_kill_bad_wepkey_timer(void)
{
   DPRINTF(WCDBG, ("Stop bad WEP key detection.\n"));
   
   if (wifi_sta_badwepkey_timer != NULL)
   {
       wifi_sta_badwepkey_timer = delay_msec_timer_cancel(wifi_sta_badwepkey_timer);
   }
}

#ifdef WIFI_DIRECT_SUPPORT
/**
 * @brief Set BSS Type
 *
 * @param iface     Interface 
 * @param bss_type  BSS type 
 *
 * @return OK or FAIL.
 */
int wlan_set_bss_type(char *iface, int bss_type)
{
    struct iwreq rwrq;
    int cur_bss_type;
    int retval = 0;

    // Read current bss role
    memset(&rwrq, 0, sizeof(rwrq));
    strncpy(rwrq.ifr_ifrn.ifrn_name, iface, strlen(iface));
    rwrq.u.data.length = 0;
    rwrq.u.data.pointer = &cur_bss_type;
    rwrq.u.data.flags = WOAL_SET_GET_BSS_ROLE;

    if (WlanIoctl(WOAL_SETONEINT_GETONEINT, &rwrq) == 0)
    {
        if (bss_type != cur_bss_type) {
            // Set bss role
            memset(&rwrq, 0, sizeof(rwrq));
            strncpy(rwrq.ifr_ifrn.ifrn_name, iface, strlen(iface));
            rwrq.u.data.length = 1;
            rwrq.u.data.pointer = &bss_type;
            rwrq.u.data.flags = WOAL_SET_GET_BSS_ROLE;

            if (WlanIoctl(WOAL_SETONEINT_GETONEINT, &rwrq) != 0)
            {
                DPRINTF((DBG_ERROR | DBG_OUTPUT),("WLAN: wlan_set_bss_type %d FAILED\n", bss_type));
                retval = -1;
            }
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("WLAN: wlan_set_bss_type %d FAILED\n", bss_type));
        retval = -1;
    }

    return retval;
}


/**
 * @brief Get BSS Type
 *
 * @param iface     Interface 
 * @param bss_type  Pointer to bss_type 
 *
 * @return 0 or -1
 */
int wlan_get_bss_type(char *iface, int *bss_type)
{
    struct iwreq rwrq;
    int cur_bss_type;
    int retval = 0;
#ifdef WIFI_DIRECT_SUPPORT
    bool wfd_enabled;
    uint8_t wfd_role;

    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_WFD, &wfd_enabled);
    wifi_intf_get_var_wfd_bss_role(WIFI_IFACE_ID_WFD, &wfd_role);

    if ((strcmp(iface, WIFI_IFACE_NAME_STATION) == 0) &&
       ((!wfd_enabled) || (wfd_enabled && wfd_role == WIFIDIRECT_GO_ROLE)))  
    {
        *bss_type = BSS_TYPE_STA;
        return 0;
    } 
#else
    if (strcmp(iface, WIFI_IFACE_NAME_STATION) == 0)
    {
        *bss_type = BSS_TYPE_STA;
        return 0;
    } 
#endif      
          
    // Read current bss role (only valid for UAP and WFD)
    memset(&rwrq, 0, sizeof(rwrq));
    strncpy(rwrq.ifr_ifrn.ifrn_name, iface, strlen(iface));
    rwrq.u.data.length = 0;
    rwrq.u.data.pointer = &cur_bss_type;
    rwrq.u.data.flags = WOAL_SET_GET_BSS_ROLE;

    if (WlanIoctl(WOAL_SETONEINT_GETONEINT, &rwrq) == 0)
    {
        *bss_type = cur_bss_type;
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("WLAN: wlan_get_bss_type FAILED\n"));
        retval = -1;
    }

    return retval;
}

ALIGN8 static t_u8 BG_SCAN_CFG[] = 
  { 0x6B, 0x00, 0x55, 0x00, 0x59, 0x20, 0x00, 0x00, 
    0x01, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 
    0xE8, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x01, 
    0x08, 0x00, 0x00, 0x44, 0x49, 0x52, 0x45, 0x43, 
    0x54, 0x2D, 0x02, 0x01, 0x02, 0x00, 0x02, 0x00, 
    0x01, 0x01, 0x15, 0x00, 0x00, 0x01, 0x02, 0x0A, 
    0x00, 0x64, 0x00, 0x00, 0x06, 0x02, 0x0A, 0x00, 
    0x64, 0x00, 0x00, 0x0B, 0x02, 0x0A, 0x00, 0x64, 
    0x00, 0x05, 0x01, 0x02, 0x00, 0x28, 0x00, 0x1E, 
    0x01, 0x02, 0x00, 0x00, 0x00
};

/**
 * @brief Set BG (Back Ground Scan) configuration
 *
 * @param iface     Interface 
 
 * @return OK or FAIL.
 */
int wlan_set_bgscan_cfg(char *iface)
{
    struct iwreq rwrq;
    int retval = 0;

    // Set BG SCAN Configuration
    memset(&rwrq, 0, sizeof(rwrq));
    strncpy(rwrq.ifr_ifrn.ifrn_name, iface, strlen(iface));
    rwrq.u.data.length = sizeof(BG_SCAN_CFG);
    rwrq.u.data.pointer = &BG_SCAN_CFG;
    rwrq.u.data.flags = 0;

    if (WlanIoctl(WOAL_HOST_CMD, &rwrq) != 0)
    {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("WLAN: wlan_set_bgscan_cfg FAILED\n"));
        retval = -1;
    }

    return retval;
}

#endif


/** 
 * \brief Apply the current wireless settings to the STA.
 * 
 * Reads the current wireless settings from the datastore and applies the settings
 * to the protocol stack.  Called when the wireless stack is initialized and whenever
 * the settings are changed.
 **/
void wConfigApplyStaSettings(void)
{
    wifi_intf_info_t *pWinfo;
    bool hw_enabled;

    DPRINTF(WCDBG, ("WLAN: ==> wConfigApplyStaSettings\n"));

    // Cancel the timer in case someone caused this to happen directly.
    tx_timer_deactivate(&wcfgStaInfoTimer);

    // New settings - cancel any pending rejoin
    tx_timer_deactivate(&wcfgAdHocRejoinTimer);

    if (wifi_intf_get_var_is_wps_running())
    {
        DPRINTF(WCDBG, ("WLAN: <== wConfigApplyStaSettings - WPS is running.  Wait for it to finish.\n"));
        return;
    }

    //REVISIT: Prevents a possible attempt to perform wlan ioctls while the hw is in process of shutting down after a scan operation.
    //Need to revisit to find a more cleaner solution. 
    if(g_wlan_power_state == WLAN_SHUTTING_DOWN)
    {
        DPRINTF(WCDBG, ("WLAN: <== wConfigApplyStaSettings - HW in powering down state. Wait for power down to complete\n"));
        return;
    }

    // Indicate change
    wifi_intf_sta_set_link_status(WIFI_INTF_LINK_EVENT_DOWN);

    // initialize the SSID if it hasn't been set yet
    wirelessEnsureValidStaSSID();

    if(!WlanIsWirelessHWActive())
    {
        DPRINTF(WCDBG, ("WLAN: <== wConfigApplyStaSettings - HW disabled\n"));
        return;
    }

    // Before applying new settings tear everything down
    WlanShimCDeauth();
    WlanClearEncryption();

    // Start applying current settings
    pWinfo = (wifi_intf_info_t*)MEM_MALLOC(sizeof(wifi_intf_info_t));
    ASSERT(pWinfo != NULL);
    fillInWirelessIfaceInfo(WIFI_IFACE_ID_STATION, pWinfo);
    DPRINTF(WCDBG, ("WLAN: wConfigApplyStaSettings SSID = %s \n",pWinfo->SSID));

#if 0  // The normal code should handle this. TEST TEST TEST
    // WiFi certification test 5.2.52 requires that we prevent WEP from being
    // ussed for 802.11N.  If we're set for WEP, make sure they don't use N.  
    if (pWinfo->SecurityMode == WEP)
    {
        // We're using WEP -- prevent them from running 802.11N
        WlanSetInfraBandBG();
    }
    else
    {
        // Not using WEP -- can try to use N (may be disallowed later by WPA-TKIP: test 5.2.51).
        WlanSetInfraBandBGN();
    }
#endif        
    // Verify we should be enabled
    wifi_hw_get_var_is_enabled(&hw_enabled);
    if (hw_enabled && (pWinfo->enabled) && 
        (strlen((char *)pWinfo->SSID) > 0) && (!gWifiStaDisabledByWiredLink))
    {
        if(pWinfo->sta.CommunicationMode == INFRASTRUCTURE )
        {
            WlanSetMode(INFRASTRUCTURE);

            //Set channel to default (update this only after association is complete)
            wifi_intf_set_var_channel(WIFI_IFACE_ID_STATION, DEFAULT_WIFI_CHAN);

            if(WlanSetEncryption(pWinfo))
            {
                DPRINTF(WCDBG, ("WLAN: wConfigApplyStaSettings Returned Error to Set ENCRYPTION\n"));
            }

            if((pWinfo->SecurityMode == WPA_PSK) || (pWinfo->SecurityMode == WPA2_PSK))
            {
                DPRINTF(WCDBG, ("WLAN: wConfigApplyStaSettings Infrastructure: WPA/WPA2\n"));

                #ifdef REASSOCIATION
                // Due to the four-way-handshake and key exchange, etc, association/reassociation 
                // is handled by the supplicant, not the driver.
                reassociation(FALSE);
                #endif /* REASSOCIATION */
            }
            else
            {
                DPRINTF(WCDBG, ("WLAN: wConfigApplyStaSettings Infrastructure\n"));

                // Associate (SetEssid causes the driver to scan, locate the AP and associate)
                if(WlanSetEssid((char *)pWinfo->SSID, strlen((char *)pWinfo->SSID), 0))
                {
                    DPRINTF(WCDBG, ("WLAN: wConfigApplyStaSettings Returned Error to Set Essid\n"));

                    //Association failed, increment retry counter as this might be a bad WEP key
                    if (pWinfo->SecurityMode == WEP) 
                    {
                        g_wifi_sta_badwepkey_retry_counter++;
                    }

                    if (g_wifi_sta_badwepkey_retry_counter == WIFI_STA_BAD_KEY_RETRY_COUNT)
                    {
                        DPRINTF(WCDBG, ("Retry limit for WEP bad key reached!\n"));    
                        wifi_intf_sta_set_state(WLAN_BAD_KEY_ERROR);
                        g_wifi_sta_badwepkey_retry_counter = 0;        
                    }
                    
                    // Associate failed, so try again in a bit
                    tx_timer_deactivate(&wcfgStaInfoTimer);
                    tx_timer_change(&wcfgStaInfoTimer, 100, 0);
                    tx_timer_activate(&wcfgStaInfoTimer);
                }
                else
                {
                    #if REASSOCIATION
                    // Successfully associated.  Let the driver handle reassociation from now on.
                    reassociation(TRUE);
                    #endif /* REASSOCIATION */
                }
            }
        }
        else
        {
            /* MODE IS  AD-HOC CREATE NETWORK
            */
            WlanSetMode(AD_HOC);

            WlanSetChannel(pWinfo);
            WlanSetEncryption(pWinfo);  

            #ifdef REASSOCIATION
            // We handle ad hoc rejoins outside the driver's reassociation thread (see wcfgAdHocRejoinTimer)
            reassociation(FALSE);
            #endif /* REASSOCIATION */

            WlanSetEssid((char *)pWinfo->SSID, strlen((char *)pWinfo->SSID), 0); 
        }
    }
    else
    {
        // Unconfigured or disabled.  Make sure supplicant is disabled.
        ProcessSetSupp(pWinfo, FALSE);
    }

    MEM_FREE_AND_NULL(pWinfo);

    DPRINTF(WCDBG, ("WLAN: <== wConfigApplyStaSettings\n"));
}

wifi_intf_customie_info_t *wifi_intf_get_customie_info(uint8_t index)
{
    return g_customie_info[index];    
}    

/** 
 * \brief Append a custom IE
 * 
 * Appends a custom information element. 
 *  
 **/
static void wlan_append_custom_ie(uint8_t iface_id)
{
    int i = 0;

    for (i = 0; i < MAX_NUM_CUSTOM_IE; i++)
    {
        if (g_customie_info[i] != NULL)
        {
            if (g_customie_info[i]->ie_length > 0)
            {
                wifi_intf_set_custom_ie(iface_id,
                                i,
                                g_customie_info[i]->mgmt_subtype_mask,
                                g_customie_info[i]->buffer,
                                g_customie_info[i]->ie_length);
            }                
        }    
    }        
}

/** 
 * \brief Set custom IE 
 *  
 * Set custom information element. 
 *  
 **/
bool wifi_intf_set_custom_ie(uint8_t iface_id,
                             int ie_index,
                             uint16_t mgmt_subtype_mask,
                             uint8_t * ie_buffer,
                             int ie_length)
{
    mlan_ds_misc_custom_ie *cust_ie;
    struct ifreq ifr;
    u8      *buf;
    bool    ret = FALSE;
    net_device *net_dev = NULL;

    uint8_t uap_active_config;
    bool enabled;
    bool is_active;

    wifi_intf_get_var_is_enabled(iface_id, &enabled);
    wifi_hw_get_var_is_active(&is_active);
    uap_active_config = wifi_get_uap_active_config(); 

    if (!enabled || !is_active) 
    {
        //If the UAP/UAP_Pri0/WFD interface is disabled or if the hardware is not active, Bail out!
        return false;
    }
   
    if (((iface_id == WIFI_IFACE_ID_UAP) || (iface_id == WIFI_IFACE_ID_UAP_PRI0)) && uap_active_config != iface_id)
    {
        //if the uAP/uAP_Pri0 interface is active and the currently active uAP config does not match, Bail out!
        return false;
    }

    if ((ie_length > 0) && (ie_length <= MAX_IE_SIZE) && (ie_buffer != NULL))
    {
        if (iface_id == WIFI_IFACE_ID_UAP || iface_id == WIFI_IFACE_ID_UAP_PRI0)
            net_dev = wlan_get_net_uap_dev();
#ifdef WIFI_DIRECT_SUPPORT        
        else if (iface_id == WIFI_IFACE_ID_WFD)
            net_dev = wlan_get_net_wfd_dev();
#endif        
        if (net_dev)
        {
            buf = (u8 *)MEM_MALLOC(sizeof(mlan_ds_misc_custom_ie));
            if (buf) {
                memset(buf, 0, (sizeof(mlan_ds_misc_custom_ie)));

                /* Set IE info */
                cust_ie = (mlan_ds_misc_custom_ie *) buf;
                cust_ie->type = 0x169;
                cust_ie->len = ie_length + (sizeof(custom_ie)-MAX_IE_SIZE);
                cust_ie->ie_data_list[0].ie_index = ie_index;
                cust_ie->ie_data_list[0].mgmt_subtype_mask = mgmt_subtype_mask;
                cust_ie->ie_data_list[0].ie_length = ie_length;
                memcpy(cust_ie->ie_data_list[0].ie_buffer, ie_buffer, ie_length);

                if (mgmt_subtype_mask == 0)
                {
                    /* Remove custom IE */
                    if (g_customie_info[ie_index] != NULL)
                    {
                        MEM_FREE_AND_NULL(g_customie_info[ie_index]);                        
                    }    

                } else {
                    /* Store custom IE */                    
                    if (ie_index >= 0)
                    {
                        if (g_customie_info[ie_index] == NULL)
                        {
                            g_customie_info[ie_index] =  MEM_MALLOC(sizeof(wifi_intf_customie_info_t));
                            if (g_customie_info[ie_index] == NULL)
                            {
                                DPRINTF(WCDBG, ("wifi_intf_set_custom_ie: malloc failure\n"));
                                MEM_FREE_AND_NULL(buf);
                                LEAVE();
                                return ret;
                            }    
                        }    
                        
                        g_customie_info[ie_index]->mgmt_subtype_mask = mgmt_subtype_mask;
                        g_customie_info[ie_index]->ie_length = ie_length;
                        memcpy(g_customie_info[ie_index]->buffer, ie_buffer, ie_length);
                    }    
                }

                /* Initialize the ifr structure */
                memset(&ifr, 0, sizeof(ifr));
                ifr.ifr_ifru.ifru_data = (void *) buf;

                if (net_dev->do_ioctl(net_dev, &ifr, WOAL_CUSTOM_IE_CFG) == 0)
                {
                    ret = TRUE;
                } else {
                    DPRINTF(WCDBG, ("wifi_intf_set_custom_ie: FAILED to set CUSTOM IE\n"));                    
                }

                MEM_FREE_AND_NULL(buf);
            } else{
                DPRINTF(WCDBG, ("wifi_intf_set_custom_ie: malloc failure\n"));
            }
        } else {
            DPRINTF(WCDBG, ("wifi_intf_set_custom_ie: net_dev NULL\n"));
        }
    } else {
        DPRINTF(WCDBG, ("wifi_intf_set_custom_ie: Invalid CUSTOM IE length %d buffer 0x%X\n", 
                        ie_length, ie_buffer));
    }

    LEAVE();
    return ret;
}

/** 
 * \brief Apply the current wireless settings to the uAP.
 * 
 * Tears down the existing uAP instance and re-inits it with
 * current settings.
 **/
void wConfigApplyUapSettings(void)
{
    net_device *uap_dev;
    wifi_intf_info_t *p_uap_config_ds;
    bool hw_enabled;
    uint8_t iface_id = WIFI_IFACE_ID_UAP;
       
    DPRINTF(WCDBG,("WLAN: ==>  wConfigApplyUapSettings\n"));

    tx_timer_deactivate(&wcfgUapInfoTimer);

    //Update the NVRAM channel number for all the wireless interfaces
    wifi_update_channel_number();    
    
    // Get uap dev
    if ((uap_dev = wlan_get_net_uap_dev())) {
        bool uap_pri0_enabled;
        bool uap_enabled;        
       
        wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP, &uap_enabled);
        wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP_PRI0, &uap_pri0_enabled);

        p_uap_config_ds = (wifi_intf_info_t*)MEM_MALLOC(sizeof(wifi_intf_info_t));
        if (p_uap_config_ds) {
            if (uap_pri0_enabled && g_wifi_uap_current_active_config == WIFI_IFACE_ID_UAP_PRI0)
            {
                DPRINTF(WCDBG,("WLAN: ==>  wConfigApplyUapSettings: Active config is UAP_Pri0\n"));
                iface_id = WIFI_IFACE_ID_UAP_PRI0;
            }    
            else if (uap_enabled && g_wifi_uap_current_active_config == WIFI_IFACE_ID_UAP)
            {
                DPRINTF(WCDBG,("WLAN: ==>  wConfigApplyUapSettings: Active config is UAP\n"));
                iface_id = WIFI_IFACE_ID_UAP;
                wirelessEnsureValidUapSSID();  
            }

            fillInWirelessIfaceInfo(iface_id, p_uap_config_ds);
          
            // Set BSS Type
            //wlan_set_bss_type(WIFI_IFACE_NAME_UAP, MLAN_BSS_ROLE_UAP); //ADDED FOR CUSTOM IE 

            // Stop BSS
            send_bss_ctl_ioctl(WIFI_IFACE_NAME_UAP, UAP_BSS_STOP);

            netdrvr_uap_shared_notify_link_down(WIFI_IFACE_ID_UAP);

            // Before we turn on, verify that we should be enabled.  (Check HW enable, too -
            // because sometimes exceptions cause the power to be on even when the HW enabled
            // flag is off [doing a scan, running wps on sta, etc])
            wifi_hw_get_var_is_enabled(&hw_enabled);
            if (hw_enabled && p_uap_config_ds->enabled && !gWifiUapDisabledByAdHocSta) {
                DPRINTF(WCDBG, ("WLAN: uAP is enabled -- configure and start.\n"));
                
                // Configure BSS
                bss_configure(WIFI_IFACE_NAME_UAP, (void *) p_uap_config_ds);

                // Append Custom IE
                wlan_append_custom_ie(iface_id);

                // Start BSS
                if (send_bss_ctl_ioctl(WIFI_IFACE_NAME_UAP, UAP_BSS_START) == 0) {
                    netdrvr_uap_shared_notify_link_up(WIFI_IFACE_ID_UAP);
                } else {
                    DPRINTF(WCDBG,("WLAN: FAILED to start uAP\n"));
                }
            } else {
                DPRINTF(WCDBG, ("WLAN: uAP is disabled.\n"));
            }
            MEM_FREE_AND_NULL(p_uap_config_ds);
        } else {
            DPRINTF(WCDBG,("ERR:Cannot malloc p_uap_config_ds wConfigApplyUapSettings!\n"));
        }
    } else {
        DPRINTF(WCDBG,("ERR:Cannot get uap dev wConfigApplyUapSettings!\n"));
    }

    DPRINTF(WCDBG, ("WLAN: <== wConfigApplyUapSettings\n"));
}

#ifdef WIFI_DIRECT_SUPPORT
static int wfd_group_owner_flag = 0;
static int wfd_start_find_phase_flag = 0;
static int wfd_client_flag = 0;

/**
 * @brief Start Find Phase
 *
 * @return OK or FAIL.
 */
error_type_t
wlan_wfd_start_find_phase(void)
{
    error_type_t retval = FAIL;
    wfd_group_owner_flag = 0;
    wfd_client_flag = 0;
    wfd_start_find_phase_flag = 1;
    retval = wifi_intf_set_var_wfd_bss_role(WIFI_IFACE_ID_WFD, WIFIDIRECT_CLIENT_ROLE);
    if (retval == OK) {
        retval = wifi_intf_set_var_is_enabled(WIFI_IFACE_ID_WFD, true);
    }
    return retval;
}


void wlan_wfd_start_find_phase_internal(void)
{
    DPRINTF(WCDBG, ("WLAN: WFD start find phase.\n"));
    wfd_start_find_phase_flag = 0;

    // Set BSS Type
    wlan_set_bss_type(WIFI_IFACE_NAME_WFD, MLAN_BSS_ROLE_STA);

    // Stop wifi-direct
    wifidirectcmd_set_status(WIFIDIRECT_MODE_STOP_STATUS);

    // Initialize find stuff
    extern void wfd_init_find_phase(void);
    wfd_init_find_phase();

    // Configure Wifi-Direct
    wfd_configure();

    // Params Configure
    wfd_params_configure();

    // Start wifi-direct
    wifidirectcmd_set_status(WIFIDIRECT_MODE_START_STATUS);

    // Setup WPS event handler
    extern int wps_event_init(WPS_DATA *);
    wps_event_init(NULL);

    // Set Background scan
    wlan_set_bgscan_cfg(WIFI_IFACE_NAME_WFD);

    // Start wifi-direct find phase
    wifidirectcmd_set_status(WIFIDIRECT_MODE_START_FIND_PHASE);
}
#endif

/** 
 * \brief Apply the current wireless settings to the WFD.
 * 
 * Tears down the existing WFD instance and re-inits it with
 * current settings.
 **/
void wConfigApplyWfdSettings(void)
{
#ifdef WIFI_DIRECT_SUPPORT
    net_device *wfd_dev;
    wifi_intf_info_t *p_wfd_config_ds;
    bool hw_enabled;

    DPRINTF(WCDBG,("WLAN: ==>  wConfigApplyWfdSettings\n"));

    tx_timer_deactivate(&wcfgWfdInfoTimer);

    // Get wfd dev
    if ((wfd_dev = wlan_get_net_wfd_dev())) {
        wirelessEnsureValidWfdSSID();

        p_wfd_config_ds = (wifi_intf_info_t*)MEM_MALLOC(sizeof(wifi_intf_info_t));
        if (p_wfd_config_ds) {
            // Fill Wireless Interface Information
            fillInWirelessIfaceInfo(WIFI_IFACE_ID_WFD, p_wfd_config_ds);

            // Set BSS Type
            wlan_set_bss_type(WIFI_IFACE_NAME_WFD, MLAN_BSS_ROLE_UAP);

            // Stop BSS
            send_bss_ctl_ioctl(WIFI_IFACE_NAME_WFD, UAP_BSS_STOP);

            netdrvr_uap_shared_notify_link_down(WIFI_IFACE_ID_WFD);

            // Before we turn on, verify that we should be enabled.  (Check HW enable, too -
            // because sometimes exceptions cause the power to be on even when the HW enabled
            // flag is off [doing a scan, running wps on sta, etc])
            wifi_hw_get_var_is_enabled(&hw_enabled);
            if (hw_enabled && p_wfd_config_ds->enabled && !gWifiUapDisabledByAdHocSta) {
                // Check for Autonomous GO
                if (p_wfd_config_ds->WFDBSSRole == WIFIDIRECT_GO_ROLE) {
                    DPRINTF(WCDBG, ("WLAN: WFD GO is enabled -- configure and start.\n"));

                    wifi_intf_stop_wps();

                    wfd_group_owner_flag = 1;

                    // Stop wifi-direct
                    wifidirectcmd_set_status(WIFIDIRECT_MODE_STOP_STATUS);

                    p_wfd_config_ds->Channel = WIFIDIRECT_CONFIG_OPERATING_CHANNEL_CHANNEL_DEFAULT;

                    // Configure BSS
                    bss_configure(WIFI_IFACE_NAME_WFD, (void *) p_wfd_config_ds);

                    // Configure Wifi-Direct
                    wfd_configure();

                    // Start wifi-direct
                    wifidirectcmd_set_status(WIFIDIRECT_MODE_START_STATUS);

                    // Set to group owner mode
                    wifidirectcmd_set_status(WIFIDIRECT_MODE_START_GROUP_OWNER);

                    // Update IE to show WPS
                    extern int wps_update_ie(char *);
                    wps_update_ie(WIFI_IFACE_NAME_WFD);

                    // Append Custom IE
                    wlan_append_custom_ie(WIFI_IFACE_ID_WFD);

                    // Start BSS
                    if (send_bss_ctl_ioctl(WIFI_IFACE_NAME_WFD, UAP_BSS_START) == 0) {
                        netdrvr_uap_shared_notify_link_up(WIFI_IFACE_ID_WFD);
                    } else {
                        DPRINTF(WCDBG,("WLAN: FAILED to start WFD uAP\n"));
                    }
                } else {
                    DPRINTF(WCDBG, ("WLAN: WFD Client is enabled\n"));

                    wfd_group_owner_flag = 0;

                    // Indicate change
                    wifi_intf_sta_set_link_status(WIFI_INTF_LINK_EVENT_DOWN);

                    // Append Custom IE
                    wlan_append_custom_ie(WIFI_IFACE_ID_WFD);

                    if (wfd_start_find_phase_flag) {
                        wlan_wfd_start_find_phase_internal();
                    } else {
                        wpaSupConfig sConfig;

                        // Start Supplicant (WFD only uses WPA2)
                        sConfig.WPA_ON = 1;
                        sConfig.ifname = WIFI_IFACE_NAME_WFD;
                        SupConfig(sConfig); 
                    }
                }
            } else {
                DPRINTF(WCDBG, ("WLAN: wfd is disabled.\n"));
            }
            MEM_FREE_AND_NULL(p_wfd_config_ds);
        } else {
            DPRINTF(WCDBG,("ERR:Cannot malloc p_wfd_config_ds wConfigApplyWfdSettings!\n"));
        }
    } else {
        DPRINTF(WCDBG,("ERR:Cannot get wfd dev wConfigApplyWfdSettings!\n"));
    }

    DPRINTF(WCDBG, ("WLAN: <== wConfigApplyWfdSettings\n"));
#endif //WIFI_DIRECT_SUPPORT
}


void wlan_wfd_start_client(int peer_config_timeout)
{
#ifdef WIFI_DIRECT_SUPPORT
    // Only allow this to be called once per find
    if (wfd_client_flag == 0) {
        wfd_client_flag = 1;

        /* Changed from 5 to 2 during WFD certification */
        tx_thread_sleep(5);

        // Start wifi-direct client
        wifidirectcmd_set_status(WIFIDIRECT_MODE_START_CLIENT);

        /* sleep as per peer's GO config timeout */
        tx_thread_sleep(peer_config_timeout);

        // Start WPS Enrollee
        wirelessStartWFDWPSEnrollee(NULL, NULL);
    }
#endif
}

void wlan_wfd_start_group_owner(void)
{
#ifdef WIFI_DIRECT_SUPPORT
    wifi_intf_info_t *p_wfd_config_ds;

    if (wfd_group_owner_flag == 0) {
        netdrvr_uap_shared_notify_link_down(WIFI_IFACE_ID_WFD);
        wifi_intf_stop_wps();

        p_wfd_config_ds = (wifi_intf_info_t*)MEM_MALLOC(sizeof(wifi_intf_info_t));
        if (p_wfd_config_ds) {
            wfd_group_owner_flag = 1;

            fillInWirelessIfaceInfo(WIFI_IFACE_ID_WFD, p_wfd_config_ds);

            // Random Passphrase ?????

            // Set BSS Type
            wlan_set_bss_type(WIFI_IFACE_NAME_WFD, MLAN_BSS_ROLE_UAP);

            // Stop wifi-direct
            wifidirectcmd_set_status(WIFIDIRECT_MODE_STOP_STATUS);

            p_wfd_config_ds->Channel = WIFIDIRECT_CONFIG_OPERATING_CHANNEL_CHANNEL_DEFAULT;

            // Configure BSS
            bss_configure(WIFI_IFACE_NAME_WFD, (void *) p_wfd_config_ds);

            // Configure Wifi-Direct
            wfd_configure();

            // Start wifi-direct
            wifidirectcmd_set_status(WIFIDIRECT_MODE_START_STATUS);

            // Set to group owner mode
            wifidirectcmd_set_status(WIFIDIRECT_MODE_START_GROUP_OWNER);

            // Start BSS
            if (send_bss_ctl_ioctl(WIFI_IFACE_NAME_WFD, UAP_BSS_START) == 0) {
                netdrvr_uap_shared_notify_link_up(WIFI_IFACE_ID_WFD);
                wirelessStartWFDWPSRegistrar(NULL);
            } else {
                DPRINTF(WCDBG,("WLAN: wlan_start_group_owner FAILED to start WFD uAP\n"));
            }
        } else {
            DPRINTF(WCDBG,("WLAN: wlan_start_group_owner FAILED to get memory\n"));
        }
    }
#endif
}


/*---------------------------------------------------------------------------*/
int wConfigInitSta(void)
{	
    moal_private * moalPriv;
    moal_handle  * moalHand;
    mlan_adapter * mlanAdap;
    uint8_t region_code;
    uint8_t wws_config;
    wifi_intf_antenna_t tx_antenna;
    wifi_intf_antenna_t rx_antenna;

    DPRINTF(WCDBG, ("WLAN: ==> wConfigInit\n"));

	if(g_wirelessInitialized)
    {
        DPRINTF(WCDBG, ("WLAN: <== wConfigInit: already initialized\n"));
		return AT_SUCCESS;
	}

    // Keep a global copy to net_dev
    if ((g_wlanDev = wlan_get_net_dev()) != NULL)
    {
        moalPriv = (moal_private *) g_wlanDev->priv;
        ASSERT(moalPriv != NULL);
    
        moalHand = moalPriv->phandle;
        ASSERT(moalHand != NULL);
    
        mlanAdap = (mlan_adapter *) moalHand->pmlan_adapter;
        ASSERT(mlanAdap != NULL);
    
        // Keep a global pointer of mlan_private
        g_pmlanPriv = mlanAdap->priv[0];
        ASSERT(g_pmlanPriv!= NULL);

        WlanWirelessSetRadioPresent();
        WlanWirelessHWActive(TRUE);

        wifi_hw_get_var_region_code(&region_code);
        wifi_hw_get_var_wws_config(&wws_config);
        // TBD
        /*
        if(region_code != g_RegionID)
        {
            // ensure that the wireless region code matches the real nvram region code. -jrs
            region_code = g_RegionID;
            wifi_intf_set_var_region_code(region_code);
        }
        */
        		
    	DPRINTF(WCDBG, ("Set the REGION CODE 0x%X\n", region_code));

        WlanSetRegion(region_code);

        //Set the current world wide safe config
        WlanSetWwsCfg(wws_config);            
        
        wifi_hw_get_var_tx_antenna(&tx_antenna);
        wifi_hw_get_var_rx_antenna(&rx_antenna);
        WlanSetAntenna(&tx_antenna, &rx_antenna);
#if 0
        // enable 802.11d
        DPRINTF(WCDBG, ("Enable 802.11d\n"));
        wlan_11d_enable(g_pmlanPriv, NULL, ENABLE_11D);
#else
        // disable 802.11d
        DPRINTF(WCDBG, ("Disable 802.11d\n"));

        wlan_11d_enable(g_pmlanPriv, NULL, DISABLE_11D);
#endif

        // if power down after event do not apply settings
        if (g_after_power_up_event == NONE)
        {
            DPRINTF(WCDBG, ("wConfigApplyStaSettings\n"));
            wConfigApplyStaSettings();
        }
        else
        {
            DPRINTF(WCDBG, ("wirelessEnsureValidStaSSID\n"));
            wirelessEnsureValidStaSSID();
        }

        g_wirelessInitialized = TRUE;

        DPRINTF(WCDBG, ("WLAN: <== wConfigInit\n"));
        return AT_SUCCESS;
    }

    DPRINTF(WCDBG, ("WLAN: <== wConfigInit: wlan_get_net_dev == NULL\n"));
    return AT_FAILURE;
}

int wConfigInitUap(void)
{
    wConfigApplyUapSettings();

    return AT_SUCCESS;
}

int wConfigInitWfd(void)
{
    wConfigApplyWfdSettings();

    return AT_SUCCESS;
}

/*---------------------------------------------------------------------------*/

/** 
 * \brief Initiate a wireless scan.
 * 
 * \param ssid NULL for a generic scan.  SSID of network to scan
 * for directed scans. (ssid memory will not be freed)
 * 
 * This function schedules a scan and returns immediately.  To
 * collect results, wait for the wifi_intf_get_var_is_wireless_scanning()
 * to return false, then read the results from the
 * datastore in e_WirelessScanInfo.
 * 
 * \return 0 on success, -1 (out of memory), -2 (HW not enabled)
 **/
int wirelessStartScan(unsigned char *ssid)
{
    unsigned char *ssidCopy = NULL;
    bool isWirelessScanning;

    wifi_intf_get_var_is_wireless_scanning(&isWirelessScanning);
    if (!isWirelessScanning)
    {
        if (ssid != NULL)
        {
            ssidCopy = (unsigned char *)MEM_MALLOC(strlen((char*)ssid) + 1);
    
            if (ssidCopy == NULL)
            {
                // Out of memory -- just quit
                return -1;
            }
    
            strcpy((char*)ssidCopy, (char*)ssid);
    
            gScanMsg.param2  = (UINT32)strlen((char*)ssid);
        }
        else
        {
            gScanMsg.param2  = 0x0;
        }

        isWirelessScanning = TRUE;
        wifi_intf_set_var_is_wireless_scanning(isWirelessScanning);
    
        gScanMsg.msgType = MSG_WIRELESS_START_SCAN;
        gScanMsg.param1  = (UINT32)ssidCopy;
    
        //if (WlanIsWirelessInstalled() != WB_OK)
        tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
        if ((g_wlan_power_state == WLAN_OFF) || (g_wlan_power_state == WLAN_SHUTTING_DOWN))
        {
            g_after_power_up_event = POWER_DOWN_AFTER_SCAN;
            tx_mutex_put(&wlan_start_stop_mutex);

            // Try to turn on the radio to scan
            WlanStartup();
        }
        else
        {
            tx_mutex_put(&wlan_start_stop_mutex);
            SYMsgSend(WIRELESSCONFIGID, &gScanMsg);
        }
    }
    else
    {
        DPRINTF(WCDBG, ("Request to start Scan occurred while it is already running\n"));
    }

    return 0;
}


struct net_device *WlanGetNetDev(int bss_type)
{
    net_device *device = NULL;

    if(WlanIsWirelessHWActive())
    {
        switch(bss_type)
        {
#ifdef WIFI_DIRECT_SUPPORT
            case MLAN_BSS_TYPE_STA:
                device = wlan_get_net_dev();
                break;

            case MLAN_BSS_TYPE_UAP:
                {
                    bool wfd_enabled = false;
                    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_WFD, &wfd_enabled);
                    if (!wfd_enabled)
                    {
                        //DPRINTF(WCDBG, ("UAP: WlanGetNetDev = %#x\n", wlan_get_net_uap_dev()));
                        device = wlan_get_net_uap_dev();
                    }
                    else
                    {
                        //DPRINTF(WCDBG, ("WFD: WlanGetNetDev = %#x\n", wlan_get_net_wfd_dev()));
                        device = wlan_get_net_wfd_dev();
                    }
                }
                break;

            case MLAN_BSS_TYPE_WIFIDIRECT:
                //DPRINTF(WCDBG, ("WFD: WlanGetNetDev = %#x\n", wlan_get_net_wfd_dev()));
                device = wlan_get_net_wfd_dev();
                break;
#else
            case MLAN_BSS_TYPE_STA:
                //DPRINTF(WCDBG, ("WLAN: WlanGetNetDev = %#x\n", wlan_get_net_dev()));
                device = wlan_get_net_dev();
                break;

            case MLAN_BSS_TYPE_UAP:
                //DPRINTF(WCDBG, ("UAP: WlanGetNetDev = %#x\n", wlan_get_net_uap_dev()));
                device = wlan_get_net_uap_dev();
                break;
#endif
        }
    }
/*
    if (device == NULL)
    {
        DPRINTF(WCDBG, ("WLAN: WlanGetNetDev = NULL!!! 0x%X\n", WlanIsWirelessHWActive()));
    }
*/
    return device;
}

// -jrs was SHIMD Lower Funcs
/*---------------------------------------------------------------------------*/
/* driver call this API to forward recived packet to upper layer */
void WlanShimDRecvPacket(unsigned char *Pkt, unsigned long Len, void *bufDesc, int bss_type) {
	//ENTER();
    if (bss_type != MLAN_BSS_TYPE_STA)
        wifi_uap_intf_rx_packet_recvd((char*)Pkt, Len, bufDesc);
    else
        wifi_intf_rx_packet_recvd((char*)Pkt, Len, bufDesc);
	//LEAVE();
}

/* wlan function calls this function to get recv buff from upper layer */
void WlanShimDGetRxBuf(char **rcv_buf, int *buf_len, void ** buf_desc) {
	//ENTER();
	wifi_intf_get_rx_buffer(rcv_buf, buf_len, buf_desc);	
	//LEAVE();
}

/* wlan layer calls this function to intimate the upper layer that 
 * pkt is been sent and can continue with next pkt*/
void WlanShimDTxPktSent(char *pkt, unsigned long pkt_len, void * handle) {
	//ENTER();
	wifi_intf_tx_packet_sent(pkt, pkt_len, handle);
	//LEAVE();
}

/******************************************************************************
 * Function Name: WirelessLowerLayerHasPacketOutstanding
 *
 * Description: This is used to determine if the lower layer driver (v9 
 *    currently) has a packet outstanding (either as a queued packet or in the 
 *    current TX buffer).  If not, it is ok to send down the next packet. 
 ******************************************************************************/
BOOL WirelessLowerLayerHasPacketOutstanding(int bss_type)
{
    net_device *dev = WlanGetNetDev(bss_type);
    mlan_private* priv;

    if(!WlanIsWirelessHWActive() || dev == NULL)
    {
        // If the card is disabled, the lower layer driver obviously doesn't
        // have any packet outstanding.  However, it's better to reply "TRUE,"
        // because that will keep the upper layer code from trying to send
        // packets down.
        return TRUE;
    }

    priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];

    if (!wlan_wmm_lists_empty(priv->adapter))
    {
        return TRUE;
    }

    return FALSE;
}

// -jrs was SHIMD Upper Funcs
/*---------------------------------------------------------------------------*/
#define SET	1
#define RESET 	0

/* This API is used to set the multicast address */
void WlanSetMulticast(unsigned char Num, unsigned char *List[MAX_MC_ADDRS], int bss_type) {
	// In wlan: see SendSetMulticast() in main.c: is sending multicast address to the fw
	int i;
	net_device * dev = WlanGetNetDev(bss_type);

	ENTER();

    if(!WlanIsWirelessHWActive() || (dev == NULL))
    {
        return;
    }

    XASSERT(Num <= MAX_MC_ADDRS, Num);

	dev->mc_count = Num;
	
	if(Num > 0) 
		for(i = 0; (i < Num) && (i < MAX_MC_ADDRS); i++)
			dev->mc_list[i] = List[i];
		
	dev->set_multicast_list(dev);

	LEAVE();
}

void WlanMainProcess(void)
{
    net_device *dev = wlan_get_net_dev();
    moal_private *priv;

    if (dev != NULL)
    {
        priv = dev->priv;
        mlan_main_process(priv->phandle->pmlan_adapter);
    }
}


BOOL WlanIsConnected(void)
{
    net_device *dev = wlan_get_net_dev();
    mlan_private* priv;

    if(!WlanIsWirelessHWActive() || dev == NULL)
    {
        // stack is not running; so must not be connected
        return FALSE;
    }

    priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];

    if ((priv->media_connected == MTRUE)
        && ((priv->bss_mode != MLAN_BSS_MODE_IBSS)
        || (priv->adhoc_is_link_sensed == MTRUE)))
    {
        // -jrs added check against AdhocLinkSensed.  Without this we start the
        // Treck stack too early in adhoc mode.  This results in the wmm queues
        // stalling because scans leave the queues when they are performed before
        // the adhoc network is started and the queues only start when there is really
        // a link.  Thus we need to make sure that we don't report link status connected
        // if adhoc is not really connected.
        return TRUE;
    }
    return FALSE;
}

void wlan_start_rejoin_attempts(void)
{
    wifi_intf_comm_mode_t comm_mode;
    wifi_intf_get_var_comm_mode(WIFI_IFACE_ID_STATION, &comm_mode);

    if(!WlanIsWirelessHWActive())
    {
        return;
    }

    if (comm_mode == AD_HOC)
    {
        DPRINTF(WCDBG, ("WIFI: Starting rejoin timer -- had ad hoc connection, but lost it.\n"));
        tx_timer_deactivate(&wcfgAdHocRejoinTimer);
        tx_timer_change(&wcfgAdHocRejoinTimer, AD_HOC_REJOIN_TIME, 0);
        tx_timer_activate(&wcfgAdHocRejoinTimer);
    }
}


/** 
 * \brief Ensure NVRAM and wireless board have correct MAC
 * address
 * 
 **/
void WirelessInitMACAddress(void)
{
#if 0
#ifdef GET_MAC_FROM_WIRELESS_BOARD
    char macOnBoard[6];
    tDSVarData wlanMacDSVal;

    wlanMacDSVal.pVar = macOnBoard;
    wlanMacDSVal.name = e_WlanMACAddress;

    DSGetVar(&wlanMacDSVal);

    DPRINTF(WCDBG, ("WLAN: Checking MAC address stored on wireless board\n"));
    if (WlanGetMacAddr(macOnBoard))
    {
        DPRINTF(WCDBG, ("WLAN: MAC from wireless board is %02X:%02X:%02X:%02X:%02X:%02X\n",
                    (uint8_t)macOnBoard[0], (uint8_t)macOnBoard[1], (uint8_t)macOnBoard[2],
                    (uint8_t)macOnBoard[3], (uint8_t)macOnBoard[4], (uint8_t)macOnBoard[5]));

        DPRINTF(WCDBG, ("WLAN: Storing WLAN MAC to datastore.\n"));
        DSSetVar(&wlanMacDSVal);
    }
    else
    {
        DPRINTF(WCDBG, ("WLAN: MAC not stored because WLAN not active!\n"));
        DSUnlockVar(e_WlanMACAddress);
    }
#else
    DPRINTF(WCDBG, ("WLAN: Setting wireless board's MAC to what is stored in NVRAM\n"));
    WlanSetMacAddrFromStaticNVRAM();
#endif
#else // if 0

    char macOnBoard[6];
    DPRINTF(WCDBG, ("WLAN: Checking MAC address stored on wireless board\n"));
    if (WlanGetMacAddr(WIFI_IFACE_NAME_STATION, macOnBoard))
    {
        int iface_id;

        for (iface_id = 0; iface_id < WIFI_NUM_IFACES; iface_id++)
        {
            if (iface_id == WIFI_IFACE_ID_STATION)
                WlanGetMacAddr(WIFI_IFACE_NAME_STATION, macOnBoard);

            if (iface_id == WIFI_IFACE_ID_UAP)
                WlanGetMacAddr(WIFI_IFACE_NAME_UAP, macOnBoard);

            if (iface_id == WIFI_IFACE_ID_WFD)
                WlanGetMacAddr(WIFI_IFACE_NAME_WFD, macOnBoard);

            DPRINTF(WCDBG, ("WLAN: MAC from wireless board for interface %d is %02X:%02X:%02X:%02X:%02X:%02X\n",
                        iface_id,
                        (uint8_t)macOnBoard[0], (uint8_t)macOnBoard[1], (uint8_t)macOnBoard[2],
                        (uint8_t)macOnBoard[3], (uint8_t)macOnBoard[4], (uint8_t)macOnBoard[5]));

            wifi_intf_set_var_mac_addr(iface_id, (uint8_t*)macOnBoard, 6);
        }
    }
#endif // if 0
}

/******************************************************************************
 * Function name: WlanWirelessSetRadioPresent
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void WlanWirelessSetRadioPresent(void)
{
    wifi_intf_set_var_is_wireless_radio_present(TRUE);
}


/******************************************************************************
 * Function name: wlanStartStopThreadFunc
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void wlanStartStopThreadFunc()
{
    uint32_t flags;

    SysWaitForInitLevel(INITLEVEL_1);

    while(true)
    {
        if(TX_SUCCESS == tx_event_flags_get(&wlan_startup_shutdown_flags, 
                                            WLAN_STARTUP_SHUTDOWN_MASK, 
                                            TX_OR_CLEAR, 
                                            &flags, 
                                            TX_WAIT_FOREVER))
        {
            if (flags & WLAN_STARTUP_FLAG)
            {
                while (1)
                {
                    tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
                    if (g_wlan_power_state == WLAN_OFF)
                    {
                        g_wlan_power_state = WLAN_STARTING_UP;
                        tx_mutex_put(&wlan_start_stop_mutex);
                        WlanWirelessHWTurnPowerOn();
                        break;
                    } 
                    else if ((g_wlan_power_state == WLAN_ON) || (g_wlan_power_state == WLAN_STARTING_UP))
                    {
                        // Already up or starting up
                        tx_mutex_put(&wlan_start_stop_mutex);
                        break;
                    }
            
                    // WLAN must be shutting down, wait for WLAN_OFF
                    tx_mutex_put(&wlan_start_stop_mutex);
                    tx_thread_sleep(10);
                }
            }

            if (flags & WLAN_SHUTDOWN_FLAG)
            {
                while (1)
                {
                    tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
                    if (g_wlan_power_state == WLAN_ON)
                    {
                        g_wlan_power_state = WLAN_SHUTTING_DOWN;
                        tx_mutex_put(&wlan_start_stop_mutex);
                        WlanWirelessHWTurnPowerOff();
                        break;
                    } 
                    else if ((g_wlan_power_state == WLAN_OFF) || (g_wlan_power_state == WLAN_SHUTTING_DOWN))
                    {
                        // Already down or shutting down
                        tx_mutex_put(&wlan_start_stop_mutex);
                        break;
                    }

                    // WLAN must be starting up, wait for WLAN_ON
                    tx_mutex_put(&wlan_start_stop_mutex);
                    tx_thread_sleep(10);
                }
            }
        }
    }
}


/******************************************************************************
 * Function name: WirelessInit
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void WirelessInit(void)
{
    wifi_intf_radio_power_init();

    void mlan_event_initialize();
    mlan_event_initialize();

    wifi_oem_pre_init();   
    
    // Shared Resources
    wifi_intf_create_resources();    // Let the driver create its OS resources
    tx_mutex_create(&wlan_scan_mutex, "wlan_scan_mutex", 0);
    tx_mutex_create(&wlan_start_stop_mutex, "wlan_start_stop_mutex", 0);
    tx_event_flags_create(&wlan_startup_shutdown_flags, "wlan_startup_shutdown_flags");

    tx_queue_create(&wconfigMsgQ, "wconfigMsgQ",TX_4_ULONG, wconfigMsgQBuf, NUM_WCONFIG_MSGS*sizeof(MESSAGE));
    SYRegister(WIRELESSCONFIGID, &wconfigMsgQ);

    tx_queue_create(&wtimerMsgQ, "wtimerMsgQ",sizeof(MESSAGE), 
            wtimerMsgQBuf, NUM_WCONFIG_MSGS*sizeof(MESSAGE));

    // Station Resources
    tx_timer_create(&wcfgStaInfoTimer, "wcfgStaInfoTimer", 
            wcfgInfoTimerFunc, WIFI_IFACE_ID_STATION, 100, 0, TX_NO_ACTIVATE);

    tx_timer_create(&wcfgAdHocRejoinTimer, "wcfgAdHocRejoinTimer", 
            wcfgAdHocRejoinTimerFunc, 0, AD_HOC_REJOIN_TIME, 0, TX_NO_ACTIVATE);

    // Micro-AP Resources
    tx_timer_create(&wcfgUapInfoTimer, "wcfgUapInfoTimer", 
            wcfgInfoTimerFunc, WIFI_IFACE_ID_UAP, 100, 0, TX_NO_ACTIVATE);

    // Wifi-Direct Resources
    tx_timer_create(&wcfgWfdInfoTimer, "wcfgWfdInfoTimer", 
            wcfgInfoTimerFunc, WIFI_IFACE_ID_WFD, 100, 0, TX_NO_ACTIVATE);


    // Start threads

    tx_thread_create( &wlanStartStopThread, "wlanStartStop",
            (void(*)(ULONG))wlanStartStopThreadFunc, (int) NULL, wlanStartStopStack,
            STARTSTOP_STACK_SIZE, THR_PRI_NORMAL, THR_PRI_NORMAL,
            1, TX_AUTO_START );

    tx_thread_create( &wtimerThread, "mlan timer thread",
		       	(void(*)(ULONG))wtimerThreadFunc, 0x0, wtimerStack,
			WTIMER_STACK_SIZE, THR_PRI_NORMAL, THR_PRI_NORMAL,
			1, TX_AUTO_START );


    // TODO - Move this?  Wait to create this until the initial scan has completed?
    tx_thread_create( &wconfigThread, "twconfig",
                (void(*)(ULONG))wconfigThreadFunc, (int) NULL, wconfigStack,
            WCONFIG_STACK_SIZE, THR_PRI_NORMAL, THR_PRI_NORMAL,
            1, TX_AUTO_START );

    startWPSThread();

#ifdef HAVE_WIFI_CERT_TEST     
    startSigma();
#endif

}

/******************************************************************************
 * Function name: WlanWirelessHWDisable
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void WlanWirelessHWDisable(void)
{
    wifi_hw_set_var_is_enabled(FALSE);
}


/******************************************************************************
 * Function name: WlanWirelessHWEnable
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void WlanWirelessHWEnable(void)
{
    wifi_hw_set_var_is_enabled(TRUE);
}


/******************************************************************************
 * Function name: WlanWirelessHWActive
 *
 * Description: 
 *
 * Parameters: 
 *
 * Author: 
 *
 * Notes: 
 *
 *****************************************************************************/
void WlanWirelessHWActive(bool UsingWireless)
{
    gUsingWireless = UsingWireless;
}


/** 
 * \brief Start up the wireless protocol stack and HW drivers.
 * 
 * \param none
 * 
 * \return  if successful AT_SUCCESS, AT_FAILURE if not.
 * 
 * This function will startup the wireless I/O drivers.  If the
 * start of the I/O drivers is successful it will then start the
 * higher level wireless protocol stack.
 **/
ATSTATUS WlanStartupInternal(void)
{
    ATSTATUS retVal = AT_FAILURE;

    DPRINTF(WCDBG, ("WLAN: ==> WlanStartupInternal\n"));

    g_curWirelessSettings.mode = 0;

    wConfigInitSta();  //-jrs
    wConfigInitUap();
    wConfigInitWfd();

    WirelessInitMACAddress();

    wifi_intf_startup();

    wifi_oem_init();

    tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
    g_wlan_power_state = WLAN_ON;
    tx_mutex_put(&wlan_start_stop_mutex);

    switch(g_after_power_up_event)
    {
    case POWER_DOWN_AFTER_SCAN:
        // Send Start Scan Message
        SYMsgSend(WIRELESSCONFIGID, &gScanMsg);
        break;
    case POWER_DOWN_AFTER_MAC_INIT:
        // Turn off power
        g_after_power_up_event = NONE;
        WlanShutdown();
        break;
    case RUN_WPS_AFTER_POWER_ON:
        // Send WPS Start Message
        SYMsgSend(WPSID, &gWPSMsg);

        // TODO: this may be a race condition - once we set this the code may power 
        //       down until the WPS thread picks up the message and marks "running_wps"
        g_after_power_up_event = NONE;
        break;
    default:
        break;
    }

    DPRINTF(WCDBG, ("WLAN: <== WlanStartupInternal\n"));
    return retVal;
}

/** 
 * \brief Start up the wireless protocol stack any time other
 * than at bootup.
 * 
 * \param none
 * 
 * \return  if successful AT_SUCCESS, AT_FAILURE if not.
 * 
 * This function will startup the wireless I/O drivers.  If the
 * start of the I/O drivers is successful it will then start the
 * higher level wireless protocol stack.
 **/
ATSTATUS WlanStartup(void)
{
    tx_event_flags_set(&wlan_startup_shutdown_flags, WLAN_STARTUP_FLAG, TX_OR);

    return(AT_SUCCESS);
}

/** 
 * \brief Shutdown the wireless protocol stack and HW drivers.
 * 
 * \param none
 * 
 * \return  if successful AT_SUCCESS, AT_FAILURE if not.
 * 
 * This function will shut down the wireless protocol stack and HW
 * driver and hold the HW in reset.
 **/
ATSTATUS WlanShutdownInternal(void)
{
    wpaSupConfig sConfig;

    DPRINTF(WCDBG, ("WLAN: ==> WlanShutdownInternal\n"));

    WlanWirelessHWActive(FALSE);

    // notify the wireless MAC level driver that the protocol stack is going down
    wifi_intf_shutdown_notify();

    sConfig.WPA_ON = 0;
    wake_up_interruptible(&((moal_private *)(g_wlanDev->priv))->ioctl_wait_q);
    SupConfig(sConfig); 

    g_wirelessInitialized = FALSE;

    wifi_oem_deinit();

    // deactivate timers
    tx_timer_deactivate(&wcfgStaInfoTimer);
    tx_timer_deactivate(&wcfgUapInfoTimer);
    tx_timer_deactivate(&wcfgWfdInfoTimer);
    tx_timer_deactivate(&wcfgAdHocRejoinTimer);

    if (wifi_sta_badwepkey_timer != NULL)
    { 
        wifi_sta_badwepkey_timer = delay_msec_timer_cancel(wifi_sta_badwepkey_timer);
    }

    DPRINTF(WCDBG, ("WLAN: <== WlanShutdownInternal\n"));
    return AT_SUCCESS;
}

void WlanShutdownComplete(void)
{
    tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
    g_wlan_power_state = WLAN_OFF;
    tx_mutex_put(&wlan_start_stop_mutex);
}

/** 
 * \brief WlanIsShutdownCompleted
 * 
 * \param none
 * 
 * \return  true if shutdown completed otherwise false
 * 
 * This function returns a boolean to indicate if shutdown has completed
 **/
BOOL WlanIsShutdownCompleted(void)
{
    BOOL retval = false;

    tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
    if (g_wlan_power_state == WLAN_OFF)
    {
        retval = true;
    }
    tx_mutex_put(&wlan_start_stop_mutex);

    return retval;
}

/** 
 * \brief Shutdown the wireless protocol stack and HW drivers.
 * 
 * \param none
 * 
 * \return  if successful AT_SUCCESS, AT_FAILURE if not.
 * 
 * This function will shut down the wireless protocol stack and HW
 * driver and hold the HW in reset.
 **/
ATSTATUS WlanShutdown()
{
    // Resume in case the module is in Host Sleep
    wlan_usb_resume();

    wifi_intf_sta_set_state(WLAN_INACTIVE);
    wifi_intf_sta_set_link_status(WIFI_INTF_LINK_EVENT_DOWN);
    netdrvr_uap_shared_notify_link_down(WIFI_IFACE_ID_UAP);
    netdrvr_uap_shared_notify_link_down(WIFI_IFACE_ID_WFD);

    tx_event_flags_set(&wlan_startup_shutdown_flags, WLAN_SHUTDOWN_FLAG, TX_OR);

    return(AT_SUCCESS);
}

void wifi_sta_set_disable_by_wired_link(BOOL disabled_by_wired)
{
    if (gWifiStaDisabledByWiredLink != disabled_by_wired)
    {
        if (disabled_by_wired)
        {
            wifi_intf_sta_set_state(WLAN_INACTIVE);
        }
                
        gWifiStaDisabledByWiredLink = disabled_by_wired;
        notify_wifi_sta_vars_changed();
    }
}



/******************************************************************************
 * Function name: WlanIsWirelessHWPowerOn
 *
 * Description: Check nReset value
 *
 * Parameters: NA
 *
 * Author: 
 *
 * Notes: Returns value of nReset
 *
 *****************************************************************************/
bool WlanIsWirelessHWPowerOn(void)
{
    bool radio_on = false;

    wifi_intf_get_radio_on_status(&radio_on);

    return(radio_on);
}


/******************************************************************************
 * Function name: WlanWirelessHWTurnPowerOff
 *
 * Description: Set nReset low (Disable Wireless module)
 *
 * Parameters: NA
 *
 * Author: 
 *
 * Notes: Notifies control panel to turn off connect LED
 *
 *****************************************************************************/
void WlanWirelessHWTurnPowerOff(void)
{
    DPRINTF(DBG_OUTPUT|DBG_LOUD, ("  TURN OFF RADIO  \n"));

    // Turn off radio
    wifi_intf_turn_radio_off();

    // Update state
    wifi_intf_sta_set_state(WLAN_INACTIVE);
    wifi_intf_uap_set_state(WLAN_INACTIVE, NULL, 0, NULL, NULL);

    //Reset the bad key retry counter
    g_wifi_sta_badwepkey_retry_counter = 0;
}


/******************************************************************************
 * Function name: WlanWirelessHWTurnPowerOn
 *
 * Description: Set nReset high (Enable Wireless module)
 *
 * Parameters: NA
 *
 * Author: 
 *
 * Notes:  Notifies control panel to blink connect LED
 *
 *****************************************************************************/
void WlanWirelessHWTurnPowerOn(void)
{
    // Initiate power manager wake
    //pwr_mgr_goto_active_state(0x0);
    
    // Wait for power manager to indicate active state
    //pwr_mgr_wait_until_active_state();

    DPRINTF(DBG_OUTPUT|DBG_LOUD, ("  TURN ON RADIO  \n"));

    // Turn on radio
    wifi_intf_turn_radio_on();

    if (WlanIsWirelessInstalled())
    {
#if 0
        // Update state appropriately
        if (WlanIsWirelessADHOC())
            wifi_intf_sta_set_state(WLAN_ADHOC);
        else
            wifi_intf_sta_set_state(WLAN_ASSOCIATING);
#endif
    }
}


error_type_t wifi_hw_get_var_is_active(bool *active)
{
    if (active == NULL) 
    {
        return FAIL;
    }

    if (gUsingWireless && WlanIsWirelessInstalled())
    {
        *active = true;
    }
    else
    {
        *active = false;
    }

    return OK;
}

/** 
 * \brief Returns the state of the wireless HW and protocol stacks.
 * 
 * \param none
 * 
 * \return  TRUE if the wireless HW and protocol stacks are operational; FALSE else.
 * 
 * The wireless protocol stacks and HW may be enabled and disabled through the
 * WlanStartup and WlanShutdown functions.  This function may be used to determine
 * whether or not the stack is started or shutdown
 **/
BOOL WlanIsWirelessHWActive(void)
{
    bool active;

    wifi_hw_get_var_is_active(&active);

    return active;
}

/** 
 * \brief Returns the state of the wireless net enable.
 * 
 * \param none
 * 
 * \return  TRUE if the wireless is enabled ; FALSE else.
 * 
 **/
BOOL WlanIsWirelessHWEnabled(void)
{
    if (WlanIsWirelessInstalled())
    {
        bool is_wireless_hw_enabled;
        wifi_hw_get_var_is_enabled(&is_wireless_hw_enabled);

        return is_wireless_hw_enabled;
    }

    return FALSE;
}


/** 
 * \brief Returns the state of the ADHOC wireless communication mode.
 * 
 * \param none
 * 
 * \return  TRUE if the ADHOC ; FALSE else.
 * 
 **/
BOOL WlanIsWirelessADHOC(void)
{
    wifi_intf_comm_mode_t comm_mode;
    wifi_intf_get_var_comm_mode(WIFI_IFACE_ID_STATION, &comm_mode);
    if(comm_mode != AD_HOC)
    {
        return FALSE;
    }

    return TRUE;
}

/** 
 * \brief Returns whether or not a product has wireless HW installed and capable of operating.
 * 
 * \param none
 * 
 * \return  WB_OK if board attached and OK; else WB_NOT_INSTALLED or WB_BAD_FIRMWARE
 * 
 * This function allows wireless and non-wireless products to be created from the same
 * binary image.  It is used to determine whether or not the wireless HW is installed in the
 * product.
 **/
BOOL WlanIsWirelessInstalled(void)
{
    bool present;

    wifi_hw_get_var_is_radio_present(&present);
    if (present)
    {
        return TRUE;
    }
    return FALSE;
}


int wifi_intf_get_var_station_list(uint8_t iface_id, uap_mlan_ds_sta_list *sta_list)
{
    net_device *wifi_dev;
    struct ifreq ifr;

    if (sta_list == NULL)
    {
        return -1;
    }

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_ifru.ifru_data = (void *)sta_list;

    if (iface_id == WIFI_IFACE_ID_UAP || iface_id == WIFI_IFACE_ID_UAP_PRI0) 
    {
        // Get uap dev
        strncpy(ifr.ifr_ifrn.ifrn_name, WIFI_IFACE_NAME_UAP, strlen(WIFI_IFACE_NAME_UAP));
        if ((wifi_dev = wlan_get_net_uap_dev()) == NULL)
        {
            DPRINTF(WCDBG,("ERR: Can't get uAP dev (%s:%d)!\n", __FILE__, __LINE__));
            return -1;
        }
    }
#ifdef WIFI_DIRECT_SUPPORT
    else if (iface_id == WIFI_IFACE_ID_WFD) 
    {
        // Get uap dev
        strncpy(ifr.ifr_ifrn.ifrn_name, WIFI_IFACE_NAME_WFD, strlen(WIFI_IFACE_NAME_WFD));
        if ((wifi_dev = wlan_get_net_wfd_dev()) == NULL)
        {
            DPRINTF(WCDBG,("ERR: Can't get WFD dev (%s:%d)!\n", __FILE__, __LINE__));
            return -1;
        }
    }
#endif    
    else
    {
        return -1;
    }

    if (wifi_dev->do_ioctl(wifi_dev, (struct ifreq *) &ifr, UAP_GET_STA_LIST) != 0)
    {
        return -1;
    }

    return sta_list->sta_count;
}

int wlan_wpaon_ioctl(mlan_private *priv)
{
	int	ret = 0;

	ENTER();
	
    priv->sec_info.wpa_enabled = TRUE;
    priv->sec_info.wpa2_enabled = TRUE;

	LEAVE();

	return ret;
}

int wlan_wpaoff_ioctl(mlan_private *priv)
{
	int	ret = 0;

	ENTER();
	
    priv->sec_info.wpa_enabled = FALSE;
    priv->sec_info.wpa2_enabled = FALSE;

	LEAVE();

	return ret;
}

#define MAX_SSID_LEN         32
#define SSID_MAX_WPS_IE_LEN  256
#define SSID_MAX_WPA_IE_LEN  40
#define SCAN_AP_LIMIT        64

#define WPS_STATUS_SUCCESS      (0)
#define WPS_STATUS_FAIL         (-1)

#define WPS_START_REG_DISCOVERY_PHASE   1
#define WPS_END_REG_DISCOVERY_PHASE     0

/**
 * struct wpa_scan_result - Scan results
 * @bssid: BSSID
 * @ssid: SSID
 * @ssid_len: length of the ssid
 * @wpa_ie: WPA IE
 * @wpa_ie_len: length of the wpa_ie
 * @rsn_ie: RSN IE
 * @rsn_ie_len: length of the RSN IE
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @maxrate: maximum supported rate
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 */

/** Max WFD_IE length */
#define MAX_WFD_IE_LEN                256

#if 0
typedef struct driver_scan_result {
    u8 bssid[ETH_ALEN];
    u8 ssid[32];
    size_t ssid_len;
    u8 wpa_ie[SSID_MAX_WPA_IE_LEN];
    size_t wpa_ie_len;
    u8 rsn_ie[SSID_MAX_WPA_IE_LEN];
    size_t rsn_ie_len;
    u8 wps_ie[SSID_MAX_WPS_IE_LEN];
    size_t wps_ie_len;
    u8 wfd_ie[MAX_WFD_IE_LEN];
    size_t wfd_ie_len;
    int freq;
    u16 caps;
    int qual;
    int noise;
    int level;
    int maxrate;
} SCAN_RESULTS, *PSCAN_RESULTS;
#endif

/** 
 *  @brief  Send WPS session command to WLAN driver
 *
 *  @param enable     0 - WPS Session Disable, 1 - WPS Session Enable
 *  @return           WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int wps_wlan_session_control(int enable)
{
    struct net_device *dev = wlan_get_net_dev();
    int ret = WPS_STATUS_SUCCESS;
    mlan_private *priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];
    //mlan_adapter *Adapter = priv->adapter;

    if (enable)
    {
      priv->wps.session_enable = TRUE;
    }
    else
    {
      priv->wps.session_enable = FALSE;
    }

    return ret;
}

void wps_set_gen_ie_helper(u8* buf, int buf_len)
{
    struct net_device *dev = wlan_get_net_dev();
    mlan_private *priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];
    
    wlan_set_gen_ie_helper(priv, buf, buf_len);
}


/** 
 *  @brief Reset WPS IE for scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wlan_reset_scan_wps_ie(void)
{
    u8      buf[10];
    int     buf_len;
    u8      wps_oui[4] = {0x00, 0x50, 0xF2, 0x04};
    int     ret = WPS_STATUS_SUCCESS;
    struct net_device *dev = wlan_get_net_dev();
    mlan_private *priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];

    buf[0] = 221;      /* ElementID of WPS_IE */
    memcpy(&buf[2], wps_oui, sizeof(wps_oui));
    buf[1] = sizeof(wps_oui);        /* IE len */
    buf_len = sizeof(wps_oui) + 2;   /* Add IE_ID and IE_Len fileds */

    wlan_set_gen_ie_helper(priv, buf, buf_len);

    return ret;
}


/** 
 *  @brief  Prepare WPS IE
 *
 *  @param ptr      A pointer to buffer which store this attribute
 *  @return         Length of Attribute TLV
 */
/* XXX: Actually we should do this for both Enrollee & Registrar */
int wps_ie_prepare(u8 * ptr)
{
    PTLV_DATA_HEADER ptlv;
    u8 msg_value_char;
    u16 msg_value_short;
    u16 message_length = 0;
    int length;

    ENTER();

    /* OUI */
    {
        char array[] = WIFIDIRECT_CONFIG_DEVICE_INFO_PRIMARY_OUI_DEFAULT;
        memcpy(ptr, array, sizeof(array));
        message_length += sizeof(array);
        ptr += message_length;
    }

    /* Version */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Version);
    ptlv->length = 1;
    ptr += SZ_TLV_HEADER;
    msg_value_char = WIFIDIRECT_WPS_VERSION_DEFAULT;
    memcpy(ptr, &msg_value_char, 1);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Request Type */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Request_Type);
    ptlv->length = 1;
    ptr += SZ_TLV_HEADER;
    msg_value_char = WIFIDIRECT_WPS_REQUEST_TYPE_DEFAULT;      /* 0x0 Enrollee, Info only */
    memcpy(ptr, &msg_value_char, 1);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Config Methods */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Config_Methods);
    ptlv->length = 2;
    ptr += SZ_TLV_HEADER;
    msg_value_short = htons(WIFIDIRECT_WPS_SPEC_CONFIG_METHODS_DEFAULT);
    memcpy(ptr, &msg_value_short, 2);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* UUID */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_UUID_E);
    ptr += SZ_TLV_HEADER;
    length = WPS_UUID_MAX_LEN;
    get_device_uuid_binary((char *) ptr, &length);
    ptlv->length = length;
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Primary Device Type */
    {
        char array[] = WIFIDIRECT_WPS_PRIMARY_DEVICE_TYPE_DEFAULT;
        ptlv = (PTLV_DATA_HEADER) ptr;
        ptlv->type = htons(SC_Primary_Device_Type);
        ptlv->length = sizeof(array);
        ptr += SZ_TLV_HEADER;
        memcpy(ptr, array, ptlv->length);
        message_length += SZ_TLV_HEADER + ptlv->length;
        ptr += ptlv->length;
        ptlv->length = htons(ptlv->length);
    }

    /* RF Bands */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_RF_Band);
    ptlv->length = 1;
    ptr += SZ_TLV_HEADER;
    msg_value_char = WIFIDIRECT_WPS_RFBAND_DEFAULT;
    memcpy(ptr, &msg_value_char, 1);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Association State */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Association_State);
    ptlv->length = 2;
    ptr += SZ_TLV_HEADER;
    msg_value_short = htons(WIFIDIRECT_WPS_ASSOCIATION_STATE_DEFAULT);       /* Not Associated */
    memcpy(ptr, &msg_value_short, ptlv->length);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Configuration Error */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Configuration_Error);
    ptlv->length = 2;
    ptr += SZ_TLV_HEADER;
    msg_value_short = htons(WIFIDIRECT_WPS_CONFIGURATION_ERROR_DEFAULT);
    memcpy(ptr, &msg_value_short, 2);
    message_length += (SZ_TLV_HEADER + ptlv->length);
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);

    /* Device Password ID */
    ptlv = (PTLV_DATA_HEADER) ptr;
    ptlv->type = htons(SC_Device_Password_ID);
    ptlv->length = 2;
    ptr += SZ_TLV_HEADER;
    msg_value_short = htons(WIFIDIRECT_WPS_DEVICE_PASSWORD_DEFAULT);  /* PIN or PBC ???*/
    memcpy(ptr, &msg_value_short, 2);
    message_length += SZ_TLV_HEADER + ptlv->length;
    ptr += ptlv->length;
    ptlv->length = htons(ptlv->length);


    /* Device Name */
    {
        char array[WPS_DEVICE_NAME_MAX_LEN + 1];
        array[WPS_DEVICE_NAME_MAX_LEN]=0x0;
		char *tmpstr;
        ptlv = (PTLV_DATA_HEADER) ptr;
        ptlv->type = htons(SC_Device_Name);
        ptr += SZ_TLV_HEADER;
        length = WPS_DEVICE_NAME_MAX_LEN;
	    platvars_get_usb_mfgstr(&tmpstr);
	    strncpy(array, tmpstr, WPS_DEVICE_NAME_MAX_LEN);
        length = strlen(array);
        memcpy(ptr, array, length);
        ptlv->length = length;
        message_length += SZ_TLV_HEADER + ptlv->length;
        ptr += ptlv->length;
        ptlv->length = htons(ptlv->length);
    }



    LEAVE();
    return message_length;
}


/** 
 *  @brief Set WPS IE for scan to WLAN Driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wlan_set_scan_wps_ie(void)
{
    u8      *buf;
    int     wps_ie_size, buf_len;
    int     ret = WPS_STATUS_SUCCESS;

    ENTER();

    buf = (u8 *)MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf) {
        memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        buf[0] = 221;          /* ElementID of WPS_IE */
        wps_ie_size = wps_ie_prepare(&buf[2]);
        buf[1] = (u8)wps_ie_size;  /* IE len */
        buf_len = wps_ie_size + 2; /* Add IE_ID and IE_Len fileds */

        wps_set_gen_ie_helper(buf, buf_len);

        MEM_FREE_AND_NULL(buf);
    }

    LEAVE();
    return ret;

}


/** 
 *  @brief Send scan ioctl command to WLAN driver
 *
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_wlan_set_scan(char *ssid, int ssid_len)
{
    return WlanShimCSetScanWithSsid(ssid, ssid_len);
}

/** 
 *  @brief          Get scan result from WLAN driver
 *
 *  @param results  A pointer to scan result buffer
 *  @return         Number of scan if success, WPS_STATUS_FAIL if fail
 */
int wlan_get_scan_results(SCAN_RESULTS *results)
{
    
    int 			i, min_aps = 0, ap_count = 0;
    struct net_device *dev = wlan_get_net_dev();
    mlan_private *priv = ((pmlan_adapter)((moal_private *) dev->priv)->phandle->pmlan_adapter)->priv[0];
    mlan_adapter		*Adapter = priv->adapter;

    min_aps = (SCAN_AP_LIMIT < Adapter->num_in_scan_table) ?
			SCAN_AP_LIMIT  : Adapter->num_in_scan_table;

    DPRINTF(WCDBG, ("wlan_get_scan_results found %d devices\n", Adapter->num_in_scan_table));

    for(i = 0; i < min_aps; i++)
    {
        if (gWifiScanConfig == SCAN_2_4GHZ_ONLY)
        {
            if (Adapter->pscan_table[i].channel > MAX_CHANNELS_BG)
            {
                //If we have scan restriction in place for 2.4GHz only, do not associate with an AP advertising in the
                //5GHz band
                continue;
            }    
        }

        ap_count++;
        //DPRINTF(WCDBG, ("wlan_get_scan_results %d ssid %s\n", i, Adapter->pscan_table[i].ssid.ssid));
        memcpy(results[i].bssid, Adapter->pscan_table[i].mac_address, ETH_ALEN);
        memcpy(results[i].ssid, Adapter->pscan_table[i].ssid.ssid, Adapter->pscan_table[i].ssid.ssid_len);
        results[i].ssid_len = Adapter->pscan_table[i].ssid.ssid_len;
        memcpy(&results[i].caps, &Adapter->pscan_table[i].cap_info,sizeof(IEEEtypes_CapInfo_t)); 
        results[i].freq = mapping_chan_to_freq(Adapter->pscan_table[i].channel);

        if (Adapter->pscan_table[i].pwpa_ie)
        {
            if (Adapter->pscan_table[i].pwpa_ie->vend_hdr.element_id == WPA_IE) {
              results[i].wpa_ie_len = Adapter->pscan_table[i].pwpa_ie->vend_hdr.len
                          + 2; 
              memcpy(results[i].wpa_ie, (Adapter->pscan_table[i].pwpa_ie), results[i].wpa_ie_len); 
            
              //DPRINTF(WCDBG, ("wlan_get_scan_results WPA IE len %d\n", results[i].wpa_ie_len));
            }
        }

        if (Adapter->pscan_table[i].prsn_ie)
        {
            if (Adapter->pscan_table[i].prsn_ie->ieee_hdr.element_id == RSN_IE) {
              results[i].rsn_ie_len = Adapter->pscan_table[i].prsn_ie->ieee_hdr.len
                          + 2; 
              memcpy(results[i].rsn_ie, (Adapter->pscan_table[i].prsn_ie), results[i].rsn_ie_len); 
    
              //DPRINTF(WCDBG, ("wlan_get_scan_results RSN IE len %d\n", results[i].rsn_ie_len));
            }
        }

        if (Adapter->pscan_table[i].pwps_ie)
        {
            if (Adapter->pscan_table[i].pwps_ie->vend_hdr.element_id == WPS_IE) {
              results[i].wps_ie_len = Adapter->pscan_table[i].pwps_ie->vend_hdr.len
                          + 2; 
              memcpy(results[i].wps_ie, (Adapter->pscan_table[i].pwps_ie), results[i].wps_ie_len); 
            
              //DPRINTF(WCDBG, ("wlan_get_scan_results WPS IE len %d\n", results[i].wps_ie_len));
            }
        }
    }
    return ap_count;    
}

void wirelessStartWPSEnrollee(char *ssid, char *pin)
{
    wirelessStartWPS(WIFI_IFACE_NAME_STATION, WPS_ENROLLEE, ssid, pin);
}

void wirelessStartWPSRegistrar(char *pin)
{
    wirelessStartWPS(WIFI_IFACE_NAME_UAP, WPS_REGISTRAR, NULL, pin);
}

void wirelessStartWFDWPSRegistrar(char *pin)
{
    wirelessStartWPS(WIFI_IFACE_NAME_WFD, WPS_REGISTRAR, NULL, pin);
}

void wirelessStartWFDWPSEnrollee(char *ssid, char *pin)
{
    wirelessStartWPS(WIFI_IFACE_NAME_WFD, WPS_ENROLLEE, ssid, pin);
}


/** 
 * \brief Start WPS Registrar or Enrollee in PIN or PBC mode
 * 
 * \param iface Interface
 *
 * \param role Registrar or Enrollee
 * 
 * \param pin Start pin mode with given pin. (NULL for
 * push-button mode)
 * 
 * \param ssidForPIN When in pin mode, this is used to select
 * which access point to connect to.
 * 
 * 
 **/
void wirelessStartWPS(char *iface, int role, char *ssid, char *pin)
{
    char          *pinCopy = NULL;
    char          *ssidCopy = NULL;
    MESSAGE        wpsMsg;
    t_wps_request *wps_config;
    
    updateWPSOutcome(WPS_STATE_PROCESSING);
   
    // Make a local copy of the ssid, if one was provided
    if (ssid != NULL)
    {
        ssidCopy = (char*)MEM_MALLOC(strlen(ssid) + 1);
        ASSERT(ssidCopy != NULL);

        strcpy(ssidCopy, ssid);
    }

    // Make a local copy of the pin so caller doesn't have to worry about it
    if (pin != NULL)
    {
        pinCopy = (char*)MEM_MALLOC(strlen(pin) + 1);
        ASSERT(pinCopy != NULL);

        strcpy(pinCopy, pin);
    }

    wps_config = (t_wps_request *)MEM_MALLOC(sizeof(t_wps_request));
    ASSERT(wps_config != NULL);

    wps_config->role  = role;
    wps_config->iface = (char*)MEM_MALLOC(11);
    ASSERT(wps_config->iface != NULL);
    if (iface != NULL)
    {
        strcpy(wps_config->iface, iface);
    }
    else
    {
        if (role == WPS_REGISTRAR)
        {
            strcpy(wps_config->iface, WIFI_IFACE_NAME_UAP);
        }
        else
        {
            strcpy(wps_config->iface, WIFI_IFACE_NAME_STATION);
        }
    }
    wps_config->ssid      = ssidCopy;
    wps_config->pin       = pinCopy;
 
    memset(&wpsMsg, 0, sizeof(MESSAGE));
    wpsMsg.msgType = MSG_RUN_WPS;
    wpsMsg.param3  = wps_config;

    if (wifi_intf_get_var_is_wps_running() == FALSE)
    {
        DPRINTF(WCDBG, ("Set WPS is running\n"));
        wifi_intf_set_var_is_wps_running(TRUE);

        // Make sure a startup or shutdown is not in progress
        tx_mutex_get(&wlan_start_stop_mutex, TX_WAIT_FOREVER);
        if ((g_wlan_power_state == WLAN_OFF) || (g_wlan_power_state == WLAN_SHUTTING_DOWN))
        {
            // Keep a copy of the message we would have sent and send it
            // we finish turning on the power
            memcpy(&gWPSMsg, &wpsMsg, sizeof(wpsMsg));
    
            g_after_power_up_event = RUN_WPS_AFTER_POWER_ON;
            tx_mutex_put(&wlan_start_stop_mutex);
            DPRINTF(WCDBG, ("Turn on radio and start WPS\n"));
            WlanStartup();
        }
        else
        {
            tx_mutex_put(&wlan_start_stop_mutex);
            DPRINTF(WCDBG, ("Send message to start WPS\n"));
            SYMsgSend(WPSID, &wpsMsg);
        }
    }
    else
    {
        DPRINTF(WCDBG, ("Request to start WPS occurred while it is already running\n"));
        SYMsgSend(WPSID, &wpsMsg);
    }
}

/** 
 * \brief Stop WPS if it is currently running
 * 
 **/
void wirelessCancelWPS(void)
{
    MESSAGE wpsMsg;
    wpsMsg.msgType = MSG_CANCEL_WPS;
    SYMsgSend(WPSID, &wpsMsg);
}

void notify_wifi_sta_vars_changed(void)
{
    if(WlanIsWirelessInstalled())
    {
        DPRINTF(WCDBG, ("WLAN: startApplyStaTimer\n"));

        // Make sure they use a valid SSID.  Re-init to default if necessary.
        wirelessEnsureValidStaSSID();

        // Stop trying to rejoin
        tx_timer_deactivate(&wcfgAdHocRejoinTimer);

        /* update timer; when timeout done,
         * get and apply settings
         */
        tx_timer_deactivate(&wcfgStaInfoTimer);
        tx_timer_change(&wcfgStaInfoTimer, 175, 0);
        tx_timer_activate(&wcfgStaInfoTimer);
    }
}

void notify_wifi_uap_vars_changed()
{
    if(WlanIsWirelessInstalled())
    {
        DPRINTF(WCDBG, ("WLAN: startApplyUapTimer\n"));

        // Make sure they use a valid SSID.  Re-init to default if necessary.
        //wirelessEnsureValidUapSSID();

        /* update timer; when timeout done,
         * get and apply settings
         */
        tx_timer_deactivate(&wcfgUapInfoTimer);
        tx_timer_change(&wcfgUapInfoTimer, 175, 0);
        tx_timer_activate(&wcfgUapInfoTimer);
    }
}

void notify_wifi_uap_pri0_vars_changed()
{
    if(WlanIsWirelessInstalled())
    {
        DPRINTF(WCDBG, ("WLAN: startApplyUapPri0Timer\n"));

        /* update timer; when timeout done,
         * get and apply settings
         */
        tx_timer_deactivate(&wcfgUapInfoTimer);
        tx_timer_change(&wcfgUapInfoTimer, 175, 0);
        tx_timer_activate(&wcfgUapInfoTimer);
    }
}

void notify_wifi_wfd_vars_changed()
{
    if(WlanIsWirelessInstalled())
    {
        DPRINTF(WCDBG, ("WLAN: startApplyWfdTimer\n"));

        // Make sure they use a valid SSID.  Re-init to default if necessary.
        wirelessEnsureValidWfdSSID();

        /* update timer; when timeout done,
         * get and apply settings
         */
        tx_timer_deactivate(&wcfgWfdInfoTimer);
        tx_timer_change(&wcfgWfdInfoTimer, 175, 0);
        tx_timer_activate(&wcfgWfdInfoTimer);
    }
}

void notify_wifi_hw_vars_changed()
{
    MESSAGE nxtMsg;
    nxtMsg.msgType = MSG_WIRELESS_UPDATE;
    nxtMsg.param1  = WIFI_NUM_IFACES;
    
    // Shut down the radio so we are forced to power back up and re-apply
    // the settings
    WlanShutdown();

    tx_queue_send(&wconfigMsgQ, &nxtMsg, TX_NO_WAIT);
}

//Setuserscan : Added to make sure that we can restrict scan to specific bands
int WlanSetUserScanCfg(char * ssid, int ssid_len)
{
    wlan_ioctl_user_scan_cfg scan_req;
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;

    tx_mutex_get(&wlan_scan_mutex, TX_WAIT_FOREVER);
    
    memset(&scan_req, 0x00, sizeof(scan_req));

    if (ssid != NULL)
    {
        //Scan with a specific SSID
        strncpy(scan_req.ssid_list[0].ssid, ssid, sizeof(scan_req.ssid_list[0].ssid));
        scan_req.ssid_list[0].max_len = 0;
    }

    //Restrict scan to only 2.4GHz Band
    if (gWifiScanConfig == SCAN_2_4GHZ_ONLY)
    {
        scan_req.chan_list[0].chan_number= 0;
        scan_req.chan_list[0].radio_type = 0;
        scan_req.chan_list[0].radio_type |= 0x80; //Band specified
    }    

    //Send the ioctl
    iwr.u.data.pointer = (caddr_t) & scan_req;
    iwr.u.data.length = sizeof(scan_req);
    iwr.u.data.flags = WOAL_SET_USER_SCAN;
 
    if (g_wlanDev->do_ioctl(g_wlanDev,(struct ifreq *) &iwr, WOAL_SET_GET_2K_BYTES) < 0)
    {
        DPRINTF(WCDBG,("WlanSetUserScan: IOCTL failed!!\n"));
        ret = MLAN_STATUS_FAILURE; 
    }        
    
    tx_mutex_put(&wlan_scan_mutex);

    return ret; 
}

error_type_t wifi_intf_get_var_rssi_dbm(uint8_t iface_id, int32_t *rssi)
{
    if (rssi == NULL)
    {
        return WIFI_INTF_FAIL;
    }

    if (iface_id != WIFI_IFACE_ID_STATION)
    {
        return WIFI_INTF_UNAVAILABLE;
    }

    if (WlanIsWirelessHWActive() && g_pmlanPriv->media_connected == MTRUE) 
    {
        struct  iwreq rwrq;
        int     cmd_req[2] = {1,    // IOCTL MAGIC: type:    RSSI
                              2};   // IOCTL MAGIC: subtype: Beacon AVG

        rwrq.u.data.length  = 2;
        rwrq.u.data.pointer = cmd_req;
        rwrq.u.data.flags = WOAL_SIGNAL;
        g_wlanDev->do_ioctl(g_wlanDev,(struct ifreq *) &rwrq, WOAL_SET_GET_SIXTEEN_INT);

        *rssi = cmd_req[0];         // adjust a bit to more closely match what we get from scan rssi
        return WIFI_INTF_OK;
    }
    else
    {
        *rssi = 0;
        return WIFI_INTF_FAIL;
    }
}

int rssi_dbm_to_signal_strength(int rssi_dbm)
{
    int strength = 0;

    // Set the signal strength (coarse scale, 1-5)
    if      (rssi_dbm < -70) strength = 1;
    else if (rssi_dbm < -60) strength = 2;
    else if (rssi_dbm < -50) strength = 3;
    else if (rssi_dbm < -35) strength = 4;
    else                     strength = 5;

    return strength;
}

error_type_t wifi_intf_get_var_bssid(uint8_t iface_id, uint8_t * bssid, uint8_t len)
{
    if (bssid == NULL)
    {
        return WIFI_INTF_FAIL;
    }

    if (iface_id != WIFI_IFACE_ID_STATION)
    {
        return WIFI_INTF_UNAVAILABLE;
    }

    if (WlanIsWirelessHWActive() && g_pmlanPriv->media_connected == MTRUE) 
    {
        memcpy(bssid, g_pmlanPriv->curr_bss_params.bss_descriptor.mac_address, MIN(ETH_ALEN, len));
        return WIFI_INTF_OK;
    }
    else
    {
        memset(bssid, 0, MIN(ETH_ALEN, len));
    }
    return WIFI_INTF_FAIL;
}

/** 
 * \brief Called by lower layers to determine if scans should be 
 *        limited to a particular channel.
 * 
 * \param none
 * 
 * \return channel that is allowed to be scanned or -1 to allow 
 *         scan of all channels
 * 
 * Current use is to prevent scans from disrupting print jobs 
 * that are active on the uAP. 
 *  
 **/
int wifi_get_scan_channel_restriction()
{
    int restrict_to_channel = -1;
#if 0
    // Are there print jobs active on the uAP?
    if (WifiUapIsEnabled() && wifi_uap_active_print_path_count)
    {
        restrict_to_channel = wifi_uap_channel_when_print_started;
        DPRINTF(WCDBG,("WIFI: Print job active on uAP - scan restricted to channel %d\n", restrict_to_channel));
    }
#endif
    return restrict_to_channel;
}

#ifdef WIFI_DIRECT_SUPPORT
void wifi_wfd_clear_active_ssid_flag(void)
{
     gWfdActiveSsidSet = false;    
}    

/** 
 * \brief Apply the current wireless settings to the uAP.
 * 
 * Tears down the existing uAP instance and re-inits it with
 * current settings.
 **/
void wifi_get_active_wfd_ssid_prefix(char *ssid_prefix)
{
    struct ifreq ifr;
    mlan_uap_bss_param *bss;
    apcmdbuf_bss_configure * cmd_buf;
    t_u8 *buf = NULL;
    t_u16 cmd_len;
    t_u16 buf_len = 0;
    net_device *wfd_dev;

    if (gWfdActiveSsidSet)    
    {
        //If the active ssid prefix is already set, don't bother sending down the ioctl
        wifi_intf_get_var_ssid_prefix( WIFI_IFACE_ID_WFD, (uint8_t *)ssid_prefix, MAX_SSID_NAME_LEN);
        return;
    }

    // Get wfd dev
    if ((wfd_dev = wlan_get_net_wfd_dev()) == NULL)
    {
        DPRINTF(WCDBG,("ERR:Cannot get wfd dev WlanSetWfdConfig!\n"));
        return;
    }

    buf_len = sizeof(apcmdbuf_bss_configure) + sizeof(mlan_uap_bss_param);
    buf = (t_u8 *) MEM_MALLOC(buf_len);
    if (!buf) {
        DPRINTF(WCDBG,("ERR:Cannot allocate buffer\n"));
        return;
    }

    memset((char *) buf, 0, buf_len);

    cmd_len = sizeof(apcmdbuf_bss_configure);
    cmd_buf = (apcmdbuf_bss_configure *) buf;
    bss = (mlan_uap_bss_param *) (buf + cmd_len);

    cmd_buf->action = MLAN_ACT_GET;

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, WIFI_IFACE_NAME_WFD, strlen(WIFI_IFACE_NAME_WFD));
    ifr.ifr_ifru.ifru_data = (void *) cmd_buf;

    if (wfd_dev->do_ioctl(wfd_dev, (struct ifreq *) &ifr, UAP_BSS_CONFIG) != 0)
    {
        DPRINTF(WCDBG,("WLAN: FAILED to retrieve settings\n"));
    }

    memcpy(ssid_prefix, bss->ssid.ssid, 9);
    ssid_prefix[9] = '\0';
 
    gWfdActiveSsidSet = true;
    wifi_intf_set_var_ssid_prefix( WIFI_IFACE_ID_WFD, (uint8_t *)ssid_prefix, MAX_SSID_NAME_LEN);
    
    MEM_FREE_AND_NULL(cmd_buf);
}
#endif

void wifi_update_channel_number(void)
{
    int channel;
    bool iface_enabled;

    channel = WlanGetChannel();

    //REVISIT: The current wireless modules operates using a single channel for the 
    //uAP and the wireless STA (uAP follows the STA). For the purpose, it is essential to update the nvram variable 
    //for channel information for both the uAP and the STA interfaces since this information will 
    //be made available to the rest of the system using wifi_intf_get_var_channel() API.
    //However, if we do decide to move to a wireless module capable of using dedicated channels for the uAP
    //and the STA, we should modify the following lines of code accordingly.
    wifi_intf_set_var_channel(WIFI_IFACE_ID_STATION, (uint8_t)channel);
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP, &iface_enabled);
    if (iface_enabled)
        wifi_intf_set_var_channel(WIFI_IFACE_ID_UAP, (uint8_t)channel);
    
    //Treat WFD and uAP_Pri0 the same as uAP
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_WFD, &iface_enabled);
    if (iface_enabled)
        wifi_intf_set_var_channel(WIFI_IFACE_ID_WFD, (uint8_t)channel);
         
    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP_PRI0, &iface_enabled);
    if (iface_enabled)
        wifi_intf_set_var_channel(WIFI_IFACE_ID_UAP_PRI0, (uint8_t)channel);

}    
