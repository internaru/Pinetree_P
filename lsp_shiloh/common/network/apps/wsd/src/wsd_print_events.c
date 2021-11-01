/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file wsd_print_events.c
 *
 * \brief This file implements the WSD-Printing Eventing
 * protocol
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include "dprintf.h"
#include "lassert.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "platform.h"

#include "dc/dc_Dpws.h" // Main DPWSCore API include file.
#include "net_wsd_api.h"
#include "wsd_print.h"
#include "net_wsd_print_config.h"
#include "plat_print_config.h"

#include "print_events_common_api.h"
#include "PrintMgr.h"

#include "delay_api.h"  // delay_msec_timer_non_blocking()
typedef struct delay_msec_timer_t OS_TIMER;

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
#define NET_WSD_MQ_NAME           "/netWsdPrint"
#define NET_WSD_MQ_NUM_MSG_BUFS   5

// Debug configuration
#define DBG_PRFX "WSD.PRINT.EVENT: "

#define THREAD_STACK_SIZE   PTHREAD_STACK_MIN

//--------------------------------------
// Global Definitions
//--------------------------------------
static pthread_t         wsd_print_event_thread;
static ALIGN8 uint8_t    wsd_print_event_stack[ THREAD_STACK_SIZE ];
//static net_wsd_msg_buf_t g_net_wsd_print_msg_bufs[ NET_WSD_MQ_NUM_MSG_BUFS ]; // msg queue buffer space
static mqd_t             g_net_wsd_print_msg_queue; // msg queue control block

typedef struct wsd_print_event_s
{
    ATLISTENTRY node;
    wsd_get_printer_status_condition_options_t* option;
    int32_t id;
}wsd_print_event_t;

static ATLISTENTRY      wsd_print_event_history;
static OS_TIMER*        wsd_print_event_printer_status;
static OS_TIMER*        wsd_print_event_job_status;
static bool             wsd_print_event_device_status_ok_to_send  = true;
static bool             wsd_print_event_job_status_ok_to_send     = true;
static struct dpws      *g_dpws;


static Observer_t       *print_status_obs  = NULL;

//--------------------------------------
// Local Function Declarations
//--------------------------------------
static void* wsd_print_event_main(void *input );
static void wsd_print_event_handle_printer_element_changed( struct dpws * dpws, wsd_printer_elements_changed_t, uint32_t, void* );
static void wsd_print_event_handle_printer_status_summary( struct dpws * dpws, uint32_t status );
static int32_t wsd_print_event_handle_printer_status_condition( struct dpws * dpws, uint32_t status, int32_t id );
static void wsd_print_event_handle_job_status( struct dpws * dpws, uint32_t job_id );
static void wsd_print_event_handle_job_end_state( struct dpws * dpws, uint32_t job_id );
static void wsd_print_event_start_timer( OS_TIMER * );
#ifdef __linux__
static void wsd_print_event_timer_cb( void* cb_data );
#else // threadx
static void wsd_print_event_timer_callback( unsigned long param );
#endif // threadx
static void print_status_updater(Observer_t *observer, Subject_t *s);

//--------------------------------------
// Local Function Definitions
//--------------------------------------
void wsd_print_events_init( )
{
    UINT tx_res;

    /*
     *  create message queue
     */
    posix_create_message_queue(&g_net_wsd_print_msg_queue, NET_WSD_MQ_NAME, NET_WSD_MQ_NUM_MSG_BUFS, 
                               sizeof(net_wsd_msg_buf_t));
    
    tx_res = posix_create_thread( &wsd_print_event_thread, wsd_print_event_main, 0, "WSD Print Events",
                                       wsd_print_event_stack, THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);    
    XASSERT( 0 == tx_res, tx_res );


    // validation of msg buf size
    ASSERT( sizeof( net_wsd_msg_buf_t ) == NET_WSD_MQ_MSG_BUF_BYTES);
   

    // initialize timers
#ifdef __linux__
    wsd_print_event_printer_status = NULL;
    wsd_print_event_job_status = NULL;
#else // threadx
    tx_res = tx_timer_create( &wsd_print_event_printer_status,
                              "wsd_print_event_printer_status",
                              wsd_print_event_timer_callback,
                              (unsigned long) WSD_DEVICE_STATUS,
                              1,
                              0,
                              TX_NO_ACTIVATE );
    XASSERT( TX_SUCCESS == tx_res, tx_res );

    tx_res = tx_timer_create( &wsd_print_event_job_status,
                              "wsd_print_event_job_status",
                              wsd_print_event_timer_callback,
                              (unsigned long) WSD_JOB_STATUS,
                              1,
                              0,
                              TX_NO_ACTIVATE );
    XASSERT( TX_SUCCESS == tx_res, tx_res );
#endif // threadx

    // Register for updates from the printer device
    if ( print_status_obs == NULL )
    {
        print_status_obs = Observer_Constructor(PrintJobMgr_PrintStatus_ID, print_status_updater);
    }
    pec_attachobserver_printstatus(print_status_obs);
    ASSERT(print_status_obs != NULL);

}

