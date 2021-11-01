/** 
 *
 * ============================================================================
 * Copyright (c) 2006   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 * Description: This is the scan to host parser.  It handles job information and
 the scan to host protocal.
 *
 *****************************************************************************/

/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to makefile to prevent agMessage.h
 * from being included in the scan code.  But we need agMessage.h in this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#include "posix_ostools.h"

#include "agRouter.h"
#include "ATypes.h"
#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "regAddrs.h"
#include "agJobUtils.h"
#include "sys_init_api.h"
#include "ResourceMap.h"
#include "platform.h"
#include "data_access_api.h"
#include "error_types.h"
#ifdef HAVE_SLEEPMODE
#include "powerSave.h"
#endif
#include "ioutils.h" // for cpu_to_be (swapping)
#include "event_observer.h"
#include "response_subject.h"
#include "platform_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scos.h"
#include "scandbg.h"
#include "scantools.h"
#include "piehw.h"
#include "scanvars.h"
#include "scanhwerr.h"
#include "scanmech.h"
#include "scanman.h"
#include "asp.h"
#include "scanapp.h"
#include "pic.h"
//#include "icedma.h"
#include "copyapp.h"
#include "copyapp_api.h"
#include "scanplat.h"
#include "fraction.h"
#include "scands.h"
#include "scanapp_observers_api.h"
#include "scancap.h"
#include "adfsensor.h"
#include "scanmsg.h"
#include "scanapp_uiscan.h"
#include "scanapp_dataprocess.h"
#include "scanapp_margins.h"
#include "scanapp_imagelog.h"
#include "spec.h"
#include "db_api.h"

/* define to turn on more verbose messages */
//#define SCANAPP_DEBUG
/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN PC: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
///Using Method.
// DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
// DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)


#define MSG_SAJ_MESSAGE (INTRA_MSG_BASE + 42)

/* davep 16-Apr-2013 ; build under ThreadX */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 4096
#endif
#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED 0
#endif

#define SCANAPP_QUEUE_SIZE 20 ///< Number of messages in the scanner queue
//#define SCANAPP_STACK_SIZE POSIX_MIN_STACK_SIZE ///< The size of the stack.
#define SCANAPP_STACK_SIZE  PTHREAD_STACK_MIN ///< The size of the stack.
#define SCANAPP_JR_STACK_SIZE PTHREAD_STACK_MIN ///< The size of the stack for the host reader thread
#define MIN_SCAN_SIZE 10   ///< Min Scan size is 1/10 inch.

#define	USE_SDH_A3_MERGE_PCSCAN		1
#define	USE_SDH_LEGAL_MERGE_PCSCAN		1

#define PAGE_SIDE_FRONT 0
#define PAGE_SIDE_BACK  1

static void scanapp_cancel_job( void );
static int pipe_write( void *data, int datalen );

static const char ScanCmpString[] ="ASP\01"; ///< Must match the asp_cookie     
static const char SIQCmpString[] ="#!siq"; /* scan image quality data file */
static const char SIQEndOfFile[] = "#END-OF-FILE-FOR-USB"; 
//uint8_t SIQEndOfFile[] = "#END-OF-FILE"; 
static uint32_t RequestedIOTO=0;     ///< this is the timeout that comes in the scan reserve command.

ALIGN(8)
static uint8_t scanapp_stack[SCANAPP_STACK_SIZE]; ///< The actual stack.
static mqd_t scanapp_msgq; ///< The message queue structure.
static pthread_t scanapp_id; ///< The thread structure.
ALIGN(8)
static uint8_t scanapp_jr_stack[SCANAPP_JR_STACK_SIZE]; ///< The actual scanapp jr stack.
static pthread_t scanapp_jr_id; ///< The thread structure for scanapp junior
static bool scanapp_jr_started;

static IOReg *Pipe; ///< Storage for the IO Pipe.
static CURRENT_RESOURCE *job_resources; ///< Storage for the job resources.
/* davep 15-Apr-2013 ; adding page_side */
#define MAX_PAGE_SIDES 2
static uint32_t cur_row_number[MAX_PAGE_SIDES][SCAN_DATA_TYPE_LAST];
static int bytes_per_pixel_map[SCAN_DATA_TYPE_LAST];

static uint32_t scan_pix_per_row = 0;
static uint32_t scan_pix_per_row_padded = 0;
static uint32_t scan_bits_per_pixel = 0;
static uint32_t scan_bits_per_pixel_received = 0;

#ifdef HAVE_IMAGELOG
static scanOnePageBuf_t	scanOnePageImageBuf[MAX_PAGE_SIDES];
#endif

/* davep 23-Jul-2013 ; track how many page sides we've seen in data messages so
 * we can decide if we're doing a dual scan
 */
static uint32_t max_page_side;

// messages for Scan App Jr (saj)
typedef enum
{
    SA_JR_MSG_LOCK_SCAN = 1,
    SA_JR_MSG_RELEASE_SCAN,
    SA_JR_MSG_START_SCAN_JOB,
    SA_JR_MSG_CANCEL_SCAN_JOB,
    SA_JR_MSG_SET_JOB_SETTINGS,
    SA_JR_MSG_SET_DEFAULT_JOB_SETTINGS,    
    SA_JR_MSG_GET_JOB_SETTINGS,
    SA_JR_MSG_RESET_XMIT_BUFS,
    SA_JR_MSG_RESET_TIMEOUT_COUNTER,    
    SA_JR_MSG_ADF_IS_PAPER_PRESENT,
    SA_JR_MSG_GET_PLATFORM_STRING,
    SA_JR_MSG_GET_ADF_INFORMATION,
} SA_JR_MSGS;

/* Useful for debugging IO problems: stick a packet counter into an unused
 * header field.
 */
//#define DBG_NUMBER_SCAN_PACKETS
#ifdef DBG_NUMBER_SCAN_PACKETS
static uint32_t dbg_scan_packet_ctr = 0;
#endif

typedef struct scanapp_observer_data_s
{
    Response_Subject_T_BaseClassMembers;
    scanapp_event_t event;
} scanapp_observer_data_t;

static scanapp_observer_data_t scanapp_observer_data;

struct pcscan_stats {
	/* count incoming messages */
	uint32_t sizes;
	uint32_t scan_starts;
	uint32_t scan_ends;
	uint32_t page_starts;
	uint32_t page_ends;
	uint32_t page_datas;
	uint32_t readies;

	/* total data rows received (across all channels) */
	uint32_t total_rows;

	/* erase border parameter */
	int32_t eraseLeftPixel;
	int32_t eraseRightPixel;
	int32_t eraseTopPixel;
	int32_t eraseBottomPixel;
	
	/* State of ADF */
	int32_t adf_status;
	uint8_t scanJob_type;
};

typedef enum
{
	PC_SCAN_NONE = 0,
	PC_SCAN_NORMAL = 1,
	PC_SCAN_NORMAL_IQT_PROGRAM = 2,
	PC_SCAN_FACTORY_REGISTRATION = 3,	
} SCAN_APP_JOB_TYPE;

typedef enum
{
    SCAN_APP_IDLE = 0,              ///< Scan is idle.
    SCAN_APP_LOCKING = 1,           ///< We are in the process of getting the resources.
    SCAN_APP_READY_TO_SCAN = 2,     ///< We have the resources and are ready to scan
    SCAN_APP_SCANNING = 3,          ///< We are currently scanning.
    SCAN_APP_WAIT_FOR_RELEASE = 4,  ///< We are releasing the scan
    SCAN_APP_HSTCANCEL = 5,         ///< In the process of canceling the scan.
    SCAN_APP_TOCANCEL = 6,          ///< Timeout cancel
    SCAN_APP_CANCELED = 7           ///< When the job is cancelled yet not released.
} SCAN_APP_STATE;

typedef enum 
{
	ADF_STATUS_NONE = 0,
	ADF_STATUS_PAPER_PRESENT = 1,
	ADF_STATUS_PAPER_JAM = 4,
	ADF_STATUS_COVER_OPEN = 5,
} SCAN_ADF_STATUS;

static SCAN_APP_STATE State;    ///< State variable.
static struct pcscan_stats	pcscan_stats;

SCAN_JOB_SETTINGS JobSettings;  ///< The scan job settings.
static int tliCountry;

static SCAN_JOB_SETTINGS DefJobSettings = 
{
    24, ///< Gamma Values from 0 - 99
    100, ///< Brightness Values 
    100, ///< Contrast Values 
    600, ///< Resolution any number from 75 to 1200.
    {1,1,1,1}, ///< XYScale (x-den,x-num,y-den,y-num)
    100, ///< Sharp values range from [0,200] 0 max smooth, 100 no change, 200 big sharpen
    0, ///< deprecated1 Not currently used.
    8, ///< BitsPerPixel Value 8
	/*
	* Reserved value could be used for IQT Auto Registration with SINDOH's IQT Program 
	* <reserved1> 0: Normal Scan Mode, 1: FB IQT, 2: ADF front IQT, 3: ADF back IQT
	* <reserved2> Margin Value of X(pixel)
	* <reserved3> Margin Value of Y(pixel)
	*/
    0, ///< reserved1
    0, ///< reserved2
    0, ///< reserved3
    0, ///< flags
    SCAN_DATA_TYPE_PLANE, ///< DataType from scan_data_type enum
    {0,0,1100,850}, ///< scan_window in hundredths of an inch (top,left,bottom,right)
    
    /* davep 20-Mar-2007 ;  set length to -1 since we init it at run-time */
    {0,0,-1,850}, ///< ScannableArea in hundredths of an inch 
    
    e_ColorScan ///< Type of scan.
};

static struct scanvars *g_scanvars;

/* davep 04-Jan-2009 ; adding scan capabilities for ADF vs flatbed detection */
static struct scan_platform_capabilities scan_capabilities;

/* davep 07-May-2009 ; kludge to detect if we're scanning from the ADF so we
 * can tell the host we're sending him mirror image data
 */
static bool g_scan_started_from_adf;

#if USE_SDH_A3_MERGE_PCSCAN
static bool g_scan_started_from_A3fb = FALSE;
static bool g_scan_started_from_DLTfb = FALSE;
static bool g_scan_started_from_Legalfb = FALSE;
#endif

/* local prototypes */
static scan_err_t choose_document_source( struct scanvars *sv, 
                                          SCAN_JOB_SETTINGS *job_settings,
                                          bool adf_paper_present );
static scan_err_t setup_scan_area( struct scanvars *sv, SCAN_JOB_SETTINGS *settings );

/* davep 21-Sep-2007 ; track our state transitions (temp debug) */
#define STATE_TRANSITION(new_state)  (state_transition( &State, (new_state), __LINE__ ))

static void state_transition( SCAN_APP_STATE *curr_state, 
                              SCAN_APP_STATE new_state, 
                              int line_number )
{
    DBG_PRINTF_SDH_DEBUG( "scanapp state from %d to %d at %d\n", *curr_state, new_state,
          line_number );
    *curr_state = new_state;
}


/**
 * \brief scanner_malloc_page_memory
 *
 * malloc memory for save 1 page scanned image
 *
 */
#ifdef HAVE_IMAGELOG
static void scanapp_imagelog_malloc_page_memory(uint8_t page_side)
{	
	uint32_t user_height_hinchs = JobSettings.scan_window.bottom - JobSettings.scan_window.top;
	uint32_t nTotalImageHeightPixel = (JobSettings.Resolution * user_height_hinchs)/100;

	uint32_t bytes_per_pixel = 0;
	uint32_t bytes_per_rows = 0;		
	bytes_per_pixel = scan_bits_per_pixel_received/8;
	bytes_per_rows = scan_pix_per_row_padded * bytes_per_pixel;
	
	if(scanOnePageImageBuf[page_side].buf != NULL) {
		MEM_FREE_AND_NULL(scanOnePageImageBuf[page_side].buf);
		scanOnePageImageBuf[page_side].write_index = 0;
		scanOnePageImageBuf[page_side].bufsize_bytes = 0;
	}
	if(scanOnePageImageBuf[page_side].buf == NULL) {
		scanOnePageImageBuf[page_side].bufsize_bytes = (bytes_per_rows * nTotalImageHeightPixel);
		scanOnePageImageBuf[page_side].buf = MEM_MALLOC(scanOnePageImageBuf[page_side].bufsize_bytes);
		memset(scanOnePageImageBuf[page_side].buf, 0xFF, scanOnePageImageBuf[page_side].bufsize_bytes);
		scanOnePageImageBuf[page_side].write_index = 0;
	}
}
#endif

#ifdef HAVE_IMAGELOG
static void scanapp_imagelog_copy_scandata( MESSAGE *msg )
{
	scan_data_type dtype;
	uint16_t num_rows;
	uint8_t *scanData;
	bool last_buffer;
	uint8_t add_page_side;

	uint32_t bytes_per_pixel = 0;
	uint32_t bytes_per_rows = 0;
	uint32_t num_bytes = 0;

	msg_scan_planedata_decode( msg, &num_rows, &dtype, &scanData, &last_buffer );
	msg_scan_planedata_get_page_side( msg, &add_page_side );

	if( add_page_side > 0 ) {
		add_page_side = 1;
    }
    else {
		add_page_side = 0;
	}

	bytes_per_pixel = scan_bits_per_pixel_received/8;
	bytes_per_rows = scan_pix_per_row_padded * bytes_per_pixel;
	num_bytes = num_rows * bytes_per_rows;

	/// Copy Image Data into 'One Page Memory'
	if(scanOnePageImageBuf[add_page_side].write_index < scanOnePageImageBuf[add_page_side].bufsize_bytes) {
		if(scanOnePageImageBuf[add_page_side].write_index + num_bytes > scanOnePageImageBuf[add_page_side].bufsize_bytes ) {
			num_bytes = scanOnePageImageBuf[add_page_side].bufsize_bytes - scanOnePageImageBuf[add_page_side].write_index;
		}			
		if(scanOnePageImageBuf[add_page_side].buf != NULL && scanData != NULL) {
			memcpy( &scanOnePageImageBuf[add_page_side].buf[ scanOnePageImageBuf[add_page_side].write_index ], scanData, num_bytes);
			scanOnePageImageBuf[add_page_side].write_index += num_bytes;
		}
	}
}
#endif

#ifdef HAVE_IMAGELOG
int scanapp_imagelog_start(uint8_t add_page_side)
{
	UINT32 scan_data_type = 0;
	
	uint32_t user_width_hinchs = JobSettings.scan_window.right - JobSettings.scan_window.left;
	uint32_t user_height_hinchs = JobSettings.scan_window.bottom - JobSettings.scan_window.top;
	uint32_t nTotalImageHeightPixel = (JobSettings.Resolution * user_height_hinchs)/100;


	uint32_t bytes_per_pixel = 0;
	uint32_t bytes_per_rows = 0;
	bytes_per_pixel = scan_bits_per_pixel_received/8;
	bytes_per_rows = scan_pix_per_row_padded * bytes_per_pixel;

	UINT32 pdfWidth = (user_width_hinchs * 72) / 100;
	UINT32 pdfHeight = (user_height_hinchs * 72) / 100;
	
	if( JobSettings.ScanType == e_ColorScan ) {
		//Color Mode
		scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_24BPP_MEM;
	}
	else if( JobSettings.ScanType == e_MonochromeScan ) {
		//Gray Mode (8bit)
		scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
	}
	
	if(scanOnePageImageBuf[add_page_side].buf != NULL) {
		SCAN_IL_PDF_Start( scanOnePageImageBuf[add_page_side].buf, scanOnePageImageBuf[add_page_side].bufsize_bytes,
								scan_pix_per_row_padded, nTotalImageHeightPixel, pdfWidth, pdfHeight, 
								pcscan_stats.page_starts, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_PC_SCAN );
	}
	else {
		DBG_PRINTF_SDH_DEBUG("SCAN_IL_PDF_Start CAN NOT. Because NULL Data\n");
	}
	
	return 0;
}
#endif

void swap_scan_header(SCAN_HEADER *header)
{
    header->Cookie = asp_htonl(header->Cookie);
    header->Message = asp_htonl(header->Message);
    header->Param1 = asp_htonl(header->Param1);
    header->Param2 = asp_htonl(header->Param2);    
    header->Status = asp_htonl(header->Status);
    header->DataLength = asp_htonl(header->DataLength);
    header->Reserved1 = asp_htonl(header->Reserved1);
    header->Reserved2 = asp_htonl(header->Reserved2);        
}

void swap_data_header(SCAN_DATA_HEADER *header)
{
    header->DataType = asp_htonl(header->DataType);
    header->RowNumber = asp_htonl(header->RowNumber);
    header->NumberOfRows = asp_htonl(header->NumberOfRows);
    header->BytesPerPixel = asp_htonl(header->BytesPerPixel);
    header->PixelsPerRow = asp_htonl(header->PixelsPerRow);
    header->PixelsPerRowPadded = asp_htonl(header->PixelsPerRowPadded);
}

void swap_job_settings(SCAN_JOB_SETTINGS *settings)
{
    settings->Gamma = asp_htonl(settings->Gamma);
    settings->Brightness = asp_htonl(settings->Brightness);
    settings->Contrast = asp_htonl(settings->Contrast);
    settings->Resolution = asp_htonl(settings->Resolution);
    settings->XYScale.x_numerator = asp_htonl(settings->XYScale.x_numerator);
    settings->XYScale.x_denominator = asp_htonl(settings->XYScale.x_denominator);
    settings->XYScale.y_numerator = asp_htonl(settings->XYScale.y_numerator);
    settings->XYScale.y_denominator = asp_htonl(settings->XYScale.y_denominator);
    settings->Sharp = asp_htonl(settings->Sharp);
    settings->deprecated1 = asp_htonl(settings->deprecated1);
    settings->BitsPerPixel = asp_htonl(settings->BitsPerPixel);
    settings->reserved1 = asp_htonl(settings->reserved1);
    settings->reserved2 = asp_htonl(settings->reserved2);
    settings->reserved3 = asp_htonl(settings->reserved3);
    settings->flags = asp_htonl(settings->flags);
    settings->DataType = asp_htonl(settings->DataType);
    settings->scan_window.top = asp_htonl(settings->scan_window.top);
    settings->scan_window.left = asp_htonl(settings->scan_window.left);
    settings->scan_window.bottom = asp_htonl(settings->scan_window.bottom);
    settings->scan_window.right = asp_htonl(settings->scan_window.right);
    settings->ScannableArea.top = asp_htonl(settings->ScannableArea.top);
    settings->ScannableArea.left = asp_htonl(settings->ScannableArea.left);
    settings->ScannableArea.bottom = asp_htonl(settings->ScannableArea.bottom);
    settings->ScannableArea.right = asp_htonl(settings->ScannableArea.right);
    settings->ScanType = asp_htonl(settings->ScanType);
    
    if (tliCountry == IS_RUSSIA_OEM)
    	settings->ScanType -= RU_SCAN_TYPE_START;
}

static bool adf_paper_is_present( void )
{
    /* REVISIT: the OBA design used to store the ADF sensor state in the data
     *   store, which still may make sense.  Problem is the ds has changed and
     *   we haven't had a chance to hook things back up.  Go straight to the ADF
     *   sensor for now (a stub routine will take care of things if a physical sensor
     *   isn't present).
     */
#if 1
    struct scan_platform_capabilities scan_capabilities;
    
    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return false;
    }
    
    return adf_sensor_paper_present();
#else
    struct scan_platform_capabilities scan_capabilities;
    uint8_t paperval=0;
    
    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return false;
    }
//    scands_get_paper_present(&paperval);
    if (paperval == 1)
        return true;
    else
        return false;
#endif
}

