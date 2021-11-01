/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_link.c
  * 
  *  This file contains the overall state machine network 'link' component.
  * 
  */

//    Launches a 'net-link' thread to handle network 'link' status updates
//  via the net_link_init() entrypoint.  

#ifdef __cplusplus
extern "C" {
#endif

// public headers
#include <string.h>
#include <stdint.h>

// common headers
#include "error_types.h"
#include "os_network.h" // treck-port includes
#include "net_api.h"
#include "net_intf_api.h" // get_intf_mac_addr()
#include "net_link_api.h"
#include "net_iface.h"

// Debug configuration
#define DBG_PRFX "net.link: "
#define DBG_ERR_ON
#define DBG_ON /* DBG_MSG */
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

// local debug
#define EXTRA_LOCAL_DEBUG 0 /* additional dbg when enabled (non-zero) */
#if EXTRA_LOCAL_DEBUG // debug
#define adv_state(link, updated_state) \
{ char* link_name = (link?link->name:"<unbound link>"); dbg_printf("adv(link %s x%x state %d -> %d)\n", link_name, link, link->state, updated_state); link->state = updated_state; }
#define adv_status(llink, updated_status) \
{ dbg_printf("adv(llink x%x state %d -> %d)\n", llink, llink->status, updated_status); llink->status = updated_status; }
#else
#define adv_state(link, updated_state)     link->state = updated_state
#define adv_status(llink, updated_status) llink->status = updated_status
#define DBG_IGNORED(...) { (void)0; } /* optimize out */
#endif

const char* g_net_link_state_strs[] = 
{
    "NET_LINK_STATE_DOWN_DELAY",
    "NET_LINK_STATE_DOWN_PENDING",
    "NET_LINK_STATE_DOWN",
    "NET_LINK_STATE_UP_DELAY",
    "NET_LINK_STATE_UP",
};

const char* g_net_link_msg_strs[] = 
{
    "NET_LINK_MSG_UP_EVENT",    
    "NET_LINK_MSG_UP_POLL",     
    "NET_LINK_MSG_UP",          
    "NET_LINK_MSG_DOWN_EVENT",  
    "NET_LINK_MSG_DOWN_PENDING",
    "NET_LINK_MSG_DOWN",        
    "NET_LINK_MSG_RESET_EVENT",
    "NET_LINK_MSG_ENABLE_LINKS",
};
typedef error_type_t net_rcode_t; // revisit

// netlink dbg - nlmsg headers etc..
typedef struct nlmsghdr nlmsghdr_t;
typedef struct ifinfomsg ifinfomsg_t;
typedef struct rtattr rtattr_t; // msg attr

// global link-state desc
net_link_context_t g_net_link_ctxt =
{
    .link_cnt = 0, // runtime determined
    .links = {NULL},
    .logical_link_cnt = 0,
    .logical_links = {NULL},
    .enumerated = false,
    .oem_config.func_table = &g_net_link_ctxt.oem_func_table
};

// net link state machine process data
#define NET_LINK_THREAD_STACK_SIZE PTHREAD_STACK_MIN
static pthread_t net_link_thread; // thread on which state machine runs
static ALIGN8 uint8_t net_link_stack[NET_LINK_THREAD_STACK_SIZE];
net_link_msg_buf_t g_net_link_msg_buf[NET_LINK_MQ_NUM_MSG_BUFS];
mqd_t              g_net_link_msg_queue; // link-status msg queue control block

// net link event process data
static pthread_t net_link_event_thread; // thread on which event-listener runs
mqd_t              g_net_link_event_msg_queue; // link-event msg queue control block
static ALIGN8 uint8_t net_link_event_stack[NET_LINK_THREAD_STACK_SIZE];
net_link_msg_buf_t g_net_link_event_msg_buf[NET_LINK_MQ_NUM_MSG_BUFS]; // link-event msg queue buffer

// prototypes
void* link_status_change_event_listener(void* reply);
void launch_file_change_listener(void);
void link_event_list_insert(net_link_event_node_t *node);
void link_event_list_remove(net_link_event_node_t *node);

//=========================================================================================================================
// Local routines
//=========================================================================================================================

// state loop
static void* net_link_state_loop(void* unused);

// local link management routines
static net_rcode_t net_link_enumerate_links(void);
void net_link_event_handler(net_link_event_t link_event, void *user_data);
void link_up_timer_func(void *user_data);
void activate_link_up_timer(net_link_t *link);
void link_down_timer_func(void *user_data);
void activate_link_down_timer(net_link_t *link);

// local utility routines
static net_link_logical_t *net_link_recover_local_context(ttUserInterface treck_interface);
net_rcode_t net_link_send_msg_wait_opt(
    net_link_msg_type_t type,
    net_link_t *link,
    void *udata,
    void *data2,
    unsigned long threadx_wait_opt);
net_link_logical_t *net_link_logical_lookup_by_handle(net_link_handle_t link_handle);
void link_enable_timer_func(void *user_data);
static void net_enable_exclusive_links(net_link_logical_t *llink);
static void net_disable_exclusive_links(net_link_t *up_link);
static void net_link_insert_link(net_link_logical_t *llink, net_link_t *net_link);
net_link_t *net_link_lookup_by_name(const char *link_name);
net_link_t *net_link_lookup_by_link_index(unsigned int link_index);
void net_link_set_priority_link_up(net_link_logical_t *llink);

// Treck interface callbacks
static int net_link_shared_open(ttUserInterface treck_interface);
static int net_link_shared_close(ttUserInterface treck_interface);
static int net_link_shared_send(ttUserInterface treck_interface, char *data_ptr, int data_len, int flag);
static int net_link_shared_recv(ttUserInterface treck_interface, char **data_ptr, int *data_len, ttUserBufferPtr user_buff_ptr
#ifdef TM_DEV_RECV_OFFLOAD // this define must be link agnostic
                          , ttDevRecvOffloadPtr offload_ptr
#endif // TM_DEV_RECV_OFFLOAD
                        );
static int net_link_shared_ioctl(ttUserInterface treck_interface, int flag, void *option_ptr, int option_len);
static int net_link_shared_get_phys_addr(ttUserInterface treck_interface, char * phys_addr);

//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

#define NET_LINK_LOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_lock(&g_net_link_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        }

#define NET_LINK_UNLOCK_CONTEXT() \
        { \
            unsigned int os_rcode = pthread_mutex_unlock(&g_net_link_ctxt.mtx); \
            XASSERT(os_rcode == OS_SUCCESS, os_rcode); \
            UNUSED_VAR(os_rcode); \
        }


// null_mac() -- true or false;  true if all scanned mac bytes are null
bool null_mac(uint8_t* mac, int mac_len)
{
    if (mac_len <= 0) return true;
    int i = 0;
    while (mac_len-- > 0) if (mac[i++]) return false;
    return true;
}

//=========================================================================================================================
// Link (mac)-layer Module entry point
//=========================================================================================================================

void net_link_init(void)
{
    net_rcode_t net_rcode;
    unsigned int os_rcode = ~NET_OK;

    // add 'net' cmd interface
    register_net_cmds();

    os_rcode = posix_mutex_init(&g_net_link_ctxt.mtx);
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating g_net_link_ctxt mutex (os_rcode=0x%02x)\n", os_rcode);
        DBG_ASSERT(0);
    }

    /*
     * create net link state machine message queue
     */

    // validation of msg buf size
    DBG_ASSERT(sizeof(net_link_msg_buf_t) == NET_LINK_MQ_MSG_BUF_BYTES);

    posix_create_message_queue(&g_net_link_msg_queue, NET_LINK_MQ_NAME,
                                                            NET_LINK_MQ_NUM_MSG_BUFS,
                                                            OS_4_ULONG /*NET_LINK_MQ_MSG_BUF_SIZE*/);

    net_rcode = net_link_enumerate_links();
    if(net_rcode != NET_OK)
    {
        goto error;
    }

    /*
     * create net link status state machine thread
     */
    os_rcode = posix_create_thread(&net_link_thread, net_link_state_loop, (void *)0, NET_MAC_THREAD_NAME,
                                        &net_link_stack, NET_LINK_THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(os_rcode != OS_SUCCESS)
    {
        DBG_ERR("error creating thread %s (os_rcode=0x%02x)\n", os_rcode, NET_MAC_THREAD_NAME);
        goto error;
    }

    // launch a network event listener thread
    uint32_t launched = OK;
    os_rcode = posix_create_thread(&net_link_event_thread,
                                    link_status_change_event_listener, (void*)&launched,
                                    NET_MAC_THREAD_NAME "-wifi",
                                   &net_link_event_stack,
                                    NET_LINK_THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    // continue even if there's no network listener
    if (os_rcode != OK || launched != OK)
    {
        DBG_ERR("%s() no " NET_MAC_THREAD_NAME "-wifi" " thread! (launched %d err %d)\n", __func__, launched, os_rcode);
        DBG_ASSERT(os_rcode == OK && launched == OK);
    }

    net_link_t* link = NULL;
    int i = 0;
    while ((link = g_net_link_ctxt.links[i++]))
    {
        // tickle link state-machine
        uint8_t mac_addr[MAC_ADDR_BYTE_SIZE] = { 0 };
        if (get_intf_mac_addr(link->name, mac_addr) && !null_mac(mac_addr, MAC_ADDR_BYTE_SIZE)) // mac currently bound?
        {
            net_link_event_handler_cb(NET_LINK_EVENT_UP, (void*)link);
        }
        else
        {
            net_link_event_handler_cb(NET_LINK_EVENT_RESET, (void*)link);
        }
    }

    launch_file_change_listener(); // conf-file change listener

error:
    DBG_VERBOSE("<=== %s()\n", __func__);
    return;
}


#if defined(DBG_VERBOSE_ON) || defined(DBG_RELEASE_ON)
char* get_net_link_msg_literal(net_link_msg_type_t msg)
{
    if (msg < 0 || msg > NET_LINK_NUM_MSGS) return "<unknown msg>";
    return (char*) g_net_link_msg_strs[msg];
}
#endif

//=========================================================================================================================
// State loop
//=========================================================================================================================
/*   
 *  IMPORTANT NOTE:  Correct operation of this state machine depends on the following:
 * 
 *  1) The value of the state variable MUST NOT be directly modified outside of the state
 *  machine.
 * 
 *  2) Adding a message to the queue MUST NOT be conditional on the current value of the
 *  state variable.
 * 
 *  These conditions are required because the current value of the state variable does not
 *  account for messages pending in the message queue and therefore does not reflect
 *  actual state. The value of the state variable is only accurate relative to the last
 *  message processed, whereas actual state is a function of the current state variable
 *  value plus any messages still pending in the message queue. Once a message is sent it
 *  cannot be taken back, and its effect on the state variable is determined the moment it
 *  enters the message queue.
 * 
 *  When reading the state variable from outside the state machine, be aware that the
 *  the state variable value lags behind actual state.
 */
static void* net_link_state_loop(void* unused)
{
    DBG_VERBOSE("==>%s\n",__func__);
    unsigned int os_rcode;
    net_link_msg_t *msg;
    net_link_t *link;

    while(1)
    {
        link = NULL;
        os_rcode = posix_wait_for_message(g_net_link_msg_queue, (char*)&g_net_link_msg_buf, sizeof(net_link_msg_t), OS_WAIT_FOREVER);
        if(os_rcode != OS_SUCCESS)
        {
            DBG_ERR("error reading g_net_link_msg_queue (os_rcode=0x%02x)\n", os_rcode);
            goto error;
        }
        msg = (net_link_msg_t*)&g_net_link_msg_buf; // revisit msg = &g_net_link_msg_buf.msg;
        ASSERT(msg->type < NET_LINK_NUM_MSGS);
        
        /*
         *  process stateless messages
         */

        switch(msg->type)
        {
        case NET_LINK_MSG_ENABLE_LINKS:
            DBG_VERBOSE("stateless msg NET_LINK_MSG_ENABLE_LINKS link x%x\n", (unsigned int)msg->data1);
            ASSERT(msg->data1);
            //net_enable_exclusive_links( (net_link_logical_t *)msg->data1 );
            net_enable_exclusive_links( ((net_link_t *)msg->data1)->logical_link);	//LALIT: Pass current data. To solve ASSERT during init.
            continue; // bypass state machine

        default:
            // message is stateful - continue on to sate machine
            break;
        }

        /*
         *  process stateful messages
         */

        DBG_ASSERT(msg->link);
        DBG_ASSERT(msg->link->state < NET_LINK_NUM_STATES);
        DBG_VERBOSE("%s (%08x) msg %s in state %s\n", msg->link->name, msg->link, 
                    g_net_link_msg_strs[msg->type], g_net_link_state_strs[msg->link->state]);
        link = msg->link;

        switch(link->state)
        {
        /***************************************************
         * NET_LINK_STATE_DOWN
         ***************************************************/
        case NET_LINK_STATE_DOWN:
        {

            switch(msg->type)
            {
            case NET_LINK_MSG_UP_EVENT:

                // delayed to filter out up/down glitches
                adv_state(link, NET_LINK_STATE_UP_DELAY);
                activate_link_up_timer(link);
                break;

            default:

                break;
            }
        } break;

        /***************************************************
         * NET_LINK_STATE_UP_DELAY
         ***************************************************/
        case NET_LINK_STATE_UP_DELAY:
        {

            switch(msg->type)
            {
            case NET_LINK_MSG_DOWN_EVENT:

                DBG_MSG("canceling %s link-up timer\n", link->name);
                adv_state(link, NET_LINK_STATE_DOWN);
                net_timer_cancel(link->link_up_timer);
                break;

            case NET_LINK_MSG_DOWN_PENDING:

                DBG_MSG("canceling %s link-up timer\n", link->name);   
                adv_state(link, NET_LINK_STATE_DOWN);
                net_timer_cancel(link->link_up_timer);

                // immediately send the NET_LINK_IOCTL_DOWN_ACK  
                link->api_funcs.ioctl((ttUserInterface)NULL, NET_LINK_IOCTL_DOWN_ACK,(void *)0, 0);

                break;

            case NET_LINK_MSG_UP:
                {
                    net_link_t *link = msg->link;
                    ASSERT(link->logical_link);
                    net_link_logical_t *logical_link = link->logical_link;
                    DBG_MSG("%s link up\n", link->name);

                    NET_LINK_LOCK_CONTEXT();

                    adv_state(link, NET_LINK_STATE_UP);
                    net_link_t *prev_priority_link_up;
                    prev_priority_link_up = logical_link->priority_link_up;
                    net_link_set_priority_link_up(logical_link);
                    ASSERT(logical_link->priority_link_up);

                    // check if any exclusive links should be disabled
                    net_disable_exclusive_links(link);

                    /* 
                     *  Notify upper layers only if this is highest priority link currently
                     *  up.
                     */

                    if(logical_link->priority_link_up->priority < link->priority)
                    {
                        // a higher priority link is already up - don't notify upper layers
                        DBG_VERBOSE("higher priority %s link already up\n", logical_link->priority_link_up->name);
                        NET_LINK_UNLOCK_CONTEXT();
                        break;
                    }

                    net_link_event_t link_event = NET_LINK_EVENT_NONE;
                    switch(logical_link->status)
                    {
                    case NET_LINK_STATUS_UP:
                        DBG_VERBOSE("%s() link %s currently UP\n", __func__, link->name);
#ifdef __linux__
                        // nothing tbd
                        UNUSED_VAR(prev_priority_link_up);
#else // ! __linux__
                        DBG_VERBOSE("%s() link %s currently UP -> reset\n", __func__, link->name);
                        // link is already up so just issue a reset
                        ASSERT(prev_priority_link_up);
                        DBG_MSG("%s link preempted by %s\n", prev_priority_link_up->name, 
                                logical_link->priority_link_up->name);
                        link_event = NET_LINK_EVENT_RESET;
#endif // ! __linux__
                        break;

                    case NET_LINK_STATUS_DOWN:
                        DBG_VERBOSE("%s() link %s currently DOWN -> UP\n", __func__, link->name);
                        // link currently down so notify link up
                        adv_status(logical_link, NET_LINK_STATUS_UP);
                        link_event = NET_LINK_EVENT_UP;
                        break;

                    default:
                        // unhandled status
                        DBG_ERR("%s() NET_LINK_MSG_UP: unhandled status %d llink x%x status %d\n", __func__, logical_link->status, logical_link); 
                        DBG_ASSERT(0);
                        break;
                    }

                    NET_LINK_UNLOCK_CONTEXT();

                    if(link_event != NET_LINK_EVENT_NONE)
                    {
                        DBG_VERBOSE("%s() link %s -> %d (llink %#x)\n", __func__, link->name, link_event, logical_link);
                        net_link_notify(logical_link, link_event);
                    }
                }
                break;

            default:

                DBG_IGNORED("ignored %s msg %d in state STATE_UP_DELAY\n", msg->link->name, msg->type);

                break;
            }
        } break; // case NET_LINK_STATE_UP_DELAY

        /***************************************************
         * NET_LINK_STATE_UP
         ***************************************************/
        case NET_LINK_STATE_UP:
        {
            switch(msg->type)
            {
            case NET_LINK_MSG_DOWN_EVENT:

                adv_state(link, NET_LINK_STATE_DOWN_DELAY);
                if(link->flags & NET_LINK_FLAG_DEBOUNCE_ENABLE)
                {
                    // delay notification to filter out up/down glitches
                    activate_link_down_timer(link);
                }
                else
                {
                    // send immediate notification
                    net_link_send_msg(NET_LINK_MSG_DOWN, link, (void *)0, (void *)0);
                }
                break;

            case NET_LINK_MSG_DOWN_PENDING:

                {
                    ASSERT(link->logical_link);
                    net_link_logical_t *logical_link = link->logical_link;
                    net_link_event_t link_event = NET_LINK_EVENT_NONE;

                    NET_LINK_LOCK_CONTEXT();
                    if(logical_link->priority_link_up == link)
                    {
                        // hand off event to upper layer
                        link_event = NET_LINK_EVENT_DOWN_PENDING;
                    }
                    else
                    {
                        // immediately send the NET_LINK_IOCTL_DOWN_ACK  
                        link->api_funcs.ioctl((ttUserInterface)NULL, NET_LINK_IOCTL_DOWN_ACK,(void *)0, 0);
                    }
                    NET_LINK_UNLOCK_CONTEXT();

                    // notify only after link context is unlocked
                    if(link_event != NET_LINK_EVENT_NONE)
                    {
                        net_link_notify(logical_link, link_event);
                    }
                }
                break;

            case NET_LINK_MSG_RESET_EVENT:

                ASSERT(link->logical_link);
                if(link->logical_link->priority_link_up == link)
                {
                    // hand off event to upper layer
                    net_link_notify(link->logical_link, NET_LINK_EVENT_RESET);
                }
                break;

            default:

                DBG_IGNORED("ignored %s msg %d in state STATE_UP\n", msg->link->name, msg->type);

                break;
            }
        } break; // case NET_LINK_STATE_UP

        /***************************************************
         * NET_LINK_STATE_DOWN_DELAY
         ***************************************************/
        case NET_LINK_STATE_DOWN_DELAY:
        {

            switch(msg->type)
            {
            case NET_LINK_MSG_UP_EVENT:
                DBG_MSG("canceling %s link down timer\n", link->name);   
                adv_state(link, NET_LINK_STATE_UP);
                net_timer_cancel(link->link_down_timer);
                break;

            case NET_LINK_MSG_DOWN:
                {
                    NET_LINK_LOCK_CONTEXT();

                    DBG_MSG("%s link down\n", link->name);
                    adv_state(link, NET_LINK_STATE_DOWN);
                    net_link_logical_t *logical_link = link->logical_link;
                    net_link_set_priority_link_up(logical_link);

                    // check if any exclusive links should be enabled
                    net_enable_exclusive_links(logical_link);

                    net_link_event_t link_event = NET_LINK_EVENT_NONE;
#ifdef __linux__
                    logical_link->priority_link_up = link;
#else // ! __linux__
                    net_link_t *prev_priority_link_up = logical_link->priority_link_up;
                    /* 
                     *  Notify upper layers only if this link was the highest
                     *  priority link up.
                     */
                    DBG_ASSERT(logical_link->status == NET_LINK_STATUS_UP);
                    DBG_ASSERT(prev_priority_link_up);
                    if(prev_priority_link_up != link)
                    {
                        // link was not the highest priority link up - don't notify upper layers
                        NET_LINK_UNLOCK_CONTEXT();
                        break;
                    }

                    if(logical_link->priority_link_up)
                    {
                        // logical link is still up so just issue a reset to switch to new underlying link
                        link_event = NET_LINK_EVENT_RESET;
                    }
                    else
                    {
                        // logical link is down
                        logical_link->status = NET_LINK_STATUS_DOWN;
                        link_event = NET_LINK_EVENT_DOWN;
                    }
#endif // ! __linux__

                    NET_LINK_UNLOCK_CONTEXT();

                    // notify only after link context is unlocked
                    if(link_event != NET_LINK_EVENT_NONE)
                    {
                        net_link_notify(logical_link, link_event);
                    }
                }
                break;

            case NET_LINK_MSG_RESET_EVENT:

                // take action only if this link is the highest priority link up
                ASSERT(link->logical_link);
                if(link->logical_link->priority_link_up == link)
                {
                    // we are already waiting for link down timer so expedite link down message
                    net_timer_cancel(link->link_down_timer);
                    net_link_send_msg(NET_LINK_MSG_DOWN, link, (void *)0, (void *)0);
                }
                break;

            default:

                DBG_IGNORED("ignored %s msg %d in state STATE_DOWN_DELAY\n", msg->link->name, msg->type);

                break;
            }
        } break; // case NET_LINK_STATE_DOWN_DELAY

        /***************************************************
         * NET_LINK_STATE_DOWN_PENDING
         ***************************************************/
        case NET_LINK_STATE_DOWN_PENDING:
        {

            switch(msg->type)
            {
            case NET_LINK_MSG_DOWN:
                ASSERT(0); // not implemented
                break;

            default:

                DBG_IGNORED("ignored %s msg %d in state STATE_DOWN_PENDING\n", msg->link->name, msg->type);

                break;
            }
        } break; // case NET_LINK_STATE_DOWN_PENDING

        default:

            DBG_IGNORED("ignored %s msg %d in state <unknown>\n", msg->link->name, msg->type);
            // unknown state - ignore
            XASSERT(0, msg->link->state);
            break;
        }
    } // forever

error:
    DBG_MSG("network link start failed!\n");
    return (void*)-1;
}

//=========================================================================================================================
// Module API routines
//=========================================================================================================================

void *net_link_get_user_data(net_link_handle_t link_handle)
{
    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle(link_handle);
    ASSERT(logical_link);
    return logical_link->user_data;
}

void net_link_set_user_data(net_link_handle_t link_handle, void *user_data)
{
    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle(link_handle);
    ASSERT(logical_link);

    logical_link->user_data = user_data;
    return;
}

// return 1-based index from link's name
unsigned int net_link_name_to_index(const char *link_name)
{
    ASSERT(link_name);
    net_link_t *link = net_link_lookup_by_name(link_name);
    if(!link)
    {
        return 0;
    }

    // IMPORTANT: note that link_index used at public API layer is offset by one (1-based)
    // vs. OEM-assigned oem_enum index (0-based).  This is to maintain consistency with
    // official BSD socket API if_index (also 1-based)

    return link->oem_enum+1;
}

void net_link_get_name(unsigned int link_index, char *link_name, uint8_t len)
{
    if(!len)
    { 
        return;
    }
    ASSERT(link_name);
    link_name[0] = '\0';

    net_link_t *link = net_link_lookup_by_link_index(link_index);
    if(!link)
    { 
        // not found - return null string
        DBG_ERR("%s(%d) link '<undefined>' oem '<undefined>'\n", __func__, link_index);
        return;
    }

#ifdef __linux__
    DBG_VERBOSE("%s(%s) oem %d if_index %d\n", __func__, link_name, link->oem_enum, if_nametoindex(link_name));
#endif

    strncpy(link_name, link->name, len);
    link_name[len-1] = '\0';
    return;
}

error_type_t net_link_get_active_link(unsigned int link_handle, unsigned int* link_index)
{
    if (!link_index) return FAIL;
    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle((net_link_handle_t)link_handle); // cast

    //DBG_VERBOSE("%s(%x) llink x%x\n", __func__, (unsigned int)link_handle, logical_link);

    if (logical_link)
    {
        NET_LINK_LOCK_CONTEXT();
        *link_index = 0; // assume no links active
        if(logical_link->priority_link_up)
        {
            ASSERT(logical_link);
            ASSERT(logical_link->priority_link_up);
        // IMPORTANT: note that link_index used at public API layer is offset by one (1-based)
        // vs. OEM-assigned oem_enum index (0-based).  This is to maintain consistency with
        // official BSD socket API if_index (also 1-based)
            DBG_VERBOSE("%s(%x) llink x%x index %d\n", __func__, (unsigned int)link_handle, logical_link, logical_link->priority_link_up->oem_enum+1);
            *link_index = logical_link->priority_link_up->oem_enum+1;
        }
        NET_LINK_UNLOCK_CONTEXT();
    }
#ifdef __linux__
    else
    {
        DBG_IGNORED("%s(%d) no llink .. ignored\n", __func__, (unsigned int)link_handle, logical_link);
        *link_index = link_handle; // revisit REVISIT untranslated index?
        return FAIL;
    }
#endif
    return OK; 
}

net_rcode_t net_link_get_phys_addr(unsigned int link_index, uint8_t *phys_addr, uint8_t len)
{
    net_rcode_t net_rcode = NET_OK;
    if(!len)
    { 
        // return zero address
        goto done;
    }
    ASSERT( NULL != phys_addr );
    memset(phys_addr, 0, len); // default

    net_link_t *link = net_link_lookup_by_link_index(link_index);
    if(!link)
    { 
        // return zero address
        net_rcode = NET_BAD_INDEX;
        goto done;
    }

    int treck_dev_rcode;
    char tmp_addr[TM_MAX_PHYS_ADDR];
    uint8_t tmp_len;
    treck_dev_rcode = link->api_funcs.get_phys_addr((ttUserInterface)0, tmp_addr);
    if(treck_dev_rcode != TM_DEV_OKAY)
    {
        // return zero address
        goto done;
    }
    tmp_len = (len>TM_MAX_PHYS_ADDR)?TM_MAX_PHYS_ADDR:len;
    memcpy(phys_addr, (uint8_t *)tmp_addr, tmp_len);

done:
    return net_rcode;
}

// filtered link status either UP or DOWN
net_link_status_t net_link_get_status(net_link_handle_t link_handle)
{
    net_link_status_t link_status = NET_LINK_STATUS_DOWN;

    if(!g_net_link_ctxt.enumerated)
    {
        // links are not yet fully enumerated
        DBG_ERR("%s(%d) not ready\n", __func__, link_handle);
        DBG_ASSERT(0);
        goto done;
    }

    net_link_logical_t *llink = net_link_logical_lookup_by_handle(link_handle);
    DBG_ASSERT(llink);

    if (llink->priority_link_up)
    {
#ifdef __linux__
        if (intf_active(llink->priority_link_up->name))
#endif // __linux__
        {
            link_status = NET_LINK_STATUS_UP;
        }
        DBG_VERBOSE("%s(%d) llink x%x (priority link up x%x)\n", __func__, link_handle, llink, llink->priority_link_up );
    }

done:
    return link_status;
}

net_link_handle_t net_link_get_next_logical_link(net_link_handle_t link_handle)
{
    DBG_VERBOSE("==>%s (link_handle=%08x)\n", __func__, link_handle);

    net_link_handle_t next_link_hndl = (net_link_handle_t)NET_LINK_INVALID_HANDLE;

    if(!g_net_link_ctxt.enumerated)
    {
        // links are not yet fully enumerated
        DBG_ERR("==>%s (link_handle=%08x) not ready ..\n", __func__, link_handle);
        goto done;
    }

    uint8_t link_enum = 0;
    if(link_handle == (net_link_handle_t)NET_LINK_INVALID_HANDLE)
    {
        // return first (available?) item in list
        if(g_net_link_ctxt.logical_link_cnt)
        {
            link_enum = 0; 
            while(link_enum < NET_MAX_INTERFACES && !g_net_link_ctxt.logical_links[link_enum])
            {
                link_enum++;
            }
            if(link_enum < NET_MAX_INTERFACES)
            {
                next_link_hndl = (net_link_handle_t)(uint32_t)link_enum;
            }
        }
    }
    else
    {
        // return next item in list
        link_enum = net_link_get_enum(link_handle);
        XASSERT(g_net_link_ctxt.logical_links[link_enum], link_enum);
        do{
            link_enum++;
        } while(link_enum < NET_MAX_INTERFACES && !g_net_link_ctxt.logical_links[link_enum]);
        if(link_enum < NET_MAX_INTERFACES)
        {
            next_link_hndl = (net_link_handle_t)(uint32_t)link_enum;
        }
    }

done:
    DBG_VERBOSE("<==%s(%d) llink x%x\n", __func__, link_handle, next_link_hndl);
    return next_link_hndl;
}

void net_link_get_bindings(net_link_handle_t link_handle, net_link_bindings_t *bindings)
{
    ASSERT(bindings);
    //memset(bindings, 0, sizeof(net_link_bindings_t));

    if(!g_net_link_ctxt.enumerated)
    {
        // links are not yet fully enumerated
        DBG_ASSERT(0);
        return;
    }

    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle(link_handle);
    ASSERT(logical_link);
    ASSERT(logical_link->link_cnt);
    if(logical_link->link_cnt == 1)
    {
        // exclusive interface
        net_link_t *link = logical_link->links;
        bindings->open = link->api_funcs.open;
        bindings->close = link->api_funcs.close;
        bindings->send = link->api_funcs.send;
        bindings->recv = link->api_funcs.recv;
        bindings->ioctl = link->api_funcs.ioctl;
        bindings->get_phys_addr = link->api_funcs.get_phys_addr;
    }
    else
    {
        // shared interface
        bindings->open = net_link_shared_open;
        bindings->close = net_link_shared_close;
        bindings->send = net_link_shared_send;
        bindings->recv = net_link_shared_recv;
        bindings->ioctl = net_link_shared_ioctl;
        bindings->get_phys_addr = net_link_shared_get_phys_addr;
    }

    return;
}

uint8_t net_link_get_enum(net_link_handle_t link_handle)
{
    XASSERT(!((uint32_t)link_handle & 0xFFFFFF00), (uint32_t)link_handle); // verify link_index in range
    return (uint8_t)((uint32_t)link_handle & 0x000000FF);
}

//##########TODO remove this temp mapping function
net_iface_link_type_t net_link_name_to_type(const char *link_name)
{
#ifdef HAVE_ETHERNET
    if ( strcmp(link_name, NET_ETH_LINK_NAME) == 0 )
    {
        return NET_IFACE_LINK_TYPE_ETH;
    }
#endif // HAVE_ETHERNET
#ifdef HAVE_WLAN
    if ( strcmp(link_name, NET_WLAN_LINK_NAME) == 0 )
    {
        return NET_IFACE_LINK_TYPE_WLAN;
    }
#endif // HAVE_WLAN

    return NET_IFACE_LINK_TYPE_NONE;
}

// enumerate network links and attach to network interfaces
net_rcode_t net_link_enumerate_links(void)
{
    DBG_VERBOSE("==>%s\n",__func__);
    net_rcode_t net_rcode = NET_OK;

    net_link_instance_oem_func_table_t instance_oem_func_table;
    net_link_instance_oem_config_t instance_oem_config;
    instance_oem_config.func_table = &instance_oem_func_table;

    net_link_logical_t *logical_link;
    net_link_t *link;
    net_link_t *tmp_link;
    uint8_t i;

    net_link_get_oem_config(&g_net_link_ctxt.oem_config);

    i = 0;
    while(1)
    {
        net_rcode = g_net_link_ctxt.oem_config.func_table->instance_get_oem_config(i, &instance_oem_config);
        DBG_VERBOSE("==>%s link '%d' ret %d (err %d)\n",__func__, i, net_rcode, (net_rcode==NET_BAD_INDEX));
        if(net_rcode == NET_BAD_INDEX)
        {
            // no more links to enumerate - exit loop
            net_rcode = NET_OK;
            break;
        }
        if(net_rcode != NET_OK)
        {
            DBG_ERR("error enumerating %s link (enum %d)\n", instance_oem_config.name, i);
            i++; // advance to next link instance
            continue;
        }

        link = (net_link_t *)MEM_MALLOC(sizeof(net_link_t));
        if(!link)
        {
            DBG_ERR("error allocating mem for %s link (enum %d)\n", instance_oem_config.name, i);
            i++; // advance to next link instance
            continue;
        }

        ASSERT(instance_oem_config.func_table->init);
        DBG_VERBOSE("==>%s link '%s' init @ x%x\n",__func__, instance_oem_config.name, (unsigned int)instance_oem_config.func_table->init);
        net_rcode = instance_oem_config.func_table->init(&link->api_funcs);
        if(net_rcode != NET_OK)
        {
            DBG_ERR("#############################################\n"
                    "%s link init failed!\n"
                    "#############################################\n"
                    , instance_oem_config.name);
            MEM_FREE_AND_NULL(link);
            i++; // advance to next link instance
            continue;
        }

        ASSERT(link->api_funcs.event_register);
        link->api_funcs.event_register(net_link_event_handler_cb, (void *)link);

        ASSERT(instance_oem_config.name);
        strncpy(link->name, instance_oem_config.name, NET_LINK_NAME_SIZE);
        link->name[NET_LINK_NAME_SIZE-1] = '\0'; // guarantee nul-terminated c-string

        link->oem_enum = i;
        link->flags = instance_oem_config.flags;
        link->priority = instance_oem_config.priority;
        link->enabled = false;
        link->state = NET_LINK_STATE_DOWN;
        link->link_up_timer = NET_TIMER_INVALID_HANDLE;
        link->link_down_timer = NET_TIMER_INVALID_HANDLE;
        link->logical_link = NULL;

        // verify link name is unique
        tmp_link = net_link_lookup_by_name(link->name);
        if(tmp_link)
        {   
            // name already exists
            DBG_ERR("Name \"%s\" already exists for link enum %d!\n", link->name, i);
            net_rcode = NET_NOT_PERMITTED;
            goto done;
        }

        // index logical link via OEM-assigned enumeration value for the link
        logical_link = net_link_logical_lookup_by_enum(instance_oem_config.logical_link_enum);
        if(!logical_link)
        {
            // link logical does not yet exist for this iface enum - create new
            logical_link = (net_link_logical_t *)MEM_MALLOC(sizeof(net_link_logical_t));
            if(!logical_link)
            {
                DBG_ERR("%s: error detected line %d)\n", __func__, __LINE__);
                net_rcode = NET_FAIL;
                goto done;
            }

            logical_link->logical_enum = instance_oem_config.logical_link_enum;
            logical_link->link_cnt = 0;
            logical_link->links = NULL;
            logical_link->have_exclusive_link = false;
            logical_link->priority_link_up = NULL;
            logical_link->bound_link = NULL;
            logical_link->status = NET_LINK_STATUS_DOWN;
            logical_link->timer_hndl_enable = NET_TIMER_INVALID_HANDLE;
            logical_link->event_nodes = NULL;
            logical_link->user_data = (void *)0;

            g_net_link_ctxt.logical_links[instance_oem_config.logical_link_enum] = logical_link;
            g_net_link_ctxt.logical_link_cnt++;
        }

        net_link_insert_link(logical_link, link);
        g_net_link_ctxt.links[i] = link;
        g_net_link_ctxt.link_cnt++;

        DBG_VERBOSE("link %s (x%x) init complete\n", link->name, logical_link);

        logical_link->priority_link_up = link;

        i++; // advance to next link instance
    }

    uint8_t link_cnt = 0;
    for(i = 0; i < NET_MAX_INTERFACES; i++)
    {
        logical_link = g_net_link_ctxt.logical_links[i];
        if(!logical_link)
        {
            DBG_ERR("%s() skipped link %d!\n", __func__, i);
            continue;
        }

        ASSERT(logical_link->link_cnt);
        if(logical_link->link_cnt == 1)
        {
            /*
             *  exclusive interface -- link is statically bound
             * 
             *  NOTE: for shared interfaces, a link will be dynamically bound when the
             *  overlying network interface driver calls net_link_shared_open() and
             *  unbound when the driver calls net_link_shared_close().  Only one link may
             *  be bound at a time.
             */
            ASSERT(logical_link->links);
            logical_link->bound_link = logical_link->links;
        }

        if(logical_link->have_exclusive_link)
        {
            // start the exclusive link enable delay timer
            net_timer_cancel(logical_link->timer_hndl_enable); // cancel outstanding timer if any
            logical_link->timer_hndl_enable = net_timer_start_ms(
                                                    NET_TIMER_FLAG_REAL_TIME,
                                                    EXCLUSIVE_LINK_ENABLE_DELAY_SEC*1000,
                                                   link_enable_timer_func, 
                                                   (void *)logical_link);
            ASSERT(logical_link->timer_hndl_enable != NET_TIMER_INVALID_HANDLE);
        }

        link_cnt++;
        if(link_cnt >= g_net_link_ctxt.logical_link_cnt)
        {
            break;
        }
    }

    NET_LINK_LOCK_CONTEXT();
    ASSERT(!g_net_link_ctxt.enumerated);
    g_net_link_ctxt.enumerated = true; // enum list is complete and is now accessible externally
    NET_LINK_UNLOCK_CONTEXT();

done:
    DBG_VERBOSE("<==%s (net_rcode=%d)\n",__func__, net_rcode);
    return net_rcode;
}

// net link event monitor

void net_link_event_handler(net_link_event_t link_event, void *user_data)
{
    net_rcode_t net_rcode;
    ASSERT(user_data);
    net_link_t *link = (net_link_t *)user_data;

    DBG_VERBOSE("==>%s (link=%s event=%d)\n",__func__, link->name, link_event);

    void *msg_data1 = (void *)0;
    void *msg_data2 = (void *)0;
    net_link_msg_type_t msg_type = NET_LINK_NUM_MSGS; 

    switch(link_event)
    {
    case NET_LINK_EVENT_DOWN_PENDING:

        msg_type = NET_LINK_MSG_DOWN_PENDING;
        break;

    case NET_LINK_EVENT_DOWN:

        DBG_MSG("got %s link down event\n", link->name);  
        msg_type = NET_LINK_MSG_DOWN_EVENT;
        break;

    case NET_LINK_EVENT_UP:

        DBG_MSG("got %s link up event\n", link->name);   
        msg_type = NET_LINK_MSG_UP_EVENT;
        break;

    case NET_LINK_EVENT_RESET:
        msg_type = NET_LINK_MSG_RESET_EVENT;
        break;

    default:
        DBG_ERR("%s() link %#x event %d\n", __func__, link, link_event);
        // unexpected link event - ignore
        XASSERT(0, link_event);
        break;
    }

    if(msg_type < NET_LINK_NUM_MSGS)
    {
        net_rcode = net_link_send_msg_no_wait(msg_type, link, msg_data1, msg_data2);
        if(net_rcode == NET_WOULD_BLOCK)
        {
            // msg queue full
            DBG_MSG("%s link tx_queue full while sending msg %s\n", link->name, g_net_link_msg_strs[msg_type]);
            ASSERT(0); // if hit this then *may* need to increase msg queue size
        }
    }

    DBG_VERBOSE("<==%s\n",__func__);
    return;
}

void link_up_timer_func(void *user_data)
{
    net_rcode_t net_rcode;

    net_link_t *link = (net_link_t *)user_data;
    ASSERT(link);
    DBG_VERBOSE("%s() link %s\n", __func__, link->name);

    // assume sending msg from timer (i.e. a non-thread)
    net_rcode = net_link_send_msg_no_wait(NET_LINK_MSG_UP, link, (void *)0, (void *)0);
    if(net_rcode == NET_WOULD_BLOCK)
    {
        // msg queue full
        DBG_MSG("%s link os_queue full while sending msg NET_LINK_MSG_UP\n", link->name);
        DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
    }
    return;
}

void activate_link_up_timer(net_link_t *link)
{
    net_timer_cancel(link->link_up_timer); // cancel any previous link timer instance
    link->link_up_timer = net_timer_start_ms(
                            NET_TIMER_FLAG_REAL_TIME,
                            LINK_UP_NOTIFY_DELAY_SEC*1000,
                            link_up_timer_func,
                            (void *)link);
    ASSERT(link->link_up_timer != NET_TIMER_INVALID_HANDLE);
    return;
}

void link_down_timer_func(void *user_data)
{
    net_rcode_t net_rcode;

    net_link_t *link = (net_link_t *)user_data;
    ASSERT(link);
    DBG_MSG("%s() link %s\n", __func__, link->name);

    // sending msg from timer (i.e. a non-thread)
    net_rcode = net_link_send_msg_no_wait(NET_LINK_MSG_DOWN, link, (void *)0, (void *)0);
    if(net_rcode == NET_WOULD_BLOCK)
    {
        // msg queue full
        DBG_MSG("%s link os_queue full while sending msg NET_LINK_MSG_DOWN\n", link->name);
        DBG_ASSERT(0); // if hit this then *may* need to increase msg queue size
    }
    return;
}

void activate_link_down_timer(net_link_t *link)
{
    net_timer_cancel(link->link_down_timer); // cancel any previous link timer instance
    link->link_down_timer = net_timer_start_ms(
                                NET_TIMER_FLAG_REAL_TIME,
                                LINK_DOWN_NOTIFY_DELAY_MSEC,
                                link_down_timer_func,
                                (void *)link);
    ASSERT(link->link_down_timer != NET_TIMER_INVALID_HANDLE);
    return;
}


//=========================================================================================================================
// Local utility routines
//=========================================================================================================================

/* 
 *  net_link_recover_local_context()
 * 
 *  Recover reference to net_link_logical_t context previously stored in the Treck
 *  interface.
 * 
 *  This routine relies on a MARVELL MODIFICATION adding the user_data field to Treck
 *  ttDeviceEntry struct at ~line 2304 of trtype.h.
 */
static net_link_logical_t *net_link_recover_local_context(ttUserInterface treck_interface)
{
    ttDeviceEntryPtr devEntryPtr;
    devEntryPtr = (ttDeviceEntryPtr)treck_interface;
    net_link_handle_t link_handle = (net_link_handle_t)devEntryPtr->user_data;
    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle(link_handle);
    ASSERT(logical_link);
    return logical_link;
}

// intra-link-layer notification
net_rcode_t net_link_send_msg_wait_opt(
    net_link_msg_type_t type,
    net_link_t *link,
    void *udata,
    void *data2,
    unsigned long os_wait_opt)
{
    net_link_msg_t buf;
    buf.type = type;
    buf.link = link;
    buf.data1 = udata;
    buf.data2 = data2;

    unsigned int os_rcode = posix_message_send(g_net_link_msg_queue, (char *)&buf, sizeof(buf),
                                            MQ_DEFAULT_PRIORITY, os_wait_opt);
    if(os_rcode == OS_QUEUE_FULL)
    {
        /* 
         *  msg queue full - restart the timer to retry
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and os_wait_opt must
         *  therefore be set to OS_NO_WAIT
         */
        return NET_WOULD_BLOCK;
    }
    else if(os_rcode != OS_SUCCESS)
    {
        // any other errors are unexpected and unhandled
        DBG_ERR("%s() errno %d (x%x): %s\n", __func__, errno, errno, strerror(errno));
        XASSERT(0, os_rcode);
    }

    return NET_OK;
}

// enum 0-based
net_link_logical_t *net_link_logical_lookup_by_enum(uint8_t logical_link_enum)
{
    XASSERT(logical_link_enum < NET_MAX_INTERFACES, logical_link_enum);
    return g_net_link_ctxt.logical_links[logical_link_enum];
}

// link_handle 0-based
net_link_logical_t *net_link_logical_lookup_by_handle(net_link_handle_t link_handle)
{
    net_link_logical_t *logical_link = NULL;

    XASSERT(!((uint32_t)link_handle & 0xFFFFFF00), (uint32_t)link_handle); // verify link_index in range
    uint8_t link_enum = (uint8_t)((uint32_t)link_handle & 0x000000FF);

    if((link_enum < NET_MAX_INTERFACES) && (link_enum < g_net_link_ctxt.oem_config.link_cnt))
    {
        logical_link = g_net_link_ctxt.logical_links[link_enum];
    }

    if (!logical_link)
    {
        DBG_IGNORED("%s(%d) link_enum %d llink x%x\n", __func__, link_handle, link_enum, logical_link);
    }

    return logical_link;
}

/*
 *  link_enable_timer_func()
 * 
 *  This routine is called EXCLUSIVE_LINK_ENABLE_DELAY_SEC seconds after initialization of the
 *  registered link interface drivers.  The delay allows time for a higher priority link to come up
 *  soon after init (e.g. an Ethernet cable that is already plugged in), in which case any lower
 *  priority exclusive links will remain disabled.  If a higher priority link has not come up then
 *  lower priority exclusive links will be enabled at this time.
 */
void link_enable_timer_func(void *user_data)
{
    ASSERT(user_data);
    net_link_logical_t *llink = (net_link_logical_t *)user_data;

    // assume ThreadX timer callback (i.e. a non-thread)
    net_rcode_t net_rcode;
    net_rcode = net_link_send_msg_no_wait(NET_LINK_MSG_ENABLE_LINKS, (net_link_t *)NULL,
                                                (void *)llink, (void *)0);
    if(net_rcode == NET_WOULD_BLOCK)
    {
        // msg queue full
        DBG_MSG("tx_queue full while sending NET_LINK_MSG_ENABLE_LINKS (enum=%d)\n", llink->logical_enum);
        ASSERT(0); // if hit this then *may* need to increase msg queue size
    }
    return;
}

/* 
 *  net_enable_exclusive_links()
 * 
 *  Called whenever a link goes down.  Enables all exclusive links with a priority higher than the
 *  highest priority link still in an "up" state.
 */
void net_enable_exclusive_links(net_link_logical_t *logical_link)
{
    ASSERT(logical_link);
    DBG_VERBOSE("==>%s llink x%x enum=%d\n", __func__, logical_link, logical_link->logical_enum);

    NET_LINK_LOCK_CONTEXT();

    // confirm logical link has one or more attached links
//    DBG_ERR("link_cnt: %d\n", logical_link->link_cnt);
    ASSERT(logical_link->link_cnt);
    ASSERT(logical_link->links);

#if defined __linux__ && defined HAVE_WIRELESS
    // startup the interface(s) if enabled:
#ifdef HAVE_UAP
    // uAP restarts in 1-2 seconds
    if (wifi_uap_enabled() && !wifi_uap_running())
    {
        char* intf_name = NET_IF_NAME_UAP;
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, intf_name, 0);
#else
        DBG_VERBOSE("%s() enable wireless '%s' links\n",__func__, intf_name);
        // uAP restarts in 1-2 seconds
        //stop_etc_network_service(intf_name); // ifdown TEMPTEMP
        //start_etc_network_service(intf_name, false); // ifup TEMPTEMP
#endif // ! HAVE_NET_SERVICES
    }
#endif // HAVE_UAP

#ifdef HAVE_WLAN
    // wifi-sta restarts in ~10 seconds
    if (wifi_sta_enabled() && !wifi_sta_running())
    {
        //char* intf_name = NET_IF_NAME_STA;
#ifdef HAVE_NET_SERVICES
        net_service_request(NET_SERVICE_START, NET_SERVICE_IFCONFIG, intf_name, 0);
#else
        // wifi-sta restarts in 10+ seconds
        //stop_etc_network_service(intf_name); // ifdown TEMPTEMP
        //start_etc_network_service(intf_name, false); // ifup TEMPTEMP
#endif // ! HAVE_NET_SERVICES
    }
#endif // HAVE_WLAN
#endif // __linux__ && HAVE_WIRELESS

    if (!(logical_link->have_exclusive_link))
    {
        // nothing to do
        goto done;
    }

    // find priority of highest-priority link in "up" state
    link_priority_t highest_link_up_priority = NET_LINK_PRIORITY_LOWEST;
    if (logical_link->priority_link_up)
    {
        highest_link_up_priority = logical_link->priority_link_up->priority;
    }

    // enable any disabled exclusive links with a priority greater than highest_link_up_priority
    // NOTE: we enable all of them since no guarantee that any given link will come up after enabled
    net_link_t* link = logical_link->links;
    while(link)
    {
        if( link->flags & NET_LINK_FLAG_SHARED_EXCLUSIVE
            && (!link->enabled) && (link->priority < highest_link_up_priority) )
        {
            DBG_MSG("enabling %s link\n", link->name);
            link->enabled = true;
            link->api_funcs.ioctl( (ttUserInterface)NULL, NET_LINK_IOCTL_ENABLE,(void *)0, 0 );
        }
        link = link->next;
    }

done:

    NET_LINK_UNLOCK_CONTEXT();
    DBG_VERBOSE("<==%s\n", __func__);
    return;
}

/* 
 *  net_disable_exclusive_links()
 * 
 *  Called whenever a link comes up.  Disables any exclusive links with a lower link priority.
 */
void net_disable_exclusive_links(net_link_t *up_link)
{
    if (!up_link) return;

    DBG_VERBOSE("==> %s() %s(link %#x llink %#x)\n", __func__, up_link->name, up_link, up_link->logical_link);

#ifdef __linux__

        // revisit: exclusive links aren't necessary in linux impl

#else // ! __linux__
    DBG_ASSERT(up_link->logical_link);
    net_link_logical_t *logical_link = up_link->logical_link;
    NET_LINK_LOCK_CONTEXT();

     // confirm logical link has one or more attached links
    ASSERT(logical_link->link_cnt);
    ASSERT(logical_link->links);

    if( (up_link->state != NET_LINK_STATE_UP) ||
        (logical_link->link_cnt == 1) ||
        (!logical_link->have_exclusive_link) )
    {
        DBG_VERBOSE("==> %s(%s) link %#x llink %#x up\n", __func__, up_link->name, up_link, logical_link);
        // nothing to do
        goto done;
    }

    ASSERT(logical_link->links);
    net_link_t *link = logical_link->links;
    while(link)
    {
        if( link->flags & NET_LINK_FLAG_SHARED_EXCLUSIVE
            && link->enabled && (link->priority > up_link->priority) )
        {
            DBG_MSG("disabling %s link\n", link->name);
            link->enabled = false;
            link->api_funcs.ioctl( (ttUserInterface)NULL, NET_LINK_IOCTL_DISABLE,(void *)0, 0 );
        }
        link = link->next;
    }

done:
    NET_LINK_UNLOCK_CONTEXT();
#endif // ! __linux__
    return;
}

// insert link into interface list sorted in order of decreasing priority (i.e. list head points
// to link of highest priority)
void net_link_insert_link(net_link_logical_t *logical_link, net_link_t *net_link)
{
    NET_LINK_LOCK_CONTEXT();
    net_link_t *cur = logical_link->links;
    net_link_t **prev = &logical_link->links;

    DBG_VERBOSE("==> %s(%s) link %#x llink %#x \n", __func__, net_link->name, net_link, logical_link);

    while(cur)
    {
        XASSERT(net_link->priority != cur->priority, cur->priority); // ensure priority is unique per interface
        if(net_link->priority < cur->priority)
        {
            break;
        }
        prev = &cur->next;
        cur = cur->next;
    }
    *prev = net_link;
    net_link->next = cur;

    net_link->logical_link = logical_link;

    logical_link->link_cnt++;
    if(net_link->flags & NET_LINK_FLAG_SHARED_EXCLUSIVE)
    {
        logical_link->have_exclusive_link = true;
    }

    NET_LINK_UNLOCK_CONTEXT();
    return;
}

net_link_t *net_link_lookup_by_name(const char *link_name)
{
    //DBG_VERBOSE("==>%s (link_name=%s)\n", __func__, link_name);

    ASSERT(link_name);

    NET_LINK_LOCK_CONTEXT();
    net_link_logical_t *link_logical;
    net_link_t *link = NULL;

    uint8_t i;
    uint8_t link_cnt = 0;
    for(i = 0; i < NET_MAX_INTERFACES; i++)
    {
        link_logical = g_net_link_ctxt.logical_links[i];
        if (!link_logical)
        {
            DBG_IGNORED("==>%s(%s) NOT found (no llink?)\n", __func__, link_name);
            continue;
        }

        link = link_logical->links;
        while (link)
        {
            if (strcmp(link_name, link->name) == 0)
            {
                //DBG_VERBOSE("==>%s found %s)\n", __func__, link_name);
                goto done;
            }
            link = link->next;
        }

        link_cnt++;
        if (link_cnt >= g_net_link_ctxt.logical_link_cnt)
        {
            DBG_IGNORED("==>%s(%s) NOT found (links %d llinks %d)\n", __func__, link_name, link_cnt, g_net_link_ctxt.logical_link_cnt);
            break;
        }
    }

done:
    NET_LINK_UNLOCK_CONTEXT();

    if (!link)
    {
        DBG_VERBOSE("<==%s(%s) not found!\n", __func__, link_name);
    }
    return link;
}

/* 
 *  net_link_lookup_by_link_index()
 * 
 *  Lookup link context via public API layer link_index value.
 * 
 *  IMPORTANT: note that link_index used at public API layer is offset by one (1-based)
 *  vs. OEM-assigned oem_enum index (0-based).  This is to maintain consistency with
 *  official BSD socket API if_index (also 1-based)
 */
net_link_t *net_link_lookup_by_link_index(unsigned int link_index) // 1-based lookup
{
    DBG_VERBOSE("==>%s (link_index=%d)\n", __func__, link_index);
    net_link_t *link = NULL;
    if (link_index<1)
    {
        DBG_ERR("<==%s Invalid (1-based) link index %d?\n", __func__, link_index);
        DBG_XASSERT(link_index>0, link_index);
        goto done; 
    }

    // convert link_index to internal offset index value
    uint8_t link_enum = ((uint8_t)((link_index-1) & 0x000000FF));

    XASSERT(!((link_index-1) & 0xFFFFFF00), link_index); // verify link_index in range
    DBG_VERBOSE("%s: oem_enum=%d\n", __func__, link_enum);

    if(link_enum < NET_MAX_PHYSICAL_LINKS)
    {
        link = g_net_link_ctxt.links[link_enum];
        DBG_VERBOSE("%s(%d) link %#x (offset %d)\n", __func__, link_index, link, link_enum);
    }

done:
    if (!link)
    {
        DBG_VERBOSE("<==%s() link #%d not found!\n", __func__, link_index);
    }
    return link;
}

// find highest priority link in up state
void net_link_set_priority_link_up(net_link_logical_t *logical_link)
{
    DBG_VERBOSE("==>%s (logical_enum=%d)\n", __func__, logical_link->logical_enum);
    link_priority_t highest_link_up_priority = NET_LINK_PRIORITY_LOWEST;

    NET_LINK_LOCK_CONTEXT();
    logical_link->priority_link_up = NULL;
    net_link_t *link = logical_link->links;
    while(link)
    {
        if(link->state == NET_LINK_STATE_UP && link->priority < highest_link_up_priority)
        {
            logical_link->priority_link_up = link;
            highest_link_up_priority = link->priority;
        }
        link = link->next;
    }
    NET_LINK_UNLOCK_CONTEXT();
    DBG_VERBOSE("<==%s (priority link up = %s)\n", __func__, 
                logical_link->priority_link_up?logical_link->priority_link_up->name:"none");
    return;
}

//=========================================================================================================================
// Network link event registration
//=========================================================================================================================

net_link_event_handle_t net_link_register_events(
    net_link_handle_t link_handle,
    net_link_event_notify_func_t notify_func,
    void *user_data)
{
    ASSERT(notify_func);

    net_link_logical_t *logical_link = net_link_logical_lookup_by_handle(link_handle);
    ASSERT(logical_link);

    // create a new node and register the events
    net_link_event_node_t *node;
    node = (net_link_event_node_t *)MEM_MALLOC(sizeof(net_link_event_node_t));
    if(!node)
    {
        DBG_ASSERT(0);
        return NULL;
    }

    node->notify_func = notify_func;
    node->logical_link = logical_link;
    node->user_data = user_data;
    link_event_list_insert(node);

    return (net_link_event_handle_t)node;
}

void net_link_deregister_events(net_link_event_handle_t event_handle)
{
    ASSERT(event_handle);

    net_link_event_node_t *node = (net_link_event_node_t *)event_handle;
    if(node)
    {
        link_event_list_remove(node);
        MEM_FREE_AND_NULL(node);
    }

    return;
}

void link_event_list_insert(net_link_event_node_t *node)
{
    // insert at beginning of linked list
    ASSERT(node);
    ASSERT(node->logical_link);
    NET_LINK_LOCK_CONTEXT();
    node->next = node->logical_link->event_nodes;
    node->logical_link->event_nodes = node;
    NET_LINK_UNLOCK_CONTEXT();
}

void link_event_list_remove(net_link_event_node_t *node)
{
    ASSERT(node);
    ASSERT(node->logical_link);
    NET_LINK_LOCK_CONTEXT();
    net_link_event_node_t *cur_node = node->logical_link->event_nodes;
    net_link_event_node_t **prev = &node->logical_link->event_nodes;
    while(cur_node)
    {
        if(cur_node == node)
        {
            // remove callback from linked list
            *prev = cur_node->next;
            cur_node->next = NULL;
            break;
        }
        prev = &cur_node->next;
        cur_node = cur_node->next;
    }
    NET_LINK_UNLOCK_CONTEXT();

    return;
}

#ifdef DEBUG
#define str_event(event) case event: return #event;
static const char* event_str(net_link_event_t event)
{
    switch (event)
    {
        str_event(NET_LINK_EVENT_NONE);
        str_event(NET_LINK_EVENT_DOWN_PENDING);
        str_event(NET_LINK_EVENT_DOWN);
        str_event(NET_LINK_EVENT_UP);
        str_event(NET_LINK_EVENT_HW_PRESENCE);
        str_event(NET_LINK_EVENT_RESET);
        default: break;
    }
    return "";
}
#else
#define str_event(event) "bug"
static const char* event_str(net_link_event_t event) {return "";}
#endif // DEBUG

// notify all registered callbacks
void net_link_notify(net_link_logical_t *logical_link, net_link_event_t link_event)
{
    DBG_VERBOSE("==>%s %s(logical_enum=%d event=%d %s)\n", __func__, logical_link->bound_link->name,
        logical_link->logical_enum,
            link_event, event_str(link_event));

    net_link_event_notify_func_t notify_func;
    void *user_data;
    net_link_event_node_t *next_node;

    NET_LINK_LOCK_CONTEXT();
    net_link_event_node_t *node = logical_link->event_nodes;
    while(node)
    {
        DBG_VERBOSE("%s() event %d node x%x notify x%x\n", __func__, link_event, node, node->notify_func);
        // save now in case node gets removed while unlocked below
        next_node = node->next;
        notify_func = node->notify_func;
        user_data = node->user_data;

        NET_LINK_UNLOCK_CONTEXT();
        ASSERT(notify_func);
        notify_func(link_event, user_data);
        NET_LINK_LOCK_CONTEXT();

        node = next_node;
    }
    NET_LINK_UNLOCK_CONTEXT();

    DBG_VERBOSE("<== %s(x%x,%d)\n", __func__, logical_link, link_event);
    return;
}


//=========================================================================================================================
// OS (formerly Treck) interface callbacks
//=========================================================================================================================
/*  These callbacks are used to multiplex more than one link into a single shared
 *  interface.
 */

int net_link_shared_open(ttUserInterface treck_interface)
{
    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    ASSERT(logical_link);

    if(logical_link->bound_link)
    {
        // Already bound to a link!!  Must first call net_link_shared_close().
        ASSERT(0);
        return TM_DEV_ERROR;
    }

    logical_link->bound_link = logical_link->priority_link_up;

    DBG_VERBOSE("%s: logical_enum=%d; bound_link=%s\n", __func__, logical_link->logical_enum,
                logical_link->bound_link->name);
    DBG_ASSERT(logical_link->bound_link->api_funcs.open);
    return logical_link->bound_link->api_funcs.open(treck_interface);
}

int net_link_shared_close(ttUserInterface treck_interface)
{

    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    DBG_ASSERT(logical_link);

    if(!logical_link->bound_link)
    {
        // Not currently bound to a link!!  Must first call net_link_shared_open().
        DBG_ASSERT(0);
        return TM_DEV_ERROR;
    }

    net_link_t *bound_link = logical_link->bound_link;
    logical_link->bound_link = NULL;

    DBG_VERBOSE("%s: logical_enum=%d; bound_link=%s\n", __func__, logical_link->logical_enum, bound_link->name);
    DBG_ASSERT(bound_link->api_funcs.close);
    return bound_link->api_funcs.close(treck_interface);
}

int net_link_shared_send(ttUserInterface treck_interface, char *data_ptr, int data_len, int flag)
{
    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    ASSERT(logical_link);

    if(!logical_link->bound_link)
    {
// can't have an assert here because stack can still try to send packets (e.g. broadcast) even after the interface
// is closed and the corresponding route table entries are deleted
//        ASSERT(0);
        return TM_DEV_ERROR;
    }

    ASSERT(logical_link->bound_link->api_funcs.send);
    return logical_link->bound_link->api_funcs.send(treck_interface, data_ptr, data_len, flag);
}

int net_link_shared_recv(ttUserInterface treck_interface,
                        char **data_ptr, int *data_len,
                        ttUserBufferPtr user_buff_ptr
#ifdef TM_DEV_RECV_OFFLOAD // this define must be link agnostic
                        , ttDevRecvOffloadPtr offload_ptr
#endif
                        )
{
    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    ASSERT(logical_link);

    if(!logical_link->bound_link)
    {
// can't have an assert here because stack can still try to send packets (e.g. broadcast) even after the interface
// is closed and the corresponding route table entries are deleted
//        ASSERT(0);
        return TM_DEV_ERROR;
    }

    ASSERT(logical_link->bound_link->api_funcs.recv);
    return logical_link->bound_link->api_funcs.recv(treck_interface, data_ptr, data_len, user_buff_ptr
#ifdef TM_DEV_RECV_OFFLOAD // this define must be link agnostic
                                              , offload_ptr
#endif
                                              );
}

int net_link_shared_ioctl(ttUserInterface treck_interface, int flag, void *option_ptr, int option_len)
{
    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    ASSERT(logical_link);

    if(!logical_link->bound_link)
    {
// can't have an assert here because we continue to call NET_LINK_IOCTL_SEND_COMPLETE from net timer thread
//        ASSERT(0);
        return TM_DEV_ERROR;
    }

    ASSERT(logical_link->bound_link->api_funcs.ioctl);
    return logical_link->bound_link->api_funcs.ioctl(treck_interface, flag, option_ptr, option_len);
}

int net_link_shared_get_phys_addr(ttUserInterface treck_interface, char *phys_addr)
{
    net_link_logical_t *logical_link = net_link_recover_local_context(treck_interface);
    ASSERT(logical_link);

    if(!logical_link->bound_link)
    {
        // Not currently bound to a link!!  Must first call net_link_shared_open().
        ASSERT(0);
        return TM_DEV_ERROR;
    }

    ASSERT(logical_link->bound_link->api_funcs.get_phys_addr);
    return logical_link->bound_link->api_funcs.get_phys_addr(treck_interface, phys_addr);
}

//=========================================================================================================================
// Public Physical Link Enumeration
//=========================================================================================================================

uint8_t net_link_enum_get_count(void)
{
    return g_net_link_ctxt.link_cnt;
}

uint8_t net_link_enum_from_link_index(unsigned int link_index)
{
    DBG_MSG("==>%s (link_index=%d)\n", __func__, link_index); // DBG_VERBOSE
    uint8_t link_enum = NET_LINK_INVALID_ENUM;
    if(!link_index)
    {
        goto done; 
    }

    // convert link_index to oem_enum index value
    XASSERT(!((link_index-1) & 0xFFFFFF00), link_index); // verify link_index in range
    link_enum = ((uint8_t)((link_index-1) & 0x000000FF));
    //DBG_VERBOSE("%s: oem_enum=%d\n", __func__, oem_enum);

    if(link_enum >= g_net_link_ctxt.oem_config.link_cnt
       || !g_net_link_ctxt.links[link_enum])
    {
        link_enum = NET_LINK_INVALID_ENUM;
    }

done:
    return link_enum;
}

unsigned int net_link_enum_to_link_index(uint8_t link_enum)
{
    unsigned int link_index = NET_LINK_INVALID_INDEX;

    if(link_enum < g_net_link_ctxt.oem_config.link_cnt
       && g_net_link_ctxt.links[link_enum])
    {
        link_index = ((unsigned int)link_enum)+1;
    }

    DBG_MSG("%s() %d -> %d\n", __func__, link_enum, link_index); // DBG_VERBOSE

    return link_index;
}

#ifdef __cplusplus
}
#endif

// eof net_link.c
