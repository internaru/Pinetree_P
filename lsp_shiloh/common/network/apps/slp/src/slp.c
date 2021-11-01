/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lassert.h"
#include "dprintf.h"
#include "logger.h"
#include "platform.h"
#include "error_types.h"
#include "memAPI.h"
//#include "net_app_api.h"
#include "net_api.h"
#include "net_slp_vars.h"
#include "net_slp_api.h"
#include "agMessage.h"
#include <pthread.h>
#include <posix_ostools.h>
#ifdef __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
//#include <linux/string.h>
#else
#include "net_iface_api.h"
#include "net_iface_vars.h"
#endif
#include <unistd.h>
#include <fcntl.h>

#include "ioutils.h"

#include "slp.h"
#include "net_slp_config.h"

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "SLP: "
#define DBG_ON
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON

#ifdef DBG_RELEASE_ON
#define DBG_PRINTF_MACRO(...) dbg_printf(DBG_PRFX __VA_ARGS__)
#else
#define DBG_PRINTF_MACRO(...) DPRINTF((DBG_SOFT|DBG_OUTPUT), (DBG_PRFX __VA_ARGS__))
#endif
#ifdef DBG_ON
#define DBG_MSG(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif
#ifdef DBG_ERR_ON
#define DBG_ERR(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_ERR(...)
#endif
#ifdef DBG_VERBOSE_ON
#define DBG_VERBOSE(...) DBG_PRINTF_MACRO(__VA_ARGS__)
#else
#define DBG_VERBOSE(...)
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#define DBG_CMD(...) cmd_printf(__VA_ARGS__) // omit module prefix for debug command output
#endif

//=========================================================================================================================

/* To add :#define SO_REUSEPORT 15 */
#define SO_REUSEPORT 15
 
#define SRV_REQ          1
#define SRV_RPLY         2
#define SRV_REG          3
#define SRV_DEREG        4
#define SRV_ACK          5
#define ATTR_RQST        6
#define ATTR_RPLY        7
#define DA_ADVERT        8
#define SRV_TYPE_RQST    9
#define SRV_TYPE_RPLY    10
#define SA_ADVERT        11

#define NUM_REPLIES 5
#define SIZE_OF_ERROR_CODE  2
#define SIZE_OF_ATTR_LENGTH 2
#define NUM_OF_ATTR_AUTHS   2

#define SLP_LANG_CODE 0x656e // en = English
#define SLP_CHAR_CODE 0x0003 // US-ASCII

#define NET_MAX_INTERFACES          5
#define SLP_STACK_SIZE POSIX_MIN_STACK_SIZE ///< The size of the stack.
#define SLP_MSG_QUEUE_SIZE          20

static ALIGN8 UINT8 slp_app_stack[SLP_STACK_SIZE];
static mqd_t slp_msgq; ///< The message queue structure.
static pthread_t slp_thread_id; ///< The thread structure.

typedef enum {
    SLP_STATE_UNINITIALIZED = 0,
    SLP_STATE_DISABLED,
    SLP_STATE_STOPPED,
    SLP_STATE_RUNNING,
} slp_state_t;

typedef struct
{
    bool                ipv4_configured;
#ifdef HAVE_IPV6        
    bool                ipv6_configured;
#endif // HAVE_IPV6     
    slp_state_t         state; // **MUST NOT** be modified outside the state machine
    int                 sock;
    uint32_t            announce_remain;
    uint32_t            announce_timer;  
    uint32_t            reply_timer;  
    char	    	if_name[IF_NAMESIZE];
} slp_context_t;

static bool                g_ctxt_enabled;
pthread_mutex_t g_mtx;
static slp_context_t	   g_ctxt[NET_MAX_INTERFACES];

typedef struct 
{
    uint32_t ip_addr;
    uint16_t port_num;
    uint32_t timeout;
    uint16_t xid;
} slpv1_reply_t;
slpv1_reply_t slp_replies[ NUM_REPLIES ] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

typedef struct 
{
    struct sockaddr_storage ip_addr;
    uint16_t port_num;
    uint32_t timeout;
    uint16_t xid;
}slpv2_reply_t;

#pragma pack(1)
typedef struct 
{
    uint8_t   ver;
    uint8_t   function_id;
    uint16_t  length;
    uint8_t   flags;
    uint8_t   dialect;
    uint16_t  lang_code;
    uint16_t  char_encode;
    uint16_t  xid;
} slpv1_header_t;

#pragma pack(4)
typedef struct _slp_attrrply
{
    slpv1_header_t  header;
    uint16_t        err_code;
    uint16_t        length;
    char            attr_list;
} slpv1_attr_reply_t;

#pragma pack(1)
typedef struct 
{
    uint8_t   ver;
    uint8_t   function_id;
    uint8_t   length[3];
    uint16_t  flags;
    uint8_t   next_ext_offset[3];
    uint16_t  xid;
    uint16_t  lang_tag_length;
    uint16_t  lang_tag;
} slpv2_header_t;

#pragma pack(4)
typedef struct 
{
    slpv2_header_t   header;
    uint16_t         err_code;
    uint16_t         length;
    char             attr_list;
} slpv2_attr_reply_t;

slpv2_reply_t slpv2_replies[ NUM_REPLIES ];
net_slp_config_t slp_config;

#define DSTADDR_DATASIZE (CMSG_SPACE(sizeof(struct in_pktinfo)))
#define dstaddr(x) (&(((struct in_pktinfo *)(CMSG_DATE(x)))->ipi_addr))
union control_data 
{
  struct cmsghdr cmsg;
  u_char data[DSTADDR_DATASIZE];
};

//=========================================================================================================================
// net_app module registration
//=========================================================================================================================

void stud_to_call_functions();  // Adding stud to call functions that aren't current used so
                                // they compile.
static void net_slp_app_init(void);
static void* net_slp_execute(void *unused);

static void SLPSocketCBFunc(int socket_descriptor, void *user_data, int event_flags);
static void sendv1Reply(UINT if_enum, struct sockaddr_storage *sockaddr, uint16_t xid);
static void sendv2Reply(UINT if_enum, struct sockaddr_storage *sockaddr, uint16_t xid);
static uint8_t slp_in_exclusion_list( UINT if_enum, char *data, unsigned short pktlen, bool use_v4_address );
static uint32_t get3Bytes(unsigned char*);
static int8_t set3Bytes(uint32_t, char*);
static void checkTimers(UINT if_enum, uint32_t* timeout);

static void slp_start_announce_timer(UINT if_enum);
static void slp_cancel_announcements(UINT if_enum);
static void slp_send_announcement(UINT if_enum);
static void sendSLPAutoReply(UINT if_enum);
static void slp_announce_timer_func( union sigval data);
static void slp_open_socket(UINT if_enum);
static void slp_close_socket(UINT if_enum);
static void slp_join_multicast_groups(UINT if_enum);
static void slp_leave_multicast_groups(UINT if_enum);
static void slp_start_reply_timer(UINT if_enum, uint32_t timer_ticks);
static void slp_reply_timer_func( union sigval data);
static void slp_network_event_func(uint32_t events, void *context);
static bool is_network_up(UINT if_enum);
static void slp_send_reply(UINT if_enum);

//#define SLP_SEND_MSG(if_enum, msg)   slp_app_execute(g_app_hndl, (void *)if_enum, (void*)msg, 0);
#define SLP_SEND_MSG(if_enum, msg)   slp_send_msg(if_enum, msg);
#define DUMMY_CALL() stub_to_call_functions();

