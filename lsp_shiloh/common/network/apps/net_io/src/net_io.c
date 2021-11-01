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
#include <pthread.h>
#include "error_types.h"
#include "lassert.h"
#include "agConnectMgr.h"
#include "logger.h"
#include "io_device.h"
#include "net_io_api.h"
#include "utils.h"
#include "net_logger.h"
#ifdef HAVE_PRINT_SUPPORT
#include "PrintJobInfo.h"
#endif

// private headers
#include "net_io.h"

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "NETIO: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_NET_IO 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE2(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__) // iterative logging of data flow - slow
//#define DBG_VERBOSE2(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__) // iterative logging of data flow - slow
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


//=========================================================================================================================
// Config options -- rdj TODO move to net_print_config.c
//=========================================================================================================================

/**
 * @brief Limits max number of simultaneous network *print* jobs.
 * 
 * >1 print pipes allow job pipelining (i.e. back to back print jobs) from different network hosts
 * (i.e. multiple simultaneous TCP connections). Otherwise only pipelining of multiple jobs from the
 * same host is possible. Goal is to allocate enough network print channels to keep the engine busy
 * without any pause between consecutive jobs.
 */
#define NET_IO_CHANNEL_POOL_MAX_SIZE 3


//=========================================================================================================================
// Data types
//=========================================================================================================================

typedef struct
{
    net_io_session_t    *head;
    net_io_session_t    **tail;
} net_io_acquiring_list_t;

typedef struct 
{
    net_io_channel_t        *scan_channel;
    net_io_channel_t        *print_channel_free_list;
    net_io_acquiring_list_t print_session_acquiring_list;
    net_io_acquiring_list_t scan_session_acquiring_list;
    pthread_mutex_t     mtx;
} net_io_ctxt_t;

static net_io_ctxt_t g_net_io_ctxt = {0};


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

// I/O Device Interface Callbacks
static int32_t net_channel_read(io_device_impl_t *user_ctxt, void *buf, uint32_t len, uint32_t timeout);
static int32_t net_channel_write(io_device_impl_t *user_ctxt, void *buf, uint32_t len, uint32_t timeout);
static int32_t net_channel_ioctl(io_device_impl_t *user_ctxt, ioctl_cmd_t cmd, void *details);

// Channel Helper Routines
static net_io_channel_t *net_channel_create(Channel_type_t channel_type);
static net_io_channel_t *net_channel_acquire(Channel_type_t channel_type);
static void net_channel_connect(net_io_channel_t *net_channel);
static void netio_channel_detach(net_io_session_t *session);

// Linked List Helper Routines
static void net_channel_free_list_insert_head(net_io_channel_t *net_channel);
static net_io_channel_t *net_channel_free_list_remove_head(Channel_type_t channel_type);
static void net_session_acquiring_list_insert_tail(net_io_session_t *session);
static net_io_session_t *net_session_acquiring_list_remove_head(Channel_type_t channel_type);
static void net_session_acquiring_list_remove(net_io_session_t *session);


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================
#ifdef DEBUG 
# define NET_IO_LOCK_CONTEXT()   DBG_XASSERT( 0 == pthread_mutex_lock(&g_net_io_ctxt.mtx), errno );	
# define NET_IO_UNLOCK_CONTEXT() DBG_XASSERT( 0 == pthread_mutex_unlock(&g_net_io_ctxt.mtx), errno );	
#else
# if 0 /// ignore error return in release code
#  define NET_IO_LOCK_CONTEXT()   pthread_mutex_lock(&g_net_io_ctxt.mtx);
#  define NET_IO_UNLOCK_CONTEXT() pthread_mutex_unlock(&g_net_io_ctxt.mtx);	
# else /// release assert on non functional OS 
#  define NET_IO_LOCK_CONTEXT()   REL_XASSERT( 0 == pthread_mutex_lock(&g_net_io_ctxt.mtx), errno );	
#  define NET_IO_UNLOCK_CONTEXT() REL_XASSERT( 0 == pthread_mutex_unlock(&g_net_io_ctxt.mtx), errno );	
# endif 
#endif

