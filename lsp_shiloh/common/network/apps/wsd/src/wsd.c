/*
 * ============================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file wsd.c
 *\brief Main starting point for WSD code.  This will initialize the main WSD 
 *       dispatcher loop, create a pool of listening threads, start the SDK,
         and then begin any registered protocols (WS-Print or WS-Scan)
 */

//--------------------------------------
// Included files
//--------------------------------------
#ifndef __linux__
#include "dprintf.h"
#include "lassert.h"
#endif

#include "logger.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "platform_api.h"
#include "checksum.h"

#include "net_wsd_api.h"
#include "net_wsd_config.h"
#define DBG_PRFX "WSD: "
#include "wsd.h"
#include "platform.h"

#include "net_iface_vars.h"
#include "net_iface.h" // net_interface_t
#include "net_intf_api.h"
#include "platform_api.h"

//--------------------------------------
// Local Definitions and Types
//--------------------------------------

#define THREAD_STACK_SIZE   PTHREAD_STACK_MIN
#define STACK_ALIGN_VALUE   8
#define FWVERSION_STR_LEN   9

#define WSD_STATE_INITIALIZING          0x10
#define WSD_STATE_SHUTDOWN_REQUESTED    0x20
#define WSD_STATE_READY                 0x40
#define WSD_STATE_RUNNING               0x80

typedef enum {
    WSD_THREAD_UNUSED,
    WSD_THREAD_IN_USE,
} wsd_thread_state_t;

typedef struct {
    bool ipv4_configured;
#ifdef HAVE_IPV6        
    bool ipv6_configured;
#endif // HAVE_IPV6     
} wsd_context_t;

//--------------------------------------
// Global Definitions
//--------------------------------------
static wsd_dpws_data_t wsd_dpws_data;
static net_wsd_config_t wsd_config_vals;
static struct dpws master_dpws;
static pthread_t wsd_thread;
static ALIGN8 uint8_t wsd_stack[ THREAD_STACK_SIZE ];
static net_iface_event_handle_t wsd_iface_handle = NULL;
static pthread_t **wsd_threads;
static uint8_t **wsd_thread_stacks;
static char **wsd_thread_names;
static wsd_thread_state_t *wsd_thread_init;
static wsd_protocol_instance_t *wsd_protocols[ WSD_NUM_PROTOCOLS ] = {0}; // use wsd_protocols_t as an index into this array
static wsd_job_info_t *wsd_job_history;
static uint32_t wsd_job_history_index = 0;
static uint32_t wsd_job_id = 123;
static wsd_context_t g_wsd_ctxt = {0};
static pthread_mutex_t wsd_ready_mutex = POSIX_MUTEX_INITIALIZER;
static pthread_cond_t wsd_ready_cond = PTHREAD_COND_INITIALIZER;
static uint32_t wsd_ready_flags = 0;
static uint32_t wsd_state_flags = 0;

// public
void wsd_set_enabled(void)
{
    wsd_ready_flags = WSD_FLAG_ENABLED;
}

bool wsd_initializing(void)
{
    return (wsd_state_flags & WSD_STATE_INITIALIZING);
}

bool wsd_ready(void)
{
    if (wsd_initializing()) return false;
    DBG_ERR("%s() initialized ..\n", __func__);
    return (wsd_state_flags & WSD_STATE_READY);
}

bool wsd_busy(void)
{
    return (wsd_state_flags & WSD_STATE_RUNNING);
}

// private
// alias dpws server restart routine
int wsd_restart_dpws_server(void)
{
    //revist int dc_reactor_init(void* reactor, DC_BOOL shared); // revisit extern cast
    //revisit int ret = dc_reactor_init((void*)&master_dpws.reactor, DC_TRUE);
    //revisit if (ret)
    //revisit {
    //revisit     DBG_ERR("%s() dc_reactor_init() err %d\n", __func__, ret);
    //revisit}
    int ret = dpws_server_init_ex( &master_dpws, NULL, DC_LISTENER_ALL, 0, 10 );
    if (ret)
    {
        DBG_ERR("%s() server_init_ex() err %d\n", __func__, ret);
    }
    return ret;
}

// Order of this array must match the wsd_job_states_t enum in wsd.h
char *wsd_job_states[ ] ={
    "Aborted",
    "Canceled",
    "Completed",
    "Creating",
    "Pending",
    "Pending-Held",
    "Processing",
    "ProcessingStopped",
    "Started",
    "Terminating",
};

char *wsd_job_state_reasons[ ] ={
    "CompressionError",
    "DocumentAccessError",
    "DocumentFormatError",
    "DocumentTimeoutError",
    "DocumentTransferError",
    "JobCanceledAtDevice",
    "JobCanceledByUser",
    "JobCompletedSuccessfully",
    "JobCompletedWithErrors",
    "JobCompletedWithWarnings",
    "JobIncoming",
    "JobPasswordWait",
    "JobPrinting",
    "None",
    "PrinterStopped",
    "UnsupportedCompression",
    "UnsupportedDocumentFormat",
    "InvalidScanTicket",
    "ImageTransferError",
    "JobScanning",
    "JobScanningAndTransferring",
    "JobTimedOut",
    "JobTransferring",
    "ScannerStopped",
};

#ifdef HAVE_DBG_PRINTF
static const char *g_net_wsd_msg_strs[ NET_WSD_NUM_MSGS ] ={
    "NET_WSD_PRINTER_STATUS_SUMMARY",
    "NET_WSD_PRINTER_ELEMENTS_CHANGE",
    "NET_WSD_PRINTER_STATUS_CONDITION",
    "NET_WSD_JOB_STATUS",
    "NET_WSD_JOB_END_STATE",
};
#endif // HAVE_DBG_PRINTF

//--------------------------------------
// Local Function Declarations
//--------------------------------------
static int start_dpws(unsigned int if_index);
void wsd_custom_settings(int16_t device);
static void* wsd_main(void* input);
static void wsd_network_event_cb( net_iface_event_flags_t events, void *context );
static void* wsd_serve_request(void* input);
static int wsd_boot_server(struct dpws *dpws);
static uint32_t wsd_calc_meta_data_checksum(void);

//--------------------------------------
// Local Function Definitions
//--------------------------------------

