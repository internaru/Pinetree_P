/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/**
 *
 * \file http_network_parser.c
 *
 * \brief HTTP Network Parser
 *
 */


#define USE_FW_KEY_CERT_H 1

//--------------------------------------
// Included files
//--------------------------------------
#include <string.h>
#include <stdbool.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "lassert.h"
#include "logger.h"
#include "http_api.h"
#include "http_network_parser.h"
#include "http_parse.h"
#include "dprintf.h"
#include "error_types.h"
#ifdef HAVE_ETHERNET
#include "net_iface_vars.h"
#endif
#ifdef HAVE_HTTPS
#include "openssl/ssl.h"
#include "openssl/err.h"
#ifdef USE_FW_KEY_CERT_H
#include "fw_key.h"
#include "fw_cert.h"
#else
#include "rommgrapi.h"
#include "rom_id_mapping.h"
#endif
#endif

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_NET
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE 
#define DBG_PRFX    "HTTP NET: "

#define THREAD_STACK_SIZE POSIX_MIN_STACK_SIZE 

typedef struct http_sockets_s
{
    uint32_t                    port_number;
    http_net_socket_protocol_t  socket_type;
    int                         socket_descriptor;
    struct sockaddr_storage     socket_address;
    struct http_sockets_s       *next;
} http_sockets_t;


//--------------------------------------
// Global Definitions
//--------------------------------------

static pthread_t            http_network_thread_id;
static ALIGN8 uint8_t       http_network_stack[ THREAD_STACK_SIZE ];
static http_sockets_t       *sockets = NULL;

#ifdef HAVE_HTTPS
static SSL_CTX* ctx;
static SSL** ssl;
static bool ssl_init_failed = false;
#endif


// --------------------------------------
// Local Function Declarations
//--------------------------------------
void* http_network_parser( void* input );
static int startServerSocket( struct sockaddr_storage *servaddr, http_net_socket_protocol_t conn_type, uint32_t port );
static error_type_t contextAccept( int server_fd, uint32_t port_number );
static error_type_t contextWrite( HttpPageContext *context );
static error_type_t contextRead( HttpPageContext *context );
static void closeLRUConnection( void );

static int buildSelectLists( fd_set *read_fds,
                             fd_set *write_fds,
                             fd_set *exception_fds,
                             int *highestFD,
                             http_sockets_t *sockets );
#ifdef HAVE_HTTPS
static int init_ssl_connection( SSL *ssl );
#endif // HAVE_HTTPS

#ifdef HAVE_CMD
static const char *http_cmd_sockets_desc;
static const char *http_cmd_sockets_usage;
static const char *http_cmd_sockets_notes;
static int http_cmd_sockets_cb( int argc, char *argv[] );
#endif  // HAVE_CMD

//--------------------------------------
// Local Function Definitions
//--------------------------------------

int get_server_handle(http_net_socket_protocol_t type, uint32_t port)
{
    http_sockets_t *tmp_sockets =sockets;
    while( tmp_sockets != NULL)
    {
        if (( tmp_sockets->port_number == port) && ( tmp_sockets->socket_type == type))
        {
           return tmp_sockets->socket_descriptor;
        }
        tmp_sockets = tmp_sockets->next;
    }
    return -1;
    
}

/** 
 * \brief start up http network parser
 * 
 * Start the http network parser thread
 * 
 **/
void http_network_parser_init( void )
{
    posix_create_thread(&http_network_thread_id, http_network_parser, 0, "http_net_parser", http_network_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    #ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register_subcmd( "http",
                                 "sockets",
                                 http_cmd_sockets_desc,
                                 http_cmd_sockets_usage,
                                 http_cmd_sockets_notes,
                                 http_cmd_sockets_cb );
    XASSERT( CMD_OK == i_res, i_res );
    #endif // HAVE_CMD

}


/** 
 * \brief create select lists for web server port 
 * 
 * \param[out] read_fds - file set for read sockets
 * 
[out] * \param[out] write_fds - file set for write sockets
 * 
 * \param[out] exception_fds - file set for exception sockets
 * 
 * \param[out] highestFD - highest numerical socket 
 *  
 * \param[in] sockets Handle to linked list of listening sockets
 * 
 * Function recieves a HttpPageContext structure and removes and
 * cleans up memory if needed of the structure's dataChain
 * 
 **/
static int buildSelectLists( fd_set *read_fds,
                             fd_set *write_fds,
                             fd_set *exception_fds,
                             int *highestFD,
                             http_sockets_t *socks )
{
    error_type_t e_res;
    HttpPageContext * curList = NULL;
    HttpPageContext * next_context = NULL;
    int32_t activeCount = 0;

    /* TODO need to remove any references to using the context pointer
     * outside of an HTTP function call (i.e. curList->state ). */

    memset( read_fds, 0, sizeof(fd_set) );
    memset( write_fds, 0, sizeof(fd_set) );
    memset( exception_fds,0, sizeof(fd_set) );

    lock_contexts();

    *highestFD = 0;
    e_res = http_context_get_first( e_http_connection_network, &next_context );

    while( OK == e_res )
    {
        curList = next_context;

        /* NOTE: could add "if request_cb != NULL" here or something similar
         * if we can't accept the performance hit from a function call
         * and semaphore get on each context iteration */

        /* check if this context was handled by a registered resource
         * and if it needs to be freed */
        e_res = http_context_check_for_done_state( curList );
        if ( OK == e_res )
        {
            DBG_MSG( "finished %#x for current write - reset for next read\n", curList->deviceInfo );
            http_context_destroy( curList, false );
        }

        activeCount++;
        DBG_VERBOSE( "Active Socket: %#x\n", curList->deviceInfo );
        if ( ( ( e_http_state_initialized == curList->state ) ||
               ( e_http_state_header_received == curList->state ) )
             &&
             ( curList->event_cb == NULL ) )
        {
            // More information to still read
            FD_SET( ( intptr_t )curList->deviceInfo, read_fds );
            DBG_VERBOSE( "Add to read\n" );
        }
        else if ( ( curList->request_cb == NULL ) &&
                  ( curList->event_cb == NULL ) &&
                ( ( ( curList->state >= e_http_state_body_received ) &&
                    ( curList->state < e_http_state_body_complete ) ) ||
                  ( curList->dataChain != NULL ) ) )
        {
            // Have recieved all data - now send a response
            DBG_VERBOSE( "Add to write\n" );
            FD_SET( ( intptr_t )curList->deviceInfo, write_fds );
        }
        else if ( ( curList->event_cb != NULL ) &&
                  ( ( ( curList->state >= e_http_state_body_received ) &&
                      ( curList->state <= e_http_state_body_complete ) ) &&
                    ( curList->dataChain != NULL ) ) )
        {
            // Have recieved all data - now send a response
            DBG_VERBOSE( "Add to write\n" );
            FD_SET( ( intptr_t )curList->deviceInfo, write_fds );
        }

        if ( ( intptr_t )curList->deviceInfo > *highestFD )
        {
            *highestFD = ( intptr_t )curList->deviceInfo;
        }

        e_res = http_context_get_next( curList, e_http_connection_network, &next_context );
    }

    unlock_contexts();

    while( socks != NULL )
    {
        FD_SET( socks->socket_descriptor, read_fds );
        if ( socks->socket_descriptor > *highestFD )
        {
            *highestFD = socks->socket_descriptor;
        }
        socks = socks->next;
    }

    DBG_VERBOSE( "Active Count: %d\n", activeCount );

    return activeCount;
}


