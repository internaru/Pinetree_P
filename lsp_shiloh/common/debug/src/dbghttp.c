/* ==============================================================================
* (c) Copyright 2006-2010 Marvell International Ltd. 
* All Rights Reserved
*
*                             Marvell Confidential
*
* The purpose of the "Console Operations over EWS" is to hijack the serial port
* debugger input and output, that would normally come/go to the RS232 uart, and
* redirect it to the Embedded WEB Server (EWS).  Since EWS is supported on both
* the LAN and USB Device interfaces, this gives the user the option of using
* either physical interface to transmit HTTP 1.1 POST and GET transactions to:
*
*                             <IP Address>/console
*
* To utilize this capability, we use the http_resource_<function> layer.  We
* register for the /console and provide a callback address to the http_resource
* manager so that it can notify us of the following events:
*
                              HTTP_RESOURCE_HEADER_DATA
                              HTTP_RESOURCE_BODY_DATA
                              HTTP_RESOURCE_BODY_DATA_END
                              HTTP_RESOURCE_CLOSED
*
* Currently, we only support the first three events as once activated, we never
* close the HTTP resource.  We receive only one unique HEADER_DATA and BODY_DATA_
* END event for each POST or GET transaction.  However, we may receive multiple
* BODY_DATA events for either transaction.  Since the status of POST or GET only
* appears in the HEADER_DATA event, we keep this, as-well-as the unique request
* number in a local, static structure so that we know what we are processing and
* who we are processing it for.  If another request comes in, while we are in the
* middle of processing a current request sequence, we will reject it with:
*
                              HTTP_409_CONFLICT
*
* All error conditions must be reported to the http_resource manager during the
* BODY_DATA_END phase of the transaction, rather success or failure.  It should
* be noted that we do not touch any input/output data, modify it, or add/delete
* characters, etc.  All data must appear as it would on the serial debugger.
*
* Two threadX threads are used to facilitate these operations.  One that is in
* constant communication with the http_resource manager, and the other to actually
* execute the serial debugger command.  Since many serial commands can take an
* extended period of time to execute, we must allow for, and service periodic
* GET requests to stream data to the host.  This also allows us to queue, up to
* ten deep, multiple POST requests as long as they are recieved in proper order.
*
* This HTTP debug service is always available, in both RELEASE and DEBUG builds,
* and will be activated at the first receipt of and POST or GET transaction to
* /console.  At that point, the serial port is non-operational until an nRESET,
* power-on, or telnet transaction occurs.  This will reset the console device
* back to the default RS232 serial uart.
*
*  ==============================================================================
*/
#include <string.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "memAPI.h"
#include "ATypes.h"
#include "lassert.h"
#include "sys_init_api.h"
#include "http_api.h"
#include "dbg_api.h"

// Defines the circular get buffer and MALLOC'd
// http_resource buffer size for GET operations
#define DBG_GET_MSG_SIZE (16 * KILO)

typedef struct {
    pthread_mutex_t mutex;
    uint32_t wrapped;
    uint8_t *ptr;
    uint8_t  str[DBG_GET_MSG_SIZE];
} DBG_GET_MSG;

typedef struct {
    uint8_t *ptr;
    uint8_t  str[126]; /* Used to be MAX_SERIAL_DBG_MSG, which used to be 126 */
} DBG_POST_MSG;

typedef struct {
    uint32_t state;
    uint32_t request;
} DBG_HTTP_STATE;

typedef struct {
    uint32_t request;
    uint32_t event;
    uint32_t length;
    void    *ptr;
} DBG_HTTP_MESSAGE;

// Defines for request type.  NULL is used for type 0...
#define GET  1
#define POST 2

#include "logger.h"
#define DBG_PRFX            "\nCONSOLE: "
#define LOGGER_SUBMODULE    0x00080000
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE

// Debug thread stack, message, and queue sizes and priority declarations
#define DBG_HTTP_STACK_SIZE    POSIX_MIN_STACK_SIZE
#define DBG_HTTP_CMDSTACK_SIZE POSIX_MIN_STACK_SIZE
#define DBG_HTTP_MESSAGE_SIZE   4
#define DBG_HTTP_QUEUE_SIZE    10
#define DBG_HTTP_TASK_PRI      THR_PRI_NORMAL

static DBG_GET_MSG getHttpMsg;
static DBG_POST_MSG postHttpMsg;
static DBG_HTTP_STATE currHttpOwner;

static pthread_t dbgHTTPcmd;
static pthread_t dbgHTTPtask;
static uint8_t dbgHTTPstack[DBG_HTTP_STACK_SIZE] ALIGN8;
static uint8_t dbgHTTPcmdstack[DBG_HTTP_CMDSTACK_SIZE] ALIGN8;