void net_wsd_init(void)
{
    unsigned int posix_res;

    posix_res = posix_create_thread(&wsd_thread, wsd_main, 0, "WSD",
            wsd_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT(posix_res == 0, posix_res);

#ifdef __linux__
    //  per interface initialization?
#else // threadx
    tx_res = tx_event_flags_create( &wsd_ready_flags, "wsd_ready_flags" );
    XASSERT( TX_SUCCESS == tx_res, tx_res );
#endif // threadx
}

void wsd_register_protocol(wsd_protocols_t protocol, wsd_func_table_t *func_table)
{
    wsd_protocol_instance_t *proto;

    proto = (wsd_protocol_instance_t *) MEM_MALLOC(sizeof ( wsd_protocol_instance_t));
    if (!proto) {
        DBG_ERR("error allocating mem for wsd_protocol_instance_t\n");
        return;
    }
    proto->func_table = *func_table;
    //DBG_ASSERT((protocol == WSD_PRINT) || (protocol == WSD_SCAN));
    proto->type = protocol;

    wsd_protocols[ protocol ] = proto;
}

static error_type_t wsd_startup(void)
{
    uint8_t i;
    uint8_t j;
    unsigned int tx_res;

    XASSERT(wsd_config_vals.num_dispatcher_threads < 99, wsd_config_vals.num_dispatcher_threads); // Otherwise the strlen below will not allocate enough memory

    wsd_threads = MEM_MALLOC(sizeof ( pthread_t *) * wsd_config_vals.num_dispatcher_threads);
    if (wsd_threads == NULL) {
        return FAIL;
    }
    wsd_thread_names = MEM_MALLOC(sizeof ( char *) * wsd_config_vals.num_dispatcher_threads);
    if (wsd_thread_names == NULL) {
        MEM_FREE_AND_NULL(wsd_threads);
        return FAIL;
    }

    wsd_thread_init = MEM_MALLOC(sizeof ( wsd_thread_state_t) * wsd_config_vals.num_dispatcher_threads);
    if (wsd_thread_init == NULL) {
        MEM_FREE_AND_NULL(wsd_thread_names);
        MEM_FREE_AND_NULL(wsd_threads);
        return FAIL;
    }

    wsd_thread_stacks = MEM_MALLOC(sizeof ( uint8_t *) * wsd_config_vals.num_dispatcher_threads);
    if (wsd_thread_stacks == NULL) {
        MEM_FREE_AND_NULL(wsd_thread_names);
        MEM_FREE_AND_NULL(wsd_thread_init);
        MEM_FREE_AND_NULL(wsd_threads);
        return FAIL;
    }

    for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++) {
        wsd_thread_init[ i ] = WSD_THREAD_UNUSED;
        wsd_thread_names[ i ] = (char *) MEM_MALLOC(strlen("wsd_thread_XXX")); // XXX is 2 digits for thread index and 1 for null char
        if (wsd_thread_names[ i ] == NULL) {
            for (j = 0; j < i; j++) {
                MEM_FREE_AND_NULL(wsd_thread_names[ j ]);
                MEM_FREE_AND_NULL(wsd_thread_stacks[ j ]);
                MEM_FREE_AND_NULL(wsd_threads[ j ]);
            }
            MEM_FREE_AND_NULL(wsd_thread_names);
            MEM_FREE_AND_NULL(wsd_thread_init);
            MEM_FREE_AND_NULL(wsd_threads);
            MEM_FREE_AND_NULL(wsd_thread_stacks);
            return FAIL;
        }
        minSprintf(wsd_thread_names[ i ], "wsd_thread_%02d", i);
        wsd_thread_stacks[ i ] = MEM_MALLOC_ALIGN(THREAD_STACK_SIZE, STACK_ALIGN_VALUE);
        if (wsd_thread_stacks[ i ] == NULL) {
            for (j = 0; j < i; j++) {
                MEM_FREE_AND_NULL(wsd_thread_names[ j ]);
                MEM_FREE_AND_NULL(wsd_thread_stacks[ j ]);
                MEM_FREE_AND_NULL(wsd_threads[ j ]);
            }
            MEM_FREE_AND_NULL(wsd_thread_names[ i ]);
            MEM_FREE_AND_NULL(wsd_thread_names);
            MEM_FREE_AND_NULL(wsd_thread_init);
            MEM_FREE_AND_NULL(wsd_threads);
            MEM_FREE_AND_NULL(wsd_thread_stacks);
            return FAIL;
        }

        wsd_threads[ i ] = MEM_MALLOC(sizeof ( pthread_t));
        if (wsd_threads[ i ] == NULL) {
            for (j = 0; j < i; j++) {
                MEM_FREE_AND_NULL(wsd_thread_names[ j ]);
                MEM_FREE_AND_NULL(wsd_thread_stacks[ j ]);
                MEM_FREE_AND_NULL(wsd_threads[ j ]);
            }
            MEM_FREE_AND_NULL(wsd_thread_names[ i ]);
            MEM_FREE_AND_NULL(wsd_thread_stacks[ i ]);
            MEM_FREE_AND_NULL(wsd_thread_names);
            MEM_FREE_AND_NULL(wsd_thread_init);
            MEM_FREE_AND_NULL(wsd_threads);
            MEM_FREE_AND_NULL(wsd_thread_stacks);
            return FAIL;
        }
        memset(wsd_threads[ i ], 0x00, sizeof ( pthread_t));
    }

    wsd_job_history = MEM_MALLOC(wsd_config_vals.num_job_history_elements * sizeof ( wsd_job_info_t));
    if (wsd_job_history == NULL) {
        for (j = 0; j < wsd_config_vals.num_dispatcher_threads; j++) {
            MEM_FREE_AND_NULL(wsd_thread_names[ j ]);
            MEM_FREE_AND_NULL(wsd_thread_stacks[ j ]);
            MEM_FREE_AND_NULL(wsd_threads[ j ]);
        }
        MEM_FREE_AND_NULL(wsd_thread_names);
        MEM_FREE_AND_NULL(wsd_thread_init);
        MEM_FREE_AND_NULL(wsd_threads);
        MEM_FREE_AND_NULL(wsd_thread_stacks);
        return FAIL;
    }

    ASSERT(wsd_job_history != NULL);
    memset(wsd_job_history, 0x00, wsd_config_vals.num_job_history_elements * sizeof ( wsd_job_info_t));
    wsd_job_history_index = 0;

#ifdef __linux__
    tx_res = posix_mutex_init(&wsd_ready_mutex);
#else // threadx
    tx_res = tx_mutex_create( &wsd_ready_mutex, "wsd_ready_mutex", 1 );
#endif // threadx
    XASSERT( OS_SUCCESS == tx_res, tx_res );

    XASSERT(tx_res == 0, tx_res);

    for (i = 0; i < WSD_NUM_PROTOCOLS; i++) {
        if (wsd_protocols[ i ] != NULL) {
            wsd_protocols[ i ]->func_table.startup();
        }
    }

    wsd_register_directed_probe(); // register w/http

    wsd_set_flags(NULL, WSD_STATE_READY);
    DBG_VERBOSE("%s() ready\n", __func__);

    return OK;
}

static void wsd_idle(void)
{
    DBG_VERBOSE("%s() ..\n", __func__);
    int i;
    for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++)
    {
        if (wsd_thread_init[ i ] == WSD_THREAD_IN_USE)
        {

             OS_THREAD* worker = wsd_threads[i]; // wsd_thread_init[i];
             DBG_VERBOSE("%s() thread %#x busy\n", __func__, worker);

             wsd_protocols[ i ]->func_table.shutdown(); // free 
        }
    }

    wsd_state_flags &= ~WSD_STATE_RUNNING; // idling
    XASSERT(wsd_state_flags & (WSD_STATE_INITIALIZING | WSD_STATE_READY), wsd_state_flags);

    wsd_clear_flags(NULL, wsd_ready_flags);
}

static void wsd_shutdown(void)
{
    uint8_t i;

    wsd_clear_flags(NULL, WSD_STATE_READY);

    for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++)
    {
        if (wsd_thread_init[ i ] == WSD_THREAD_IN_USE)
        {
            pthread_join(*(wsd_threads[ i ]),NULL);
        }

        MEM_FREE_AND_NULL(wsd_thread_names[ i ]);
        MEM_FREE_AND_NULL(wsd_thread_stacks[ i ]);
        MEM_FREE_AND_NULL(wsd_threads[ i ]);
    }
    MEM_FREE_AND_NULL(wsd_threads);
    MEM_FREE_AND_NULL(wsd_thread_stacks);
    MEM_FREE_AND_NULL(wsd_thread_names);
    MEM_FREE_AND_NULL(wsd_thread_init);

    MEM_FREE_AND_NULL(wsd_job_history);
    pthread_mutex_destroy(&wsd_ready_mutex);

    wsd_deregister_directed_probe(); // unregister with http

    for (i = 0; i < WSD_NUM_PROTOCOLS; i++) {
        if (wsd_protocols[ i ] != NULL) {
            wsd_protocols[ i ]->func_table.shutdown();
        }
    }
    DBG_VERBOSE("%s() done\n", __func__);
}

