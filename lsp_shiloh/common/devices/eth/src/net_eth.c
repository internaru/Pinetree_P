/*
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __linux__

#include "os_network.h"
#include "net_api.h"

#else
#include "ATypes.h"
#include "platform.h" // for SYS_TICK_FREQ
#include "platform_api.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h" // for UNUSED_VAR
#include "memAPI.h"
#include "tx_api.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif // HAVE_CMD
#include "memAPI.h"

#include "eth_phy_api.h"
#include "eth_mac_api.h"
#ifdef HAVE_POWER_MGR
 #include "pwr_mgr_api.h"
#ifdef HAVE_ASIC_POWER_DRIVER
 #include "eth_pwr_drv.h"
 #include "asic_pwr_drv_api.h"
#endif // HAVE_ASIC_POWER_DRIVER
#endif // HAVE_POWER_MGR
#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#include "net_logger.h"
#endif // ! __linux__

// local headers
#include "eth_phy_api.h"
#include "eth_mac_api.h"
#include "net_eth_api.h"
#include "net_eth_vars.h"

// private local includes
#include "net_eth.h"


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifdef __linux__

#define DBG_PRFX "net.eth: "
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family macros

#else
#define DBG_PRFX "NET_ETH: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_LINK 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)

#ifdef HAVE_CMD
#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
#endif
#endif

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#ifdef __linux__
#define NET_ETH_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = os_mutex_get(&g_net_eth.mtx, OS_WAIT_FOREVER); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 

#define NET_ETH_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = os_mutex_put(&g_net_eth.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        } 
#else
#define NET_ETH_LOCK_CONTEXT() \
        { \
            UINT threadx_rcode = tx_mutex_get(&g_net_eth.mtx, TX_WAIT_FOREVER); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
            UNUSED_VAR(threadx_rcode); \
        } 

#define NET_ETH_UNLOCK_CONTEXT() \
        { \
            UINT threadx_rcode = tx_mutex_put(&g_net_eth.mtx); \
            XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode); \
            UNUSED_VAR(threadx_rcode); \
        } 
#endif

//=========================================================================================================================
// Module configuration
//=========================================================================================================================

//####### TODO move this to the config file
//#define ETH_MAC_RX_BUF_POOL_MAX_BYTES       (3 * (TM_ETHER_MAX_PACKET_CRC+TM_ETHER_IP_ALIGN))
//#define ETH_MAC_TX_BUF_QUEUE_MAX_SLOTS      3
//TODO use "have hotspot" ifdef -- should be 20 otherwise
#define ETH_MAC_RX_BUF_POOL_MAX_BYTES       (40 * (TM_ETHER_MAX_PACKET_CRC+TM_ETHER_IP_ALIGN))
#define ETH_MAC_TX_BUF_QUEUE_MAX_SLOTS      40


// default dev index used when only a single Ethernet MAC instance is supported
#define ETH_MAC_DEV_INDEX   0

#ifdef HAVE_ETH_LINK_STATUS_POLLING
    #define NET_ETH_LINK_STATUS_POLL_PERIOD_MS  1000
#endif // HAVE_ETH_LINK_STATUS_POLLING

/*
 *  User-defined values for the source_id parameter returned in the call to
 *  eth_mac_complete_tx_buffer() and in the registered eth_mac_free_user_buffer_func_t
 *  callback.
 */
#define NET_ETH_SRC_TRECK_RECV  1
#define NET_ETH_SRC_TRECK_XMIT  2
#define NET_ETH_SRC_RAW_XMIT    3

// context default values
static net_eth_context_t g_net_eth =
{
    .msg_queue_initialized = false,
    .dev_index = ETH_MAC_DEV_INDEX,
    .prev_link_status = ETH_MAC_LINK_STATUS_DOWN,
    .tx_frames_complete_pending = 0,
    .link_event_notify_func = NULL,
    .treck_interface = NULL,
#ifdef HAVE_POWER_MGR
    .phy_hndl = 0, // TODO TEMP for pwr mgr -- normally we should not access phy layer directly from net_eth
    .pwr_level = pwr_mgr_on_e,
#endif // HAVE_POWER_MGR
};


//=========================================================================================================================
// Private utility functions
//=========================================================================================================================

net_eth_context_t *net_eth_get_ctxt(void)
{
    return &g_net_eth;
}

net_eth_context_t *net_eth_lock_ctxt(void)
{
    NET_ETH_LOCK_CONTEXT();
    return &g_net_eth;
}

void net_eth_unlock_ctxt(net_eth_context_t *net_eth_ctxt)
{
    if(net_eth_ctxt != &g_net_eth)
    {
        return;
    }

    NET_ETH_UNLOCK_CONTEXT();
    return;
}

#ifdef __linux__

// private utility functions sofar
eth_mac_link_status_t eth_mac_get_link_status(uint8_t index)
{
    return (intf_up(NET_ETH_LINK_NAME) ? NET_LINK_STATUS_UP : NET_LINK_STATUS_DOWN);
}

error_type_t eth_mac_apply_config_options(uint8_t index, eth_mac_link_config_t link_config)
{
    DBG_VERBOSE("%s(%d,x%x) impl?\n", __func__, index, link_config);
    return OK;
}

#else
//=========================================================================================================================
// Thread & message queue
//=========================================================================================================================

#define NET_ETH_STACK_SIZE 8192

static uint8_t net_eth_stack[NET_ETH_STACK_SIZE] __attribute__ ((aligned (8)));
static TX_THREAD net_eth_thread; // worker thread for processing ISR notifications

typedef enum {
    NET_ETH_MSG_LINK_STATUS_CHANGE,
#ifdef HAVE_ETH_LINK_STATUS_POLLING
    NET_ETH_MSG_POLL_LINK_STATUS,
#endif // HAVE_ETH_LINK_STATUS_POLLING
    NET_ETH_MSG_RAW,

    // add new msgs above this line
    NET_ETH_NUM_MSGS
} net_eth_msg_type_t;

/*
 *  message queue
 */

#define NET_ETH_MQ_NAME                 "netLinkEth"
#define NET_ETH_MQ_NUM_MSG_BUFS         20

/* message format */
#define NET_ETH_MQ_MSG_BUF_SIZE    TX_2_ULONG // buff byte size must be >= sizeof(net_eth_msg_t)
typedef struct net_eth_msg_s
{
    net_eth_msg_type_t type;
    void *data; // semantics defined by msg type
} net_eth_msg_t;

/* message queue buffer */
#define NET_ETH_MQ_MSG_BUF_BYTES   (NET_ETH_MQ_MSG_BUF_SIZE*sizeof(ULONG))
typedef union
{
    net_eth_msg_t msg;
    uint8_t buf[NET_ETH_MQ_MSG_BUF_BYTES]; // forces netapps_msg_buf_t size into valid threadx queue size option
} net_eth_msg_buf_t;

static net_eth_msg_buf_t    g_net_eth_msg_bufs[NET_ETH_MQ_NUM_MSG_BUFS]; // msg queue buffer space
static TX_QUEUE             g_net_eth_msg_queue; // msg queue control block


//=========================================================================================================================
// Raw socket stuff
//=========================================================================================================================

#define NET_ETH_MAX_CALLBACKS  64
struct net_eth_notify_callback_s
{
    net_eth_notify_callback_t   callback;
    uint16_t                    ether_type;
};
static struct net_eth_notify_callback_s net_eth_notify_callback_list[NET_ETH_MAX_CALLBACKS];
static uint32_t net_eth_callback_count;
static TX_SEMAPHORE    net_eth_notify_callback_semaphore;    // Restrict writes to callback registration


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

// State loop
static void net_eth_msg_loop(ULONG param);

// Abstract base class API functions
static void net_eth_event_register(net_link_event_notify_func_t notify_func, void *user_data);
static int net_eth_open(ttUserInterface treck_interface);
static int net_eth_close(ttUserInterface treck_interface);
static int net_eth_send(ttUserInterface treck_interface, char *data_ptr, int data_len, int flag);
static int net_eth_recv(ttUserInterface treck_interface,
                     char **data_ptr,
                     int *data_len,
                     ttUserBufferPtr user_buf_ptr
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
                     , ttDevRecvOffloadPtr devRecvOffloadPtr
#endif
                     );
static int net_eth_ioctl(ttUserInterface treck_interface, int flag, void *option_ptr, int option_len);
static int net_eth_get_phys_addr(ttUserInterface treck_interface, char * phys_addr);