#define SLP_LOCK_CONTEXT() \
        { \
            UINT pthread_rcode; \
            pthread_rcode = pthread_mutex_lock( &g_mtx ); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define SLP_UNLOCK_CONTEXT() \
        { \
            UINT pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_mtx); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 


//=========================================================================================================================
// Module entry point
//=========================================================================================================================
void net_slp_init(void)
{

    net_slp_var_init();
// Not sure where best place to create new slp thread and message queue are?
// Either here or in net_slp_app_init.  Going to try it here first.
    /* create the queue */ 
    posix_create_message_queue( &slp_msgq, "/slpapp", SLP_MSG_QUEUE_SIZE, 
                                         sizeof(slp_msg_buf_t) );    


    net_slp_app_init();
    return;
}

//=========================================================================================================================
// net_app entry point
//=========================================================================================================================

static void net_slp_app_init()
{
    int i;
    struct ifaddrs *ifaddr, *ifa;
    UINT if_enum = 0;
    UINT posix_rcode;

    posix_mutex_init( &g_mtx );

    SLP_LOCK_CONTEXT();

    net_slp_config( &slp_config );

    for( i = 0; i < NUM_REPLIES; i++ )
    {
        memset(&slpv2_replies[i].ip_addr, 0, sizeof(struct sockaddr_storage));
        slpv2_replies[i].port_num = 0;
        slpv2_replies[i].timeout = 0;
    }

    // TODO:  Loop on next interface in ifaddrs structure
    // Comment out until I know what to do here.
    /* need to loop thru the multiple network interfaces
       http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html
       */

    if (getifaddrs(&ifaddr) == -1)
    { 
      ASSERT( 0 );
    }

    // Walk through linked list, maintaining head pointer so list can be freed later
    for (ifa = ifaddr; ifa != NULL && if_enum < NET_MAX_INTERFACES; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr == NULL)
      {
        continue;
      }


    /* may need to use a network manager calls to reqister callback
       function for network events */
//    UINT if_count = net_iface_enum_get_count();
//    for ( if_enum = 0; if_enum < if_count; if_enum++ )
//    {    
//	    unsigned int if_index = net_iface_enum_to_if_index(if_enum);
      unsigned int iface_index;
      iface_index = if_nametoindex(ifa->ifa_name);


        strcpy(g_ctxt[if_enum].if_name, ifa->ifa_name);
        g_ctxt[if_enum].sock = -1;
	    g_ctxt[if_enum].reply_timer = NET_TIMER_INVALID_HANDLE;
    	g_ctxt[if_enum].announce_timer = NET_TIMER_INVALID_HANDLE;
/*  Network event registration may be provide by the new Network Manager */
/*	    net_iface_register_events(if_index,
				  NET_IFACE_EVENT_IPV4_CONFIG_CHANGE
#ifdef HAVE_IPV6
				  | NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE
#endif // HAVE_IPV6
				  , slp_network_event_func, (void *)if_enum);
*/
    	net_slp_get_var_enabled(&g_ctxt_enabled);
	    g_ctxt[if_enum].state = SLP_STATE_DISABLED;
    	if(g_ctxt_enabled)
	    {
	        g_ctxt[if_enum].state = SLP_STATE_STOPPED;
    	    SLP_SEND_MSG(if_enum, SLP_MSG_NET_UP_POLL);
	    }
        if_enum++;
    }
    SLP_UNLOCK_CONTEXT();

    posix_rcode = posix_create_thread( &slp_thread_id, &net_slp_execute, 0, "SLP App",
                                       slp_app_stack, SLP_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT(posix_rcode==0, posix_rcode);

    DBG_VERBOSE( "init complete\n" );
    return;
}

//=========================================================================================================================
// State Machine
//=========================================================================================================================

/*   
 *  IMPORTANT NOTE:  Correct operation of this state machine depends on the following:
 * 
 *  1) g_ctxt.state MUST NOT be modified outside of the state machine
 * 
 *  2) adding a message to the queue MUST NOT be conditional on the current value of g_ctxt.state
 * 
 *  This is because the current value of g_ctxt.state does not account for messages pending in the
 *  queue. The value of g_ctxt.state reflects state relative to the last message pulled from
 *  the queue, whereas actual state is defined by the most recent message added to the queue (i.e.
 *  once a message is sent it cannot be taken back, and its effect on g_ctxt.state is a function of
 *  the current value of g_ctxt.state and any earlier messages still pending in the queue).
 * 
 *  When reading g_ctxt.state from outside the state machine, be aware that g_ctxt.state lags behind
 *  actual state.  
 */
static void* net_slp_execute(void *unused)
{
    slp_msg_buf_t msg_buf;
    UINT if_enum;;
    slp_msg_t msg;

    while (1)
    {
       if(0 == posix_wait_for_message(slp_msgq, (char*)&msg_buf,  sizeof(slp_msg_buf_t),
                                       POSIX_WAIT_FOREVER))
       { 
         msg = msg_buf.msg;
         if_enum = msg_buf.if_enum;

         switch(g_ctxt[if_enum].state)
         {
         /***************************************************
          * SLP_STATE_UNINITIALIZED
          ***************************************************/
         case SLP_STATE_UNINITIALIZED:
             // ignore messagest
             break;

         /***************************************************
          * SLP_STATE_DISABLED
          ***************************************************/
         case SLP_STATE_DISABLED:
             switch(msg)
             {
             case SLP_MSG_ENABLE:
                 SLP_LOCK_CONTEXT();
                 g_ctxt[if_enum].state = SLP_STATE_STOPPED;
                 SLP_SEND_MSG(if_enum, SLP_MSG_NET_UP_POLL);
                 SLP_UNLOCK_CONTEXT();
                 break;

             default:
                 // ignore other messages
                 break;
             }

             break;

         /***************************************************
          * SLP_STATE_STOPPED
          ***************************************************/
         case SLP_STATE_STOPPED:
             switch(msg)
             {
             case SLP_MSG_NET_UP:
             case SLP_MSG_NET_UP_POLL:
                 SLP_LOCK_CONTEXT();
                 if(is_network_up(if_enum))
                 {
                     slp_open_socket(if_enum);
                     slp_join_multicast_groups(if_enum);
                     if ( slp_config.auto_announce )
                     {
                         g_ctxt[if_enum].announce_remain = 3;
                         slp_start_announce_timer(if_enum);
                     }
                     g_ctxt[if_enum].state = SLP_STATE_RUNNING;
                 }
                 SLP_UNLOCK_CONTEXT();
                 break;

             case SLP_MSG_DISABLE:
                 SLP_LOCK_CONTEXT();
                 g_ctxt[if_enum].state = SLP_STATE_DISABLED;
                 SLP_UNLOCK_CONTEXT();
                 break;

             default:
                 // ignore other messages
                 break;
             }
             break;

         /***************************************************
          * SLP_STATE_RUNNING
          ***************************************************/
         case SLP_STATE_RUNNING:
             switch(msg)
             {
             case SLP_MSG_DISABLE:
             case SLP_MSG_NET_DOWN:
                 SLP_LOCK_CONTEXT();
                 slp_leave_multicast_groups(if_enum);
                 if ( slp_config.auto_announce )
                 {
                     slp_cancel_announcements(if_enum);
                 }
                 slp_close_socket(if_enum);
                 if(msg == SLP_MSG_DISABLE)
                 {
                     g_ctxt[if_enum].state = SLP_STATE_DISABLED;
                 }
                 else // SLP_MSG_NET_DOWN
                 {
                     g_ctxt[if_enum].state = SLP_STATE_STOPPED;
                 }
                 SLP_UNLOCK_CONTEXT();
                 break;

             case SLP_MSG_SEND_REPLY:
                 slp_send_reply(if_enum);
                 break;

             case SLP_MSG_SEND_ANNOUNCE:
                 slp_send_announcement(if_enum);
                 break;

             default:
                 // ignore other messages
                 break;
             }
             break;

         default:
             // unknown state - ignore
             XASSERT(0, g_ctxt[if_enum].state);
         }
       }
    }
    return 0;
}

// May Take this code out until we know how to tie into the new network manager api
static void slp_network_event_func(uint32_t events, void *context)
{
    unsigned int if_enum = (unsigned int)context;

//    unsigned int if_index = net_iface_enum_to_if_index(if_enum);
    unsigned int if_index; 
    if_index = if_nametoindex(g_ctxt[if_enum].if_name);
#if 0
    SLP_LOCK_CONTEXT();

    bool ipv4_prev = g_ctxt[if_enum].ipv4_configured;
    if(events & NET_IFACE_EVENT_IPV4_CONFIG_CHANGE)
    {
        net_iface_get_var_ipv4_configured(if_index, &g_ctxt[if_enum].ipv4_configured);
    }

#ifdef HAVE_IPV6
    bool ipv6_prev = g_ctxt[if_enum].ipv6_configured;
    if(events & NET_IFACE_EVENT_IPV6_LOCAL_CONFIG_CHANGE)
    {
        net_iface_get_var_ipv6_local_configured(if_index, &g_ctxt[if_enum].ipv6_configured);
    }
#endif // HAVE_IPV6

    if(is_network_up(if_enum))
    {
        if(g_ctxt[if_enum].ipv4_configured > ipv4_prev 
#ifdef HAVE_IPV6
           || g_ctxt[if_enum].ipv6_configured > ipv6_prev
#endif // HAVE_IPV6
           )
        {
            /*  Net just came up (i.e. either ipv4 or ipv6 just came up).  Notify the state machine.  Note that
             *  we purposely do not use current value of g_ctxt.state in making this determination.
             */
            SLP_SEND_MSG(if_enum, SLP_MSG_NET_UP);
        }
    }
    else // network is down
    {
        if((g_ctxt[if_enum].ipv4_configured < ipv4_prev
#ifdef HAVE_IPV6
            && !g_ctxt[if_enum].ipv6_configured)||(g_ctxt[if_enum].ipv6_configured < ipv6_prev && !g_ctxt[if_enum].ipv4_configured 
#endif // HAVE_IPV6
           ))
        {
            /*  Net just went down (i.e. either ipv4 just went down and ipv6 not configured or ipv6 just went
             *  down and ipv4 not configured).  Notify the state machine.  Note that we purposely do not use
             *  current value of g_ctxt.state in making this determination.
             */
            SLP_SEND_MSG(if_enum, SLP_MSG_NET_DOWN);
        }
    }

    SLP_UNLOCK_CONTEXT();
#endif
    return;
}
//#endif

static bool is_network_up(UINT if_enum)
{
#if 0 // may need to hard code a return value until network manager api is available
    bool ipv6_enabled = false;
    bool need_ipv6 = false;
    SLP_LOCK_CONTEXT();

    unsigned int if_index = net_iface_enum_to_if_index(if_enum);

    bool ipv4_enabled;
    net_iface_get_var_ipv4_enabled(if_index, &ipv4_enabled);
    bool need_ipv4 = ipv4_enabled && !g_ctxt[if_enum].ipv4_configured;

#ifdef HAVE_IPV6
    net_iface_get_var_ipv6_enabled(if_index, &ipv6_enabled);
    need_ipv6 = ipv6_enabled && !g_ctxt[if_enum].ipv6_configured;
#endif // HAVE_IPV6

    SLP_UNLOCK_CONTEXT();
    return (ipv4_enabled || ipv6_enabled) && !(need_ipv4 || need_ipv6);
#endif
return true;
}

void net_slp_notify_var_enabled(void)
{
    SLP_LOCK_CONTEXT();
    bool enabled;
    struct ifaddrs *ifaddr, *ifa;
    net_slp_get_var_enabled(&enabled);
    DBG_MSG("var change notification: enabled=%d\n", enabled);

    UINT if_enum = 0;
    /* need to loop thru the multiple network interfaces
       http://www.kernel.org/doc/man-pages/online/pages/man3/getifaddrs.3.html */
    if (getifaddrs(&ifaddr) == -1)
    { 
      ASSERT( 0 );
    }

    // Walk through linked list, maintaining head pointer so list can be freed later
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr == NULL)
      {
        continue;
      }

      if(enabled && !g_ctxt_enabled)
      {
        g_ctxt_enabled = true;
        SLP_SEND_MSG(if_enum, SLP_MSG_ENABLE);
      }
      else if(!enabled && g_ctxt_enabled)
      {
        g_ctxt_enabled = false;
        SLP_SEND_MSG(if_enum, SLP_MSG_DISABLE);
      }
      if_enum++;
    }
    SLP_UNLOCK_CONTEXT();
    return;
}

