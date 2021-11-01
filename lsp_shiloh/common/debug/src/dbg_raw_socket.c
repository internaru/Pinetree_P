/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <stdio.h>
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
#include "net_iface_api.h"
#include "net_iface_vars.h"
#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#endif
#include "pthread.h"
#include "posix_ostools.h"
#include "ATypes.h"
#include "sys_init_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "dbg_api.h"

#include "logger.h"
#define DBG_PRFX            "\nRAW SOCKET: "
#define LOGGER_SUBMODULE    0x00080000
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE

#ifdef HAVE_IPV6
#ifdef __linux__
#define DBG_RAW_SOCKET_LOOPBACK_ADDR       "::ffff:127.0.0.1"
#else
#define DBG_RAW_SOCKET_LOOPBACK_ADDR       "::1"
#endif
#else // !HAVE_IPV6
#define DBG_RAW_SOCKET_LOOPBACK_ADDR       "127.0.0.1"
#endif // HAVE_IPV6

#define DBG_RAW_SOCKET_CLIENT_LIST_INSERT(_item_) dbg_raw_socket_list_insert_head(&dbg_raw_socket_client_list, (void*)_item_)
#define DBG_RAW_SOCKET_CLIENT_LIST_GET(_index_) dbg_raw_socket_list_get(dbg_raw_socket_client_list, _index_)
#define DBG_RAW_SOCKET_CLIENT_LIST_LOOKUP(_sock_) dbg_raw_socket_list_lookup(dbg_raw_socket_client_list, _sock_)
#define DBG_RAW_SOCKET_CLIENT_LIST_REMOVE(_item_) dbg_raw_socket_list_remove(&dbg_raw_socket_client_list, (void*)_item_)

