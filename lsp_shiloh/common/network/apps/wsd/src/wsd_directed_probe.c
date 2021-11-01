/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file wsd_directed_probe.c
 *
 * \brief This file contains the interface between WSD and the HTTP server 
 *        used to handle directed probes
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include <stdint.h>
#include "logger.h"
#include "memAPI.h"
#include "net_wsd_api.h"
#include "wsd.h"
#include "http_api.h"
#include "dc/dc_DpwsRequest.h"

#include "net_iface_vars.h"
#include "dprintf.h"


// Debug configuration
#define DBG_PRFX "NET-WSD-PROBE: "

#define THREAD_STACK_SIZE   PTHREAD_STACK_MIN
#define WSD_DIRECTED_PROBE_URI "/StableWSDiscoveryEndpoint/schemas-xmlsoap-org_ws_2005_04_discovery"

typedef struct probe_data_node_s
{
    ATLISTENTRY node;
    uint8_t* buf;       ///< data buffer
    uint32_t buflen;    ///< length of buffer
    uint32_t bufoffset; ///< current read offset in buffer
}probe_data_node_t;

typedef struct 
{
    struct dpws     *dpws;
    transport_fns_t *callbacks;
    http_context_t  *context;
    ATLISTENTRY     recv_data_list;
    char            *write_buf;
    uint32_t        write_buf_len;
    uint32_t        write_buf_at_byte;
} wsd_directed_probe_t;

//--------------------------------------
// Local Function Declarations
//--------------------------------------
static void wsd_directed_probe_callback( http_request_t request, http_resource_event_t event, char *buffer, uint32_t buffer_size_in_bytes );

//--------------------------------------
// Local Function Definitions
//--------------------------------------
void wsd_register_directed_probe( void )
{
    http_resource_register( WSD_DIRECTED_PROBE_URI, 0, wsd_directed_probe_callback );
}

void wsd_deregister_directed_probe( void )
{
    http_resource_deregister( WSD_DIRECTED_PROBE_URI, 0 );
}

/**
 * Callback function used by the SOAP engine to write outgoing response data.
 * Note: it is the responsibility of the transport layer to provide buffering.
 * @param dpws the dpws struct associated to the current request
 * @param transport_data the opaque transport-specific parameter associated
 * to the request
 * @param buf a pointer to the beginning of the buffer to be written
 * @param buflen the number of characters to be written
 * @return the number of written characters, or -1 in case of error.
 */
int wsd_directed_probe_fsend(struct dpws *dpws, void *transport_data, const char *buf, size_t buflen)
{
    char *buffer = ( char * ) MEM_MALLOC( buflen );
    if ( buffer == NULL )
    {
        return -1;
    }
    memcpy( buffer, buf, buflen );
    http_resource_send_data( ( http_request_t )transport_data, buffer, buflen );

    return buflen;
}

/**
 * Callback function used by the SOAP engine to read incoming request data.
 * @param dpws the dpws struct associated to the current request
 * @param transport_data the opaque transport-specific parameter associated
 * to the request
 * @param buf a pointer to the beginning of the buffer where read data can
 * be stored
 * @param buflen the size of the buffer
 * @return the number of read characters, or -1 in case of error.
 */
