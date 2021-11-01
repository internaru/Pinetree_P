
/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file http_methods_support.c
 *
 * \brief Handles HTTP GET, POST, PUT, DELETE, and HEAD requests
 *        along with various other vendor specific functions
 *
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include "memAPI.h"

#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "dprintf.h"
#include "lassert.h"
#ifndef __linux__
#include "cpu_api.h"
#endif
#ifdef HAVE_NETWORK
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#else
#include "trsocket.h"
#endif
#include <pthread.h>
#include <posix_ostools.h>
#endif // NETWORK
#include "http_parse.h"
#include "http_methods_support.h"
#include "http_vars.h"
#include "expat.h"
#include "debug.h"
#include "http_midas_extended_error_codes.h"
#include "agMessage.h"
#include "agRouter.h"
#include "agConnectMgr.h"
#include "utils.h"

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
#define MAX_XML_TAG_LENGTH          256

// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_METHODS
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE
#define DBG_PRFX    "HTTP METHODS: "

#ifdef HAVE_GCPP
#include "gcpp_oem_config.h"
//gcpp_error_t gcpp_client_notification_func (gcpp_client_event_t event, void *event_context);
#endif

//--------------------------------------
// Global Definitions
//--------------------------------------

//--------------------------------------
// Local Function Declarations
//--------------------------------------
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int MidasDebugCallback( int argc, char *argv[] );
#endif

bool processReceivedXmlData( HttpPageContext* context, char* xmlData, XML_HANDLER handler, uint32_t index, bool duplicates );
void xmlStartElementHandle(void *data, const char *tagName, const char **attr);
void xmlEndElementHandle(void *data, const char *end_tag);
void xmlContentHandle( void *data, const char *xml_data, int len );
bool parseXML( char* sData, uint32_t  nDataLen, HttpPageContext* context, XML_HANDLER );
//--------------------------------------
// Local Function Definitions
//--------------------------------------

/**
 * \brief Initialize protocol specific code
 **/
void http_methods_initialize( void )
{

    #ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register( "midas",
                          NULL,
                          NULL,
                          NULL,
                          MidasDebugCallback );
    ASSERT( CMD_OK == i_res );
    #endif // HAVE_CMD

    return;
}


/**
 * \brief Validates if provided url is password protected
 *
 * \return true  ==> Password Protected
 *         false ==> No password required
 *
 **/
bool http_methods_is_password_protected( char *request, char *url )
{
    bool is_password_protected;

    ASSERT( NULL != request );
    ASSERT( NULL != url );

    is_password_protected = false;

    /* Protect example */
    if ( ( 0 == strncmp( GET_REQUEST, request, strlen( GET_REQUEST ) ) ) &&
         ( strstri( url, "/view/private" ) != NULL ) )
    {
        is_password_protected = true;
    }

    if ( false == is_password_protected )
    {
#if 0 /* TODO, work with SW team for this based on customer needs */
        /* Protect against any PUT, POST, and DELETE calls on all pages */
        if ( ( 0 == strncmp( PUT_REQUEST, request, strlen( PUT_REQUEST ) ) )   ||
             ( 0 == strncmp( POST_REQUEST, request, strlen( POST_REQUEST ) ) ) ||
             ( 0 == strncmp( DELETE_REQUEST, request, strlen( DELETE_REQUEST ) ) ) )
        {
            is_password_protected = true;
        }
#endif
    }

    return is_password_protected;
}


error_type_t http_methods_get_current_password( char * url, char * password, uint32_t password_len )
{
    error_type_t e_res;
    char current_password[ HTTP_VAR_PASSWORD_STRING_MAX_LEN ];
    uint32_t smallest_string_len;

    ASSERT( NULL != url );
    ASSERT( NULL != password );

    /* use the smaller of the two */
    smallest_string_len = password_len;
    if ( password_len > HTTP_VAR_PASSWORD_STRING_MAX_LEN )
    {
        smallest_string_len = HTTP_VAR_PASSWORD_STRING_MAX_LEN;
    }
    e_res = http_get_var_password( current_password, smallest_string_len );

    if ( OK == e_res )
    {
        strncpy( password, current_password, smallest_string_len );
    }
    else
    {
        password[ 0 ] = '\0';
    }

    return e_res;
}

void add_http_ok_response( HttpPageContext *context, char *type )
{
    addDataToHttpContext( HTTP_OK_1, strlen( HTTP_OK_1 ), false, context, false );
    addDataToHttpContext( type, strlen( type ), false, context, false );
    addDataToHttpContext( HTTP_OK_2, strlen( HTTP_OK_2 ), false, context, false );
}