#define DBG_RAW_SOCKET_GLOBAL_LOCK() \
        { \
            int px_status; \
            px_status = pthread_mutex_lock( &dbg_raw_socket_global_mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

#define DBG_RAW_SOCKET_GLOBAL_UNLOCK() \
        { \
            int px_status; \
            px_status = pthread_mutex_unlock( &dbg_raw_socket_global_mtx ); \
            XASSERT(px_status == 0, px_status); \
        } 

// thread stack size
#define DBG_RAW_SOCKET_STACK_SIZE POSIX_MIN_STACK_SIZE

// raw io default settings
#define DBG_RAW_SOCKET_BASE_PORT     50411 
#define DBG_RAW_SOCKET_BASE_BACKLOG  5


//=========================================================================================================================
// Data types
//=========================================================================================================================

typedef struct
{
    void        *next;
    int         sock;
    uint32_t    flags;
} dbg_raw_socket_list_t;


//=========================================================================================================================
// Global statics
//=========================================================================================================================

// main thread
static bool dbg_raw_socket_init_complete = false;
static pthread_mutex_t dbg_raw_socket_global_mtx;
static pthread_t dbg_raw_socket_thread_id;
ALIGN8 static unsigned char dbg_raw_socket_thread_stack[DBG_RAW_SOCKET_STACK_SIZE];

static int dbg_raw_socket_invite_sock;
static int dbg_raw_socket_invite_port;

static dbg_raw_socket_list_t *dbg_raw_socket_client_list = NULL;

        
//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static void *dbg_raw_socket_main( void *unused );

//=========================================================================================================================
// Linked List Helper Routines
//=========================================================================================================================

void *dbg_raw_socket_list_insert_head(dbg_raw_socket_list_t **list, void *item)
{
    dbg_raw_socket_list_t *temp = (dbg_raw_socket_list_t *)item;
    
    ASSERT( list != NULL );
    ASSERT( item != NULL );

    DBG_RAW_SOCKET_GLOBAL_LOCK();
    // add to list
    temp->next = *list;
    *list = temp;
    DBG_RAW_SOCKET_GLOBAL_UNLOCK();
    
    return temp;
}

void *dbg_raw_socket_list_get(dbg_raw_socket_list_t *list, uint32_t index)
{
    uint32_t i = 0;
    dbg_raw_socket_list_t *cur;

    DBG_RAW_SOCKET_GLOBAL_LOCK();
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
    DBG_RAW_SOCKET_GLOBAL_UNLOCK();
    
    return cur;
}

void *dbg_raw_socket_list_lookup(dbg_raw_socket_list_t *list, int sock)
{
    dbg_raw_socket_list_t *cur;

    DBG_RAW_SOCKET_GLOBAL_LOCK();
    cur = list;

    while(cur)
    {
        if(cur->sock == sock)
        {
            break;
        }

        cur = cur->next;
    }
    DBG_RAW_SOCKET_GLOBAL_UNLOCK();

    return cur;
}

void *dbg_raw_socket_list_remove(dbg_raw_socket_list_t **list, void *item)
{
    dbg_raw_socket_list_t *cur, **prev;

    ASSERT( list != NULL );

    if (item == NULL)
    {
        return NULL;
    }
    
    DBG_RAW_SOCKET_GLOBAL_LOCK();
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
        
        prev = (dbg_raw_socket_list_t **)&cur->next;
        cur = cur->next;
    }
    DBG_RAW_SOCKET_GLOBAL_UNLOCK();

    return cur;
}

void dbg_raw_socket_rcvr(char* data, int len, void* arg)
{
    dbg_raw_socket_list_t *client;

    // process all current clients
    int i = 0;
    while ((client = DBG_RAW_SOCKET_CLIENT_LIST_GET(i++)) != NULL)
    {
        int sock = client->sock;
        
        // check for valid sock
        if (sock >= 0)
        {
#ifdef __linux__
            int sock_rcode = send(sock, (void *)data, len, MSG_NOSIGNAL);
#else                
            int sock_rcode = send(sock, (void *)data, len, 0);
#endif                
            if(sock_rcode == -1)
            {
                int sock_error = thread_errno;
            
                if(sock_error == EWOULDBLOCK || sock_error == EAGAIN)
                {
                    /* 
                     *  NOTE: should only get here when the socket's file descriptor is marked O_NONBLOCK 
                     *  and the requested operation would block
                     */
                     
                     sock_rcode = 0;
                }
            }
            
            if(sock_rcode == -1)
            {
#ifdef __linux__
                close(sock);
#else                
                tfClose(sock);
#endif                
                
                DBG_RAW_SOCKET_CLIENT_LIST_REMOVE(DBG_RAW_SOCKET_CLIENT_LIST_LOOKUP(sock));
            }
        }
    } 
}

int dbg_raw_socket_sock_data(dbg_raw_socket_list_t *item, unsigned char *data, int len)
{
    int ret;

    ret = recv(item->sock, (void *)data, len, 0);
    if (ret == -1)
    {
        DBG_PRINTF(LOG_DEBUG, DBG_PRFX "dbg_raw_socket_sock_data: Closing sock = %d\n", item->sock);
       
#ifdef __linux__
        close(item->sock);
#else                
        tfClose(item->sock);
#endif                
        
        DBG_RAW_SOCKET_CLIENT_LIST_REMOVE(DBG_RAW_SOCKET_CLIENT_LIST_LOOKUP(item->sock));
                        
        ret = 0;
    }
    
    return ret;
}

#ifdef __linux__
void *dbg_raw_socket_get_in_addr( struct sockaddr *sa )
{
#ifdef HAVE_IPV6
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
#else    
    return &(((struct sockaddr_in*)sa)->sin_addr);
#endif    
}
#endif

/** 
 * \brief Debug Raw Socket main program thread
 * 
 **/
static void *dbg_raw_socket_main( void *unused )
{
    int sock;
    struct sockaddr_storage sock_addr;
#ifdef __linux__
    socklen_t addr_len;
#else
    int addr_len;
#endif

    dbg_printf("%s: TID = %d\n", __func__, gettid());

    // main loop
    while (1)
    {
        DBG_PRINTF(LOG_INFO, DBG_PRFX "waiting for new connection\n");
        
        memset(&sock_addr, 0, sizeof(struct sockaddr_storage)); 
        addr_len = sizeof(struct sockaddr_storage);
        
        // accept the client connection
        sock = accept(dbg_raw_socket_invite_sock, (struct sockaddr *)&sock_addr, &addr_len);
        if(sock == -1)
        {
            DBG_PRINTF(LOG_ERR, DBG_PRFX "message socket accept error (sock_error=%d)\n", thread_errno);
        }
        else
        {
    #ifdef __linux__
    #ifdef HAVE_IPV6
            char addr_str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, dbg_raw_socket_get_in_addr((struct sockaddr *)&sock_addr), addr_str, INET6_ADDRSTRLEN);
    #else // !HAVE_IPV6
            char addr_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, dbg_raw_socket_get_in_addr((struct sockaddr *)&sock_addr), addr_str, INET_ADDRSTRLEN);
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

    #ifndef DEBUG // only accept connections from the loopback address on message socket for release builds
            if (strcmp(addr_str, DBG_RAW_SOCKET_LOOPBACK_ADDR) == 0)
    #endif // ifndef DEBUG
            {
                DBG_PRINTF(LOG_DEBUG, DBG_PRFX "adding new connection, sock = %d from ip = %s\n", sock, addr_str);
                
                // add to client list
                dbg_raw_socket_list_t *new_client = MEM_MALLOC(sizeof(dbg_raw_socket_list_t));
                ASSERT( new_client != NULL );
                memset(new_client, 0, sizeof(dbg_raw_socket_list_t));
                new_client->sock = sock;
                DBG_RAW_SOCKET_CLIENT_LIST_INSERT( new_client );
            }
    #ifndef DEBUG 
            else
            {
                DBG_PRINTF(LOG_ERR, DBG_PRFX "rejecting non-loopback connection, sock = %d from ip = %s\n", sock, addr_str);
                
                // Reject non-loopback connection
    #ifdef __linux__
                close(sock);
    #else                
                tfClose(sock);
    #endif                
                sock = -1;
            }
    #endif // ifndef DEBUG
        }
    } 

    return 0;
} 