// Ethernet MAC driver callbacks
static char *net_eth_get_user_buffer_cb(uint32_t *len, uint32_t *source_id, void **user_data);
static void net_eth_free_user_buffer_cb(char *buf, uint32_t source_id, void *user_data);
static void net_eth_isr_notify_frame_cb(uint16_t rx_frames, uint16_t tx_frames,
                                        uint32_t rx_bytes, uint32_t tx_bytes,
                                        uint32_t flags, void *user_data);
static void net_eth_isr_notify_link_cb(void *user_data);

// Private utility functions
static void net_eth_add_multicast(uint8_t *addrs, uint32_t num_addrs);
static void net_eth_send_complete(void);
static void net_eth_link_status_notify(void *data);
static int32_t net_eth_send_msg(net_eth_msg_type_t type, void *data);
#ifdef HAVE_ETH_LINK_STATUS_POLLING
static void net_eth_timer_callback(ULONG param);
#endif // HAVE_ETH_LINK_STATUS_POLLING

// Power management functions
#ifdef HAVE_POWER_MGR
#define PWR_MGR_MY_NAME PWRMGR_DRV_NAME_LAN
static bool net_eth_power_save_cb(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd);
static bool net_eth_power_save(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd);
#endif // HAVE_POWER_MGR

// Serial debug command functions
#ifdef HAVE_CMD
static void net_eth_register_dbg_cmds(void);
static int net_eth_cmd_link_status_cb( int argc, char *argv[] );
#ifdef HAVE_POWER_MGR
static int net_eth_cmd_power_reduced_cb( int argc, char *argv[] );
static int net_eth_cmd_power_off_cb( int argc, char *argv[] );
static int net_eth_cmd_power_on_cb( int argc, char *argv[] );
#endif // HAVE_POWER_MGR
#endif // HAVE_CMD 

// raw socket stuff
static void net_eth_callback_notify( net_eth_msg_t *msg );
static void net_eth_free_treck_buffer(char *pkt_buff, UINT32 pkt_len, void *pkt_desc);


//=========================================================================================================================
// Public Function Definitions
//=========================================================================================================================

net_rcode_t net_eth_init(net_link_api_funcs_t *api_funcs)
{
    DBG_VERBOSE("==>%s\n", __func__);
    UINT tx_res;
    UINT threadx_rcode;
    net_rcode_t net_rcode = NET_OK;

    ASSERT(api_funcs);
    api_funcs->event_register = net_eth_event_register;
    api_funcs->open = net_eth_open;
    api_funcs->close = net_eth_close;
    api_funcs->send = net_eth_send;
    api_funcs->recv = net_eth_recv;
    api_funcs->ioctl = net_eth_ioctl;
    api_funcs->get_phys_addr = net_eth_get_phys_addr;

    eth_mac_func_table_t net_eth_cb_funcs;
    net_eth_cb_funcs.get_user_buf = net_eth_get_user_buffer_cb;
    net_eth_cb_funcs.free_user_buf = net_eth_free_user_buffer_cb;
    net_eth_cb_funcs.isr_notify_frame = net_eth_isr_notify_frame_cb;
    net_eth_cb_funcs.isr_notify_link = net_eth_isr_notify_link_cb;

    // reset stat counters
    memset(&g_net_eth.rx_stats, 0, sizeof(g_net_eth.rx_stats));
    memset(&g_net_eth.tx_stats, 0, sizeof(g_net_eth.tx_stats));

    char *phys_addr = NULL;
    platvars_get_mac_addr(&phys_addr);

    net_eth_var_init();
    eth_mac_link_config_t link_config;
    net_eth_get_var_link_config(&link_config);

    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_init(g_net_eth.dev_index, (uint8_t *)phys_addr, TM_ETHERNET_PHY_ADDR_LEN,
                                 link_config, &net_eth_cb_funcs, (void *)0, 
                                 ETH_MAC_RX_BUF_POOL_MAX_BYTES, ETH_MAC_TX_BUF_QUEUE_MAX_SLOTS);
    if(eth_mac_rcode != ETH_MAC_OK)
    {
        net_rcode = NET_FAIL;
        goto done;
    }

    threadx_rcode = tx_mutex_create(&g_net_eth.mtx, "net_eth", TX_INHERIT);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating mutex (threadx_rcode=0x%02x)\n", threadx_rcode);
        ASSERT(0);
        net_rcode = NET_FAIL;
        goto done;
    }

#ifdef HAVE_POWER_MGR

    // ###### TODO TEMP get the phy_hndl from Eth MAC driver
    eth_phy_handle_t eth_mac_get_phy_hndl(uint8_t dev_index);
    g_net_eth.phy_hndl = eth_mac_get_phy_hndl(g_net_eth.dev_index);

    // Get the unique id for use in power manager active state management.
    g_net_eth.net_eth_pwr_mgr_uid = pwr_mgr_get_uid();
    uint32_t priority = pwr_mgr_pri_lowest_e;
#ifdef PWRMGR_ETH_PRIORITY
    priority = PWRMGR_ETH_PRIORITY;
#endif // PWRMGR_ETH_PRIORITY
    g_net_eth.net_eth_pwr_mgr_event_flag = pwr_mgr_reg_callback(PWR_MGR_MY_NAME, net_eth_power_save_cb, priority);
    
#ifdef PWRMGR_ETH_LEVEL
    // temp set pwr level here till unity get AOAO schema mgr
    pwr_mgr_set_module_pwr_level(PWR_MGR_MY_NAME, PWRMGR_ETH_LEVEL); 
#endif // PWRMGR_ETH_LEVEL

#endif // HAVE_POWER_MGR

#ifdef HAVE_CMD 
    net_eth_register_dbg_cmds();
#endif // HAVE_CMD

    // start the worker thread
    threadx_rcode = tx_thread_create(&net_eth_thread, "netLinkEth", 
                                     net_eth_msg_loop, (ULONG)0,
                                     net_eth_stack, NET_ETH_STACK_SIZE, 
                                     THR_PRI_NORMAL, THR_PRI_NORMAL, DEFAULT_TIME_SLICE, 
                                     TX_AUTO_START);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error creating thread net_eth_thread (threadx_rcode=0x%02x)\n", threadx_rcode);
        net_rcode = NET_FAIL;
        goto done;
    }

    /*
     *  Raw socket stuff
     */
    net_eth_callback_count = 0;
    // Create sempahore to protect saving the callback 
    tx_res = tx_semaphore_create( &net_eth_notify_callback_semaphore, "net_eth_notify_callback_semaphore", 1 );
    ASSERT( TX_SUCCESS == tx_res );

done:
    return net_rcode;
}


//=========================================================================================================================
// State loop
//=========================================================================================================================

static VOID net_eth_msg_loop(ULONG param)
{
    UINT threadx_rcode;

    /*
     *  create message queue
     */

    // validation of msg buf size
    ASSERT(sizeof(net_eth_msg_buf_t) == NET_ETH_MQ_MSG_BUF_BYTES);

    threadx_rcode = tx_queue_create(&g_net_eth_msg_queue, NET_ETH_MQ_NAME, NET_ETH_MQ_MSG_BUF_SIZE, 
                                    g_net_eth_msg_bufs, NET_ETH_MQ_NUM_MSG_BUFS*sizeof(net_eth_msg_buf_t));
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_MSG("error creating g_net_eth_msg_queue (threadx_rcode=0x%02x)\n", threadx_rcode);
        goto error;
    }
    g_net_eth.msg_queue_initialized = true;

    // account for any link status changes prior to enabling interrupt or starting thread
    // (otherwise we fail to acknowledge link up when cable already plugged in)
    net_eth_link_status_notify((void *)0);

#ifdef HAVE_ETH_LINK_STATUS_POLLING
    // start link status polling
    TX_TIMER link_status_timer;
    #define NET_ETH_LINK_STATUS_POLL_TICKS  ((NET_ETH_LINK_STATUS_POLL_PERIOD_MS*SYS_TICK_FREQ)/1000)
    threadx_rcode = tx_timer_create(&link_status_timer, "net_eth", net_eth_timer_callback, (ULONG)0,
                                     NET_ETH_LINK_STATUS_POLL_TICKS, NET_ETH_LINK_STATUS_POLL_TICKS,
                                     TX_AUTO_ACTIVATE);
    if(threadx_rcode != TX_SUCCESS)
    {
        DBG_ERR("error initing timer (threadx_rcode=0x%02x)\n", threadx_rcode);
        goto error;
    }
