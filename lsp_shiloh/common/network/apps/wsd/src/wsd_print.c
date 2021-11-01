/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file wsd_print.c
 *
 * \brief This file contains the WS-Print response code
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------

#include "memAPI.h" // memTotalFreeMemAvailable()
#include "dprintf.h"
#include "lassert.h"
#include "platform_api.h"
#include "printvars_api.h"
#include "paper_types_api.h"
#include "paper_size_api.h"
#include "net_iface_vars.h"
#include "print_events_common_api.h"

#include "net_wsd_vars.h"
#include "net_wsd_print_config.h"
#include "wsd_print.h"
#include "wprt.nsmap"
#include "plat_print_config.h"  // Should be removed and replaced with calls to oids?
#include "oid_api.h"
#include "oid_sm_api.h" // OID_SM_PRINT_MEDIA_DEFAULT, OID_SM_PRINT_MEDIA_TYPE_DEFAULT

#ifdef HAVE_CMD 
#include "cmd_proc_api.h"
#endif //HAVE_CMD

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
 
// Debug configuration
#define DBG_PRFX "NET-WSPRINT: "
#define WSD_CMD "wsd" /* wsd_prt */

#define TRAY_NAME               "Tray"
#define UNKNOWN                 "unknown"
#define DEFAULT_JOB_NAME        "Default Job Name"
#define SUPPLY                  "Supply"
#define JOB_ABORT_TIMEOUT       60
#define OID_LENGTH              64
#define MILLISECONDS_IN_SECOND  1000


#define WSD_PRINT_NS            "http://schemas.microsoft.com/windows/2006/08/wdp/print"

#define WSD_PRINTER_STATUS      "wprt:PrinterStatus"
#define WSD_PRINTER_DESCRIPTION "wprt:PrinterDescription"
#define WSD_PRINTER_CONFIG      "wprt:PrinterConfiguration"
#define WSD_DEFAULT_PRT_TICKET  "wprt:DefaultPrintTicket"
#define WSD_PRINTER_CAPS        "wprt:PrinterCapabilities"
#define WSD_PRINT_JOB_STATUS    "wprt:JobStatus"
#define WSD_PRINT_TICKET        "wprt:PrintTicket"
#define WSD_DOCUMENTS           "wprt:Documents"

typedef error_type_t wsd_get_printer_elements_response( struct dpws* ws, struct wprt__PrinterElementDataType* response );
typedef struct 
{
    char                                *request;
    wsd_get_printer_elements_response   *request_fcn;
} wsd_get_printer_elements_requests_t;


// Flags for WSD Print Event flags group
#define WSD_PRINT_FLAG_INACTIVE_EVENT   0x01
#define WSD_PRINT_FLAG_FILL_BUFFER      0x02

typedef struct wsd_job_data_node_s
{
    ATLISTENTRY node;
    uint8_t* buf;       ///< data buffer
    uint32_t buflen;    ///< length of buffer
    uint32_t bufoffset; ///< current read offset in buffer
}wsd_job_data_node_t;

//--------------------------------------
// Global Definitions
//--------------------------------------

error_type_t wsd_get_printer_description    ( struct dpws* ws, struct wprt__PrinterElementDataType *response );
error_type_t wsd_get_printer_configuration  ( struct dpws* ws, struct wprt__PrinterElementDataType *response );
error_type_t wsd_get_printer_status         ( struct dpws* ws, struct wprt__PrinterElementDataType *response );
error_type_t wsd_get_default_print_ticket   ( struct dpws* ws, struct wprt__PrinterElementDataType *response );
error_type_t wsd_get_printer_capabilities   ( struct dpws* ws, struct wprt__PrinterElementDataType *response );
void wsd_get_print_job_status( struct dpws *ws, 
                               struct wprt__JobStatusType *job_status, 
                               wsd_job_info_t *job_info );
void wsd_get_print_ticket( struct dpws *ws, struct wprt__PrintTicketType *ticket, wsd_job_info_t *job_info );
void wsd_get_documents( struct dpws *ws, struct wprt__DocumentsType *doc );
void wsd_get_document_processing( struct dpws *ws, struct wprt__DocumentProcessingType *doc );

wsd_get_printer_elements_requests_t wsd_get_printer_elements_allowed_values [] =
{
    { "wprt:PrinterDescription",    wsd_get_printer_description     },
    { "wprt:PrinterConfiguration",  wsd_get_printer_configuration   },
    { "wprt:PrinterStatus",         wsd_get_printer_status          },
    { "wprt:DefaultPrintTicket",    wsd_get_default_print_ticket    },
    { "wprt:PrinterCapabilities",   wsd_get_printer_capabilities    },
};
#define WSD_NUM_GET_PRINTER_ELEMENTS_REQUESTS   ( sizeof( wsd_get_printer_elements_allowed_values ) / sizeof( wsd_get_printer_elements_requests_t ) )

wsd_get_printer_status_options_t wsd_get_printer_status_values [] = 
{
    // status_category   printer_state   printer_primary_state_reason  printer_state_reason

//    "Idle", "Processing", "Stopped"
//    "MarkerFailure", "MediaLow", "None", "OutputAreaAlmostFull", "Paused"
//Use value below    { STATCAT_ONLINE,       "Idle",         "None",             "None"              },
    { STATCAT_INFO,         "Processing",   "None",             "None",             },
    { STATCAT_DOOROPEN,     "Stopped",      "DoorOpen",         "None"              },
//    { STATCAT_INKLOW,       "Idle",         "MarkerSupplyLow",  "MarkerSupplyLow"   },
//    { STATCAT_INKOUT,       "Idle",         "MarkerSupplyEmpty","MarkerSupplyEmpty" },
    { STATCAT_PAPEROUT,     "Stopped",      "MediaEmpty",       "MediaEmpty"        },
    { STATCAT_PAPERJAM_TYPE,"Stopped",      "MediaJam",         "MediaJam",         },
    { STATCAT_PAPERJAM_AREA,"Stopped",      "MediaJam",         "MediaJam",         },
    { STATCAT_INPUT,        "Stopped",      "MediaNeeded",      "MediaNeeded"       },
    { STATCAT_OUTPUT,       "Stopped",      "OutputAreaFull",   "OutputAreaFull"    },
    { STATCAT_MISPRINT,     "Stopped",      "AttentionRequired","AttentionRequired" },
    { STATCAT_ERROR,        "Stopped",      "AttentionRequired","AttentionRequired" },
};
#define NUM_WSD_GET_PRINTER_STATUS_VALUES   ( sizeof( wsd_get_printer_status_values ) / sizeof( wsd_get_printer_status_options_t ) )

char *supply_color[] = 
{
    "Black",
    "Cyan",
    "Magenta",
    "Yellow",
};

wsd_get_printer_status_options_t wsd_status_idle = 
    { STATCAT_ONLINE,       "Idle",         "None",             "None"              };

static net_wsd_print_config_t   wsd_print_config_vals;
static plat_print_config_t      plat_config_vals;

static pthread_mutex_t          wsd_print_mutex = POSIX_MUTEX_INITIALIZER;
static pthread_cond_t           wsd_print_cond = PTHREAD_COND_INITIALIZER;
static uint32_t                 wsd_print_flags = 0;


//--------------------------------------
// Local Function Declarations
//--------------------------------------
void wsd_print_startup( void );
int16_t wsd_print_dpws_service_class( int16_t );
void wsd_print_dpws_device_type( int16_t device );
void wsd_print_shutdown( void );
//static void wsd_print_job_timer_callback( ULONG param );
static void wsd_print_io_notify_func(net_io_session_event_t event, void *user_ctxt);
static int32_t wsd_print_io_recv_func(char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt);
void wsd_job_event_notify_func( net_io_job_event_t event, net_io_job_hndl_t job_hndl, void *user_ctxt );
static bool wsd_validate_print_ticket( struct wprt__PrintTicketType *ticket, struct dpws *ws, char **detail );

#ifdef HAVE_CMD
static const char *wsd_print_cmd_status_door_desc;
static const char *wsd_print_cmd_status_door_usage;
static const char *wsd_print_cmd_status_door_notes;
static int wsd_print_cmd_status_door_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_status_paperout_desc;
static const char *wsd_print_cmd_status_paperout_usage;
static const char *wsd_print_cmd_status_paperout_notes;
static int wsd_print_cmd_status_paperout_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_status_paperjam_desc;
static const char *wsd_print_cmd_status_paperjam_usage;
static const char *wsd_print_cmd_status_paperjam_notes;
static int wsd_print_cmd_status_paperjam_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_status_printing_desc;
static const char *wsd_print_cmd_status_printing_usage;
static const char *wsd_print_cmd_status_printing_notes;
static int wsd_print_cmd_status_printing_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_status_manual_duplex_desc;
static const char *wsd_print_cmd_status_manual_duplex_usage;
static const char *wsd_print_cmd_status_manual_duplex_notes;
static int wsd_print_cmd_status_manual_duplex_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_rate_desc;
static const char *wsd_print_cmd_rate_usage;
static const char *wsd_print_cmd_rate_notes;
static int wsd_print_cmd_rate_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_enable_desc;
static const char *wsd_print_cmd_enable_usage;
static const char *wsd_print_cmd_enable_notes;
static int wsd_print_cmd_enable_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_disable_desc;
static const char *wsd_print_cmd_disable_usage;
static const char *wsd_print_cmd_disable_notes;
static int wsd_print_cmd_disable_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_restart_desc;
static const char *wsd_print_cmd_restart_usage;
static const char *wsd_print_cmd_restart_notes;
static int wsd_print_cmd_restart_cb( int argc, char *argv[] );

static const char *wsd_print_cmd_info_desc;
static const char *wsd_print_cmd_info_usage;
static const char *wsd_print_cmd_info_notes;
static int wsd_print_cmd_info_cb( int argc, char *argv[] );
#endif // HAVE_CMD


void wsd_print_set_flags( uint32_t flags );
void wsd_print_clear_flags( uint32_t flags );

void *wsd_print_mime_write_open( struct soap*, void*, const char*, const char*, const char*, enum soap_mime_encoding );
int wsd_print_mime_write( struct soap *soap, void *handle, const char *buf, size_t len );
void wsd_print_mime_write_close(struct soap *soap, void *handle );

//--------------------------------------
// Local Function Definitions
//--------------------------------------
    
void wsd_print_init( void )
{
  //  UINT tx_res;

    wsd_func_table_t wsd_print_func_table =
    {
        wsd_print_startup,
        wsd_print_dpws_service_class,
        wsd_print_dpws_device_type,
        wsd_print_shutdown,
        NULL,
        NULL,
        NULL,
        wsd_print_mime_write_open,
        wsd_print_mime_write,
        wsd_print_mime_write_close,
        NULL,         
    };


    net_wsd_print_config( &wsd_print_config_vals );
    plat_print_config( &plat_config_vals );

    wsd_register_protocol( WSD_PRINT, &wsd_print_func_table );   
   
#ifdef HAVE_CMD
    int i_res;

    i_res = cmd_register_cmd( WSD_CMD,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "info",
                                 wsd_print_cmd_info_desc,
                                 wsd_print_cmd_info_usage,
                                 wsd_print_cmd_info_notes,
                                 wsd_print_cmd_info_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "status",
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD " status",
                                 "door",
                                 wsd_print_cmd_status_door_desc,
                                 wsd_print_cmd_status_door_usage,
                                 wsd_print_cmd_status_door_notes,
                                 wsd_print_cmd_status_door_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD " status",
                                 "paperout",
                                 wsd_print_cmd_status_paperout_desc,
                                 wsd_print_cmd_status_paperout_usage,
                                 wsd_print_cmd_status_paperout_notes,
                                 wsd_print_cmd_status_paperout_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD " status",
                                 "paperjam",
                                 wsd_print_cmd_status_paperjam_desc,
                                 wsd_print_cmd_status_paperjam_usage,
                                 wsd_print_cmd_status_paperjam_notes,
                                 wsd_print_cmd_status_paperjam_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD " status",
                                 "printing",
                                 wsd_print_cmd_status_printing_desc,
                                 wsd_print_cmd_status_printing_usage,
                                 wsd_print_cmd_status_printing_notes,
                                 wsd_print_cmd_status_printing_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD " status",
                                 "manual_duplex",
                                 wsd_print_cmd_status_manual_duplex_desc,
                                 wsd_print_cmd_status_manual_duplex_usage,
                                 wsd_print_cmd_status_manual_duplex_notes,
                                 wsd_print_cmd_status_manual_duplex_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "rate",
                                 wsd_print_cmd_rate_desc,
                                 wsd_print_cmd_rate_usage,
                                 wsd_print_cmd_rate_notes,
                                 wsd_print_cmd_rate_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "enable",
                                 wsd_print_cmd_enable_desc,
                                 wsd_print_cmd_enable_usage,
                                 wsd_print_cmd_enable_notes,
                                 wsd_print_cmd_enable_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "disable",
                                 wsd_print_cmd_disable_desc,
                                 wsd_print_cmd_disable_usage,
                                 wsd_print_cmd_disable_notes,
                                 wsd_print_cmd_disable_cb );
    XASSERT( CMD_OK == i_res, i_res );
    i_res = cmd_register_subcmd( WSD_CMD,
                                 "restart",
                                 wsd_print_cmd_restart_desc,
                                 wsd_print_cmd_restart_usage,
                                 wsd_print_cmd_restart_notes,
                                 wsd_print_cmd_restart_cb );
    XASSERT( CMD_OK == i_res, i_res );
#endif
}

void wsd_print_startup( void )
{
    wsd_print_events_init( );
}

