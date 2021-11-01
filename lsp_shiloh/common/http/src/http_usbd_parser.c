/*
 * ============================================================================
 * Copyright (c) 2006-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_usbd_parser.c
 * 
 * \brief Handle HTTP over USB processing 
 * 
 * Accepts the HTTP requests over USB.  Handles the parsing of
 * these requests and returns back the page requested or
 * appropriate error codes
 * *
 **/
//--------------------------------------
// Included files
//--------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "error_types.h"
#include "utils.h"
#include "platform_api.h"
#include "ATypes.h"
#include "lassert.h"
#include "dprintf.h"
#include "logger.h"
#include "sys_init_api.h"
#include "usb_device_api.h"
#include "memAPI.h"
#include "http_parse.h"
#include "html.h"
#include "htmlssi.h"
#include "platform.h"

#ifndef __linux__
#include "cpu_api.h"
#endif

// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_USB
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE 
#define DBG_PRFX    "HTTP USB: "

/* Local Definitions and Types */

/** \brief The HTTP USBD Parser's stack size in bytes */

#define HTTP_USBD_STACK_SIZE POSIX_MIN_STACK_SIZE

/** \brief The buffer size to hold USB data we'll parse through */
#define MAX_REWIND_BYTES    512

/** \brief THe size of the ews data buffer */
#define HTTP_PARSER_BUF_LEN 2048

/** \brief Device info context providing info about the
 *         endpoint to the HTTP server */
static http_usb_device_info_t http_usb_device_info = 
{
    .network_port = 0,      // Process requests from all ports
};

/* Local Variables */

/** \brief A buffer to hold USB data we'll parse through */
static uint8_t http_rewind_buffer[ MAX_REWIND_BYTES ];

/** \brief The number of bytes that have been rewound into the rewind buffer */
static uint32_t bytes_rewound = 0;

/** \brief The thread data structure */
static pthread_t http_usbd_parser_thread_id;

/** \brief The thread's stack data structure */
static ALIGN8 uint8_t http_usbd_parser_stack[ HTTP_USBD_STACK_SIZE ];

/** \brief our USB Device Pipe Data buffer */
static uint8_t http_usbd_ews_buffer[ HTTP_PARSER_BUF_LEN ];


/* Local Function Declarations */

static void http_usbd_write_data_chain( HttpPageContext *context );
static int32_t http_usb_read( uint8_t *buffer, uint32_t len, uint32_t timeout);
static int32_t http_usb_write( uint8_t *buffer, uint32_t len );
static void http_usb_rewind( uint8_t *buffer, uint32_t len );
static void* http_usbd_parser( void* thread_param );
static error_type_t http_usbd_blocking_send( HttpPageContext * context,
                                             char * buffer,
                                             uint32_t size,
                                             bool free_buffer );
static error_type_t http_usbd_blocking_recv( HttpPageContext * context,
                                             char * buffer,
                                             uint32_t size,
                                             uint32_t *timeout );


/* Public Functions */

/** 
 *
 * \brief Inititalize HTTP USB device parser
 * 
 */