// compare expected oem interfaces against current os-inventory and return a bag of if_index values
//    e.g, bag[0] = 2, bag[1..n] = 0 (for a single interface at if_index == 2)
//         bag[0] = 2, bag[1] = 17, bag[2..n] (for two interfaces at if_index==2 and if_index==17)
//         etc..
//
//    (revisit alternative -- use if_nameindex() to generate currently known os-interface list)
//
//static unsigned int bag[NET_MAX_INTERFACES];
typedef unsigned int bagof_if_index_t;
int bagof_if_index(bagof_if_index_t* bag, int bag_byte_size)
{
    unsigned int if_index = 0;
    int active_intf = 0;
    if (bag && bag_byte_size>0)
    {
        memset(bag, 0, bag_byte_size);
#ifdef __linux__
        int number_of_oem_intf = net_link_enum_get_count() + 1; // runtime binding 1:1 w/links
        int link_index;
        for (link_index=1; link_index<number_of_oem_intf; link_index++) // 1-based link_index
        {
            net_link_t* link = net_link_lookup_by_link_index(link_index);
            if (link)
            {
                net_interface_t* intf = net_iface_lookup_by_name(link->name); // oem-specific
                if_index = if_nametoindex(intf->name);
                if (intf && if_index > 0) // intf known to os?
                {
                    DBG_VERBOSE("%s() identified active intf %s if_index %d\n", __func__, intf->name, if_index);
                    bag[active_intf++] = if_index;
                } // (dynamic) interface
                if (active_intf > bag_byte_size) break; // don't overflow given bag
            } // link
        }
#else // ! __linux__
        // TODO must account for multiple simultaneous interfaces
        if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);
        if (!if_index) {
            // interface has not been added
            return 0;
        }
#endif // threadx
    }
    
    DBG_VERBOSE("%s() found %d active intf%s\n", __func__, active_intf, active_intf==1?"":"s");
    return active_intf;
}

static void* wsd_main(void* input)
{
    unsigned long requested_flags = WSD_FLAG_ENABLED | WSD_FLAG_IP_CONFIGURED; // initial wait criteria
    unsigned long actual_flags;

    /* Wait for all levels to start */
    SysWaitForInit();

    net_wsd_var_init();

    net_wsd_config( &wsd_config_vals );

    // register an event listener for each network interface of interest
#ifdef __linux__
    // how many network interfaces currently available?
    int number_of_oem_intf = net_link_enum_get_count() + 1; // runtime binding 1:1 w/links (1-based)
    int link_index;
    for (link_index=1; link_index<number_of_oem_intf; link_index++) // 1-based link_index
    {
        net_link_t* link = net_link_lookup_by_link_index(link_index);
        if (link)
        {
            // wsd over selective interfaces..
            if (!strcmp(link->name, NET_ETH_LINK_NAME) || !strcmp(link->name, NET_STA_LINK_NAME))
            {
                net_interface_t* intf = net_iface_lookup_by_name(link->name); // oem-specific
                unsigned int if_index = if_nametoindex(intf->name);
                if (intf && if_index > 0) // intf known to os?
                {
                    wsd_iface_handle = net_iface_register_events(if_index,
                         NET_IFACE_EVENT_LINK_STATUS_CHANGE
                       | NET_IFACE_EVENT_IPV4_CONFIG_CHANGE
#ifdef HAVE_IPV6
                       | NET_IFACE_EVENT_IPV6_CONFIG_CHANGE
#endif
                        , wsd_network_event_cb,
                        (void *) intf);
                    DBG_VERBOSE("%s() registering wsd_handle %#x intf %s if_index %d\n", __func__, wsd_iface_handle, intf->name, if_index, intf, wsd_iface_handle);
                } // (dynamic) interface
            } // wsd-specific interface
        } // link
    }
#else // ! __linux__
    // TODO must account for multiple simultaneous interfaces
    if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    if (!if_index) {
        // interface has not been added
        return 0;
    }
#endif // threadx

    // Create wsd app threads & stacks
    while (wsd_startup() != OK)
    {
        posix_sleep_ticks(200);
        DBG_VERBOSE("\n%s() retry..\n", __func__);
    }

    // forever wait on wsd interface events of interest
    while (1)
    {
        requested_flags = WSD_FLAG_ENABLED | WSD_FLAG_IP_CONFIGURED; // reset wait-event criteria

        wsd_state_flags = WSD_STATE_INITIALIZING;

#ifdef __linux__

        while (!(wsd_ready_flags & requested_flags)) {
            posix_sleep_ticks(200);
            pthread_cond_wait(&wsd_ready_cond, &wsd_ready_mutex);
        }
        actual_flags = wsd_ready_flags; // latch

        wsd_clear_flags(NULL, WSD_FLAG_ENABLED);

#else // threadx
        tx_event_flags_get( &wsd_ready_flags, requested_flags, TX_AND, &actual_flags, TX_WAIT_FOREVER);
#endif // threadx

        if (actual_flags & WSD_FLAG_ENABLED) // wsd enabled?
        {
            wsd_state_flags |= WSD_STATE_INITIALIZING;

            // any available wsd-of-interest interfaces?
            {
                bagof_if_index_t if_index_bag[NET_MAX_INTERFACES];
                int number_of_active_intfs = bagof_if_index(&if_index_bag[0], sizeof(if_index_bag)/sizeof(bagof_if_index_t));

                DBG_VERBOSE("%s() start %d interface%s\n", __func__, number_of_active_intfs, number_of_active_intfs==1?"":"s");

                int if_offset = 0;
                unsigned int if_index = (unsigned int)*(if_index_bag+if_offset++);
                while (if_index > 0)
                {
                    DBG_VERBOSE("Calling start_dpws (if_index %d)\n", if_index);
                    int error = start_dpws(if_index); // zero on success
                    if_index = *(if_index_bag+if_offset++);
                    if (!error)
                    {
                        if_index = 0; // only 1 interface required to start dpws -- done
                    }
                    else
                    {
                        dpws_stop_server(1); // BYE blocking w/nominal delay
                    }

                    dpws_shutdown();
                }

                wsd_deregister_directed_probe(); // unregister w/http
                wsd_idle();
            }
        }
        else // disabled, needs a shutdown?
        {
            DBG_VERBOSE("%s() disabled..\n", __func__);
        }
        posix_sleep_ms(500); // Prevent runaway thread if fail during init
    }

    wsd_shutdown(); // free wsd app resources

    return 0;
}

/* Function printing information related to a device. */
static void print_device_info(short hrefDevice)
{
    int len = 10, sz, i, j;
    short hrefs[10], hrefSP;
    char ** transportAddrs = NULL;

    if (hrefDevice < 1) return; // device data?
    if (!wsd_ready()) return; // service settled?

    // One service port (HTTP binding) created by default but potentially multiple
    // local addresses (multiple network interfaces & IP versions)
    hrefSP = dpws_get_default_service_port(hrefDevice);
    transportAddrs = dpws_get_transport_addresses(hrefSP, &sz);
    dpws_release_handle(hrefSP);

    DBG_MSG("\n+ Device [%s] started\n\nAddresses:\n", (char *)dpws_get_ptr_att(hrefDevice, DPWS_STR_DEVICE_ID));
    for (i = 0; i < sz; i++)
    {
        DBG_MSG("[%d] %s\n", i, transportAddrs[i]);
        dpws_free(transportAddrs[i]);	// because of dpws_get_transport_addresses
    }
    dpws_free(transportAddrs);	// because of dpws_get_transport_addresses

    if (!dpws_get_service_handles(hrefDevice, hrefs, &len))
    {
        DBG_MSG("\n++ Hosted services:\n\n");
        for (i = 0; i < len; i++)
        {
            hrefSP = dpws_get_default_service_port(hrefs[i]);
            transportAddrs = dpws_get_transport_addresses(hrefSP, &sz);	// same as for device
            dpws_release_handle(hrefSP);
            DBG_MSG("Service ID: %s\nAddresses:\n", (char *)dpws_get_ptr_att(hrefs[i], DPWS_STR_SERVICE_ID));
            for (j = 0; j < sz; j++)
            {
                DBG_MSG("[%d] %s\n", j, transportAddrs[j]);
                dpws_free(transportAddrs[j]);	// because of dpws_get_transport_addresses
            }
            dpws_free(transportAddrs);	// because of dpws_get_transport_addresses
        }
        dpws_release_handles(hrefs, len);
    }
}

