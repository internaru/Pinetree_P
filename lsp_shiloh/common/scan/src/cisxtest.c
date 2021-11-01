/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file cisxtest.c
 *
 * \brief Tests for CISX Scan block
 *
 * davep Mar 2011
 *
 **/

#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

//#include "logger.h"
#include "lassert.h"
#include "list.h"
#include "memAPI.h"
#include "cpu_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "cisx.h"
#include "icetest.h"
#include "icetesttest.h"
#include "pic.h"
#include "pictest.h"
#include "piccbi.h"
#include "pichw.h"
#include "scantask.h"
#include "ostools.h"
#include "picdma_descrip.h"

static uint8_t icetest_channels[ ICETEST_IDMA_NUM_CHANNELS ];
static uint8_t pic_channels[ PIC_WDMA_NUM_CHANNELS ];
static int num_icetest_channels;
static int num_pic_channels;

/* davep 01-Jun-2011 ; this sensor spec only used in testing */
static const struct cisx_sensor_conf cisx_test_sensor = {
    num_segments: 3,
    segments: {
    /* hardwire to some simple numbers based on dma aligned pixels per row */
//        { 0, 880 },
//        { 0, 880 },
//        { 0, 880 },
        { 84, 1728 },
        { 84, 1728 },
        { 84, 1728 },
//        { 20, 860 },
//        { 20, 860 },
//        { 20, 860 },
    }
};

/* Created the 'bufsensor' stuff to load a buffer with pixels that look like
 * they came from a 3-segment sensor. The bufsensor buffer would be fed into
 * CISX, hopefully creating a nice normal row of output pixels.
 */
struct bufsensor_segment {
    int num_dummy;
    int num_data;

    uint16_t *ptr;
    int dummy_counter;
    int data_counter;

    uint16_t dummy_pixel;
};

struct bufsensor {
    int num_segments;
    struct bufsensor_segment segments[CISX_SENSOR_MAX_SEGMENTS];
};

static void bufsensor_print( struct bufsensor *bufsensor )
{
    int i;

    for( i=0 ; i<3 ; i++ ) {
        dbg2( "i=%d dummy=%d data=%d %d %d\n", 
                i, 
                bufsensor->segments[i].num_dummy, 
                bufsensor->segments[i].num_data,
                bufsensor->segments[i].dummy_counter, 
                bufsensor->segments[i].data_counter );
    }
}

static void bufsensor_init( const struct cisx_sensor_conf *cisxsensor, struct bufsensor *bufsensor )
{
    int i;
    static const uint16_t dummy_pixel[] = {
        0xaaaa, 0xbbbb, 0xcccc };

    /* have only tested with 3 segment sensors */
    XASSERT( cisxsensor->num_segments==3, cisxsensor->num_segments );

    memset( bufsensor, 0, sizeof(struct bufsensor) );

    bufsensor->num_segments = cisxsensor->num_segments;

    for( i=0 ; i<cisxsensor->num_segments ; i++ ) {
        bufsensor->segments[i].num_dummy = cisxsensor->segments[i].num_dummy;
        bufsensor->segments[i].num_data = cisxsensor->segments[i].num_data;

        bufsensor->segments[i].ptr = 0;
        bufsensor->segments[i].dummy_counter = 0;
        bufsensor->segments[i].data_counter = 0;
        bufsensor->segments[i].dummy_pixel = dummy_pixel[i];
    }
    bufsensor_print( bufsensor );
}

static void bufsensor_load_pointers( struct bufsensor *bufsensor, uint16_t *ptr_init[3] )
{
    int i;

    /* have only tested with 3 segment sensors */
    XASSERT( bufsensor->num_segments==3, bufsensor->num_segments );

    for( i=0 ; i<bufsensor->num_segments ; i++ ) {
        bufsensor->segments[i].ptr = ptr_init[i];

        /* these values count down to zero */
        bufsensor->segments[i].dummy_counter = bufsensor->segments[i].num_dummy;
        bufsensor->segments[i].data_counter = bufsensor->segments[i].num_data;
    }
}

