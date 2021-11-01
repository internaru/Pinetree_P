/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_io_api.h
 * 
 * @brief Network I/O layer
 *  
 *  
 * PURPOSE: 
 *  
 * The purpose of the Network I/O layer is to provide an abstracted mechanism for 
 * processing jobs originating over the network.  This API supports queuing of incoming 
 * network jobs, piping job data to and from various printer subsystems, and notifying of 
 * job status and job completion. This API should be reserved for piping of raw job data 
 * such as PDL data. When possible, any extraneous XML, SOAP, or other protocol data
 * should be filtered off by higher-layer network applications such as IPP, WS-Print & 
 * WS-Scan, etc. 
 *  
 * The Network I/O layer maps the 4.4 BSD send and recv socket interface to the internal 
 * I/O device pipes defined in io_device.h. Further, it abstracts details of the 
 * underlying I/O device layer that pertain specifically to network I/O (vs. USB, etc). 
 * 
 * The interactions between network connection endpoints and the I/O device pipes are 
 * relatively complex compared to USB or parallel connection endpoints, due in part to the 
 * transient nature of network connections. Unlike USB or parallel endpoints that maintain 
 * an exclusive connection to the same host across multiple jobs, a network endpoint is 
 * temporary, typically existing only for the life of a particular job and switching from 
 * one host to another between jobs. 
 * 
 * Additionally, processing "back-to-back" jobs, or job pipelining, is more challenging 
 * with network connections because consecutive jobs from different hosts to the same 
 * network port are delivered over different connections, and the connection lifetimes 
 * must therefore overlap to accomodate pipelining. 
 * 
 * The Network I/O module allows the solutions to these problems to be leveraged across 
 * different network protocols and application types. Although the API is designed to map 
 * directly to the TCP send and recv socket functions, the overlying network application 
 * may act as a filter to process higher-layer protocol data such as XML-encoded 
 * transactions, and also to compress, decompress, or otherwise transform raw PDL data.
 * For example, the WS-Print network application must process XML-encoded SOAP remote 
 * procedure calls in the network data stream prior to extracting and piping raw PDL job 
 * data over the I/O Device pipes. On the other end of the spectrum, print job data sent 
 * to raw port 9100 is forwarded unfiltered directly from the BSD socket to the I/O device 
 * pipe. 
 *  
 *  
 * USAGE MODEL: 
 *  
 * In a typical use case, a network application must first call net_io_open() to create a 
 * new I/O session and register session callbacks. The lifetime of an I/O session maps 
 * roughly to the lifetime of the corresponding job. Although it is acceptable to send
 * multiple jobs within the same I/O session, doing so will preclude job pipelining 
 * resulting in reduced job throughput. There is no hard limit to the number of sessions 
 * that can be opened simultaneously. Jobs will be queued in the order in which their
 * corresponding sessions are opened. 
 *  
 * A network connection ma illustratesy encapsulate multiple network I/O sessions, e.g. as 
 * in the case of several non-contiguous IPP print jobs delivered over a persistent HTTP 
 * connection. However, it is not possible for a single I/O session to span multiple 
 * network connections. 
 *  
 * Once a session is opened, the Network I/O layer will attempt to acquire a network I/O 
 * channel from a channel pool, potentially delaying until an existing session closes 
 * and a free I/O channel becomes available. The number of available channels is limited 
 * by channel type via the NET_IO_CHANNEL_POOL_MAX_SIZE define. To maximize job 
 * throughput, a system should allocate one channel per job pipeline stage so that
 * individual stages can function in parallel. 
 *  
 * For example, a printer with distinct processing, printing, and finishing stages should
 * allocate three print pipe channels. A pending job can then enter the print 'processing' 
 * stage while earlier jobs are still active in the 'printing' and 'finishing' stages on 
 * other channels. In this way, job throughput becomes a function of the latency of the 
 * longest stage rather than the latecy of all stages combined, or in some cases even 
 * greater latency, e.g. due to the print engine spooling down between contiguous jobs to
 * due inactivity at the 'printing' stage. 
 *  
 * After creating a new session, the network application must wait for acquisition of an 
 * I/O channel as signaled by the NET_IO_SESSION_EVENT_INACTIVE event. When possible, a
 * network application should gate the flow of data from the peer until after receiving 
 * this event. For example, an application might wait to accept an incoming TCP connection
 * until after acquiring the channel, thereby maintaining a smaller TCP window and 
 * conserving socket recv buffer storage. In the case of a persistent TCP connection for
 * HTTP, an application might delay a "100 continue" response until after acquiring the 
 * channel. Freeing resources in this way can increase performance of an existing job and 
 * thereby speed up channel acquision for the pending I/O session. 
 *  
 * Following reception of the NET_IO_SESSION_EVENT_INACTIVE event, the application must 
 * call net_io_ready() once it is ready to start sending raw I/O data over the newly 
 * acquired I/O channel. If possible a network application should delay this call until 
 * after I/O data is known to be pending in the TCP recv buffer, thus avoiding premature 
 * locking of channel resources which could block another channel from being scheduled.
 * For example, a network application might accept an incoming TCP connection after 
 * receiving NET_IO_SESSION_EVENT_INACTIVE but then use the select() socket API to wait 
 * for recv data to become available before calling net_io_ready(). 
 *  
 * If the network application desires to receive out-of-band job events, it must call 
 * net_io_job_register_event_cb() once for each job, after receiving 
 * NET_IO_SESSION_EVENT_INACTIVE but before calling net_io_ready(). In the first callback 
 * (signaling job started), a job handle will be provided which can be used to acquire 
 * job-specific information and control via the net_io_job_x routines. These routines 
 * might be used when job status is to be transmitted out-of-band over a different network 
 * connection (as with IPP or WSD), rather than in-band as reverse channel data over the 
 * same TCP connection as incoming data (as with raw port printing). 
 * 
 * Upon calling net_io_ready(), the session's channel becomes a candidate for scheduling 
 * by the system I/O connection manager. When the required resources become available, 
 * they are locked by the system connection manager and the channel's I/O pipe is handed 
 * to a PDL parser to read, write, and otherwise process the job data. At this point the 
 * network application will receive a NET_IO_SESSION_EVENT_ACTIVE event to signal that the
 * channel's data is being actively processed. Following this event the parser will call 
 * the user's registered net_io_recv_func_t callback to receive job data over the channel,
 * and will send NET_IO_SESSION_EVENT_HAVE_SEND_DATA events to request to send data to the
 * network application. 
 *  
 * When the network application determines that it is done transmitting and receiving job 
 * data, it must call net_io_close() to terminate the session and free the channel for use 
 * by other pending network connections. 
 *  
 * The network application may receive a NET_IO_SESSION_EVENT_INACTIVE event at any time 
 * following the the NET_IO_SESSION_EVENT_ACTIVE event, even before all job data has been 
 * processed. Typically this will happen after a timeout during the callback to the 
 * network application's registered net_io_recv_func_t. In this case, the network 
 * application should again wait until I/O data is known to be pending in the TCP recv 
 * buffer and then call net_io_ready(), repeating this process until all job data has been 
 * consumed by the parser. 
 *  
 *  
 * BLOCK DIAGRAM:
 *  
 * The following diagram illustrates the position of the Network I/O API layer in the 
 * system relative to other components: 
 *  
 * *                                ----------------------
 * *                               | Network Applications | 
 * *                                ---------------------- 
 * *                                         |        
 * *                               ----------------------- 
 * *                              | ** Network I/O API ** | 
 * *                               ----------------------- 
 * *                                         |
 * *                                 ------------------ 
 * *                                | I/O Device Pipes | 
 * *                                 ------------------ 
 * *                                         |
 * *                                --------------------
 * *                               | Connection Manager | 
 * *                                -------------------- 
 * *                                         |
 * *                                    ----------- 
 * *                                   |  Parsers  | 
 * *                                    ----------- 
 * *                                         |
 * *                                  --------------- 
 * *                                 | Print Job API | 
 * *                                  --------------- 
 * *                                         |
 * *                               ---------------------
 * *                              | System HW Resources | 
 * *                               --------------------- 
 */