/** 
 * \brief Main control loop for network connections
 * 
 * \param input - unused
 * 
 * Function starts the sockets required for the web server, then
 * builds select lists and services requests on the sockets
 * 
 **/
void* http_network_parser( void* input )
{
    error_type_t e_res;
    int32_t result;
    int highestFD;
    fd_set write_fds;
    fd_set read_fds;
    fd_set exception_fds;
    HttpPageContext * context = NULL;
    HttpPageContext * next_context = NULL;
    int32_t activeCount;
    struct timeval select_wait;
    //uint32_t i;
    bool restart_loop;
    http_config_t http_config;
#ifdef HAVE_HTTPS
#ifndef USE_FW_KEY_CERT_H
    int8_t *fw_cert;
    int8_t *fw_key;
    rommgr_id_mapping_t cert_id = ROMMGR_SSL_CERT;
    rommgr_id_mapping_t key_id = ROMMGR_SSL_KEY;
    uint32_t fw_cert_size;
    uint32_t fw_key_size;
#endif
    int err;
    const unsigned char *fw_cert_pointer;
    const unsigned char *fw_key_pointer;
    X509*    x509_fw_cert;
    EVP_PKEY* evp_fw_key;
    const SSL_METHOD *meth;
#endif

    /* Wait for all levels to start */
    SysWaitForInit( );

    e_res = http_config_get( &http_config );
    if ( OK != e_res ) 
    {
        ASSERT( 0 );
    }

#ifdef HAVE_HTTPS
    ssl = MEM_CALLOC( http_config.max_active_http_connections * sizeof( SSL* ) , 1);
    if ( ssl == NULL )
    {
        DBG_ERR( "SSL: Error, could not get memory for active connections.\n" );
        ssl_init_failed = true;
        ASSERT(0);
    }

    // Get a pointer to the location of the certificate stored in NVM
#ifndef USE_FW_KEY_CERT_H
    fw_cert = RomMgrReadData( IM_DATA_SECTOR , (uint32_t)cert_id, &fw_cert_size );
#endif
    if( ( fw_cert == NULL ) || ( fw_cert_size == 0 ) )
    {
        DBG_ERR( "SSL: Error, could not read fw_cert.der.\n" );
        ssl_init_failed = true;
        ASSERT(0);
    }
    if( ssl_init_failed == false )
    {
        fw_cert_pointer = (const unsigned char *)fw_cert;
        // Convert DER format certificate in NVM to X509 in RAM
        x509_fw_cert = d2i_X509( NULL, &fw_cert_pointer, (long)fw_cert_size );
        ASSERT( NULL != x509_fw_cert );
#ifndef USE_FW_KEY_CERT_H
        MEM_FREE_AND_NULL( fw_cert );
#endif

        // Get a pointer to the location of the private key stored in NVM
#ifndef USE_FW_KEY_CERT_H
        fw_key = RomMgrReadData( IM_DATA_SECTOR , (uint32_t)key_id, &fw_key_size );
#endif
        if( ( fw_key == NULL ) || ( fw_key_size == 0 ) )
        {
            DBG_ERR( "SSL: Error, could not read fw_key.der.\n" );
            ssl_init_failed = true;
            ASSERT(0);
        }
        if( ssl_init_failed == false )
        {
            fw_key_pointer = (const unsigned char *)fw_key;
            // Convert DER format private key in NVM to EVP_PKEY in RAM
            evp_fw_key = d2i_AutoPrivateKey( NULL, &fw_key_pointer, (long)fw_key_size );
            
#ifndef USE_FW_KEY_CERT_H
            MEM_FREE_AND_NULL( fw_key );
#endif

            if( NULL == evp_fw_key )
            {
                DBG_ERR( "SSL: Error, fw_key.der corrupt.\n" );
                ssl_init_failed = true;
                ASSERT(0);
            }
            else
            {

                SSL_load_error_strings();
                OpenSSL_add_ssl_algorithms();
                meth = SSLv23_server_method();
                ctx = SSL_CTX_new( meth );
                ASSERT( NULL != ctx );

                // Read in the certificate
                err = SSL_CTX_use_certificate( ctx, x509_fw_cert );
                ASSERT( err == 1 );

                // Read in the private key
                err = SSL_CTX_use_PrivateKey( ctx, evp_fw_key );
                ASSERT( err == 1 );

                // Check private key for a match to the certificate public key
                err = SSL_CTX_check_private_key( ctx );
                ASSERT( err == 1 );
            }
        }
    }
#endif // HAVE_HTTPS

    /* start listening sockets */
    /*
    for ( i = 0; i < http_config.socket_table_size; i++ )
    {
        http_network_add_port( http_config.socket_table[ i ].port_number, http_config.socket_table[ i ].socket_type );
    }
    */

    /* Initialize wait time for select call - wait 1 second */
    memset( &select_wait, 0x00, sizeof( struct timeval ) );
    select_wait.tv_sec = 1;


    /* accept and process new tcp connection */
    while( true )
    {
        http_sockets_t *cur_sock;
        http_custom_web_server_processing( );

        http_reset_connections( e_http_connection_network );

        cur_sock = sockets;
        restart_loop = false;

        while ( cur_sock != NULL )
        {
            if ( cur_sock->socket_descriptor < 0 )
            {
                // -1 indicates port is currently in use and we should retry
                if (cur_sock->socket_descriptor == -1)
                {
                    
                #ifdef HAVE_EVENT_LOGGER
                event_logger_set_log_entry( EVENTLOG_LAN_SOCKET_ERROR ); 
                #endif
                    DBG_ERR( "TCP socket on port %d failed, try again...\n", cur_sock->port_number );
                    
                sleep(1);   // sleep for 1 seconds then try again
                cur_sock->socket_descriptor = startServerSocket( &cur_sock->socket_address, 
                                                                 cur_sock->socket_type, 
                                                                 cur_sock->port_number );
                                                                     
                restart_loop = true;
                continue;
            }
                // failed for some other reason
                else  
                {
                    break;
                }
            }
            
            cur_sock = cur_sock->next;
        }
        
        if ( restart_loop == true )
        {
            continue;
        }

        /* Initialize wait time for select call - wait 1 second */
        memset( &select_wait, 0x00, sizeof( struct timeval ) );
        select_wait.tv_sec = 1;

        FD_ZERO(&read_fds); 
        FD_ZERO(&write_fds); 
        activeCount = buildSelectLists( &read_fds, &write_fds, &exception_fds, &highestFD, sockets );
        result = select( highestFD + 1, &read_fds, &write_fds, &exception_fds, &select_wait );
        DBG_VERBOSE( "select result: %d\n", result );

        if ( result == 0 )
        {
            DBG_VERBOSE("Nothing after select wait - restarting loop\n" );
            continue;
        }
        else if( result < 0 )
        {
            #ifdef HAVE_EVENT_LOGGER
            event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
            #endif
            result = errno;
            DBG_ERR( "Select Error: ErrNum: %d\n", 
                     result );
            continue;
        }

        cur_sock = sockets;
        while ( cur_sock != NULL )
        {
            if (cur_sock->socket_descriptor < 0)
            {
                DBG_MSG( "Skipping TCP socket on port %d - failed to start!\n", cur_sock->port_number );
            }
            else
            {
            if ( ( cur_sock->socket_type == HTTP_NET_SOCKET_TCP ) && 
                 ( FD_ISSET( cur_sock->socket_descriptor, &read_fds ) ) )
            {

                if( activeCount >= http_config.max_active_http_connections )
                {
                    DBG_MSG( "activeCount: %d >= MAX: %d\n", activeCount, http_config.max_active_http_connections );
                    closeLRUConnection( );
                }

                /* Accept new Client */
                e_res = contextAccept( cur_sock->socket_descriptor, cur_sock->port_number );
                if( ( OK != e_res ) && ( HTTP_PARSING_INCOMPLETE != e_res ) && ( HTTP_DATA_TO_WRITE != e_res ) && (HTTP_PARSING_JUNK != e_res ))
                {
                    #ifdef HAVE_EVENT_LOGGER
                    event_logger_set_log_entry( EVENTLOG_LAN_SOCKET_ERROR ); 
                    #endif
                    DBG_ERR( "Failed to accept new Socket: server_fd: %d\n", cur_sock->socket_descriptor );
                    /* clear the fd so we can restart the connection */
#ifdef __linux__                    
                        close( cur_sock->socket_descriptor );
#else                    
                    tfClose( cur_sock->socket_descriptor );
#endif                    
                    cur_sock->socket_descriptor = -1;
                }
            }
            else if ( ( cur_sock->socket_type == HTTP_NET_SOCKET_UDP) &&
                    ( FD_ISSET( cur_sock->socket_descriptor, &read_fds )) )
            {
                struct sockaddr_storage destAddr;
                char *udp_result = MEM_MALLOC( http_config.udp_read_buffer_size );
                if ( udp_result == NULL )
                {
                    DBG_ERR( "Failed to allocate buffer for UDP buffer - skipping to next socket\n" );
                    cur_sock = cur_sock->next;
                    continue;
                }
                
#ifdef __linux__                    
                socklen_t sockAddrSize = sizeof ( struct sockaddr_storage );
#else                    
                int sockAddrSize = sizeof ( struct sockaddr_storage );
#endif
                int32_t errorCode;

                memset( &destAddr, 0x00, sockAddrSize );

                // Look for Response
                errorCode = recvfrom( cur_sock->socket_descriptor, 
                                      udp_result, 
                                      http_config.udp_read_buffer_size, 
                                      0, 
                                          ( struct sockaddr * )&destAddr, 
                                      &sockAddrSize );
                if ( errorCode < 0 )
                {
                    #ifdef HAVE_EVENT_LOGGER
                    event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
                    #endif
                    DBG_ERR( "Error %d receiving udp packet: %s\n", 
                                 errno );
                }
                else
                {
                    udp_result[ errorCode ] = 0;
                    DBG_MSG( "Got udp response: '%s'\n", udp_result );
                    process_udp_response( udp_result, &destAddr );
                }
                MEM_FREE_AND_NULL(udp_result);
            }
            }

            cur_sock = cur_sock->next;

        }

        e_res = http_context_get_first( e_http_connection_network, &next_context );

        // Send all our available data for contexts that can send
        while ( OK == e_res )
        {
            context = next_context;

            XASSERT( context->connType == e_http_connection_network, context->connType );

            // Did this one say it can write?
            if ( FD_ISSET( ( intptr_t )context->deviceInfo, &write_fds ))
            {
                e_res = contextWrite( context );
                if ( ( HTTP_PARSING_JUNK == e_res ) ||
                     ( FAIL == e_res ) ||
                     ( HTTP_MEMORY_OUT == e_res) ||
                     ( OK == e_res ) ) /* OK means this context has been
                                          freed, so we can't use the
                                          pointer anymore, set this flag
                                          so we don't do that */
                {
                    /* activeContext list potentially changed 
                     * restart for loop */
                    e_res = FAIL;
                }
            }
            else if ( FD_ISSET( ( intptr_t )context->deviceInfo, &read_fds ))
            {
                e_res = contextRead( context );
                if ( ( HTTP_PARSING_JUNK == e_res ) ||
                     ( FAIL == e_res ) ||
                     ( HTTP_MEMORY_OUT == e_res ) )
                {
                    /* activeContext list potentially changed
                     * restart for loop */
                    e_res = FAIL;
                }
            }

            if ( FAIL != e_res )
            {
                e_res = http_context_get_next( context, e_http_connection_network, &next_context );
            }
        }
    }
    return 0;
}