int wsd_directed_probe_frecv( struct dpws *dpws, void *transport_data, char *buf, size_t buflen )
{
    probe_data_node_t* data_node;
    ATLISTENTRY* list_node;
    wsd_directed_probe_t *probe_info;
    http_request_t request = (http_request_t )transport_data;
    int bytes_available;
    int bytes = 0;

    http_resource_get_data_ptr( request, ( void** )&probe_info );
    while ( bytes < buflen )
    {
        list_node = ATListHead(&probe_info->recv_data_list);
        if ( list_node == NULL)
        {
            break;
        }
        data_node = CONTAINING_RECORD(list_node, probe_data_node_t, node);
        bytes_available = data_node->buflen - data_node->bufoffset;
        if ( ( buflen - bytes ) >= bytes_available)
        {
            // Copy all of current link;
            memcpy( buf + bytes, data_node->buf + data_node->bufoffset, bytes_available );
            bytes += bytes_available;
            http_resource_buffer_free( request, (char*)data_node->buf );
            ATRemoveHeadList( &probe_info->recv_data_list );
            MEM_FREE_AND_NULL( data_node );
        }
        else 
        {
            memcpy( buf + bytes, data_node->buf + data_node->bufoffset, buflen - bytes );
            bytes += buflen - bytes;
            data_node->bufoffset += buflen - bytes;
        }
    }

    return bytes;
}

/**
 * Callback function used by the SOAP engine to notify the transport layer
 * that it is starting the response.
 * @param dpws the dpws struct associated to the current request
 * @param transport_data the opaque transport-specific parameter associated
 * to the request
 * @param status a parameter that can take four values: DC_SOAP_RESPONSE,
 * DC_SOAP_EMPTY_RESPONSE, DC_SOAP_SENDER_FAULT, DC_SOAP_RECEIVER_FAULT.
 * @param mtype the content type of the response.
 * @param len the length of the response contents if known, otherwise 0.
 * @return DPWS_OK on success, -1 on error
 */
int wsd_directed_probe_fresponse( struct dpws *dpws, void *transport_data, int status, media_type_t *mtype, size_t len )
{
    probe_data_node_t* data_node;
    ATLISTENTRY* list_node;
    http_status_code_t code;
    char *buf;
    wsd_directed_probe_t *probe_info;
    http_request_t request = (http_request_t )transport_data;

    http_resource_get_data_ptr( request, ( void** )&probe_info );

    if(mtype != NULL)
    {
        if ( ( mtype->type != NULL ) && ( mtype->subtype != NULL ) )
        {
            DBG_VERBOSE( "MediaType: %s/%s\n", mtype->type, mtype->subtype );
        }    
    }

    switch ( status )
    {
        case 200:
            code = HTTP_200_OK;
            break;
        case 201:
            code = HTTP_201_CREATED;
            break;
        case 202:
            code = HTTP_202_ACCEPTED;
            break;
        case 204:
            code = HTTP_204_NO_CONTENT;
            break;
        case 301:
            code = HTTP_301_MOVED;
            break;
        case 304:
            code = HTTP_304_NOT_MODIFIED;
            break;
        case 400:
            code = HTTP_400_BAD_REQUEST;
            break;
        case 401:
            code = HTTP_401_UNAUTHORIZED;
            break;
        case 404:
            code = HTTP_404_NOT_FOUND;
            break;
        case 405:
            code = HTTP_405_METHOD_NOT_ALLOWED;
            break;
        case 409:
            code = HTTP_409_CONFLICT;
            break;
        default:
        case 500:
            code = HTTP_500_SERVER_ERROR;
            break;
        case 503:
            code = HTTP_503_SERVICE_UNAVAILABLE;
            break;
    }
    http_resource_set_status_code( request, code );
    if(mtype != NULL)
    {

		if ( ( mtype->type != NULL ) && ( mtype->subtype != NULL ) && ( *mtype->type != '\0' ) && ( *mtype->subtype != '\0' ) )
		{
			buf = MEM_MALLOC( strlen(  mtype->type ) + strlen( mtype->subtype ) + 1 );
			if ( buf != NULL )
			{
				minSprintf( buf, "%s/%s", mtype->type, mtype->subtype );
				http_resource_add_header_field( request, "Content-Type", strlen( "Content-Type" ), buf, strlen( buf ) );
				MEM_FREE_AND_NULL( buf );
				net_wsd_config_t *config = wsd_get_config();
				if(config->wsd_custom_func_table->directed_probes_custom_header)
				{
					if( config->wsd_custom_func_table->directed_probes_custom_header(request) == FAIL )
					{
						return -1;
					}
				}
			}
			else
			{
				return -1;
			}
		}
	}
    if ( len != 0 )
    {
        http_resource_set_content_length( request, len );
    }

	// In an error condition, some links might still exist
	// Delete all the links and free any buffers
    while ( ( list_node = ATRemoveHeadList( &probe_info->recv_data_list ) ) != NULL )
    {
        data_node = CONTAINING_RECORD(list_node, probe_data_node_t, node);
        http_resource_buffer_free( request, (char*)data_node->buf );
        MEM_FREE_AND_NULL( data_node );
    }

    http_resource_header_complete( request );

    return DPWS_OK;
}