int16_t wsd_print_dpws_service_class( int16_t device_model )
{
    int ret;
    //  Structure containing the WSDL namespace and location for the hosted service.
    struct wsdl_info wprt_wsdl = { WSD_PRINT_NS, WSD_PRINT_NS"/WSDPrinterService.wsdl" };

    // Qualified name used to specify the type supported by the device (could have been prefixed).
    // Prefixed qualified name used to define the service port type (from WSDL). Using
    // a prefix will ensure it will be used in messages.
    struct prefixed_qname wprt_service_type = { { WSD_PRINT_NS, "PrinterServiceType" }, "wprt" };
    uint16_t service_class;

    // Create scanner service class
    service_class = dpws_create_service_class();
    ret = DPWS_ADD_PTR_ATT( service_class, DPWS_PTR_PREFIXED_TYPE, &wprt_service_type );	XASSERT( ret == 0, ret );
    ret = DPWS_ADD_PTR_ATT( service_class, DPWS_PTR_WSDL, &wprt_wsdl );	                        XASSERT( ret == 0, ret );
    ret = DPWS_ADD_PTR_ATT( service_class, DPWS_PTR_HANDLING_FUNCTION, &wprt_serve_request );   XASSERT( ret == 0, ret );
    ret = DPWS_SET_STR_ATT( service_class, DPWS_STR_ID, "http://www.microsoft.com/printer" );   XASSERT( ret == 0, ret );
    ret = dpws_register_service_class( device_model, service_class );                           XASSERT( ret == 0, ret );
    ret = dpws_release_handle( service_class );                                                 XASSERT( ret == 0, ret );

    return service_class;
}

void wsd_print_dpws_device_type( int16_t device )
{
    int ret;
    struct prefixed_qname wprt_device_type = { { WSD_PRINT_NS, "PrintDeviceType" }, "wprt" };
    
    ret = DPWS_ADD_PTR_ATT( device, DPWS_PTR_PREFIXED_TYPE, &wprt_device_type );  
    XASSERT( ret == 0, ret );

}

