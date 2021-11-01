/*
 * ============================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#else
#include "trsocket.h"
#endif
#include "pthread.h"
#include "posix_ostools.h"
#include "ATypes.h"
#include "sys_init_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "error_types.h"
#include "logger.h"
#include "io_device.h"
#include "net_logger.h"
#include "net_io_api.h"
#include "net_raw_io_api.h"
#include "net_api.h"
#include "ctype.h"
//#define USE_UNIX_DOMAIN_SOCKET
#ifdef USE_UNIX_DOMAIN_SOCKET
#include <sys/un.h>
#define INVITE_SOCKET_PATH "/tmp/invitesocket"
#endif

#ifdef HAVE_NET_LPD
#include <poll.h>
#define LPD_BUF_SIZE (32*1024 + 1)
#define SEND_OK(sock) send(sock, "\0", 1, MSG_NOSIGNAL)

#endif
//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "NETRAW: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_RAW_IO 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)

//=========================================================================================================================
// Defines
//=========================================================================================================================

#define NET_RAW_IO_INVITE_PORT         4490

#ifdef HAVE_IPV6
#define NET_RAW_IO_LOOPBACK_ADDR       "::1"
#else // !HAVE_IPV6
#define NET_RAW_IO_LOOPBACK_ADDR       "127.0.0.1"
#endif // HAVE_IPV6

#define NET_RAW_IO_MSG_LIST_INSERT(_item_) net_raw_io_list_insert_head(&raw_io_msg_list, (void*)_item_)
#define NET_RAW_IO_MSG_LIST_GET(_index_) net_raw_io_list_get(raw_io_msg_list, _index_)
#define NET_RAW_IO_MSG_LIST_LOOKUP(_sock_) net_raw_io_list_lookup(raw_io_msg_list, _sock_)
#define NET_RAW_IO_MSG_LIST_REMOVE(_item_) net_raw_io_list_remove(&raw_io_msg_list, (void*)_item_)

#define NET_RAW_IO_CLIENT_LIST_INSERT(_item_) net_raw_io_list_insert_head((net_raw_io_list_header_t **)&raw_io_client_list, (void*)_item_)
#define NET_RAW_IO_CLIENT_LIST_GET(_index_) net_raw_io_list_get((net_raw_io_list_header_t *)raw_io_client_list, _index_)
#define NET_RAW_IO_CLIENT_LIST_LOOKUP(_sock_) net_raw_io_list_lookup((net_raw_io_list_header_t *)raw_io_client_list, _sock_)
#define NET_RAW_IO_CLIENT_LIST_REMOVE(_item_) net_raw_io_list_remove((net_raw_io_list_header_t **)&raw_io_client_list, (void*)_item_)

#define NET_RAW_IO_SESSION_LIST_INSERT(_client_,_item_) net_raw_io_session_list_insert_head(_client_, (void*)_item_)
#define NET_RAW_IO_SESSION_LIST_LOOKUP(_client_,_sock_) net_raw_io_session_list_lookup(_client_, _sock_)
#define NET_RAW_IO_SESSION_LIST_REMOVE(_client_,_item_) net_raw_io_session_list_remove(_client_, (void*)_item_)

#define NET_RAW_IO_GLOBAL_LOCK() \
        { \
            int px_status; \
            px_status = pthread_mutex_lock( &rawio_global_mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_GLOBAL_UNLOCK() \
        { \
            int px_status; \
            px_status = pthread_mutex_unlock( &rawio_global_mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_LOCK_CLIENT_CONTEXT(client_ctxt) \
        { \
            int px_status; \
            px_status = pthread_mutex_lock( &client_ctxt->mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client_ctxt) \
        { \
            int px_status; \
            px_status = pthread_mutex_unlock( &client_ctxt->mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_LOCK_SESSION_CONTEXT(session_ctxt) \
        { \
            int px_status; \
            px_status = pthread_mutex_lock( &session_ctxt->mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_UNLOCK_SESSION_CONTEXT(session_ctxt) \
        { \
            int px_status; \
            px_status = pthread_mutex_unlock( &session_ctxt->mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define NET_RAW_IO_STATE_TRANSITION(session_ctxt, next_state) \
        { \
            ASSERT(session_ctxt->client_ctxt); \
            DBG_VERBOSE("%s %08x from %s to %s\n", session_ctxt->client_ctxt->name, session_ctxt, g_net_raw_io_state_strs[session_ctxt->state], g_net_raw_io_state_strs[next_state]); \
            session_ctxt->state = next_state; \
        }

// thread stack size
#define NET_RAW_IO_STACK_SIZE POSIX_MIN_STACK_SIZE

// listen socket default backlog -- may be overriden by registered client application
#define NET_RAW_IO_DEFAULT_BACKLOG 5


//=========================================================================================================================
// Data types
//=========================================================================================================================

typedef enum
{
    NET_RAW_IO_MSG_NEW_CLIENT,
    NET_RAW_IO_MSG_ACCEPT_PENDING,
    NET_RAW_IO_MSG_SESSION_INACTIVE,
    NET_RAW_IO_MSG_RECV_DATA_PENDING,
    NET_RAW_IO_MSG_SESSION_ACTIVE,
    NET_RAW_IO_MSG_HAVE_SEND_DATA,
    NET_RAW_IO_MSG_CONNECTION_CLOSED,
    NET_RAW_IO_MSG_CLOSE,

    // add new msgs above this line
    NET_RAW_IO_NUM_MSGS
} net_raw_io_msg_type_t;


static const char *g_net_raw_io_msg_strs[NET_RAW_IO_NUM_MSGS] = 
{
#ifdef HAVE_DBG_PRINTF
    "NET_RAW_IO_MSG_NEW_CLIENT",
    "NET_RAW_IO_MSG_ACCEPT_PENDING",
    "NET_RAW_IO_MSG_SESSION_INACTIVE",
    "NET_RAW_IO_MSG_RECV_DATA_PENDING",
    "NET_RAW_IO_MSG_SESSION_ACTIVE",
    "NET_RAW_IO_MSG_HAVE_SEND_DATA",
    "NET_RAW_IO_MSG_CONNECTION_CLOSED",
    "NET_RAW_IO_MSG_CLOSE",
#endif //HAVE_DBG_PRINTF
};


typedef enum
{
    NET_RAW_IO_STATE_OPENING,
    NET_RAW_IO_STATE_WAIT_RECV_DATA,
    NET_RAW_IO_STATE_READY,
    NET_RAW_IO_STATE_ACTIVE,
    NET_RAW_IO_STATE_CLOSING, // IMPORTANT: do not send messages to a session context in this state!

    // add new states above this line
    NET_RAW_IO_NUM_STATES
} net_raw_io_state_t;


static const char *g_net_raw_io_state_strs[NET_RAW_IO_NUM_STATES] = 
{
#ifdef HAVE_DBG_PRINTF
    "NET_RAW_IO_STATE_OPENING",
    "NET_RAW_IO_STATE_WAIT_RECV_DATA",
    "NET_RAW_IO_STATE_READY",
    "NET_RAW_IO_STATE_ACTIVE",
    "NET_RAW_IO_STATE_CLOSING",
#endif
};


#define SOCK_FD_READ    0x01
#define SOCK_FD_WRITE   0x02
#define SOCK_FD_ERROR   0x04

// standard context header
typedef struct
{
    void        *next;
    int         sock;
    uint32_t    sock_fd_flags;
} net_raw_io_list_header_t;

struct net_raw_io_session_ctxt_s;

typedef enum
{
    NET_LPD_CONTROL_CMD,
    NET_LPD_CONTROL_FILE,
    NET_LPD_DATA_CMD,
    NET_LPD_DATA_FILE,
    NET_LPD_DATA_FILE_PROCESSED,
    NET_LPD_STATE_OTHER,
} net_lpd_state_t;

typedef struct net_lpd_info_s
{
    int                                 dataSizeLPD;
    bool                                net_closed;
    bool                                dataStream;
    bool                                isDataSizeProvided;	
    net_lpd_state_t                     lpd_state;
} net_lpd_info_t;

typedef struct 
{
    net_raw_io_list_header_t            hdr;

    Channel_type_t                      type;
    uint16_t                            port;
    const char                          *name;
    int                                 message_sock;
    net_raw_io_sock_config_func_ptr_t   sock_config_func_ptr;
    pthread_mutex_t                     mtx;
    struct net_raw_io_session_ctxt_s    *session_list; // list of all open sessions for this client
} net_raw_io_client_ctxt_t;

typedef struct net_raw_io_session_ctxt_s
{
    net_raw_io_list_header_t            hdr;

    net_raw_io_client_ctxt_t            *client_ctxt;
    net_raw_io_state_t                  state;
    net_io_session_hndl_t               session_hndl;
    bool                                net_closed;
    net_lpd_info_t                      *lpd_info;
    pthread_mutex_t                     mtx;
} net_raw_io_session_ctxt_t;

/* message format */
typedef struct
{
    net_raw_io_client_ctxt_t *client;
    net_raw_io_msg_type_t type;
    void *data; // app-specific message data
} net_raw_io_msg_t;


//=========================================================================================================================
// Global statics
//=========================================================================================================================

// main thread
static pthread_mutex_t rawio_global_mtx;
static pthread_t rawio_thread_id;
ALIGN8 static unsigned char rawio_thread_stack[NET_RAW_IO_STACK_SIZE];

// Socket file descriptors used in select()
static fd_set rawio_read_socks;

