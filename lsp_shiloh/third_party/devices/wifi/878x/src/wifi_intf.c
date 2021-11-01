/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <sys/types.h>
#include "wlanInterface.h"
#include "wifi_intf.h"
#include "interrupt_api.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "wifi_intf_api.h"
#include "wifi_intf_vars.h"
#include "wifi_intf_config.h"
#include "os_net_device.h"
#include "logger.h"
#include "wifi_intf_event_api.h"
#include "event_observer.h"
#ifdef HAVE_POWER_MGR
#include "pwr_mgr_api.h"
#endif

#include "raw_socket_api.h"
#include "raw_bsd.h"


#define WDERR   (DBG_ERROR | DBG_OUTPUT)
#define WDSOFT  (DBG_SOFT | DBG_OUTPUT)
#define WDLOUD  (DBG_LOUD | DBG_OUTPUT)
#define WDENTRY (DBG_ENTRY | DBG_OUTPUT)

struct  wifi_handle_s
{
    void * network_handle;
};

UINT8 gWirelessBroadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
extern UINT8 gAddr[];

TX_MUTEX gTxQueueMutex;
t_packet_q gWirelessTXQ = {0,0,0};
TX_MUTEX gRxQueueMutex;
t_packet_q gWirelessRXQ = {0,0,0};
static BOOL gWirelessIsOpen = FALSE;
#ifdef HAVE_POWER_MGR
static BOOL gWirelessPwrOffAllow = FALSE;  // when true, always allow power off; when false, check more conditions..
#endif
#define STA 0
#define UAP 1

int wifi_intf_raw_send(RAW_IFACE_HANDLE iface, char* buff, int length);

ttUserInterface uAP_treck_interface = (ttUserInterface)-1; //this is intentionally not set to NULL, null is a valid raw interface index

TX_MUTEX gUAPTxQueueMutex;
t_packet_q gUAPWirelessTXQ = {0,0,0};
TX_MUTEX gUAPRxQueueMutex;
t_packet_q gUAPWirelessRXQ = {0,0,0};
UINT8 gUAPWirelessIsOpen = 0;

RAW_IFACE_HANDLE raw_iface_handle_uap0  = -1;
//RAW_IFACE_HANDLE raw_iface_handle_mlan0 = -1;
extern RAW_IFACE_HANDLE net_iface_raw;

bool wifi_raw_debug = false;

// the one and only network device -jrs
TX_MUTEX gWNetDevLock;
TX_MUTEX gLinkDownAckMutex;

#define WIRELESS_STA_XMIT_FLAG  1
#define WIRELESS_UAP_XMIT_FLAG  2
#define WIRELESS_XMIT_FLAG_MASK WIRELESS_STA_XMIT_FLAG | WIRELESS_UAP_XMIT_FLAG

#define WIRELESS_DRVR_XMIT_STACK_SIZE 16 * 1024
ALIGN8 char wireless_drvr_xmit_stack[WIRELESS_DRVR_XMIT_STACK_SIZE];
TX_THREAD wireless_drvr_xmit_thread;
TX_EVENT_FLAGS_GROUP wireless_drvr_xmit_flags;
TX_TIMER wireless_drvr_xmit_timer;
#define WIRELESS_DRVR_XMIT_CALLBACK_TIME    100

static BOOL gWirelessNetdrvrEnabled = FALSE;
static wlan_tkip_workaround_t gTKIPWorkaroundInfo;
static wlan_type_t g_current_wlan_sta_state = WLAN_INACTIVE;
static wlan_type_t g_current_wlan_uap_state = WLAN_INACTIVE;
static TX_MUTEX gWPSStateMutex;
static wifi_intf_wps_outcome_t gWPSState = WPS_STATE_NOT_YET_RUN;
static TX_MUTEX gWPSIsRunningMutex;
static bool gWPSIsRunning = false;
static bool gIsWirelessRadioPresent = false;

static BOOL wifi_intf_start_xmit(BOOL fTxComplete);
static BOOL wifi_uap_intf_start_xmit(BOOL fTxComplete);
wifi_intf_callback_funcs_t g_wifi_intf_callback_funcs = {0};

static uint32_t g_current_sta_link_status = Link_Down;
static uint32_t g_current_uap_link_status = Link_Down;



/********************************************* 
 *  WIFI UAP Events
 *********************************************/

#define WIFI_INTF_UAP_EVENT_ID 0x12d5f14a
#define IPV4_ALEN 4
#define DEVICE_NAME_LEN 32+1
/** Wifi Interface link status structure */
typedef struct wifi_intf_uap_event_s
{
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;
    wifi_intf_link_status_t intf_link_status;
    bool enabled;
    char device_name[DEVICE_NAME_LEN];    
    unsigned int device_name_length;
    unsigned char mac_addr[ETH_ALEN];
    unsigned char ip_addr[IPV4_ALEN];
} wifi_intf_uap_event_t;

/** Wifi Interface Event Constructor, initializes, doubles as get struct */
static wifi_intf_uap_event_t * wifi_intf_uap_event()
{
    /** Static structure */
    static wifi_intf_uap_event_t *ptr = 0;
    if ( 0 == ptr )
    {
        ptr = (wifi_intf_uap_event_t *) MEM_MALLOC(sizeof(wifi_intf_uap_event_t));
        ASSERT(ptr);
        memset(ptr, 0x0, sizeof(wifi_intf_uap_event_t));
		ptr->subject_observer_oid_key = WIFI_INTF_UAP_EVENT_ID;
        MInitList(&(ptr->observers));
    }
    return ptr;
}

int32_t wifi_intf_uap_get_num_assoc(void)
{
    uap_mlan_ds_sta_list *sta_list;
    int32_t ret_val = 0;
    bool radio_on;
    error_type_t ret_code;

    ret_code = wifi_intf_get_radio_on_status(&radio_on);

    if (ret_code == OK && radio_on)
    {
        sta_list = MEM_MALLOC(sizeof(uap_mlan_ds_sta_list));
        ASSERT(sta_list != NULL);

        if (wifi_intf_get_var_station_list(WIFI_IFACE_ID_UAP, sta_list) < 0)
        {
            ret_val = 0;
        }
        else
        {
            ret_val = sta_list->sta_count;
        }
    
        MEM_FREE_AND_NULL(sta_list);
    }

    return ret_val;
}

/** Notify function */
void wifi_intf_uap_notify_event(wifi_intf_link_status_t intf_link_status,
                                       bool enabled, 
                                       char *device_name,
                                       unsigned int device_name_length, 
                                       unsigned char *mac_addr,
                                       unsigned char *ip_addr)
{
    wifi_intf_uap_event()->intf_link_status = intf_link_status;
    wifi_intf_uap_event()->enabled = enabled;
    memset(wifi_intf_uap_event()->device_name, 0x0, DEVICE_NAME_LEN);
    memset(wifi_intf_uap_event()->mac_addr, 0x0, ETH_ALEN);
    memset(wifi_intf_uap_event()->ip_addr, 0x0, IPV4_ALEN);
    wifi_intf_uap_event()->device_name_length = 0;

    if (device_name && (device_name_length > 0)) {
		if (device_name_length > DEVICE_NAME_LEN)
             device_name_length = DEVICE_NAME_LEN;

        memcpy(wifi_intf_uap_event()->device_name, device_name, device_name_length);
        wifi_intf_uap_event()->device_name_length = device_name_length;
    }

    if (mac_addr) {
        memcpy(wifi_intf_uap_event()->mac_addr, mac_addr, ETH_ALEN);
    }

    if (ip_addr) {
        memcpy(wifi_intf_uap_event()->ip_addr, ip_addr, IPV4_ALEN);
    }

    NOTIFY_OBSERVERS(wifi_intf_uap_event(), observers);
}

/** Public attach */
void wifi_intf_uap_attach_event_observer( Observer_t *o)
{
    ATTACH_OBSERVER(wifi_intf_uap_event()->observers, o);
}

