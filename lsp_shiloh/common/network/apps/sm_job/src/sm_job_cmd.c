/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "cmd_proc_api.h"
#include "lassert.h"
#include "debug.h"
#include "memAPI.h"
#include "ioutils.h"
#include "dprintf.h"
#include "logger.h"
#include "net_logger.h"
#include "sm_job_api.h"
#include "sm_job_internal_api.h"
#include "sm_job_support_api.h"

#ifdef DEBUG

//=============================================================================
// Defines
//=============================================================================

#define BASE_JOB_NAME "smjob_debug_job"
#define BASE_JOB_USER_NAME "smjob_debug_user"
#define DEBUG_JOB_NUM_LEN 8
#define FILE_PATH_TMP "/tmp"

#define DBG_PRFX "SM JOB CMD: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//=============================================================================
// Local variables
//=============================================================================

static smjob_ticket_t *g_job_ticket;
static smjob_doc_ticket_t *g_doc_ticket;
static smjob_status_t g_job_status;
static char *g_job_name;
static char *g_job_user_name;
static char *g_file_name;
static uint32_t g_debug_job_number = 1;

//=============================================================================
// Local function prototypes
//=============================================================================

void _smjob_dump_job_ticket( smjob_ticket_t *ticket );
void _smjob_dump_doc_ticket( smjob_doc_ticket_t *ticket );
void _smjob_dump_list( const ATLISTENTRY *cur_list);
void _smjob_dump_stack();
void _smjob_dump_jobs_w_docs_list();
void _smjob_dump_jobs_wo_docs_list();
void _smjob_dump_jobs_held_list();
void _smjob_dump_jobs_done_list();
void _smjob_dump_jobs_stack_list();

//=============================================================================
// Local functions
//=============================================================================

//-----------------------------------------------------------------------------
void _smjob_dump_job_ticket( smjob_ticket_t *ticket )
{
    smjob_print_doc_processing_t print_proc;
    smjob_scan_doc_processing_t scan_proc;

    ASSERT( NULL != ticket );

    cmd_printf( "SM Job Ticket: %p\n", ticket );
    cmd_printf( "\t  job type: %s\n", smjob_type_to_string( ticket->job_type ) );
    cmd_printf( "\t  job name: %s\n", ticket->job_description.job_name );
    cmd_printf( "\t  originating user name: %s\n", ticket->job_description.originating_user_name );
    cmd_printf( "\t  orientation: %s\n", smjob_orientation_to_string( ticket->doc_processing.orientation ) );
    cmd_printf( "\t  resolution width: %d\n", ticket->doc_processing.resolution.xfeed_dir );
    cmd_printf( "\t  resolution height: %d\n", ticket->doc_processing.resolution.feed_dir );
    cmd_printf( "\t  resolution units: %s\n", smjob_resunits_to_string( ticket->doc_processing.resolution.units ) );
    if ( ( SMJOB_TYPE_PRINT == ticket->job_type ) |
         ( SMJOB_TYPE_FAXOUT == ticket->job_type ) )
    {
        print_proc = ticket->doc_processing.type.print;
        cmd_printf( "\t  print processing copies: %d\n", print_proc.copies );
        cmd_printf( "\t  print processing media size: %d\n", print_proc.media_size );
        cmd_printf( "\t  print processing media type: %d\n", print_proc.media_type );
        cmd_printf( "\t  print processing sheet collate: %s\n", print_proc.sheet_collate ? "TRUE" : "FALSE" );
        cmd_printf( "\t  print processing sides: %s\n", print_proc.sides );
        cmd_printf( "\t  print processing quality: %s\n", smjob_quality_to_string( print_proc.quality ) );
    }
    else // SMJOB_TYPE_UNSPECIFIED, SMJOB_TYPE_SCAN, SMJOB_TYPE_FAXIN
    {
        scan_proc = ticket->doc_processing.type.scan;
        cmd_printf( "\t  scan processing input source: %s\n", smjob_input_source_to_string( scan_proc.input_source ) );
        cmd_printf( "\t  scan processing compression: %s\n", scan_proc.compression );
        cmd_printf( "\t  scan processing format: %s\n", scan_proc.format );
    }

    #if 0
    smjob_attribute_mask_t     must_honor;
    smjob_attribute_mask_t     omitted;
    #endif

    cmd_printf( "\t  Vendor Extension: %p\n", ticket->vnd_extensions );
}

//-----------------------------------------------------------------------------
void _smjob_dump_doc_ticket( smjob_doc_ticket_t *ticket )
{
    ASSERT( NULL != ticket );

    smjob_print_doc_description_t *print_desc;
    smjob_print_doc_processing_t *print_proc;
    int i;

    cmd_printf( "\nSM Document Ticket: %p\n", ticket );

    // Print the doc description values
    print_desc = &ticket->doc_description.type.print;
    cmd_printf( "\n  Document Description: \n" );
    cmd_printf( "\tDoc Name: %s\n", ticket->doc_description.doc_name );
    cmd_printf( "\tType:\n" );
    cmd_printf( "\t\tPrint Compression: %s\n", print_desc->compression );
    cmd_printf( "\t\tPrint Format: %s\n", print_desc->format );
    cmd_printf( "\tVendor Extensions: %p\n", ticket->doc_description.vnd_extensions );

    // Print the doc processing values
    print_proc = &ticket->doc_processing.type.print; // NOTE: not incl type=smjob_scan_doc_processing_t scan
    cmd_printf( "\n  Document Processing: \n" );
    cmd_printf( "\tOrientation: %s\n",
                smjob_orientation_to_string( ticket->doc_processing.orientation ) );
    cmd_printf( "\tResolution: \n" );
    cmd_printf( "\t\tWidth: %ld\n", (long)ticket->doc_processing.resolution.xfeed_dir );
    cmd_printf( "\t\tHeight: %ld\n", (long)ticket->doc_processing.resolution.feed_dir );
    cmd_printf( "\t\tUnits: %p\n", smjob_resunits_to_string( ticket->doc_processing.resolution.units ) );
    cmd_printf( "\tType:\n");
    cmd_printf( "\t\tPrint Copies: %ld\n", (long)print_proc->copies );
    cmd_printf( "\t\tPrint Media Size: %d\n", print_proc->media_size );
    cmd_printf( "\t\tPrint Media Type: %d\n", print_proc->media_type );
    cmd_printf( "\t\tPrint Sheets Collate: %d\n", print_proc->sheet_collate );
    cmd_printf( "\t\tPrint Sides: %s\n", print_proc->sides );
    cmd_printf( "\t\tPrint Quality: %s\n", smjob_quality_to_string( print_proc->quality ) );
    cmd_printf( "\tVendor Extensions: %p\n\n", ticket->doc_processing.vnd_extensions );

    // Print the attributes
    for ( i = 0; i < SMJOB_NUM_ATTRIBUTE_GROUPS; i++ )
    {
        cmd_printf( "  Attributes Must Honor[%d]: %ld \n", i, (long)ticket->must_honor.mask[i] );
        cmd_printf( "  Attributes Omitted [%d]: %ld \n", i, (long)ticket->omitted.mask[i] );
    }

    // Print any vendor extensions
    cmd_printf( "\n  Vendor Extensions: %p \n", ticket->vnd_extensions );

}