void wsd_print_shutdown( void )
{
    wsd_print_events_shutdown( );
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__CreatePrintJob(struct dpws* ws, struct wprt__CreatePrintJobRequestType *request, struct wprt__CreatePrintJobResponseType *response)
{
    wsd_job_info_t *job_info;
//    UINT tx_res;
    char *detail = NULL;

    wsd_lock_job_history( );

    if ( wsd_validate_print_ticket( request->PrintTicket, ws, &detail ) == false )
    {
        wsd_unlock_job_history( );
        return dpws_fault( ws, SENDER_FAULT_VALUE, "At least one input argument is invalid ", "wprt:InvalidArgs" , detail );
    }

    if ( wsd_new_job( WSD_PRINT, &job_info ) == NET_WSD_RESOURCE_FAILURE )
    {
        return dpws_fault( ws, RECEIVER_FAULT_VALUE, "The service is temporarily blocked and can't accept a new job request.", "wprt:ServerErrorNotAcceptingJobs" , NULL );
    }

    // Clear out old information
    job_info->name = MEM_MALLOC( strlen( request->PrintTicket->JobDescription->JobName->__item ) + 1 );
    if ( job_info->name == NULL )
    {
        return dpws_fault( ws, RECEIVER_FAULT_VALUE, "The service has an unexpected error", "wprt:ServerErrorInternalError" , detail );
    }
    strcpy( job_info->name, request->PrintTicket->JobDescription->JobName->__item );
    job_info->user_name = MEM_MALLOC( strlen( request->PrintTicket->JobDescription->JobOriginatingUserName->__item ) + 1 );
    if ( job_info->user_name == NULL )
    {
        MEM_FREE_AND_NULL( job_info->name );
        return dpws_fault( ws, RECEIVER_FAULT_VALUE, "The service has an unexpected error", "wprt:ServerErrorInternalError" , detail );
    }
    strcpy( job_info->user_name, request->PrintTicket->JobDescription->JobOriginatingUserName->__item );
    job_info->state = WSD_JOB_STATE_STARTED;
    job_info->reasons = WSD_JOB_STATE_REASON_JOB_INCOMING;
    wsd_print_send_msg( NET_WSD_JOB_STATUS, ( void * )job_info->id );

    job_info->state = WSD_JOB_STATE_PENDING;
    wsd_print_send_msg( NET_WSD_JOB_STATUS, ( void * )job_info->id );

#if 0 
    tx_timer_delete( &wsd_print_job_timer );
    tx_res = tx_timer_create( &wsd_print_job_timer,
                              "wsd_print_job_timer",
                              wsd_print_job_timer_callback,
                              job_info->id,
                              JOB_ABORT_TIMEOUT * SYS_TICK_FREQ,
                              0,
                              TX_AUTO_ACTIVATE );
    XASSERT( OK == tx_res, tx_res );
#endif
    
    response->JobId = dpws_malloc( ws, sizeof( struct wprt__JobIdType ) );
    response->JobId->__item = job_info->id;

    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__SendDocument(struct dpws* ws, struct wprt__SendDocumentRequestType *request, struct wprt__SendDocumentResponseType *response)
{
    int32_t net_io_rcode;
    net_io_func_table_t func_table;
    wsd_job_info_t *job_info;
    ULONG requested_flags;
    ULONG actual_flags;

    wsd_lock_job_history( );
    job_info = wsd_get_job_info( request->JobId->__item );
    if ( job_info == NULL )
    {   
        char *reason = dpws_malloc( ws, strlen( "     : Incorrect JobId" ) );
        minSprintf( reason, "%d: Incorrect JobId", request->JobId->__item );
        wsd_unlock_job_history( );
        return dpws_fault( ws, SENDER_FAULT_VALUE, "Specified JobId not found", "wprt:ClientErrorJobIdNotFound", reason );
    }

    // Found the job indicated - verify we can accept a document
    if ( ( job_info->state != WSD_JOB_STATE_PENDING ) ||
         ( request->LastDocument->__item != xsd__boolean__true_ ) )
    {
        // We don't handle multiple documents per job
        DBG_ERR( "%s - Only allow one document per job\n", __FUNCTION__ );
        wsd_unlock_job_history( );
        return dpws_fault( ws, SENDER_FAULT_VALUE, "Print Service does not support jobs with multiple documents.", "wprt:ClientErrorMultipleDocumentsNotSupported", NULL );
    }

    // Shutdown io timeout timer
//    tx_timer_deactivate( &wsd_print_job_timer );

    if ( soap_check_mime_attachments( dpws_dpws2soap( ws ) ) )
    {
        bool found = false;
        uint32_t i;
        // Check that the options are supported

        for ( i = 0; i < plat_config_vals.num_compressions; i++ )      
        {
            if ( strcmp( request->DocumentDescription->Compression->__item, plat_config_vals.compressions[ i ] ) == 0 )
            {
                found = true;
                break;
            }
        }

        if ( found == false)
        {
            char *reason;
            job_info->state     = WSD_JOB_STATE_ABORTED;
            job_info->reasons   = WSD_JOB_STATE_REASON_UNSUPPORTED_COMPRESSION;
            job_info->active    = false;

            // REVISIT TODO - should an event notify be sent at this point??

            reason = dpws_malloc( ws,  strlen( " parameter value not supported" ) + strlen( request->DocumentDescription->Compression->__item ) + 1 );
            DBG_ERR( "%s - Unknown compression option: %s\n", __FUNCTION__, request->DocumentDescription->Compression->__item );
            minSprintf( reason, "%s parameter value not supported", request->DocumentDescription->Compression->__item );
            wsd_unlock_job_history( );
            return dpws_fault( ws, SENDER_FAULT_VALUE, reason, "wprt:ClientErrorCompressionNotSupported", NULL );
        }
        
        found = false;

        for ( i = 0; i < plat_config_vals.num_formats; i++ )
        {
            if ( strcmp( request->DocumentDescription->Format->__item, plat_config_vals.formats[ i ] ) == 0 )
            {
                found = true;
                break;
            }
        }

        if ( found == false )
        {
            char *reason;
            job_info->state     = WSD_JOB_STATE_ABORTED;
            job_info->reasons   = WSD_JOB_STATE_REASON_UNSUPPORTED_DOCUMENT_FORMAT;
            job_info->active    = false;

            // REVISIT TODO - should an event notify be sent at this point??

            reason = dpws_malloc( ws,  strlen( " parameter value not supported" ) + strlen( request->DocumentDescription->Format->__item ) + 1 );
            DBG_ERR( "%s - Unknown format option: %s\n", __FUNCTION__, request->DocumentDescription->Compression->__item );
            minSprintf( reason, "%s parameter value not supported", request->DocumentDescription->Format->__item );
            wsd_unlock_job_history( );
            return dpws_fault( ws, SENDER_FAULT_VALUE, reason, "wprt:ClientErrorFormatNotSupported", NULL );
        }

        wsd_print_clear_flags( WSD_PRINT_FLAG_FILL_BUFFER );
        wsd_print_clear_flags( WSD_PRINT_FLAG_INACTIVE_EVENT );

        func_table.io_notify = wsd_print_io_notify_func;
        func_table.io_recv = wsd_print_io_recv_func;
        net_io_rcode = net_io_open( e_Print, &func_table, (void *)request->JobId->__item, &job_info->io_session_hndl );
        if( net_io_rcode != NET_IO_SUCCESS )
        {
            job_info->state     = WSD_JOB_STATE_PROCESSING_STOPPED;
            job_info->reasons   = WSD_JOB_STATE_REASON_NONE;
            job_info->active    = false;
            wsd_unlock_job_history( );
            return dpws_fault( ws, RECEIVER_FAULT_VALUE, "The service had an unexpected error", "wprt:ServerErrorTemporaryError" , NULL );
        }

        requested_flags = WSD_PRINT_FLAG_INACTIVE_EVENT;
//        tx_event_flags_get( &wsd_print_flags, requested_flags, TX_AND, &actual_flags, TX_WAIT_FOREVER);
        
        pthread_mutex_lock(&wsd_print_mutex);
        while(!(wsd_print_flags & requested_flags))
        {
            pthread_cond_wait(&wsd_print_cond, &wsd_print_mutex);
        }
        actual_flags = wsd_print_flags;
        wsd_print_flags = 0;
        pthread_mutex_unlock(&wsd_print_mutex);


        net_io_job_register_event_cb( wsd_job_event_notify_func, job_info->io_session_hndl, (void *)request->JobId->__item );

        wsd_unlock_job_history( );

        wsd_print_set_flags( WSD_PRINT_FLAG_FILL_BUFFER );
        soap_get_mime_attachment( dpws_dpws2soap( ws ), (void *)request->JobId->__item );

        wsd_lock_job_history( );
        job_info = wsd_get_job_info( request->JobId->__item );
        if ( job_info != NULL )
        {
            job_info->job_data_recv_done = true;
        }
    }
    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__AddDocument(struct dpws* ws, struct wprt__AddDocumentRequestType *wprt__AddDocumentRequest, struct wprt__AddDocumentResponseType *wprt__AddDocumentResponse)
{
    return dpws_fault( ws, RECEIVER_FAULT_VALUE, "Print Service does not support the AddDocument operation", "wprt:ServerErrorAddDocumentNotSupported", NULL );
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__CancelJob(struct dpws* ws, struct wprt__CancelJobRequestType *request, struct wprt__CancelJobResponseType *response)
{
    wsd_job_info_t *job_info;

    wsd_lock_job_history( );
    job_info = wsd_get_job_info( request->JobId->__item );
    if ( job_info == NULL )
    {   
        char *reason = dpws_malloc( ws, strlen( "     : Incorrect JobId" ) );
        minSprintf( reason, "%d: Incorrect JobId", request->JobId->__item );
        wsd_unlock_job_history( );
        return dpws_fault( ws, SENDER_FAULT_VALUE, "Specified JobId not found", "wprt:ClientErrorJobIdNotFound", reason );
    }

    switch ( job_info->state )
    {
        case WSD_JOB_STATE_ABORTED:
        case WSD_JOB_STATE_CANCELED:
        case WSD_JOB_STATE_COMPLETED:
        case WSD_JOB_STATE_TERMINATING:
            wsd_unlock_job_history( );
            return dpws_fault( ws, SENDER_FAULT_VALUE, "At least one input argument is invalid ", "wprt:InvalidArgs" , "JobId" );
            break;
        case WSD_JOB_STATE_PENDING_HELD:
        case WSD_JOB_STATE_PROCESSING:
        case WSD_JOB_STATE_PROCESSING_STOPPED:
            // REVISIT send job cancel message
            //

            net_io_job_cancel( job_info->job_hndl );

            if ( job_info->io_session_hndl )
            {
                net_io_close( job_info->io_session_hndl );
                job_info->io_session_hndl = NULL;
            }

            job_info->state = WSD_JOB_STATE_TERMINATING;
            job_info->reasons = WSD_JOB_STATE_REASON_JOB_CANCELED_BY_USER;

            wsd_print_send_msg( NET_WSD_JOB_STATUS, ( void * )job_info->id );
            wsd_print_send_msg( NET_WSD_JOB_END_STATE, (void*)job_info->id);

            break;
        case WSD_JOB_STATE_PENDING:
            if ( job_info->io_session_hndl )
            {
                net_io_close( job_info->io_session_hndl );
                job_info->io_session_hndl = NULL;
            }
            // Intentionally fall through
        case WSD_JOB_STATE_STARTED:
            job_info->state = WSD_JOB_STATE_CANCELED;
            job_info->reasons = WSD_JOB_STATE_REASON_JOB_CANCELED_BY_USER;
            job_info->active = false;
            // Shutdown io timeout timer
           // tx_timer_deactivate( &wsd_print_job_timer );

            wsd_print_send_msg( NET_WSD_JOB_END_STATE, ( void * )job_info->id );
            break;
        default:
            XASSERT( 0, job_info->state );
    }
    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__GetPrinterElements(struct dpws* ws, struct wprt__GetPrinterElementsRequestType *request, struct wprt__GetPrinterElementsResponseType *response)
{
    uint32_t i;
    uint32_t j;
    int ret = DPWS_OK;

    dpws_set_namespaces( ws, wprt_namespaces );

    response->PrinterElements = dpws_malloc( ws, sizeof( struct wprt__PrinterElementsType ) );
    response->PrinterElements->ElementData = dpws_malloc( ws, sizeof( struct wprt__PrinterElementDataType ) * request->RequestedElements->__sizeName );
    response->PrinterElements->__sizeElementData = 0;

    // This is the information for the request
    for ( i = 0; i < request->RequestedElements->__sizeName; i++ )
    {

        // Build the response
        for ( j = 0; j < WSD_NUM_GET_PRINTER_ELEMENTS_REQUESTS; j++ )
        {
            if ( strcmp( request->RequestedElements->Name[ i ].__item, wsd_get_printer_elements_allowed_values[ j ].request ) == 0 )
            {
                if ( wsd_get_printer_elements_allowed_values[ j ].request_fcn( ws, &response->PrinterElements->ElementData[ response->PrinterElements->__sizeElementData++ ] ) != NET_WSD_SUCCESS )
                {
                    DBG_ERR( "%s - Invalid request function response\n", __FUNCTION__ );
                    ret = DPWS_ERR_INTERNAL_ERROR;
                }
                else
                {
                    DBG_VERBOSE( "%s - elements: %d, name: %s, valid: %d\n", __FUNCTION__, response->PrinterElements->__sizeElementData, response->PrinterElements->ElementData[ 0 ].Name, response->PrinterElements->ElementData[ 0 ].Valid );
                }
                break;
            }
        }
        if ( j == WSD_NUM_GET_PRINTER_ELEMENTS_REQUESTS )
        {
            // Did not find matching element!
            DBG_ERR( "%s - Failed to find matching element Element: %s\n", __FUNCTION__, request->RequestedElements->Name[ i ].__item );
            return dpws_fault( ws, SENDER_FAULT_VALUE, "The [action] cannot be processed at the receiver", "wsa:ActionNotSupported", request->RequestedElements->Name[ i ].__item );
        }
    }
    DBG_VERBOSE( "%s: No more elements - ret: %d\n", __FUNCTION__, ret );

    return ret;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__GetJobElements(struct dpws* ws, struct wprt__GetJobElementsRequestType *request, struct wprt__GetJobElementsResponseType *response)
{
    uint32_t j;
    wsd_job_info_t *job_info;

    // Check if the ID is still valid
    wsd_lock_job_history( );
    job_info = wsd_get_job_info( request->JobId->__item );
    if ( job_info == NULL )
    {
        char *details;

        wsd_unlock_job_history( );

        // Send error condition
        DBG_ERR( "Job not found - sending error\n" );
        details = dpws_malloc( ws, strlen( "Jobid: " ) + 10 ); // 10 for job id length and null
        ASSERT( details != NULL );
        minSprintf( details, "Jobid: %d", request->JobId->__item );
        return dpws_fault( ws, SENDER_FAULT_VALUE, "Specified JobId not found", "wprt:ClientErrorJobIdNotFound", details );
    }
    else
    {
        response->JobElements = dpws_malloc( ws, sizeof( struct wprt__JobElementsType ) );
        response->JobElements->__sizeElementData = request->RequestedElements->__sizeName;
        response->JobElements->ElementData = dpws_malloc( ws, sizeof( struct wprt__JobElementDataType ) * request->RequestedElements->__sizeName );

        for ( j = 0; j < request->RequestedElements->__sizeName; j++ )
        {
            response->JobElements->ElementData[ j ].Valid = xsd__boolean__false_;
            if ( strcmp( request->RequestedElements->Name[ j ].__item, WSD_PRINT_JOB_STATUS ) == 0 )
            {
                struct wprt__JobStatusType *JobStatus;
                response->JobElements->ElementData[ j ].__union_JobElementDataType = SOAP_UNION__wprt__union_JobElementDataType_JobStatus;
                response->JobElements->ElementData[ j ].Name = dpws_malloc( ws, strlen( WSD_PRINT_JOB_STATUS  ) + 1 );
                strcpy( response->JobElements->ElementData[ j ].Name, WSD_PRINT_JOB_STATUS);
                response->JobElements->ElementData[ j ].Valid = xsd__boolean__true_;

                JobStatus = dpws_malloc( ws, sizeof( struct wprt__JobStatusType ) );
                wsd_get_print_job_status( ws, JobStatus, job_info );
                response->JobElements->ElementData[ j ].union_JobElementDataType.JobStatus = JobStatus;
            }
            else if ( strcmp( request->RequestedElements->Name[ j ].__item, WSD_PRINT_TICKET ) == 0 )
            {
                response->JobElements->ElementData[ j ].__union_JobElementDataType = SOAP_UNION__wprt__union_JobElementDataType_PrintTicket;
                response->JobElements->ElementData[ j ].Name = dpws_malloc( ws, strlen( WSD_PRINT_TICKET ) + 1 );
                strcpy( response->JobElements->ElementData[ j ].Name, WSD_PRINT_TICKET );
                response->JobElements->ElementData[ j ].union_JobElementDataType.PrintTicket = dpws_malloc( ws, sizeof( struct wprt__PrintTicketType ) );
                response->JobElements->ElementData[ j ].Valid = xsd__boolean__true_;
                wsd_get_print_ticket( ws, response->JobElements->ElementData[ j ].union_JobElementDataType.PrintTicket, job_info );
            }
            else if ( strcmp( request->RequestedElements->Name[ j ].__item, WSD_DOCUMENTS ) == 0 )
            {
                response->JobElements->ElementData[ j ].__union_JobElementDataType = SOAP_UNION__wprt__union_JobElementDataType_Documents;
                response->JobElements->ElementData[ j ].Name = dpws_malloc( ws, strlen( WSD_DOCUMENTS ) + 1 );
                strcpy( response->JobElements->ElementData[ j ].Name, WSD_DOCUMENTS );
                response->JobElements->ElementData[ j ].union_JobElementDataType.Documents = dpws_malloc( ws, sizeof( struct wprt__DocumentsType ) );
                response->JobElements->ElementData[ j ].Valid = xsd__boolean__true_;
                wsd_get_documents( ws, response->JobElements->ElementData[ j ].union_JobElementDataType.Documents );
            }
            else
            {
                wsd_unlock_job_history( );

                DBG_ERR( "Invalid action: %s\n", request->RequestedElements->Name[ j ].__item );
                return dpws_fault( ws, SENDER_FAULT_VALUE, "The [action] cannot be processed at the receiver", "wsa:ActionNotSupported", request->RequestedElements->Name[ j ].__item );
            }
        }
    }
    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__GetActiveJobs(struct dpws* ws, struct wprt__GetActiveJobsRequestType *request, struct wprt__GetActiveJobsResponseType *response)
{
    uint32_t i;
    uint32_t num_active = 0;
    wsd_job_info_t *history;
    net_wsd_config_t *config;

    response->ActiveJobs = dpws_malloc( ws, sizeof( struct wprt__ListOfSummarysType ) );

    wsd_lock_job_history( );
    history = wsd_get_job_history( );
    config = wsd_get_config( );

    for ( i = 0; i < config->num_job_history_elements; i++ )
    {
        if ( ( history[ i ].id != 0 ) &&
             ( history[ i ].active == true ) &&
             ( history[ i ].protocol == WSD_PRINT ) )
        {
            num_active++;
        }
    }

    response->ActiveJobs->__sizeJobSummary = num_active;
    response->ActiveJobs->JobSummary = dpws_malloc( ws, sizeof( struct wprt__JobSummaryType ) * num_active );

    num_active = 0;
    for ( i = 0; i < config->num_job_history_elements; i++ )
    {
        if ( ( history[ i ].id != 0 ) &&
             ( history[ i ].active == true ) &&
             ( history[ i ].protocol == WSD_PRINT ) )
        {
            // 0 - 2147483647
            response->ActiveJobs->JobSummary[ num_active ].JobId = dpws_malloc( ws, sizeof( struct wprt__JobIdType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobId->__item = history[ i ].id;
        
            response->ActiveJobs->JobSummary[ num_active ].JobName = dpws_malloc( ws, sizeof( struct wprt__JobNameType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobName->__item = dpws_malloc( ws, strlen( history[ i ].name ) + 1 );
            strcpy( response->ActiveJobs->JobSummary[ num_active ].JobName->__item, history[ i ].name );
        
            response->ActiveJobs->JobSummary[ num_active ].JobOriginatingUserName = dpws_malloc( ws, sizeof( struct wprt__JobOriginatingUserNameType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobOriginatingUserName->__item = dpws_malloc( ws, strlen( history[ i ].user_name ) +1 );
            strcpy( response->ActiveJobs->JobSummary[ num_active ].JobOriginatingUserName->__item, history[ i ].user_name );
        
            response->ActiveJobs->JobSummary[ num_active ].JobState = dpws_malloc( ws, sizeof( struct wprt__JobStateType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobState->__item = dpws_malloc( ws, strlen( wsd_get_job_state_string( history[ i ].state ) ) + 1 );
            strcpy( response->ActiveJobs->JobSummary[ num_active ].JobState->__item, wsd_get_job_state_string( history[ i ].state ) );
        
            response->ActiveJobs->JobSummary[ num_active ].JobStateReasons = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonsType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobStateReasons->__sizeJobStateReason = 1;
            response->ActiveJobs->JobSummary[ num_active ].JobStateReasons->JobStateReason = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonType ) );
            response->ActiveJobs->JobSummary[ num_active ].JobStateReasons->JobStateReason[ 0 ].__item = dpws_malloc( ws, strlen( wsd_job_state_reason_str( history[ i ].reasons ) ) + 1 );
            strcpy( response->ActiveJobs->JobSummary[ num_active ].JobStateReasons->JobStateReason[ 0 ].__item, wsd_job_state_reason_str( history[ i ].reasons ) );
        
            response->ActiveJobs->JobSummary[ num_active ].KOctetsProcessed = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->ActiveJobs->JobSummary[ num_active ].KOctetsProcessed->__item = history[ i ].koctets_processed;
        
            response->ActiveJobs->JobSummary[ num_active ].MediaSheetsCompleted = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->ActiveJobs->JobSummary[ num_active ].MediaSheetsCompleted->__item = history[ i ].sheets_completed;
        
            response->ActiveJobs->JobSummary[ num_active ].NumberOfDocuments = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->ActiveJobs->JobSummary[ num_active ].NumberOfDocuments->__item = history[ i ].num_documents;

            num_active++;
        }
    }

    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__GetJobHistory(struct dpws* ws, struct wprt__GetJobHistoryRequestType *request, struct wprt__GetJobHistoryResponseType *response)
{
    uint32_t i;
    uint32_t num_in_history = 0;
    wsd_job_info_t *history;
    net_wsd_config_t *config;

    response->JobHistory = dpws_malloc( ws, sizeof( struct wprt__ListOfSummarysType ) );

    wsd_lock_job_history( );
    history = wsd_get_job_history( );
    config = wsd_get_config( );

    for ( i = 0; i < config->num_job_history_elements; i++ )
    {
        if ( ( history[ i ].id != 0 ) &&
             ( history[ i ].active == false ) &&
             ( history[ i ].protocol == WSD_PRINT ) )
        {
            num_in_history++;
        }
    }

    response->JobHistory->__sizeJobSummary = num_in_history;
    response->JobHistory->JobSummary = dpws_malloc( ws, sizeof( struct wprt__JobSummaryType ) * num_in_history );

    num_in_history = 0;
    for ( i = 0; i < config->num_job_history_elements; i++ )
    {
        if ( ( history[ i ].id != 0 ) &&
             ( history[ i ].active == false ) &&
             ( history[ i ].protocol == WSD_PRINT ) )
        {
            response->JobHistory->JobSummary[ num_in_history ].JobId = dpws_malloc( ws, sizeof( struct wprt__JobIdType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobId->__item = history[ i ].id;

            response->JobHistory->JobSummary[ num_in_history ].JobName = dpws_malloc( ws, sizeof( struct wprt__JobNameType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobName->__item = dpws_malloc( ws, strlen( history[ i ].name ) + 1 );
            strcpy( response->JobHistory->JobSummary[ num_in_history ].JobName->__item, history[ i ].name );

            response->JobHistory->JobSummary[ num_in_history ].JobOriginatingUserName = dpws_malloc( ws, sizeof( struct wprt__JobOriginatingUserNameType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobOriginatingUserName->__item = dpws_malloc( ws, strlen( history[ i ].user_name ) + 1 );
            strcpy( response->JobHistory->JobSummary[ num_in_history ].JobOriginatingUserName->__item, history[ i ].user_name );

            response->JobHistory->JobSummary[ num_in_history ].JobState = dpws_malloc( ws, sizeof( struct wprt__JobStateType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobState->__item = dpws_malloc( ws, strlen( wsd_get_job_state_string( history[ i ].state ) ) + 1 );
            strcpy( response->JobHistory->JobSummary[ num_in_history ].JobState->__item, wsd_get_job_state_string( history[ i ].state ) );

            response->JobHistory->JobSummary[ num_in_history ].JobStateReasons = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonsType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobStateReasons->__sizeJobStateReason = 1;
            response->JobHistory->JobSummary[ num_in_history ].JobStateReasons->JobStateReason = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonType ) );
            response->JobHistory->JobSummary[ num_in_history ].JobStateReasons->JobStateReason[ 0 ].__item = dpws_malloc( ws, strlen( wsd_job_state_reason_str( history[ i ].reasons ) ) + 1 );
            strcpy( response->JobHistory->JobSummary[ num_in_history ].JobStateReasons->JobStateReason[ 0 ].__item, wsd_job_state_reason_str( history[ i ].reasons ) );

            response->JobHistory->JobSummary[ num_in_history ].KOctetsProcessed = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->JobHistory->JobSummary[ num_in_history ].KOctetsProcessed->__item = history[ i ].koctets_processed;

            response->JobHistory->JobSummary[ num_in_history ].MediaSheetsCompleted = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->JobHistory->JobSummary[ num_in_history ].MediaSheetsCompleted->__item = history[ i ].sheets_completed;

            response->JobHistory->JobSummary[ num_in_history ].NumberOfDocuments = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
            response->JobHistory->JobSummary[ num_in_history ].NumberOfDocuments->__item = history[ i ].num_documents;

            num_in_history++;
        }
    }

    wsd_unlock_job_history( );

    return DPWS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wprt__SetEventRate(struct dpws* ws, struct wprt__SetEventRateRequestType *request, struct wprt__SetEventRateResponseType *response )
{
    wsd_dpws_data_t *state;
    wsd_print_element_change_t *info;

    if ( ( request->EventRate->__item < 1 ) || ( request->EventRate->__item  > 600 ) )
    {
        return dpws_fault( ws, SENDER_FAULT_VALUE, "At least one input argument is invalid.", "wprt:InvalidArgs", NULL );
    }

    state = wsd_get_dpws_data( );
    if ( state != NULL )
    {
        state->event_rate = request->EventRate->__item;
        DBG_VERBOSE( "%s -- new rate: %d\n", __FUNCTION__, state->event_rate );
    }
    else
    {
        DBG_ERR( "%s - failed to get state to set event rate\n", __FUNCTION__ );
    }

    //response->dummy = NULL;

    // Send message to for Printer Change Event
    info = ( wsd_print_element_change_t * )MEM_MALLOC( sizeof( wsd_print_element_change_t ) );
    if ( info == NULL )
    {
        return dpws_fault( ws, RECEIVER_FAULT_VALUE, "The service has an unexpected error", "wprt:ServerErrorInternalError" , NULL );
    }
    info->element = WSD_PRINTER_ELEMENT_EVENT_RATE;
    info->value = request->EventRate->__item;
    info->data = NULL;
    wsd_print_send_msg( NET_WSD_PRINTER_ELEMENTS_CHANGE, ( void * )info );

    return DPWS_OK;
}


error_type_t wsd_get_printer_description( struct dpws* ws, struct wprt__PrinterElementDataType *response )
{
    char local_stack_str[256] = { 0 }, *str = local_stack_str;

    response->__union_PrinterElementDataType = SOAP_UNION__wprt__union_PrinterElementDataType_PrinterDescription;                      
    response->Name      = dpws_malloc( ws, strlen( WSD_PRINTER_DESCRIPTION ) + 1 );         
    strcpy( response->Name, WSD_PRINTER_DESCRIPTION );                                      
    response->Valid     = xsd__boolean__true_;                                              

    response->union_PrinterElementDataType.PrinterDescription = dpws_malloc( ws, sizeof( struct wprt__PrinterDescriptionType ) );

    response->union_PrinterElementDataType.PrinterDescription->ColorSupported = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    if ( NUM_MARKING_COLOR_CHANNELS > 1 )
    {
        response->union_PrinterElementDataType.PrinterDescription->ColorSupported->__item = xsd__boolean__true_;
    }
    else
    {
        response->union_PrinterElementDataType.PrinterDescription->ColorSupported->__item = xsd__boolean__false_;
    }

    platvars_get_1284str( &str );
    str += 2;  //  skip length stuff

    response->union_PrinterElementDataType.PrinterDescription->DeviceId                          = dpws_malloc( ws, sizeof( struct wprt__DeviceIdType ) );
    response->union_PrinterElementDataType.PrinterDescription->DeviceId->__item                  = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( response->union_PrinterElementDataType.PrinterDescription->DeviceId->__item, str );

    response->union_PrinterElementDataType.PrinterDescription->MultipleDocumentJobsSupported     = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    response->union_PrinterElementDataType.PrinterDescription->MultipleDocumentJobsSupported->__item = xsd__boolean__false_;
    
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinute                    = dpws_malloc( ws, sizeof( struct wprt__IntOneExtType ) );
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinute->__item            = plat_config_vals.ppm_mono;
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinute->__item            = 30;
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinuteColor               = dpws_malloc( ws, sizeof( struct wprt__IntOneExtType ) );
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinuteColor->__item       = plat_config_vals.ppm_color;
    response->union_PrinterElementDataType.PrinterDescription->PagesPerMinuteColor->__item       = 50;

    str = local_stack_str;
    platvars_get_prtnamestr( &str );

    response->union_PrinterElementDataType.PrinterDescription->PrinterName                       = dpws_malloc( ws, sizeof( struct wprt__LocalizedStringType ) );
    response->union_PrinterElementDataType.PrinterDescription->PrinterName[ 0 ].__item           = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( response->union_PrinterElementDataType.PrinterDescription->PrinterName[ 0 ].__item, str );
    response->union_PrinterElementDataType.PrinterDescription->__sizePrinterName                 = 1;    // Only support 1 language

    response->union_PrinterElementDataType.PrinterDescription->PrinterInfo                       = dpws_malloc( ws, sizeof( struct wprt__LocalizedStringType ) );
    response->union_PrinterElementDataType.PrinterDescription->PrinterInfo[ 0 ].__item           = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( response->union_PrinterElementDataType.PrinterDescription->PrinterInfo[ 0 ].__item, str );
    response->union_PrinterElementDataType.PrinterDescription->__sizePrinterInfo                 = 1;    // Only support 1 language
    
    str = local_stack_str;
    net_get_var_location(str, sizeof(local_stack_str)-1); // prefer var to oid -- oid_get_string(OID_SM_SYSTEM_LOCATION, 0, str)

    response->union_PrinterElementDataType.PrinterDescription->PrinterLocation                   = dpws_malloc( ws, sizeof( struct wprt__LocalizedStringType ) );
    response->union_PrinterElementDataType.PrinterDescription->PrinterLocation[ 0 ].__item       = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( response->union_PrinterElementDataType.PrinterDescription->PrinterLocation[ 0 ].__item, str );
    response->union_PrinterElementDataType.PrinterDescription->__sizePrinterLocation             = 1;    // Only support 1 language

    return NET_WSD_SUCCESS;
}

error_type_t wsd_get_printer_configuration( struct dpws* ws, struct wprt__PrinterElementDataType *response )
{
    struct wprt__PrinterConfigurationType *config;
    mediacnfg_t media_config;
    error_type_t err;
    uint32_t i;
    uint32_t num_carts;
    wsd_dpws_data_t *state;
    uint32_t supply_level;
    char *color;

    err = printvar_get_trayconfig( &media_config );
    XASSERT( err == OK, err );

    response->__union_PrinterElementDataType = SOAP_UNION__wprt__union_PrinterElementDataType_PrinterConfiguration;
    response->Name      = dpws_malloc( ws, strlen( WSD_PRINTER_CONFIG ) + 1 );
    strcpy( response->Name, WSD_PRINTER_CONFIG );
    response->Valid     = xsd__boolean__true_;

    err = oid_get_uint32( OID_CONSUMABLE_MODEL_NAME, COLOR_CYAN, &supply_level );
    if ( err == OK )
    {
        num_carts = 4;
    }
    else
    {
        num_carts = 1;
    }

    config = dpws_malloc( ws, sizeof( struct wprt__PrinterConfigurationType ) );
    //config->Consumables = dpws_malloc( ws, sizeof( struct wprt__ConsumablesType ) * num_carts );
    config->Consumables = dpws_malloc( ws, sizeof( struct wprt__ConsumablesType ) );
    config->Consumables->__sizeConsumableEntry  = num_carts;
    config->Consumables->ConsumableEntry = dpws_malloc( ws, sizeof( struct wprt__ConsumableEntryType ) * num_carts );

    for ( i = 0; i < num_carts; i++ )
    {
        err = oid_get_uint32( OID_CONSUMABLE_PERCENT_REMAINING, i, &supply_level );
        if ( err != OK )
        {
            DBG_ERR( "supply level oid lookup failed - using hardcoded value\n" );
            supply_level = plat_config_default_supply_level( );
        }

        if ( net_wsd_map_color( i, &color ) != OK )
        {
            color = "unknown";
        }

        //config->Consumables->ConsumableEntry[i] = dpws_malloc( ws, sizeof( struct wprt__ConsumableEntryType ) );
        wsd_populate_consumable_structure( ws, &config->Consumables->ConsumableEntry[i], color, supply_level, false );
    }

    config->Finishings = dpws_malloc( ws, sizeof( struct wprt__FinishingsType ) );
    config->Finishings->CollationSupported = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    config->Finishings->CollationSupported->__item = plat_config_vals.collation_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    config->Finishings->DuplexerInstalled  = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    config->Finishings->DuplexerInstalled->__item = plat_config_vals.duplexer_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    config->Finishings->HolePunchInstalled = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    config->Finishings->HolePunchInstalled->__item = plat_config_vals.hole_punch_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    config->Finishings->JogOffsetSupported = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    config->Finishings->JogOffsetSupported->__item = plat_config_vals.jog_offset_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    config->Finishings->StaplerInstalled   = dpws_malloc( ws, sizeof( struct wprt__BoolExtType ) );
    config->Finishings->StaplerInstalled->__item = plat_config_vals.stapler_supported ? xsd__boolean__true_ : xsd__boolean__false_;

    config->InputBins = dpws_malloc( ws, sizeof( struct wprt__InputBinsType ) );
    config->InputBins->__sizeInputBinEntry  = NUM_INPUTS;
    config->InputBins->InputBinEntry        = dpws_malloc( ws, sizeof( struct wprt__InputBinEntryType ) * NUM_INPUTS );
    for ( i = 0; i < NUM_INPUTS; i++ )
    {
        DBG_VERBOSE( "%d - Media size: %d, Media Type: %d\n", i, media_config.mediaSize[ i ], media_config.mediaType[ i ] );
        wsd_populate_input_bins_structure( ws, 
                                           &config->InputBins->InputBinEntry[ i ], 
                                           plat_config_tray_capacity( ), 
                                           plat_config_tray_level( ), 
                                           media_config.mediaSize[ i ],
                                           media_config.mediaType[ i ],
                                           i
                                           );
    }

    config->OutputBins          = NULL;
    config->PrinterEventRate    = dpws_malloc( ws, sizeof( struct wprt__EventRateType ) );
    state = wsd_get_dpws_data( );
    config->PrinterEventRate->__item = state->event_rate;

    config->Storage = NULL;
    config->Storage = dpws_malloc( ws, sizeof( struct wprt__StorageBaseType ) );
    config->Storage->__sizeStorageEntry = 1;

    config->Storage->StorageEntry = dpws_malloc( ws, sizeof( struct wprt__StorageEntryType ) * config->Storage->__sizeStorageEntry );
    config->Storage->StorageEntry[ 0 ].Free = dpws_malloc( ws, sizeof( struct wprt__LevelFreeBaseType ) );
#if 0 // revisit when implemented -- see memAPI.h
    int32_t ram_space_overall = memGetSystemMemorySize();
    config->Storage->StorageEntry[ 0 ].Free->__item = memTotalFreeMemAvailable()  / ( ram_space_overall / 100 ); // Free space as a percentage
#else
    config->Storage->StorageEntry[ 0 ].Free->__item = 98;
#endif

    config->Storage->StorageEntry[ 0 ].Name = dpws_malloc( ws, strlen( "RAM" ) + 1 );
    strcpy( config->Storage->StorageEntry[ 0 ].Name, "RAM" );

    config->Storage->StorageEntry[ 0 ].Size = dpws_malloc( ws, sizeof( struct wprt__IntOneExtType ) );
#if 0 // revisit when implemented -- see memAPI.h
    config->Storage->StorageEntry[ 0 ].Size->__item = ram_space_overall / 1024;
#else
    config->Storage->StorageEntry[ 0 ].Size->__item = 4;
#endif

    config->Storage->StorageEntry[ 0 ].Type = dpws_malloc( ws, sizeof( struct wprt__StorageTypeBaseType ) );
    config->Storage->StorageEntry[ 0 ].Type->__item = dpws_malloc( ws, strlen( "RAM" ) + 1 );
    strcpy( config->Storage->StorageEntry[ 0 ].Type->__item, "RAM" );

    config->Storage->StorageEntry[ 0 ].Removed = NULL;

    response->union_PrinterElementDataType.PrinterConfiguration = config;

    return NET_WSD_SUCCESS;
}

wsd_get_printer_status_options_t *wsd_get_status_values( uint32_t status )
{
    uint32_t i;

    for ( i = 0; i < NUM_WSD_GET_PRINTER_STATUS_VALUES; i++ )
    {
        if ( STATCATCMP( wsd_get_printer_status_values[ i ].status_category, status ) )
        {
            return &wsd_get_printer_status_values[ i ];
        }
    }
    return &wsd_status_idle;
}

error_type_t wsd_get_printer_status ( struct dpws* ws, struct wprt__PrinterElementDataType *response )
{
    uint32_t status;
    struct wprt__PrinterStatusType *printer_status;
    wsd_get_printer_status_options_t *wsd_status;
     
    response->__union_PrinterElementDataType = SOAP_UNION__wprt__union_PrinterElementDataType_PrinterStatus;
    response->Name      = dpws_malloc( ws, strlen( WSD_PRINTER_STATUS ) + 1 );
    strcpy( response->Name, WSD_PRINTER_STATUS );
    response->Valid     = xsd__boolean__true_;

    printer_status = dpws_malloc( ws, sizeof( struct wprt__PrinterStatusType ) );

    printer_status->PrinterCurrentTime = dpws_malloc( ws, sizeof( struct wprt__DateTimeExtType ) );
    printer_status->PrinterCurrentTime->__item = time(NULL);
    // Options are Idle, Processing, Stopped
    status = pec_get_current_printstatus();
    DBG_VERBOSE( "Printer Status: %#X\n", status );
/*************************  Patch Modifications *************************/    
    if(!(wsd_print_flags & WSD_PRINT_FLAG_INACTIVE_EVENT))
		wsd_status = wsd_get_status_values( status );
	else	
		wsd_status = wsd_get_status_values( 0 );
/*************************  Patch Modifications *************************/

    printer_status->PrinterState               = dpws_malloc( ws, sizeof( struct wprt__PrinterStateType ) );
    printer_status->PrinterState->__item       = dpws_malloc( ws, strlen( wsd_status->printer_state ) + 1 );
    strcpy( printer_status->PrinterState->__item, wsd_status->printer_state );
    printer_status->QueuedJobCount             = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
    printer_status->QueuedJobCount->__item     = 0;

    if ( wsd_status->printer_primary_state_reason != NULL )
    {
        printer_status->PrinterPrimaryStateReason          = dpws_malloc( ws, sizeof( struct wprt__PrinterStateReasonType ) );
        printer_status->PrinterPrimaryStateReason->__item  = dpws_malloc( ws, strlen( wsd_status->printer_primary_state_reason ) + 1 );
        strcpy( printer_status->PrinterPrimaryStateReason->__item, wsd_status->printer_primary_state_reason );
    }
    else
    {
        printer_status->PrinterPrimaryStateReason  = NULL;
    }

    if ( wsd_status->printer_state_reason != NULL )
    {
        printer_status->PrinterStateReasons         = dpws_malloc( ws, sizeof( struct wprt__PrinterStateReasonsType ) );
        printer_status->PrinterStateReasons->__sizePrinterStateReason = 1;
        printer_status->PrinterStateReasons->PrinterStateReason = dpws_malloc( ws, sizeof( struct wprt__PrinterStateReasonType ) );
        printer_status->PrinterStateReasons->PrinterStateReason[ 0 ].__item = dpws_malloc( ws, strlen( wsd_status->printer_state_reason ) + 1 );
        strcpy( printer_status->PrinterStateReasons->PrinterStateReason[ 0 ].__item, wsd_status->printer_state_reason );
    }
    else
    {
        printer_status->PrinterStateReasons        = NULL;
    }
        
    printer_status->ActiveCondition            = NULL;
    printer_status->ConditionHistory           = NULL;

    response->union_PrinterElementDataType.PrinterStatus = printer_status;

    return NET_WSD_SUCCESS;
}

error_type_t wsd_get_default_print_ticket( struct dpws* ws, struct wprt__PrinterElementDataType *response )
{
    struct wprt__PrintTicketType *ticket;

    response->__union_PrinterElementDataType = SOAP_UNION__wprt__union_PrinterElementDataType_DefaultPrintTicket;
    response->Name      = dpws_malloc( ws, strlen( WSD_DEFAULT_PRT_TICKET ) + 1 );
    strcpy( response->Name, WSD_DEFAULT_PRT_TICKET );
    response->Valid     = xsd__boolean__true_;

    ticket = dpws_malloc( ws, sizeof( struct wprt__PrintTicketType ) );
    wsd_get_print_ticket( ws, ticket, NULL );
    response->union_PrinterElementDataType.DefaultPrintTicket = ticket;

    return NET_WSD_SUCCESS;
}

error_type_t wsd_get_printer_capabilities( struct dpws* ws, struct wprt__PrinterElementDataType *response )
{
    struct wprt__PrinterCapabilitiesType *caps;
    error_type_t err;
    uint32_t i;
    uint32_t j;
    printer_setup_t printer_setup;
    char *str;

    err = printvar_get_printersetup( &printer_setup );
    XASSERT( err == OK, err );

    response->__union_PrinterElementDataType = SOAP_UNION__wprt__union_PrinterElementDataType_PrinterCapabilities;
    response->Name      = dpws_malloc( ws, strlen( WSD_PRINTER_CAPS ) + 1 );
    strcpy( response->Name, WSD_PRINTER_CAPS );
    response->Valid     = xsd__boolean__true_;

    caps = dpws_malloc( ws, sizeof( struct wprt__PrinterCapabilitiesType ) );
    caps->DocumentValues = dpws_malloc( ws, sizeof( struct wprt__DocumentValuesType ) );
    caps->DocumentValues->DocumentDescription = dpws_malloc( ws, sizeof( struct _wprt__DocumentValuesType_DocumentDescription ) );
    caps->DocumentValues->DocumentDescription->Compression = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );

    caps->DocumentValues->DocumentDescription->Compression->__sizeAllowedValue = plat_config_vals.num_compressions; 
    caps->DocumentValues->DocumentDescription->Compression->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType )  * plat_config_vals.num_compressions );
    for ( i = 0; i < plat_config_vals.num_compressions; i++ )
    {
        caps->DocumentValues->DocumentDescription->Compression->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.compressions[ i ] ) + 1 );
        strcpy( caps->DocumentValues->DocumentDescription->Compression->AllowedValue[ i ].__item, plat_config_vals.compressions[ i ] );
    }

    caps->DocumentValues->DocumentDescription->Format = dpws_malloc( ws, sizeof( struct wprt__ValueStringListType ) );
    caps->DocumentValues->DocumentDescription->Format->__sizeAllowedValue = plat_config_vals.num_formats;
    caps->DocumentValues->DocumentDescription->Format->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_formats );
    for ( i = 0; i < plat_config_vals.num_formats; i++ )
    {
        caps->DocumentValues->DocumentDescription->Format->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.formats[ i ] ) + 1 );
        strcpy( caps->DocumentValues->DocumentDescription->Format->AllowedValue[ i ].__item, plat_config_vals.formats[ i ] );
    }

    caps->JobValues = dpws_malloc( ws, sizeof( struct wprt__JobValuesType ) );
    caps->JobValues->DocumentProcessing = dpws_malloc( ws, sizeof( struct _wprt__JobValuesType_DocumentProcessing ) );

    caps->JobValues->DocumentProcessing->MediaColor = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->MediaColor->__sizeAllowedValue = plat_config_vals.num_media_colors;
    caps->JobValues->DocumentProcessing->MediaColor->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_media_colors );
    for ( i = 0; i < plat_config_vals.num_media_colors; i++ )
    {
        caps->JobValues->DocumentProcessing->MediaColor->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.media_colors[ i ] ) + 1 );
        strcpy( caps->JobValues->DocumentProcessing->MediaColor->AllowedValue[ i ].__item, plat_config_vals.media_colors[ i ] );
    }

    caps->JobValues->DocumentProcessing->MediaSizeName = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->MediaSizeName->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * paper_get_num_media_sizes() );
    j = 0;

    for ( i = 0; i < paper_get_num_media_sizes(); i++ )
    {
        if ( net_wsd_map_mediasize( paper_get_entry( i ), &str ) == NET_WSD_SUCCESS )
        {
            caps->JobValues->DocumentProcessing->MediaSizeName->AllowedValue[ j ].__item = dpws_malloc( ws, strlen( str ) + 1 );
            strcpy( caps->JobValues->DocumentProcessing->MediaSizeName->AllowedValue[ j ].__item, str );
            j++;
        }
    }
   
    caps->JobValues->DocumentProcessing->MediaSizeName->__sizeAllowedValue = j;

    caps->JobValues->DocumentProcessing->MediaType = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->MediaType->__sizeAllowedValue = 1;
    caps->JobValues->DocumentProcessing->MediaType->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * paper_get_num_media_types() );
    j = 0;
    for ( i = 0; i < paper_get_num_media_types(); i++ )
    {
        if ( net_wsd_map_mediatype( papertypes_get_entry( i ), &str ) == NET_WSD_SUCCESS )
        {
            caps->JobValues->DocumentProcessing->MediaType->AllowedValue[ j ].__item = dpws_malloc( ws, strlen( str ) + 1 );
            strcpy( caps->JobValues->DocumentProcessing->MediaType->AllowedValue[ j ].__item, str );
            j++;
        }
    }
    caps->JobValues->DocumentProcessing->MediaType->__sizeAllowedValue = j;


    caps->JobValues->DocumentProcessing->NumberUp = dpws_malloc( ws, sizeof( struct _wprt__JobValuesType_DocumentProcessing_NumberUp ) );
    caps->JobValues->DocumentProcessing->NumberUp->Direction = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->NumberUp->Direction->__sizeAllowedValue = plat_config_vals.num_directions;
    caps->JobValues->DocumentProcessing->NumberUp->Direction->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_directions);
    for ( i = 0; i < plat_config_vals.num_directions; i++ )
    {
        caps->JobValues->DocumentProcessing->NumberUp->Direction->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.directions[ i ] ) + 1 );
        strcpy( caps->JobValues->DocumentProcessing->NumberUp->Direction->AllowedValue[ i ].__item, plat_config_vals.directions[ i ] );
    }

    caps->JobValues->DocumentProcessing->NumberUp->PagesPerSheet = dpws_malloc( ws, sizeof( struct wprt__ValueIntListType ) );
    caps->JobValues->DocumentProcessing->NumberUp->PagesPerSheet->__sizeAllowedValue = plat_config_vals.num_pages_per_sheet;
    caps->JobValues->DocumentProcessing->NumberUp->PagesPerSheet->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) * plat_config_vals.num_pages_per_sheet );
    for ( i = 0; i < plat_config_vals.num_pages_per_sheet; i++ )
    {
        caps->JobValues->DocumentProcessing->NumberUp->PagesPerSheet->AllowedValue[ i ].__item = plat_config_vals.pages_per_sheet[ i ];
    }

    caps->JobValues->DocumentProcessing->Orientation = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->Orientation->__sizeAllowedValue = plat_config_vals.num_orientations;
    caps->JobValues->DocumentProcessing->Orientation->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_orientations );
    for ( i = 0; i < plat_config_vals.num_orientations; i++ )
    {
        caps->JobValues->DocumentProcessing->Orientation->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.orientations[ i ] ) + 1 );
        strcpy( caps->JobValues->DocumentProcessing->Orientation->AllowedValue[ i ].__item, plat_config_vals.orientations[ i ] );
    }

    caps->JobValues->DocumentProcessing->PrintQuality = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->PrintQuality->__sizeAllowedValue = plat_config_vals.num_print_quality;
    caps->JobValues->DocumentProcessing->PrintQuality->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_print_quality );
    for ( i = 0; i < plat_config_vals.num_print_quality; i++ )
    {
        caps->JobValues->DocumentProcessing->PrintQuality->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.print_quality[ i ] ) + 1 );
        strcpy( caps->JobValues->DocumentProcessing->PrintQuality->AllowedValue[ i ].__item, plat_config_vals.print_quality[ i ] );
    }

    caps->JobValues->DocumentProcessing->Resolution = dpws_malloc( ws, sizeof( struct _wprt__JobValuesType_DocumentProcessing_Resolution ) );
    caps->JobValues->DocumentProcessing->Resolution->__sizeAllowedValue = plat_config_vals.num_resolutions;
    caps->JobValues->DocumentProcessing->Resolution->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__ResolutionEntryType ) * plat_config_vals.num_resolutions );
    for ( i = 0; i < plat_config_vals.num_resolutions; i++ )
    {
        caps->JobValues->DocumentProcessing->Resolution->AllowedValue[ i ].Height = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
        caps->JobValues->DocumentProcessing->Resolution->AllowedValue[ i ].Height->__item = plat_config_vals.resolutions[ i ].height;
        caps->JobValues->DocumentProcessing->Resolution->AllowedValue[ i ].Width = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
        caps->JobValues->DocumentProcessing->Resolution->AllowedValue[ i ].Width->__item = plat_config_vals.resolutions[ i ].width;
    }

    caps->JobValues->DocumentProcessing->Sides = dpws_malloc( ws, sizeof( struct wprt__ValueTokenListType ) );
    caps->JobValues->DocumentProcessing->Sides->__sizeAllowedValue = plat_config_vals.num_sides;
    caps->JobValues->DocumentProcessing->Sides->AllowedValue = dpws_malloc( ws, sizeof( struct wprt__NMTOKENSExtType ) * plat_config_vals.num_sides );
    for ( i = 0; i < plat_config_vals.num_sides; i++ )
    {
        caps->JobValues->DocumentProcessing->Sides->AllowedValue[ i ].__item = dpws_malloc( ws, strlen( plat_config_vals.sides[ i ] ) + 1 );
        strcpy( caps->JobValues->DocumentProcessing->Sides->AllowedValue[ i ].__item, plat_config_vals.sides[ i ] );
    }

    caps->JobValues->JobProcessing = dpws_malloc( ws, sizeof( struct _wprt__JobValuesType_JobProcessing ) );
    caps->JobValues->JobProcessing->Copies = dpws_malloc( ws, sizeof( struct wprt__ValueIntRangeType ) ); 
    caps->JobValues->JobProcessing->Copies->MaxValue = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    caps->JobValues->JobProcessing->Copies->MaxValue->__item = plat_config_vals.max_copies;
    caps->JobValues->JobProcessing->Copies->MinValue = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    caps->JobValues->JobProcessing->Copies->MinValue->__item = plat_config_vals.min_copies;

    caps->JobValues->JobProcessing->JobFinishings = NULL;

    caps->JobValues->JobProcessing->Priority = dpws_malloc( ws, sizeof( struct wprt__ValueIntRangeType ) );
    caps->JobValues->JobProcessing->Priority->MaxValue = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    caps->JobValues->JobProcessing->Priority->MaxValue->__item = plat_config_vals.max_priority;
    caps->JobValues->JobProcessing->Priority->MinValue = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    caps->JobValues->JobProcessing->Priority->MinValue->__item = plat_config_vals.min_priority;

    response->union_PrinterElementDataType.PrinterCapabilities = caps;

    return NET_WSD_SUCCESS;
}

