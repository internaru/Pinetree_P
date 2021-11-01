/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "memAPI.h"
#include "lassert.h"
#include "error_types.h"
#include "logger.h"
#include "io_device.h"
#include "net_logger.h"
#include <pthread.h>
#include <posix_ostools.h>
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include "net_iface_api.h"
#endif
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "net_raw_io_api.h"
#include "net_print_api.h"


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "RAW_PRINT: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_PRINT 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


//=========================================================================================================================
// Config options -- rdj TODO move to net_print_config.c
//=========================================================================================================================

#define NET_PRINT_PORT                  9100
#define NET_PRINT_SEND_BUF_SIZE         (2 * 1024)
#define NET_PRINT_RECV_BUF_SIZE         (30 * 1024)
#define NET_PRINT_PEND_RECV_BUF_SIZE    (2 * 1518)


//=========================================================================================================================
// Data types
//=========================================================================================================================


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static bool net_print_sock_config_func(int listen_sock);


//=========================================================================================================================
// Module API
//=========================================================================================================================

int32_t net_print_init(void)
{
    int32_t net_raw_io_rcode = NET_RAW_IO_OK;

    net_raw_io_rcode = net_raw_io_register(e_Print, NET_PRINT_PORT, "rawPrint", net_print_sock_config_func);
    if(net_raw_io_rcode != NET_RAW_IO_OK)
    {
        DBG_ERR("net_raw_io_register failed (net_raw_io_rcode=0x%02x)\n", net_raw_io_rcode);
        goto error;
    }

    return NET_PRINT_OK;

error:
    return NET_PRINT_FATAL_ERROR;
}


//=========================================================================================================================
// Callback Routines
//=========================================================================================================================

// configuration of listen socket specific to print client
// assumes bound socket
static bool net_print_sock_config_func(int listen_sock)
{
    uint32_t sock_option;
    int sock_rcode;

    // enable connection timeouts
    sock_option = 1;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_KEEPALIVE failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }

    // adjust send buff size to account for bulk flow in the rx direction (i.e. for print jobs)
    sock_option = NET_PRINT_SEND_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_SNDBUF failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }

    // adjust recv buff size to account for bulk flow in the rx direction (i.e. for print jobs)
    sock_option = NET_PRINT_RECV_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_RCVBUF failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }

#ifdef __linux__
#else
    /*
     * Decrease the ack delay to improve our performance. This decreases the
     * delay between when we get data and send the ack for it
    */
// rdj TODO find if really necessary
//    sock_option = 2;
//    sock_rcode = setsockopt(sock, IP_PROTOTCP, TM_TCP_DELAY_ACK, (char *)&sock_option, sizeof(sock_option));
#endif

    // one active connection plus backlog of 9 for a total of 10 simultaneous connections
    sock_rcode = listen(listen_sock, 9);
    if(sock_rcode == -1)
    {
        DBG_ERR("listen failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }
    
#ifdef __linux__
#else
    /* Adjust recv buf size for pending backlog connections. This option is only allowed on an already
    * listening socket.
    */
    sock_option = NET_PRINT_PEND_RECV_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, IP_PROTOTCP, TM_TCP_PEND_ACCEPT_RECV_WND,
                           (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt TM_TCP_PEND_ACCEPT_RECV_WND failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }
#endif
    
    // successful configuration
    return true;

done:
    // configuration failed
    return false;
}