static char *extract_header_value( struct dpws *dpws, char *request, const char *header_name )
{
    char *end_ptr;
    char *start_ptr;
    char *dst;

    start_ptr = strstr( request, header_name );
    if ( NULL != start_ptr )
    {
        start_ptr = strchr( start_ptr, ':' );
        if ( NULL != start_ptr )
        {
            // Skip colon
            start_ptr++;
            // Skip whitespace
            while ( ( ( *start_ptr == ' ' ) ||
                      ( *start_ptr == '\r' ) ||
                      ( *start_ptr == '\n' ) ||
                      ( *start_ptr == '\t' ) ) && 
                    ( *start_ptr != '\0' ) )
            {
                start_ptr++;
            }
            // We are now at the start of the header
            start_ptr = start_ptr;

            // Look for end of header
            end_ptr = strchr( start_ptr, '\r' );

            dst = dpws_malloc( dpws, end_ptr - start_ptr + 1 );
            if ( NULL != dst )
            {
                strncpy( dst, start_ptr, end_ptr - start_ptr );
                dst[ end_ptr - start_ptr ] = '\0';
            }

            return dst;
        }
    }
    return NULL;
}

static void* wsd_serve_directed_probe_request( void* input )
{
    //TX_THREAD* thr = tx_thread_identify();
    wsd_directed_probe_t *probe_info;
    http_request_t request = (http_request_t )input;

    http_resource_get_data_ptr( request, ( void** )&probe_info );

    dpws_process_soap_request( probe_info->dpws, (void*)request, probe_info->callbacks, probe_info->context );
    http_resource_set_data_ptr( request, NULL );
    http_resource_send_complete( request );

    dpws_end(probe_info->dpws);	            // frees transient message memory.
    MEM_FREE_AND_NULL(probe_info->dpws);	// disposable runtime structure: not efficient
    //DBG_ERR( "%s exit and terminate %s\n", __FUNCTION__, thr->tx_thread_name );
    MEM_FREE_AND_NULL( probe_info->context );
    MEM_FREE_AND_NULL( probe_info->callbacks );
    MEM_FREE_AND_NULL( probe_info );
    //tx_thread_terminate( thr );
    return 0;
}