static void scanapp_observer_constructor( void )
{
	memset(&scanapp_observer_data, 0x00, sizeof(scanapp_observer_data));
    scanapp_observer_data.event = SCANAPP_EVENT_NULL;
    scanapp_observer_data.subject_observer_oid_key = SCANAPP_OBSERVER_ID;
    MInitList(&scanapp_observer_data.observers);
}

static void scanapp_observer_notify_event( scanapp_event_t event )
{
    scanapp_observer_data_t * my_status;
    scanapp_observer_data.event = event;
    my_status = &scanapp_observer_data;
    
    NOTIFY_OBSERVERS( my_status, observers );
}

scanapp_event_t scanapp_observer_get_event( Subject_t *s )
{
    ASSERT( VALIDATE_OBJECT_KEY( s,
                                 scanapp_observer_data_t, 
                                 subject_observer_oid_key,
                                 SCANAPP_OBSERVER_ID ) );
    
    // message causes notify causes get so no lock needed. 
    return scanapp_observer_data.event;
}

void scanapp_observer_attach( Observer_t *o )
{
    ATTACH_OBSERVER( scanapp_observer_data.observers, o );
}

void scanapp_observer_detach(Observer_t *o)
{
    DETACH_OBSERVER( scanapp_observer_data.observers, o );
}


/**
 * \brief Log a scan event.
 *
 * RIP Status Manager.  
 *
 * Report a scan event. For example, we're starting a scan, finishing a scan,
 * etc. Originally created to replace the Status Manager. The SM was used to
 * interface with the control panel AKA user interface.
 *
 * This plugs the scan app events into the event_observer.h framework,
 * now anyone can register for scan app events but typically only the
 * user interface would do this.
 *
 */

void scanapp_log_event( scanapp_event_t evt )
{
    /* do the event observer/notifier thing */
    scanapp_observer_notify_event( evt );
}

/* FUNCTION NAME: ScanGetMemory */

/** 
 * \brief Get the memory for use in the scanner.
 * 
 * \param Size The number of bytes to get
 * 
 * \retval void* A pointer to the allocation or NULL if we did not get it.
 * 
 * \author Dirk Bodily
 * 
 * \date 11/15/2005
 * 
 **/
void *ScanGetMemory(uint32_t Size)
{
    uint8_t *Buffer;
    uint32_t Cnt=0;
    //
    // Loop for a while trying to get the memory.
    while((Buffer = MEM_MALLOC(Size)) == NULL)
    {
        posix_sleep_ticks(3);
        if(Cnt++ >200)      // try this 200 times.  about 6 seconds.
            break;      // give up, we are not getting the memory
    }
    ASSERT(Buffer);
    //
    // Clear out the buffer.
    //
    if( Buffer != NULL ) {
        memset( Buffer, 0, Size );
    }
    return Buffer;
}