/** Public detach */
void wifi_intf_uap_detach_event_observer( Observer_t *o)
{
    DETACH_OBSERVER(wifi_intf_uap_event()->observers, o);
}

/** Public get */
wifi_intf_link_status_t wifi_intf_uap_get_link_status( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_uap_event_t, subject_observer_oid_key, WIFI_INTF_UAP_EVENT_ID));
    return(wifi_intf_uap_event()->intf_link_status);
}

/** Public get */
error_type_t wifi_intf_uap_get_device_name( Subject_t *s, char * device_name, unsigned int *device_name_len)
{
    error_type_t retval = FAIL;
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_uap_event_t, subject_observer_oid_key, WIFI_INTF_UAP_EVENT_ID));
    if (device_name && device_name_len && (*device_name_len > 0))
    {
        memcpy(device_name, wifi_intf_uap_event()->device_name, MIN(*device_name_len, DEVICE_NAME_LEN));
        *device_name_len = wifi_intf_uap_event()->device_name_length;
	retval = OK;
    }

    return(retval);
}

/** Public get */
error_type_t wifi_intf_uap_get_mac_addr( Subject_t *s, unsigned char * mac_addr)
{
    error_type_t retval = FAIL;
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_uap_event_t, subject_observer_oid_key, WIFI_INTF_UAP_EVENT_ID));
    if (mac_addr)
    {
        memcpy(mac_addr, wifi_intf_uap_event()->mac_addr, ETH_ALEN);
	retval = OK;
    }

    return(retval);
}

/** Public get */
error_type_t wifi_intf_uap_get_ip_addr( Subject_t *s, unsigned char * ip_addr)
{
    error_type_t retval = FAIL;
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_uap_event_t, subject_observer_oid_key, WIFI_INTF_UAP_EVENT_ID));
    if (ip_addr)
    {
        memcpy(ip_addr, wifi_intf_uap_event()->ip_addr, IPV4_ALEN);
	retval = OK;
    }

    return(retval);
}


/********************************************* 
 *  WIFI STA Events
 *********************************************/

#define WIFI_INTF_STA_EVENT_ID 0x18228437 

typedef struct wifi_intf_sta_event_s
{
    // Common values added to all subjects
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;

	// STA Link Events
    wifi_event_t latest_event;
    int latest_event_details;
} wifi_intf_sta_event_t;

/** Singleton Constructor, initializes, doubles as get struct */
static wifi_intf_sta_event_t *wifi_intf_sta_event()
{
    static wifi_intf_sta_event_t *ptr = 0;
    if ( 0 == ptr)
    {
        // Construct a copy of the subject and populate it with initial values
        ptr = (wifi_intf_sta_event_t *)MEM_MALLOC(sizeof(wifi_intf_sta_event_t));

        // Initialize the general subject fields
        ptr->subject_observer_oid_key = WIFI_INTF_STA_EVENT_ID;
        MInitList(&(ptr->observers));

        // Initialize WIFI specific values
        ptr->latest_event = WIFI_LINK_DOWN_EVENT;
        ptr->latest_event_details = 0;
    }
    return ptr;
}

/** Notify function */
void wifi_intf_sta_notify_event( wifi_event_t event, int details )
{   
    // Update the subject
    wifi_intf_sta_event()->latest_event = event;
    wifi_intf_sta_event()->latest_event_details = details;

    // Notify our observers
    NOTIFY_OBSERVERS(wifi_intf_sta_event(), observers);
}

void wifi_intf_sta_attach_event_observer(Observer_t *o)
{
    ATTACH_OBSERVER(wifi_intf_sta_event()->observers, o );
}

void wifi_intf_sta_detach_event_observer(Observer_t *o)
{
    DETACH_OBSERVER(wifi_intf_sta_event()->observers, o );
}

wifi_event_t wifi_intf_sta_get_latest_event_type(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_sta_event_t, subject_observer_oid_key, WIFI_INTF_STA_EVENT_ID));

    return wifi_intf_sta_event()->latest_event;
}

int wifi_intf_sta_get_latest_event_details(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, wifi_intf_sta_event_t, subject_observer_oid_key, WIFI_INTF_STA_EVENT_ID ));

    return wifi_intf_sta_event()->latest_event_details;
}



/******************************************************************************
 * Function Name: wifi_intf_get_tkip_workaround_state
 *
 * Description: Get TKIP workaround state
 *
 ******************************************************************************/
wlan_tkip_workaround_t wifi_intf_get_tkip_workaround_state(void)
{
    return gTKIPWorkaroundInfo;
}

/******************************************************************************
 * Function Name: wifi_intf_set_tkip_workaround_state
 *
 * Description: Set TKIP workaround state
 *
 ******************************************************************************/
void wifi_intf_set_tkip_workaround_state(wlan_tkip_workaround_t tkip_info)
{
    gTKIPWorkaroundInfo = tkip_info;
}


/******************************************************************************
 * Function Name: wifi_intf_sta_set_link_status
 *
 * Description: Set the link status
 *
 ******************************************************************************/
void wifi_intf_sta_set_link_status(wifi_intf_link_event_t link_event)
{
    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("\n**  wifi_intf_set_link_status: link_event 0x%X connected 0x%X\n",
            link_event, WlanIsConnected()));

    if (link_event == WIFI_INTF_LINK_EVENT_UP)
    {
        if (!gWirelessNetdrvrEnabled)
        {
            // Do not indicate link up when disabled
            link_event = WIFI_INTF_LINK_EVENT_DOWN;
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("**  wifi_intf_set_link_status 1: WIFI_INTF_LINK_EVENT_DOWN\n\n"));
        }
        else if (wifi_intf_get_var_is_wps_running() != FALSE)
        {
            // do not indicate link up until WPS is complete
            link_event = WIFI_INTF_LINK_EVENT_DOWN;
            DPRINTF((DBG_SOFT | DBG_OUTPUT), ("**  wifi_intf_set_link_status 2: WIFI_INTF_LINK_EVENT_DOWN\n\n"));
        }        
    }

    // Link Status Callback
    if (g_wifi_intf_callback_funcs.set_link_status) 
    {
        g_wifi_intf_callback_funcs.set_link_status(link_event);
    }
}

void wifi_intf_free_treck_buffer(char *pkt_buff, UINT32 pkt_len, void *pkt_desc)
{
    tfFreeDriverBuffer((ttPacketPtr)pkt_desc);
}

/******************************************************************************
 * Function Name: wifi_intf_sta_link_status
 *
 * Description: Notify observers of STA link status events
 *
 ******************************************************************************/
static void wifi_intf_sta_link_status(wifi_event_t link_status)
{
    if (!gWirelessNetdrvrEnabled)
    {
        // If driver not enabled notify of link down events only
        if (link_status == WIFI_LINK_DOWN_EVENT)
        {
            wifi_intf_sta_notify_event( link_status, 0);
        }
    }
    else
    {
        wifi_intf_sta_notify_event( link_status, 0);
    }
}


/******************************************************************************
 * Function Name: wifi_intf_sta_get_current_link_status
 *
 * Description: Get wireless STA interface current link status
 *
 ******************************************************************************/
uint32_t wifi_intf_sta_get_current_link_status(void)
{
        return g_current_sta_link_status;
}

/******************************************************************************
 * Function Name: wifi_intf_uap_get_current_link_status
 *
 * Description: Get wireless uAP interface current link status
 *
 ******************************************************************************/
uint32_t wifi_intf_uap_get_current_link_status(void)
{
        return g_current_uap_link_status;
}

/******************************************************************************
 * Function Name: wifi_intf_sta_get_state
 *
 * Description: Get wireless interface state
 *
 ******************************************************************************/
wlan_type_t wifi_intf_sta_get_state(void)
{
    return g_current_wlan_sta_state;
}