void wsd_print_events_shutdown( )
{
  //  UINT tx_res;

    pec_detachobserver_printstatus(print_status_obs);

#ifdef __linux__
    
#else // threadx
    tx_res = tx_thread_terminate( &wsd_print_event_thread );
    XASSERT( TX_SUCCESS == tx_res, tx_res );

    tx_res = tx_thread_delete( &wsd_print_event_thread );
    XASSERT( TX_SUCCESS == tx_res, tx_res );
#endif // threadx
    
    //revisit pthread_join (wsd_print_event_thread,NULL);
    
    if ( g_dpws )
    {
        MEM_FREE_AND_NULL( g_dpws );
    }

#if 0        
    tx_res = tx_queue_delete( &g_net_wsd_print_msg_queue );
    XASSERT( TX_SUCCESS == tx_res, tx_res );


    tx_res = tx_timer_delete( &wsd_print_event_printer_status );
    XASSERT( TX_SUCCESS == tx_res, tx_res );

    tx_res = tx_timer_delete( &wsd_print_event_job_status );
    XASSERT( TX_SUCCESS == tx_res, tx_res );
#endif    
    DBG_VERBOSE("%s() done\n", __func__);
}

void wsd_print_subscription_failed(struct dpws * dpws, short event_source, struct wsa_endpoint_ref * sink) 
{ 
    DBG_VERBOSE( "Removing subscriber: source: %d - address: %s\n", event_source, sink->address );
    // Could replace with a counter of failures - for now just kill it on first error
    dpws_remove_subscriber( event_source, sink ); 
}