void wsd_populate_input_bins_structure( struct dpws* ws, struct wprt__InputBinEntryType *bin, uint32_t capacity, uint32_t level, mediasize_t size, mediatype_t type, uint32_t index )
{
    char local_stack_str[64], *str = local_stack_str;
    
    bin->Capacity         = dpws_malloc( ws, sizeof( struct wprt__CapacityBaseType ) );
    bin->Capacity->__item = capacity;
    bin->FeedDirection    = dpws_malloc( ws, sizeof( struct wprt__InputBinFeedDirectionType ) );
    bin->FeedDirection->__item = plat_config_vals.feed_direction;
    bin->Level            = dpws_malloc( ws, sizeof( struct wprt__LevelFreeBaseType ) );
    bin->Level->__item    = level;
    bin->MediaColor       = NULL;

    if ( net_wsd_map_mediasize( size, &str ) != NET_WSD_SUCCESS )
    {
        DBG_ERR( "Failed to map mediasize %d to string\n", size );
        // str = "any"; 
        oid_get_string(OID_SM_PRINT_MEDIA_DEFAULT, 0, str, sizeof(local_stack_str)-1); // str = "na_letter_8.5x11in";
    }
    bin->MediaSize        = dpws_malloc( ws, sizeof( struct wprt__InputBinMediaSizeType ) );
    bin->MediaSize->__item = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( bin->MediaSize->__item, str );

    if ( net_wsd_map_mediatype( type, &str ) != NET_WSD_SUCCESS )
    {
        DBG_ERR( "Failed to map mediatype %d to string\n", type );
        // str = "anySupportedType";
        oid_get_string(OID_SM_PRINT_MEDIA_TYPE_DEFAULT, 0, str, sizeof(local_stack_str)-1); // str = "plain";
    }
    bin->MediaType        = dpws_malloc( ws, sizeof( struct wprt__InputBinMediaTypeBaseType ) );
    bin->MediaType->__item = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( bin->MediaType->__item, str );
    bin->Name             = dpws_malloc( ws, strlen( TRAY_NAME ) + 2 ); // 1 for null 1 for tray index
    minSprintf( bin->Name, "%s%d", TRAY_NAME, index + 1 );
    bin->Removed = dpws_malloc( ws, sizeof( enum xsd__boolean ) );
    *bin->Removed = xsd__boolean__false_;

}