static int rawio_invite_sock;
#ifndef USE_UNIX_DOMAIN_SOCKET
static int rawio_invite_port = NET_RAW_IO_INVITE_PORT;
#endif
static int rawio_nfds;

static net_raw_io_list_header_t *raw_io_msg_list = NULL;

static net_raw_io_client_ctxt_t *raw_io_client_list = NULL;
        
        
//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static int32_t net_raw_io_recv_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt);
#ifdef HAVE_NET_LPD
static int32_t net_lpd_io_recv_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt);
#endif
static void net_raw_io_notify_func(net_io_session_event_t event, void *user_ctxt);
static void *net_raw_io_main( void *unused );


//=========================================================================================================================
// Linked List Helper Routines
//=========================================================================================================================

void *net_raw_io_list_insert_head(net_raw_io_list_header_t **list, void *item)
{
    net_raw_io_list_header_t *temp = (net_raw_io_list_header_t *)item;
    ASSERT( list != NULL );

    NET_RAW_IO_GLOBAL_LOCK();
    // add to list
    temp->next = *list;
    *list = temp;
    NET_RAW_IO_GLOBAL_UNLOCK();
    
    return temp;
}

void *net_raw_io_list_get(net_raw_io_list_header_t *list, uint32_t index)
{
    uint32_t i = 0;
    net_raw_io_list_header_t *cur;

    NET_RAW_IO_GLOBAL_LOCK();
    cur = list;

    while(cur)
    {
        if(i == index)
        {
            break;
        }

        i++;
        cur = cur->next;
    }
    NET_RAW_IO_GLOBAL_UNLOCK();

    return cur;
}

void *net_raw_io_list_lookup(net_raw_io_list_header_t *list, int sock)
{
    net_raw_io_list_header_t *cur;

    NET_RAW_IO_GLOBAL_LOCK();
    cur = list;

    while(cur)
    {
        if(cur->sock == sock)
        {
            break;
        }

        cur = cur->next;
    }
    NET_RAW_IO_GLOBAL_UNLOCK();

    return cur;
}

void *net_raw_io_list_remove(net_raw_io_list_header_t **list, void *item)
{
    net_raw_io_list_header_t *cur, **prev;

    ASSERT( list != NULL );
    ASSERT( item != NULL );
    
    NET_RAW_IO_GLOBAL_LOCK();
    cur = *list;
    prev = list;

    while(cur)
    {
        if(cur == item)
        {
            // remove from the list
            *prev = cur->next;
            break;
        }
        
        prev = (net_raw_io_list_header_t **)&cur->next;
        cur = cur->next;
    }
    NET_RAW_IO_GLOBAL_UNLOCK();

    return cur;
}

void net_raw_io_session_list_insert_head(net_raw_io_client_ctxt_t *client, net_raw_io_session_ctxt_t *session)
{
    DBG_VERBOSE("Adding session 0x%x for client %s\n", session, client->name);
    NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
    ASSERT(client);
    ASSERT(session);
    ASSERT(session->hdr.next == NULL);
    session->hdr.next = client->session_list;
    client->session_list = session;
    NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
    return;
}

void net_raw_io_session_list_remove(net_raw_io_client_ctxt_t *client, net_raw_io_session_ctxt_t *session)
{
    ASSERT(client);
    ASSERT(session);
    NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
    net_raw_io_session_ctxt_t *cur_session = client->session_list;
    net_raw_io_session_ctxt_t **prev = &client->session_list;
    while(cur_session)
    {
        if(cur_session == session)
        {
            DBG_VERBOSE("Removing session 0x%x for client %s\n", cur_session, client->name);
            // remove the item from the list
            *prev = cur_session->hdr.next;
            cur_session->hdr.next = NULL;
            break;
        }
        prev = (net_raw_io_session_ctxt_t **)&cur_session->hdr.next;
        cur_session = cur_session->hdr.next;
    }
    NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
    return;
}

net_raw_io_session_ctxt_t *net_raw_io_session_list_get_head(net_raw_io_client_ctxt_t *client)
{
    ASSERT(client);
    NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
    net_raw_io_session_ctxt_t *session = client->session_list;
    NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
    return session;
}

net_raw_io_session_ctxt_t *net_raw_io_session_list_get_next(net_raw_io_client_ctxt_t *client, net_raw_io_session_ctxt_t *session_ctx)
{
    NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
    ASSERT(client);
    net_raw_io_session_ctxt_t *session = client->session_list;
    // is the session still on the client list
    while(session)
    {
        if(session == session_ctx)
        {
            // found the session; return next session
            session = session->hdr.next;
            break;
        }
        session = session->hdr.next;
    }
    NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
    return session;
}

net_raw_io_session_ctxt_t *net_raw_io_session_list_lookup(net_raw_io_client_ctxt_t *client, int session_socket)
{
    ASSERT(client);
    NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
    net_raw_io_session_ctxt_t *session = client->session_list;
    while(session)
    {
        if(session->hdr.sock == session_socket)
        {
            // found the session
            break;
        }
        session = session->hdr.next;
    }
    NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
    return session;
}

int32_t net_raw_io_send_msg(net_raw_io_client_ctxt_t *client_ctxt, net_raw_io_msg_type_t type, void *data)
{
    int sock_rcode;
    
    ASSERT(client_ctxt);

    net_raw_io_msg_t msg;

    memset(&msg, 0, sizeof(net_raw_io_msg_t));
    msg.client = client_ctxt;
    msg.type = type;
    msg.data = data;

	/* send a message to the message socket */
#ifdef HAVE_DBG_PRINTF
    DBG_VERBOSE("%s: sending message %s\n", client_ctxt->name, g_net_raw_io_msg_strs[type]);
#endif    
	sock_rcode = send(client_ctxt->message_sock, (void *)&msg, sizeof(net_raw_io_msg_t), 0);
    if(sock_rcode == -1)
    {
        int sock_error = thread_errno;
        DBG_ERR("%s: message socket send error (sock_error=%d)\n", client_ctxt->name, sock_error);
    
        if(sock_error == EWOULDBLOCK || sock_error == EAGAIN)
        {
            /* 
             *  NOTE: should only get here when the socket's file descriptor is marked O_NONBLOCK 
             *  and the requested operation would block
             */
            return NET_RAW_IO_WOULDBLOCK;
        }
        else 
        {
            // any other errors are unexpected and unhandled
            XASSERT(0, sock_error);
        }
    }
    
    return NET_RAW_IO_OK;
}

#ifdef __linux__
void *net_raw_io_get_in_addr( struct sockaddr *sa )
{
#ifdef HAVE_IPV6
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
#else    
    return &(((struct sockaddr_in*)sa)->sin_addr);
#endif    
}
#endif

#ifdef USE_UNIX_DOMAIN_SOCKET
void net_raw_io_handle_new_connection() 
{
    int sock;
    DBG_VERBOSE("handle new connection\n");
    // accept the pending message connection
    sock = accept(rawio_invite_sock, NULL, NULL);
    if(sock == -1)
    {
        DBG_ERR("message socket accept error (sock_error=%d)\n", thread_errno);
    }
    else
    {
        //DBG_VERBOSE("Adding new connection, sock = %d from ip = %s\n", sock, addr_str);
        DBG_VERBOSE("Adding new connection, sock = %d \n", sock);
        // add to msg list
        net_raw_io_list_header_t *new_msg = MEM_MALLOC(sizeof(net_raw_io_list_header_t));
        ASSERT( new_msg != NULL );
        memset(new_msg, 0, sizeof(net_raw_io_list_header_t));
        new_msg->sock = sock;
        NET_RAW_IO_MSG_LIST_INSERT( new_msg );
    }
}
#else
void net_raw_io_handle_new_connection() 
{
    int sock;
    struct sockaddr_storage sock_addr;
#ifdef __linux__
    socklen_t addr_len;
#else
    int addr_len;
#endif
    
    DBG_VERBOSE("handle new connection\n");
    
    // TODO: should we filter and only accept localhost address for message socket?
    memset(&sock_addr, 0, sizeof(struct sockaddr_storage)); 
    addr_len = sizeof(struct sockaddr_storage);
    
    // accept the pending message connection
    sock = accept(rawio_invite_sock, (struct sockaddr *)&sock_addr, &addr_len);
    if(sock == -1)
    {
        DBG_ERR("message socket accept error (sock_error=%d)\n", thread_errno);
    }
    else
    {
#ifdef __linux__
#ifdef HAVE_IPV6
        char addr_str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, net_raw_io_get_in_addr((struct sockaddr *)&sock_addr), addr_str, INET6_ADDRSTRLEN);
#else // !HAVE_IPV6
        char addr_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, net_raw_io_get_in_addr((struct sockaddr *)&sock_addr), addr_str, INET_ADDRSTRLEN);
#endif // HAVE_IPV6
#else
#ifdef HAVE_IPV6
        char addr_str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, (void *)(&sock_addr.addr.ipv6.sin6_addr), addr_str, INET6_ADDRSTRLEN);
#else // !HAVE_IPV6
        char addr_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, (void *)(&sock_addr.addr.ipv4.sin_addr), addr_str, INET_ADDRSTRLEN);