// public
void show_wsd_device(void)
{
    print_device_info(wsd_dpws_data.device);
}

static int start_dpws(unsigned int if_index)
{
    int32_t status;
    uint32_t boot_seq;
    int ret;
    uint16_t i;
    uint32_t prev_checksum;
    uint32_t cur_checksum;
    struct localized_string loc_str = {NULL, NULL};
    char * friendly_name;
    char hostname[ NET_IFACE_HOSTNAME_SIZE ];
    uint16_t device_model;
    uint16_t device;
    int16_t service;
    uint32_t meta_version;
    int16_t service_port;
    int16_t service_class[ WSD_NUM_PROTOCOLS ] = {0};
    char *service_name[ WSD_NUM_PROTOCOLS ] = {"printer"};
    char uuid[ WSD_UUID_LENGTH ];

    DBG_VERBOSE("%s()==>\n", __func__);

    memset(&wsd_dpws_data, -1, sizeof ( wsd_dpws_data));


    dc_ip_filter_t ip_selector = {
        NULL, // consider all interfaces
        DC_FALSE, // but not loopback
        0, // Initialize to empty then update below
        0, // no address filtering
        NULL // no address filtering
    };

    DBG_VERBOSE("Initialize DPWSCore\n");

#ifdef HAVE_IPV6
    bool intf_enabled = false;
    bool ipv4_enabled;
    bool ipv6_enabled;
    net_iface_get_var_intf_enabled(if_index, &intf_enabled);
    DBG_VERBOSE("%s() intf %d enabled %d?\n", __func__, if_index, intf_enabled);
    net_iface_get_var_ipv4_enabled(if_index, &ipv4_enabled);
    net_iface_get_var_ipv6_enabled(if_index, &ipv6_enabled);
    if (ipv4_enabled && ipv6_enabled) {
        ip_selector.proto = DC_PROTO_ANY; // dual IPv4/v6
    } else if (ipv4_enabled) {
        ip_selector.proto = DC_PROTO_INET; // IPv4 only
    } else {
        ip_selector.proto = DC_PROTO_INET6; // IPv6 only
    }
#else   // HAVE_IPV6
    ip_selector.proto = DC_PROTO_INET; // IPv4 only
#endif  // HAVE_IPV6

    //ip_selector.proto = DC_PROTO_ANY; // dual IPv4/v6
    if ( net_iface_get_var_hostname( if_index, hostname, NET_IFACE_HOSTNAME_SIZE ) != OK )
    {
        DBG_ASSERT(0);
        return 1;
    }

    if ((status = dpws_init_ex(&ip_selector, hostname, DPWS_DEFAULT_VERSION))) // Can add hostname as 2nd parameter if desired
    {
        DBG_ERR("Failed to initialize DSPWCore stack - error: %d\n", status);
        return 1;
    }

    platvars_get_powerup_counter(&boot_seq);

    DBG_VERBOSE("Setting boot sequence and port number\n");
    ret = DPWS_SET_INT_ATT(DC_REGISTRY_HANDLE, DPWS_INT_BOOT_SEQ, boot_seq);
    XASSERT(ret == 0, ret);

    // Configure the HTTP listen port.
    ret = DPWS_SET_INT_ATT(DC_REGISTRY_HANDLE, DPWS_INT_HTTP_PORT, wsd_config_vals.port);
    XASSERT(ret == 0, ret);

    // Device instance data that will be accessible in the service implementation
    wsd_dpws_data.event_rate = wsd_config_vals.default_event_rate;

    DBG_VERBOSE("Calling dpws_client_init\n");
    dpws_client_init(&wsd_dpws_data.dpws, NULL);
  
    wsd_get_var_meta_data_checksum(&prev_checksum);

    if ( platvars_get_prtnamestr( &loc_str.s ) != OK )
    {
        DBG_ASSERT(0);
        return 1;
    }

#define WORKING_STR_BUFFER_SIZE 256
    friendly_name = (char *) MEM_MALLOC(WORKING_STR_BUFFER_SIZE);

    if ( friendly_name == NULL )
    {
        DBG_ERR("Malloc failed for friendly name\n");
        return 1;
    }
    sprintf( friendly_name, "%s (%s)", hostname, loc_str.s );

    // Create print device model
    device_model = dpws_create_device_model();

    // Configure the mandatory device model attributes
    ret = DPWS_SET_STR_ATT(device_model, DPWS_PTR_MODEL_NAME, &loc_str);
    XASSERT(ret == 0, ret);

    loc_str.s = DEFAULT_MFG_STRING;

    ret = DPWS_SET_STR_ATT(device_model, DPWS_PTR_MANUFACTURER, &loc_str);
    XASSERT(ret == 0, ret); 

    // Configure the optional device model attributes
    if (platvars_get_usb_prodstr(&loc_str.s) != OK)
    {
        DBG_ASSERT(0);
        return 1;
    }

    ret = DPWS_SET_STR_ATT(device_model, DPWS_STR_MODEL_NUMBER, loc_str.s);
    ret = DPWS_SET_STR_ATT(device_model, DPWS_STR_MODEL_URL, wsd_config_vals.support_url);
    ret = DPWS_SET_STR_ATT(device_model, DPWS_STR_PRESENTATION_URL, "");
    ret = DPWS_SET_STR_ATT(device_model, DPWS_STR_MANUFACTURER_URL, wsd_config_vals.manufacturer_url);

    // Create device, configure hosted service & enable device
    for (i = 0; i < WSD_NUM_PROTOCOLS; i++) {
        if (wsd_protocols[ i ] != NULL) {
            service_class[ i ] = wsd_protocols[ i ]->func_table.dpws_service_class(device_model);
            DBG_VERBOSE("Calling dpws_service_class function of wsd_protocols i =%d\n", i);
        }
    }

    device = dpws_create_device(0, device_model);
    dpws_release_handle(device_model);

    wsd_custom_settings( device );

    for (i = 0; i < WSD_NUM_PROTOCOLS; i++) {
        if (wsd_protocols[ i ] != NULL) {
            wsd_protocols[ i ]->func_table.dpws_device_type(device);
            DBG_VERBOSE("Calling dpws_device type function of wsd_protocols i =%d\n", i);
        }
    }

    if (wsd_get_var_meta_version(&meta_version) != NET_WSD_SUCCESS) {
        DBG_ERR("Failed to get meta_version\n");
        meta_version = 0;
    }
    cur_checksum = wsd_calc_meta_data_checksum();
    if ( prev_checksum != cur_checksum )
    {
        DBG_VERBOSE("Updating MetaVersion\n");
        meta_version++;
        wsd_set_var_meta_version(meta_version);
        wsd_set_var_meta_data_checksum(cur_checksum);
    }
    ret = DPWS_SET_INT_ATT(device, DPWS_INT_METADATA_VERSION, meta_version);
    XASSERT(ret == 0, ret);
    ret = DPWS_SET_STR_ATT(device, DPWS_PTR_FRIENDLY_NAME, &loc_str);
    XASSERT(ret == 0, ret);

    net_wsd_get_var_uuid(uuid, WSD_UUID_LENGTH);
    DBG_VERBOSE("%s() uuid %s\n", __func__, uuid);
    ret = DPWS_SET_STR_ATT(device, DPWS_STR_DEVICE_ID, uuid);
    XASSERT(ret == 0, ret);

    // Configure the optional device attributes
    if ( platvars_get_fw_version( &loc_str.s ) != OK ) 
    {
        DBG_ASSERT(0);
        return 1;
    }

    ret = DPWS_SET_STR_ATT(device, DPWS_STR_FIRMWARE_VERSION, loc_str.s);
    XASSERT(ret == 0, ret);
    if (platvars_get_prod_serialnum(&loc_str.s) != OK) {
        DBG_ASSERT(0);
        return 1;
    }

    ret = DPWS_SET_STR_ATT(device, DPWS_STR_SERIAL_NUMBER, loc_str.s);
    XASSERT(ret == 0, ret);
    wsd_dpws_data.device = device;

    // finished string retrieval
    MEM_FREE_AND_NULL(friendly_name);

    for (i = 0; i < WSD_NUM_PROTOCOLS; i++)
    {
        DBG_VERBOSE("%s() wsd_protocols[%d] x%x\n", __func__, i, wsd_protocols[i]);
        if ( wsd_protocols[ i ] != NULL )
        {
            service = dpws_get_service_by_class(device, service_class[ i ]);
            XASSERT(service != -1, service);
            wsd_dpws_data.event_source[ i ] = service;
            service_port = dpws_get_default_service_port(service);
            DBG_VERBOSE("%s(%d) service %#x port %#x\n", __func__, i, service, service_port);
            XASSERT(service_port != -1, service_port);
            dpws_release_handle(service);
            ret = DPWS_SET_STR_ATT(service_port, DPWS_STR_ADDRESS, service_name[i]);
            XASSERT(ret == 0, ret);
            dpws_release_handle(service_port);
        }
    }
    
    if ((status = dpws_enable_device(device))) // Enables device (especially schedule hello messages).
    {
        DBG_ERR("Could not execute dpws_enable_device (err %d)\n", status);
        return 1;
    }
    
    dpws_release_handle(device);

    // initializes the dpws structure for server-side operations.
    if ( ( status = dpws_server_init_ex( &master_dpws, NULL, DC_LISTENER_ALL, 0, 10 ) ) )
    {
        DBG_ERR("Could not initialize DPWS server (err %d %s)\n", status, (char*) dpws_get_error_msg(&master_dpws));
        wsd_restart_dpws_server();
        dpws_server_shutdown(&master_dpws);
        return 1;
    }

    // Enabled Keep alive
    if ( wsd_config_vals.transactions_per_socket > 1 )
    {
        master_dpws.soap.imode |= SOAP_IO_KEEPALIVE;
        master_dpws.soap.omode |= SOAP_IO_KEEPALIVE;
        master_dpws.soap.max_keep_alive = wsd_config_vals.transactions_per_socket;
    }

    // Add MTOM support for print jobs
    master_dpws.soap.imode |= SOAP_ENC_MTOM | SOAP_MIME_POSTCHECK; // SOAP_IO_CHUNK | 
    master_dpws.soap.omode |= SOAP_ENC_MTOM; // SOAP_IO_FLUSH | 

#ifdef __linux__
    DBG_ASSERT(wsd_protocols[WSD_PRINT]);
    if (wsd_protocols[WSD_PRINT])
    {
        master_dpws.soap.fmimewriteopen = wsd_protocols[ WSD_PRINT ]->func_table.mime_write_open;
        master_dpws.soap.fmimewrite = wsd_protocols[ WSD_PRINT ]->func_table.mime_write;
        master_dpws.soap.fmimewriteclose = wsd_protocols[ WSD_PRINT ]->func_table.mime_write_close;
    }
#endif

    if ( (ret = wsd_boot_server( &master_dpws )) != 0 ) 
    {
        DBG_ERR("Could not boot server (err %d)...\n", ret);
        return ret;
    }

    DBG_VERBOSE("%s()<==\n", __func__);
    
    return 0;
}