void wsd_populate_consumable_structure( struct dpws* ws, struct wprt__ConsumableEntryType *consumable, char *color, uint32_t level, bool event )
{
    consumable->Color = dpws_malloc( ws, sizeof( struct wprt__ConsumableColorType ) );
    consumable->Color->__item = dpws_malloc( ws, strlen( color ) + 1 );
    strcpy( consumable->Color->__item, color );
    consumable->Level = dpws_malloc( ws, sizeof( struct wprt__LevelFreeBaseType ) );
    consumable->Level->__item = level;
    consumable->Model = NULL;

    consumable->Name = dpws_malloc( ws, strlen( color ) + strlen( SUPPLY ) + 1 ); // 1 for NULL
    ASSERT( consumable->Name != NULL );
    minSprintf( consumable->Name, "%s%s", color, SUPPLY );

    consumable->Type  = dpws_malloc( ws, sizeof( struct wprt__ConsumableTypeBaseType ) );
    consumable->Type->__item = dpws_malloc( ws, strlen( plat_config_supply_type( ) ) + 1 );
    strcpy( consumable->Type->__item, plat_config_supply_type( ) );
    if ( event == true )
    {
        consumable->Removed = dpws_malloc( ws, sizeof( enum xsd__boolean ) );
        *consumable->Removed = xsd__boolean__true_;
    }
    else
    {
        consumable->Removed = NULL;
    }

}