/******************************************************************************
 * Function Name: wifi_intf_sta_set_state
 *
 * Description: Set wireless interface state
 *
 ******************************************************************************/
void wifi_intf_sta_set_state(wlan_type_t state)
{
    uint32_t cp_link_status;
    wifi_event_t event_link_status;

    g_current_wlan_sta_state = state;

    switch(state)
    {
    case WLAN_INACTIVE:
        cp_link_status = Link_Down;
		event_link_status = WIFI_LINK_DOWN_EVENT;
        break;
    case WLAN_WPS_RUNNING:
    case WLAN_ASSOCIATING:
    case WLAN_ASSOCIATED:
        cp_link_status = Link_Connecting;
		event_link_status = WIFI_LINK_CONNECTING_EVENT;
        break;
    case WLAN_ADHOC:
    case WLAN_CONNECTED:
        cp_link_status = Link_Connected;
		event_link_status = WIFI_LINK_CONNECTED_EVENT;
        break;
    case WLAN_BAD_KEY_ERROR:
    case WLAN_WPS_ERROR:
        cp_link_status = Link_Error;
		event_link_status = WIFI_LINK_ERROR_EVENT;
        break;
    default:
        cp_link_status = Link_Down;
		event_link_status = WIFI_LINK_DOWN_EVENT;
        break;
    }

    if (g_current_sta_link_status == cp_link_status)
        return;
        
	g_current_sta_link_status = cp_link_status;

    wifi_intf_sta_link_status(event_link_status);
}


/******************************************************************************
 * Function Name: wifi_intf_uap_set_state
 *
 * Description: Set wireless interface uap state
 *
 ******************************************************************************/
void wifi_intf_uap_set_state(wlan_type_t state, 
                             char *device_name, 
                             unsigned int device_name_length, 
                             unsigned char *mac_addr, 
                             unsigned char *ip_addr)
{
    uint32_t cp_link_status;
    bool uap_enabled;
    g_current_wlan_uap_state = state;

    switch(state)
    {
    case WLAN_WPS_RUNNING:
        cp_link_status = Link_Connecting;
        break;
    case WLAN_CONNECTED:
        cp_link_status = Link_Connected;
        break;
    case WLAN_WPS_REGISTRAR_DEVICE_CONNECTING:
		cp_link_status = Link_Device_Connecting;
        break;
    case WLAN_WPS_ERROR:
        cp_link_status = Link_Error;
        break;
    default:
        cp_link_status = Link_Down;
        break;
    }

    g_current_uap_link_status = cp_link_status;

    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP, &uap_enabled);

    wifi_intf_uap_notify_event( cp_link_status, uap_enabled, device_name, device_name_length, mac_addr, ip_addr);
}


/******************************************************************************
 * Function Name: wifi_intf_uap_device_connected
 *
 * Description: A device connected to the uap/go
 *
 ******************************************************************************/
void wifi_intf_uap_device_connected(char * device_name, unsigned int device_name_length, unsigned char * mac_addr, unsigned char * ip_addr)
{
    wifi_intf_uap_set_state(WLAN_CONNECTED, device_name, device_name_length, mac_addr, ip_addr);
}


/******************************************************************************
 * Function Name: wifi_intf_start_new_connection
 *
 * Description: Start a new connection using WPS
 *
 ******************************************************************************/
void wifi_intf_start_new_connection()   // TODO -- what about PIN mode?
{
    // Make sure wireless is active
    if (gWirelessNetdrvrEnabled)
    {
        wirelessStartWPSEnrollee(NULL, NULL);
    }
}


/******************************************************************************
 * Function Name: wifi_intf_start_existing_connection
 *
 * Description: Start an existing wireless connection
 *
 ******************************************************************************/
void wifi_intf_start_existing_connection()
{
    // Make sure wireless is active
    if (gWirelessNetdrvrEnabled)
    {
        WlanWirelessHWEnable();
    }
}


/******************************************************************************
 * Function Name: wifi_intf_turn_off_radio
 *
 * Description: Turn off radio
 *
 ******************************************************************************/
void wifi_intf_turn_off_radio()
{
    if (wifi_intf_get_var_is_wps_running() == FALSE)
    {
        if (WlanIsWirelessHWActive())
        {
            wifi_intf_sta_set_state(WLAN_INACTIVE);
            wifi_intf_sta_set_link_status(WIFI_INTF_LINK_EVENT_DOWN);
			wifi_intf_uap_set_state(WLAN_INACTIVE, NULL, 0, NULL, NULL); 
            WlanWirelessHWDisable();
        }
    }
    else
    {
        // WPS certification test 5.1.7 
        // 2 minute timeout with multiple push button events for PBC Config method
        // 
        // Test Goal:  The test verifies that the STAUT exercises the push 
        // button 2 minutes timer correctly. As long as the period between 
        // when the user pushes the WPS button on the AP and when the user 
        // pushes the WPS button on the STAUT is less than 2 minutes, the 
        // STAUT must be able to join the APs WLAN using PBC method through 
        // the APs internal Registrar.
        // 
        // Restart WPS to reset the PBC timer
        wirelessStartWPSEnrollee(NULL, NULL);
    }
}


/******************************************************************************
 * Function Name: wifi_intf_xmit_timer_callback
 *
 * Description: Wireless interface transmit timer callback
 *
 ******************************************************************************/
static void wifi_intf_xmit_timer_callback (ULONG param)
{
    BOOL packets_to_send;

    // Stop timer
    tx_timer_deactivate(&wireless_drvr_xmit_timer);

    // Check for packets to send
    tx_mutex_get(&gTxQueueMutex, TX_WAIT_FOREVER);
    packets_to_send = packet_q_have_more(&gWirelessTXQ);
    tx_mutex_put(&gTxQueueMutex);

    if ((packets_to_send) || WirelessLowerLayerHasPacketOutstanding(STA))
    {
        // Try Xmit again
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_STA_XMIT_FLAG, TX_OR);
    }

    tx_mutex_get(&gUAPTxQueueMutex, TX_WAIT_FOREVER);
    packets_to_send = packet_q_have_more(&gUAPWirelessTXQ);
    tx_mutex_put(&gUAPTxQueueMutex);

    if ((packets_to_send) || WirelessLowerLayerHasPacketOutstanding(UAP))
    {
        // Try Xmit again
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_UAP_XMIT_FLAG, TX_OR);
    }

}


/******************************************************************************
 * Function Name: wifi_intf_xmit_thread
 *
 * Description: Wireless interface transmit thread
 *
 ******************************************************************************/
static void wifi_intf_xmit_thread(int param)
{
    uint32_t flags;

    while(true)
    {
        if(TX_SUCCESS == tx_event_flags_get(&wireless_drvr_xmit_flags, 
                                            WIRELESS_XMIT_FLAG_MASK, 
                                            TX_OR_CLEAR, 
                                            &flags, 
                                            TX_WAIT_FOREVER))
        {
            if (flags & WIRELESS_STA_XMIT_FLAG)
            {
                if (!wifi_intf_start_xmit(TRUE))
                {
                    WlanMainProcess();
                    tx_timer_deactivate(&wireless_drvr_xmit_timer);
                    tx_timer_change(&wireless_drvr_xmit_timer, WIRELESS_DRVR_XMIT_CALLBACK_TIME, 0);
                    tx_timer_activate (&wireless_drvr_xmit_timer);
                }
            }

            if (flags & WIRELESS_UAP_XMIT_FLAG)
            {
                if (!wifi_uap_intf_start_xmit(TRUE))
                {
                    WlanMainProcess();
                    tx_timer_deactivate(&wireless_drvr_xmit_timer);
                    tx_timer_change(&wireless_drvr_xmit_timer, WIRELESS_DRVR_XMIT_CALLBACK_TIME, 0);
                    tx_timer_activate (&wireless_drvr_xmit_timer);
                }
            }
        }

        // TBD add exit for thread delete
    }
}