void add_http_special_uri_response( HttpPageContext *context, error_type_t e_res )
{
    if ( OK == e_res )
    {
        /* set status code */
        addDataToHttpContext( HTTP_STATUS_200, strlen( HTTP_STATUS_200 ), false, context, false );
        addDataToHttpContext( HTTP_REQUIRED_FIELDS, strlen( HTTP_REQUIRED_FIELDS ), false, context, false );

        /* set content type */
        addDataToHttpContext( HTTP_CONTENT_TYPE, strlen( HTTP_CONTENT_TYPE ), false, context, false );
        addDataToHttpContext( (char * )HttpdContentTypes[ context->contextTypeIndex ],
                              strlen( HttpdContentTypes[ context->contextTypeIndex ] ),
                              false,
                              context,
                              false );
        addDataToHttpContext( ENDOFHEADER, strlen( ENDOFHEADER ), false, context, false );

    }
    else
    {
        /* set status code */
        addDataToHttpContext( HTTP_STATUS_409, strlen( HTTP_STATUS_409 ), false, context, false );
        addDataToHttpContext( HTTP_REQUIRED_FIELDS, strlen( HTTP_REQUIRED_FIELDS ), false, context, false );

        /* set content type */
        addDataToHttpContext( HTTP_CONTENT_TYPE, strlen( HTTP_CONTENT_TYPE ), false, context, false );
        addDataToHttpContext( (char * )HttpdContentTypes[ context->contextTypeIndex ],
                              strlen( HttpdContentTypes[ context->contextTypeIndex ] ),
                              false,
                              context,
                              false );
        addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );

        /* set extended status code */
        addDataToHttpContext( HTTP_MIDAS_XML_EXTENDED_CODE_BUSY, strlen( HTTP_MIDAS_XML_EXTENDED_CODE_BUSY ), false, context, false );
        addDataToHttpContext( ENDOFHEADER, strlen( ENDOFHEADER ), false, context, false );
    }

    /* prepare to respond with the basic MIDAS status page */
    setPageValues( "/config/status", context, false );
}

/**
 *
 * \brief This is a callback definition for the exception URI handlers
 * Handlers of this format will handle requests that come in via these special
 * callbacks
 *
 * \param[in] special_uri The special URI to be handled
 *
 * \return OK upon success, FAIL if the special request failed to be executed
 *
 */
typedef error_type_t (*http_special_uri_handler_t)( HttpPageContext *context );


/**
 *
 * \brief This structure defines the members needed to handle a special
 * URI request
 *
 */
typedef struct http_internal_special_uri_info_s
{
    char * uri;
    http_special_uri_handler_t handler;

} http_internal_special_uri_info_t;


static error_type_t http_special_uri_handler_clean_print_heads( HttpPageContext *context )
{
    error_type_t e_res;

    e_res = FAIL;
    add_http_special_uri_response( context, e_res );

    return e_res;
}


static error_type_t http_special_uri_handler_print_test_page( HttpPageContext *context )
{
    error_type_t e_res;
    MESSAGE message;

    memset( &message, 0, sizeof( message ) );
    message.msgType = MSG_INTERNAL_PAGE;
    message.param1 = e_DEMOPAGE;
#ifdef __linux__
    e_res = SYMsgSend( INTERNALMGRID, &message );
#else
    e_res = SYMsgSendNoWait( INTERNALMGRID, &message );
#endif    

    add_http_special_uri_response( context, e_res );

    return e_res;
}


static error_type_t http_special_uri_handler_print_configuration_page( HttpPageContext *context )
{
    error_type_t e_res;
    MESSAGE message;

    memset( &message, 0, sizeof( message ) );
    message.msgType = MSG_INTERNAL_PAGE;
    message.param1 = e_CONFIGPAGE;
#ifdef __linux__
    e_res = SYMsgSend( INTERNALMGRID, &message );
#else
    e_res = SYMsgSendNoWait( INTERNALMGRID, &message );
#endif

    add_http_special_uri_response( context, e_res );

    return e_res;
}


#ifdef HAVE_GCPP