void wsd_get_print_job_status( struct dpws *ws, struct wprt__JobStatusType *status, wsd_job_info_t *job_info )
{

    status->JobId = dpws_malloc( ws, sizeof( struct wprt__JobIdType ) );
    status->JobId->__item = job_info->id;

    status->JobState = dpws_malloc( ws, sizeof( struct wprt__JobStateType ) );
    status->JobState->__item = dpws_malloc( ws, strlen( wsd_get_job_state_string( job_info->state ) ) + 1 );
    strcpy( status->JobState->__item, wsd_get_job_state_string( job_info->state ) );

    status->JobStateReasons = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonsType ) );
    status->JobStateReasons->__sizeJobStateReason = 1;
    status->JobStateReasons->JobStateReason = dpws_malloc( ws, sizeof( struct wprt__JobStateReasonType ) );
    status->JobStateReasons->JobStateReason[ 0 ].__item = dpws_malloc( ws, strlen( wsd_job_state_reason_str( job_info->reasons ) ) + 1 );
    strcpy( status->JobStateReasons->JobStateReason[ 0 ].__item, wsd_job_state_reason_str( job_info->reasons ) );

    status->KOctetsProcessed = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
    status->KOctetsProcessed->__item = job_info->koctets_processed;

    status->MediaSheetsCompleted = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
    status->MediaSheetsCompleted->__item = job_info->sheets_completed;

    status->NumberOfDocuments = dpws_malloc( ws, sizeof( struct wprt__IntZeroExtType ) );
    status->NumberOfDocuments->__item = job_info->num_documents;

}

void wsd_get_print_ticket( struct dpws *ws, struct wprt__PrintTicketType *ticket, wsd_job_info_t *job_info )
{
    ticket->DocumentProcessing  = dpws_malloc( ws, sizeof( struct wprt__DocumentProcessingType ) );
    wsd_get_document_processing( ws, ticket->DocumentProcessing );

    ticket->JobDescription      = dpws_malloc( ws, sizeof( struct wprt__JobDescriptionType ) );
    ticket->JobDescription->JobName = dpws_malloc( ws, sizeof( struct wprt__JobNameType ) );
    if ( job_info == NULL )
    {
        ticket->JobDescription->JobName->__item = dpws_malloc( ws, strlen( DEFAULT_JOB_NAME ) + 1 );
        strcpy( ticket->JobDescription->JobName->__item, DEFAULT_JOB_NAME );
    }
    else
    {
        ticket->JobDescription->JobName->__item = dpws_malloc( ws, strlen( job_info->name ) + 1 );
        strcpy( ticket->JobDescription->JobName->__item, job_info->name );
    }

    ticket->JobDescription->JobOriginatingUserName = dpws_malloc( ws, sizeof( struct wprt__JobOriginatingUserNameType ) );
    if ( job_info == NULL )
    {
        ticket->JobDescription->JobOriginatingUserName->__item = dpws_malloc( ws, strlen( UNKNOWN ) + 1 );
        strcpy( ticket->JobDescription->JobOriginatingUserName->__item, UNKNOWN );
    }
    else
    {
        ticket->JobDescription->JobOriginatingUserName->__item = dpws_malloc( ws, strlen( job_info->user_name ) + 1 );
        strcpy( ticket->JobDescription->JobOriginatingUserName->__item, job_info->user_name );
    }


    ticket->JobProcessing       = dpws_malloc( ws, sizeof( struct wprt__JobProcessingType ) );
    ticket->JobProcessing->Copies = dpws_malloc( ws, sizeof( struct wprt__JobCopiesType ) );
    ticket->JobProcessing->Copies->__item = plat_config_vals.min_copies;
    ticket->JobProcessing->Copies->MustHonor = NULL;

    ticket->JobProcessing->JobFinishings = dpws_malloc( ws, sizeof( struct wprt__JobFinishingsType ) );
    ticket->JobProcessing->JobFinishings->Collate = dpws_malloc( ws, sizeof( struct wprt__BooleanExtType ) );
    ticket->JobProcessing->JobFinishings->Collate->__item = plat_config_vals.collation_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    ticket->JobProcessing->JobFinishings->Collate->MustHonor = NULL;

    ticket->JobProcessing->JobFinishings->HolePunch = NULL;

    ticket->JobProcessing->JobFinishings->JogOffset = dpws_malloc( ws, sizeof( struct wprt__BooleanExtType ) );
    ticket->JobProcessing->JobFinishings->JogOffset->__item = plat_config_vals.jog_offset_supported ? xsd__boolean__true_ : xsd__boolean__false_;
    ticket->JobProcessing->JobFinishings->JogOffset->MustHonor = NULL;

    ticket->JobProcessing->JobFinishings->Staple = NULL;

    ticket->JobProcessing->Priority = dpws_malloc( ws, sizeof( struct wprt__JobPriorityType ) );
    ticket->JobProcessing->Priority->__item = ( plat_config_vals.max_priority  - plat_config_vals.min_priority ) / 2;
    ticket->JobProcessing->Priority->MustHonor = NULL;

}

void wsd_get_documents( struct dpws *ws, struct wprt__DocumentsType *doc )
{

    doc->__sizeDocument = 1;
    doc->Document = dpws_malloc( ws, sizeof( struct wprt__DocumentType ) );
    doc->Document[ 0 ].DocumentDescription = dpws_malloc( ws, sizeof( struct wprt__DocumentDescriptionType ) );
    doc->Document[ 0 ].DocumentDescription->Compression = dpws_malloc( ws, sizeof( struct wprt__CompressionType ) );
    doc->Document[ 0 ].DocumentDescription->Compression->__item = dpws_malloc( ws, strlen( plat_config_vals.compressions[ 0 ] ) + 1 );
    strcpy( doc->Document[ 0 ].DocumentDescription->Compression->__item, plat_config_vals.compressions[ 0 ] );
    doc->Document[ 0 ].DocumentDescription->Compression->MustHonor = NULL;

    doc->Document[ 0 ].DocumentDescription->DocumentId = 1;

    doc->Document[ 0 ].DocumentDescription->DocumentName = NULL;
    doc->Document[ 0 ].DocumentDescription->DocumentUrl = NULL;

    doc->Document[ 0 ].DocumentDescription->Format = dpws_malloc( ws, sizeof( struct wprt__DocumentFormatType ) );
    doc->Document[ 0 ].DocumentDescription->Format->__item = dpws_malloc( ws, strlen( plat_config_vals.formats[ 0 ] ) + 1 );
    strcpy( doc->Document[ 0 ].DocumentDescription->Format->__item, plat_config_vals.formats[ 0 ] );
    doc->Document[ 0 ].DocumentDescription->Format->MustHonor = NULL;

    doc->Document[ 0 ].DocumentProcessing = dpws_malloc( ws, sizeof( struct wprt__DocumentProcessingType ) );
    wsd_get_document_processing( ws, doc->Document[ 0 ].DocumentProcessing );

   
}

void wsd_get_document_processing( struct dpws *ws, struct wprt__DocumentProcessingType *doc )
{
    error_type_t err;
    mediacnfg_t media_config;
    char *str;

    err = printvar_get_trayconfig( &media_config );
    XASSERT( err == OK, err );


    doc->MediaColor = dpws_malloc( ws, sizeof( struct wprt__MediaColorType ) );
    doc->MediaColor->__item = dpws_malloc( ws, strlen( plat_config_vals.media_colors[ 0 ] ) + 1 );
    strcpy( doc->MediaColor->__item, plat_config_vals.media_colors[ 0 ] );
    doc->MediaColor->MustHonor = NULL;

    doc->MediaSizeName = dpws_malloc( ws, sizeof( struct wprt__MediaSizeNameType ) );
    if ( net_wsd_map_mediasize( media_config.DefmediaSize, &str ) != NET_WSD_SUCCESS ) ASSERT( 0 );
    doc->MediaSizeName->__item = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( doc->MediaSizeName->__item, str );
    doc->MediaSizeName->MustHonor = NULL;

    doc->MediaType = dpws_malloc( ws, sizeof( struct wprt__MediaTypeBaseType ) );
    if ( net_wsd_map_mediatype( media_config.DefmediaType, &str ) != NET_WSD_SUCCESS ) ASSERT( 0 );
    doc->MediaType->__item = dpws_malloc( ws, strlen( str ) + 1 );
    strcpy( doc->MediaType->__item, str );
    doc->MediaType->MustHonor = NULL;

    doc->NumberUp = dpws_malloc( ws, sizeof( struct wprt__NumberUpType ) );
    doc->NumberUp->Direction = dpws_malloc( ws, sizeof( struct wprt__NUpDirectionType ) );
    doc->NumberUp->Direction->__item = plat_config_vals.nup_direction[ 0 ];
    doc->NumberUp->Direction->MustHonor = NULL;

    doc->NumberUp->PagesPerSheet = dpws_malloc( ws, sizeof( struct wprt__NUpPagesPerSheetType ) );
    doc->NumberUp->PagesPerSheet->__item = plat_config_vals.pages_per_sheet[ 0 ];
    doc->NumberUp->PagesPerSheet->MustHonor = NULL;

    doc->Orientation = dpws_malloc( ws, sizeof( struct wprt__OrientationType ) );
    doc->Orientation->__item = dpws_malloc( ws, strlen( plat_config_vals.orientations[ 0 ] ) + 1 );
    strcpy( doc->Orientation->__item, plat_config_vals.orientations[ 0 ] );
    doc->Orientation->MustHonor = NULL;

    doc->PrintQuality = dpws_malloc( ws, sizeof( struct wprt__PrintQualityType ) );
    doc->PrintQuality->__item = dpws_malloc( ws, strlen( plat_config_vals.print_quality[ 0 ] ) + 1 );
    strcpy( doc->PrintQuality->__item, plat_config_vals.print_quality[ 0 ] );
    doc->PrintQuality->MustHonor = NULL;

    doc->Resolution = dpws_malloc( ws, sizeof( struct wprt__ResolutionType ) );
    doc->Resolution->Height = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    doc->Resolution->Height->__item = plat_config_vals.resolutions[ 0 ].height;
    doc->Resolution->Width = dpws_malloc( ws, sizeof( struct wprt__IntExtType ) );
    doc->Resolution->Width->__item = plat_config_vals.resolutions[ 0 ].width;
    doc->Resolution->MustHonor = NULL;

    doc->Sides = dpws_malloc( ws, sizeof( struct wprt__SidesType ) );
    doc->Sides->__item = dpws_malloc( ws, strlen( plat_config_vals.sides[ 0 ] ) + 1 );
    strcpy( doc->Sides->__item, plat_config_vals.sides[ 0 ] );
    doc->Sides->MustHonor = NULL;

}