static void slp_start_announce_timer(UINT if_enum)
{
    srand(time(NULL));
    uint32_t timer_ticks = (((uint32_t)rand()) % (SYS_TICK_FREQ * 1)) + (SYS_TICK_FREQ * 4); 
    
    SLP_LOCK_CONTEXT();
       
    net_timer_cancel(g_ctxt[if_enum].announce_timer);
    g_ctxt[if_enum].announce_timer = net_timer_start_ticks(
                                slp_announce_timer_func, 
                                (void *)if_enum,
                                (void *)NULL, 
                                timer_ticks);
    ASSERT(g_ctxt[if_enum].announce_timer != NET_TIMER_INVALID_HANDLE);
    
    SLP_UNLOCK_CONTEXT();     
}

static void slp_cancel_announcements(UINT if_enum)
{
    SLP_LOCK_CONTEXT();

    g_ctxt[if_enum].announce_remain = 0;
    net_timer_cancel(g_ctxt[if_enum].announce_timer);

    SLP_UNLOCK_CONTEXT();
    return;
}

static void slp_start_reply_timer(UINT if_enum, uint32_t timer_ticks)
{
    SLP_LOCK_CONTEXT();
       
    net_timer_cancel(g_ctxt[if_enum].reply_timer);
    g_ctxt[if_enum].reply_timer = net_timer_start_ticks(
                                slp_reply_timer_func, 
                                (void *)if_enum,
                                (void *)NULL, 
                                timer_ticks);
    ASSERT(g_ctxt[if_enum].reply_timer != NET_TIMER_INVALID_HANDLE);
    
    SLP_UNLOCK_CONTEXT();     
}

static void slp_send_announcement(UINT if_enum)
{
    SLP_LOCK_CONTEXT();
    if(!g_ctxt[if_enum].announce_remain)
    {
        SLP_UNLOCK_CONTEXT();
        return;
    }

    sendSLPAutoReply(if_enum);
    g_ctxt[if_enum].announce_remain--;

    // do we need to send another unsolicited reply? 
    if(g_ctxt[if_enum].announce_remain)
    {
        slp_start_announce_timer(if_enum);
    }

    SLP_UNLOCK_CONTEXT();
    return;
}

/** 
 * \fn void sendSLPAutoReply( void )
 * 
 * \brief Send generic auto response for SLP
 * 
 * Send an SLP multicast advertising that we are on the network.
 * Function will send both and SLP v1 and v2 packets
 * 
 **/
static void sendSLPAutoReply(UINT if_enum)
{
    struct sockaddr_storage sockaddr;
#ifdef HAVE_IPV6
        struct sockaddr_in6 *ip6ptr = (struct sockaddr_in6*)&sockaddr;
        ip6ptr->sin6_family            = PF_INET6;
        ip6ptr->sin6_port              = htons( slp_config.port_number);
        ip6ptr->sin6_addr.s6_addr32[0] = (u_long) 0;
        ip6ptr->sin6_addr.s6_addr32[1] = (u_long) 0;
        ip6ptr->sin6_addr.s6_addr32[2] = (u_long) htonl ( 0xFFFF );
        ip6ptr->sin6_addr.s6_addr32[3] = (u_long) slp_config.v1_address;
#else // !HAVE_IPV6
        struct sockaddr_in  *ip4ptr = (struct sockaddr_in*)&sockaddr;
        ip4ptr->sin_family  = PF_INET;
        ip4ptr->sin_port    = htons( slp_config.port_number);
        ip4ptr->sin_addr    = slp_config.v1_address;
#endif // HAVE_IPV6

    if(g_ctxt[if_enum].ipv4_configured
#ifdef HAVE_IPV6
       || g_ctxt[if_enum].ipv6_configured
#endif // HAVE_IPV6
       )
    {
        sendv1Reply(if_enum, &sockaddr, 0 );
    }
}

static void slp_announce_timer_func(union sigval data)
{
    ASSERT(data.sival_ptr);
    net_timer_data_t *oneshot_data = (net_timer_data_t *)data.sival_ptr;
    UINT if_enum = (UINT)oneshot_data->data1;        
    SLP_SEND_MSG(if_enum, SLP_MSG_SEND_ANNOUNCE);
    return;
}