static mqd_t dbgHTTPmsg;
static mqd_t dbgHTTPcmdmsg;

// This function advances any pointer passed to it in the circular buffer

static void nextPtr( uint8_t **ptr)
{
    if( ++(*ptr) >= &getHttpMsg.str[DBG_GET_MSG_SIZE]) *ptr = &getHttpMsg.str[0];
}

// Entry point for the character output routine invoked by writing to StdOut

void dbg_http_char_out( uint8_t c)
{
    int px_status;
    
    px_status = pthread_mutex_lock( &getHttpMsg.mutex );
    XASSERT( px_status==0, px_status );

    *getHttpMsg.ptr = c;
    nextPtr( &getHttpMsg.ptr);

    if( getHttpMsg.ptr == &getHttpMsg.str[0]) {
        getHttpMsg.wrapped++;
        // This print statement causes the serial output thread to hang - which then blocks
        // any thread that writes a serial output message.  Do not enable unless find root 
        // cause of the thread hang...
        //DBG_PRINTF(LOG_INFO, DBG_PRFX "GET buffer has wrapped %u times\n", getHttpMsg.wrapped );
    }

    px_status = pthread_mutex_unlock( &getHttpMsg.mutex );
    XASSERT( px_status==0, px_status );
}

// Entry point for the string output routine invoked by writing to StdOut

void dbg_http_str_out( uint8_t *str, uint32_t len)
{
    uint32_t i;

    for( i = 0; i < len; i++) dbg_http_char_out( *str++);
}

void _dbg_watcher(char* data, int len, void* arg)
{
  dbg_http_str_out((uint8_t*)data, len);
}

static void initHttpPostMsg( void)
{
    postHttpMsg.ptr = &postHttpMsg.str[0];
}

static uint32_t appendHttpPostMsg( uint8_t *msg, uint32_t len)
{
    uint32_t i;

    for( i = 0; i < len; i++) {
        // Keep the pointer within the string.  Truncate at size...
        if( (uint32_t)postHttpMsg.ptr < (uint32_t)&postHttpMsg.str[sizeof(postHttpMsg.str)]) {
            *postHttpMsg.ptr++ = *msg++;
        } else {
            DBG_PRINTF(LOG_INFO, DBG_PRFX "POST buffer has been truncated at %u bytes\n", sizeof(postHttpMsg.str));
        }
    }

    return( (uint32_t)postHttpMsg.ptr - (uint32_t)&postHttpMsg.str[0]);
}

static void initHttpGetMsg( void)
{
    getHttpMsg.wrapped = 0;
    getHttpMsg.ptr = &getHttpMsg.str[0];
}

static uint32_t sendHttpGetMsg( http_request_t request)
{
    uint32_t len = 0;
    void *msg = NULL;
    int px_status;
    
    px_status = pthread_mutex_lock( &getHttpMsg.mutex );
    XASSERT( px_status==0, px_status );

    // If either is true, then our buffer is NOT empty!
    if(( getHttpMsg.wrapped) || ( getHttpMsg.ptr != &getHttpMsg.str[0])) {
        void *ptr;
        void *str;

        msg = MEM_MALLOC_ALIGN( DBG_GET_MSG_SIZE, cpu_get_dcache_line_size());
        XASSERT( msg != NULL, (uint32_t)msg);
        str = msg;

        // Handle the partial buffer case first if we've wrapped...
        if( getHttpMsg.wrapped) {
            ptr = (void *)getHttpMsg.ptr;
            len = (uint32_t)&getHttpMsg.str[DBG_GET_MSG_SIZE] - (uint32_t)ptr;
            memcpy( str, (const void *)ptr, (size_t)len);
            str = (void *)((uint32_t)msg + len);
        }

        // We always have something to transfer from the start...
        ptr = (void *)&getHttpMsg.str[0];
        len = (uint32_t)getHttpMsg.ptr - (uint32_t)ptr;
        memcpy( str, (const void *)ptr, (size_t)len);
        len = ((uint32_t)str - (uint32_t)msg) + len;

        // All done with the transfer so reset the GET buffer!
        initHttpGetMsg();
    }

    px_status = pthread_mutex_unlock( &getHttpMsg.mutex );
    XASSERT( px_status==0, px_status );

    // The http resource layer will FREE this memory!
    if( len) {
        uint32_t status;

        status = http_resource_send_data( request, (char *)msg, len);
        if ( OK != status )
        {
            DBG_PRINTF(LOG_INFO, DBG_PRFX "resource send data failed, possible network shutdown!\n");
        }
    }

    return( len);
}