gcpp_error_t gcpp_client_notification_func (gcpp_client_event_t event, void *event_context)
{
    DBG_PRINTF(LOG_DEBUG,"Executing gcpp_client_notification_func_t...\n");
    switch (event)
    {
        case GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE:
            DBG_PRINTF(LOG_NOTICE,"Indicates completion of the GCP Anonymous Registration request\n");
            DBG_PRINTF(LOG_DEBUG,"Registration token is %s\n", gcpp_oem_get_registration_token());
            DBG_PRINTF(LOG_DEBUG,"Invite page URL is %s\n", gcpp_oem_get_invite_page_url());
            DBG_PRINTF(LOG_DEBUG,"Complete Invite URL is %s\n", gcpp_oem_get_complete_invite_url());
            DBG_PRINTF(LOG_DEBUG,"Token duration is %ld\n", gcpp_oem_get_token_duration());
            gcpp_oem_set_gcp_reg_status(GCP_CLAIM_PENDING);
            break;

        case GCPP_EVENT_REGISTRATION_SUCCESS:
            DBG_PRINTF(LOG_DEBUG,"User Email is %s\n", gcpp_oem_get_user_email());
            DBG_PRINTF(LOG_DEBUG,"Printer ID is %s\n", gcpp_oem_get_printer_id());
            gcpp_oem_set_gcp_reg_status(GCP_REGISTERED);
            DBG_PRINTF(LOG_NOTICE,"GCPP registration successful\n");
            DBG_PRINTF(LOG_DEBUG,"net_gcpp_get_printer_id: %s\n", net_gcpp_get_printer_id(false));
            break;

        case GCPP_EVENT_REGISTRATION_FAILED:
            gcpp_oem_set_gcp_reg_status(GCP_NOT_REGISTERED);
            DBG_PRINTF(LOG_NOTICE,"GCPP registration failed\n");
            break;

        case GCPP_EVENT_PRINTER_REMOVED:
            gcpp_oem_set_gcp_reg_status(GCP_NOT_REGISTERED);
            DBG_PRINTF(LOG_NOTICE,"Notification that GCP Printer was removed (or) cancellation is complete\n");
            break;

        default:
           DBG_PRINTF(LOG_NOTICE,"%d - this event not handled in client notification func\n", event);
           break;
    }

    return NET_GCPP_SUCCESS;
}

static error_type_t http_special_uri_handler_gcpp_cancel_registration ( HttpPageContext *context )
{
    error_type_t retval = FAIL;
    bool ret = false;

    ret = gcpp_oem_cancel_registration();
    if ( ret == true )
    {
        add_http_ok_response( context, (char*)HttpdContentTypes[ HTTP_TYPE_HTML ] );
    }
    else
    {
        addDataToHttpContext( HTTP_INTRNL_ERROR, strlen( HTTP_INTRNL_ERROR ), false, context, false );
    }

    addDataToHttpContext( NULL, 0, false, context, true );

    context->state = e_http_state_body_complete;

    return retval;
}

static error_type_t http_special_uri_handler_gcpp_unregister( HttpPageContext *context )
{
    error_type_t retval = FAIL;

    gcpp_oem_unregister_printer();

    add_http_ok_response( context, (char*)HttpdContentTypes[ HTTP_TYPE_HTML ] );
    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return retval;
}

/* Starts gcp registration */
static error_type_t http_special_uri_handler_start_gcpp_register( HttpPageContext *context )
{
    bool ret = false;
    error_type_t retval = FAIL;

//    gcpp_oem_register_client_notification_func(gcpp_client_notification_func);

    ret = gcpp_oem_register();
    if (ret == true)
    {
        retval = OK;
        add_http_ok_response( context, (char*)HttpdContentTypes[ HTTP_TYPE_HTML ] );
    }
    else
    {
        addDataToHttpContext( HTTP_INTRNL_ERROR, strlen( HTTP_INTRNL_ERROR ), false, context, false );
    }

    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return retval;
}

static error_type_t http_special_uri_handler_check_gcpp_register_status( HttpPageContext *context )
{
    GCP_REG_STATUS reg_status = INVALID_STATE;
    error_type_t retval = OK;

    add_http_ok_response( context, (char*)HttpdContentTypes[ HTTP_TYPE_HTML ] );

    reg_status = gcpp_oem_get_gcp_reg_status();
    switch ( reg_status )
    {

        case GCP_REG_IN_PROGRESS:
        {
            /* In Progress */
            addDataToHttpContext( "In Progress", strlen( "In Progress" ), false, context, true );
            break;
        }
        case GCP_CLAIM_PENDING:
        {
            /* Waiting for user to claim printer */
            addDataToHttpContext( "Claim Pending", strlen( "Claim Pending" ), false, context, true );
            break;
        }
        case GCP_REGISTERED:
        {
            /* Registration Complete */
            addDataToHttpContext( "Registration Complete", strlen( "Registration Complete" ), false, context, true );

            break;
        }
        case GCP_NOT_REGISTERED:
        {
            /* Not registered */
            addDataToHttpContext( "Not Registered", strlen( "Not Registered" ), false, context, true );

            break;
        }
        default:
        {
            retval = FAIL;
            break;
        }
    }

    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return retval;
}