static void slp_open_socket(UINT if_enum)
{
    if(g_ctxt[if_enum].sock != -1)
    {
        // already open
        return;
    }

//    int treck_rcode;
//    int sock_rcode;
    struct sockaddr_storage sockaddr;

//    unsigned int iface_id = net_iface_enum_to_if_index(if_enum);
//    if_indextoname(iface_id, g_ctxt[if_enum].if_name);

    unsigned int iface_id = if_nametoindex(g_ctxt[if_enum].if_name);
    if(!iface_id)
    {
        DBG_ERR("if_nametoindex failed!\n");
        goto error;
    }
#ifdef HAVE_IPV6
    g_ctxt[if_enum].sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
#else // !HAVE_IPV6
    g_ctxt[if_enum].sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif // HAVE_IPV6
    if(g_ctxt[if_enum].sock == -1)
    {
//        treck_rcode = tfGetSocketError(-1);
        DBG_ERR("slp_open_socket failed on %s: %d \n", g_ctxt[if_enum].if_name, errno );
        goto error;
    }

    {
	int n = 1;
    
    if (setsockopt(g_ctxt[if_enum].sock, SOL_SOCKET, SO_REUSEADDR, (char *) &n, 
                   sizeof(n)) == -1) 
	{
         ASSERT( 0 );
    }
    
    if (setsockopt(g_ctxt[if_enum].sock, SOL_SOCKET, SO_BINDTODEVICE, 
                   g_ctxt[if_enum].if_name, strlen(g_ctxt[if_enum].if_name)) < 0) 
	{
            ASSERT( 0 );
    }
    }

    /* Bind the UDP socket to a well known UDP port */
    memset(&sockaddr, 0, sizeof(struct sockaddr_storage)); 
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6ptr = (struct sockaddr_in6*)&sockaddr;
    ip6ptr->sin6_family            = PF_INET6;
    ip6ptr->sin6_port              = htons( slp_config.port_number);
#else // !HAVE_IPV6
    struct sockaddr_in  *ip4ptr = (struct sockaddr_in*)&sockaddr;
    ip4ptr->sin_family  = PF_INET;
    ip4ptr->sin_port    = htons( slp_config.port_number);
#endif // HAVE_IPV6
    if(bind (g_ctxt[if_enum].sock, (struct sockaddr *)&sockaddr, 
             sizeof(struct sockaddr_storage)) == -1)
    {
        DBG_ERR("bind failed on %s: errno:%d \n", g_ctxt[if_enum].if_name, errno);
        goto error;
    }
// Who knows wtf to do with this. Commenting out for now until we get Network Mangager? 
// TODO: fix this mess
//    tfRegisterSocketCBParam(g_ctxt[if_enum].sock, SLPSocketCBFunc, (void *)if_enum, TM_CB_RECV);
    return;

error:
//    ASSERT(0);
    return;
}

static void slp_close_socket(UINT if_enum)
{
    if(g_ctxt[if_enum].sock != -1)
    {
        close(g_ctxt[if_enum].sock);
        g_ctxt[if_enum].sock = -1;
    }
    return;
}

static void slp_join_multicast_groups(UINT if_enum)
{
    int sock_rcode;
    uint8_t ttl_8, optval;
    #ifdef HAVE_IPV6
    uint32_t ttl_32;
    #endif // HAVE_IPV6

    if(g_ctxt[if_enum].ipv4_configured)
    {
        // Join IPv4 Multicast Group
        struct ip_mreq sockopt;
        sockopt.imr_multiaddr.s_addr = slp_config.v1_address;
        sockopt.imr_interface.s_addr = 0;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                                 IPPROTO_IP, 
                                 IP_ADD_MEMBERSHIP, 
                                 (void *)&sockopt, 
                                 sizeof(sockopt));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to join multicast group for v1 address on %s: errno(%d)\n", 
                    g_ctxt[if_enum].if_name, errno);
        }

        sockopt.imr_multiaddr.s_addr = slp_config.v2_ipv4_address;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                                 IPPROTO_IP, 
                                 IP_ADD_MEMBERSHIP, 
                                 (void *)&sockopt, 
                                 sizeof(sockopt));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to join multicast group for v2 ipv4 address on %s: errno(%d)\n",
                     g_ctxt[if_enum].if_name, errno);
        }

        // Set hops
// rdj TODO create Unity var for this?
//        uint8_t ttl_8 = (uint8_t)DSGetIntVar(e_SLP_TTL);
        ttl_8 = slp_config.ttl;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IP, 
                               IP_MULTICAST_TTL, 
                               (void *)&ttl_8, 
                               sizeof(ttl_8));
        if(sock_rcode == -1)
        {
	    DBG_ERR("failed to set multicast TTL on %s of %d: errno(%d)\n", 
                g_ctxt[if_enum].if_name, ttl_8, errno);
        }

        // Set pktinfo so the destination address can be included
        optval = 1;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IP, 
                               IP_PKTINFO, 
                               (void *)&optval, 
                               sizeof(optval));
        if(sock_rcode == -1)
        {
	    DBG_ERR("failed to set PKTINFO on %s: errno(%d)\n", 
                g_ctxt[if_enum].if_name, errno);
        }
    }

#ifdef HAVE_IPV6
    DBG_VERBOSE( "IPV6 configured on %s: %d\n", g_ctxt[if_enum].if_name, 
                 g_ctxt[if_enum].ipv6_configured );
    if(g_ctxt[if_enum].ipv6_configured)
    {
        DBG_VERBOSE( "Join Group on %s\n", g_ctxt[if_enum].if_name );
        // Join IPv6 Multicast Group
        struct ipv6_mreq sockoptv6;
        sockoptv6.ipv6mr_interface = 0;
        memcpy( &sockoptv6.ipv6mr_multiaddr, &slp_config.v2_ipv6_address.s6_addr32, sizeof( slp_config.v2_ipv6_address.s6_addr32 ) );

        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IPV6, 
                               IPV6_ADD_MEMBERSHIP, 
                               (void *)&sockoptv6, 
                               sizeof(sockoptv6));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to join IPv6 multicast group on %s: errno(%d)\n", 
                    g_ctxt[if_enum].if_name, errno);
        }

        // Set Multicast hops
// rdj TODO create Unity var for this?
//        uint32_t ttl_32 = (UINT8 )DSGetIntVar(e_SLP_TTL);
        ttl_32 = (uint32_t)slp_config.ttl;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IPV6, 
                               IP_MULTICAST_TTL, 
                               (void *)&ttl_32, 
                               sizeof(ttl_32));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to set IPv6 multicast TTL on %s of %d: errno(%d)\n", 
                    g_ctxt[if_enum].if_name, ttl_32, errno);
        }

       // Set pktinfo so the destination address can be included
        optval = 1;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IPV6, 
                               //IP_PKTINFO, 
                               IPV6_RECVPKTINFO, 
                               (void *)&optval, 
                               sizeof(optval));
        if(sock_rcode == -1)
        {
	    DBG_ERR("failed to set PKTINFO on %s: errno(%d)\n", 
                g_ctxt[if_enum].if_name, errno);
        }
    }
#endif // HAVE_IPV6

    return;
}

static void slp_leave_multicast_groups(UINT if_enum)
{
    int sock_rcode;

    if(g_ctxt[if_enum].ipv4_configured)
    {
        struct ip_mreq sockopt;

        // SLPv1 Group
        sockopt.imr_interface.s_addr = 0;
        sockopt.imr_multiaddr.s_addr = slp_config.v1_address;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IP, 
                               IP_DROP_MEMBERSHIP, 
                               (void *)&sockopt, 
                               sizeof(sockopt));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to leave multicast group for v1 address on %s - errno(%s)\n", 
                    g_ctxt[if_enum].if_name, errno);
        }

        // SLPv2 Group
        sockopt.imr_multiaddr.s_addr = slp_config.v2_ipv4_address;
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IP, 
                               IP_DROP_MEMBERSHIP, 
                               (void *)&sockopt, 
                               sizeof(sockopt));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to leave multicast group for v2 ipv4 address on %s: errno(%d)\n",
                     g_ctxt[if_enum].if_name, errno);
        }
    }

#ifdef HAVE_IPV6
    if(g_ctxt[if_enum].ipv6_configured)
    {
        struct ipv6_mreq sockoptv6;
        sockoptv6.ipv6mr_interface = 0;
        memcpy( &sockoptv6.ipv6mr_multiaddr, &slp_config.v2_ipv6_address.s6_addr32, 
                sizeof( slp_config.v2_ipv6_address.s6_addr32 ) );
        sock_rcode = setsockopt(g_ctxt[if_enum].sock, 
                               IPPROTO_IPV6, 
                               IPV6_DROP_MEMBERSHIP, 
                               (void *)&sockoptv6, 
                               sizeof(sockoptv6));
        if(sock_rcode == -1)
        {
            DBG_ERR("failed to leave IPv6 multicast group on %s: errno(%d)\n", 
                    g_ctxt[if_enum].if_name, errno);
        }
    }
#endif // HAVE_IPV6

    return;
}