static void bufsensor_run( struct bufsensor *bufsensor, int pixels_per_row, uint16_t *image, uint16_t *output_buffer )
{
    int i;
    uint16_t *ptr, *endptr;

    dbg2( "%s ppr=%d\n", __FUNCTION__, pixels_per_row );

    ptr = output_buffer;
    endptr = output_buffer + pixels_per_row;

    dbg2( "%s ptr=%p endptr=%p\n", __FUNCTION__, ptr, endptr);

    /* round robin walk through the list of sensor elements, taking dummy
     * pixels then data pixels, putting them into the output buffer.
     */
    while( ptr < endptr ) {
        for( i=0 ; i<3 ; i++ ) {
            if( bufsensor->segments[i].dummy_counter ) {
                bufsensor->segments[i].dummy_counter--;
                *ptr++ = bufsensor->segments[i].dummy_pixel;
//                *ptr++ = ~0;
            }
            else if( bufsensor->segments[i].data_counter ) {
                bufsensor->segments[i].data_counter--;
                *ptr++ = *bufsensor->segments[i].ptr++;
            }
        } /* end for() */

        if( ptr > endptr ) {
            /* pointer overrun! dump some debug */
            scanlog_hex_dump( (unsigned char *)image, 64 );
            scanlog_hex_dump( (unsigned char *)output_buffer, pixels_per_row*sizeof(uint16_t) );

            dbg2( "%s ptr=%p endptr=%p\n", __FUNCTION__, ptr, endptr );

            bufsensor_print(bufsensor);
        }
        XASSERT( ptr<=endptr, (uint32_t)ptr );

    } /* end while() */
}

#if 0
static void stamp_row_counter( uint16_t row_counter, uint16_t *pixels, int num_pixels )
{
    int i;
    uint16_t *ptr16;
//    uint16_t *endptr16;
    struct cisx_sensor sensor;

    cisx_get_sensor( &sensor );
    ptr16 = pixels;
    for( i=0 ; i<sensor.num_segments ; i++ ) {
        ptr16 += sensor.segments[i].num_dummy;
    }
    *ptr16 = row_counter;

//    /* davep 31-May-2011 ; stamp a row number onto a pixel buffer so can track
//     * the output
//     *
//     * Simply searches for the first pixel that matches previous row_counter,
//     * replaces with a new row_counter. I'm counting on the dummy pixels being
//     * 0xffff.
//     */
//    ptr16 = pixels;
//    endptr16 = pixels + num_pixels;
//
//    if( row_counter_ptr ) {
//        *row_counter_ptr = row_counter++;
//    }
//    else {
//        while( ptr16 < endptr16 ) {
//            if( *ptr16==0 ) {
////                dbg2( "%s %d %p %d %#x %#x\n", __FUNCTION__, __LINE__, pixels, num_pixels, *ptr16, row_counter );
////                scanlog_hex_dump( (unsigned char *)ptr16, 32 );
//
//                *ptr16 = row_counter;
//                row_counter_ptr = ptr16;
//                row_counter++;
//                break;
//            }
//            ptr16++;
//        }
//        /* make sure we found it */
//        XASSERT( ptr16 < endptr16, (uint32_t)ptr16 );
//    }
}
#endif