#endif // HAVE_IPV6
#endif // __linux__

        // only accept connections from the loopback address on message socket
        if (strcmp(addr_str, NET_RAW_IO_LOOPBACK_ADDR) == 0)
        {
            DBG_VERBOSE("Adding new connection, sock = %d from ip = %s\n", sock, addr_str);
            
            // add to msg list
            net_raw_io_list_header_t *new_msg = MEM_MALLOC(sizeof(net_raw_io_list_header_t));
            ASSERT( new_msg != NULL );
            memset(new_msg, 0, sizeof(net_raw_io_list_header_t));
            new_msg->sock = sock;
            NET_RAW_IO_MSG_LIST_INSERT( new_msg );
        }
        else
        {
            DBG_ERR("Rejecting non-loopback connection, sock = %d from ip = %s\n", sock, addr_str);
#ifdef __linux__
            close(sock);
#else                
            tfClose(sock);
#endif                
            sock = -1;
        }
    }
}
#endif

net_raw_io_session_ctxt_t *net_raw_io_open(net_raw_io_client_ctxt_t *client_ctxt)
{
    ASSERT(client_ctxt);

    int px_status;
    int32_t net_io_rcode;
    net_io_func_table_t func_table;
    net_raw_io_session_ctxt_t *session_ctxt = NULL;
    bool mtx_created = false;

    func_table.io_notify = net_raw_io_notify_func;
#ifdef HAVE_NET_LPD
    if(client_ctxt->port == NET_LPD_PORT)
        func_table.io_recv = net_lpd_io_recv_func;
    else
        func_table.io_recv = net_raw_io_recv_func;
#else
    func_table.io_recv = net_raw_io_recv_func;
#endif

    session_ctxt = (net_raw_io_session_ctxt_t *)MEM_MALLOC(sizeof(net_raw_io_session_ctxt_t));
    if(!session_ctxt)
    {
        goto error;
    }
    // initialize context
    memset(session_ctxt, 0, sizeof(net_raw_io_session_ctxt_t));
#ifdef HAVE_NET_LPD
        if(client_ctxt->port == NET_LPD_PORT)
        {
            session_ctxt->lpd_info = (net_lpd_info_t *)MEM_MALLOC(sizeof(net_lpd_info_t));
            if(!session_ctxt->lpd_info)
            {
                goto error;
            }
            session_ctxt->lpd_info->dataSizeLPD = 0;
            session_ctxt->lpd_info->isDataSizeProvided = false;
            session_ctxt->lpd_info->dataStream = false;
        }
        else
        {
            session_ctxt->lpd_info = NULL;
        }
#else
        session_ctxt->lpd_info = NULL;
#endif
    session_ctxt->hdr.sock = -1;
    session_ctxt->hdr.sock_fd_flags = SOCK_FD_READ;
    session_ctxt->client_ctxt = client_ctxt;
    session_ctxt->state = NET_RAW_IO_STATE_OPENING;
    session_ctxt->net_closed = false;

    px_status = posix_mutex_init( &session_ctxt->mtx );
    if(px_status != 0)
    {
        DBG_ERR("error creating session mutex for %s (px_status=0x%02x)\n", client_ctxt->name, px_status);
        goto error;
    }
    mtx_created = true;

    net_io_rcode = net_io_open(client_ctxt->type, &func_table, (void *)session_ctxt, &session_ctxt->session_hndl);
    if(net_io_rcode != NET_IO_SUCCESS)
    {
        goto error;
    }

    // add to client's list of open sessions
    net_raw_io_session_list_insert_head(client_ctxt, session_ctxt);

    return session_ctxt;

error:
    // cleanup
    if(session_ctxt)
    {
        if(mtx_created)
        {
            pthread_mutex_destroy(&session_ctxt->mtx);
            mtx_created = false;
        }
        MEM_FREE_AND_NULL(session_ctxt);
    }
    return NULL;
}

void net_raw_io_create_client_sock(net_raw_io_client_ctxt_t *client_ctxt)
{
    int sock = -1;
    int sock_rcode = -1;
    struct sockaddr_storage local_addr;
    memset(&local_addr, 0, sizeof(struct sockaddr_storage)); 

    ASSERT(client_ctxt);

#ifdef HAVE_IPV6
    sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#else // !HAVE_IPV6
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif // HAVE_IPV6
    if(sock == -1)
    {
        DBG_ERR("%s socket request failed! (SocketError=%d)\n", client_ctxt->name, thread_errno);
        goto done;
    }

    /*
     *  bind the socket to a well known TCP port
     */
#ifdef __linux__
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)&local_addr;
    
    ip6addr->sin6_family = AF_INET6;
    ip6addr->sin6_port = htons(client_ctxt->port);
    ip6addr->sin6_addr = in6addr_any;
    //ip6addr->sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    struct sockaddr_in *ip4addr = (struct sockaddr_in *)&local_addr;
    
    ip4addr->sin_family = AF_INET;
    ip4addr->sin_port = htons(client_ctxt->port);
    ip4addr->sin_addr.s_addr = INADDR_ANY;
    //ip4addr->sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#else
#ifdef HAVE_IPV6
    unsigned int iface_id = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    if(!iface_id)
    {
        DBG_ERR("if_nametoindex failed! Unknown interface name '%s'!\n", NET_IFACE_INTERFACE_NAME);
        sock_rcode = -1;
        goto done;
    }
    local_addr.addr.ipv6.sin6_family = PF_INET6;
    local_addr.addr.ipv6.sin6_port = htons(client_ctxt->port);
    local_addr.addr.ipv6.sin6_scope_id = iface_id;
    local_addr.addr.ipv6.sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    local_addr.addr.ipv4.sin_family = PF_INET;
    local_addr.addr.ipv4.sin_port = htons(client_ctxt->port);
    local_addr.addr.ipv4.sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#endif // __linux__

    sock_rcode = bind(sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_storage));
    if(sock_rcode == -1)
    {
        DBG_ERR("%s bind request failed! (SocketError=%d)\n", client_ctxt->name, thread_errno);
        goto done;
    }

    /*
     *  configure socket options
     */

    if(client_ctxt->sock_config_func_ptr)
    {
        // use client-specific socket config options
        // assumes client will call "listen" with desired backlog size
        bool success = client_ctxt->sock_config_func_ptr(sock);
        if(!success)
        {
            sock_rcode = -1;
            goto done;
        }
    }
    else
    {
        // listen with default sock options and backlog
        // one active connection plus NET_RAW_IO_DEFAULT_BACKLOG for a total of NET_RAW_IO_DEFAULT_BACKLOG+1 simultaneous connections
        sock_rcode = listen(sock, NET_RAW_IO_DEFAULT_BACKLOG);
        if(sock_rcode == -1)
        {
            DBG_ERR("default listen failed for %s! (SocketError=%d)\n", client_ctxt->name, thread_errno);
            goto done;
        }
    }

done:
    if(sock_rcode == -1)
    {
        // clean up
        if(sock != -1)
        {
            close(sock);
            sock = -1;
        }
    }

    client_ctxt->hdr.sock = sock;
    return;
}

void net_raw_io_close(net_raw_io_session_ctxt_t *session_ctxt)
{
    NET_RAW_IO_LOCK_SESSION_CONTEXT(session_ctxt);
    ASSERT(session_ctxt);
    ASSERT(session_ctxt->client_ctxt);
    
    // disable socket notifications
    session_ctxt->hdr.sock_fd_flags &= (~SOCK_FD_READ);
    
    // MUST be last message for this context
    if (net_raw_io_send_msg(session_ctxt->client_ctxt, NET_RAW_IO_MSG_CLOSE, session_ctxt) != NET_RAW_IO_OK)
    {
        DBG_ERR("error sending NET_RAW_IO_MSG_CLOSE message for %s!\n", session_ctxt->client_ctxt->name);
    }
    
    NET_RAW_IO_STATE_TRANSITION(session_ctxt, NET_RAW_IO_STATE_CLOSING);
    NET_RAW_IO_UNLOCK_SESSION_CONTEXT(session_ctxt);
    return;
}

void net_raw_io_accept(net_raw_io_session_ctxt_t *session_ctxt)
{
    ASSERT(session_ctxt);

    net_raw_io_client_ctxt_t *client_ctxt = session_ctxt->client_ctxt;
    ASSERT(client_ctxt);

    #ifdef HAVE_IPV6
    struct sockaddr_storage src_addr;
    #else
    struct sockaddr_in src_addr;
    #endif

    // a net_io channel has been acquired -- accept the pending connection
    // rdj TODO don't block here
#ifdef __linux__
    socklen_t addr_len = sizeof(src_addr);
#else
    int addr_len = sizeof(src_addr);
#endif
    session_ctxt->hdr.sock = accept(client_ctxt->hdr.sock, (struct sockaddr *)&src_addr, &addr_len);
    if(session_ctxt->hdr.sock == -1)
    {
        // close net_io session
        DBG_ERR("accept failed for %s! (SocketError=%d)\n", client_ctxt->name, thread_errno);
        goto error;
    }
    
    NET_RAW_IO_STATE_TRANSITION(session_ctxt, NET_RAW_IO_STATE_WAIT_RECV_DATA);

    return;

error:
    net_raw_io_close(session_ctxt);
    return;
}