//=========================================================================================================================
// Module API
//=========================================================================================================================

void dbg_raw_socket_init(void)
{
    int px_status, sock_rcode;
    struct sockaddr_storage local_addr;

    dbg_raw_socket_invite_sock = -1;
    dbg_raw_socket_invite_port = DBG_RAW_SOCKET_BASE_PORT; 

#ifdef HAVE_IPV6
    dbg_raw_socket_invite_sock = socket(AF_INET6, SOCK_STREAM, 0);
#else // !HAVE_IPV6
    dbg_raw_socket_invite_sock = socket(AF_INET, SOCK_STREAM, 0);
#endif // HAVE_IPV6
    XASSERT( dbg_raw_socket_invite_sock != -1, errno );

    // prevent socket inheritance across fork/exec
    fcntl(dbg_raw_socket_invite_sock, F_SETFD, (FD_CLOEXEC | fcntl(dbg_raw_socket_invite_sock, F_GETFD)));
    
    /* So that we can re-bind to it without TIME_WAIT problems */
    int reuse_addr = 1;
    setsockopt(dbg_raw_socket_invite_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr, sizeof(reuse_addr));
    
    /*
     *  bind the socket to a well known TCP port
     */

    memset(&local_addr, 0, sizeof(struct sockaddr_storage)); 
    
#ifdef __linux__
#ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)&local_addr;
    
    ip6addr->sin6_family = AF_INET6;
    ip6addr->sin6_port = htons(dbg_raw_socket_invite_port);
    ip6addr->sin6_addr = in6addr_any;
#else // !HAVE_IPV6
    struct sockaddr_in *ip4addr = (struct sockaddr_in *)&local_addr;
    
    ip4addr->sin_family = AF_INET;
    ip4addr->sin_port = htons(dbg_raw_socket_invite_port);
    ip4addr->sin_addr.s_addr = INADDR_ANY;
#endif // HAVE_IPV6
#else
#ifdef HAVE_IPV6
    unsigned int iface_id = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    XASSERT( iface_id != 0, iface_id );
    
    local_addr.addr.ipv6.sin6_family = PF_INET6;
    local_addr.addr.ipv6.sin6_port = htons(dbg_raw_socket_invite_port);
    local_addr.addr.ipv6.sin6_scope_id = iface_id;
    local_addr.addr.ipv6.sin6_len = sizeof(struct sockaddr_in6);
#else // !HAVE_IPV6
    local_addr.addr.ipv4.sin_family = PF_INET;
    local_addr.addr.ipv4.sin_port = htons(dbg_raw_socket_invite_port);
    local_addr.addr.ipv4.sin_len = sizeof(struct sockaddr_in);
#endif // HAVE_IPV6
#endif // __linux__

    sock_rcode = bind(dbg_raw_socket_invite_sock, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_storage));
    if (sock_rcode == -1)
    {
        XASSERT( 0, thread_errno );
    }

    sock_rcode = listen(dbg_raw_socket_invite_sock, DBG_RAW_SOCKET_BASE_BACKLOG);
    if (sock_rcode == -1)
    {
        XASSERT( 0, thread_errno );
    }
    
    px_status = posix_mutex_init( &dbg_raw_socket_global_mtx );
    XASSERT( px_status==0, px_status );
    
    dbg_raw_socket_init_complete = true;
    
    px_status = posix_create_thread(&dbg_raw_socket_thread_id, dbg_raw_socket_main, 0, "dbgrawsocket", &dbg_raw_socket_thread_stack, sizeof(dbg_raw_socket_thread_stack), POSIX_THR_PRI_NORMAL);
    XASSERT( px_status==0, px_status );
    
    // register for debug output
    dbg_watcher_register(dbg_raw_socket_rcvr, NULL);

}