//-----------------------------------------------------------------------------
void _smjob_dump_list( const ATLISTENTRY *cur_list)
{
    const ATLISTENTRY *list_entry;
    smjob_t *smjob;
    int i;

    list_entry = ATListHead((ATLISTENTRY *)cur_list);

    while (list_entry)
    {
        smjob = CONTAINING_RECORD(list_entry, struct smjob_s, listnode);
        ASSERT(smjob);

        cmd_printf( "\n" );
        cmd_printf( "Job Entry: \n" );

        cmd_printf( "\tjob_event_mask:        %d\n", smjob->job_event_mask );
        cmd_printf( "\tdocument:              %s\n", smjob->sm_doc ? "document present" : "no document" );
#ifndef NET_SM_STREAMING
        cmd_printf( "\tbytes read:            %ld\n", smjob->bytes_read );
#endif

        cmd_printf( "\n");
        cmd_printf( "Job Status:\n");

        cmd_printf( "\tjob ID:                %d\n", smjob->job_status->job_id);
        cmd_printf( "\tnum docs:              %d\n", smjob->job_status->num_docs);
        cmd_printf( "\tjob state:             %s\n", smjob_state_to_string(smjob->job_status->state));
        cmd_printf( "\tjob state num reasons: %d\n", smjob->job_status->num_reasons);
        for ( i = 0; i < smjob->job_status->num_reasons; i++ )
        {
            cmd_printf( "\tjob state reason:      %s\n", smjob_reason_to_string(smjob->job_status->reasons[i]));
        }
        cmd_printf( "\tkoctets processed:     %d\n", smjob->job_status->koctets_processed);
        cmd_printf( "\tsheets completed:      %d\n", smjob->job_status->sheets_completed);

        cmd_printf("\n");
        _smjob_dump_job_ticket( smjob->job_ticket );

        list_entry = ATListNext((ATLISTENTRY *)cur_list, (ATLISTENTRY *)list_entry);

        if ( list_entry )
        {
            cmd_printf("----------------------------------------------------- \n");
        }
    }

    cmd_printf( "\n" );

}

//-----------------------------------------------------------------------------
void _smjob_dump_stack()
{
    uint32_t job_id;
    smjob_t *smjob;
    smjob_rcode_t sm_res;
    int i;

    // Get the first job
    // Set the job_id to SMJOB_INVALID_JOB_ID for first call to smjob_mgr_get_next_job
    // in order to get the first valid job
    job_id = SMJOB_INVALID_JOB_ID;
    job_id = smjob_get_next( job_id );
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        // Stack is empty
        return;
    }
    sm_res = smjob_mgr_job_lookup_stack ( job_id, &smjob ); // NOTE: Not a public API function
    if ( SMJOB_OK != sm_res )
    {
        //cmd_printf("%s: Failure getting job for job_id = %ld \n", __func__, job_id);
        return;
    }

    // Print out the info for this job
    while ( ( smjob ) && ( SMJOB_OK == sm_res ) )
    {
        cmd_printf( "\n" );
        cmd_printf( "Job Entry: \n" );

        cmd_printf( "\tjob_event_mask:        %d\n", smjob->job_event_mask );
        cmd_printf( "\tdocument:              %s\n", smjob->sm_doc ? "document present" : "no document" );
#ifndef NET_SM_STREAMING
        cmd_printf( "\tbytes read:            %ld\n", smjob->bytes_read );
#endif

        cmd_printf( "\n");
        cmd_printf( "Job Status:\n");

        cmd_printf( "\tjob ID:                %d\n", smjob->job_status->job_id);
        cmd_printf( "\tnum docs:              %d\n", smjob->job_status->num_docs);
        cmd_printf( "\tjob state:             %s\n", smjob_state_to_string(smjob->job_status->state));
        cmd_printf( "\tjob state num reasons: %d\n", smjob->job_status->num_reasons);
        for ( i = 0; i < smjob->job_status->num_reasons; i++ )
        {
            cmd_printf( "\tjob state reason:      %s\n", smjob_reason_to_string(smjob->job_status->reasons[i]));
        }
        cmd_printf( "\tkoctets processed:     %d\n", smjob->job_status->koctets_processed);
        cmd_printf( "\tsheets completed:      %d\n", smjob->job_status->sheets_completed);

        cmd_printf("\n");
        _smjob_dump_job_ticket( smjob->job_ticket );

        // Get the next job
        job_id = smjob_get_next( job_id );
        if ( SMJOB_INVALID_JOB_ID != job_id )
        {
            sm_res = smjob_mgr_job_lookup_stack ( job_id, &smjob ); // NOTE: Not a public API function
            cmd_printf("----------------------------------------------------- \n");
        }
        else
        {
            // Found the end of stack; get out of while loop
            sm_res = SMJOB_FAIL;
        }
    }

    cmd_printf("\n");

}