/******************************************************************************
 * Function Name: wifi_intf_init
 *
 * Description: Initialize wireless interface
 *
 ******************************************************************************/
error_type_t wifi_intf_init(const wifi_intf_callback_funcs_t * wifi_intf_callback_funcs)
{
    g_wifi_intf_callback_funcs = *wifi_intf_callback_funcs;

    wifi_intf_var_init();

    gTKIPWorkaroundInfo = NO_TKIP_WORKAROUND;

    WirelessInit();

//    raw_iface_handle_mlan0 = raw_socket_add_interface(WIFI_IFACE_NAME_STATION, 
//                                                      wifi_intf_raw_send);

    return OK;
}


/******************************************************************************
 * Function Name: wifi_intf_shutdown
 *
 * Description: Shutdown wireless interface
 *
 ******************************************************************************/
void wifi_intf_shutdown(void)
{
    if(WlanIsWirelessHWActive())
    {
        if (wifi_intf_get_var_is_wps_running() != FALSE)
        {
            wirelessCancelWPS();
    
            while (wifi_intf_get_var_is_wps_running() != FALSE)
            {
                tx_thread_sleep(10);
            }
        }
        tx_thread_sleep(100);
#ifdef HAVE_POWER_MGR
        gWirelessPwrOffAllow = TRUE;
#endif
        WlanShutdown();
        // The shutdown routine returns before all the threads have a chance
        // to completely terminate.  Usually, that's what we want (so that we
        // don't hang the control panel, serial port, etc. when they call that
        // routine).  In this case, though, we want to make sure we exit
        // all threads before returning.  Wait to let them finish.
        tx_thread_sleep(300);
    }
}


/******************************************************************************
 * Function Name: wifi_intf_shutdown_notify
 *
 * Description: Wireless interface shutdown notification
 *
 ******************************************************************************/
void wifi_intf_shutdown_notify(void)
{
    uint32_t status;
    //status = tx_mutex_get(&gWNetDevLock, TX_WAIT_FOREVER);
    //XASSERT(status == TX_SUCCESS, status);

    DPRINTF((DBG_SOFT | DBG_OUTPUT), ("wifi_intf_shutdown_notify:\n"));
    wifi_intf_sta_set_link_status(WIFI_INTF_LINK_EVENT_DOWN);

    status = tx_mutex_get(&gWNetDevLock, TX_WAIT_FOREVER);
    XASSERT(status == TX_SUCCESS, status);
    //netDev = NULL;
    // BUGBUG: what else needs to happen here?  Purge TX Queues?
    // Test results so far show that nothing else needs to happen here.
    // The purge in the close call appears to be enough.  Continue to monitor.

    tx_mutex_put(&gWNetDevLock);
}


/******************************************************************************
 * Function Name: wifi_intf_get_rx_buffer
 *
 * Description: Get a receive buffer.  
 *
 ******************************************************************************/
void wifi_intf_get_rx_buffer(char** rxBuff, int* buffLen, void** buffDesc)
{
    if (g_wifi_intf_callback_funcs.get_rx_buffer) 
    {
        // Callback
        g_wifi_intf_callback_funcs.get_rx_buffer(rxBuff, buffLen, buffDesc);
    }
    else
    {
        // No callback, No buffer
        *rxBuff = NULL;
        *buffLen = 0;
    }
}

/******************************************************************************
 * Function Name: wifi_intf_rx_packet_recvd
 *
 * Description: Recieve packet has been received
 *
 ******************************************************************************/
void wifi_intf_rx_packet_recvd(char* pktBuff, uint32_t pktLen, void* buffDesc)
{
    //put packet into the rx queue and let treck know we
    //got it
    UINT32 res;
    int RetCode;

    //if this is a raw packet for an open raw socket put it in its queue
    //if(wifi_intf_is_packet_raw(pktBuff, pktLen, &handle) && WlanIsConnected())
    if(packet_is_raw(net_iface_raw, pktBuff, pktLen))
    {
        //is a raw packet, through it in its queue and notify the socket
        //the socket then reads it using the wirelessrawrecv function in its own
        //thread
        DPRINTF(WDSOFT, ("Rx Raw Wireless Packet @ %#x\n", pktBuff));

        if (raw_handle_new_recv_packet(net_iface_raw, pktBuff, pktLen, buffDesc, wifi_intf_free_treck_buffer) != RAW_ENOERROR)
        {
            tfFreeDriverBuffer((ttPacketPtr) buffDesc );
        }
    }
    else
    {
        if ((gWirelessIsOpen == TRUE) && WlanIsConnected())
        {
            tx_mutex_get(&gRxQueueMutex, TX_WAIT_FOREVER);
            res = packet_q_add_pkt(&gWirelessRXQ, pktLen, pktBuff, buffDesc, NULL);
            tx_mutex_put(&gRxQueueMutex);
    
            if (res == PACKETQ_OK)
            {
                //tell stack we got a packet, it will then call our receive packet to read it
                // Callback

                if (g_wifi_intf_callback_funcs.recv_buffer)
                {
                    RetCode = g_wifi_intf_callback_funcs.recv_buffer();
                }
                else
                {
                    RetCode = FAIL;
                }
                if(RetCode != OK)
                {
                    // interface isn't open yet. Treck didn't take the packet. Just dump it.
                    DPRINTF(WDSOFT, ("Wireless dumping packet because interface isn't open\n"));
                    tx_mutex_get(&gRxQueueMutex, TX_WAIT_FOREVER);
                    packet_q_remove_pkt(&gWirelessRXQ);
                    if (g_wifi_intf_callback_funcs.free_buffer)
                    {
                        // Callback
                        g_wifi_intf_callback_funcs.free_buffer(buffDesc);
                    }
                    tx_mutex_put(&gRxQueueMutex);
                }
            }
            else
            {
//                DPRINTF(WDERR, ("wlan rx q full\n"));
                if (g_wifi_intf_callback_funcs.free_buffer)
                {
                    // Callback
                    g_wifi_intf_callback_funcs.free_buffer(buffDesc);
                }
            }
        }
        else
        {
            // wireless isn't open. We shouldn't be receiving a packet. Dump it. 
            DPRINTF(WDSOFT, ("Wireless dumping packet because interface isn't open (or isn't connected)\n"));
            if (g_wifi_intf_callback_funcs.free_buffer)
            {
                // Callback
                g_wifi_intf_callback_funcs.free_buffer(buffDesc);
            }
        }
    }
}

/******************************************************************************
 * Function Name: wifi_intf_complete_tx_packet
 *
 * Description: Transmit packet has been completed.
 *
 ******************************************************************************/
static void wifi_intf_complete_tx_packet(char* pktBuff, uint32_t pktLen, void * handle, BOOL appCall)
{
    // check to see if this is one of the packets sent through the raw path
    // if it is a raw packet it was allocated using MEM_MALLOC
    if(is_valid_raw_iface_handle((RAW_IFACE_HANDLE)handle))
    {
        //DPRINTF(WDSOFT, ("wifi_intf_complete_tx_packet raw pkt @ %#x, freeing buffer\n", pktBuff));
        MEM_FREE_AND_NULL(pktBuff);
    }
    else
    {
        if (g_wifi_intf_callback_funcs.complete_tx_packet)
        {
            // Callback
            g_wifi_intf_callback_funcs.complete_tx_packet(handle, appCall);
        }
    }
}


/******************************************************************************
 * Function Name: wifi_intf_tx_packet_sent
 *
 * Description: Transmit packet has been sent
 *
 ******************************************************************************/
//called by underlying layer when pkt has been sent
void wifi_intf_tx_packet_sent(char* pktBuff, uint32_t pktLen, void * handle)
{
    wifi_intf_complete_tx_packet(pktBuff, pktLen, handle, TRUE);

    // start the next send if any -jrs
    if (handle == uAP_treck_interface || ((RAW_IFACE_HANDLE)handle == raw_iface_handle_uap0))
    {
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_UAP_XMIT_FLAG, TX_OR);
    }
    else
    {
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_STA_XMIT_FLAG, TX_OR);
    }
}

