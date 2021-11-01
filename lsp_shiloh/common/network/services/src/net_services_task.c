/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// NET_SERVICES msg interface and state machine

// Typical usage:
//
//   Service is configured w/NET_SERVICE_CONFIG, followed by a restart (RENEW)

// Examples:
//
//   e.g, net_service_request(NET_SERVICE_STOP, NET_SERVICE_STA, NULL, NULL);
//   e.g, net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, NULL, NULL);
//   e.g, net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_UAP, NULL, NULL);
//   e.g, net_service_request(NET_SERVICE_RENEW, NET_SERVICE_MDNS, NULL, NULL);

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// common headers
#include "oid_api.h"
#include "oid_producer_api.h"
#include "net_api.h" // net api collection

// debug
#define DBG_PRFX "net.service.task: "
//off #define DBG_VERBOSE_ON
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#include "net_debug.h"

#define DEBOUNCE_REQUEST_WINDOW 100 /* revisit tunable debounce time */

// local exclusion
OS_MUTEX g_net_service_ctxt_mtx;

#define NET_SERVICE_CONTEXT_LOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_service_ctxt_mtx); \
            XASSERT(os_rcode == 0, os_rcode); \
        }

#define NET_SERVICE_CONTEXT_UNLOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_service_ctxt_mtx); \
            XASSERT(os_rcode == 0, os_rcode); \
        } 

// globals

static __attribute__ ((aligned (8))) char net_services_stack[POSIX_MIN_STACK_SIZE];
pthread_t             net_services_thread;
net_service_msg_buf_t g_net_service_msg_buf[NET_SERVICE_MQ_NUM_MSG_BUFS]; // msg queue buffer space
mqd_t                 g_net_service_msg_queue;
net_services_ctxt_t   g_net_services_ctxt;

// timer
timer_t downstream_request_timer; // 'config' change window timer
static int requested_activity_running;

void net_service_config_cb(union sigval ignored)
{
    DBG_VERBOSE("%s() running\n", __func__);
    requested_activity_running = 0; // expire timer
}

// setup a network health checkup
int initiate_downstream_activity_timer(long ms_delta)
{
    DBG_VERBOSE("%s(%d ms)\n", __func__, delta);
    requested_activity_running = start_ms_timer(&downstream_request_timer, ms_delta, net_service_config_cb);
    return requested_activity_running;
}

bool downstream_inprogress(void)
{
    if ((bool)timer_active(&downstream_request_timer)) // cast
    {
        //noisy DBG_VERBOSE("%s() .. waiting ..\n", __func__);
        return true;
    }
    return false;
}