#ifndef NET_IO_API_H
#define NET_IO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io_device.h"

/**
 * @name Network I/O return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
#define NET_IO_SUCCESS          OK
#define NET_IO_FATAL_ERROR      -11  ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_IO_LOW_MEM          -12  ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_IO_NOT_PERMITTED    -13  ///< The operation is not permitted at this time.
//@}

/**
 * @brief Network I/O session events
 */
typedef enum
{
    /**
     * The Network I/O session is now inactive.  The Network I/O layer will not accept incoming data or
     * attempt to send reverse data on this channel without a subsequent call to net_io_ready().
     * 
     * There are two possible triggers for this event: 1) A network I/O channel has been newly acquired
     * following a call to net_io_open(), and 2) the network I/O session has transitioned from an active
     * state back to an inactive state, possibly due to the network application returning an EOF,
     * timeout, or error condition via the net_io_recv_func_t callback.
     */
    NET_IO_SESSION_EVENT_INACTIVE,

    /**
     * The network I/O session has been scheduled by the system I/O connection manager, and an I/O
     * parser will proceed to actively send and/or receive data over the network I/O channel.
     * 
     * When the I/O session is active, the I/O parser reads data from the network application via the
     * registered net_io_recv_func_t callback. The network application will be notified of reverse
     * channel data from the I/O parser via the NET_IO_SESSION_EVENT_HAVE_SEND_DATA event, after which
     * the application must fetch the data via repeated calls to net_io_get_send_data(). The network
     * application can assume no data will be sent or received by the I/O parser until after the
     * NET_IO_SESSION_EVENT_ACTIVE event notification callback has returned.
     */
    NET_IO_SESSION_EVENT_ACTIVE,

    /**
     * Reverse channel data is pending for the I/O session. To acquire the data the user should
     * repeatedly call net_io_get_send_data() in a *different* thread context from that of the event
     * notification callback, until an empty send buffer is returned. A new
     * NET_IO_SESSION_EVENT_HAVE_SEND_DATA event will generated only after net_io_get_send_data()
     * returns an empty send buffer.
     */
    NET_IO_SESSION_EVENT_HAVE_SEND_DATA,

} net_io_session_event_t;