// return boot_seq counter
static uint32_t wsd_interface_dropped(net_interface_t* intf)
{
    uint32_t boot_seq = -1;
    platvars_get_powerup_counter(&boot_seq);

    DBG_VERBOSE("%s() boot_seq counter %d\n", __func__, boot_seq);

    /*  Interface just went down (i.e. either ipv4 just went down and ipv6 not configured or ipv6 just went
     *  down and ipv4 not configured).  Notify the dpws state machine.
     */

    if (intf)
    {
        DBG_ERR("Interface %s is now down\n", intf->name); // VERBOSE
        if (wsd_state_flags & WSD_STATE_INITIALIZING) {
            DBG_ERR("Interface down while still initializing - scheduling shutdown\n");
            wsd_state_flags |= WSD_STATE_SHUTDOWN_REQUESTED;
            wsd_ready_flags |= WSD_STATE_SHUTDOWN_REQUESTED;
        }

        platvars_inc_powerup_counter();
        platvars_get_powerup_counter(&boot_seq);

        DBG_VERBOSE("\n%s() join %s intf\n", __func__, intf->name);
        int ret = dpws_join_network(boot_seq); // restart dpws advertisements ..
        if (ret)
        {
            DBG_ERR("\n  %s NOT wsd-enabled (err %d)\n", intf->name, ret);
        }

        wsd_clear_flags(intf, WSD_FLAG_IP_CONFIGURED);
    }

    return boot_seq;
}