void* net_services_task(void* unused)
{
    NET_SERVICE_CONTEXT_LOCK(); // lock initialized prior to launch ..

    void register_net_services_cmds(void); // extern cast
    register_net_services_cmds();

    // initialize net services state machine
    g_net_services_ctxt.enabled = true; // revisit oid/var state setting?
    add_service_state(0, NET_SERVICES_INIT);

    /*
     * create net service state machine message queue
     */
    ASSERT(sizeof(net_service_msg_buf_t) == NET_SERVICE_MSGQ_BUF_BYTES);

    posix_create_message_queue(&g_net_service_msg_queue,
                                NET_SERVICE_MQ_NAME,
                                NET_SERVICE_MQ_NUM_MSG_BUFS,
                                OS_4_ULONG /*NET_SERVICE_MQ_MSG_BUF_SIZE*/);

    NET_SERVICE_CONTEXT_UNLOCK();

    // schedule periodic network checkup
    initiate_net_services_timer(1000 *
#if NET_LINK_STATUS_CHANGE_POLLING
            7 /* sooner */
#else
            15 /* later -- allow generous settle-time */
#endif
        ); // schedule initial checkup
  
    while (1) // forever -- no return
    {
        error_type_t os_rcode = posix_wait_for_message(g_net_service_msg_queue, (char*)&g_net_service_msg_buf, sizeof(net_service_msg_t), OS_WAIT_FOREVER);
        if(os_rcode != OS_SUCCESS)
        {
            DBG_ERR("%s() err reading g_net_service_msg_queue (os_rcode=0x%02x)\n", __func__, os_rcode);
            goto error;
        }
        net_service_msg_t* msg = (net_service_msg_t*)&g_net_service_msg_buf; // revisit msg = &g_net_service_msg_buf.msg;

        if (msg->reply) *(uint32_t*)(msg->reply) = OK; // cast

        // currently busy/active in requested service/state?
        if (g_net_services_ctxt.state & (msg->service | msg->request))
        {
            DBG_VERBOSE("%s() %s/%s busy ..\n", __func__, str_request(msg->request), str_service(msg->service));
            continue; // proceed w/current activity
        }

        //tbd? add_service_state(msg->service, msg->request); // mark this request inprogress
        add_service_state(0, NET_SERVICES_BUSY);

        bool do_one = (msg->service == NET_SERVICES_ALL ? false : true); // all or one service?

        switch (msg->request)
        {
            case NET_SERVICE_CONFIG: // conf file update
            {
                switch (msg->service)
                {
                    case NET_SERVICE_UNAVAILABLE:
                      default:
                        DBG_VERBOSE("%s() msg CONFIG x%x (unavailable unknown)\n", __func__, (unsigned int)msg);
                    break;

                    case NET_SERVICES_ALL: // fallthrough
                        DBG_VERBOSE("%s() CONFIG all..\n", __func__);
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                    {
                        uint8_t ssid[MAX_SSID_NAME_LEN];
                        uint8_t index = 0;
                        uap_get_var_ssid((uint8_t)index, ssid, MAX_SSID_NAME_LEN);
                        if (!strcmp((char*)ssid, DEFAULT_UAP_SSID)) generate_uniq_ssid((char*)ssid);
                        DBG_VERBOSE("%s() msg CONFIG wifi-uap ssid '%s'\n", __func__, ssid);
                        net_service_workq(msg->request, msg->service, (void*)ssid, msg->reply);
                        if (do_one) break;
                    }
#endif
#ifdef HAVE_WLAN
                    case NET_SERVICE_WLAN:
                    {
                        char ssid[MAX_SSID_NAME_LEN];
                        uint8_t index = 0;
                        oid_get_string(OID_WIFI_INTF_SSID, index, ssid, MAX_SSID_NAME_LEN);
                        if (!strcmp((char*)ssid, DEFAULT_WIFI_SSID)) generate_uniq_ssid(ssid);
                        DBG_VERBOSE("%s() msg CONFIG wifi-sta ssid '%s'\n", __func__, ssid);
                        net_service_workq(msg->request, msg->service, (void*)ssid, msg->reply);
                        if (do_one) break;
                    }
#endif
#endif // HAVE_WIRELESS
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                    {
                        char service_name[NET_MDNS_SERVICE_NAME_SIZE];
                        uint8_t index = 0;
                        oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, index, service_name, NET_MDNS_SERVICE_NAME_SIZE);
                        if (!strcmp(service_name, DEFAULT_PLATFORM_HOSTNAME)) generate_uniq_service_name(service_name);
                        DBG_VERBOSE("%s() msg CONFIG service '%s'\n", __func__, service_name);
                        net_service_workq(msg->request, msg->service, (void*)service_name, msg->reply);
                        if (do_one) break;
                    }
#endif
                }

            } break;

            case NET_SERVICE_START:
            {

                initiate_downstream_activity_timer(DEBOUNCE_REQUEST_WINDOW);

                switch (msg->service)
                {
                    case NET_SERVICE_UNAVAILABLE:
                      default:
                        DBG_VERBOSE("%s() msg START service %s\n", __func__, str_service(msg->service));
                    break;

                    case NET_SERVICE_IFCONFIG:
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // stop_etc_network_service();
                        if (do_one) break;
                    break;


                    case NET_SERVICES_ALL: // fallthrough
                        DBG_VERBOSE("%s() START all..\n", __func__);

#ifdef HAVE_SNMP_SUBAGENT
                    case NET_SERVICE_SNMP:
                        /* start_snmp_service(); // 1-time only (see rcS) */
                        if (do_one) break;
#endif
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                        DBG_VERBOSE("%s() msg START wifi-uap\n", __func__);
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply);
                        if (do_one)
                        {
                            //early? net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, msg->udata, msg->reply);
                            break;
                        }
#endif
#ifdef HAVE_WLAN
                    case NET_SERVICE_STA: // aka mlan
                        DBG_VERBOSE("%s() msg START wifi-sta\n", __func__);
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // start_wifi_service();
                        if (do_one)
                        {
                            //early? net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, msg->udata, msg->reply);
                            break;
                        }
#endif
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                        DBG_VERBOSE("%s() msg START mdns\n", __func__);
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply);
                        if (do_one) break;
#endif
                }
            } break;

            case NET_SERVICE_STOP:
            {

                initiate_downstream_activity_timer(DEBOUNCE_REQUEST_WINDOW);

                switch (msg->service)
                {
                    case NET_SERVICE_UNAVAILABLE:
                      default:
                        DBG_ERR("%s() msg STOP service %d\n", __func__, msg->service);
                    break;

                    case NET_SERVICE_IFCONFIG:
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // stop_etc_network_service();
                        if (do_one) break;
                    break;

                    case NET_SERVICES_ALL: // fallthrough
                        DBG_VERBOSE("%s() STOP all..\n", __func__);

#ifdef HAVE_SNMP_SUBAGENT
                    case NET_SERVICE_SNMP:
                        /* stop_snmp_service(); // never turned off */
                        if (do_one) break;
#endif
#ifdef HAVE_WLAN
                    case NET_SERVICE_STA: // aka mlan
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // stop_wifi_service();
                        if (do_one) break;
#endif
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // stop_mdns_service();
                        if (do_one) break;
#endif
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply); // stop_uap_service();
                        if (do_one) break;
#endif
                }
            } break;

            case NET_SERVICE_RENEW: // stop if disabled, start if enabled
            {
                // check for downstream activity timer before commencing a stop/start sequence.
                if (downstream_inprogress())
                {
                    // try request again
                    posix_sleep_ms(1); // token stall
                    net_service_request(msg->request, msg->service, msg->udata, msg->reply);
                    break;
                }

                switch (msg->service)
                {
                    default:
                    case NET_SERVICE_UNAVAILABLE:
                        DBG_ERR("%s() msg RENEW service %s\n", __func__, str_service(msg->service));
                    break;

                    case NET_SERVICE_IFCONFIG:
                        net_service_workq(msg->request, msg->service, msg->udata, msg->reply);
                    break;

                    case NET_SERVICES_ALL: // fallthrough
#ifdef HAVE_SNMP_SUBAGENT
#endif
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP: // aka wifi-uap
                        net_service_workq(NET_SERVICE_STOP, msg->service, msg->udata, msg->reply);
                        if (wifi_uap_enabled())
                            net_service_workq(NET_SERVICE_START, NET_SERVICE_MDNS, msg->udata, msg->reply);
                        if (do_one) break;
#endif // HAVE_UAP
#ifdef HAVE_WLAN
                    case NET_SERVICE_STA: // aka mlan aka wlan aka wifi-sta
                        net_service_workq(NET_SERVICE_STOP, msg->service, msg->udata, msg->reply);
                        if (wifi_sta_enabled())
                            net_service_workq(NET_SERVICE_START, NET_SERVICE_MDNS, msg->udata, msg->reply);
                        if (do_one) break;
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                        net_service_workq(NET_SERVICE_STOP, msg->service, msg->udata, msg->reply);
                        if (mdns_enabled())
                            net_service_workq(NET_SERVICE_START, NET_SERVICE_MDNS, msg->udata, msg->reply);
                        if (do_one) break;
#endif // HAVE_MDNS
                    case NET_SERVICE_SNMP:
                        break;
                }
            } break;

            // finished requestion, return to idle
            DBG_VERBOSE("%s() finished req %s/%s\n", __func__, str_service(msg->service), str_request(msg->request));
        }

        // clear most-recent activity (possibly still BUSY ..)
        //tbd? clr_service_state(msg->service, msg->request); // done w/this request

    } // forever do ..