/**
 * @brief Network I/O job events
 * 
 * The first five job events map to the corresponding job-states defined in section 4.3.7 of RFC
 * 2911. A NET_IO_JOB_EVENT_STARTED event will always be the first notification sent for a job and
 * is the first opportunity the application will have to acquire the job handle required for
 * subsequent calls to the net_io_job_x routines. The job will always complete with exactly one of the
 * NET_IO_JOB_EVENT_COMPLETED, NET_IO_JOB_EVENT_ABORTED, NET_IO_JOB_EVENT_CANCELED events. Upon
 * receiving one of these three events, any final net_io_job_x routines must be called from the
 * context of the event notification callback before returning from the callback, at which point the
 * job handle becomes invalid. The NET_IO_JOB_EVENT_SHEET_COMPLETED event does not correspond to a
 * job state, but functions as a trigger to allow the application to request progress of the
 * koctect, sheet, and page counters.
 */
typedef enum 
{
    /**
     * Maps to the 'processing' job-state attribute defined in section 4.3.7 of RFC 2911. This will
     * always be the first event sent for a job and is the first opportunity the application will have
     * to acquire the job handle required for subsequent calls to the net_io_job_x routines.
     */
    NET_IO_JOB_EVENT_STARTED,

    /**
     * Maps to the 'processing-stopped' job-state attribute defined in section 4.3.7 of RFC 2911. The
     * application will be notified with another NET_IO_JOB_EVENT_STARTED notification when the job
     * continues processing. The job may also be canceled or aborted while still in the paused state,
     * resulting in either a NET_IO_JOB_EVENT_CANCELED or NET_IO_JOB_EVENT_ABORTED notification,
     * respectively.
     */
    NET_IO_JOB_EVENT_PAUSED,

    /**
     * Maps to the 'completed' job-state attribute defined in section 4.3.7 of RFC 2911. This event
     * indicates that the job has completed, possibly with warnings/errors, and that all job media
     * sheets have been stacked in the output bin. Upon receiving this event the application may
     * conclude that all koctect, sheet, and page counters have reached their final values. Any final
     * calls to the net_io_job_x routines must be done from the context of this notification callback
     * before returning from the callback, at which time the job handle becomes invalid.
     */
    NET_IO_JOB_EVENT_COMPLETED,

    /**
     * Maps to the 'aborted' job-state attribute defined in section 4.3.7 of RFC 2911. This event
     * indicates that the job has been prematurely aborted by the system. More detail is available via
     * net_io_job_get_state_reason(). Upon receiving this event the application may conclude that all
     * koctect, sheet, and page counters have reached their final values. Any final calls to the
     * net_io_job_x routines must be done from the context of this notification callback before returning
     * from the callback, at which time the job handle becomes invalid.
     */
    NET_IO_JOB_EVENT_ABORTED,

    /**
     * Maps to the 'canceled' job-state attribute defined in section 4.3.7 of RFC 2911. This event
     * indicates that the job has been canceled either by the user via a call to net_io_job_cancel() or
     * at the device via a system job manager cancel request, e.g. as triggered by the control panel
     * cancel button. If the event is received following a call to net_io_job_cancel() then the network
     * application should assume that the cancel was triggered by the user and not by a system job
     * manager cancel request. Upon receiving this event the application may conclude that all koctect,
     * sheet, and page counters have reached their final values. Any final calls to the net_io_job_x
     * routines must be done from the context of this notification callback before returning from the
     * callback, at which time the job handle becomes invalid.
     */
    NET_IO_JOB_EVENT_CANCELED,

    /**
     * Indicates that a media sheet has finished processing and has been stacked in the output bin. The
     * network application may wish to use this event as an opportunity to request progress of the
     * koctect, page, and sheet counters via the net_io_job_get_koctets_processed(),
     * net_io_job_get_pages_completed(), net_io_job_get_sheets_completed() routines, respectively. It is
     * okay for the application to call these routines from the context of the event notification
     * callback routine.
     */
    NET_IO_JOB_EVENT_SHEET_COMPLETED

} net_io_job_event_t;

