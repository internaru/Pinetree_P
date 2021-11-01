/*
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file http.c
 *
 * \brief This file is the main API implementation file for the HTTP module.
 * API functions described in the http_api.h file are implemented here.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "logger.h"
#include "http_api.h"
#include "http_vars.h"

#ifdef HAVE_NETWORK
#include "net_api.h"
#include "http_network_parser.h"
#ifdef HAVE_ETHERNET
#include "net_iface_vars.h"
#endif
#endif

#include "io_device.h"

#ifdef HAVE_USB_DEVICE
#include "http_usbd_parser.h"
#endif

#include "http_parse.h"
#include "http_config.h"
#include "http_file_mappings.h"
#include "http_methods_support.h"
#include "htmlssi.h"
#include "memAPI.h"
#include "dprintf.h"
#include "error_types.h"
#include "lassert.h"
#include "logger.h"
#include "hwconfig_api.h"
#include "debug.h"
#ifdef HAVE_PRINT_SUPPORT
#include "printvars_api.h"
#endif
#include "utils.h"

#include "http_rom.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifndef __linux__
#include "cpu_api.h"
#endif

// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_HTTP
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE 
#define DBG_PRFX    "HTTP: "

/* Local Structures */

typedef struct
{
    char ** ArgNames;
    char ** ArgValues;
    uint32_t  ArgCnt;
} CGIARGSTRUCT;

/** \brief This structure contains information related to the resource
 * registration and related callbacks */
typedef struct http_server_resource_node
{
    struct http_server_resource_node    *next;
    http_resource_event_cb_t             resource_cb;
    http_resource_event_flag_cb_t        event_cb;
    char                                *resource;
    int32_t                              port;
    void                                *usr_ctxt;
} http_server_resource_node_t;

/** \brief Enumeration for chunk header states */
typedef enum chunk_header_state_s
{
    HTTP_CHUNK_INIT,
    HTTP_CHUNK_FIRST_CR,
    HTTP_CHUNK_FIRST_NL,
    HTTP_CHUNK_DATA,
    HTTP_CHUNK_END_CR,
    HTTP_CHUNK_END_NL,
    HTTP_CHUNK_DONE
} chunk_header_state_t;

// Maximum size in bytes for incoming chunk headers 
#define MAX_CHUNK_SIZE  20


/* Local functions */
static void parseHeader( HttpPageContext* context );
static char DecodeBase64char( char encodedVal );
static void initializeHttpPageContext( HttpPageContext *context, void *deviceInfo, http_ioconn_t conn );
static bool HasValidCredentials( char *request, char *url );
static void closeConnection( HttpPageContext *context, bool reset_socket );
static void deleteDataChain ( HttpPageContext *context );
static void checkHeaderThenBeginParse( HttpPageContext* context );
static uint32_t http_internal_get_content_length( char* buffer, uint32_t bufLen );
static bool http_is_resource_already_registered( char *request, int32_t port, http_server_resource_node_t *node );
static uint8_t get_supported_methods( char *resource );
static bool http_is_registered_resource( char *request, int32_t port, http_server_resource_node_t *node );
static error_type_t http_add_data_to_chain( HttpPageContext *context, char *buffer, uint32_t length );

static char *http_create_chunk_header( uint32_t size );

static uint32_t string_n_skip_white_space( char * input_str,
        uint32_t str_size_in_bytes,
        char ** output_string );

static error_type_t http_internal_context_header_parse_uri(
        HttpPageContext * context );

static error_type_t http_internal_context_header_add_data(
        HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_bytes_parsed );

static error_type_t http_internal_context_body_add_data(
        HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_bytes_parsed );

static void http_internal_context_notify_cb_header_complete( 
        HttpPageContext * context );

static void http_internal_context_notify_cb_body_data_end( 
        HttpPageContext * context );


static error_type_t http_chunk_get_length( char **buffer,
                                           uint32_t *length,
                                           uint32_t *chunk_length );

static bool http_have_full_chunk_info( char *buffer,
                                       uint32_t length );

#ifdef HAVE_CMD
static const char *http_cmd_registered_desc;
static const char *http_cmd_registered_usage;
static const char *http_cmd_registered_notes;
static int http_cmd_registered_cb( int argc, char *argv[] );

static const char *http_cmd_contexts_desc;
static const char *http_cmd_contexts_usage;
static const char *http_cmd_contexts_notes;
static int http_cmd_contexts_cb( int argc, char *argv[] );

#ifdef DEBUG
static const char *http_cmd_get_pwd_desc;
static const char *http_cmd_get_pwd_usage;
static const char *http_cmd_get_pwd_notes;
static int http_cmd_get_pwd_cb( int argc, char *argv[] );

static const char *http_cmd_set_pwd_desc;
static const char *http_cmd_set_pwd_usage;
static const char *http_cmd_set_pwd_notes;
static int http_cmd_set_pwd_cb( int argc, char *argv[] );
#endif // DEBUG

static const char *http_cmd_defaults_desc;
static const char *http_cmd_defaults_usage;
static const char *http_cmd_defaults_notes;
static int http_cmd_defaults_cb( int argc, char *argv[] );

#endif // HAVE_CMD

/* Local defines */
#define HTTP_ABSOLUTE_START "http://"
#define HTTPS_ABSOLUTE_START "https://"
#define COMMA_SPACE ", "

#define CGIARG_NAMES    1200
#define CGIARG_VALUES   1024

#define HTTP_CLIENT_CREDENTIALS "Authorization:"
#define HTTP_EXPECTED_AUTH_TYPE "Basic"

#define CHUNK_HEADER_SIZE 10 // 4 bytes for CR/LF, 4 bytes for size
#define CHUNK_BUFFER_LEN  20

/* Attempt to make this as big as the largest set of files that will be
 * passed back, so we don't have to Realloc.  Due to this this 
 * should be moved to the config layer: TODO */
#define HTTP_PARSER_BUF_LEN 32768

// Value is not important - just the number of characters in the value
#define HTTP_FAKE_CONTENT_LENGTH_INT    1000000000
#define HTTP_FAKE_CONTENT_LENGTH_STR    "1000000000"

#define MAX_NUM_CGI_ARGS    256
#define CGIARG_SIZE         ( MAX_NUM_CGI_ARGS * sizeof( char ** ) )

#define MS_PER_TICK         ( MILLISEC_PER_TICK )

/* Local variables */
pthread_mutex_t http_context_protect_mutex = POSIX_MUTEX_INITIALIZER;
static HttpPageContext *activeContexts = NULL;
static http_server_resource_node_t *http_server_resource_head = NULL;
static uint32_t http_context_id = 0;



void http_init( void )
{
    http_vars_init();


    DBG_ISZERO_CHECK( cmd_register_cmd( "http",
					NULL,
					NULL,
					NULL,
					NULL,
					NULL ) );
    DBG_ISZERO_CHECK(cmd_register_subcmd( "http",
					  "registered",
					  http_cmd_registered_desc,
					  http_cmd_registered_usage,
					  http_cmd_registered_notes,
					  http_cmd_registered_cb ) );
    DBG_ISZERO_CHECK(cmd_register_subcmd( "http",
					  "contexts",
					  http_cmd_contexts_desc,
					  http_cmd_contexts_usage,
					  http_cmd_contexts_notes,
					  http_cmd_contexts_cb ));

    #ifdef DEBUG
    DBG_ISZERO_CHECK(cmd_register_subcmd( "http",
					  "get_pwd",
					  http_cmd_get_pwd_desc,
					  http_cmd_get_pwd_usage,
					  http_cmd_get_pwd_notes,
					  http_cmd_get_pwd_cb ));
    DBG_ISZERO_CHECK(cmd_register_subcmd( "http",
					  "set_pwd",
					  http_cmd_set_pwd_desc,
					  http_cmd_set_pwd_usage,
					  http_cmd_set_pwd_notes,
					  http_cmd_set_pwd_cb ));
    #endif // DEBUG
    DBG_ISZERO_CHECK(cmd_register_subcmd( "http",
					  "defaults",
					  http_cmd_defaults_desc,
					  http_cmd_defaults_usage,
					  http_cmd_defaults_notes,
					  http_cmd_defaults_cb ));


    /* Initialize protocol specific information / variables */
    //http_methods_initialize(); -- moved to usbd thread to fix timing problems, update after new observers in place

	// Must initialize these modules after the cmd registration is complete
	// because some of these add more cmds to the http command
    #ifdef HAVE_USB_DEVICE
    http_usbd_parser_init();
    #endif

    #ifdef HAVE_NETWORK
    http_network_parser_init();
    #endif

    #ifdef HAVE_HTTP_TEST
    void initialize_http_test( void );
    initialize_http_test();
    #endif
}