void net_raw_io_send_data(net_raw_io_session_ctxt_t *session_ctxt)
{
    ASSERT(session_ctxt);
    ASSERT(session_ctxt->client_ctxt);

    int sock_rcode;
    uint32_t len;
    char *buf;

    ASSERT(session_ctxt->hdr.sock != -1);

    buf = net_io_get_send_data(session_ctxt->session_hndl, &len);
#ifdef HAVE_NET_LPD
    if (session_ctxt->client_ctxt->port == 515)
    {
        //DBG_ERR("LPD just return buf = %s",buf);
        return;
    }
#endif
    while(buf)
    {
        sock_rcode = send(session_ctxt->hdr.sock, buf, len, MSG_NOSIGNAL);
        if(sock_rcode == -1)
        {
            DBG_ERR("%s %08x socket send error (sock_error=%d)\n", session_ctxt->client_ctxt->name,
                    session_ctxt, thread_errno);
        }
        else
        {
            DBG_VERBOSE("%s %08x sent %d of %d bytes\n", session_ctxt->client_ctxt->name, session_ctxt, sock_rcode, len);
        }
        // rdj TODO handle sock error
        // rdj TODO check that entire buf was sent
        // rdj TODO handle potential blocking send issue
        // rdj TODO limit max time spent in this loop - 
            // e.g. send another NET_RAW_IO_MSG_HAVE_SEND_DATA msg after n iterations or n bytes sent
            // possibly only if other sessions exist?
        MEM_FREE_AND_NULL(buf);
        buf = net_io_get_send_data(session_ctxt->session_hndl, &len);
    }
    DBG_VERBOSE("%s %08x send buf queue empty\n", session_ctxt->client_ctxt->name, session_ctxt);

    return;
}

#ifdef HAVE_NET_LPD
static int recvSocket(int socket_, unsigned char *data,size_t size,int flags)
{
    int readLength;
    struct pollfd pollSocket;
    pollSocket.fd        = socket_;
    pollSocket.events    = POLLIN | POLLERR | POLLHUP;
    pollSocket.revents    = 0;
    memset(data,0,size);
    if (poll(&pollSocket, 1, 10000) > 0)
    {
        if ((pollSocket.revents & POLLIN) || (pollSocket.revents & POLLPRI))
        {
            readLength = recv(socket_, (void *)data, size, flags);
            if (readLength > 0)
            {
                return readLength;
            }
            else if (readLength == 0)
            {
                //DBG_ERR("End of Stream");
                return 0;
            }
            else if (errno == EAGAIN || errno == EINTR)
            {
               return -1;
                //usleep(100*1000);
            }
            else
            {
                DBG_ERR("ERROR_DISCONNECTED");
                return -1;
            }
        }
        else
        {
            DBG_ERR("ERROR_DISCONNECTED");
            return -1; 
        }
    }
    return 0;
}

static int32_t net_lpd_io_recv_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt)
{
    int sock_error = 0;

    ASSERT(user_ctxt);
    net_raw_io_session_ctxt_t *session_ctxt = (net_raw_io_session_ctxt_t *)user_ctxt;

    ASSERT(session_ctxt->client_ctxt);

    int sock_rcode;
    fd_set read_fds;
    struct timeval sel_timeout = {0};
    struct timeval *sel_timeout_ptr = &sel_timeout;

    if(*timeout_msec == -1)
    {
        sel_timeout_ptr = NULL; // wait forever
    }
    else if(*timeout_msec > 0)
    {
        sel_timeout.tv_sec = *timeout_msec / 1000;
        sel_timeout.tv_usec = (*timeout_msec % 1000) * 1000;
    }

    FD_ZERO(&read_fds);
    FD_SET(session_ctxt->hdr.sock, &read_fds);

    //DBG_ERR("%s %08x net_lpd_io_recv_func recv requesting %d bytes (timeout=%d sec %d msec)\n", session_ctxt->client_ctxt->name, 
    //            session_ctxt, len, *timeout_msec / 1000, *timeout_msec % 1000);
    sock_rcode = select(session_ctxt->hdr.sock + 1, &read_fds, NULL, NULL, sel_timeout_ptr);
    sock_error = thread_errno;
    //DBG_ERR("session_ctxt->hdr.sock = %d \n",session_ctxt->hdr.sock);
    //DBG_ERR("select sock_rcode = %d errno =%d\n",sock_rcode,sock_error);
    if(sock_rcode == -1)
    {
        DBG_ERR("%s %08x socket select error (sock_error=%d)\n", session_ctxt->client_ctxt->name, 
                session_ctxt, sock_error);
        return -1;
    }
    if(sock_rcode == 0)
    {
        // timeout occured on select
        DBG_ERR("%s %08x select timeout", session_ctxt->client_ctxt->name, session_ctxt);
        if(session_ctxt->lpd_info->dataStream == false)
        {
                SEND_OK(session_ctxt->hdr.sock);
        }
        *timeout_msec = 0;
        return -1;
    }
    memset(buf,0,len);
    sock_rcode = recv(session_ctxt->hdr.sock, buf, len, MSG_DONTWAIT);
    sock_error = thread_errno;
    //DBG_ERR("sock_rcode = %d errno =%d\n",sock_rcode,sock_error);
   
    if(sock_rcode == 1)
    {
       if(SEND_OK(session_ctxt->hdr.sock) > 0)
	   {	
        if(buf[0] == '\0')
        {
           session_ctxt->lpd_info->dataStream = false;
           return 0;
        }
        else
        {
           return 1;
        }
	   }
	   else
	   	return -1;
    }

    else if(sock_rcode == -1)
    {
        sock_error = thread_errno;
        DBG_ERR("%s %08x socket recv error (sock_error=%d)\n", session_ctxt->client_ctxt->name, session_ctxt, sock_error);
        session_ctxt->net_closed = true;
        return 0;
    }
    else if(sock_rcode == 0)
    {
        // connection has been closed by peer -- report EOF
        DBG_ERR("%s %08x recv EOF\n", session_ctxt->client_ctxt->name, session_ctxt);
        // indicate network connection was closed
        if(!session_ctxt->net_closed)
        {
            session_ctxt->net_closed = true;
        }
    }
    else
    {
        DBG_VERBOSE("%s %08x recv got %d bytes\n", session_ctxt->client_ctxt->name, session_ctxt, sock_rcode);
        if(session_ctxt->lpd_info->dataStream == true)
        {
            if(session_ctxt->lpd_info->isDataSizeProvided)
            {
                session_ctxt->lpd_info->dataSizeLPD-=sock_rcode;
                if(session_ctxt->lpd_info->dataSizeLPD <= 0)
                {
                    session_ctxt->lpd_info->dataStream = false;
                    if(buf[sock_rcode-1] == '\0')
                        sock_rcode--;
                    DBG_ERR("received all bytes");
                    if(SEND_OK(session_ctxt->hdr.sock) > 0)
                    {
                        DBG_ERR("zero octet sent %s %08sx", session_ctxt->client_ctxt->name, session_ctxt);
                    }
                    else
                    {
                        DBG_ERR("failed to Acknowledge");
                    }
                }
            }
        }
        else
        {
            DBG_ERR("data stream already processed ..consume data..- %s %08x ", session_ctxt->client_ctxt->name, session_ctxt);
            memset(buf,0,sock_rcode-1);
            sock_rcode = -1;
            if(SEND_OK(session_ctxt->hdr.sock) > 0)
            {
                DBG_ERR("zero octet sent %s %08sx", session_ctxt->client_ctxt->name, session_ctxt);
            }
            else
            {
                DBG_ERR("failed to Acknowledge");
            }
        }
    }
    return sock_rcode;
}