/**
 * @brief Job state reason
 * 
 * These values map to corresponding job-state-reason attribute values defined in section 4.3.8 of
 * RFC 2911. Only a subset of the job-state-reason attribute values are represented here, as some of
 * the values are not applicable in the context of the Network I/O layer. The job state reasons
 * provide additional information about the job while in a particular state, and some state reasons
 * apply only to certain job states.
 */
typedef enum 
{
    /**
     * No additional information is available for the current job state.
     */
    NET_IO_JOB_STATE_REASON_NONE,

    /**
     * The job was aborted due to an unsupported document format requested in a PJL language hint. This
     * value is intended to map to the unsupported-document-format value of the job-state-reason
     * attribute as defined in section 4.3.8 of RFC 2911.
     */
    NET_IO_JOB_STATE_REASON_UNSUPPORTED_PDL,

    /**
     * The job was aborted due to an error/corruption detected in the job data while parsing. This value
     * is intended to map to the document-format-error value of the job-state-reason attribute as
     * defined in section 4.3.8 of RFC 2911.
     */
    NET_IO_JOB_STATE_REASON_PARSER_ERROR

} net_io_job_state_reason_t;

/**
 * @brief Handle to a Network I/O session
 * 
 * Unique handle generated by net_io_open() used to reference a particular Network I/O session in
 * successive API calls.
 */
typedef struct net_io_session_s *net_io_session_hndl_t;
#define NET_IO_INVALID_SESSION_HANDLE   0

/**
 * @brief Handle to a job
 * 
 * Unique handle provided in the NET_IO_JOB_EVENT_STARTED notification callback used to reference a
 * particular job in subsequent calls to the net_io_job_x routines.
 */
typedef struct net_io_job_s *net_io_job_hndl_t;
#define NET_IO_INVALID_JOB_HANDLE       0

/**
 * @brief Network I/O session event function pointer
 * 
 * @param event One of the session events defined by net_io_session_event_t.
 * 
 * @param[in] user_ctxt The opaque user data passed to the Network I/O layer in the call to
 *       net_io_ready().
 * 
 * This function is implemented by a network application and registered with the network I/O layer
 * via net_io_ready(). The registered function is called from the network I/O layer to notify the
 * network application of a session event.
 * 
 * The function is typically called in the context of the system I/O connection manager or I/O
 * parser thread. Therefore, processing time should be kept at a minimum. If necessary the caller
 * should signal the application via a message queue or some other mechanism and switch execution to
 * the context of an application thread.
 */