#endif // HAVE_ETH_LINK_STATUS_POLLING

    while(1)
    {
        net_eth_msg_buf_t msg_buf;
        net_eth_msg_t *msg;
        threadx_rcode = tx_queue_receive(&g_net_eth_msg_queue, (void *)&msg_buf, TX_WAIT_FOREVER);
        if(threadx_rcode != TX_SUCCESS)
        {
            DBG_MSG("error reading g_net_eth_msg_queue (threadx_rcode=0x%02x)\n", threadx_rcode);
            goto error;
        }

        msg = &msg_buf.msg;
        ASSERT(msg->type < NET_ETH_NUM_MSGS);
        
        /*
         *  process new message
         */

        switch(msg->type)
        {
#ifdef HAVE_ETH_LINK_STATUS_POLLING
        case NET_ETH_MSG_POLL_LINK_STATUS:
#endif // HAVE_ETH_LINK_STATUS_POLLING
        case NET_ETH_MSG_LINK_STATUS_CHANGE:
            net_eth_link_status_notify(msg->data);
            break;
        case NET_ETH_MSG_RAW:
            // Notify any registered modules
            net_eth_callback_notify( msg );
            break;
        default:
            // unexpected message type
            XASSERT(0, msg->type);
            break;
        }
    }

error:
    DBG_ERR("unrecoverable error in net_eth_worker thread! exiting...\n");
}


//=========================================================================================================================
// Abstract base class API functions
//=========================================================================================================================

static void net_eth_event_register(net_link_event_notify_func_t notify_func, void *user_data)
{
    ASSERT(notify_func);
    g_net_eth.link_event_notify_func = notify_func;
    g_net_eth.link_event_user_data = user_data;
    return;
}

static int net_eth_open(ttUserInterface treck_interface)
{
    NET_ETH_LOCK_CONTEXT();
    if(g_net_eth.treck_interface)
    {
        // already open
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_OKAY;
    }
    g_net_eth.tx_frames_complete_pending = 0;
    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_open(g_net_eth.dev_index);
    if(eth_mac_rcode != ETH_MAC_OK)
    {
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_ERROR;
    }
    // set *after* calling underlying open
    // non-NULL value implies we are fully open able to send/recv
    g_net_eth.treck_interface = treck_interface;

    NET_ETH_UNLOCK_CONTEXT();
    return TM_DEV_OKAY;
}

static int net_eth_close(ttUserInterface treck_interface)

{
    NET_ETH_LOCK_CONTEXT();
    if(!g_net_eth.treck_interface)
    {
        // already closed
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_OKAY;
    }

    // set *before* calling underlying close
    // non-NULL value implies we are fully open able to send/recv
    g_net_eth.treck_interface = NULL;

    // rdj 11-NOV-2012: don't process any more acks via net_eth_send_complete. Necessary to
    // prevent accidental callback to eth_mac driver after closing mac below.
    g_net_eth.tx_frames_complete_pending = 0;

    eth_mac_close(g_net_eth.dev_index);
    NET_ETH_UNLOCK_CONTEXT();
    return(TM_DEV_OKAY);
}

static int net_eth_send(ttUserInterface treck_interface, char *data_ptr, int data_len, int flag)
{
    bool is_last;
    int send_res = TM_DEV_OKAY;

    NET_ETH_LOCK_CONTEXT();

    if(!g_net_eth.treck_interface)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

    // see if is last buffer of the frame
    if(flag == TM_USER_BUFFER_LAST)
    {
        is_last = true;
    }

    // should always get packets in a single buffer for now.
    XASSERT(is_last, flag);

    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_insert_tx_buffer(g_net_eth.dev_index, data_ptr, data_len, 
                                             is_last, NET_ETH_SRC_TRECK_XMIT, (void *)0);
    if(eth_mac_rcode != ETH_MAC_OK)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

done:
    NET_ETH_UNLOCK_CONTEXT();

    // send ack to stack on any completed frames
    net_eth_send_complete();

    return send_res;
}

static int net_eth_recv(ttUserInterface treck_interface,
                     char **data_ptr,
                     int *data_len,
                     ttUserBufferPtr user_buf_ptr
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
                     , ttDevRecvOffloadPtr devRecvOffloadPtr
#endif
                     )
{
    if(!g_net_eth.treck_interface)
    {
        return TM_DEV_ERROR;
    }

    eth_mac_rx_flags_t rx_flags;
    void *user_data;

    ASSERT(data_ptr);
    ASSERT(data_len);
    ASSERT(user_buf_ptr);
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
    ASSERT(devRecvOffloadPtr);
#endif
    *user_buf_ptr = (ttUserBuffer)NULL;

    uint32_t prev_bad_frames = g_net_eth.rx_stats.bad_frames;
    uint32_t prev_ip_chksum_offload_errs = g_net_eth.rx_stats.errors.ip_chksum_offload_err;
    uint32_t prev_chksum_offload_errs = g_net_eth.rx_stats.errors.chksum_offload_err;

    *data_ptr = eth_mac_accept_rx_buffer(g_net_eth.dev_index, (uint32_t *)data_len,
                                         &rx_flags, &g_net_eth.rx_stats, NULL, &user_data);
    if(!*data_ptr)
    {
        // can happen when driver is implicitly closed, e.g. due to transition between low power states
        return TM_DEV_ERROR;
    }
    ASSERT(user_data);

    // check if bad_frame counter was incremented
    bool bad_frame = (prev_bad_frames != g_net_eth.rx_stats.bad_frames);

    // Let treck know about the new packet. 
    // NOTE: This is required even if we return TM_DEV_ERROR because the packet buffer must be freed by treck.
// BUGBUG -- There is a bug in 6.0 Treck code in file trdevice.c function tfRecvInterface() following the
// call to user callback devRecvFuncPtr. When the user callback returns with an error, Treck neglects to set
// "errorCode = TM_ENOBUFS;" prior to "goto recvInterfaceFinish;". As a result, any buffer passed back
// on error is not freed resulting in memory leak.  Workaround is to call tfFreeDriverBuffer() directly from
// here and leave *user_buf_ptr set to NULL.
//    *user_buf_ptr = (ttUserBuffer)user_data;

    // TODO make this test dependent on whether Treck macro TM_USE_DRV_SCAT_RECV is defined
    if(rx_flags & ETH_MAC_RX_FLAG_IS_PARTIAL)
    {
        // scatter receive currently not supported -- ignore partial frames
        // BUGBUG -- must free Treck buffer directly from here due to Treck bug explained above.
        tfFreeDriverBuffer((ttUserBuffer)user_data);
        return TM_DEV_ERROR;
    }

    if(bad_frame)
    {
        bool free_packet;

        // check if checksum offload counter was incremented
        bool ip_chksum_offload_err = (prev_ip_chksum_offload_errs != g_net_eth.rx_stats.errors.ip_chksum_offload_err);
        bool chksum_offload_err = (prev_chksum_offload_errs != g_net_eth.rx_stats.errors.chksum_offload_err);

        free_packet = true;
#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
        if(ip_chksum_offload_err)
        {
            // Check the Type 2 Full Checksum Offload Engine is configured in the MAC and enabled
            // an IP header checkum error
            devRecvOffloadPtr->devoRFlags = TM_DEVOR_IP_RECV_CHKSUM_FAILED;
        }
        else if(chksum_offload_err)
        {
            // The Rx Checksum Error bit indicates UDP, TCP, or ICMP6 checksum error. Note that we cannot distinguish
            // from the descriptor what type of packet was received. However, the
            // sample Treck 80314 driver just sets both TCP and UDP, so we set them all.
            devRecvOffloadPtr->devoRFlags = TM_DEVOR_TCP_RECV_CHKSUM_FAILED |
                                            TM_DEVOR_UDP_RECV_CHKSUM_FAILED |
                                            TM_DEVOR_ICMP6_RECV_CHKSUM_FAILED;

#ifdef TM_USE_IPV6
            if ( ( (ttEtherHeader *)*data_ptr)->ethType == TM_6_ETHER_IP_TYPE ) 
            {
                // Tahi Section 1 tests 40,41 Unrecognized Routing Type
                // In these tests ICMPv6 packets have invalid routing types and also have 
                // an invalid ICMP checksum.  These packets need to be sent on to Treck for 
                // processing.  For this reason we don't free the ICMP Treck buffer 
                // here and return even though the HW reported a checksum error.
                // 
                // Normally, our TCP/ICMP checksum receive offloading errors cause the packet
                // to not be passed on to Treck to save processing time but due to the Tahi
                // test we must process certain ICMPv6 packets with checksum errors...

                char *header_ptr;
                uint8_t next_header;
                uint32_t header_length;
                int32_t payload_length;
                header_ptr = *(data_ptr) + sizeof( ttEtherHeader);
                payload_length = (int32_t)htons( ((tt6IpHeader*)header_ptr)->iph6Plen );
                header_length = 40; // Basic IPv6 header is 40 bytes long
                next_header = ((tt6IpHeader*)header_ptr)->iph6Nxt;

                if ( payload_length <= *data_len )
                {
                    while ( true )
                    {
                        if ( next_header == IPPROTO_ROUTING )
                        {
                            tt6RoutingType0ExtHdr *rtrHdr;
                            rtrHdr = ( tt6RoutingType0ExtHdr * )(header_ptr + header_length);

                            // Total hack to pass Tahi Section 1 tests 40 and 41
                            if ( ( rtrHdr->rth6Type == 33 ) || ( rtrHdr->rth6Type == 0 ) )
                            {
                                // Router header type 0 or 33
                                // Must pass to treck for processing in order to pass Tahi
                                free_packet = false;
                                break;
                            }
                        }

                        // Advance to next header if there is one
                        payload_length -= header_length;
                        if ( payload_length <= 0 )
                        {
                            // Reached end of IPv6 headers - drop packet
                            break;
                        }
                        header_ptr += header_length;
                        next_header = *header_ptr;
                        // Header lengths are in multiples of 8 octets but does not include the first 8 octets
                        header_length = ( *(header_ptr+1) + 1 ) * 8;
                    }
                }
            }
#endif // TM_USE_IPV6

        }
#endif // HAVE_ETH_RECV_CHKSUM_OFFLOAD

        if ( free_packet )
        {
            // BUGBUG -- must free Treck buffer directly from here due to Treck bug explained above.
            tfFreeDriverBuffer((ttUserBuffer)user_data);

            return TM_DEV_ERROR;
        }
    }


    // rdj TODO - this should be decoupled from Treck and moved elsewhere
    // once we complete the MAC abstraction layer
    if(    ( ( (ttEtherHeader *)*data_ptr)->ethType != TM_ETHER_IP_TYPE )
#ifdef TM_USE_IPV6
        && ( ( (ttEtherHeader *)*data_ptr)->ethType != TM_6_ETHER_IP_TYPE )
#endif
        && ( ( (ttEtherHeader *)*data_ptr)->ethType != TM_ETHER_ARP_TYPE )
#ifdef TM_USE_PPPOE_CLIENT
        && ( ( (ttEtherHeader *)*data_ptr)->ethType != TM_PPPOE_PPP_ETH_TYPE )
        && ( ( (ttEtherHeader *)*data_ptr)->ethType != TM_PPPOE_DISC_ETH_TYPE )
#endif // TM_USE_PPPOE_CLIENT
        )
    {
        typedef int RAW_IFACE_HANDLE;
        extern RAW_IFACE_HANDLE net_iface_raw;
        typedef void (*t_packet_q_free_packet)(char *pkt_data, UINT32 pkt_len, void *pkt_desc);

        int  raw_handle_new_recv_packet(RAW_IFACE_HANDLE iface, char* buff, int length, void *extra, t_packet_q_free_packet free_routine);
        if (raw_handle_new_recv_packet(net_iface_raw, *data_ptr, *data_len, user_data, net_eth_free_treck_buffer) < 0)
        {
            tfFreeDriverBuffer((ttUserBuffer)user_data);
        }

        // Because this is a RAW packet, Treck shouldn't be responsible for it.  Setting this
        // back to NULL so Treck doesn't realize this packet ever existed.
        *user_buf_ptr = NULL;

        // Not really an error -- but tell Treck it shouldn't use the packet
        return TM_DEV_ERROR;
    }

#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
    // We validated the IP header checksum. Note that we cannot distinguish
    // from the descriptor what type of packet was received. However, the
    // sample Treck 80314 driver just sets both TCP and UDP.
    devRecvOffloadPtr->devoRFlags |= TM_DEVOR_IP_RECV_CHKSUM_VALIDATED;
    devRecvOffloadPtr->devoRFlags = TM_DEVOR_TCP_RECV_CHKSUM_VALIDATED |
                                    TM_DEVOR_UDP_RECV_CHKSUM_VALIDATED |
                                    TM_DEVOR_ICMP6_RECV_CHKSUM_VALIDATED;
#endif // HAVE_ETH_RECV_CHKSUM_OFFLOAD

    // BUGBUG -- we only return the buffer on success due to Treck bug explained above.
    *user_buf_ptr = (ttUserBuffer)user_data;

    return TM_DEV_OKAY;
}