static scan_err_t make_sensor_data( int pixels_per_row_in, int pixels_per_row_out, uint16_t *output_buffer )
{
    size_t bytes_per_row_in, bytes_per_row_out;
    uint16_t *image;
    uint16_t *ptr_list[3];
    int i;
    struct bufsensor bufsensor;

    /* Initialize output_buffer into pixels that look like they came from a
     * three segment sensor.
     *
     * pixels_per_row_in is the number of data pixels only
     * pixels_per_row_out is the number of data pixels + dummy pixels (and size
     *      of output_buffer)
     *
     */

    bytes_per_row_in = pixels_per_row_in * sizeof(uint16_t);
    bytes_per_row_out = pixels_per_row_out * sizeof(uint16_t);

    /* Create a temp buffer we'll pretend is a nice image. We'll then scramble
     * it as if it came from a 3-segment sensor
     */
    image = MEM_MALLOC_ALIGN( bytes_per_row_in, e_32_byte );
    if( !image ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    /* initialize the row to something we can easily see in the output */
    for( i=0 ; i<pixels_per_row_in ; i++ ) {
        image[i] = i;
    }

    memset( output_buffer, 0xee, bytes_per_row_out );

    /* now scramble the input image so it appears to come from a three segment
     * sensor 
     */
    bufsensor_init(&cisx_test_sensor, &bufsensor);

    ptr_list[0] = image;
    ptr_list[1] = bufsensor.segments[0].num_data + ptr_list[0];
    ptr_list[2] = bufsensor.segments[1].num_data + ptr_list[1];
    bufsensor_load_pointers( &bufsensor, ptr_list );

    /* XXX assuming a 3 segment sensor */
    XASSERT( bufsensor.segments[0].num_data 
            + bufsensor.segments[1].num_data 
            + bufsensor.segments[2].num_data == pixels_per_row_in, 
            pixels_per_row_in );

    bufsensor_run( &bufsensor, pixels_per_row_out, image, output_buffer );

    PTR_FREE( image );

    return SCANERR_NONE;
}

static void cisxtest_buffer_init( scan_cmode_t cmode, uint8_t *icetest_idma_buf, int buflen_bytes, 
                                    int pixels_per_row, int num_rows, 
                                    uint16_t *icetest_pixel_data, uint32_t
                                    *row_counter, bool dual_scan )
{
    int bytes_per_row;
    uint32_t num32, *ptr32, *endptr32;
    int rcnt, pcnt, ccnt;
    int color, num_colors, color_table[3];
    uint32_t sanity;
    uint16_t *pixel_ptr;
    int scanner_number;

    dbg2( "%s cmode=%d ppr=%d nr=%d\n", __FUNCTION__, cmode, pixels_per_row, num_rows );

    /* icetest imda uses 32-bit values */
    bytes_per_row = pixels_per_row * sizeof(uint32_t);
    XASSERT( buflen_bytes==bytes_per_row * num_rows, buflen_bytes );

    /* make sure everything is DMA aligned */
    XASSERT( ICE_DMA_IS_ALIGNED((uint32_t)icetest_idma_buf), (uint32_t)icetest_idma_buf );
//    XASSERT( ICE_DMA_IS_ALIGNED(bytes_per_row), bytes_per_row );

    ptr32 = (uint32_t *)icetest_idma_buf;
    endptr32 = (uint32_t *)(icetest_idma_buf + buflen_bytes);

    if( cmode==SCAN_CMODE_MONO ) {
        color_table[0] = PIC_CBI_MEVEN;
        num_colors = 1;
    }
    else {
        color_table[0] = PIC_CBI_CEVEN_0;
        color_table[1] = PIC_CBI_CEVEN_1;
        color_table[2] = PIC_CBI_CEVEN_2;
        num_colors = 3;

        /* davep 04-Apr-2011 ; we send the same row 3 times so we'll need
         * a buffer that's modulo 3 so we will always send an exact triplet
         * (r,g,b)
         */
        XASSERT( num_rows%3==0, num_rows );
        num_rows /= 3;
    }

    sanity = 0;

#define PUSH_PIXEL \
    do { \
            XASSERT( ptr32 < endptr32, (uint32_t)ptr32 );\
            *ptr32++ = num32;\
            sanity++;\
            pixel_ptr++;\
    } while(0);

    // if we are dual scan, we need to switch back and forth between scan sources, but
    // be sure to do it appropriate to the color planes.
    scanner_number=0;

    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<num_rows ; rcnt++ ) {

//        dbg2( "%s rcnt=%d\n", __FUNCTION__, rcnt );

        /* send in planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            /* davep 31-May-2011 ; stamp a row counter onto each row */
//            stamp_row_counter( *row_counter, icetest_pixel_data, pixels_per_row );
//            memset( icetest_pixel_data, *row_counter, pixels_per_row*sizeof(uint16_t) );
            *row_counter += 1;

            pixel_ptr = icetest_pixel_data;

            color = color_table[ccnt];

            /* first pixel in line */
            num32 = PIC_ADCN_ANT_PIC_TAGOUT(scanner_number)
                    | PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                    | PIC_ADCN_ANT_COLOR(color) 
                    | PIC_ADCN_ANT_DATA(*pixel_ptr)
                    ;
            PUSH_PIXEL;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<pixels_per_row-2 ; pcnt++ ) {
                num32 = PIC_ADCN_ANT_PIC_TAGOUT(scanner_number)
                     | PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_NORMP) 
                      | PIC_ADCN_ANT_COLOR( color ) 
                      | PIC_ADCN_ANT_DATA( *pixel_ptr )
                      ;
                PUSH_PIXEL;
            }

            /* send in a last pixel */
            num32 = PIC_ADCN_ANT_PIC_TAGOUT(scanner_number)
                  | PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_LASTP) 
                  | PIC_ADCN_ANT_COLOR( color ) 
                  | PIC_ADCN_ANT_DATA( *pixel_ptr )
                  ;
            PUSH_PIXEL;

        } /* end for ccnt */

        if (dual_scan) {
            scanner_number = (scanner_number+1)%2;
        }

    } /* end for rcnt */

    dbg2( "%s sanity=%d ptr=%p endptr=%p\n", __FUNCTION__, sanity, ptr32, endptr32 );

    XASSERT( sanity==pixels_per_row*num_rows*num_colors, sanity );

    XASSERT( ptr32==endptr32, (uint32_t)ptr32 );
}