/******************************************************************************
 * Function Name: wifi_intf_open
 *
 * Description: Open wireless interface
 *
 ******************************************************************************/
error_type_t wifi_intf_open(wifi_handle_t *handle)
{
    DPRINTF(WDENTRY, ("==>wifi_intf_open\n"));

    gWirelessIsOpen = TRUE;
    DPRINTF(WDENTRY, ("<==wifi_intf_open\n"));
    return OK; 
}

/******************************************************************************
 * Function Name: wifi_intf_startup
 *
 * Description: Wireless interface startup
 *
 ******************************************************************************/
void wifi_intf_startup(void)
{
    uint32_t status;
    net_device* netDev;

    status = tx_mutex_get(&gWNetDevLock, TX_WAIT_FOREVER);
    XASSERT(status == TX_SUCCESS, status);

    netDev = WlanGetNetDev(STA);

    if(netDev == NULL)
    {
        tx_mutex_put(&gWNetDevLock);
        DPRINTF(WDENTRY, ("<==wifi_intf_open FAILED\n"));
        //return TM_DEV_ERROR;
        return;
    }

    netDev->open(netDev);           //-jrs

    tx_mutex_put(&gWNetDevLock);
}


/******************************************************************************
 * Function Name: wifi_intf_close
 *
 * Description: Close wireless interface and clear any remaining transmit packets.
 *
 ******************************************************************************/
error_type_t wifi_intf_close(wifi_handle_t * handle)
{
    uint32_t pktLen;
    char* pktBuff;
    void* pktDesc;
    net_device* netDev;

    DPRINTF(WDENTRY, ("==>wifi_intf_close\n"));

    // flush the TX queue
    tx_mutex_get(&gTxQueueMutex, TX_WAIT_FOREVER);
    while(PACKETQ_OK == packet_q_get_pkt(&gWirelessTXQ, &pktLen, &pktBuff, &pktDesc, NULL))
    {
        packet_q_remove_pkt(&gWirelessTXQ);
        wifi_intf_complete_tx_packet(pktBuff, pktLen, pktDesc, FALSE);
    }
    tx_mutex_put(&gTxQueueMutex);

    // Get rid of any outstanding packets that have already been handed to the module
    netDev = WlanGetNetDev(STA);
    if (netDev != NULL)
    {
        WlanWmmCleanupQueues(netDev);
    }

    gWirelessIsOpen = FALSE;

    //gNetCloseDone = 1;
    DPRINTF(WDENTRY, ("<==wifi_intf_close (NetClose)\n"));

    return(OK);
}


/******************************************************************************
 * Function Name: wifi_intf_wait_for_link_down_ack
 *
 * Description: Wait for link down acknowledgement 
 *
 ******************************************************************************/
void wifi_intf_wait_for_link_down_ack(uint32_t delay)
{
    tx_mutex_get(&gLinkDownAckMutex, delay);
}


/******************************************************************************
 * Function Name: wifi_intf_ioctl
 *
 * Description: Wireless interface IOCTLs
 *
 ******************************************************************************/
error_type_t wifi_intf_ioctl(wifi_handle_t * handle, int flag, void *optionPtr, int optionLen)
{
    uint32_t status;
    net_device* netDev;

    // WiFi Station
    if(flag & WIFI_INTF_IOCTL_LINK_ENABLE)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("wifi_intf_ioctl: WIFI_INTF_IOCTL_LINK_ENABLE\n"));
        gWirelessNetdrvrEnabled = true;
        wifi_sta_set_disable_by_wired_link(FALSE);
    }

    if(flag & WIFI_INTF_IOCTL_LINK_DISABLE)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("wifi_intf_ioctl: WIFI_INTF_IOCTL_LINK_DISABLE\n"));

        gWirelessNetdrvrEnabled = false;

        // Do not shutdown if WPS is active
        if (wifi_intf_get_var_is_wps_running() == FALSE)
        {
            wifi_sta_set_disable_by_wired_link(true);
        }
        else
        {
            DPRINTF(WDSOFT, ("WPS active - do not shutdown\n"));
        }
    }

    if(flag & WIFI_INTF_IOCTL_LINK_DOWN_ACK)
    {
        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("wifi_intf_ioctl: WIFI_INTF_IOCTL_LINK_DOWN_ACK\n"));
        tx_mutex_put(&gLinkDownAckMutex);
    }

    if(flag & WIFI_INTF_IOCTL_SET_MCAST_LIST)
    {
        unsigned char *List[MAX_MC_ADDRS], *Ptr;
        int i;

        DPRINTF((DBG_SOFT | DBG_OUTPUT), ("wifi_intf_ioctl: WIFI_INTF_IOCTL_SET_MCAST_LIST\n"));

        status = tx_mutex_get(&gWNetDevLock, TX_WAIT_FOREVER);
        XASSERT(status == TX_SUCCESS, status);

        netDev = WlanGetNetDev(STA);
        if(netDev == NULL)
        {
            tx_mutex_put(&gWNetDevLock);
            return TM_DEV_ERROR;
        }

        Ptr = optionPtr;
        for( i = 0; (i < optionLen) && (i < MAX_MC_ADDRS); i++)
        {
            List[i] = Ptr;
            Ptr += 6;
        }

        tx_mutex_put(&gWNetDevLock);

       //hash each of the ethernet addresses and set the bit in the hash table
       WlanSetMulticast(optionLen, List, STA);    
    }

    if(flag & WIFI_INTF_IOCTL_IP_TRAFFIC_DETECTED)
    {
        //if (g_current_wlan_sta_state == WLAN_ASSOCIATED)
        {
            DPRINTF((DBG_LOUD | DBG_OUTPUT), ("wifi_intf_ioctl: WIFI_INTF_IOCTL_IP_TRAFFIC_DETECTED\n"));
            wifi_intf_sta_set_state(WLAN_CONNECTED);
        }
    }

    if(flag & WIFI_INTF_IOCTL_SET_AUTO_ARP)
    {
        WlanARPFilter(optionPtr, optionLen);
    }

    return OK;
}

/******************************************************************************
 * Function Name: wifi_intf_get_phys_addr
 *
 * Description: Get the MAC address being used by the wireless driver
 *
 ******************************************************************************/
error_type_t wifi_intf_get_phys_addr(wifi_handle_t * handle,  char * physicalAddress)
{
    if ((ttUserInterface)handle == uAP_treck_interface)
    {
        wifi_intf_get_var_mac_addr(UAP, (uint8_t*)physicalAddress, 6);
    }
    else
    {
        wifi_intf_get_var_mac_addr(STA, (uint8_t*)physicalAddress, 6);
    }
    DPRINTF(WDENTRY, ("<==wifi_intf_get_phys_addr = %02x-%02x-%02x-%02x-%02x-%02x\n", physicalAddress[0], physicalAddress[1], physicalAddress[2], physicalAddress[3], physicalAddress[4], physicalAddress[5]));
    return(OK);
}


/******************************************************************************
 * Function Name: wifi_intf_send
 *
 * Description: Send wireless data
 *
 ******************************************************************************/