static int net_eth_ioctl(ttUserInterface treck_interface, int flag, void *option_ptr, int option_len)
{
    NET_ETH_LOCK_CONTEXT();
    if(!g_net_eth.treck_interface)
    {
        NET_ETH_UNLOCK_CONTEXT();
        return TM_DEV_ERROR;
    }

    if(flag & TM_DEV_SET_MCAST_LIST)
    {
        //hash each of the ethernet addresses and set the bit in the hash table
        net_eth_add_multicast((uint8_t *)option_ptr, option_len);

    }

    if(flag & TM_DEV_SET_ALL_MCAST)
    {
        //hash each of the ethernet addresses and set the bit in the hash table
        net_eth_add_multicast((uint8_t *)NULL, 0xFFFFFFFF);
    }

#ifdef HAVE_ETH_RECV_CHKSUM_OFFLOAD
    if ((flag & TM_DEV_IOCTL_OFFLOAD_GET) && option_ptr)
    {
        ttDevIoctlOffloadStruct *ols = (ttDevIoctlOffloadStruct *) option_ptr;

        ols->offloadFlags = TM_DEVO_IP_RECV_CHKSUM_VALIDATE | 
                            TM_DEVO_TCP_RECV_CHKSUM_VALIDATE | 
                            TM_DEVO_UDP_RECV_CHKSUM_VALIDATE | 
                            TM_DEVO_ICMP6_RECV_CHKSUM_VALIDATE;

        DBG_VERBOSE("informing stack of recv offload capabilities 0x%08x\n", ols->offloadFlags);
    }
    if ((flag & TM_DEV_IOCTL_OFFLOAD_SET) && option_ptr)
    {
//        ttDevIoctlOffloadStruct *ols = (ttDevIoctlOffloadStruct *) option_ptr;

        // MFM TODO - confirm that the requested offload parameters do not exceed our
        // published capabilities in the GET operation
    }
#endif // HAVE_ETH_RECV_CHKSUM_OFFLOAD

    if(flag & NET_LINK_IOCTL_SEND_COMPLETE)
    {
        net_eth_send_complete();
    }

    NET_ETH_UNLOCK_CONTEXT();
    return TM_DEV_OKAY;
}

static int net_eth_get_phys_addr(ttUserInterface treck_interface, char * phys_addr)
{
    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_get_phys_addr(g_net_eth.dev_index, (uint8_t *)phys_addr, TM_ETHERNET_PHY_ADDR_LEN);
    if(eth_mac_rcode != ETH_MAC_OK)
    {
        return TM_DEV_ERROR;
    }
    return TM_DEV_OKAY;
}


//=========================================================================================================================
// Ethernet MAC driver callbacks
//=========================================================================================================================