static void wsd_network_event_cb(net_iface_event_flags_t events, void *context)
{
    if (!context) return;
    net_interface_t* iface = (net_interface_t*) context; // cast
    if (!iface) return;
    unsigned int if_index = if_nametoindex(iface->name);
    DBG_VERBOSE("%s %s(event %#x if_index %d)\n", __func__, iface->name, events, if_index);
    if (if_index<1) return; // inactive intf?

    uint32_t boot_seq;  
    dc_ip_filter_t ip_selector = {
        NULL, //&netif, // consider all interfaces
        DC_FALSE, // but not loopback
        DC_PROTO_ANY, // Initialize to IPV4 and IPV6
        0, // no address filtering
        NULL // no address filtering
    };

    DBG_MSG("%s() %s event %d\n", __func__, iface->name, events);

    bool ipv4_prev = g_wsd_ctxt.ipv4_configured;
    if (events & NET_IFACE_EVENT_IPV4_CONFIG_CHANGE) {
        net_iface_get_var_ipv4_configured(if_index, &g_wsd_ctxt.ipv4_configured);
        DBG_ERR("%s() %s -> v4(%d)\n", __func__, iface->name, g_wsd_ctxt.ipv4_configured);
    }

#ifdef HAVE_IPV6
    bool ipv6_prev = g_wsd_ctxt.ipv6_configured;
    if (events & NET_IFACE_EVENT_IPV6_CONFIG_CHANGE) {
        net_iface_get_var_ipv6_configured(if_index, &g_wsd_ctxt.ipv6_configured);
        DBG_ERR("%s() %s -> v6(%d)\n", __func__, iface->name, g_wsd_ctxt.ipv6_configured);
    }
#endif // HAVE_IPV6

    // up or down?
    if (events & NET_IFACE_EVENT_LINK_STATUS_CHANGE)
    {
        DBG_VERBOSE("%s() link %s change %d (ENABLED %d? INITIALIZING %d?)\n", __func__,
            iface->name,
                wsd_ready_flags,
                WSD_FLAG_ENABLED == (wsd_ready_flags & WSD_FLAG_ENABLED)? 1: 0,
                WSD_STATE_INITIALIZING == (wsd_ready_flags & WSD_STATE_INITIALIZING)? 1 : 0);

        /* Check if logical link is up */
        net_link_status_t link_status;
        ASSERT(iface->link_handle != (net_link_handle_t)NET_LINK_INVALID_HANDLE);
        link_status = net_link_get_status(iface->link_handle);
        if( link_status != NET_LINK_STATUS_UP )
        {
            DBG_ERR("%s() %s link down\n", __func__, iface->name);
            boot_seq = wsd_interface_dropped(iface);
        }
        events &= ~NET_IFACE_EVENT_LINK_STATUS_CHANGE; // clear wsd's link event
    }

    if (events > 0)
    {
        if (intf_active(iface->name))
        {
            DBG_ERR("%s() intf %s event %d v4 %d->%d v6 %d->%d\n",
                __func__, iface->name, events, ipv4_prev, g_wsd_ctxt.ipv4_configured, ipv6_prev, g_wsd_ctxt.ipv6_configured);

            if (g_wsd_ctxt.ipv4_configured > ipv4_prev
#ifdef HAVE_IPV6
                    || g_wsd_ctxt.ipv6_configured > ipv6_prev
#endif // HAVE_IPV6
                )
            {
                /*  Net just came up (i.e. either ipv4 or ipv6 just came up).  Note that we
                    purposely do not use current value of g_wsd_ctxt.state in making this determination.
                 */
                if ( !(wsd_ready_flags & WSD_STATE_INITIALIZING) ) // wsd ready?
                {
                    DBG_ERR("Interface %s is now up\n", iface->name); // VERBOSE
                    wsd_set_flags((void*)iface, WSD_FLAG_IP_CONFIGURED);
                    platvars_inc_powerup_counter();
                    platvars_get_powerup_counter(&boot_seq);
                    dpws_update_network(&ip_selector, NULL);
                    DBG_VERBOSE("\n%s() join %s intf\n", __func__, iface->name);
                    int ret = dpws_join_network(boot_seq);
                    if (ret) // error?
                    {
                        DBG_ERR("\n  %s NOT wsd-enabled (err %d)\n", iface->name, ret);
                    }
                }
            }
        }
        else // interface is down
        {
            DBG_VERBOSE("%s() intf %s event %d v4 %d->%d v6 %d->%d\n",
                __func__, iface->name, events, ipv4_prev, g_wsd_ctxt.ipv4_configured, ipv6_prev, g_wsd_ctxt.ipv6_configured);

            if ((g_wsd_ctxt.ipv4_configured < ipv4_prev
#ifdef HAVE_IPV6
                    && !g_wsd_ctxt.ipv6_configured) || (g_wsd_ctxt.ipv6_configured < ipv6_prev && !g_wsd_ctxt.ipv4_configured
#endif // HAVE_IPV6
                ))
            {
                boot_seq = wsd_interface_dropped(iface);
            }
        }

        print_device_info(wsd_dpws_data.device);
    }

    /**
     * If the shutdown pending gets fully implemented then WSD 
     * should use a timeout on the stop server call - to allow the 
     * sending of BYE messages 
     */
    //if ( ( events & NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING ) 
    //     #ifdef HAVE_IPV6
    //     || ( events & NET_IFACE_EVENT_IPV6_SHUTDOWN_PENDING ) 
    //     #endif // HAVE_IPV6
    //     )
    //{
    //    dpws_stop_server( 1000 );
    //    wsd_clear_flags( WSD_FLAG_IP_CONFIGURED );
    //    wsd_state = WSD_STOPPED;
    //    net_iface_continue_shutdown( wsd_iface_handle, NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING );
    //}
    return;
}

wsd_dpws_data_t *wsd_get_dpws_data(void) {
    return &wsd_dpws_data;
}

/* Start server message listener */
static int wsd_boot_server(struct dpws *m_dpws)
{
    struct dpws * s_dpws;
    int status = DPWS_OK;
    pthread_t *thread;
    uint8_t *thread_stack;
    char* thread_name;
    unsigned int tx_res;

    wsd_ready_flags &= ~WSD_STATE_INITIALIZING;
    if (wsd_ready_flags & WSD_STATE_SHUTDOWN_REQUESTED) {
        DBG_ERR("Shutdown was requested before initialization - Closing now\n");
        wsd_ready_flags &= ~WSD_STATE_SHUTDOWN_REQUESTED;
        wsd_restart_dpws_server();
    }
    wsd_state_flags |= WSD_STATE_RUNNING;

    DBG_VERBOSE("%s() ..\n", __func__);

    while (TRUE) {
        // Allocate a DPWS runtime structure for the next server task.
        s_dpws = MEM_MALLOC(sizeof ( struct dpws));
        if (s_dpws == NULL) {
            DBG_ERR("Failed to malloc memory for dpws structure.\n");
            posix_sleep_ticks(100);
            continue;
        }

        // monitors listening sockets for incoming messages and prepares a second runtime structure for processing.
        status = dpws_accept_thr(m_dpws, s_dpws);
        
        if (status) {
            DBG_ERR("%s - Error: %s\n", __FUNCTION__, (char *) dpws_get_error_msg(m_dpws));
            break;
        }


        if (wsd_get_thread(&thread, &thread_stack, &thread_name) != NET_WSD_SUCCESS)
        {
            DBG_ERR("Failed to get a thread in wsd - throw away current request\n");
            if (s_dpws)
            {
                MEM_FREE_AND_NULL(s_dpws);
            }
            continue;
        }

        /* spawn a new thread */
        tx_res = pthread_create(thread, NULL, wsd_serve_request, s_dpws);	        
        XASSERT(tx_res == 0, tx_res);
    }
    wsd_state_flags &= ~WSD_STATE_RUNNING;

    // wait for any lagging threads to roost
    bool roosting = true;
    while (roosting)
    {
        int i;
        for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++)
        {
            if (wsd_thread_init[ i ] == WSD_THREAD_IN_USE)
            {
                if (pthread_kill(*(wsd_threads[i]),0) == ESRCH)
                {
                    DBG_ERR( "%s - marking thread %d as finished \n", __func__, i); // VERBOSE
                    wsd_thread_init[ i ] = WSD_THREAD_UNUSED;
                }
                else
                {
                    DBG_ERR("%s() waiting for thread #%d %s (%#x) to finish\n", __func__, i+1, wsd_thread_names[i], wsd_threads[i]);
                }
                break; // not done yet..
            }
        }
        if (i >= wsd_config_vals.num_dispatcher_threads) roosting = false; // done
        else
        {
            //DBG_ERR("%s() waiting for thread(s) to finish\n", __func__);
            posix_sleep_ms(10); // wait awhile
        }
    }

    // cleanup
    if (status)
    {
        DBG_ERR("%s - Error: %s\n", __FUNCTION__, (char *) dpws_get_error_msg(m_dpws));
    }
    if (s_dpws)
    {
        MEM_FREE_AND_NULL(s_dpws); // free unused runtime structure.
    }

    dpws_server_shutdown(m_dpws); // Clean the DPWS registry.

    if (status)
        return status; // err

    return 0;
}

/* One-shot server task */
static void* wsd_serve_request(void* input) {
    struct dpws * dpws;   
    int ret = 0;
    dpws = (struct dpws *) input;

    ret = dpws_serve(dpws);
    if (ret != OK) // performs message processing.
    {
        DBG_ERR("%s(%#x) dpws_server returned err %d\n", __func__, dpws, ret);
    }
    DBG_VERBOSE("Calling dpws_end in wsd_serve_request function\n");
    dpws_end(dpws); // frees transient message memory.
    MEM_FREE_AND_NULL(dpws); // disposable runtime structure: not efficient

    return 0;
}