typedef void (*net_io_session_event_func_t)(net_io_session_event_t event, void *user_ctxt);

/**
 * @brief Network I/O job event function pointer
 * 
 * @param event One of the job events defined by net_io_job_event_t.
 * 
 * @param A unique per-job handle used to reference a job in successive API calls.
 * 
 * @user_ctxt The opaque user data passed to the Network I/O layer in the call to
 *       net_io_job_register_event_cb().
 * 
 * This function is implemented by a network application and registered with the network I/O layer
 * via net_io_job_register_event_cb(). The registered function is called from the network I/O layer to
 * notify the network application of a job event.
 * 
 * The function is typically called in the context of the system I/O connection manager or I/O
 * parser thread. Therefore, processing time should be kept at a minimum. If necessary the caller
 * should signal the application via a message queue or some other mechanism and switch execution to
 * the context of an application thread.
 */
typedef void (*net_io_job_event_func_t)(net_io_job_event_t event, net_io_job_hndl_t job_hndl, void *user_ctxt);

/**
 * @brief Network I/O receive data function pointer
 * 
 * @param[in] buf Pointer to a buffer owned by the network I/O layer into which incoming data is to
 *       be copied.
 * 
 * @param len Length in bytes of the buffer.
 * 
 * @param[in,out] timeout_msec Pointer to storage allocated by the Network I/O layer 
 *       indicating maximum time in milliseconds that the function should block waiting
 *       for data. A value of -1 means 'infinite', in which case the function should block
 *       until 'len' bytes of data have been copied into 'buf' or until an EOF or error
 *       condition has occurred.
 *  
 *       The function should return immediately after copying 'len' bytes of data into
 *       'buf' or to report an EOF or error condition.
 *  
 *       If the timeout expires before any data is acquired, or before an EOF or
 *       error condition occurs, the user must signal a timeout by resetting the value
 *       pointed to by timeout_msec to 0 and then return a value of -1 to indicate an
 *       error condition.
 * 
 *       If the timeout expires after partial data has been acquired, the user should
 *       simply return the number of bytes received and NOT signal a timeout.
 * 
 *       If only partial data has been acquired when an EOF condition is detected, the
 *       user should immediately return the number of bytes received and NOT wait for the
 *       timeout to expire and NOT signal a timeout. Note that in this case the sm_job API
 *       is not aware of the EOF condition and will likely call back again, at which point
 *       the user can report the EOF by returning 0. 
 * 
 * @param[in] user_ctxt The opaque user data passed to the Network I/O layer in the call to
 *       net_io_open().
 * 
 * @return Standard BSD socket recv return value.
 * 
 * @retval >0 Number of bytes actually received.
 * 
 * @retval 0 EOF condition detected. No bytes were copied to the receive buffer and no more incoming
 *         data is available for this stream.
 * 
 * @retval -1 An error or timeout occurred. If timeout, then the value pointed to by timeout_msec
 *         must also be reset to 0 to distinguish from a general error condition. Otherwise, the
 *         Network I/O layer assumes an unrecoverable error has occurred resulting in premature
 *         termination of the incoming data stream.
 * 
 * Pointer to a callback function to be implemented by the overlying network application. The
 * Network I/O layer repeatedly calls this function to access the incoming data stream sourced by
 * the network application.
 */
typedef int32_t (*net_io_recv_func_t)(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt);

/**
 * @brief Network I/O session callbacks
 * 
 * This dispatch table must be populated by a network application and passed to the Network I/O
 * layer in the call to net_io_open().
 */
typedef struct 
{
    net_io_recv_func_t          io_recv; ///< Used to access the data I/O stream sourced by the application. REQUIRED.
    net_io_session_event_func_t io_notify; ///< Used to notify the application of network I/O session events. REQUIRED.
} net_io_func_table_t;

/**
 * @brief Initialize the Network I/O layer.
 *
 * @retval NET_IO_SUCCESS Initialization was successful.
 * 
 * @retval NET_IO_LOW_MEM Initialization failed because system memory is low. A future attempt may
 *         succeed.
 * 
 * @retval NET_IO_FATAL_ERROR Initialization failed because an unrecoverable error has occurred.
 * 
 * This routine should be called exactly once following a system startup.
 */