static error_type_t http_special_uri_handler_get_gcpp_invite_url( HttpPageContext *context )
{
    char * claim_url = NULL;

    claim_url = gcpp_oem_get_invite_url();
    if ( claim_url)
    {
        add_http_ok_response( context, (char*) HttpdContentTypes[ HTTP_TYPE_HTML ] );
        addDataToHttpContext( claim_url , strlen( claim_url ), false, context, true );
    }
    else
    {
        addDataToHttpContext( HTTP_INTRNL_ERROR, strlen( HTTP_INTRNL_ERROR ), false, context, false );

    }

    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return OK;
}

static error_type_t http_special_uri_handler_get_gcpp_claim_token( HttpPageContext *context )
{
    char * claim_token = NULL;

    claim_token = gcpp_oem_get_registration_token();
    if ( claim_token)
    {
        add_http_ok_response( context, (char*) HttpdContentTypes[ HTTP_TYPE_HTML ] );
        addDataToHttpContext( claim_token, strlen( claim_token ), false, context, true );
    }
    else
    {
        addDataToHttpContext( HTTP_INTRNL_ERROR, strlen( HTTP_INTRNL_ERROR ), false, context, false );

    }

    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return OK;
}

static error_type_t http_special_uri_handler_print_gcpp_reg_page( HttpPageContext *context )
{
    /* Call API to print reg page here */

    add_http_ok_response( context, (char*) HttpdContentTypes[ HTTP_TYPE_HTML ] );
    addDataToHttpContext( NULL, 0, false, context, true );
    context->state = e_http_state_body_complete;

    return OK;
}
#endif


static http_internal_special_uri_info_t special_uri_table[] =
{
    {
        .uri = "/service/operation/1",
        .handler = http_special_uri_handler_clean_print_heads
    },

    {
        .uri = "/service/operation/2",
        .handler = http_special_uri_handler_print_test_page
    },

    {
        .uri = "/service/operation/3",
        .handler = http_special_uri_handler_print_configuration_page
    },

#ifdef HAVE_GCPP
    {
        .uri = "/gcpp/start_registration",
        .handler = http_special_uri_handler_start_gcpp_register
    },
    {
        .uri = "/gcpp/check_reg_status",
       .handler = http_special_uri_handler_check_gcpp_register_status
    },
    {
        .uri = "/gcpp/print_reg_page",
       .handler = http_special_uri_handler_print_gcpp_reg_page
    },
    {
       .uri = "/gcpp/get_invite_url",
       .handler = http_special_uri_handler_get_gcpp_invite_url
    },
    {
       .uri = "/gcpp/get_claim_token",
       .handler = http_special_uri_handler_get_gcpp_claim_token
    },
    {
        .uri = "/gcpp/cancel_registration",
        .handler = http_special_uri_handler_gcpp_cancel_registration
    },
    {
        .uri = "/gcpp/unregister",
        .handler = http_special_uri_handler_gcpp_unregister
    },
#endif
};

#define NUM_SPECIAL_URI_INFO_ENTRIES sizeof( special_uri_table ) / sizeof( http_internal_special_uri_info_t )

static http_internal_special_uri_info_t post_uri_table[] =
{
   {   .uri = "/view/network/gcp",                            .handler = handleEwsPost                },
};
#define NUM_POST_ENTRIES sizeof( post_uri_table ) / sizeof( http_internal_special_uri_info_t )

static const char pointer_to_null[1] = "";


/**
 * \brief Parse and HTTP GET request and either supply requested
 * data or indicate an error
 *
 * \param context - context with data
 *
 * \param is_included_file - Is this file embedded in another
 *        HTTP request?
 *
 * \return bool - success / failure
 *
 * Given a context that has recieved an HTTP GET request,
 * examine the filename requested and either send the file if it
 * is in our system - or return a 404 file not found error
 *
 **/
