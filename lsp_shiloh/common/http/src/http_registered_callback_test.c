/*
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <string.h>
#include <stdlib.h>
#include "cmd_proc_api.h"
#include "http_api.h"
#include "lassert.h"
#include "dprintf.h"
#include "memAPI.h"
#include "tx_api.h"
#include "ATypes.h"
#include "sys_init_api.h"
#include "http_rom.h"

void http_test_callback( http_request_t handle, http_resource_event_flag_t event, void *usr_ctxt );


#define THREAD_STACK_SIZE POSIX_MIN_STACK_SIZE 
static pthread_t            http_test_thread_id;
static ALIGN8 uint8_t       http_test_stack[ THREAD_STACK_SIZE ];

pthread_mutex_t http_test_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t  http_test_cond  = PTHREAD_COND_INITIALIZER;
uint32_t http_test_connection_flags = 0;

static http_request_t       http_handle;
void* http_tester( void* input );

void initialize_http_test( void )
{
    posix_create_thread(&http_test_thread_id, http_tester, 0, "http_tester", http_test_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
}

void http_test_callback( http_request_t handle, http_resource_event_flag_t event, void *usr_ctxt )
{
    char *buffer;
    uint32_t length;
    dbg_printf( "http_test: %s enter - handle: 0x%08X, event: %d, usr_ctxt: 0x%X\n", __FUNCTION__, handle, event, usr_ctxt );
    switch (event)
    {
        case HTTP_RESOURCE_HEADER_DATA_AVAILABLE:
            http_resource_get_header_length( handle, &length );
            dbg_printf( "http_test: Header data length: %d bytes\n", length );
            buffer = MEM_MALLOC( length );
            http_resource_get_header( handle, buffer, length );
            dbg_printf( "http_test: Header data: \n%s\n", buffer );
            MEM_FREE_AND_NULL( buffer );
            pthread_mutex_lock(&http_test_mutex);
            http_test_connection_flags |= 1;
            pthread_cond_broadcast(&http_test_cond);
            pthread_mutex_unlock(&http_test_mutex);
            http_handle = handle;
            break;
        case HTTP_RESOURCE_CONNECTION_ABORTED:
            dbg_printf( "http_test: CONNECTION ABORTED\n" );
            break;            
    }

}

void http_old_test_callback( http_request_t request,         
                             http_resource_event_t event,    
                             char *buffer,                   
                             uint32_t bytes )
{
    static int sock;
    uint32_t length;
//    dbg_printf( "http_test: %s enter - handle: 0x%08X, event: %d, size: %d\n", __FUNCTION__, request, event, bytes );
    switch ( event )
    {
        case HTTP_RESOURCE_HEADER_DATA:
            http_resource_get_header_length( request, &length );
            dbg_printf( "http_test: Header data length: %d bytes\n", length );
            dbg_printf( "http_test: Header from passed in buffer: %s\n", buffer );
            http_resource_buffer_free( request, buffer );

            sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
            struct sockaddr_storage sockaddr;
            sockaddr.addr.ipv4.sin_family = AF_INET;
            sockaddr.addr.ipv4.sin_len = sizeof( struct sockaddr_in);
            sockaddr.addr.ipv4.sin_port = htons( 9100 );
            inet_pton(AF_INET, "127.0.0.1", (void *)&sockaddr.addr.ipv4.sin_addr );
            if ( connect( sock, (const void *)&sockaddr, sizeof( sockaddr ) ) == -1 )
            {
                dbg_printf( "http_test: Failed to connect\n" );
            }
            break;
        case HTTP_RESOURCE_BODY_DATA:
            if ( send( sock, buffer, bytes, 0 ) != bytes )
            {
                dbg_printf( "http_test: Failed to send %d bytes\n", bytes );
            }
            http_resource_buffer_free( request, buffer );    
            break;
        case HTTP_RESOURCE_BODY_DATA_END:
            if ( buffer )
            {
                if ( send( sock, buffer, bytes, 0 ) != bytes )
                {
                    dbg_printf( "http_test: Failed to send %d bytes\n", bytes );
                }
                http_resource_buffer_free( request, buffer );    
            }
#ifdef __linux__                    
            close( sock );
#else                    
            tfClose( sock );
#endif            
            http_handle = request;
            pthread_mutex_lock(&http_test_mutex);
            http_test_connection_flags |= 2;
            pthread_cond_broadcast(&http_test_cond);
            pthread_mutex_unlock(&http_test_mutex);
            break;
        case HTTP_RESOURCE_CLOSED:
            dbg_printf( "http_test: didn't see that coming...\n" );
            break;
    }
}

void* http_tester( void* input )
{
    ULONG requested_flags;
    ULONG actual_flags;
    char buffer[1024];
    int bytes;
    uint32_t content_length;
    uint32_t total_bytes;
    uint32_t timeout;
    char *out_buf;
    http_response_type_t content_type;
    http_request_method_t method;
    int sock;

    char * methods[] = {
       "GET",  
       "PUT",  
       "POST", 
       "DELETE",
       "HEAD"
    };

    sock = -1;

    SysWaitForInit( );

    http_resource_register( "/old", 1119, http_old_test_callback );
    http_resource_register_event( "/test", 1119, 0, http_test_callback, ( void * )0xBEAF );
    http_resource_register_event( "/test2", 1119, 0, http_test_callback, ( void * )0xDEAD );

    while ( 1 )
    {
        requested_flags = 3;
        total_bytes = 0;
        pthread_mutex_lock(&http_test_mutex);
        while(!(http_test_connection_flags & requested_flags))
        {
            pthread_cond_wait(&http_test_cond, &http_test_mutex);
        }
        actual_flags = http_test_connection_flags;
        http_test_connection_flags = 0;
        pthread_mutex_unlock(&http_test_mutex);

        http_resource_get_content_length( http_handle, &content_length );
        http_resource_get_content_type( http_handle, &content_type );
        http_resource_get_method( http_handle, &method );
        dbg_printf( "http_test: Got event - Method type: %s -- read in %d bytes of data\n", methods[method], content_length );
        dbg_printf( "http_test: Content type: %d\n", content_type );

        if ( http_resource_get_header_value( http_handle, "User-Agent", buffer, 1024 ) == OK )
            dbg_printf( "http_test: User-Agent: %s\n", buffer );
        if ( http_resource_get_header_value( http_handle, "Connection", buffer, 1024 ) == OK )
            dbg_printf( "http_test: Connection: %s\n", buffer );
        if ( http_resource_get_header_value( http_handle, "Accept-Language", buffer, 1024 ) == OK )
            dbg_printf( "http_test: Accept-Language: %s\n", buffer );

        if ( http_resource_get_header_value( http_handle, "Content-Length", buffer, 1024 ) == OK )
        {
            dbg_printf( "http_test: Content-Length: %s\n", buffer );
        }
        else
        {
            dbg_printf( "http_test: Failed to find Content Length\n" );
        }

        if ( actual_flags & 1 )
        {

            if ( method == HTTP_METHOD_PUT )
            {
                dbg_printf( "http_test: Create socket\n" );
                sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
                struct sockaddr_storage sockaddr;
                sockaddr.addr.ipv4.sin_family = AF_INET;
                sockaddr.addr.ipv4.sin_len = sizeof( struct sockaddr_in);
                sockaddr.addr.ipv4.sin_port = htons( 9100 );
                inet_pton(AF_INET, "127.0.0.1", (void *)&sockaddr.addr.ipv4.sin_addr );
                if ( connect( sock, (const void *)&sockaddr, sizeof( sockaddr ) ) == -1 )
                {
                    dbg_printf( "http_test: Failed to connect\n" );
                }
            }

            while ( total_bytes < content_length )
            {
                timeout = 1000;
                bytes = http_resource_read( http_handle, buffer, 1024, &timeout );
                if ( bytes <= 0 )
                {
                    if ( timeout == 0 )
                    {
                        dbg_printf( "http_test: Timeout try again - bytes: %d, timeout: %d\n", bytes, timeout );
                        continue;
                    }
                    else
                    {
                        dbg_printf( "Bad things happened get out of town!\n" );
                        break;
                    }
                }
                if ( method == HTTP_METHOD_PUT )
                {
                    if ( send( sock, buffer, bytes, 0 ) != bytes )
                    {
                        dbg_printf( "http_test: Failed to send %d bytes\n", bytes );
                    }
                }
                total_bytes += bytes;
            }
            if ( method == HTTP_METHOD_PUT )
            {
                dbg_printf( "http_test: Closing socket\n" );
#ifdef __linux__                    
                close( sock );
#else                    
                tfClose( sock );
#endif                
            }
            dbg_printf( "http_test: Total Bytes: %d\n", total_bytes );
        }

        if ( total_bytes < content_length )
        {
            dbg_printf( "one more read...\n" );
            bytes = http_resource_read( http_handle, buffer, 1024, &timeout );
            continue;
        }

        dbg_printf( "http_test: adding content type\n" );
        http_resource_set_status_code( http_handle, HTTP_200_OK );
        dbg_printf( "http_test: adding status code\n" );
        http_resource_set_content_type( http_handle, HTTP_TYPE_PLAIN );
        if ( method == HTTP_METHOD_GET )
        {
            dbg_printf( "http_test: header complete\n" );
            http_resource_header_complete( http_handle );

            dbg_printf( "http_test: writing data\n" );
            out_buf = MEM_MALLOC( 1024 );
            ASSERT( out_buf != NULL );
            memset(out_buf, 'S',1024 );
            if ( actual_flags & 1 )
            {
                http_resource_write( http_handle, out_buf, 1024 );
            }
            else
            {
                http_resource_send_data( http_handle, out_buf, 1024 );
            }
        }
        else
        {
            dbg_printf( "http_test: adding set content length\n" );
            http_resource_set_content_length( http_handle, 5 );

            dbg_printf( "http_test: header complete\n" );
            http_resource_header_complete( http_handle );

            out_buf = MEM_MALLOC( 5 + 1 );
            strcpy( out_buf, "scott" );
            if ( actual_flags & 1 )
            {
                http_resource_write( http_handle, out_buf, 5 );
            }
            else
            {
                http_resource_send_data( http_handle, out_buf, 5 );
            }
        }
        dbg_printf( "http_test: send complete\n" );
        http_resource_send_complete( http_handle );
        dbg_printf( "http_test: Finished with transaction\n" );
    }
}