static void make_wait_flags( uint32_t *icetest_wait, uint32_t *pic_wait )
{
    int i;

    *icetest_wait = 0;
    for( i=0 ; i<num_icetest_channels ; i++ ) {
        XASSERT( icetest_channels[i] < ICETEST_IDMA_NUM_CHANNELS, icetest_channels[i] );
        SET_BIT( *icetest_wait, icetest_channels[i] );
    }

    *pic_wait = 0;
    for( i=0 ; i<num_pic_channels ; i++ ) {
        XASSERT( pic_channels[i] < PIC_WDMA_NUM_CHANNELS, pic_channels[i] );
        SET_BIT( *pic_wait, pic_channels[i] );
    }
}

static void cisxtest_run( void )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    scan_msg_t msg;
    struct scos_mq *msgq;
    int icetest_received_rows, pic_received_rows;
    uint32_t icetest_wait, pic_wait;
//    int i;
    uint8_t *ptr;
//    uint16_t row_number;
    
    dbg2("%s start\n", __FUNCTION__ );

    msgq = scantask_get_msgq();

    icetest_received_rows = 0;
    pic_received_rows = 0;

    make_wait_flags( &icetest_wait, &pic_wait );

    /* spin waiting for all DMA to complete */
    while( icetest_wait || pic_wait ) {
        /* infinite loop unless our PIC DMA completes */
        
        scerr = scos_mq_receive( msgq, (void *)&msg, USEC_PER_SECOND*5 );

        if( scerr == SCANERR_TIMEDOUT ) {
            /* dump out some stats so we know where we are */
            dbg1("%s no messages; wait some more.\n", __FUNCTION__ );

            icetest_dump();
//            icetest_ddma_dump();
            cisx_dump();
            cisx_ddma_data_peek();

//            pic_wdma_debug_log();
            pic_dump();
//            int i;
//            for( i=0 ; i<num_pic_channels ; i++ ) {
//                pic_wdma_channel_dump( pic_channels[i] );
//            }
//            ASSERT(0);
            continue;
        }

        if( scerr != 0 ) {
            XASSERT( 0, scerr );
            continue;
        }
        
//        dbg2( "%s msg=%#x p1=0x%x p2=0x%x p3=%p\n", __FUNCTION__, 
//                    msg.msgType, msg.param1, msg.param2, msg.param3 );

        switch( msg.msgType ) {
            case SMSG_ICETEST_IDMA_DATA :
                /* ICETest DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                XASSERT( icebuf->channel < ICETEST_IDMA_NUM_CHANNELS, icebuf->channel );
                icebuf_sanity( icebuf );

                icetest_received_rows += icebuf->num_rows;

                dbg2("icetest idma icebuf=%p channel=%d num_rows=%d max_rows=%d bpr=%d data=%p total=%d\n", 
                            icebuf, icebuf->channel, icebuf->num_rows,
                            icebuf->max_rows, icebuf->bytes_per_row, 
                            icebuf->data, icetest_received_rows );

                /* is this the last buffer in this channel? */
                if( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg2( "icetest channel %d is done\n", icebuf->channel );
                    CLR_BIT( icetest_wait, icebuf->channel );
                }

                /* davep 27-May-2011 ; XXX temp debug */
#if 0
                uint8_t *ptr8;
                int j;
                ptr8 = icebuf->data;
                for( j=0 ; j<icebuf->num_rows ; j++ ) {
                    scanlog_hex_dump( ptr8, 64 );
                    ptr8 += icebuf->bytes_per_row;                     
                }
#endif

                icetest_interrupt_disable();
                icetest_idma_add_ready( icebuf->channel, &icebuf );
                icetest_interrupt_enable();
                break;

            case SMSG_PIC_WDMA_DATA :
                /* PIC DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                XASSERT( icebuf->channel < PIC_WDMA_NUM_CHANNELS, icebuf->channel );
                icebuf_sanity( icebuf );

                pic_received_rows += icebuf->num_rows;

                dbg2("pic wdma icebuf=%p channel=%d num_rows=%d max_rows=%d bpr=%d data=%p total=%d\n", 
                            icebuf, icebuf->channel, icebuf->num_rows,
                            icebuf->max_rows, icebuf->bytes_per_row, 
                            icebuf->data, pic_received_rows );

                if( !(GET_BIT( pic_wait, icebuf->channel )) ) {
                    /* WTF? buffer after done? */
                    XASSERT(0,icebuf->channel);
                }

                /* is this the last buffer in this channel? */
                if( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg2( "pic wdma channel %d is done\n", icebuf->channel );
                    CLR_BIT( pic_wait, icebuf->channel );
                }

                ptr = icebuf->data;
                cpu_dcache_invalidate_region( icebuf->data, icebuf->datalen );
#if 0
                for( i=0 ; i<icebuf->num_rows ; i++ ) {
                    row_number = *(uint16_t*)ptr;
                    dbg2( "%s picwdma ch=%d row_number=%d\n", __FUNCTION__, icebuf->channel, row_number );

                    scanlog_hex_dump( ptr, 32 );
                    ptr += icebuf->bytes_per_row;
                }
#endif

//                scanlog_hex_dump( icebuf->data, 64 );
//                scanlog_hex_dump( icebuf->data+icebuf->datalen-64, 64 );
//                scanlog_hex_dump( icebuf->data, icebuf->datalen );

                pic_interrupt_disable();
                pic_wdma_add_ready( icebuf->channel, &icebuf );
                pic_interrupt_enable();
                break;

            default :
                dbg2( "%s msg=%#x p1=0x%x p2=0x%x p3=%p\n", __FUNCTION__, 
                            msg.msgType, msg.param1, msg.param2, msg.param3 );
                XASSERT( 0, msg.msgType );
                break;

        } /* end switch(msg.param1) */

    } /* end while( ) */

    dbg2( "%s done\n", __FUNCTION__ );
}