//=========================================================================================================================
// Module API
//=========================================================================================================================
net_io_channel_t* netio_debug_chan_list[NET_IO_CHANNEL_POOL_MAX_SIZE];
int32_t net_io_init(void)
{
    DBG_VERBOSE("==>%s\n", __func__);

    int px_status;
    int32_t net_io_rcode = NET_IO_SUCCESS;
    net_io_channel_t *net_channel;
    int i;

    px_status = posix_mutex_init( &g_net_io_ctxt.mtx );
    if(px_status != 0)
    {
        DBG_ERR("error creating g_net_io_ctxt mutex (px_status=0x%02x)\n", px_status);
        net_io_rcode = NET_IO_FATAL_ERROR;
        goto done;
    }

    g_net_io_ctxt.print_session_acquiring_list.head = NULL;
    g_net_io_ctxt.print_session_acquiring_list.tail = &g_net_io_ctxt.print_session_acquiring_list.head;
    g_net_io_ctxt.scan_session_acquiring_list.head = NULL;
    g_net_io_ctxt.scan_session_acquiring_list.tail = &g_net_io_ctxt.scan_session_acquiring_list.head;

    g_net_io_ctxt.scan_channel = NULL;
    g_net_io_ctxt.print_channel_free_list = NULL;
    for(i = 0; i<NET_IO_CHANNEL_POOL_MAX_SIZE; i++)
    {
        net_channel = net_channel_create(e_Print);
        if(!net_channel)
        {
            net_io_rcode = NET_IO_LOW_MEM;
            goto done;
        }
        net_channel_free_list_insert_head(net_channel);
        netio_debug_chan_list[i] = net_channel;
    }

done:
    if(net_io_rcode != NET_IO_SUCCESS)
    {
        // cleanup
        net_channel = net_channel_free_list_remove_head(e_Print);
        while(net_channel)
        {
            MEM_FREE_AND_NULL(net_channel);
            net_channel = net_channel_free_list_remove_head(e_Print);
        }
    }

    return net_io_rcode;
}

int32_t net_io_open(Channel_type_t channel_type, net_io_func_table_t *func_table,
                    void *user_ctxt, net_io_session_hndl_t *session_hndl)
{
    DBG_VERBOSE("==>%s\n", __func__);

    net_io_session_t *session = NULL;
    int32_t net_io_rcode = NET_IO_SUCCESS;

    *session_hndl = NULL;
    
    session = (net_io_session_t *)MEM_MALLOC(sizeof(net_io_session_t));
    if(!session)
    {
        return NET_IO_LOW_MEM;        
    }

    ASSERT(func_table);
    session->app_recv = func_table->io_recv;
    session->app_notify = func_table->io_notify;
    session->job_event_func = NULL;
    session->user_ctxt = user_ctxt;
    session->next = NULL;
    session->channel_type = channel_type;
    session->ready = false;
    session->cancel_data = false;

    NET_IO_LOCK_CONTEXT();
    session->state = NET_IO_STATE_INACTIVE;
    session->cur_job = NULL;
    session->net_channel = net_channel_acquire(channel_type);
    if(!session->net_channel)
    {
        session->state = NET_IO_STATE_ACQUIRING;
        net_session_acquiring_list_insert_tail(session);
    }
    else
    {
        // session now owns the channel
        session->net_channel->session = session;
    }
#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x open\n", session);
#endif // NETIO_DEBUG
    NET_IO_UNLOCK_CONTEXT();

    if(session->net_channel && session->app_notify)
    {
        session->app_notify(NET_IO_SESSION_EVENT_INACTIVE, session->user_ctxt);
    }

    *session_hndl = (net_io_session_hndl_t)session;
    DBG_VERBOSE("<==%s (session=%08x rcode=%d)\n", __func__, session, net_io_rcode);
    return net_io_rcode;
}