static scan_err_t copy_to_host_scanvar_setup( SCAN_JOB_SETTINGS *job_settings )
{
    struct scanvars *sv;
    e_COPYQUALITY copy_quality;
    
#ifdef HAVE_COPY_SUPPORT
    return copyapp_setup_scanning( &g_scanvars );
#else
    DBG_PRINTF_SDH_DEBUG("%s No copy support in this firmware so falling back on best effort.\n", __FUNCTION__ );
#endif
    
    copy_quality = (e_COPYQUALITY)job_settings->reserved1;
    
    switch( copy_quality )
    {
    case e_TEXT: 
        sv = scanvar_text_copy_new();
        break;
        
    case e_DRAFT: 
        sv = scanvar_draft_copy_new();
        break;
        
    case e_MIXED: 
        sv = scanvar_mixed_copy_new();
        break;
        
    case e_FILM_PHOTO: 
        sv = scanvar_filmphoto_copy_new();
        break;
        
    case e_PICTURE:
        sv = scanvar_picture_copy_new();
        break;
        
    case e_FAX_TEXT :
        sv = scanvar_text_fax_new(); 
        break;
        
    case e_FAX_MIXED :
        sv = scanvar_mixed_fax_new(); 
        sv->hp_alg_invert_data = true;
        break;
        
    default:
        DBG_PRINTF_SDH_DEBUG( "%s unknown copy quality %d\n", __FUNCTION__, copy_quality );
        return SCANERR_INVALID_PARAM;
    }
    
    if( sv==NULL ) {
        DBG_PRINTF_SDH_ERR( "%s failed to get scanvars\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }
    
    XASSERT( g_scanvars==NULL, g_scanvars->id );
    PTR_ASSIGN( g_scanvars, sv );
    
    return SCANERR_NONE;
}

/**
 * \brief  scan to host through copy path for testing / debugging
 *
 * \author David Poole
 * \date 13-Sep-2006
 *
 */

static scan_err_t setup_scan_params_for_copy( SCAN_JOB_SETTINGS *job_settings ) 
{
    e_COPYQUALITY quality;
    scan_err_t scerr;
    
    /* be paranoid about memory leaks; if user sends down multiple set config
     * requests, quietly throw away the previous one 
     */
    if( g_scanvars != NULL ) {
        /* free the previous; */
        DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars. setup_scan_params_for_copy\n" );
        scanvar_delete( &g_scanvars );
    }
    
    XASSERT( g_scanvars==NULL, (uint32_t)g_scanvars );
    
    DBG_PRINTF_SDH_DEBUG("%s setting up for copy-to-host\n", __FUNCTION__ );
    
    DBG_PRINTF_SDH_DEBUG("  Quality=%ld\n", job_settings->reserved1 );
    if( job_settings->reserved1 != SCANAPP_COPY_QUALITY_DEFAULT ) {
        /*  We have a specific value to mean "use default" so don't change
         *  unless we're told to.
         */
        quality = job_settings->reserved1;
#ifdef HAVE_COPY_SUPPORT
        copyapp_set_fp_quality( quality, COPY_CMODE_MONO );
#endif
    }
    
    scerr = copy_to_host_scanvar_setup( job_settings );
    if( scerr != 0 ) {
        /* hopefully, copyapp.c logs a more specific error than this */
        DBG_PRINTF_SDH_ERR( "failed to set up scanner for copy!\n" );
        return scerr;
    }
    
    /* davep 13-Aug-2009 ; let's try to directly use the host scale factors */
    DBG_PRINTF_SDH_DEBUG("  XYScale=%ld %ld %ld %ld\n",
         job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
         job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator);
    scerr = scanvar_set_xyscale( g_scanvars, 
                                 job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
                                 job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator );
    if( scerr != SCANERR_NONE ) {
        DBG_PRINTF_SDH_ERR( "%s scanvar_set_xyscale failed scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }
    
    return SCANERR_NONE;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 14-Jun-2012
 */

scan_err_t allocate_global_scanvar( SCAN_JOB_SETTINGS *job_settings ) 
{
    /* be paranoid about memory leaks; if user sends down multiple set config
     * requests, quietly throw away the previous one 
     */
    if( g_scanvars != NULL ) {
        /* free the previous; */
        DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars: allocate_global_scanvar\n" );
        scanvar_delete( &g_scanvars );
    }
    
    if( job_settings->ScanType == e_ColorScan) {
        g_scanvars = scanplat_sv_color_scan_new();
    }
    else if( job_settings->ScanType == e_MonochromeScan || job_settings->ScanType == e_BilevelScan) {
        g_scanvars = scanplat_sv_mono_scan_new();
    }
    else { 
        return SCANERR_INVALID_PARAM;
    }
    
    if( g_scanvars==NULL ) {
        DBG_PRINTF_SDH_ERR( "out of memory for scanap scanvar\n" );
        return SCANERR_OUT_OF_MEMORY;
    }
    
    return SCANERR_NONE;
}

int scanapp_setup_merge_scanarea(int mergeJobType, SCAN_JOB_SETTINGS *merge_job_settings)
{
	if(mergeJobType == PC_SCAN_MERGEJOB_A3) {
		merge_job_settings->scan_window.right		= 850;	//WIDTH	: Letter
		merge_job_settings->scan_window.bottom	= 1170;	//HEIGHT	: A4
		merge_job_settings->ScannableArea.right	= 850;	//WIDTH	: Letter
		merge_job_settings->scan_window.top		= 0; 		
		merge_job_settings->scan_window.left		= 0;
		merge_job_settings->ScannableArea.top		= 0;
		merge_job_settings->ScannableArea.left	= 0;
		merge_job_settings->ScannableArea.bottom	= -1;
	}
	else if(mergeJobType == PC_SCAN_MERGEJOB_11X17) {
		merge_job_settings->scan_window.right		= 850;	//WIDTH	: Letter
		merge_job_settings->scan_window.bottom	= 1100;	//HEIGHT	: Letter
		merge_job_settings->ScannableArea.right	= 850;	//WIDTH	: Letter
		merge_job_settings->scan_window.top		= 0; 		
		merge_job_settings->scan_window.left		= 0;
		merge_job_settings->ScannableArea.top		= 0;
		merge_job_settings->ScannableArea.left	= 0;
		merge_job_settings->ScannableArea.bottom	= -1;
	}
	else if(mergeJobType == PC_SCAN_MERGEJOB_LEGAL_1ST_PAGE) {
		merge_job_settings->scan_window.top		= 0;
		merge_job_settings->scan_window.bottom	= 725;
		merge_job_settings->scan_window.right		= 850;
		merge_job_settings->scan_window.left		= 0;
		merge_job_settings->ScannableArea.top		= 0;
		merge_job_settings->ScannableArea.bottom	= -1;
		merge_job_settings->ScannableArea.left	= 0;
		merge_job_settings->ScannableArea.right	= 850;
	}
	else if(mergeJobType == PC_SCAN_MERGEJOB_LEGAL_2ND_PAGE) {
		merge_job_settings->scan_window.top		= 445;
		merge_job_settings->scan_window.bottom	= 1170;
		merge_job_settings->scan_window.left		= 0;
		merge_job_settings->scan_window.right		= 850;
		merge_job_settings->ScannableArea.top		= 0;
		merge_job_settings->ScannableArea.bottom	= -1;
		merge_job_settings->ScannableArea.left	= 0;
		merge_job_settings->ScannableArea.right	= 850;
	}
	
	return 0;
}


/**
 * \brief Construct a scanvar for an upcoming scan.
 *
 * If there is already an existing scanvar, delete it. Allows for recovery from
 * weird protocol behavior (usually the host should send only one
 * SetScanJobSettings message).
 *
 * \param scan_type The type of scan we are doing.
 * \author David Poole
 * \date 30-Dec-2005
 *
 * davep 14-Jun-2012 ; roll a bunch of code from message loop into this
 *                     function so we have better error checking
 */

static scan_err_t setup_scan_params( struct scanvars *sv, SCAN_JOB_SETTINGS *job_settings )
{
    scan_err_t scerr;
    
    ASSERT( sv!=NULL );
    
    /* davep 12-Oct-2006 ; adding support for cal-to-host */
    if( job_settings->flags & SCAN_JOB_FLAGS_CAL_TO_HOST ) {
        DBG_PRINTF_SDH_DEBUG("%s cal to host enabled\n", __FUNCTION__ );
        scanvar_set_capture_cal( sv, true );
    }
    
    /* davep 26-Jul-2011 ; add host side disable cal, pie */
    if( job_settings->flags & SCAN_JOB_FLAGS_NO_CAL ) {
        DBG_PRINTF_SDH_DEBUG( "%s disable cal for this scan\n", __FUNCTION__ );
        sv->cal_mode = SCANVAR_CAL_DISABLED;
    }
    if( job_settings->flags & SCAN_JOB_FLAGS_NO_PIE ) {
        DBG_PRINTF_SDH_DEBUG( "%s disable image enhancements for this scan\n", __FUNCTION__ );
        scanvar_disable_image_enhancements( sv );
        if( job_settings->reserved1 == IQT_FACTORY_REGISTRATION ) {
			pcscan_stats.scanJob_type = PC_SCAN_FACTORY_REGISTRATION;
		}
		else {
			pcscan_stats.scanJob_type = PC_SCAN_NORMAL_IQT_PROGRAM;
		}
#if 0
		//Temp Code : In IQT Pc Scan, drop cache for temporary (memory miss)
		if( (job_settings->Resolution == 600) && (job_settings->DataType == SCAN_DATA_TYPE_RGB) ) {
			system("echo 3 > /proc/sys/vm/drop_caches");
			DBG_PRINTF_SDH_DEBUG("IQT PC Scan (600,Color) Drop Caches\n");
		}
#endif
	}
    else {
		pcscan_stats.scanJob_type = PC_SCAN_NORMAL;
	}
	
	if (job_settings->Gamma >= 10 && job_settings->Gamma <= 100) {
		if( job_settings->ScanType == e_ColorScan ) {
			job_settings->Gamma = SDH_SCAN_COLOR_GAMMA_VAL;
		}
		else {
			job_settings->Gamma = SDH_SCAN_GRAY_GAMMA_VAL;
		}
		DBG_PRINTF_SDH_DEBUG("  Gamma = %ld\n",job_settings->Gamma);
		scerr = scanvar_set_gamma(sv, job_settings->Gamma);
		if( scerr != SCANERR_NONE ) {
			return scerr;
		}
	}
    
	DBG_PRINTF_SDH_DEBUG("  Brightness = %ld\n",job_settings->Brightness);
	//if( job_settings->ScanType == e_ColorScan )
	if( job_settings->ScanType == e_ColorScan || job_settings->ScanType == e_MonochromeScan) {	// mod.lsh to use same range gray and color scan
		//Driver setting value is ( 0 ~ 200 ) --> MFP Setting value is ( 70 ~ 130 )
		job_settings->Brightness = ((job_settings->Brightness * 160)/200) + 20;	// mod.lsh to remove color background level (CS problem)
		DBG_PRINTF_SDH_DEBUG("  Brightness truncated to = %ld\n",job_settings->Brightness);		
	}
    
    scerr = scanvar_set_brightness(sv, job_settings->Brightness);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
    
    DBG_PRINTF_SDH_DEBUG("  Contrast(Driver) = %ld\n",job_settings->Contrast);
    //Driver Setting Value : 0 ~ 200, Machine Setting Value : 50 ~ 150
    job_settings->Contrast = (job_settings->Contrast + 100)/2;
	if(job_settings->Contrast < 0 || job_settings->Contrast > 150) {
		job_settings->Contrast = 100;
		DBG_PRINTF_SDH_DEBUG("  Contrast truncated to = %ld\n",job_settings->Contrast);
	}
	DBG_PRINTF_SDH_DEBUG("  Contrast(MFP) = %ld\n",job_settings->Contrast);

    scerr = scanvar_set_contrast( sv, job_settings->Contrast);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }    
#if 0	// del.lsh not to clear LUT set before
    DBG_PRINTF_SDH_DEBUG("  Sharpness = %ld\n",job_settings->Sharp);
    scerr = scanvar_set_sharpness( sv, job_settings->Sharp);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#endif
    DBG_PRINTF_SDH_DEBUG("  Requested Resolution = %ld\n",job_settings->Resolution);
    scerr = scanvar_set_dpi( sv, job_settings->Resolution );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
    
    DBG_PRINTF_SDH_DEBUG("  XYScale = %ld %ld %ld %ld\n",
         job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
         job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator);
    scerr = scanvar_set_xyscale( sv, 
                                 job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
                                 job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#if 0	// del.lsh not to clear LUT set before
    DBG_PRINTF_SDH_DEBUG("  Sharp = %ld\n",job_settings->Sharp);
    scerr = scanvar_set_sharpness( sv, job_settings->Sharp );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#endif
    DBG_PRINTF_SDH_DEBUG("  BitsPerPixel = %ld\n",job_settings->BitsPerPixel);
    scerr = scanvar_set_bpp( sv, job_settings->BitsPerPixel);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
    
    ///SINDOH yhlee Added, If Bi-Levl Scan Job Setuped, to Lowlevel notity to 8, to UpperLevel(PC) notify to 1.
    if(job_settings->ScanType == e_BilevelScan) {
		job_settings->BitsPerPixel = 1;
		DBG_PRINTF_SDH_DEBUG("  BitsPerPixel -> %ld\n",job_settings->BitsPerPixel);
	}
    
    /* burtp 02-Oct-2009; Need the PP state now because setup_scan_area depends
     * on the document source.
     *  NOTE:
     *    If we just woke up from powersave the PP sensor state may not had a chance
     *    to update. Pause a little while to try and reduce PP sensing race conditions.
     */
//            tx_thread_sleep( 1.5 * SECONDS );
    
    g_scan_started_from_adf = adf_paper_is_present();
    DBG_PRINTF_SDH_DEBUG("%s: g_scan_started_from_adf=%d\n", __FUNCTION__, g_scan_started_from_adf);
    
    scerr = choose_document_source( sv, job_settings, g_scan_started_from_adf );
    if( scerr != 0 ) {
        return scerr; 
    }
    
    DBG_PRINTF_SDH_DEBUG("  scan_window: top: %ld, left: %ld, bottom: %ld, right: %ld\n",
         job_settings->scan_window.top,
         job_settings->scan_window.left, 
         job_settings->scan_window.bottom-job_settings->scan_window.top,
         job_settings->scan_window.right-job_settings->scan_window.left);

#if USE_SDH_A3_MERGE_PCSCAN
	if( g_scan_started_from_A3fb == TRUE ) {
		///When A3 Scan Job Requested, have to Upadate size for A3 merge Algolithm. (WIDTH:Letter, HEIGHT:A4)
		scanapp_setup_merge_scanarea( PC_SCAN_MERGEJOB_A3, job_settings );
	}
	else if(g_scan_started_from_DLTfb == TRUE) {
		scanapp_setup_merge_scanarea( PC_SCAN_MERGEJOB_11X17, job_settings );
	}
	else if(g_scan_started_from_Legalfb == TRUE) {
		scanapp_setup_merge_scanarea( PC_SCAN_MERGEJOB_LEGAL_1ST_PAGE, job_settings );
	}
#endif
	scerr = setup_scan_area( sv, job_settings);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
    
    DBG_PRINTF_SDH_DEBUG("  ScanType = %ld\n",job_settings->ScanType);
    DBG_PRINTF_SDH_DEBUG("  DataType = %ld\n",job_settings->DataType);
    if (job_settings->ScanType == e_MonochromeScan || job_settings->ScanType == e_BilevelScan) {
        scanvar_set_pieout( sv, PIE_PIXEL_MONO);
    }
    else {
        switch((scan_data_type)job_settings->DataType)
        {
        case SCAN_DATA_TYPE_PLANE:
            scanvar_set_pieout( sv, PIE_PIXEL_3_PLANES);
            break;
        case SCAN_DATA_TYPE_XRGB:
            scanvar_set_pieout(sv, PIE_PIXEL_XRGB);
            break;
        case SCAN_DATA_TYPE_MONO:
            scanvar_set_pieout(sv, PIE_PIXEL_MONO);
            break;
        case SCAN_DATA_TYPE_RGBX:
            scanvar_set_pieout(sv, PIE_PIXEL_RGBX);
            break;
        case SCAN_DATA_TYPE_RGB:
            scanvar_set_pieout(sv, PIE_PIXEL_RGB);
            break;
        default:
            DBG_PRINTF_SDH_ERR("SCAN: Invalid Scan data type value %lx\n",
                 job_settings->DataType);
            return SCANERR_INVALID_PARAM;
            break;
        }
    }

    return SCANERR_NONE;
}

void scanapp_notify_adf_status(UINT32 eventReason, UINT32 eventStatus)
{
	DBG_PRINTF_SDH_DEBUG("ADFStatus Notify EventReason=%d, EventStatus=%d\n", eventReason, eventStatus);

	if( scan_capabilities.has_adf ) {
		switch(eventReason) {
			case SCANMAN_EVENT_ADF_PAPER_PRESENT:
				if(eventStatus == 1)
					pcscan_stats.adf_status = ADF_STATUS_PAPER_PRESENT;
				else 
					pcscan_stats.adf_status = ADF_STATUS_NONE;		
				break;
			case SCANMAN_EVENT_ADF_PAPER_JAM:
			case SCANMAN_EVENT_ADF_PAPER_NOPICK:
				if(eventStatus == 1)
					pcscan_stats.adf_status = ADF_STATUS_PAPER_JAM;
				else 
					pcscan_stats.adf_status = ADF_STATUS_NONE;
				break;
			case SCANMAN_EVENT_ADF_TOP_COVER_OPEN:
				if(eventStatus == 1)
					pcscan_stats.adf_status = ADF_STATUS_COVER_OPEN;
				else 
					pcscan_stats.adf_status = ADF_STATUS_NONE;
				break;
			default:
				pcscan_stats.adf_status = ADF_STATUS_NONE;
				break;
		}
	}
	else {
		/* flatbed therefore report FAIL since we don't have an ADF */
	}
}

/**
 * \brief Setup the start job for ScanApp's upstream and downstream resources. 
 *
 * Sends a start job message to whatever upstream resource we're hooked up
 * with.
 *
 * \author Brad Smith
 * \date 26-Aug-2005
 *
 */

static void
scanapp_start_job( void )
{
    MESSAGE msg;
    error_type_t err;
    
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
    
    /* davep 07-May-2009 ; kludge to detect if we're scanning from the adf (not
     * reliable!)
     */
    if( g_scan_started_from_adf ) {
        DBG_PRINTF_SDH_DEBUG( "%s maybe scanning from adf so we'll send mirror data types\n", __FUNCTION__ );
    }
    
    scanman_set_scanpapersize(0x00);
    
    /* send the startjob to my upstream */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBSTART;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = g_scanvars;
    /* no further parameters */
    
    err = SYMsgSend( job_resources->Source, &msg );
    XASSERT( err==OK, err );

    /* re-initialize static variables */
    dither_buffer(NULL,0,0);
}

/**
 * \brief write data to Pipe while also handling short writes
 *
 * A wrapper around Pipe->WriteConnect. Keep calling WriteConnect until all the
 * data is sent.
 *
 *
 * \author David Poole
 * \date 08-May-2009
 *
 */

static int pipe_write( void *data, int datalen ) 
{
    int retcode;
    
    /* davep 14-Sep-2009 ; fun debug to track what messages we're sending
     * downstream 
     */
#ifdef SCANAPP_DEBUG
    if( datalen==sizeof(SCAN_HEADER) ) {
        DBG_PRINTF_SDH_DEBUG( "scanapp %s msg=%d\n",  
              __FUNCTION__, asp_htonl(((SCAN_HEADER *)data)->Message) );
    }
#endif
    
    /* Debug lost IO data packets: number them using an unused header field */
#ifdef DBG_NUMBER_SCAN_PACKETS
    if( datalen==sizeof(SCAN_HEADER) ) {
        dbg_scan_packet_ctr++;
        ((SCAN_HEADER *)data)->Reserved2 = dbg_scan_packet_ctr;
    }
#endif
    
    while( datalen>0 ) {
        retcode = Pipe->WriteConnect(Pipe, data, datalen );
        
        /* davep 08-May-2009 ; XXX temp debug */
        if( retcode != datalen ) {
            DBG_PRINTF_SDH_DEBUG( "%s pipe short write retcode=%d\n", __FUNCTION__, retcode );
        }
        
        if( retcode < 0 ) {
            /* end of file or error */
            return retcode;
        }
        
        datalen -= retcode;
        data += retcode;
    }
    
    return 0;
}

/**
 * \brief  Ordered dither.
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 * davep 19-Jul-2013 ; moved from kernel soft_piehp.c into userspace scanapp.c
 *
 * http://www.efg2.com/Lab/Library/ImageProcessing/DHALF.TXT
 * http://www.imagemagick.org
 *
 * _Digital Halftoning_, Robert Ulichney 
 * # ISBN-10: 0262210096
 *
 */

/* 26 because we want to scale 0-255 into 0-9 */
static const uint8_t threshold_map_b[3][3] = { 
    {  78, 192, 104 },
    { 156,  26, 234 },
    {  52, 208, 130 }
};


void dither_buffer( uint8_t *buf, uint32_t pixels_per_row, uint32_t rows_per_buffer )
{
    int i;
    uint8_t *ptr;
    int x;
    static int y=0;

    /* re-initialize */
    if( buf==NULL ) {
        y = 0;
        return;
    }

//    DBG_PRINTF_SDH_DEBUG( "%s pixels_per_row=%d rows_per_buffer=%d\n", __FUNCTION__,
//            pixels_per_row, rows_per_buffer );

    for( i=0 ; i<rows_per_buffer ; i++ ) {
        ptr = buf + (i*pixels_per_row);

        for( x=0 ; x<pixels_per_row; x++ ) {
            if( *ptr < threshold_map_b[ x % 3 ][ y % 3] ) {
                *ptr++ = 0;
            }
            else  {
                *ptr++ = ~0;
            }
        }
        y++;
    }
}

#if SDH_USE_ERASE_BORDER
static void scanapp_erase_border(uint16_t num_rows, uint8_t *scanData, uint8_t page_side, scan_data_type dataType)
{
	uint32_t bytes_per_pixel = scan_bits_per_pixel_received/8;
	uint32_t bytes_per_rows = scan_pix_per_row_padded * bytes_per_pixel;
	uint32_t num_bytes = num_rows * bytes_per_rows;

	//White padding (Erase Border) : left and rigth
	if( (scanData) && (pcscan_stats.eraseLeftPixel || pcscan_stats.eraseRightPixel) ) {
		uint32_t rowIdx;
		uint32_t user_width_hinchs = JobSettings.scan_window.right - JobSettings.scan_window.left;
		uint32_t eraseLeftDataSize = (pcscan_stats.eraseLeftPixel * bytes_per_pixel);

		uint32_t nTotalImageWidthPixel = (JobSettings.Resolution * user_width_hinchs)/100;
		uint32_t eraseRightPixelPoint = nTotalImageWidthPixel - pcscan_stats.eraseRightPixel;
		uint32_t eraseRightDataPoint = eraseRightPixelPoint * bytes_per_pixel;
		uint32_t eraseRightDataSize = bytes_per_rows - eraseRightDataPoint;
	
		//DBG_PRINTF_SDH_DEBUG("Padding Data Size:BytePerRow(%d), L(%d), R(%d), RPoint(%d)\n", bytes_per_rows, eraseLeftDataSize, eraseRightDataSize, eraseRightDataPoint);
		
		for(rowIdx=0; rowIdx<num_rows; rowIdx++) {
			memset( scanData + (rowIdx * bytes_per_rows), 0xFF, eraseLeftDataSize );
			memset( scanData + (rowIdx * bytes_per_rows) + eraseRightDataPoint, 0xFF, eraseRightDataSize);
		}
	}
	
	//White padding (Erase Border) : top and bottom
	if( (scanData) && (pcscan_stats.eraseTopPixel || pcscan_stats.eraseBottomPixel) ) {
		uint32_t buf_height = num_rows;
		uint32_t nPaddingHeight = 0;
		uint32_t nImageHeight = cur_row_number[page_side][dataType];
		uint32_t user_height_hinchs = JobSettings.scan_window.bottom - JobSettings.scan_window.top;
		uint32_t nTotalImageHeightPixel = (JobSettings.Resolution * user_height_hinchs)/100;
		uint32_t nEraseTop = pcscan_stats.eraseTopPixel;
		uint32_t nEraseBottom = nTotalImageHeightPixel - pcscan_stats.eraseBottomPixel;
		
		if( nImageHeight < nEraseTop )
		{
			if( nImageHeight + buf_height < nEraseTop )
			{
				memset( scanData, 0xFF, num_bytes);
			}
			else
			{
				nPaddingHeight = nEraseTop - nImageHeight;
				memset( scanData, 0xFF, bytes_per_rows * nPaddingHeight );
			}
			//DBG_PRINTF_SDH_DEBUG("padding top %p, %d\n", scanData, bytes_per_rows * nPaddingHeight );
		}
		else if( nEraseBottom < nImageHeight + buf_height)
		{
			if( nEraseBottom < nImageHeight )
			{
				memset( scanData, 0xFF, num_bytes);
			}
			else
			{
				nPaddingHeight = (nImageHeight + buf_height) - nEraseBottom;
				//DBG_PRINTF_SDH_DEBUG("padding bottom 2 %p, %d\n", scanData+( (nEraseBottom-nImageHeight) * bytes_per_rows), bytes_per_rows * nPaddingHeight);
				memset( scanData + ((nEraseBottom-nImageHeight) * bytes_per_rows), 0xFF, bytes_per_rows * nPaddingHeight);
			}
		}
	}	
}
#endif //SDH_USE_ERASE_BORDER

/**
 * \brief Send a strip to the host
 *
 * \author Brad Smith
 * \author David Poole
 * \date 25-Aug-2005
 *
 * davep 22-Apr-2011 ; converted to new scan messages
 *
 */

static void scan_send_data( MESSAGE *msg )
{
    int datawidth, datalen;
    int pixels_per_byte = 0;
    SCAN_DATA_HEADER *dataheader;
    SCAN_HEADER *header;
    int retcode;
    scan_data_type dtype;
    uint16_t num_rows;
    uint8_t *data;
    bool last_buffer;
    uint8_t page_side;
    
    msg_scan_planedata_decode( msg, &num_rows, &dtype, &data, &last_buffer );
    ASSERT( data!=NULL );
    
    /* davep 15-Apr-2013 ; adding page_side */
    msg_scan_planedata_get_page_side( msg, &page_side );

    XASSERT( page_side<2, page_side );

    /* davep 23-Jul-2013 ; track largest page_side so we can infer we're doing
     * dual scanning
     */
    if( page_side > max_page_side ) {
        max_page_side = page_side;
    }

#if SDH_USE_ERASE_BORDER
	if( (pcscan_stats.scanJob_type == PC_SCAN_NORMAL) && 
		(g_scan_started_from_A3fb == FALSE) &&
		(g_scan_started_from_DLTfb == FALSE) &&
		(g_scan_started_from_Legalfb == FALSE) 
		) {
		scanapp_erase_border( num_rows, data, page_side, dtype );
	}
	else {
		//DBG_PRINTF_SDH_DEBUG("This is merge scan job, so we don't erase border\n");
	}
#endif


//    DBG_PRINTF_SDH_DEBUG( "%s num_rows=%d dtype=%d last=%d\n", __FUNCTION__, num_rows, dtype, last_buffer );
//    PTR_FREE( data );
//    return;
    
#if 0	///SINDOH Yhlee block 
    /* davep 19-Jul-2013 ; tinkering with ordered dither */
    if( JobSettings.DataType==SCAN_DATA_TYPE_BILEVEL) {
        dither_buffer( data, scan_pix_per_row_padded, num_rows );
    }
#endif

	dataheader = (SCAN_DATA_HEADER *)ScanGetMemory(sizeof(SCAN_DATA_HEADER));
	header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
	header->Cookie = ASP_COOKIE;
	header->Message = e_ScanImageData;
	header->Status = e_Success;
	/* davep 16-Apr-2013 ; adding page_side */
	header->Param1 |= SCAN_MSG_IMAGE_DATA_SET_PAGE_SIDE(page_side);
	
	dataheader->DataType = dtype;
	dataheader->BytesPerPixel = 1;
	dataheader->NumberOfRows = num_rows;

	///SINDOH Yhlee add.
	if(JobSettings.ScanType == e_BilevelScan) {
		/*
		 ********************************************************************************************************
		 * < SINDOH Added for Bi-Level Scan >
		 * 		1. Host PC Request Bi-Level Scan with jobSetting->ScanType == e_BilevelScan
		 * 		2. Halftone Mode devided into Threshold & Error Diffusion
		 *		3. Although Host Request Bi-Level, Scan Sub System is not available,
		 * 			so, Regarding Gray Scan with mono data type.
		 * 		4. If Some Image Data is incomming into app area, have to check 'ScanType'
		 *********************************************************************************************************
		 */
		/// For Bi-Level Scan, Update the variable regarding pixel size
		pixels_per_byte = 8;
		scan_bits_per_pixel = 1;
#if 0
		DBG_PRINTF_SDH_DEBUG("=====================================================================\n");
		DBG_PRINTF_SDH_DEBUG("  [ scan_send_data ] : Original \n");
		DBG_PRINTF_SDH_DEBUG("		dataheader->DataType 			: %ld\n", dataheader->DataType);
		DBG_PRINTF_SDH_DEBUG("		dataheader->BytesPerPixel	: %ld\n", dataheader->BytesPerPixel); 				
		DBG_PRINTF_SDH_DEBUG("		dataheader->NumberOfRows		: %ld\n", dataheader->NumberOfRows);
		DBG_PRINTF_SDH_DEBUG("		scan_pix_per_row				: %ld\n", scan_pix_per_row);
		DBG_PRINTF_SDH_DEBUG("		scan_pix_per_row_padded		: %ld\n", scan_pix_per_row_padded);
		DBG_PRINTF_SDH_DEBUG("		scan_bits_per_pixel			: %ld\n", scan_bits_per_pixel);
		DBG_PRINTF_SDH_DEBUG("		pixels_per_byte					: %ld\n", pixels_per_byte);
		DBG_PRINTF_SDH_DEBUG("=====================================================================\n");
#endif
	 }
	else {
		/* we're only using this for HalfPack data but initialize it to something
		 * useful anyway
		 */
		pixels_per_byte = 1;
	}
	
	//
	// Figure the current row number.  Varies depending on the data we are sending.
	//
	XASSERT( dtype < SCAN_DATA_TYPE_LAST, dtype );
	
	dataheader->RowNumber = cur_row_number[page_side][dtype];
	cur_row_number[page_side][dtype] += num_rows;
	
	ASSERT( scan_pix_per_row > 0 );
	ASSERT( scan_pix_per_row_padded > 0 );
	ASSERT( scan_bits_per_pixel > 0 );
	
	// sanity check
	if (scan_pix_per_row > scan_pix_per_row_padded)
	{
		DBG_PRINTF_SDH_DEBUG("WARNING!!! pix_per_row (%ld) > pix_per_row_padded (%ld)!\n",
			 scan_pix_per_row, scan_pix_per_row_padded );
		scan_pix_per_row = scan_pix_per_row_padded;
	}
	dataheader->PixelsPerRow = scan_pix_per_row;
	dataheader->PixelsPerRowPadded = scan_pix_per_row_padded;
	
	datawidth = scan_pix_per_row_padded;
	
	if( scan_bits_per_pixel >= 8 ) {
		dataheader->BytesPerPixel = scan_bits_per_pixel / 8;
		datalen = num_rows * datawidth * dataheader->BytesPerPixel;
	}
	else {
		/* so far, this is the only place pixels_per_byte is being used; it's
		 * ignored except for HalfPack output
		 */
		dataheader->BytesPerPixel = -scan_bits_per_pixel;
		pixels_per_byte = 8 / scan_bits_per_pixel;
		
		if( (num_rows*datawidth) % pixels_per_byte != 0 ) {
			/* in other words, the number of pixels and bits per pixel doesn't
			 * match up with the number of bytes we're getting
			 */
			DBG_PRINTF_SDH_DEBUG( "WARNING! byte size not modulo pixel size!\n" );
		}
		datalen = (num_rows * datawidth) / pixels_per_byte;
		XASSERT( datalen>0, datalen );
	}
	
	///SINDOH Yhlee add.
	if(JobSettings.ScanType == e_BilevelScan) {
		if(JobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) {
			if (page_side == PAGE_SIDE_FRONT)
				scanData_gray2bw_strip( data, datawidth, num_rows, SCAN_DITHERING_PHOTH );
			else
				scanData_gray2bw_strip_back( data, datawidth, num_rows, SCAN_DITHERING_PHOTH );
		}
		else {
			if (page_side == PAGE_SIDE_FRONT)
				scanData_gray2bw_strip( data, datawidth, num_rows, SCAN_DITHERING_TEXT );
			else
				scanData_gray2bw_strip_back( data, datawidth, num_rows, SCAN_DITHERING_TEXT );
		}	
		
#if 0
		DBG_PRINTF_SDH_DEBUG("=====================================================================\n");
		DBG_PRINTF_SDH_DEBUG("  [ scan_send_data ] : Modified \n");
		DBG_PRINTF_SDH_DEBUG("		dataheader->DataType 			: %ld\n", dataheader->DataType);
		DBG_PRINTF_SDH_DEBUG("		dataheader->BytesPerPixel	: %ld\n", dataheader->BytesPerPixel); 				
		DBG_PRINTF_SDH_DEBUG("		dataheader->NumberOfRows		: %ld\n", dataheader->NumberOfRows);
		DBG_PRINTF_SDH_DEBUG("		scan_pix_per_row				: %ld\n", scan_pix_per_row);
		DBG_PRINTF_SDH_DEBUG("		scan_pix_per_row_padded		: %ld\n", scan_pix_per_row_padded);
		DBG_PRINTF_SDH_DEBUG("		scan_bits_per_pixel			: %ld\n", scan_bits_per_pixel);
		DBG_PRINTF_SDH_DEBUG("		pixels_per_byte					: %ld\n", pixels_per_byte);
		DBG_PRINTF_SDH_DEBUG("		datawidth							: %ld\n", datawidth);
		DBG_PRINTF_SDH_DEBUG("		datalen							: %ld\n", datalen);
		DBG_PRINTF_SDH_DEBUG("=====================================================================\n");
#endif

	}
		
	/* davep 12-Oct-2006 ; XXX comment this out before checking in */
#ifdef SCANAPP_DEBUG
	DBG_PRINTF_SDH_DEBUG("%s rows=%d ppr=%ld pprp=%ld dtype=%d datawidth=%d datalen=%d side=%d\n", __FUNCTION__,
		 num_rows, scan_pix_per_row, scan_pix_per_row_padded, dtype, datawidth,
		 datalen, page_side );
#endif
		
	header->DataLength = sizeof(SCAN_DATA_HEADER) + datalen;
	swap_scan_header(header);
	retcode = pipe_write( (uint8_t *)header, sizeof(SCAN_HEADER));
	swap_data_header(dataheader);
	retcode = pipe_write( (uint8_t *)dataheader, sizeof(SCAN_DATA_HEADER));
	
	// It's OK to just forward on the data pointer -- we want it
	// to be freed by the connection mgr.
	retcode = pipe_write( (uint8_t *)data, datalen );	
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Dec-2008
 *
 */

static void send_data_blob( void *data, int datalen )
{
    SCAN_HEADER *header;
    int32_t pipe_retcode;
    
    DBG_PRINTF_SDH_DEBUG( "%s data=%p datalen=%d\n", __FUNCTION__, data, datalen );
    
    header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
    if( header==NULL ) {
        DBG_PRINTF_SDH_ERR( "%s unable to get memory for scan header\n" );
        
        /* must free the data pointer (someone has to!) */
        PTR_FREE( data );
        return;
    }
    
    header->Cookie = ASP_COOKIE;
    header->Message = e_ScanDataBlob;
    header->Status = e_Success;
    header->DataLength = datalen;
    
    swap_scan_header(header);
    pipe_retcode = pipe_write( (uint8_t *)header, sizeof(SCAN_HEADER));
    DBG_PRINTF_SDH_DEBUG( "%s header pipe_retcode=%ld\n", __FUNCTION__, pipe_retcode );
    
    pipe_retcode = pipe_write(  (uint8_t *)data, datalen );
    DBG_PRINTF_SDH_DEBUG( "%s data pipe_retcode=%ld\n", __FUNCTION__, pipe_retcode );
}

/**
 * \brief  Transmit a SCAN_HEADER 
 *
 * Allocates memory, copies the parameter to the memory, and sends a fully
 * prepared scan header out onto the wire.
 *
 * The code downstream of pipe_write() will free() the pointer. The
 * hdr_to_send parameter is assumed to be on the stack.  
 *
 * This function will NOT assume the parameter needs to be freed! This function
 * malloc's a new buffer then memcpy's the parameter to it.
 *
 * \author David Poole
 * \date 15-Apr-2013
 */

static scan_err_t send_scan_header( IOReg *Pipe, const SCAN_HEADER *hdr_to_send )
{
    SCAN_HEADER *header;
    int retcode;

//    DBG_PRINTF_SDH_DEBUG( "scanapp %s msg=%d status=%d state=%d\n", __FUNCTION__,
//            hdr_to_send->Message, hdr_to_send->Status, State );
    
    header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
    if( !header ) {
        DBG_PRINTF_SDH_DEBUG( "%s out of memory for scan header\n", __FUNCTION__ );
        /* we're kind of hosed at this point; the rest of the code wasn't
         * written with memory recovery in mind
         */
        return SCANERR_OUT_OF_MEMORY;
    }
    memcpy( header, hdr_to_send, sizeof(SCAN_HEADER));
    retcode = pipe_write(header, sizeof(SCAN_HEADER));

    return SCANERR_NONE;
}

/**
 * \brief  Send an ASP message
 *
 * \param Pipe contains function pointer we call to send data
 * \param msg The message type to send
 *
 * Data length is hardwired to 0.
 *
 * \author David Poole
 * \date 13-Sep-2006
 *
 */

static void send_cmd( IOReg *Pipe, SCAN_MESSAGE_TYPE msg, uint32_t Status )
{
    SCAN_HEADER *header;
    int retcode;
    
    DBG_PRINTF_SDH_DEBUG( "scanapp %s msg=%d; status=%d; state=%d \n", __FUNCTION__, msg, Status, State );
    //agprintf( "%s msg=%d; status=%d; state=%d \n", __FUNCTION__, msg, Status, State);
    
    header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
    if( !header ) {
        DBG_PRINTF_SDH_ERR( "%s out of memory for scan header\n", __FUNCTION__ );
        /* we're kind of hosed at this point; the rest of the code wasn't
         * written with memory recovery in mind
         */
    }
    else {
        header->Cookie = ASP_COOKIE;
        header->Message = msg;
        header->Status = Status;
        header->DataLength = 0;
        
        swap_scan_header(header);
        retcode = pipe_write(header, sizeof(SCAN_HEADER));
    }
}

static void send_success( IOReg *Pipe, SCAN_MESSAGE_TYPE msg )
{
    send_cmd(Pipe,msg,e_Success);
}

static void send_failure( IOReg *Pipe, SCAN_MESSAGE_TYPE msg )
{
    send_cmd(Pipe,msg,e_Fail);
}

static void send_busy( IOReg *Pipe, SCAN_MESSAGE_TYPE msg )
{
    send_cmd(Pipe,msg,e_Busy);
}

static void send_notrecogized( IOReg *Pipe, SCAN_MESSAGE_TYPE msg )
{
    send_cmd(Pipe,msg,e_CmdNotRecognized);
}

static void flush_pipe()
{
    uint8_t buff[32];
    uint8_t reads = 32;
    uint8_t bytesRead;
    do
    {
        bytesRead = Pipe->ReadConnect(Pipe, buff, 32, PARSEREAD_ANY);
        DBG_PRINTF_SDH_DEBUG("flushing %d \n", bytesRead);
    } while ( bytesRead && --reads );
}

/**
 * \brief Setup a start page for the host and scanner.
 *
 *
 * \author Brad Smith
 * \date 26-Aug-2005
 *
 */

static void scan_start_page( uint8_t page_side )
{
    SCAN_HEADER hdr;

    DBG_PRINTF_SDH_DEBUG( "scanapp %s page_side=%d\n", __FUNCTION__, page_side );

    memset( cur_row_number[page_side], 0, sizeof(cur_row_number[page_side]) );

    /* davep 07-Jun-2009 ; adding better support for ADF (scantask
     * now in charge of pagestart/pageend)
     */
    /* davep 15-Apr-2013 ; adding page_side */
    DBG_PRINTF_SDH_DEBUG("Sending StartSheet to host\n");
    send_success( Pipe, e_StartSheet );

	DBG_PRINTF_SDH_DEBUG("Sending StartPage to host\n");
    /* davep 15-Apr-2013 ; adding page_side so we have to carefully create our
     * own scan header
     */
    memset( &hdr, 0, sizeof(SCAN_HEADER));
    hdr.Cookie = ASP_COOKIE;
    hdr.Message = e_StartPage;
    hdr.Status = e_Success;
    hdr.Param1 |= SCAN_MSG_START_PAGE_SET_PAGE_SIDE(page_side);
    swap_scan_header( &hdr );

    send_scan_header( Pipe, &hdr );
}

static void
increment_scanned_page_count( bool adf_scan )
{
    uint32_t     numScans=0;
    
    if (adf_scan == true) {
//        scands_get_host_pages_scanned_ADF(&numScans);
        numScans++;
//        scands_set_host_pages_scanned_ADF(&numScans);
    }
    else {
//        scands_get_host_pages_scanned_flatbed(&numScans);
        numScans++;
//        scands_set_host_pages_scanned_flatbed(&numScans);
        
    }
}

/**
 * \brief  Return the scan platform string.
 *
 * Return a set of strings describing the scan platform. The strings are like
 * HTTP header strings.
 *
 * key: value\r\n
 *
 * Example:
 * endian: MSB\r\n
 * flatbed: 0\r\n
 *
 * Originally created to simply return endian MSB/LSB. Getting tired of
 * guessing.
 *
 * \author David Poole
 * \date 07-Sep-2010
 *
 */

static scan_err_t send_platform_string( IOReg *Pipe )
{
    SCAN_HEADER *header;
    int retcode;
    char *msg, *ptr, *s, *endptr;
    int msglen;
    int slen;
    
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
    
#define MSG_SIZE 1024
    /* build the full platform string here */
    msg = MEM_MALLOC( MSG_SIZE+1 );
    if( msg==NULL ) {
        /* memory alloc failure */
        return SCANERR_OUT_OF_MEMORY;
    }
    memset( msg, 0, MSG_SIZE+1 );
    
    ptr = msg;
    endptr = msg + MSG_SIZE;
    
#undef MSG_SIZE
    
#define CONCAT do {                             \
        slen = strlen( s );                     \
        XASSERT( ptr+slen < endptr, slen );     \
        strncat( ptr, s, slen );                \
        ptr += slen;                            \
    } while(0);
    
#ifdef ASP_ON_LITTLE_ENDIAN
    s = "endian: LSB\r\n";
#else 
    s = "endian: MSB\r\n"; 
#endif
    CONCAT;
    
    if( scan_capabilities.has_adf ) {
        s = "adf: 1\r\n";
    }
    else {
        s = "adf: 0\r\n";
    }
    CONCAT;
    
    if( scan_capabilities.has_flatbed ) {
        s = "flatbed: 1\r\n";
    }
    else {
        s = "flatbed: 0\r\n";
    }
    CONCAT;
    
    msglen = ptr - msg;
    
#undef CONCAT
    
    header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
    header->Cookie = ASP_COOKIE;
    header->Message = e_GetPlatformString;
    header->Status = e_Success;
    header->DataLength = msglen;
    
    swap_scan_header(header);
    
    retcode = pipe_write(header, sizeof(SCAN_HEADER));

    /* the pipe_write function will free the pointer */
    retcode = pipe_write( msg, msglen );
    
    return SCANERR_NONE;
}

/**
 * \brief Handle end of a scanned page.
 *
 * Does all the necessary pageend tasks in a scan-to-host.  Sends an
 * end page and end sheet to the host.  Increments all the scan page counts.
 *
 * \author Brad Smith
 * \date 25-Aug-2005
 *
 */

static void scan_end_page( uint8_t page_side )
{
    SCAN_HEADER hdr;
    uint32_t       PageCnt=0;
    
    DBG_PRINTF_SDH_DEBUG( "scanapp %s page_side=%d\n", __FUNCTION__, page_side );
    
    // Send end-page & end-sheet to host

    /* davep 15-Apr-2013 ; adding page_side so must craft our own message */
    memset( &hdr, 0, sizeof(SCAN_HEADER));
    hdr.Cookie = ASP_COOKIE;
    hdr.Message = e_EndPage;
    hdr.Status = e_Success;
    hdr.Param1 |= SCAN_MSG_END_PAGE_SET_PAGE_SIDE(page_side);
    swap_scan_header( &hdr );
    send_scan_header( Pipe, &hdr );

    /* davep 23-Jul-2013 ; if we're doing duplex scanning only send end_sheet
     * when 2nd side is done 
     */
    DBG_PRINTF_SDH_DEBUG( "%s page_side=%d max_page_side=%d\n", __FUNCTION__, 
            page_side, max_page_side );
    if( page_side==1 && max_page_side==1 ) {
        /* second end_page of dual scan so send end sheet */
        send_success( Pipe, e_EndSheet );
    }
    else if( page_side==0 && max_page_side==0 ) {
        /* only end_page we'll see so send end sheet */
        send_success( Pipe, e_EndSheet );
    }
    
    //
    // Update the counts.
    //
    // Increment the page counts.
#if 0
    if(JobSettings.ScanType == e_ColorScan)
    {
//        scands_get_color_scan_pages(&PageCnt);
        PageCnt++;
//        scands_set_color_scan_pages(&PageCnt);
    }
    else
    {
//        scands_get_mono_scan_pages(&PageCnt);
        PageCnt++;
//        scands_set_mono_scan_pages(&PageCnt);
    }
#else
	PageCnt++;
#endif
    
    /* The config page wants to track how many pages came from the flatbed
     * versus the adf.  This could get confusing given the mono versus color
     * counts kept above ... but they are distict for now.
     */
    increment_scanned_page_count(g_scan_started_from_adf);    
}

/**
 * \brief End a scan job. 
 *
 * Sends an end-job message to the host. Sends the necessary message
 * to the system job manager to release myself as a resource.
 *
 * \author Brad Smith
 * \date 26-Aug-2005
 *
 */

static void scanapp_end_job( void )
{
    if ( State != SCAN_APP_TOCANCEL )
    {
        DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
        send_success( Pipe, e_EndJob );
        
        // Make sure all writes are completed. We need to get a ReleaseResources
        // from the host before we send a FREERECIPE to CMPARSER and SJMID.
        Pipe->SendIOCmd(Pipe, e_close_write, 0);
        
		DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars: scanapp_end_job\n" );
        scanvar_delete( &g_scanvars );
        XASSERT( g_scanvars==NULL, (uint32_t)g_scanvars );
        
        STATE_TRANSITION( SCAN_APP_WAIT_FOR_RELEASE );
        
		pcscan_stats.page_starts	= 0;
    }
}

/**
 * \brief  When our USB parsing fails, tell the status manager we've failed.
 *
 * \author David Poole
 * \date 21-Dec-2005
 *
 */

static void
abort_parse( void )
{
    /* davep 18-Oct-2010 ; FIXME this function is superflous; replace with
     * something better
     */
	scanapp_cancel_job();

    STATE_TRANSITION( SCAN_APP_TOCANCEL );
    
    scanapp_log_event( SCANAPP_EVENT_DEVICE_CANCEL );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 28-Sep-2007
 *
 */

static void
send_cancel_ack_msg( void *param3 )
{
    MESSAGE msg;
    error_type_t err;
    
    /* davep 18-Oct-2007 ; WTF is param3? SJM seems to want it */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_ACK;
    msg.param2 = COPYAPPID;
    msg.param3 = param3;
    err = SYMsgSend( SJMID, &msg);
    XASSERT( err==OK, err );
}

/**
 * \brief  verify user scaler settings 
 *
 * Since these numbers come from the user and could easily crash the firmware
 * (bad scale values), do some very careful checks before we accept them.
 *
 * \author David Poole
 * \date 15-Mar-2007
 *
 */

static scan_err_t verify_scaler( XY_SCALE_TYPE *XYScale )
{
    scan_err_t scerr;
    
    /* be very conservative to avoid zero division, integer overflow, etc. Note
     * that the XYScale fields are currently unsigned long. We pass around int
     * later in the code. 
     */
#define CHECK_RANGE(n)  ((n)==0||(n)>SCANVAR_MAX_SCALE_TERM)
    if( CHECK_RANGE(XYScale->x_numerator) ) {
        return SCANERR_INVALID_PARAM;
    }
    if( CHECK_RANGE(XYScale->x_denominator) ) {
        return SCANERR_INVALID_PARAM;
    }
    if( CHECK_RANGE(XYScale->y_numerator) ) {
        return SCANERR_INVALID_PARAM;
    }
    if( CHECK_RANGE(XYScale->y_denominator) ) {
        return SCANERR_INVALID_PARAM;
    }
#undef CHECK_RANGE
    
    scerr = scanvar_verify_xyscale( XYScale->x_numerator, 
                                    XYScale->x_denominator, 
                                    XYScale->y_numerator, 
                                    XYScale->y_denominator );
    
    return scerr;
}

/**
 * \brief verify we can support the requested scan dpi
 *
 * \author David Poole
 * \date 18-Nov-2010
 *
 */

static scan_err_t verify_resolution( SCAN_JOB_SETTINGS *job_settings )
{
    int i;
    uint32_t max_dpi;
    
    /* As a nifty feature, we'll convert weird dpis (e.g., 333, 800, etc) to a
     * hardware dpi and use the scalar to convert to that dpi.
     *
     * However, we will only scale down to prevent really stupid stuff from
     * happening (e.g., 9600 dpi).
     *
     * Find the max dpi this platform will support. If the request dpi is
     * greater than that, don't attempt.
     */
    
    max_dpi = 0;
    
    /* davep 07-Dec-2010 ; TODO ; don't hardwire this to flatbed. Doing the
     * happy lazy thing for now. See also ConvertHostResToDeviceRes().
     */
    for( i=0 ; ; i++ ) {
        if( scan_capabilities.fb_supported_horiz_dpi[i]==0 ) {
            /* end of list */
            break;
        }
        if( max_dpi < scan_capabilities.fb_supported_horiz_dpi[i] ) {
            max_dpi = scan_capabilities.fb_supported_horiz_dpi[i];
        }
    }
    
    if( job_settings->Resolution > max_dpi ) {
        /* failure! boo! */
        return SCANERR_INVALID_PARAM;
    }
    
    /* success! */
    return SCANERR_NONE;
}

/** 
 * \brief Check the scan_job_settings structure for correct values.
 * This takes the job_settings structure and verifies the settings that they
 * are within range.
 * 
 * \param job_settings The structure to verify
 * 
 * \retval uint32_t 1 is fail, 0 is valid.
 * 
 * \author Dirk Bodily
 * 
 * \date 1/11/2006
 * 
 * davep 15-Mar-2007 ; add better check on scale
 **/

static scan_err_t VerifySettings(SCAN_JOB_SETTINGS *job_settings)
{
    scan_err_t scerr;
    
    if(job_settings->BitsPerPixel != 8 && job_settings->BitsPerPixel != 16 ) {
        DBG_PRINTF_SDH_DEBUG("%s bad BPP=%ld\n", __FUNCTION__, job_settings->BitsPerPixel );
        return SCANERR_INVALID_PARAM;
    }
    if((job_settings->ScanType != e_MonochromeScan) &&
       (job_settings->ScanType != e_ColorScan) && 
       (job_settings->ScanType != e_ScanThroughCopy) &&
		(job_settings->ScanType != e_BilevelScan)) {
        DBG_PRINTF_SDH_DEBUG( "%s bad scantype=%ld\n", __FUNCTION__, job_settings->ScanType );
        return SCANERR_INVALID_PARAM;
    }
    
    /* davep 03-Jan-2012 ; add sneaky check for our test/debug tools so we can
     * do planar even if PIE doesn't support it
     */
    if( job_settings->DataType==SCAN_DATA_TYPE_PLANE &&
        ((job_settings->flags & SCAN_JOB_FLAGS_CAL_TO_HOST) || (job_settings->BitsPerPixel==16)) ) {
        /* allow */
    }
    else {
        if( !scancap_has_data_type( (scan_data_type)job_settings->DataType ) ) {
            DBG_PRINTF_SDH_DEBUG( "%s bad data_type=%d\n", __FUNCTION__, job_settings->DataType );
            return SCANERR_INVALID_PARAM;
        }
    }
    
    if((job_settings->Gamma < 10) ||
       (job_settings->Gamma > 100)) {
        DBG_PRINTF_SDH_DEBUG( "%s bad gamma=%ld\n", __FUNCTION__, job_settings->Gamma);
        return SCANERR_INVALID_PARAM;
    }
    scerr = verify_resolution( job_settings );
    if( scerr != SCANERR_NONE ) {
        DBG_PRINTF_SDH_DEBUG( "%s bad resolution=%ld\n", __FUNCTION__, job_settings->Resolution );
        return scerr;
    }
    scerr = verify_scaler( &job_settings->XYScale ); 
    if( scerr != SCANERR_NONE  ) {
        return scerr;
    }
    
    /* davep 07-May-2009 ; XXX useful debug to capture which setting is failing */
#if 0    
    if(job_settings->scan_window.bottom < DefJobSettings.ScannableArea.top+MIN_SCAN_SIZE){
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if(job_settings->scan_window.bottom > DefJobSettings.ScannableArea.bottom) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if (job_settings->scan_window.left < DefJobSettings.ScannableArea.left) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if(job_settings->scan_window.left > DefJobSettings.ScannableArea.right) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if(job_settings->scan_window.right < DefJobSettings.ScannableArea.left+MIN_SCAN_SIZE) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if(job_settings->scan_window.right > DefJobSettings.ScannableArea.right) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if (job_settings->scan_window.top < DefJobSettings.ScannableArea.top) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if(job_settings->scan_window.top > DefJobSettings.ScannableArea.bottom) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if((job_settings->scan_window.bottom - job_settings->scan_window.top) < MIN_SCAN_SIZE) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
    if((job_settings->scan_window.right - job_settings->scan_window.left) < MIN_SCAN_SIZE) {
        DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, __LINE__ );
    }
#endif
    
    if((job_settings->scan_window.bottom < DefJobSettings.ScannableArea.top+MIN_SCAN_SIZE) || 
       (job_settings->scan_window.bottom > DefJobSettings.ScannableArea.bottom) ||
       (job_settings->scan_window.left < DefJobSettings.ScannableArea.left) ||
       (job_settings->scan_window.left > DefJobSettings.ScannableArea.right) ||
       (job_settings->scan_window.right < DefJobSettings.ScannableArea.left+MIN_SCAN_SIZE) ||
       (job_settings->scan_window.right > DefJobSettings.ScannableArea.right) ||
       (job_settings->scan_window.top < DefJobSettings.ScannableArea.top) ||
       (job_settings->scan_window.top > DefJobSettings.ScannableArea.bottom) ||
       ((job_settings->scan_window.bottom - job_settings->scan_window.top) < MIN_SCAN_SIZE) ||
       ((job_settings->scan_window.right - job_settings->scan_window.left) < MIN_SCAN_SIZE)) {
        
        DBG_PRINTF_SDH_DEBUG( "%s bad area=%ld,%ld,%ld,%ld\n", __FUNCTION__, 
              job_settings->scan_window.top, 
              job_settings->scan_window.left, 
              job_settings->scan_window.bottom, 
              job_settings->scan_window.right );
        
        return SCANERR_INVALID_PARAM;
    }
    
    /* davep 13-Sep-2006 ; sneaky variables for scan-to-host via copy path */
    if( job_settings->ScanType == e_ScanThroughCopy ) {
        /* Hiding the copy quality in the 'reserved1' field. 
         *
         * A value of 0 means "use the default" and won't actually change
         * anything. Since e_COPYQUALITY of 0 is also "text", I'm shifting the
         * values up by one (thus the -1).
         */
        if( job_settings->reserved1 > 0 && 
            (job_settings->reserved1-1) >= e_COPYQUALITY_MAX ) {
            DBG_PRINTF_SDH_DEBUG("%s reserved1=%#lx\n", __FUNCTION__, job_settings->reserved1 );
            return SCANERR_INVALID_PARAM;
        }
    }
    
    return SCANERR_NONE;
}

#ifdef HAVE_SCANHIQ_SUPPORT
/**
 * \brief  Look for special end of siqfile string. 
 *
 * USB doesn't have any sort of concept of "socket close" so we won't know when
 * the host has finished sending us the file. Look for a kludgy "end of file"
 * string (as of this writing, it's "# END-OF-FILE-FOR-USB") so we know we have
 * the entire file.
 *
 * \author David Poole
 * \date 21-Dec-2005
 *
 */

static bool
siqfile_eof( char *buf, int buflen )
{
    int eof_len;
    int i;
    
    eof_len = strlen( (char *)SIQEndOfFile );
    
    if( buflen < eof_len ) {
        return false;
    }
    
    /* search backwards from the end of buffer three times; should
     * handle any weird end of line character combination: \r|\n|\r\n
     */
    for( i=0 ; i<3 ; i++ ) {
        if( memcmp( &buf[buflen-eof_len-i], SIQEndOfFile, eof_len )==0 ) {
            DBG_PRINTF_SDH_DEBUG( "found siq end-of-file marker, ending USB parse\n" );
            return true;
        }
    }
    
    return false;
}

/**
 * \brief  read a siqfile (SIQ - Scan Image Quality) from USB
 *
 * Loops reading SIQFILE data from USB. When entire file has been read, send to
 * scanvars for parsing and storage.
 *
 * \author David Poole
 * \date 21-Dec-2005
 *
 */

static int
parse_siq_file( char *initial_data, int len_of_initial_data,IOReg *scanhostpipe )
{
    char *buf, *tmpbuf;
    int buflen, bufmaxlen;
    int32_t bytes_read;
    char *readbuf;
    int zero_read_count;
    
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
    
#define READBUFSIZE 2048
    
    /* we read into a scratch buffer then copy the data into our destination
     * buffer. The destination buffer will be realloc'd as necessary 
     */
    readbuf = MEM_MALLOC( READBUFSIZE );
    if( readbuf==NULL ) {
        DBG_PRINTF_SDH_DEBUG( "out of memory for siqfile buffer\n" );
        return -1;                    
    }
    
    bufmaxlen = 2048 + len_of_initial_data;
    buflen = 0;
    buf = MEM_MALLOC( bufmaxlen );
    if( buf==NULL ) {
        PTR_FREE( readbuf );
        DBG_PRINTF_SDH_DEBUG( "out of memory for siqfile buffer\n" );
        return -1;
    }
    
    memset( buf, 0, bufmaxlen );
    memcpy( buf, initial_data, len_of_initial_data );
    buflen = len_of_initial_data;
    
    zero_read_count = 0;
    
    while( 1 ) {
        memset( readbuf, 0, READBUFSIZE );
        
        bytes_read = scanhostpipe->ReadConnect(scanhostpipe, readbuf, READBUFSIZE, PARSEREAD_ANY);
        
        if( bytes_read < 0 ) {
            DBG_PRINTF_SDH_DEBUG( "pipe returned %ld; siqfile parse failed\n", bytes_read );
            abort_parse();
            goto fail;
        }
        
        DBG_PRINTF_SDH_DEBUG( "read %ld bytes from usb\n", bytes_read );
        
        /* this isn't foolproof, but if we haven't seen the end-of-file marker,
         * quit anyway
         */
        if( bytes_read==0 ) {
            zero_read_count++;
            if( zero_read_count==10 ) {
                DBG_PRINTF_SDH_DEBUG( "assuming end of siq file\n" );
                break;
            }
            posix_sleep_ticks( 100 );
            continue;
        }
        zero_read_count = 0;
        
        /* If we don't have enough space for the current data, reallocate our
         * buffer. Otherwise, just copy into destination buffer and go back for
         * more.
         */
        /* -1 to convert from index to length */
        if( bytes_read > bufmaxlen-buflen-1 ) {
            /* reallocate */
            DBG_PRINTF_SDH_DEBUG( "realloc siqfile buf from %d to %d\n", bufmaxlen, bufmaxlen*2 );
            tmpbuf = MEM_MALLOC( bufmaxlen*2 );
            if( tmpbuf==NULL ) {
                DBG_PRINTF_SDH_DEBUG( "out of memory for siqfile buffer\n" );
                goto fail;
            }
            
            DBG_PRINTF_SDH_DEBUG( "move old %p to new %p\n", buf, tmpbuf );
            
            memset( tmpbuf, 0, bufmaxlen*2 );
            memcpy( tmpbuf, buf, buflen );
            
            PTR_FREE( buf );
            PTR_ASSIGN( buf, tmpbuf );
            
            bufmaxlen *= 2;
        }
        
        /* concatenate new data onto our big buffer */
        memcpy( &buf[buflen], readbuf, bytes_read );
        buflen += bytes_read;
        XASSERT( buflen<bufmaxlen, buflen );
        
        /* try to parse for end-of-file string since USB can't tell us when the
         * file actually ends (no concept of socket close or end of file)
         */
        if( siqfile_eof( buf, buflen ) ) {
            break;
        }
        
    } /* end while(1) */
    
#undef READBUFSIZE
    
    
    /* pass the data off to the siq parser; ignore any error */
    scanvar_parse_file( buf, buflen, bufmaxlen );
    
    PTR_FREE( buf );
    PTR_FREE( readbuf );
    
    DBG_PRINTF_SDH_DEBUG( "%s done\n", __FUNCTION__ );
    return 0;
    
fail:
    PTR_FREE( readbuf );
    PTR_FREE( buf );
    return -1;
}
#endif /* HAVE_SCANHIQ_SUPPORT */

/**
 * \brief Setup a scan erase
 *
 * Setup a scan erase
 *
 */
#if SDH_USE_ERASE_BORDER
static scan_err_t setup_scan_eraseborder( SCAN_JOB_SETTINGS *settings )
{
    scan_err_t scerr = SCANERR_NONE;

	uint32_t eraseLeftPixel = 0;
	uint32_t eraseRightPixel = 0;
	uint32_t eraseTopPixel = 0;
	uint32_t eraseBottomPixel = 0;
	
	uint32_t user_width_hinchs = 0;
	uint32_t user_height_hinchs = 0;
	uint32_t user_width_pixel = 0;
	uint32_t user_height_pixel = 0;

	user_width_hinchs = settings->scan_window.right - settings->scan_window.left;
	user_height_hinchs = settings->scan_window.bottom - settings->scan_window.top;
	user_width_pixel = (user_width_hinchs * settings->Resolution)/100;
	user_height_pixel = (user_height_hinchs * settings->Resolution)/100;
	
	eraseLeftPixel = (settings->Resolution * DEFAULT_BORDER_LEFT_MM * 100) / (2540);
	eraseRightPixel = (settings->Resolution * DEFAULT_BORDER_LEFT_MM * 100) / (2540);
	eraseTopPixel = (settings->Resolution * DEFAULT_BORDER_LEFT_MM * 100) / (2540);
	eraseBottomPixel = (settings->Resolution * DEFAULT_BORDER_LEFT_MM * 100) / (2540);

	pcscan_stats.eraseLeftPixel = eraseLeftPixel;
	pcscan_stats.eraseRightPixel = eraseRightPixel;
	pcscan_stats.eraseTopPixel = eraseTopPixel;
	pcscan_stats.eraseBottomPixel = eraseBottomPixel;
	
	DBG_PRINTF_SDH_DEBUG("%s L Pixel (%d)\n", __FUNCTION__, eraseLeftPixel);
	DBG_PRINTF_SDH_DEBUG("%s R Pixel (%d)\n", __FUNCTION__, eraseRightPixel);
	DBG_PRINTF_SDH_DEBUG("%s T Pixel (%d)\n", __FUNCTION__, eraseTopPixel);
	DBG_PRINTF_SDH_DEBUG("%s B Pixel (%d)\n", __FUNCTION__, eraseBottomPixel);
	
	
	return scerr;
}
#endif //SDH_USE_ERASE_BORDER


/**
 * \brief calculate the scan area based on the user's request and hardware 
 *
 * Moved the scanvar_set_area() into a function so we can encapsulate all the
 * margin evils into one place.
 *
 * \author David Poole
 * \date 25-Jun-2009
 *
 * davep 03-Feb-2012 ; pull in setup_scan_area() from previous project. Better
 *                     handling of clipping user area to available flatbed
 *                     margins
 */

static scan_err_t setup_scan_area( struct scanvars *sv, SCAN_JOB_SETTINGS *settings )
{
    int32_t user_x, user_y;
    int32_t user_width, user_height;
    scan_err_t scerr = SCANERR_NONE;
    struct scan_flatbed_margins fb_margins;
    struct scan_adf_margins adf_margins;
    uint32_t dpi_horiz, dpi_vert;
    
    /* all these numbers are in 1/100" */
    
    user_x = settings->scan_window.left;
    user_y = settings->scan_window.top;
    
    user_width = settings->scan_window.right - settings->scan_window.left;
    user_height = settings->scan_window.bottom - settings->scan_window.top;
    
    DBG_PRINTF_SDH_DEBUG( "%s left=%d top=%d right=%d bottom=%d\n", __FUNCTION__, 
          settings->scan_window.left, 
          settings->scan_window.top,
          settings->scan_window.right,
          settings->scan_window.bottom );
    DBG_PRINTF_SDH_DEBUG( "%s x=%d y=%d width=%d height=%d\n", 
          __FUNCTION__, user_x, user_y, user_width, user_height );
    
    scanvar_get_hw_dpi( sv, &dpi_horiz, &dpi_vert );
    
    /* We need to get the platform margin data based on the document source, dpi, 
     * and color mode.
     * Adjust platform margins (need to clip off bits of the mech)
     */

	//Factory Auto Registration IQT -> Reset DB
	if( pcscan_stats.scanJob_type == PC_SCAN_FACTORY_REGISTRATION ) {
		if( sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED ) {
			scanner_service_reset_DB( IQT_AUTO_REGISTRATION_FROM_FB );
		}	
		else if( sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF ) {
			scanner_service_reset_DB( IQT_AUTO_REGISTRATION_FROM_ADF_FRONT );
		}
		else if( sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF_DUPLEX ) {
			scanner_service_reset_DB( IQT_AUTO_REGISTRATION_FROM_ADF_BACK );
		}
		DBG_PRINTF_SDH_DEBUG("%s: PC_SCAN_FACTORY_REGISTRATION\n", __FUNCTION__ );
	}

	//Sync Margin Table with DB
	scanner_service_sync_marginTable();
	
	if (sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED) {
		scerr = scanmech_get_fb_margins( sv->dpi,
											 sv->cmode,
											 &fb_margins );

        if( scerr == SCANERR_NONE ) {
            DBG_PRINTF_SDH_DEBUG("%s: Flatbed left_mar_x=%d top_mar_y=%d\n", __FUNCTION__, 
                 fb_margins.left_margin_hinches_x, fb_margins.top_margin_hinches_y);

            user_x += fb_margins.left_margin_hinches_x;
            user_y += fb_margins.top_margin_hinches_y;
		}
        else {
			DBG_PRINTF_SDH_DEBUG("Can't get FB Margin. ERR: %d\n", scerr);
		}
		
		if(user_height > MAX_SCAN_FB_HEIGHT_HINCHES) {
			user_height = MAX_SCAN_FB_HEIGHT_HINCHES;
			DBG_PRINTF_SDH_DEBUG("%s: Flatbed height is limited to : %d\n", __FUNCTION__, user_height);
		}

		DBG_PRINTF_SDH_DEBUG( "%s user x=%d y=%d width=%d height=%d\n", __FUNCTION__, user_x, user_y, user_width, user_height );
    }
    else if (sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF || sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
		scerr = scanmech_get_adf_margins( sv->dpi,
											  sv->cmode,
											  &adf_margins );
											  		
        if( scerr == SCANERR_NONE ) {
            
            DBG_PRINTF_SDH_DEBUG("%s: ADF left_mar=%d\n", __FUNCTION__, adf_margins.left_margin_hinches);            
            user_x += adf_margins.left_margin_hinches;
			/* SINDOH */
			//For ADF top margin, we use margin table located in Kernel
		}
    }
    
    scerr = scanvar_set_area( sv, user_x, user_y, user_width, user_height );
    
#if SDH_USE_ERASE_BORDER
	//Setup scan erase border
	if( (pcscan_stats.scanJob_type == PC_SCAN_NORMAL) && 
		(g_scan_started_from_A3fb == FALSE) && 
		(g_scan_started_from_DLTfb == FALSE) &&
		(g_scan_started_from_Legalfb == FALSE) 
		) {
		setup_scan_eraseborder(settings);
	}
	else {
		DBG_PRINTF_SDH_DEBUG("This is merge scan job, so we don't Setup erase border\n");
	}
#endif
    
    return scerr;
}

/**
 * \brief Cancel the job  
 * 
 * There is no Official Blessed One True Way to cancel a job. So pretend we got
 * a user cancel.
 *
 * \author David Poole
 * \date 25-Jun-2009
 *
 */

static void scanapp_cancel_job( void )
{
    MESSAGE msg;
    error_type_t err;
    
    /* send a user cancel message to the job manager (this is the only way I
     * know to cancel a job; the SJM is a bit opaque)
     */
    
    /* Paul W. 15-Aug-2009 ; Switched to a SYS_REQUEST cancel where we specify
     * the resource in param3 then it will cancel the first scan job in the job
     * queue.  The former, SYS_REQUSER, cancels the first job in the job queue
     * regardless of job type.
     * 
     * I added this to avoid canceling print jobs that were in the job queue.
     */
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_REQUEST;
    msg.param2 = SCANPARSEID;
    msg.param3 = (void*) e_IOScanParser;
    
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );

	DBG_PRINTF_SDH_DEBUG("%s requested\n", __FUNCTION__);

}

/**
 * \brief  Based on request from user and current ADF status, pick a scan
 * document source.
 *
 * \author David Poole
 * \date 15-Jul-2009
 *
 */

static scan_err_t choose_document_source( struct scanvars *sv, 
                                          SCAN_JOB_SETTINGS *job_settings,
                                          bool adf_paper_present )
{
    uint8_t docsrc;
    
    ASSERT( sv!=NULL );
    
    docsrc = SCAN_JOB_FLAGS_GET_DOCUMENT_SOURCE(job_settings->flags);
    
    /* davep 27-Jul-2009 ; if we don't have an ADF, hardwire to flatbed no
     * matter what the host says (short, sweet, easy fix that I'll probably get
     * yelled at for in a couple years)
     */
    if( !scan_capabilities.has_adf ) {
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        return 0;
    }
    
    switch( docsrc ) {
    case SCAN_JOB_SETTINGS_DOC_SRC_AUTO :
        if( adf_paper_present ) {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
        }
        else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        }
        break;
        
    case SCAN_JOB_SETTINGS_DOC_SRC_FLATBED :
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        break;
        
    case SCAN_JOB_SETTINGS_DOC_SRC_ADF :
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
        break;

    case SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX:
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF_DUPLEX);
        break;
        
#if USE_SDH_A3_MERGE_PCSCAN
	case 4: //SCAN_JOB_SETTINGS_DOC_SRC_FLATBED_A3:
		DBG_PRINTF_SDH_DEBUG("A3 Merge Scan SET\n");
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        g_scan_started_from_A3fb = TRUE;
		break;
	case 5:
		DBG_PRINTF_SDH_DEBUG("DLT Merge Scan SET\n");
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
		g_scan_started_from_DLTfb = TRUE;
		scanapp_merge_adf_roller_move(SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE);
		break;
	case 6:
		DBG_PRINTF_SDH_DEBUG("Legal Merge Scan SET\n");
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
		g_scan_started_from_Legalfb = TRUE;
#endif
        
    default :
        DBG_PRINTF_SDH_DEBUG( "%s ignoring unknown docsrc=%d from host\n", 
              __FUNCTION__, docsrc );
        
        if( adf_paper_present ) {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
        }
        else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        }
        break;
    }
    
    return 0;
}

void scanapp_merge_adf_roller_move(int direction) 
{
    scan_err_t scerr;
	uint32_t req_id;
		    
	if(direction == SCAN_MERGE_IOCTL_PARAM_A3_TO_BACK) {
		//The documnet set on the FB Glass move to Back for next FB Scan.
		scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, SCAN_MERGE_IOCTL_PARAM_A3_TO_BACK, NULL, &req_id);
		
		if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint BACK > direction:0 : Success\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint BACK : FAIL : %d\n", scerr);						
		}
	}
	else if(direction == SCAN_MERGE_IOCTL_PARAM_A3_EJECT) {
		//The documnet set on the FB Glass move to Front for Eject the paper.
		scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, SCAN_MERGE_IOCTL_PARAM_A3_EJECT, NULL, &req_id);
		
		if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint FOWARD > direction:1 : Success\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint FOWARD : FAIL : %d\n", scerr);						
		}
	}
	else if(direction == SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE) {
		//The documnet set on the FB Glass move to Back just 6mm before FB Scan.
		scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE, NULL, &req_id);
		
		if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint Back(6mm) : Success\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint Back(6mm) : FAIL : %d\n", scerr);						
		}		
	}
	else if(direction == SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK) {
		//The documnet set on the FB Glass move to Back for next FB Scan.
		scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK, NULL, &req_id);
		
		if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint BACK : Success\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint BACK : FAIL : %d\n", scerr);						
		}
	}
	else if(direction == SCAN_MERGE_IOCTL_PARAM_DLT_EJECT) {		
		//The documnet set on the FB Glass move to Front for Eject the paper.
		scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, SCAN_MERGE_IOCTL_PARAM_DLT_EJECT, NULL, &req_id);
		
		if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint FOWARD : Success\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT scanpoint FOWARD : FAIL : %d\n", scerr);						
		}
	}
}