error:
    return NULL;
}

/**************************************************/
/** \brief Initialize the net_services() thread(s)*
 *                                                *
 */
void launch_net_services_responder(void)
{
    launch_net_services_backend(); // worker-thread

    /* create thread for network services management */
    posix_create_thread(&net_services_thread,
                       net_services_task,
                       (void *) 1,
                       "net services",
                      &net_services_stack,
	              sizeof(net_services_stack),
                      POSIX_THR_PRI_NORMAL);
}

//=========================================================================================================================
// Module entry point
//=========================================================================================================================

// declare the common/network init() such that an oem-specific net_iface_init() can override
void net_services_init(void)
{
    DBG_VERBOSE("==>%s\n",__func__);

    error_type_t os_rcode = posix_mutex_init(&g_net_service_ctxt_mtx);
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating g_net_service_ctxt mutex (os_rcode=0x%02x)\n", os_rcode);
        ASSERT(0);
    }

    launch_net_services_backend();
    launch_net_services_responder();

    posix_sleep_ms(DEBOUNCE_REQUEST_WINDOW); // token settle-time

    NET_SERVICE_CONTEXT_LOCK();

    // cleanup/(re)start any needed services
    net_service_request(NET_SERVICE_STOP, NET_SERVICE_MDNS, NULL, NULL);
    //net_service_request(NET_SERVICE_START, NET_SERVICE_SNMP, NULL, NULL);
    //net_service_request(NET_SERVICE_START, NET_SERVICE_STA, NULL, NULL);
    //net_service_request(NET_SERVICE_START, NET_SERVICE_UAP, NULL, NULL);

    NET_SERVICE_CONTEXT_UNLOCK();
    
    return;
}