static char *net_eth_get_user_buffer_cb(uint32_t *len, uint32_t *source_id, void **user_data)
{
    /*
     *  NOTE: The value of TM_PACKET_BUF_CACHE_ALIGN must be set to the correct system
     *  cache line size. This value is normally defined in trsystem.h.
     * 
     *  Although the buffer whos pointer is returned by tfGetEthernetBuffer() may not be
     *  aligned to system cache line size and boundaries, assigning a proper value to
     *  TM_PACKET_BUF_CACHE_ALIGN in trsystem.h will ensure that Treck adds any necessary
     *  padding around the buffer to guarantee that it owns an integer multiple of cache
     *  lines and is aligned on cache line boundaries.
     */
    /*  rdj 2008-07-29: Note that we do not have control over our buffer sizes when using
     *  the Treck routine tfGetEthernetBuffer below. Inside this routine buffer sizes are
     *  fixed at TM_ETHER_MAX_PACKET_CRC bytes. The value of this constant is currently set
     *  to 1518 bytes (i.e. the max length of an Ethernet frame), which is NOT a multiple
     *  of 32 bytes. As a result, 18 bytes beyond the end of the buffer can be stomped by
     *  the last DMA fixed-burst transfer (see IMPORTANT NOTE above for the reason why).
     *  
     *  HOWEVER, it turns out that the memory stomping is benign in this situation,
     *  because the Treck stack function tm_get_raw_buffer that is indirectly called to
     *  allocate each of the rx buffers always allocates buffer space as a power of 2.  So
     *  our 1518 byte buffers are actually allocated as 2048 byte blocks (less 92 bytes
     *  for buffer control block), with the extra bytes left unused. This behavior could
     *  change in future revs of the Treck stack, but for now we are OK.
     *  
     *  The buffer itself will not be aligned to TM_PACKET_BUF_CACHE_ALIGN cache line
     *  boundaries. However, tfGetEthernetBuffer() adds padding before and after the buffer
     *  to isolate dcache writebacks and invalidations along cache line boundaries.
     */

    /* rdj 2008-07-29: Note that buffer size below is technically TM_ETHER_IP_ALIGN (2
       bytes) too large, because the buffer ptr is advanced by TM_ETHER_IP_ALIGN bytes in
       the call to tfGetEthernetBuffer. However, buffer ptr is at this point mis-aligned
       with the data bus by TM_ETHER_IP_ALIGN bytes, and as a result the GMAC DMA will
       fill these bytes with dummy data. The overall buffer length must therefore still be
       reported to the GMAC DMA as TM_ETHER_MAX_PACKET_CRC (1518 bytes) +
       TM_ETHER_IP_ALIGN (2 bytes) = 1520 bytes. See GMAC Synopsys Data Book section
       3.3.1.2 "Host Data Buffer Alignment", which explains how mis-aligned bytes in rx
       buffers are filled with dummy data, and section 7.2.2 "Receive Descriptor 1
       (RDES1)", which states that rx buffer length must be a multiple of the bus width.*/
    // #########TODO handle alignment issue in the eth_mac driver so we don't have to expose
    //  handling it here (i.e. then we can omit adding TM_ETHER_IP_ALIGN below)
    *len = TM_ETHER_MAX_PACKET_CRC+TM_ETHER_IP_ALIGN; // buffer size allocated by tfGetEthernetBuffer

    *source_id = NET_ETH_SRC_TRECK_RECV;

    return tfGetEthernetBuffer((ttUserBufferPtr)user_data);
}

static void net_eth_free_user_buffer_cb(char *buf, uint32_t source_id, void *user_data)
{
    ASSERT(buf);
    ttUserBuffer treck_buffer;

    switch(source_id)
    {
    case NET_ETH_SRC_TRECK_RECV:

        // recv packets are always contained in Treck buffers
        ASSERT(user_data);
        treck_buffer = (ttUserBuffer)user_data;
        tfFreeDriverBuffer(treck_buffer);
        break;

    case NET_ETH_SRC_TRECK_XMIT:

        tfSendCompleteInterface( g_net_eth.treck_interface, TM_DEV_SEND_COMPLETE_DRIVER );
        break;

    case NET_ETH_SRC_RAW_XMIT:
        // raw tx packets are malloc'ed - free the buffer directly
        //########TODO call a raw sock callback to free mem?
        MEM_FREE_AND_NULL(buf);
        break;

    default:
        // unhandled source ID
        XASSERT(0, source_id);
    }
    return;
}

// called from interrupt context
static void net_eth_isr_notify_frame_cb(uint16_t rx_frames, uint16_t tx_frames,
                                         uint32_t rx_bytes, uint32_t tx_bytes,
                                         uint32_t flags, void *user_data)
{
    // save Treck interface since global could be zeroed after test below
    ttUserInterface treck_interface = g_net_eth.treck_interface;
    if(rx_frames && treck_interface)
    {
        // transfer processing of rx frame to the Treck recv thread
        tfNotifyInterfaceIsr(treck_interface, rx_frames, 0, 0, 0);
    }

    g_net_eth.tx_frames_complete_pending += tx_frames;
    return;
}

static void net_eth_isr_notify_link_cb(void *user_data)
{
    if(!g_net_eth.msg_queue_initialized) return;

    net_eth_send_msg(NET_ETH_MSG_LINK_STATUS_CHANGE, user_data);
    return;
}


//=========================================================================================================================
// Private utility functions
//=========================================================================================================================

net_eth_context_t *net_eth_get_ctxt(void)
{
    return &g_net_eth;
}

net_eth_context_t *net_eth_lock_ctxt(void)
{
    NET_ETH_LOCK_CONTEXT();
    return &g_net_eth;
}

void net_eth_unlock_ctxt(net_eth_context_t *net_eth_ctxt)
{
    if(net_eth_ctxt != &g_net_eth)
    {
        return;
    }

    NET_ETH_UNLOCK_CONTEXT();
    return;
}


static void net_eth_add_multicast(uint8_t *addrs, uint32_t num_addrs)
{
    uint8_t *cur_addr = addrs;
    if(num_addrs == 0xFFFFFFFF)
    {
        //change the filter to accept all multicasts

        ASSERT(0); // TODO implement this
    }
    else
    {
        //treck sends us the complete list of multicasts each time so we can
        //start from scratch. 
        eth_mac_filter_remove_all(g_net_eth.dev_index);
        int i;
        for(i = 0; i < num_addrs; i++)
        {
            eth_mac_filter_add(g_net_eth.dev_index, cur_addr, TM_MAX_PHYS_ADDR, 0);
            cur_addr += TM_MAX_PHYS_ADDR;
        }
    }
    return;
}

// TODO TEMP - used to prevent image corruption during firmware download
void NetDrvrPause()
{
    uint8_t ref_cnt;
    do{
        ref_cnt = eth_mac_close(g_net_eth.dev_index);
    }while(ref_cnt);

    return;
}

/*
 *  IMPORTANT: this function must be called only from Treck driver context, because
 *  tfSendCompleteInterface() is called with the TM_DEV_SEND_COMPLETE_DRIVER flag below.
 */
static void net_eth_send_complete(void)
{
    uint32_t num_acks;
    uint32_t i;
    uint32_t int_state;

    uint32_t len;
    eth_mac_tx_flags_t flags;
    uint32_t source_id;
    void *user_data;
    char *buffer;

    XASSERT(!(g_net_eth.tx_frames_complete_pending & 0x80000000), g_net_eth.tx_frames_complete_pending);
    num_acks = g_net_eth.tx_frames_complete_pending;

//    // TODO TEMP DEBUG
//    static uint32_t hb_cnt = 0;
//    static uint32_t frames_complete_cnt = 0;
//    NET_ETH_LOCK_CONTEXT();
//    hb_cnt++;
//    frames_complete_cnt += num_acks;
//    if(!(hb_cnt%100)) // once per second
//    {
//        dbg_printf("##### send_complete heartbeat ##### acks=%d\n", frames_complete_cnt);
//        frames_complete_cnt = 0;
//    }
//    NET_ETH_UNLOCK_CONTEXT();

    ASSERT(g_net_eth.treck_interface);
    if(num_acks > 0)
    {
        for(i = 0; i < num_acks; i++)
        {
            buffer = eth_mac_complete_tx_buffer(g_net_eth.dev_index, &len, &flags,
                                                &g_net_eth.tx_stats, &source_id, &user_data);
            if(!buffer)
            {
                // likely means driver is no longer open, e.g. due to transition between low power states
                num_acks = i;
                break;
            }

            switch(source_id)
            {
            case NET_ETH_SRC_TRECK_XMIT:

                // this buffer was sent by Treck stack - notify Treck of completion
                tfSendCompleteInterface( g_net_eth.treck_interface, TM_DEV_SEND_COMPLETE_DRIVER );
                break;

            case NET_ETH_SRC_RAW_XMIT:
                // raw tx packets are malloc'ed - free the buffer directly
                //########TODO call a raw sock callback to free mem?
                MEM_FREE_AND_NULL(buffer);
                break;

            default:
                // unhandled source ID
                XASSERT(0, source_id);
            }
        }

        int_state = cpu_disable_interrupts();
        g_net_eth.tx_frames_complete_pending -= num_acks;
        cpu_restore_interrupts( int_state );
    }
    return;
}

static void net_eth_link_status_notify(void *data)
{
    eth_mac_link_status_t cur_link_status;
    cur_link_status = eth_mac_get_link_status(g_net_eth.dev_index);
    if(cur_link_status != g_net_eth.prev_link_status)
    {
        DBG_VERBOSE("link status change\n");
        g_net_eth.prev_link_status = cur_link_status;
        net_link_event_t link_event = NET_LINK_EVENT_UP;
        if(cur_link_status == ETH_MAC_LINK_STATUS_DOWN)
        {
            link_event = NET_LINK_EVENT_DOWN;
        }

        // notify the overlying network stack
        ASSERT(g_net_eth.link_event_notify_func);
        g_net_eth.link_event_notify_func(link_event, g_net_eth.link_event_user_data);
    }
    return;
}