/**
 * \brief  Parse scan control data from the host.
 *
 * Look for ASP (Agilent Scan Protocol) or SIQfile (Scan Image Quality) data
 * coming over USB.  Parse the protocol and do what is requested.
 *
 * Leveraged from code originally in the message loop in scanapp_task().
 * 
 * \param[in,out] ScanTimeout Timer count used by the caller to disconnect from
 *          the host after a certain amount of inactivity 
 * 
 * \retval Returns true if the pipe read was successful, and no errors.  Returns false if the pipe was closed
 * or we received an error (or we parsed a SIQ file) and we should quit 
 *
 * \author David Poole
 * \date 21-Dec-2005
 *
 */


static bool poll_messages_from_host(IOReg *scanhostpipe)
{
    int32_t bytesRead;
    SCAN_HEADER read_header;
    SCAN_HEADER *read_header_msg;
    error_type_t err;
    MESSAGE msg;
//    static unsigned long counter=0;
    
    // This next section handles the communication with the host.  
    // Commands are received and processed.
#ifdef SCANAPP_DEBUG
	//DBG_PRINTF_SDH_DEBUG( "%s %d\n", __FUNCTION__, counter++ );
#endif
    
    // Now do the parsing
    // Read whatever is there and then parse.  If nothing go back to the top.
    memset( &read_header, 0, sizeof(read_header) );
    bytesRead = scanhostpipe->ReadConnect(scanhostpipe, &read_header, sizeof(SCAN_HEADER), PARSEREAD_EXACT);
    //
    // Check for an error on the channel
    //
    //

    if( bytesRead < 0 )
    {
        DBG_PRINTF_SDH_DEBUG( "%s error? bytesRead=%d\n", __FUNCTION__, bytesRead );

        scanapp_jr_started = false;
        // We had an error on the channel.  Abort everything.
		if( (State != SCAN_APP_IDLE) && (job_resources != NULL) ) {
			// If state is idle we are done and we can return
			abort_parse();
		}
        return false;
    }
    
    /* davep 21-Dec-2005 ; adding SIQ parser */
#ifdef HAVE_SCANHIQ_SUPPORT
    if( memcmp( (char *)&read_header, SIQCmpString, strlen((char *)SIQCmpString) ) == 0) {
        DBG_PRINTF_SDH_DEBUG( "parsing host scan IQ file\n" );
        
        parse_siq_file( (char *)&read_header, bytesRead,scanhostpipe );
        /* ignoring error code */
        
        //
        // We're finished parsing. Tell the connection manager the results of the parse.
        //
        memset( &msg, 0, sizeof(msg) );
        msg.param1 = 0; // TODO: replace this with the results of the parse
        msg.msgType = MSG_END_PARSE;  // this should close the pipe
        msg.param3 = scanhostpipe;          // The pipe that we are parsing.
        err = SYMsgSend(CMPARSER, &msg);      // send the response.
        XASSERT( err==OK, err );
        
        /* returning false tells caller we've read all the data we want and
         * we're done
         */
        DBG_PRINTF_SDH_DEBUG("Finished parsing siq file, exiting SAJ!!!\n");        
        return false;
    }
#endif
    if (bytesRead != sizeof(SCAN_HEADER)) {
        DBG_PRINTF_SDH_DEBUG("we didn't read the proper number of bytes for a scan header!!!\n");
        return true;  // problem?
    }
    
    swap_scan_header(&read_header);
    DBG_PRINTF_SDH_DEBUG("asppkt: cookie=%#lx message=%ld param1=%#lx param2=%#lx datalength=%ld status=%#lx\n",
         read_header.Cookie, read_header.Message, read_header.Param1,
         read_header.Param2, read_header.DataLength, read_header.Status );
    
    if(read_header.Cookie != ASP_COOKIE)
    {
        flush_pipe();
        send_notrecogized( scanhostpipe, 0 );
        return true;  // problem?
    } 
//    DBG_PRINTF_SDH_DEBUG( "%s state=%d hostmsg=%ld\n", __FUNCTION__, State, read_header.Message );
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SAJ_MESSAGE;  
    read_header_msg = MEM_MALLOC(sizeof(SCAN_HEADER)); // freed by parse_saj_messsages()
    if (read_header_msg==NULL)
    {
        DBG_PRINTF_SDH_DEBUG("ERROR, out of memory mallocing read_header_msg!!\n");
        /* memory alloc failure */
        return false;
    }
    memcpy(read_header_msg, &read_header,sizeof(SCAN_HEADER));
    
    switch (read_header.Message)
    {
    case e_LockScanResource:
        DBG_PRINTF_SDH_DEBUG("Received LockScanResource from host\n");
        msg.param1 = SA_JR_MSG_LOCK_SCAN;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_ReleaseScanResource:
        DBG_PRINTF_SDH_DEBUG("Received ReleaseScanResource from host\n");
        msg.param1 = SA_JR_MSG_RELEASE_SCAN;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_StartScanJob:
        DBG_PRINTF_SDH_DEBUG("Received StartScanJob from host\n");
        msg.param1 = SA_JR_MSG_START_SCAN_JOB;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_CancelScanJob:
        DBG_PRINTF_SDH_DEBUG("Received CancelScanJob from host:\n");
        msg.param1 = SA_JR_MSG_CANCEL_SCAN_JOB;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_SetScanJobSettings:
    {
        SCAN_JOB_SETTINGS *tmpJobSettings;  ///< The scan job settings.
        
        DBG_PRINTF_SDH_DEBUG("Received SetScanJobSettings from host:\n");
        // for this case, we don't ass the read_header_msg to ScanApp, but the
        // Job Settings - so free the read_header_msg and create the jobsettings
        PTR_FREE(read_header_msg);
        tmpJobSettings = MEM_MALLOC(sizeof(SCAN_JOB_SETTINGS)); // freed by parse_saj_messsages()
        
        // now we need to read the job settings from the host
        bytesRead = scanhostpipe->ReadConnect(scanhostpipe, tmpJobSettings, sizeof(SCAN_JOB_SETTINGS), 
                                              PARSEREAD_EXACT);
        
        msg.param1 = SA_JR_MSG_SET_JOB_SETTINGS;
        msg.param3 = tmpJobSettings;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
    }
    break;
    
    case e_SetDefaultScanJobSettings:
        DBG_PRINTF_SDH_DEBUG("Received SetDefaultScanJobSettings from host:\n");
        msg.param1 = SA_JR_MSG_SET_DEFAULT_JOB_SETTINGS;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_GetScanJobSettings:
        DBG_PRINTF_SDH_DEBUG("Received GetScanJobSettings from host:\n");
        msg.param1 = SA_JR_MSG_GET_JOB_SETTINGS;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_ResetXmitBuffers:
        DBG_PRINTF_SDH_DEBUG("Received ResetXmitBuffers from host:\n");
        msg.param1 = SA_JR_MSG_RESET_XMIT_BUFS;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_ResetTimeoutCounter: 
        DBG_PRINTF_SDH_DEBUG("Received ResetTimeoutCounter from host:\n");
        msg.param1 = SA_JR_MSG_RESET_TIMEOUT_COUNTER;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_AdfIsPaperPresent:
        DBG_PRINTF_SDH_DEBUG("Received AdfIsPaperPresent from host:\n");
        msg.param1 = SA_JR_MSG_ADF_IS_PAPER_PRESENT;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
        
    case e_GetPlatformString :
        DBG_PRINTF_SDH_DEBUG("Received GetPlatformString from host:\n");
        msg.param1 = SA_JR_MSG_GET_PLATFORM_STRING;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;
       
	case e_AdfInformation:
        DBG_PRINTF_SDH_DEBUG("Received e_AdfInformation from host:\n");
        msg.param1 = SA_JR_MSG_GET_ADF_INFORMATION;
        msg.param3 = read_header_msg;
        err = SYMsgSend(SCANPARSEID,&msg);
        XASSERT(err==OK, err);
        break;	
		break;
        
    default:
        flush_pipe();
        send_notrecogized( scanhostpipe, read_header.Message );
        break;
    }
    
    return true;
}

