/****************************************************************************** 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Application Lib
 * Author: Sandra Capri
 *         David Poole
 *
 * Copy/Paste/Rewrite of core scanlib.c for Linux userspace app. Is responsible
 * for communicating with linux kernel kscantask module through /dev node.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "scos.h"

#include "ATypes.h"
#include "list.h" 
#include "lassert.h" 
#include "memAPI.h"

/* need the siqfile_print_fn capabilities in scanvars.h */
#define USE_SIQFILE_PRINT_FN 1

#include "scantypes.h"
#include "scancore.h" 
#include "scanvars.h" 
#include "scantask.h"
#include "scandbg.h" 
#include "scan.h" 
#include "scanlib.h"
#include "scanman.h"
#include "safetylock.h"
#include "mbuffer.h"
#include "scanhwerr.h"
#include "smsg_decode.h"
#include "scanmem.h"
#include "scanmech.h"
#include "ostools.h"
#include "logger.h"

/* Define to turn on more verbose debugging */
//#define SCANLIB_DEBUG  

#ifdef SCANLIB_DEBUG
  #define scanlib_dbg2 dbg2
#else
  #define scanlib_dbg2(...) 
#endif

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "[S_L_U] "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(3)
///Using Method.
// for Error Log Level Display : DBG_PRINTF_SDH_ERR(...)
// for Debug Log Level Display : DBG_PRINTF_SDH_DEBUG(...)

struct scanlib_scan_info {
    int pie_required_rows;
    int scan_required_rows;
    int bytes_per_row;
};

static struct scanlib_scan_info scanlib_self;

static struct safety_lock scanlib_global_lock;

static scan_callback_fn_t scan_callback_fn;

#define MODULE_NAME "scanman" 
#define DEV_NAME "/dev/"MODULE_NAME
#define MAX_STRIP_WAIT_TIME		200		// 200*10ms = 2s

static struct scanvars *sv;

static FILE *devfile;  // handle for scan device file
static struct mbuffer siqfile_buf; 

/* davep 01-Nov-2012 ; quit scanning when this is true (can be synchronously
 * set but eventually hook to a signal)
 */
static struct scos_sem scan_complete_lock;
static bool scanlib_failed_flag=false;

#if 1	// add.lsh for copying stop debugging
	static uint8_t scanning_0 = 0;
	static uint8_t scanning_1 = 0;
	static uint16_t buf_cnt_0 = 0;
	static uint16_t buf_cnt_1 = 0;
	
	static uint16_t sec_cnt = 0;
	static uint8_t trace_marker = 0;
#endif

static ScannerHWError scannerErrorCode=ScannerHWError_ScannerStarting; 

/*
 *  Small thread to read events from kernel space and feed them into
 *  scantask/scanman in user space
 */
#define SCANLIB_KMSGS_TASK_PRI     POSIX_THR_PRI_NORMAL
#define SCANLIB_KMSGS_STACK_SIZE   (POSIX_MIN_STACK_SIZE  * 4)// has to be big enough for pthreads

// align the stack
ALIGN(8)
static UINT8 scanlib_kmsgs_stack[SCANLIB_KMSGS_STACK_SIZE];
static pthread_t scanlib_kmsgs_task_id;



ScannerHWError scan_get_hwerr( void )
{
    /* note no thread/interrupt protection! */
    return scannerErrorCode;
}

void scan_set_hwerr( ScannerHWError scanner_hw_err )
{
    /* note no thread/interrupt protection! */
    scannerErrorCode = scanner_hw_err;
}

