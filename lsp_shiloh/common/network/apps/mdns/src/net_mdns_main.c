/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
// net_mdns_main.c -- provides mdns startup support via net_mdns_init()

#ifdef __cplusplus
extern "C" {
#endif

// os headers
#include <stdio.h>
#include <string.h>

// common headers
#include "oid_api.h"
#include "net_api.h"
#include "net_mdns_api.h"
#include "net_mdns_vars.h" // net_mdns_vars_init()
#include "net_conf_api.h" // DEFAULT_PLATFORM_HOSTNAME
#include "os_system.h"
#include "net_mdns_config.h"

// Debug configuration
#define DBG_PRFX "net.mdns: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h"    // DBG_ macro family

#define USE_POSIX_TIMER 1
#define USE_DELAY_TIMER ~USE_POSIX_TIMER /* preferred */

// globals
timer_t defer_mdns_service_restart_timer;

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

static pthread_mutex_t g_net_mdns_context_mutex;

#define MDNS_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_lock(&g_net_mdns_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

#define MDNS_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode; \
            os_rcode = pthread_mutex_unlock(&g_net_mdns_context_mutex); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
        } 

// mdns_enabled() -- queries oid, returns true or false
bool mdns_enabled(void)
{
    uint32_t mdns_enabled = 0;
    oid_get_uint32(OID_NETWORK_BONJOUR_ENABLED, 0, &mdns_enabled); // 1st-index
    return (mdns_enabled==0 ? false : true);
}

bool mdns_running(void) // true/false running or not-running -- os-determined
{
    static bool running_last_query = false;
    bool running = (bool) os_system("test -z \"$(ps | grep -v grep | grep mDNSResponderPosix)\"");
    if ((running_last_query != running))
    {
        DBG_VERBOSE("%s() %s\n", __func__, (running?"up":"DOWN"));
    }
    running_last_query = running;
    return running;
}

#if 1 // using runtime mDNS Service (daemon mDNSResponderPosix)

//===========================================================================================================================
//      mdns_startup
//===========================================================================================================================

void mdns_startup(uint8_t intf_enum)
{
    MDNS_LOCK_CONTEXT();
    bool enabled = mdns_enabled();

#ifdef DBG_VERBOSE_ON
    char service_name[NET_MDNS_SERVICE_NAME_SIZE];
    oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, intf_enum, service_name, NET_MDNS_SERVICE_NAME_SIZE);
    DBG_VERBOSE("%s service name '%s' %s\n", __func__, service_name, (enabled?"enabled":"disabled"));
#endif

    if (enabled) {
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_START, NET_SERVICE_MDNS, NULL, NULL);
#else
	start_mdns_service();
#endif
    }

    MDNS_UNLOCK_CONTEXT();

    return;
}


// restart_mdns_service_cb() -- restart only if enabled
#if USE_POSIX_TIMER
static void restart_mdns_service_cb(union sigval sigval_ignored)
#else
void restart_mdns_service_cb(void* ignored)
#endif
{
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_RENEW, NET_SERVICE_MDNS, NULL, NULL);
#else
    bool enabled = mdns_enabled();
    DBG_VERBOSE("%s() %senabled\n", __func__, (enabled?"":"NOT ")); 
    if (enabled)
    {
        start_mdns_service();
    }
#endif
}

// activate restart timer (debounce multiple requests)
error_type_t activate_mdns_restart_timer(unsigned long ms_delay)
{
#if USE_POSIX_TIMER // use discrete posix timer
    timer_t* timer = &defer_mdns_service_restart_timer;
    // ignore if any remaining time on current timer
    if (timer && *timer)
    {
        struct itimerspec time_left;
        timer_gettime(*timer, &time_left);
        long still_ticking = time_left.it_value.tv_nsec + time_left.it_value.tv_sec;
        if (still_ticking > 0)
        {
            DBG_VERBOSE("%s(%d) remaining (%d,%d) (now %d)\n", __func__, ms_delay, time_left.it_value.tv_sec, time_left.it_value.tv_nsec, posix_gettime_ticks());
            // ignore reset if current timer fits within ms_delay
            return OK;
        }
    }

    DBG_VERBOSE("%s() activating %d ms delay..\n", __func__, ms_delay);
    return start_ms_timer(timer, ms_delay, restart_mdns_service_cb);

#else // use delay timer
    delay_msec_timer_non_blocking(ms_delay, restart_mdns_service_cb, (void*)&defer_mdns_service_restart_timer, false); //cast
    return OK;
#endif
}


// stop mdns regardless, restart if enabled
//    if ms_delay == 0, use default delay
error_type_t restart_mdns_service(uint8_t intf_index, long ms_delay)
{
    error_type_t res = OK;
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_RENEW, NET_SERVICE_MDNS, NULL, NULL);
#else
    res = stop_mdns_service(); // stop service regardless
    if (ms_delay<=0) ms_delay = RESTART_MDNS_SERVICE_DEFAULT_DELAY;
    res |= activate_mdns_restart_timer(ms_delay);
#endif
    return res;
}

#else // otherwise linking static libraries..