void net_io_ready(net_io_session_hndl_t session_hndl)
{
    net_io_session_t *session = NULL;

    ASSERT(session_hndl);
    session = (net_io_session_t *)session_hndl;

    DBG_VERBOSE("==>%s (session=%08x), state = %d\n", __func__, session, session->state);

    NET_IO_LOCK_CONTEXT();
#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x ready state=%d\n", session, session->state);
#endif // NETIO_DEBUG
    switch(session->state)
    {
    case NET_IO_STATE_ACQUIRING:

        session->ready = true;
        break;

    case NET_IO_STATE_INACTIVE:

        ASSERT(session->net_channel);
        ASSERT(session == session->net_channel->session);

        session->cancel_data = false;
        session->ready = true;
        net_channel_connect(session->net_channel);
        break;

    case NET_IO_STATE_ACTIVE:

        // should already be 'ready' - ignore the request
        ASSERT(session->ready);
        break;

    default:
        // unexpected session state
        XASSERT(0, session->state);
    }
    NET_IO_UNLOCK_CONTEXT();

    return;
}

void net_io_close(net_io_session_hndl_t session_hndl)
{
    net_io_session_t *session = NULL;

    ASSERT(session_hndl);
    session = (net_io_session_t *)session_hndl;

    DBG_VERBOSE("==>%s (session=%08x), state = %d\n", __func__, session, session->state);

    NET_IO_LOCK_CONTEXT();

#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x close state=%d\n", session, session->state);
#endif // NETIO_DEBUG

    switch(session->state)
    {
    case NET_IO_STATE_ACQUIRING:
        ASSERT(!session->net_channel);
        net_session_acquiring_list_remove(session);
        break;

    case NET_IO_STATE_INACTIVE:
    case NET_IO_STATE_ACTIVE:
        {
            // clear out any outstanding send data
            ASSERT(!session->next);
            ASSERT(session->net_channel);
            ASSERT(session == session->net_channel->session);
            ASSERT(!session->net_channel->next);
            net_io_send_buf_node_t *tmp_node;
            while(session->net_channel->send_queue)
            {
                tmp_node = session->net_channel->send_queue;
                session->net_channel->send_queue = tmp_node->next;
                MEM_FREE_AND_NULL(tmp_node->buf); // free the actual buffer
                MEM_FREE_AND_NULL(tmp_node); // free the container
            }
            session->net_channel->send_queue_tail = &session->net_channel->send_queue;
            session->net_channel->send_queue_emptied = true;
#ifdef HAVE_PRINT_SUPPORT
            netio_job_detach(session);
#endif
            netio_channel_detach(session);
        }
        break;

    default:
        // unexpected session state
        XASSERT(0, session->state);
    }
    NET_IO_UNLOCK_CONTEXT();

    MEM_FREE_AND_NULL(session);

    return;
}

char *net_io_get_send_data(net_io_session_hndl_t session_hndl, uint32_t *len)
{
    ASSERT(session_hndl);
    net_io_session_t *session = (net_io_session_t *)session_hndl;

    ASSERT(len);
    *len = 0;

    char *buf = NULL;

    NET_IO_LOCK_CONTEXT();
    if(session->net_channel)
    {
        ASSERT(session == session->net_channel->session);
        if(session->net_channel->send_queue)
        {
            net_io_send_buf_node_t *tmp_node = session->net_channel->send_queue;
            buf = tmp_node->buf;
            *len = tmp_node->len;
            session->net_channel->send_queue = tmp_node->next;
            if(session->net_channel->send_queue_tail == &tmp_node->next)
            {
                // last item in the list -- update tail pointer
                session->net_channel->send_queue_tail = &session->net_channel->send_queue;
            }
            // free the container -- caller must free the actual buffer
            MEM_FREE_AND_NULL(tmp_node);
        }
        else
        {
            // note that send_queue_emptied lags behind net_channel->send_queue == NULL by one
            // iteration of calling net_io_get_send_data()
            session->net_channel->send_queue_emptied = true;
        }
    }
    NET_IO_UNLOCK_CONTEXT();
    return buf;
}


//=========================================================================================================================
// I/O Device Interface Callbacks
//=========================================================================================================================