int dbg_socket_receive(unsigned char *data, int len, int timeout_ms)
{
    int ret = 0;
    dbg_raw_socket_list_t *client;
    fd_set fds;
    int fds_nfds;
    int socks; 
    struct timeval timeout;  
    
    if (!dbg_raw_socket_init_complete)
        return 0;

    // zero the fd_set
    FD_ZERO(&fds);
    fds_nfds = 0;
    
	// add all registered client connections to the fd_set
    int i=0;
    int count=0;
    while ((client = DBG_RAW_SOCKET_CLIENT_LIST_GET(i++)) != NULL)
    {
        // only add valid (accepted) sockets for listening
        if (client->sock >= 0)
        {
            FD_SET(client->sock, &fds);
            
            if (client->sock > fds_nfds)
            {
                fds_nfds = client->sock;
            }
            
            count++;
        }
    } 

    // first check if any clients were found in list
    if (count > 0)
    {
        // set ms timeout
        timeout.tv_sec = timeout_ms/MILLISEC_PER_SECOND;
        timeout.tv_usec = (timeout_ms%MILLISEC_PER_SECOND) * (USEC_PER_MILLISEC);
        
        socks = select(fds_nfds+1, &fds, 0, 0, &timeout);
        if (socks > 0)
        {
            // process all clients
            int i = 0;
            while (ret == 0 && (client = DBG_RAW_SOCKET_CLIENT_LIST_GET(i++)) != NULL)
            {
                // check for valid sock
                if (client->sock >= 0)
                {
                    if (FD_ISSET(client->sock, &fds))
                    {
                        ret = dbg_raw_socket_sock_data(client, data, len);
                    }
                }
            } 
        }
    }
    
    return ret;
}
