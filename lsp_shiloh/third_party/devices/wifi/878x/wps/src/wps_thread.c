/*
 * ============================================================================
 * Copyright (c) 2008-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file wps_thread.c
 * 
 * \brief Contains the thread WPS executes on.
 * 
 **/

// *********************************************************************

#include <tx_api.h>
#include <agMessage.h>
#include <logger.h>
#include <memAPI.h>
#include <sys/types.h>
#include <SysInit.h>
#include <os_net_device.h>
#include "wlanInterface.h"
#include "wifi_intf.h"
#include "wifi_intf_api.h"
#include "wifi_intf_vars.h"
#include "wps_events.h"
#include "wps_def.h"
#include "mlanevent.h"

int SupConfig(wpaSupConfig sConfig);

// *********************************************************************

#define WPS_STACK_SIZE    14 * 1024
#define NUM_WPS_MSGS      8 
			
// *********************************************************************

ALIGN8 char wpsStack[WPS_STACK_SIZE];

TX_THREAD wpsThread;
TX_QUEUE  wpsMsgQ;
UINT32    wpsMsgQBuf[NUM_WPS_MSGS * sizeof(MESSAGE)];
static BOOL wpsThreadStarted = FALSE;


// *********************************************************************

void wpsThreadFunc(UINT unused);
int wps_main(int role, char *iface, char *ssid, char *pin);
int WlanShimCDeauth(void);
int wps_wlan_session_control(int enable);
void abort_wps(void);

// *********************************************************************

static void detected_wifi_event(const char *iface, const t_u8 *event_data, t_u32 len)
{
#ifdef WIFI_DIRECT_SUPPORT
    const event_header *mlan_event = (const event_header *)event_data;

    if (strcmp(WIFI_IFACE_NAME_WFD, iface) == 0)
    {
        if (mlan_event->event_id == EVENT_WFD_GENERIC)
        {
            apeventbuf_wfd_generic *wfd_generic = (apeventbuf_wfd_generic *)mlan_event->event_data;
            u16 subtype = wfd_generic->event_type;

            if (subtype == EV_TYPE_WFD_PROVISION_DISCOVERY_REQ)
            {
                uint8_t role;
                wifi_intf_get_var_wfd_bss_role(WIFI_IFACE_ID_WFD, &role);

                // If Autonomous GO then start WPS Registrar
                if (role == WIFIDIRECT_GO_ROLE)
                {
                    wirelessStartWFDWPSRegistrar(NULL);
                }
            }
        }
    }
#endif
}

// *********************************************************************

/** 
 * \brief Create the thread that runs WPS and start running it.
 * 
 **/
void startWPSThread(void)
{
    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("WPS: ==> startWPSThread\n"));

    // Create our message queue
    tx_queue_create(&wpsMsgQ, "wpsMsgQ", TX_4_ULONG, wpsMsgQBuf, NUM_WPS_MSGS * sizeof(MESSAGE));

    // Register our message queue with the system
    SYRegister(WPSID, &wpsMsgQ);

    wpsThreadStarted = TRUE;
    tx_thread_create( &wpsThread, 
                       "WPS (WiFi Setup)",
                       (void(*)(ULONG)) wpsThreadFunc,
                       0, 
                       wpsStack,
                       WPS_STACK_SIZE, 
                       THR_PRI_NORMAL, 
                       THR_PRI_NORMAL,
                       1, 
                       TX_AUTO_START );
    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("WPS: <== startWPSThread\n"));
}


/** 
 * \brief Main processing loop for the WPS thread.
 * 
 **/