//===========================================================================================================================
//      mdns_startup
//===========================================================================================================================
static void mdns_startup(uint8_t iface_enum)
{
    mStatus result;
    int32_t mdns_rcode = MDNS_OK;
    bool mdns_initialized = false;
    ULONG timer_ticks;

    ASSERT(!(g_mdns_ctxt[iface_enum]->started)); // if not true then state machine will stall
    ASSERT(g_mdns_ctxt[iface_enum]->sock == TM_SOCKET_ERROR); // if not true then startup will result in a resource leak

    // NOTE: mDNS_Init calls our mDNSPlatformInit routine

    //static CacheEntity gRRCache[500]; //DNSSD_TODO we are sharing this with client, may not be good.
    result = mDNS_Init(&(g_mdns_ctxt[iface_enum]->internal_mdns_ctxt),
                       &gMDNSPlatformSupport, g_mdns_ctxt[iface_enum]->RRCache, MDNS_CACHE_SIZE, mDNStrue, NULL, NULL);

    if(result != mStatus_NoError)
    {
        // clean up and exit
        DBG_ERR("mDNS_Init failed! (mStatus=%d)\n", result);
        mdns_rcode = MDNS_FAIL;
        goto error;
    }
    mdns_initialized = true;

    // create and configure network socket
    g_mdns_ctxt[iface_enum]->sock = open_socket(iface_enum);
    if(g_mdns_ctxt[iface_enum]->sock == TM_SOCKET_ERROR)
    {
        mdns_rcode = MDNS_FAIL;
        goto error;
    }

    mdns_rcode = configure_socket(iface_enum, g_mdns_ctxt[iface_enum]->sock);
    if(mdns_rcode != MDNS_OK)
    {
        goto error;
    }

    register_domain_name(iface_enum);

    mdns_rcode = register_interfaces(iface_enum);
    if(mdns_rcode != MDNS_OK)
    {
        goto error;
    }

    mdns_rcode = register_all_services(iface_enum);
    if(mdns_rcode != MDNS_OK)
    {
        goto error;
    }

    mDNSs32 mdns_time = mDNS_Execute(&(g_mdns_ctxt[iface_enum]->internal_mdns_ctxt));

    timer_ticks = mdns_get_timer_ticks(iface_enum, mdns_time);
    mdns_start_exe_timer(iface_enum, timer_ticks);

    MDNS_LOCK_CONTEXT();
    g_mdns_ctxt[iface_enum]->started = true;
    g_mdns_ctxt[iface_enum]->state = MDNS_STATE_RUNNING;
    MDNS_UNLOCK_CONTEXT();
    return;

error:
    if(mdns_initialized)
    {
        mDNS_Close(&(g_mdns_ctxt[iface_enum]->internal_mdns_ctxt));
        mdns_initialized = false;
    }
    if(g_mdns_ctxt[iface_enum]->sock != TM_SOCKET_ERROR)
    {
        tfClose(g_mdns_ctxt[iface_enum]->sock);
        g_mdns_ctxt[iface_enum]->sock = TM_SOCKET_ERROR;
    }
    if(mdns_rcode == MDNS_ECONFIG)
    {
        // non-fatal recoverable error
        MDNS_SEND_MSG(MDNS_MSG_ERROR, g_mdns_ctxt[iface_enum]);
    }
    else
    {
        // fatal unrecoverable error (mDNS will be permanently shut down)
        MDNS_SEND_MSG(MDNS_MSG_FATAL_ERROR, g_mdns_ctxt[iface_enum]);
        ASSERT(0);
    }

    return;
}


//===========================================================================================================================
//      mdns_shutdown
//===========================================================================================================================
static void mdns_shutdown(uint8_t iface_enum)
{
    MDNS_LOCK_CONTEXT();
    if(!(g_mdns_ctxt[iface_enum]->started))
    {
        MDNS_UNLOCK_CONTEXT();
        return;
    }

    g_mdns_ctxt[iface_enum]->started = false;
    g_mdns_ctxt[iface_enum]->registered = false;
    MDNS_UNLOCK_CONTEXT();

    mDNS_Close(&(g_mdns_ctxt[iface_enum]->internal_mdns_ctxt)); // sends 'goodbyes'

    // closing socket *after* mDNS_Close allows the 'goodbyes' to go out
    if(g_mdns_ctxt[iface_enum]->sock != TM_SOCKET_ERROR)
    {
        tfClose(g_mdns_ctxt[iface_enum]->sock);
        g_mdns_ctxt[iface_enum]->sock = TM_SOCKET_ERROR;
    }

    return;
}

#endif // runtime service or static libraries?

// retrieve current hostname;  if changed,
//   generate an updated config file and store in the filesystem
error_type_t net_mdns_init(void)
{
char oid_service_name[NET_MDNS_SERVICE_NAME_SIZE];

    // initialize global vars
    net_mdns_var_init(1); // 1 mdns set shared among all interfaces

    // initialize mdns oids
    oid_register_module_mdns();

    net_mdns_config(); // may use mdns oids

    // retrieve last-known bonjour service name -- pause if not-yet-available
    error_type_t res = oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, oid_service_name, sizeof(oid_service_name));
    while (res != OK)
    {
        DBG_VERBOSE("%s(): retrieving bonjour service name..\n", __func__);
        posix_sleep_ms(1000); // wait 1
        res = oid_get_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, oid_service_name, sizeof(oid_service_name));
    }
    
    // generate default 'oid' service name (if not currently defined)
    if ( !oid_service_name[0] || !strcmp(oid_service_name, "")) // update if currently NOT defined
    {
        DBG_VERBOSE("%s(): updating bonjour service -> '%s'\n", __func__, DEFAULT_PLATFORM_HOSTNAME);
        oid_set_string(OID_NETWORK_BONJOUR_SERVICE_NAME, 0, DEFAULT_PLATFORM_HOSTNAME, strlen(DEFAULT_PLATFORM_HOSTNAME)); // update long-term
        strcpy(oid_service_name, DEFAULT_PLATFORM_HOSTNAME);

        mdns_write_conf(DEFAULT_PLATFORM_HOSTNAME); // bind latest service name in conf file
    }

    register_mdns_cmds(); // register cmds

    // activate bonjour (if enabled)
    DBG_MSG("%s() starting bonjour service '%s'\n", __func__, oid_service_name);
    mdns_startup(0); // all interfaces

    return OK;
}

#ifdef __cplusplus
}
#endif

// eof