/** 
 * \brief Do a blocking send call on buffer
 * 
 * \param context - handle to current request
 * 
 * \param buffer - data to send
 * 
 * \param size - how much data is in buffer
 * 
 * \return OK on success, else FAIL
 * 
 * This function will immediately send the buffer with a
 * blocking call.  Buffer will get freed as part of this call.
 * 
 **/
static error_type_t http_network_blocking_send( HttpPageContext *context,
                                                char *buffer,
                                                uint32_t size,
                                                bool free_buffer )
{
    int32_t result;
    int32_t nbytes;
    int32_t at_byte;
    bool still_sending;
    error_type_t e_res;

    ASSERT( NULL != context );
    ASSERT( NULL != buffer );
    ASSERT( 0 != size );

    result = 0;
    at_byte = 0;
    still_sending = true;
    e_res = OK;

    do
    {
        DBG_VERBOSE( "Block Send data\n" );
        context->lastAccess = posix_gettime_ticks( );
#ifdef HAVE_HTTPS
        // Pull read/write fd from ssl and compare
        /*if( ( ssl != NULL)  && 
                ( ( (intptr_t)context->deviceInfo == SSL_get_rfd( ssl ) ) || 
                  ( (intptr_t)context->deviceInfo == SSL_get_wfd( ssl ) ) ) )
        {
            nbytes = SSL_write( ssl, buffer + at_byte, size - at_byte );
            ASSERT( nbytes > 0 );
        }
        else
        {*/
#endif
            nbytes = send( ( intptr_t )context->deviceInfo, 
                    buffer + at_byte,
                    size - at_byte,
                    0
#ifdef __linux__
                  | MSG_NOSIGNAL /*don't generate SIGPIPE err*/
#endif
                    );
#ifdef HAVE_HTTPS
        //}
#endif

        DBG_VERBOSE( "NBytes: %d\n", nbytes );

        if ( nbytes < 0 )
        {
            result = errno;
            if ( result == ENOBUFS )
            {
                /* Ran out of buffers while trying to send 
                 * - sleep this thread for a little while */
                DBG_MSG( "wired network buffers full -- sleeping thread for a bit...\n" );
                posix_sleep_ms( 50 );
                nbytes = 0;
            }
            else 
            {
                #ifdef HAVE_EVENT_LOGGER
                event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
                #endif
                DBG_ERR( "Block Send: Context: %d, ErrNum: %d\n", 
                         context->deviceInfo,
                         result );

                still_sending = false;
                e_res = FAIL;
                http_context_destroy( context, true );
            }
        }
        else
        {
            at_byte += nbytes;
            if ( at_byte == size )
            {
                // All the data has been sent
                still_sending = false;
            }
        }

    } while( ( true == still_sending ) && ( context->state != e_http_state_resetting ) );

    // if an error occurs, don't free the buffer.
    if ( e_res == OK && free_buffer == true )
    {
        // Free incoming buffer
        MEM_FREE_AND_NULL( buffer );
    }

    return e_res;

}

