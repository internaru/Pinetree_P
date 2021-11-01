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
#include "net_raw_io_api.h"
#include "net_scan_api.h"


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "NETSCAN: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SCAN 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


//=========================================================================================================================
// Config options -- rdj TODO move to net_scan_config.c
//=========================================================================================================================

#define NET_SCAN_PORT                   9101
#define NET_SCAN_SEND_BUF_SIZE          (30 * 1024)
#define NET_SCAN_RECV_BUF_SIZE          (2 * 1024)


//=========================================================================================================================
// Data types
//=========================================================================================================================


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static bool net_scan_sock_config_func(int listen_sock);


//=========================================================================================================================
// Module API
//=========================================================================================================================

int32_t net_scan_init(void)
{
    int32_t net_raw_io_rcode = NET_RAW_IO_OK;

    net_raw_io_rcode = net_raw_io_register(e_Scan, NET_SCAN_PORT, "rawScan", net_scan_sock_config_func);
    if(net_raw_io_rcode != NET_RAW_IO_OK)
    {
        DBG_ERR("net_raw_io_register failed (net_raw_io_rcode=0x%02x)\n", net_raw_io_rcode);
        goto error;
    }

    return NET_SCAN_OK;

error:
    return NET_SCAN_FATAL_ERROR;
}


//=========================================================================================================================
// Callback Routines
//=========================================================================================================================

// configuration of listen socket specific to scan client
// assumes bound socket
static bool net_scan_sock_config_func(int listen_sock)
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

    // adjust send buff size to account for bulk flow in the tx direction (i.e. for scan jobs)
    sock_option = NET_SCAN_SEND_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_SNDBUF failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }

    // adjust recv buff size to account for bulk flow in the tx direction (i.e. for scan jobs)
    sock_option = NET_SCAN_RECV_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_RCVBUF failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }

    // one active connection plus backlog of 9 for a total of 10 simultaneous connections
    sock_rcode = listen(listen_sock, 9);
    if(sock_rcode == -1)
    {
        DBG_ERR("listen failed! (SocketError=%d)\n", thread_errno);
        goto done;
    }
    
    // successful configuration
    return true;

done:
    // configuration failed
    return false;
}