//-----------------------------------------------------------------------------
void _smjob_dump_jobs_w_docs_list()
{
    cmd_printf("\n");
    cmd_printf("------------Start of JOBS_W_DOCS_LIST------------ \n");

    _smjob_dump_list( smjob_mgr_get_requested_list( JOBS_W_DOCS_LIST) ); // NOTE: Not a public API function

    cmd_printf("\n");
    cmd_printf("------------End of JOBS_W_DOCS_LIST------------ \n");
}

//-----------------------------------------------------------------------------
void _smjob_dump_jobs_wo_docs_list()
{
    cmd_printf("\n");
    cmd_printf("------------Start of JOBS_WO_DOCS_LIST------------ \n");

    _smjob_dump_list( smjob_mgr_get_requested_list( JOBS_WO_DOCS_LIST) ); // NOTE: Not a public API function

    cmd_printf("\n");
    cmd_printf("------------End of JOBS_WO_DOCS_LIST------------ \n");
}

//-----------------------------------------------------------------------------
void _smjob_dump_jobs_held_list()
{
    cmd_printf("\n");
    cmd_printf("------------Start of JOBS_HELD_LIST------------ \n");

    _smjob_dump_list( smjob_mgr_get_requested_list( JOBS_HELD_LIST) ); // NOTE: Not a public API function

    cmd_printf("\n");
    cmd_printf("------------End of JOBS_HELD_LIST------------ \n");
}

//-----------------------------------------------------------------------------
void _smjob_dump_jobs_stack_list()
{
    cmd_printf("\n");
    cmd_printf("------------Start of Job History Stack List------------ \n");

    _smjob_dump_stack();

    cmd_printf("\n");
    cmd_printf("------------End of Job History Stack List------------ \n");
}

//-----------------------------------------------------------------------------
void _smjob_dump_jobs_done_list()
{
    cmd_printf("\n");
    cmd_printf("------------Start of COMPLETED_JOBS_LIST------------ \n");

    _smjob_dump_list( smjob_pif_get_requested_list( COMPLETED_JOBS_LIST) ); // NOTE: Not a public API function

    cmd_printf("\n");
    cmd_printf("------------End of COMPLETED_JOBS_LIST------------ \n");
}

//=============================================================================
// Callback functions
//=============================================================================

//-----------------------------------------------------------------------------
/** 
 * Notify the user of asynchronous job status events.
 *  
 * The device will use this callback to notify the user of job related status changes. The
 * user can then request current job status via smjob_get_status() to get the status 
 * changes indicated by the 'job_events' flags. 
 *  
 * The user may query job status from the context of this callback.
 *  
 * @param job_id - job for which the status change applies 
 *  
 * @param job_events - a subset of the status event flags requested by the user during 
 *        callback registration.
 *  
 * @param [in] user_data - The opaque user context provided in the call to 
 *       smjob_create_job().
 */
static void status_func_CB_dbg( uint32_t job_id, smjob_status_events_t job_events, 
                                void *user_data )
{
    smjob_rcode_t sm_res;
    int8_t i;

    // To get status for a job, can either poll with smjob_get_status (very slow), 
    // or can register a callback function with smjob_create.

    DBG_PRINTF_ERR("%s: job_id = %d, job_events = %d \n", __func__, job_id, (uint32_t)job_events );
    sm_res = smjob_get_status( job_id, &g_job_status );
    if ( SMJOB_OK != sm_res )
    {
        cmd_printf("%s: ERROR, couldn't get g_job_status \n", __func__ );
    }
    else
    {
        // Did an event happen that we are interested in?
        if ( 0 < ( SMJOB_STATUS_EVENT_STATE & job_events ) )
        {
            DBG_PRINTF_ERR("%s: EVENT_STATE: state is %s \n", __func__, 
                            smjob_state_to_string( g_job_status.state ) );
            switch ( g_job_status.state )
            {
                    case SMJOB_STATE_PENDING:
                        break;
                    case SMJOB_STATE_PENDING_HELD:
                        break;
                    case SMJOB_STATE_PROCESSING:
                        break;
                    case SMJOB_STATE_PROCESSING_STOPPED:
                        break;
                    case SMJOB_STATE_CANCELED:
                    case SMJOB_STATE_ABORTED:
                    case SMJOB_STATE_COMPLETED:
                        // Clean up the memory allocations
                        DBG_PRINTF_ERR("\n %s: Memory clean \n", __func__);
                        MEM_FREE_AND_NULL( g_job_name );
                        MEM_FREE_AND_NULL( g_job_user_name );
                        if(g_job_ticket)
                        {
                            MEM_FREE_AND_NULL( g_job_ticket );
                        }
                        if(g_doc_ticket)
                        {
                            MEM_FREE_AND_NULL( g_doc_ticket );
                        }
                        break;
                    default:
                        break;
            }
        }

        if ( 0 < ( SMJOB_STATUS_EVENT_STATE_REASONS & job_events ) )
        {
            DBG_PRINTF_ERR("%s: EVENT_STATE_REASONS: num_reasons = %ld, ", __func__,
                            g_job_status.num_reasons );
            for ( i = 0; i < g_job_status.num_reasons; i++ )
            {
                DBG_PRINTF_ERR("reason = %s \n", smjob_reason_to_string( g_job_status.reasons[i] ) );
            }
        }

        if ( 0 < ( SMJOB_STATUS_EVENT_KOCTETS_PROCESSED & job_events ) )
        {
            DBG_PRINTF_ERR("%s: EVENT_STATE_KOCTETS_PROCESSED: koctets_processed = %ld \n", __func__,
                            g_job_status.koctets_processed );
        }

        if ( 0 < ( SMJOB_STATUS_EVENT_SHEETS_COMPLETED & job_events ) )
        {
            DBG_PRINTF_ERR("%s: EVENT_STATE_SHEETS_COMPLETED: sheets_completed = %ld \n", __func__, 
                            g_job_status.sheets_completed );
        }

    }

}