int32_t net_io_init(void);

/**
 * @brief Initiates an I/O session with the Network I/O layer.
 * 
 * @param channel_type Specifies the class of the I/O channel, e.g. print, scan, fax, etc.
 * 
 * @param[in] func_table Pointer to a Network I/O callback table that has been allocated and
 *       populated by the caller (i.e. the network application). Upon return, the caller may free
 *       memory allocated for this table.
 * 
 * @param[in] user_ctxt Opaque user data passed back to the network application during calls to the
 *       application's registered callback functions.
 * 
 * @param[out] session_hndl Pointer to storage allocated by the caller used to pass back a unique handle
 *       to the I/O session. The handle is used to reference the specific I/O session in successive
 *       API calls.
 * 
 * @retval NET_IO_SUCCESS A new session has been successfully created and initialized.
 * 
 * @retval NET_IO_LOW_MEM The request failed because system memory is low. A future attempt may
 *         succeed.
 * 
 * When possible, a network application should call this routine prior to accepting an incoming
 * network connection known to contain I/O data. In addition, the application should accept the
 * incoming TCP connection only after having received a NET_IO_SESSION_EVENT_INACTIVE event,
 * indicating that an I/O channel has been acquired for the session.  In this way, the TCP recv
 * buffer can remain at its pre-accept size to prevent unnecessary buffering of recv data for
 * potentially numerous pending Network I/O sessions.
 */
int32_t net_io_open(Channel_type_t channel_type, net_io_func_table_t *func_table,
                    void *user_ctxt, net_io_session_hndl_t *session_hndl);

/**
 * @brief Signals the Network I/O layer that the network application is ready to send and/or receive
 *        data over a network I/O channel.
 * 
 * @param session_hndl The unique session handle generated by net_io_open().
 * 
 * When possible, a network application should call this routine only after a TCP connection has
 * been accepted *and* I/O data is known to be pending in the TCP recv buffer. In this way, the
 * network I/O session will not lock channel resources unnecessarily and potentially block another
 * ready I/O channel from being scheduled by the system I/O connection manager.
 * 
 * After this routine is called, the corresponding I/O channel will be placed in the "ready" list
 * for scheduling by the system I/O connection manager. A NET_IO_SESSION_EVENT_ACTIVE event will be
 * generated to notify the network application when the channel has been scheduled, prior to
 * sending/receiving data over the I/O channel.
 * 
 * If this routine is called when the I/O session is already in an active state (i.e. an
 * NET_IO_SESSION_EVENT_ACTIVE event has been received, but a corresponding
 * NET_IO_SESSION_EVENT_INACTIVE event has not yet been received) then the request will be ignored.
 * 
 * The function must be called from a different thread context than the
 * NET_IO_SESSION_EVENT_INACTIVE event notification callback.
 */
void net_io_ready(net_io_session_hndl_t session_hndl);

/**
 * @brief Terminates the Network I/O session.
 * 
 * @param session_hndl The unique session handle generated by net_io_open().
 * 
 * Must be called once for every corresponding call to net_io_open(). Note that an I/O session does
 * not implicitly terminate when the session transitions from an 'active' to 'inactive' state. An
 * inactive session continues to lock the underlying I/O channel, which could potentially block a
 * net_io_open() request from acquiring an I/O channel for a different TCP connection.
 * 
 * This call can be made anytime following a corresponding call to net_io_open(), regardless of
 * whether the session is currently active or inactive.
 */
void net_io_close(net_io_session_hndl_t session_hndl);