static int32_t http_network_blocking_recv( HttpPageContext *context,
                                           char *buffer,
                                           uint32_t size,
                                           uint32_t *timeout )
{
    struct timeval select_wait;
    int32_t bytes;
    fd_set read_fds;
    int32_t result;

    DBG_VERBOSE( "%s - enter size=%d timeout=%d\n", __FUNCTION__, size, *timeout );
    memset( &read_fds, 0, sizeof(fd_set) );
    FD_SET( ( intptr_t )context->deviceInfo, &read_fds );

    bytes = 0;
    select_wait.tv_sec = *timeout / 1000;
    select_wait.tv_usec = ( *timeout % 1000 ) * 1000;
    result = select( (intptr_t)context->deviceInfo + 1, &read_fds, NULL, NULL, &select_wait );
    if ( result <= 0 )
    {
        if ( result == -1 )
        {
            int err;
            err = errno;
            DBG_ERR( "%s - select error: %d\n", __FUNCTION__, err );
            http_context_destroy( context, true );
            bytes = -1;
        }
        else
        {
            *timeout = 0;
        }
    }
    else
    {
        bytes = recv( ( intptr_t )context->deviceInfo,
                       buffer,
                       size,
                       MSG_DONTWAIT );
        if ( bytes == -1 )
        {
            int result;
            result = errno;
            DBG_ERR( "%s - recv error: %d\n", __FUNCTION__, result );
            http_context_destroy( context, true );
        }
        if ( bytes == 0 )
        {
#ifdef __linux__                    
            close( (intptr_t)context->deviceInfo );
#else                    
            tfClose( (intptr_t)context->deviceInfo );
#endif            
        }
    }

    DBG_VERBOSE( "%s - exit bytes=%d\n", __FUNCTION__, bytes );
    return bytes;
}



/** 
 * \brief Look at current connections and close least recently
 * used
 * 
 * This function will search through the list of active
 * connections and see which connection has not been used for
 * the longest time.  This connection will then be closed to
 * allow a new connection to take its place.
 * 
 **/
static void closeLRUConnection( void )
{
    error_type_t e_res;
    int32_t oldestTime = -1;
    HttpPageContext * oldestContext = NULL;
    HttpPageContext * context;
    HttpPageContext * next_context;

    // Find oldest context
    e_res = http_context_get_first( e_http_connection_network, &next_context );
    while ( OK == e_res )
    {
        context = next_context;
        DBG_MSG( "CurTime: %d\n", context->lastAccess );
        if( context->lastAccess < oldestTime )
        {
            DBG_MSG( "New oldest: time: %d - socketNumber: %#x\n", context->lastAccess, context->deviceInfo );
            oldestContext = context;
            oldestTime = oldestContext->lastAccess;
        }
        e_res = http_context_get_next( context, e_http_connection_network, &next_context );
    }
    ASSERT( NULL != oldestContext );
    http_context_destroy( oldestContext, true );
}


/** 
 * \brief Start up listening socket for web server
 *
 * \param servaddr - pointer to socket structure
 * 
 * \param conn_type - signifies UDP or TCP
 *
 * \return int socketHandle
 *
 * Create a socket for the web server.  This can either be a TCP
 * or UDP socket and then populate the socket storage structure
 * with its information
 *
 **/
static int startServerSocket( struct sockaddr_storage *servaddr, http_net_socket_protocol_t conn_type, uint32_t port )
{
    int sockfd;
    int result;

    if( conn_type == HTTP_NET_SOCKET_UDP )
    {
        #ifdef HAVE_IPV6
        sockfd = socket( PF_INET6, SOCK_DGRAM, IPPROTO_UDP );
        #else // !HAVE_IPV6
        sockfd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
        #endif // HAVE_IPV6
    }
    else
    {
        #ifdef HAVE_IPV6
        sockfd = socket( PF_INET6, SOCK_STREAM, IPPROTO_TCP );
        #else // !HAVE_IPV6
        sockfd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
        #endif // HAVE_IPV6
    }

    if ( sockfd < 0 )
    {
        #ifdef HAVE_EVENT_LOGGER
        event_logger_set_log_entry( EVENTLOG_LAN_SOCKET_ERROR ); 
        #endif
        DBG_ERR( "socket error: 0x%04x\n", errno );
        return -1;
    }

    // So that we can re-bind to it without TIME_WAIT problems
    int reuse_addr = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_addr, sizeof(reuse_addr));
    
    //bzero( servaddr, sizeof( struct sockaddr_storage ));
    memset(servaddr, 0, sizeof(struct sockaddr_storage)); 

