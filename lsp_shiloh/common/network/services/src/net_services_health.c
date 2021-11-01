/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// Net Services Checkup routine:
//
//    Compare service enable/disable settings against running (or not) services
//  and adjust as necessary (typically periodically).    
//

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// common headers
#include "posix_ostools.h"
#include "os_system.h"
#include "oid_api.h"
#include "net_api.h" // api collection
#include "net_link_api.h" // start_ms_timer
#include "timerfd.h" // TFD_CLOEXEC

#define DBG_PRFX "net.service.checkup: "
#define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "net_debug.h"


// globals
#ifdef HAVE_WIRELESS
static bool retry_uap_intf = true;
static bool retry_sta_intf = true;
#endif


// periodic timer to check on interface/link connections
static timer_t l_net_services_health_timer;

static void net_services_checkup_cb(union sigval sig)
{
    net_services_checkup(NET_SERVICES_ALL); // query all services
}

// setup a network health checkup
int initiate_net_services_timer(long ms_delta)
{
    //noisy DBG_VERBOSE("%s(%d ms)\n", __func__, delta);
    return start_ms_timer(&l_net_services_health_timer, ms_delta, net_services_checkup_cb);
}

// restart checkup timer
int renew_net_services_timer(long ms_delta)
{
timer_t* timer = &l_net_services_health_timer;

    if (*timer) // timer initialized?
    {
        struct itimerspec time_remaining;
        struct itimerspec onetime;
        memset( &onetime, 0, sizeof(struct itimerspec) );
        memset( &time_remaining, 0, sizeof(struct itimerspec) );
        int sec_delay = ms_delta/1000L;
        onetime.it_value.tv_sec  = sec_delay;
        //periodic? .. onetime.it_interval.tv_sec = ms_delay/1000L;

        // take seconds out of nanoseconds
        long long ns_delay = ms_delta*1000000L - (sec_delay ? sec_delay*1000000000L : 0);
        onetime.it_value.tv_nsec = ns_delay;
        //periodic? .. onetime.it_interval.tv_nsec = ns_delay;

        // initiate timer
        error_type_t px_status = timer_settime(*timer, TFD_CLOEXEC, &onetime, &time_remaining);
        if (px_status != 0)
        {
            DBG_ERR("%s() timer x%x error '%s' (%d)\n", __func__, (unsigned int)timer, strerror(thread_errno), thread_errno);
            XASSERT( px_status==0, px_status );
            return thread_errno;
        }

        //noisy DBG_VERBOSE("%s() x%x restarted %d ms (@ %d)\n", __func__, (unsigned int)timer, ms_delta, posix_gettime_ticks());
    }
    else initiate_net_services_timer(ms_delta);

    return OK;
}

// periodic (or demand) network intf checkup
error_type_t net_services_checkup(net_service_t service)
{
    bool enabled = false;
    bool running = false;
#ifdef HAVE_MDNS
    static bool mdns_enabled_and_running = false;
#endif
#ifdef HAVE_UAP
    static bool uap_enabled_and_running = false;
#endif
#ifdef HAVE_WLAN
    static bool sta_enabled_and_running = false;
#endif

    //noisy DBG_VERBOSE("%s(%s) (uap %d mdns %d)\n", __func__, (service==NET_SERVICES_ALL?"all":"one"), uap_enabled_and_running, mdns_enabled_and_running);

    // for each service, checkup enabled vs running, disabled vs stopped ..
    bool check_all_services = (service == NET_SERVICES_ALL);

    switch (service)
    {
        case NET_SERVICES_ALL:

#ifdef HAVE_WIRELESS
#ifdef HAVE_UAP
        case NET_SERVICE_UAP:
        {
            enabled = wifi_uap_enabled(); // oid check
            running = wifi_uap_running(); // live check
            if (!uap_enabled_and_running)
            {
                if (enabled && !running)
                {
                    retry_uap_intf = true; // reset
                    if (wifi_uap_hw_ready()) // hw can disappear anytime
                    {
                        net_service_request(NET_SERVICE_RENEW, NET_SERVICE_UAP, NULL, NULL);
                    }
                    else
                    {
                        DBG_VERBOSE("%s() wifi/uap hw unavailable ..\n", __func__);
                        net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, NET_IF_NAME_UAP, NULL);

                        // q?  disable the interface or keep looking for a reconnect?
                        // a:  keep looking rather than uap_set_var_is_enabled(0, false); // disable intf
                    }
                }
                else
                if (!enabled && running)
                {
                    uint32_t reply = OK;
                    if (retry_uap_intf)
                        net_service_request(NET_SERVICE_STOP, NET_SERVICE_UAP, NULL, (void*)&reply); // cast
                    if (reply != OK)
                    {
                        dbg_printf("%s() uap stop err %d (x%x)\n", __func__, reply, reply); // informational
                        retry_uap_intf = false;
                    }
                }
            }
            uap_enabled_and_running = enabled & running;
        }
        if (!check_all_services) break; // fallthrough otherwise
#endif // HAVE_UAP

#ifdef HAVE_WLAN
        case NET_SERVICE_WLAN: // aka wifi/(mlan/wlan)
        {
            enabled = wifi_sta_enabled(); // oid check
            running = wifi_sta_running(); // live check
            if (!sta_enabled_and_running)
            {
                if (enabled && !running) // revisit (!enabled & running)?
                {
                    retry_sta_intf = true; // reset
                    if (wifi_sta_ready()) // ipv4 bound?
                    {
                        net_service_request(NET_SERVICE_RENEW, NET_SERVICE_STA, NULL, NULL);
                    }
                    else
                    {
                        DBG_VERBOSE("%s() wifi/sta hw unavailable ..\n", __func__);
                        net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, NET_IF_NAME_STA, NULL);

                        // q?  disable the interface or keep looking for a reconnect?
                        // a:  keep looking rather than wifi_intf_set_var_is_enabled(0, false); // disable intf
                    }
                }
                else
                if (!enabled && running)
                {
                    uint32_t reply;
                    if (retry_sta_intf)
                        net_service_request(NET_SERVICE_STOP, NET_SERVICE_STA, NULL, (void*)&reply); // cast
                    if (reply != OK)
                    {
                        dbg_printf("%s() sta stop err %d (x%x)\n", __func__, reply, reply); // informational
                        retry_sta_intf = false;
                    }
                }
            }
            sta_enabled_and_running = enabled & running;
        }
        if (!check_all_services) break; // fallthrough otherwise
#endif // HAVE_WLAN
#endif // HAVE_WIRELESS

#ifdef HAVE_MDNS
        case NET_SERVICE_MDNS:
        {
            enabled = mdns_enabled();
            running = mdns_running();
            if (!mdns_enabled_and_running)
            {
                if (!enabled && running)
                {
                    net_service_request(NET_SERVICE_STOP, NET_SERVICE_MDNS, NULL, NULL);
                }
                else
                if (enabled && !running)
                {
                    net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, NULL, NULL);
                }
            }
            mdns_enabled_and_running = enabled & running;
        }
        if (!check_all_services) break; // fallthrough otherwise
#endif // HAVE_MDNS

        default: // fallthrough
        break;
    }

    static long next_interval = 7 * 1001; // revisit tuning
    renew_net_services_timer(next_interval); // reschedule timer
    return OK;
}

#ifdef __cplusplus
}
#endif

//eof