void wpsThreadFunc(UINT unused)
{
    MESSAGE  wpsMsg;
    BOOL     run = TRUE;

//    DSNOTIFYREGISTER(e_WirelessInfo, WIRELESSCONFIGID);

    // Wait for the rest of the system to initialize
    SysWaitForInit();

    mlan_event_request_notifications(detected_wifi_event);

    while(run)
    {
        if(TX_SUCCESS == tx_queue_receive(&wpsMsgQ, &wpsMsg, TX_WAIT_FOREVER))
        {
            DPRINTF((DBG_SOFT|DBG_OUTPUT), ("WPS received a message\n"));

            switch(wpsMsg.msgType)
            {
                case MSG_RUN_WPS:
                {
                    t_wps_request *request = (t_wps_request *)wpsMsg.param3;

                    if (WlanIsWirelessHWActive())
                    {
                        MESSAGE      wConfigMsg;
                        wpaSupConfig sConfig;
                        int          wps_ret_val;
                        bool         wps_sta_enrollee = false;

                        wifi_intf_set_var_is_wps_running(TRUE);

                        if (strcmp(request->iface, WIFI_IFACE_NAME_STATION) == 0)
                        {
                            wps_sta_enrollee = true;
                        }

                        if (wps_sta_enrollee)
                        {
                            // Make sure we aren't trying to run the supplicant.
                            sConfig.WPA_ON = 0;
                            SupConfig(sConfig);

                            // Disconnect from whatever we were connected to before
                            WlanShimCDeauth();
                        }

                        // Start Wireless connect LED blinking
                        if (wps_sta_enrollee)
                        	wifi_intf_sta_set_state(WLAN_WPS_RUNNING);
                        else
                            wifi_intf_uap_set_state(WLAN_WPS_RUNNING, NULL, 0, NULL, NULL);

                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Start wps_main\n"));
                        wps_ret_val = wps_main(request->role, request->iface, request->ssid, request->pin);
                        if (wps_ret_val != WPS_STATUS_SUCCESS)
                        {
                            // Indicate WPS error
                            if (wps_sta_enrollee)
                            	wifi_intf_sta_set_state(WLAN_WPS_ERROR);
                            else
                                wifi_intf_uap_set_state(WLAN_WPS_ERROR, NULL, 0, NULL, NULL);

	                        // No outcome was set.  Something bad happened.
	                        updateWPSOutcome(WPS_STATE_UNKNOWN_FAILURE);
                        }

                        // Free the memory used for the request
                        if (request->iface != NULL)
                        {
                            MEM_FREE_AND_NULL(request->iface);
                        }
                        if (request->ssid != NULL)
                        {
                            MEM_FREE_AND_NULL(request->ssid);
                        }
                        if (request->pin != NULL)
                        {
                            MEM_FREE_AND_NULL(request->pin);
                        }
                        MEM_FREE_AND_NULL(request);


                        if (wps_sta_enrollee)
                        {
                            // Be absolutely sure we have unassociated from the AP and
                            // that the WPS session is completely disabled.
                            WlanShimCDeauth();
                        }

                        wps_wlan_session_control(0);

                        // Mark that we aren't running WPS any more.  This must be done
                        // before we apply the wireless settings.
                        wifi_intf_set_var_is_wps_running(FALSE);


                        if (wps_sta_enrollee)
                        {
                            // Regardless of how things turned out, re-config our settings.
                            // This will either configure us to use the new settings or 
                            // go back to the previous valid ones (in case of failures).
                            wConfigMsg.msgType = MSG_WIRELESS_UPDATE;
                            wConfigMsg.param1  = WIFI_IFACE_ID_STATION;
                            SYMsgSend(WIRELESSCONFIGID, &wConfigMsg);
                        }
                    }
                    else
                    {
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("ERROR: Wireless not active\n"));
                    }
                    break;
                }

                case MSG_THREAD_EXIT:
                    run = FALSE;
                    break;
        
                default:
                    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("WPS thread received an unknown message of type %d\n", wpsMsg.msgType));
                    break;
            }
        }
    }
    mlan_event_cancel_notifications(detected_wifi_event);
    SYUnRegister(WPSID);
    tx_queue_delete(&wpsMsgQ);
}

/** 
 * \brief Checks for a cancel message and aborts if necessary.
 * 
 * If a cancel message has been sent to WPS's thread queue, this
 * routine will cause a user_abort to occur.
 * 
 * \return 0 if no messages present, 1 on abort, 2 on refresh
 * timer
 **/
int cancelWPSIfRequested(void)
{
    MESSAGE wpsMsg;

    if(TX_SUCCESS == tx_queue_receive(&wpsMsgQ, &wpsMsg, TX_NO_WAIT))
    {
        if (wpsMsg.msgType == MSG_CANCEL_WPS)
        {
            abort_wps();
            return 1;
        }
        else if (wpsMsg.msgType == MSG_RUN_WPS)
        {
            // Free the memory used to store the SSID and PIN
            if (wpsMsg.param1)
            {
                void * temp_ptr = (void *) wpsMsg.param1;
                MEM_FREE_AND_NULL(temp_ptr);
            }
            if (wpsMsg.param3)
            {
                MEM_FREE_AND_NULL(wpsMsg.param3);
            }

            return 2;
        }
        if (wpsMsg.msgType == MSG_THREAD_EXIT)
        {
            // abort wps and then repost the stop message to the queue so that
            // the main wps message processing loop will exit
            abort_wps();
            SYMsgSend(WPSID, &wpsMsg);
            return 1;
        }
        else
        {
            DPRINTF((DBG_SOFT|DBG_OUTPUT), ("WPS thread received an unknown message of type %d (while running)\n", wpsMsg.msgType));
        }
    }
    return 0;
}

char *generate_wps_pin( void )
{
    char *pin = MEM_MALLOC( WPS_PIN_LENGTH );
    uint8_t mac_addr[ TM_ETHERNET_PHY_ADDR_LEN ];
    uint32_t *tmp;
    uint32_t rand_number;
    uint32_t checksum = 0;
    uint8_t i;
    uint8_t cur_digit;
    uint32_t factor = 1;

    if ( pin == NULL)
    {
        return pin;
    }

    // Get the lower bytes of the MAC - more unique between devices
    tmp = (uint32_t *)( mac_addr + 2 );

    net_iface_get_var_phys_addr(mac_addr, NET_IFACE_PHYS_ADDR_SIZE);
    rand_number = tx_time_get() ^ *tmp;
    rand_number %= 10000000;

    // Loop over 7 digits to generate checksum
    for( i = 0; i < 7; i++ )
    {   
        cur_digit = rand_number / factor % 10;
        if ( i % 2)
        {
            checksum += cur_digit;
        }
        else
        {
            checksum += cur_digit * 3;
        }
        factor *= 10;
    }

    checksum = ( 10 - ( checksum % 10 ) ) % 10;
    minSprintf( pin, "%07d%d\n", rand_number, checksum );
    
    return pin;
}

/** 
 *  @brief  updates the WPS state
 *
 *  @param new WPS state  
 */

void updateWPSOutcome(wifi_intf_wps_outcome_t newState)
{ 
    //Set the WPS state 
    wifi_intf_set_var_wps_state(newState);

    //Notify observers regarding new WPS state
    //do not send notify for WPS processing, as it should go out with current wps time information
    if (newState != WPS_STATE_PROCESSING)
        wps_notify_event(newState, 0);

    //Cancel currently active WPS sessions    
    CancelWPSSession(); 
}