#if __linux__
    #ifdef HAVE_IPV6
    struct sockaddr_in6 *ip6addr = (struct sockaddr_in6 *)servaddr;
    ip6addr->sin6_family = PF_INET6;
    ip6addr->sin6_port = htons( port );
    ip6addr->sin6_addr = in6addr_any;
    #else // !HAVE_IPV6
    struct sockaddr_in *ip4addr = (struct sockaddr_in *)servaddr;
    ip4addr->sin_family = PF_INET;
    ip4addr->sin_port = htons( port );
    ip4addr->sin_addr.s_addr = INADDR_ANY;
    #endif // HAVE_IPV6
#else
    #ifdef HAVE_IPV6
    servaddr->addr.ipv6.sin6_family = PF_INET6;
    servaddr->addr.ipv6.sin6_port = htons( port );
    servaddr->addr.ipv6.sin6_len = sizeof( struct sockaddr_in6 );
    #else // !HAVE_IPV6
    servaddr->addr.ipv4.sin_family = PF_INET;
    servaddr->addr.ipv4.sin_port = htons( port );
    servaddr->addr.ipv4.sin_len = sizeof( struct sockaddr_in );
    #endif // HAVE_IPV6
#endif

    DBG_VERBOSE( "Attempting to bind to port %d on socket %d\n", port, sockfd );
    result = bind( sockfd, ( struct sockaddr* )servaddr, sizeof( struct sockaddr_storage ));
    if ( result < 0 )
    {
        int _errno = errno;
        #ifdef HAVE_EVENT_LOGGER
        event_logger_set_log_entry( EVENTLOG_LAN_BIND_ERROR ); 
        #endif
        DBG_ERR( "bind error: 0x%04x on port %d\n", _errno, port );
        close(sockfd);
        
        // if address in use, return -1 which should retry
        if (_errno == EADDRINUSE)
            return -1;
        
        return -2;
    }

    if ( conn_type == HTTP_NET_SOCKET_TCP )
    {
        listen( sockfd, 8 );
    }

    return sockfd;
}


/** 
 * \brief Accept new socket and create local structure
 * 
 * \param server_fd - server socket id
 * 
 * \return error_type_t
 * - OK upon success
 *
 * - FAIL if there was an error opening the client socket
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The data from the IO pipe was bad and no context
 * was created
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory and no
 * context was created
 *
 * Accept a new socket, create HttpPageContext for socket, read
 * data that is currently available
 * 
 **/
static error_type_t contextAccept( int server_fd, uint32_t port_number )
{
    error_type_t e_res;
    HttpPageContext * context = NULL;
    int client_fd;
    struct sockaddr_storage clientAddr;
#ifdef __linux__                    
    socklen_t len; 
#else                    
    int len; 
#endif
    char *receive_buffer = NULL;
    int32_t result;
    int32_t nbytes = 0;
    uint32_t atIndex = 0;
    uint32_t new_bytes_parsed = 0;
#ifdef HAVE_HTTPS
    unsigned long ssl_error;
    int err = 1;
    int line;
    const char*    file;
    uint32_t i;
    uint32_t ssl_index = 0;

    if( ( port_number == HTTPS_SERVER_PORT ) && ( ssl_init_failed == true ) ) 
    {
        // Refuse to accept https connections if we failed to read a
        // certificate or private key
        return FAIL;
    }
#endif
    http_config_t http_config;
    if ( OK != http_config_get( &http_config ) ) 
    {
        DBG_ERR( "Failed to get http config\n" );
        ASSERT( 0 );
        return FAIL;
    }

    e_res = OK;

    receive_buffer = MEM_MALLOC( http_config.tcp_read_buffer_size );
    if ( receive_buffer == NULL )
    {
        DBG_ERR( "Failed to allocate buffer for tcp receive\n" );
        return FAIL;
    }

    len = sizeof( clientAddr );
    memset( &clientAddr, 0, len );

    /* We have a new connection to handle */
    client_fd = accept( server_fd, ( struct sockaddr* )&clientAddr, &len );

    DBG_MSG( "Accepting new client: %#x\n", client_fd );

    if( client_fd < 0 )
    {
        #ifdef HAVE_EVENT_LOGGER
        event_logger_set_log_entry( EVENTLOG_LAN_ACCEPT_ERROR ); 
        #endif
        result = errno;
        DBG_ERR( "Accept Error: ErrNum: %d\n", 
                 result );
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        atIndex = 0;

#ifdef HAVE_HTTPS
        // Have SSL read the data if this is an https request
        if( port_number == HTTPS_SERVER_PORT )
        {
            for( i = 0 ; i < http_config.max_active_http_connections ; i++ )
            {
                if( ssl[ i ] == NULL )
                {
                    ssl_index = i;
                    i = http_config.max_active_http_connections;
                }
            }
            ssl[ ssl_index ] = SSL_new( ctx );
            ASSERT( NULL != ctx );
            // Set the sock/fd number
            SSL_set_fd( ssl[ ssl_index ], client_fd );
            // server mode
            SSL_set_accept_state( ssl[ ssl_index ] ); 

            // Do we have data waiting to be read?
            if( SSL_pending( ssl[ ssl_index ] ) )
            {
                // Have we already initialized SSL?
                if( !SSL_is_init_finished( ssl[ ssl_index ] ) )
                {
                    // If not initialized, do it now
                    err = init_ssl_connection( ssl[ ssl_index ] );
                }
                else
                {
                    DBG_VERBOSE( "SSL: is inited\n" );
                }
            }
            else
            {
                DBG_VERBOSE( "SSL: Not pending\n");
            }

            if( err <= 0 )
            {
                DBG_ERR( "SSL: SSL_accept failed: " );
                err = SSL_get_error( ssl[ ssl_index ], err );
                // If SSL_accept fails we'd like to know why
                if( err == SSL_ERROR_SSL )
                {
                    DBG_ERR( "SSL_ERROR_SSL\n" );
                    ssl_error = ERR_get_error_line( &file, &line );
                    // Print file and line number while the error was found
                    DBG_ERR( "SSL: file = %s line = %d\n", file, line );
                    DBG_ERR( "SSL: ssl_error = %d\n", ssl_error );
                }
                else
                {
                    DBG_ERR( "%d\n", err );
                }
                e_res = FAIL;
            }
            else  // SSL_accept succeeded
            {
                // Read as much of header as is currently available
                nbytes = SSL_read( ssl[ ssl_index ], receive_buffer + atIndex, http_config.tcp_read_buffer_size - 1 - atIndex );
                DBG_VERBOSE( "nbytes: %d - atIndex: %d\n", nbytes, atIndex );
                if( nbytes > 0 )
                {
                    atIndex += nbytes;
                }
                switch( SSL_get_error( ssl[ ssl_index ], nbytes ) )
                {
                    case SSL_ERROR_NONE:
                        break;
                    case SSL_ERROR_WANT_READ:
                        // Fall through
                    case SSL_ERROR_WANT_WRITE:
                        nbytes = SSL_read( ssl[ ssl_index ], receive_buffer + atIndex, 
                                sizeof( receive_buffer ) - 1 - atIndex );
                        if( nbytes > 0 )
                        {
                            atIndex += nbytes;
                        }
                        XASSERT( nbytes > 0, nbytes );
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        // Fall through
                    default:
                        break;
                }

                e_res = http_context_create( &context,
                        (char *)receive_buffer,
                        atIndex,
                        &new_bytes_parsed,
                        e_http_connection_network,
                        (void *)(intptr_t)client_fd,
                        http_network_blocking_send,
                        http_network_blocking_recv );


                if ( ( HTTP_PARSING_JUNK == e_res ) || ( HTTP_MEMORY_OUT == e_res ) )
                {
                    if ( context != NULL)
                    {
                        http_context_destroy( context, true );
                    }
                    else
                    {
                        ssl_free_context( client_fd );
                        tfResetConnection( client_fd );
                        tfClose( client_fd );
                    }
                }

                if ( ( OK == e_res ) || ( HTTP_PARSING_INCOMPLETE == e_res ) )
                {
                    /* TODO determine if we need to be aware of this case and handle
                     * it as an error condition */
                    //ASSERT ( new_bytes_parsed == atIndex );
                    if( new_bytes_parsed != atIndex )
                    {
                        DBG_MSG( "SSL: new_bytes_parsed = %d, atIndex = %d\n", new_bytes_parsed, atIndex );
                    }
                }
            }
        }
        else // Not https
        {
#endif
            /* Read as much of header as is currently available */
            while ( ( atIndex < http_config.tcp_read_buffer_size ) && 
                    ( ( nbytes = recv( client_fd, receive_buffer + atIndex, http_config.tcp_read_buffer_size - atIndex, MSG_DONTWAIT ) ) > 0 ) )
            {
                DBG_VERBOSE( "nbytes: %d - atIndex: %d\n", nbytes, atIndex );
                atIndex += nbytes;
            }

            e_res = http_context_create( &context,
                                         (char *)receive_buffer,
                                         atIndex,
                                         &new_bytes_parsed,
                                         e_http_connection_network,
                                         (void *)(intptr_t)client_fd,
                                         http_network_blocking_send,
                                         http_network_blocking_recv );

            if ( ( HTTP_PARSING_JUNK == e_res ) || ( HTTP_MEMORY_OUT == e_res ) )
            {
                if ( context != NULL )
                {
                    http_context_destroy( context, true );
                }
                else
                {
#ifdef __linux__                    
                    shutdown( client_fd, SHUT_RDWR );
                    close( client_fd );
#else                    
                    tfResetConnection( client_fd );
                    tfClose( client_fd );
#endif
                }
            }

#ifdef HAVE_HTTPS
        }
#endif
    }

    if ( OK == e_res )
    {
        /* Process the http context */
        e_res = http_context_process( context );

        if ( ( e_res == FAIL ) )
        {
            http_context_destroy( context, false );
        }
    }

    MEM_FREE_AND_NULL( receive_buffer );

    return e_res;
}