bool parseHttpGet( HttpPageContext *context, bool is_included_file )
{
    bool is_special_case;
    bool retVal;
    error_type_t e_res;
    uint32_t special_uri_handler_index;
    int i_res;
    http_internal_special_uri_info_t * matching_special_uri_entry;

    is_special_case = false;
    retVal = false;
    matching_special_uri_entry = NULL;

    // The majority of the files are statically saved in ROM with SSI tags,
    // There are some files that are dynamically created - these files
    // are checked for with the if/else statements below with the final
    // else case checking for ROM based pages

    for ( special_uri_handler_index = 0;
          special_uri_handler_index < NUM_SPECIAL_URI_INFO_ENTRIES;
          special_uri_handler_index++ )
    {
        /* compare the two strings, 0 return result means the two strings
         * match */
        i_res = strncmp( context->requested_uri,
                         special_uri_table[ special_uri_handler_index ].uri,
                         strlen( special_uri_table[ special_uri_handler_index ].uri ) );
        /* match ? */
        if ( 0 == i_res )
        {
            matching_special_uri_entry = &( special_uri_table[ special_uri_handler_index ] );
            is_special_case = true;
            break;
        }
    }

    if ( true == is_special_case )
    {
        /* make the handler call to handle the special URI */
        e_res = matching_special_uri_entry->handler( context );

        if ( OK == e_res )
        {
            /* set status code */
            addDataToHttpContext( HTTP_STATUS_200, strlen( HTTP_STATUS_200 ), false, context, false );
            addDataToHttpContext( HTTP_REQUIRED_FIELDS, strlen( HTTP_REQUIRED_FIELDS ), false, context, false );

            /* set content type */
            addDataToHttpContext( HTTP_CONTENT_TYPE, strlen( HTTP_CONTENT_TYPE ), false, context, false );
            addDataToHttpContext( (char * )HttpdContentTypes[ context->contextTypeIndex ],
                                  strlen( HttpdContentTypes[ context->contextTypeIndex ] ),
                                  false,
                                  context,
                                  false );
            addDataToHttpContext( ENDOFHEADER, strlen( ENDOFHEADER ), false, context, false );

        }
        else
        {
            /* set status code */
            addDataToHttpContext( HTTP_STATUS_409, strlen( HTTP_STATUS_409 ), false, context, false );
            addDataToHttpContext( HTTP_REQUIRED_FIELDS, strlen( HTTP_REQUIRED_FIELDS ), false, context, false );

            /* set content type */
            addDataToHttpContext( HTTP_CONTENT_TYPE, strlen( HTTP_CONTENT_TYPE ), false, context, false );
            addDataToHttpContext( (char * )HttpdContentTypes[ context->contextTypeIndex ],
                                  strlen( HttpdContentTypes[ context->contextTypeIndex ] ),
                                  false,
                                  context,
                                  false );
            addDataToHttpContext( "\r\n", strlen( "\r\n" ), false, context, false );

            /* set extended status code */
            addDataToHttpContext( HTTP_MIDAS_XML_EXTENDED_CODE_BUSY, strlen( HTTP_MIDAS_XML_EXTENDED_CODE_BUSY ), false, context, false );
            addDataToHttpContext( ENDOFHEADER, strlen( ENDOFHEADER ), false, context, false );
        }

        /* prepare to respond with the basic MIDAS status page */
        retVal = setPageValues( "/config/status", context, false );

    }
    else
    {
        retVal = getContextInfoForFilename( context->requested_uri, context, is_included_file );
    }

    return retVal;
}


/**
 * \brief parse and HTTP POST request
 *
 * \param context - context to initialize
 *
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP POST request, parse the data to
 * see if it contains correct formatting and a file that we know
 * about.  If successful, data is saved in the system, otherwise
 * the appropriate HTTP error is added to the context
 *
 **/
bool parseHttpPost( HttpPageContext *context )
{
    bool retVal;
    char *postData;
    uint32_t special_uri_handler_index;
    error_type_t e_res;
    int i_res;
// write no read warning:      bool is_special_case;
    http_internal_special_uri_info_t * matching_special_uri_entry;
    matching_special_uri_entry = NULL;

    DBG_MSG( "Post Request\n" );

    retVal = false;

    for ( special_uri_handler_index = 0;
          special_uri_handler_index < NUM_POST_ENTRIES;
          special_uri_handler_index++ )
    {
        /* compare the two strings, 0 return result means the two strings
         * match */
        i_res = strncmp( context->requested_uri,
                         post_uri_table[ special_uri_handler_index ].uri,
                         strlen( post_uri_table[ special_uri_handler_index ].uri ) );
        /* match ? */
        if ( 0 == i_res )
        {
            matching_special_uri_entry = &( post_uri_table[ special_uri_handler_index ] );
// write no read warning:            is_special_case = true;
            break;
        }
    }

    if ( matching_special_uri_entry != NULL )
    {
        e_res = matching_special_uri_entry->handler( context );

        if ( OK == e_res )
        {
            /* set status code */
              retVal = true;
        }
    }
    else
    {
      if ( NULL != context )
      {
          // Make sure they provided content length
          if ( context->content_length == context->request_body_length )
          {
              postData = context->request_body;
              retVal = true;
          }
      }

      // Check to see if the POST was valid - complete header with context length defined
      if ( true == retVal )
      {
          postData = strstri( context->request_header, HTTP_X_HTTP_METHOD_OVERRIDE );
          if ( NULL != postData  )
          {
              retVal = parseHttpPut( context );
          }
          else
          {
              retVal = false;
          }
      }
    }

    return retVal;
}


/**
 * \brief Parse an HTTP PUT request
 *
 * \param context - current context, contains HTTP request
 *
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP PUT request, parse the data to
 * see if it contains correct formatting and a file that we know
 * about.  If successful, data is saved in the system, otherwise
 * the appropriate HTTP error is added to the context
 *
 **/