static void verify( void )
{
    /* TODO */
}

void sneaky_eoi( scan_cmode_t cmode, struct ice_dma_buffer *icebuf )
{
    int i, num_channels;
    uint32_t *ptr32, num32;

    num_channels = 1;
    if( cmode==SCAN_CMODE_COLOR ) {
        num_channels = 3;
    }

    ptr32 = (uint32_t *)(icebuf->data + icebuf->datalen);
    ptr32--;
    /* should now be pointing to last pixel */

    for( i=0 ; i<num_channels ; i++ ) {
        num32 = *ptr32;

        /* change data type to EOI */
        num32 &= ~PIC_ADCN_ANT_DATATYPE(~0);
        num32 |= PIC_ADCN_ANT_DATATYPE(PIC_CBI_EOI); 
        *ptr32 = num32;

        dbg2( "%s %d %p %#x\n", __FUNCTION__, __LINE__, ptr32, *ptr32 );

        /* back up to end of previous line of 32-bit values */
        ptr32 -= icebuf->bytes_per_row/4;
    }
}

static scan_err_t cisxtest_icetest_setup( scan_cmode_t cmode, uint32_t total_rows, bool dual_scan )
{
    scan_err_t scerr, final_scerr;
    int icetest_bytes_per_row, icetest_pixels_per_row, data_pixels_per_row;
    int i, num_buffers, rows_per_buffer, buffers_allocated;
    struct ice_dma_buffer *icebuf;
    uint32_t row_counter, rows_remaining;
    uint16_t *icetest_pixel_data;

    dbg2( "%s cmode=%d total_rows=%d\n", __FUNCTION__, cmode, total_rows);

    final_scerr = SCANERR_NONE;
    icetest_pixel_data = NULL;

    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    memset( icetest_channels, 0, sizeof(icetest_channels) );
    icetest_channels[0] = 0;
    num_icetest_channels = 1;

    /* pixels out of cisx (used to initialize pixel buffers to "good" pixels */
    data_pixels_per_row = 0;
    /* pixels into cisx (used to malloc icetest buffers, etc) */
    icetest_pixels_per_row = 0;
    for( i=0 ; i<3 ; i++ ) {
        data_pixels_per_row += cisx_test_sensor.segments[i].num_data;
        icetest_pixels_per_row += cisx_test_sensor.segments[i].num_dummy + cisx_test_sensor.segments[i].num_data;
    }

    if( cmode==SCAN_CMODE_COLOR ) {
        total_rows *= 3;
    }

    /* davep 13-Mar-2013 ; If we are doing dual scan, we will need 2x the
     * rows (one set of rows for each sensor).
     */
    if( dual_scan ) {
        total_rows *= 2;
    }

    /* the ICETest block runs on 32-bit values */
    icetest_bytes_per_row = icetest_pixels_per_row * sizeof(uint32_t);

    /* davep 30-Mar-2011 ; XXX bleah; magic numbers */
    rows_per_buffer = 6;

    /* weird math to round up */
    num_buffers = (total_rows+rows_per_buffer-1) / rows_per_buffer;
    ASSERT( num_buffers );

    buffers_allocated = icetest_idma_open( icetest_channels, num_icetest_channels, num_buffers, 
                                            total_rows, icetest_bytes_per_row );
    XASSERT( buffers_allocated==num_buffers, buffers_allocated );

    /* create a buffer of pixels we'll pretend is coming from the sensor */
    icetest_pixel_data = MEM_MALLOC_ALIGN( icetest_pixels_per_row*sizeof(uint16_t), e_32_byte );
    if( !icetest_pixel_data ) {
        final_scerr = SCANERR_OUT_OF_MEMORY;
        goto fail;
    }

    /* initialize our buffer with scrambled pixels */
    scerr = make_sensor_data( data_pixels_per_row, icetest_pixels_per_row, icetest_pixel_data );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto fail;
    }

    /* Create a block of icebufs we'll feed into icetest IDMA. The pixel rows
     * will be copies of our icetest_pixel_data 
     */

    rows_remaining = total_rows;
    row_counter = 0;

    for( i=0 ; i<num_buffers ; i++ ) {
        icebuf = icebuf_new( rows_per_buffer, icetest_bytes_per_row, ICEBUF_TAG_ICETEST_IDMA );
        if( icebuf==NULL ) {
            final_scerr = SCANERR_OUT_OF_MEMORY;
            goto fail;
        }

        /* initialize each row by combining 16-bits of CBI metadata with the
         * 16-bit pixel from icetest_pixel_data
         */
        cisxtest_buffer_init( cmode, icebuf->data, icebuf->datalen,
                                icetest_pixels_per_row, icebuf->max_rows, 
                                icetest_pixel_data, &row_counter, dual_scan );

        icebuf->num_rows = MIN( icebuf->max_rows, rows_remaining );
        rows_remaining -= icebuf->num_rows;
        XASSERT( rows_remaining>=0, rows_remaining );

        /* davep 19-Apr-2011 ; set the magic EOI on the last pixel */
        if( 0 ) {
//        if( i==num_buffers-1 ) {
            sneaky_eoi( cmode, icebuf );
        }

        /* davep 26-May-2011 ; XXX temp debug */
#if 0
        uint32_t *ptr32, *endptr32;
        uint8_t *ptr8;
        int j;
        ptr8 = icebuf->data;
        for( j=0 ; j<icebuf->num_rows ; j++ ) {
            /* skip the dead pixels: 64 dead pixels, 3 segments, 4 bytes per pixel */
            scanlog_hex_dump( ptr8+64*3*4, 64 );

            ptr32 = (uint32_t *)ptr8;
            endptr32 = (uint32_t *)(ptr8 + icebuf->bytes_per_row - sizeof(uint32_t));
            dbg2( "%s %d %#08x %#08x\n", __FUNCTION__, __LINE__, *ptr32, *endptr32 );
            ptr8 += icebuf->bytes_per_row;                     
        }
#endif

//        dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//        scanlog_hex_dump( icebuf->data, icebuf->datalen );
//        scanlog_hex_dump( icebuf->data, 384+16 );
//        ASSERT(0);
//        ptr32 = (uint32_t *)icebuf->data;
//        endptr32 = (uint32_t *)(icebuf->data + icebuf->datalen);
//        endptr32--; /* point to last entry in buffer */
//        dbg2( "%s %d %#08x %#08x\n", __FUNCTION__, __LINE__, *ptr32, *endptr32 );

        cpu_dcache_writeback_region( icebuf->data, icebuf->datalen );

        icetest_idma_add_ready( 0, &icebuf );
    }

    XASSERT( rows_remaining==0, rows_remaining );

    PTR_FREE( icetest_pixel_data );

    /* davep 25-May-2011 ; XXX temp debug */