static void slp_reply_timer_func(union sigval data)
{
    ASSERT((void*)data.sival_ptr);
    net_timer_data_t *oneshot_data = (net_timer_data_t *)data.sival_ptr;
    UINT if_enum = (UINT)oneshot_data->data1;        
    SLP_SEND_MSG(if_enum, SLP_MSG_SEND_REPLY);
    return;
}

void DumpSLPHeader( slpv1_header_t *slp_header )
{
    uint8_t *data;
    uint32_t i;

    DBG_MSG("\tVersion=%d\n\tFunctionID=%d\n\tLength=%d\n\tFlags=%d\n\tDialect=%d\n\tLangCode=%d\n\tCharEncode=%d\n\tXID=%d\n",
           slp_header->ver,
           slp_header->function_id,
           slp_header->length,
           slp_header->flags,
           slp_header->dialect,
           slp_header->lang_code,
           slp_header->char_encode,
           slp_header->xid);

    data = (uint8_t*)slp_header;
    for ( i = 0; i < sizeof( slpv1_header_t ); i++ )
    {
        DBG_VERBOSE( "%02X\n", data[ i ] );
    }

}

void DumpSLPv2Header( slpv2_header_t *slp_header )
{
    uint8_t *data;
    uint32_t i;
    DBG_MSG("\n\tVersion=%d\n\tFunctionID=%d\n\tLength=%d\n\tFlags=%d\n\tNextExtOffset=%d\n\tXID=%d\n\tLangTagLength=%d\n\tLangTag=%d\n",
            slp_header->ver,
            slp_header->function_id,
            get3Bytes( slp_header->length ),
            slp_header->flags,
            get3Bytes( slp_header->next_ext_offset ),
            slp_header->xid,
            slp_header->lang_tag_length,
            slp_header->lang_tag
            );
    data = (uint8_t*)slp_header;
    for ( i = 0; i < sizeof( slpv2_header_t ); i++ )
    {
        DBG_VERBOSE( "%02X\n", data[ i ] );
    }
}

//commenting out until new network manager becomes available
// Hopefully it will have socket event notification
//if 0  