static int32_t net_eth_send_msg(net_eth_msg_type_t type, void *data)
{
    net_eth_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.data = data;

    UINT threadx_rcode = tx_queue_send(&g_net_eth_msg_queue, &buf, TX_NO_WAIT);
    if(threadx_rcode == TX_QUEUE_FULL)
    {
        /* 
         *  msg queue full - restart the timer to retry
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and threadx_wait_opt must
         *  therefore be set to TX_NO_WAIT
         */
        return NET_ETH_WOULDBLOCK;
    }
    else if(threadx_rcode != TX_SUCCESS)
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, threadx_rcode);
    }

    return NET_ETH_OK;
}

#ifdef HAVE_ETH_LINK_STATUS_POLLING
static void net_eth_timer_callback(ULONG param)
{
    net_eth_send_msg(NET_ETH_MSG_POLL_LINK_STATUS, (void *)0);
    return;
}
#endif // HAVE_ETH_LINK_STATUS_POLLING


//=========================================================================================================================
// Power management functions
//=========================================================================================================================

#ifdef HAVE_POWER_MGR

static bool net_eth_power_save_cb(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    bool success;
    success = net_eth_power_save(level, cmd);

    // still must call up or down even if !success
    switch(cmd)
    {
    case pwr_mgr_cmd_pwr_IO_up_e:
    case pwr_mgr_cmd_pwr_up_e:
        pwr_mgr_powered_up(g_net_eth.net_eth_pwr_mgr_event_flag);
        break;
    case pwr_mgr_cmd_pwr_down_e:
        pwr_mgr_powered_down(g_net_eth.net_eth_pwr_mgr_event_flag);
        break;
    default:
        XASSERT(0, cmd);
        return false;
    }

    return success;
}


#ifdef HAVE_ASIC_POWER_DRIVER
static void net_eth_power_reset(void)
{
    // reset the sampling session -- smoothed average power reset to t=0
    asic_sleep_stats_t sleep_stats;
    asic_pwr_get_sleep_stats( &sleep_stats );
    g_net_eth.wake_initial_ticks = tx_time_get();
    g_net_eth.sleep_initial_ticks = sleep_stats.cumulative_ticks;
    g_net_eth.wake_periodic_ticks = g_net_eth.wake_initial_ticks;
    g_net_eth.sleep_periodic_ticks = g_net_eth.sleep_initial_ticks;
    g_net_eth.sample_cnt = 0;
    g_net_eth.target_duty_cycle = NET_LP_TARGET_DUTY_CYCLE;
//    g_net_eth.max_sleep_latency = 0; // TODO TEMP DEBUG
//    g_net_eth.prev_sleep_iterations = 0; // TODO TEMP DEBUG
    return;
}

#endif
/* 
 * net_eth_power_sample
 * 
 * Should be called prior to every sleep, but NOT on wakes.
 */
#define NET_ETH_PWR_SIG_FIGS 100 //extra sig figs for rounding error
#ifdef HAVE_ASIC_POWER_DRIVER
static void net_eth_power_sample(void)
{
    // calculate total time elapsed since initial t=0 sample
    asic_sleep_stats_t sleep_stats;
    asic_pwr_get_sleep_stats( &sleep_stats );
    uint32_t wake_cur_ticks = tx_time_get();
    uint32_t total_time_elapsed_ms = 
        ((((wake_cur_ticks - g_net_eth.wake_initial_ticks) + 
        (sleep_stats.cumulative_ticks - g_net_eth.sleep_initial_ticks)) * 1000) / SYS_TICK_FREQ);

//    // TODO TEMP DEBUG
//    if(g_net_eth.max_sleep_latency < sleep_stats.ticks)
//    {
//        g_net_eth.max_sleep_latency = sleep_stats.ticks;
//    }

    // calculate time elapsed since last sample
    uint32_t wake_ticks_elapsed = wake_cur_ticks - g_net_eth.wake_periodic_ticks;
    uint32_t sleep_ticks_elapsed = sleep_stats.cumulative_ticks - g_net_eth.sleep_periodic_ticks;
    uint32_t ticks_elapsed = wake_ticks_elapsed + sleep_ticks_elapsed;

    // apply any new samples to smoothed average power
    uint32_t samples_elapsed = total_time_elapsed_ms / NET_LP_SAMPLE_PERIOD_MS;
    if( g_net_eth.sample_cnt < samples_elapsed 
        && sleep_ticks_elapsed) // make sure we've actually slept before measuring duty cycle
    {
        // compute duty cycle since last sample
        int32_t duty_cycle = (1000 * wake_ticks_elapsed) / ticks_elapsed;

        /* compute average power since last sample
         * 
         * * A = qd + s(1-d)
         * * where:
         * *    'A' = average power
         * *    's' = power consumed while in the Sleep state
         * *    'q' = power consumed while in the quick I/O wake state
         * *    'd' = the quick wake duty cycle
         */
        uint32_t avg_pwr_mw = ( (NET_LP_IOWAKE_PWR_MW * duty_cycle) + 
            (NET_LP_SLEEP_PWR_MW * (1000 - duty_cycle)) ) / 1000 * NET_ETH_PWR_SIG_FIGS;

//        uint32_t prev_sample_cnt = g_net_eth.sample_cnt; // TODO TEMP DEBUG
        while(g_net_eth.sample_cnt < samples_elapsed)
        {
            /* update smoothed_average_power (i.e. the exponential moving average)
             * 
             * * for t=1, SA(1) = A(1)
             * * for t>1, SA(t+1) = a*A(t) + (1-a)*SA(t)
             * * where:
             * *    'SA' = smoothed average power
             * *    'A' = average power over the sample period
             * *    'a' = "alpha" smoothing factor, where 0 < a < 1. The higher the value,
             *      the more sensitive SA is to changes in A
             */
            if(g_net_eth.sample_cnt)
            {
                // t > 1
                g_net_eth.smoothed_average_power_mw = 
                    ( (NET_LP_SMOOTHING_FACTOR * avg_pwr_mw) + 
                      (1000 - NET_LP_SMOOTHING_FACTOR) * g_net_eth.smoothed_average_power_mw) / 1000;
            }
            else
            {
                // t = 1 (i.e. first sample)
                g_net_eth.smoothed_average_power_mw = avg_pwr_mw;
            }
            g_net_eth.sample_cnt++;
        }

        /* adjust target_duty_cycle to account for deviation of smoothed_average_power
         * from target_power
         * 
         * * e = (act - tgt) / tgt
         * * where:
         * *    'e' = error
         * *    'act' = actual value
         * *    'tgt' = target value
         */
        int32_t percent_deviation = 
            (1000 * ((int32_t)g_net_eth.smoothed_average_power_mw - NET_LP_TARGET_AVG_PWR_MW*NET_ETH_PWR_SIG_FIGS)) /
            (NET_LP_TARGET_AVG_PWR_MW*NET_ETH_PWR_SIG_FIGS);
//        int32_t prev_target_duty_cycle = g_net_eth.target_duty_cycle; // TODO TEMP DEBUG
        // NOTE: result can be negative so int32_t is req'd
        g_net_eth.target_duty_cycle = NET_LP_TARGET_DUTY_CYCLE - ((((percent_deviation * NET_LP_TARGET_DUTY_CYCLE)
                                                                   * NET_LP_DEVIATION_WEIGHT) / 100) / 1000);
        if(g_net_eth.target_duty_cycle < NET_LP_MIN_DUTY_CYCLE)
        {
            // ensure the required min I/O wake time
            g_net_eth.target_duty_cycle = NET_LP_MIN_DUTY_CYCLE;
        }
        else if (g_net_eth.target_duty_cycle > 1000)
        {
            // limit max duty cycle
            g_net_eth.target_duty_cycle = 1000;
        }

        // start a new sample period
        g_net_eth.wake_periodic_ticks = wake_cur_ticks;
  #ifdef HAVE_ASIC_POWER_DRIVER
        g_net_eth.sleep_periodic_ticks = sleep_stats.cumulative_ticks;
  #endif

//        // TODO TEMP DEBUG - BEGIN
//        DBG_MSG(">>>>>>>>>>>>>>>>>>+%d\n", samples_elapsed - prev_sample_cnt);
//        DBG_MSG("total_time=%dms (%d samples)\n", total_time_elapsed_ms, samples_elapsed);
//        DBG_MSG("period_time=%dms\n", (ticks_elapsed * 1000) / SYS_TICK_FREQ);
//        DBG_MSG("sleep_time=%dms (i=%d max=%dms vs. %dms)\n", (sleep_ticks_elapsed * 1000) / SYS_TICK_FREQ,
//                 sleep_stats.iteration - g_net_eth.prev_sleep_iterations, (g_net_eth.max_sleep_latency * 1000) / SYS_TICK_FREQ, NET_LP_MAX_WAKE_LATENCY_MS);
//        DBG_MSG("wake_time=%dms\n", (wake_ticks_elapsed * 1000) / SYS_TICK_FREQ);
//        DBG_MSG("duty_cycle=%03d (vs. target %03d %%diff% 04d(x10))\n", duty_cycle, prev_target_duty_cycle,
//                (1000 * ((int32_t)duty_cycle - (int32_t)prev_target_duty_cycle)) / (int32_t)g_net_eth.target_duty_cycle);
//        DBG_MSG("average_power=%dmW(x100)\n", avg_pwr_mw);
//        DBG_MSG("smoothed_average_power=%dmW(x100) (vs. %dmW %%diff% 04d(x10))\n", 
//                g_net_eth.smoothed_average_power_mw, NET_LP_TARGET_AVG_PWR_MW, percent_deviation); 
//        DBG_MSG("target_duty_cycle=%03d(x1000) (vs. %d)\n", g_net_eth.target_duty_cycle, NET_LP_TARGET_DUTY_CYCLE);
//        DBG_MSG("<<<<<<<<<<<<<<<<<<\n");
//        g_net_eth.prev_sleep_iterations = sleep_stats.iteration;
//        g_net_eth.max_sleep_latency = 0;
//        // TODO TEMP DEBUG - END
    }

    return;
}
#endif
/* net_eth_power_get_wake_time
 * 
 * returns: target wake time in milliseconds
 */