//    icetest_dump();
//    icetest_ddma_dump();

    /* turn on the ICETest hardware */
    icetest_enable( true );
//    icetest_set_test_mode( ICETEST_CONFIG_DATA_TO_SCAN );
    icetest_set_test_mode( ICETEST_CONFIG_DATA_TO_CISX );

    /* prime the ICETest idma pump */
    icetest_idma_channel_enable( 0 );
    icetest_idma_channel_launch( 0 );

    /* davep 19-Feb-2013 ; XXX temp debug */
//    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//    pic_dump();

    /* fire it up! */
    icetest_idma_late_launch();

    return SCANERR_NONE;

fail : 
    icetest_idma_close();
    if( icetest_pixel_data ) {
        PTR_FREE( icetest_pixel_data );
    }
    return final_scerr;
}

static scan_err_t cisxtest_cisx_setup( scan_cmode_t cmode, bool use_nsensor )
{
    scan_err_t scerr;

    dbg2( "%s cmode=%d\n", __FUNCTION__, cmode );

    scerr = cisx_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* allocate memory for cisx */
    scerr = cisx_open( cmode, &cisx_test_sensor, use_nsensor );
    XASSERT( scerr==SCANERR_NONE, scerr );
    
    cisx_launch();

    /* XXX temp debug */
//    cisx_dump();
//    cisx_ddma_dump();

    return SCANERR_NONE;
}