/** 
 * \brief Write available information to socket
 * 
 * \param context - context with data to write
 * 
 * \return error_type_t:
 * - OK upon success
 *
 * - FAIL if there was an error reading from the client socket
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The data from the IO pipe was bad
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory 
 *
 * Write available information for current context, if finish
 * write reset socket for a new read
 * 
 **/
static error_type_t contextWrite( HttpPageContext *context )
{
    error_type_t e_res;
    bool connectionClosed;
    int32_t result = 0;
    int32_t nbytes;
    bool done_writing;
#ifdef HAVE_HTTPS
    uint32_t i;
    uint32_t ssl_index;
    http_config_t http_config;
    if ( OK != http_config_get( &http_config ) ) 
    {
        ASSERT( 0 );
        return FAIL;
    }
#endif

    ASSERT( NULL != context );

    /* TODO Determine a better way to do this entire write function
     * hiding the context entirely from the parser here */

    e_res = OK;
    done_writing = false;

    context->lastAccess = posix_gettime_ticks( );

    DBG_MSG( "Data to write from %#x\n", context->deviceInfo );

    DBG_VERBOSE( "Entering send data loop\n" );
    // Send until we can't
    connectionClosed = false;

    do
    {
        nbytes = 0;
        if ( context->dataChain && ( 0 != ( context->dataChain->size - context->dataChain->atByte ) ) &&
             ( context->state != e_http_state_resetting ) )
        {
            DBG_VERBOSE( "Sending data chain element\n" );
#ifdef HAVE_HTTPS
            ssl_index = http_config.max_active_http_connections;
            // Look for an ssl connection associated with this socket
            for( i = 0 ; i < http_config.max_active_http_connections ; i++ )
            {
                // Pull read/write fd from ssl and compare
                if( ( ssl[ i ] != NULL)  && 
                          (intptr_t)context->deviceInfo == SSL_get_wfd( ssl[ i ] ) ) 
                {
                    ssl_index = i;
                    i = http_config.max_active_http_connections;
                }
            }
            //  Did we find an ssl context associated with this socket?
            if( ssl_index < http_config.max_active_http_connections )
            {
                nbytes = SSL_write( ssl[ ssl_index ], 
                        context->dataChain->data + context->dataChain->atByte,
                        context->dataChain->size - context->dataChain->atByte );
                switch( SSL_get_error( ssl[ ssl_index ], nbytes ) )
                {
                    case SSL_ERROR_NONE:
                        break;
                    case SSL_ERROR_WANT_READ:
                        // Fall through
                    case SSL_ERROR_WANT_WRITE:
                        nbytes = SSL_write( ssl[ ssl_index ], 
                                context->dataChain->data + context->dataChain->atByte,
                                context->dataChain->size - context->dataChain->atByte );
                        XASSERT( nbytes > 0, nbytes );
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        // Fall through
                    default:
                        http_context_destroy( context, true );
                        done_writing = true;
                        connectionClosed = true;
                        e_res = FAIL;
                }
            }
            else  // not ssl
            {
#endif
                nbytes = send( ( intptr_t )context->deviceInfo, 
                        context->dataChain->data + context->dataChain->atByte, 
                        context->dataChain->size - context->dataChain->atByte, 
                        MSG_DONTWAIT 
#ifdef __linux__
                      | MSG_NOSIGNAL /*don't generate SIGPIPE err*/
#endif
                );
#ifdef HAVE_HTTPS
            }
#endif
            DBG_VERBOSE( "NBytes: %d\n", nbytes );
            if ( ( e_res == OK ) && ( nbytes < 0 ) )
            {
                result = errno;
                if ( result == ENOBUFS )
                {
                    // Ran out of buffers while trying to send - sleep this thread for a little while
                    DBG_MSG( "Web buffers full -- sleeping thread for a bit...\n" );
                    posix_sleep_ms(50);
                }
                else if ( result != EWOULDBLOCK )
                {
                    #ifdef HAVE_EVENT_LOGGER
                    event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
                    #endif
                    DBG_ERR( "Send: Context: %#x, ErrNum: %d\n",
                             context->deviceInfo,
                             result, 
                             strerror( result ) );
                    http_context_destroy( context, true );

                    done_writing = true;
                    connectionClosed = true;
                }
                else 
                {
                    #ifdef HAVE_EVENT_LOGGER
                    event_logger_set_log_entry( EVENTLOG_LAN_WOULD_BLOCK ); 
                    #endif
                    DBG_VERBOSE( "Network send would block error...\n" );
                    posix_sleep_ms(50);
                }
            }
            else
            {
                /* Did we send all the data for that link?  If so, release the link. */
                context->dataChain->atByte += nbytes;
            }
        }
        else
        {
            if (context->dataChain)
            {
               deleteFirstDataElement( context );
            }
            else
            {           
                done_writing = true;
            }
        }

    } while( false == done_writing );


    /* Did we complete this context? */
    if ( connectionClosed == true )
    {
        /* The calling functions because we just freed memory it was using */
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        if( context->state >= e_http_state_body_complete )
        {
            DBG_MSG( "finished %#x for current write - reset for next read\n", context->deviceInfo );
            http_context_destroy( context, false );
        }
    }

    return e_res;
}