// Scan App Junior (saj)
// the thread that reads from the host (connection manager stuff).  Could be USB, or network or....
// does a blocking read until input data is received
void *scanapp_jr_task(void *scanhostpipe)
{
    bool retval;
    int s;
    
    DBG_PRINTF_SDH_DEBUG("%s is running\n", __FUNCTION__);
    
    s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (s != 0)
    {
        errno = s;
        perror("can't set cancel state for scanapp_jr thread");
        return (void *) -1;
    }
    retval = true;
    while (retval == true)
    {
        retval = poll_messages_from_host((IOReg *)scanhostpipe);
    }
    // when we leave the while loop, it is because our pipe closed or a serious error has occured
    // so we just quietly exit this thread.  We'll be created again if another scan job appears
    DBG_PRINTF_SDH_DEBUG("%s is exiting\n",__FUNCTION__);
    return 0;
}

static int kill_scanapp_jr_task(void)
{
    int retval;
    void *res;
    
    DBG_PRINTF_SDH_DEBUG("Sending cancel to scanapp_jr_task\n");
    retval = pthread_cancel(scanapp_jr_id);
    if (retval != 0)
    {
        errno = retval;
        perror("can't cancel scanapp_jr thread");
        return -1;
    }

    retval = pthread_join(scanapp_jr_id, &res);
    if (retval != 0)
    {
        errno = retval;
        perror("can't cancel scanapp_jr thread");
        return -1;
    }
    if (res == PTHREAD_CANCELED)
    {
        DBG_PRINTF_SDH_DEBUG("scanapp_jr thread was cancelled\n");
        scanapp_jr_started = false;
    }
    else
    {
        DBG_PRINTF_SDH_DEBUG("scanapp_jr thread wasn't canceled (shouldn't happen!!!!!!!!!!!!!!!)\n");
        return -1;
    }
    return 0;
}