//-----------------------------------------------------------------------------
static int32_t recv_func_CB_dbg( char *buf, uint32_t len, uint32_t *timeout_ms, void *user_data )
{
    int fd;
    int32_t bytes_read = 0;

    // Copy len bytes into *buf, and return num bytes (which could/should = len) or 0 if EOF,
    // or -1 if error or timeout occurred.
    
    fd = open(g_file_name, O_RDONLY);

    if (fd != -1)
    {
        bytes_read = read(fd, buf, len);
    }
    else
    {
        cmd_printf("%s ERROR : Could not open file: %s\n", __func__, g_file_name);
        DBG_PRINTF_ERR("%s ERROR : Could not open file: %s\n", __func__, g_file_name);
    }

    close(fd);

    cmd_printf("%s: read %ld bytes from file: %s\n", __func__, (long)bytes_read, g_file_name);
    DBG_PRINTF_ERR("%s: read %ld bytes from file: %s\n", __func__, bytes_read, g_file_name);

    return bytes_read;
}

//=============================================================================
// External commands
//=============================================================================

//-----------------------------------------------------------------------------
static int smjob_cmd_test_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_printf( "This is a test, only a test\n" );

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_compression_cb( int argc, char *argv[] )
{
    int cmd_res;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_type_t job_type = SMJOB_TYPE_UNSPECIFIED;
    smjob_compression_t *value = NULL;
    uint32_t index = 0;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( 0 == ( strcmp(argv[1], "PRINT") ) )
    {
        job_type = SMJOB_TYPE_PRINT;
    }
    else if ( 0 == ( strcmp(argv[1], "SCAN") ) )
    {
        job_type = SMJOB_TYPE_SCAN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXIN") ) )
    {
        job_type = SMJOB_TYPE_FAXIN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXOUT") ) )
    {
        job_type = SMJOB_TYPE_FAXOUT;
    }
    else
    {
        job_type = SMJOB_TYPE_UNSPECIFIED;
    }

    cmd_res = str_mkint( argv[2], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[2]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_compression(job_type, index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: compression: %s \n", __func__, value);
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_format_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_type_t job_type = SMJOB_TYPE_UNSPECIFIED;
    smjob_format_t *value = NULL;
    uint32_t index;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( 0 == ( strcmp(argv[1], "PRINT") ) )
    {
        job_type = SMJOB_TYPE_PRINT;
    }
    else if ( 0 == ( strcmp(argv[1], "SCAN") ) )
    {
        job_type = SMJOB_TYPE_SCAN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXIN") ) )
    {
        job_type = SMJOB_TYPE_FAXIN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXOUT") ) )
    {
        job_type = SMJOB_TYPE_FAXOUT;
    }
    else
    {
        job_type = SMJOB_TYPE_UNSPECIFIED;
    }

    cmd_res = str_mkint( argv[2], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[2]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_format(job_type, index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: format: %s \n", __func__, value?value:"NULL" );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_orientation_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_type_t job_type = SMJOB_TYPE_UNSPECIFIED;
    smjob_orientation_t value = SMJOB_ORIENTATION_PORTRAIT;
    uint32_t index = 0;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( 0 == ( strcmp(argv[1], "PRINT") ) )
    {
        job_type = SMJOB_TYPE_PRINT;
    }
    else if ( 0 == ( strcmp(argv[1], "SCAN") ) )
    {
        job_type = SMJOB_TYPE_SCAN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXIN") ) )
    {
        job_type = SMJOB_TYPE_FAXIN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXOUT") ) )
    {
        job_type = SMJOB_TYPE_FAXOUT;
    }
    else
    {
        job_type = SMJOB_TYPE_UNSPECIFIED;
    }

    cmd_res = str_mkint( argv[2], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[2]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_orientation(job_type, index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: orientation: %s \n", __func__, smjob_orientation_to_string( value ) );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_resolution_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_type_t job_type = SMJOB_TYPE_UNSPECIFIED;
    smjob_resolution_t value = {600,600,SMJOB_RES_UNIT_DOTS_PER_INCH};
    uint32_t index = 0;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( 0 == ( strcmp(argv[1], "PRINT") ) )
    {
        job_type = SMJOB_TYPE_PRINT;
    }
    else if ( 0 == ( strcmp(argv[1], "SCAN") ) )
    {
        job_type = SMJOB_TYPE_SCAN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXIN") ) )
    {
        job_type = SMJOB_TYPE_FAXIN;
    }
    else if ( 0 == ( strcmp(argv[1], "FAXOUT") ) )
    {
        job_type = SMJOB_TYPE_FAXOUT;
    }
    else
    {
        job_type = SMJOB_TYPE_UNSPECIFIED;
    }

    cmd_res = str_mkint( argv[2], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[2]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_resolution(job_type, index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: resolution width: %ld \n", __func__, (long)value.xfeed_dir );
        cmd_printf( "%s: resolution height: %ld \n", __func__, (long)value.feed_dir );
        cmd_printf( "%s: resolution units: %s \n", __func__, smjob_resunits_to_string( value.units ) );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_input_source_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_input_source_t value = SMJOB_INPUT_SOURCE_ADF;
    uint32_t index = 0;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_res = str_mkint( argv[1], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[1]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_input_source(index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: input_source: %s \n", __func__, smjob_input_source_to_string( value ) );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_copies_cb( int argc, char *argv[] )
{
    int cmd_res = CMD_OK;
    smjob_rcode_t sm_res = SMJOB_OK;
    uint32_t min_value = 0;
    uint32_t max_value = 0;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_copies(&min_value, &max_value);
    if ( SMJOB_FAIL == sm_res )
    {
        cmd_printf( "%s: ERROR: \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: min copies = %ld, max copies = %ld \n", __func__, 
                    (long)min_value, (long)max_value );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_collate_cb( int argc, char *argv[] )
{
    int cmd_res = CMD_OK;
    smjob_rcode_t sm_res = SMJOB_OK;
    bool value = FALSE;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_sheet_collate(&value);
    if ( SMJOB_FAIL == sm_res )
    {
        cmd_printf( "%s: ERROR: \n", __func__ );
    }
    else
    {
        if ( TRUE == value )
        {
            cmd_printf( "%s: sheet collate is TRUE \n", __func__ );
        }
        else
        {
            cmd_printf( "%s: sheet collate is FALSE \n", __func__ );
        }
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_sides_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_sides_t *value = NULL;
    uint32_t index = 0;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_res = str_mkint( argv[1], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[1]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_sides(SMJOB_TYPE_PRINT, index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: sides: %s \n", __func__, value );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_get_quality_cb( int argc, char *argv[] )
{
    int cmd_res = 0;
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_quality_t value = SMJOB_QUALITY_DRAFT;
    uint32_t index = 0;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_res = str_mkint( argv[1], &index );
    if ( 0 != cmd_res )
    {
        cmd_printf("%s ERROR : Invalid index = %s \n", __func__, argv[1]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_supported_quality(index, &value);
    if ( SM_BAD_INDEX == sm_res )
    {
        cmd_printf( "%s: ERROR: bad index \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: quality: %s \n", __func__, smjob_quality_to_string(value) );
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_enable_on_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    smjob_mgr_turn_on_printing(); // NOTE: Not a public API function
    cmd_printf( "%s: Printing has been turned ON \n", __func__ );

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_enable_off_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    smjob_mgr_turn_off_printing(); // NOTE: Not a public API function
    cmd_printf( "%s: Printing has been turned OFF \n", __func__ );

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_enable_query_cb( int argc, char *argv[] )
{
    bool query; 

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    query = smjob_mgr_query_print_state(); // NOTE: Not a public API function
    cmd_printf( "%s: Printing is %s \n", __func__, query ? "OFF" : "ON" );

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_create_job( int argc, char *argv[] )
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_status_events_t flagged_events = SMJOB_STATUS_EVENT_STATE;
    int cmd_res = 0;
    uint32_t name_len;
    uint32_t job_id;

    // NOTE: could change to accept job_type argument
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    // Build the g_job_name
    name_len = strlen(BASE_JOB_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_name);
    sprintf(g_job_name, "%s_%lu", BASE_JOB_NAME, (long unsigned)g_debug_job_number);

    // Build the g_job_user_name
    name_len = strlen(BASE_JOB_USER_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_user_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_user_name);
    sprintf(g_job_user_name, "%s_%lu", BASE_JOB_USER_NAME, (long unsigned)g_debug_job_number);

    ++g_debug_job_number;

    //----------------------------------------------
    // Get a job ticket
    //----------------------------------------------

    // Get the memory for the job ticket (user owns storage for the job ticket)
    g_job_ticket = (smjob_ticket_t *)MEM_MALLOC(sizeof(smjob_ticket_t));
    ASSERT( g_job_ticket );
    memset( g_job_ticket, 0, sizeof(smjob_ticket_t) );
    
    // Get a default job ticket
    sm_res = smjob_get_default_ticket( SMJOB_TYPE_PRINT, g_job_ticket );
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
    }
    else
    {
        cmd_res = CMD_ERROR;
    }

    // Edit the job ticket
    // NOTE: can use the smjob_get_supported_* functions here to see the device capabilities
    //       for options to edit the ticket
    strncpy( g_job_ticket->job_description.job_name, g_job_name, SMJOB_MAX_NAME_LEN );
    g_job_ticket->job_description.job_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    strncpy( g_job_ticket->job_description.originating_user_name, g_job_user_name, SMJOB_MAX_NAME_LEN );
    g_job_ticket->job_description.originating_user_name[SMJOB_MAX_NAME_LEN-1] = '\0';
       
    // Print out the results
    _smjob_dump_job_ticket( g_job_ticket );

    //----------------------------------------------
    // Get a job id
    //----------------------------------------------

    // Set the flagged_events; this is what we want our status_func_CB_dbg to respond to
    // Can OR together multiple events
    //flagged_events = SMJOB_STATUS_EVENT_STATE;

    // Get a job id
    job_id = smjob_create( g_job_ticket, status_func_CB_dbg, flagged_events, NULL );
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        cmd_res = CMD_ERROR;
        cmd_printf("%s ERROR : Invalid job_id = %d returned from smjob_create \n", __func__, job_id);
        return cmd_res;
    }
    else
    {
        cmd_printf("%s job_id = %d returned from smjob_create \n", __func__, job_id);
        cmd_res = CMD_OK;
    }
    // NOTE: could remove g_job_ticket memory now

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_cancel_current( int argc, char *argv[] )
{
    smjob_rcode_t sm_res;
    uint32_t jobid;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    jobid = smjob_pif_get_cur_print_job_id(); // NOTE: Not a public API function
    if ( ( SMJOB_INVALID_JOB_ID == jobid ) |
         ( INITIAL_JOB_ID_VALUE > jobid ) |
         ( MAX_JOB_ID_VALUE < jobid ) )
    {
        cmd_printf("%s ERROR : Invalid current jobid = %d (%s) \n", __func__, jobid, (argv[1]?argv[1]:"<unbound>"));
        return CMD_ERROR;
    }

    sm_res = smjob_cancel( jobid );

    if ( SMJOB_OK == sm_res )
    {
        cmd_printf( "%s: Successfully cancelled current job \n", __func__ );
    }
    else
    {
        cmd_printf( "%s: ERROR Could not cancel current job \n", __func__ );
    }

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_cancel_jobid( int argc, char *argv[] )
{
    int cmd_res = CMD_OK;
    smjob_rcode_t sm_res;
    uint32_t jobid;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_res = str_mkint( argv[1], &jobid );
    if ( CMD_OK != cmd_res )
    {
        cmd_printf("%s ERROR : Bad jobid = %s \n", __func__, argv[1]);
        return CMD_USAGE_ERROR;
    }

    if ( ( SMJOB_INVALID_JOB_ID == jobid ) |
         ( INITIAL_JOB_ID_VALUE > jobid ) |
         ( MAX_JOB_ID_VALUE < jobid ) )
    {
        cmd_printf("%s ERROR : Invalid jobid = %s \n", __func__, argv[1]);
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_cancel( jobid );

    if ( SMJOB_OK == sm_res )
    {
        cmd_printf( "%s: Successfully cancelled job with ID = %ld \n", __func__, (long)jobid );
    }
    else
    {
        cmd_printf( "%s: ERROR Could not cancel job with ID = %ld \n", __func__, (long)jobid );
    }


    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_jwd( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_w_docs_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_jnd( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_wo_docs_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_held( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_held_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_done( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_done_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_stack( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_stack_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_dump_all( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    _smjob_dump_jobs_w_docs_list();
    _smjob_dump_jobs_wo_docs_list();
    _smjob_dump_jobs_held_list();
    _smjob_dump_jobs_done_list();
    _smjob_dump_jobs_stack_list();

    return CMD_OK;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_ticket_default_print_cb( int argc, char *argv[] )
{
    smjob_ticket_t job_ticket;
    smjob_rcode_t sm_res;
    int cmd_res;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    sm_res = smjob_get_default_ticket( SMJOB_TYPE_PRINT, &job_ticket );
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
        _smjob_dump_job_ticket( &job_ticket );
    }
    else
    {
        cmd_res = CMD_ERROR;
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_default_document_print_cb( int argc, char *argv[] )
{
    smjob_rcode_t sm_res;
    smjob_ticket_t job_ticket;
    smjob_doc_ticket_t doc_ticket;
    smjob_status_events_t job_event_mask;
    uint32_t job_id;
    int cmd_res;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    // Get a default job ticket
    sm_res = smjob_get_default_ticket( SMJOB_TYPE_PRINT, &job_ticket );
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
        _smjob_dump_job_ticket( &job_ticket );
    }
    else
    {
        cmd_res = CMD_ERROR;
        cmd_printf("%s ERROR : Couldn't get default ticket \n", __func__);
        return cmd_res;
    }

    // Need to set job_type in the job ticket
    job_ticket.job_type = SMJOB_TYPE_PRINT;

    // Get a job id
    job_event_mask = 0;
    job_id = smjob_create( &job_ticket, status_func_CB_dbg, job_event_mask, NULL );
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        cmd_res = CMD_ERROR;
        cmd_printf("%s ERROR : Invalid job_id returned from smjob_create \n", __func__);
        return cmd_res;
    }
    else
    {
        cmd_res = CMD_OK;
    }

    // Get the default doc
    sm_res = smjob_get_default_doc_ticket(job_id, &doc_ticket);
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
        _smjob_dump_doc_ticket( &doc_ticket );
    }
    else
    {
        cmd_res = CMD_ERROR;
    }

    sm_res = smjob_cancel( job_id );

    return cmd_res;
    
}

//-----------------------------------------------------------------------------
static int smjob_cmd_print_test( int argc, char *argv[] )
{
    smjob_rcode_t sm_res;
    smjob_status_events_t flagged_events;
    int cmd_res;
    uint32_t name_len;
    uint32_t job_id;
    uint32_t doc_id;
    void *user_context = NULL;
    bool is_jpeg = FALSE;
//    bool is_other = FALSE;
    char *suffix;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    // Get the filename and determine file type by the suffix
    name_len = strlen(argv[1]) + 1;
    g_file_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_file_name);
    sprintf(g_file_name, "%s/%s", FILE_PATH_TMP, argv[1]);
    suffix = strrchr( g_file_name, '.' );
    suffix++; // Swallow the "."
cmd_printf("%s SUFFIX = %s \n", __func__, suffix);
    if ( ( 0 == strcasecmp( suffix, "jpeg" ) ) || 
         ( 0 == strcasecmp( suffix, "jpg" ) ) )
    {
        is_jpeg = TRUE;
cmd_printf("%s is_jpeg is TRUE \n", __func__ );
    }
    else
    {
    //    is_other = TRUE;
cmd_printf("%s is_other is TRUE \n", __func__ );
    }

    // Build the g_job_name
    name_len = strlen(BASE_JOB_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_name);
    sprintf(g_job_name, "%s_%lu", BASE_JOB_NAME, (long unsigned)g_debug_job_number);

    // Build the g_job_user_name
    name_len = strlen(BASE_JOB_USER_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_user_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_user_name);
    sprintf(g_job_user_name, "%s_%lu", BASE_JOB_USER_NAME, (long unsigned)g_debug_job_number);

    ++g_debug_job_number;

    /* Normal flow for smjob:
    *
    *    Get a default job ticket: 
    *      --> smjob_get_default_ticket
    *    Edit the default job ticket  
    *       * Add job name and job user name
    *    Get a job id with the job ticket: 
    *      --> smjob_create    
    *    Get a default document ticket with the job id: 
    *      --> smjob_get_default_document
    *        * Can repeat for multiple documents
    *    Edit the default document ticket
    *      + Add filename
    *      + Add file type
    *      + Attach receive function
    *    When data is available, add the document to the job: 
    *      --> smjob_add_document
    *    
    */

    //----------------------------------------------
    // Get a job ticket
    //----------------------------------------------

    // Get the memory for the job ticket (user owns storage for the job ticket)
    g_job_ticket = (smjob_ticket_t *)MEM_MALLOC(sizeof(smjob_ticket_t));
    ASSERT( g_job_ticket );
    memset( g_job_ticket, 0, sizeof(smjob_ticket_t) );
    
    // Get a default job ticket
    sm_res = smjob_get_default_ticket( SMJOB_TYPE_PRINT, g_job_ticket );
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
    }
    else
    {
        cmd_res = CMD_ERROR;
    }

    // Edit the job ticket
    // NOTE: can use the smjob_get_supported_* functions here to see the device capabilities
    //       for options to edit the ticket
    strncpy( g_job_ticket->job_description.job_name, g_job_name, SMJOB_MAX_NAME_LEN );
    g_job_ticket->job_description.job_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    strncpy( g_job_ticket->job_description.originating_user_name, g_job_user_name, SMJOB_MAX_NAME_LEN );
    g_job_ticket->job_description.originating_user_name[SMJOB_MAX_NAME_LEN-1] = '\0';
       
    // Print out the results
    _smjob_dump_job_ticket( g_job_ticket );

    //----------------------------------------------
    // Get a job id
    //----------------------------------------------

    // Set the flagged_events; this is what we want our status_func_CB_dbg to respond to
    // Can OR together multiple events
    flagged_events = SMJOB_STATUS_EVENT_STATE;

    // Get a job id
    job_id = smjob_create( g_job_ticket, status_func_CB_dbg, flagged_events, NULL );
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        cmd_res = CMD_ERROR;
        cmd_printf("%s ERROR : Invalid job_id returned from smjob_create \n", __func__);
        return cmd_res;
    }
    else
    {
        cmd_res = CMD_OK;
    }
    // NOTE: could remove g_job_ticket memory now

    //----------------------------------------------
    // Get a doc handle
    //----------------------------------------------

    // Get mem for the *doc_processing that's in the doc ticket and the doc ticket itself
    g_doc_ticket = (smjob_doc_ticket_t *)MEM_MALLOC(sizeof(smjob_doc_ticket_t));
    memset(g_doc_ticket, 0, sizeof(smjob_doc_ticket_t));
    ASSERT( g_doc_ticket );

    // Get a default document ticket 
    sm_res = smjob_get_default_doc_ticket(job_id, g_doc_ticket);
    if ( SMJOB_OK == sm_res )
    {
        cmd_res = CMD_OK;
    }
    else
    {
        cmd_res = CMD_ERROR;
    }

    // Edit the doc ticket 
    // NOTE: can use the smjob_get_supported_* functions here to see the device capabilities
    //       for options to edit the ticket
    strncpy( g_doc_ticket->doc_description.doc_name, g_file_name, SMJOB_MAX_NAME_LEN );
    g_doc_ticket->doc_description.doc_name[SMJOB_MAX_NAME_LEN-1] = '\0';

    strcpy(g_doc_ticket->doc_description.type.print.compression, SMJOB_COMPRESSION_NONE);
    if ( TRUE == is_jpeg )
    {
        strcpy(g_doc_ticket->doc_description.type.print.format, SMJOB_FORMAT_IMAGE_JPEG);
    }
    g_doc_ticket->doc_description.type.print.recv_func = recv_func_CB_dbg;
    g_doc_ticket->doc_description.type.print.user_data = user_context;
    
    // Print out the results
    _smjob_dump_doc_ticket( g_doc_ticket );

    //----------------------------------------------
    // Add the doc handle to the job handle
    //----------------------------------------------

    // Get initial data
    //   Cannot call smjob_add_document until we have some data. 
    // 
    //   NOTE: Here we "cheat" since we are not streaming data; 
    //         instead point to a pre-existing file on /tmp/ 

    // Add the document to the job; this will start the pif thread to talk
    // with the net_io layer, and print the document.
    doc_id = smjob_add_document( job_id, g_doc_ticket, TRUE );
    // NOTE: could remove g_doc_ticket memory now
    if ( SMJOB_INVALID_DOC_ID == doc_id )
    {
        cmd_printf("%s ERROR : Invalid doc_id returned from smjob_create \n", __func__);
        cmd_res = CMD_ERROR;
    }
    else
    {
        cmd_res = CMD_OK;
    }

    return cmd_res;
}

//-----------------------------------------------------------------------------
static int smjob_cmd_scan_test( int argc, char *argv[] )
{
    smjob_rcode_t sm_res;
    smjob_status_events_t event_flags;
    int cmd_res = CMD_OK;
    uint32_t name_len;
    uint32_t job_id;
    smjob_ticket_t ticket;

    if ( argc != 1 )
    {
        cmd_res = CMD_USAGE_ERROR;
        goto done;
    }

    // Build the g_job_name
    name_len = strlen(BASE_JOB_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_name);
    sprintf(g_job_name, "%s_%ulu", BASE_JOB_NAME, g_debug_job_number);

    // Build the g_job_user_name
    name_len = strlen(BASE_JOB_USER_NAME) + DEBUG_JOB_NUM_LEN + 1;
    g_job_user_name = MEM_CALLOC(name_len, 1);
    ASSERT(g_job_user_name);
    sprintf(g_job_user_name, "%s_%ulu", BASE_JOB_USER_NAME, g_debug_job_number);

    ++g_debug_job_number;

    //----------------------------------------------
    // Get a job ticket
    //----------------------------------------------

    // Get a default job ticket
    sm_res = smjob_get_default_ticket( SMJOB_TYPE_SCAN, &ticket );
    if ( SMJOB_OK != sm_res )
    {
        cmd_res = CMD_ERROR;
        goto done;
    }

    // Edit the job ticket
    // NOTE: can use the smjob_get_supported_* functions here to see the device capabilities
    //       for options to edit the ticket
    strncpy( ticket.job_description.job_name, g_job_name, SMJOB_MAX_NAME_LEN );
    ticket.job_description.job_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    strncpy( ticket.job_description.originating_user_name, g_job_user_name, SMJOB_MAX_NAME_LEN );
    ticket.job_description.originating_user_name[SMJOB_MAX_NAME_LEN-1] = '\0';
       
    // Print out the results
    _smjob_dump_job_ticket( &ticket );

    //----------------------------------------------
    // Get a job id
    //----------------------------------------------

    // Set the flagged_events; this is what we want our status_func_CB_dbg to respond to
    // Can OR together multiple events
    event_flags = SMJOB_STATUS_EVENT_STATE | SMJOB_STATUS_EVENT_SCAN_DATA;

    // Get a job id
    job_id = smjob_create( &ticket, status_func_CB_dbg, event_flags, NULL );
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        cmd_printf("%s ERROR : smjob_create failed! \n", __func__);
        cmd_res = CMD_ERROR;
        goto done;
    }
    cmd_printf("%s : smjob_create job_id=%d \n", __func__, (unsigned int)job_id);

    uint32_t timeout_ms = 25000;
    uint32_t size = 0;
    uint32_t len = 0;
    uint8_t *buf;
    int i = 0;
    do
    {
        i++;
        size += len;
        sm_res = smjob_retrieve_image(job_id, &buf, &len, timeout_ms);
        memFree(buf);
        cmd_printf("####%d: len=%d \n", i, (unsigned int)len);
    }while(sm_res == SMJOB_OK && len>0);
    cmd_printf("%s : done retrieving image (size=%d, len=%d, sm_res=%d) \n", __func__, 
               (unsigned int)size, (unsigned int)len, (unsigned int)sm_res);

done:
    return cmd_res;
}
//-----------------------------------------------------------------------------
void smjob_cmd_init( void )
{
    int cmd_res;

    /* top-level command */
    cmd_res = cmd_register( "smjob", NULL, NULL, NULL, NULL );
    XASSERT( ( CMD_OK == cmd_res ), cmd_res );

    /* sanity test command */
    cmd_res = cmd_register( "smjob test",
                            "Run the smjob test",
                            NULL,
                            NULL,
                            smjob_cmd_test_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* job ticket commands */
    cmd_res = cmd_register( "smjob ticket",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob ticket default",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* command for displaying default job ticket */
    cmd_res = cmd_register( "smjob ticket default print",
                            "Display the default print job ticket",
                            NULL,
                            NULL,
                            smjob_cmd_ticket_default_print_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* doc ticket commands */
    cmd_res = cmd_register( "smjob doc",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob doc default",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* command for displaying default doc ticket */
    cmd_res = cmd_register( "smjob doc default print",
                            "Display the default print document ticket",
                            NULL,
                            NULL,
                            smjob_cmd_default_document_print_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* oem get_config commands */
    cmd_res = cmd_register( "smjob get_config",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config compression",
                            "Get the oem config settings",
                            "<PRINT|SCAN|FAXIN|FAXOUT> <#>",
                            "Must specify the job type (PRINT|SCAN|FAXIN|FAXOUT), and an index number",
                            smjob_cmd_get_compression_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config format",
                            "Get the oem config settings",
                            "<PRINT|SCAN|FAXIN|FAXOUT> <#>",
                            "Must specify the job type (PRINT|SCAN|FAXIN|FAXOUT), and an index number",
                            smjob_cmd_get_format_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config orientation",
                            "Get the oem config settings",
                            "<PRINT|SCAN|FAXIN|FAXOUT> <#>",
                            "Must specify the job type (PRINT|SCAN|FAXIN|FAXOUT), and an index number",
                            smjob_cmd_get_orientation_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config resolution",
                            "Get the oem config settings",
                            "<PRINT|SCAN|FAXIN|FAXOUT> <#>",
                            "Must specify the job type (PRINT|SCAN|FAXIN|FAXOUT), and an index number",
                            smjob_cmd_get_resolution_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config input_source",
                            "Get the oem config settings",
                            "<#>",
                            "Must specify an index number, scan only",
                            smjob_cmd_get_input_source_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config copies",
                            "Get the oem config settings",
                            NULL,
                            "Print only",
                            smjob_cmd_get_copies_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config collate",
                            "Get the oem config settings",
                            NULL,
                            "Print only",
                            smjob_cmd_get_collate_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config sides",
                            "Get the oem config settings",
                            "<#>",
                            "Must specify an index number, print only",
                            smjob_cmd_get_sides_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob get_config quality",
                            "Get the oem config settings",
                            "<#>",
                            "Must specify an index number, print only",
                            smjob_cmd_get_quality_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* turn printing on/off/query */
    cmd_res = cmd_register( "smjob enable_print",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob enable_print on",
                            "Turn printing on",
                            NULL,
                            NULL,
                            smjob_cmd_enable_on_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob enable_print off",
                            "Turn printing off",
                            NULL,
                            NULL,
                            smjob_cmd_enable_off_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob enable_print query",
                            "Query whether printing is on or off",
                            NULL,
                            NULL,
                            smjob_cmd_enable_query_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* create jobs */
    cmd_res = cmd_register( "smjob create",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob create job",
                            "Create a new job with a default job ticket",
                            NULL,
                            NULL,
                            smjob_cmd_create_job );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );
    
    /* cancel jobs */
    cmd_res = cmd_register( "smjob cancel",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob cancel current",
                            "Cancel the current print job",
                            NULL,
                            NULL,
                            smjob_cmd_cancel_current );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob cancel jobid",
                            "Cancel a particular job by job ID",
                            "<job ID number>",
                            NULL,
                            smjob_cmd_cancel_jobid );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* dump lists */
    cmd_res = cmd_register( "smjob dump",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump jwd",
                            "Dump info on all jobs in the job with docs list",
                            NULL,
                            NULL,
                            smjob_cmd_dump_jwd );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump jnd",
                            "Dump info on all jobs in the job without docs list",
                            NULL,
                            NULL,
                            smjob_cmd_dump_jnd );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump held",
                            "Dump info on all jobs in the jobs held list",
                            NULL,
                            NULL,
                            smjob_cmd_dump_held );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump done",
                            "Dump info on all jobs in the jobs completed list",
                            NULL,
                            NULL,
                            smjob_cmd_dump_done );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump stack",
                            "Dump info on all jobs in the job history stack",
                            NULL,
                            NULL,
                            smjob_cmd_dump_stack );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob dump all",
                            "Dump info on all jobs in all the lists",
                            NULL,
                            NULL,
                            smjob_cmd_dump_all );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* print interface commands */
    cmd_res = cmd_register( "smjob print",
                            NULL,
                            NULL,
                            NULL,
                            NULL );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "smjob print test",
                            "Step through the smjob api flow with a file on filesystem", 
                            "<filename>",
                            NULL,
                            smjob_cmd_print_test );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    /* create jobs */
    cmd_res = cmd_register( "smjob scan",
                            NULL,
                            NULL,
                            NULL,
                            smjob_cmd_scan_test );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );


}

#endif
