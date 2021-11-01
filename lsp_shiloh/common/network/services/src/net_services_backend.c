/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// NET_SERVICES backend msg interface and state machine

// Typical usage:
//
//   Service is configured w/NET_SERVICE_CONFIG, followed by a restart (RENEW)
//
//  (Filtered) Requests are then dispatched to a work queue.

// Examples:
//
//   e.g, net_service_request(NET_SERVICE_STOP, NET_SERVICE_STA, NULL, NULL);
//     -> net_service_packet(NET_SERVICE_STOP, NET_SERVICE_STA, NULL, &reply);

#define publicSignature (const char*)"net_service_mule" /* sanitized */

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
#define DBG_PRFX publicSignature ": "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
#include "net_debug.h"

// local locks
OS_MUTEX g_net_service_backend_mtx;
#define os_time() posix_gettime_ticks()

// globals
long g_all_workq_req_counter = 0;
long g_folded_workq_req_counter = 0;
int  g_current_req = 0;

// local globals
static long g_last_req_time = 0;

#define FOLD_SAME_REQUEST_TIME 250 /* fold 'same requests' as they occur */

#define NET_SERVICE_CONTEXT_LOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_service_backend_mtx); \
            XASSERT(os_rcode == 0, os_rcode); \
        }

#define NET_SERVICE_CONTEXT_UNLOCK() \
        { \
            int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_service_backend_mtx); \
            XASSERT(os_rcode == 0, os_rcode); \
        } 

static __attribute__ ((aligned (8))) char l_backend_stack[2*POSIX_MIN_STACK_SIZE];

pthread_t             net_services_backend_thread;
#define NET_SERVICE_BACKEND_MQ_BUFS NET_SERVICE_MQ_NUM_MSG_BUFS
mqd_t                 l_backend_msgq;
static net_services_ctxt_t   l_backend_ctxt;
net_service_msg_buf_t l_backend_msg_buf[NET_SERVICE_BACKEND_MQ_BUFS]; // msg queue buffer
static __attribute__ ((aligned (8))) char l_backend_stack[2*POSIX_MIN_STACK_SIZE];

// global 
int task_running_ttl = 0; // running task 'time-to-live' (before timer expire)
timer_t work_packet_ttl_timer; // 'config' change window timer

// timer running? (returns 'busy >0' if active)
long timer_active(timer_t* timer) 
{
    if (!timer || !*timer) return 0; // ignore if timer is uninitialized
    if (task_running_ttl<=0) return 0; // time-to-live active?
    struct itimerspec time_left;
    timer_gettime(*timer, &time_left);
    long pending = time_left.it_value.tv_nsec + time_left.it_value.tv_sec;
    if (pending > 0)
    {
        long ms_remaining = time_left.it_value.tv_sec*1000 + time_left.it_value.tv_nsec/1000000L;
        if (ms_remaining > 0) // cast
        {
            task_running_ttl = ms_remaining;
            dbg_printf("%s(x%x) remaining (%d) ..\n", __func__, timer, ms_remaining);
            return ms_remaining; // return milliseconds remaining
        }
    }
    return 0;
}

static void timer_cancel(timer_t* timer)
{
    if (!timer || !*timer) return; // ignore if timer is uninitialized
    static struct itimerspec zero_time = { { 0, 0} };
    timer_settime(*timer, 0, &zero_time, NULL);

    if (timer == &work_packet_ttl_timer)
    {
        DBG_VERBOSE("%s(work_packet_ttl_timer x%x)\n", __func__, timer);
    }
    else
    {
        DBG_VERBOSE("%s(x%x)\n", __func__, timer);
    }
    return;
}


#define TIME_TO_LIVE 20000 /* ~20 seconds */

void net_service_work_packet_cb(union sigval ignored)
{
    task_running_ttl = 0; // expire timer

    // need to cancel an unresponsive packet request?
    DBG_VERBOSE("%s() done..\n", __func__);
    //revisit tbd DBG_ASSERT(0);
}

// return true/false whether or not a current service request is active..
bool request_inprogress(void)
{
    bool service_request_inprogress = (bool)timer_active(&work_packet_ttl_timer); // cast
    if (service_request_inprogress) DBG_VERBOSE("%s() ..\n", __func__);
    return service_request_inprogress;
}