static scan_err_t cisxtest_pic_setup( scan_cmode_t cmode, uint32_t pixels_per_row, uint32_t total_rows )
{
    struct pic_dma_test pictest;
    scan_err_t scerr;
    int i;
    uint32_t pic_bytes_per_row, pic_pixels_per_row_out;
    int pic_bpp;

    dbg2( "%s cmode=%d ppr=%d total_rows=%d\n", __FUNCTION__, cmode, pixels_per_row, total_rows );

    scerr = pic_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 21-Jun-2011 ; XXX temp debug ; skip PIC */
//    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_DISCARD);
//    pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
//    return SCANERR_NONE;

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest_choose_channels( cmode, &pictest );

    /* copy the channels to the global so cisxtest_run() can use */
    num_pic_channels = pictest.num_channels;
    XASSERT( pictest.num_channels<=PIC_WDMA_NUM_CHANNELS, pictest.num_channels );
    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_channels[i] = pictest.channels[i];
    }
    
    /* XXX - compile-time hardwired the bpp */
    pic_bpp = 8;
//    pic_bpp = 16;

    pic_pixels_per_row_out = pixels_per_row;
    scerr = pic_marg_calc_fit( 0, &pic_pixels_per_row_out, pic_bpp, PIC_MARG_PIXELFIT_TRUNCATE_RIGHT );
    XASSERT( scerr==SCANERR_NONE, scerr );

    pic_bytes_per_row = pic_pixels_per_row_out * (pic_bpp/8);
    XASSERT( ICE_DMA_IS_ALIGNED(pic_bytes_per_row), pic_bytes_per_row );

    pictest.total_rows = total_rows; 
    pictest.pixels_per_row = pic_pixels_per_row_out;
    pictest.bytes_per_row = pic_bytes_per_row;
    pictest.num_empties = pictest.total_rows; 
    pictest.buffers_per_channel = pictest.total_rows;
    pictest.rows_per_buffer = 6;

    pictest_setup_dma( &pictest );

    /* davep 21-Jun-2011 ; add margins to handle unaligned CISX pixel widths */
    pic_marg_set_left( 0 );
    pic_marg_set_right( pic_pixels_per_row_out-1 );
    pic_marg_set_bypass( false );

    /* davep 28-Aug-2012 ; add support for 8-bpp */
    scerr = pic_wdma_set_bpp( pic_bpp );
    XASSERT( scerr==SCANERR_NONE, scerr );

//    /* davep 31-May-2011 ; XXX temp debug */
//    pic_wdma_channel_dump( 0 );
//    pic_wdma_channel_dump( 1 );
//    pic_wdma_channel_dump( 2 );

    return SCANERR_NONE;
}