error_type_t wsd_get_thread(pthread_t **thread_ptr, uint8_t **stack_ptr, char **thread_name)
{
    uint8_t i;
    unsigned int tx_result;
    for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++) {
         if (wsd_thread_init[ i ] == WSD_THREAD_IN_USE) {
               
               if (pthread_kill(*(wsd_threads[i]),0) == ESRCH)
                {
                    DBG_VERBOSE( "%s - marking thread %d as finished \n", __FUNCTION__, i);
                    wsd_thread_init[ i ] = WSD_THREAD_UNUSED;

                    // Thread is not in use - delete old thread (if exists) then create new one
                    DBG_VERBOSE( "%s - Calling pthread-join \n", __FUNCTION__);
                    tx_result = pthread_join(*(wsd_threads[ i ]), NULL);
                    //XASSERT(tx_result == 0, tx_result);
                }
                else {
                    DBG_MSG("%s - thread(%d) state: RUNNING\n", __FUNCTION__, i);
                }
        } else {
            DBG_VERBOSE("%s - thread %d is idle\n", __FUNCTION__, i);
        }
    }

    // Look through threads for an unused thread
    for (i = 0; i < wsd_config_vals.num_dispatcher_threads; i++) {
        if (wsd_thread_init[ i ] == WSD_THREAD_UNUSED) {
            wsd_thread_init[ i ] = WSD_THREAD_IN_USE;
            *thread_ptr = wsd_threads[ i ];
            *stack_ptr = wsd_thread_stacks[ i ];
            *thread_name = wsd_thread_names[ i ];
            
            return NET_WSD_SUCCESS;
        }

        DBG_VERBOSE("%s %d: state = %d\n", __FUNCTION__, i, wsd_thread_init[ i ]);
    }

    // No available threads!
    DBG_ERR("%s exit - FAIL!\n", __FUNCTION__);

    return NET_WSD_RESOURCE_FAILURE;
}

void wsd_set_flags(void* context, uint32_t flags)
{
    net_interface_t* intf = (net_interface_t*) context; // simple cast
    //revisit conditional if ( ! ( wsd_ready_flags & flags)) // update if changed
    {
#ifdef __linux__
        DBG_VERBOSE("%s(%#x) flags %d -> %d\n", __func__, intf, wsd_ready_flags, (wsd_ready_flags | flags));
        pthread_mutex_lock(&wsd_ready_mutex);
            wsd_ready_flags |= flags;
            pthread_cond_broadcast(&wsd_ready_cond); // trigger flag change
        pthread_mutex_unlock(&wsd_ready_mutex);
#else // threadx
        tx_event_flags_set( &wsd_ready_flags, flags, TX_OR );
#endif // threadx
    }
}

void wsd_clear_flags(void* context, uint32_t flags)
{
    net_interface_t* intf = (net_interface_t*) context; UNUSED_VAR(intf); // simple cast

    {
#ifdef __linux__
        pthread_mutex_lock(&wsd_ready_mutex);
            wsd_ready_flags &= ~flags;
            pthread_cond_broadcast(&wsd_ready_cond); // trigger
        pthread_mutex_unlock(&wsd_ready_mutex);
#else // threadx
        tx_event_flags_set( &wsd_ready_flags, ~flags, TX_AND );
#endif // threadx
    }
}

char *wsd_get_job_state_string(wsd_job_states_t state)
{
    XASSERT(state < NUM_WSD_JOB_STATES, state);
    return wsd_job_states [ state ];
}

void wsd_custom_settings(int16_t device)
{

    net_wsd_wstransfer_custom_namespace_t *wst_custom_ns;
    net_wsd_wstransfer_custom_hosted_service_t **hs;
    uint16_t wst_service_port;
    uint16_t service;
    struct prefixed_qname port_type;
    struct prefixed_qname service_type;
    short source;
    uint32_t num;
    uint32_t i;
    uint32_t j;
    int ret;

    // Add any custom namespaces
    if (wsd_config_vals.wsd_custom_func_table->wsd_get_wstransfer_custom_namespace) {
        if (wsd_config_vals.wsd_custom_func_table->wsd_get_wstransfer_custom_namespace(&wst_custom_ns, &num) == NET_WSD_SUCCESS) {
            for (i = 0; i < num; i++) {
                if (wst_custom_ns[ i ].localname != NULL) {
                    port_type.prefix = wst_custom_ns[ i ].prefix;
                    port_type.qname.lname = wst_custom_ns[ i ].localname;
                    port_type.qname.ns = wst_custom_ns[ i ].ns;
                    ret = DPWS_ADD_PTR_ATT(device, DPWS_PTR_PREFIXED_TYPE, &port_type);
                    XASSERT(ret == 0, ret);
                }
            }
            ASSERT(wsd_config_vals.wsd_custom_func_table->wsd_free_wstransfer_custom_namespace != NULL);
            wsd_config_vals.wsd_custom_func_table->wsd_free_wstransfer_custom_namespace(wst_custom_ns, num);
        }
    }

    // Add any custom hosted services
    if (wsd_config_vals.wsd_custom_func_table->wsd_get_wstransfer_custom_hosted_service) {
        if (wsd_config_vals.wsd_custom_func_table->wsd_get_wstransfer_custom_hosted_service(&hs, &num) == NET_WSD_SUCCESS) {

            for (i = 0; i < num; i++) {
                service_type.prefix = hs[ i ]->type.prefix;
                service_type.qname.lname = hs[ i ]->type.localname;
                service_type.qname.ns = hs[ i ]->type.ns;

                service = dpws_create_service_class();
                ret = DPWS_ADD_PTR_ATT(service, DPWS_PTR_PREFIXED_TYPE, &service_type);
                XASSERT(ret == 0, ret);

                source = dpws_create_hosted_service(device, service);
                ret = DPWS_SET_STR_ATT(source, DPWS_STR_SERVICE_ID, hs[ i ]->service_id);
                XASSERT(ret == 0, ret);

                for (j = 0; j < hs[ i ]->num_addresses; j++) {
                    wst_service_port = dpws_create_service_port();
                    ret = DPWS_SET_STR_ATT(wst_service_port, DPWS_STR_ADDRESS, hs[ i ]->addresses[ j ]);
                    XASSERT(ret == 0, ret);

                    dpws_bind_service(source, wst_service_port);
                    dpws_release_handle(wst_service_port);
                }
                dpws_release_handle(service);

            }
            ASSERT(wsd_config_vals.wsd_custom_func_table->wsd_free_wstransfer_custom_hosted_service != NULL);
            wsd_config_vals.wsd_custom_func_table->wsd_free_wstransfer_custom_hosted_service(hs, num);
        }
    }

}

void wsd_lock_job_history(void)
{
        
    while (pthread_mutex_lock(&wsd_ready_mutex) != 0)
    {
        posix_sleep_ticks(100);
    }
    //    tx_mutex_get( &wsd_ready_mutex, TX_WAIT_FOREVER );
}

void wsd_unlock_job_history(void)
{
    //tx_mutex_put( &wsd_ready_mutex ); 
    pthread_mutex_unlock(&wsd_ready_mutex);
}

// wsd_lock_job_history must be called and not released until finished 
// using the wsd_job_info_t structure

error_type_t wsd_new_job(wsd_protocols_t type, wsd_job_info_t **job_info)
{
    uint32_t i;

    for (i = 0; i < wsd_config_vals.num_job_history_elements; i++) {
        if ((wsd_job_history[ i ].active == true) &&
                (wsd_job_history[ i ].protocol == type)) {
            return NET_WSD_RESOURCE_FAILURE;
        }
    }

    i = wsd_job_history_index++;
    if (wsd_job_history_index >= wsd_config_vals.num_job_history_elements) {
        wsd_job_history_index = 0;
    }

    if (wsd_job_history[ i ].name != NULL) {
        MEM_FREE_AND_NULL(wsd_job_history[ i ].name);
    }
    if (wsd_job_history[ i ].user_name != NULL) {
        MEM_FREE_AND_NULL(wsd_job_history[ i ].user_name);
    }

    if (wsd_job_history[ i ].doc_name != NULL) {
        MEM_FREE_AND_NULL(wsd_job_history[ i ].doc_name);
    }

    if (wsd_job_history[ i ].ticket != NULL) {
        if (wsd_protocols[ type ]->func_table.free_ticket != NULL) {
            wsd_protocols[ type ]->func_table.free_ticket((void*) wsd_job_history[ i ].ticket);
        }
    }
    if (wsd_job_history[ i ].requested_ticket != NULL) {
        if (wsd_protocols[ type ]->func_table.free_ticket != NULL) {
            wsd_protocols[ type ]->func_table.free_ticket((void*) wsd_job_history[ i ].requested_ticket);
        }
    }
    memset(&wsd_job_history[ i ], 0x00, sizeof ( wsd_job_info_t));
    wsd_job_history[ i ].id = wsd_job_id++;
    wsd_job_history[ i ].active = true;
    wsd_job_history[ i ].protocol = type;
    wsd_job_history[ i ].name = NULL;
    wsd_job_history[ i ].user_name = NULL;
    wsd_job_history[ i ].doc_name = NULL;

    *job_info = &wsd_job_history[ i ];

    return NET_WSD_SUCCESS;
}