static int32_t net_channel_read(io_device_impl_t *user_ctxt, void *buf, uint32_t len, uint32_t timeout)
{
    int32_t sock_rcode;
    int32_t result = CONIO_CONERR;
    uint32_t timeout_msec = timeout*10; // convert from centiseconds to milliseconds
    bool ctxt_locked = false;

    ASSERT(buf);
    ASSERT(len);
    ASSERT(user_ctxt);
    net_io_channel_t *net_channel = (net_io_channel_t *)user_ctxt;

    NET_IO_LOCK_CONTEXT();
    ctxt_locked = true;

    if(!net_channel->session)
    {
        // channel has been orphaned (e.g. net_io_close on active channel)
        result = CONIO_CONDONE;
        DBG_VERBOSE("%s: chan=%08x channel has been orphaned!\n", __func__, net_channel);
        goto done;
    }
    ASSERT(net_channel == net_channel->session->net_channel);

    if(net_channel->session->cancel_data && net_channel->session->channel_type == e_Print)
    {
        // job cancelled - terminate recv bulk data flow for print job
        result = CONIO_CONDONE;
        DBG_VERBOSE("%s: chan=%08x job cancelled - no recv data available\n", __func__, net_channel);
        goto done;
    }

    switch(net_channel->session->state)
    {
    case NET_IO_STATE_INACTIVE:

        DBG_VERBOSE("%s: chan=%08x state Inactive to Active\n", __func__, net_channel);
        // first read indicates our I/O channel has been scheduled by the I/O connection manager
        net_channel->session->state = NET_IO_STATE_ACTIVE;
        if(net_channel->session->app_notify)
        {
            net_channel->session->app_notify(NET_IO_SESSION_EVENT_ACTIVE, net_channel->session->user_ctxt);
        }

        // fall through

    case NET_IO_STATE_ACTIVE:

        if(!net_channel->session->app_recv)
        {
            result = CONIO_CONDONE;
            DBG_VERBOSE("%s: chan=%08x no app_recv CB!\n", __func__, net_channel);
            goto done;
        }
        else
        {
            // must unlock session context BEFORE calling app_recv callback to avoid deadlock with send
            // save cb func ptr and user context since session could be freed following the unlock
            net_io_recv_func_t app_recv_cb = net_channel->session->app_recv;
            void *user_ctxt_t = net_channel->session->user_ctxt;
            NET_IO_UNLOCK_CONTEXT();
            ctxt_locked = false;

            sock_rcode = app_recv_cb((char *)buf, len, &timeout_msec, user_ctxt_t);
            result = sock_rcode;
            if(sock_rcode <= 0)
            {
                // socket recv EOF, timeout, or unrecoverable error
                if(sock_rcode < 0 && timeout_msec == 0)
                {
                    // no data read due to timeout
                    result = CONIO_TIMEOUT;
                }
                else
                {
                    // no data read due to EOF, TCP connection reset, or unrecoverable socket error
                    result = CONIO_CONDONE;
                }
            }
            else 
            {
#ifdef HAVE_PRINT_SUPPORT
                NET_IO_LOCK_CONTEXT();
                netio_job_report_octets_processed(net_channel, result);
                NET_IO_UNLOCK_CONTEXT();
#endif
            }
        }
        break;

    default:
        // unexpected channel state
        XASSERT(0, net_channel->session->state);
    }

done:
    if(ctxt_locked)
    {
        NET_IO_UNLOCK_CONTEXT();
    }

    DBG_VERBOSE2("%s: chan=%08x len=%d tout=%d res=%d\n", __func__, net_channel, len, timeout, result);
    return result;
}

// maps I/O channel write onto network socket send
/* Note we ignore the timeout param because we are required to accept any data passed to us and
 * return immediately.  The caller allocates buffer storage and passes ownership to us. It is our
 * responsibility to free buffer memory after sending over the network socket. A bulk transfer
 * source (e.g. scan & fax) places its own limit on the amount of buffered data generated (e.g.
 * before pausing the scan bar). We must allocate a small amount of overhead (12 bytes per buffer)
 * to temporarily queue up the data prior to sending over the network socket.
 */