void wsd_directed_probe_callback( http_request_t request, http_resource_event_t event, char *buffer, uint32_t buffer_size_in_bytes )
{
    wsd_directed_probe_t *probe_info;
    media_type_t *type;
    char *header;
    char *tmp;
    probe_data_node_t* data_node;
    wsd_dpws_data_t *dpws_data = wsd_get_dpws_data( );

    switch ( event )
    {
        case HTTP_RESOURCE_HEADER_DATA:
            probe_info = MEM_MALLOC( sizeof( wsd_directed_probe_t ) );
            if ( probe_info == NULL )
            {
                ASSERT( 0 );
                return;
            }
            probe_info->context = MEM_MALLOC( sizeof( http_context_t ) );
            if ( probe_info->context == NULL )
            {
                ASSERT( 0 );
                return;
            }
            dpws_init_http_context( probe_info->context );

            probe_info->callbacks = MEM_MALLOC( sizeof( transport_fns_t ) );
            if ( probe_info->callbacks == NULL )
            {
                ASSERT( 0 );
                return;
            }
            probe_info->callbacks->frecv = wsd_directed_probe_frecv;
            probe_info->callbacks->fresponse = wsd_directed_probe_fresponse;
            probe_info->callbacks->fsend = wsd_directed_probe_fsend;

            probe_info->dpws = MEM_MALLOC( sizeof( struct dpws ) );
            if ( probe_info->dpws == NULL )
            {
                ASSERT( 0 );
                return;
            }
            dpws_client_init( probe_info->dpws, NULL );
            http_resource_set_data_ptr( request, ( void* )probe_info );

            probe_info->context->action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"; 
            probe_info->context->host = extract_header_value( probe_info->dpws, buffer, "Host" );
            header = extract_header_value( probe_info->dpws, buffer, "Content-Type" );
            if (header != NULL)
            {
                type = dpws_malloc( probe_info->dpws, sizeof( media_type_t ) );
                type->type = header;
                tmp = strchr( header, '/' );
                if ( tmp != NULL )
                {
                    *tmp = '\0';
                    type->subtype = ++tmp;
                    type->params = NULL;
                    probe_info->context->mtype = type;
                }
            }
            probe_info->context->needs_length = DC_TRUE;
            probe_info->context->path = (char*)dpws_get_ptr_att_item( dpws_data->device, DPWS_STR_DEVICE_ID, 0 );

            ATInitList( &probe_info->recv_data_list );
            http_resource_buffer_free( request, buffer );

            break;
        case HTTP_RESOURCE_BODY_DATA:
            http_resource_get_data_ptr( request, ( void** )&probe_info );
            data_node = MEM_MALLOC( sizeof( probe_data_node_t ) );
            ASSERT( data_node != NULL );
            data_node->buf = (uint8_t*)buffer;
            data_node->buflen = buffer_size_in_bytes;
            data_node->bufoffset = 0;
            ATInsertTailList(&probe_info->recv_data_list, &data_node->node);
            break;
        case HTTP_RESOURCE_BODY_DATA_END:
            {
                pthread_t *thread;
                uint8_t *thread_stack;
                char* thread_name;
                UINT tx_res;

                http_resource_get_data_ptr( request, ( void** )&probe_info );
                
                if ( buffer != NULL )
                {
                    data_node = MEM_MALLOC( sizeof( probe_data_node_t ) );
                    ASSERT( data_node != NULL );
                    data_node->buf = (uint8_t*)buffer;
                    data_node->buflen = buffer_size_in_bytes;
                    data_node->bufoffset = 0;
                    ATInsertTailList(&probe_info->recv_data_list, &data_node->node);
                }

                if ( wsd_get_thread( &thread, &thread_stack, &thread_name ) != NET_WSD_SUCCESS )
                {
                    DBG_ERR( "Failed to get a thread in wsd - throw away current request\n" );
                    return;
                }

                /* spawn a new thread */
                tx_res = posix_create_thread(thread, wsd_serve_directed_probe_request, (void*)request, thread_name,
                                       thread_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
                ASSERT( 0 == tx_res );     
                break;
            }
        case HTTP_RESOURCE_CLOSED:
            {
                wsd_directed_probe_t *probe_info;
                http_resource_get_data_ptr( request, ( void** )&probe_info );

                if ( probe_info != NULL )
                {
                    DBG_MSG( "Cleaning up probe_info data\n" );
                    http_resource_set_data_ptr( request, NULL );
                    dpws_end(probe_info->dpws);	            // frees transient message memory.
                    MEM_FREE_AND_NULL(probe_info->dpws);	// disposable runtime structure: not efficient
                    MEM_FREE_AND_NULL( probe_info->context );
                    MEM_FREE_AND_NULL( probe_info->callbacks );
                    MEM_FREE_AND_NULL( probe_info );
                }
            }
            break;
    }
}