#endif
void net_raw_io_process_message(net_raw_io_msg_t *msg)
{
    net_raw_io_client_ctxt_t *client_ctxt;
    net_raw_io_session_ctxt_t *session_ctxt;

    ASSERT(msg);
    ASSERT(msg->type < NET_RAW_IO_NUM_MSGS);

    client_ctxt = msg->client;
    ASSERT(client_ctxt);

#ifdef HAVE_DBG_PRINTF
    DBG_VERBOSE("net_raw_io_process_message: recieved msg type: %s \n", g_net_raw_io_msg_strs[msg->type]);
#endif

    session_ctxt = (net_raw_io_session_ctxt_t *)msg->data;
    if(session_ctxt)
    {
        DBG_VERBOSE("%s session %08x w/ chan %#x, pipe %#x got msg %s in state %s\n", client_ctxt->name, session_ctxt, 
                    net_io_debug_get_channel_from_session(session_ctxt->session_hndl), net_io_debug_get_pipe_from_session(session_ctxt->session_hndl),
                    g_net_raw_io_msg_strs[msg->type], g_net_raw_io_state_strs[session_ctxt->state]);
    }
    else
    {
        DBG_VERBOSE("%s got stateless msg %s\n", client_ctxt->name, g_net_raw_io_msg_strs[msg->type]);
    }
    
    /*
     *  process stateless messages
     */

    switch(msg->type)
    {
    case NET_RAW_IO_MSG_NEW_CLIENT:
        {
            net_raw_io_create_client_sock(client_ctxt);
            if(client_ctxt->hdr.sock == -1)
            {
                DBG_ERR("failed to create sock for client %s\n", client_ctxt->name);
            }
            else
            {
                // add client to linked list
                DBG_MSG("Adding raw io client %s\n", client_ctxt->name);
                NET_RAW_IO_CLIENT_LIST_INSERT(client_ctxt);
            }

        }
        break;
        
    case NET_RAW_IO_MSG_ACCEPT_PENDING:
        {
            // net_raw_io_session_ctxt_t *new_session_ctxt;
            // new_session_ctxt = 
            net_raw_io_open(client_ctxt);
            
            // wait for NET_IO_SESSION_EVENT_INACTIVE event
        }
        ASSERT(!session_ctxt);
        break; // we will bypass stateful message processing

    case NET_RAW_IO_MSG_CONNECTION_CLOSED:

        /*
         * Peer has closed the incoming side of TCP connection.
         * 
         */

        ASSERT(session_ctxt);
        net_raw_io_close(session_ctxt);
        break; // proceed to stateful message processing

    default:
        // ignore other messages
        break;
    }

    /*
     *  process stateful messages
     */

    if(!session_ctxt)
    {
        // message is not stateful - ignore
        return;
    }

    ASSERT(session_ctxt->state < NET_RAW_IO_NUM_STATES);

    switch(session_ctxt->state)
    {
    /***************************************************
     * NET_RAW_IO_STATE_OPENING
     ***************************************************/
    case NET_RAW_IO_STATE_OPENING:

        switch(msg->type)
        {
        case NET_RAW_IO_MSG_SESSION_INACTIVE:

            // an I/O channel has been acquired
            // NOTE: we wait for channel acquisition because accepting the connection can increase the recv buf size
            DBG_VERBOSE("calling net_raw_io_accept\n");
            net_raw_io_accept(session_ctxt);
            
            // enable further socket notifications
            NET_RAW_IO_LOCK_CLIENT_CONTEXT(client_ctxt);
            client_ctxt->hdr.sock_fd_flags |= SOCK_FD_READ;
            NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client_ctxt);

            // wait for NET_RAW_IO_MSG_RECV_DATA_PENDING
            break;

        default:
            // ignore other messages
            DBG_VERBOSE("%s %08x ignoring msg %s!\n", client_ctxt->name, session_ctxt, g_net_raw_io_msg_strs[msg->type]);
            break;
        }
        break;

    /***************************************************
     * NET_RAW_IO_STATE_WAIT_RECV_DATA
     ***************************************************/
    case NET_RAW_IO_STATE_WAIT_RECV_DATA:

        switch(msg->type)
        {
        case NET_RAW_IO_MSG_RECV_DATA_PENDING:

            // signal sys I/O connection manager that we are ready
            // NOTE: we wait till data pending so that we don't unnecessarily lock system resources before actually ready
#ifdef HAVE_NET_LPD
            if(session_ctxt->client_ctxt->port == 515 && session_ctxt->lpd_info->dataStream == false)
            {
                //int res=-1;
                unsigned char *bufLPRData = (unsigned char *)malloc(LPD_BUF_SIZE);
                memset(bufLPRData,0,LPD_BUF_SIZE);    
                int iter = 0;
                bool dataFilepresent = false;
                bool isError = false;
                long readLength = recvSocket(session_ctxt->hdr.sock,bufLPRData, LPD_BUF_SIZE-1, MSG_DONTWAIT);
                if(readLength <= 0)
                {
                    DBG_ERR("readLength < 0");
                    isError = true;
                }
                //DBG_ERR("\recvSocket[%s] ",bufLPRData);
                while(isError == false)
                {
                    iter++;
                    if(iter > 500)
                    {
                        //For safety to avoid infinite loop...just a work around
                        //possibly this code fragment will never hit
                        DBG_ERR("Multiple Iteration no result..Quiting... %d",iter);
                        //isError = true;
                        break;
                    }
                    char cmd;
                    cmd = bufLPRData[0];
                    DBG_ERR("LPD command [%x]",cmd);
                    switch (cmd)
                    {
                    case 0x00:
                        isError = true;
                        SEND_OK(session_ctxt->hdr.sock);
                        break;
                    case 0x01:
                        
                        //DBG_ERR("\n[[[0x01] [%s]",bufLPRData);
                        if(SEND_OK(session_ctxt->hdr.sock) < 0)
                        {
                            DBG_ERR("Write failed");
                        }
                        memset(bufLPRData,0,LPD_BUF_SIZE);
                        isError = true;
                        break;
                    case 0x02:
                        //DBG_ERR("\n\n[[[0x02][%s]",bufLPRData);
                        if(strstr((const char *)bufLPRData," cfA"))
                        {
                           //DBG_ERR("\n[%s]\n",bufLPRData);
	                        {
	                            DBG_ERR("Consume Control file");
	                            memset(bufLPRData,0,LPD_BUF_SIZE);
	                            if(SEND_OK(session_ctxt->hdr.sock) > 0)
								{
									
		                            iter++;
		                            readLength = recvSocket(session_ctxt->hdr.sock,bufLPRData, LPD_BUF_SIZE-1, MSG_DONTWAIT);
		                            if(readLength <= 0)
		                            {
		                                
		                              memset(bufLPRData,0,LPD_BUF_SIZE);
		                              isError = true;
		                            }
		                            //DBG_ERR("[%s]\n",bufLPRData);
		                            else
									{
			                            if(SEND_OK(session_ctxt->hdr.sock) > 0)
			                            {
			                                memset(bufLPRData,0,LPD_BUF_SIZE);
			                               readLength = recvSocket(session_ctxt->hdr.sock,bufLPRData, LPD_BUF_SIZE-1, MSG_DONTWAIT);
			                               if(readLength <= 0)
			                               {
			                                   
			                                 //DBG_ERR("[%s]\n",bufLPRData);
			                                 memset(bufLPRData,0,LPD_BUF_SIZE);
			                                 isError = true;
			                               }
			                            }
										else
			                            {
			                                isError = true;
			                            }
		                            }
		                            
								}
	                            else
	                            {
	                                isError = true;
	                            }
	                            //continue;
	                        }
						}
						else
						{
                            if(SEND_OK(session_ctxt->hdr.sock) > 0)
                            {
                               memset(bufLPRData,0,LPD_BUF_SIZE); 
                               readLength = recvSocket(session_ctxt->hdr.sock,bufLPRData, LPD_BUF_SIZE-1, MSG_DONTWAIT);
                               if(readLength <= 0)
                               {
                                   isError = true;
                                   break;
                               }
							}
    						else
                            {
                               isError = true;
                            }                        
                        }
                        break;
                    case 0x03:                    
                        if (strstr((const char *)bufLPRData," dfA"))
                        {
                            DBG_ERR("Found Data file");
                            if(SEND_OK(session_ctxt->hdr.sock) > 0)
                            {
                               dataFilepresent = true;
                               session_ctxt->lpd_info->dataStream = true;
                               unsigned char *end = bufLPRData+1;
                               while(*end && *end != ' ')
                               {
                                   end++;
                               }
                               if(end > bufLPRData+1 && *end == ' ')
                               {
                                   end--;
                                   char dataStreamSize[20];
                                   memset(dataStreamSize,0,sizeof(dataStreamSize));
                                   memcpy(dataStreamSize,bufLPRData+1,(end-bufLPRData > 20?20:end-bufLPRData));
                                   session_ctxt->lpd_info->dataSizeLPD = atoi(dataStreamSize);
                                   session_ctxt->lpd_info->isDataSizeProvided = (session_ctxt->lpd_info->dataSizeLPD?true:false);
                               }
                            }
							else
                            {
                               isError = true;
                            }
                        }
                        else
                        {
                        	DBG_ERR("Data file missing");
	                        if(SEND_OK(session_ctxt->hdr.sock) <= 0)
							{
	                            isError = true;
							}
                        }
                        break;    
                    case 0x04:
                    case 0x05:
                        isError = true;
                        SEND_OK(session_ctxt->hdr.sock);
                        break;
                    default:
                        isError = true;
                        break;
                    }
                    if(dataFilepresent || isError)
                    {
                        break;
                    }
               }
               
               if(bufLPRData)
               {           
                   free(bufLPRData);
                   //bufLPRData = NULL;
               }
            }
#endif
            NET_RAW_IO_STATE_TRANSITION(session_ctxt, NET_RAW_IO_STATE_READY);
            net_io_ready(session_ctxt->session_hndl);

            // wait for NET_IO_SESSION_EVENT_INACTIVE event
            break;

        case NET_RAW_IO_MSG_CONNECTION_CLOSED:
            {
                // I/O channel is inactive and network connection has closed - end the session
                net_raw_io_close(session_ctxt);
            }
            break;

        case NET_RAW_IO_MSG_HAVE_SEND_DATA:
            // must continue to send data while in the "inactive" state
            net_raw_io_send_data(session_ctxt);
            break;

        default:
            // ignore other messages
            DBG_VERBOSE("%s %08x ignoring msg %s!\n", client_ctxt->name, session_ctxt, g_net_raw_io_msg_strs[msg->type]);
            break;
        }
        break;

    /***************************************************
     * NET_RAW_IO_STATE_READY
     ***************************************************/
    case NET_RAW_IO_STATE_READY:

        switch(msg->type)
        {
        case NET_RAW_IO_MSG_SESSION_ACTIVE:
            NET_RAW_IO_STATE_TRANSITION(session_ctxt, NET_RAW_IO_STATE_ACTIVE);
            break;

        case NET_RAW_IO_MSG_HAVE_SEND_DATA:
            net_raw_io_send_data(session_ctxt);
            break;

        default:
            // ignore other messages
            DBG_VERBOSE("%s %08x ignoring msg %s!\n", client_ctxt->name, session_ctxt, g_net_raw_io_msg_strs[msg->type]);
            break;
        }
        break;

    /***************************************************
     * NET_RAW_IO_STATE_ACTIVE
     ***************************************************/
    case NET_RAW_IO_STATE_ACTIVE:

        switch(msg->type)
        {
        case NET_RAW_IO_MSG_HAVE_SEND_DATA:
            net_raw_io_send_data(session_ctxt);
            break;

        case NET_RAW_IO_MSG_SESSION_INACTIVE:

            // I/O channel has gone inactive 
            if(session_ctxt->net_closed)
            {
                // network connection has closed and no recv bytes pending - end the session
                net_raw_io_close(session_ctxt);
            }
            else
            {
                NET_RAW_IO_LOCK_SESSION_CONTEXT(session_ctxt);
                // network connection still open - go back to waiting for new incoming data
                NET_RAW_IO_STATE_TRANSITION(session_ctxt, NET_RAW_IO_STATE_WAIT_RECV_DATA);

                // enable socket notifications
                session_ctxt->hdr.sock_fd_flags |= SOCK_FD_READ;
                NET_RAW_IO_UNLOCK_SESSION_CONTEXT(session_ctxt);
            }
            break;

        default:
            // ignore other messages
            DBG_VERBOSE("%s %08x ignoring msg %s!\n", client_ctxt->name, session_ctxt, g_net_raw_io_msg_strs[msg->type]);
            break;
        }
        break;

    /***************************************************
     * NET_RAW_IO_STATE_CLOSING
     ***************************************************/
    case NET_RAW_IO_STATE_CLOSING:

        switch(msg->type)
        {
        case NET_RAW_IO_MSG_CLOSE:

            net_raw_io_session_list_remove(session_ctxt->client_ctxt, session_ctxt);
            
            net_io_close(session_ctxt->session_hndl);
            if(session_ctxt->hdr.sock != -1)
            {
                DBG_VERBOSE("%s %08x TCP connection closed\n", session_ctxt->client_ctxt->name, session_ctxt);

#ifdef __linux__
                close(session_ctxt->hdr.sock);
#else                
                tfClose(session_ctxt->hdr.sock);
#endif                
                session_ctxt->hdr.sock = -1;
            }
            pthread_mutex_destroy(&session_ctxt->mtx);
#ifdef HAVE_NET_LPD
            if(session_ctxt->client_ctxt->port == 515)
            {
                MEM_FREE_AND_NULL(session_ctxt->lpd_info);
            }
#endif
            MEM_FREE_AND_NULL(session_ctxt);
            // must not recieve any futher messages referencing this context

            break;

        default:
            // ignore other messages
            DBG_VERBOSE("%s %08x ignoring msg %s!\n", client_ctxt->name, session_ctxt, g_net_raw_io_msg_strs[msg->type]);
            break;
        }
        break;

    default:
        // unknown state - ignore
        XASSERT(0, session_ctxt->state);
        break;
    }
    
    DBG_VERBOSE("net_raw_io_process_message: exit\n");
    return;
}