static int32_t net_channel_write(io_device_impl_t *user_ctxt, void *buf, uint32_t len, uint32_t timeout)
{
    net_io_send_buf_node_t *buf_node = NULL;

    ASSERT(buf);
    ASSERT(len);
    ASSERT(user_ctxt);
    net_io_channel_t *net_channel = (net_io_channel_t *)user_ctxt;

    NET_IO_LOCK_CONTEXT();
    if(!net_channel->session)
    {
        // channel has been orphaned (e.g. net_io_close on active channel)
        MEM_FREE_AND_NULL(buf);
        NET_IO_UNLOCK_CONTEXT();
        return len;
    }
    ASSERT(net_channel == net_channel->session->net_channel);

    buf_node = (net_io_send_buf_node_t *)MEM_MALLOC(sizeof(net_io_send_buf_node_t));
    if(!buf_node)
    {
        ASSERT(0);
        MEM_FREE_AND_NULL(buf);
        NET_IO_UNLOCK_CONTEXT();

        // TODO not sure what to do here -- at present the return code semantics for net_write have not been clearly
        // defined in io_device_handle_t in io_device.h
        return -1;
    }

    buf_node->next = NULL;
    buf_node->buf = buf;
    buf_node->len = len;

    *net_channel->send_queue_tail = buf_node;
    net_channel->send_queue_tail = &buf_node->next;

    if(net_channel->send_queue_emptied && net_channel->session->app_notify)
    {
        net_channel->send_queue_emptied = false;
        NET_IO_UNLOCK_CONTEXT();
        net_channel->session->app_notify(NET_IO_SESSION_EVENT_HAVE_SEND_DATA, net_channel->session->user_ctxt);
    }
    else
    {
        NET_IO_UNLOCK_CONTEXT();
    }

    return len;
}

static int32_t net_channel_ioctl(io_device_impl_t *user_ctxt, ioctl_cmd_t cmd, void *details)
{
    int32_t result = 0;
    net_io_channel_t *net_channel = (net_io_channel_t *)user_ctxt;

    DBG_VERBOSE("%s: got ioctl %d (chan = %#x)\n", __func__, cmd, net_channel);

    NET_IO_LOCK_CONTEXT();
    if(!net_channel->session)
    {
        // channel has been orphaned (e.g. net_io_close on active channel)
        if(cmd == e_close && !net_channel->is_free)
        {
            netio_job_deregister_channel_events(net_channel);
            net_channel_free_list_insert_head(net_channel);
        }
        NET_IO_UNLOCK_CONTEXT();
        goto done;
    }
    ASSERT(net_channel == net_channel->session->net_channel);

    switch(cmd)
    {
    case e_close:

#ifdef NETIO_DEBUG
    dbg_printf("NETIO: %08x e_close state=%d\n", net_channel->session, net_channel->session->state);
#endif // NETIO_DEBUG

        switch(net_channel->session->state)
        {
        case NET_IO_STATE_ACQUIRING:
        case NET_IO_STATE_INACTIVE:
            // ignore the ioctl
            break;

        case NET_IO_STATE_ACTIVE:
            net_channel->session->state = NET_IO_STATE_INACTIVE;
            if(net_channel->session->app_notify)
            {
                net_channel->session->app_notify(NET_IO_SESSION_EVENT_INACTIVE, net_channel->session->user_ctxt);
            }

            break;

        default:
            // unhandled state
            XASSERT(0, net_channel->session->state);
            break;
        }
        break;

    case e_WaitForWrites:
    case e_ResetWrites:
        // currently unhandled
        DBG_VERBOSE("warning: unhandled net_channel_ioctl %d\n", cmd);
        break;

    case e_IOInterface:
        result = e_network;
        break;

    default:
	// silently ignore cmds we don't grok.
        break;
    }
    NET_IO_UNLOCK_CONTEXT();

done:
    return result;
}


//=========================================================================================================================
// Channel Helper Routines
//=========================================================================================================================

// sanity check should be unique but not a catastrophy if it isn't 
#ifdef HAVE_PRINT_SUPPORT
#define net_io_jobstart_observer_id     net_io_pageend_observer_id // TODO change this to 0x5D83A981 once jobstarted observer is defined
#define net_io_pageend_observer_id      0x5D83A982
#define net_io_jobend_observer_id       0x5D83A983
#define net_io_jobcancel_observer_id    0x5D83A984
#endif