static void SLPSocketCBFunc(int socket_descriptor, void *user_data, int event_flags)
{
    UINT if_enum = (UINT)user_data;
    int error_code;
    char *data;
    char *data1;
    struct sockaddr_storage sockaddr;
    int sock_length;
    unsigned short length;
    uint8_t ret_val;
    slpv1_header_t slp_header;
    struct iovec iov[1];
    struct msghdr msg;
    union control_data cmsg;
#ifdef HAVE_IPV6
struct sockaddr_in6 *ip6ptr = (struct sockaddr_in6*)&sockaddr;
#else
struct sockaddr_in *ip4ptr = (struct sockaddr_in*)&sockaddr;
#endif

    SLP_LOCK_CONTEXT();

    if(g_ctxt[if_enum].state != SLP_STATE_RUNNING)
    {
        SLP_UNLOCK_CONTEXT();
        return;
    }

    sock_length = sizeof( struct sockaddr_storage );
    error_code = recvfrom( g_ctxt[if_enum].sock,
                           ( char* ) &slp_header,
                           sizeof( slp_header ),
                           MSG_PEEK,
                           ( struct sockaddr * ) &sockaddr, 
                           ( socklen_t *)&sock_length );

    iov[0].iov_base = (char *)&slp_header;
    iov[0].iov_len = sizeof(slp_header);

    memset(&msg, 0, sizeof msg);
    msg.msg_name = &sockaddr;
    msg.msg_namelen = sock_length;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &cmsg;
    msg.msg_controllen = sizeof cmsg;

    switch (slp_header.ver)
    {
        case SLP_VERSION_1:
        {
            DBG_VERBOSE( "Version 1 packet on %s\n" , g_ctxt[if_enum].if_name);

            slp_header.length      = cpu_to_be16( slp_header.length );
            slp_header.lang_code   = cpu_to_be16( slp_header.lang_code );
            slp_header.char_encode = cpu_to_be16( slp_header.char_encode );
            slp_header.xid         = cpu_to_be16( slp_header.xid );

            // get rest of packet
            length = slp_header.length;
            if (length > 1500)
            {
                DBG_ERR( "Invalid length on %s: %d\n", g_ctxt[if_enum].if_name, length );
                // bad length. grab part of it and dump it. The receive function will dump anything left.
                sock_length = sizeof(struct sockaddr_storage);
                error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);

                SLP_UNLOCK_CONTEXT();
                return;
            }
            data = (char*)malloc(length);
            data1 = data;
    
            if(data == NULL)
            {
                DBG_ERR("Malloc failed to allocate %d bytes on %s\n", length, g_ctxt[if_enum].if_name);
                // grab the header again, but not with a peek. This will dump the packet.
                sock_length = sizeof(struct sockaddr_storage);
                error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);
                SLP_UNLOCK_CONTEXT();
                return;
            }
            
            sock_length = sizeof(struct sockaddr_storage);
            error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);

            if (error_code == -1)
            {
                /* Retrieve the socket error for failed recvfrom call */
                DBG_ERR("RecvFromTo Error on %s: %d\n", g_ctxt[if_enum].if_name, errno);
                free(data1);
                SLP_UNLOCK_CONTEXT();
                return;
            }
            else
            {
                if (error_code != length)
                {
                    DBG_MSG( "error_code %d != length %d\n", error_code, length );
                    int i;
                    for ( i = 0; i < error_code; i++ )
                    {
                        DBG_MSG( "%02X\n", data[ i ] ); 
                    }
                    // packet was the wrong length. Dump it.
                    free(data1);
                    SLP_UNLOCK_CONTEXT();
                    return;
                }
            }
            
            data += sizeof( slp_header );
            length -= sizeof( slp_header );

            switch( slp_header.function_id )
            {
                case SRV_REQ:
                    
                    break;
                case SRV_RPLY:
                    
                    break;
                case SRV_REG:
                    
                    break;
                case SRV_DEREG:
                    
                    break;
                case SRV_ACK:
                    
                    break;
                case ATTR_RQST:
                    DBG_VERBOSE( "V1 Attr rqst\n" );

                    // check to see if SLP is enabled. If not, just dump the packet. 
                    if (!g_ctxt_enabled)
                    {
                        DBG_MSG( "Not enabled\n" );
                        break;
                    }

                    // first check to see if we are in the exclusion list.
                    if ( slp_in_exclusion_list( if_enum, data, length, true ) )
                    {
                        DBG_VERBOSE( "In exclusion list\n" );
                        DumpSLPHeader( &slp_header );
                        break;
                    }
                    
                    // now check to see if we match the URL
                    if ( net_slp_match_url (data) == 0 )
                    {
                        DBG_VERBOSE( "URL does not match\n" );
                        DumpSLPHeader( &slp_header );
                        break;
                    }
    
                    // language must be english and char code must be US-ASCII
                    if (slp_header.lang_code != SLP_LANG_CODE || 
                        slp_header.char_encode != SLP_CHAR_CODE)
                    {
                        DBG_VERBOSE( "Invalid char code\n" );
                        DumpSLPHeader( &slp_header );
                        break;
                    }

                    // OK everything matches. Now process it. 
                    uint32_t to_ipv4_addr, ipv4_addr;
                    uint16_t port;
                    struct in_pktinfo *pkti;
                    struct in6_pktinfo *pkti6;
//#ifdef HAVE_IPV6
// see http://stackoverflow.com/questions/5281409/get-destination-address-of-a-received-udp-packet

                   struct cmsghdr *cmsgptr = CMSG_FIRSTHDR(&msg);
                   for ( // iterate through all the control headersi 
                        cmsgptr = CMSG_FIRSTHDR(&msg); 
                        cmsgptr != NULL;
                        cmsgptr = CMSG_NXTHDR(&msg, cmsgptr))
                   {
                       // ignore the control headers that don't match what we want
                       if (cmsgptr->cmsg_level == IPPROTO_IP ||
                           cmsgptr->cmsg_type == IP_PKTINFO)
                       {
                           pkti = (struct in_pktinfo*)CMSG_DATA(cmsgptr);
                           to_ipv4_addr = pkti->ipi_addr.s_addr;
                       }
                       // ignore the control headers that don't match what we want
                       if (cmsgptr->cmsg_level == IPPROTO_IPV6 ||
                           cmsgptr->cmsg_type == IPV6_PKTINFO)
                       {
                           pkti6 = (struct in6_pktinfo*)CMSG_DATA(cmsgptr);
                           to_ipv4_addr = pkti6->ipi6_addr.s6_addr32[3];
                       }
                   }
#ifdef HAVE_IPV6
                   ipv4_addr = ip6ptr->sin6_addr.s6_addr32[3];
                   port = ip6ptr->sin6_port; 

#else // !HAVE_IPV6
                    ipv4_addr       = ip4ptr->sin_addr.s_addr;
                    port            = sockaddr.addr.ipv4.sin_port;
#endif // HAVE_IPV6

                    if ( to_ipv4_addr == slp_config.v1_address )
                    {
                        // if this was sent to the multicast port, save it off for a while instead 
                        // of responding immediately.
                        uint8_t index;
    
                        for (index = 0; index < NUM_REPLIES; index++)
                        {
                            // if everything is full and we get out of the loop without saving the addr,
                            // just dump it. 
    
                            if (slp_replies[index].ip_addr == 0)
                            {
                                slp_replies[ index ].ip_addr  = ipv4_addr;
                                slp_replies[ index ].port_num = port;
                                slp_replies[ index ].xid      = slp_header.xid;
                                
                                checkTimers(if_enum, &slp_replies[index].timeout );
                                break;
                            }
                        }
                    }
                    else
                    {
                        sendv1Reply( if_enum, &sockaddr, slp_header.xid );
                    }
                    break;
                case ATTR_RPLY:
                    
                    break;
                case DA_ADVERT:
                    
                    break;
                case SRV_TYPE_RQST:
                    
                    break;
                case SRV_TYPE_RPLY:
                    
                    break;
                default:
                    
                    break;
            }
            free(data1);
            break;
        }
        case SLP_VERSION_2:
        {
            slpv2_header_t slpv2_header;

            DBG_VERBOSE( "Version 2 packet on %s\n", g_ctxt[if_enum].if_name);

            sock_length = sizeof(struct sockaddr_storage);
            error_code = recvfrom(g_ctxt[if_enum].sock,
                                 (char*)&slpv2_header,
                                 sizeof(slpv2_header),
                                 MSG_PEEK,
                                 (struct sockaddr *)&sockaddr, 
                                 ( socklen_t *)&sock_length);

            slpv2_header.flags            = cpu_to_be16( slpv2_header.flags );
            slpv2_header.xid              = cpu_to_be16( slpv2_header.xid );
            slpv2_header.lang_tag_length  = cpu_to_be16( slpv2_header.lang_tag_length );
            slpv2_header.lang_tag         = cpu_to_be16( slpv2_header.lang_tag );

            // get rest of packet
            length = get3Bytes( slpv2_header.length );
            if (length > 1500)
            {
                // bad length. grab part of it and dump it. The receive function will dump anything left.
                sock_length = sizeof(struct sockaddr_storage);
                error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);
                SLP_UNLOCK_CONTEXT();
                DBG_ERR( "Too big - exit %s\n", __FUNCTION__ );
                return;
            }
            data = (char*)malloc(length);
            data1 = data;

            if(data == NULL)
            {
                DBG_ERR("Malloc failed to allocate %d bytes\n", length);
                // grab the header again, but not with a peek. This will dump the packet.
                sock_length = sizeof(struct sockaddr_storage);
                error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);
                SLP_UNLOCK_CONTEXT();
                DBG_ERR( "No data - exit %s\n", __FUNCTION__  );
                return;
            }

            sock_length = sizeof(struct sockaddr_storage);
            error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);

            if (error_code == -1)
            {
                /* Retrieve the socket error for failed recvfrom call */
                DBG_ERR("Receive Error: %d\n", errno);
                free(data1);
                SLP_UNLOCK_CONTEXT();
                DBG_ERR( "Socket Error - exit %s\n", __FUNCTION__  );
                return;
            }
            else
            {
                if (error_code != length)
                {
                    // packet was the wrong length. Dump it.
                    free(data1);
                    SLP_UNLOCK_CONTEXT();
                    DBG_ERR( "error_code != length - exit %s\n", __FUNCTION__  );
                    return;
                }
            }

            data += sizeof( slpv2_header );
            length -= sizeof( slpv2_header );

            switch( slpv2_header.function_id )
            {
                case SRV_REQ:
                    break;
                case SRV_RPLY:
                    break;
                case SRV_REG:
                    break;
                case SRV_DEREG:
                    break;
                case SRV_ACK:
                    break;
                case ATTR_RQST:
                    // check to see if SLP is enabled. If not, just dump the packet. 
//                    Enabled = DSGetIntVar(e_SLPEnabled);
                    if (!g_ctxt_enabled)
                    {
                        DBG_MSG( "Not enabled\n" );
                        break;
                    }
                    DBG_VERBOSE( "V2 Attr rqst\n" );

                    // first check to see if we are in the exclusion list.
                    #ifdef HAVE_IPV6
                    ret_val = slp_in_exclusion_list( if_enum, data, length, 
                                                     ip6ptr->sin6_addr.s6_addr32[0] == 0 );
                    #else
                    ret_val = slp_in_exclusion_list( if_enum, data, length, true );
                    #endif // HAVE_IPV6
                    if ( ret_val )
                    {
                        DBG_VERBOSE( "In exclusion list\n" );
                        break;
                    }

                    // now check to see if we match the URL
                    ret_val = net_slp_match_url (data);
                    if (!ret_val)
                    {
                        DBG_VERBOSE( "URL does not match\n" );
                        break;
                    }

                    // language tag length must be 2
                    if(slpv2_header.lang_tag_length != 2)
                    {
                        DBG_VERBOSE( "Language tag too long\n" );
                        break;
                    }

                    // language must be english 
                    if (slpv2_header.lang_tag != SLP_LANG_CODE )
                    {
                        DBG_VERBOSE( "Wrong language code\n" );
                        break;
                    }

                    // TODO add support for SCOPE comparisons

                    // OK everything matches. Now process it. 
                    if(
                       #ifdef HAVE_IPV6
                       IN6_ARE_ADDR_EQUAL( &slp_config.v2_ipv6_address.s6_addr32, 
                                           &ip6ptr->sin6_addr ) ||
                       ( ip6ptr->sin6_addr.s6_addr32[ 3 ] == slp_config.v2_ipv4_address ) 
                       #else
                       ( ip4ptr->sin_addr.s_addr == slp_config.v2_ipv4_address ) 
                       #endif // HAVE_IPV6
                       ) 
                    {
                        // if this was sent to the multicast port, save it off for a while instead 
                        // of responding immediately.
                        uint8_t index;

                        for (index = 0; index < NUM_REPLIES; index++)
                        {
                            // if everything is full and we get out of the loop without saving the addr,
                            // just dump it. 

                            if( 
                                #ifdef HAVE_IPV6 
                                IN6_IS_ADDR_UNSPECIFIED(&((struct sockaddr_in6*)&slpv2_replies[index].ip_addr)->sin6_addr)
                                #else
                                ( (struct sockaddr_in)(slpv2_replies[index].ip_addr).sin_addr.s_addr == 0 )
                                #endif //HAVE_IPV6                                 
                              )
                            {
                                #ifdef HAVE_IPV6 
                                memcpy(&slpv2_replies[index].ip_addr, &ip6ptr->sin6_addr,
                                       sizeof(struct sockaddr_storage));
                                slpv2_replies[index].port_num = ip6ptr->sin6_port;
                                #else
                                slpv2_replies[index].sin_addr.s_addr = ip4ptr->sin_addr.s_addr;
                                slpv2_replies[index].port_num = ip4ptr->.sin_port;
                                #endif // HAVE_IPV6
                                slpv2_replies[index].xid = slpv2_header.xid;

                                checkTimers(if_enum, &slpv2_replies[index].timeout);
                                break;
                            }
                        }
                    }
                    else
                    {
                        sendv2Reply( if_enum, &sockaddr, slpv2_header.xid );
                    }
                    break;
                case ATTR_RPLY:
                    break;
                case DA_ADVERT:
                    break;
                case SRV_TYPE_RQST:
                    break;
                case SRV_TYPE_RPLY:
                    break;
                default:
                    break;
            }
            free(data1);

            break;
        }
        default:
            DBG_ERR( "Received version %d packet on %s - threw out\n", g_ctxt[if_enum].if_name, slp_header.ver );
            DumpSLPHeader( &slp_header );
            // grab the header again, but not with a peek. This will dump the packet.
            sock_length = sizeof(struct sockaddr_storage);
            error_code = recvmsg(g_ctxt[if_enum].sock, &msg, 0);
    }

    SLP_UNLOCK_CONTEXT();
    return;
}
//#endif
static void slp_send_reply(UINT if_enum)
{
    uint32_t cur_time = 0;
    uint8_t index;
    struct sockaddr_storage sockaddr;
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6ptr = (struct sockaddr_in6*)&sockaddr;
#else
    struct sockaddr_in  *ip4ptr = (struct sockaddr_in*)&sockaddr;
#endif

    SLP_LOCK_CONTEXT();

    // is there anything in the response list that we need to process?
//    cur_time = tx_time_get();
    cur_time = (uint32_t)posix_gettime_ticks();
    for (index = 0; index < NUM_REPLIES; index++)
    {
        if (slp_replies[index].ip_addr != 0 && 
            cur_time >= slp_replies[index].timeout)
        {
#ifdef HAVE_IPV6
          ip6ptr->sin6_family            = PF_INET6;
          ip6ptr->sin6_port              = slp_replies[index].port_num;
          ip6ptr->sin6_addr.s6_addr32[0] = (u_long) 0;
          ip6ptr->sin6_addr.s6_addr32[1] = (u_long) 0;
          ip6ptr->sin6_addr.s6_addr32[2] = (u_long) htonl ( 0xFFFF );
          ip6ptr->sin6_addr.s6_addr32[3] = (u_long) slp_replies[ index ].ip_addr;
#else // !HAVE_IPV6
          ip4ptr->sin_family  = PF_INET;
          ip4ptr->sin_port    = htons( slp_replies[index].port_num);
          ip4ptr->sin_addr    = slp_replies[index].ip_addr;
#endif // HAVE_IPV6

            sendv1Reply( if_enum, &sockaddr, slp_replies[ index ].xid );
            slp_replies[index].ip_addr = 0;
        }

        if (
            #ifdef HAVE_IPV6
            !IN6_IS_ADDR_UNSPECIFIED(&((struct sockaddr_in6*)&slpv2_replies[index].ip_addr)->sin6_addr) && 
            #else
            ( ((struct sockaddr_in*)&slpv2_replies[index].ip_addr)->sin_addr.s_addr != 0 ) && 
            #endif // HAVE_IPV6
            cur_time >= slpv2_replies[index].timeout)
        {
            #ifdef HAVE_IPV6
            memcpy(&ip6ptr, &slpv2_replies[index].ip_addr, 
                 sizeof(struct sockaddr_storage)); 
            ip6ptr->sin6_family      = AF_INET6;
            ip6ptr->sin6_port        = slpv2_replies[index].port_num;
            #else
            // TODO: This doesn't seem right to me.  array ip addr may be wrong.
            ip4ptr->sin_family  = PF_INET;
            ip4ptr->sin_port    = slpv2_replies[index].port_num; 
            ip4ptr->sin_addr    = slpv2_replies[index].ip_addr; 
            #endif // HAVE_IPV6

            sendv2Reply( if_enum, &sockaddr, slpv2_replies[ index ].xid );
            memset( &slpv2_replies[ index ].ip_addr, 0, sizeof( struct sockaddr_storage ) );
        }
    }

    // see if there are any other replies in the list that haven't timed out yet
    // if yes then set up the smallest timer amount
    for (index = 0; index < NUM_REPLIES; index++)
    {
        if (slp_replies[index].ip_addr != 0)
        {
            uint32_t curTicks = (uint32_t)posix_gettime_ticks();

            //if this reply's timer has already expired we need to
            //send it out right away
            if (curTicks >= slp_replies[index].timeout)
            {
                cur_time = 1;
                break;
            }
            //see if we need to decrease cur_time
            //if the time remaining on this reply is less than cur_time set
            //cur_time to the time remaining. We know curTicks is less
            //than timeout here because of the preceding if statement
            else if((slp_replies[index].timeout - curTicks) < cur_time)
            {
                cur_time = slp_replies[index].timeout - curTicks;
            }
        }

        if (
            #ifdef HAVE_IPV6
            !IN6_IS_ADDR_UNSPECIFIED(&((struct sockaddr_in6*)&slpv2_replies[index].ip_addr)->sin6_addr)
            #else
            ((struct sockaddr_in*)&slpv2_replies[ index ].ip_addr)->sin_addr.s_addr != 0
            #endif // HAVE_IPV6
            )
        {
            uint32_t curTicks = (uint32_t)posix_gettime_ticks();

            //if this reply's timer has already expired we need to
            //send it out right away
            if (curTicks >= slpv2_replies[index].timeout)
            {
                cur_time = 1;
                break;
            }
            //see if we need to decrease cur_time
            //if the time remaining on this reply is less than cur_time set
            //cur_time to the time remaining. We know curTicks is less
            //than timeout here because of the preceding if statement
            else if((slpv2_replies[index].timeout - curTicks) < cur_time)
            {
                cur_time = slpv2_replies[index].timeout - curTicks;
            }
        }
    }
    
    if(cur_time)
    {
        // set up the timer again. 
        slp_start_reply_timer(if_enum, cur_time);
    }

    SLP_UNLOCK_CONTEXT();
}
// This is not being used until some other functions and functionality for socket
// event notification is brought in with the network manager application 
//#if 0
static uint8_t slp_in_exclusion_list (UINT if_enum, char *data, unsigned short pktlen, bool use_v4_address )
{
    uint16_t length;
    char *tmp_data;
    char *str;
    char ipv4_addr_string[ 16 ];
    uint32_t ip_value;
    struct ifreq ifr;

    #ifdef HAVE_IPV6
    char ipv6_addr_string[INET6_ADDRSTRLEN];
//    net_iface_ipv6_addr_t         ipv6_addr;
    struct in6_addr         ipv6_addr;
    #endif // HAVE_IPV6

    unsigned int if_index;
    if ((if_index = if_nametoindex(g_ctxt[if_enum].if_name)) == 0) {
        DBG_ERR( "No index found for %s interface\n", g_ctxt[if_enum].if_name);
        return 1;
    }
//    unsigned int if_index = net_iface_enum_to_if_index(if_enum);

    // exclusion list is right behind the header;
    tmp_data = data; 
    length = cpu_to_be16( *((uint16_t *) tmp_data) );
    if (length == 0)
    {
        // nothing in the list
        return 0;
    }
    else if (length > pktlen)
    {
        // bad packet
        DBG_ERR( "exclustion list on %s - Bad packet\n ", g_ctxt[if_enum].if_name );
        return 1;
    }

    // Advance past the length field
    tmp_data += 2;


     /* to get an IPv4 IP address */
     ifr.ifr_addr.sa_family = AF_INET;

     /* want IP address attached to name */
     strncpy(ifr.ifr_name, g_ctxt[if_enum].if_name, IF_NAMESIZE-1);

     if( ioctl(g_ctxt[if_enum].sock, SIOCGIFADDR, &ifr) != OK)
    {
        ip_value = 0;
    }
    inet_ntop( AF_INET, (void *)&((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr,
                ipv4_addr_string, INET_ADDRSTRLEN );

    #ifdef HAVE_IPV6
     /* to get an IPv6 IP address */
     ifr.ifr_addr.sa_family = AF_INET6;

     /* want IP address attached to name */
     strncpy(ifr.ifr_name, g_ctxt[if_enum].if_name, IF_NAMESIZE-1);

     if( ioctl(g_ctxt[if_enum].sock, SIOCGIFADDR, &ifr) != OK)
    {
        memset( &ipv6_addr, 0x00, sizeof( struct in6_addr ) );
    }
    inet_ntop(AF_INET6, (void *)&((struct sockaddr_in6 *)&ifr.ifr_addr)->sin6_addr, 
              ipv6_addr_string, INET6_ADDRSTRLEN);
    #endif // HAVE_IPV6

    // just look for IP address. We don't support host name currently.
    // exclusion list will be in the format xxx.xxx.xxx.xxx with addresses
    // separated by commas. 
    while (length > 0)
    {
        if ( ( use_v4_address == true ) && 
             ( memcmp( ipv4_addr_string, tmp_data, strlen( ipv4_addr_string ) ) == 0 ) )
        {
            DBG_VERBOSE( "Exclusion list v4: %s\n", tmp_data );
            // we are in the exclusion list 
            return 1;
        }
        #ifdef HAVE_IPV6
        else if ( ( use_v4_address == false ) &&
                  ( strncasecmp( ipv6_addr_string, tmp_data, strlen( ipv6_addr_string ) ) == 0 ) )
        {
            DBG_VERBOSE( "Exclusion list v4: %s\n", tmp_data );
            // we are in the exclusion list 
            return 1;
        }
        #endif // HAVE_IPV6
        else
        {
            str= (char*)memchr (tmp_data, ',', length);
            if (str != NULL)
            {
                length -= (str - tmp_data + 1);
                tmp_data = str + 1;
            }
            else
            {
                length = 0;
            }
        }
    }

    return 0;
}
//#endif

// This function takes a pointer to 3 bytes of data and converts it to a uint32_t
static uint32_t get3Bytes(unsigned char* data)
{
    uint32_t result;
    result = (data[0]) << 16;
    result |= (data[1]) << 8;
    result |= (data[2]);
    return result;
}

// This function takes a uint32_t and writes the lower 3 bytes to the char pointer
// If the upper byte is not 0, function will return -1 -- otherwise it returns 0
static int8_t set3Bytes(uint32_t val, char* data)
{
    if((val & 0xFF000000) != 0)
    {
        return -1;
    }
    data[0] = (val & 0xFF0000) >> 16;
    data[1] = (val & 0xFF00) >> 8;
    data[2] = val & 0xFF;

    return 0;
}

static void sendv1Reply(UINT if_enum, struct sockaddr_storage *sockaddr, uint16_t xid )
{
    int32_t error_code;
//    char * str_error;
    uint32_t length;
    slpv1_attr_reply_t* reply;

    reply = ( slpv1_attr_reply_t* )malloc( sizeof( slpv1_attr_reply_t ) + slp_config.max_attr_list_length );

    memset(reply, 0x00, sizeof( sizeof( slpv1_attr_reply_t ) + slp_config.max_attr_list_length ) );
    net_slp_add_attr_list( &reply->attr_list, SLP_VERSION_1, (struct in6_addr*)sockaddr );

    reply->header.ver           = 1;
    reply->header.function_id   = ATTR_RPLY;
    length                      = sizeof( slpv1_header_t ) + sizeof( unsigned int ) + strlen( &reply->attr_list );
    reply->header.length        = cpu_to_be16( length );
    reply->header.flags         = 0;
    reply->header.dialect       = 0;
    reply->header.lang_code     = cpu_to_be16( SLP_LANG_CODE );
    reply->header.char_encode   = cpu_to_be16( SLP_CHAR_CODE );
    reply->header.xid           = cpu_to_be16( xid );
    reply->err_code             = cpu_to_be16( 0 );
    reply->length               = cpu_to_be16( strlen( &reply->attr_list ) );

    error_code = sendto(g_ctxt[if_enum].sock,
                       (char*)reply,
                       length,
                       0,
                       (struct sockaddr *)sockaddr,
                       sizeof(struct sockaddr_storage));

    if ( error_code == -1 )
    {
        // Retrieve the socket error for failed recvfrom call 
        DBG_ERR("SLPV1 SendTo Error on %s: %d\n", g_ctxt[if_enum].if_name, errno);
    }
    free(reply);
}

static void sendv2Reply(UINT if_enum, struct sockaddr_storage* sockaddr, uint16_t xid )
{
    int32_t error_code;
    uint32_t length;
    slpv2_attr_reply_t* reply;

    reply = ( slpv2_attr_reply_t* )malloc( sizeof( slpv2_attr_reply_t ) + slp_config.max_attr_list_length );
    memset(reply, 0x00, sizeof( sizeof( slpv2_attr_reply_t ) + slp_config.max_attr_list_length ) );
    
    net_slp_add_attr_list( &reply->attr_list, SLP_VERSION_2, (struct in6_addr*)sockaddr );

    reply->header.ver           = 2;
    reply->header.function_id   = ATTR_RPLY;
    length = sizeof( slpv2_header_t ) + SIZE_OF_ERROR_CODE + SIZE_OF_ATTR_LENGTH + strlen( &reply->attr_list ) + NUM_OF_ATTR_AUTHS;
    if( set3Bytes(length, (char*)&reply->header.length[0]) != 0 )
    {
        DBG_ERR( "data length exceeds 3 byte limit\n" );
        return;
    }
    reply->header.flags = cpu_to_be16( 0 );
    if( set3Bytes( 0, (char*)&reply->header.next_ext_offset[ 0 ] ) != 0 )
    {
        DBG_ERR( "next_ext_offset exceeds 3 byte limit\n" );
    }
    reply->header.xid               = cpu_to_be16( xid );
    reply->header.lang_tag_length   = cpu_to_be16( 2 );
    reply->header.lang_tag          = cpu_to_be16( SLP_LANG_CODE ) ;
    reply->err_code                 = cpu_to_be16( 0 );
    reply->length                   = cpu_to_be16( strlen( &reply->attr_list ) );
    #ifdef HAVE_IPV6
//    unsigned int iface_id = net_iface_enum_to_if_index(if_enum);
    unsigned int iface_id = if_nametoindex(g_ctxt[if_enum].if_name);
    if(!iface_id)
    {
        DBG_ERR("if_nametoindex failed!\n");
        ASSERT(0);
    }
    ((struct sockaddr_in6*)&sockaddr)->sin6_scope_id = iface_id;
    #endif // HAVE_IPV6

    error_code = sendto(g_ctxt[if_enum].sock,
                       (char*)reply,
                       length,
                       0,
                       (struct sockaddr *)sockaddr,
                       sizeof(struct sockaddr_storage));

    if ( error_code == -1 )
    {
        /* Retrieve the socket error for failed recvfrom call */
        DBG_ERR("SLPV2 sendto error: %d\n", errno);
    }
    free(reply);
}

static void checkTimers(UINT if_enum, uint32_t* timeout)
{
    uint8_t i;
    uint32_t cur_time;

    // set up a random time between 0 and 500 milliseconds
    srand(time(NULL));
    cur_time = (uint32_t) rand();
    cur_time = (cur_time %(SYS_TICK_FREQ / 2)); 
    cur_time++; //make sure isn't 0

    *timeout = (uint32_t)posix_gettime_ticks() + cur_time;

    for (i = 0; i < NUM_REPLIES; i++)
    {
		if ((slp_replies[i].ip_addr != 0) &&
			(slp_replies[i].timeout < *timeout))
        {
            cur_time = 0; // an earlier timer callback is already scheduled
            break;
        }
        if (
            #ifdef HAVE_IPV6
            !IN6_IS_ADDR_UNSPECIFIED(&((struct sockaddr_in6*)&slpv2_replies[i].ip_addr)->sin6_addr) &&
            #else
            ( ((struct sockaddr_in*)&slpv2_replies[ i ].ip_addr)->sin_addr.s_addr != 0 ) &&
            #endif // HAVE_IPV6
            slpv2_replies[i].timeout < *timeout)
		{
			cur_time = 0; // an earlier timer callback is already scheduled
			break;
		}
    }

    // start the timer
    if (cur_time != 0)
    {
        slp_start_reply_timer(if_enum, cur_time);
    }

    return;
}

//void slp_app_execute(const uint32_t hndl, void *data1, void *data2, uint32_t flags)
void slp_send_msg(UINT if_enum, slp_msg_t msg)
{
    UINT pthread_rcode;
    slp_msg_buf_t buf;

    buf.if_enum = if_enum;
    buf.msg = msg;

    pthread_rcode = posix_message_send(slp_msgq, (char*)&buf, sizeof(buf), 
//                       MQ_DEFAULT_PRIORITY, 0); 
    MQ_DEFAULT_PRIORITY, 1); 

    if(pthread_rcode != 0)
    {
        XASSERT(0, pthread_rcode);
    }

    return;
}

void stud_to_call_functions()
{
SLPSocketCBFunc(0,0,0);
slp_network_event_func(0,0);
}
//void net_app_register(const char *name, const net_app_func_table_t *func_table, net_app_handle_t *hndl)
//{
//    ASSERT(name);
//    ASSERT(func_table);
//    ASSERT(hndl);

//    DBG_VERBOSE("SLP Registration complete \n", app->name);
//    return;
//}