bool scanner_is_alive( void )
{
    return scannerErrorCode==ScannerHWError_None;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 02-Sep-2009
 *
 */

static void scanlib_set_callback( scan_callback_fn_t new_callback )
{
    DBG_PRINTF_SDH_DEBUG( "%s old=%p new=%p\n", __func__, scan_callback_fn, new_callback );

    scan_callback_fn = new_callback;
}

static scan_callback_fn_t scanlib_get_callback( void )
{
    return scan_callback_fn;
}

scan_err_t scanlib_call_msg_callback( scan_msg_t *msg )
{
//    ASSERT( scan_callback_fn!=NULL );
    if ( scan_callback_fn != NULL )
    {
        return scan_callback_fn( msg );
    }
    return scanman_msg_send( msg );
}

static void scanlib_lock( void )
{
    scan_err_t scerr;

    /* davep 22-Oct-2008 ;  if this assertion fails, someone is trying to call
     * scanlib2_setup() while a scanlib2_setup/run/done call are already
     * active.  
     *
     * scanlib2 is not recursive.  We cannot do a scan while we're doing a
     * scan.
     */
    scerr = safety_lock_try_lock( &scanlib_global_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scanlib_unlock( void )
{
    safety_lock_unlock( &scanlib_global_lock );
}

bool scanlib_is_locked( void )
{
    scan_err_t scerr;

    /* Try to grab the lock. If we don't get the lock, assume scanlib has it.
     * If we get the lock, scanlib isn't running so release the lock
     * immediately.
     *
     * This is a poor excuse for a busy flag. Used as a crude mechnanism to
     * tell if there is a running scan.
     */
    scerr = safety_lock_try_lock( &scanlib_global_lock );
    if ( scerr != SCANERR_NONE ) {
        return true;
    }
    safety_lock_unlock( &scanlib_global_lock );
    return false;
}

static void scanlib_scan_end( struct scanvars **psv )
{
//    const struct scanvars *tmp_sv;
    struct scanvars *sv;

    PTR_ASSIGN( sv, *psv );

    /* make sure our stack is clean */
//    tmp_sv = scanvar_pop();
//    XASSERT( tmp_sv==sv, tmp_sv->id );
//    tmp_sv = NULL;

    /* Scanlib no longer has control of the scanvar. If we're using ScanMan,
     * the scanvar has been sent back in a SMSG_PAGE_END. Then sv will be NULL
     * after the call to scantask_scan_end().
     *
     * But if we're not using ScanMan (such as with internal scans or test/debug
     * scans), sv will not be NULL after the call to scantask_scan_end(). We
     * have to return control of the scanvar back to the caller.
     *
     * For example, cmdline.c handles the "scan var new"/"scan start" and is
     * also responsible for allocating/freeing his own scanvar.
     */
    scantask_scan_end( &sv );
    if ( sv != NULL ) {
        /* we'll pass the scanvar back to the caller */
        PTR_ASSIGN( *psv, sv );
    }
}

static void scanfw_siqfile_print( const char *fmt, va_list ap )
{
    scan_err_t scerr;
    int retcode;
    char *ptr;
    int avail;

    avail = siqfile_buf.max - siqfile_buf.len;
    ptr = siqfile_buf.buf + siqfile_buf.len;

    while( 1 )  {
//        dbg2( "%s buf=%p ptr=%p avail=%d\n", __func__, siqfile_buf.buf, ptr, avail );

        retcode = vsnprintf( ptr, avail, (char *)fmt, ap );
        if ( retcode<0 ) {
            /* error */
            DBG_PRINTF_SDH_DEBUG( "%s error retcode=%d\n", __func__, retcode );
            return;
        }

//        dbg2( "%s avail=%d retcode=%d\n", __func__, avail, retcode );

        if ( retcode < avail ) {
            /* success! */
            siqfile_buf.len += retcode;
            break;
        }

//        dbg1( "%s overflow; realloc and try again\n", __func__ );

        scerr = mbuffer_realloc( &siqfile_buf );
        if ( scerr != SCANERR_NONE ) {
            /* ignore this string */
            DBG_PRINTF_SDH_DEBUG( "%s cannot print this string\n", __func__ );
            return;
        }

        /* realloc() might have changed the buffer so reinitialize our local
         * ptr/len 
         */
        avail = siqfile_buf.max - siqfile_buf.len;
        ptr = siqfile_buf.buf + siqfile_buf.len;
    }
}

void scanlib_quit(void)
{
    scan_err_t scerr;

    scerr = scos_sem_post( &scan_complete_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

void scanlib_failed(void)
{
    scanlib_failed_flag = true;
}

static scan_err_t buffer_read( FILE *devfile, uint8_t* buf, uint32_t buflen_bytes )
{
    int retcode;
    uint8_t *ptr;
    uint32_t bytes_remain;

    ASSERT( buflen_bytes );

//    dbg2( "%s buf=%p buflen_bytes=%d\n", __func__, buf, buflen_bytes );

    ptr = buf;
    bytes_remain = buflen_bytes;

    while( bytes_remain > 0 ) {
        retcode = fread( ptr, 1, bytes_remain, devfile );
        if( retcode <= 0 ) {
            DBG_PRINTF_SDH_ERR( "%s read failed errno=%d\n", __func__, errno );
            return SCANERR_MSG_FAIL; 
        }
//        dbg2( "%s read bytes=%d returned bytes=%d\n", 
//                    __func__, bytes_remain, retcode );

        ptr += retcode;
        bytes_remain -= retcode;
    }

    return SCANERR_NONE;
}

static scan_err_t handle_data_message(FILE *devfile, scan_msg_t *msg)
{
    scan_err_t scerr;
    uint16_t num_rows;
    scan_data_type dtype;
    uint8_t *ptr, *buf=NULL;
    bool last_buffer;
    uint32_t num_bytes;
    uint8_t page_side;
    uint8_t nLoop=0;

	trace_marker = 0x01;	// add.lsh for copying stop debugging
	
    /* decode the message because we need to know the buffer size */
    scan_msg_page_data_decode(msg, &num_rows, &dtype, &ptr, &last_buffer);
    
    /* davep 04-Apr-2013 ; adding page side support for dual scan */
    scan_msg_page_data_get_page_side( msg, &page_side );

    num_bytes = num_rows * scanlib_self.bytes_per_row;

#if 1	// add.lsh for copying stop debugging
	if(page_side == 0)
	{
		if(scanning_0 == 0){	// last buffer
	    	scanning_0 = 1;
	    	buf_cnt_0++;
	    	dbg1( "[C_S] Start Buff(f)!! (num_rows=%d dtype=%d ptr=%p  last=%d)\n", 
	            num_rows, dtype, ptr, last_buffer );
		}
	    else if(last_buffer){	// first buffer
	    	dbg1( "[C_S] Last Buff(f)!! (num_rows=%d dtype=%d ptr=%p  last=%d, cnt=%d)\n", 
	            num_rows, dtype, ptr, last_buffer, buf_cnt_0 );
			scanning_0 = 0;
			buf_cnt_0 = 0;
		}
		else{
			buf_cnt_0++;
		}
	}
	else
	{
		if(scanning_1 == 0){	// last buffer
	    	scanning_1 = 1;
	    	buf_cnt_1++;
	    	dbg1( "[C_S] Start Buff(b)!! (num_rows=%d dtype=%d ptr=%p  last=%d)\n", 
	            num_rows, dtype, ptr, last_buffer );
		}
	    else if(last_buffer){	// first buffer
	    	dbg1( "[C_S] Last Buff(b)!! (num_rows=%d dtype=%d ptr=%p  last=%d, cnt=%d)\n", 
	            num_rows, dtype, ptr, last_buffer, buf_cnt_1 );
			scanning_1 = 0;
			buf_cnt_1 = 0;
		}
		else{
			buf_cnt_1++;
		}
	}
#endif

#if 1
	buf = scanmem_alloc(num_bytes);	//Marvell Modified 20131010 SDK <- there is some problem in SINDOH ui scan.
	while((buf == NULL) && (nLoop<MAX_STRIP_WAIT_TIME))
    {
		nLoop++;
		usleep(10*1000);
		buf = scanmem_alloc(num_bytes);	//Marvell Modified 20131010 SDK <- there is some problem in SINDOH ui scan.
	}
#else
	buf = malloc(num_bytes);			//SINDOH ui scan debugging.
#endif
    if (buf==NULL)
    {
        dbg1( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, num_bytes, scanmem_get_free_size(), scanmem_get_heap_size() );
        return SCANERR_OUT_OF_MEMORY;
    }

	trace_marker = 0x02;	// add.lsh for copying stop debugging

    // read the scan data from the scanner
    scerr = buffer_read(devfile, buf, num_bytes);
    if (scerr != SCANERR_NONE) {
        /* buffer_read() logs the error */
        dbg1( "[C_S] Error : buffer_read (%d)\n", scerr);	// add.lsh for copying stop debugging
        PTR_FREE(buf);
        return scerr;
    }
    // send the data upstream
    /* davep 16-May-2013 ; note I'm sending num_rows as max_rows because we
     * only allocate exactly as much as necessary 
     */
     
	trace_marker = 0x03;	// add.lsh for copying stop debugging
	
    scanlib_send_page_data(&buf, dtype, num_rows, num_rows, page_side, last_buffer);

    // we can't free the buffer we malloc'd, so the upstream receiver of 
    // the message will have to do it
	
	trace_marker = 0x04;	// add.lsh for copying stop debugging
	
    return SCANERR_NONE;
}

static scan_err_t handle_data_blob_message( FILE *devfile, scan_msg_t *msg )
{
    scan_err_t scerr;
    uint32_t num_bytes;
    uint8_t *ptr;
    uint8_t *buf;

    /* decode the message because we need to know the buffer size */
    scan_msg_data_blob_decode( msg, &ptr, &num_bytes );
    
    //DBG_PRINTF_SDH_DEBUG( "%s num_bytes=%d ptr=%p last=%d\n", __func__, num_bytes, ptr );

#if 1
    buf = scanmem_alloc( num_bytes );	//Marvell Modified 20131010 SDK <- there is some problem in SINDOH ui scan.
#else
	buf = malloc( num_bytes );			//SINDOH ui scan debugging.
#endif
    if( buf==NULL ) {
        DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d\n", __func__, num_bytes );
        return SCANERR_OUT_OF_MEMORY;
    }

    scerr = buffer_read( devfile, buf, num_bytes );
    if( scerr != SCANERR_NONE ) {
        /* buffer_read() logs the error */
        PTR_FREE( buf );
        return scerr;
    }

    /* attach the new buffer to the message */
    PTR_ASSIGN( msg->param3, buf );

    /* re-send the message downstream */
    scerr = scanlib_call_msg_callback( msg );
    XASSERT( scerr==0, scerr );

    return SCANERR_NONE;
}

static scan_err_t scanlib_parse_message(FILE *devfile, scan_msg_t *msg)
{
    uint32_t pixels_per_row_padded, pixels_per_row, total_rows, bits_per_pixel, scerr;
    uint8_t page_side;

    //DBG_PRINTF_SDH_DEBUG( "%s msgtype=%d p1=%d p2=0x%X p3=%p\n", __func__,msg->msgType, msg->param1, msg->param2, msg->param3 );

    switch(msg->msgType)
    {
    case SMSG_SCAN_SIZE:
        DBG_PRINTF_SDH_DEBUG("received scan_size from %s\n",DEV_NAME);
        scan_msg_size_decode(msg, &pixels_per_row_padded, &pixels_per_row, 
                             &total_rows, &bits_per_pixel);
        DBG_PRINTF_SDH_DEBUG( "%s ppr=%d pprp=%d tr=%d bpp=%d\n", __func__,
              pixels_per_row, pixels_per_row_padded,
              total_rows, bits_per_pixel);
        scanlib_send_scan_size(pixels_per_row_padded, pixels_per_row,
                               total_rows, bits_per_pixel);
        scanlib_self.bytes_per_row = pixels_per_row_padded * (bits_per_pixel/8);
        break;
        
    case SMSG_SCAN_START:
        DBG_PRINTF_SDH_DEBUG("received scan_start from %s\n",DEV_NAME);
        break;
        
    case SMSG_PAGE_START:
        scan_msg_page_start_decode(msg,&page_side);
        DBG_PRINTF_SDH_DEBUG("received page_start page_side=%d from %s\n",page_side,DEV_NAME);
        scanlib_send_page_start(page_side);
        break;
        
    case SMSG_PAGE_DATA:
//        dbg1("received page_data from %s\n",DEV_NAME);
        scerr = handle_data_message(devfile, msg);
        
        if (scerr != SCANERR_NONE)
        {
            DBG_PRINTF_SDH_DEBUG("send SMSG_SC_CMD_MEM_MALLOC_FAIL\n");
//            msg->msgType = SMSG_SC_CMD_MEM_MALLOC_FAIL;
//            scerr = scanlib_call_msg_callback( msg );
//            XASSERT( scerr==0, scerr );
        }
        break;

    case SMSG_PAGE_END :
        scan_msg_page_end_decode(msg,&page_side);
        DBG_PRINTF_SDH_DEBUG("received page_end page_side=%d from %s\n",page_side,DEV_NAME);
        scanlib_send_page_end(page_side);
        break;

    case SMSG_SCAN_END :
        DBG_PRINTF_SDH_DEBUG("received scan_end from %s\n",DEV_NAME);
        break;

    case SMSG_SCAN_READY :
        /* scan_ready is the "ack" from scantask telling us he's done */
        DBG_PRINTF_SDH_DEBUG("received scan_ready from %s\n",DEV_NAME);

        /* davep 11-Jan-2011 ; quit when we get this message */
        scanlib_quit();
        if (!scanlib_failed_flag)
        {
            scanlib_scan_end(&sv);
        }
#if 0	//del.lsh.for USER CANCEL (looks like Marvell BUG..lock(one time) -> unlock(two times) per one job
        scanlib_unlock();
#endif
        scanlib_failed_flag = false;
        scan_ready_scantask();
        break;

    case SMSG_DATA_BLOB : 
        /* data blobs are usually from cal-to-host */
        DBG_PRINTF_SDH_DEBUG("received data_blob from %s\n",DEV_NAME);
        handle_data_blob_message( devfile, msg );
        break;

    case SMSG_CAL_CALIBRATION_IN_PROGRESS :
        DBG_PRINTF_SDH_DEBUG("received SMSG_CAL_CALIBRATION_IN_PROGRESS from %s\n",DEV_NAME);
		scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;

    case SMSG_CAL_CALIBRATION_DONE :
        DBG_PRINTF_SDH_DEBUG("received SMSG_CAL_CALIBRATION_DONE from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;
        
    case SMSG_SCAN_ADF_PAPER_EVENT :
		DBG_PRINTF_SDH_DEBUG("received adf_paper_event from %s\n",DEV_NAME);
//	    scanlib_send_adf_jam(msg->param2);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;
        
    case SMSG_SCAN_FAILED : 
        DBG_PRINTF_SDH_DEBUG("received scan_failed from %s\n",DEV_NAME);

        scanlib_failed();

        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;
        
//add kmg
    case SMSG_SC_AFE_OFFSET_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_AFE_OFFSET_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;
        
    case SMSG_SC_AFE_GAIN_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_AFE_GAIN_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;
        
    case SMSG_SC_AFE_SETTING_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_AFE_SETTING_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break; 
        
    case SMSG_SC_CIS_HOME_POSITION_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_AFE_SETTING_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break; 
        
    case SMSG_SC_WHITE_STRIP_READ_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_WHITE_STRIP_READ_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;       
        
    case SMSG_SC_BLACK_STRIP_READ_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_BLACK_STRIP_READ_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;    
        
    case SMSG_SC_BAD_PIXEL_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_BAD_PIXEL_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;                      
        
    case SMSG_SC_CIS_HOME_OFF_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_CIS_HOME_OFF_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break; 
        
    case SMSG_SC_CIS_HOME_ON_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_CIS_HOME_ON_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;         
               
    case SMSG_SC_PIC_OVF_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_PIC_OVF_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;     
        
    case SMSG_SC_BAD_PIXEL_FAIL_B : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_BAD_PIXEL_FAIL_B from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;                                             

    case SMSG_SC_CMD_QUEUE_OVR_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_CMD_QUEUE_OVR_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;  
        
    case SMSG_SC_CMD_COMPLETE_OVR_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_CMD_COMPLETE_OVR_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;  
                      
    case SMSG_SC_SCAN_ASSERT_FAIL : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_SCAN_ASSERT_FAIL from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;  
        
    case SMSG_SC_PIPE_WAIT_TIMEOUT : 
        DBG_PRINTF_SDH_DEBUG("received SMSG_SC_PIPE_WAIT_TIMEOUT from %s\n",DEV_NAME);
        scerr = scanlib_call_msg_callback( msg );
        XASSERT( scerr==0, scerr );
        break;  
                               
    default :
        /* epic fail! */
        DBG_PRINTF_SDH_ERR("%s unknown message 0x%x param1=0x%x param2=0x%x param3=%p\n",
             __func__, msg->msgType, msg->param1, msg->param2, msg->param3 );
        //XASSERT( 0, msg->msgType );	//blocked by SINDOH yhlee
        break;
    }
    return SCANERR_NONE;
}

scan_err_t scanlib2_setup_and_run(struct scanvars **psv)
{
    siqfile_print_fn old_p;
    char *ptr;
    int bytes_written, bytes_remain;
    scan_err_t scerr;

    DBG_PRINTF_SDH_DEBUG("%s\n", __func__);

    scanlib_lock();

    PTR_ASSIGN( sv, *psv );

    /* davep 02-Sep-2009 ; (Bug 13964) moving scan callback into
     * scanvar/scanlib. Assert is to make sure we cleaned up properly when we
     * left previously. If we're not running, no callback should be set.
     */
//-jrs removed this check on advice of davep 20121130
//-jrs    XASSERT(scanlib_get_callback()==NULL, (uint32_t)scanlib_get_callback());
//-jrs    if (sv->scan_callback_fn != NULL)
//-jrs    {
        scanlib_set_callback( sv->scan_callback_fn );
//-jrs    }

    /* initialize our globals to defaults ("constructor") */
    memset( &scanlib_self, 0, sizeof(scanlib_self));
    mbuffer_new(&siqfile_buf, 1024*4);  // Dave - size?
    
    // convert the scanvar to a siqfile
    old_p = set_siqfile_print_fn(scanfw_siqfile_print);
    scanvar_to_siqfile(sv);
    set_siqfile_print_fn(old_p);
    
//    /* davep 26-Mar-2013 ; XXX temp debug */
//    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//    scanvar_to_siqfile(sv);

    // and write the siqfile to the device file
    ptr = siqfile_buf.buf;
    bytes_written = 0;
    bytes_remain = siqfile_buf.len;
    while(bytes_remain)
    {
        bytes_written = fwrite(ptr, 1, bytes_remain, devfile);
        
        DBG_PRINTF_SDH_DEBUG("%s fwrite() siqfile bytes_written=%d\n", __func__, bytes_written);
        if (bytes_written < 0)
        {
            perror("fwrite() to dev\n");
            break;
        }

        bytes_remain -= bytes_written;
        ptr += bytes_written;
    }
    bytes_written = fwrite("#END-OF-FILE-FOR-USB", 1, 20, devfile);
    fflush(devfile);
    
/* Dump Scanvar Configuration to SIQ file */
#if 1
{
	FILE *fs;
	
	/* read the entire file into a buffer */
    fs = fopen( "/data/lsh.siq", "rb" );
    if( fs ) {
    	fclose(fs);
	    fs = fopen( "/data/lsh.siq", "wb" );
		fwrite(siqfile_buf.buf, 1, siqfile_buf.len, fs);
		if (fs)
			fclose(fs);
		fs = NULL;
    }
}
#endif
    mbuffer_free(&siqfile_buf);
    
    // now that the siqfile has been given to the scanner, wait for
    // messages to be processed until the scan has completed
    scerr = scos_sem_wait( &scan_complete_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
/*
	//Blocked by SINDOH yhlee
    if (!scanlib_failed_flag)
    {
        scanlib_scan_end(&sv);
    }
*/
    scanlib_unlock();
    scanlib_failed_flag = false;
    return EXIT_SUCCESS;
}
    
static void* scanlib_kmsgs_task(void* unused)
{
    scan_msg_t msg;
    int bytes_read;

    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        bytes_read = fread(&msg, 1, sizeof(msg), devfile);

        if ( bytes_read==0 ) {
            sleep(1);
#if 1	// add.lsh for copying stop debugging
            if(scanning_0 || scanning_1){
            	sec_cnt++;
            	dbg2( "[C_S] WARN : No more msg in scanning!!(time(%d sec), trace : %d, buf_cnt:f-%d,b-%d ) \n", 
            		sec_cnt, trace_marker, buf_cnt_0, buf_cnt_1);
            }
            else{
            	sec_cnt = 0;
            }
#endif
            continue;
        }

        scanlib_dbg2( "bytes_read=%d\n", bytes_read );
    #ifdef SCANLIB_DEBUG
        scanlog_hex_dump((unsigned char *)&msg,sizeof(msg));
    #endif

        scanlib_parse_message(devfile, &msg);
    }

    return NULL;
}
   

/**
 * \brief  create a global lock to prevent more than one active
 * scanlib2_setup/run/done.   And any other initialization needed
 *
 * We can't scan within a scan. Sorry. :-P
 *
 * \author David Poole
 * \date 22-Oct-2008
 *
 */
scan_err_t handle_send_ioctl( uint32_t request, int arg, void *ptr )
{
	ioctl(fileno(devfile), request, arg, ptr);
	scanlib_dbg2("RYU : send ioctl\n");
	return 0;
}

scan_err_t scanlib_onetime_init( void )
{
//    unsigned int pxretcode;
    scan_err_t scerr;

    safety_lock_init( &scanlib_global_lock );

    devfile = fopen(DEV_NAME, "r+");
    if (devfile==NULL)
    {
        /* ZOMG! */
        printf("ERROR WHILE OPENING SCAN DEVICE FILE %s: SCAN IS NON-FUNCTIONAL: is the Scan Kernel Loadable Module loaded?\n",DEV_NAME); 
        perror("fopen devfile");
        return EXIT_FAILURE;
    }

    scerr = scos_sem_init( &scan_complete_lock, "scan_complete", 0 );
    if (scerr != SCANERR_NONE)
    {
        return scerr;
    }
    
//    pxretcode = posix_create_thread( &scanlib_kmsgs_task_id, &scanlib_kmsgs_task, 0, "scanlib_kmsgs_task",
//                                    scanlib_kmsgs_stack, SCANLIB_KMSGS_STACK_SIZE, SCANLIB_KMSGS_TASK_PRI );
//    XASSERT(pxretcode==0, pxretcode);

 	scanlib_set_callback( scanman_msg_send );	// add.lsh. for msg to scanman in idle mode (jam clear)
 	
    return 0;

    /* call this function so compiler won't complain */
    scanlib_get_callback();
}

void scanlib_kmsgs_task_init( void ) // move.lsh. from scanlib_onetime_init()
{
	unsigned int pxretcode;
	pxretcode = posix_create_thread( &scanlib_kmsgs_task_id, &scanlib_kmsgs_task, 0, "scanlib_kmsgs_task",
                                    scanlib_kmsgs_stack, SCANLIB_KMSGS_STACK_SIZE, SCANLIB_KMSGS_TASK_PRI );
	DBG_PRINTF_SDH_DEBUG("scanlib_kmsgs_task start!!\n");
    XASSERT(pxretcode==0, pxretcode);
}