#if 0
static void wsd_print_job_timer_callback( ULONG param )
{
    wsd_job_info_t *job_info;


    wsd_lock_job_history( );

    job_info = wsd_get_job_info( param );
    if ( job_info != NULL )
    {
        DBG_ERR( "%s - Aborting job\n", __FUNCTION__ );

        job_info->state     = WSD_JOB_STATE_ABORTED;
        job_info->reasons   = WSD_JOB_STATE_REASON_DOCUMENT_TIMEOUT_ERROR;
        job_info->active    = false;

        wsd_print_send_msg_no_wait( NET_WSD_JOB_END_STATE, ( void * )param );
    }
    else
    {
        DBG_ERR( "Job info is null\n" );
    }
    wsd_unlock_job_history( );
}
#endif

static int32_t wsd_print_io_recv_func( char *buf, uint32_t len, uint32_t *timeout_msec, void *user_ctxt )
{
    uint32_t start_time;
    uint32_t cur_time;
    ATLISTENTRY* list_node;
    wsd_job_data_node_t* data_node;
    uint32_t bytes_available;
    
    //start_time = tx_time_get( );
    start_time = posix_gettime_ticks();
    
    wsd_job_info_t *job_info = wsd_get_job_info( ( uint32_t ) user_ctxt );
    int bytes_read = 0;

    if ( job_info == NULL )
    {
        DBG_ERR( "%s - failed to load job info\n", __FUNCTION__ );
        return -1;
    }

    while ( bytes_read < len )
    {
        list_node = ATListHead(&job_info->job_data_list);
        if ( list_node == NULL)
        {
            //cur_time = tx_time_get( );
            cur_time = posix_gettime_ticks();

            if ( ( ( cur_time - start_time ) >= ( *timeout_msec / ( MILLISECONDS_IN_SECOND / TIMER_INTS_PER_SECOND ) ) ) || 
                 ( bytes_read != 0 ) )
            {
                break;
            }
            else
            {
                //tx_thread_sleep( SYS_TICK_FREQ / 5 );
                posix_sleep_ticks( SYS_TICK_FREQ / 5 );
                continue;
            }
        }
        data_node = CONTAINING_RECORD(list_node, wsd_job_data_node_t, node);
        bytes_available = data_node->buflen - data_node->bufoffset;
        if ( ( len - bytes_read ) >= bytes_available)
        {
            // Copy all of current link;
            memcpy( buf + bytes_read, data_node->buf + data_node->bufoffset, bytes_available );
            bytes_read += bytes_available;
            job_info->job_data_bytes_buffered -= bytes_available;
            ATRemoveHeadList(&job_info->job_data_list);
            MEM_FREE_AND_NULL( data_node );
        }
        else 
        {
            uint32_t bytes_to_read = len - bytes_read;
            memcpy( buf + bytes_read, data_node->buf + data_node->bufoffset, bytes_to_read );
            bytes_read += bytes_to_read;
            data_node->bufoffset += bytes_to_read;
            job_info->job_data_bytes_buffered -= bytes_to_read;
        }
    }
    if ( job_info->job_data_bytes_buffered <= wsd_print_config_vals.signal_to_recv_threshold )
    {
        DBG_VERBOSE( "Data dropped below high threshold - notify to recieve more data: %d\n", job_info->job_data_bytes_buffered );
        wsd_print_set_flags( WSD_PRINT_FLAG_FILL_BUFFER );
    }

    if ( ( bytes_read == 0 ) && ( job_info->job_data_recv_done == false ) )
    {
        DBG_ERR( "No data ready - timeout was: %d\n", *timeout_msec );
        bytes_read = -1;
        *timeout_msec = 0;
    }

    return bytes_read;
}

static void wsd_print_io_notify_func( net_io_session_event_t event, void *user_ctxt )
{
    char *buf;
    uint32_t job_id;
    wsd_job_info_t *job_info;
    uint32_t len;
    
    job_id = (uint32_t) user_ctxt;
    wsd_lock_job_history( );
    job_info = wsd_get_job_info( job_id );
    if ( job_info == NULL )
    {
        wsd_unlock_job_history( );
        DBG_ERR( "%s - No Job Info\n", __FUNCTION__ );
        return;
    }

    switch(event)
    {
        case NET_IO_SESSION_EVENT_INACTIVE:
            DBG_VERBOSE( "%s - NET_IO_SESSION_EVENT_INACTIVE - job id: %d\n", __FUNCTION__, job_info->id );
            DBG_VERBOSE( "job_info->io_ready_sent = %d - job_info->job_data_bytes_buffered: %d\n", job_info->io_ready_sent, job_info->job_data_bytes_buffered );
            if ( job_info->io_ready_sent == true )
            {			
				job_info->state = WSD_JOB_STATE_COMPLETED;
				job_info->reasons = WSD_JOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY;	
				DBG_VERBOSE( "Send message: NET_WSD_JOB_END_STATE\n");
				wsd_print_send_msg_no_wait( NET_WSD_JOB_END_STATE, ( void * )job_info->id );				
				job_info->active = false;				
				wsd_print_set_flags( WSD_PRINT_FLAG_INACTIVE_EVENT);
				DBG_VERBOSE( "Set net_io_ready!!!\n");
				net_io_ready( job_info->io_session_hndl );
                DBG_VERBOSE( "Closing io connection - marking job as done\n" );
                net_io_close( job_info->io_session_hndl );
                job_info->io_session_hndl = NULL;
            }
            else if( job_info->io_ready_sent == false )
            {
                // Notify job that it can start to receive data
                DBG_VERBOSE( "Got inactive event - notify start to receive data\n" );
                wsd_print_set_flags( WSD_PRINT_FLAG_INACTIVE_EVENT );
            }
            break;

        case NET_IO_SESSION_EVENT_ACTIVE:
            DBG_VERBOSE( "%s - Event active. Setting state to Processing!!!\n", __FUNCTION__ );
            job_info->state = WSD_JOB_STATE_PROCESSING;
            job_info->reasons = WSD_JOB_STATE_REASON_JOB_PRINTING;
            job_info->num_documents = 1;
            wsd_print_send_msg( NET_WSD_JOB_STATUS, ( void * )job_info->id );
            break;

        case NET_IO_SESSION_EVENT_HAVE_SEND_DATA:
            DBG_VERBOSE( "%s - Event have send data\n", __FUNCTION__ );
            while ( 1 )
            {
                buf = net_io_get_send_data( job_info->io_session_hndl, &len );
                if ( buf == NULL )
                {
                    break;
                }
                MEM_FREE_AND_NULL( buf );
            }
            
            break;

        default:
            // unhandled event
            XASSERT(0, event);
            break;
    }
    wsd_unlock_job_history( );

    return;
}

void wsd_job_event_notify_func( net_io_job_event_t event, net_io_job_hndl_t job_hndl, void *user_ctxt )
{
    uint32_t job_id;
    wsd_job_info_t *job_info;
    bool job_end = false;

    wsd_lock_job_history( );
    job_id = (uint32_t) user_ctxt;
    job_info = wsd_get_job_info( job_id );
    if ( job_info == NULL )
    {
        wsd_unlock_job_history( );
        DBG_ERR( "%s - No Job Info\n", __FUNCTION__ );
        return;
    }

    DBG_VERBOSE( "Enter %s - event: %d for job id: %d\n", __FUNCTION__, event, job_id );
    job_info->koctets_processed = net_io_job_get_koctets_processed( job_hndl );
    job_info->sheets_completed = net_io_job_get_sheets_completed( job_hndl );
    
    switch ( event )
    {
        case NET_IO_JOB_EVENT_STARTED:
            job_info->job_hndl = job_hndl;
            break;
        case NET_IO_JOB_EVENT_PAUSED:
            job_info->state = WSD_JOB_STATE_PROCESSING_STOPPED;
            break;
        case NET_IO_JOB_EVENT_COMPLETED:
            job_info->state = WSD_JOB_STATE_COMPLETED;
            job_info->reasons = WSD_JOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY;
            job_end = true;
            break;
        case NET_IO_JOB_EVENT_ABORTED:
            job_info->state = WSD_JOB_STATE_ABORTED;
            switch( net_io_job_get_state_reason( job_hndl ) )
            {
                case NET_IO_JOB_STATE_REASON_NONE:
                    break;
                case NET_IO_JOB_STATE_REASON_UNSUPPORTED_PDL:
                    job_info->reasons = WSD_JOB_STATE_REASON_UNSUPPORTED_DOCUMENT_FORMAT;
                    break;
                case NET_IO_JOB_STATE_REASON_PARSER_ERROR:
                    job_info->reasons = WSD_JOB_STATE_REASON_DOCUMENT_FORMAT_ERROR;
                    break;
            }
            job_end = true;
            break;
        case NET_IO_JOB_EVENT_CANCELED:
            job_info->state = WSD_JOB_STATE_CANCELED;
            if ( job_info->reasons != WSD_JOB_STATE_REASON_JOB_CANCELED_BY_USER )
            {
                // Know this job has been canceled on device
                job_info->reasons = WSD_JOB_STATE_REASON_JOB_CANCELED_AT_DEVICE;
            }
            job_end = true;
            break;
        case NET_IO_JOB_EVENT_SHEET_COMPLETED:
            break;
        default:
            XASSERT( 0, event );
            break;
    }

    wsd_print_send_msg_no_wait( NET_WSD_JOB_STATUS, ( void * )job_id );
    
    if ( job_end == true )
    {
        job_info->active = false;
        wsd_print_send_msg_no_wait( NET_WSD_JOB_END_STATE, ( void * )job_id );
        
    }    
    wsd_unlock_job_history( );

}