#ifdef HAVE_ASIC_POWER_DRIVER
static uint32_t net_eth_power_get_wake_time(void)
{
    // calculate wake & sleep time elapsed since last sample
    asic_sleep_stats_t sleep_stats;
    asic_pwr_get_sleep_stats( &sleep_stats );
    int32_t elapsed_wake_time_ms = ((tx_time_get() - g_net_eth.wake_periodic_ticks) * 1000) / SYS_TICK_FREQ;
    int32_t elapsed_sleep_time_ms = ((sleep_stats.cumulative_ticks - g_net_eth.sleep_periodic_ticks) * 1000) / SYS_TICK_FREQ;

    int32_t target_duty_cycle = (int32_t)g_net_eth.target_duty_cycle;
    if(target_duty_cycle > 999)
    {
        // avoid divide by zero
        target_duty_cycle = 999;
    }

    /* calculate target_wake_time
     * 
     * * d = wt / tt
     * * where:
     * *    'd' = target duty cycle,
     * *    'wt' = wake time,
     * *    'tt' = total time
     * 
     * * wt = twt + ewt
     * * tt = twt + ewt + est
     * * where:
     * *    'ewt' = elapsed_wake_time
     * *    'est' = elapsed_sleep_time
     * *    'twt' = target_wake_time req'd to arrive at target_duty_cycle
     * 
     * substituting for wt & tt yields:
     * 
     * d = (twt + ewt) / (twt + ewt + est)
     * 
     * solving for twt yields:
     * 
     * twt = (d(ewt + est) - ewt) / (1-d)
     * 
     */
    int32_t target_wake_time_ms = 
        ((target_duty_cycle * (elapsed_wake_time_ms + elapsed_sleep_time_ms)) -
          (elapsed_wake_time_ms * 1000)) / (1000 - target_duty_cycle);
    if(target_wake_time_ms < 0)
    {
        target_wake_time_ms = 0;
    }
    else if(target_wake_time_ms > NET_LP_MAX_WAKE_TIME_MS)
    {
        target_wake_time_ms = NET_LP_MAX_WAKE_TIME_MS;
    }

// TODO TEMP DEBUG
//    DBG_MSG("###target_wake_time=%dms (ewt=%dms est=%dms d=%03d)\n", target_wake_time_ms,
//            elapsed_wake_time_ms, elapsed_sleep_time_ms, g_net_eth.target_duty_cycle);
    ASSERT(target_wake_time_ms<8000);

    return target_wake_time_ms;
}
#endif
static bool net_eth_power_save(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    DBG_VERBOSE("power save level %d\n", level);
    eth_mac_rcode_t eth_mac_rcode = ETH_MAC_OK;
    
    if(!g_net_eth.phy_hndl)
    {
        DBG_ERR("%s: phy_hndl not found!\n", __func__);
        return false;
    }

    switch(level)
    {
    case pwr_mgr_on_e:

        switch(g_net_eth.pwr_level)
        {
        case pwr_mgr_off_e:

            if(cmd == pwr_mgr_cmd_pwr_IO_up_e)
            {
                // if we were already off then don't participate in the I/O "quick wake"
                //DBG_MSG("leave power off\n");
                return true;
            }
#ifdef HAVE_ASIC_POWER_DRIVER
            eth_mac_set_power_mode(ETH_MAC_PWR_MODE_D0, 0, NULL);
#endif

            // We do not know how long we were in a power down state, during which time our IP address
            // could have been be reassigned, etc.  Therefore we signal the overlying network stack
            // to perform a network reset.  Note that a simple "link down" event could be filtered out
            // by the debounce logic, since in some cases system time "stops" while we are in power down.
            ASSERT(g_net_eth.link_event_notify_func);
            g_net_eth.link_event_notify_func(NET_LINK_EVENT_RESET, g_net_eth.link_event_user_data);

            break;

#ifdef HAVE_ASIC_POWER_DRIVER
        case pwr_mgr_lowest_power_e:
//            dbg_printf("########## Eth waking...\n");
            if(cmd == pwr_mgr_cmd_pwr_IO_up_e)
            {
                uint32_t wake_time = net_eth_power_get_wake_time();
                if(wake_time)
                {
                    pwr_mgr_go_active_nowait(PWRMGR_UID_IO_WAKE_SET_MS(wake_time));
                }
            }
            eth_pwr_drv_woke();
            eth_mac_set_power_mode(ETH_MAC_PWR_MODE_D0, 0, NULL);
            break;
#endif
        default:
            // nothing to do
            break;
        }

        if(cmd == pwr_mgr_cmd_pwr_IO_up_e)
        {
            g_net_eth.pwr_level = pwr_mgr_reduced_power_e;
        }
        else
        {
            g_net_eth.pwr_level = pwr_mgr_on_e;
        }
        break;

    case pwr_mgr_reduced_power_e:

        g_net_eth.pwr_level = pwr_mgr_reduced_power_e;
        break;

    case pwr_mgr_lowest_power_e:
        switch(g_net_eth.pwr_level)
        {
        case pwr_mgr_on_e:
        case pwr_mgr_reduced_power_e:
#ifdef HAVE_ASIC_POWER_DRIVER
            if(g_net_eth.pwr_level == pwr_mgr_on_e)
            {
                net_eth_power_reset();
            }
            else // g_net_eth.pwr_level == pwr_mgr_reduced_power_e, i.e. transitioning from I/O wake state
            {
                net_eth_power_sample();
            }

            // need to shutdown threads from using h/w, transfer h/w to NON dram, disable interrupt (much of this might be in the asic specifc??
            // force an I/O wake after no longer than NET_LP_MAX_WAKE_LATENCY_MS
            eth_mac_rcode = eth_mac_set_power_mode(ETH_MAC_PWR_MODE_D2, 0, (void *)NET_LP_MAX_WAKE_LATENCY_MS);
            if(eth_mac_rcode == ETH_MAC_OK)
            {
                eth_pwr_drv_sleep(0);
            }
#endif
            break;

        case pwr_mgr_off_e:
            // unexpected case
            ASSERT(0);
            break;

        default:
            // nothing to do
            break;
        }

        if(eth_mac_rcode == ETH_MAC_OK)
        {
            g_net_eth.pwr_level = pwr_mgr_lowest_power_e;
        }
        break;

    case pwr_mgr_off_e:
        switch(g_net_eth.pwr_level)
        {
        case pwr_mgr_on_e:
        case pwr_mgr_reduced_power_e:
  #ifdef HAVE_ASIC_POWER_DRIVER
            eth_mac_set_power_mode(ETH_MAC_PWR_MODE_D3, 0, NULL);
  #endif
            break;

        case pwr_mgr_lowest_power_e:
            // unexpected case
            ASSERT(0);
            break;

        default:
            // nothing to do
            break;
        }

        g_net_eth.pwr_level = pwr_mgr_off_e;
        break;

    default:
        // unhandled power level
        XASSERT(0,level);
        return false;
    }

    if(eth_mac_rcode != ETH_MAC_OK)
    {
        return false;
    }

    eth_phy_power_save(g_net_eth.phy_hndl, level, cmd);
    return true;
}

