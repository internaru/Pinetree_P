/** @file  mlanevent.c
 *
 *  @brief Source file for mlanevent processing
 *
 *  Copyright (C) 2008-2012, Marvell International Ltd. All
 *  Rights Reserved
 */

#include <tx_api.h>
#include "mlanevent.h"
#include "logger.h"
#include "agLinkedList.h"
#include "string.h"

#include "agMessage.h"
#include "agRouter.h"
//#include "MfpSystem.h"
#include "sys_init_api.h"
#include "memAPI.h"
#include "wlanInterface.h"
#ifdef HAVE_ASIC_POWER_DRIVER
#include "usbh_wifi_pwr_drv.h"
#endif

#define DBG_PREFIX "[Wifi Event] "
#define DBG_EVENT      DBG_OUTPUT | DBG_LOUD
#define DBG_EVENT_ERR  DBG_OUTPUT | DBG_LOUD | DBG_ERROR

#define MLAN_EVENT_MSG_COUNT    16 
#define MLAN_EVENT_STACK_SIZE   1024 * 4

static TX_THREAD mlan_event_thread;
static ALIGN8 char mlan_event_stack[MLAN_EVENT_STACK_SIZE];
static TX_QUEUE  mlan_event_msg_q;
static UINT32    mlan_event_msg_q_storage[MLAN_EVENT_MSG_COUNT * sizeof(MESSAGE)];
static TX_MUTEX  notification_list_mutex;

static LINK_HEADER notification_list = {NULL, NULL};

static void log_events_to_serial_port(const char *iface, const t_u8 *event_data, t_u32 len);
static void mlan_event_thread_func(UINT32 unused);

void mlan_event_initialize()
{
    tx_queue_create(&mlan_event_msg_q, "mlan_event_msg_q", TX_4_ULONG, mlan_event_msg_q_storage, MLAN_EVENT_MSG_COUNT*sizeof(MESSAGE));
    SYRegister(MLAN_EVENT_ID, &mlan_event_msg_q);

    tx_mutex_create(&notification_list_mutex, "mlan_event_mutex", 0);

    agLinkInit(&notification_list);

    tx_thread_create(&mlan_event_thread, "mlan_drvr_events", (void(*)(ULONG))mlan_event_thread_func, 
                     (int) NULL, mlan_event_stack, MLAN_EVENT_STACK_SIZE, THR_PRI_NORMAL, THR_PRI_NORMAL,
                     1, TX_AUTO_START );
}

static void mlan_event_thread_func(UINT32 unused)
{
    MESSAGE mlan_event_msg;

    SysWaitForInit();

    mlan_event_request_notifications(log_events_to_serial_port);

    while(TRUE)
    {
        // Wait for an event
        if(TX_SUCCESS == tx_queue_receive(&mlan_event_msg_q, &mlan_event_msg, TX_WAIT_FOREVER))
        {
            LINK_MEMBER  *cur_link;
            mlan_event_notify_function notify_func;
            char  *iface          = (char *)mlan_event_msg.param1;
            t_u8  *event_data     = (t_u8 *)mlan_event_msg.param3;
            t_u32  event_data_len = (t_u32)mlan_event_msg.param2;

            tx_mutex_get(&notification_list_mutex, TX_WAIT_FOREVER);

            for (cur_link = agGetHead(&notification_list); cur_link != NULL; cur_link = cur_link->NextLink)
            {
                notify_func = (mlan_event_notify_function)cur_link->Data;

                if (notify_func != NULL)
                {
                    notify_func(iface, event_data, event_data_len);
                }
            }

            tx_mutex_put(&notification_list_mutex);

            MEM_FREE_AND_NULL(event_data);
        }
    }
}