static void releaseHttpMsg( DBG_HTTP_MESSAGE *msg)
{
    error_type_t status;

    if( msg->ptr != NULL) {
        status = http_resource_buffer_free( msg->request, msg->ptr);
        XASSERT( status == OK, status);
    }
}

// This is the GET/POST callback from the http_resource_register.  This
// is called whenever the http debug registration has some activity...

static void dbg_http_cb( http_request_t request,
                         http_resource_event_t event,
                         char *buffer,
                         uint32_t buffer_size_in_bytes)
{
    int px_status;
    DBG_HTTP_MESSAGE msg;

    msg.request = request;
    msg.event = event;
    msg.length = buffer_size_in_bytes;
    msg.ptr = buffer;

    px_status = mq_send( dbgHTTPmsg, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY );
    XASSERT( px_status==0, px_status );
}

#include "cmd_wrapper_api.h"
static void _outfn(void* ignore, char* data, unsigned int data_len)
{
  /* Use of dbg_printf here leads to a infinite loop due to cmd_steal_debug()
     We use a lower level API. (Hacks always lead to bad things) */
  extern int dbg_outbuf_add_char(char c);
  while (data_len--) dbg_outbuf_add_char(*data++);
}

static void _run_debug_cmd(char *buffer, int len)
{
  static void* _hdl = NULL;
  if (!_hdl) _hdl = cmd_wrapper_create("HTTP");
  if (!_hdl) return;
  cmd_wrapper_pipeline(_hdl, buffer, len, _outfn, NULL);
  cmd_wrapper_pipeline(_hdl,    ";",   1, _outfn, NULL);
}

// This is the HTTP command thread that actually invokes the Debugger

static void *dbg_http_command( void *input)
{
    int px_status;
    DBG_HTTP_MESSAGE cmdMsg;

    DBG_PRINTF(LOG_INFO, DBG_PRFX "Command thread is now running!\n");
    dbg_printf("%s: TID = %d\n", __func__, gettid());

    while( 1) {
        // Suspend until we receive a message from the HTTP task thread
        px_status = mq_receive( dbgHTTPcmdmsg, (char *)&cmdMsg, sizeof(cmdMsg), MQ_DEFAULT_PRIORITY );
        XASSERT( px_status==0, px_status );
        
        _run_debug_cmd(cmdMsg.ptr, cmdMsg.length);
    }
    
    return 0;
}

// This is the HTTP task that communicates with the HTTP resource layer and
// the HTTP command thread to process POST and GET requests to "/console"