void net_raw_io_sock_msg(net_raw_io_list_header_t *item)
{
    int sock_rcode;
   	net_raw_io_msg_t msg;

    sock_rcode = recv(item->sock, (void *)&msg, sizeof(net_raw_io_msg_t), 0);
    if (sock_rcode > 0 && sock_rcode == sizeof(net_raw_io_msg_t))
    {
        net_raw_io_process_message(&msg);
    }
    else if (sock_rcode == -1)
    {
        DBG_ERR("socket %d recv error (sock_error=%d)\n", item->sock, thread_errno);
    }
    else
    {
        DBG_ERR("socket %d - invalid message\n", item->sock);
    }
}

void net_raw_io_build_select_list() 
{
    int i;
    net_raw_io_list_header_t *msg;
    net_raw_io_client_ctxt_t *client;
    net_raw_io_session_ctxt_t *session;
    
    DBG_VERBOSE("building select list\n");
    
	// clear out all the fd_set socks, always start with no file descriptors
	FD_ZERO(&rawio_read_socks);
    
    // always add rawio_invite_sock
    FD_SET(rawio_invite_sock,&rawio_read_socks);
    
    // set nfds to rawio_invite_sock to start
    rawio_nfds = rawio_invite_sock;
    
	// add all registered connections to the fd_set
    i=0;
    // -jrs lock at this level to prevent manipulation of the list while it is being iterated
    // This is due to the index based interation of the lists below.  rdj to clean-up when
    // restructuring net_raw_io/net_io.
    NET_RAW_IO_GLOBAL_LOCK(); 
    while ((msg = NET_RAW_IO_MSG_LIST_GET(i++)) != NULL)
    {
        // only add valid (accepted) sockets for listening
        if (msg->sock >= 0)
        {
            DBG_VERBOSE("Listening to read message socket %d\n", msg->sock);
            FD_SET(msg->sock, &rawio_read_socks);
            
            if (msg->sock > rawio_nfds)
            {
                rawio_nfds = msg->sock;
            }
        }
    } 

    i=0;
    while ((client = NET_RAW_IO_CLIENT_LIST_GET(i++)) != NULL)
    {
        // only add valid (accepted) sockets for listening
        if (client->hdr.sock >= 0)
        {
            NET_RAW_IO_LOCK_CLIENT_CONTEXT(client);
            if (client->hdr.sock_fd_flags & SOCK_FD_READ)
            {
                DBG_VERBOSE("Listening to read client socket %d\n", client->hdr.sock);
                FD_SET(client->hdr.sock, &rawio_read_socks);
                
                if (client->hdr.sock > rawio_nfds)
                {
                    rawio_nfds = client->hdr.sock;
                }
            }
            else
            {
                DBG_VERBOSE("read client socket %d has been disabled\n", client->hdr.sock);
            }
            NET_RAW_IO_UNLOCK_CLIENT_CONTEXT(client);
            
            session = net_raw_io_session_list_get_head((net_raw_io_client_ctxt_t *)client);
            while (session != NULL)
            {
                // only add valid (accepted) sockets for listening
                if (session->hdr.sock >= 0)
                {
                    NET_RAW_IO_LOCK_SESSION_CONTEXT(session);
                    if (session->hdr.sock_fd_flags & SOCK_FD_READ)
                    {
                        DBG_VERBOSE("Listening to read session %#x, socket %d\n", session->session_hndl, session->hdr.sock);
                        FD_SET(session->hdr.sock, &rawio_read_socks);
                        
                        if (session->hdr.sock > rawio_nfds)
                        {
                            rawio_nfds = session->hdr.sock;
                        }
                    }
                    else
                    {
                        DBG_VERBOSE("read session %#x, socket %d has been disabled\n", session->session_hndl, session->hdr.sock);
                    }
                    NET_RAW_IO_UNLOCK_SESSION_CONTEXT(session);
                }
                session = net_raw_io_session_list_get_next((net_raw_io_client_ctxt_t *)client, session);
            } 
        }
    } 
    NET_RAW_IO_GLOBAL_UNLOCK();    
    DBG_VERBOSE("nfds = %d\n", rawio_nfds);
}

void net_raw_io_process_read_socks()
{
    int i;
    net_raw_io_list_header_t *msg;
    net_raw_io_client_ctxt_t *client;
    net_raw_io_session_ctxt_t *session;
    
    DBG_VERBOSE("process read socks\n");

    // process all messages first
    i = 0;
    // -jrs lock at this level to prevent manipulation of the list while it is being iterated
    // This is due to the index based interation of the lists below.  rdj to clean-up when
    // restructuring net_raw_io/net_io.
    NET_RAW_IO_GLOBAL_LOCK();
    while ((msg = NET_RAW_IO_MSG_LIST_GET(i++)) != NULL)
    {
        // check for valid sock
        if (msg->sock >= 0)
        {
            DBG_VERBOSE("Checking for message on socket = %d\n", msg->sock);
            if (FD_ISSET(msg->sock, &rawio_read_socks))
            {
                DBG_VERBOSE("Handling message on socket = %d\n", msg->sock);
                net_raw_io_sock_msg(msg);
            }
        }
    } 

    // check for new connections to client sockets
    i = 0;
    while ((client = NET_RAW_IO_CLIENT_LIST_GET(i++)) != NULL)
    {
        // check for valid sock
        if (client->hdr.sock >= 0)
        {
            DBG_VERBOSE("Checking for client message on socket = %d\n", client->hdr.sock);
            if (FD_ISSET(client->hdr.sock, &rawio_read_socks))
            {
                DBG_VERBOSE("Handling client message on socket = %d\n", client->hdr.sock);
                
                /* send a message indicating accept pending on client */
                if (net_raw_io_send_msg(client, NET_RAW_IO_MSG_ACCEPT_PENDING, NULL) != NET_RAW_IO_OK)
                {
                    DBG_ERR("error sending accept pending message for %s!\n", client->name);
                }
                else
                {
                    // disable future socket notifications on this client until
                    // I/O channel has been acquired.
                    client->hdr.sock_fd_flags &= (~SOCK_FD_READ);
                }
            }
        }
        
        // check for data pending on any sessions for this client
        session = net_raw_io_session_list_get_head(client);
        while (session != NULL)
        {
            // check for valid sock
            if (session->hdr.sock >= 0)
            {
                DBG_VERBOSE("Checking for session message on socket = %d\n", session->hdr.sock);
                if (FD_ISSET(session->hdr.sock, &rawio_read_socks))
                {
                    DBG_VERBOSE("Handling session message on socket = %d\n", session->hdr.sock);
                    
                    /* send a message indicating data pending on session */
                    if (net_raw_io_send_msg(client, NET_RAW_IO_MSG_RECV_DATA_PENDING, session) != NET_RAW_IO_OK)
                    {
                        DBG_ERR("error sending session data pending message for %s!\n", client->name);
                    }
                    else
                    {
                        // disable socket notifications, will now be handled in the
                        // net_raw_io_recv_func function
                        session->hdr.sock_fd_flags &= (~SOCK_FD_READ);
                    }
                }
            }
            session = net_raw_io_session_list_get_next(client, session);
        } 
    }   
    NET_RAW_IO_GLOBAL_UNLOCK();      
}