error_type_t wifi_intf_send(wifi_handle_t * handle, char *dataPtr, int dataLength)
{
    //add the packet to the queue if possible
    UINT32 QRes;
    int sendRes = OK;
    uint32_t status;
    net_device* netDev;

    status = tx_mutex_get(&gWNetDevLock, TX_WAIT_FOREVER);
    XASSERT(status == TX_SUCCESS, status);

    if (((ttUserInterface)handle == uAP_treck_interface) || ((RAW_IFACE_HANDLE)handle == raw_iface_handle_uap0))
    {
        netDev = WlanGetNetDev(UAP);
    }
    else
    {
        netDev = WlanGetNetDev(STA);
    }

    if(netDev == NULL)
    {
        tx_mutex_put(&gWNetDevLock);
        return FAIL;
    }
    tx_mutex_put(&gWNetDevLock);

    //add pkt to the send queue


    if ((ttUserInterface)handle == uAP_treck_interface || ((RAW_IFACE_HANDLE)handle == raw_iface_handle_uap0))
    {
        tx_mutex_get(&gUAPTxQueueMutex, TX_WAIT_FOREVER);
        QRes = packet_q_add_pkt(&gUAPWirelessTXQ, dataLength, dataPtr, (void *)handle, NULL);
        tx_mutex_put(&gUAPTxQueueMutex);
    }
    else
    {
        tx_mutex_get(&gTxQueueMutex, TX_WAIT_FOREVER);
        QRes = packet_q_add_pkt(&gWirelessTXQ, dataLength, dataPtr, (void *)handle, NULL);
        tx_mutex_put(&gTxQueueMutex);
    }

    if(QRes == PACKETQ_OK)
    {
        sendRes = TM_DEV_OKAY;
    }
    else
    {
        //queue is full (out of memory), can't add packet, need to flush any pkts that were part of the
        //frame, the stack takes care of recovering on this.
//        DPRINTF(WDERR, ("wifi_intf_send: %s tx Q full\n", netDev->name));
        sendRes = TM_DEV_ERROR;
    }

    // Whether we were able to queue to packet or not, we should try to start
    // sending more data.  The routine will not actually hand a packet to the
    // lower layer if it is busy.
    if ((ttUserInterface)handle == uAP_treck_interface || ((RAW_IFACE_HANDLE)handle == raw_iface_handle_uap0))
    {
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_UAP_XMIT_FLAG, TX_OR);
    }
    else
    {
        tx_event_flags_set(&wireless_drvr_xmit_flags, WIRELESS_STA_XMIT_FLAG, TX_OR);
    }

    return sendRes;
}

/******************************************************************************
 * Function Name: wifi_intf_start_xmit
 *
 * Description: Start a wireless transmit
 *
 ******************************************************************************/
static BOOL wifi_intf_start_xmit(BOOL fTxComplete)
{
    UINT32 resQ;
    uint32_t pktLen;
    BOOL pktSent = FALSE;
    int xmitFailed;
    char* pktBuff;
    void* pktDesc;
    static sk_buff skb;
    net_device* netDev;

    if(!WirelessLowerLayerHasPacketOutstanding(STA))
    {
        tx_mutex_get(&gTxQueueMutex, TX_WAIT_FOREVER);
        resQ = packet_q_get_pkt(&gWirelessTXQ, &pktLen, &pktBuff, &pktDesc, NULL);      
        if(resQ == PACKETQ_OK)
        {
            packet_q_remove_pkt(&gWirelessTXQ);
            tx_mutex_put(&gTxQueueMutex);

            netDev = WlanGetNetDev(STA);
            if(netDev != NULL)
            {
                skb.data = (unsigned char *)pktBuff;
                skb.len = pktLen;
                skb.handle = pktDesc;
                xmitFailed = netDev->hard_start_xmit(&skb, netDev);
            }
            else
            {
                xmitFailed = 1;
            }

            if(xmitFailed)
            {
                DPRINTF(WDERR, ("wifi_intf_start_xmit wlan layer busy.  Dropping pkt @ %#x\n", pktBuff));
                wifi_intf_complete_tx_packet(pktBuff, pktLen, pktDesc, TRUE);
            }
            else
            {
                pktSent = TRUE;
            }
        }
        else
        {
            tx_mutex_put(&gTxQueueMutex);
        }
    }

    return pktSent;
}



/******************************************************************************
 * Function Name: wifi_intf_recv
 *
 * Description: Receive a wireless receive data
 *
 ******************************************************************************/
error_type_t wifi_intf_recv(wifi_handle_t * handle, char** dataPtr, uint32_t * dataLength, void** bufHandlePtr)
{
    UINT32 res;
    int errorCode = TM_DEV_OKAY;

    //get information from the next rx packet
    if ((ttUserInterface)handle == uAP_treck_interface)
    {
        tx_mutex_get(&gUAPRxQueueMutex, TX_WAIT_FOREVER);
        res = packet_q_get_pkt(&gUAPWirelessRXQ, (UINT32*)dataLength, dataPtr, (void**)bufHandlePtr, NULL);

        ASSERT(res == PACKETQ_OK);

        packet_q_remove_pkt(&gUAPWirelessRXQ);
        tx_mutex_put(&gUAPRxQueueMutex);
    }
    else
    {
        tx_mutex_get(&gRxQueueMutex, TX_WAIT_FOREVER);
        res = packet_q_get_pkt(&gWirelessRXQ, (UINT32*)dataLength, dataPtr, (void**)bufHandlePtr, NULL);

        ASSERT(res == PACKETQ_OK);

        packet_q_remove_pkt(&gWirelessRXQ);
        tx_mutex_put(&gRxQueueMutex);
    }

    return errorCode;
}


//called by underlying layer to give us a received pkt
void wifi_uap_intf_rx_packet_recvd(char* pktBuff, UINT32 pktLen, void* buffDesc)
{
    //put packet into the rx queue and let treck know we
    //got it
    UINT32 res;
    int RetCode;
//    RAWHANDLE handle;

//    agprintf("WRXInd: %d, len %d\n", gMACInfo.stats.rxPktCount, pktLen);

    if (gUAPWirelessIsOpen == TRUE)
    {
        if(packet_is_raw(raw_iface_handle_uap0, pktBuff, pktLen))
        {
            //is a raw packet, through it in its queue and notify the socket
            //the socket then reads it using the wirelessrawrecv function in its own
            //thread
            DPRINTF(WDSOFT, ("Rx UAP Raw Wireless Packet @ %#x\n", pktBuff));

            if (raw_handle_new_recv_packet(raw_iface_handle_uap0, pktBuff, pktLen, buffDesc, wifi_intf_free_treck_buffer) != RAW_ENOERROR)
            {
                tfFreeDriverBuffer((ttPacketPtr) buffDesc );
            }
        }
        else
        {
            tx_mutex_get(&gUAPRxQueueMutex, TX_WAIT_FOREVER);
            res = packet_q_add_pkt(&gUAPWirelessRXQ, pktLen, pktBuff, buffDesc, NULL);
            tx_mutex_put(&gUAPRxQueueMutex);

            if (res == PACKETQ_OK)
            {
                //tell stack we got a packet, it will then call our receive packet to read it
                tfNotifyInterfaceTask(uAP_treck_interface, 1, 0, 0, 0);
            }
            else
            {
//                DPRINTF(WDERR, ("uap rx q full\n"));
                RetCode = tfFreeDriverBuffer((ttPacketPtr) buffDesc );
                if(RetCode != TM_ENOERROR)
                {
                    DPRINTF(WDERR, ("Wireless Packet Free uap Buff Error %d\n", RetCode));
                }
            }
        }
    }
    else
    {
        // wireless isn't open. We shouldn't be receiving a packet. Dump it. 
        DPRINTF(WDSOFT, ("Wireless dumping packet because uap interface isn't open (or isn't connected)\n"));
        RetCode = tfFreeDriverBuffer((ttPacketPtr) buffDesc );
        if(RetCode != TM_ENOERROR)
        {
            DPRINTF(WDERR, ("Wireless Packet Free Buff Error %d\n", RetCode));
        }
    }
}

/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 11/6/2003
 *
 *
 * Description:
 *
 ******************************************************************************/