/**
 * @brief Accept reverse I/O channel data from the Network I/O layer.
 * 
 * @param session_hndl The unique session handle generated by net_io_open().
 * 
 * @param[out] len Pointer to storage allocated by the caller used to pass back the number of bytes
 *       of data contained in the buffer.
 * 
 * @return Pointer to the data buffer, or NULL if no more data to send. Caller MUST FREE BUFFER when
 *         done sending the data.
 * 
 * Following a NET_IO_SESSION_EVENT_HAVE_SEND_DATA event notification, this function should be
 * called repeatedly until an empty buffer is returned. The next NET_IO_SESSION_EVENT_HAVE_SEND_DATA
 * event will be generated only after this function has returned an empty send buffer.
 * 
 * The caller is expected to transmit the data over the TCP connection associated with this I/O
 * session, possibly further encapsulating, transforming, or otherwise filtering the outgoing data
 * stream. The buffer must be freed by the caller once the data has been sent.
 * 
 * In certain cases the caller may choose not to transmit the data at all, e.g. as when the host is
 * not designed to process reverse channel data over the same TCP connection as the incoming data
 * stream. In such cases, pertinent status information is typically collected via the event driven
 * "observer" mechanism and then transmitted over a different TCP port. However, the network
 * application is still expected to service any NET_IO_NOTIFY_HAVE_DATA notifications via this
 * function and should simply discard the send data by immediately freeing the buffers.
 * 
 * The function must be called from a different thread context than the NET_IO_NOTIFY_HAVE_DATA
 * event notification callback.
 */
char *net_io_get_send_data(net_io_session_hndl_t session_hndl, uint32_t *len);

#ifdef HAVE_PRINT_SUPPORT
/**
 * @brief Register for notification of job events.
 * 
 * @param event_func Pointer to a function to be called when one of the net_io_job_event_t job
 *                   events have occurred.
 * 
 * @param session_hndl The unique session handle generated by net_io_open().
 * 
 * @param user_ctxt Opaque user data passed back to the network application during calls to the
 *       application's registered job event callback function.
 * 
 * @retval NET_IO_SUCCESS Job event registration completed successfully.
 * 
 * @retval NET_IO_NOT_PERMITTED Job event registration is not permitted following a call to
 * net_io_ready().
 * 
 * This function must be called prior to using any of the net_io_job_x routines. The net_io_job_x
 * routines are typically used when job status is to be transmitted out-of-band over a different
 * network connection (as with IPP or WSD), rather than as reverse channel data over the same TCP
 * connection as the incoming data (as with raw port printing).
 * 
 * The registered event_func callback is valid only for a single job even if multiple jobs are
 * processed within the same I/O session context. Thus, this function must be called once for every
 * job.
 * 
 * The function may only be called while the net I/O session is in the "inactive" state, i.e. after
 * receiving the NET_IO_SESSION_EVENT_INACTIVE event but before a subsequent call to net_io_ready().
 * Note that it is possible for more than one job to overlap within the same I/O session context.
 * Thus, the network application should register a different user_ctxt value for each new job, so
 * that overlapping job events can be mapped back to the correct job.
 * 
 * It is normal for the session to transition between the "inactive" and "active" states several
 * times for the same print job, e.g. as in the case of parsing PJL commands bracketing the actual
 * job raster data. Although the network application must call net_io_ready() each time 
 * to initiate the transition from the "inactive" to "active" state, it is only necessary
 * to register for job event notification once per job per session.  If the 
 * NET_IO_SESSION_EVENT_INACTIVE event is received after having received a 
 * NET_IO_JOB_EVENT_STARTED event, then job processing is considered complete and a new 
 * registration must be made for any subsequent job started in the context of the same I/O
 * session. 
 * 
 * The net_io_job_x routines require a job handle that is unique per job instance. The unique job
 * handle is provided to the user during job event notifications via the registered event_func
 * handler. The first event will always be NET_IO_JOB_EVENT_STARTED. This is the first opportunity
 * the user will have to acquire the unique job handle needed to use the net_io_job_x routines for
 * the job.
 * 
 * The job handle becomes invalid after receiving one of the NET_IO_JOB_EVENT_COMPLETED,
 * NET_IO_JOB_EVENT_ABORTED, or NET_IO_JOB_EVENT_CANCELED events. Upon receiving one of these three
 * events, any final net_io_job_x routines must be called from the context of the event_func
 * callback before returning from the callback.
 */
int32_t net_io_job_register_event_cb(net_io_job_event_func_t event_func, 
                                     net_io_session_hndl_t session_hndl, void *user_ctxt);

/**
 * Cancel a job that has already been started.
 * 
 * @param job_hndl The unique job handle provided in the NET_IO_JOB_EVENT_STARTED notification
 *                 callback.
 * 
 * Cancels a specific job in the pipelined job queue, no matter where it is relative to other jobs
 * in queue. Note the distinction from a system job manager cancel request that always cancels the
 * oldest job in the queue. In either case a NET_IO_JOB_EVENT_CANCELED event will be sent once the
 * job has been canceled. If the event is received following a call to this function, then the
 * network application should assume that the cancel was triggered by this function call and not by
 * a system job manager cancel request.
 */
