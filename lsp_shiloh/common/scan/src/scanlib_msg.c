/*
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: functions shared between different versions of scanlib
 *
 * Originally created to remove some copy/paste code between ThreadX and linux
 * userspace code.
 *
 * davep 15-Jan-2013
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanman.h"
#include "scanlib.h"
#include "scantask.h"
#include "scandbg.h"
#include "scanmech.h"

/* Define to turn on more verbose debugging */
//#define SCANLIB_DEBUG  

#ifdef SCANLIB_DEBUG
  #define scanlib_dbg2 dbg2 
#else
  #define scanlib_dbg2(...) 
#endif

#ifdef __KERNEL__
extern bool bScanEC;
#endif

void scanlib_send_scan_size( uint32_t pixels_per_row_padded, 
                             uint32_t pixels_per_row,
                             uint32_t total_rows, 
                             uint32_t bits_per_pixel )
{
    scan_msg_t send_msg;
    scan_err_t err;
    uint32_t n;

    /* stupid human check */
    XASSERT(   bits_per_pixel==1 
            || bits_per_pixel==2
            || bits_per_pixel==4
            || bits_per_pixel==8
            || bits_per_pixel==16
            || bits_per_pixel==24
            || bits_per_pixel==32, bits_per_pixel );

    XASSERT( pixels_per_row_padded >= pixels_per_row, pixels_per_row );

    memset( &send_msg, 0, sizeof(send_msg) );

    send_msg.msgType = SMSG_SCAN_SIZE;
    send_msg.param1 = pixels_per_row_padded;
    send_msg.param2 = pixels_per_row;
    
    n = 0;
    n |= SMSG_SIZE_SET_NUM_ROWS( total_rows );
    n |= SMSG_SIZE_SET_PIXEL_SIZE( bits_per_pixel );
    send_msg.param3 = (void *)n;

    err = scanlib_call_msg_callback( &send_msg );
    XASSERT( err==0, err );
}

void scanlib_send_page_data( uint8_t **buf, scan_data_type dtype, 
                             uint32_t num_rows, uint32_t max_rows,
                             int page_side, bool last_buffer )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

//    dbg2( "%s dtype=%d nr=%d mr=%d side=%d last=%d\n", __FUNCTION__, 
//            dtype, num_rows, max_rows, page_side, last_buffer );

    /* davep 04-Apr-2013 ; only two sides of paper so far */
    XASSERT(page_side==0||page_side==1, page_side );

    XASSERT( num_rows<=max_rows, max_rows );

    memset( &send_msg, 0, sizeof(send_msg) );

    /* send data to scan manager for further processing */
    send_msg.msgType = SMSG_PAGE_DATA;

    send_msg.param1 = SMSG_DATA_SET_NUM_ROWS( num_rows );
    send_msg.param1 |= SMSG_DATA_SET_MAX_ROWS( max_rows );

    send_msg.param2 = SMSG_DATA_SET_DATATYPE( dtype );

    /* davep 04-Apr-2013 ; adding page side */
    send_msg.param2 |= SMSG_DATA_SET_PAGE_SIDE( page_side );

    if( last_buffer ) {
        send_msg.param2 |= SMSG_DATA_LAST_BUFFER_FLAG;
    }

    /* take the pointer away from the caller */
    PTR_ASSIGN( send_msg.param3, *buf );

    scerr = scanlib_call_msg_callback( &send_msg );
    XASSERT( scerr==0, scerr );
}

void scanlib_send_page_start( uint8_t page_side )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

    scanlib_dbg2( "%s\n", __FUNCTION__ );

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_PAGE_START;
    send_msg.param1 |= SMSG_PAGE_START_SET_PAGE_SIDE(page_side);

    scerr = scanlib_call_msg_callback( &send_msg );
    XASSERT( scerr==0, scerr );
}


void scanlib_send_page_end( uint8_t page_side )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

    scanlib_dbg2( "%s\n", __FUNCTION__ );

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_PAGE_END;
    send_msg.param1 |= SMSG_PAGE_END_SET_PAGE_SIDE(page_side);

    scerr = scanlib_call_msg_callback( &send_msg );
    XASSERT( scerr==0, scerr );
}

// parameter value 1:jam, 0:jam clear
void scanlib_send_adf_jam(uint8_t jam )
{
    scan_msg_t send_msg;
    scan_err_t scerr;

    //scanlib_dbg2( "%s\n", __FUNCTION__ );
    dbg1( "[S_L_M] %s(%d)\n", __FUNCTION__, jam );

    memset( &send_msg, 0, sizeof(send_msg) );
    send_msg.msgType = SMSG_SCAN_ADF_PAPER_EVENT;
    send_msg.param1 = SCAN_ADF_PAPER_JAM;
	send_msg.param2 = jam;
	
    scerr = scanlib_call_msg_callback( &send_msg );
    XASSERT( scerr==0, scerr );
}


void scanlib_send_sc(uint8_t sc )
{
#ifdef __KERNEL__
    scan_msg_t send_msg;
    scan_err_t scerr;

    //scanlib_dbg2( "%s\n", __FUNCTION__ );
    dbg1( "[S_L_M] %s(%d)\n", __FUNCTION__, sc );

	if(bScanEC == false) // add.lsh to avoid overwriting SC error
	{
		scanmech_SC_halt();  //To stop the motor and clutch in Soc SC Error   mod.kks.  2014. 06.02
		
		memset( &send_msg, 0, sizeof(send_msg) );
		send_msg.msgType = sc;
		
		scerr = scantask_msg_callback( &send_msg );
		XASSERT( scerr==0, scerr );

		bScanEC = true;
	}
#endif	
}