error_type_t wifi_uap_intf_open(wifi_handle_t * handle)
{
    DPRINTF(WDENTRY, ("==>wifi_uap_intf_open\n"));

    uAP_treck_interface = (ttUserInterface)handle;
    gUAPWirelessIsOpen = TRUE;

    DPRINTF(WDENTRY, ("<==wifi_uap_intf_open\n"));
    return TM_DEV_OKAY; 
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 11/6/2003
 *
 *
 * Description:
 *
 ******************************************************************************/
error_type_t wifi_uap_intf_close(wifi_handle_t * handle)
{
    //UINT32 status;
    UINT32 pktLen;
    char* pktBuff;
    void* pktDesc;
    net_device* netDev;

    DPRINTF(WDENTRY, ("==>wifi_uap_intf_close\n"));

    // flush the TX queue
    tx_mutex_get(&gUAPTxQueueMutex, TX_WAIT_FOREVER);
    while(PACKETQ_OK == packet_q_get_pkt(&gUAPWirelessTXQ, &pktLen, &pktBuff, &pktDesc, NULL))
    {
        packet_q_remove_pkt(&gUAPWirelessTXQ);
        wifi_intf_complete_tx_packet(pktBuff, pktLen, pktDesc, FALSE);
    }
    tx_mutex_put(&gUAPTxQueueMutex);

    // Get rid of any outstanding packets that have already been handed to the module
    netDev = WlanGetNetDev(UAP);
    if (netDev != NULL)
    {
        WlanWmmCleanupQueues(netDev);
    }

    gUAPWirelessIsOpen = FALSE;

    DPRINTF(WDENTRY, ("<==wifi_uap_intf_close (NetClose)\n"));

    return(TM_DEV_OKAY);
}



/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 11/6/2003
 *
 *
 * Description:
 *
 ******************************************************************************/
error_type_t wifi_uap_intf_ioctl(wifi_handle_t * handle, int flag, 
                      void TM_FAR *optionPtr, int optionLen)
{
    UINT32 status = 0;

    if(flag & WIFI_INTF_IOCTL_LINK_ENABLE)
    {
    }

    if(flag & WIFI_INTF_IOCTL_LINK_ENABLE)
    {
    }

    if(flag & WIFI_INTF_IOCTL_LINK_ENABLE)
    {
    }

    if(flag & WIFI_INTF_IOCTL_LINK_ENABLE)
    {
    }

    if(flag & TM_DEV_SET_MCAST_LIST)
    {
        unsigned char *List[MAX_MC_ADDRS], *Ptr;
        int i;

        Ptr = optionPtr;
        for( i = 0; (i < optionLen) && (i < MAX_MC_ADDRS); i++)
        {
            List[i] = Ptr;
            Ptr += 6;
        }

        //hash each of the ethernet addresses and set the bit in the hash table
        WlanSetMulticast(optionLen, List, UAP);    
    }

    return status;
}

/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 10/30/2003
 *
 *
 * Description:
 *
 ******************************************************************************/
static BOOL wifi_uap_intf_start_xmit(BOOL fTxComplete)
{
    UINT32 resQ;
    UINT32 pktLen;
    BOOL pktSent = FALSE;
    int xmitFailed;
    char* pktBuff;
    void* pktDesc;
    static sk_buff skb;
    net_device* netDev;

    if(!WirelessLowerLayerHasPacketOutstanding(UAP))
    {
        tx_mutex_get(&gUAPTxQueueMutex, TX_WAIT_FOREVER);
        resQ = packet_q_get_pkt(&gUAPWirelessTXQ, &pktLen, &pktBuff, &pktDesc, NULL);      
        if(resQ == PACKETQ_OK)
        {
            packet_q_remove_pkt(&gUAPWirelessTXQ);
            tx_mutex_put(&gUAPTxQueueMutex);

            // send the packet to the wireless
            netDev = WlanGetNetDev(UAP);
            if(netDev != NULL)
            {
                skb.data = (unsigned char *)pktBuff;
                skb.len = pktLen;
                skb.handle = pktDesc;
                xmitFailed = netDev->hard_start_xmit(&skb, netDev);
            }
            else
            {
                xmitFailed = 1;
            }

            if(xmitFailed)
            {
                DPRINTF(WDERR, ("wifi_uap_intf_start_xmit wlan layer busy.  Dropping pkt @ %#x\n", pktBuff));
                wifi_intf_complete_tx_packet(pktBuff, pktLen, pktDesc, TRUE);
            }
            else
            {
                pktSent = TRUE;
            }
        }
        else
        {
            tx_mutex_put(&gUAPTxQueueMutex);
        }
    }

    return pktSent;
}

void wifi_uap_intf_init(void)
{
    DPRINTF(WDENTRY, ("==>wifi_uap_intf_init\n"));

// moved to dev open
//    uAP_treck_interface = treck_interface;

    raw_iface_handle_uap0 = raw_socket_add_interface(WIFI_IFACE_NAME_UAP, wifi_intf_raw_send);

    DPRINTF(WDENTRY, ("<==wifi_uap_intf_init\n"));
}



/******************************************************************************
 * Function Name: wifi_intf_raw_init
 *
 * Description: Initial RAW wireless connection
 *
 ******************************************************************************/
void wifi_intf_create_resources(void)
{
    // Station Resources
    tx_mutex_create(&gRxQueueMutex, "gRxQueueMutex", TX_INHERIT);
    tx_mutex_create(&gTxQueueMutex, "gTxQueueMutex", TX_INHERIT);

    tx_mutex_create(&gLinkDownAckMutex, "gLinkDownAckMutex", TX_INHERIT);

    // Micro-AP Resources
    tx_mutex_create(&gUAPRxQueueMutex, "gUAPRxQueueMutex", TX_INHERIT);
    tx_mutex_create(&gUAPTxQueueMutex, "gUAPTxQueueMutex", TX_INHERIT);

    // Shared Resources
    tx_mutex_create(&gWNetDevLock,       "gWNetDevLock", TX_INHERIT);
    tx_mutex_create(&gWPSStateMutex,     "gWPSStateMutex", TX_INHERIT);
    tx_mutex_create(&gWPSIsRunningMutex, "gWPSIsRunningMutex", TX_INHERIT);

    // Shared Resources for transferring packets
    tx_event_flags_create(&wireless_drvr_xmit_flags, "WirelessDrvrXmitFlags");
    tx_timer_create (&wireless_drvr_xmit_timer, "WirelessDrvrXmitTimer", 
                     wifi_intf_xmit_timer_callback, 
                     0, 
                     WIRELESS_DRVR_XMIT_CALLBACK_TIME, 
                     WIRELESS_DRVR_XMIT_CALLBACK_TIME, 
                     TX_NO_ACTIVATE);

    tx_thread_create( &wireless_drvr_xmit_thread,
                      "wifi_intf_xmit_thread",
                      (void(*)(ULONG))wifi_intf_xmit_thread,
                      (int) NULL, 
                      wireless_drvr_xmit_stack,
                      WIRELESS_DRVR_XMIT_STACK_SIZE, 
                      THR_PRI_NORMAL, 
                      THR_PRI_NORMAL,
                      1, 
                      TX_AUTO_START );
}

/******************************************************************************
 * Function Name: wifi_intf_raw_send
 *
 * Description: Send a RAW packet
 *
 ******************************************************************************/
int wifi_intf_raw_send(RAW_IFACE_HANDLE iface, char* buff, int length)
{
//    char* pEtherDataBuf;
    int status;

// rdj - buffer is now copied in raw_send() in raw_socket.c, but wifi still owns and must free when done
//    pEtherDataBuf = (char*)MEM_MALLOC(length);
//    if(pEtherDataBuf == NULL)
//    {
//        DPRINTF(WDERR, ("wifi_intf_sendRaw: FAILED TO GET ETHER BUFFER\n"));
//        return -1;
//    }
//
//    memcpy(pEtherDataBuf, buff, length);

    status = wifi_intf_send((wifi_handle_t *)iface, buff, length);
    if(status != TM_DEV_OKAY)
    {
        DPRINTF(WDERR, ("wifi_intf_sendRaw: TX Queue FULL, failed to send packet @ %#x; pkt freed\n", buff));
        MEM_FREE_AND_NULL(buff);
    }
    return length;
}

/******************************************************************************
 * Function Name: wifi_intf_get_var_wps_state
 *
 * Description: Get WPS state
 *
 ******************************************************************************/
wifi_intf_wps_outcome_t wifi_intf_get_var_wps_state()
{
    UINT threadx_rcode = tx_mutex_get(&gWPSStateMutex, TX_WAIT_FOREVER); 
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

    wifi_intf_wps_outcome_t ret = gWPSState;

    threadx_rcode = tx_mutex_put(&gWPSStateMutex);
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);
    
    return ret;
}