/** 
 * \brief Net Raw I/O main program thread
 * 
 **/
static void *net_raw_io_main( void *unused )
{
    int socks; // number of sockets ready
    // struct timeval timeout;  // timeout for select()

    // main loop
    while (1)
    {
        // build socket list for select to use
        net_raw_io_build_select_list();

        // set 5 second timeout
        // timeout.tv_sec = 5;
        // timeout.tv_usec = 0;

        DBG_VERBOSE("issuing select()\n");
        socks = select(rawio_nfds + 1, &rawio_read_socks, 0, 0, NULL);
        if (socks > 0)
        {
            DBG_VERBOSE("%d socks ready!\n", socks);

            // check to see if there is a pending message connection
            if (FD_ISSET(rawio_invite_sock, &rawio_read_socks))
            {
                net_raw_io_handle_new_connection();
            }

            net_raw_io_process_read_socks();
        }
        else if (socks == 0)
        {
            DBG_VERBOSE("no sockets ready, assume timeout\n");
        }
        else
        {
            DBG_ERR("socket select error (sock_error=%d)\n", thread_errno);
        }

    } // end while (1)

    return 0;
} // end net_raw_io_main()


//=========================================================================================================================
// Callback Routines
//=========================================================================================================================

static int32_t net_raw_io_recv_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt)
{
    int sock_error;

    ASSERT(user_ctxt);
    net_raw_io_session_ctxt_t *session_ctxt = (net_raw_io_session_ctxt_t *)user_ctxt;

    ASSERT(session_ctxt->client_ctxt);

    int sock_rcode;
    fd_set read_fds;
    struct timeval sel_timeout = {0};
    struct timeval *sel_timeout_ptr = &sel_timeout;

    if(*timeout_msec == -1)
    {
        sel_timeout_ptr = NULL; // wait forever
    }
    else if(*timeout_msec > 0)
    {
        sel_timeout.tv_sec = *timeout_msec / 1000;
        sel_timeout.tv_usec = (*timeout_msec % 1000) * 1000;
    }

    FD_ZERO(&read_fds);
    FD_SET(session_ctxt->hdr.sock, &read_fds);

    DBG_VERBOSE("%s %08x recv requesting %d bytes (timeout=%d sec %d msec)\n", session_ctxt->client_ctxt->name, 
                session_ctxt, len, *timeout_msec / 1000, *timeout_msec % 1000);
    sock_rcode = select(session_ctxt->hdr.sock + 1, &read_fds, NULL, NULL, sel_timeout_ptr);
    if(sock_rcode == -1)
    {
        sock_error = thread_errno;
        DBG_ERR("%s %08x socket select error (sock_error=%d)\n", session_ctxt->client_ctxt->name, 
                session_ctxt, sock_error);
        return -1;
    }
    if(sock_rcode == 0)
    {
        // timeout occured on select

        DBG_VERBOSE("%s %08x recv timeout\n", session_ctxt->client_ctxt->name, session_ctxt);
        *timeout_msec = 0;
        return -1;
    }

    sock_rcode = recv(session_ctxt->hdr.sock, buf, len, 0);
    if(sock_rcode == -1)
    {
        sock_error = thread_errno;
        DBG_ERR("%s %08x socket recv error (sock_error=%d)\n", session_ctxt->client_ctxt->name, session_ctxt, sock_error);
        session_ctxt->net_closed = true;  // stefan lets force close this on error between select and recv.
        return -1;
    }
    else if(sock_rcode == 0)
    {
        // connection has been closed by peer -- report EOF
        DBG_VERBOSE("%s %08x recv EOF\n", session_ctxt->client_ctxt->name, session_ctxt);

        // indicate network connection was closed
        if(!session_ctxt->net_closed)
        {
            session_ctxt->net_closed = true;
        }
    }
    else
    {
        DBG_VERBOSE("%s %08x recv got %d bytes\n", session_ctxt->client_ctxt->name, session_ctxt, sock_rcode);
    }

    return sock_rcode;
}

static void net_raw_io_notify_func(net_io_session_event_t event, void *user_ctxt)
{
    ASSERT(user_ctxt);
    net_raw_io_session_ctxt_t *session_ctxt = (net_raw_io_session_ctxt_t *)user_ctxt;

    ASSERT(session_ctxt->client_ctxt);

    switch(event)
    {
    case NET_IO_SESSION_EVENT_INACTIVE:
        if (net_raw_io_send_msg(session_ctxt->client_ctxt, NET_RAW_IO_MSG_SESSION_INACTIVE, session_ctxt) != NET_RAW_IO_OK)
        {
            DBG_ERR("error sending NET_IO_SESSION_EVENT_INACTIVE message for %s!\n", session_ctxt->client_ctxt->name);
        }
        break;

    case NET_IO_SESSION_EVENT_ACTIVE:
        if (net_raw_io_send_msg(session_ctxt->client_ctxt, NET_RAW_IO_MSG_SESSION_ACTIVE, session_ctxt) != NET_RAW_IO_OK)
        {
            DBG_ERR("error sending NET_RAW_IO_MSG_SESSION_ACTIVE message for %s!\n", session_ctxt->client_ctxt->name);
        }
        break;

    case NET_IO_SESSION_EVENT_HAVE_SEND_DATA:
        if (net_raw_io_send_msg(session_ctxt->client_ctxt, NET_RAW_IO_MSG_HAVE_SEND_DATA, session_ctxt) != NET_RAW_IO_OK)
        {
            DBG_ERR("error sending NET_RAW_IO_MSG_HAVE_SEND_DATA message for %s!\n", session_ctxt->client_ctxt->name);
        }
        break;

    default:
        // unhandled event
        XASSERT(0, event);
        break;
    }
    return;
}


//=========================================================================================================================
// Module API
//=========================================================================================================================
#ifdef USE_UNIX_DOMAIN_SOCKET
int32_t net_raw_io_init(void)
{
    int32_t rcode = NET_RAW_IO_OK;
    int px_status, sock_rcode;
    struct sockaddr_un  local_addr;

    rawio_invite_sock = -1;
    rawio_nfds = 0;

    rawio_invite_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    XASSERT( rawio_invite_sock != -1, errno );
    
    // prevent socket inheritance across fork/exec calls
    fcntl(rawio_invite_sock, F_SETFD, (FD_CLOEXEC | fcntl(rawio_invite_sock, F_GETFD)));
    
    memset(&local_addr, 0, sizeof(struct sockaddr_un)); 
    unlink(INVITE_SOCKET_PATH);
    local_addr.sun_family = AF_UNIX;
    strncpy(local_addr.sun_path, INVITE_SOCKET_PATH, sizeof(local_addr.sun_path) - 1);
    sock_rcode = bind(rawio_invite_sock, (struct sockaddr *)&local_addr,  sizeof (struct sockaddr_un));
    if (sock_rcode == -1)
    {
      // perror("socket bind");
        DBG_ERR("message socket bind error %d (sock_error=%d)\n", rawio_invite_sock, thread_errno);
        XASSERT( 0, thread_errno );
    }
    sock_rcode = listen(rawio_invite_sock, NET_RAW_IO_DEFAULT_BACKLOG);
    if (sock_rcode == -1)
    {
        DBG_ERR("message socket listen error %d (sock_error=%d)\n", rawio_invite_sock, thread_errno);
        XASSERT( 0, thread_errno );
    }
    
    px_status = posix_mutex_init( &rawio_global_mtx );
    XASSERT( px_status==0, px_status );
    
    DBG_VERBOSE("starting net_raw_io thread, stack size = %d\n", sizeof(rawio_thread_stack));    
    px_status = posix_create_thread(&rawio_thread_id, net_raw_io_main, 0, "netrawio", &rawio_thread_stack, sizeof(rawio_thread_stack), POSIX_THR_PRI_NORMAL);
    XASSERT( px_status==0, px_status );

    return rcode;
}