// setup a network service change request
static int initiate_service_busy_timer(long ms_delta)
{
    task_running_ttl = start_ms_timer(&work_packet_ttl_timer, ms_delta, net_service_work_packet_cb);
    return task_running_ttl;
}

// busy state bookkeeping
int back_to_busy(ns_packet_req_t* work_packet)
{
    if (work_packet)
    {
        work_packet->finish_time = os_time();
    }
    add_service_state(0, NET_SERVICES_BUSY);
    return initiate_service_busy_timer(TIME_TO_LIVE);
}

// idle state bookkeeping
void back_to_idle(ns_packet_req_t* work_packet)
{
    if (work_packet)
    {
        work_packet->finish_time = os_time();
        timer_cancel(&work_packet_ttl_timer);
    }
    clr_service_state(0, NET_SERVICES_BUSY);
    task_running_ttl = 0; // clear ttl here
}


// principle worker thread -- offloads time-consuming os-level 
// requests from the front-end service request task.
//
// side-effect:  NET_SERVICE_CONFIG generates a NET_SERVICE_RENEW
//
void* net_services_backend(void* ignored)
{
    DBG_VERBOSE("%s()..\n", __func__);

    NET_SERVICE_CONTEXT_LOCK(); // lock already initialized ..

    // initialize net services state machine
    l_backend_ctxt.enabled = true; // oid/var setting?
    add_service_state(0, NET_SERVICES_INIT);

    /*
     * create net service state machine message queue
     */
    DBG_ASSERT(sizeof(net_service_msg_buf_t) == NET_SERVICE_MSGQ_BUF_BYTES);

    posix_create_message_queue(&l_backend_msgq,
                                "/ns_mule", // see publicSignature
                                NET_SERVICE_BACKEND_MQ_BUFS,
                                OS_4_ULONG /*NET_SERVICE_MQ_MSG_BUF_SIZE*/);

    NET_SERVICE_CONTEXT_UNLOCK();

    while (1) // forever -- no returns
    {
        static net_service_msg_t last_req = { 0 }; // long-term store previous req
        error_type_t os_rcode = posix_wait_for_message(l_backend_msgq, (char*)&l_backend_msg_buf, sizeof(net_service_msg_t), OS_WAIT_FOREVER);
        if (os_rcode != OS_SUCCESS)
        {
            DBG_ERR("%s() l_backend_msgq %s (err %d os_rcode %d)\n", __func__, strerror(thread_errno), thread_errno, os_rcode);
            continue;
        }
        net_service_msg_t* msg = (net_service_msg_t*)&l_backend_msg_buf;

        DBG_VERBOSE("%s: action %s service %s\n", publicSignature, str_request(msg->request), str_service(msg->service));

#if 0 // || defined DBG_VERBOSE_ON
        char current_state[32];
        #define service_state(state) case state: DBG_MSG("ns_mule() in " #state ":  "); break;
        switch (l_backend_ctxt.state)
        {
            service_state(NET_SERVICES_INIT);
            service_state(NET_SERVICES_BUSY);
            service_state(NET_SERVICES_STOP);
            service_state(NET_SERVICES_START);
            service_state(NET_SERVICES_BOUND);
            default:
                sprintf(current_state, "currently <state x%3x %d> ", l_backend_ctxt.state, l_backend_ctxt.state);
                DBG_MSG(current_state);
            break;
        };
#endif

        // bundle the current request
        net_service_msg_t this_req = { msg->request, msg->service, msg->udata, msg->reply };
        long this_req_time = os_time();
        ns_packet_req_t   current_work_packet  = { this_req, this_req_time, 0 };
        g_last_req_time = this_req_time;

        // update history
        DBG_ASSERT(track_request(
            (msg->request==NET_SERVICE_RENEW ? NET_SERVICE_RENEW:
               (msg->request==NET_SERVICE_STOP ? NET_SERVICE_STOP:
                 (msg->request==NET_SERVICE_START ? NET_SERVICE_START:
                   (msg->request==NET_SERVICE_CONFIG ? NET_SERVICE_CONFIG:
                     0 /* generic default */)
                 )
               )
            ),
            msg->service));

        // advance circular buffer ptr
        g_current_req = (g_current_req >= NUM_TRACKED_REQUESTS ? 0 : g_current_req+1); // advance log ptr
        g_log_req[g_current_req] = current_work_packet;
        last_req = this_req;

        if (msg->reply) *(uint32_t*)msg->reply = OK; // cast

        // currently busy/active in requested service/state?
        if (l_backend_ctxt.state & (msg->service | msg->request))
        {
            DBG_VERBOSE("%s() %s/%s busy ..\n", publicSignature, str_request(msg->request), str_service(msg->service));
            continue; // proceed w/current activity
        }

        add_service_state(msg->service, msg->request); // mark inprogress

        switch (msg->request)
        {
            case NET_SERVICE_CONFIG: // conf file update, and is followed by RENEW
            {

                back_to_busy(&g_log_req[g_current_req]);

                switch (msg->service)
                {
                    case NET_SERVICE_UNAVAILABLE:
                      default:
                        DBG_VERBOSE("%s() msg CONFIG x%x (unavailable unknown)\n", publicSignature, (unsigned int)msg);
                    break;

                    case NET_SERVICES_ALL: // fallthrough
                        DBG_VERBOSE("%s() CONFIG all..\n", publicSignature);
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                    {
                        char ssid[MAX_SSID_NAME_LEN];
                        uint8_t index = 0;
                        uap_get_var_ssid(index, (uint8_t*)ssid, MAX_SSID_NAME_LEN); // cast
                        if (!strcmp(ssid, DEFAULT_UAP_SSID)) generate_uniq_ssid(ssid);
                        DBG_VERBOSE("%s() msg CONFIG wifi-uap ssid '%s'\n", publicSignature, ssid);
                        uap_update_conf_files(ssid);
                    } break;
#endif // HAVE_UAP
#ifdef HAVE_WLAN
                    case NET_SERVICE_WLAN:
                    {
                        char ssid[MAX_SSID_NAME_LEN];
                        uint8_t index = 0;
                        oid_get_string(OID_WIFI_INTF_SSID, index, ssid, MAX_SSID_NAME_LEN);
                        if (!strcmp((char*)ssid, DEFAULT_WIFI_SSID)) generate_uniq_ssid(ssid);
                        DBG_VERBOSE("%s() msg CONFIG wifi-sta ssid '%s'\n", publicSignature, ssid);
                        update_wifi_conf(ssid);
                    } break;
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                    {
                        char service_name[NET_MDNS_SERVICE_NAME_SIZE];
                        uint8_t index = 0;
                        oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, index, service_name, NET_MDNS_SERVICE_NAME_SIZE);
                        if (!strcmp(service_name, DEFAULT_PLATFORM_HOSTNAME)) generate_uniq_service_name(service_name);
                        DBG_VERBOSE("%s() msg CONFIG service '%s'\n", publicSignature, service_name);
                        update_mdns_conf(service_name); 
                    } break;
#endif // HAVE_MDNS
                }

                back_to_idle(&g_log_req[g_current_req]);

                // time to refresh service..
                net_service_request(NET_SERVICE_RENEW, msg->service, NULL, NULL);

            } break;

            case NET_SERVICE_START:
            {
                back_to_busy(&g_log_req[g_current_req]);

                switch (msg->service)
                {
                    int res = OK;
                    case NET_SERVICE_UNAVAILABLE:
                    default:
                        DBG_VERBOSE("%s() msg START service %s\n", publicSignature, str_service(msg->service));
                    case NET_SERVICES_ALL: // ignored
                    break;

                    case NET_SERVICE_IFCONFIG:
                        start_etc_network_service((char*)msg->udata, true); // cast void* -> char* intf_name
                    break;

#ifdef HAVE_SNMP_SUBAGENT
                    case NET_SERVICE_SNMP:
                        /* start_snmp_service(); // 1-time only (see rcS) */
                        break;
#endif // HAVE_SNMP
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                        res = start_uap_service();
                        if (res != OK)
                        {
                            DBG_ERR(" uAP start service err %d\n", res);
                        }
                        break;
#endif // HAVE_UAP
#ifdef HAVE_WLAN
                    case NET_SERVICE_STA: // aka mlan
                        res = start_wifi_service();
                        if (res != OK)
                        {
                            DBG_ERR(" STA start service err %d\n", res);
                        }
                        break;
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS

#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                        //res = start_mdns_service(); // no delay
                        if (res != OK)
                        {
                            //DBG_ERR(" MDNS start service err %d\n", res);
                        }
                        break;
#endif // HAVE_MDNS
                }

                back_to_idle(&g_log_req[g_current_req]);

            } break;

            case NET_SERVICE_STOP:
            {

                back_to_busy(&g_log_req[g_current_req]);

                switch (msg->service)
                {
                    int res = OK;
                    case NET_SERVICE_UNAVAILABLE:
                      default:
                        DBG_VERBOSE(" msg STOP service %d passthru\n", msg->service);
                    case NET_SERVICES_ALL: // fallthrough
                    break;

                    case NET_SERVICE_IFCONFIG:
                        stop_etc_network_service((char*)msg->udata); // cast intf
                    break;

#ifdef HAVE_SNMP_SUBAGENT
                    case NET_SERVICE_SNMP:
                        break;
#endif
#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
                    case NET_SERVICE_UAP:
                        res = stop_uap_service();
                        if (res != OK)
                        {
                            DBG_ERR(" uAP Stop service err %d\n", res);
                        }

                        break;
#endif // HAVE_UAP
#ifdef HAVE_WLAN
                    case NET_SERVICE_STA: // aka mlan
                        res = stop_wifi_service();
                        if (res != OK)
                        {
                            DBG_ERR(" STA Stop service err %d\n", res);
                        }
                        break;
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS
#ifdef HAVE_MDNS
                    case NET_SERVICE_MDNS:
                        res = stop_mdns_service();
                        if (res != OK)
                        {
                            DBG_ERR(" MDNS Stop service err %d\n", res);
                        }
                        break;
#endif // HAVE_MDNS
                }

                back_to_idle(&g_log_req[g_current_req]);

            } break;

            default:
                DBG_ERR("%s() ignored %d\n", publicSignature, msg->request);
              break;
        }

        clr_service_state(msg->service, msg->request); // clear inprogress

    } // forever -- no returns

    DBG_ERR("%s() stopped?\n", publicSignature);
    DBG_ASSERT(0);
    return NULL;
}