/******************************************************************************
 * Function Name: wifi_intf_set_var_wps_state
 *
 * Description: Set WPS state
 *
 ******************************************************************************/
void wifi_intf_set_var_wps_state(wifi_intf_wps_outcome_t state)
{
    UINT threadx_rcode = tx_mutex_get(&gWPSStateMutex, TX_WAIT_FOREVER); 
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

    gWPSState = state;

    threadx_rcode = tx_mutex_put(&gWPSStateMutex);
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);
}


/******************************************************************************
 * Function Name: wifi_intf_get_var_is_wps_running
 *
 * Description: Get WPS running status
 *
 ******************************************************************************/
bool wifi_intf_get_var_is_wps_running()
{
    UINT threadx_rcode = tx_mutex_get(&gWPSIsRunningMutex, TX_WAIT_FOREVER); 
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

    bool ret = gWPSIsRunning;

    threadx_rcode = tx_mutex_put(&gWPSIsRunningMutex);
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);
    
    return ret;
}

/******************************************************************************
 * Function Name: wifi_intf_set_var_is_wps_running
 *
 * Description: Set WPS running status
 *
 ******************************************************************************/
void wifi_intf_set_var_is_wps_running(bool running)
{
    UINT threadx_rcode = tx_mutex_get(&gWPSIsRunningMutex, TX_WAIT_FOREVER); 
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

    gWPSIsRunning = running;

    threadx_rcode = tx_mutex_put(&gWPSIsRunningMutex);
    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);
}

/******************************************************************************
 * Function Name: wifi_intf_get_var_is_wireless_radio_present
 *
 * Description: Get wireless radio presence
 *
 ******************************************************************************/
error_type_t wifi_hw_get_var_is_radio_present(bool *present)
{
    if (present == NULL)
    {
        return FAIL;
    }

    *present = gIsWirelessRadioPresent;

    return OK;
}

/******************************************************************************
 * Function Name: wifi_hw_get_var_is_shutdown_completed
 *
 * Description: Get wireless radio shutdown completed state
 *
 ******************************************************************************/
error_type_t wifi_hw_get_var_is_shutdown_completed(bool *shutdown)
{
    if (shutdown == NULL)
    {
        return FAIL;
    }

    *shutdown = WlanIsShutdownCompleted();

    return OK;
}

/******************************************************************************
 * Function Name: wifi_intf_set_var_is_wireless_radio_present
 *
 * Description: Set wireless radio presence
 *
 ******************************************************************************/
void wifi_intf_set_var_is_wireless_radio_present(bool present)
{
    gIsWirelessRadioPresent = present;
}

/******************************************************************************
 * Function Name: wifi_intf_start_wps
 *
 * Description: Start WPS (Wifi Protected Setup)
 *
 ******************************************************************************/
void wifi_intf_start_wps(uint8_t * ssid, uint8_t ssid_len, uint8_t * pin, uint8_t pin_len)
{
    wirelessStartWPSEnrollee((char*)ssid, (char*)pin);
}

/******************************************************************************
 * Function Name: wifi_intf_start_uap_wps_registrar
 *
 * Description: Start WPS (Wifi Protected Setup) as a registrar on UAP
 *
 ******************************************************************************/
void wifi_intf_start_uap_wps_registrar(uint8_t * pin, uint8_t pin_len)
{
    wirelessStartWPSRegistrar((char *)pin);
}


/******************************************************************************
 * Function Name: wifi_intf_start_wfd_wps_registrar
 *
 * Description: Start WPS (Wifi Protected Setup) as a registrar on WFD GO
 *
 ******************************************************************************/
void wifi_intf_start_wfd_wps_registrar(uint8_t * pin, uint8_t pin_len)
{
    wirelessStartWFDWPSRegistrar((char *)pin);
}


/******************************************************************************
 * Function Name: wifi_intf_stop_wps
 *
 * Description: Stop WPS session in-progress (Wifi Protected Setup)
 *
 ******************************************************************************/
void wifi_intf_stop_wps()
{
    wirelessCancelWPS();
}

/******************************************************************************
 * Function Name: wifi_intf_start_radio
 *
 * Description: Start radio and enable (over power cycle)
 *
 ******************************************************************************/
void wifi_intf_start_radio()
{
    WlanStartup();
#ifdef HAVE_POWER_MGR
    gWirelessPwrOffAllow = FALSE;
#endif
}

/******************************************************************************
 * Function Name: wifi_intf_stop_radio
 *
 * Description: Stop radio and disable (over power cycle)
 *
 ******************************************************************************/
void wifi_intf_stop_radio()
{
    WlanShutdown();
}

/******************************************************************************
 * Function Name: wifi_intf_start_scan
 *
 * Description: Start wireless scan
 *
 ******************************************************************************/
void wifi_intf_start_scan(uint8_t * ssid, uint8_t len)
{
    wirelessStartScan(ssid);
}

/******************************************************************************
 * Function Name: wifi_intf_usb_suspend
 *
 * Description: Suspend USB
 *
 ******************************************************************************/
error_type_t wifi_intf_usb_suspend(void * remote_wakeup_callback)
{
    error_type_t retval = FAIL;

    if (WlanUSBSuspend(remote_wakeup_callback) == 0)
        retval = OK;

    return retval;
}

/******************************************************************************
 * Function Name: wifi_intf_usb_resume
 *
 * Description: Resume USB
 *
 ******************************************************************************/
void wifi_intf_usb_resume(void)
{
    WlanUSBResume();
}

/******************************************************************************
 * Function Name: wifi_intf_set_usb_port
 *
 * Description: Set wifi USB port
 *
 ******************************************************************************/
void wifi_intf_set_usb_port(int port)
{
    WlanSetUSBPort(port);
}


/******************************************************************************
 * Function Name: wifi_intf_sta_get_link_speed
 *
 * Description: Get wifi STA link speed
 *
 ******************************************************************************/
error_type_t wifi_intf_sta_get_link_speed(int *link_speed)
{
    error_type_t retval = OK;

    *link_speed = WlanGetRate();

    // Convert speed from bps to kbps
    *link_speed = *link_speed / 1000;

    return retval;
}


/******************************************************************************
 * Function Name: wifi_intf_disallow_pwr_mgr
 *
 * Description: Disallow power save
 *
 ******************************************************************************/
void wifi_intf_disallow_pwr_mgr(uint32_t *uid)
{
#ifdef HAVE_POWER_MGR
    if (gWirelessPwrOffAllow)
    {
        return;
    }
    // we don't have the 'override' that allows power off, so wakeup
    if (*uid == 0)
        *uid = pwr_mgr_get_uid();

    pwr_mgr_go_active_wait(*uid);
    DPRINTF(WDERR, ("wifi_intf_disallow_pwr_mgr: pwr_mgr_go_active_wait 0x%08X\n",*uid));
#endif
}

/******************************************************************************
 * Function Name: wifi_intf_allow_pwr_mgr
 *
 * Description: Allow power save
 *
 ******************************************************************************/
void wifi_intf_allow_pwr_mgr(uint32_t *uid)
{
#ifdef HAVE_POWER_MGR
    if (*uid == 0)
        *uid = pwr_mgr_get_uid();

    pwr_mgr_go_idle(*uid);

    DPRINTF(WDERR, ("wifi_intf_allow_pwr_mgr: pwr_mgr_go_idle 0x%08X\n",*uid));
#endif
}