bool parseHttpPut( HttpPageContext *context )
{
    bool retVal = false;
    char* putData;

    // Make sure they provided content length, so we know how much data to expect
    if ( context->content_length == context->request_body_length )
    {
        putData = context->request_body;
        retVal = true;
    }

    // Check to see if the PUT was valid
    if ( retVal )
    {
        DBG_MSG( "PUT Request with data: '%s'\n", putData );

        // See if the requested file is known to our system
        if( getContextInfoForFilename( context->requested_uri, context, false ) == true )
        {
            char* putData = context->request_body;

            // Parse the put data
            if ( processReceivedXmlData( context, putData, XML_OIDS, 0, false ) == true )
            {
                addDataToHttpContext( HTTP_OK_SIMPLE, strlen( HTTP_OK_SIMPLE ), false, context, false );
            }
        }
    }

    return retVal;
}


/**
 * \brief Parse and handle and HTTP DELETE request
 *
 * \param context - current context, contains request
 *
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP DELETE request, parse the data
 * to see if it contains correct formatting and a file that we
 * know about.  If successful, the data is deleted from the
 * system, otherwise the appropriate HTTP error is added to the
 * context
 *
 **/
bool parseHttpDelete( HttpPageContext *context )
{
    bool retVal;

    retVal = false;

#if 0
    // NOT CURRENTLY USED
    DBG_MSG( "Delete Request\n" );

    if ( retVal == true )
    {
        addDataToHttpContext( HTTP_OK_SIMPLE, strlen( HTTP_OK_SIMPLE ), false, context, false );
        context->isComplete = true;
    }
#endif

    return retVal;
}

/**
 * \brief Parse and handle and HTTP HEAD request
 *
 * \param context - current context, contains request
 *
 * \return bool - indicates success/failure
 *
 * Given a context with an HTTP HEAD request, check if the file
 * exists in the system and if so return the HTTP header for
 * this file - no data.
 *
 **/
bool parseHttpHead( HttpPageContext *context )
{
    char* header;

    // The majority of the files are statically saved in ROM with SSI tags,
    // There are some files that are dynamically created - these files
    // are checked for with the if statement below with the else case checking
    // for ROM based pages

    if ( setPageValues( context->requested_uri, context, false ) == false )
    {
        // Return a file not found header
        addDataToHttpContext( HTTP_NOT_FOUND, strlen( HTTP_NOT_FOUND ), false, context, false );
    }
    else
    {
        http_response_type_t contextTypeIndex = getContextTypeIndex( context->requested_uri );
        if ( context->parseSSI == false )
        {
            header = ( char* ) MEM_MALLOC_ALIGN( strlen( HTTP_OK_FILE_LENGTH_1 ) +
                                           strlen( HTTP_OK_FILE_LENGTH_2 ) +
                                           strlen( HTTP_OK_FILE_LENGTH_3 ) +
                                           strlen( HttpdContentTypes[ contextTypeIndex ] ) +
                                           LENGTH_OF_uint32_t +
                                           LENGTH_OF_NULL,
                                           cpu_get_dcache_line_size() );
            // Put the header on the page context to be sent later.
            minSprintf( header, "%s%d%s%s%s",
                        HTTP_OK_FILE_LENGTH_1,
                        context->pageSize,
                        HTTP_OK_FILE_LENGTH_2,
                        HttpdContentTypes[ contextTypeIndex ],
                        HTTP_OK_FILE_LENGTH_3 );
            addDataToHttpContext( header, strlen( header ), true, context, false );
        }
        else
        {
            header = ( char* ) MEM_MALLOC_ALIGN( ( strlen( HTTP_OK_1 ) +
                                                   strlen( HTTP_OK_2 ) +
                                                   strlen( HttpdContentTypes[ contextTypeIndex ] ) +
                                                   LENGTH_OF_char +
                                                   LENGTH_OF_NULL ),
                                                 cpu_get_dcache_line_size() );
            // Put the header on the page context to be sent later.
            minSprintf( header, "%s%s%s\r\n", HTTP_OK_1, HttpdContentTypes[ contextTypeIndex ], HTTP_OK_2 );
            addDataToHttpContext( header, strlen( header ), true, context, false );
        }
    }
    context->state = e_http_state_body_complete;
    return true;
}

#ifdef HAVE_CMD
#define HTTP_DBG ( DBG_TERMINAL|DBG_SOFT )

/**
 * \brief allows access to serial terminal commands
 *
 * \param argc - number of arguments
 *
 * \param argv - arguments
 *
 * \return - always 0
 *
 * Allows to send commands over serial terminal to the FW
 *
 **/
