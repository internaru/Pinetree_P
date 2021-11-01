/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_IO_H
#define NET_IO_H

typedef enum
{
    /* Network I/O session is waiting for a free network I/O channel to become available in the free
     * channel pool.
     */
    NET_IO_STATE_ACQUIRING,

    /* Session still exists but channel has been disconnected by I/O connection manager.
     */
    NET_IO_STATE_INACTIVE,

    /* Channel has been scheduled by the I/O connection manager and is actively sending/receiving
     * data.
     */
    NET_IO_STATE_ACTIVE,

    // add new states above this line
    NET_IO_NUM_STATES
} net_io_state_t;

typedef struct net_io_send_buf_node_s
{
    struct net_io_send_buf_node_s *next; // linked list
    char        *buf;
    uint32_t    len;
} net_io_send_buf_node_t;

struct net_io_session_s;

typedef struct net_io_channel_s
{
    struct net_io_channel_s *next; // linked list
    struct net_io_session_s *session;
    Channel_type_t          channel_type;
    void                    *pipe_hndl; // IOReg *pipe = (IOReg *)(session->net_channel->pipe_hndl);
    net_io_send_buf_node_t  *send_queue;
    net_io_send_buf_node_t  **send_queue_tail;
    bool                    send_queue_emptied; // lags send_queue being set to NULL by one iteration

    /* is_free - True if channel is in the "free" pool/list.
     * 
     * It is valid for is_free to be false while not attached to a session (session ptr is
     * NULL). For example, when net_io_close() is called on on an active channel, the session
     * is freed immediately but the channel cannot be released until a corresponding e_close
     * from connection manager.
     */
    bool                    is_free; 

// TODO create observer objects during net_channel_create(), but dont attach until net_io_job_register_event_cb()
// then detach when receive e_close -- guaranteed that jobstart, jobcancel, etc. will happen prior to e_close, if
// going to happen at all.  Otherwise we still own the job ctxt and need to free.  Else job has started and we are
// guaranteed to get a jobcancel or jobdone to use to free the job ctxt. 
    Observer_t              *jobstart_observer;
    Observer_t              *pageend_observer;
    Observer_t              *jobend_observer;
    Observer_t              *jobcancel_observer;
    bool                    observers_attached; // true if observers have been attached
} net_io_channel_t;

struct net_io_job_s;

typedef struct net_io_session_s
{
    struct net_io_session_s *next; // linked list
    Channel_type_t          channel_type;
    net_io_state_t          state;

    /* This flag is set by calling net_io_ready() anytime after calling net_io_open(). Typically used to
     * signal the I/O connection manager that data is known to be pending in the TCP recv buf (note that
     * existence of TCP connection alone does not imply "ready to send/recv data"). However this is not
     * required. Must be set to "true" after opening the session or after transitioning from "active"
     * state to "inactive" state to place the session into the I/O connection manager's "ready" list for
     * resource scheduling.
     */
    bool                    ready; 

    net_io_channel_t        *net_channel;
    struct net_io_job_s     *cur_job; // set on user cb registration, reset after job processing complete (i.e. job started event and subsequent inactive -- NOT the same as job completion)
    net_io_recv_func_t      app_recv;
    net_io_session_event_func_t app_notify;
    void                    *user_ctxt;
    net_io_job_event_func_t job_event_func;
    void                    *job_event_user_ctxt;


    // signals no more PDL/scan data should be transferred to/from the printer, e.g. on job cancel
    bool        cancel_data; 

} net_io_session_t;


// local public routines (internal to netio module)
void netio_job_deregister_channel_events(net_io_channel_t *net_channel);
void netio_job_channel_init(net_io_channel_t *net_channel);
void netio_job_detach(net_io_session_t *session); // detach job from session, if any
void netio_job_report_octets_processed(net_io_channel_t *net_channel, uint32_t octets);
pthread_mutex_t *net_io_get_mtx(void);

#endif // NET_IO_H