/** 
 * \brief Read available information from socket and add to the http context
 * 
 * \param context - valid http context with data to read
 * 
 * \return error_type_t:
 * - OK upon success
 *
 * - FAIL if there was an error reading from the client socket
 *
 * - HTTP_PARSING_INCOMPLETE The request buffer does not contain the full
 * request, caller should continue reading data from its I/O pipe and
 * then call http_context_add_data() until OK is returned.  It is possible
 * the I/O pipe has timed-out.  In that case the context should be destroyed 
 * using: http_context_destroy()
 *
 * - HTTP_PARSING_JUNK The data from the IO pipe was bad
 *
 * - HTTP_MEMORY_OUT There was an problem getting dynamic memory
 * 
 * Read available information for current context, check for
 * complete header to begin parsing data
 * 
 **/
static error_type_t contextRead( HttpPageContext *context )
{
    error_type_t e_res;
    int32_t nbytes = 0;
    int32_t result = 0;
    int32_t atIndex = 0;
    char *receive_buffer;
    uint32_t actual_request_bytes_parsed;
    bool recv_valid;
#ifdef HAVE_HTTPS
    uint32_t i;
    uint32_t ssl_index;
#endif
    http_config_t http_config;
    if ( OK != http_config_get( &http_config ) ) 
    {
        DBG_ERR( "Failed to get http config\n" );
        ASSERT( 0 );
        return FAIL;
    }

    ASSERT( NULL != context );
    e_res = OK;

    receive_buffer = MEM_MALLOC( http_config.tcp_read_buffer_size );
    if ( receive_buffer == NULL )
    {
        DBG_ERR( "Failed to allocate buffer for tcp receive\n" );
        return FAIL;
    }

    context->lastAccess = posix_gettime_ticks( );

    DBG_MSG( "More data to read on %#x\n", context->deviceInfo );

    atIndex = 0;
    recv_valid = true;

    /* Read as much of header as is currently available */
    while ( ( true == recv_valid ) && ( context->state != e_http_state_resetting ) )
    {
        if ( atIndex < http_config.tcp_read_buffer_size )
        {
#ifdef HAVE_HTTPS
            ssl_index = http_config.max_active_http_connections;
            // Look for an ssl connection associated with this socket
            for( i = 0 ; i < http_config.max_active_http_connections ; i++ )
            {
                // Pull read/write fd from ssl and compare
                if( ( ssl[ i ] != NULL)  && 
                        ( (intptr_t)context->deviceInfo == SSL_get_rfd( ssl[ i ] ) ) )
                {
                    ssl_index = i;
                    i = http_config.max_active_http_connections;
                }
            }
            //  Did we find an ssl context associated with this socket?
            if( ssl_index < http_config.max_active_http_connections )
            {
                nbytes = SSL_read( ssl[ ssl_index ], receive_buffer + atIndex, http_config.tcp_read_buffer_size - atIndex );
                if( nbytes > 0 )
                {
                    atIndex += nbytes;
                }
                switch( SSL_get_error( ssl[ ssl_index ], nbytes ) )
                {
                    case SSL_ERROR_NONE:
                        break;
                    case SSL_ERROR_WANT_READ:
                        // Fall through
                    case SSL_ERROR_WANT_WRITE:
                        nbytes = SSL_read( ssl[ ssl_index ], receive_buffer + atIndex, 
                                           http_config.tcp_read_buffer_size - atIndex );
                        if( nbytes > 0 )
                        {
                            atIndex += nbytes;
                        }
                        XASSERT( nbytes > 0, nbytes );
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        // Fall through
                    default:
                        http_context_destroy( context, true );
                        e_res = FAIL;
                }
                recv_valid = false;
            }
            else  // not ssl
            {
#endif
                nbytes = recv(
                        ( intptr_t )context->deviceInfo,
                        receive_buffer + atIndex,
                        http_config.tcp_read_buffer_size - atIndex,
                        MSG_DONTWAIT );
#ifdef HAVE_HTTPS
            }
#endif
        }
        else
        {
            /* filled up our buffer */
            recv_valid = false;
        }

        if ( true == recv_valid )
        {
            if ( nbytes > 0 )
            {
                DBG_VERBOSE( "nbytes: %d - atIndex: %d\n", nbytes, atIndex );
                atIndex += nbytes;
            }
            else
            {
                /* recv passed an error instead of # bytes transferred */
                recv_valid = false;
            }
        }
    }

    if( ( OK == e_res ) && ( nbytes == 0 ) )
    {
        /* Socket was closed from other end - remove from activeContext list */
        DBG_MSG( "Connection( %#x ) closed from other end - shut down our side\n", context->deviceInfo );
        http_context_destroy( context, false );
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        if( nbytes < 0 )
        {
            result = errno;
            if ( result != EWOULDBLOCK )
            {
                #ifdef HAVE_EVENT_LOGGER
                event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
                #endif
                DBG_ERR( "Recv: Context: %#x, ErrNum: %d\n",
                         context->deviceInfo,
                         result, 
                         strerror( result ) );
                http_context_destroy( context, true );
                e_res = FAIL;
            }
        }
    }

    if ( OK == e_res )
    {
        e_res = http_context_add_data( context,
                receive_buffer,
                atIndex,
                &actual_request_bytes_parsed );
        if ( HTTP_PARSING_JUNK == e_res )
        {
            http_context_destroy( context, true );
        }
    }

    if ( OK == e_res )
    {
        /* Process the http context */
        e_res = http_context_process( context );

        if ( ( e_res == FAIL ) )
        {
            http_context_destroy( context, false );
        }
    }

    MEM_FREE_AND_NULL( receive_buffer );

    return e_res;
}