static int MidasDebugCallback( int argc, char *argv[ ] )
{
    #define MIDAS_SERIAL_DEBUG_HELP "midas pw set <new password>"

    if ( argc != 4 )
    {
        DBG_CMD( "%s\n", MIDAS_SERIAL_DEBUG_HELP );
    }
    else
    {
        if ( ( strncmp( argv[1], "pw", 3 ) == 0 ) &&
             ( strncmp( argv[2], "set", 4 ) == 0 ) )
        {
            error_type_t e_res;
            uint32_t smallest_string_len;

            smallest_string_len = strlen( argv[3] );
            e_res = FAIL;
            if ( smallest_string_len <= HTTP_VAR_PASSWORD_STRING_MAX_LEN )
            {
                e_res = http_set_var_password( argv[3], smallest_string_len );
            }

            if ( OK == e_res )
            {
                DBG_CMD( "HTTP password set!\n" );
            }
            else
            {
                DBG_CMD( "HTTP failed to set password!\n" );
            }
        }
    }

    return 0;
}
#endif //HAVE_CMD


/**
 * \brief Starts parsing of supplied xml data
 *
 * \param sData - xml data to parse
 *
 * \param nDataLen - length of xml data
 *
 * \param context - context to update with new oids / parse
 * errors
 *
 * \param xmlHandler - which xml handler will deal with the
 * callbacks
 *
 * \return bool - success/failure of parse
 *
 * This functions creates the simple xml parser and decides
 * which handler will be used for the callbacks.  Memory is
 * allocated for xml tag string - but also freed as part of this
 * function.
 *
 **/
bool parseXML( char* sData, uint32_t nDataLen, HttpPageContext* context, XML_HANDLER xmlHandler )
{
    int32_t isFinal;
    XML_Parser xmlParser;

    context->xmlTag = ( char* )MEM_MALLOC_ALIGN( MAX_XML_TAG_LENGTH,
                                                 cpu_get_dcache_line_size() );
    if ( context->xmlTag == NULL )
    {
        return false;
    }
    *context->xmlTag = (char)0;

    xmlParser = XML_ParserCreate( NULL );
    ASSERT(xmlParser != NULL);
    if (xmlParser == NULL)
    {
        MEM_FREE_AND_NULL(context->xmlTag);
        return false;
    }

    // Tell parser to send back context with each handler call
    XML_SetUserData( xmlParser, ( void * )context );

    // Set handler for start of xml tag and end of tag
    XML_SetElementHandler( xmlParser, xmlStartElementHandle, xmlEndElementHandle );

    // Set handler for context inside tags
    XML_SetCharacterDataHandler( xmlParser, xmlContentHandle );

    /* XML_Parse requires the last call to XML_Parse to have "isFinal" set
     * to 'true' */
    isFinal = true;

    /* trim the white space */
    while ( ( ( *sData == ' ' ) ||
              ( *sData == '\r' ) ||
              ( *sData == '\n' ) ||
              ( *sData == '\t' ) ) &&
            ( *sData != (char)0 ) )
    {
        sData++;
        ASSERT( nDataLen != 0 );
        nDataLen--;
    }
    if ( XML_Parse( xmlParser, sData, nDataLen, isFinal ) == false)
    {
        MEM_FREE_AND_NULL( context->xmlTag );
        return false;
    }

    XML_ParserFree( xmlParser );

    MEM_FREE_AND_NULL( context->xmlTag );
    return true;
}


/**
 * \param context - current context
 *
 * \param xmlData - Data that was POSTed or PUT
 *
 * \param handler - What type of XML data is it?
 *
 * \param index - Used only for subscriptions
 *
 * \param duplicates - Used only for subscriptions
 *
 * Parse XML data and then save off the results as needed.  This
 * function can be used to save oids, subscriptions and region
 * codes.  If the parse fails, HTTP response added to the
 * context
 *
 **/
bool processReceivedXmlData( HttpPageContext* context, char* xmlData, XML_HANDLER handler, uint32_t index, bool duplicates )
{
    bool result;

    result = true;

    if ( parseXML( xmlData, strlen( xmlData ), context, handler ) == true )
    {
        if ( context->state == e_http_state_body_complete )
        {
            DBG_ERR( "XML Parse Succeeded - but oid LUT failed\n" );
            result = false;
        }
        else
        {
            DBG_MSG( "XML Parse Succeeded\n" );
            switch ( handler )
            {
                case XML_OIDS:
                    result = saveOidValues( context );
                    break;
                default:
                    result = false;
                    break;
            }
            if ( result == false )
            {
                DBG_ERR( "Failed save data\n" );
                addDataToHttpContext( HTTP_BAD_REQUEST, strlen( HTTP_BAD_REQUEST ), false, context, false );
            }
            context->state = e_http_state_body_complete;
        }
    }
    else
    {
        result = false;
        DBG_ERR( "Failed XML Parse\n" );
        addDataToHttpContext( HTTP_NOT_FOUND, strlen( HTTP_NOT_FOUND ), false, context, false );
        context->state = e_http_state_body_complete;
    }

    return result;
}