static error_type_t http_internal_context_body_add_data(
        HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_bytes_parsed )
{
    bool callback_complete;
    error_type_t e_res;
    uint32_t cur_body_length;
    char * cb_buffer;
    uint32_t cb_size;

    ASSERT( NULL != context );
    ASSERT( NULL != request_buffer );
    XASSERT( ( e_http_state_header_received == context->state ), context->state );
    ASSERT( NULL != actual_bytes_parsed );

    DBG_VERBOSE( "%s - enter, bytes to process: %d\n", __FUNCTION__, request_buffer_len_in_bytes );

    callback_complete = false;
    *actual_bytes_parsed = 0;
    e_res = OK;

    if( ( 0 == request_buffer_len_in_bytes ) )
    {
        if ( 0 != context->content_length )
        {
            /* We don't have the whole request yet */
            e_res = HTTP_PARSING_INCOMPLETE;
        }
        else
        {
            /* There is no body to parse */
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        // Check for registered resource
        // If it is found - we don't want to buffer anything here just send it
        // on to the registered function
        if ( NULL != context->request_cb )
        {
            /* parsing incomplete until otherwise told */
            e_res = HTTP_PARSING_INCOMPLETE;

            /* handle chunked encoding */
            if (  context->content_length == CHUNKED_ENCODING )
            {
                bool have_full_chunk;
                char * actual_chunk_start;
                uint32_t real_data_left;
                error_type_t my_res;
                
                DBG_VERBOSE( "Have chunk encoded request - %d bytes\n", request_buffer_len_in_bytes );    
                have_full_chunk = true;
                actual_chunk_start = request_buffer;
                real_data_left = request_buffer_len_in_bytes;

                *actual_bytes_parsed = request_buffer_len_in_bytes;

                if ( context->atByte < context->pageSize )
                {
                    DBG_VERBOSE( "Mid chunk -- %d/%d\n", context->atByte, context->pageSize );
                    if ( real_data_left + context->atByte <= context->pageSize )
                    {
                        DBG_VERBOSE("Send entire buffer\n" );
                        // All data part of previously defined chunk
                        if ( actual_chunk_start != NULL )
                        {
                            context->num_buffers++;
                            cb_buffer = MEM_MALLOC( real_data_left );
                            if ( cb_buffer == NULL )
                            {
                                e_res = HTTP_MEMORY_OUT;
                            }
                            else if ( context->state == e_http_state_resetting )
                            {
                                DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
                                e_res = HTTP_NO_CONTEXT;
                            }
                            else
                            {
                                memcpy( cb_buffer, actual_chunk_start, real_data_left );
                                context->request_cb( context->id, HTTP_RESOURCE_BODY_DATA, cb_buffer, real_data_left );
                                context->atByte += real_data_left;
                                real_data_left = 0;
                                if (  context->atByte == context->pageSize )
                                {
                                    DBG_VERBOSE( "resetting chunk info #1\n");
                                    context->atByte = 0;
                                    context->pageSize = 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        DBG_VERBOSE( "rest of last chunk\n" );
                        cb_size = context->pageSize - context->atByte;
                        cb_buffer = (char*)MEM_MALLOC( cb_size );
                        if ( cb_buffer == NULL)
                        {
                            DBG_ERR( "Malloc failed(%d) - trying to alloc %d bytes\n", __LINE__, real_data_left );
                            e_res = HTTP_MEMORY_OUT;
                        }
                        else if ( context->state == e_http_state_resetting )
                        {
                            DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
                            e_res = HTTP_NO_CONTEXT;
                        }
                        else
                        {
                            memcpy( cb_buffer, actual_chunk_start, cb_size );
                            real_data_left -= cb_size;
                            actual_chunk_start += cb_size;

                            // All data part of previously defined chunk
                            if ( cb_buffer != NULL )
                            {
                                context->num_buffers++;
                            }
                            context->request_cb( context->id, HTTP_RESOURCE_BODY_DATA, cb_buffer, cb_size );
                            context->atByte += cb_size;
                            if ( context->atByte == context->pageSize )
                            {
                                DBG_VERBOSE( "resetting chunk info #2\n" );
                                context->atByte = 0;
                                context->pageSize = 0;
                            }
                        }
                    }
                }

                /* TODO XXX FIXME
                 * This is a simple implementation that does absolutely
                 * NO optimizations for memory or speed.  This could be
                 * done but is not for the sake of getting it done and
                 * creating a testable solution.  The prime example
                 * here is concatenating the buffers and doing a
                 * memcpy of the data.  This means we touch all partial
                 * chunks at least twice */

                /* we already have some chunked data,
                 * concatenate and then process. */
                if ( NULL != context->partial_chunk )
                {
                    DBG_VERBOSE( "Have partial chunk saved - add it to new data\n" );    
                    real_data_left = request_buffer_len_in_bytes + context->partial_chunk_length;
                    actual_chunk_start = MEM_MALLOC( real_data_left );
                    if ( NULL == actual_chunk_start )
                    {
                        DBG_ERR( "Malloc failed(%d) - trying to alloc %d bytes\n", __LINE__, real_data_left );
                        e_res = HTTP_MEMORY_OUT;
                    }
                    else
                    {
                        /* got a buffer, copy over data to create a
                         * new "request buffer" */
                        memcpy( actual_chunk_start,
                                context->partial_chunk,
                                context->partial_chunk_length );

                        memcpy( ( actual_chunk_start + context->partial_chunk_length ),
                                request_buffer,
                                request_buffer_len_in_bytes );
                        MEM_FREE_AND_NULL( context->partial_chunk );
                    }
                }

                /* don't enter loop on error */
                if ( e_res == HTTP_MEMORY_OUT )
                {
                    have_full_chunk = false;
                }

                /* while we have chunks, send more data to callbacks
                 * when we reach a partial chunk save and exit, for 
                 * later parsing when more data is received. */
                while ( ( true == have_full_chunk ) && ( real_data_left > 0 ) )
                {
                    DBG_VERBOSE( "start of have_full_chunk loop\n" );    
                    // Verify entire chunk header is present
                    // Look for \r\n
                    // Look for hex value
                    // Look for \r\n
                    have_full_chunk = http_have_full_chunk_info( actual_chunk_start,
                                                                 real_data_left );

                    /* here we have a full chunk so we'll parse it
                     * and process it.  If it's not a zero-length
                     * chunk then we'll pass it on to the callback */
                    if ( true == have_full_chunk )
                    {
                        DBG_VERBOSE( "Have a full chunk header\n" );    
                        my_res = http_chunk_get_length( &actual_chunk_start,
                                                        &real_data_left,
                                                        &context->pageSize );
                        DBG_VERBOSE( "Chunk size = %d\n", context->pageSize );    
                        /* at this point we KNOW we have a full chunk,
                         * so this function MUST return OK */
                        ASSERT( OK == my_res ); UNUSED_VAR(my_res);

                        /* if this is a zero-length chunk we are DONE! */
                        if ( context->pageSize == 0 )
                        {
                            e_res = OK;
                            context->state = e_http_state_body_received;
                            // Zero length chunk - no body data!
                            callback_complete = true;
                            have_full_chunk = false;    // So we exit the while loop
                        }
                        /* otherwise we have data so continue processing */
                        else
                        {
                            if ( real_data_left == 0 )
                            {
                                // No data left - chunk header was last item in this buffer
                                DBG_VERBOSE( "No data left after header - wait for next buffer\n" );

                            }
                            else
                            {
                                // Send data to callback
                                if ( context->pageSize >= real_data_left + context->atByte )
                                {
                                    // Partial or complete chunk
                                    context->atByte += real_data_left;
                                    cb_size = real_data_left;
                                }
                                else
                                {
                                    DBG_VERBOSE( "Multiple chunks in current buffer\n" );    
                                    cb_size = context->pageSize - context->atByte;
                                    context->atByte = context->pageSize;
                                }

                                /* allocate memory for this chunk data to pass onto the callback */
                                cb_buffer = ( char * ) MEM_MALLOC( cb_size );
                                if ( cb_buffer == NULL )
                                {
                                    DBG_ERR( "Failed to allocate buffer for copy of body: %d\n", cb_size );
                                    e_res = HTTP_MEMORY_OUT;
                                }
                                else if ( context->state == e_http_state_resetting )
                                {
                                    DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
                                    e_res = HTTP_NO_CONTEXT;
                                }
                                else
                                {
                                    memcpy( cb_buffer,
                                            actual_chunk_start,
                                            cb_size );
                                    context->num_buffers++;

                                    DBG_VERBOSE( "Sending %d bytes in callback\n", cb_size );    
                                    context->request_cb( context->id,
                                                       HTTP_RESOURCE_BODY_DATA,
                                                       cb_buffer,
                                                       cb_size );
                                    context->request_body_length += request_buffer_len_in_bytes;
                                    callback_complete = true;

                                    if ( context->atByte == context->pageSize )
                                    {
                                        DBG_VERBOSE( "Reset atByte and pagePtr\n" );
                                        context->atByte = 0;
                                        context->pageSize = 0;
                                    }
                                }

                                if ( ( e_res != HTTP_MEMORY_OUT ) && ( e_res != FAIL ) )
                                {
                                    /* set pointers and length then we'll go
                                     * back to the beginning of the loop
                                     * and check for more full chunks */
                                    actual_chunk_start += cb_size;
                                    real_data_left -= cb_size;
                                }
                            }
                        }
                    }
                    /* at this point we only have a partial chunk
                     * so we'll save it and parse it/send it later */
                    else
                    {
                        DBG_VERBOSE( "Don't have full chunk header\n" );    
                        if ( real_data_left > 0 )
                        {
                            DBG_VERBOSE( "Save off partial chunk header\n" );    
                            /* allocate memory for the entire set of
                             * chunked encoded data */
                            context->partial_chunk = MEM_MALLOC( real_data_left );
                            if ( NULL == context->partial_chunk )
                            {
                                /* we are now out of memory, let the
                                 * caller know */
                                e_res = HTTP_MEMORY_OUT;
                                DBG_ERR( "Malloc failed(%d) - trying to alloc %d bytes\n", __LINE__, real_data_left );
                                *actual_bytes_parsed = 0; 
                            }
                            else
                            {
                                context->partial_chunk_length = real_data_left;
                                /* save data into temp buffer and return,
                                 * issue NO callback */
                                memcpy( context->partial_chunk,
                                        actual_chunk_start,
                                        real_data_left );
                                e_res = HTTP_PARSING_INCOMPLETE;
                            }
                        }
                    }

                    /* exit loop upon error */
                    if ( ( e_res == FAIL ) ||
                         ( e_res == HTTP_MEMORY_OUT ) )
                    {
                        have_full_chunk = false;
                    }
                }
            }
            else
            {
                cb_buffer = NULL;
                cb_size = 0;

                // allocate new memory if we have data 
                if ( 0 != request_buffer_len_in_bytes )
                {
                    // allocate new buffer that is only body data to send
                    // on to callback
                    /* Do we have all the request yet? */
                    if ( context->content_length > ( context->request_body_length + request_buffer_len_in_bytes ) )
                    {
                        /* We don't have the whole request yet */
                        e_res = HTTP_PARSING_INCOMPLETE;

                        *actual_bytes_parsed = request_buffer_len_in_bytes;
                    }
                    else
                    {
                        /* valid request and we have it all! */
                        e_res = OK;
                        *actual_bytes_parsed = context->content_length - context->request_body_length;
                        context->state = e_http_state_body_received;
                    }

                    cb_size = *actual_bytes_parsed;
                    cb_buffer = ( char * ) MEM_MALLOC( cb_size );
                    if ( cb_buffer == NULL )
                    {
                        DBG_ERR( "Malloc failed(%d) - trying to alloc %d bytes\n", __LINE__, cb_size + 1 );
                        e_res = HTTP_MEMORY_OUT;
                    }
                    else if ( context->state == e_http_state_resetting )
                    {
                        DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
                        e_res = HTTP_NO_CONTEXT;
                    }
                    else
                    {
                        /* move data from request buffer to callback buffer */
                        memcpy( cb_buffer, request_buffer, cb_size );
                        context->num_buffers++;
                        /* issue callback */
                        context->request_cb( context->id,
                                           HTTP_RESOURCE_BODY_DATA,
                                           cb_buffer,
                                           cb_size );
                        context->request_body_length += *actual_bytes_parsed;

                        callback_complete = true;
                    }
                }
            }
        }
        /* Pass request on to default handler */
        else
        {

            if ( ( context->event_cb == NULL ) &&
                 ( context->request_cb == NULL ) &&
                 ( context->content_length == CHUNKED_ENCODING ) )
            {
                // Did not find match - and this is chunk encoded - not supported (yet?)
                DBG_ERR( "Got chunked encoded request - not handled yet!!!\n" );
                addDataToHttpContext( HTTP_SERVICE_UNAVAIL, strlen( HTTP_SERVICE_UNAVAIL ), false, context, false );
                context->state = e_http_state_body_complete;
            }
            else
            {
                if ( NULL == context->request_body )
                {
                    cur_body_length = 0;
                }
                else
                {
                    cur_body_length = context->request_body_length;
                }

                /* Do we have all the request yet? */
                if ( context->content_length > cur_body_length + request_buffer_len_in_bytes )
                {
                    /* We don't have the whole request yet */
                    if ( context->event_cb != NULL )
                    {
                        // External module will read rest of data
                        e_res = OK;
                    }
                    else
                    {
                        e_res = HTTP_PARSING_INCOMPLETE;
                    }

                    *actual_bytes_parsed = request_buffer_len_in_bytes;
                }
                else
                {
                    /* valid request and we have it all! */
                    *actual_bytes_parsed = context->content_length - cur_body_length;
                    context->state = e_http_state_body_received;
                }

                /* One time allocation per context request */
                if ( NULL == context->request_body )
                {
                    if ( context->content_length == CHUNKED_ENCODING )
                    {
                        // If data is chunked - only allocate memory sufficient for this piece - other memory 
                        // will be allocated by other modules as part of the http_resource_read() API
                        context->request_body = MEM_MALLOC_ALIGN( *actual_bytes_parsed + LENGTH_OF_NULL,
                                                                  cpu_get_dcache_line_size() );
                    }
                    else
                    {
                        context->request_body = MEM_MALLOC_ALIGN( context->content_length + LENGTH_OF_NULL,
                                                                  cpu_get_dcache_line_size() );
                    }
                    if ( NULL == context->request_body )
                    {
                        e_res = HTTP_MEMORY_OUT;
                        DBG_ERR( "Malloc failed(%d) - trying to alloc %d bytes\n", __LINE__, context->content_length );
                    }
                    DBG_VERBOSE( "context->request_body: %08X\n", context->request_body );
                }

                if ( HTTP_MEMORY_OUT != e_res )
                {
                    ASSERT( NULL != context->request_body );

                    memcpy( ( context->request_body + cur_body_length ),
                            request_buffer,
                            *actual_bytes_parsed );

                    context->request_body_length += *actual_bytes_parsed;
                    context->request_body[ context->request_body_length ] = '\0';
                }
            }
        }
    }

    /* If we get FAIL here, then we don't have any body to parse,
     * so all the data is extra -- we are done! */
    if ( ( FAIL == e_res ) && 
         ( context->content_length != CHUNKED_ENCODING ) )
    {
        e_res = OK;
        context->state = e_http_state_body_received;
    }

    DBG_VERBOSE( "%s - exit(%d), bytes processed: %d callback complete \n", 
		 __FUNCTION__, e_res, *actual_bytes_parsed, callback_complete );

    return e_res;
}


static error_type_t http_internal_context_header_add_data(
        HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_bytes_parsed )
{
    error_type_t e_res;
    uint32_t num_white_space_chars;
    char * header_start = NULL;
    char * header_end = NULL;
    char * temp_header = NULL;
    uint32_t old_header_length;

    ASSERT( NULL != context );
    ASSERT( NULL != request_buffer );
    ASSERT( 0 != request_buffer_len_in_bytes );
    XASSERT( ( e_http_state_initialized == context->state ), context->state );
    ASSERT( NULL != actual_bytes_parsed );

    e_res = OK;
    *actual_bytes_parsed = 0;

    DBG_VERBOSE( "%s - enter - %d bytes\n", __FUNCTION__, request_buffer_len_in_bytes );

    if ( NULL == context->request_header )
    {
        DBG_VERBOSE( "%s - context->request_header is null\n", __FUNCTION__ );
        /* Find the start of the header */
        num_white_space_chars = string_n_skip_white_space( request_buffer, request_buffer_len_in_bytes, &header_start );

        *actual_bytes_parsed += num_white_space_chars;

        /* It's all whitespace, so we need more data, and all of the data
         * is whitespace so we don't need to save any of it */
        if ( num_white_space_chars == request_buffer_len_in_bytes )
        {
            e_res = HTTP_PARSING_INCOMPLETE;
        }
    }
    else
    {
        header_start = request_buffer;
    }

    ASSERT( NULL != header_start );

    DBG_VERBOSE( "%s - request_header_length: %d\n", __FUNCTION__, context->request_header_length );
    if ( ( OK == e_res ) && ( context->request_header_length == 0 ) )
    {
        // Verify we have enough bytes to do comparison
        if ( ( request_buffer_len_in_bytes - ( header_start - request_buffer ) ) >= strlen( DELETE_REQUEST ) )
        {
            // Check to see if the request starts with valid HTTP method.  If not,
            // reset connection and quit
            if ( ( strncmp( GET_REQUEST,    header_start, strlen( GET_REQUEST    ) ) != 0 ) &&
                 ( strncmp( POST_REQUEST,   header_start, strlen( POST_REQUEST   ) ) != 0 ) &&
                 ( strncmp( PUT_REQUEST,    header_start, strlen( PUT_REQUEST    ) ) != 0 ) &&
                 ( strncmp( HEAD_REQUEST,   header_start, strlen( HEAD_REQUEST   ) ) != 0 ) &&
                 ( strncmp( DELETE_REQUEST, header_start, strlen( DELETE_REQUEST ) ) != 0 ) )
            {
                DBG_ERR( "Invalid HTTP request - resetting connection\n" );
                e_res = HTTP_PARSING_JUNK;
            }
        }
    }

    if ( OK == e_res )
    {
        /* determine if the end of the header is here */
        header_end = strnstri( header_start,
                ENDOFHEADER,
                ( request_buffer_len_in_bytes - ( request_buffer - header_start ) ) );

        if ( NULL == header_end )
        {
            /* We have some header, but not all */
            e_res = HTTP_PARSING_INCOMPLETE;

            old_header_length = context->request_header_length;

            context->request_header_length += request_buffer_len_in_bytes - ( request_buffer - header_start );

            *actual_bytes_parsed += request_buffer_len_in_bytes - ( request_buffer - header_start  );
        }
        else
        {
            /* We have the entire header */
            context->state = e_http_state_header_received;

            header_end += strlen( ENDOFHEADER );

            old_header_length = context->request_header_length;

            context->request_header_length += ( header_end - header_start ) + LENGTH_OF_NULL;

            *actual_bytes_parsed += header_end - header_start;
        }

        /* We have to save the data in the header buffer, even if we don't
         * have it all */
        if ( NULL == context->request_header )
        {
            temp_header = MEM_MALLOC_ALIGN( context->request_header_length,
                                            cpu_get_dcache_line_size() );
            if ( NULL == temp_header )
            {
                e_res = HTTP_MEMORY_OUT;
            }
        }
        else
        {
            if ( context->request_header_length > old_header_length )
            {
                temp_header = MEM_MALLOC_ALIGN( context->request_header_length,
                                                cpu_get_dcache_line_size() );
                if ( NULL == temp_header )
                {
                    e_res = HTTP_MEMORY_OUT;
                }
            }
        }

        if ( HTTP_MEMORY_OUT != e_res )
        {

            ASSERT( NULL != temp_header );

            if ( NULL == context->request_header )
            {
                /* Move the caller's buffer to our own internal buffer */
                /* TODO COULD OPTIMIZE BY USING CDMA or smart memory copy
                 * or redesign to use a chain of buffers and never need
                 * to move memory */
                memcpy( temp_header,
                        header_start,
                        ( context->request_header_length ) );
            }
            else
            {
                /* Move the caller's buffer to our own internal buffer */
                /* TODO COULD OPTIMIZE BY USING CDMA or smart memory copy
                 * or redesign to use a chain of buffers and never need
                 * to move memory */

                /* start with the initial header section we already have */
                memcpy( temp_header,
                        context->request_header,
                        old_header_length );

                /* then copy new section of header we have */
                memcpy( temp_header + old_header_length,
                        header_start,
                        ( context->request_header_length - old_header_length ) );

                MEM_FREE_AND_NULL( context->request_header );
            }

            context->request_header = temp_header;

            /* If we are 'OK' then we have the entire header and can
             * NULL-terminate it */
            if ( OK == e_res )
            {
                /* null terminate the complete buffer */
                context->request_header[ ( context->request_header_length - 1 ) ] = '\0';
            }
            else
            {
                if ( strnstri( context->request_header, ENDOFHEADER, context->request_header_length ) != NULL )
                {
                    // Clean this up so that we aren't copying data left and right
                    e_res = OK;
                    context->state = e_http_state_header_received;

                    /* determine if the end of the header is here */
                    header_end = strnstri( context->request_header,
                                           ENDOFHEADER,
                                           context->request_header_length );
                    header_end += strlen( ENDOFHEADER );

                    if ( header_end != context->request_header + context->request_header_length )
                    {
                        uint32_t extra_data;
                        extra_data = context->request_header + context->request_header_length - header_end;
                        *actual_bytes_parsed -= extra_data;
                        context->request_header_length -= extra_data;
                        *header_end = '\0';
                    }

                }
            }
        }
    }

    DBG_VERBOSE( "%s - exit - %d bytes\n", __FUNCTION__, *actual_bytes_parsed );

    return e_res;
}


static void http_internal_context_notify_cb_header_complete( 
        HttpPageContext * context )
{
    http_server_resource_node_t *head;

    ASSERT( NULL != context );
    ASSERT( NULL != context->requested_uri );
    XASSERT( ( e_http_state_header_received == context->state ), context->state );

    /* determine if another module is responsible for this particular
     * URI and notify them, if so. */
    head = http_server_resource_head;

    while ( head != NULL )
    {
        DBG_VERBOSE( "header compare: %s =?= %s\n", context->requested_uri, head->resource ? head->resource : "null" );
        if ( http_is_registered_resource( context->requested_uri, context->port, head ) == true )
        {
            context->request_cb = head->resource_cb;
            context->event_cb = head->event_cb;
            if ( context->request_cb != NULL )
            {
                // Give module its own copy of the header - they can free when done
                char *header = MEM_MALLOC_ALIGN( context->request_header_length, cpu_get_dcache_line_size() );
                if ( header == NULL )
                {
                    ASSERT( 0 );
                }
                if ( context->state == e_http_state_resetting )
                {
                    DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
                }
                else
                {
                    memcpy( header, context->request_header, context->request_header_length );
                    context->num_buffers++;
                    DBG_VERBOSE( "Callback for header data\n" );
                    context->request_cb( context->id,
                            HTTP_RESOURCE_HEADER_DATA,
                            header,
                            context->request_header_length );
                }
            }
            else
            {
                context->event_cb( context->id, HTTP_RESOURCE_HEADER_DATA_AVAILABLE, head->usr_ctxt );
            }
            break;
        }
        head = head->next;
    }
}


static void http_internal_context_notify_cb_body_data_end( 
        HttpPageContext * context )
{
    if( NULL == context )
    {
        DBG_ERR("\n%s  Context is NULL %d\n", __FUNCTION__, __LINE__);
        return;
    }
	if( NULL == context->requested_uri )
    {
        DBG_ERR("\n%s  context->requested_uri is NULL %d\n", __FUNCTION__, __LINE__);
        return;
    }
    ASSERT( NULL != context );
    ASSERT( NULL != context->requested_uri );

    if ( context->request_cb != NULL )
    {
        if ( context->request_body != NULL )
        {
            context->num_buffers++;                                        
        }
        else
        {
            context->request_body_length = 0;
        }

        if ( context->state == e_http_state_resetting )
        {
            DBG_MSG( "Skipping callback for context: %d since it has been reset\n", context->id );
        }
        else
        {
            context->request_cb( context->id,
                    HTTP_RESOURCE_BODY_DATA_END,
                    context->request_body,
                    context->request_body_length );
        }

        if ( context->request_body != NULL )
        {
            context->request_body = NULL;
            context->request_body_length = 0;
        }

    }
}


static error_type_t http_internal_context_header_parse_uri(
        HttpPageContext * context )
{
    char *header;
    char *uri;
    char *temp_uri;
    int32_t bytes_remaining;
    uint32_t uri_length;
    error_type_t e_res;
    uint32_t u_res;

    ASSERT( NULL != context );
    ASSERT( NULL != context->request_header );
    ASSERT( 0 != context->request_header_length );
    XASSERT( ( e_http_state_header_received == context->state ), context->state );

    e_res = OK;

    header = context->request_header;
    bytes_remaining = context->request_header_length;

    /* Find the requested url - first skip the HTTP command */
    while ( ( bytes_remaining > 0 ) &&
            ( *header != '\0' ) &&
            ( ( *header != ' ' ) &&
              ( *header != '\r' ) &&
              ( *header != '\n' ) &&
              ( *header != '\t' ) ) )
    {
        header++;
        bytes_remaining--;
    }

    /* Now skip any gap */
    string_n_skip_white_space( header, bytes_remaining, &temp_uri );

    /* Find the beginning of the URI */
    u_res = strncasecmp( temp_uri, HTTP_ABSOLUTE_START, sizeof(HTTP_ABSOLUTE_START) );
    if ( 0 == u_res )
    {
        temp_uri += strlen( HTTP_ABSOLUTE_START );
        /* Look for next '/' character */
        while ( ( temp_uri < ( context->request_header + context->request_header_length ) ) &&
                ( *temp_uri != '/' ) &&
                ( *temp_uri != '\0' ) )
        {
            temp_uri++;
        }
    }

    uri = temp_uri;

    /* Now we point to the start of the url - find length */
    uri_length = 0;
    while ( ( temp_uri < ( context->request_header + context->request_header_length ) ) && 
            ( *temp_uri != '?' ) && 
            ( *temp_uri != ' ' ) &&
            ( *temp_uri != '\r' ) &&
            ( *temp_uri != '\n' ) &&
            ( *temp_uri != '\t' ) && 
            ( *temp_uri != '\0' ) )
    {
        uri_length++;
        temp_uri++;
    }
    /* Add null terminater */
    uri_length++;

    context->requested_uri = ( char * )MEM_MALLOC_ALIGN( uri_length,
                                                         cpu_get_dcache_line_size() );
    if ( context->requested_uri == NULL )
    {
        DBG_ERR( "Failed malloc(%d) of %d bytes\n", uri_length );
        e_res = HTTP_MEMORY_OUT;
    }
    else
    {
        /* Save our URI */
        strncpy( context->requested_uri, uri, uri_length - 1 );
        context->requested_uri[ uri_length - 1 ] = '\0';
    }

    DBG_VERBOSE( "%s - uri = '%s'\n", __FUNCTION__, context->requested_uri );

    return e_res;
}

static error_type_t http_allocate_cgi_args( HttpPageContext *context )
{
    context->cgi_args = ( http_request_cgi_args_t * )MEM_MALLOC_ALIGN( sizeof( http_request_cgi_args_t ), cpu_get_dcache_line_size() );
    if ( context->cgi_args == NULL )
    {
        return FAIL;
    }
    memset( context->cgi_args, 0x00, sizeof( http_request_cgi_args_t ) );

    context->cgi_args->names = ( char ** )MEM_MALLOC_ALIGN( CGIARG_SIZE, cpu_get_dcache_line_size() );
    if ( context->cgi_args->names == NULL )
    {
        MEM_FREE_AND_NULL( context->cgi_args );
        return FAIL;
    }
    memset( context->cgi_args->names, 0x00, CGIARG_SIZE );

    context->cgi_args->values = ( char ** )MEM_MALLOC_ALIGN( CGIARG_SIZE, cpu_get_dcache_line_size() );
    if ( context->cgi_args->values == NULL )
    {
        MEM_FREE_AND_NULL( context->cgi_args->names );
        MEM_FREE_AND_NULL( context->cgi_args );
        return FAIL;
    }
    memset( context->cgi_args->values, 0x00, CGIARG_SIZE );

    context->cgi_args->cnt = 0;
    return OK;
}


static error_type_t http_internal_context_header_parse_cgi( HttpPageContext * context )
{
    char * cur_ptr;
    error_type_t e_res;

    cur_ptr = strstr( context->request_header, context->requested_uri );
    if ( 0 == cur_ptr )
    {
        return OK;
    }
    cur_ptr += strlen( context->requested_uri );
    if ( '?' == *cur_ptr )
    {
        char *start;
        char temp;
        cur_ptr++;

        e_res = http_allocate_cgi_args( context );
        if ( OK != e_res )
        {
            return e_res;
        }

        start = cur_ptr;
        while( ( *cur_ptr != ' ' ) &&
               ( *cur_ptr != '\t' ) &&
               ( *cur_ptr != '\r' ) &&
               ( *cur_ptr != '\n' ) &&
               ( *cur_ptr != 0 ) )
        {
            if ( *cur_ptr == '&' )
            {
                // Finished value - starting new name
                temp = *cur_ptr;
                *cur_ptr = 0;
                context->cgi_args->values[ context->cgi_args->cnt ] = ( char * ) MEM_MALLOC_ALIGN( cur_ptr - start + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
                ASSERT( context->cgi_args->values[ context->cgi_args->cnt ] != NULL );
                urlDecode( start, context->cgi_args->values[ context->cgi_args->cnt ], cur_ptr - start + LENGTH_OF_NULL );
                *cur_ptr = temp;
                start = ++cur_ptr;
                context->cgi_args->cnt++;
            }
            else if ( *cur_ptr == '=' )
            {
                // Finished name - starting value
                temp = *cur_ptr;
                *cur_ptr = 0;
                context->cgi_args->names[ context->cgi_args->cnt ] = ( char * ) MEM_MALLOC_ALIGN( cur_ptr - start + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
                ASSERT( context->cgi_args->names[ context->cgi_args->cnt ] != NULL );
                urlDecode( start, context->cgi_args->names[ context->cgi_args->cnt ], cur_ptr - start + LENGTH_OF_NULL );
                *cur_ptr = temp;
                start = ++cur_ptr;
            }
            cur_ptr++;
        }
        if ( context->cgi_args->names[ context->cgi_args->cnt ] != NULL )
        {
            // Need to terminate last value
            temp = *cur_ptr;
            *cur_ptr = 0;
            context->cgi_args->values[ context->cgi_args->cnt ] = ( char * ) MEM_MALLOC_ALIGN( cur_ptr - start + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
            ASSERT( context->cgi_args->values[ context->cgi_args->cnt ] != NULL );
            urlDecode( start, context->cgi_args->values[ context->cgi_args->cnt ], cur_ptr - start + LENGTH_OF_NULL );
            *cur_ptr = temp;
            context->cgi_args->cnt++;
        }
    }
    return OK;
}

/**
 *
 * \brief Creates and adds an HTTP context to the HTTP internal modules.
 * The new context is returned via the pointer in the parameter list for use
 * with other HTTP API functions.
 *
 * \param new_context A pointer to a memory location large enough to hold
 * a pointer to the created context data structure.
 *
 * \param request_buffer A buffer containing the request to associate with
 * the new context.  The buffer does NOT have to be a full request.  Verify
 * that the request is complete by checking the return error_type_t
 *
 * \param request_buffer_len_in_bytes The size of the request_buffer in bytes
 *
 * \param actual_request_bytes_parsed The number of valid bytes in the buffer
 * passed.  This value should be useful to track pipe I/O reads and determining
 * if the pipe reads have started reading another request
 *
 * \param io_type The IO type, one of the HTTP_IO_TYPE enums
 *
 * \param device_info A caller-specific data.  Users of the HTTP module may 
 * use this value to save data and refer to it later when processing http
 * data.
 *
 * \return
 * - OK upon successful parsing of the request buffer
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The request buffer contains junk data and should be
 * destroyed using http_context_destroy()
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 *
 */
error_type_t http_context_create( HttpPageContext ** new_context,
                                  char * request_buffer,
                                  uint32_t request_buffer_len_in_bytes,
                                  uint32_t * actual_request_bytes_parsed,
                                  http_ioconn_t io_type,
                                  void * device_info,
                                  http_blocking_send_cb_t blocking_send_cb,
                                  http_blocking_recv_cb_t blocking_recv_cb
                                  )
{
    error_type_t e_res;
    HttpPageContext * context;

    char * header_start;
    char * header_end;

    lock_contexts();

    ASSERT( NULL != new_context );
    ASSERT( NULL != request_buffer );
    ASSERT( NULL != actual_request_bytes_parsed );
    ASSERT( NULL != blocking_send_cb );

    header_start = NULL;
    header_end = NULL;

    e_res = OK;
    *actual_request_bytes_parsed = 0;

    /* always create the handle and return it */
    *new_context = ( HttpPageContext * )MEM_MALLOC_ALIGN( sizeof( HttpPageContext ),
                                                          cpu_get_dcache_line_size() );
    context = *new_context;

    if ( context == NULL )
    {
        DBG_ERR( "Not enough memory to allocate context\n" );
        e_res = HTTP_MEMORY_OUT;
    }
    else
    {
        int px_res;

        initializeHttpPageContext( context, device_info, io_type );

        // Add to activeContexts linked list
        context->nextContext = activeContexts;
        activeContexts = context;

        context->blocking_send_cb = blocking_send_cb;
        context->blocking_recv_cb = blocking_recv_cb;

        px_res = sem_init(&context->data_chain_semaphore, 0, 1);
        XASSERT(px_res == 0, px_res);
        if(px_res != 0)
        {
            e_res = FAIL;
        }
        
        px_res = sem_init(&context->resource_done_semaphore, 0, 0);
        XASSERT(px_res == 0, px_res);
        if ( px_res != 0 )
        {
            e_res = FAIL;
        }

    }

    if ( OK == e_res )
    {
        if ( 0 == request_buffer_len_in_bytes )
        {
            e_res = HTTP_PARSING_INCOMPLETE;
        }
    }


    if ( OK == e_res )
    {
        /* handle header data */
        e_res = http_internal_context_header_add_data( context,
                request_buffer,
                request_buffer_len_in_bytes,
                actual_request_bytes_parsed );
    }

    if ( OK == e_res )
    {
        /* Get the size of the request body, once we have the entire header */
        context->content_length = http_internal_get_content_length( context->request_header,
                                                                    context->request_header_length );

        /* If we have the entire header, then get the URI */
        e_res = http_internal_context_header_parse_uri( context );
        if ( OK == e_res )
        {
            e_res = http_internal_context_header_parse_cgi( context );
        }
    }

    if ( OK == e_res )
    {
        uint32_t body_bytes_parsed;
        char * body_start;

        ASSERT ( NULL != context->requested_uri );

        /* Notify any module responsible for this request */
        http_internal_context_notify_cb_header_complete( context );

        /* Now handle the request body */
        body_start = request_buffer + *actual_request_bytes_parsed;

        body_bytes_parsed = 0;

        e_res = http_internal_context_body_add_data(
                context,
                body_start,
                ( request_buffer_len_in_bytes - *actual_request_bytes_parsed ),
                &body_bytes_parsed );

        *actual_request_bytes_parsed += body_bytes_parsed;
    }

    if ( OK == e_res )
    {
        /* notify any module responsible for this request */
        http_internal_context_notify_cb_body_data_end( context );
    }
    
    unlock_contexts();
    UNUSED_VAR( header_end );
    UNUSED_VAR( header_start );
    return e_res;
}


/**
 *
 * \brief Destroys a current context, freeing all associated memory
 *
 * \param context A valid http context
 *
 * \warning This function MUST be called after error or completion of a call to
 * http_context_create() in order to prevent resource leaks.
 *
 */
void http_context_destroy( HttpPageContext * context, bool reset_socket )
{
    lock_contexts();

    /* since http_context destroy might be called after an HTTP_MEMORY_OUT
     * error during a http_context_create() call, the context may be NULL */
    if( NULL != context )
    {
        closeConnection( context, reset_socket );
    }

    unlock_contexts();
}


/**
 *
 * \brief Adds data to an existing HTTP context.
 *
 * \param request_buffer A buffer containing more of the request to associate
 * with the new context.  The buffer does NOT have to be a full request.  Verify
 * that the request is complete by checking the return error_type_t
 *
 * \param request_buffer_len_in_bytes The size of the request_buffer in bytes
 *
 * \param actual_request_bytes_parsed The number of valid bytes in the buffer
 * passed.  This value should be useful to track pipe I/O reads and determining
 * if the pipe reads have started reading another request
 *
 * \return
 * - OK upon successful parsing of the request buffer
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The request buffer contains junk data and should be
 * destroyed using http_context_destroy()
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 *
 */
error_type_t http_context_add_data( HttpPageContext * context,
        char * request_buffer,
        uint32_t request_buffer_len_in_bytes,
        uint32_t * actual_request_bytes_parsed )
{
    error_type_t e_res;

    lock_contexts();

    ASSERT( NULL != context );
    ASSERT( NULL != request_buffer );
    ASSERT( NULL != actual_request_bytes_parsed );
    context->lastAccess = (uint32_t)posix_gettime_ticks();

    e_res = OK;

    *actual_request_bytes_parsed = 0;

    if ( 0 == request_buffer_len_in_bytes )
    {
        e_res = HTTP_PARSING_INCOMPLETE;
    }


    if ( e_http_state_initialized == context->state )
    {
        /* Handle header if we still haven't done so */
        if ( OK == e_res )
        {
            /* handle header data */
            e_res = http_internal_context_header_add_data( context,
                    request_buffer,
                    request_buffer_len_in_bytes,
                    actual_request_bytes_parsed );
        }

        if ( OK == e_res )
        {
            /* Get the size of the request body, once we have the entire header */
            context->content_length = http_internal_get_content_length( context->request_header,
                                                                        context->request_header_length );
            /* If we have the entire header, then get the URI */
            e_res = http_internal_context_header_parse_uri( context );
            if ( OK == e_res )
            {
                e_res = http_internal_context_header_parse_cgi( context );
            }
        }

        if ( OK == e_res )
        {
            ASSERT ( NULL != context->requested_uri );

            /* Notify any module responsible for this request */
            http_internal_context_notify_cb_header_complete( context );
        }
    }

    if ( OK == e_res )
    {
        uint32_t body_bytes_parsed;
        char * body_start;

        /* Now handle the request body */
        body_start = request_buffer + *actual_request_bytes_parsed;

        body_bytes_parsed = 0;

        e_res = http_internal_context_body_add_data( context,
                                                     body_start,
                                                     ( request_buffer_len_in_bytes - *actual_request_bytes_parsed ),
                                                     &body_bytes_parsed );
        *actual_request_bytes_parsed += body_bytes_parsed;
    }

    // This code breaks non-ascii data - is there another check that could be done?
    //if ( OK == e_res )
    //{
    //    int test = strnlen( context->request_body, context->request_body_length );
    //    if ( test != context->request_body_length )
    //    {
    //        DBG_ERR( "Garbage data received - throw out data and restart - got: %d, expected: %d!\n", test, context->request_body_length );
    //        e_res = HTTP_PARSING_JUNK;
    //    }
    //}
    if ( OK == e_res )
    {
        /* notify any module responsible for this request */
        http_internal_context_notify_cb_body_data_end( context );
    }
    
    unlock_contexts();

    return e_res;
}


/**
 *
 * \brief Processes an HTTP request.  All the data must be added to the context
 *
 * \param context A valid http context
 *
 * \param response_buffer A pointer to the string buffer pointer where the
 * response data is held.  This data should be returned via the IO pipe 
 * which it came from (in most cases).
 *
 * \param response_len_in_bytes The size of the string (in bytes) in the response_buffer
 *
 * \return
 * - OK upon successful parsing of the context
 *
 * - HTTP_PARSING_INCOMPLETE The context does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 *
 */
error_type_t http_context_process( HttpPageContext * context )
{
    error_type_t e_res;
    uint32_t response_len_in_bytes;

    lock_contexts();

    ASSERT( NULL != context );
    ASSERT( NULL != context->request_header );
    ASSERT( 0 != context->request_header_length );

    e_res = FAIL;

    if ( ( NULL != context->request_cb ) ||
         ( NULL != context->event_cb ) )
    {
        e_res = OK;
    }
    else
    {
        checkHeaderThenBeginParse( context );

        context->lastAccess = (uint32_t)posix_gettime_ticks();

        fillContextWithData( context, false, false );

        ASSERT( NULL != context->dataChain );
        response_len_in_bytes = context->dataChain->size - context->dataChain->atByte;

        if ( response_len_in_bytes > 0 )
        {
            e_res = HTTP_DATA_TO_WRITE;
        }
    }

    unlock_contexts();

    return e_res;
}

error_type_t http_context_wait_for_done_state( HttpPageContext * context )
{
    error_type_t e_res;
    uint32_t response_len_in_bytes;
    char *response_buffer;
    int px_res;

    ASSERT( NULL != context );

    e_res = OK;

    /* block waiting for resource complete */
    px_res = sem_wait(&context->resource_done_semaphore);
    ASSERT(px_res == 0);

    /* if the data chain is NOT null then we might have some data to write */
    if ( NULL != context->dataChain )
    {
        response_len_in_bytes = context->dataChain->size - context->dataChain->atByte;

        response_buffer = context->dataChain->data;

        /* check if resource has pushed data to be sent onto data chain */
        if ( ( response_len_in_bytes > 0 ) && ( NULL != response_buffer ) )
        {
            e_res = HTTP_DATA_TO_WRITE;
        }
    }

    return e_res;
}

error_type_t http_context_check_for_done_state( HttpPageContext * context )
{
    int px_res;
    error_type_t e_res;

    ASSERT( NULL != context );

    e_res = FAIL;

    /* block waiting for resource complete */
    px_res = sem_trywait(&context->resource_done_semaphore);
    if ( 0 == px_res )
    {
        if ( ( context->dataChain == NULL) ||
             ( ( context->dataChain->atByte == context->dataChain->size) &&
               ( context->dataChain->next == NULL ) ) )
        {
            e_res = OK;
        }
    }

    return e_res;
}

/** 
 * \brief Get pointer to the first context of a specific type 
 * 
 * \param connection_type The type of connection's context to get
 *
 * \param context A memory location to be filled with the first context of
 * the type specified
 *
 * \return error_type_t:
 * OK upon success, context will be filled with a valid pointer
 *
 * HTTP_NO_CONTEXT if no context of the type specified was found
 *
 * This function is to be used as the first step in iterating over a list of
 * contexts for a specific type.
 * 
 * 
 **/
error_type_t http_context_get_first( http_ioconn_t connection_type, HttpPageContext ** context )
{
    error_type_t e_res;

    lock_contexts();

    ASSERT( NULL != context );

    e_res = OK;

    *context = activeContexts;

    /* skip the loop, so we return the failure */
    if ( NULL == *context )
    {
        e_res = HTTP_NO_CONTEXT;
    }

    while ( OK == e_res )
    {
        if ( connection_type == (*context)->connType )
        {
            e_res = HTTP_CONTEXT_FOUND;
        }
        else
        {
            *context = (*context)->nextContext;
        }

        /* get out of the loop if we are at the end of the list */
        if ( NULL == *context )
        {
            e_res = HTTP_NO_CONTEXT;
        }
    }

    if ( HTTP_CONTEXT_FOUND == e_res )
    {
        e_res = OK;
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Get pointer to the next context of a specific type 
 * 
 * \param context The current context caller is iterating over
 * This must be a context of the same type and returned through a call to
 * http_context_get_first() or this same function
 *
 * \param connection_type The type of connection's context to get
 *
 * \param next_context A memory location to be filled with the first context of
 * the type specified
 *
 * \return error_type_t:
 * OK upon success, context will be filled with a valid pointer
 *
 * HTTP_NO_CONTEXT if no more contexts of the type specified was found
 *
 * This function is to be used as the iteration step in iterating over a list of
 * contexts for a specific type.  First call http_context_get_first() in order
 * to get the initial iterator context.
 * 
 * 
 **/
error_type_t http_context_get_next( HttpPageContext * context,
        http_ioconn_t connection_type,
        HttpPageContext ** next_context )
{
    error_type_t e_res;

    lock_contexts();

    ASSERT( NULL != context );
    ASSERT( NULL != next_context );
    ASSERT( context->connType == connection_type );

    e_res = OK;

    *next_context = context->nextContext;

    /* skip the loop, so we return the failure */
    if ( NULL == *next_context )
    {
        e_res = HTTP_NO_CONTEXT;
    }

    while ( OK == e_res )
    {
        if ( connection_type == (*next_context)->connType )
        {
            e_res = HTTP_CONTEXT_FOUND;
        }
        else
        {
            *next_context = (*next_context)->nextContext;
        }

        /* get out of the loop if we are at the end of the list */
        if ( NULL == *next_context )
        {
            e_res = HTTP_NO_CONTEXT;
        }
    }

    if ( HTTP_CONTEXT_FOUND == e_res )
    {
        e_res = OK;
    }

    unlock_contexts();

    return e_res;
}


/* TODO Determine if this really needs to be exposed externally
 * and if we can remove the recursive calls. */

/** 
 * \brief Fill context with data to send
 *
 * \param context - current context, contains pointers to file
 * to send
 * 
 * \param addLastChunk - When finish adding data to context, do
 * you want the last chunk header added to context
 *
 * \return Returns 1 if context complete, 0 if more data to send
 *
 * Given an active web context, fills the context with data that
 * should be sent back to the requester.  Limits itself to
 * prevent too much memory from being allocated across the
 * system.
 * 
 **/
int32_t fillContextWithData( HttpPageContext *context, bool add_last_chunk, bool included_file )
{
    if ( context->state == e_http_state_body_complete )
    {
        return true;
    }
    else
    {
        if ( false == context->parseSSI )
        {
            addDataToHttpContext( context->pagePtr + context->atByte, context->pageSize - context->atByte, false, context, false );
            context->atByte += context->pageSize - context->atByte;
        }
        else
        {
            // Keep shoving data into the context until we've maxed out our memory
            while ( context->atByte < context->pageSize )
            {
                char *nextSSITag;

                // What's next? Page data or an SSI tag?
                // NOTE That strstr goes past the end of pages and into subsequent pages
                // because the pages are not NULL terminated.
                nextSSITag = strstri( context->pagePtr + context->atByte, SSITagStart );
                if ( nextSSITag != context->pagePtr + context->atByte )
                {
                    int bytesToSend;
                    // We have some page data to serve before the next SSI tag
                    if ( nextSSITag == NULL || ( nextSSITag > context->pagePtr + context->pageSize ) )
                    {
                        // The next tag isn't on this page
                        bytesToSend = context->pageSize - context->atByte;
                    }
                    else
                    {
                        // Compute the number of bytes before the next tag
                        bytesToSend = nextSSITag - ( context->pagePtr + context->atByte );
                    }
                    if ( context->outputEnabled == HTTP_OUTPUT_ENABLE )
                    {
                        addDataToHttpContext( context->pagePtr + context->atByte, bytesToSend, false, context, false );
                    }
                    context->atByte += bytesToSend;
                }
                else
                {
                    char *ssiTag, *endOfTag;
                    int   tagLength;
                    // We're at an SSI tag
                    endOfTag = strstri( nextSSITag, SSITagEnd );
                    ASSERT( endOfTag != NULL );
                    tagLength = endOfTag - nextSSITag + strlen( SSITagEnd );

                    // Create a buffer that contains ONLY the SSI tag.  This buffer is modified by subsequent calls.
                    ssiTag = ( char* )MEM_MALLOC_ALIGN( tagLength + LENGTH_OF_NULL,
                                                        cpu_get_dcache_line_size() );
                    memcpy( ssiTag, nextSSITag, tagLength );
                    ssiTag[ tagLength ] = '\0';

                    /* Replace the SSI Tag with the appropriate value */
                    context->outputEnabled = HttpSsiHandler( context, (const char *)ssiTag );
                    // Release the buffer containing the tag, since it has now been processed
                    MEM_FREE_AND_NULL( ssiTag );

                    // The tag has been added to the context, so move past it
                    context->atByte += tagLength;
                }
            }
        }

        if ( ( included_file == false ) && ( context->loc_of_content_length != 0 ) )
        {
            char *header_end;
            uint32_t length;
            char tmp_buf[LENGTH_OF_uint32_t + LENGTH_OF_NULL];

            // Determine location of fake data length
            context->loc_of_content_length = strstr( context->dataChain->data, HTTP_FAKE_CONTENT_LENGTH_STR );

            // Figure out the content length of the body data
            length = context->dataChain->size;
            header_end = strstr( context->dataChain->data, ENDOFHEADER );
            ASSERT( header_end != NULL );
            header_end += strlen( ENDOFHEADER ); // Advance past end of header marker
            length -= ( header_end - context->dataChain->data );
            sprintf( tmp_buf, "%d", (int)length );

            // Clear out fake value and copy in the real one
            memset( context->loc_of_content_length, ' ', LENGTH_OF_uint32_t - strlen( tmp_buf ) );
            memcpy( context->loc_of_content_length + LENGTH_OF_uint32_t - strlen( tmp_buf ), tmp_buf, strlen( tmp_buf ) );
            context->loc_of_content_length = 0;
        }

        // Did this round of filling the context get all the data for the page?
        if ( context->atByte >= context->pageSize )
        {
            context->state = e_http_state_body_complete;
            if ( add_last_chunk == true )
            {
                addDataToHttpContext( NULL, 0, false, context, true );
            }
        }
    }

    return ( e_http_state_body_complete == context->state );
}


/** 
 * \brief Given a filename, fill in the relevant info for the
 * web context
 *
 * \param filename - requested page
 * 
 * \param context - current context
 * 
 * \param includedFile - If true, this file is included as part 
 * of separate HTTP requestif request is valid and an HTTP OK 
 * response should not be added 
 *
 * \return success/failure - is page found
 *
 * Given a filename, fill in the relevant info for the web page
 * context.  Fills in the response header, determines MIME type,
 * etc.
 * 
 **/
bool getContextInfoForFilename( char *filename, HttpPageContext *context, bool includedFile )
{
    bool foundFile = false;

    foundFile = setPageValues( filename, context, includedFile);
    if ( foundFile == false )
    {
        if ( context->state != e_http_state_body_complete )
        {
            DBG_ERR( "Failed to find file: '%s' adding 404 Error\n", filename );
            // Return a file not found header
            addDataToHttpContext( HTTP_NOT_FOUND, strlen( HTTP_NOT_FOUND ), false, context, false );
            context->state = e_http_state_body_complete;
        }
    }
    else if ( includedFile == false )
    {
        int8_t contextTypeIndex = context->contextTypeIndex;
        char* header = ( char* ) MEM_MALLOC_ALIGN( strlen( HTTP_OK_FILE_LENGTH_1 ) +
                                                   strlen( HTTP_OK_FILE_LENGTH_2 ) +
                                                   strlen( HTTP_OK_FILE_LENGTH_3 ) +
                                                   strlen( HttpdContentTypes[ contextTypeIndex ] ) +
                                                   LENGTH_OF_NULL +
                                                   LENGTH_OF_uint32_t,
                                                   cpu_get_dcache_line_size() );
        // Put the header on the page context to be sent later.
        // Content length is fake for now - will get fixed after page is built
        sprintf( header, 
                 "%s%d%s%s%s", 
                 HTTP_OK_FILE_LENGTH_1, 
                 HTTP_FAKE_CONTENT_LENGTH_INT,
                 HTTP_OK_FILE_LENGTH_2,
                 HttpdContentTypes[ contextTypeIndex ], 
                 HTTP_OK_FILE_LENGTH_3 );
        addDataToHttpContext( header, strlen( header ), true, context, false );
        // Putting non-zero value in here as flag - actual location is determined when full
        // response has been added to context.  If calculate location here, reallocs can happen
        // later - thus moving buffer and pointer would be invalid
        context->loc_of_content_length = HTTP_FAKE_CONTENT_LENGTH_STR;
    }

    return foundFile;
}


/** 
 * \brief Determine if the complete HTTP header has been
 * received
 * 
 * \param context - current context
 * 
 * Checks the request and request length of the context to make
 * sure they have some data, then checks for the end of the HTTP
 * header ( /r/n/r/n )  If the end of header is found, the receive
 * complete flag is set in the context.
 * 
 **/
static void checkHeaderThenBeginParse( HttpPageContext* context )
{
    DBG_VERBOSE( "%s - enter, state=%d\n", __FUNCTION__, context->state );
    if ( context->state == e_http_state_initialized )
    {
        DBG_MSG( "checkHeader: NO - initialized\n" );
    }
    else if ( context->state == e_http_state_header_received )
    {
        DBG_MSG( "checkHeader: NO - header received\n" );
    }
    else
    {
        parseHeader( context );
    }
}



/** 
 * 
 * \brief Searches a character buffer for content length value
 * 
 * \param buffer - buffer to search
 * 
 * \param bufLen - size of buffer
 * 
 * \return int - content length
 * CHUNKED_ENCODING will be returned if this header
 * indicates this is a chunked encoding request
 * 
 * Searchs the character buffer for the HTTP content length
 * header, then converts it from a string to an integer.
 * Returns 0 upon any error
 * 
 */
static uint32_t http_internal_get_content_length( char* buffer,
                                                  uint32_t bufLen )
{
    uint32_t val;
    char *strLength;
    char *tmp_name;

    DBG_VERBOSE( "%s - enter\n", __FUNCTION__ );

    val = 0;

    if ( NULL != buffer )
    {
        strLength = strstri( buffer, HTTP_CONTENT_LENGTH );

        // Make sure they provided content length
        if ( strLength != NULL )
        {
            strLength += strlen( HTTP_CONTENT_LENGTH );

            // Move past any spaces separating the header from the actual length
            while ( ( strLength < buffer + bufLen ) && 
                    ( *strLength == ' ' ) )
            {
                strLength++;
            }
            val = (uint32_t)atoi( strLength );
        }

        // Check for chunked encoding
        // handled in uris who request it, but not pages
        // overwrite return value if this is a chunked encoding request
        tmp_name = strstri( buffer, HTTP_TRANSFER_ENCODING );
        if ( tmp_name != NULL )
        {
            tmp_name = skipWhiteSpace( tmp_name + strlen( HTTP_TRANSFER_ENCODING ) );
            if ( 0 == strncmp( "chunked", tmp_name, strlen( "chunked" ) ) )
            {
                val = CHUNKED_ENCODING;
            }
        }
    }

    DBG_VERBOSE( "%s - exit - length: %d\n", __FUNCTION__, val );

    return val;
}


/** 
 * \brief Add HTTP response header Method Not Allowed to context
 * 
 * \param context - context to add header
 * 
 * Add the HTTP response header "Method Not Allowed" to the
 * given context and mark context as complete
 * 
 **/
void addMethodNotAllowed( HttpPageContext* context )
{
    if ( context->state != e_http_state_body_complete )
    {
        uint8_t methods_supported;

        methods_supported = get_supported_methods( context->requested_uri );

        DBG_MSG( "Page Request: '%s' - Methods Allowed: %#x\n", context->requested_uri, methods_supported );

        if ( methods_supported == 0 )
        {
            DBG_ERR( "%s - adding 404\n", __FUNCTION__ );
            addDataToHttpContext( HTTP_NOT_FOUND, strlen( HTTP_NOT_FOUND ), false, context, false );
        }
        else
        {
            bool first_header = true;
            addDataToHttpContext( HTTP_METHOD_NOT_ALLOWED, strlen( HTTP_METHOD_NOT_ALLOWED ), false, context, false );
            if ( methods_supported & HTTP_GET_SUPPORT )
            {
                if ( first_header == false )
                {
                    addDataToHttpContext( COMMA_SPACE, strlen( COMMA_SPACE ), false, context, false );
                }
                addDataToHttpContext( GET_REQUEST, strlen( GET_REQUEST ), false, context, false );
                first_header = false;
            }
            if ( methods_supported & HTTP_PUT_SUPPORT )
            { 
                if ( first_header == false )
                {
                    addDataToHttpContext( COMMA_SPACE, strlen( COMMA_SPACE ), false, context, false );
                }
                addDataToHttpContext( PUT_REQUEST, strlen( PUT_REQUEST ), false, context, false );
                first_header = false;
            }
            if ( methods_supported & HTTP_POST_SUPPORT )
            { 
                if ( first_header == false )
                {
                    addDataToHttpContext( COMMA_SPACE, strlen( COMMA_SPACE ), false, context, false );
                }
                addDataToHttpContext( POST_REQUEST, strlen( POST_REQUEST ), false, context, false );
                first_header = false;
            }
            if ( methods_supported & HTTP_DELETE_SUPPORT )
            { 
                if ( first_header == false )
                {
                    addDataToHttpContext( COMMA_SPACE, strlen( COMMA_SPACE ), false, context, false );
                }
                addDataToHttpContext( DELETE_REQUEST, strlen( DELETE_REQUEST ), false, context, false );
                first_header = false;
            }
            if ( methods_supported & HTTP_HEAD_SUPPORT )
            { 
                if ( first_header == false )
                {
                    addDataToHttpContext( COMMA_SPACE, strlen( COMMA_SPACE ), false, context, false );
                }
                addDataToHttpContext( HEAD_REQUEST, strlen( HEAD_REQUEST ), false, context, false );
                first_header = false;
            }
            addDataToHttpContext( ENDOFHEADER, strlen( ENDOFHEADER ), false, context, false );
        }

        context->state = e_http_state_body_complete;
    }
}

error_type_t save_oid_translate_to_protocol( HttpPageContext *context, oidLink *link )
{
    uint32_t oid_val_len;
    error_type_t result;
    // Set new value
    if ( link->oid_val_str != NULL )
    {
        oid_val_len = strnlen( link->oid_val_str, OID_MAX_STRING_SIZE_IN_BYTES );
    }
    else
    {
        oid_val_len = 0;
    }

    // get the internal value for the oid and translate for 
    // the product specific protocol.
    if ( HTTP_TYPE_HTML == context->contextTypeIndex)
    {
        result = http_translate_html_protocol_to_oid( link->oid,
                                                      link->index, 
                                                      link->oid_val_str,
                                                      oid_val_len );
    }
    else if ( HTTP_TYPE_XML == context->contextTypeIndex )
    {
        result = http_translate_xml_protocol_to_oid( link->oid,
                                                     link->index,
                                                     link->oid_val_str,
                                                     oid_val_len );
    }
    else
    {
        result = FAIL;
    }

    return result;
}

error_type_t save_oid_values_string( HttpPageContext* context, oidLink *link )
{
    char oid_val_str[ OID_MAX_STRING_SIZE_IN_BYTES ];
    error_type_t result;

    #ifdef HAVE_WIRELESS
	uint32_t oid_val_len;
	uint32_t wps_pin_len = 8;
	oid_val_len= strnlen( link->oid_val_str, OID_MAX_STRING_SIZE_IN_BYTES );
    if (( link->oid == OID_WIFI_INTF_WPS_PIN ) && ( oid_val_len == wps_pin_len ) )
    {
        result = save_oid_translate_to_protocol( context, link );
    	return result;
    }
    #endif // HAVE_WIRELESS

    // Get the current values in case a later oid fails, we will need to restore values
    result = oid_get_string( link->oid, link->index, oid_val_str, OID_MAX_STRING_SIZE_IN_BYTES );
    if ( result == OK )
    {
        result = save_oid_translate_to_protocol( context, link );
    }

    if ( result == OK )
    {
        DBG_MSG( "Set oid: %#08X.%d\n", link->oid, link->index );
        strcpy( link->oid_val_str, oid_val_str );
    }
    else
    {
        DBG_ERR( "Failed in set of string oid: %#08X.%d = '%s' -- result: %d\n", link->oid, link->index, link->oid_val_str, result );
    }
    return result;
}


error_type_t save_oid_values_int( HttpPageContext* context, oidLink *link )
{
    uint32_t oid_val_int;
    error_type_t result;

    // Get the current values in case a later oid fails, we will need to restore values
    result = oid_get_uint32( link->oid, link->index, &oid_val_int );
    if ( result == OK )
    {
        result = save_oid_translate_to_protocol( context, link );
    }
    if ( result == OK )
    {
        DBG_MSG( "Set oid: %#08X.%d\n", link->oid, link->index );
        link->oid_val_int = oid_val_int;
    }
    else
    {
        DBG_ERR( "Failed in set of int oid: %#08X.%d = %d -- result: %d\n", link->oid, link->index, link->oid_val_int, result );
    }

    return result;
}


/** 
 * \brief Save oids in linked list
 * 
 * \param context - context that contains oid linked list
 * 
 * \result true for success, else false
 * 
 * Take a linked list of oids and save each one.  Before
 * overwritting any oid values, the current values are saved.
 * If there is an error saving and oid - the oids that were
 * previously updated are reverted to their original value and
 * false is returned from this function.
 * 
 **/
bool saveOidValues( HttpPageContext* context )
{
    oidLink *save_link = context->oidsToSave;
    oidLink *restore_link = context->oidsToSave;
    uint32_t oid_val_len;
    error_type_t result;
    uint8_t type;

    // Use an all or none approach to saving oids
    result = OK;
    while ( save_link != NULL )
    {
        type = oid_type_id_get( save_link->oid );

        switch ( type )
        {
            case OID_TYPE_STRING:
                result = save_oid_values_string( context, save_link );
                DBG_MSG( "Save string for oid: %#08X.%d = %d\n", save_link->oid, save_link->index, result );
                break;
            case OID_TYPE_UINT32:
                result = save_oid_values_int( context, save_link );
                DBG_MSG( "Save uint32 for oid: %#08X.%d = %d\n", save_link->oid, save_link->index, result );
                break;
            default:
                DBG_ERR( "%s Unhandled type: %d\n", __FUNCTION__, type );
                result = FAIL;
                break;
        }

        if ( result != OK )
        {
            break;
        }

        save_link = save_link->next;
    }

    // if an error occured during saving - need to resave oids with their original values
    if ( result != OK )
    {
        while ( ( restore_link != save_link ) && ( restore_link != NULL ) )
        {
            switch ( type )
            {
                case OID_TYPE_STRING:
                    oid_val_len = strnlen( restore_link->oid_val_str, OID_MAX_STRING_SIZE_IN_BYTES );
                    oid_set_string( restore_link->oid, restore_link->index, restore_link->oid_val_str, oid_val_len );
                    break;
                case OID_TYPE_UINT32:
                    oid_set_uint32( restore_link->oid, restore_link->index, restore_link->oid_val_int );
                    break;
                default:
                    break;
            }

            DBG_MSG( "Restored oid: %#08X.%d\n", restore_link->oid, restore_link->index );
            restore_link = restore_link->next;
        }
    }

    // Free all oid links - oids are saved and or restored now
    while ( context->oidsToSave != NULL )
    {
        save_link = context->oidsToSave;
        if ( save_link->oid_val_str )
        {
            MEM_FREE_AND_NULL( save_link->oid_val_str );
        }
        context->oidsToSave = context->oidsToSave->next;
        MEM_FREE_AND_NULL( save_link );
    }

    return result ? false : true;
}


/** 
 * \brief skip initial whitespace on string
 * 
 * \param str - input string
 * 
 * \result char* - string advanced past white space
 * 
 * Skip any initial spaces, tabs, newlines and carraige returns
 * 
 **/
char* skipWhiteSpace( char* str )
{
    while ( ( ( *str == ' ' ) ||
              ( *str == '\r' ) ||
              ( *str == '\n' ) ||
              ( *str == '\t' ) ) && 
            ( *str != '\0' ) )
    {
        str++;
    }
    return str;
}

/** 
 *
 * \brief Skip initial whitespace on fixed-size string
 * 
 * \param[in] input_str - input string, This value must not be NULL.
 *
 * \param[in] str_size_in_bytes - the number of characters to look at in the
 * string, must not be 0
 *
 * \param[out] output_string - The destination pointer for the output string
 * pointer.  *output_string will point to the input_string advanced past the
 * white space.  This value will never be NULL
 * 
 * \return The number of whitspace characters skipped.
 *
 * Skip any initial spaces, tabs, newlines and carraige returns
 * This function looks at the first str_size_in_bytes characters but never
 * beyond input_str + str_size_in_bytes.  Also will not look beyond the
 * NULL-terminating character
 * 
 */
static uint32_t string_n_skip_white_space( char * input_str,
        uint32_t str_size_in_bytes,
        char ** output_string )
{
    bool continue_skipping;
    uint32_t counter;

    continue_skipping = true;
    counter = 0;

    ASSERT( NULL != input_str );
    ASSERT( NULL != output_string );
    ASSERT( 0 != str_size_in_bytes );

    while ( true == continue_skipping )
    {
        if ( ( *input_str != '\0' ) &&
              ( ( *input_str == ' ' ) ||
               ( *input_str == '\r' ) ||
               ( *input_str == '\n' ) ||
               ( *input_str == '\t' ) ) )
        {
            counter++;
            input_str++;
        }
        else
        {
            continue_skipping = false;
        }

        if ( counter == str_size_in_bytes )
        {
            continue_skipping = false;
        }
    }

    *output_string = input_str;

    return counter;
}



/** 
 * \brief Given a filename that was part of an HTTP request,
 * return the index to the correct content type value
 * 
 * \param filename - name of file in HTTP request
 * 
 * \return char - index in array of HTTP content type values
 * 
 * Map the requested filename to the appropriate HTTP content
 * type and return the index into our array of these values.
 * The default type is plain text.
 * 
 **/
http_response_type_t getContextTypeIndex( char* filename )
{
    char* extension;

    // Find the start of the extension
    extension = strchr( filename, '.' );

    if ( extension == NULL )
    {
        return HTTP_TYPE_HTML;
    }

    // Advance past '.'
    extension++;

    // Determine which content type to use
    if ( 0 == strncmp( "htm", extension, strlen( "htm" ) ) )
    {
        return HTTP_TYPE_HTML;
    }
    else if ( 0 == strncmp( "xml", extension, strlen( "xml" ) ) )
    {
        return HTTP_TYPE_XML;
    }
    else if ( ( 0 == strncmp( "jpg", extension, strlen( "jpg" ) ) ) ||
              ( 0 == strncmp( "jpeg", extension, strlen( "jpeg" ) ) ) )
    {
        return HTTP_TYPE_JPEG;
    }
    else if ( 0 == strncmp( "gif", extension, strlen( "gif" ) ) )
    {
        return HTTP_TYPE_GIF;
    }
    else if ( 0 == strncmp( "png", extension, strlen( "png" ) ) )
    {
        return HTTP_TYPE_PNG;
    }
    else if ( 0 == strncmp( "exe", extension, strlen( "exe" ) ) )
    {
        return HTTP_TYPE_OCTET;
    }
    else if ( 0 == strncmp( "css", extension, strlen( "css" ) ) )
    {
        return HTTP_TYPE_CSS;
    }
    else if ( 0 == strncmp( "js", extension, strlen( "js" ) ) )
    {
        return HTTP_TYPE_JAVASCRIPT;
    }

    // If nothing else return plain
    return HTTP_TYPE_PLAIN;
}


/** 
 * \brief Given a filename update context to point to the file
 * in memory
 * 
 * \param context - current context
 * 
 * \param filename - file requested in HTTP request
 * 
 * \param includedFile - file is an included file from a
 * previous request - look at file path rather than url path in
 * searches
 * 
 * \return bool - success/failure
 * 
 * This function will look through the table(s) in the FW that
 * map the various EWS pages by comparing the provided filename
 * with the FW filenames.  When a match is found, the context's
 * pagePtr, pageSize and parseSSI values are updated.  If the
 * page is not found - false is returned.
 * 
 **/
bool setPageValues( char* filename, HttpPageContext * context, bool includedFile)
{
    int32_t fileID;
    bool foundFile = false;
    http_filename_mappings_t *curFile;
    char *comparisonFile;
    error_type_t e_res;
    http_config_t my_http_config;

    e_res = http_config_get( &my_http_config );
    if ( OK != e_res ) 
    {
        DBG_ERR( "Failed to get http_config - exiting\n" );
        return false;
    }

    if ( filename == NULL )
    {
        DBG_ERR( "No file name sent to setPageValues - exiting\n" );
        return false;
    }

    if ( ( strcmp( filename, "/" ) == 0 ) && ( NULL != my_http_config.default_root_page ) )
    {
        // Redirect to actual root page
        addDataToHttpContext( HTTP_MOVED_PERM_1, strlen( HTTP_MOVED_PERM_1 ), false, context, false );
        addDataToHttpContext( my_http_config.default_root_page, strlen( my_http_config.default_root_page ), false, context, false );
        addDataToHttpContext( HTTP_MOVED_PERM_2, strlen( HTTP_MOVED_PERM_2 ), false, context, false );
        context->state = e_http_state_body_complete;
        return false;
    }

    curFile = http_file_mappings;
    context->pagePtr = 0;

    while ( ( curFile->file_path != NULL ) && ( curFile->url_path != NULL ) )
    {
        if ( includedFile == true )
        {
            comparisonFile = curFile->file_path;
        }
        else
        {
            comparisonFile = curFile->url_path;
        }

        if ( strcmp( comparisonFile, filename ) == 0 )
        {
            char *realFilename = curFile->file_path;

            if ( ( context->connType != e_http_connection_usb ) && ( includedFile != true ) )
            {
                http_socket_config_t *socks = my_http_config.socket_table;
                int i = 0;
                bool supported_port = false;

                for ( i = 0; i < my_http_config.socket_table_size; i++ )
                {
                    // Check if this socket is supported by the current file requested
                    if ( ( ( 1 << i ) & curFile->supported_ports ) != 0 )
                    {
                        // Check if port number matches current connections port number
                        if ( context->port == socks[ i ].port_number )
                        {
                            supported_port = true;
                            break;
                        }
                    }
                }

                if ( supported_port == false )
                {
                    DBG_ERR( "File found but not supported on this port: %s\n", filename );
                    return false;
                }
            }

            for ( fileID=0; !foundFile && (fileID < http_rom_files); fileID++ )
            {
                if ( 0 == strncmp( http_rom_file_map[ fileID ].file_name,
                            realFilename,
                            strlen( http_rom_file_map[ fileID ].file_name ) ) )
                {
                    foundFile = true;
                    context->pagePtr = ( char* )( http_rom_page + http_rom_file_map[ fileID ].page_offset );
                    context->pageSize = http_rom_file_map[ fileID ].length;
                    context->contextTypeIndex = getContextTypeIndex( realFilename );
                    if ( ( context->contextTypeIndex == HTTP_TYPE_HTML ) || 
                         ( context->contextTypeIndex == HTTP_TYPE_JAVASCRIPT ) || 
                         ( context->contextTypeIndex == HTTP_TYPE_XML ) )
                    {
                        context->parseSSI = true;
                    }
                    else
                    {
                        context->parseSSI = false;
                    }
                    break;
                }
            }
            break;
        }
        curFile++;
    }

    return foundFile;
}


/** 
 * \brief Given a string, convert any url encoded characters
 * back to ascii
 * 
 * \param original - original string
 * 
 * \param final - pointer to buffer that will store the
 * converted string.  Must be at least as large as the original
 * string
 * 
 * \param bufSize - length of final buffer
 * 
 * Given a string such as "my%20network%40home" that is URL
 * encoded, convert this string back to the ascii equivalent of
 * "my network@home".  Scans the original string until a space,
 * new line, carraige return or null is found - or the final
 * string fills the buffer (based off of bufSize).
 * 
 **/
void urlDecode( char* original, char* final, uint32_t bufSize ) 
{
    char* cur;
    char* cur2;
    char tmpBuf[3];

    cur = original;
    cur2 = final;
    // Look through filename and convert any encoded letters to be their ASCII equivalent
    // For example %5D -> ']'
    while ( ( *cur != 0x0d ) && ( *cur != 0x0a ) && ( *cur != 0x00 ) )
    {
        if ( *cur != '%' )
        {
            *cur2++ = *cur++;
        }
        else
        {
            // Found an encoded letter
            // take next two letter and convert them to ints and save the results as a letter
            cur++;
            memcpy( tmpBuf, cur, 2 );
            tmpBuf[2] = 0;
            *cur2++ = strtol( tmpBuf, NULL, 16 );
            cur += 2;
        }

        // Check to see if the final buffer is full
        if ( cur2 == ( final + bufSize - LENGTH_OF_NULL ) )
        {
            break;
        }
    }
    *cur2 = 0;
}


/** 
 * \brief Process generic EWS pages that are posted 
 *
 * \param context - current context
 * 
 * Handle posted web page data (NOT XML). Process this data
 * appropriately.
 * 
 **/
error_type_t handleEwsPost( HttpPageContext* context )
{
    char* filename2;
    char* start;
    char* cur;
    uint32_t fileNameLen;
    uint32_t postDataLen;
    uint32_t i;
    uint32_t len;
    char *postData = context->request_body;

    if ( false == setPageValues( context->requested_uri, context, false ) )
    {
        return FAIL;
    }

    if ( ( postData == NULL ) || ( *postData == ( char ) 0 ) )
    {
        getContextInfoForFilename( context->requested_uri, context, false );
    }
    else
    {
        DBG_MSG( "POST was handed a page it doesn't recognize - must be html - '%s'\n", context->requested_uri );

        if ( context->cgi_args == NULL )
        {
            if ( FAIL == http_allocate_cgi_args( context ) )
            {
                return FAIL;
            }
        }

        cur = context->requested_uri;
        while ( ( *cur != 0x20) && ( *cur != 0x0d) && ( *cur != 0x0a) && ( *cur != 0x00 ) )
        {
            cur++;
        }
        *cur = 0;
        fileNameLen = cur - context->requested_uri + LENGTH_OF_NULL;
        filename2 = ( char* ) MEM_MALLOC_ALIGN( fileNameLen,
                                                cpu_get_dcache_line_size() );
        if ( filename2 != NULL )
        {
            urlDecode( context->requested_uri, filename2, fileNameLen );
        }
        else
        {
            return FAIL;
        }

        DBG_MSG( "Args = %s\n", postData );

        start = postData;
        cur = postData;
        while ( *cur != ( char ) 0 )
        {
            switch ( *cur )
            {
                case '?':
                case '&':
                    len = cur-start;
                    context->cgi_args->values[ context->cgi_args->cnt ] =(char*)MEM_MALLOC_ALIGN( len + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
                    strncpy( context->cgi_args->values[ context->cgi_args->cnt ], start, len );
                    context->cgi_args->values[ context->cgi_args->cnt ][ len ] = 0;
                    for ( i = 0; i < len; i ++ )
                    {
                        if ( context->cgi_args->values[ context->cgi_args->cnt ][ i ] == '+' )
                        {
                            context->cgi_args->values[ context->cgi_args->cnt ][ i ] = ' ';
                        }
                    }
                    start = ++cur;
                    context->cgi_args->cnt++;
                    break;
                case '=':
                    len = cur-start;
                    context->cgi_args->names[ context->cgi_args->cnt ] =(char*)MEM_MALLOC_ALIGN( len + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
                    strncpy( context->cgi_args->names[ context->cgi_args->cnt ], start, len );
                    context->cgi_args->names[ context->cgi_args->cnt ][ len ] = 0;
                    for ( i = 0; i < len; i ++ )
                    {
                        if ( context->cgi_args->names[ context->cgi_args->cnt ][ i ] == '+' )
                        {
                            context->cgi_args->names[ context->cgi_args->cnt ][ i ] = ' ';
                        }
                    }

                    start = ++cur;
                    break;
                default:
                    cur++;
                    break;
            }
        }
        if ( context->cgi_args->names[ context->cgi_args->cnt ] != NULL )
        {
            len = cur - start;
            context->cgi_args->values[ context->cgi_args->cnt ] =(char*)MEM_MALLOC_ALIGN( len + LENGTH_OF_NULL, cpu_get_dcache_line_size() );
            strncpy( context->cgi_args->values[ context->cgi_args->cnt ], start, len );
            context->cgi_args->values[ context->cgi_args->cnt ][ len ] = 0;
            for ( i = 0; i < len; i ++ )
            {
                if ( context->cgi_args->values[ context->cgi_args->cnt ][ i ] == '+' )
                {
                    context->cgi_args->values[ context->cgi_args->cnt ][ i ] = ' ';
                }
            }
            context->cgi_args->cnt++;
        }

        ASSERT( context->cgi_args->cnt < MAX_NUM_CGI_ARGS );

        for ( i = 0; i < context->cgi_args->cnt; i++ )
        {
            postDataLen = strlen( context->cgi_args->values[ i ] ) + LENGTH_OF_NULL;
            char *tmp = ( char * )MEM_MALLOC_ALIGN( postDataLen, cpu_get_dcache_line_size() );
            if ( tmp == NULL )
			{
				MEM_FREE_AND_NULL( filename2 );
				return FAIL;	
			}
            urlDecode( context->cgi_args->values[ i ], tmp, postDataLen );
            MEM_FREE_AND_NULL( context->cgi_args->values[ i ] );
            context->cgi_args->values[ i ] = tmp;
        }

        HttpCgiHandler( context, filename2, context->cgi_args->names, context->cgi_args->values, context->cgi_args->cnt );
        MEM_FREE_AND_NULL( filename2 );
    }

    return OK;
}


/** 
 * \brief Decode a base64 encoded string and return the result 
 * 
 * \param str - Base64 encoded string
 * 
 * \return char * - the decoded string - this memory has been
 * malloced and needs to be freed when finished
 * 
 * Decode a base64 encoded string.  The new string is malloced
 * and needs to be freed by the calling function
 * 
 **/
char *decode_base_64_string( char *str )
{
    // Decode the credentials ( they are base64 encoded ) 
    char    *ptrRead  = str;
    char    *ptrWrite = ( char * )MEM_MALLOC_ALIGN( strlen( str ),
                                                    cpu_get_dcache_line_size() );
    char    *result = ptrWrite;
    char    nextBits;
    char    curByte  = 0;
    uint32_t  bitCount = 0;

    memset( ptrWrite, 0x00, strlen( str ) );
    while ( ( nextBits = DecodeBase64char( *( ptrRead++ ) ) ) != ( char )( -1 ) )
    {
        switch ( bitCount )
        {
            case 0: 
                curByte = nextBits << 2; 
                bitCount = 6; 
                break;

            case 6: 
                *( ptrWrite++ ) = curByte | ( nextBits >> 4 ); 
                curByte = nextBits << 4; 
                bitCount = 4; 
                break;

            case 4: 
                *( ptrWrite++ ) = curByte | ( nextBits >> 2 ); 
                curByte = nextBits << 6; 
                bitCount = 2; 
                break;

            case 2: 
                *( ptrWrite++ ) = curByte | nextBits; 
                bitCount = 0; 
                break;

            default: 
                ASSERT( 0 && "Impossible state" );
        }
    }

    return result;
}



/** 
 * \brief Close socket and remove context information
 * 
 * \param context - context to delete
 * 
 * Tell all parties involved to close a current context.  This
 * involves closing the socket and freeing all allocated memory
 **/
static void closeConnection( HttpPageContext *context, bool reset_socket )
{
    oidLink* next;
    bool close_connection;

    ASSERT( NULL != context );

    close_connection = true;

    // Let the SSI handler know to free its state and release the context's memory
    HTMLReleaseState( context );

    if ( ( reset_socket == false ) && ( NULL != context->request_header ) )
    {
        if( NULL == strstri( context->request_header, HTTP_CONNECTION_CLOSE ) )
        {
            close_connection = false;
        }
    }

    if ( true == close_connection )
    {
        #ifdef HAVE_NETWORK
        /* TODO Move this down to the network parser layer */
        if ( context->connType == e_http_connection_network )
        {
            if ( reset_socket == true )
            {
#ifdef __linux__                    
                shutdown( ( intptr_t )context->deviceInfo, SHUT_RDWR );
#else                    
                tfResetConnection( ( intptr_t ) context->deviceInfo );
#endif                
            }
#ifdef __linux__                    
            close( ( intptr_t )context->deviceInfo );
#else                    
            tfClose( ( intptr_t )context->deviceInfo );
#endif                
            #ifdef HAVE_HTTPS
            ssl_free_context( (int32_t)context->deviceInfo );
            #endif // HAVE_HTTPS
        }
        #endif // HAVE_NETWORK
    }

    deleteDataChain( context );

    if ( context->request_header != NULL )
    {
        MEM_FREE_AND_NULL( context->request_header );
    }
    context->request_header_length = 0;

    if ( context->request_body != NULL )
    {
        MEM_FREE_AND_NULL( context->request_body );
    }
    context->request_body_length = 0;

    if ( context->requested_uri != NULL )
    {
        http_server_resource_node_t *head;

        head = http_server_resource_head;
        while ( head != NULL )
        {
            DBG_VERBOSE( "close compare: %s =?= %s\n", context->requested_uri, head->resource ? head->resource : "null" );
            if ( http_is_registered_resource( context->requested_uri, context->port, head ) == true )
            {
                DBG_VERBOSE( "Found match - state: %d\n", context->state );
                if ( ( context->state >= e_http_state_header_received ) && ( context->state < e_http_state_body_complete ) )
                {
                    if ( context->request_cb != NULL )
                    {
                        head->resource_cb( context->id, HTTP_RESOURCE_CLOSED, NULL, 0 );
                    }
                    else
                    {
                        head->event_cb( context->id, HTTP_RESOURCE_CONNECTION_ABORTED, head->usr_ctxt );
                    }
                }
                break;
            }
            head = head->next;
        }

        MEM_FREE_AND_NULL( context->requested_uri );
    }

    if ( context->xmlTag != NULL )
    {
        MEM_FREE_AND_NULL( context->xmlTag );
        context->xmlTag = NULL;
    }

    while ( context->oidsToSave != NULL )
    {
        if ( context->oidsToSave->oid_val_str != NULL )
        {
            MEM_FREE_AND_NULL( context->oidsToSave->oid_val_str );
        }
        next = context->oidsToSave->next;
        MEM_FREE_AND_NULL( context->oidsToSave );
        context->oidsToSave = next;        
    }

    if ( context->cgi_args != NULL )
    {
        uint32_t i;
        for ( i = 0; i < context->cgi_args->cnt; i++ )
        {
            MEM_FREE_AND_NULL( context->cgi_args->names[ i ] );
            MEM_FREE_AND_NULL( context->cgi_args->values[ i ] );
        }
        
        MEM_FREE_AND_NULL( context->cgi_args->names );
        MEM_FREE_AND_NULL( context->cgi_args->values );
        MEM_FREE_AND_NULL( context->cgi_args );
    }

    if ( context->partial_chunk != NULL )
    {
        MEM_FREE_AND_NULL( context->partial_chunk );
    }

    if ( ( true == close_connection ) || 
         ( context->connType == e_http_connection_usb ) )
    {
        int px_res;

        if ( activeContexts == context )
        {
            activeContexts = context->nextContext;
        }
        else
        {
            HttpPageContext *prevContext = activeContexts;
            while ( ( prevContext != NULL ) && ( prevContext->nextContext != context ) )
            {
                prevContext = prevContext->nextContext;
            }
            if ( prevContext != NULL )
            {
                prevContext->nextContext = context->nextContext;
            }
        }

        px_res = sem_destroy(&context->data_chain_semaphore);
        ASSERT(0 == px_res);
        
        px_res = sem_destroy(&context->resource_done_semaphore);
        ASSERT(0 == px_res);

        MEM_FREE_AND_NULL( context );
    }
    else
    {
        context->state                  = e_http_state_initialized;
        context->id                     = http_context_id++;
        context->request_cb             = NULL;
        context->event_cb               = NULL;
        context->num_buffers            = 0;
        context->pagePtr                = NULL;
        context->pageSize               = 0;
        context->atByte                 = 0;
        context->parseSSI               = false;
        context->outputEnabled          = HTTP_OUTPUT_ENABLE;
        context->content_length         = 0;
        context->request_data_read      = 0;
        context->lastAccess             = posix_gettime_ticks( );

        // Clients using registered resources are responsible for freeing their own data
        // If something is still here it is potentially a resource leak
        ASSERT( context->tmpData == NULL );
        context->partial_chunk_length   = 0;
        context->response_content_length= CHUNKED_ENCODING;
        context->response_length        = 0;

    }
}

/** 
 * \brief clean up first link in dataChain link
 * 
 * \param context - Entire page context structure for current
 * connection - will remove entire dataChain linked list from
 * this structure
 * 
 * Remove the first element from the data chain and free any
 * memory
 * 
 **/
void deleteFirstDataElement ( HttpPageContext *context )
{
    PageDataLink *oldLink;
    int px_res;
    px_res = sem_wait(&context->data_chain_semaphore);
    ASSERT(px_res == 0);

    oldLink = context->dataChain;
    // Write connect free usb data
    if ( ( context->connType != e_http_connection_usb ) &&
            (context->dataChain->data != NULL ) )
    {
        MEM_FREE_AND_NULL( context->dataChain->data );
    }
    context->dataChain = context->dataChain->next;
    px_res = sem_post(&context->data_chain_semaphore);
    ASSERT(px_res == 0);

    MEM_FREE_AND_NULL( oldLink );
}


/** 
 * \brief remove all elements of linked list of a dataChain
 * 
 * \param context  - Entire page context structure for current
 * connection - will remove entire dataChain linked list from
 * this structure
 * 
 * Function recieves a HttpPageContext structure and removes and
 * cleans up memory if needed of the structure's dataChain
 * 
 **/
static void deleteDataChain ( HttpPageContext *context )
{
    while ( context->dataChain != NULL )
    {
        deleteFirstDataElement( context );
    }
}


/** 
 * \brief Compare two tag paths, making the namespaces optional
 * 
 * \param user_tag - tag path provided from user's XML
 * 
 * \param comparison_tag - tag path that is being compared to
 * the user's tag path
 * 
 * \return bool: true = tags match else false
 * 
 * This function will compare two tag paths and determine if
 * they are pointing to the same xml elements.  The namespace
 * options in the user's path are optional - but if they are
 * present they must match the comparison tag's namespace
 * 
 **/
bool compare_tag_with_optional_namespace( char *user_tag, char *comparison_tag )
{
    char    *end_of_user_tag;
    char    *end_of_comparison_tag;
    char    *end_of_user_namespace;
    char    *end_of_comparison_namespace;
    bool tag_matches = true;

    while ( ( user_tag != NULL ) && ( tag_matches == true ) )
    {
        end_of_user_namespace = strchr( user_tag, ':' );
        end_of_comparison_namespace = strchr( comparison_tag, ':' );
        end_of_user_tag = strchr( user_tag, '|' );
        end_of_comparison_tag = strchr( comparison_tag, '|' );

        // Check if namespace in user tag
        if ( ( end_of_user_namespace == NULL ) ||
             ( ( end_of_user_tag != NULL ) && ( end_of_user_namespace > end_of_user_tag ) ) )
        {
            // Namespace not in current tag - skip to tag in comparison
            if ( end_of_comparison_namespace != NULL )
            {
                comparison_tag = ++end_of_comparison_namespace;
            }

            if ( strncmp( comparison_tag, user_tag, end_of_comparison_tag - comparison_tag ) != 0 )
            {
                tag_matches = false;
                break;
            }
        }
        else
        {
            // User provided namespace for this tag
            if ( strncmp( comparison_tag, user_tag, end_of_comparison_tag - comparison_tag ) != 0 )
            {
                tag_matches = false;
                break;
            }
        }

        if ( ( end_of_user_tag == NULL ) && ( end_of_comparison_tag == NULL ) )
        {
            user_tag = NULL;
        }
        else if ( ( end_of_user_tag == NULL ) || ( end_of_comparison_tag == NULL ) )
        {
            tag_matches = false;
        }
        else
        {
            user_tag = ++end_of_user_tag;
            comparison_tag = ++end_of_comparison_tag;
        }            
    }

    return tag_matches;
}


void lock_contexts()
{
    int px_res;
    px_res = pthread_mutex_lock(&http_context_protect_mutex);    
    ASSERT(px_res == 0);
}


void unlock_contexts()
{
    int px_res;
    px_res = pthread_mutex_unlock( &http_context_protect_mutex );
    ASSERT(px_res == 0);
}


/**
 * Look at buffer provided to determine if it contains a 
 * complete HTTP chunk description, return
 * the length of the chunk via the actual_length parameter.
 * 
 * \param buffer - buffer to look at
 * \param length - length of buffer
 *
 * \return bool true if complete, else false 
 *  
 * The incoming buffer must match either of the two formats below: 
 * \r\n[HEX VALUE]\r\n 
 * [HEX VALUE]\r\n 
 */
static bool http_have_full_chunk_info( char *buffer,
                                       uint32_t length )
{
    bool b_res;
    uint32_t at_byte = 0;

    b_res = true;

    if ( length == 0 )
    {
        b_res = false;
    }

    if ( true == b_res )
    {
        if ( NULL == buffer )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        // Look for \r\n
        // Look for hex value
        // Look for \r\n
        if ( *buffer == '\r' )
        {
            at_byte++;
            buffer++;

            if ( ( at_byte >= length ) || ( *buffer != '\n' ) )
            {
                b_res = false;
            }
        }
    }

    if ( true == b_res )
    {
        at_byte++;
        buffer++;
        if ( at_byte >= length )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        ASSERT( *buffer != '\n' );

        while ( ( true == b_res ) && 
                ( ( ( *buffer >= '0' ) && ( *buffer <= '9' ) ) ||
                  ( ( *buffer >= 'a' ) && ( *buffer <= 'f' ) ) ||
                  ( ( *buffer >= 'A' ) && ( *buffer <= 'F' ) ) ) )
        {
            at_byte++;
            buffer++;
            if ( ( at_byte >= length ) )
            {
                b_res = false;
            }
        }
    }

    if ( true == b_res )
    {
        if ( *buffer != '\r' )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        at_byte++;
        buffer++;

        if ( at_byte >= length ) 
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        if (*buffer != '\n' )
        {
            b_res = false;
        }
    }

    return b_res;
}


/**
 * Look at buffer provided to determine if it contains a 
 * complete HTTP chunk description, if so, return
 * the length of the chunk via the actual_length parameter.
 * 
 * \param buffer - buffer to look at
 * \param length - length of buffer
 * \param chunk_length - Size in bytes of the next chunk
 *
 * \return OK upon finding full chunk, FAIL otherwise
 *  
 * The incoming buffer must match either of the two formats below: 
 * \r\n[HEX VALUE]\r\n 
 * [HEX VALUE]\r\n 
 */
static error_type_t http_chunk_get_length( char **buffer,
                                           uint32_t *length,
                                           uint32_t *chunk_length )
{
    bool b_res;
    error_type_t e_res;
    uint32_t at_byte = 0;

    b_res = true;

    if ( length == 0 )
    {
        b_res = false;
    }

    if ( true == b_res )
    {
        if ( NULL == buffer )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        // Look for \r\n
        // Look for hex value
        // Look for \r\n
        if ( **buffer == '\r' )
        {
            at_byte++;
            (*buffer)++;

            if ( ( at_byte >= *length ) || ( **buffer != '\n' ) )
            {
                b_res = false;
            }

            if ( true == b_res )
            {
                // Covers the \n that follows the \r
                at_byte++;
                (*buffer)++;
                if ( at_byte >= *length )
                {
                    b_res = false;
                }
            }
        }
    }

    if ( true == b_res )
    {
        ASSERT( **buffer != '\n' );

        *chunk_length = strtol( *buffer, NULL, 16 );
        while ( ( true == b_res ) && 
                ( ( ( **buffer >= '0' ) && ( **buffer <= '9' ) ) ||
                  ( ( **buffer >= 'a' ) && ( **buffer <= 'f' ) ) ||
                  ( ( **buffer >= 'A' ) && ( **buffer <= 'F' ) ) ) )
        {
            at_byte++;
            (*buffer)++;
            if ( ( at_byte >= *length ) )
            {
                b_res = false;
            }
        }
    }

    if ( true == b_res )
    {
        if ( **buffer != '\r' )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        at_byte++;
        (*buffer)++;

        if ( at_byte >= *length ) 
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        if (**buffer != '\n' )
        {
            b_res = false;
        }
    }

    if ( true == b_res )
    {
        at_byte++;
        (*buffer)++;
    }

    if ( true == b_res )
    {
        e_res = OK;
    }
    else
    {
        e_res = FAIL;
    }

    if ( true == b_res )
    {
        *length -= at_byte;
    }
    
    DBG_VERBOSE( "%s exit - e_res = %d\n", __FUNCTION__, e_res );

    return e_res;
}

/** 
 * \brief Read the header data and start processing HTTP request
 * 
 * \param context - current context, contains HTTP request
 * 
 * Check for valid credetials on the request, determine what
 * type of HTTP request was received and hand off to the correct
 * handler function
 * 
 **/
static void parseHeader( HttpPageContext* context )
{
    bool validRequest = false;
    bool authorized = false;

    DBG_MSG( "Got request for url: %s on context: %#x\n", context->requested_uri, context->deviceInfo );

    authorized = HasValidCredentials( context->request_header, context->requested_uri );
    if ( !authorized )
    {
        DBG_MSG( "Page Request: '%s' - Not Authorized\n", context->request_header );
        addDataToHttpContext( HTTP_UNAUTHORIZED_1, strlen( HTTP_UNAUTHORIZED_1 ), false, context, false );
        addDataToHttpContext( HTTP_BASIC_REALM, strlen( HTTP_BASIC_REALM ), false, context, false );
        addDataToHttpContext( HTTP_UNAUTHORIZED_2, strlen( HTTP_UNAUTHORIZED_2 ), false, context, false );
        context->state = e_http_state_body_complete;
        return;
    }

    // Determine which type of HTTP request was received and hand off to correct function
    if ( 0 == strncmp( GET_REQUEST, context->request_header, strlen( GET_REQUEST ) ) )
    {
        validRequest = parseHttpGet( context, false );
    }
    else if ( 0 == strncmp( POST_REQUEST, context->request_header, strlen( POST_REQUEST ) ) )
    {
        validRequest = parseHttpPost( context );
    }
    else if ( 0 == strncmp( PUT_REQUEST, context->request_header, strlen( PUT_REQUEST ) ) )
    {
        validRequest = parseHttpPut( context );
    }
    else if ( 0 == strncmp( DELETE_REQUEST, context->request_header, strlen( DELETE_REQUEST ) ) )
    {
        validRequest = parseHttpDelete( context );
    }
    else if ( 0 == strncmp( HEAD_REQUEST, context->request_header, strlen( HEAD_REQUEST ) ) )
    {
        validRequest = parseHttpHead( context );
    }

    if ( !validRequest && ( context->state != e_http_state_body_complete ) )
    {
        DBG_MSG( "Page Request: '%s'\n", context->request_header );
        addDataToHttpContext( HTTP_NOT_FOUND, strlen( HTTP_NOT_FOUND ), false, context, false );
        context->state = e_http_state_body_complete;
    }
}


/** 
 * \brief Decodes a base64 character into the 6-byte value it represents.
 *
 * \param encodedVal - character to decode
 *
 * \return - char if successful, -1 if char is not legal
 *
 * Decodes a base64 character into the 6-byte value it
 * represents.
 * 
 **/
static char DecodeBase64char( char encodedVal )
{
    // "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    if ( encodedVal >= 'A' && encodedVal <= 'Z' ) return encodedVal - 'A';
    if ( encodedVal >= 'a' && encodedVal <= 'z' ) return encodedVal - 'a' + 26;
    if ( encodedVal >= '0' && encodedVal <= '9' ) return encodedVal - '0' + 52;
    if ( encodedVal == '+' )                      return 62;
    if ( encodedVal == '/' )                      return 63;
    return( char )-1;
}


/** 
 * \brief Check if user has credentials to requested page
 *
 * \param request - HTTP request
 *
 * \return success/failure
 *
 * Checks to see if the requested resource is protected.  If so,
 * verifies that the user has provided correct authentication
 * info.
 * 
 **/
static bool HasValidCredentials( char *request, char *url )
{
    char *hdrAuthInfo = NULL;
    char *username;
    char *password;
    char correct_password[ HTTP_VAR_PASSWORD_STRING_MAX_LEN ];
    bool authResult;
    error_type_t e_res;

    if ( http_methods_is_password_protected( request, url ) == false )
    {
        return true;
    }

    e_res = http_methods_get_current_password( request, correct_password, HTTP_VAR_PASSWORD_STRING_MAX_LEN );

    if ( OK != e_res )
    {
        authResult = false;
        goto HasValidCredentialsExit;
    }

    if ( strlen( correct_password ) == 0 )
    {
        authResult = true;
        goto HasValidCredentialsExit;

    }

    // Make sure they provided authentication info:
    hdrAuthInfo = strstri( request, HTTP_CLIENT_CREDENTIALS );
    if ( hdrAuthInfo == NULL )
    {
        authResult = false;
        goto HasValidCredentialsExit;
    }

    // They need to use 'basic' auth, since that's what we requested
    hdrAuthInfo = strstri( hdrAuthInfo, HTTP_EXPECTED_AUTH_TYPE );
    if ( hdrAuthInfo == NULL )
    {
        authResult = false;
        goto HasValidCredentialsExit;
    }

    // Move past auth type and whitespace to the actual data
    hdrAuthInfo += strlen( HTTP_EXPECTED_AUTH_TYPE );
    while ( *hdrAuthInfo == ' ' ) hdrAuthInfo++;

    username = decode_base_64_string( hdrAuthInfo );

    // Find the ':' separating the username and password
    password = strstri( username, ":" );

    // no ':' means we can't locate the password.  Don't authorize them.
    if ( password == NULL )
    {
        authResult = false;
    }
    else
    {
        password++;
        authResult = strcmp( correct_password, password ) == 0 ? true : false;
    }

    MEM_FREE_AND_NULL( username );

    HasValidCredentialsExit:

    return authResult;
}

/** 
 * \brief Add buffer of date to current context
 *
 * \param data - data to add
 * 
 * \param size - length of data
 * 
 * \param needsToBeFreed - has data been malloced?
 * 
 * \param context - current context
 * 
 * \param chunkHeader - add HTTP chuch header before data?
 *
 * Adds a buffer of data to the provided web context.
 * 
 **/
void addDataToHttpContext( char *data, uint32_t size, bool needs_to_be_freed, HttpPageContext *context, bool chunk_header )
{
    PageDataLink *chunk_link;
    uint32_t new_size;

    ASSERT( NULL != context );

    int px_res;
    px_res = sem_wait(&context->data_chain_semaphore);
    ASSERT(px_res == 0);

    chunk_link = context->dataChain;

    if ( chunk_link == NULL )
    {
        chunk_link = (PageDataLink *)MEM_MALLOC_ALIGN( sizeof( PageDataLink ),
                                                       cpu_get_dcache_line_size() );
        if ( chunk_link == NULL )
        {
            DBG_ERR( "Failed in malloc of PageDataLink - marking context as done\n" );
            context->state = e_http_state_body_complete;
            sem_post(&context->data_chain_semaphore);
            return;
        }
        chunk_link->atByte     = 0;
        chunk_link->data       = NULL;
        chunk_link->buff_size  = 0;
        chunk_link->size       = 0;
        chunk_link->next       = NULL;
        context->dataChain     = chunk_link;
    }

    if ( chunk_header == true )
    {
        if ( size == 0 )
        {
            new_size = size + strlen(HTTP_FINAL_CHUNK);
        }
        else
        {
            new_size = size + CHUNK_HEADER_SIZE;
        }
    }
    else
    {
        new_size = size;
    }

    if ( chunk_link->data == NULL )
    {
        DBG_VERBOSE( "Alloc memory for response\n" );
        chunk_link->data = ( char* ) MEM_MALLOC_ALIGN( HTTP_PARSER_BUF_LEN,
                                                       cpu_get_dcache_line_size() );
        chunk_link->buff_size = HTTP_PARSER_BUF_LEN;
    }
    else if ( chunk_link->size + new_size > chunk_link->buff_size )
    {
        while ( chunk_link->size + new_size > chunk_link->buff_size )
        {
            DBG_VERBOSE( "Relloc more memory (%d) for response\n", chunk_link->buff_size + HTTP_PARSER_BUF_LEN );
            chunk_link->data = ( char* ) MEM_REALLOC( chunk_link->data, chunk_link->buff_size + HTTP_PARSER_BUF_LEN );
            if ( chunk_link->data == NULL )
            {
                break;
            }
            chunk_link->buff_size += HTTP_PARSER_BUF_LEN;
        }
    }

    if ( chunk_link->data == NULL )
    {
        DBG_ERR( "Failed in malloc of output data buffer - marking context as done\n");
        context->state = e_http_state_body_complete;
        sem_post(&context->data_chain_semaphore);
        return;
    }

    if ( chunk_header == true )
    {
        if ( size == 0 )
        {
            char chunksize = strlen( HTTP_FINAL_CHUNK );
            memcpy( chunk_link->data + chunk_link->size, HTTP_FINAL_CHUNK, chunksize );
            chunk_link->size += chunksize;
        }
        else
        {
            minSprintf( chunk_link->data + chunk_link->size, "\r\n%06x\r\n", size );
            chunk_link->size += CHUNK_HEADER_SIZE;
        }
    }

    if ( NULL != data )
    {
        memcpy( chunk_link->data + chunk_link->size, data, size );
        chunk_link->size += size;
    }

    if ( needs_to_be_freed == true )
    {
        ASSERT( NULL != data );
        MEM_FREE_AND_NULL( data );
    }
    sem_post(&context->data_chain_semaphore);
}


/** 
 * \brief Initialize a context to send and receive data
 * 
 * \param context - context to initialize
 * 
 * \param deviceInfo - connection identifier ( socket number in
 * network )
 * 
 * \param conn - indicates USB or Network
 *
 * Initialize a page context with default values, these
 * parameters are referenced throughout the life of the context
 * to determine what needs to be displayed and how memory should
 * be used
 * 
 **/
static void initializeHttpPageContext( HttpPageContext *context, void *deviceInfo, http_ioconn_t conn )
{
    context->id                      = http_context_id++;
    context->state                   = e_http_state_initialized;
    context->request_cb              = NULL;
    context->event_cb                = NULL;
    context->num_buffers             = 0;
    context->blocking_send_cb        = NULL;
    context->atByte                  = 0;
    context->dataChain               = NULL;
    context->loc_of_content_length   = NULL;
    context->connType                = conn;
    context->deviceInfo              = deviceInfo;
    context->pagePtr                 = NULL;
    context->pageSize                = 0;
    context->parseSSI                = false;
    context->outputEnabled           = HTTP_OUTPUT_ENABLE;
    context->nextContext             = NULL;
    context->request_header          = NULL;
    context->request_header_length   = 0;
    context->cgi_args                = NULL;
    context->request_body            = NULL;
    context->request_body_length     = 0;
    context->lastAccess              = posix_gettime_ticks();
    context->xmlTag                  = NULL;
    context->oidsToSave              = NULL;
    context->tmpData                 = NULL;
    context->requested_uri           = NULL;
    context->content_length          = 0;
    context->request_data_read       = 0;
    context->contextTypeIndex        = HTTP_TYPE_HTML;
    context->partial_chunk           = NULL;
    context->partial_chunk_length    = 0;
    context->response_content_length = CHUNKED_ENCODING;
    context->response_length         = 0;

    switch( context->connType )
    {
        #ifdef HAVE_NETWORK
        case e_http_connection_network:
        {
            struct sockaddr_storage addr;
            memset(&addr, 0, sizeof(struct sockaddr_storage)); 
#ifdef __linux__
            socklen_t addr_len = sizeof( struct sockaddr_storage );
#else
           int addr_len = sizeof( struct sockaddr_storage );
#endif

            if ( getsockname( ( int )context->deviceInfo, ( struct sockaddr * )&addr, &addr_len ) != 0 )
            {
                DBG_ERR( "Failed to get sock name for new request\n" );
                ASSERT( 0 );
                context->port = 0;
            }
            else
            {
#ifdef __linux__
#ifdef HAVE_IPV6
               struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)&addr;
               context->port = ntohs( ip6addr->sin6_port );
#else // !HAVE_IPV6
                struct sockaddr_in *ip4addr = (struct sockaddr_in *)&addr;
                context->port = ntohs( ip4addr->sin_port );
#endif // HAVE_IPV6
#else
                context->port = ntohs( addr.ss_port );
#endif
            }

            break;
        }
        #endif
        case e_http_connection_usb:
        {
            http_usb_device_info_t *usbDeviceInfo = (http_usb_device_info_t *)deviceInfo;

            if ( ( usbDeviceInfo != NULL ) && ( IS_PTR_VALID( usbDeviceInfo ) ) )
            {
                // The USB device info struct will tell us which network port
                // this USB endpoint should look like (0 used as "all ports").
                context->port = usbDeviceInfo->network_port;
            }
            else
            {
                DBG_WARN( "USB connection failed to provide device info struct\n", context->connType );
                ASSERT(0);
                context->port = 0;
            }

            break;
        }

        default:
        {
            DBG_WARN( "Unexpected connection type: %d\n", context->connType );
            XASSERT("Unexpected connection type" == 0, context->connType);
            context->port = 0;

            break;
        }
    }
}


/** 
 * \brief Check requested URL to see which HTTP methods are
 * supported
 * 
 * \param resource - URL of current request
 * 
 * \return  Bitmapped value indicating which HTTP methods are
 * supported
 * 
 * This function compares the requested URL to a master list of
 * all supported URLs. If the URL is found, it returns a
 * bitmasked value indicating which HTTP headers it supports.
 * If the file is not in the master list, return 0.
 * 
 **/
uint8_t get_supported_methods( char *resource )
{
    uint32_t i = 0;

    while ( http_file_mappings[ i ].file_path != NULL )
    {
        DBG_VERBOSE( "Comparing: '%s' - '%s'\n", resource, http_file_mappings[ i ].url_path );
        if ( strstr( resource, http_file_mappings[ i ].url_path ) != NULL )
        {
            return http_file_mappings[ i ].supported_http_methods;
        }
        i++;
    }
    return 0;
}

#ifdef DEBUG
/** 
 * \brief Verify that not currently running on the allowed threads
 * 
 * \return true if running on different thread, else false
 * 
 * The HTTP server add data functions must NOT be called from
 * the context of the callback notifications.  If so, the all
 * connections in the web server could get blocked sending data
 * for a specific request.  This function compares the name of
 * the current thread against the names of the network and usb
 * threads
 * 
 * TODO This check is to protect API calls, but it goes
 * against the architecture heirarchy due to an intimate
 * understanding of the thread names.  It is for debug purposes
 * only.
 **/
static bool http_server_check_for_different_thread( void )
{
#if defined( WIRED_NETWORK ) || defined ( HAVE_USB_DEVICE )
    pthread_t current_tid = pthread_self();
#ifdef WIRED_NETWORK
    pthread_t network_tid = http_network_parser_get_tid();
#endif
#ifdef HAVE_USB_DEVICE
    pthread_t usb_tid = http_usbd_parser_get_tid();
#endif

    if(
#ifdef WIRED_NETWORK
        (pthread_equal(current_tid, network_tid))
#endif    
#if defined( WIRED_NETWORK ) && defined ( HAVE_USB_DEVICE )
        ||
#endif
#ifdef HAVE_USB_DEVICE
        (pthread_equal(current_tid, usb_tid))
#endif    
    )
    {
        DBG_ERR("Adding data to http request from within %s callback!!!!!!\n", pthread_equal(current_tid, network_tid)  ? "Network" : "USBD");
        ASSERT( 0 ) ;
        return false;
    }
#endif // #if defined( WIRED_NETWORK ) || defined ( HAVE_USB_DEVICE )
    return true;
}
#else
static inline bool http_server_check_for_different_thread( void ) {return true;}
#endif



/** 
 * \brief Add the HTTP status code to the response
 * 
 * \param request - handle to current request
 * 
 * \param code - HTTP status code enumeration
 * 
 * \return OK on success, else FAIL
 * 
 * This is the first call that should be made when responding to
 * an new request.  This will add the correct HTTP response.
 * 
 **/
error_type_t http_resource_set_status_code( http_request_t request,
        http_status_code_t code )
{
    HttpPageContext *context;
    char *response;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        XASSERT( context->state == e_http_state_body_received, context->state );
        if ( context->state != e_http_state_body_received )
        {
            DBG_ERR( "set status failed - wrong state - context->state: %d\n", context->state );
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        switch ( code )
        {
            case HTTP_200_OK:
                response = HTTP_STATUS_200;
                break;
            case HTTP_201_CREATED:
                response = HTTP_STATUS_201;
                break;
            case HTTP_202_ACCEPTED:
                response = HTTP_STATUS_202;
                break;
            case HTTP_204_NO_CONTENT:
                response = HTTP_STATUS_204;
                break;
            case HTTP_301_MOVED:
                response = HTTP_STATUS_301;
                break;
            case HTTP_304_NOT_MODIFIED:
                response = HTTP_STATUS_304;
                break;
            case HTTP_400_BAD_REQUEST:
                response = HTTP_STATUS_400;
                break;
            case HTTP_401_UNAUTHORIZED:
                response = HTTP_STATUS_401;
                break;
            case HTTP_403_FORBIDDEN:
                response = HTTP_STATUS_403;
                break;
            case HTTP_405_METHOD_NOT_ALLOWED:
                response = HTTP_STATUS_405;
                break;
            case HTTP_409_CONFLICT:
                response = HTTP_STATUS_409;
                break;
            case HTTP_500_SERVER_ERROR:
                response = HTTP_STATUS_500;
                break;
            case HTTP_503_SERVICE_UNAVAILABLE:
                response = HTTP_STATUS_503;
                break;
            default:
                ASSERT( HTTP_404_NOT_FOUND == code );
                response = HTTP_STATUS_404;
                break;
        }

        addDataToHttpContext( response, strlen( response ), false, context, false );
        addDataToHttpContext( HTTP_REQUIRED_FIELDS, strlen( HTTP_REQUIRED_FIELDS ), false, context, false );

        context->state = e_http_state_status_code_added;
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Add the HTTP type field to the headers
 * 
 * \param request - handle to current request
 * 
 * \param type - response type for request
 * 
 * \return OK on success, else FAIL
 * 
 * This function adds the HTTP content-type field to the current
 * request header response.  This function must not be called
 * before http_server_set_status_code.  It is recommended that
 * this function be called with each response, but not required.
 * 
 **/
error_type_t http_resource_set_content_type( http_request_t request,
                                             http_response_type_t type )
{
    HttpPageContext *context;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        //Removed to avoid crash
        //XASSERT( context->state == e_http_state_status_code_added, context->state );
        if ( context->state != e_http_state_status_code_added )
        {
            DBG_ERR( "set content type failed - wronge state - context->state: %d\n", context->state );
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        switch( type )
        {
            case HTTP_TYPE_HTML:
                /* purposely falling through */
            case HTTP_TYPE_XML:
                /* purposely falling through */
            case HTTP_TYPE_PLAIN:
                /* purposely falling through */
            case HTTP_TYPE_JPEG:
                /* purposely falling through */
            case HTTP_TYPE_GIF:
                /* purposely falling through */
            case HTTP_TYPE_PNG:
                /* purposely falling through */
            case HTTP_TYPE_SOAP_XML:
                /* purposely falling through */
            case HTTP_TYPE_OCTET:
                /* purposely falling through */
            case HTTP_TYPE_CSS:
                /* purposely falling through */
            case HTTP_TYPE_JAVASCRIPT:
                /* purposely falling through */
            case HTTP_TYPE_IPP:
                /* All acceptable content types */
                break;
            default:
                XASSERT( 0, type );
                DBG_ERR( "unknown content type - type: %d\n", type );
                e_res = FAIL;
                break;
        }
    }

    if ( OK == e_res )
    {

        addDataToHttpContext( HTTP_CONTENT_TYPE, strlen( HTTP_CONTENT_TYPE ), false, context, false );
        addDataToHttpContext( (char * )HttpdContentTypes[ type ], strlen( HttpdContentTypes[ type ] ), false, context, false );
        addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );

        context->state = e_http_state_type_added;
    }

    unlock_contexts();

    return e_res;
}

/**
 * \brief Add the HTTP Content-Length field to the headers
 *
 * \param request - handle to current request
 *
 * \param length - The size in bytes of the output response
 *
 * \return OK on success, else FAIL
 *
 * This function adds the HTTP content-length field to the 
 * current request header response.  This function must not be 
 * called before http_server_set_status_code.  By calling this 
 * function, HTTP chunking is turned off for the current 
 * response 
 *
 **/
error_type_t http_resource_set_content_length( http_request_t request, uint32_t length )
{
    #define CONTENT_LENGTH_STRING_LENGTH 20
    unsigned int string_size;
    error_type_t e_res;
    char length_str[ CONTENT_LENGTH_STRING_LENGTH ];
    HttpPageContext *context;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK)
    {
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        XASSERT( ( context->state == e_http_state_status_code_added ) || ( context->state == e_http_state_type_added ), context->state );
        if ( ( context->state != e_http_state_status_code_added ) &&
             ( context->state != e_http_state_type_added ) )
        {
            DBG_ERR( "set content length failed - wrong state - context->state: %d\n", context->state );
            e_res = FAIL;
        }
    }

    if ( e_res == OK )
    {
        e_res = min_n_k_sprintf( length_str,
                                 CONTENT_LENGTH_STRING_LENGTH,
                                 &string_size,
                                 "%d",
                                 length );
        ASSERT( string_size <= CONTENT_LENGTH_STRING_LENGTH );
        if ( DEBUG_STRING_CAPPED == e_res )
        {
            DBG_ERR( "Overflowed content length field\n" );
            e_res = FAIL;
        }
    }

    if ( e_res == OK )
    {
        addDataToHttpContext( HTTP_CONTENT_LENGTH, strlen( HTTP_CONTENT_LENGTH ), false, context, false );
        addDataToHttpContext( length_str, strlen( length_str ), false, context, false );
        addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );
    
        context->response_content_length = length;
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Add custom HTTP header
 * 
 * \param request - handle to current request
 * 
 * \param name - HTTP header name
 * 
 * \param name_size - string length of name
 * 
 * \param value - the value to associate with name
 * 
 * \param value_size - string length of value
 * 
 * \return OK on success, else FAIL
 * 
 * This function adds a custom HTTP header field to the current
 * request.  This function must NOT be called before a call to
 * http_server_set_status_code has been made.  The buffers for
 * name and value are NOT freed as part of this call.
 * 
 **/
error_type_t http_resource_add_header_field( http_request_t request,
        char *name,
        uint32_t name_size,
        char *value,
        uint32_t value_size )
{
    HttpPageContext *context;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        XASSERT( ( context->state == e_http_state_status_code_added ) || ( context->state == e_http_state_type_added ), context->state );
        if ( ( context->state != e_http_state_status_code_added ) &&
                ( context->state != e_http_state_type_added ) )
        {
            DBG_ERR( "add header field failed - wrong state - context->state: %d\n", context->state );
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        addDataToHttpContext( name, name_size, false, context, false );
        addDataToHttpContext( ": ", strlen( ": " ), false, context, false );
        addDataToHttpContext( value, value_size, false, context, false );
        addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Signal the HTTP header is complete and can be sent
 * 
 * \param request - handle to current request
 * 
 * \return OK on success, else FAIL
 * 
 * This function signals to the web server that the HTTP header
 * is now complete and can be sent.  This function can NOT be
 * called until after a call to http_server_set_status_code is
 * made.
 * 
 **/
error_type_t http_resource_header_complete( http_request_t request )
{
    HttpPageContext *context;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        context->lastAccess = posix_gettime_ticks();
        ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        if ( ( context->state == e_http_state_status_code_added ) ||
             ( context->state == e_http_state_type_added ) )
        {
            if ( context->response_content_length == CHUNKED_ENCODING )
            {
                // Add chunk header before sending out header
                addDataToHttpContext( HTTP_TRANSFER_CHUNKED, strlen( HTTP_TRANSFER_CHUNKED ), false, context, false );
            }
            else
            {
                // Add extra \r\n to signal end of header
                addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );
            }

            if (context->request_cb != NULL)
            {
                e_res = context->blocking_send_cb( context,
                                                   context->dataChain->data,
                                                   context->dataChain->size,
                                                   true );
                context->dataChain->data = NULL; // Memory was already freed by blocking_send_cb
                deleteDataChain( context );
            }
            context->state = e_http_state_header_complete;
        }
        else
        {
            DBG_ERR( "Send header failed - wrong state - context->state: %d\n", context->state );
            e_res = FAIL;
        }
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Create an HTTP chunk header in heap space
 * 
 * \param size - size in chunk header
 * 
 * \return char* allocated memory with chunk header - NULL if
 * malloc fails
 * 
 * This function will allocate memory and then fill this memory
 * with the correct chunk header for the given chunk size.  If
 * the size is 0 - then the special transfer termination header
 * is created.
 * 
 **/
static char *http_create_chunk_header( uint32_t size )
{
    char *buffer;

    if ( size == 0 )
    {
        buffer = MEM_MALLOC_ALIGN( strlen( HTTP_FINAL_CHUNK ),
                                   cpu_get_dcache_line_size() );
        if ( buffer != NULL )
        {
            memcpy( buffer, HTTP_FINAL_CHUNK, strlen( HTTP_FINAL_CHUNK ) );
        }
    }
    else
    {
        buffer = MEM_MALLOC_ALIGN( CHUNK_HEADER_SIZE + LENGTH_OF_NULL,
                                   cpu_get_dcache_line_size() );
        if ( buffer != NULL )
        {
            minSprintf( buffer, "\r\n%06x\r\n", size );
        }
    }
    return buffer;
}


/** 
 * \brief Send body data for the request
 * 
 * \param request - handle to request information
 * 
 * \param buffer - data that should be sent, buffer will be
 * freed when it is sent
 * 
 * \param size - how big the buffer is
 * 
 * \return OK on success, else FAIL
 * 
 * This function allows to send data for the current request.
 * This data is in the body of the HTTP response.  This is a
 * blocking call while the send is occuring.  The buffer will be
 * freed by this function.
 * 
 **/
error_type_t http_resource_send_data( http_request_t request, char *buffer, uint32_t size )
{
    HttpPageContext *context;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        DBG_ERR( "%s Failed to map id to context - id: %d\n", __FUNCTION__, request );
        e_res = FAIL;
    }

    if ( ( e_res == OK ) && ( context->request_cb == NULL ) )
    {
        DBG_ERR( "%s should not be called except on HTTP requests that were registered with a call to http_register_resource\n", __FUNCTION__ );
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        context->lastAccess = posix_gettime_ticks();
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        if ( context->state == e_http_state_header_complete ) 
        {
            if ( ( context->response_content_length != CHUNKED_ENCODING ) &&
                 ( context->response_length + size > context->response_content_length ) )
            {
                DBG_ERR( "%s - Content length of %d bytes was specified but trying to send %d bytes\n", __FUNCTION__, context->response_content_length, context->response_length + size );
                e_res = FAIL;
            }

            if ( ( e_res == OK ) &&
                 ( context->response_content_length == CHUNKED_ENCODING ) )
            {
                char * chunk_header;
        
                chunk_header = http_create_chunk_header( size );
        
                if ( NULL != chunk_header )
                {
                    e_res = context->blocking_send_cb( context,
                                                       chunk_header,
                                                       CHUNK_HEADER_SIZE,
                                                       true );
                }
                else
                {
                    e_res = FAIL;
                }
            }
    
            if ( OK == e_res )
            {
    
                e_res = context->blocking_send_cb( context,
                                                   buffer,
                                                   size,
                                                   true );
                if ( ( e_res == OK ) &&
                     ( context->response_content_length != CHUNKED_ENCODING ) )
                {
                    context->response_length += size;
                }
            }
        }
        else
        {
            e_res = FAIL;
            DBG_ERR( "Send data failed - wrong state - context->state: %d\n", context->state );
        }
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Send body data for the request
 * 
 * \param request - handle to request information
 * 
 * \param buffer - data that should be sent, buffer will NOT be
 * freed 
 * 
 * \param size - how big the buffer is
 * 
 * \return OK on success, else FAIL
 * 
 * This function allows to send data for the current request.
 * This data is in the body of the HTTP response.  This is a
 * blocking call while the send is occuring.  The buffer will 
 * NOT be freed by this function. 
 * 
 **/
error_type_t http_resource_send_data_no_free( http_request_t request, char *buffer, uint32_t size )
{
    HttpPageContext *context;
    error_type_t e_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        DBG_ERR( "%s Failed to map id to context - id: %d\n", __FUNCTION__, request );
        e_res = FAIL;
    }

    if ( ( e_res == OK ) && ( context->request_cb == NULL ) )
    {
        DBG_ERR( "%s should not be called except on HTTP requests that were registered with a call to http_register_resource\n", __FUNCTION__ );
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        context->lastAccess = posix_gettime_ticks();
        DBG_ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        if ( context->state == e_http_state_header_complete ) 
        {
            if ( ( context->response_content_length != CHUNKED_ENCODING ) &&
                 ( context->response_length + size > context->response_content_length ) )
            {
                DBG_ERR( "%s - Content length of %d bytes was specified but trying to send %d bytes\n", __FUNCTION__, context->response_content_length, context->response_length + size );
                e_res = FAIL;
            }

            if ( ( e_res == OK ) &&
                 ( context->response_content_length == CHUNKED_ENCODING ) )
            {
                char * chunk_header;
        
                chunk_header = http_create_chunk_header( size );
        
                if ( NULL != chunk_header )
                {
                    e_res = context->blocking_send_cb( context,
                                                       chunk_header,
                                                       CHUNK_HEADER_SIZE,
                                                       true );
                }
                else
                {
                    e_res = FAIL;
                }
            }
    
            if ( OK == e_res )
            {
    
                e_res = context->blocking_send_cb( context,
                                                   buffer,
                                                   size,
                                                   false );
                if ( ( e_res == OK ) &&
                     ( context->response_content_length != CHUNKED_ENCODING ) )
                {
                    context->response_length += size;
                }
            }
        }
        else
        {
            e_res = FAIL;
            DBG_ERR( "Send data failed - wrong state - context->state: %d\n", context->state );
        }
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Notifies web server that request has been filled
 * 
 * \param request - handle to request information
 * 
 * \return OK on success, else FAIL
 * 
 * After the appropriate HTTP response has been sent, this
 * function notifies the web server that the request is
 * finished.  This allows to correctly terminate the request so
 * the requestor can process the response.
 * 
 **/
error_type_t http_resource_send_complete( http_request_t request )
{
    HttpPageContext *context;
    error_type_t e_res;
    int px_res;

    lock_contexts();

    e_res = OK;

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        context->lastAccess = posix_gettime_ticks();
        ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        if ( context->request_cb != NULL )
        {
            // Verify all memory has been freed
            XASSERT( context->num_buffers == 0, context->num_buffers ); 
        }

        // If content length was specified, verify that all data has been sent
        if ( ( context->response_content_length != CHUNKED_ENCODING ) &&
             ( context->response_length != context->response_content_length ) )
        {
            DBG_ERR( "Response specified content length was %d bytes but only send %d bytes\n", context->response_content_length, context->response_length );
            ASSERT( context->response_length == context->response_content_length );
        }

        if ( context->state == e_http_state_header_complete )
        {
            if ( context->response_content_length == CHUNKED_ENCODING )
            {
                char * chunk_header;
        
                // add chunk header
                chunk_header = http_create_chunk_header( 0 );
                if ( NULL != chunk_header )
                {
                    if ( context->request_cb != NULL )
                    {
                        e_res = context->blocking_send_cb( context,
                                                           chunk_header,
                                                           CHUNK_HEADER_SIZE,
                                                           true );
                    }
                    else
                    {
					    // Event CB case
                        e_res = http_add_data_to_chain( context, chunk_header, CHUNK_HEADER_SIZE );
                    }
                }
                else
                {
                    e_res = FAIL;
                }
            }
    
            context->state = e_http_state_body_complete;
        }
        else
        {
            DBG_ERR( "Send finished failed - wrong state - context->state: %d\n", context->state );
            e_res = FAIL;
        }

        /* unblock transport layers waiting for resource complete signal */
        px_res = sem_post(&context->resource_done_semaphore);
        ASSERT(px_res == 0);
    }

    unlock_contexts();

    return e_res;
}


/** 
 * \brief Register a resource with the web server
 * 
 * \param resource - string pointer to resource name
 * 
 * \param callback - callback function when resource is
 * requested
 * 
 * \return OK for success - else FAIL
 * 
 * This function registers a resource with the web server.  When
 * the provided resource is requested, the callback function
 * will be notified.
 * 
 **/
error_type_t http_resource_register( char *resource,
                                     int32_t port,
                                     http_resource_event_cb_t callback )
{
    http_server_resource_node_t *node;
    error_type_t e_res;

    ASSERT( NULL != callback );

    lock_contexts();

    e_res = OK;

    DBG_MSG( "Resource register request: %s - port: %d\n", resource ? resource : "null", port );

    if ( ( resource == NULL) && ( port == 0 ) )
    {
        DBG_ERR( "Invalid register request - must specify port and/or resource\n" );
        ASSERT( 0 );
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        node = http_server_resource_head;
        while ( node != NULL )
        {
            if ( http_is_resource_already_registered( resource, port, node ) == true )
            {
                DBG_MSG( "Resource register request: %s -- ALREADY EXISTS\n", resource ? resource: "null", port );
                e_res = FAIL;
                break;
            }
    
            node = node->next;
        }
    }

    if ( OK == e_res )
    {
        /* create a new node and register events */
        node = ( http_server_resource_node_t * )MEM_MALLOC_ALIGN( sizeof( http_server_resource_node_t ),
                                                                  cpu_get_dcache_line_size() );
        if( node == NULL )
        {
            /* NO MEMORY! */
            DBG_ERR( "Not enough memory for resource register!\n" );
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        node->resource_cb = callback;
        node->event_cb = NULL;
        node->resource = resource;
        node->port     = port;
        node->usr_ctxt = NULL;

        /* insert at beginning of linked list */
        node->next = http_server_resource_head;
        http_server_resource_head = node;
        DBG_MSG( "Resource register request: %s - port: %d - ADDED\n", resource ? resource : "null", port );

        #ifdef HAVE_NETWORK
        if( port != 0 )
        {
            // Tell network code to open TCP socket and start to listen for new connections
            http_network_add_port( port, HTTP_NET_SOCKET_TCP );
        }
        #endif // HAVE_NETWORK
    }

    unlock_contexts();

    return e_res;
}

error_type_t http_resource_deregister( char *resource, int32_t port )
{
    http_server_resource_node_t *node;
    http_server_resource_node_t *prev_node;

    DBG_VERBOSE( "Request to remove node: %s - %d\n", resource ? resource : "null", port );

    lock_contexts();
    prev_node = NULL;
    node = http_server_resource_head;
    while ( node != NULL )
    {
        DBG_VERBOSE( "Checking for node to remove: %s - %d\n", node->resource ? node->resource : "null", node->port );
        if ( http_is_resource_already_registered( resource, port, node ) == true )
        {
            DBG_MSG( "De-register request: %s - port: %d\n", resource ? resource : "null", port );

            if ( node == http_server_resource_head )
            {
                http_server_resource_head = node->next;
            }
            else
            {
                prev_node->next = node->next;
            }

            #ifdef HAVE_NETWORK
            if( port != 0 )
            {
                http_server_resource_node_t *cur_node;
                bool remove_port = true;
                cur_node = http_server_resource_head;
                // Check if any other registered callbacks use this port
                // If not - then tell the network code to close it
                while ( cur_node != NULL )
                {
                    if ( cur_node->port == node->port )
                    {
                        remove_port = false;
                        break;
                    }
                    cur_node = cur_node->next;
                }
                if ( remove_port == true)
                {
                    http_network_remove_port( port, HTTP_NET_SOCKET_TCP );
                }
            }
            #endif // HAVE_NETWORK

            MEM_FREE_AND_NULL( node );
            unlock_contexts();
            return OK;
        }
        prev_node = node;
        node = node->next;
    }
    unlock_contexts();

    DBG_ERR( "Failed to find node to de-register\n" );
    // Failed to find resource - return failure
    return FAIL;
}

error_type_t http_resource_buffer_free( http_request_t request, char *buffer )
{
    HttpPageContext *context;
    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL ) 
    {
        unlock_contexts();
        return FAIL;
    }

    if ( context->request_cb == NULL )
    {
        DBG_ERR( "%s should not be called except on HTTP requests that were registered with a call to http_register_resource\n", __FUNCTION__ );
        unlock_contexts();
        return FAIL;
    }

    context->num_buffers--;
    MEM_FREE_AND_NULL( buffer );
    unlock_contexts();
    return OK;
}

#ifdef HAVE_NETWORK
/** 
 * \brief Retrieves the peer address information for an HTTP
 * connection
 * 
 * \param request - handle to request information
 * 
 * \param addr - buffer for peer address
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to gather the peer address information for an
 * HTTP request.  The buffer for addr must be allocated by the
 * calling function.
 * 
 **/
error_type_t http_resource_get_peer_addr( http_request_t request, struct sockaddr_storage *addr )
{
    HttpPageContext *context;
    error_type_t e_res = OK;
#ifdef __linux__    
    socklen_t addr_length = sizeof(struct sockaddr_storage);
#else
    int addr_length = sizeof(struct sockaddr_storage);
#endif

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        if ( ( context->connType != e_http_connection_network ) || 
             ( getpeername( (intptr_t)context->deviceInfo, (void *)addr, &addr_length ) != 0 ) )
        {
            e_res = FAIL;
        }
    }
    unlock_contexts();
    return e_res;
}


/** 
 * \brief Retrieves the sock address information for an HTTP
 * connection
 * 
 * \param request - handle to request information
 * 
 * \param addr - buffer for sock address
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to gather the sock address information for an
 * HTTP request.  The buffer for addr must be allocated by the
 * calling function.
 * 
 **/
error_type_t http_resource_get_sock_addr( http_request_t request, struct sockaddr_storage *addr )
{
    error_type_t e_res = OK;
    HttpPageContext *context;
#ifdef __linux__    
    socklen_t addr_length = sizeof(struct sockaddr_storage);
#else
    int addr_length = sizeof(struct sockaddr_storage);
#endif

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        if ( ( context->connType != e_http_connection_network ) || 
    		 ( getsockname( (intptr_t)context->deviceInfo, (void *)addr, &addr_length ) != 0 ) )
        {
            e_res = FAIL;
        }
    }
    unlock_contexts();
    return e_res;
}
#endif // HAVE_NETWORK


/** 
 * \brief This function allows a module to fetch the requested resource
 * without having to parse the HTTP header
 * 
 * \param request - handle to request information
 * 
 * \param resource - pointer to pass back the URI
 * 
 * \return OK on success, else FAIL
 * 
 * This function allows a module to fetch the requested resource
 * without having to parse the HTTP header
 * 
 **/
error_type_t http_resource_get_requested_resource( http_request_t request, char **resource )
{
    error_type_t e_res = OK;
    HttpPageContext *context;
    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        DBG_ERR( "%s - failed to find context for request: %d\n", __FUNCTION__, request );
        e_res = FAIL;
    }
    else
    {
        *resource = context->requested_uri;
    }
    unlock_contexts();
    return e_res;
}

/** 
 * \brief Allows module to store a void* with the request header
 * 
 * \param request - handle to request information
 * 
 * \param data - void * to save
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to store some data with the request header to
 * be retrieved later.  Helps modules keep track of their
 * structures.
 * 
 **/
error_type_t http_resource_set_data_ptr( http_request_t request, void *data )
{
    error_type_t e_res = OK;
    HttpPageContext *context;

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }
    else
    {
        context->tmpData = data;
    }
    unlock_contexts();
    return e_res;
}

/** 
 * \brief Allows module to get a void* that was previously saved
 * by the module
 * 
 * \param request - handle to request information
 * 
 * \param data - place to save retrieved data
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to retrieve some data that it had previosly
 * saved in the request header. Helps modules keep track of
 * their structures.
 * 
 **/
error_type_t http_resource_get_data_ptr( http_request_t request, void **data )
{
    error_type_t e_res = OK;
    HttpPageContext *context;

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }
    else
    {
        *data = context->tmpData;
    }
    unlock_contexts();
    return e_res;
}

/** 
 * \brief Allows module to get any URL parameters
 * 
 * \param request - handle to request information
 * 
 * \param params - place to save URL parameters
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to retrieve the URL parameters that were
 * sent.  These will be in name/value pairs.  The value of
 * params will be NULL if no parameters were received.
 * 
 **/
error_type_t http_resource_get_resource_params( http_request_t request, http_request_cgi_args_t **params)
{
    HttpPageContext *context;
    error_type_t e_res = OK;

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }
    else
    {
        if (  context->cgi_args == NULL )
        {
            DBG_MSG( "No cgi args\n" );
            e_res = HTTP_ERROR_NO_RESULTS;
        }
        else
        {
            *params = context->cgi_args;
        }
    }
    unlock_contexts();
    return e_res;
}

bool http_is_resource_already_registered( char *request, int32_t port, http_server_resource_node_t *node )
{
    if ( node->port == port )
    {
        if ( ( request == NULL ) && ( node->resource == NULL ) )
        {
            return true;
        }
        else if ( ( ( request == NULL ) && ( node->resource != NULL ) ) ||
                  ( ( request != NULL ) && ( node->resource == NULL ) ) )
        {
            return false;
        }
        else if ( strcmp( node->resource, request ) == 0 )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool http_is_registered_resource( char *request, int32_t port, http_server_resource_node_t *node )
{
    DBG_VERBOSE( "%s: request: '%s' - port: %d - node request: '%s' - port: %d\n", __FUNCTION__, request, port, node->resource ? node->resource : "null", node->port );
    if ( node->port == 0 )
    {
        // Only have to match resource
        if ( ( strcmp( node->resource, request ) == 0 ) ||
             ( ( node->resource[ strlen( node->resource ) - 1 ] == '*' ) &&
               ( strncmp( node->resource, request, strlen( node->resource ) - 1 ) == 0 ) ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ( node->resource == NULL )
    {
        // Only have to match port
        if ( port == node->port )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Both are set so both must match
        // OR: we are coming in over USB (context port is 0), in which case only resource needs to match
        if ( ( ( node->port == port ) || ( port == 0 )) &&
             (
              ( strcmp( node->resource, request ) == 0 ) ||
              ( ( node->resource[ strlen( node->resource ) - 1 ] == '*' ) &&
                ( strncmp( node->resource, request, strlen( node->resource ) - 1 ) == 0 ) )
             ) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


HttpPageContext *getHttpContextFromId( http_request_t id )
{
    HttpPageContext *cur = activeContexts;
    while ( cur != NULL )
    {
        if ( ( cur->id == id ) && ( cur->state != e_http_state_resetting ) )
        {
            return cur;
        }
        cur = cur->nextContext;
    }

    DBG_ERR( "%s invalid handle: %d\n", __FUNCTION__, id );
//    ASSERT( 0 );
    return NULL;
}

error_type_t http_resource_send_100_continue( http_request_t request )
{
    HttpPageContext *context;
    error_type_t e_res;

    e_res = OK;
    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        context->lastAccess = posix_gettime_ticks();
        ASSERT( http_server_check_for_different_thread() ); // Check to make sure not being called back on my thread
        DBG_MSG( "Sending data to: %d - state: %d\n", request, context->state );
        e_res = context->blocking_send_cb( context,
                                           HTTP_STATUS_100_COMPLETE,
                                           strlen( HTTP_STATUS_100_COMPLETE ),
                                           false );
        DBG_MSG( "Done sending data to: %d\n", request );
    }

    unlock_contexts();
    return e_res;
}

#ifdef HAVE_NETWORK
/** 
 * \brief Retrieves the address of the server accepting this request
 * HTTP connection
 * 
 * \param request - handle to request information
 * 
 * \param addr - buffer for this server address
 * 
 * \return OK on success, else FAIL
 * 
 * Allows a module to gather the server address information for an
 * HTTP request.  The buffer for addr must be allocated by the
 * calling function.
 * 
 **/
error_type_t http_resource_get_server_addr( http_request_t request, 
                                            struct sockaddr_storage *addr )
{
    HttpPageContext *context;

    lock_contexts();
    context = getHttpContextFromId( request );
    error_type_t e_res;
#ifdef __linux__    
    socklen_t addr_length;
#else
    int addr_length;
#endif
    int i_res;

    ASSERT( context != NULL );

    e_res = OK;

    if ( context->connType == e_http_connection_network )
    {
        addr_length = sizeof(struct sockaddr_storage);

        i_res = getsockname( (intptr_t)context->deviceInfo,
                             (void *)addr,
                             &addr_length );

        if ( 0 != i_res )
        {
            memset( addr, 0, sizeof( struct sockaddr_storage ) );
            e_res = FAIL;
        }
    }
    else
    {
        memset( addr, 0, sizeof( struct sockaddr_storage ) );
        e_res = FAIL;
    }

    unlock_contexts();
    return e_res;
}
#endif


#ifdef HAVE_CMD
static const char *http_cmd_registered_desc  = "List the resources that have been registered with the HTTP module";
static const char *http_cmd_registered_usage = NULL;
static const char *http_cmd_registered_notes = NULL;
static int http_cmd_registered_cb( int argc, char *argv[] )
{
    http_server_resource_node_t *node;
    node = http_server_resource_head;
    cmd_printf( "Resources Registered with HTTP module: \n");
    while ( node != NULL )
    {
        cmd_printf( "  Resource: '%s' - Port: %d\n", node->resource ? node->resource : "null", (int)node->port );
        node = node->next;
    }
    return CMD_OK;
}

char *http_map_state_to_string( http_state_t state )
{
    char *str;
    switch (state)
    {
        case e_http_state_initialized:
            str = "initialized";
            break;
        case e_http_state_header_received:
            str = "header_received";
            break;
        case e_http_state_body_received:
            str = "body_received";
            break;
        case e_http_state_status_code_added:
            str = "status_code_added";
            break;
        case e_http_state_type_added:
            str = "type_added";
            break;
        case e_http_state_header_complete:
            str = "header_complete";
            break;
        case e_http_state_body_complete:
            str = "body_complete";
            break;
        case e_http_state_header_sent:
            str = "header_sent";
            break;
        case e_http_state_body_sent:
            str = "body_sent";
            break;
        case e_http_state_done:
            str = "done";
            break;
        case e_http_state_resetting:
            str = "resetting";
            break;
        default:
            str = "unknown";
            break;
    }
    return str;
}

static const char *http_cmd_contexts_desc  = "List the current connections to the HTTP server";
static const char *http_cmd_contexts_usage = NULL;
static const char *http_cmd_contexts_notes = NULL;
static int http_cmd_contexts_cb( int argc, char *argv[] )
{
    HttpPageContext * context;
    lock_contexts();
    context = activeContexts;
    cmd_printf( "Current Contexts with HTTP: \n");
    while ( context != NULL )
    {
        cmd_printf( "  Handle: %#08X - State: %s - Rsrc: '%s' - %s - devInfo: %d - id: %d\n", 
                    (unsigned int)((uintptr_t)context),
                    http_map_state_to_string( context->state ),
                    context->requested_uri ? context->requested_uri : "null",
                    context->connType ? "usb" : "net",
                    (unsigned int)((intptr_t)context->deviceInfo),
                    (int)context->id
                    );
        context = context->nextContext;
    }
    unlock_contexts();
    return CMD_OK;
}

static const char *http_cmd_defaults_desc  = "Restore to default network settings";
static const char *http_cmd_defaults_usage = NULL;
static const char *http_cmd_defaults_notes = NULL;
static int http_cmd_defaults_cb( int argc, char *argv[] )
{
    cmd_printf( "network interface variables restore defaults: \n");

    #ifdef HAVE_NETWORK
    #ifdef WIRED_NETWORK
    // TODO must account for multiple simultaneous interfaces
    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    net_iface_var_restore_defaults(if_index, NET_IFACE_VAR_ALL);
    #endif
    #endif

    return CMD_OK;
}
#if 0 
static const char *http_cmd_debug_desc  = "Configure the debug print statements for the HTTP module";
static const char *http_cmd_debug_usage = "  debug clear [ sub module(s) ]\n"
                                          "  debug [ err | msg | verbose ] [ sub module(s) ]\n"
                                          "    Sub Modules: http, methods, ssi, oid, usb, net or all\n";
static const char *http_cmd_debug_notes = "Example: http debug clear methods ssi or http verbose all";
static int http_cmd_debug_cb( int argc, char *argv[] )
{
    uint32_t temp = 0;
    uint32_t pos = 0;
    if ( argc < 3 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( strncasecmp( argv[ pos + 1 ], "clear", 5 ) == 0 )
    {
        logger_clear_module( DEBUG_LOGGER_MODULE_HTTP );
        return CMD_OK;
    }
    while( ++pos < argc )
    {
        if ( strncasecmp( argv[ pos ], "err", 3 ) == 0 )
        {
            temp |= LOG_ERR;
        } 
        else if ( strncasecmp( argv[ pos ], "msg", 3 ) == 0 )
        {
            temp |= LOG_NOTICE;
        } 
        else if ( strncasecmp( argv[ pos ], "verbose", 7 ) == 0 )
        {
            temp |= LOG_DEBUG;
        } 
        else if ( strncasecmp( argv[ pos ], "http", 4 ) == 0 )
        {
            temp |= HTTP_SUBMOD_HTTP;
        } 
        else if ( strncasecmp( argv[ pos ], "methods", 7 ) == 0 )
        {
            temp |= HTTP_SUBMOD_METHODS;
        } 
        else if ( strncasecmp( argv[ pos ], "ssi", 3 ) == 0 )
        {
            temp |= HTTP_SUBMOD_SSI;
        } 
        else if ( strncasecmp( argv[ pos ], "oid", 3 ) == 0 )
        {
            temp |= HTTP_SUBMOD_OID;
        } 
        else if ( strncasecmp( argv[ pos ], "usb", 3 ) == 0 )
        {
            temp |= HTTP_SUBMOD_USB;
        } 
        else if ( strncasecmp( argv[ pos ], "net", 3 ) == 0 )
        {
            temp |= HTTP_SUBMOD_NET;
        } 
        else if ( strncasecmp( argv[ pos ], "all", 3 ) == 0 )
        {
            temp |= 0xFFFFFF00;
        } 
        else 
        {
            cmd_printf( "Failed to match: %s\n", argv[ pos ] );
            return CMD_USAGE_ERROR;
        }
    }
    logger_enable_module_mask( DEBUG_LOGGER_MODULE_HTTP, temp );

    return CMD_OK;
}
#endif 

#ifdef DEBUG
static const char *http_cmd_get_pwd_desc  = "Get the current HTTP password";
static const char *http_cmd_get_pwd_usage = NULL;
static const char *http_cmd_get_pwd_notes = NULL;
static int http_cmd_get_pwd_cb( int argc, char *argv[] )
{
    char password[ HTTP_VAR_PASSWORD_STRING_MAX_LEN ];
    if ( http_get_var_password( password, HTTP_VAR_PASSWORD_STRING_MAX_LEN - 1 ) == OK )
    {
        cmd_printf( "  HTTP password: %s\n", password );
    }
    else
    {
        cmd_printf( "  Error getting password\n" );
        return CMD_ERROR;
    }
    return CMD_OK;
}

static const char *http_cmd_set_pwd_desc  = "Set the HTTP password";
static const char *http_cmd_set_pwd_usage = "<password>";
static const char *http_cmd_set_pwd_notes = "This password controls access to some of the HTML and XML pages.\n"
                                            "To clear the password set the password to 'clear'.";
static int http_cmd_set_pwd_cb( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( strncasecmp(argv[1], "clear", 5 ) == 0 )
    {
        http_set_var_password( "", 0 );
        cmd_printf( " Password cleared\n" );
    }
    else
    {
        if ( http_set_var_password( argv[ 1 ], strlen( argv[ 1 ] ) ) == OK )
        {
            cmd_printf( "  Password set\n" );
        }
        else
        {
            cmd_printf( "  Error setting password\n" );
            return CMD_ERROR;
        }
    }
    return CMD_OK;
}
#endif // DEBUG
#endif // HAVE_CMD



error_type_t http_resource_register_event( char *resource, 
                                           int32_t port,
                                           uint32_t flags,
                                           http_resource_event_flag_cb_t event_cb,
                                           void *usr_ctxt )
{
    http_server_resource_node_t *node;
    error_type_t e_res;

    ASSERT( NULL != event_cb );
    lock_contexts();
    e_res = OK;

    DBG_MSG( "Resource register request: %s - port: %d\n", resource ? resource : "null", port );

    if ( ( resource == NULL) && ( port == 0 ) )
    {
        DBG_ERR( "Invalid register request - must specify port and/or resource\n" );
        ASSERT( 0 );
        e_res = FAIL;
    }

    if ( e_res == OK )
    {
        node = http_server_resource_head;
        while ( node != NULL )
        {
            if ( http_is_resource_already_registered( resource, port, node ) == true )
            {
                DBG_MSG( "Resource register request: %s -- ALREADY EXISTS\n", resource ? resource: "null", port );
                e_res = FAIL;
                break;
            }
            node = node->next;
        }
    }

    if ( OK == e_res )
    {
        /* create a new node and register events */
        node = ( http_server_resource_node_t * )MEM_MALLOC_ALIGN( sizeof( http_server_resource_node_t ),
                                                                  cpu_get_dcache_line_size() );
        if( node == NULL )
        {
            /* NO MEMORY! */
            DBG_ERR( "Not enough memory for resource register!\n" );
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        node->event_cb = event_cb;
        node->resource_cb = NULL;
        node->resource = resource;
        node->port     = port;
        node->usr_ctxt = usr_ctxt;

        /* insert at beginning of linked list */
        node->next = http_server_resource_head;
        http_server_resource_head = node;
        DBG_MSG( "Resource register request: %s - port: %d - ADDED\n", resource ? resource : "null", port );

        #ifdef HAVE_NETWORK
        if( port != 0 )
        {
            // Tell network code to open TCP socket and start to listen for new connections
            http_network_add_port( port, HTTP_NET_SOCKET_TCP );
        }
        #endif // HAVE_NETWORK
    }

    unlock_contexts();
    return e_res;
}

static error_type_t read_chunk_header_from_connection( HttpPageContext *context, uint32_t *timeout, uint32_t start_time )
{
    error_type_t e_res;
    uint32_t at_byte;
    int32_t bytes_read;
    char chunk_header[ MAX_CHUNK_SIZE ];
    int32_t timeout_remaining;
    chunk_header_state_t chunk_state;

    e_res = OK;
    chunk_state = HTTP_CHUNK_INIT;
    memset( chunk_header, '\0', MAX_CHUNK_SIZE );
    at_byte = 0;

    // HTTP Chunk will take the form of either:
    //   AB01\r\n       or
    //   \r\nAB01\r\n
    // Read byte by byte and make sure data matches one of these formats
    while ( chunk_state != HTTP_CHUNK_DONE )
    {
        if ( context->partial_chunk_length > at_byte )
        {
            DBG_VERBOSE( "Pulling data from saved partial chunk\n" );
            chunk_header[ at_byte ] = context->partial_chunk[ at_byte ];
            if ( context->partial_chunk_length == at_byte + 1 ) // Add one to compensate for offset versus length comparison
            {
                DBG_VERBOSE( "Entire partial chunk has been copied - delete buffer\n" );
                MEM_FREE_AND_NULL( context->partial_chunk );
                context->partial_chunk_length = 0;
            }
        }
        else
        {
            // Read a byte and see what it is:
            timeout_remaining = *timeout - ( ( posix_gettime_ticks( ) - start_time ) * MS_PER_TICK );
            if ( timeout_remaining <= 0 )
            {
                *timeout = 0;
                e_res = FAIL;
                DBG_ERR( "Timeout while reading chunk header from connection\n" );
                if ( at_byte > 0 )
                {
                    DBG_VERBOSE( "Saving partial chunk header: '%s'\n", chunk_header );
                    context->partial_chunk = MEM_MALLOC( at_byte + LENGTH_OF_NULL );
                    if ( context->partial_chunk != NULL )
                    {
                        memcpy( context->partial_chunk, chunk_header, at_byte );
                        context->partial_chunk_length = at_byte;
                    }
                }
                break;
            }
            DBG_VERBOSE( "Reading next byte with timeout of %d ms\n", timeout_remaining );
            bytes_read = context->blocking_recv_cb( context, chunk_header + at_byte, 1, (uint32_t *)&timeout_remaining );
            if ( bytes_read != 1 )
            {
                DBG_ERR( "Failed to read from blocking_recv_cb - timeout: %d\n", timeout_remaining );
                e_res = FAIL;
                if ( timeout_remaining <= 0 )
                {
                    *timeout = 0;
                }
                break;
            }
        }
        //DBG_ERR( "Chunk_state: %d - '%s'\n", chunk_state, chunk_header );
        switch ( chunk_state )
        {
            case HTTP_CHUNK_INIT:
                if ( chunk_header[ at_byte ] == '\r' )
                {
                    chunk_state = HTTP_CHUNK_FIRST_CR;
                }
                else if ( ( ( chunk_header[ at_byte ] >= '0' ) && ( chunk_header[ at_byte ] <= '9' ) ) ||
                          ( ( chunk_header[ at_byte ] >= 'a' ) && ( chunk_header[ at_byte ] <= 'f' ) ) ||
                          ( ( chunk_header[ at_byte ] >= 'A' ) && ( chunk_header[ at_byte ] <= 'F' ) ) )
                {
                    chunk_state = HTTP_CHUNK_DATA;
                }
                else
                {
                    DBG_ERR( "%s: Got character '%c' == 0x%02X in state: %d\n", __FUNCTION__, chunk_header[ at_byte ], chunk_header[ at_byte ], chunk_state );
                    chunk_state = HTTP_CHUNK_DONE;
                    e_res = FAIL;
                }
                break;
            case HTTP_CHUNK_FIRST_CR:
                if ( chunk_header[ at_byte ] == '\n' )
                {
                    chunk_state = HTTP_CHUNK_FIRST_NL;
                }
                else
                {
                    DBG_ERR( "%s: Got character '%c' == 0x%02X in state: %d\n", __FUNCTION__, chunk_header[ at_byte ], chunk_header[ at_byte ], chunk_state );
                    chunk_state = HTTP_CHUNK_DONE;
                    e_res = FAIL;
                }
                break;
            case HTTP_CHUNK_FIRST_NL:
                if ( ( ( chunk_header[ at_byte ] >= '0' ) && ( chunk_header[ at_byte ] <= '9' ) ) ||
                     ( ( chunk_header[ at_byte ] >= 'a' ) && ( chunk_header[ at_byte ] <= 'f' ) ) ||
                     ( ( chunk_header[ at_byte ] >= 'A' ) && ( chunk_header[ at_byte ] <= 'F' ) ) )
                {
                    chunk_state = HTTP_CHUNK_DATA;
                }
                else
                {
                    DBG_ERR( "%s: Got character '%c' == 0x%02X in state: %d\n", __FUNCTION__, chunk_header[ at_byte ], chunk_header[ at_byte ], chunk_state );
                    chunk_state = HTTP_CHUNK_DONE;
                    e_res = FAIL;
                }
                break;
            case HTTP_CHUNK_DATA:
                if ( chunk_header[ at_byte ] == '\r' )
                {
                    chunk_state = HTTP_CHUNK_END_CR;
                }
                else if ( ( ( chunk_header[ at_byte ] >= '0' ) && ( chunk_header[ at_byte ] <= '9' ) ) ||
                          ( ( chunk_header[ at_byte ] >= 'a' ) && ( chunk_header[ at_byte ] <= 'f' ) ) ||
                          ( ( chunk_header[ at_byte ] >= 'A' ) && ( chunk_header[ at_byte ] <= 'F' ) ) )
                {
                    chunk_state = HTTP_CHUNK_DATA;
                }
                else
                {
                    DBG_ERR( "%s: Got character '%c' == 0x%02X in state: %d\n", __FUNCTION__, chunk_header[ at_byte ], chunk_header[ at_byte ], chunk_state );
                    chunk_state = HTTP_CHUNK_DONE;
                    e_res = FAIL;
                }
                break;
            case HTTP_CHUNK_END_CR:
                chunk_state = HTTP_CHUNK_DONE;
                if ( chunk_header[ at_byte ] != '\n' )
                {
                    DBG_ERR( "%s: Got character '%c' == 0x%02X in state: %d\n", __FUNCTION__, chunk_header[ at_byte ], chunk_header[ at_byte ], chunk_state );
                    e_res = FAIL;
                }
                break;
            default:
                XASSERT( 0, chunk_state );
                break;
        }
        at_byte++;
        if ( at_byte > MAX_CHUNK_SIZE - 1 )
        {
            DBG_ERR( "%s: Got too many bytes looking for chunk '%s' in state: %d\n", __FUNCTION__, chunk_header, chunk_state );
            e_res = FAIL;
            chunk_state = HTTP_CHUNK_DONE;
        }
    }

    if ( e_res == OK )
    {
        uint32_t offset;
        // Determine where chunk length starts
        if ( chunk_header[ 0 ] == '\r' )
        {
            offset = 2;
        }
        else
        {
            offset = 0;
        }
        DBG_VERBOSE( "Chunk header: '%s'\n", chunk_header );
        context->pageSize = strtol( &chunk_header[ offset ], NULL, 16 );
        DBG_VERBOSE( "Chunk size: %d\n", context->pageSize );
    }

    DBG_VERBOSE( "%s exit - e_res = %d - timeout: %d\n", __FUNCTION__, e_res, *timeout );
    return e_res;
}

static uint32_t dechunk_buffered_data( HttpPageContext *context, char *buffer, uint32_t len )
{
    bool have_full_chunk;
    uint32_t bytes_to_read;
    uint32_t bytes;
    char *buffer_start;
    char *tmp_buf;

    tmp_buf = NULL;
    bytes = 0;
    buffer_start = context->request_body;
    while ( bytes < len )
    {
        DBG_VERBOSE( "%s - len: %d, bytes: %d, pageSize: %d, atByte: %d, Context->request_body_length: %d\n", __FUNCTION__, len, bytes, context->pageSize, context->atByte, context->request_body_length );
        if ( ( context->pageSize == 0 ) && ( context->atByte == 0 ) )
        {
            // Verify entire chunk header is present
            // Look for \r\n
            // Look for hex value
            // Look for \r\n
            have_full_chunk = http_have_full_chunk_info( context->request_body,
                                                         context->request_body_length );

            /* here we have a full chunk so we'll parse it
             * and process it.  If it's not a zero-length
             * chunk then we'll pass it on to the callback */
            if ( true == have_full_chunk )
            {
                error_type_t res;
                DBG_VERBOSE( "%s - Have a full chunk header\n", __FUNCTION__ );    
                res = http_chunk_get_length( &context->request_body,
                                             &context->request_body_length,
                                             &context->pageSize );
                ASSERT( OK == res );
                DBG_VERBOSE( "%s - Chunk size = %d\n", __FUNCTION__, context->pageSize );    
                if ( context->pageSize == 0 )
                {
                    context->state = e_http_state_body_received;
                    DBG_MSG( "%s - Terminating chunk received - exit now\n", __FUNCTION__ );
                    break;
                }
            }
            else
            {
                DBG_VERBOSE( "%s - Save partial chunk header ...\n", __FUNCTION__ );
                context->partial_chunk = MEM_MALLOC( context->request_body_length + LENGTH_OF_NULL );
                if ( context->partial_chunk == NULL )
                {
                    DBG_ERR( "Failed to malloc memory for partial chunk header\n" );
                    return FAIL;
                }
                memcpy( context->partial_chunk, context->request_body, context->request_body_length );
                context->partial_chunk[ context->request_body_length ] = '\0';
                context->partial_chunk_length = context->request_body_length;
                context->request_body_length = 0;
                break;
            }
        }
        // Which is smallest (saved bytes, bytes in chunk, or data requested by module)
        bytes_to_read = MIN( context->request_body_length, context->pageSize - context->atByte );
        bytes_to_read = MIN( len - bytes, bytes_to_read );
        memcpy( buffer + bytes, context->request_body, bytes_to_read );
        bytes += bytes_to_read;
        context->atByte += bytes_to_read;
        context->request_body += bytes_to_read;
        context->request_body_length -= bytes_to_read;

        if ( context->atByte == context->pageSize )
        {
            DBG_VERBOSE( "%s - full chunk read, reset chunk info\n", __FUNCTION__ );
            context->atByte = 0;
            context->pageSize = 0;
        }

        if ( context->request_body_length == 0 )
        {
            DBG_MSG( "%s - ran out of buffered data, read from socket\n", __FUNCTION__ );
            break;
        }
    }

    if ( context->request_body_length > 0 )
    {
        DBG_VERBOSE( "%s - Updated Context->request_body_length: %d\n", __FUNCTION__, context->request_body_length );
        tmp_buf = (char*)MEM_MALLOC( context->request_body_length );
        if ( tmp_buf == NULL )
        {
            DBG_ERR( "%s - failed to malloc memory for remaining body data\n", __FUNCTION__ );
            return FAIL;
        }
        memcpy( tmp_buf, context->request_body, context->request_body_length );
    }
    MEM_FREE_AND_NULL( buffer_start );
    context->request_body = tmp_buf;
 
    DBG_VERBOSE( "%s exit - bytes: %d\n", __FUNCTION__, bytes );
    return bytes;
}

int32_t http_resource_read( http_request_t request, char *buffer, uint32_t len, uint32_t *timeout )
{
    int32_t timeout_remaining;
    int32_t start_time;
    int32_t bytes;
    int32_t cur_bytes_read;
    uint32_t bytes_to_read;

    HttpPageContext *context;


    lock_contexts();
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        DBG_ERR( "req %d: %s - invalid request handle\n", request, __FUNCTION__);
        unlock_contexts();
        return -1;
    }

    if ( context->event_cb == NULL )
    {
        DBG_ERR( "req %d: %s requires callback reqistration\n", request, __FUNCTION__ );
        unlock_contexts();
        return -1;
    }
    context->lastAccess = posix_gettime_ticks();
    if ( ( context->request_data_read >= context->content_length ) ||
         ( ( context->state >= e_http_state_body_received ) && ( context->content_length == CHUNKED_ENCODING ) ) )
    {
        DBG_MSG2( "req %d: read EOF (data_read=%d content-length=%d)\n", 
                 request, context->request_data_read, context->content_length);
        DBG_VERBOSE( "req %d: %s  - data_read: %d - content-length: %d - state: %d\n", 
                 request, __FUNCTION__, context->request_data_read, context->content_length, context->state );
        unlock_contexts();
        return 0; // EOF
    }

    // Save off current time at start of function to determine timeout later
    start_time = posix_gettime_ticks();
    bytes = 0;

    // Limit read to only the size of the current request
    if ( context->request_data_read + len > context->content_length )
    {
        len = context->content_length - context->request_data_read;
    }

    // Check to see if any body data has been saved into the context, if so copy to buffer
    if ( context->request_body_length > 0 )
    {
        char *tmp_buf;
        tmp_buf = NULL;

        if ( context->content_length == CHUNKED_ENCODING )
        {
            // Dechunk the buffer
            bytes += dechunk_buffered_data( context, buffer, len );
        }
        else
        {

            bytes_to_read = MIN( len, context->request_body_length );

            memcpy( buffer, context->request_body, bytes_to_read );
            context->request_body_length -= bytes_to_read;
            if ( context->request_body_length > 0 )
            {
                DBG_VERBOSE( "Updated Context->request_body_length: %d\n", context->request_body_length );
                tmp_buf = (char*)MEM_MALLOC( context->request_body_length );
                if ( tmp_buf == NULL )
                {
                    DBG_ERR( "req %d: %s - failed to malloc memory for remaining body data\n", 
                             request, __FUNCTION__ );
                    unlock_contexts();
                    return FAIL;
                }

                memcpy( tmp_buf, context->request_body + bytes_to_read, context->request_body_length );
            }

            MEM_FREE_AND_NULL( context->request_body );
            context->request_body = tmp_buf;
            bytes += bytes_to_read;
        }
    }

    // Do a select to wait for new data on socket 
    DBG_VERBOSE( "%s - bytes( %d) < len (%d )\n", __FUNCTION__, bytes, len );
    timeout_remaining = 100;
    while ( bytes < len )
    {
        if ( timeout_remaining <= 0 )
        {
            *timeout = 0;
            break;
        }

        if ( context->content_length != CHUNKED_ENCODING )
        {
            // context->request_data_read = Total number of bytes of body data read
            // context->content_length    = Total number of bytes of body data
            cur_bytes_read = context->blocking_recv_cb( context, buffer + bytes, len - bytes, (uint32_t *)&timeout_remaining );
            if ( cur_bytes_read >= 0 )
            {   
                bytes += cur_bytes_read;
            }
            else
            {
                DBG_VERBOSE( "req %d: Error in non-chunked blocking_recv_cb \n", request );
                if ( timeout_remaining == 0 )
                {
                    DBG_VERBOSE( "req %d: read timeout in blocking_recv_cb (bytes=%d)\n", request, bytes );
                    *timeout = 0;
                }

                if ( bytes == 0 )
                {
                    // Nothing was read so send back error condition
                    DBG_VERBOSE( "req %d: read ERROR (timeout=%d; non-chunked recv)\n", request, *timeout);
                    bytes = CONIO_TIMEOUT;
                }
                break;
            }

            DBG_VERBOSE( "%s - bytes: %d\n", __FUNCTION__, bytes );

            if ( context->request_data_read + bytes >= context->content_length )
            {
                DBG_ERR( "%s - entire request has been received\n", __FUNCTION__ );
                break;
            }
        }
        else
        {
            // context->pageSize = size of http chunk
            // context->atByte   = Number of bytes of chunk that have been read
            
            // Do we know the size of the next chunk?
            if ( context->pageSize == 0 )
            {
                // This call will read the chunk header from the socket
                // and populate the context->pageSize field
		
                if ( read_chunk_header_from_connection( context, timeout, start_time ) == OK )
                {
                    if ( context->pageSize == 0 )
                    {
                        DBG_MSG2( "req %d: e_http_state_body_received (got chunk terminator)\n", request );
                        context->state = e_http_state_body_received;
                        break;
                    }
                }
                else
                {
                    DBG_VERBOSE( "req %d: Failed to read chunk header from connection - timeout: %d\n",
                                 request, *timeout );
                    if ( ( *timeout != 0 ) && ( bytes == 0  ) )
                    {
                        // Not a timeout and no data has been read so reset bytes to error condition
                        DBG_ERR( "req %d: read ERROR - expected chunk header\n", request );
                        bytes = CONIO_TIMEOUT;
                    }
                    break;
                }
            }
            // read rest of old chunk (up to len bytes)
            bytes_to_read = MIN( context->pageSize - context->atByte, len - bytes );
            cur_bytes_read = context->blocking_recv_cb( context, buffer + bytes, bytes_to_read, (uint32_t *)&timeout_remaining );
            if ( cur_bytes_read >= 0 )
            {
                bytes += cur_bytes_read;
                context->atByte += cur_bytes_read;
                if ( context->atByte == context->pageSize )
                {
                    DBG_VERBOSE( "Full chunk read (%d bytes) - resetting chunk info\n", context->pageSize );
                    context->atByte = 0;
                    context->pageSize = 0;
                }
                DBG_VERBOSE( "%s - bytes: %d\n", __FUNCTION__, bytes );
            }
            else 
            {
                DBG_ERR( "req %d: Error in chunked blocking_recv_cb\n", request );
                if ( timeout_remaining == 0 )
                {
                    DBG_VERBOSE( "req %d: Timed out in blocking_recv_cb\n", request );
                    *timeout = 0;
                }

                if ( bytes == 0 )
                {
                    // Nothing was read so send back error condition
                    DBG_ERR( "req %d: read ERROR (timeout=%d; chunked recv)\n", request, *timeout);
                    bytes = CONIO_TIMEOUT;
                }
                break;
            }
        }

        timeout_remaining = *timeout - (((uint32_t)posix_gettime_ticks() - start_time )
                                         * MS_PER_TICK );
        DBG_VERBOSE( "req %d: %s - timeout_remaining=%d\n", request, __FUNCTION__, timeout_remaining );
    }

    if ( bytes > 0 )
    {
        context->request_data_read += bytes;
    }
    if ( context->request_data_read >= context->content_length )
    {
        DBG_MSG2( "req %d: e_http_state_body_received (got content_length bytes)\n", request );
        context->state = e_http_state_body_received;
    }

    // return error on timeout
    if( (*timeout == 0) && (bytes == 0) )
    {
        DBG_ERR( "req %d: read ERROR - timeout\n", request );
        bytes = CONIO_TIMEOUT;
    }

    context->lastAccess = posix_gettime_ticks();
    DBG_VERBOSE( "req %d: %s - exit after reading: %d/%d bytes in %d ticks\n", 
                 request, __FUNCTION__, bytes, len, posix_gettime_ticks() - start_time  );
    unlock_contexts();
    return bytes;
}

error_type_t http_add_data_to_chain( HttpPageContext *context, char *buffer, uint32_t length )
{
    PageDataLink *data_link;

    int px_res;
    context->lastAccess = posix_gettime_ticks();
    data_link = (PageDataLink *)MEM_MALLOC_ALIGN( sizeof( PageDataLink ),
                                                   cpu_get_dcache_line_size() );
    if ( data_link == NULL )
    {
        DBG_ERR( "%s - Failed to malloc memory for data_link\n", __FUNCTION__ );
        return FAIL;
    }
    data_link->atByte     = 0;
    data_link->data       = buffer;
    data_link->buff_size  = length;
    data_link->size       = length;
    data_link->next       = NULL;

    px_res = sem_wait(&context->data_chain_semaphore);
    ASSERT(px_res == 0);
    if ( context->dataChain == NULL )
    {
        context->dataChain = data_link;
    }
    else
    {
        PageDataLink *link;
        link = context->dataChain;
        while ( link->next != NULL )
        {
            link = link->next;
        }
        link->next = data_link;
    }
    sem_post(&context->data_chain_semaphore);
    return OK;
}

int32_t http_resource_write( http_request_t request, char *buffer, uint32_t len )
{
    HttpPageContext *context;

    lock_contexts();

    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        DBG_ERR( "%s - failed to get context from request\n", __FUNCTION__ );
        unlock_contexts();
        return FAIL;
    }

    if ( context->event_cb == NULL )
    {
        DBG_ERR( "%s should not be called except on HTTP requests that were registered with a call to http_resource_event_register\n", __FUNCTION__ );
        unlock_contexts();
        return FAIL;
    }

    if ( context->state != e_http_state_header_complete ) 
    {
        DBG_ERR( "%s - context in wrong state for send\n", __FUNCTION__ );
        unlock_contexts();
        return FAIL;
    }

    if ( (context->response_content_length != CHUNKED_ENCODING ) && 
         ( context->response_length + len > context->response_content_length ) )
    {
        DBG_ERR( "%s - Content length of %d bytes was specified but trying to send %d bytes\n", __FUNCTION__, context->response_content_length, context->response_length + len );
        unlock_contexts();
        return FAIL;
    }

    if ( context->response_content_length == CHUNKED_ENCODING )
    {
        char *chunk_buf;
        chunk_buf = http_create_chunk_header( len );

        if ( ( chunk_buf == NULL ) ||
             http_add_data_to_chain( context, chunk_buf, CHUNK_HEADER_SIZE ) != OK )
        {
            DBG_ERR( "%s - Failed in add chunk header to chain (len: %d, buf: %x)\n", __FUNCTION__, len, chunk_buf );
            unlock_contexts();
            return FAIL;
        }
    }

    if ( http_add_data_to_chain( context, buffer, len ) != OK )
    {
        DBG_ERR( "%s - Failed in add data to chain (len: %d, buffer: %x)\n", __FUNCTION__, len, buffer );
        unlock_contexts();
        return FAIL;
    }

    if (context->response_content_length != CHUNKED_ENCODING )
    {
        context->response_length += len;
    }

    unlock_contexts();
    return len;
}

error_type_t http_resource_get_header_length( http_request_t request, uint32_t *len )
{
    error_type_t e_res;
    HttpPageContext *context;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( ( context == NULL ) || 
         ( len == NULL ) )
    {
        e_res = FAIL;
    }
    else
    {
        *len = context->request_header_length;
    }
    unlock_contexts();
    return e_res;
}

error_type_t http_resource_get_header( http_request_t request, char *buffer, uint32_t buffer_len )
{
    error_type_t e_res;
    HttpPageContext *context;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( ( context == NULL ) || 
         ( buffer == NULL ) || 
         ( context->request_header_length > buffer_len ) )
    {
        e_res = FAIL;
    }
    else
    {
        memcpy( buffer, context->request_header, context->request_header_length );
    }
    unlock_contexts();
    return e_res;
}

error_type_t http_resource_get_method( http_request_t request, http_request_method_t *method )
{
    error_type_t e_res;
    HttpPageContext *context;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( ( context == NULL ) || 
         ( method == NULL ) )
    {
        e_res = FAIL;
    }
    if ( e_res == OK )
    {
        if ( strncmp( GET_REQUEST, context->request_header, strlen( GET_REQUEST ) ) == 0 )
        {
            *method = HTTP_METHOD_GET;
        }
        else if ( strncmp( POST_REQUEST, context->request_header, strlen( POST_REQUEST ) ) == 0 )
        {
            *method = HTTP_METHOD_POST;
        }
        else if ( strncmp( PUT_REQUEST, context->request_header, strlen( PUT_REQUEST ) ) == 0 )
        {
            *method = HTTP_METHOD_PUT;
        }
        else if ( strncmp( DELETE_REQUEST, context->request_header, strlen( DELETE_REQUEST ) ) == 0 )
        {
            *method = HTTP_METHOD_DELETE;
        }
        else if ( strncmp( HEAD_REQUEST, context->request_header, strlen( HEAD_REQUEST ) ) == 0 )
        {
            *method = HTTP_METHOD_HEAD;
        }
        else
        {
            e_res = FAIL;
        }
    }
    unlock_contexts();
    return e_res;
}

error_type_t http_resource_get_content_length( http_request_t request, uint32_t *len )
{
    error_type_t e_res;
    HttpPageContext *context;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( ( context == NULL ) || 
         ( len == NULL ) )
    {
        e_res = FAIL;
    }
    else
    {
        *len = context->content_length;
    }
    unlock_contexts();
    return e_res;
}

error_type_t http_resource_get_content_type( http_request_t request, http_response_type_t *type )
{
    char *value;
    uint32_t i;
    value = (char *)MEM_MALLOC( 256 );
    if ( value == NULL )
    {
        return FAIL;
    }
    *type = HTTP_TYPE_UNKNOWN;
    if ( http_resource_get_header_value(request,"Content-Type", value, 255 ) == OK )
    {
        for ( i = 0; i < NUM_HTTP_CONTENT_TYPES; i++ )
        {
            if ( strcmp( value, HttpdContentTypes[ i ] ) == 0 )
            {
                *type = ( http_response_type_t ) i;
                break;
            }
        }
    }

    MEM_FREE_AND_NULL( value );
    return OK;
}

error_type_t http_resource_get_header_value( http_request_t request, char *header_name, char *header_value, uint32_t header_value_len )
{
    char *str_value_start;
    char *str_value;
    char tmp;
    HttpPageContext *context;

    DBG_VERBOSE( "%s - enter\n", __FUNCTION__ );

    lock_contexts();
    context = getHttpContextFromId( request );
    if ( ( NULL != context ) &&
         ( NULL != header_value ) )
    {
        // Make sure request includes the header field
        str_value = strstri( context->request_header, header_name );
        if ( str_value != NULL )
        {
            str_value += strlen( header_name );

            // Skip passed colon separating name from value
            str_value++;

            // Move past any spaces separating the header from the actual value
            while ( ( str_value < context->request_header + context->request_header_length ) && 
                    ( *str_value == ' ' ) )
            {
                str_value++;
            }
            str_value_start = str_value;
            while ( *str_value != '\r' )
            {
                str_value++;
            }
            tmp = *str_value;
            *str_value = '\0';
            DBG_VERBOSE( "%s - header value: %s\n", __FUNCTION__, str_value_start );
            if ( strlen( str_value_start ) > header_value_len )
            {
                *header_value = '\0';
                *str_value = tmp;
                DBG_VERBOSE( "%s - Buffer too small\n", __FUNCTION__ );
                unlock_contexts();
                return HTTP_ERROR_BUFFER_TOO_SMALL;
            }
            else
            {
                strcpy( header_value, str_value_start );
                *str_value = tmp;
                DBG_VERBOSE( "%s - exit\n", __FUNCTION__ );
                unlock_contexts();
                return OK;
            }
        }
        else
        {
            DBG_VERBOSE( "%s - Header not found\n", __FUNCTION__ );
            unlock_contexts();
            return HTTP_ERROR_NO_RESULTS;
        }
    }

    DBG_ERR( "%s - No memory provided(0x%08X) or invalid handle(%d)\n", __FUNCTION__, header_value, request );
    unlock_contexts();
    return FAIL;
}


error_type_t http_resource_reset_connection( http_request_t request )
{
    error_type_t e_res;
    HttpPageContext *context;
    UINT px_res;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
    }
    else
    {
        DBG_MSG( "Marking context to be reset: 0x%08X - id: %d\n", context, request );
        context->state = e_http_state_resetting;

        /* unblock transport layers waiting for resource complete signal
         * this is also done in the send_complete() call but that must 
         * be called in this case */
        px_res = sem_post( &( context->resource_done_semaphore ) );
        XASSERT( px_res == 0, px_res );
    }
    unlock_contexts();
    return e_res;
}

void http_reset_connections( http_ioconn_t connection_type )
{
    HttpPageContext *contexts;
    HttpPageContext *cur_context;

    lock_contexts();
    contexts = activeContexts;

    while ( contexts != NULL )
    {
        cur_context = contexts;
        contexts = contexts->nextContext;

        /* only reset connections of the requested connection type */
        if ( ( cur_context->connType == connection_type ) && 
             ( cur_context->state == e_http_state_resetting ) )
        {
            DBG_MSG( "Forcing closed context: 0x%08X - id: %d\n", cur_context, cur_context->id );
            closeConnection( cur_context, true );
        }
    }
    unlock_contexts();
}

#ifdef HAVE_NETWORK
error_type_t http_get_tcp_recv_buf_size(http_request_t request, uint32_t *size)
{
    error_type_t e_res = OK;
    HttpPageContext *context;
    int sock_rcode;

    *size = 0;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
        goto done;
    }
    if(context->connType != e_http_connection_network)
    {
        e_res = FAIL;
        goto done;
    }

    // adjust recv buff size to account for bulk flow in the rx direction
    int listen_sock = (int)context->deviceInfo;
    unsigned long sock_option;
    socklen_t option_len = sizeof(sock_option);
    sock_rcode = getsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (char *)&sock_option, &option_len);
    int err;
    err = errno;
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_RCVBUF failed! (SocketError=%d)\n", err);
        e_res = FAIL;
        goto done;
    }
    XASSERT(option_len == sizeof(sock_option), option_len);

    *size = sock_option;

done:
    unlock_contexts();
    return e_res;
}

error_type_t http_set_tcp_recv_buf_size(http_request_t request, uint32_t size)
{
    error_type_t e_res = OK;
    HttpPageContext *context;
    int sock_rcode;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
        goto done;
    }
    if(context->connType != e_http_connection_network)
    {
        e_res = FAIL;
        goto done;
    }

    // adjust recv buff size to account for bulk flow in the rx direction
    int listen_sock = (int)context->deviceInfo;
    unsigned long sock_option = size;
    socklen_t option_len = sizeof(sock_option);
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, (char *)&sock_option, option_len);
    int err;
    err = errno;
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_RCVBUF failed! (SocketError=%d)\n", err);
        e_res = FAIL;
        goto done;
    }

done:
    unlock_contexts();
    return e_res;
}

error_type_t http_get_tcp_send_buf_size(http_request_t request, uint32_t *size)
{
    error_type_t e_res = OK;
    HttpPageContext *context;
    int sock_rcode;

    *size = 0;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
        goto done;
    }
    if(context->connType != e_http_connection_network)
    {
        e_res = FAIL;
        goto done;
    }

    // adjust send buff size to account for bulk flow in the tx direction
    int listen_sock = (int)context->deviceInfo;
    unsigned long sock_option;
    socklen_t option_len = sizeof(sock_option);
    sock_rcode = getsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&sock_option, &option_len);
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_SNDBUF failed! (SocketError=%d)\n", errno);
        e_res = FAIL;
        goto done;
    }
    XASSERT(option_len == sizeof(sock_option), option_len);

    *size = sock_option;

done:
    unlock_contexts();
    return e_res;
}

error_type_t http_set_tcp_send_buf_size(http_request_t request, uint32_t size)
{
    error_type_t e_res = OK;
    HttpPageContext *context;
    int sock_rcode;

    lock_contexts();
    e_res = OK;
    context = getHttpContextFromId( request );
    if ( context == NULL )
    {
        e_res = FAIL;
        goto done;
    }
    if(context->connType != e_http_connection_network)
    {
        e_res = FAIL;
        goto done;
    }

    // adjust send buff size to account for bulk flow in the tx direction
    int listen_sock = (int)context->deviceInfo;
    unsigned long sock_option = size;
    socklen_t option_len = sizeof(sock_option);
    sock_rcode = setsockopt(listen_sock, SOL_SOCKET, SO_SNDBUF, (char *)&sock_option, option_len);   
    int err;
    err = errno;
    if(sock_rcode == -1)
    {
        DBG_ERR("setsockopt SO_SNDBUF failed! (SocketError=%d)\n", err);
        e_res = FAIL;
        goto done;
    }

done:
    unlock_contexts();
    return e_res;
}
#endif // HAVE_NETWORK