void net_io_job_cancel(net_io_job_hndl_t job_hndl);

/**
 * @brief Acquire additional information about the job's current state.
 * 
 * @param job_hndl The unique job handle provided in the NET_IO_JOB_EVENT_STARTED notification
 *                 callback.
 * 
 * @return One of the net_io_job_state_reason_t values that further qualifies the current job state.
 * 
 * The job state reason provided by this function applies only to the state the job was in at the
 * time the call was made. To ensure the reason maps to the intended state, the function should
 * be called directly from the context of the registered job event callback routine before returning
 * from the callback. Some state reasons apply only to certain job states. The response is intended
 * to map to one of the specific job-state-reason attribute values defined in section 4.3.8 of RFC
 * 2911.
 */
net_io_job_state_reason_t net_io_job_get_state_reason(net_io_job_hndl_t job_hndl);

/**
 * @brief Get the number of kilo-octets processed thus far.
 * 
 * @param job_hndl The unique job handle provided in the NET_IO_JOB_EVENT_STARTED notification
 *                 callback.
 * 
 * @return Amount of job data processed thus far in units of 1024 octets.
 * 
 * The number of K octets is rounded up to the nearest 1024 octets. For example, a job that has
 * processed between 1 and 1024 octets inclusive will report a value of 1. Job data is considered
 * "processed" once it has been consumed by the I/O parser.
 */
uint32_t net_io_job_get_koctets_processed(net_io_job_hndl_t job_hndl);

/**
 * @brief Get the number of pages completed for the job thus far.
 * 
 * @param job_hndl The unique job handle provided in the NET_IO_JOB_EVENT_STARTED notification
 *                 callback.
 * 
 * @return Number of pages completed for the job thus far.
 * 
 * Returns the number of pages, or impressions, completed for the job thus far including marking
 * onto a media sheet and stacking in the output bin. Total pages for a document is defined by the
 * number of discrete impressions contained in the original source document data, regardless of how
 * the impressions are actually rendered on a media sheet. For example, a media sheet marked on
 * both sides counts as at least two pages. Likewise, a media sheet marked with four discrete
 * impressions in an N-up arrangement counts as four pages, but only if the impressions existed in
 * the source data as discrete impressions not already rendered by host software into an N-up
 * arrangement.
 */
uint32_t net_io_job_get_pages_completed(net_io_job_hndl_t job_hndl);

/**
 * @brief Get the number of physical media sheets completed for the job thus far.
 * 
 * @param job_hndl The unique job handle provided in the NET_IO_JOB_EVENT_STARTED notification
 *                 callback.
 * 
 * @return Number of media sheets completed for the job thus far.
 * 
 * A media sheet is counted as "completed" only after marking is done on one or both sides and the
 * sheet has been stacked in the output bin.
 */
uint32_t net_io_job_get_sheets_completed(net_io_job_hndl_t job_hndl);
#endif //ifdef HAVE_PRINT_SUPPORT

/**
 * @brief Debug routine to retrieve the channel associated with a session.
 * 
 * @param session_hndl The unique session handle generated by net_io_open(). 
 *  
 * @return void* containing the channel.  This should be treated as opaque.
 *  
 * This routine is intended to obtain the value only for debug logging and trace. 
 * Since this is intended only for debug output trace and logging the routine returns 
 * the channel as an opaque void pointer. 
 */
void* net_io_debug_get_channel_from_session(net_io_session_hndl_t session_hndl);

/**
 * @brief Debug routine to retrieve the pipe associated with a session.
 * 
 * @param session_hndl The unique session handle generated by net_io_open(). 
 *  
 * @return void* containing the pipe.  This should be treated as opaque.
 *  
 * This routine is intended to obtain the value only for debug logging and trace. 
 * Since this is intended only for debug output trace and logging the routine returns 
 * the pipe as an opaque void pointer. 
 */
void* net_io_debug_get_pipe_from_session(net_io_session_hndl_t session_hndl);

#ifdef __cplusplus
}
#endif

#endif // NET_IO_API_H