void process_mlan_drvr_event(char *iface, t_u8 *payload, t_u32 len)
{
    MESSAGE event_msg;

    if ((payload != NULL) && len >= sizeof(event_header))
    {
        // We may show this message to an arbitrary number of people.
        // Break context onto an independent thread so they don't slow the driver down.
        event_msg.msgType = MSG_MLAN_DRVR_EVENT;
        event_msg.param1  = (int)iface;
        event_msg.param2  = len;
        event_msg.param3  = MEM_MALLOC(len);
        if (event_msg.param3 != NULL)
        {
            memcpy(event_msg.param3, payload, len);
            SYMsgSend(MLAN_EVENT_ID, &event_msg);
        }
        else
        {
            DPRINTF(DBG_EVENT_ERR, ("[Wifi Event %s] Can't process message (not enough memory)\n", iface));
        }
    }
}

int mlan_event_request_notifications(mlan_event_notify_function notification_func)
{
    LINK_MEMBER  *new_notification_link;
    int ret_val = 0;

    new_notification_link = (LINK_MEMBER *)MEM_MALLOC(sizeof(LINK_MEMBER));

    if (new_notification_link != NULL)
    {
        tx_mutex_get(&notification_list_mutex, TX_WAIT_FOREVER);

        new_notification_link->Data = notification_func;
        agAddTail(&notification_list, new_notification_link);
        ret_val = 0;

        tx_mutex_put(&notification_list_mutex);
    }
    else
    {
        ret_val = -1;
    }

    return ret_val;
}

int mlan_event_cancel_notifications(mlan_event_notify_function notification_func)
{
    int ret_val = -1;
    LINK_MEMBER *notifier;

    tx_mutex_get(&notification_list_mutex, TX_WAIT_FOREVER);

    for (notifier = notification_list.LinkHead; notifier != NULL; notifier = notifier->NextLink)
    {
        if (notifier->Data == notification_func)
        {
            agDelItem(&notification_list, notifier);
            ret_val = 0;
            break;
        }
    }

    tx_mutex_put(&notification_list_mutex);

    return ret_val;
}

static void log_events_to_serial_port(const char *iface, const t_u8 *event_data, t_u32 len)
{
    const event_header     *mlan_event = (const event_header *)event_data;
    eventbuf_sta_assoc     *sta_assoc;
    eventbuf_sta_deauth    *sta_deauth;
    eventbuf_bss_start     *bss_start;
    eventbuf_rsn_connect   *rsn_connect;
    apeventbuf_wfd_generic *wfd_generic;
    u16 subtype;
    bool uap_enabled;

    wifi_intf_get_var_is_enabled(WIFI_IFACE_ID_UAP, &uap_enabled);

    switch(uap_le32_to_cpu(mlan_event->event_id))
    {
        case MICRO_AP_EV_ID_STA_ASSOC:
            sta_assoc = (eventbuf_sta_assoc *)mlan_event->event_data;
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] Device associated: %02x:%02x:%02x:%02x:%02x:%02x\n", iface, 
                                (int)sta_assoc->sta_mac_address[0], (int)sta_assoc->sta_mac_address[1],
                                (int)sta_assoc->sta_mac_address[2], (int)sta_assoc->sta_mac_address[3],
                                (int)sta_assoc->sta_mac_address[4], (int)sta_assoc->sta_mac_address[5]));

            wifi_intf_uap_notify_event( WLAN_ASSOCIATED, uap_enabled, (char *)iface, strlen(iface), sta_assoc->sta_mac_address, 0);

            break;

        case MICRO_AP_EV_ID_STA_DEAUTH:
            sta_deauth = (eventbuf_sta_deauth *)mlan_event->event_data;

            DPRINTF(DBG_EVENT, ("[Wifi Event %s] Device de-authed: %02x:%02x:%02x:%02x:%02x:%02x, reason: %d\n", iface,
                                (int)sta_deauth->sta_mac_address[0], (int)sta_deauth->sta_mac_address[1],
                                (int)sta_deauth->sta_mac_address[2], (int)sta_deauth->sta_mac_address[3],
                                (int)sta_deauth->sta_mac_address[4], (int)sta_deauth->sta_mac_address[5],
                                uap_le16_to_cpu(sta_deauth->reason_code)));
            wifi_intf_uap_notify_event( WLAN_DEAUTH, uap_enabled, (char *)iface, strlen(iface), sta_deauth->sta_mac_address, 0);
            break;

        case MICRO_AP_EV_ID_BSS_START:
            bss_start = (eventbuf_bss_start *)mlan_event->event_data;
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] BSS started: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                (int)bss_start->ap_mac_address[0], (int)bss_start->ap_mac_address[1],
                                (int)bss_start->ap_mac_address[2], (int)bss_start->ap_mac_address[3],
                                (int)bss_start->ap_mac_address[4], (int)bss_start->ap_mac_address[5]));
            break;
    
        case MICRO_AP_EV_BSS_IDLE:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] BSS is now IDLE\n", iface));
            break;
    
        case MICRO_AP_EV_BSS_ACTIVE:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] BSS is now ACTIVE\n", iface));
            break;

        case MICRO_AP_EV_RSN_CONNECT:
            rsn_connect = (eventbuf_rsn_connect *)mlan_event->event_data;
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] RSN connect by: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                (int)rsn_connect->sta_mac_address[0], (int)rsn_connect->sta_mac_address[1],
                                (int)rsn_connect->sta_mac_address[2], (int)rsn_connect->sta_mac_address[3],
                                (int)rsn_connect->sta_mac_address[4], (int)rsn_connect->sta_mac_address[5]));
            break;

        case MICRO_AP_EV_WMM_STATUS_CHANGE:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] WMM status change\n", iface));
            break;

        case UAP_EVENT_ID_HS_WAKEUP:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] HS wakeup\n", iface));
            break;
    
        case UAP_EVENT_ID_DRV_HS_ACTIVATED:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] HS activated\n", iface));
            break;
    
        case UAP_EVENT_ID_DRV_HS_DEACTIVATED:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] HS deactivated\n", iface));
