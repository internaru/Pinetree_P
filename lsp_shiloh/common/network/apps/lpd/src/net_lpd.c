//Minimal LPD implementation

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
#include "net_lpd_api.h"


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "LPD_PRINT: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_PRINT 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


#define NET_LPD_SEND_BUF_SIZE         (2 * 1024)
#define NET_LPD_RECV_BUF_SIZE         (30 * 1024)
#define NET_LPD_PEND_RECV_BUF_SIZE    (2 * 1518)


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static bool net_lpd_sock_config_func(int listen_sock);


//=========================================================================================================================
// Module API
//=========================================================================================================================

int32_t net_lpd_init(void)
{
    int32_t net_lpd_io_rcode = NET_LPD_IO_OK;
    net_lpd_io_rcode = net_raw_io_register(e_Print, NET_LPD_PORT, "lpdPrint", net_lpd_sock_config_func);
    if(net_lpd_io_rcode != NET_RAW_IO_OK)
    {
        DBG_ERR("net_raw_io_register failed (net_lpd_io_rcode=0x%02x)\n", net_lpd_io_rcode);
        return NET_LPD_FATAL_ERROR;
    }
    return  NET_LPD_IO_OK;
}


//=========================================================================================================================
// Callback Routines
//=========================================================================================================================

// configuration of listen socket specific to print client
// assumes bound socket
static bool net_lpd_sock_config_func(int listen_sock)
{
    uint32_t sock_option;
    int sock_rcode;

    // enable connection timeouts
    sock_option = 1;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_KEEPALIVE, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_KEEPALIVE failed! (SocketError=%d)\n", thread_errno);
        return false;
    }

    // adjust send buff size to account for bulk flow in the rx direction (i.e. for print jobs)
    sock_option = NET_LPD_SEND_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_SNDBUF failed! (SocketError=%d)\n", thread_errno);
        return false;
    }

    // adjust recv buff size to account for bulk flow in the rx direction (i.e. for print jobs)
    sock_option = NET_LPD_RECV_BUF_SIZE;
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (char *)&sock_option, sizeof(sock_option));
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_RCVBUF failed! (SocketError=%d)\n", thread_errno);
        return false;
    }
    // one active connection plus backlog of 9 for a total of 10 simultaneous connections
    sock_rcode = listen(listen_sock, 9);
    if(sock_rcode == -1)
    {
        DBG_ERR("listen failed! (SocketError=%d)\n", thread_errno);
        return false;
    }
    // successful configuration
    return true;
}