/**
 * \brief Parse xml tag name fields
 *
 * \param data - pointer to HttpPageContext for current
 * connection
 *
 * \param tag_name - name of tag that we are currently parsing
 *
 * \param attr - list attributes for current tag - even indicies
 * are attribute names and odd are attribute values. List is
 * terminated by 0 entry
 *
 * This function parses the opening of an xml tag.  The tag name
 * is added to the 'path' through the xml file.  The checks are
 * performed to see if the current tag forces any new oids to be
 * created.
 *
 **/
void xmlStartElementHandle(void *data, const char *tagName, const char **attr)
{
    HttpPageContext *context = ( HttpPageContext *)data;
    if ( context == NULL )
    {
        return;
    }

    if( (uint32_t)(*context->xmlTag) == 0 )
    {
        strcpy( context->xmlTag, tagName );
    }
    else
    {
        minSprintf( context->xmlTag + strlen( context->xmlTag ), "|%s", tagName );
    }
    checkXmlTagtoCreateOids( context->xmlTag, context );
    checkXmlTagsWithoutValues( context->xmlTag, context );

}

/**
 * \brief Given value inside xml tags - determine what should be
 * done with this data
 *
 * \param data - pointer to HttpPageContext for current
 * connection
 *
 * \param xml_data - value from xml tag - NOT null terminated
 *
 * \param len - length of xml_data string
 *
 * This function determines what to do with the data we have
 * received from the xml parser.  Normally this data will be
 * used to prepare an oid to be saved.  The xml_data parameter
 * is NOT null terminated.
 *
 **/
void xmlContentHandle( void *data, const char *xml_data, int len )
{
    HttpPageContext *context;
    char *my_buffer;
    char *tmp;
    uint16_t trim_len;

    context = (HttpPageContext *)data;
    if ( context == NULL )
    {
        return;
    }

    my_buffer = (char * )MEM_MALLOC_ALIGN( len + 1,
                                           cpu_get_dcache_line_size() );
    if ( my_buffer == NULL )
    {
        return;
    }

    memset( my_buffer, 0, len + 1 );
    strncpy( my_buffer, xml_data, len );

    tmp = skipWhiteSpace( ( char* ) my_buffer );
    trim_len = strlen( tmp );

    /* prevent underflow, by not decrementing trim_len if it is
     * already at 0 */
    if ( 0 != trim_len )
    {
        trim_len--;
        while ( ( trim_len > 0 ) &&
                ( ( tmp[ trim_len ] == ' ' ) ||
                  ( tmp[ trim_len ] == '\r' ) ||
                  ( tmp[ trim_len ] == '\n' ) ||
                  ( tmp[ trim_len ] == '\t' ) ) )
        {
            trim_len--;
        }
    }

    trim_len++;
    tmp[ trim_len ] = 0;

    if ( *tmp != 0 )
    {
        ConvertXmlTagtoOid( context->xmlTag, my_buffer, context );
    }


    MEM_FREE_AND_NULL( my_buffer );
}


/**
 * \brief Callback for end xml tag
 *
 * \param data - pointer to HttpPageContext for current
 * connection
 *
 * \param end_tag - name of tag that is closing
 *
 * This function parses the closing of an xml tag.  Update the
 * current 'xml path' value by removing this name
 *
 **/
void xmlEndElementHandle(void *data, const char *end_tag)
{
    char* last_tag;
    HttpPageContext *context = ( HttpPageContext *)data;
    if ( context == NULL )
    {
        return;
    }

    last_tag = strrchr( context->xmlTag, '|' );
    if( last_tag == NULL )
    {
        *context->xmlTag = 0;
    }
    else
    {
        // Terminate string at point of last tag marker
        *last_tag = 0;
    }
}  /* End of end handler */


#ifdef HAVE_NETWORK
/**
 * \brief A UDP packet has been received, handle any processing
 * based off this data
 *
 * \param udp_response - This is the data recieved in the UDP
 * packet
 *
 * \param destAddr - pointer to socket address structure that
 * recieved the UDP packet
 *
 * Currently do nothing
 *
 **/
void process_udp_response( CHAR* udp_response, struct sockaddr_storage *destAddr )
{
}

/**
 * \brief Regularly called function to allow custom processing
 * based for the web server
 *
 * Currently do nothing
 *
 **/
void http_custom_web_server_processing( )
{
}

#endif