#ifdef HAVE_HTTPS
void ssl_free_context( int32_t device_fd )
{
    uint32_t i;
    
    http_config_t http_config;
    if ( OK != http_config_get( &http_config ) ) 
    {
        ASSERT( 0 );
        return;
    }

    for( i = 0 ; i < http_config.max_active_http_connections ; i++ )
    {
        // Pull read/write fd from ssl and compare
        if( ( ssl[ i ] != NULL)  && 
            ( ( (int32_t)device_fd == SSL_get_rfd( ssl[ i ] ) ) || 
              ( (int32_t)device_fd == SSL_get_wfd( ssl[ i ] ) ) ) )
        {
            SSL_shutdown( ssl[ i ] );
            SSL_free( ssl[ i ] );
            ssl[ i ] = NULL;
        }
    }
}

static int init_ssl_connection( SSL *ssl )
{
    int err;
    int ret;

    ret = 0;
    err = SSL_accept( ssl );
    if( err <= 0 )
    {
        if( BIO_sock_should_retry( err ) );
        {
            ret = 1;
        }
    }
    return ret;
}
#endif

error_type_t http_network_add_port( uint32_t port, http_net_socket_protocol_t type )
{
    http_sockets_t *new_socket;

    if ( port == 0 )
    {
        return FAIL;
    }

    new_socket = sockets;
    while ( new_socket != NULL )
    {
        if ( ( new_socket->port_number == port ) &&
             ( new_socket->socket_type == type ) )
        {
            DBG_MSG( "%s - socket already open\n", __FUNCTION__ );
            return OK;
        }
        new_socket = new_socket->next;
    }

    new_socket = ( http_sockets_t *)MEM_MALLOC( sizeof( http_sockets_t ) );
    if ( new_socket == NULL )
    {
        #ifdef HAVE_EVENT_LOGGER
        event_logger_set_log_entry( EVENTLOG_LAN_FAIL ); 
        #endif
        ASSERT( "HTTP: Failed to allocate memory for new web server socket\n" );
        return FAIL;
    }

    DBG_MSG( "Adding new %s socket for port %d\n", type ? "TCP" : "UDP", port );

    new_socket->socket_type = type;
    new_socket->port_number = port;
    new_socket->socket_descriptor = startServerSocket( &new_socket->socket_address, 
                                                       type, 
                                                       port );

    if ( sockets == NULL )
    {
        new_socket->next = NULL;
        sockets = new_socket;
    }
    else
    {
        new_socket->next = sockets;
        sockets = new_socket;
    }

    return OK;
}

error_type_t http_network_remove_port( uint32_t port, http_net_socket_protocol_t type )
{
    error_type_t e_res;
    http_sockets_t *prev_sock;
    http_sockets_t *cur_sock;
    http_config_t config;
    uint32_t i;

    if ( port == 0 )
    {
        return FAIL;
    }

    if ( http_config_get( &config ) != OK )
    {
        return FAIL;
    }
    for ( i = 0; i < config.socket_table_size; i++ )
    {
        if ( ( config.socket_table[ i ].port_number == port ) && 
             ( config.socket_table[ i ].socket_type == type ) )
        {
            DBG_ERR( "%s: Warning removing a default socket\n", __FUNCTION__ );
        }
    }

    DBG_MSG( "Removing %s socket for port %d\n", type ? "TCP" : "UDP", port );
    cur_sock = sockets;
    prev_sock = NULL;
    e_res = FAIL;
    while ( ( e_res == FAIL ) && ( cur_sock != NULL ) )
    {
        if ( ( cur_sock->port_number == port ) && ( cur_sock->socket_type == type ) )
        {
            // Found the socket to close
            if ( cur_sock == sockets )
            {
                sockets = sockets->next;
            }
            else
            {
                prev_sock->next = cur_sock->next;
            }

#ifdef __linux__                    
            close( cur_sock->socket_descriptor );
#else                    
            tfClose( cur_sock->socket_descriptor );
#endif            
            MEM_FREE_AND_NULL( cur_sock );
            e_res = OK;
            break;
        }
        prev_sock = cur_sock;
        cur_sock = cur_sock->next;
    }

    if ( e_res == OK )
    {
        HttpPageContext * context;
        HttpPageContext * next_context;

        // Look for any active connections on this port that just got closed
        // if any are found - mark them for close after current transaction
        e_res = http_context_get_first( e_http_connection_network, &next_context );
        while ( OK == e_res )
        {
            context = next_context;
            if( context->port == port )
            {
                DBG_MSG( "Marking context id: %d - socket: %#X for delete\n", context->id, context->deviceInfo );
                context->state = e_http_state_resetting;
            }
            e_res = http_context_get_next( context, e_http_connection_network, &next_context );
        }
        e_res = OK;
    }
    else
    {
        DBG_ERR( "Failed to find type/port to remove\n" );
    }

    return e_res;
}

#ifdef HAVE_CMD
static const char *http_cmd_sockets_desc  = "List the resources that have been registered with the HTTP module";
static const char *http_cmd_sockets_usage = NULL;
static const char *http_cmd_sockets_notes = NULL;
static int http_cmd_sockets_cb( int argc, char *argv[] )
{
    http_sockets_t *sock;

    cmd_printf( "Sockets currently used by HTTP module: \n");
    sock = sockets;
    while( sock != NULL )
    {
        cmd_printf( "  Socket: % 3d - Type: %s - Port: %d\n", (int)sock->socket_descriptor, sock->socket_type ? "TCP" : "UDP", (int)sock->port_number );
        sock = sock->next;
    }
    return CMD_OK;
}

#ifdef WIRED_NETWORK
#ifdef DEBUG
pthread_t http_network_parser_get_tid( void )
{
    return http_network_thread_id;
}
#endif // DEBUG
#endif

#endif // HAVE_CMD