#ifdef HAVE_ASIC_POWER_DRIVER
            usb_host_wifi_pwr_usb_resume();
#else
            WlanUSBResume();
#endif
            break;
   
        case UAP_EVENT_HOST_SLEEP_AWAKE:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] Host sleep awake\n", iface));
            break;



        case EVENT_WFD_SERVICE_DISCOVERY:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] WFD Service Discovery Event\n", iface));
            break;
    
        case EVENT_WFD_GENERIC:
            wfd_generic = (apeventbuf_wfd_generic *)(mlan_event->event_data);
            subtype = uap_le32_to_cpu(wfd_generic->event_type);
            switch(subtype)
            {
                case EV_TYPE_WFD_PEER_DETECTED:
#if 0
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Detected Peer: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
#endif
                    break;

                case EV_TYPE_WFD_NEGOTIATION_REQUEST:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Negotiation Request: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_NEGOTIATION_RESPONSE:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Negotiation Response: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_NEGOTIATION_RESULT:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Negotiation Result: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_INVITATION_REQ:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Invitation request: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_INVITATION_RESP:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Invitation response: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_DEVICE_DISCOVERY_REQ:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Dev. Discovery Request: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_DEVICE_DISCOVERY_RESP:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Dev. Discovery Response: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_PROVISION_DISCOVERY_REQ:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Prov. Discovery Request: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;

                case EV_TYPE_WFD_PROVISION_DISCOVERY_RESP:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] Prov. Discovery Response: %02x:%02x:%02x:%02x:%02x:%02x\n", iface,
                                        (int)wfd_generic->peer_mac_addr[0], (int)wfd_generic->peer_mac_addr[1],
                                        (int)wfd_generic->peer_mac_addr[2], (int)wfd_generic->peer_mac_addr[3],
                                        (int)wfd_generic->peer_mac_addr[4], (int)wfd_generic->peer_mac_addr[5]));
                    break;


                default:
                    DPRINTF(DBG_EVENT, ("[Wifi Event %s] WFD generic event (subtype: %d)\n", iface, subtype));
                    break;
            }
            break;

        default:
            DPRINTF(DBG_EVENT, ("[Wifi Event %s] Generic event, type: %d\n", iface, uap_le32_to_cpu(mlan_event->event_id)));
            break;
    }
}