/**************************************************/
/** \brief Initialize the net services backend    *
 *    thread that offloads major time utilizers   *
 * from the main network services responder.      *
 *                                                *
 */
void launch_net_services_backend(void) // worker thread
{
    DBG_VERBOSE("%s()..\n", __func__);
    error_type_t os_rcode = posix_mutex_init(&g_net_service_backend_mtx);
    if (os_rcode == OS_SUCCESS)
    {
        /* create thread for network services management */
        posix_create_thread(&net_services_backend_thread,
                           net_services_backend,
                           NULL,
                           "net services workq",
                          &l_backend_stack,
                          sizeof(l_backend_stack),
                          POSIX_THR_PRI_NORMAL);
    }
    else
    {
        DBG_ERR("%s() g_net_service_ctxt mutex '%s' (err %d os_rcode=0x%02x)\n", __func__, strerror(thread_errno), thread_errno, os_rcode);
        DBG_ASSERT(0);
    }
    return;
}

error_type_t net_service_workq(
    net_service_request_t request,
    net_service_t service,
    void *udata,
    void *reply)
{
    static net_service_msg_t last_req = { 0 };
    net_service_msg_t this_req = { request, service, udata, reply };

    g_all_workq_req_counter++; // overall req counter

    // how much time elapsed since previous request
           long this_req_time = os_time();
    long req_time_delta = this_req_time - g_last_req_time;

    // if this latest request == previous request within 'fold-time', throw it away
    if (req_time_delta < FOLD_SAME_REQUEST_TIME)
    {
        if (!(memcmp(&this_req, &last_req, sizeof(net_service_msg_t)))) // duplicate request?
        {
            DBG_VERBOSE("%s(%s %s) filtered..\n", __func__,
                str_request(request), str_service(request));
            g_folded_workq_req_counter++; // overall req counter
            return OK; // fold request into last one..
        }
    }
    last_req = this_req; // track last request

    net_service_msg_t buf;
    buf.request = request;
    buf.service = service;
    buf.udata   = udata;
    buf.reply   = reply;

    DBG_VERBOSE("%s(%s %s x%x)\n", __func__, str_request(request), str_service(service), udata);

    unsigned int os_rcode = posix_message_send(l_backend_msgq, (char *)&buf, sizeof(buf),
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
        DBG_ASSERT(os_rcode==OS_SUCCESS);
    }

    return NET_OK;
}

#ifdef __cplusplus
}
#endif

//eof