static scan_err_t cisx_test_simple( scan_cmode_t cmode, uint32_t total_rows )
{
    scan_err_t scerr, final_scerr;
    int i;
    uint32_t pixels_per_row;
    bool dual_scan;

    /* davep 02-Apr-2013 ; TODO add run-time setting of dual scan for testing */
//    dual_scan = cisx_get_dual_flag();
    dual_scan = false;

    /* how many pixels are we going to get out of CISX? */
    pixels_per_row = 0;
    for( i=0 ; i<cisx_test_sensor.num_segments ; i++ ) {
        pixels_per_row += cisx_test_sensor.segments[i].num_data;
    }

    dbg2( "%s cmode=%d ppr=%d total_rows=%d\n", __FUNCTION__, cmode, pixels_per_row, total_rows );
        
    /* need PIC WDMA */
    scerr = cisxtest_pic_setup( cmode, pixels_per_row, total_rows );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto leave;
    }

    /* need CISX */
    scerr = cisxtest_cisx_setup( cmode, dual_scan );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto leave;
    }

    /* davep 20-Feb-2013 ; turn off CISX until ICEtest configured */
    cisx_interrupt_disable();

    /* need ICE Test */
    scerr = cisxtest_icetest_setup( cmode, total_rows, dual_scan );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto leave;
    }

    /* davep 20-Feb-2013 ; check for unhappy CISX before we enable interrupts
     * so we can do big dumps in thread context rather than interrupt context
     */
    if( cisx_get_int_pending() ) {
        icetest_dump();
        cisx_dump();
        pic_dump();
        XASSERT(0,cisx_get_int_pending());
    }

    /* davep 13-Mar-2013 ; XXX temp debug */
    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
    cisx_dump();

    icetest_interrupt_enable();
    pic_interrupt_enable();
    cisx_interrupt_enable();

    /* Run it ! */
#if 1
    cisxtest_run();
    verify();
#else
    icetest_idma_cancel();
    pic_wdma_cancel();
#endif

    /* we're now done; prepare for validation and cleanup */

    icetest_interrupt_disable();
    pic_interrupt_disable();
    cisx_interrupt_disable();

    icetest_enable( false );
    icetest_idma_disable();

    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        cisx_odma_channel_disable( i );
    }
    for( i=0 ; i<CISX_IDMA_NUM_CHANNELS ; i++ ) {
        cisx_idma_channel_disable( i );
    }

    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_disable( pic_channels[i] );
    }

    icetest_idma_sanity();
    icetest_idma_close();

    cisx_sanity();
    cisx_close();

    pic_wdma_sanity();
    pic_wdma_close();

    /* success! */
    final_scerr = SCANERR_NONE;

leave : 
    /* clean up */
    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = pic_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = cisx_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    dbg2( "%s done\n", __FUNCTION__ );

    return final_scerr;
}

scan_err_t cisx_test( void )
{
    scan_err_t scerr;
    uint32_t total_rows;

    dbg2( "%s\n", __FUNCTION__ );

    total_rows = 30;

//    scerr = cisx_test_simple( SCAN_CMODE_COLOR, total_rows );
//    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = cisx_test_simple( SCAN_CMODE_MONO, total_rows );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = cisx_test_simple( SCAN_CMODE_MONO, total_rows );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = cisx_test_simple( SCAN_CMODE_COLOR, total_rows );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = cisx_test_simple( SCAN_CMODE_MONO, total_rows );
    XASSERT( scerr==SCANERR_NONE, scerr );

#if 0
    scerr = cisx_test_simple( SCAN_CMODE_COLOR, total_rows );
    XASSERT( scerr==SCANERR_NONE, scerr );

    int i;
    for( i=0 ; i<10 ; i++ ) {
        dbg2("%s before heapsize=%d heapfree=%d\n", __FUNCTION__, 
                    memGetHeapSize(NULL), memGetHeapFreeMemorySize(NULL) );

        scerr = cisx_test_simple( SCAN_CMODE_MONO, total_rows );
        XASSERT( scerr==SCANERR_NONE, scerr );

        dbg2("%s after heapsize=%d heapfree=%d\n", __FUNCTION__, 
                    memGetHeapSize(NULL), memGetHeapFreeMemorySize(NULL) );
    }

    scerr = cisx_test_simple( SCAN_CMODE_MONO, 300 );
    XASSERT( scerr==SCANERR_NONE, scerr );
#endif

    dbg2( "%s done\n", __FUNCTION__ );

    return SCANERR_NONE;
}