/* FUNCTION NAME: CleanUpJob */

/** 
 * \brief Finish the job my sending finish to system.
 * Send a message to the connection manager that we are finished. Then
 * send a message to the system job manager with a free resource.  Set all
 * status flags to inactive. get out of here!
 * 
 * \author Dirk Bodily
 * 
 * \date 2/28/2006
 * 
 **/
void CleanUpJob(void)
{
    MESSAGE msg;
    uint32_t err;
    
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
    
    // tell scanapp_jr_task it's time to go away now - this can block
    kill_scanapp_jr_task();
    //
    // We're finished parsing. Tell the connection manager the results of the parse.
    //
    msg.param1 = 0; // results of the parse, we always return good.  Fail will
                    // chuck data to next valid data.
    msg.msgType = MSG_END_PARSE;  // this should close the pipe
    msg.param3 = Pipe;          // The pipe that we are parsing.
    err = SYMsgSend(CMPARSER, &msg);      // send the response.
    XASSERT( err==OK, err );
    //
    // If state is idle we are done and we can return
    // 
    
    msg.msgType = MSG_FREERECIPE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = job_resources;
    
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );
    STATE_TRANSITION( SCAN_APP_IDLE );
}

void parse_saj_messages(MESSAGE saj_message, uint32_t *ScanTimeout)
{
    SCAN_HEADER read_header;
    MESSAGE msg;
    SCAN_HEADER *header;
    scan_err_t scerr;
    int32_t pipe_retcode;
    int32_t retcode;
    SCAN_JOB_SETTINGS *jsettings;
    STARTRECIPE *StartIt;
    bool is_alive;
    uint32_t err;
	error_type_t db_retval;
	int Auth_type = 0;
    
    memcpy(&read_header,saj_message.param3,sizeof(SCAN_HEADER));
    switch (saj_message.param1)
    {
    case SA_JR_MSG_LOCK_SCAN:
        // Host wants to scan. Attempt to get the resources and reply to the host after
        // we get an ACK or NAK for the resources.
        //
        // First see if we are going, if not return an error.
        //
        is_alive = scanner_is_alive();
        if(!is_alive || State != SCAN_APP_IDLE)
        {
            DBG_PRINTF_SDH_DEBUG( "%s is_alive=%d state=%d sending FAILURE!\n", 
                  __FUNCTION__, is_alive, State );
            send_failure( Pipe, e_LockScanResource );
            break;
        }
        
		Auth_type = db_get_var_int("CONF_SOLUTIONSP", "AuthType", &db_retval);
		if(db_retval == FAIL)
		{
			DBG_PRINTF_SDH_DEBUG("%s DB Read fail : AuthType\n", __func__);
		}
		
		DBG_PRINTF_SDH_DEBUG("Auth_type(%d)\n", Auth_type);
		if ( Auth_type != 0 ){
			send_failure( Pipe, e_LockScanResource );
			DBG_PRINTF_SDH_DEBUG("Set Persinal Scan Job\n");
			break;
		}
        
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        STATE_TRANSITION( SCAN_APP_LOCKING );
        StartIt = (STARTRECIPE *) ScanGetMemory(sizeof(STARTRECIPE));
        msg.msgType = MSG_STARTRECIPE;
        StartIt->Job = ejob_ScanToHost;
        StartIt->AdditionalResource = 0;
        StartIt->Pipe = Pipe;
        
        //
        // If param2 of the header is not zero then we want to block
        // on the lock.  Set that here.
        //
        if(read_header.Param2 == 0 )
            StartIt->Wait = e_DoNotWait;
        else
            StartIt->Wait = e_WaitForever;
        StartIt->SendingModule = SCANPARSEID;
        RequestedIOTO = read_header.Param1;
        DBG_PRINTF_SDH_DEBUG( "%s RequestedIOTO=%d\n", __FUNCTION__, RequestedIOTO );
        msg.param3 = StartIt;       // freed by sjm
        err = SYMsgSend(SJMID, &msg);         // send the message.
        XASSERT( err==OK, err );
        
#ifdef HAVE_SLEEPMODE
        /* burtp 09-Oct-2009; While the scan has not officially started, lets hedge our
         *  bets and wake everyone up (we need to get the PP sensor going as soon as 
         *  possible).  If the scan doesn't start for any reason, the power manager will
         *  put us back to sleep when it's timer expires.
         *  (Power manager will call our mech code to turn things on soon as our thread
         *   yields control)
         */
        powerManagerGeneralWakeup("ScanAppLockAttempt", POWERMANAGER_WAKEUP_GENERIC);
#endif    
        break;
        
        
    case SA_JR_MSG_RELEASE_SCAN:
        //
        // in HSTCANCEL, let HOST wrap up, we will act like TIMEOUT
        // in SCANNING, let HOST exit.  Happen on CP cancel during ADF, 
        // long pause while flushing ADF, host timesout and send RELEASE or CANCEL
        if ((State == SCAN_APP_HSTCANCEL) || (State == SCAN_APP_SCANNING))
        {
            DBG_PRINTF_SDH_DEBUG("convert SCANNING or HSTCANCEL ->>  TOCANCEL\n");
            send_success( Pipe, e_ReleaseScanResource );
            // Make sure all writes to the host are complete.
            pipe_retcode = Pipe->SendIOCmd(Pipe, e_close_write, 0 );
            abort_parse();
            break;
        }
        // states to send a failure and exit.
        if((State == SCAN_APP_LOCKING) || (State == SCAN_APP_TOCANCEL) )
        {
            send_failure( Pipe, e_ReleaseScanResource );
            break;
        }
        // IDLE, READY_TO_SCAN, WAIT_FOR_RELEASE, CANCELED need close parser
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        DBG_PRINTF_SDH_DEBUG("Sending SUCCESS to host for ReleaseScanResource\n");
        send_success( Pipe, e_ReleaseScanResource );
        
	
        // Make sure all writes to the host are complete.
        /* davep 18-Oct-2007 ; (Bug 7411) bump up the timeout for slow
         * hosts (still not foolproof)
         */
		pipe_retcode = Pipe->SendIOCmd(Pipe, e_close_write, 0 );
        DBG_PRINTF_SDH_DEBUG( "%s release success pipe_retcode=%ld\n", __FUNCTION__, pipe_retcode );
        
        //
        // We're finished parsing. Tell the connection manager the results of the parse.
        //
        // tell scanapp_jr_task it's time to go away now - this can block
        kill_scanapp_jr_task();
        
        DBG_PRINTF_SDH_DEBUG("sending MSG_END_PARSE for Pipe 0x%X\n",Pipe);
        memset( &msg, 0, sizeof(msg));
        msg.msgType = MSG_END_PARSE;  // this should close the pipe
        msg.param1 = 0; // TODO: replace this with the results of the parse
        msg.param3 = Pipe;          // The pipe that we are parsing.
        err = SYMsgSend(CMPARSER, &msg);      // send the response.
        XASSERT( err==OK, err );
        /* free the configuration memory */
        if( g_scanvars != NULL) {
       		DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars: SA_JR_MSG_RELEASE_SCAN\n" );
            scanvar_delete( &g_scanvars );
        }
        //
        // If state is idle we are done and we can return
        // 
        if(State == SCAN_APP_IDLE) {
            break;
        }
        
        /* davep 25-Jun-2009 ; if we didn't run a scan, we need to cancel
         * the job so ScanMan gets shut down. ScanApp will absorb and ignore
         * the extra cancel.
         */
        if( State == SCAN_APP_READY_TO_SCAN ) {
            scanapp_cancel_job();
        }
        else if (State != SCAN_APP_CANCELED) {
            /* Tell the source that we are done talking to him. */
            msg.msgType = MSG_SCAN_JOBEND;
            msg.param3 = job_resources;
            msg.param2 = SCANPARSEID;
            err = SYMsgSend(job_resources->Source, &msg); 
            XASSERT( err==OK, err );
        }
        
        /* davep 14-Jun-2011 ; the free should be after the jobend because
         * the free releases the job_resources
         */
        memset( &msg, 0, sizeof(msg) );
        msg.msgType = MSG_FREERECIPE;
        msg.param1 = 0;
        msg.param2 = 0;
        msg.param3 = job_resources;
        err = SYMsgSend( SJMID, &msg );
        XASSERT( err==OK, err );
        
        STATE_TRANSITION( SCAN_APP_IDLE );
        break;
        
    case SA_JR_MSG_START_SCAN_JOB:
        //
        // See if we are ready to go.
        //
        if(State != SCAN_APP_READY_TO_SCAN) {
            send_failure( Pipe, e_StartScanJob );
            break;      // get out of here.
        }
		else if( scanman_get_powersave_state() == ESM_ENERGY_SAVE ) {
			DBG_PRINTF_SDH_DEBUG( "%s scanner is powersave mode. wakeup\n", __FUNCTION__ );
			uint32_t req_id;
			scerr = scanman_dev_request(SCANMECH_IOCTL_WAKE_UP_MODE, 0, NULL, &req_id);
			scanman_set_powersave_state(ESM_WAKE_UP);
		}
        
        send_success( Pipe, e_StartScanJob );
        
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
//        cur_row_number[0] = 0;
//        cur_row_number[1] = 0;
//        cur_row_number[2] = 0;
        
        // We must send the current job settings immediately following
        // the e_StartJob packet.
        DBG_PRINTF_SDH_DEBUG("Sending e_StartJob to host\n");
        header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
        header->Cookie = ASP_COOKIE;
        header->Message = e_StartJob;
        header->Status = e_Success;
        header->DataLength = sizeof(SCAN_JOB_SETTINGS);
        
        swap_scan_header(header);
        pipe_write(header, sizeof(SCAN_HEADER));
        
        DBG_PRINTF_SDH_DEBUG("scanapp Sending JobSettings to host\n");
        // I/O will free this buffer, so we need to malloc & copy
        jsettings = (SCAN_JOB_SETTINGS *)ScanGetMemory(sizeof(SCAN_JOB_SETTINGS));
        *jsettings = JobSettings;
        swap_job_settings(jsettings);
        pipe_write(jsettings, sizeof(SCAN_JOB_SETTINGS));
        
        STATE_TRANSITION( SCAN_APP_SCANNING );  
		scanapp_log_event( SCANAPP_EVENT_SCAN_START );
        // All resources are ready. Start scanning.
        
        ASSERT( g_scanvars != NULL );
        scanapp_start_job();
        break;
        
    case SA_JR_MSG_CANCEL_SCAN_JOB:
        //
        // Make sure we are in the correct state.  This is only allowed
        // when the scanner is scanning.
        //
        if((State != SCAN_APP_SCANNING))
        {
            DBG_PRINTF_SDH_DEBUG("SAPP: Sending FAIL to host for CancelScanJob\n");
            send_failure( Pipe, e_CancelScanJob );
            break;      // get out of here.
            
        }
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        STATE_TRANSITION( SCAN_APP_HSTCANCEL );
        scanapp_log_event( SCANAPP_EVENT_HOST_CANCEL );
        
        /* davep 27-Sep-2011 ; ask SJM to cancel the job */
        scanapp_cancel_job();
        
		//Request pause the ADF
		if( g_scan_started_from_adf ) {
			DBG_PRINTF_SDH_DEBUG("[S_CANCEL] step1. pause!! PC ScanApp\n");
			scerr = scanmech_adf_client_request_pause();
		}
        break;
        
    case SA_JR_MSG_SET_JOB_SETTINGS:
        // Make sure we are in the correct state.  This is only allowed
        // when the scanner is scanning.
        //
        if((State != SCAN_APP_READY_TO_SCAN))
        {
            DBG_PRINTF_SDH_DEBUG("SAPP: Sending FAIL to host for SetScanJobSettings\n");
            if (State == SCAN_APP_CANCELED)
            {
                send_success( Pipe, e_SetScanJobSettings );  // we will fail at START SCAN
                break;
            }
            send_failure( Pipe, e_SetScanJobSettings );
            break;      // get out of here.
        }
        
        // in this case, the saj thread did not pass us the original message, but
        // the actual job settings message
        memcpy(&JobSettings,saj_message.param3,sizeof(SCAN_JOB_SETTINGS));
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        swap_job_settings(&JobSettings);
        DBG_PRINTF_SDH_DEBUG("Received new job settings from host:\n");
        
        DBG_PRINTF_SDH_DEBUG("  ScanType = %ld\n",JobSettings.ScanType);
        DBG_PRINTF_SDH_DEBUG("  DataType = %ld\n",JobSettings.DataType);
        
        //
        // Verify that the settings sent make sense.
        //
        if( VerifySettings(&JobSettings) != SCANERR_NONE )
        {
            DBG_PRINTF_SDH_DEBUG("SAPP: Sending FAIL to host for SetScanJobSettings Bad Values\n");
            header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
            header->Cookie = ASP_COOKIE;
            header->Message = e_SetScanJobSettings;
            header->DataLength = 0;
            header->Status = e_InvalidArg;
            
            swap_scan_header(header);
            pipe_write(header, sizeof(SCAN_HEADER));
            break;      // get out of here.
        }
        
		/* write SINDOH IQT Result into DB value in machine */		
		if( (JobSettings.reserved1 == IQT_AUTO_REGISTRATION_FROM_FB) ||
			(JobSettings.reserved1 == IQT_AUTO_REGISTRATION_FROM_ADF_FRONT) ||
			(JobSettings.reserved1 == IQT_AUTO_REGISTRATION_FROM_ADF_BACK) ) {
			scanner_service_write_IQTregistResult_intoDB( JobSettings.reserved1, JobSettings.reserved2, JobSettings.reserved3 );	
		}
			        
        /* davep 06-Dec-2006 ; adding sticky copy-to-host */
        if( scan_get_config() & SCAN_CONFIG_COPY_TO_HOST ) {
            JobSettings.ScanType = e_ScanThroughCopy;
            
            /* this weird-o feature will only hit once */
            scan_set_config_copy_to_host( false );
        }
        
        /* davep 13-Sep-2006 ; adding copy-to-host */
        if( JobSettings.ScanType == e_ScanThroughCopy ) {
            retcode = setup_scan_params_for_copy( &JobSettings );
            if( retcode != 0 ) {
                send_failure( Pipe, e_SetScanJobSettings );
                break;
            }
            
            /* davep 12-Oct-2006 ; adding support for cal-to-host */
            if( JobSettings.flags & SCAN_JOB_FLAGS_CAL_TO_HOST ) {
                ASSERT( g_scanvars!=NULL );
                scanvar_set_capture_cal( g_scanvars, true );
            }
            
            /* ignore the rest of the job settings message since we want to
             * use copy's settings
             */
            send_success( Pipe, e_SetScanJobSettings );
            break;
        }
        
        /* davep 30-Dec-2005 ; moving creation of scanvar from ack recipe
         * to exactly when the user specifies it
         */
        /* davep 14-Jun-2012 ; rolling a HUUUUUGE pile of code into
         * functions
         */
        scerr = allocate_global_scanvar( &JobSettings );
        if( scerr != SCANERR_NONE ) {
            send_failure( Pipe, e_SetScanJobSettings );
            break;
        }
        ASSERT( g_scanvars != NULL );
        scerr = setup_scan_params( g_scanvars, &JobSettings );
        if( scerr != SCANERR_NONE ) {
            /* we didn't like something about the host job request so
             * reject it 
             */
            if( g_scanvars ) {
		        DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars: SA_JR_MSG_SET_JOB_SETTINGS\n" );
                scanvar_delete( &g_scanvars );
                
            }
            send_failure( Pipe, e_SetScanJobSettings );
            break;
        }
        send_success( Pipe, e_SetScanJobSettings );
        break;
        
    case SA_JR_MSG_SET_DEFAULT_JOB_SETTINGS:
        //
        // Set the job settings to the default.
        //
        // Make sure we are in the correct state.  This is only not allowed
        // when the scanner is scanning.
        //
        if((State == SCAN_APP_SCANNING))
        {
            DBG_PRINTF_SDH_DEBUG("SAPP: Sending FAIL to host for SetDefaultScanJobSettings\n");
            send_failure( Pipe, e_SetDefaultScanJobSettings );
            break;      // get out of here.
        }
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        /* davep 18-Sep-2007 ; initialize the parts of the default job that are
         * platform specific
         */
        /* davep 26-Jul-2013 ; adding docsrc to scanplat get max scan length */
        DefJobSettings.ScannableArea.bottom = scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_ADF);
        
        memcpy(&JobSettings, &DefJobSettings, sizeof(SCAN_JOB_SETTINGS));
        //
        // We have gone back to default.  Send an ack.
        //
        
        send_success( Pipe, e_SetDefaultScanJobSettings );
        break;
        
    case SA_JR_MSG_GET_JOB_SETTINGS:
        DBG_PRINTF_SDH_DEBUG("Received GetScanJobSettings from host:\n");
        
        jsettings = (SCAN_JOB_SETTINGS *)ScanGetMemory(sizeof(SCAN_JOB_SETTINGS));
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
        header->Cookie = ASP_COOKIE;
        header->Message = e_GetScanJobSettings;
        header->Status = e_Success;
        header->DataLength = sizeof(SCAN_JOB_SETTINGS);
        
        swap_scan_header(header);
        pipe_write(header, sizeof(SCAN_HEADER));
        
        *jsettings = JobSettings;
        DBG_PRINTF_SDH_DEBUG("Sending current job settings:\n");
        DBG_PRINTF_SDH_DEBUG("  Gamma = %ld\n",JobSettings.Gamma);
        DBG_PRINTF_SDH_DEBUG("  Brightness = %ld\n",JobSettings.Brightness);
        DBG_PRINTF_SDH_DEBUG("  Contrast = %ld\n",JobSettings.Contrast);
        DBG_PRINTF_SDH_DEBUG("  Resolution = %ld\n",JobSettings.Resolution);
        DBG_PRINTF_SDH_DEBUG("  Sharp = %ld\n",JobSettings.Sharp);
        DBG_PRINTF_SDH_DEBUG("  BitsPerPixel = %ld\n",JobSettings.BitsPerPixel);
        DBG_PRINTF_SDH_DEBUG("  scan_window: top: %ld, left: %ld, bottom: %ld, right: %ld\n",
             jsettings->scan_window.top,
             jsettings->scan_window.left, 
             jsettings->scan_window.bottom, 
             jsettings->scan_window.right);
        DBG_PRINTF_SDH_DEBUG("  ScannableArea: top: %ld, left: %ld, bottom: %ld, right: %ld\n",
             jsettings->ScannableArea.top,
             jsettings->ScannableArea.left, 
             jsettings->ScannableArea.bottom, 
             jsettings->ScannableArea.right);
        DBG_PRINTF_SDH_DEBUG("  ScanType = %ld\n",JobSettings.ScanType);
        
        swap_job_settings(jsettings);
        pipe_write(jsettings, sizeof(SCAN_JOB_SETTINGS));
        break;
        
    case SA_JR_MSG_RESET_XMIT_BUFS:
        if(State != SCAN_APP_IDLE)
            break;              // if doing something, ignore this.
        Pipe->SendIOCmd(Pipe, e_ResetWrites, 0);
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        break;
        
    case SA_JR_MSG_RESET_TIMEOUT_COUNTER:
        DBG_PRINTF_SDH_DEBUG( "Received reset timeout counter\n" );
        *ScanTimeout = 0;
        break;
        
    case SA_JR_MSG_ADF_IS_PAPER_PRESENT:
        //
        // Report if there is paper present in the adf.
        //
        // this is a non-fail message, reset the timeout.
        *ScanTimeout = 0;
        header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
        memset(header, 0x00, sizeof(SCAN_HEADER));
        header->Cookie = ASP_COOKIE;
        header->Message = read_header.Message;        
        
        if( scan_capabilities.has_adf ) {
            // at power up, we need to give time between cal/startup, and scan to give ADF poll a chance
            if (posix_gettime_ticks( ) < (2*60*TIMER_INTS_PER_SECOND))
            {
//                posix_sleep_ticks( 2 * TIMER_INTS_PER_SECOND );
            }
#ifdef HAVE_SLEEPMODE
            // if we are asleep, or recently... give time to get valid ADF info.
            if (powerManagerTicksSinceWakeup() < (2 * TIMER_INTS_PER_SECOND))
            {
                powerManagerGeneralWakeup("AdfIsPaperPresent", POWERMANAGER_WAKEUP_GENERIC);
//                posix_sleep_ticks( 2 * TIMER_INTS_PER_SECOND );
            }
#endif
            /* we're an ADF so peek at the sensor and tell host what we find */
            header->Status = e_Success;
            {
				int32_t val=0;
				if( (pcscan_stats.adf_status == ADF_STATUS_COVER_OPEN) || (pcscan_stats.adf_status == ADF_STATUS_PAPER_JAM) ) {
					val = pcscan_stats.adf_status;
					DBG_PRINTF_SDH_DEBUG("ADF have some error(%d), val=%d\n", pcscan_stats.adf_status, val);
				}
				else {
					if( adf_paper_is_present() ) {
						val = 1;
					}
					else {
						val = 0;
					}
					DBG_PRINTF_SDH_DEBUG("ADF have No error(%d), val=%d\n", pcscan_stats.adf_status, val);
//                scands_get_paper_present(&val);
				}
                header->Param1 = val;
            }
            // header->Param1 = DSGetIntVar( e_PaperPresent );
        }
        else {
            /* flatbed therefore report FAIL since we don't have an ADF */
            header->Status = e_Fail;
        }
        swap_scan_header(header);
        DBG_PRINTF_SDH_DEBUG("ADFPaperIsPresent stat=%d, val=%d\n",header->Status,header->Param1);
        pipe_write(header, sizeof(SCAN_HEADER));
        DBG_PRINTF_SDH_DEBUG("pipe_write complete\n");
        break;
        
    case SA_JR_MSG_GET_PLATFORM_STRING:
        /* davep 07-Sep-2010 ; adding ability of host to query scan
         * capabilities (originally created so can tell the host our platform's
         * endianness)
         */
        send_platform_string( Pipe );
        break;
        
	case SA_JR_MSG_GET_ADF_INFORMATION:
        //
        // Report Machine status regarding ADF
        //
        *ScanTimeout = 0;
        header = (SCAN_HEADER *)ScanGetMemory(sizeof(SCAN_HEADER));
        memset(header, 0x00, sizeof(SCAN_HEADER));
        header->Cookie = ASP_COOKIE;
        header->Message = read_header.Message;
		header->Status = e_Success;

		int32_t val=0;
		if( IS_SCANNER_ATTACHED ) {
			DBG_PRINTF_SDH_DEBUG("MACHINE ADF INFO : HAVE ADF\n");
			/*
			 ************************************************
			 *  0 : Didn't have ADF
			 *  1 : Single ADF
			 *  2 : Duplex ADF (1pass - Dualscan)
			 *  3 : Duplex ADF (2pass)
			 ************************************************
			 */
			if(PINETREE_L) {
				DBG_PRINTF_SDH_DEBUG("MACHINE ADF INFO : HAVE Single-ADF\n");
				val = e_adf_info_Single_ADF;
			}
			else {
				DBG_PRINTF_SDH_DEBUG("MACHINE ADF INFO : HAVE Duplex(1pass)-ADF\n");
				val = e_adf_info_Duplex_ADF_1pass;	//PINETREE_S				
			}
		}
		else {
			DBG_PRINTF_SDH_DEBUG("MACHINE ADF INFO : NO-ADF\n");
			val = e_adf_info_NO_ADF;
		}
		header->Param1 = val;
		swap_scan_header(header);
		pipe_write(header, sizeof(SCAN_HEADER));
		break;
        
    }
    PTR_FREE(saj_message.param3);
}