wsd_job_info_t *wsd_get_job_history(void)
{
    return wsd_job_history;
}

net_wsd_config_t *wsd_get_config(void)
{
    return &wsd_config_vals;
}

// wsd_lock_job_history must be called and not released until finished 
// using the wsd_job_info_t structure

wsd_job_info_t *wsd_get_job_info(uint32_t job_id)
{
    uint32_t i;

    for (i = 0; i < wsd_config_vals.num_job_history_elements; i++) {
        if (wsd_job_history[ i ].id == job_id) {
            return &wsd_job_history[ i ];
        }
    }

    return NULL;
}

char *wsd_job_state_reason_str(wsd_job_state_reasons_t job_state_reason)
{
    XASSERT(job_state_reason < NUM_WSD_JOB_STATE_REASON_END, NUM_WSD_JOB_STATE_REASON_END);
    return wsd_job_state_reasons[ job_state_reason ];
}

/**
 * Puts all the changeable WSD meta data into one string and
 * then computes the checksum off of that string.  
 */
static uint32_t wsd_calc_meta_data_checksum(void)
{
    bool enabled;
    bool configured;
    char hostname[ NET_IFACE_HOSTNAME_SIZE ];
    char *fw_version;
#ifdef HAVE_IPV6
    char meta_data[ 7 * INET6_ADDRSTRLEN + 1 * INET_ADDRSTRLEN + NET_IFACE_HOSTNAME_SIZE + FWVERSION_STR_LEN ];
#else
    char meta_data[ 1 * INET_ADDRSTRLEN + NET_IFACE_HOSTNAME_SIZE + FWVERSION_STR_LEN ];
#endif

    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    meta_data[ 0 ] = '\0';
    if (net_iface_get_var_hostname(if_index, hostname, NET_IFACE_HOSTNAME_SIZE) != OK) {
        ASSERT(0);
        return 1;
    }
    strcat(meta_data, hostname);

    if ((net_iface_get_var_ipv4_enabled(if_index, &enabled) == OK) && (enabled == true) &&
            (net_iface_get_var_ipv4_configured(if_index, &configured) == OK) && (configured == true)) {
        uint32_t ipv4_addr;
        char ipv4_addr_str[ INET_ADDRSTRLEN ];
        net_iface_get_var_ipv4_addr(if_index, &ipv4_addr);
        inet_ntop(AF_INET, (void *) &ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN);
        strcat(meta_data, ipv4_addr_str);
    }

#ifdef HAVE_IPV6
    if ((net_iface_get_var_ipv6_enabled(if_index, &enabled) == OK) && (enabled == true)) {
        net_iface_ipv6_addr_t ipv6_addr;
        net_iface_ipv6_addr_hndl_t addr_hndl;
        net_iface_ipv6_addr_hndl_t prev_addr_hndl;
        char ipv6_addr_str[ INET6_ADDRSTRLEN ];

        if ((net_iface_get_var_ipv6_local_configured(if_index, &configured) == OK) && (configured == true)) {
            net_iface_get_var_ipv6_link_local_addr(if_index, &ipv6_addr, &addr_hndl);
            inet_ntop(AF_INET6, (void *) (&ipv6_addr.addr), ipv6_addr_str, INET6_ADDRSTRLEN);
            strcat(meta_data, ipv6_addr_str);
        }

        if ((net_iface_get_var_ipv6_configured(if_index, &configured) == OK) && (configured == true)) {
            net_iface_get_var_ipv6_preferred_addr(if_index, &ipv6_addr, &addr_hndl);
            while (NET_IFACE_IPV6_ADDR_UNSPECIFIED(ipv6_addr) == false) {
                inet_ntop(AF_INET6, (void *) (&ipv6_addr.addr), ipv6_addr_str, INET6_ADDRSTRLEN);
                strcat(meta_data, ipv6_addr_str);
                prev_addr_hndl = addr_hndl;
                net_iface_get_var_ipv6_next_addr(if_index, &ipv6_addr, &addr_hndl, prev_addr_hndl);
            }
        }
    }
#endif // HAVE_IPV6

    platvars_get_fw_version(&fw_version);
    strcat(meta_data, fw_version);

    DBG_MSG("Checksum string: %s\n", meta_data);

    return CalcChecksum((uint32_t *) meta_data, strlen(meta_data));
}

#ifdef HAVE_DBG_PRINTF

const char *wsd_msg_type_strs(net_wsd_msg_type_t index)
{
    XASSERT(index < NET_WSD_NUM_MSGS, index);
    return g_net_wsd_msg_strs[ index ];
}

void wsd_print_job(const char* functionName, const wsd_job_info_t* job)
{
    DBG_MSG("%s:\n\n", functionName);

    DBG_MSG("job->active            = %d\n", job->active);
    DBG_MSG("job->protocol          = %d\n", job->protocol);
    DBG_MSG("job->id                = %d\n", job->id);
    DBG_MSG("job->state             = %s\n", wsd_get_job_state_string(job->state));
    DBG_MSG("job->reasons           = %s\n", wsd_job_state_reason_str(job->reasons));
    if (job->name != NULL) {
        DBG_MSG("job->name              = %s\n", job->name);
    } else {
        DBG_MSG("job->name              =\n");
    }
    if (job->user_name != NULL) {
        DBG_MSG("job->user_name         = %s\n", job->user_name);
    } else {
        DBG_MSG("job->user_name         =\n");
    }
    DBG_MSG("job->koctets_processed = %d\n", job->koctets_processed);
    DBG_MSG("job->sheets_completed  = %d\n", job->sheets_completed);
    DBG_MSG("job->num_documents     = %d\n", job->num_documents);
    if (job->doc_name != NULL) {
        DBG_MSG("job->doc_name          = %s\n", job->doc_name);
    } else {
        DBG_MSG("job->doc_name          =\n");
    }
    DBG_MSG("job->ticket            = %X\n", job->ticket);
    DBG_MSG("job->requested_ticket  = %X\n", job->requested_ticket);
    //net_io_session_hndl_t   io_session_hndl;
    //net_io_job_hndl_t       job_hndl;
    //LINK_HEADER             job_data;
    DBG_MSG("job->job_data_bytes_buffered= %d\n", job->job_data_bytes_buffered);
    DBG_MSG("job->io_ready_sent          = %d\n", job->io_ready_sent);
    DBG_MSG("job->job_data_recv_done     = %d\n", job->job_data_recv_done);
    DBG_MSG("job->job_created_time       = %d\n", job->job_created_time);
    DBG_MSG("job->job_completed_time     = %d\n\n", job-> job_completed_time);

}

void wsd_print_history()
{
    uint32_t i;

    // Do not lock it, this function is meant for debugging only
    //wsd_lock_job_history();
    for (i = 0; i < wsd_config_vals.num_job_history_elements; i++) {
        wsd_print_job("", &wsd_job_history[i]);
    }

    //wsd_unlock_job_history();
}
#endif // HAVE_DBG_PRINTF