static net_io_channel_t *net_channel_create(Channel_type_t channel_type)
{
    DBG_VERBOSE("==>%s (type=%d)\n", __func__, channel_type);
    PIPE_INFO sys_channel;
    int32_t net_io_rcode = NET_IO_SUCCESS;
    net_io_channel_t *net_channel = NULL;

    net_channel = (net_io_channel_t *)MEM_MALLOC(sizeof(net_io_channel_t));
    if(!net_channel)
    {
        net_io_rcode = NET_IO_LOW_MEM;
        goto done;
    }

    net_channel->next = NULL;
    net_channel->session = NULL;
    net_channel->send_queue = NULL;
    net_channel->send_queue_tail = &net_channel->send_queue;
    net_channel->send_queue_emptied = true;
    net_channel->is_free = true;
    net_channel->observers_attached = false;
    net_channel->channel_type = channel_type;

    if(channel_type == e_Print)
    {
#ifdef HAVE_PRINT_SUPPORT
        netio_job_channel_init(net_channel);
#endif
    }
    
    memset(&sys_channel, 0, sizeof(PIPE_INFO));
    sys_channel.channel_type = channel_type;
    sys_channel.DocType = e_NETIOPAGE;	//e_DOCUMENT;	//LALIT: Set new Document type to get in Print Mgr for Host Print counter
    sys_channel.io.data_read = net_channel_read;
    sys_channel.io.data_read_handle = (io_device_impl_t *)net_channel;
    sys_channel.io.data_write = net_channel_write;
    sys_channel.io.data_write_handle = (io_device_impl_t *)net_channel;
    sys_channel.io.ioctl = net_channel_ioctl;

    net_channel->pipe_hndl = PipeRegister(&sys_channel);
    if(!net_channel->pipe_hndl)
    {
        net_io_rcode = NET_IO_LOW_MEM;
        goto done;
    }

#ifdef HAVE_PRINT_SUPPORT
    if(channel_type == e_Print)
    {
        PrintJobInfo_Set_foreign_context(((IOReg *)(net_channel->pipe_hndl))->pjob_info, (void *)0);
    }
#endif        

done:
    if(net_io_rcode != NET_IO_SUCCESS)
    {
        // cleanup
        if(net_channel)
        {
            MEM_FREE_AND_NULL(net_channel);
        }
    }
    DBG_VERBOSE("<==%s (chan=%08x)\n", __func__, net_channel);
    return net_channel;
}

static net_io_channel_t *net_channel_acquire(Channel_type_t channel_type)
{
    DBG_VERBOSE("==>%s (type=%d)\n", __func__, channel_type);

    net_io_channel_t *net_channel = NULL;

    switch(channel_type)
    {
    case e_Print:
    case e_Scan:
        net_channel = net_channel_free_list_remove_head(channel_type);
        break;

    default:
        // unsupported channel type
        XASSERT(0, channel_type);
        break;
    }

    DBG_VERBOSE("<==%s (chan=%08x)\n", __func__, net_channel);
    return net_channel;
}

static void netio_channel_detach(net_io_session_t *session)
{
    DBG_VERBOSE("==>%s (session=%08x)\n", __func__, session);

    NET_IO_LOCK_CONTEXT();

    ASSERT(session);

    if(!session->net_channel)
    {
        goto done;
    }

    // detach channel from session
    ASSERT(!session->net_channel->is_free);
    session->net_channel->session = NULL;

    // if state is NET_IO_STATE_ACTIVE we must wait until e_close to free the channel, to
    // prevent a new session from sending data to the wrong parser
    if(session->state != NET_IO_STATE_ACTIVE)
    {
        netio_job_deregister_channel_events(session->net_channel);
        net_channel_free_list_insert_head(session->net_channel);
    }

    session->net_channel = NULL;

done:
    NET_IO_UNLOCK_CONTEXT();
    return;
}