/** 
 * \brief The thread that manages the scan-to-host feature.
 *
 *
 * \param input Not used.
 * \author Brad Smith
 * \date 26-Aug-2005
 *
 */
void* scanapp_task( void* unused )
{
    MESSAGE msg;
    UINT pxretcode;
    error_type_t err;
    uint32_t ScanTimeout;
    uint32_t IOTimeout;
    uint16_t currIOTimeout;
    struct scanvars *sv;
    int32_t pipe_retcode;
    int pxStatus;
    uint8_t page_side;
	BOOL WaitingForInput = FALSE;
    
    //wait for system init to finish
    SysWaitForInit();
    
    STATE_TRANSITION( SCAN_APP_IDLE );
    //scanapp_log_event( SCANAPP_EVENT_IDLE );
    
    DBG_PRINTF_SDH_DEBUG("%s start\n", __FUNCTION__ );
    
    /* start with clean slate */
    ScanTimeout = 0;
    IOTimeout = 0;
    
    job_resources = NULL;
    
    /* davep 04-Jan-2009 ; get the hardware capabilities of the platform (we
     * need to know if we have an ADF or not)
     */
    scancap_get_capabilities( &scan_capabilities );

	extern int db_get_var_int(char* tblName, char* attrName, error_type_t* db_retVal);
	error_type_t db_retval = FAIL;
	char* pBuffer = NULL;

	pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);
	if((db_retval != FAIL)&&(pBuffer)) {
		if(strncmp(pBuffer, "P247B01", 7) == 0 || strncmp(pBuffer, "M247B01", 7) == 0)
			tliCountry = IS_RUSSIA_OEM;
		else
			tliCountry = gCountry;
			
		MEM_FREE_AND_NULL(pBuffer);
	}

    // main loop
    while (1) 
    {
        
        // get a message
        //
        pxretcode = posix_wait_for_sys_message( scanapp_msgq, (char *)&msg, POSIX_WAIT_FOREVER); 
        //
        // Check for a timeout
        //
        if((ScanTimeout++/TIMER_INTS_PER_SECOND) > IOTimeout)
        {
		    DBG_PRINTF_SDH_DEBUG("ScanTimeout > IOTimeout : (ScanTimeout:%d)\n", ScanTimeout);
            //
            // We timed out.  Send the status to the statusmgr
            // This should result in us getting a cancel message.
            //
            ScanTimeout = 0;            // reset the timer
            if((State == SCAN_APP_TOCANCEL) ||
               (State == SCAN_APP_IDLE))
            {
			    DBG_PRINTF_SDH_DEBUG("ScanTimeout > CleanUpJob\n");
                CleanUpJob();
            } 
            else
            {
			    DBG_PRINTF_SDH_DEBUG("ScanTimeout > SCAN_APP_TOCANCEL\n");
                STATE_TRANSITION( SCAN_APP_TOCANCEL );
                scanapp_log_event( SCANAPP_EVENT_TIMEOUT );
            }
            
        }
        //
        // Check to see if we have transfered something.  If we have then clear the 
        // timeout.
        //
        /* davep 11-Apr-2008 ; XXX comment out and see if helps my slow
         * scans
         */
//            if(Pipe->SendIOCmd(Pipe,e_TransferActive,0))
//            {
//                ScanTimeout = 0;        // reset the timeout.
//            }
        
        //
        
//        DBG_PRINTF_SDH_DEBUG( "%s state=%d sysmsg=%d\n", __FUNCTION__, State, msg.msgType );
        
        switch ((uint32_t)msg.msgType)
        {
        case MSG_RESOURCES:
            DBG_PRINTF_SDH_DEBUG("scanapp received resources param1=%#lx param2=%#lx param3=%p\n", 
                 __FUNCTION__, msg.param1, msg.param2, msg.param3 );
            job_resources = (CURRENT_RESOURCE *) msg.param3;
            
            DBG_PRINTF_SDH_DEBUG( "%s source=%d dest=%d\n", __FUNCTION__, 
                  job_resources->Source, job_resources->Destination );
            
            /* davep 24-Jul-2008 ; want to make sure I get the MSG_SCAN_SIZE
             * so reset my sanity checks here at the start of the scan
             */
            scan_pix_per_row_padded = 0;
            scan_pix_per_row = 0;
            scan_bits_per_pixel = 0;
            break; 
            
        case MSG_START_PARSE:
            DBG_PRINTF_SDH_DEBUG("scanapp received START_PARSE\n");
            //
            if (scanapp_jr_started == true)
            {
				IOReg *Pipe_tmp;
			    SCAN_HEADER read_header;

				DBG_PRINTF_SDH_DEBUG("Scanner is busy and ban this connection!!!\n");

				Pipe_tmp = (IOReg *)msg.param3;
				Pipe_tmp->ReadConnect(Pipe_tmp, &read_header, sizeof(SCAN_HEADER), PARSEREAD_ANY);
				
                msg.msgType = MSG_END_PARSE;  // this should close the pipe
                err = SYMsgSend(CMPARSER, &msg);      // send the response.
                XASSERT( err==OK, err );
				break;
			}
			
            Pipe = (IOReg *) msg.param3;        // save the pipe info.
            
            // we have a connection
            //
            
            // Now see what we received.
            platvars_get_io_timeout(&currIOTimeout);
            
            /* davep 14-Jun-2011 ; XXX race condition between the
             * START_PARSE and the e_Lock message. The lock message has the
             * user requested timeout. But we get the START_PARSE before
             * the lock message.
             *
             * So hardwire to a happy timeout for now.
             */
            RequestedIOTO = 99;
            
            if((RequestedIOTO > currIOTimeout) &&  //greater than current                
               (RequestedIOTO < (20 * 60 ))) { // less than 20 minute timeout
                IOTimeout = RequestedIOTO;
            }
            else {
                IOTimeout = currIOTimeout;
            }
            DBG_PRINTF_SDH_DEBUG( "%s RequestedIOTO=%d currIOTimeout=%d IOTimeout=%d\n", 
                  __FUNCTION__, RequestedIOTO, currIOTimeout, IOTimeout );
            ScanTimeout = 0;        // reset the timeout value.
            
            // Now that we are staring a new scan job, create ScanAppJunior thread.  It's only job
            // is to get data from the host (i.e. network or USB), and parse the ASP
            XASSERT(scanapp_jr_started == false, scanapp_jr_started);
            pxStatus = posix_create_thread( &scanapp_jr_id, &scanapp_jr_task, Pipe, "Scan App Junior", scanapp_jr_stack,
                                            SCANAPP_JR_STACK_SIZE, POSIX_THR_PRI_NORMAL);
            scanapp_jr_started = true;
            
            XASSERT(pxStatus == 0, pxStatus);
            break;
            
        case MSG_ACKRECIPE :
            DBG_PRINTF_SDH_DEBUG( "scanapp received ackrecipe\n" );
            DBG_PRINTF_SDH_DEBUG("Send LockScanResource to host\n");
            
            // We're waiting for this state before sending e_Success to the host.
			if (State != SCAN_APP_TOCANCEL)
			{
            	STATE_TRANSITION( SCAN_APP_READY_TO_SCAN );
            }
            
            if(scanner_is_alive())
            {
                DBG_PRINTF_SDH_DEBUG("Send e_Success for LockScanResource\n");
                
                //
                // Get ready to parse.
                //
                ScanTimeout = 0;        // reset the timeout value.
                send_success( Pipe, e_LockScanResource );
                //scanapp_log_event( SCANAPP_EVENT_SCAN_START );
                //scanapp_log_event( SCANAPP_EVENT_SCAN_RUNNING );
            }
            else
            {
                scanapp_log_event( SCANAPP_EVENT_SCAN_OFFLINE );
                
                //Set response to be failure
                DBG_PRINTF_SDH_DEBUG("Send e_Fail to host for LockScanResource\n");
                
                //Not sure if we need following
                STATE_TRANSITION( SCAN_APP_IDLE );
                // tell scanapp_jr_task it's time to go away now - this can block
                kill_scanapp_jr_task();
                
                msg.param1 = 0; // TODO: replace this with the results of the parse
                msg.msgType = MSG_END_PARSE;  // this should close the pipe
                msg.param3 = Pipe;          // The pipe that we are parsing.
                err = SYMsgSend(CMPARSER, &msg);      // send the response.
                XASSERT( err==OK, err );

                /* free the configuration memory */
                if( g_scanvars != NULL) {
					DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars. scanner_not_alive\n" );
                    scanvar_delete( &g_scanvars );
                }

                msg.msgType = MSG_FREERECIPE;
                msg.param1 = 0;
                msg.param2 = 0;
                msg.param3 = job_resources;
                err = SYMsgSend( SJMID, &msg );
                XASSERT( err==OK, err );
                
                send_failure( Pipe, e_LockScanResource );
            }
            
            //
            // If we received a timeout from the host, set it up here
            // if not then use the default timeout.
            //
            platvars_get_io_timeout(&currIOTimeout);
            if((RequestedIOTO > currIOTimeout) &&  //greater than current                
               (RequestedIOTO < (20 * 60 ))) { // less than 20 minute timeout
                IOTimeout = RequestedIOTO;
            }
            else {
                IOTimeout = currIOTimeout;
            }
            if (IOTimeout > 60)
            {
                // quick timeout - possible we had 2nd job waiting & host gave up too early
                // we still would get the lock, but nothing else == half open connection.
                // 8 min standard timeout seems much too long.
                ScanTimeout = (IOTimeout - 30) * TIMER_INTS_PER_SECOND;  
            }
            break;
            
        case MSG_NAKRECIPE :
            // This message is received whenever we request resources and 
            // say do not wait for the resources.  In this case we have to
            // send back to the host a refusal and reset the state variable.
            DBG_PRINTF_SDH_DEBUG( "scanapp received nakrecipe\n" );
            
            STATE_TRANSITION( SCAN_APP_IDLE );
            
            // We couldn't get the resources for this job. Send a nak to the host.
            send_busy(Pipe, e_LockScanResource);
            
            // Stop parsing. We'll need a START_PARSE to get going again.
            // tell scanapp_jr_task it's time to go away now - this can block
            kill_scanapp_jr_task();
            
            memset( &msg, 0, sizeof(msg) );
            msg.msgType = MSG_END_PARSE;  // this should close the pipe
            msg.param1 = 0;
            msg.param3 = Pipe;          // The pipe that we are parsing.
            err = SYMsgSend(CMPARSER, &msg);      // send the response.
            XASSERT( err==OK, err );
            break;
            
        case MSG_SCAN_PAGESTART :
            /* davep 15-Apr-2013 ; adding page_side for dual scanning */
            msg_scan_page_start_decode( &msg, &page_side );
            DBG_PRINTF_SDH_DEBUG( "scanapp received pagestart page_side=%d, pageNum=%d\n", page_side, pcscan_stats.page_starts);
            scan_start_page( page_side );
#ifdef HAVE_IMAGELOG
			/* Backup scan data for Image log */
			scanapp_imagelog_malloc_page_memory( page_side );			
#endif
			pcscan_stats.page_starts++;

			if(JobSettings.ScanType == e_BilevelScan)
			{
				if (page_side == PAGE_SIDE_FRONT)
					scanData_gray2bw_strip_pre(scan_pix_per_row_padded, SCAN_DATA_MINIMUM_VAL_IS_BLACK);
				else
					scanData_gray2bw_strip_back_pre(scan_pix_per_row_padded, SCAN_DATA_MINIMUM_VAL_IS_BLACK);
			}
            break;
            
        case MSG_SCAN_PAGEEND :
            /* davep 15-Apr-2013 ; adding page_side for dual scanning */
            msg_scan_page_end_decode( &msg, &page_side );
            DBG_PRINTF_SDH_DEBUG( "scanapp received pageend page_side=%d\n", page_side );
            scan_end_page( page_side );
            
            /* Send Event to Observer regarding SCAN State */
			scanapp_log_event( SCANAPP_EVENT_SCAN_RUNNING );
			
#ifdef HAVE_IMAGELOG
			/* If machine use image log function, save into SD or fixed location for send to server */
			//check using of image log function set in SP(DB value)
			if(SCAN_IL_IS_enable() == true ) {
				DBG_PRINTF_SDH_DEBUG("Scan IMAGELOG TURN-ON\n");
				scanapp_imagelog_start( page_side );
			}
#endif
			if(JobSettings.ScanType == e_BilevelScan)
			{
            	if (page_side == PAGE_SIDE_FRONT)
            		scanData_gray2bw_strip_post();
            	else
            		scanData_gray2bw_strip_back_post();
            }
            break;
            
        case MSG_SCAN_PLANEDATA :
#ifdef HAVE_IMAGELOG
			/* Backup scan data for Image log */
			scanapp_imagelog_copy_scandata( &msg );
#endif
            scan_send_data( &msg );
            break;
            
        case MSG_SCAN_DATA_BLOB :
            /* send directly to host: param3==data param1==data's bytesize */
            send_data_blob( msg.param3, msg.param1 );
            break;
            
        case MSG_SCAN_JOBEND:
            DBG_PRINTF_SDH_DEBUG( "scanapp received jobend\n" );
			if( g_scan_started_from_adf == FALSE ) {
				if( (g_scan_started_from_A3fb == TRUE) || (g_scan_started_from_DLTfb == TRUE) ) {
					/*************************************************************************
					 * If Scanned FB With A3/DLT size (for A3/DLT(11'X17') Merge) 
					 *************************************************************************/
					/* Scan Job Started From Flatbed, Check have to scan next page */ 
					if( pcscan_stats.page_starts < 2 ) {
						if(g_scan_started_from_A3fb == TRUE) {
							scanapp_merge_adf_roller_move(SCAN_MERGE_IOCTL_PARAM_A3_TO_BACK); //A3 move to back
						}
						else if(g_scan_started_from_DLTfb == TRUE) {
							scanapp_merge_adf_roller_move(SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK); //DLT move to back			
						}
						scanapp_start_job();
					}
					else {
						ASSERT( msg.param3 != NULL );
						PTR_ASSIGN( sv, msg.param3 );
						XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
						XASSERT( sv==g_scanvars, (uint32_t)sv );
						
						scanapp_end_job();
						scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );

						if(g_scan_started_from_A3fb == TRUE) {
							scanapp_merge_adf_roller_move(SCAN_MERGE_IOCTL_PARAM_A3_EJECT); //move to front (DLT eject)
						}
						else if(g_scan_started_from_DLTfb == TRUE) {
							scanapp_merge_adf_roller_move(SCAN_MERGE_IOCTL_PARAM_DLT_EJECT); //move to front (DLT eject)						
						}
						g_scan_started_from_A3fb = FALSE;
						g_scan_started_from_DLTfb = FALSE;
					}
				}
				else if( g_scan_started_from_Legalfb == TRUE ) {
					/*****************************************************************
					 * If Scanned FB With Legal size (for Legal Merge) 
					 *****************************************************************/					
					if( pcscan_stats.page_starts < 2 ) {
						WaitingForInput = TRUE;
						//scanapp_log_event(SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN);
						scanapp_log_event(SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_PCSCAN);
					}
					else {
						ASSERT( msg.param3 != NULL );
						PTR_ASSIGN( sv, msg.param3 );
						XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
						XASSERT( sv==g_scanvars, (uint32_t)sv );
						
						scanapp_end_job();
						scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
						
						g_scan_started_from_Legalfb = FALSE;						
					}
				}
				else {
					//Normal FB Scan
					ASSERT( msg.param3 != NULL );
					PTR_ASSIGN( sv, msg.param3 );
					XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
					XASSERT( sv==g_scanvars, (uint32_t)sv );
					
					scanapp_end_job();
					scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
				}
			}
            else {
				//ADF Scan
				ASSERT( msg.param3 != NULL );
				PTR_ASSIGN( sv, msg.param3 );
				XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
				XASSERT( sv==g_scanvars, (uint32_t)sv );
				
				scanapp_end_job();
				scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );			
			}
            break;
            
        case MSG_SCAN_SIZE :
            /* davep 06-Sep-2005 ; receive data from scan telling me how
             * big the data will be once all DMA requirements met, scaling
             * applied, etc.
             */
            DBG_PRINTF_SDH_DEBUG( "scanapp received scan_size\n" );
            
            uint32_t total_rows;
            msg_scan_size_decode( &msg, &scan_pix_per_row_padded, &scan_pix_per_row, 
                                  &total_rows, &scan_bits_per_pixel );
                                  
			scan_bits_per_pixel_received = scan_bits_per_pixel;
            
            DBG_PRINTF_SDH_DEBUG( "%s scan size pprp=%d ppr=%d rows=%d bpp=%d\n", __FUNCTION__,
                  scan_pix_per_row_padded, scan_pix_per_row,
                  total_rows, scan_bits_per_pixel);
            
            /* davep 24-Jul-2008 ; stupid human checks */
            XASSERT( scan_pix_per_row_padded > 0 && 
                     scan_pix_per_row_padded < 65536,
                     scan_pix_per_row_padded );
            XASSERT( scan_pix_per_row > 0 && 
                     scan_pix_per_row < 65536,
                     scan_pix_per_row );

            /* davep 23-Jul-2013 ; track the scan data messages' page_side so
             * we can infer dual scanning
             */
            max_page_side = 0;
            break;
            
        case MSG_CANCELJOB :
            DBG_PRINTF_SDH_DEBUG( "scanapp received canceljob\n" );
            
            /* davep 28-Sep-2007 ; handle a race condition; host sends ASP
             * unlock while we're cancelling
             */
            if( State==SCAN_APP_IDLE ) {
                /* Ack the cancel or the SJM will get all pissy */
                DBG_PRINTF_SDH_DEBUG( "%s acking job cancel in idle state\n", __FUNCTION__ );
                
                /* davep 18-Oct-2007 ; WTF is param3? SJM seems to want it
                 * on a cancel
                 */
                send_cancel_ack_msg( msg.param3 );
                
                break;
            }
            
            //
            // Clear the write buffer of any pending stuff.
            //
            /* davep 22-Oct-2007 ; (Bug 7411) comment this out so we don't
             * break ASP messages
             */
