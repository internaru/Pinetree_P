/** 
 * ============================================================================
 * Copyright (c) 2007 Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 * Pretend to be a scanner sending data to scanman.
 *
 * Created to test system without actually scanning. For example, can run copies
 * without capturing data. Useful for testing/debugging.
 *
 * David Poole 17-Aug-2007
 *
 **/
 
#include <stdint.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "list.h"
#include "memAPI.h"
#include "cpu_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantask.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "piehw.h"
#include "pie.h"
#include "scanvars.h"
#include "fakescan.h"
#include "ostools.h"
#include "scanlib.h"

/* define FAKESCAN_DEBUG to add more verbose data debugging */
#define FAKESCAN_DEBUG 1

/* if we fail to get memory to send to scanman, sleep this many seconds before
 * we try again
 */
#define OUT_OF_MEMORY_WAIT_SECONDS  5

/* set parts of the allocated buffers to this value so we can see the strips on
 * the output
 */
#define STRIP_BOUNDARY_PIXEL 0
//#define STRIP_BOUNDARY_PIXEL 0x66

static scan_err_t
alloc_strip( uint32_t *strip_counter, uint8_t **p_data, int bits_per_pixel, 
             int bytes_per_row, int num_rows, bool last_buffer_flag )
{
    int datalen;
    int i;
    uint8_t *data, *ptr;
    uint8_t *endptr;

    *strip_counter += 1;

    datalen = bytes_per_row * num_rows;

#ifdef FAKESCAN_DEBUG
    dbg2( "%s bpr=%d num_rows=%d datalen=%d last=%d\n", __FUNCTION__, bytes_per_row,
                num_rows, bytes_per_row*num_rows, last_buffer_flag );
#else
    if( last_buffer_flag ) {
        dbg2( "%s bpr=%d num_rows=%d datalen=%d last=%d\n", __FUNCTION__, bytes_per_row,
                    num_rows, bytes_per_row*num_rows, last_buffer_flag );
    }
#endif

    /* davep 20-Jul-2011 ; stupid human checks (trying to catch weird overflow) */
    XASSERT( num_rows<=PIE_WDMA_MAX_ROWS, num_rows );
    XASSERT( bytes_per_row < 65536, bytes_per_row );

    data = ice_dma_alloc_buffer( datalen );

    if( data == NULL ) {
        dbg2( "%s failed to get datalen=%d\n", __FUNCTION__, datalen );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* davep 17-Apr-2008 ; clear the cache bypass bit, do the memset()'s, then
     * flush and set the cache bypass bit (I might be starving the bus)
     */
    data = CACHE_BYPASS_CLEAR( data );

    /* we're moving ptr along inside the buffer so use a temp */
    ptr = data;

    /* set it to pixel white */
    memset( ptr, 0xff, datalen );

    /* set top row to black(ish) so can see strip boundaries */
    memset( ptr, STRIP_BOUNDARY_PIXEL, bytes_per_row );

    endptr = ptr + datalen;

    /* set the edges to black(ish) to see the left/right edges */
    for( i=0 ; i<num_rows ; i++ ) {
        /* make sure I'm not screwing something up somewhere */
        ptr = data + i*bytes_per_row;
        XASSERT( ptr < endptr, (uint32_t)ptr );

        if( bits_per_pixel==24 ) {
            /* stamp the left/right edges with black pixels */
            *(ptr+0) = 0;
            *(ptr+1) = 0;
            *(ptr+2) = 0;
            *(ptr+bytes_per_row-3) = 0;
            *(ptr+bytes_per_row-2) = 0;
            *(ptr+bytes_per_row-1) = 0;
        }
        else {
            *ptr = STRIP_BOUNDARY_PIXEL;                
            ptr += bytes_per_row-1;
            *ptr = STRIP_BOUNDARY_PIXEL;                
            ptr++;
        }
    }

    /* davep 25-Jan-2012 ; for laughs, stamp buffer interior to rgbcmy */
    if( bits_per_pixel==24 && *strip_counter<=6 ) {
        static const uint8_t pixels[][3] = {
            { 0xff, 0, 0 }, /* red */
            { 0, 0xff, 0 }, /* green */
            { 0, 0, 0xff }, /* blue */
            { 0, 0xff, 0xff }, /* cyan */
            { 0xff, 0, 0xff }, /* magenta */
            { 0xff, 0xff, 0 }, /* yellow */
        };

        for( i=1 ; i<num_rows ; i++ ) {
            /* +/-3 to skip the border pixels */
            ptr = data + i*bytes_per_row + 3;
            endptr = ptr + bytes_per_row-3-3;

            while( ptr < endptr ) {
                *ptr++ = pixels[ *strip_counter-1][0];
                *ptr++ = pixels[ *strip_counter-1][1];
                *ptr++ = pixels[ *strip_counter-1][2];
            }
        }
    }

    if( last_buffer_flag ) {
//        dbg2( "%s %p %p\n", __FUNCTION__, ptr, &ptr[(num_rows-1) * bytes_per_row] );

        /* set the entire buffer to black(ish) so I can see it (we're clipping
         * the bottom of the page)
         */
//        memset( buffer, STRIP_BOUNDARY_PIXEL, datalen );

        /* set the last row to black(ish) so we can see the bottom of the page */
        ptr = data;
        ptr += (num_rows-1) * bytes_per_row;
        memset( ptr, STRIP_BOUNDARY_PIXEL, bytes_per_row );
    }

    /* flush the buffer in the CPU data cache */
    cpu_dcache_writeback_region( data, datalen );

    data = CACHE_BYPASS_SET( data );

    PTR_ASSIGN( *p_data, data );

    return 0;
}

//static void
//send_plane_data( uint8_t **p_data, uint32_t num_rows, 
//                 scan_data_type dtype, int bits_per_pixel, 
//                 bool last_buffer_flag ) 
//{
//    scan_msg_t send_msg;
//    scan_err_t scerr;
//
//#ifdef FAKESCAN_DEBUG
//    dbg2( "%s data=%p num_rows=%ld last_buffer_flag=%d\n", 
//                __FUNCTION__, *p_data, num_rows, last_buffer_flag );
//#endif
//
//    memset( &send_msg, 0, sizeof(send_msg) );
//    send_msg.msgType = SMSG_PAGE_DATA;
//
//    send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(num_rows);
//
//    send_msg.param2 = SMSG_DATA_SET_DATATYPE( dtype );
//    send_msg.param2 |= SMSG_DATA_SET_PIXEL_SIZE( bits_per_pixel );
//
//    /* set last strip flag */
//    if( last_buffer_flag==ICE_DMA_LAST_BUFFER ) {
//        send_msg.param2 |= SMSG_DATA_SET_LAST_BUFFER_FLAG(-1);
//    }
//
//    /* take the pointer away from the caller */
//    send_msg.param3 = CACHE_BYPASS_CLEAR( *p_data );
//    *p_data = NULL;
//
//    scerr = scanlib_call_msg_callback( &send_msg );
//    XASSERT( scerr==0, scerr );
//}


static void
rescale_page_size( struct scanvars *sv, uint32_t *pixels_per_row, uint32_t *expected_rows )
{
    int x_num, x_denom, y_num, y_denom;

    scanvar_get_pie_scale( sv, &x_num, &x_denom, &y_num, &y_denom );

    dbg2( "%s x=%d/%d y=%d/%d\n", __FUNCTION__, x_num, x_denom, y_num, y_denom );

    /* this isn't really how the PIE XY scaler works but who cares? */
    *pixels_per_row = (*pixels_per_row * x_num) / x_denom;
    *expected_rows = (*expected_rows * y_num) / y_denom;
}

static void 
get_page_area( struct scanvars *sv, uint32_t *pixels_per_row, uint32_t *expected_rows )
{
    uint32_t dpi;
    long int x, y, width, height;

    /* Get the area; units are 1/100ths of an inch.
     *
     * Note these are SIGNED integers; x,y can be negative (I think).
     */
    scanvar_get_area( sv, &x, &y, &width, &height );

    dbg2( "%s x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__, 
                x, y, width, height );

    /* convert to our DPI appropriate pixels */
    dpi = sv->dpi;

    /* plus 50 to do a simple round up */
    *pixels_per_row = ( width * dpi + 50) / 100;
    *expected_rows = ( height * dpi + 50) / 100;

    dbg2( "%s %ld %ld\n", __FUNCTION__, *pixels_per_row, *expected_rows );
}

static void
fakescan_setup( scan_data_type *fake_dtype,
                uint32_t *fake_pixels_per_row, 
                uint32_t *fake_expected_rows,
                uint32_t *fake_bytes_per_row,
                int *fake_bits_per_pixel )
{
    scan_data_type dtype;
    uint32_t pixels_per_row, expected_rows, bytes_per_row;
    int hp_bpp, bits_per_pixel; 
    struct pie_dma_row piehp_row;
    struct scanvars *sv;

    sv = scanvar_get();

    /* set up how much of a fake page we need to scan */
    get_page_area( sv, &pixels_per_row, &expected_rows );

    dbg2( "%s ppr=%d er=%d\n", __FUNCTION__, pixels_per_row, expected_rows );

    /* adjust for the scale factor */
    rescale_page_size( sv, &pixels_per_row, &expected_rows );

    dbg2( "%s ppr=%d er=%d\n", __FUNCTION__, pixels_per_row, expected_rows );

    /* cheap hack to max out at one A4 page */
    if( expected_rows > 3507 ) {
        dbg2( "%s truncated rows from %ld to %d\n", __FUNCTION__, 
                    expected_rows, 3507 );
        expected_rows = 3507;
    }

    dbg2( "%s ppr=%ld er=%ld\n", __FUNCTION__, pixels_per_row, expected_rows );

    /* if we're copying, adjust bytes to pixels */
    if( sv->use_hp ) {
        hp_bpp = scanvar_get()->hp_bpp;
        piehp_calc_row( pixels_per_row, hp_bpp, false, &piehp_row );

        dbg2( "PIEHP row: pin=%d pout=%d bin=%d bout=%d\n", piehp_row.pixels_in,
                    piehp_row.pixels_out, piehp_row.bytes_in, piehp_row.bytes_out);

        pixels_per_row = piehp_row.pixels_out;
        bytes_per_row = piehp_row.bytes_out;
        bits_per_pixel = hp_bpp;
    }
    else if( sv->use_pie ) {
        /* use PIE's output size bumped to appropriate DMA width */
        bits_per_pixel = pie_get_bits_per_pixel( sv->pieout );
        pixels_per_row = ICE_DMA_ALIGN_ME( pixels_per_row );
        bytes_per_row = pixels_per_row * (bits_per_pixel/8);
    }
    else {
        /* use PIC's bpp and bump up to appropriate DMA width */
        XASSERT( sv->pic_bpp==8||sv->pic_bpp==16, sv->pic_bpp );
        bits_per_pixel = sv->pic_bpp;
        pixels_per_row = ICE_DMA_ALIGN_ME( pixels_per_row );
        bytes_per_row = pixels_per_row * (bits_per_pixel/8);
    }

    dtype = scanvar_get_final_data_type( sv );
    XASSERT( dtype!=SCAN_DATA_NULL_TYPE, dtype );

    *fake_pixels_per_row = pixels_per_row;
    *fake_expected_rows = expected_rows;
    *fake_bytes_per_row = bytes_per_row;
    *fake_bits_per_pixel = bits_per_pixel;
    *fake_dtype = dtype;
}

scan_err_t fakescan_run( void )
{
    scan_err_t scerr;
    uint32_t pixels_per_row, expected_rows, max_rows_per_buffer; 
    uint32_t num_rows, bytes_per_row;
    int pending_rows, total_memory, retcode;
    uint8_t *data;
    bool last_buffer_flag, scan_cancel_flag;
    struct scos_mq *msgq;
    scan_msg_t msg;
    int bits_per_pixel;
    scan_data_type dtype;
    uint32_t strip_counter;

    dbg2( "%s start\n", __FUNCTION__ );

    max_rows_per_buffer = PIE_WDMA_MAX_ROWS;
    /* end of tunable parameters */

    fakescan_setup( &dtype, &pixels_per_row, &expected_rows, 
                    &bytes_per_row, &bits_per_pixel );

#ifdef FAKESCAN_DEBUG
    dbg2( "%s ppr=%d er=%d bpr=%d bits_per_pixel=%d dtype=%d\n", __FUNCTION__, pixels_per_row,
                expected_rows, bytes_per_row, bits_per_pixel, dtype );
#endif

    /* tell our downstream how much we'll be sending */
    scanlib_send_scan_size( pixels_per_row, pixels_per_row, expected_rows, bits_per_pixel );

    scanlib_send_page_start(0);

    pending_rows = expected_rows;
    last_buffer_flag = false;
    scan_cancel_flag = false;
    total_memory = 0;
    data = NULL;
    msgq = scantask_get_msgq();
    strip_counter = 0;

    /* 
     * Now send the "page" of data as strips of data. We peek into the scantask
     * message queue looking for cancel messages.
     */
    while( pending_rows > 0 && !scan_cancel_flag ) {

//        dbg2( "%s pending_rows=%d total_memory=%d\n", __FUNCTION__,
//                    pending_rows, total_memory );

        /* peak into msgq looking for a cancel */
        scerr = scos_mq_receive( msgq, &msg, 0 );

        if( scerr==SCANERR_NONE ) {

            switch( msg.msgType ) {
                case SMSG_SCAN_CANCEL :
                    scan_cancel_flag = true;
                    break;

                /* other messages at some point? */

                default :
                    /* at this stage of the game, we're only expecting something from
                     * the scan subsystem
                     */
                    XASSERT( 0, msg.msgType );
            }

            if( scan_cancel_flag ) {
                /* if we've been told to cancel, we'll immediately hop out of our data sending
                 * loop 
                 */
                dbg2( "%s cancelling\n", __FUNCTION__ );
                break;
            }
        }

        /* how much do we send this time? */
        if( pending_rows > max_rows_per_buffer ) {
            num_rows = max_rows_per_buffer; 
        }
        else {
            num_rows = pending_rows; 
            last_buffer_flag = true;
        }

        /* get a strip buffer */
        retcode = alloc_strip( &strip_counter, &data, bits_per_pixel, 
                                    bytes_per_row, num_rows, last_buffer_flag );
        if( retcode != 0 ) {
            dbg2( "%s sleep %d waiting for memory\n", __FUNCTION__,
                        OUT_OF_MEMORY_WAIT_SECONDS );
            posix_sleep_seconds( OUT_OF_MEMORY_WAIT_SECONDS );

            /* we'll go back to the top of the loop so we'll check our message
             * queue again (don't want to block completely; want to be able to
             * quickly cancel)
             */
        }
        else {

            /* whee! we got a buffer! */
            pending_rows -= num_rows;
            XASSERT( pending_rows>=0, pending_rows );

            total_memory += bytes_per_row * num_rows;

            /* pretend to be closer to a real scanner by putting ~2ms between
             * strips 
             */
//            scos_thread_sleep( SECONDS / 30 );

            /* send it off */
            scanlib_send_page_data( &data, dtype, num_rows, num_rows, 0, last_buffer_flag );
        }

        /* go back and do another strip; isn't this fun!? */
    }

    scanlib_send_page_end(0);

    if( scan_cancel_flag ) {
        return SCANERR_SCAN_CANCELLED;
    }

    return 0;
}