static void net_channel_connect(net_io_channel_t *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x, pipe = %#08x)\n", __func__, net_channel, net_channel->pipe_hndl);
    ASSERT(net_channel);
    ASSERT(net_channel->pipe_hndl);
    DBG_PRINTF_SDH_ERR("ksjung--net_io call PipeConnect\n");
    PipeConnect(net_channel->pipe_hndl);
    return;
}


//=========================================================================================================================
// Linked List Helper Routines
//=========================================================================================================================

static void net_channel_free_list_insert_head(net_io_channel_t *net_channel)
{
    DBG_VERBOSE("==>%s (chan=%08x)\n", __func__, net_channel);
    ASSERT(net_channel);

    NET_IO_LOCK_CONTEXT();
    ASSERT(!net_channel->next);

    net_channel->is_free = true;

    net_io_session_t *session = net_session_acquiring_list_remove_head(net_channel->channel_type);
    if(session)
    {
        // hand channel to the next session in acquiring state
        XASSERT(session->state == NET_IO_STATE_ACQUIRING, session->state);
        session->net_channel = net_channel;
        XASSERT(net_channel->session == NULL, (int)net_channel->session); // confirm channel is not bound to a session
        net_channel->session = session; // session now owns the channel
        net_channel->is_free = false;
        XASSERT(session->state == NET_IO_STATE_ACQUIRING, session->state);
        session->state = NET_IO_STATE_INACTIVE;

        if(session->app_notify)
        {
            session->app_notify(NET_IO_SESSION_EVENT_INACTIVE, session->user_ctxt);
        }

        if(session->ready)
        {
            net_channel_connect(session->net_channel);
        }
    }
    else
    {
        // no sessions waiting to acquire a channel -- insert channel into the free list
        switch(net_channel->channel_type)
        {
            case e_Print:
                net_channel->next = g_net_io_ctxt.print_channel_free_list;
                g_net_io_ctxt.print_channel_free_list = net_channel;
                break;
            case e_Scan:
                // singleton
                g_net_io_ctxt.scan_channel->session = NULL;
                break;
            default:
                // unsupported channel type
                XASSERT(0, net_channel->channel_type);
                break;
        }
    }
    NET_IO_UNLOCK_CONTEXT();
    return;
}

static net_io_channel_t *net_channel_free_list_remove_head(Channel_type_t channel_type)
{
    DBG_VERBOSE("==>%s (type=%d)\n", __func__, channel_type);
    NET_IO_LOCK_CONTEXT();
    net_io_channel_t *net_channel = NULL;
    switch(channel_type)
    {
        case e_Print:
            if(g_net_io_ctxt.print_channel_free_list)
            {
                net_channel = g_net_io_ctxt.print_channel_free_list;
                g_net_io_ctxt.print_channel_free_list = net_channel->next;
                net_channel->next = NULL;
                ASSERT(net_channel->is_free);
                net_channel->is_free = false;
            }
            break;
        case e_Scan:
            if(!g_net_io_ctxt.scan_channel)
            {
                // singleton
                g_net_io_ctxt.scan_channel = net_channel_create(e_Scan);
            }
            // verify channel not in use
            ASSERT(g_net_io_ctxt.scan_channel);
            if(g_net_io_ctxt.scan_channel && !g_net_io_ctxt.scan_channel->session)
            {
                net_channel = g_net_io_ctxt.scan_channel;
                ASSERT(net_channel->is_free);
                net_channel->is_free = false;
            }
            break;
        default:
            // unsupported channel type
            XASSERT(0, channel_type);
            break;
    }
    NET_IO_UNLOCK_CONTEXT();
    DBG_VERBOSE("<==%s (chan=%08x)\n", __func__, net_channel);
    return net_channel;
}