void* wsd_print_event_main( void*  input )
{
    net_wsd_msg_buf_t msg_buf;
    net_wsd_msg_t *msg;
    int32_t id = 1;
    uint32_t job_id = 0;
    UINT tx_res;
    uint32_t new_data;
    uint32_t printer_status;
    
    ATInitList( &wsd_print_event_history );
    SysWaitForInit ();

    printer_status = 0;
    
    do
    {
        g_dpws = MEM_MALLOC( sizeof( struct dpws ) );
        if ( g_dpws == NULL )
        {
            DBG_ERR( "Failed to malloc memory for DPWS structure used by print events - try again in 1 second\n" );
            posix_sleep_ticks( 100 );
        }
    } while ( g_dpws == NULL );

    while ( 1 )
    {
        if ( id < 0 )
        {
            id = 1;
        }
        
#ifdef __linux__
        tx_res = posix_wait_for_message(g_net_wsd_print_msg_queue, (void *)&msg_buf, 
                                               sizeof(net_wsd_msg_t), POSIX_WAIT_FOREVER);
        if(tx_res != 0)
        {
            DBG_ERR("error reading g_net_wsd_msg_queue (res=0x%02x)\n", tx_res);
            break;
        }        
#else // threadx
        tx_res = tx_queue_receive( &g_net_wsd_print_msg_queue, (void *)&msg_buf, TX_WAIT_FOREVER);
        if(tx_res != TX_SUCCESS)
        {
            DBG_ERR("error reading g_net_wsd_msg_queue (threadx_rcode=0x%02x)\n", tx_res);
            XASSERT( 0, tx_res );
            continue;
        }
#endif // threadx
        
        msg = &msg_buf.msg;
        ASSERT(msg->type < NET_WSD_NUM_MSGS);

        if (!g_dpws)
        {
            DBG_ERR("%s() dpws_client_init(%#x) msg %d\n", __func__, g_dpws, msg->type);
            DBG_ASSERT(g_dpws);
        }
        dpws_client_init( g_dpws, NULL );

        g_dpws->fnotification_failure = wsd_print_subscription_failed;

        switch ( msg->type )
        {
            case NET_WSD_PRINTER_ELEMENTS_CHANGE:   // Cumulative Event - send as often as ready
                {
                    wsd_print_element_change_t *info = ( wsd_print_element_change_t * )msg->data;
                    wsd_print_event_handle_printer_element_changed(g_dpws, info->element, info->value, info->data );
                    MEM_FREE_AND_NULL( info );
                    break;
                }
            case NET_WSD_PRINTER_STATUS_SUMMARY:    // CompleteState Event - only send 1 per event rate
                new_data = ( uint32_t ) msg->data;
                if ( wsd_print_event_device_status_ok_to_send == true )
                {
                    if ( new_data == 0 )     // Message indicates use saved status
                    {
                        if ( printer_status != 0 )  // Do we have any saved status?
                        {
                            wsd_print_event_handle_printer_status_summary( g_dpws, printer_status );
                            printer_status = 0;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else                            // Send status from message
                    {
                        wsd_print_event_handle_printer_status_summary( g_dpws, new_data );
                    }                    
                    wsd_print_event_device_status_ok_to_send = false;
                    wsd_print_event_start_timer( wsd_print_event_printer_status );
                }
                else
                {
                    printer_status = new_data;
                }
                break;
            case NET_WSD_PRINTER_STATUS_CONDITION:  // Cumulative Event - send as often as ready
                id = wsd_print_event_handle_printer_status_condition( g_dpws, (uint32_t)msg->data, id );
                break;
            case NET_WSD_JOB_STATUS:    // CompleteState Event - only send 1 per event rate
                new_data = ( uint32_t ) msg->data;
                // REVISIT TO DO - verify none of these get processed after job end state has been processed for job
                if ( wsd_print_event_job_status_ok_to_send == true )
                {
                    if ( new_data == 0 )     // Message indicates use saved status
                    {
                        if ( job_id != 0 )  // Do we have any saved status?
                        {
                            wsd_print_event_handle_job_status( g_dpws, job_id );
                            job_id = 0;
                        }
                        else
                        {
                            // Nothing to send - just get out now
                            break; 
                        }
                    }
                    else                            // Send status from message
                    {
                        wsd_print_event_handle_job_status( g_dpws, new_data );
                    }                    
                    wsd_print_event_job_status_ok_to_send = false;
                    wsd_print_event_start_timer( wsd_print_event_job_status );
                }
                else
                {
                    job_id      = new_data;
                }
                break;
            case NET_WSD_JOB_END_STATE: // Cumulative Event - send as often as ready
#ifdef __linux__
                delay_msec_timer_cancel((delay_msec_timer_t*)wsd_print_event_job_status); // cast OS_TIMER*
#else
                tx_timer_deactivate( &wsd_print_event_job_status );
#endif
                wsd_print_event_job_status_ok_to_send = true;
                job_id = 0;
                wsd_print_event_handle_job_end_state( g_dpws, (uint32_t)msg->data );
                break;
            default:
                break;
        }
        dpws_end( g_dpws );
    }
    return 0;
}

static void wsd_print_event_handle_printer_element_changed( struct dpws * dpws, wsd_printer_elements_changed_t element, uint32_t value, void* data )
{
    struct wprt__PrinterElementsChangeEventType event;
    wsd_dpws_data_t *dpws_data = wsd_get_dpws_data( );
    char *str = "A4";
    int32_t res;
       
    event.ElementChanges = dpws_malloc( dpws, sizeof( struct wprt__ElementChangesType ) );
    memset( event.ElementChanges, 0x00, sizeof( struct wprt__ElementChangesType ) );
    // event.ElementChanges->DefaultPrintTicket Elements that could change:
    // --Num Copies - PDL only
    // --Collate - PDL only
    // --Default media size/type - PDL/MFP

    switch ( element )
    { 
        case WSD_PRINTER_ELEMENT_NUM_COPIES:
            event.ElementChanges->DefaultPrintTicket = dpws_malloc( dpws, sizeof( struct wprt__PrintTicketType ) );
            memset( event.ElementChanges->DefaultPrintTicket, 0x00, sizeof( struct wprt__PrintTicketType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing = dpws_malloc( dpws, sizeof( struct wprt__JobProcessingType ) );
            memset( event.ElementChanges->DefaultPrintTicket->JobProcessing, 0x00, sizeof( struct wprt__JobProcessingType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing->Copies = dpws_malloc( dpws, sizeof( struct wprt__JobCopiesType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing->Copies->__item = value;
            event.ElementChanges->DefaultPrintTicket->JobProcessing->Copies->MustHonor = dpws_malloc( dpws, sizeof( enum xsd__boolean ) );
            *event.ElementChanges->DefaultPrintTicket->JobProcessing->Copies->MustHonor = xsd__boolean__false_;
            break;
        case WSD_PRINTER_ELEMENT_COLLATE:
            event.ElementChanges->DefaultPrintTicket = dpws_malloc( dpws, sizeof( struct wprt__PrintTicketType ) );
            memset( event.ElementChanges->DefaultPrintTicket, 0x00, sizeof( struct wprt__PrintTicketType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing = dpws_malloc( dpws, sizeof( struct wprt__JobProcessingType ) );
            memset( event.ElementChanges->DefaultPrintTicket->JobProcessing, 0x00, sizeof( struct wprt__JobProcessingType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings = dpws_malloc( dpws, sizeof( struct wprt__JobFinishingsType ) );
            memset( event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings, 0x00, sizeof( struct wprt__JobFinishingsType ) );
            event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings->Collate = dpws_malloc( dpws, sizeof( struct wprt__BooleanExtType ) );;
            event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings->Collate->__item = value ? xsd__boolean__true_ : xsd__boolean__false_;
            event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings->Collate->MustHonor = dpws_malloc( dpws, sizeof( enum xsd__boolean ) );
            *event.ElementChanges->DefaultPrintTicket->JobProcessing->JobFinishings->Collate->MustHonor = xsd__boolean__false_;
            break;
        case WSD_PRINTER_ELEMENT_DEFAULT_MEDIA_SIZE:
            event.ElementChanges->DefaultPrintTicket = dpws_malloc( dpws, sizeof( struct wprt__PrintTicketType ) );
            memset( event.ElementChanges->DefaultPrintTicket, 0x00, sizeof( struct wprt__PrintTicketType ) );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing = dpws_malloc( dpws, sizeof( struct wprt__DocumentProcessingType ) );
            memset( event.ElementChanges->DefaultPrintTicket->DocumentProcessing, 0x00, sizeof( struct wprt__DocumentProcessingType ) );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaSizeName = dpws_malloc( dpws, sizeof( struct wprt__MediaSizeNameType ) );
            if ( net_wsd_map_mediasize( value, &str ) != NET_WSD_SUCCESS ) ASSERT( 0 );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaSizeName->__item = dpws_malloc( dpws, strlen( str ) + 1 );
            strcpy( event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaSizeName->__item, str );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaSizeName->MustHonor = dpws_malloc( dpws, sizeof( enum xsd__boolean ) );
            *event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaSizeName->MustHonor = xsd__boolean__false_;
            break;
        case WSD_PRINTER_ELEMENT_DEFAULT_MEDIA_TYPE:
            event.ElementChanges->DefaultPrintTicket = dpws_malloc( dpws, sizeof( struct wprt__PrintTicketType ) );
            memset( event.ElementChanges->DefaultPrintTicket, 0x00, sizeof( struct wprt__PrintTicketType ) );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing = dpws_malloc( dpws, sizeof( struct wprt__DocumentProcessingType ) );
            memset( event.ElementChanges->DefaultPrintTicket->DocumentProcessing, 0x00, sizeof( struct wprt__DocumentProcessingType ) );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaType = dpws_malloc( dpws, sizeof( struct wprt__MediaTypeBaseType ) );
            if ( net_wsd_map_mediatype( value, &str ) != NET_WSD_SUCCESS ) ASSERT( 0 );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaType->__item = dpws_malloc( dpws, strlen( str ) + 1 );
            strcpy( event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaType->__item, str );
            event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaType->MustHonor = dpws_malloc( dpws, sizeof( enum xsd__boolean ) );
            *event.ElementChanges->DefaultPrintTicket->DocumentProcessing->MediaType->MustHonor = xsd__boolean__false_;
            break;
        case WSD_PRINTER_ELEMENT_EVENT_RATE:
            event.ElementChanges->PrinterConfiguration = dpws_malloc( dpws, sizeof( struct wprt__PrinterConfigurationType ) );
            memset( event.ElementChanges->PrinterConfiguration, 0x00, sizeof( struct wprt__PrinterConfigurationType ) );
            event.ElementChanges->PrinterConfiguration->PrinterEventRate = dpws_malloc( dpws, sizeof( struct wprt__EventRateType ) );
            event.ElementChanges->PrinterConfiguration->PrinterEventRate->__item = value;
            break;
        case WSD_PRINTER_ELEMENT_CONSUMABLE_LEVEL:
            if ( net_wsd_map_color( value, &str ) != OK)
            {
                str = "unknown";
            }
            event.ElementChanges->PrinterConfiguration = dpws_malloc( dpws, sizeof( struct wprt__PrinterConfigurationType ) );
            memset( event.ElementChanges->PrinterConfiguration, 0x00, sizeof( struct wprt__PrinterConfigurationType ) );
            event.ElementChanges->PrinterConfiguration->Consumables = dpws_malloc( dpws, sizeof( struct wprt__ConsumablesType ) );
            event.ElementChanges->PrinterConfiguration->Consumables->__sizeConsumableEntry = 1;
            event.ElementChanges->PrinterConfiguration->Consumables->ConsumableEntry = dpws_malloc( dpws, sizeof( struct wprt__ConsumableEntryType ) );
            wsd_populate_consumable_structure( dpws,
                                               event.ElementChanges->PrinterConfiguration->Consumables->ConsumableEntry, 
                                               str, 
                                               ( uint32_t ) data,
                                               true );
            break;
        case WSD_PRINTER_ELEMENT_MEDIA_IN_TRAY:
            {
                mediacnfg_t media_config;
                error_type_t err;

                err = printvar_get_trayconfig( &media_config );
                err = OK;
                XASSERT( err == OK, err );

                event.ElementChanges->PrinterConfiguration = dpws_malloc( dpws, sizeof( struct wprt__PrinterConfigurationType ) );
                memset( event.ElementChanges->PrinterConfiguration, 0x00, sizeof( struct wprt__PrinterConfigurationType ) );
                event.ElementChanges->PrinterConfiguration->InputBins = dpws_malloc( dpws, sizeof( struct wprt__InputBinsType ) );
                event.ElementChanges->PrinterConfiguration->InputBins->__sizeInputBinEntry = 1;
                event.ElementChanges->PrinterConfiguration->InputBins->InputBinEntry = dpws_malloc( dpws, sizeof( struct wprt__InputBinEntryType ) );
                wsd_populate_input_bins_structure( dpws,
                                                   event.ElementChanges->PrinterConfiguration->InputBins->InputBinEntry,
                                                   plat_config_tray_capacity( ), 
                                                   plat_config_tray_level( ), 
                                                   media_config.mediaSize[ value ],
                                                   media_config.mediaType[ value ],
                                                   value
                                                   );
            }
            break;
        case WSD_PRINTER_ELEMENT_LOCATION:
            event.ElementChanges->PrinterDescription = dpws_malloc( dpws, sizeof( struct wprt__PrinterDescriptionType ) );
            event.ElementChanges->PrinterDescription->__sizePrinterLocation = 1;
            event.ElementChanges->PrinterDescription->PrinterLocation = dpws_malloc( dpws, sizeof( struct wprt__LocalizedStringType ) );
            event.ElementChanges->PrinterDescription->PrinterLocation[ 0 ].__item = dpws_malloc( dpws, strlen( (char*) data  ) +1 );
            strcpy( event.ElementChanges->PrinterDescription->PrinterLocation[ 0 ].__item, (char*)data );
            break;
        default:
            XASSERT( 0, element );
            break;
    }

    res = dpws_notify___wprt__PrinterElementsChangeEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );
}

static void wsd_print_event_handle_printer_status_summary( struct dpws * dpws, uint32_t status )
{
    struct wprt__PrinterStatusSummaryEventType event;
    wsd_get_printer_status_options_t *wsd_status;
    wsd_dpws_data_t *dpws_data = wsd_get_dpws_data( );
    int32_t res;

    wsd_status = wsd_get_status_values( status );
    DBG_VERBOSE( "%#08X - %s - %s - %s\n", status, wsd_status->printer_state, wsd_status->printer_state_reason, wsd_status->printer_primary_state_reason );

    event.StatusSummary = dpws_malloc( dpws, sizeof( struct wprt__StatusSummaryType ) );
    event.StatusSummary->PrinterPrimaryStateReason = dpws_malloc( dpws, sizeof( struct wprt__PrinterStateReasonType ) );
    event.StatusSummary->PrinterPrimaryStateReason->__item = dpws_malloc( dpws, strlen( wsd_status->printer_primary_state_reason ) + 1 );
    strcpy( event.StatusSummary->PrinterPrimaryStateReason->__item, wsd_status->printer_primary_state_reason );

    event.StatusSummary->PrinterState = dpws_malloc( dpws, sizeof( struct wprt__PrinterStateType ) );
    event.StatusSummary->PrinterState->__item = dpws_malloc( dpws, strlen( wsd_status->printer_state ) + 1 );
    strcpy( event.StatusSummary->PrinterState->__item, wsd_status->printer_state );

    event.StatusSummary->PrinterStateReasons = dpws_malloc( dpws, sizeof( struct wprt__PrinterStateReasonsType ) );
    event.StatusSummary->PrinterStateReasons->__sizePrinterStateReason = 1;
    event.StatusSummary->PrinterStateReasons->PrinterStateReason = dpws_malloc( dpws, sizeof( struct wprt__PrinterStateReasonType ) );
    event.StatusSummary->PrinterStateReasons->PrinterStateReason[ 0 ].__item = dpws_malloc( dpws, strlen( wsd_status->printer_state_reason ) + 1 );
    strcpy( event.StatusSummary->PrinterStateReasons->PrinterStateReason[ 0 ].__item, wsd_status->printer_state_reason );

    res = dpws_notify___wprt__PrinterStatusSummaryEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );

}

static int32_t wsd_print_event_handle_printer_status_condition( struct dpws * dpws, uint32_t status, int32_t id )
{
    struct wprt__PrinterStatusConditionEventType event;
    uint32_t i;
    int32_t res;
    wsd_get_printer_status_condition_options_t *option = NULL;
    wsd_get_printer_status_condition_options_t *table;
    uint32_t table_entries;
    bool option_in_list = false;
    ATLISTENTRY* node;
    wsd_print_event_t* wsd_print_event;
    wsd_dpws_data_t *dpws_data = wsd_get_dpws_data( );

    net_wsd_get_printer_status_condition_table( &table, &table_entries );
    table_entries = 1;
    for ( i = 0; i < table_entries; i++ )
    {
        if ( status == table[ i ].status )
        {
            option = &table[ i ];
            break;
        }
    }

    
    // Check to see if the current option is the same as one already in the history
    if ( option != NULL )
    {
        node = ATListHead(&wsd_print_event_history);
        while( node != NULL )
        {
            wsd_print_event = CONTAINING_RECORD(node, wsd_print_event_t, node);
            if ( wsd_print_event->option == option )
            {
                option_in_list = true;
                break;
            }
            node = ATListNext(&wsd_print_event_history, node);
        }
    }

    // If we have a new event - send status cleared events for all old events
    if ( ( option == NULL ) ||
         ( option_in_list == false ) )
    {
        while ( ( node = ATRemoveHeadList( &wsd_print_event_history ) ) != NULL )
        {
            struct wprt__PrinterStatusConditionClearedEventType event;
            event.DeviceConditionCleared = dpws_malloc( dpws, sizeof( struct wprt__DeviceConditionClearedType ) );
            event.DeviceConditionCleared->ConditionClearTime = dpws_malloc( dpws, sizeof( struct wprt__DateTimeExtType ) );
//            event.DeviceConditionCleared->ConditionClearTime->__item = tx_time_get();
            wsd_print_event = CONTAINING_RECORD(node, wsd_print_event_t, node);
            event.DeviceConditionCleared->ConditionId = wsd_print_event->id;
            res = dpws_notify___wprt__PrinterStatusConditionClearedEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );
            DBG_MSG( "PrinterStatusConditionClearedEvent: %d\n", res );

            MEM_FREE_AND_NULL( wsd_print_event );
        }
    }

    if ( option != NULL )
    {
        event.DeviceCondition = dpws_malloc( dpws, sizeof( struct wprt__DeviceConditionType ) );
        event.DeviceCondition->Component =  dpws_malloc( dpws, sizeof( struct wprt__ConditionComponentType ) );
        event.DeviceCondition->Component->Group = dpws_malloc( dpws, sizeof( struct wprt__ComponentGroupType ) );
        event.DeviceCondition->Component->Group->__item = option->component_group;
        event.DeviceCondition->Component->Name = dpws_malloc( dpws, sizeof( struct wprt__ComponentGroupType ) );
        event.DeviceCondition->Component->Name->__item = dpws_malloc( dpws, strlen( option->component_name ) + 1 );
        strcpy( event.DeviceCondition->Component->Name->__item, option->component_name );

        event.DeviceCondition->Id =         id;

        event.DeviceCondition->Name =       dpws_malloc( dpws, sizeof( struct wprt__ConditionNameType ) );
        event.DeviceCondition->Name->__item = option->name;

        event.DeviceCondition->Severity =   dpws_malloc( dpws, sizeof( struct wprt__ConditionSeverityType ) );
        event.DeviceCondition->Severity->__item = option->severity;

        event.DeviceCondition->Time =       dpws_malloc( dpws, sizeof( struct wprt__DateTimeExtType ) );
        //event.DeviceCondition->Time->__item = tx_time_get();
        event.DeviceCondition->Time->__item = posix_gettime_ms();
        res = dpws_notify___wprt__PrinterStatusConditionEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );
        DBG_MSG( "PrinterStatusConditionEvent: %d\n", res );

        // Add event to history list
        wsd_print_event = MEM_MALLOC( sizeof( wsd_print_event_t ) );
        ASSERT( wsd_print_event != NULL );
        wsd_print_event->option = option;
        wsd_print_event->id = id;
        ATInsertTailList( &wsd_print_event_history, &wsd_print_event->node );

        // Increment id for next status
        id++;
    }

    return id;
}

static void wsd_print_event_handle_job_status( struct dpws * dpws, uint32_t job_id )
{
    struct wprt__JobStatusEventType event;
    wsd_job_info_t *job_info;
    wsd_dpws_data_t *dpws_data;
    int32_t res;

    dpws_data = wsd_get_dpws_data( );
    wsd_lock_job_history( );
    job_info = wsd_get_job_info( job_id );
    if ( job_info == NULL )
    {
        wsd_unlock_job_history( );
        return;
    }

    event.JobStatus = dpws_malloc( dpws, sizeof( struct wprt__JobStatusType ) );
    event.JobStatus->JobId = dpws_malloc( dpws, sizeof( struct wprt__JobIdType ) );
    event.JobStatus->JobId->__item = job_info->id;

    event.JobStatus->JobState = dpws_malloc( dpws, sizeof( struct wprt__JobStateType ) );
    event.JobStatus->JobState->__item = dpws_malloc( dpws, strlen( wsd_get_job_state_string( job_info->state ) ) + 1 );
    strcpy( event.JobStatus->JobState->__item, wsd_get_job_state_string( job_info->state ) );

    event.JobStatus->JobStateReasons = dpws_malloc( dpws, sizeof( struct wprt__JobStateReasonsType ) );
    event.JobStatus->JobStateReasons->__sizeJobStateReason = 1;
    event.JobStatus->JobStateReasons->JobStateReason = dpws_malloc( dpws, sizeof( struct wprt__JobStateReasonType ) );
    event.JobStatus->JobStateReasons->JobStateReason[ 0 ].__item = dpws_malloc( dpws, strlen( wsd_job_state_reason_str( job_info->reasons ) ) + 1 );
    strcpy( event .JobStatus->JobStateReasons->JobStateReason[ 0 ].__item, wsd_job_state_reason_str( job_info->reasons ) );

    event.JobStatus->KOctetsProcessed = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobStatus->KOctetsProcessed->__item = job_info->koctets_processed;

    event.JobStatus->MediaSheetsCompleted = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobStatus->MediaSheetsCompleted->__item = job_info->sheets_completed;

    event.JobStatus->NumberOfDocuments = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobStatus->NumberOfDocuments->__item = job_info->num_documents;

    res = dpws_notify___wprt__JobStatusEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );
    wsd_unlock_job_history( );

}

static void wsd_print_event_handle_job_end_state( struct dpws * dpws, uint32_t job_id )
{
    struct wprt__JobEndStateEventType event;
    int32_t res;
    wsd_job_info_t *job_info;
    wsd_dpws_data_t *dpws_data;

    dpws_data = wsd_get_dpws_data( );
    wsd_lock_job_history( );
    job_info = wsd_get_job_info( job_id );
    if ( job_info == NULL )
    {
        wsd_unlock_job_history( );
        return;
    }

    event.JobEndState = dpws_malloc( dpws, sizeof( struct wprt__JobEndStateType ) );
    event.JobEndState->JobCompletedState = dpws_malloc( dpws, sizeof( struct wprt__JobStateType ) );
    event.JobEndState->JobCompletedState->__item = dpws_malloc( dpws, strlen( wsd_get_job_state_string( job_info->state ) ) + 1 );
    strcpy( event.JobEndState->JobCompletedState->__item, wsd_get_job_state_string( job_info->state ) );

    event.JobEndState->JobCompletedStateReasons = dpws_malloc( dpws, sizeof( struct wprt__JobStateReasonsType ) );
    event.JobEndState->JobCompletedStateReasons->__sizeJobStateReason = 1;
    event.JobEndState->JobCompletedStateReasons->JobStateReason = dpws_malloc( dpws, sizeof( struct wprt__JobStateReasonType ) );
    event.JobEndState->JobCompletedStateReasons->JobStateReason[ 0 ].__item = dpws_malloc( dpws, strlen( wsd_job_state_reason_str( job_info->reasons ) ) + 1 );
    strcpy( event.JobEndState->JobCompletedStateReasons->JobStateReason[ 0 ].__item, wsd_job_state_reason_str( job_info->reasons ) );

    event.JobEndState->JobId = dpws_malloc( dpws, sizeof( struct wprt__JobIdType ) );
    event.JobEndState->JobId->__item = job_info->id;

    event.JobEndState->JobName = dpws_malloc( dpws, sizeof( struct wprt__JobNameType ) );
    event.JobEndState->JobName->__item = dpws_malloc( dpws, strlen( job_info->name ) + 1 );
    strcpy( event.JobEndState->JobName->__item, job_info->name );

    event.JobEndState->JobOriginatingUserName = dpws_malloc( dpws, sizeof( struct wprt__JobOriginatingUserNameType ) );
    event.JobEndState->JobOriginatingUserName->__item = dpws_malloc( dpws, strlen( job_info->user_name ) + 1 );
    strcpy( event.JobEndState->JobOriginatingUserName->__item, job_info->user_name );

    event.JobEndState->KOctetsProcessed = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobEndState->KOctetsProcessed->__item = job_info->koctets_processed;

    event.JobEndState->MediaSheetsCompleted = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobEndState->MediaSheetsCompleted->__item = job_info->sheets_completed;

    event.JobEndState->NumberOfDocuments = dpws_malloc( dpws, sizeof( struct wprt__IntZeroExtType ) );
    event.JobEndState->NumberOfDocuments->__item = job_info->num_documents;

    res = dpws_notify___wprt__JobEndStateEvent( dpws, dpws_data->event_source[ WSD_PRINT ], &event );
    DBG_MSG( "JobEndStateEvent: %d\n", res );
    wsd_unlock_job_history( );

}

#ifdef __linux__

// activate 1 of wsd's event timers
error_type_t activate_wsd_print_event_timer(OS_TIMER* timer, unsigned long ms_delay)
{
    if (timer == wsd_print_event_printer_status)
    {
        wsd_print_event_printer_status =
            (OS_TIMER*)(delay_msec_timer_t*)
                delay_msec_timer_non_blocking(ms_delay, wsd_print_event_timer_cb, (void*)WSD_DEVICE_STATUS, false); // cast
        return OK;
    }
    else
    if (timer == wsd_print_event_job_status)
    {
        wsd_print_event_job_status =
            (OS_TIMER*)(delay_msec_timer_t*)
                delay_msec_timer_non_blocking(ms_delay, wsd_print_event_timer_cb, (void*)WSD_JOB_STATUS, false); // cast
        return OK;
    }
    return FAIL;
}

void wsd_print_event_timer_cb(void* cb_data)
{
    unsigned long param = (unsigned long)cb_data; // cast

#else // threadx
static void wsd_print_event_timer_callback( unsigned long param )
{
#endif // threadx
    if ( param == WSD_DEVICE_STATUS )
    {
        wsd_print_event_device_status_ok_to_send = true;
        wsd_print_send_msg_no_wait( NET_WSD_PRINTER_STATUS_SUMMARY, NULL );
    }
    else if ( param == WSD_JOB_STATUS )
    {
        wsd_print_event_job_status_ok_to_send = true;
        wsd_print_send_msg_no_wait( NET_WSD_JOB_STATUS, NULL );

        DBG_MSG( "%s - send job status\n", __FUNCTION__ );
    }
    else
    {
        XASSERT( 0, param );    // Corruption!
    }
}

static void wsd_print_event_start_timer( OS_TIMER* timer )
{
    uint32_t rate;
    wsd_dpws_data_t *dpws_data = wsd_get_dpws_data( );

#ifdef __linux__
    DBG_VERBOSE("%s(timer %#x) data %#x\n", __func__, timer, dpws_data);
#else
    UINT tx_res; 
    tx_res = tx_timer_deactivate( timer );
    XASSERT( tx_res == TX_SUCCESS, tx_res );
#endif

    rate = dpws_data->event_rate;
    if (rate == 0)
    {
        rate = 5;
    }

#ifdef __linux__
    if (rate > 0)
    {
        int ms_delay = rate * TIMER_INTS_PER_SECOND; // rate * ticks (rate @ 100 ticks per second)
        activate_wsd_print_event_timer(timer, ms_delay);
    }
#else // threadx
    tx_res = tx_timer_change( timer, rate * TIMER_INTS_PER_SECOND, 0 );
    XASSERT( tx_res == TX_SUCCESS, tx_res );

    tx_res = tx_timer_activate( timer );
    XASSERT( tx_res == TX_SUCCESS, tx_res );
#endif // threadx
}

int32_t wsd_print_send_msg_wait_opt( net_wsd_msg_type_t type, void *data, unsigned long threadx_wait_opt)
{
    net_wsd_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.data = data;

#if 0 
    UINT threadx_rcode = tx_queue_send( &g_net_wsd_print_msg_queue, &buf, threadx_wait_opt);
    if(threadx_rcode == TX_QUEUE_FULL)
    {
        /* 
         *  msg queue full
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and threadx_wait_opt must
         *  therefore be set to TX_NO_WAIT
         */
        return NET_WSD_NOT_PERMITTED;
    }
    else if(threadx_rcode != TX_SUCCESS)
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, threadx_rcode);
    }
#endif

    UINT pthread_rcode = posix_message_send(g_net_wsd_print_msg_queue, (char *)&buf, sizeof(buf),
                                            MQ_DEFAULT_PRIORITY, threadx_wait_opt);
    if(pthread_rcode == ETIMEDOUT)
    {
        /* 
         *  msg queue full
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and threadx_wait_opt must
         *  therefore be set to TX_NO_WAIT
         */
        return NET_WSD_NOT_PERMITTED;
    }
    else if(pthread_rcode != 0)
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, pthread_rcode);        
    }

    return NET_WSD_SUCCESS;
}

/**
 * \brief Callback when a system status is updated
 * When this happens, get the status, make a message and
 * forward it to the correct task
 */
static void print_status_updater(Observer_t *observer, Subject_t *s)
{
    uint32_t printer_status;
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t,
                               subject_observer_oid_key, PrintJobMgr_PrintStatus_ID));
    
    printer_status = pec_get_printstatus(s);
    DBG_MSG("%s - %x\n", __FUNCTION__, printer_status);

    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_SUMMARY, (void *)printer_status, POSIX_WAIT_FOREVER);
    wsd_print_send_msg_wait_opt( NET_WSD_PRINTER_STATUS_CONDITION, (void *)printer_status, POSIX_WAIT_FOREVER ); 

}