//                pipe_retcode = Pipe->SendIOCmd(Pipe,e_ResetWrites,0);
//                DBG_PRINTF_SDH_DEBUG( "%s reset pipe_retcode=%ld\n", __FUNCTION__, pipe_retcode );
            
            // dont communicate with host on I/O errors
            // postpone ABORT when ReadyToScan, we will FAIL subseq cmds instead.
            if (( State != SCAN_APP_TOCANCEL ) && (State != SCAN_APP_READY_TO_SCAN))
            {
                //
                // Shut down the host.  (CP cancel, host cancels, ...)
                //
                DBG_PRINTF_SDH_DEBUG( "%s sending abort to host\n", __FUNCTION__ );
                send_success(Pipe,e_AbortScanJob);
                
                //
                // Now wait for the writes to finish.
                //
                //                pipe_retcode = Pipe->SendIOCmd(Pipe,e_WaitForWrites,(void *)100);  // wait 5 seconds for writes.
                /* davep 18-Oct-2007 ; (Bug 7411) bump up the timeout for slow
                 * hosts (still not foolproof)
                 */
                pipe_retcode = Pipe->SendIOCmd(Pipe,e_WaitForWrites,
                                               (void *)(25*TIMER_INTS_PER_SECOND));  // wait many seconds for writes.
                /* a return value of -1 indicates the wait timed out probably
                 * indicating the remote host isn't reading
                 */
                DBG_PRINTF_SDH_DEBUG( "%s waitforwrites pipe_retcode=%ld\n", 
                      __FUNCTION__, pipe_retcode );
                /* davep 22-Feb-2012 ; (bug discovered by HarrisH) remove
                 * the reset because the flush above isn't implemented. we
                 * could kill our own abort
                 * message 
                 */
                // Flush the remaining writes.
                //Pipe->SendIOCmd(Pipe,e_ResetWrites,0);
            }
            
            //
            // Send an ack to the system jobmgr for the copy
            //
            /* davep 18-Oct-2007 ; WTF is param3? SJM seems to want it
             * on a cancel
             */
            send_cancel_ack_msg( msg.param3 );
            
            /* free the configuration memory */
            if( g_scanvars != NULL) {
				DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars. MSG_CANCELJOB\n" );
                scanvar_delete( &g_scanvars );
            }
            //
            // If we have a timeout, clean up everything
            // and exit.
            //
            if (State == SCAN_APP_TOCANCEL)
            {
                CleanUpJob();
                break;          // done here.
            }
            STATE_TRANSITION( SCAN_APP_CANCELED );
            scanapp_log_event( SCANAPP_EVENT_DEVICE_CANCEL );
            break;
            
        case MSG_SAJ_MESSAGE:
            DBG_PRINTF_SDH_DEBUG( "scanapp received scan app junior message\n" );
            parse_saj_messages(msg,&ScanTimeout);
            break;

		/*
		 * *****************************************************************************************
		 * MSG_USER_INPUT
		 * *****************************************************************************************
		 */
		case MSG_USER_INPUT:
		
#if SDH_USE_PC_SCAN_CANCEL_BY_UI		
			if( msg.param1 == PARAM_USER_CANCEL_JOB ) {
				
				DBG_PRINTF_SDH_DEBUG( "==> PARAM_USER_CANCEL_JOB\n" );
				
				if( State == SCAN_APP_SCANNING ) {
					scanapp_cancel_job();
				}				
			}
#endif
			/* Check Wait User Input */
			if (!WaitingForInput) {
				DBG_PRINTF_SDH_DEBUG("WaitingForInput : is FALSE\n");
				ScanTimeout = 0;
				break; // we should not be here, get out
			}
			WaitingForInput = FALSE;
			//
			// Change status to scanning.
			//
			if (msg.param1 == PARAM_USER_OK) {
				//For Legal Merge Job, change the scan area for next scan
				if( (g_scan_started_from_adf == FALSE) && (g_scan_started_from_Legalfb == TRUE) ) {
					DBG_PRINTF_SDH_DEBUG( "==> PARAM_USER_OK: MEDIASIZE_LEGAL\n" );
					scanapp_setup_merge_scanarea( PC_SCAN_MERGEJOB_LEGAL_2ND_PAGE, &JobSettings );
					ASSERT( g_scanvars != NULL );
					setup_scan_area(g_scanvars, &JobSettings);

					scanapp_start_job();
				}
			}
			break;
			
        default:
            /* ignore */
            DBG_PRINTF_SDH_DEBUG("scanapp ignore unknown message 0x%x, param1 = 0x%lx\n", 
                 msg.msgType, msg.param1);
            break;
        } /* end switch(msg.msgType) */
        
    } /* end while(1) */
    return 0;
}

/**
 * \brief Convert a scan data type to its pixel byte size 
 *
 * We send the bytes per pixel down to the host through ASP.  Create a fast
 * mapping from the scan data type to the byte size.
 *
 * The negative numbers are a bit of a kludge.  The ASP protocol specifies
 * BYTES per pixel but the HalfPack data types are smaller that one byte. I
 * couldn't change the prototol so I'm using negative numbers to indicate BITS
 * per pixel.
 *
 * \author David Poole
 * \date 12-Nov-2008
 *
 */

static void init_byte_per_pixel_map( void )
{
    memset( bytes_per_pixel_map, 0, sizeof(bytes_per_pixel_map) );
    
    bytes_per_pixel_map[ SCAN_DATA_TYPE_PLANE ] = 1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_RED ]   = 1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_GREEN ] = 1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_BLUE ]  = 1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_MONO ]  = 1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_XRGB ]  = 4;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_RGBX ]  = 4;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_RGB ]   = 3;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_HP1BPP ] = -1;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_HP2BPP ] = -2;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_HP4BPP ] = -4;
    bytes_per_pixel_map[ SCAN_DATA_TYPE_BRG_INTERLACED ] = 1;
}


/**
 * \brief Initialize the Scan Application.
 *
 * Called from main() before threading runs to do pre-OS setup of the Scan
 * Application.
 *
 * \author Brad Smith
 * \date 25-Aug-2005
 *
 */

void ScanAppInit( void )
{
    int pxStatus;
    PARSE_SETUP Parser;

    DBG_PRINTF_SDH_DEBUG("ScanAppInit\n");

    scanapp_jr_started = false;

    // Register as a parser
    memset( &Parser, 0, sizeof(PARSE_SETUP) );
    Parser.ParserID = SCANPARSEID;
    Parser.ParseCmpString = (char *)ScanCmpString;
    Parser.StringSize = strlen(ScanCmpString);
    Parser.ParseString = NULL;
    Parser.ParseType = e_Scan;
    
    ParserRegister(&Parser);        // register the parser.
    
    /* davep 21-Dec-2005 ; adding SIQ parsing; use almost all the same
     * parameters as the previous parse registration
     */
#ifdef HAVE_SCANHIQ_SUPPORT    
    /* we want the data to be send via our Win32 usbsend utility so we have to
     * tell the parser to expect the data on the print endpoint
     */
    Parser.ParseCmpString = (char *)SIQCmpString;
    Parser.StringSize = strlen((char *)SIQCmpString);
    Parser.ParseType = e_Print;
    ParserRegister(&Parser);  
#endif
    
    scanapp_observer_constructor();
    
    //
    // Create a mail box and register it.
    //
    
    /* create the queue */ 
    posix_create_message_queue( &scanapp_msgq, "/scanapp", SCANAPP_QUEUE_SIZE, 
                                sizeof(MESSAGE) );    
    router_register_queue(SCANPARSEID, scanapp_msgq);  // Register a message queue.
    //XASSERT(pxStatus == 0, pxStatus);
    
    //
    // Register the resource
    rm_register(e_IOScanParser, SCANPARSEID);
    
    /* davep 20-Mar-2007 ; initialize the parts of the default job that are
     * platform specific
     */
    /* davep 26-Jul-2013 ; adding docsrc to scanplat get max scan length  */
    DefJobSettings.ScannableArea.bottom = scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_FLATBED);
    
    // Initialize the scan job settings.
    memcpy(&JobSettings, &DefJobSettings, sizeof(SCAN_JOB_SETTINGS));
    
    init_byte_per_pixel_map();
    
    // start the scan task
    pxStatus = posix_create_thread( &scanapp_id, &scanapp_task, 0, "Scan_App", scanapp_stack,
                                    SCANAPP_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT(pxStatus == 0, pxStatus);

}