static bool wsd_validate_print_ticket( struct wprt__PrintTicketType *ticket, struct dpws *ws, char **detail )
{
    bool found;
    uint32_t i;
    
    if ( ticket->DocumentProcessing != NULL )
    {
        if ( ( ticket->DocumentProcessing->Sides != NULL ) &&
             ( ticket->DocumentProcessing->Sides->MustHonor != NULL ) && 
             ( *ticket->DocumentProcessing->Sides->MustHonor == true ) )
        {
            found = false;
            for ( i = 0; i < plat_config_vals.num_sides; i++ )
            {
                if ( strcmp( ticket->DocumentProcessing->Sides->__item, plat_config_vals.sides[ i ] ) == 0 )
                {
                    found = true;
                    break;
                }
            }
            if ( found == false )
            {
                *detail = dpws_malloc( ws, strlen( "Sides" ) + 1 );
                strcpy( *detail, "Sides" );
                return false;
            }
        }

        /*
        if ( ( ticket->DocumentProcessing->MediaSizeName != NULL ) &&
             ( *ticket->DocumentProcessing->MediaSizeName->MustHonor == true ) && 
             ( strcmp( ticket->DocumentProcessing->MediaSizeName->__item, ONE_SIDED ) != 0 ) )
        {
            *detail = dpws_malloc( ws, strlen( "MediaSizeName" ) + 1 );
            strcpy( *detail, "MediaSizeName" );
            return false;
        }

        ticket->DocumentProcessing->MediaType->MustHonor;
        if ( ( ticket->DocumentProcessing->Sides != NULL ) &&
             ( *ticket->DocumentProcessing->Sides->MustHonor == true ) && 
             ( strcmp( ticket->DocumentProcessing->Sides->__item, ONE_SIDED ) != 0 ) )
        {
            *detail = dpws_malloc( ws, strlen( "MediaType" ) + 1 );
            strcpy( *detail, "MediaType" );
            return false;
        }
        */

        if ( ( ticket->DocumentProcessing->NumberUp != NULL ) &&
             ( ticket->DocumentProcessing->NumberUp->Direction->MustHonor != NULL ) && 
             ( *ticket->DocumentProcessing->NumberUp->Direction->MustHonor == true ) )
        {
            found = false;
            for ( i = 0; i < plat_config_vals.num_directions; i++ )
            {
                if ((int)ticket->DocumentProcessing->NumberUp->Direction->__item == (int)plat_config_vals.nup_direction[ i ])
                {
                    found = true;
                    break;
                }
            }
            if ( found == false )
            {
                *detail = dpws_malloc( ws, strlen( "Direction" ) + 1 );
                strcpy( *detail, "Direction" );
                return false;
            }
        }

        if ( ( ticket->DocumentProcessing->NumberUp != NULL ) &&
             ( ticket->DocumentProcessing->NumberUp->PagesPerSheet->MustHonor != NULL ) && 
             ( *ticket->DocumentProcessing->NumberUp->PagesPerSheet->MustHonor == true ) )
        {
            found = false;
            for ( i = 0; i < plat_config_vals.num_pages_per_sheet; i++ )
            {
                if ( ticket->DocumentProcessing->NumberUp->PagesPerSheet->__item == plat_config_vals.pages_per_sheet[ i ] )
                {
                    found = true;
                    break;
                }
            }
            if ( found == false )
            {
                *detail = dpws_malloc( ws, strlen( "PagesPerSheet" ) + 1 );
                strcpy( *detail, "PagesPerSheet" );
                return false;
            }
        }

        if ( ( ticket->DocumentProcessing->PrintQuality != NULL ) &&
             ( ticket->DocumentProcessing->PrintQuality->MustHonor != NULL ) && 
             ( *ticket->DocumentProcessing->PrintQuality->MustHonor == true ) )
        {
            found = false;
            for ( i = 0; i < plat_config_vals.num_print_quality; i++ )
            {
                if( strcmp( ticket->DocumentProcessing->PrintQuality->__item, plat_config_vals.print_quality[ i ] ) == 0 )
                {
                    found = true;
                    break;
                }
            }
            if ( found == false )
            {
                *detail = dpws_malloc( ws, strlen( "PrintQuality" ) + 1 );
                strcpy( *detail, "PrintQuality" );
                return false;
            }
        }

        if ( ( ticket->DocumentProcessing->Resolution != NULL ) &&
             ( ticket->DocumentProcessing->Resolution->MustHonor != NULL ) && 
             ( *ticket->DocumentProcessing->Resolution->MustHonor == true ) )
        {
            found = false;
            for ( i = 0; i < plat_config_vals.num_resolutions; i++ )
            {
                if ( ( ticket->DocumentProcessing->Resolution->Height->__item == plat_config_vals.resolutions[ i ].height ) &&
                     ( ticket->DocumentProcessing->Resolution->Width->__item  == plat_config_vals.resolutions[ i ].width ) )
                {
                    found = true;
                    break;
                }
            }
            if ( found == false )
            {
                *detail = dpws_malloc( ws, strlen( "Resolution" ) + 1 );
                strcpy( *detail, "Resolution" );
                return false;
            }
        }
    }

    return true;
}

void *wsd_print_mime_write_open( struct soap *soap, void *handle, const char *id, const char *type, const char *description, enum soap_mime_encoding encoding )
{
    uint32_t job_id;
    uint32_t i;
    wsd_job_info_t *history;
    net_wsd_config_t *config;

    //DBG_ERR( "%s - handle: %#X, id: %s, type: %s, description: %s, encoding: %d\n", __FUNCTION__, handle, id, type, description, encoding );
    //XASSERT( encoding == SOAP_MIME_BINARY, encoding );

    wsd_lock_job_history( );
    history = wsd_get_job_history( );   

    config = wsd_get_config( );

    for ( i = 0; i < config->num_job_history_elements; i++ )
    {
        if ( ( history[ i ].id != 0 ) &&
             ( history[ i ].active == true ) &&
             ( history[ i ].protocol == WSD_PRINT ) )
        {
            break;
        }
    }
    if ( i == config->num_job_history_elements )
    {
        job_id = 0;
    }
    else
    {
        job_id = history[ i ].id;
    }    
    wsd_unlock_job_history( );

    ATInitList( &history[ i ].job_data_list );

    return (void*)job_id;
}

int wsd_print_mime_write( struct soap *soap, void *handle, const char *buf, size_t len )
{
    wsd_job_info_t *job_info;
    wsd_job_data_node_t* data_node;
    ULONG requested_flags;
    ULONG actual_flags;

    requested_flags = WSD_PRINT_FLAG_FILL_BUFFER;
    //tx_event_flags_get( &wsd_print_flags, requested_flags, TX_AND, &actual_flags, TX_WAIT_FOREVER);
    pthread_mutex_lock(&wsd_print_mutex);
    while(!(wsd_print_flags & requested_flags))
    {
        pthread_cond_wait(&wsd_print_cond, &wsd_print_mutex);
    }
    actual_flags = wsd_print_flags;
    wsd_print_flags = 0;
    pthread_mutex_unlock(&wsd_print_mutex);


    job_info = wsd_get_job_info( ( uint32_t ) handle );
    if ( job_info == NULL )
    {
        DBG_ERR( "%s - Failed to get job info\n", __FUNCTION__ );
        return SOAP_FAULT;
    }
    
    data_node = MEM_MALLOC( sizeof( wsd_job_data_node_t ) );
    ASSERT( data_node != NULL );
    data_node->buf = MEM_MALLOC( len );
    ASSERT( data_node->buf != NULL );
    memcpy( data_node->buf, buf, len );
    data_node->buflen = len;
    data_node->bufoffset = 0;
    ATInsertTailList(&job_info->job_data_list, &data_node->node);

    job_info->job_data_bytes_buffered += len;

    if ( ( job_info->io_ready_sent == false ) && ( job_info->job_data_bytes_buffered >= wsd_print_config_vals.signal_io_ready_threshold ) )
    {
        DBG_VERBOSE( "Setting net_io_ready\n" );
        net_io_ready( job_info->io_session_hndl );
        job_info->io_ready_sent = true;
    }
    if ( job_info->job_data_bytes_buffered >= wsd_print_config_vals.max_recv_bytes_to_buffer )
    {
        DBG_VERBOSE( "Receive buffer full - blocking waiting on read: %d\n", job_info->job_data_bytes_buffered ) ;
        wsd_print_clear_flags( WSD_PRINT_FLAG_FILL_BUFFER );
    }

    return SOAP_OK;
}

void wsd_print_mime_write_close(struct soap *soap, void *handle )
{
    DBG_VERBOSE( "%s - handle: %#X\n", __FUNCTION__, handle );
}

void wsd_print_set_flags( uint32_t flags )
{
 //   tx_event_flags_set( &wsd_print_flags, flags, TX_OR );
    
    pthread_mutex_lock(&wsd_print_mutex);
    wsd_print_flags |= flags;
    pthread_cond_broadcast(&wsd_print_cond);
    pthread_mutex_unlock(&wsd_print_mutex);
    
}

void wsd_print_clear_flags( uint32_t flags )
{
 //   tx_event_flags_set( &wsd_print_flags, ~flags, TX_AND );
    
    pthread_mutex_lock(&wsd_print_mutex);
    wsd_print_flags &= ~flags;
    pthread_cond_broadcast(&wsd_print_cond);
    pthread_mutex_unlock(&wsd_print_mutex);\
    
}


#ifdef HAVE_CMD 

static const char *wsd_print_cmd_status_door_desc = "force the door WSD status";
static const char *wsd_print_cmd_status_door_usage = NULL;
static const char *wsd_print_cmd_status_door_notes = NULL;
static int wsd_print_cmd_status_door_cb( int argc, char *argv[] )
{
    uint32_t status; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    status = STATUS_DOOROPEN_FRONT;
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)status, POSIX_WAIT_FOREVER ); 

    return CMD_OK;
}

static const char *wsd_print_cmd_status_paperout_desc = "force the paper out WSD status";
static const char *wsd_print_cmd_status_paperout_usage = NULL;
static const char *wsd_print_cmd_status_paperout_notes = NULL;
static int wsd_print_cmd_status_paperout_cb( int argc, char *argv[] )
{
    uint32_t status; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    status = STATUS_PAPER_OUT_TRAY1;
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)status, POSIX_WAIT_FOREVER ); 
    
    return CMD_OK;
}

static const char *wsd_print_cmd_status_paperjam_desc = "force the paperjam WSD status";
static const char *wsd_print_cmd_status_paperjam_usage = NULL;
static const char *wsd_print_cmd_status_paperjam_notes = NULL;
static int wsd_print_cmd_status_paperjam_cb( int argc, char *argv[] )
{
    uint32_t status; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    status = STATUS_PAPER_JAM_TRAY2;
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)status, POSIX_WAIT_FOREVER ); 
    
    return CMD_OK;
}

static const char *wsd_print_cmd_status_printing_desc = "force the printing WSD status";
static const char *wsd_print_cmd_status_printing_usage = NULL;
static const char *wsd_print_cmd_status_printing_notes = NULL;
static int wsd_print_cmd_status_printing_cb( int argc, char *argv[] )
{
    uint32_t status; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    status = STATUS_INFO_PRINTING;
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)status, POSIX_WAIT_FOREVER ); 

    return CMD_OK;
}

static const char *wsd_print_cmd_status_manual_duplex_desc = "force the manual duplex WSD status";
static const char *wsd_print_cmd_status_manual_duplex_usage = NULL;
static const char *wsd_print_cmd_status_manual_duplex_notes = NULL;
static int wsd_print_cmd_status_manual_duplex_cb( int argc, char *argv[] )
{
    uint32_t status; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    status = STATUS_INPUT_MANUAL_DUPLEX_TRAY_1;

    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)status, POSIX_WAIT_FOREVER ); 

    return CMD_OK;
}

static const char *wsd_print_cmd_rate_desc = "set the dpws event rate";
static const char *wsd_print_cmd_rate_usage = "<rate>";
static const char *wsd_print_cmd_rate_notes = "<rate> is 1-600";
static int wsd_print_cmd_rate_cb( int argc, char *argv[] )
{
    wsd_dpws_data_t *data;
    int value;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    data = wsd_get_dpws_data(  );
    if ( NULL == data )
    {
        return CMD_ERROR;
    }

    value = atoi( argv[ 1 ] );

    if ( value < 1 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( value > 600 )
    {
        return CMD_USAGE_ERROR;
    }

    data->event_rate = value;
    return CMD_OK;
}

static const char *wsd_print_cmd_enable_desc = "WSD enable";
static const char *wsd_print_cmd_enable_usage = NULL;
static const char *wsd_print_cmd_enable_notes = NULL;
static int wsd_print_cmd_enable_cb( int argc, char *argv[] )
{
    bool enabled = false;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    net_wsd_get_var_enabled( &enabled );
    if (!enabled)
    {
        net_wsd_set_var_enabled( true );
        net_wsd_get_var_enabled( &enabled );
        cmd_printf( "WSD now %s\n", enabled ? "enabled" : "disabled" );
    }
    else
    {
        cmd_printf( "WSD is %s\n", enabled ? "enabled" : "disabled" );
    }
    return CMD_OK;
}

static const char *wsd_print_cmd_disable_desc = "WSD disable";
static const char *wsd_print_cmd_disable_usage = NULL;
static const char *wsd_print_cmd_disable_notes = NULL;
static int wsd_print_cmd_disable_cb( int argc, char *argv[] )
{
    bool enabled = false;
    
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    net_wsd_get_var_enabled( &enabled );
    if (enabled)
    {
        net_wsd_set_var_enabled( false );
        net_wsd_get_var_enabled( &enabled );
        cmd_printf( "WSD now %s\n", enabled ? "enabled" : "disabled" );
    }
    else
    {
        cmd_printf( "WSD is %s\n", enabled ? "enabled" : "disabled" );
    }
    
    return CMD_OK;
}

static const char *wsd_print_cmd_restart_desc = "WSD disable";
static const char *wsd_print_cmd_restart_usage = NULL;
static const char *wsd_print_cmd_restart_notes = NULL;
static int wsd_print_cmd_restart_cb( int argc, char *argv[] )
{
    bool enabled;
    
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    if (wsd_busy())
    {
        DBG_ERR(" restart .. busy\n");
        return CMD_OK;
    }
    else
    if (wsd_initializing())
    {
        DBG_ERR(" restart .. initializing\n");
        //return CMD_OK;
    }

    net_wsd_get_var_enabled( &enabled );
    if (enabled)
    {
        int wsd_restart_dpws_server(void); // revisit extern cast
        int ret = wsd_restart_dpws_server();
        if (ret)
        {
            DBG_ERR(" restart err %d\n", ret);
            return ret;
        }
    }
    else
    {
        DBG_ERR(" restart NOT enabled\n");
    }

    return CMD_OK;
}

static const char *wsd_print_cmd_info_desc = "Find if WSD is enabled/disabled";
static const char *wsd_print_cmd_info_usage = NULL;
static const char *wsd_print_cmd_info_notes = NULL;
static int wsd_print_cmd_info_cb( int argc, char *argv[] )
{
    bool enabled;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    char plat_uuid[ WSD_UUID_LENGTH ];
    net_wsd_get_var_uuid(plat_uuid, WSD_UUID_LENGTH);

    net_wsd_get_var_enabled( &enabled );

    char* wsd_state = wsd_busy()? "running ": (wsd_ready()? "ready ": (wsd_initializing()? "initializing ": ""));
    cmd_printf( "WSD is %s .. %s(uuid '%s')\n", enabled ? "enabled" : "disabled", wsd_state, plat_uuid );

    if (wsd_ready() || wsd_busy())
    {
        show_wsd_device();
    }
    
    return CMD_OK;
}
#endif // HAVE_CMD