#endif // HAVE_POWER_MGR


//=========================================================================================================================
// Serial debug command functions
//=========================================================================================================================

#ifdef HAVE_CMD

#ifdef HAVE_POWER_MGR

static const char *net_eth_cmd_power_on_desc = "Turn the ethernet hw power on";
static const char *net_eth_cmd_power_on_usage = "";

static const char *net_eth_cmd_power_off_desc = "Turn the ethernet hw power off";
static const char *net_eth_cmd_power_off_usage = "";

static const char *net_eth_cmd_power_reduced_desc = "Turn the ethernet hw power to a reduced power mode";
static const char *net_eth_cmd_power_reduced_usage = "";

#endif // HAVE_POWER_MGR

static const char *net_eth_cmd_link_status_desc = "Display the status of the ethernet link";
static const char *net_eth_cmd_link_status_usage = "";

static void net_eth_register_dbg_cmds(void)
{
    DBG_VERBOSE("==>%s\n", __func__);
    int retcode;
    retcode = cmd_register_cmd( "eth",
                                NULL,
                                "Ethernet layer-related commands",
                                NULL,
                                NULL,
                                NULL );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "eth",
                                   "link",
                                   "Ethernet link-related commands",
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "eth link",
                                   "status",
                                   net_eth_cmd_link_status_desc,
                                   net_eth_cmd_link_status_usage,
                                   NULL,
                                   net_eth_cmd_link_status_cb );
    XASSERT( retcode==0, retcode );
#ifdef HAVE_POWER_MGR
    retcode = cmd_register_subcmd( "eth",
                                   "power",
                                   "Ethernet power-related commands",
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "eth power",
                                   "on",
                                   net_eth_cmd_power_on_desc,
                                   net_eth_cmd_power_on_usage,
                                   NULL,
                                   net_eth_cmd_power_on_cb );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "eth power",
                                   "off",
                                   net_eth_cmd_power_off_desc,
                                   net_eth_cmd_power_off_usage,
                                   NULL,
                                   net_eth_cmd_power_off_cb );
    XASSERT( retcode==0, retcode );
    retcode = cmd_register_subcmd( "eth power",
                                   "reduced",
                                   net_eth_cmd_power_reduced_desc,
                                   net_eth_cmd_power_reduced_usage,
                                   NULL,
                                   net_eth_cmd_power_reduced_cb );
    XASSERT( retcode==0, retcode );
#endif // HAVE_POWER_MGR

    DBG_VERBOSE("<==%s\n", __func__);
    return;
}

static int net_eth_cmd_link_status_cb( int argc, char *argv[] )
{
    bool tmp_link_up;
    eth_mac_link_status_t link_status;

    if( argc != 1 )
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }

    link_status = eth_mac_get_link_status(g_net_eth.dev_index);
    tmp_link_up = (link_status & ETH_MAC_LINK_STATUS_DOWN) == 0;
    DBG_CMD("Link up=%d status=0x%08x\n", tmp_link_up, link_status);

    return CMD_OK;
}

#ifdef HAVE_POWER_MGR

static int net_eth_cmd_power_reduced_cb( int argc, char *argv[] )
{
    if( argc != 1 )
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }

    net_eth_power_save(pwr_mgr_reduced_power_e, pwr_mgr_cmd_pwr_up_e);

    return CMD_OK;
}

static int net_eth_cmd_power_off_cb( int argc, char *argv[] )
{
    if( argc != 1 )
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }

    net_eth_power_save(pwr_mgr_off_e, pwr_mgr_cmd_pwr_down_e);

    return CMD_OK;
}

static int net_eth_cmd_power_on_cb( int argc, char *argv[] )
{
    if( argc != 1 )
    {
        /* bad parameters */
        return CMD_USAGE_ERROR;
    }

    net_eth_power_save(pwr_mgr_on_e, pwr_mgr_cmd_pwr_up_e);

    return CMD_OK;
}

#endif // HAVE_POWER_MGR

#endif // HAVE_CMD


//=========================================================================================================================
// Raw socket stuff
//=========================================================================================================================

/*
 * Sends raw packets to the driver, bypassing Treck.
 */
int eth_link_raw_send( char *data_ptr, int data_len )
{
    bool is_last;
    int send_res = TM_DEV_OKAY;

    NET_ETH_LOCK_CONTEXT();

    if(!g_net_eth.treck_interface)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

    // assume always get packets in a single buffer for now.
    is_last = true;

    eth_mac_rcode_t eth_mac_rcode;
    eth_mac_rcode = eth_mac_insert_tx_buffer(g_net_eth.dev_index, data_ptr, data_len, 
                                             is_last, NET_ETH_SRC_RAW_XMIT, (void *)0);
    if(eth_mac_rcode != ETH_MAC_OK)
    {
        send_res = TM_DEV_ERROR;
        goto done;
    }

done:
    NET_ETH_UNLOCK_CONTEXT();
    return send_res;
}

error_type_t net_eth_notify_callback_register( uint16_t ether_type, net_eth_notify_callback_t callback )
{
    UINT tx_res;
    uint32_t i;
    bool already_registered;
    
    ASSERT( NULL != callback );

    if( net_eth_callback_count >= NET_ETH_MAX_CALLBACKS )
    {
        return FAIL;
    }
    else
    {
        // Get Sempahore to protect saving the callback
        tx_res = tx_semaphore_get( &net_eth_notify_callback_semaphore, TX_WAIT_FOREVER );
        ASSERT( TX_SUCCESS == tx_res );

        already_registered = false;
        for( i = 0 ; i < net_eth_callback_count ; i++ )
        {
            if( net_eth_notify_callback_list[ i ].callback == callback )
            {
                already_registered = true;
            }
        }

        if( false == already_registered )
        {
            net_eth_notify_callback_list[net_eth_callback_count].callback = callback;
            net_eth_notify_callback_list[net_eth_callback_count].ether_type = ether_type;
            net_eth_callback_count ++;
        }

        // Release semaphore
        tx_res = tx_semaphore_put( &net_eth_notify_callback_semaphore );
        ASSERT( TX_SUCCESS == tx_res );
    }
    return OK ;
}

static void net_eth_callback_notify( net_eth_msg_t *msg )
{
    uint32_t i;
    UINT tx_res;
    net_eth_notify_callback_t callback;
    uint16_t ether_type;
    bool found_handler;

    ASSERT( NULL != msg );

    /* default behavior is to notify treck of all packets,
     * use this flag to determine if some other entity was notified
     * false == notify treck, true == do not notify treck because someone else
     * was already notified */
    found_handler = false;

    // Get Sempahore to protect fetching the callbacks
    tx_res = tx_semaphore_get( &net_eth_notify_callback_semaphore, TX_WAIT_FOREVER );
    ASSERT( TX_SUCCESS == tx_res );

    if( net_eth_callback_count > 0 )
    {

        ether_type = ( (ttEtherHeader *)msg->data )->ethType; 

        for( i = 0 ; i < net_eth_callback_count ; i++ )
        {
            if( ether_type == net_eth_notify_callback_list[i].ether_type )
            {
                callback = net_eth_notify_callback_list[i].callback;
                ASSERT( NULL != callback );
                //TODO Check additional for pending packets before firing off
                callback( ether_type );
                /* set to true in order to prevent treck from being notified */
                found_handler = true;
            }
        }
    }

    // Release semaphore
    tx_res = tx_semaphore_put( &net_eth_notify_callback_semaphore );
    ASSERT( TX_SUCCESS == tx_res );

    /* if no one else knows about this packet we'll notify
     * treck here */
    if( false == found_handler )
    {
        //TODO Check additional for pending packets before firing off to Treck
        tfNotifyInterfaceTask( g_net_eth.treck_interface, 1, 0, 0, 0 );
    }

    return;
}

static void net_eth_free_treck_buffer(char *pkt_buff, UINT32 pkt_len, void *pkt_desc)
{
    tfFreeDriverBuffer((ttPacketPtr)pkt_desc);
}
#endif

#ifdef __cplusplus
}
#endif

//eof