// misc metric counters
long    g_net_service_all_req_counter = 0;
long    g_net_service_folded_req_counter = 0;

// send msg request to service's 'backend'
//
//  possibly screen out this latest request if it occurs within a narrow
//  window of time
error_type_t net_service_request(
    net_service_request_t request,
    net_service_t service,
    void *udata,
    void *reply)
{
    static net_service_msg_t last_req = { 0 };
    static long last_req_time = 0;
           long this_req_time = posix_gettime_ticks();
    bool continue_request = true;

    g_net_service_all_req_counter++;

    net_service_msg_t this_req = { request, service, udata, reply };
    long req_time_delta = this_req_time - last_req_time;

    if (req_time_delta < DEBOUNCE_REQUEST_WINDOW)
    {
        //if (this_req == last_req) // duplicate request?
        if (!(memcmp(&this_req, &last_req, sizeof(net_service_msg_t))))
        {
            DBG_VERBOSE("%s(%s %s) screened..\n", __func__,
                str_request(request), str_service(service));
            g_net_service_folded_req_counter++;
            continue_request = false; // don't send this latest request
        }
    }

    // update history
    last_req_time = this_req_time;
    last_req.request = request;
    last_req.service = service;
    last_req.udata   = udata;
    last_req.reply   = reply;

    if (continue_request)
    {
        net_service_msg_t buf;
        buf.request = request;
        buf.service = service;
        buf.udata   = udata;
        buf.reply   = reply;

        DBG_VERBOSE("%s(%s %s x%x)\n", __func__, str_request(request), str_service(service), udata);

        // q? errno 110 'Connection timed out' errors w/ OS_NO_WAIT ?
        unsigned int os_rcode = posix_message_send(g_net_service_msg_queue, (char *)&buf, sizeof(buf),
                                            MQ_DEFAULT_PRIORITY, OS_WAIT_FOREVER); // alternatively OS_NO_WAIT
        if (os_rcode == OS_QUEUE_FULL)
        {
            /* 
             *  msg queue full - restart the timer to retry
             * 
             *  NOTE: should only get here when caller is a non-thread (e.g. timer) and os_wait_opt must
             *  therefore be set to OS_NO_WAIT
             */
            return NET_WOULD_BLOCK;
        }
        else if (os_rcode != OS_SUCCESS)
        {
            // any other errors are unexpected and unhandled
            DBG_ERR("%s() errno %d (x%x): %s\n", __func__, errno, errno, strerror(errno));
            XASSERT(0, os_rcode);
        }
    }

    return NET_OK;
}

#ifdef __cplusplus
}
#endif

//eof