static void *dbg_http_task( void *input)
{
    int px_status;
    error_type_t status;

    DBG_HTTP_MESSAGE msg;

    // Wait for system init to finish
    SysWaitForInit();

    // Our current state is supplied at system init!
    currHttpOwner.state = currHttpOwner.request = (uint32_t)input;

    // Initialize now as GETs can show up before a POST!
    initHttpGetMsg();

    // Register the GET/POST callback with the http resource manager
    status = http_resource_register( "/console", 0, dbg_http_cb);
    XASSERT( status == OK, status);

    // Create a mutex for exclusive access to the "get" buffer
    px_status = posix_mutex_init( &getHttpMsg.mutex );
    XASSERT( px_status==0, px_status );

    dbg_watcher_register(_dbg_watcher, NULL);

    // Create a mail box queue between the dbgTask and the dbgCallback
    posix_create_message_queue(&dbgHTTPmsg, "/DbgHttpQ", DBG_HTTP_QUEUE_SIZE, sizeof(DBG_HTTP_MESSAGE));

    // Create a mail box queue between the dbgTask and the dbgCommand
    posix_create_message_queue(&dbgHTTPcmdmsg, "/DbgHttpCmdQ", DBG_HTTP_QUEUE_SIZE, sizeof(DBG_HTTP_MESSAGE));

    // Create and kick-off the Debug HTTP command thread...
    px_status = posix_create_thread(&dbgHTTPcmd, dbg_http_command, 0, "Dbg HTTP Command", &dbgHTTPcmdstack, DBG_HTTP_CMDSTACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT( px_status==0, px_status );

    DBG_PRINTF(LOG_INFO, DBG_PRFX "Task thread is now running!\n");

    while( 1) {
        // Suspend until we receive a message from the resource callback
        px_status = mq_receive( dbgHTTPmsg, (char *)&msg, sizeof(msg), MQ_DEFAULT_PRIORITY );
        XASSERT( px_status==0, px_status );

        switch( msg.event) {
            case HTTP_RESOURCE_HEADER_DATA:
                // We wait for BODY_DATA_END to handle all error conditions!
                // HTTP operation type/state only appears in header data...
                if(( currHttpOwner.state == 0 ) && ( currHttpOwner.request == 0)) {
                    if( strstr( msg.ptr, "GET")) {
                        currHttpOwner.state = GET;
                    } else
                    if( strstr( msg.ptr, "POST")) {
                        currHttpOwner.state = POST;
                        initHttpPostMsg();
                    }

                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "Task received HEADER_DATA:");
                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "    state: %u", currHttpOwner.state);
                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "  request: %u", msg.request);
                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "    event: %u", msg.event);
                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "  bfrSize: %u", msg.length);
                    DBG_PRINTF(LOG_DEBUG, DBG_PRFX "   buffer: %s\n", msg.ptr);
                    currHttpOwner.request = msg.request;
                }

                releaseHttpMsg( &msg);
            break;

            case HTTP_RESOURCE_BODY_DATA:
                if( currHttpOwner.request == msg.request) {
                    // Send whatever data we have up to this point...
                    if( currHttpOwner.state == GET) {
                        uint32_t len;

                        len = sendHttpGetMsg( msg.request);
                        DBG_PRINTF(LOG_DEBUG, DBG_PRFX "Task sent %u GET_BODY_DATA bytes\n", len);
                    } else
                    // Create or append to the current post message...
                    if( currHttpOwner.state == POST) {
                        uint32_t len;

                        len = appendHttpPostMsg( (uint8_t *)msg.ptr, msg.length);
                        DBG_PRINTF(LOG_DEBUG, DBG_PRFX "Task received %u POST_BODY_DATA bytes\n", len);
                    }
                }

                releaseHttpMsg( &msg);
            break;

            case HTTP_RESOURCE_BODY_DATA_END:
                // We don't handle more than one request at a time!
                if( msg.request != currHttpOwner.request) {
                    status = http_resource_set_status_code( msg.request, HTTP_409_CONFLICT);
                    XASSERT( status == OK, status);

                    status = http_resource_header_complete( msg.request);
                    XASSERT( status == OK, status);
                } else {
                    // Always do these steps for GETs or POSTs...
                    status = http_resource_set_status_code( msg.request, HTTP_200_OK);
                    XASSERT( status == OK, status);

                    status = http_resource_set_content_type( msg.request, HTTP_TYPE_PLAIN);
                    XASSERT( status == OK, status);

                    status = http_resource_header_complete( msg.request);
                    XASSERT( status == OK, status);

                    // Return the last of any available GET data...
                    if( currHttpOwner.state == GET) {
                        uint32_t len;

                        len = sendHttpGetMsg( msg.request);
                        DBG_PRINTF(LOG_DEBUG, DBG_PRFX "Task sent %u GET_BODY_DATA_END bytes\n", len);
                    } else
                    // Complete the POST message and fire it off!
                    if( currHttpOwner.state == POST) {
                        uint32_t len;
                        DBG_HTTP_MESSAGE cmdMsg;

                        len = appendHttpPostMsg( (uint8_t *)msg.ptr, msg.length);
                        DBG_PRINTF(LOG_DEBUG, DBG_PRFX "Task received %u POST_BODY_DATA_END bytes\n", len);

                        cmdMsg.request = msg.request;
                        cmdMsg.event = msg.event;
                        cmdMsg.length = len;
                        cmdMsg.ptr = (void *)&postHttpMsg.str[0];

                        px_status = mq_send( dbgHTTPcmdmsg, (char *)&cmdMsg, sizeof(cmdMsg), MQ_DEFAULT_PRIORITY );
                        XASSERT( px_status==0, px_status );
                    }

                    // HTTP sequence complete.  Go brain dead...
                    currHttpOwner.state = 0;
                    currHttpOwner.request = 0;
                }

                releaseHttpMsg( &msg);

                status = http_resource_send_complete( msg.request );
                if ( OK != status )
                {
                    DBG_PRINTF(LOG_ERR, DBG_PRFX "Task faled to send complete, possible network disconnect\n");
                }
            break;

            default:
                // We might need to beef this up as currently we clear the owners state/request info...
                //DBG_PRINTF(LOG_ERR, DBG_PRFX "Task received an unhandled event: %u from state: %u\n", msg.event, currHttpOwner.state);

                // Something we don't handle!  Go brain dead...
                currHttpOwner.state = 0;
                currHttpOwner.request = 0;

                releaseHttpMsg( &msg);
            break;
        }
    }
    
    return 0;
}

// This is called from the Debugger Console to kick-off the entire
// HTTP Debug Console Operation over EWS, either network or USB Device

void dbg_http_task_init( void)
{
    int px_status;

    // Create and kick-off the Debug HTTP thread...
    px_status = posix_create_thread(&dbgHTTPtask, dbg_http_task, 0, "Dbg HTTP Task", &dbgHTTPstack, DBG_HTTP_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT( px_status==0, px_status );
}