void http_usbd_parser_init(void)
{
    /* initialize some variables */
    memset( http_rewind_buffer, 0, MAX_REWIND_BYTES );
    bytes_rewound = 0;

    /* start the thread */
    posix_create_thread(&http_usbd_parser_thread_id, http_usbd_parser, 0, "http_usbd_parser", http_usbd_parser_stack, HTTP_USBD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
}


/* Local Function Definitions */
void http_methods_initialize( void );

/** 
 * \brief Starts the parsing of HTTP requests over USB
 * 
 * This function reads the incoming data from usb and then parse
 * the data.  IO Timeouts are applied if the complete request is
 * not received in a timely manner.  Once a full request is
 * received, it is parsed and completed and the appropriate HTTP
 * response is sent out over USB
 * 
 */
static void* http_usbd_parser( void* thread_param )
{
    error_type_t e_res;
    bool valid_request;
    bool context_created;
	int32_t current_bytes_read;
    int32_t  current_bytes_buffered;
    uint32_t new_bytes_parsed;
    uint32_t io_timeout_start;
    uint16_t io_timeout_in_seconds;
    HttpPageContext * context;

    /* Wait for System Init */
    SysWaitForInit( );

    /* Initialize protocol specific information / variables
       This needs to be done after all threads have had a chance to run -
       otherwise some calls into other modules will fail (status observer)
       */
    http_methods_initialize();

    e_res = platvars_get_io_timeout(&io_timeout_in_seconds);
    ASSERT( OK == e_res );

    context = NULL;

    while ( true )
    {
        http_reset_connections( e_http_connection_usb );

        /* Read in a loop until we've read the header.  We read until the
         * double CRLF, then allow the post handler to read the actual post
         * data if necessary. */
        context_created = false;
        valid_request = false;
        io_timeout_start = 0;
        do
        {
            /* Do a USB read for one byte with a long timeout - waiting for the
             * request to come.  Then follow this up with a larger buffer but a
             * zero length timeout to grab the rest of the data.  This reduces
             * the amount polling that is required */
            DBG_VERBOSE( "http_usb_read\n" );
            current_bytes_buffered = http_usb_read( http_usbd_ews_buffer, 1, io_timeout_in_seconds * TIMER_INTS_PER_SECOND );
            if(current_bytes_buffered < 0)
            {
                // we had an error, such as usb not connected.  
                // wait for a bit and then try again
                posix_sleep_ms(1000);    // wait for 1 second
                continue;       // go and do it again.
            }

            if ( current_bytes_buffered > 0 )
            {
                DBG_MSG( "http_usb_read more\n" );
                current_bytes_read = http_usb_read( http_usbd_ews_buffer + current_bytes_buffered, HTTP_PARSER_BUF_LEN - current_bytes_buffered, 1 );
                if(current_bytes_read < 0)
                {
                    // had an error, continue.
                    if( context_created == true)
                    {
                        http_context_destroy( context, false );
                    }
                    continue;
                }
                current_bytes_buffered += current_bytes_read;

                /* new request */
                if ( context_created == false )
                {
                    DBG_MSG( "http_context_create\n" );
                    e_res = http_context_create( &context,
                                                 (char *)http_usbd_ews_buffer,
                                                 current_bytes_buffered,
                                                 &new_bytes_parsed,
                                                 e_http_connection_usb,
                                                 (void *)&http_usb_device_info,
                                                 http_usbd_blocking_send,
                                                 http_usbd_blocking_recv );
                    context_created = true;
                }
                /* current request */
                else
                {
                    DBG_MSG( "http_context_add_data\n" );
                    e_res = http_context_add_data( context,
                            (char *)( http_usbd_ews_buffer ),
                            ( current_bytes_buffered ),
                            &new_bytes_parsed );
                }

                switch ( e_res )
                {
                    case HTTP_PARSING_JUNK:
                    {
                        /* re-initialize buffer and start all over */
                        io_timeout_start = 0;
                        DBG_MSG( "http_context_destroy junk\n" );
                        http_context_destroy( context, false );
                        context_created = false;
                        break;
                    }

                    case HTTP_PARSING_INCOMPLETE:
                    {
                        /* only start the timer once per request */
                        if ( io_timeout_start == 0 )
                        {
                            DBG_MSG( "TIMEOUT STARTED, parsing incomplete\n" );
                            /* We didn't get the whole thing - so lets start the IO
                             * timeout and keep reading the USB pipe */
                            io_timeout_start = posix_gettime_ticks();
                        }
                        break;
                    }
                    
                    case HTTP_MEMORY_OUT:
                    {
                        /* We can't process anything, we are out of memory
                         * re-initialize buffer and start all over */
                        io_timeout_start = 0;
                        DBG_ERR( "http_context_destroy out of memory\n" );
                        http_context_destroy( context, false );
                        context_created = false;
                        break;
                    }

                    default:
                    {
                        ASSERT( OK == e_res );

                        /* we have a valid request now! */
                        valid_request = true;

                        /* did we read more than the actual request length? 
                         * if so, then we need to rewind */
                        if ( current_bytes_buffered > new_bytes_parsed )
                        {
                            DBG_MSG( "Rewinding %d bytes\n",
                                     ( current_bytes_buffered - new_bytes_parsed ) );

                            http_usb_rewind( ( http_usbd_ews_buffer + new_bytes_parsed ),
                                    ( current_bytes_buffered - new_bytes_parsed ) );
                        }
                        break;
                    }
                }
            }
            else
            {
                /* There was no information in the pipe -
                 * have we exceeded the IO timeout yet? */
                if ( ( posix_gettime_ticks() - io_timeout_start ) >= ( io_timeout_in_seconds * SYS_TICK_FREQ ) )
                {
                    DBG_MSG( "Restart HTTP over USB - IO timeout in read\n" );

                    if ( context_created == true )
                    {
                        DBG_MSG( "http_context_destroy timeout\n" );
                        http_context_destroy( context, false );
                    }

                    io_timeout_start = 0;
                }
            }
        } while ( false == valid_request );
        /* Now we have a valid request */

        DBG_MSG( "http_context_process\n" );
        /* Process the http context */
        ASSERT( NULL != context );
        e_res = http_context_process( context ); 

        if ( HTTP_DATA_TO_WRITE == e_res )
        {
            DBG_MSG( "http_usb_write\n" );
            http_usbd_write_data_chain( context );
        }
        else if ( OK == e_res )
        {
            e_res = http_context_wait_for_done_state( context );
            if ( HTTP_DATA_TO_WRITE == e_res )
            {
                DBG_MSG( "http_usb_write after wait for done\n" );
                http_usbd_write_data_chain( context );
            }
        }
        else
        {
            DBG_ERR( "http_context_process returned FAIL!\n" );
        }

        DBG_MSG( "http_context_destroy\n" );
        http_context_destroy( context, false );
    }
    return 0;
}


/**
 *
 * \brief loop over the context data chain and send all the response
 * data 
 *
 * \param context A valid HttpPageContext pointer
 *
 */
static void http_usbd_write_data_chain( HttpPageContext *context )
{
    bool done_writing;
    uint32_t bytes_wrote;
    ASSERT( NULL != context );
    ASSERT( NULL != context->dataChain );

    /* initialize looping parameter */
    done_writing = false;
    do
    {
        bytes_wrote = 0;

        if ( ( 0 != ( context->dataChain->size - context->dataChain->atByte ) ) &&
             ( context->state != e_http_state_resetting ) )
        {
            DBG_VERBOSE( "Sending data chain element %p with %lu bytes\n",
                         ( context->dataChain->data + context->dataChain->atByte ),
                         ( context->dataChain->size - context->dataChain->atByte ) );

            bytes_wrote = http_usb_write( (uint8_t *)( context->dataChain->data + context->dataChain->atByte ),
                                          ( context->dataChain->size - context->dataChain->atByte ) );
            DBG_VERBOSE( "bytes_wrote: %d\n", bytes_wrote );
            if ( bytes_wrote == 0 )
            {
                // error sending or no data to send
                done_writing = true;
            }
            else
            {
                /* Did we send all the data for that link?  If so, release the link. */
                context->dataChain->atByte += bytes_wrote;
            }
        }
        else
        {
            /* causes context->dataChain to point to the next chain in the link */
            deleteFirstDataElement( context );
            if ( context->dataChain == NULL )
            {
                done_writing = true;
            }
        }

    } while ( false == done_writing );
}


static error_type_t http_usbd_blocking_send( HttpPageContext * context,
                                             char * buffer,
                                             uint32_t size,
                                             bool free_buffer )
{
    uint32_t num_bytes_sent;

    ASSERT( NULL != context );
    ASSERT( NULL != buffer );
    ASSERT( 0 != size );

    context->lastAccess = tx_time_get( );
    if ( free_buffer == false )
    {
        uint8_t* usb_buff = MEM_MALLOC_ALIGN( size, cpu_get_dcache_line_size() );
        if (  usb_buff == NULL )
        {
            return FAIL;
        }
        memcpy( usb_buff, buffer, size );
        num_bytes_sent = http_usb_write( usb_buff, size );
    }
    else
    {
        num_bytes_sent = http_usb_write( (uint8_t *)( buffer ), size );
    }
    if ( num_bytes_sent != size )
    {
        return FAIL;
    }

    return OK;
}


static int32_t http_usbd_blocking_recv( HttpPageContext *context,
                                        char *buffer,
                                        uint32_t size,
                                        uint32_t *timeout )
{
    int32_t bytes;
    bytes = http_usb_read( (uint8_t*)buffer, size, *timeout );
    if ( bytes == 0)
    {
        *timeout = 0;
    }
    return bytes;
}


/** 
 * \brief Read data from the EWS USB endpoint
 * 
 * \param buffer Buffer to read the data into
 * 
 * \param len Max number of bytes to be read
 * 
 * \param timeout How long you're willing to wait, in OS timer ticks
 * 
 * \return Number of bytes read
 * 
 **/
static int32_t http_usb_read( uint8_t * buffer, uint32_t len, uint32_t timeout )
{
    int32_t bytes_read = 0;

    ASSERT( NULL != buffer );
    ASSERT( 0 != len );
    ASSERT( (len & 0x80000000) == 0);

    if ( bytes_rewound > 0 )
    {
        int32_t bytes_from_rewind;

        bytes_from_rewind = MIN( len, bytes_rewound );

        /* copy data out of the rewind buffer */
        memcpy( buffer, http_rewind_buffer, bytes_from_rewind );

        /* move data forward in the rewind buffer so we don't copy data
         * we've already copied out again */
        if ( bytes_from_rewind < bytes_rewound )
        {
            uint32_t i;

            for ( i = 0; i < ( bytes_rewound - bytes_from_rewind ); i++ )
            {
                http_rewind_buffer[ i ] = http_rewind_buffer[ i + bytes_from_rewind ];
            }
        }

        /* decrement the rewind buffer counter */
        bytes_rewound -= bytes_from_rewind;

        bytes_read = bytes_from_rewind;
    }
    else
    {
        bytes_read = usb_http_device_read( buffer, len, timeout );
    }

    return bytes_read;
}


/** 
 * \brief Write data to the EWS endpoint
 * 
 * \param buffer The data to write
 * 
 * \param len Number of bytes to write
 * 
 * \return Number of bytes written
 * 
 **/
static int32_t http_usb_write( uint8_t * buffer, uint32_t len )
{
    ASSERT( NULL != buffer );
    ASSERT( 0 != len );
    return usb_http_device_write( buffer, len, 0 );
}


/** 
 * \brief Write data to the EWS interrupt endpoint
 * 
 * \param buffer The data to write
 * 
 * \param len Number of bytes to write
 * 
 * \return Number of bytes written
 * 
 **/
int32_t http_usb_interrupt_write( uint8_t *buffer, uint32_t len )
{
    return usb_http_device_interrupt_write( buffer, len, 0 );
}

/** 
 * \brief Hold data you've read to be able to read it again
 * later.
 * 
 * \param buffer Data to keep for later
 * 
 * \param len Number of bytes to rewind
 * 
 **/
static void http_usb_rewind( uint8_t * buffer, uint32_t len )
{
    ASSERT( bytes_rewound + len <= MAX_REWIND_BYTES );

    memcpy( http_rewind_buffer + bytes_rewound, buffer, len );
    bytes_rewound += len;
}

#ifdef DEBUG
pthread_t http_usbd_parser_get_tid( void )
{
    return http_usbd_parser_thread_id;
}
#endif // DEBUG