static void net_session_acquiring_list_insert_tail(net_io_session_t *session)
{
    ASSERT(session);
    DBG_VERBOSE("==>%s (session=%08x type=%d)\n", __func__, session, session->channel_type);

    NET_IO_LOCK_CONTEXT();

    session->next = NULL;
    net_io_acquiring_list_t *acquiring_list = NULL;
    switch(session->channel_type)
    {
        case e_Print:
            acquiring_list = &g_net_io_ctxt.print_session_acquiring_list;
            break;
        case e_Scan:
            acquiring_list = &g_net_io_ctxt.scan_session_acquiring_list;
            break;
        default:
            // unsupported channel type
            XASSERT(0, session->channel_type);
            goto done;
            break;
    }

    ASSERT(acquiring_list);
    ASSERT(acquiring_list->tail);
    *acquiring_list->tail = session;
    acquiring_list->tail = &session->next;

done:
    NET_IO_UNLOCK_CONTEXT();
    return;
}

static net_io_session_t *net_session_acquiring_list_remove_head(Channel_type_t channel_type)
{
    DBG_VERBOSE("==>%s (type=%d)\n", __func__, channel_type);

    NET_IO_LOCK_CONTEXT();

    net_io_session_t *session = NULL;
    net_io_acquiring_list_t *acquiring_list = NULL;

    switch(channel_type)
    {
        case e_Print:
            acquiring_list = &g_net_io_ctxt.print_session_acquiring_list;
            break;
        case e_Scan:
            acquiring_list = &g_net_io_ctxt.scan_session_acquiring_list;
            break;
        default:
            // unsupported channel type
            XASSERT(0, channel_type);
            goto done;
            break;
    }

    ASSERT(acquiring_list);
    if(acquiring_list->head)
    {
        session = acquiring_list->head;
        acquiring_list->head = session->next;
        session->next = NULL;
        if(acquiring_list->tail == &session->next)
        {
            acquiring_list->tail = &acquiring_list->head;
        }
    }

done:
    NET_IO_UNLOCK_CONTEXT();
    DBG_VERBOSE("<==%s (session=%08x)\n", __func__, session);
    return session;
}

static void net_session_acquiring_list_remove(net_io_session_t *session)
{
    ASSERT(session);
    DBG_VERBOSE("==>%s (session=%08x type=%d)\n", __func__, session, session->channel_type);

    NET_IO_LOCK_CONTEXT();

    net_io_acquiring_list_t *acquiring_list = NULL;

    switch(session->channel_type)
    {
        case e_Print:
            acquiring_list = &g_net_io_ctxt.print_session_acquiring_list;
            break;
        case e_Scan:
            acquiring_list = &g_net_io_ctxt.scan_session_acquiring_list;
            break;
        default:
            // unsupported channel type
            XASSERT(0, session->channel_type);
            goto done;
            break;
    }

    ASSERT(acquiring_list);
    net_io_session_t *cur_session = acquiring_list->head;
    net_io_session_t **prev = &acquiring_list->head;
    while(cur_session)
    {
        if(cur_session == session)
        {
            // remove the item from the list
            *prev = cur_session->next;
            cur_session->next = NULL;
            if(acquiring_list->tail == &cur_session->next)
            {
               acquiring_list->tail = prev;
            }
            break;
        }
        prev = &cur_session->next;
        cur_session = cur_session->next;
    }

done:
    NET_IO_UNLOCK_CONTEXT();
    return;
}

void* net_io_debug_get_channel_from_session(net_io_session_hndl_t session_hndl)
{
    net_io_session_t *session = NULL;
    void* retval = NULL;

    NET_IO_LOCK_CONTEXT();
    if (session_hndl != NULL)
    {
        session = (net_io_session_t *)session_hndl;
        retval = session->net_channel;
    }
    NET_IO_UNLOCK_CONTEXT();
    return retval;
}

void* net_io_debug_get_pipe_from_session(net_io_session_hndl_t session_hndl)
{
    net_io_session_t *session = NULL;
    void* retval = NULL;

    NET_IO_LOCK_CONTEXT();
    if (session_hndl != NULL)
    {
        session = (net_io_session_t *)session_hndl;
        if (session->net_channel != NULL)
        {
            retval = session->net_channel->pipe_hndl;
        }
    }
    NET_IO_UNLOCK_CONTEXT();
    return retval;
}



//=========================================================================================================================
// Misc. Helper Routines
//=========================================================================================================================

pthread_mutex_t *net_io_get_mtx(void)
{
    return &g_net_io_ctxt.mtx;
}