int32_t net_raw_io_register(Channel_type_t type, uint16_t port, const char *name, 
                            net_raw_io_sock_config_func_ptr_t sock_config_func_ptr)
{
    net_raw_io_client_ctxt_t *client_ctxt = NULL;
    int px_status, sock_rcode;
    struct sockaddr_un msg_addr;
    bool mtx_created = false;
    
    client_ctxt = (net_raw_io_client_ctxt_t *)MEM_MALLOC(sizeof(net_raw_io_client_ctxt_t));
    if(!client_ctxt)
    {
        goto error;
    }

    memset(client_ctxt, 0, sizeof(net_raw_io_client_ctxt_t));
    
    client_ctxt->hdr.sock = -1;
    client_ctxt->hdr.sock_fd_flags = SOCK_FD_READ;
    client_ctxt->message_sock = -1;
    client_ctxt->type = type;
    client_ctxt->port = port;
    client_ctxt->name = name;
    client_ctxt->sock_config_func_ptr = sock_config_func_ptr; // may be NULL
    
    px_status = posix_mutex_init( &client_ctxt->mtx );
    if(px_status != 0)
    {
        DBG_ERR("error creating client_ctxt mutex for %s (px_status=0x%02x)\n", client_ctxt->name, px_status);
        goto error;
    }
    mtx_created = true;

    client_ctxt->message_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if(client_ctxt->message_sock == -1)
    {
        DBG_ERR("%s: socket connect error (sock_error=%d)\n", client_ctxt->name, thread_errno);
        goto error;
    }
    
    memset(&msg_addr, 0, sizeof(struct sockaddr_un)); 

    msg_addr.sun_family = AF_UNIX;
    strncpy(msg_addr.sun_path, INVITE_SOCKET_PATH, sizeof(msg_addr.sun_path) - 1);

    /* connect to message socket */
    sock_rcode = connect(client_ctxt->message_sock, (struct sockaddr *)&msg_addr, sizeof(struct sockaddr_un));
    if(sock_rcode == -1)
    {
        DBG_ERR("%s: socket connect error (sock_error=%d)\n", client_ctxt->name, thread_errno);
        goto error;
    }
	/* send a message for new client */
    if (net_raw_io_send_msg(client_ctxt, NET_RAW_IO_MSG_NEW_CLIENT, NULL) != NET_RAW_IO_OK)
    {
        DBG_ERR("error sending message for %s!\n", client_ctxt->name);
        goto error;
    }
    
    return NET_RAW_IO_OK;

error:
    // cleanup
    if(client_ctxt)
    {
        if(mtx_created)
        {
            pthread_mutex_destroy(&client_ctxt->mtx);
            mtx_created = false;
        }
        if (client_ctxt->message_sock)
        {
            close(client_ctxt->message_sock);
        }
        MEM_FREE_AND_NULL(client_ctxt);
    }
    return NET_RAW_IO_FATAL_ERROR;
}

#else
int32_t net_raw_io_init(void)
{
    int32_t rcode = NET_RAW_IO_OK;
    int px_status, sock_rcode;
    struct sockaddr_storage local_addr;

    rawio_invite_sock = -1;
    rawio_invite_port = NET_RAW_IO_INVITE_PORT;
    rawio_nfds = 0;

#ifdef HAVE_IPV6
    rawio_invite_sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#else // !HAVE_IPV6
    rawio_invite_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif // HAVE_IPV6
    XASSERT( rawio_invite_sock != -1, errno );
    
    /* So that we can re-bind to it without TIME_WAIT problems */
    int reuse_addr = 1;
    setsockopt(rawio_invite_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr, sizeof(reuse_addr));

    // prevent socket inheritance across fork/exec calls
    fcntl(rawio_invite_sock, F_SETFD, (FD_CLOEXEC | fcntl(rawio_invite_sock, F_GETFD)));
    
    /*
     *  bind the socket to a well known TCP port
     */

    memset(&local_addr, 0, sizeof(struct sockaddr_storage)); 
    
#ifdef __linux__
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)&local_addr;
    
    ip6addr->sin6_family = AF_INET6;
    ip6addr->sin6_port = rawio_invite_port;
    ip6addr->sin6_addr = in6addr_any;
    //ip6addr->sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    struct sockaddr_in *ip4addr = (struct sockaddr_in *)&local_addr;
    
    ip4addr->sin_family = AF_INET;
    ip4addr->sin_port = rawio_invite_port;
    ip4addr->sin_addr.s_addr = INADDR_ANY;
    //ip4addr->sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#else
#ifdef HAVE_IPV6
    unsigned int iface_id = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    XASSERT( iface_id != 0, iface_id );
    
    local_addr.addr.ipv6.sin6_family = PF_INET6;
    local_addr.addr.ipv6.sin6_port = rawio_invite_port;
    local_addr.addr.ipv6.sin6_scope_id = iface_id;
    local_addr.addr.ipv6.sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    local_addr.addr.ipv4.sin_family = PF_INET;
    local_addr.addr.ipv4.sin_port = rawio_invite_port;
    local_addr.addr.ipv4.sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#endif // __linux__

    sock_rcode = bind(rawio_invite_sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_storage));
    if (sock_rcode == -1)
    {
      // perror("socket bind");
        DBG_ERR("message socket bind error %d (sock_error=%d)\n", rawio_invite_sock, thread_errno);
        XASSERT( 0, thread_errno );
    }

    DBG_VERBOSE("message port = %d\n", rawio_invite_port);    
    
    sock_rcode = listen(rawio_invite_sock, NET_RAW_IO_DEFAULT_BACKLOG);
    if (sock_rcode == -1)
    {
        DBG_ERR("message socket listen error %d (sock_error=%d)\n", rawio_invite_sock, thread_errno);
        XASSERT( 0, thread_errno );
    }
    
    px_status = posix_mutex_init( &rawio_global_mtx );
    XASSERT( px_status==0, px_status );
    
    DBG_VERBOSE("starting net_raw_io thread, stack size = %d\n", sizeof(rawio_thread_stack));    
    px_status = posix_create_thread(&rawio_thread_id, net_raw_io_main, 0, "netrawio", &rawio_thread_stack, sizeof(rawio_thread_stack), POSIX_THR_PRI_NORMAL);
    XASSERT( px_status==0, px_status );

    return rcode;
}

int32_t net_raw_io_register(Channel_type_t type, uint16_t port, const char *name, 
                            net_raw_io_sock_config_func_ptr_t sock_config_func_ptr)
{
    net_raw_io_client_ctxt_t *client_ctxt = NULL;
    int px_status, sock_rcode;
    struct sockaddr_storage msg_addr;
    bool mtx_created = false;
    
    client_ctxt = (net_raw_io_client_ctxt_t *)MEM_MALLOC(sizeof(net_raw_io_client_ctxt_t));
    if(!client_ctxt)
    {
        goto error;
    }

    memset(client_ctxt, 0, sizeof(net_raw_io_client_ctxt_t));
    
    client_ctxt->hdr.sock = -1;
    client_ctxt->hdr.sock_fd_flags = SOCK_FD_READ;
    client_ctxt->message_sock = -1;
    client_ctxt->type = type;
    client_ctxt->port = port;
    client_ctxt->name = name;
    client_ctxt->sock_config_func_ptr = sock_config_func_ptr; // may be NULL
    
    px_status = posix_mutex_init( &client_ctxt->mtx );
    if(px_status != 0)
    {
        DBG_ERR("error creating client_ctxt mutex for %s (px_status=0x%02x)\n", client_ctxt->name, px_status);
        goto error;
    }
    mtx_created = true;

#ifdef HAVE_IPV6
    client_ctxt->message_sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#else // !HAVE_IPV6
    client_ctxt->message_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif // HAVE_IPV6
    if(client_ctxt->message_sock == -1)
    {
        DBG_ERR("%s: socket connect error (sock_error=%d)\n", client_ctxt->name, thread_errno);
        goto error;
    }
    
    memset(&msg_addr, 0, sizeof(struct sockaddr_storage)); 

    char *loopback = NET_RAW_IO_LOOPBACK_ADDR;
#ifdef __linux__
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)&msg_addr;
    
    ip6addr->sin6_family = PF_INET6;
    ip6addr->sin6_port = rawio_invite_port;
    inet_pton(AF_INET6, loopback, &(ip6addr->sin6_addr));
    //ip6addr->sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    struct sockaddr_in *ip4addr = (struct sockaddr_in *)&msg_addr;
    
    ip4addr->sin_family = PF_INET;
    ip4addr->sin_port = rawio_invite_port;
    inet_aton(loopback, &(ip4addr->sin_addr)); 
    //ip4addr->sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#else
#ifdef HAVE_IPV6
    msg_addr.addr.ipv6.sin6_family = PF_INET6;
    msg_addr.addr.ipv6.sin6_port = rawio_invite_port;
    msg_addr.addr.ipv6.sin6_len = sizeof(struct sockaddr_in6);
    inet_pton(AF_INET6, loopback, &(msg_addr.addr.ipv6.sin6_addr));
#else // !HAVE_IPV6
    msg_addr.addr.ipv4.sin_family = PF_INET;
    msg_addr.addr.ipv4.sin_port = rawio_invite_port;
    msg_addr.addr.ipv4.sin_len = sizeof(struct sockaddr_in);
    msg_addr.addr.ipv4.sin_addr.s_addr = inet_aton(loopback); 
#endif // HAVE_IPV6
#endif // __linux__
	/* connect to message socket */
	sock_rcode = connect(client_ctxt->message_sock, (struct sockaddr *)&msg_addr, sizeof(struct sockaddr_storage));
    if(sock_rcode == -1)
    {
        DBG_ERR("%s: socket connect error (sock_error=%d)\n", client_ctxt->name, thread_errno);
        goto error;
    }

	/* send a message for new client */
    if (net_raw_io_send_msg(client_ctxt, NET_RAW_IO_MSG_NEW_CLIENT, NULL) != NET_RAW_IO_OK)
    {
        DBG_ERR("error sending message for %s!\n", client_ctxt->name);
        goto error;
    }
    
    return NET_RAW_IO_OK;

error:
    // cleanup
    if(client_ctxt)
    {
        if(mtx_created)
        {
            pthread_mutex_destroy(&client_ctxt->mtx);
            mtx_created = false;
        }
        if (client_ctxt->message_sock)
        {
            close(client_ctxt->message_sock);
        }
        MEM_FREE_AND_NULL(client_ctxt);
    }
    return NET_RAW_IO_FATAL_ERROR;
}

#endif
