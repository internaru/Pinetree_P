/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *  Test the blocks in PIE. No exhaustive probing, just a few friendly tests to
 *  say, "Hello, PIE. How are you?"
 */

#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "memAPI.h"
#include "list.h"
#include "cpu_api.h"
#include "io.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanlog.h"
#include "piehw.h"
#include "pie.h"
#include "piereset.h"
#include "lehmer.h"
#include "safetylock.h"
#include "icedma.h"
#include "scantask.h"
#include "ostools.h"
#include "pietest.h"

#if 1
#define scanlog_scanlog_hex_dump(x,y) 
#else
#define scanlog_scanlog_hex_dump(x,y)   do { \
                                    if(scanlog_get_level()>=2) { \
                                        scanlog_hex_dump(x,y); \
                                    }\
                                } while(0);
#endif

static uint8_t pie_rchannels[ PIE_RDMA_NUM_CHANNELS ];
static uint8_t pie_wchannels[ PIE_WDMA_NUM_CHANNELS ];
static uint8_t num_pie_rchannels;
static uint8_t num_pie_wchannels;

static void regblockset( volatile uint32_t *ptr32, uint32_t count, uint32_t value )
{
    while( count ) {
        *ptr32++ = value;
        count--;
    }
}

static void regblocktest( volatile uint32_t *ptr32, uint32_t count, uint32_t value )
{
    scanlog_scanlog_hex_dump( (unsigned char *)ptr32, count*sizeof(uint32_t) );
    while( count ) {
        XASSERT( *ptr32==value, (uint32_t)ptr32 );
        ptr32++;
        count--;
    }
}

static scan_err_t pie_bde_test_memory( void )
{
    int i;
    uint32_t *lut;
    int lutsize_bytes;

    dbg2( "%s\n", __FUNCTION__ );

    lutsize_bytes = PIE_BDE_LUT_SIZE * sizeof(uint32_t);

    lut = MEM_MALLOC( lutsize_bytes );
    if( lut==NULL ) {
        dbg1( "%s out of memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    memset( lut, 0xff, lutsize_bytes );

    pie_bde_setup( PIE_BDE_MODE_COLOR, lut );

    /* verify */
    pie_bde_enable_fw_access( true );
//    scanlog_scanlog_hex_dump( (unsigned char *)PIE_BDE_LUT, lutsize_bytes );
    for( i=0 ; i<PIE_BDE_LUT_SIZE ; i++ ) {
        XASSERT( PIE_BDE_LUT[i]==PIE_BDE_LUT_ENTRY(0xffffffff), i );
    }
    pie_bde_enable_fw_access( false );

    /* once more! with feewing! */
    pie_bde_reset();
    memset( lut, 0x71, lutsize_bytes );
    pie_bde_setup( PIE_BDE_MODE_MONO, lut );
    pie_bde_enable_fw_access( true );
    for( i=0 ; i<PIE_BDE_LUT_SIZE ; i++ ) {
        XASSERT( PIE_BDE_LUT[i]==PIE_BDE_LUT_ENTRY(0x71717171), i );
    }
    pie_bde_enable_fw_access( false );

    PTR_FREE( lut );

    dbg2( "%s done\n", __FUNCTION__ );

    return 0;
}

static scan_err_t pie_csc_test_coefficent_matrices( void )
{
    /* test 3x3 matrix */
    pie_csc_reset();

//    pie_csc_dump();

    /* the reset sets the 3x3 matrices to zero so verify before we start poking
     * our own values in there 
     */
    regblocktest( PIE_CSC_RGB_MUL, 9, 0 );
    regblocktest( PIE_CSC_ESR_MUL, 9, 0 );
    regblocktest( PIE_CSC_YCC_MUL, 9, 0 );

    /* poke a value in each LUT, see if it sticks */
    regblockset( PIE_CSC_RGB_MUL, 9, ~0 );
    regblockset( PIE_CSC_ESR_MUL, 9, ~0 );
    regblockset( PIE_CSC_YCC_MUL, 9, ~0 );
    regblocktest( PIE_CSC_RGB_MUL, 9, PIE_CSC_RGB_MUL_ENTRY(~0) );
    regblocktest( PIE_CSC_ESR_MUL, 9, PIE_CSC_ESR_MUL_ENTRY(~0) );
    regblocktest( PIE_CSC_YCC_MUL, 9, PIE_CSC_YCC_MUL_ENTRY(~0) );

    return 0;
}

static void pie_csc_set_all_luts( uint32_t *lut )
{
    dbg2( "%s\n", __FUNCTION__ );

    pie_csc_rgb_enable_fw_access( true );
    if( pie_csc_has_unified_rgb_luts() ) {
        /* running on something with unified CSC RGB LUT */
        pie_csc_rgb_set_lut( lut, PIE_CSC_RGB_LUT );
    }
    else {
        /* running on something with individual CSC RGB LUTs; set all three
         * of the LUTs to the same values
         */
        pie_csc_rgb_set_lut( lut, PIE_CSC_RED_LUT );
        pie_csc_rgb_set_lut( lut, PIE_CSC_GREEN_LUT );
        pie_csc_rgb_set_lut( lut, PIE_CSC_BLUE_LUT );
    }
    pie_csc_rgb_enable_fw_access( false );
}

static void pie_csc_verify_luts( uint32_t *lut )
{
    int i;
    volatile uint32_t *ptr32;

    pie_csc_rgb_enable_fw_access( true );

    if( pie_csc_has_unified_rgb_luts() ) {
        /* TODO ; I don't have running firmware on such a chip anymore */
        ASSERT(0);
    }
    else {
        /* davep 18-Oct-2011 ; (asicbz 3687) dare not disturb foo32 until
         * 18-Oct-2013 
         */
        uint32_t foo32;

        ptr32 = PIE_CSC_RED_LUT;
        for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE-1 ; i++ ) {
            foo32 = ptr32[i];
            XASSERT( ptr32[i]==lut[i], i );
        }
        XASSERT( *PIE_CSC_RED_OVERFLOW==lut[i], i );

        ptr32 = PIE_CSC_GREEN_LUT;
        for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE-1 ; i++ ) {
            foo32 = ptr32[i];
            XASSERT( ptr32[i]==lut[i], i );
        }
        XASSERT( *PIE_CSC_GREEN_OVERFLOW==lut[i], i );

        ptr32 = PIE_CSC_BLUE_LUT;
        for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE-1 ; i++ ) {
            foo32 = ptr32[i];
            XASSERT( ptr32[i]==lut[i], i );
        }
        XASSERT( *PIE_CSC_BLUE_OVERFLOW==lut[i], i );
    }
    pie_csc_rgb_enable_fw_access( false );
}

static scan_err_t pie_csc_test_luts( void )
{
    int i;
    uint32_t *lut;
    int lutsize_bytes;

    dbg2( "%s\n", __FUNCTION__ );

    lutsize_bytes = PIE_CSC_RGB_LUT_SIZE * sizeof(uint32_t);

    lut = MEM_MALLOC( lutsize_bytes );
    if( lut==NULL ) {
        dbg1( "%s out of memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    pie_csc_reset();

    memset( lut, ~0, lutsize_bytes );
    scanlog_scanlog_hex_dump( (unsigned char *)lut, 64 );

    pie_csc_set_all_luts( lut );
//    pie_csc_dump();

    /* incrementing pattern */
    for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
        lut[i] = PIE_CSC_RGB_LUT_LUTVAL( i );
    }
    pie_csc_set_all_luts( lut );
    pie_csc_dump();

    /* verify the pattern */
    pie_csc_verify_luts( lut );

    PTR_FREE( lut );

    return 0;
}

static scan_err_t pie_csc_test_memory( void )
{
    scan_err_t scerr;

    if( pie_csc_has_unified_rgb_luts() ) {
        dbg1( "%s CSC has single LUT\n", __FUNCTION__ );
    }
    else {
        dbg1( "%s CSC has multiple LUTs\n", __FUNCTION__ );
    }

    /* poke stuff into the 3x3 matrices */
    scerr = pie_csc_test_coefficent_matrices();
    XASSERT( scerr==0, scerr );

    /* poke stuff into the LUTs, see what happens */
    scerr = pie_csc_test_luts();
    XASSERT( scerr==0, scerr );

    return 0;
}

static scan_err_t pie_tcns_test_memory( void )
{
    uint32_t *lut;
    int lutsize_bytes;
    int i;
    volatile uint32_t *ptr32;
    struct timespec curr_time;

    dbg2( "%s\n", __FUNCTION__ );

    lutsize_bytes = PIE_TCNS_LUT_SIZE * sizeof(uint32_t);

    lut = MEM_MALLOC( lutsize_bytes );
    if( lut==NULL ) {
        dbg1( "%s out of memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    pie_tcns_reset();
//    pie_tcns_dump();

    /* set all bits */
    memset( lut, 0xff, lutsize_bytes );
    pie_tcns_enable_fw_access( true );
    pie_tcns_set_lut( lut );
    scanlog_scanlog_hex_dump( (unsigned char *)PIE_TCNS_LUT, lutsize_bytes );
    pie_tcns_enable_fw_access( false );
    pie_tcns_dump();

    /* set to increasing value */
    pie_tcns_reset();
    memset( lut, 0, lutsize_bytes );
    for( i=0 ; i<PIE_TCNS_LUT_SIZE; i++ ) {
        lut[i] = PIE_TCNS_LUT_Y(i/4) 
                 | PIE_TCNS_LUT_CR(i/4)
                 | PIE_TCNS_LUT_CB(i/4);
    }
    pie_tcns_enable_fw_access( true );
    pie_tcns_set_lut( lut );
    scanlog_scanlog_hex_dump( (unsigned char *)PIE_TCNS_LUT, lutsize_bytes );
    pie_tcns_enable_fw_access( false );
    pie_tcns_dump();

    /* test our increasing value */
    pie_tcns_enable_fw_access( true );
    ptr32 = PIE_TCNS_LUT;
    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        XASSERT( *ptr32 == lut[i], i );
        XASSERT( PIE_TCNS_GET_Y(*ptr32)==PIE_TCNS_GET_Y(lut[i]), i );
        XASSERT( PIE_TCNS_GET_CB(*ptr32)==PIE_TCNS_GET_CB(lut[i]), i );
        XASSERT( PIE_TCNS_GET_CR(*ptr32)==PIE_TCNS_GET_CR(lut[i]), i );
        ptr32++;
    }
    pie_tcns_enable_fw_access( false );

    /* test with random values */
    clock_gettime( CLOCK_REALTIME, &curr_time ); 
    lehmer_srandom( curr_time.tv_nsec );

    memset( lut, 0, lutsize_bytes );
    for( i=0 ; i<PIE_TCNS_LUT_SIZE; i++ ) {
        lut[i] = PIE_TCNS_LUT_Y( (uint8_t)lehmer_random() ) 
                 | PIE_TCNS_LUT_CR( (uint8_t)lehmer_random() )
                 | PIE_TCNS_LUT_CB( (uint8_t)lehmer_random() );
    }
    pie_tcns_enable_fw_access( true );
    pie_tcns_set_lut( lut );
    scanlog_scanlog_hex_dump( (unsigned char *)PIE_TCNS_LUT, lutsize_bytes );
    pie_tcns_enable_fw_access( false );
    pie_tcns_dump();

    /* test our random values */
    pie_tcns_enable_fw_access( true );
    ptr32 = PIE_TCNS_LUT;
    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        XASSERT( *ptr32 == lut[i], i );
        XASSERT( PIE_TCNS_GET_Y(*ptr32)==PIE_TCNS_GET_Y(lut[i]), i );
        XASSERT( PIE_TCNS_GET_CB(*ptr32)==PIE_TCNS_GET_CB(lut[i]), i );
        XASSERT( PIE_TCNS_GET_CR(*ptr32)==PIE_TCNS_GET_CR(lut[i]), i );
        ptr32++;
    }
    pie_tcns_enable_fw_access( false );

    PTR_FREE( lut );

    return 0;
}

/**
 * \brief  Validate the output of PIE DMA.
 *
 * Currently hardwired to 8-bpp RGB planar output with a fixed input pattern of
 * 16-bpp 'Rr', 'Gg', 'Bb' pixel values.
 *
 * \author David Poole
 * \date 23-Apr-2010
 *
 */

#if 0
static const uint32_t valid_pixel[] = { 
                               /* r */ 0x72727272, 
                               /* g */ 0x67676767, 
                               /* b */ 0x62626262 };

static void validate_pie_wdma_icebuf( struct ice_dma_buffer *icebuf )
{
    uint32_t *ptr32, *endptr32;
    uint32_t pixel32;
    int i;

    /* XXX hardwire to look for 8-bpp 'r', 'g', or 'b'. 
     * 
     * TODO add support for RGBX, XRGB, RGB-packed, yadda yadda yadda.
     */
    dbg2( "%s ch=%d\n", __FUNCTION__, icebuf->channel );

    XASSERT( icebuf->bytes_per_row%sizeof(uint32_t)==0, icebuf->bytes_per_row );

    pixel32 = valid_pixel[icebuf->channel];

    for( i=0 ; i<icebuf->num_rows ; i++ ) {
        ptr32 = (uint32_t *)( icebuf->data + i*icebuf->bytes_per_row );
        endptr32 = ptr32 + icebuf->bytes_per_row/sizeof(uint32_t);
        while( ptr32 < endptr32 ) {
            if( *ptr32 != pixel32 ) {
                dbg2( "%s ptr32=%p %#x %#x\n", __FUNCTION__, ptr32, *ptr32, pixel32 );
                scanlog_hex_dump( icebuf->data, icebuf->datalen );
                ASSERT( 0 );
            }
            ptr32++;
        }
    }
}
#endif

static void make_wait_flags( uint32_t *pie_wait )
{
    int i;

    *pie_wait = 0;
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        XASSERT( pie_rchannels[i] < PIE_RDMA_NUM_CHANNELS, pie_rchannels[i] );
        SET_BIT( *pie_wait, pie_rchannels[i] + 8);
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        XASSERT( pie_wchannels[i] < PIE_WDMA_NUM_CHANNELS, pie_wchannels[i] );
        SET_BIT( *pie_wait, pie_wchannels[i] );
    }
}

static void pie_dma_message_loop( ATLISTENTRY *pie_rdma_done_buffers, 
                                  ATLISTENTRY *pie_wdma_done_buffers )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    scan_msg_t msg;
    struct scos_mq * msgq;
    int i;
    uint32_t qcnt;
    uint32_t pie_wait;
    int pier_tot_rows, piew_tot_rows, pier_dma_msgs, piew_dma_msgs;
//    uint8_t channel;

    dbg2("%s start\n", __FUNCTION__ );

    /* build a bitmask we can use to do a simple compare to watch for all the
     * channels finishing
     */
    make_wait_flags( &pie_wait );

    msgq = scantask_get_msgq();

    pier_tot_rows = 0;
    piew_tot_rows = 0;
    pier_dma_msgs = 0;
    piew_dma_msgs = 0;

    /* quiet a warning */
    i=0;

    /* spin waiting for PIC DMA to complete */
    while( pie_wait ) {
        /* infinite loop unless our PIC DMA completes */
//        dbg2( "waiting up to 5 seconds for messages...\n" );
        
        scerr = scos_mq_receive( msgq, (void *)&msg, USEC_PER_SECOND*5 );

        if( scerr == SCANERR_TIMEDOUT ) {
            /* dump out some stats so we know where we are */
            dbg1("%s no messages; wait some more.\n", __FUNCTION__ );

            pie_rdma_debug_log();
            pie_rdma_dump();
//            for( i=0 ; i<num_pie_rchannels ; i++ ) {
//                pie_rdma_channel_dump( pie_rchannels[i] );
//            }

            pie_wdma_debug_log();
            pie_wdma_dump();
//            for( i=0 ; i<num_pie_wchannels ; i++ ) {
//                pie_wdma_channel_dump( pie_wchannels[i] );
//            }

//            pie_dma_dump_counters();
//            struct pie_interrupt_stats *ps = get_pie_interrupt_stats();
//            dbg2( "%s count=%d\n", __FUNCTION__, ps->count );

            continue;
        }

        switch( msg.msgType ) {
            case SMSG_PIE_RDMA_DATA: 
                /* PIE read DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );
                XASSERT( icebuf->channel < PIE_RDMA_NUM_CHANNELS, icebuf->channel );

                pier_tot_rows += icebuf->num_rows;
                pier_dma_msgs += 1;

                dbg1("pier msg=%p channel=%d rows=%d bpr=%d data=%p total=%d msgs=%d\n", 
                    icebuf, icebuf->channel, icebuf->num_rows,
                    icebuf->bytes_per_row,
                    icebuf->data, pier_tot_rows, pier_dma_msgs );
//                scanlog_hex_dump( icebuf->data, 64 );
//                scanlog_scanlog_hex_dump( icebuf->data, icebuf->datalen );

                /* last buffer in this channel? */
                if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg1("pier dma channel %d done\n", icebuf->channel );
                    /* make sure we don't get more than one done message */
                    XASSERT( (GET_BIT( pie_wait, icebuf->channel + 8 )), icebuf->channel );
                    CLR_BIT( pie_wait, icebuf->channel + 8 );
                }

#if 0
                /* send it back for more use */
                pie_interrupt_disable();
                channel = icebuf->channel;
                pie_rdma_add_ready( icebuf->channel, &icebuf );
                pie_rdma_channel_launch( channel );
                pie_interrupt_enable();
#else
                ATInsertTailList( pie_rdma_done_buffers, &icebuf->listnode );
#endif
                break;

            case SMSG_PIE_WDMA_DATA: 
                /* PIE write DMA complete interrupt */
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );
                XASSERT( icebuf->channel < PIE_WDMA_NUM_CHANNELS, icebuf->channel );

                piew_tot_rows += icebuf->num_rows;
                piew_dma_msgs += 1;

                dbg1("piew msg=%p channel=%d rows=%d bpr=%d data=%p total=%d msgs=%d\n", 
                    icebuf, icebuf->channel, icebuf->num_rows,
                    icebuf->bytes_per_row,
                    icebuf->data, piew_tot_rows, piew_dma_msgs );
                
//                cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );
//                scanlog_hex_dump( icebuf->data, 64 );
//                scanlog_scanlog_hex_dump( icebuf->data, icebuf->datalen );

                /* stupid human check */
                XASSERT( icebuf->num_rows > 0, (uint32_t)icebuf );

                /* last buffer in this channel? */
                if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                    dbg1("piew dma channel %d done\n", icebuf->channel );
                    /* make sure we don't get more than one done message */
                    XASSERT( (GET_BIT( pie_wait, icebuf->channel )), icebuf->channel );
                    CLR_BIT( pie_wait, icebuf->channel );
                }

                /* davep 14-Aug-2012 ; very very simple stupid check */
#ifdef DMA_BUFFER_INIT
                XASSERT( icebuf->data[0]!=DMA_BUFFER_INIT, icebuf->data[0] );
#endif
#if 0
                validate_pie_wdma_icebuf( icebuf );

                /* send it back for more use */
                pie_interrupt_disable();
                channel = icebuf->channel;
                pie_wdma_add_ready( icebuf->channel, &icebuf );
                pie_wdma_channel_launch( channel );
                pie_interrupt_enable();
#else
                ATInsertTailList( pie_wdma_done_buffers, &icebuf->listnode );
#endif
                break;

            default :
                XASSERT( 0, msg.msgType );
                break;

        } /* end switch(msg.param1) */

    } /* end while( pie_wait ) */

    /* our message queue should be empty (no pending buffers) */
    qcnt = scos_mq_num_pending( msgq );
    XASSERT( qcnt==0, qcnt );

    dbg2("%s end\n", __FUNCTION__ );
}

static void memset_pixel_buffer( uint8_t *data, int datalen_bytes, int bits_per_pixel, uint32_t pixel_value )
{
    uint16_t *ptr16;
    int i;
    int num_pixels;
    int bytes_per_pixel;

    bytes_per_pixel = bits_per_pixel/8;
    XASSERT( datalen_bytes%bytes_per_pixel==0, datalen_bytes );
    num_pixels = datalen_bytes / bytes_per_pixel;

    if( bits_per_pixel==8 ) {
        memset( data, (uint8_t)pixel_value, datalen_bytes );
    }
    else if( bits_per_pixel==16 ) {
        ptr16 = (uint16_t *)data;
        for( i=0 ; i<num_pixels ; i++ ) {
            *ptr16 = (uint16_t)pixel_value;
            ptr16++;
        }
    }
    else {
        /* TODO */
        XASSERT( 0, bits_per_pixel );
    }
}

static void incrementing_pixel_buffer( uint8_t *data, int datalen_bytes, 
                                       int bits_per_pixel, uint32_t starting_pixel_value )
{
    uint8_t *ptr8;
    uint16_t *ptr16;
    int i;
    int num_pixels;
    int bytes_per_pixel;
    uint8_t n8;
    uint16_t n16;

    /* create a pixel buffer with incrementing values */

    bytes_per_pixel = bits_per_pixel/8;
    XASSERT( datalen_bytes%bytes_per_pixel==0, datalen_bytes );
    num_pixels = datalen_bytes / bytes_per_pixel;

    if( bits_per_pixel==8 ) {
        n8 = (uint8_t)starting_pixel_value;
        ptr8 = data;
        for( i=0 ; i<num_pixels ; i++ ) {
            *ptr8 = n8;
            ptr8++;
            n8++;
        }
    }
    else if( bits_per_pixel==16 ) {
        n16 = (uint16_t)starting_pixel_value;
        ptr16 = (uint16_t *)data;
        for( i=0 ; i<num_pixels ; i++ ) {
            *ptr16 = (uint16_t)n16;
            ptr16++;
            n16++;
        }
    }
    else {
        /* TODO */
        XASSERT( 0, bits_per_pixel );
    }

}

static int pie_setup_rdma_buffers( uint8_t channel, int num_buffers, 
                                   int rows_per_buffer, 
                                   int pixels_per_row,
                                   int bits_per_pixel,
                                   uint32_t pixel_value )
{
    int i;
    int datalen;
    int retcode;
    uint8_t *data;
    int total_bytes;
    int bytes_per_row;

    XASSERT( bits_per_pixel==8 || bits_per_pixel==16, bits_per_pixel );

    bytes_per_row = (bits_per_pixel/8) * pixels_per_row;

    total_bytes = num_buffers * rows_per_buffer * bytes_per_row;

    dbg2( "%s channel=%d buffers=%d rows=%d bpr=%d total_bytes=%d\n", 
                __FUNCTION__, channel, num_buffers, rows_per_buffer,
                bytes_per_row, total_bytes );

    datalen = rows_per_buffer * bytes_per_row;

    for( i=0 ; i<num_buffers ; i++ ) {

        data = ice_dma_alloc_buffer( datalen );
        if( data==NULL ) {
            dbg1("no memory for DMA buffers at %d of %d\n",
                        i, num_buffers );
            break;
        }

        /* initialize the block to a handy debugging value */
        if( 1 ) {
            memset_pixel_buffer( data, datalen, bits_per_pixel, pixel_value );
        } 
        else {
            incrementing_pixel_buffer( data, datalen, bits_per_pixel, pixel_value );
        }

        /* flush our memset data from cache into main memory */
        cpu_dcache_writeback_region( data, datalen );

        retcode = pie_rdma_add_buffer( channel, data, datalen, 
                rows_per_buffer, bytes_per_row );

        /* be lazy */
        XASSERT( retcode==0, retcode );
    }

    /* return the number of buffers we were able to allocate */
    return i;
}

static uint32_t pietest_round_up( uint32_t num, uint32_t dest )
{
    /* round 'num' to an even value of 'dest' using integer math */
    return dest * ((num+dest-1)/dest);
}

static scan_err_t pie_dma_test( scan_cmode_t cmode, 
                                uint32_t pie_pixel_size,
                                pie_pixel_t pieout )
{
    int retcode;
    int i;
    pie_pixel_t piein;
    uint32_t pie_pixels_per_row_in;
    struct pie_dma_row pie_row;
    int pie_strip_size_in;
    int pie_strip_size_out;
    int read_buffer_cnt;
    int write_buffer_cnt;
    int total_rows_per_channel_in;
    int read_rows_per_buffer;
    int rdma_rows_per_channel;
    uint32_t pixel_values[3];
    int total_rows_per_channel_out;
    int write_rows_per_buffer;
    ATLISTENTRY pie_rdma_buffers;
    ATLISTENTRY pie_wdma_buffers;
    struct ice_dma_buffer *icebuf;

    dbg2( "%s cmode=%d bpp=%d pieout=%d\n", __FUNCTION__,
                cmode, pie_pixel_size, pieout );

    XASSERT( pie_pixel_size==8 || pie_pixel_size==16, pie_pixel_size );

    pie_interrupt_disable();

    /* clear out the statistics so we can count interrupts, etc, for just this
     * PIE run
     */
    pie_clear_interrupt_stats();

    pie_soft_setup();

    /* shut off all the internal blocks. 
     *
     * TODO add NGA--Neutral Gray Adjust, if supported
     */
    pie_bde_set_bypass(true);
    pie_csc_set_bypass_all( true );
    pie_tcns_set_bypass( true );
    pie_cpa_set_bypass( true );
    pie_filter_set_bypass_te( true );
    pie_filter_set_bypass( true );
    pie_xy_set_bypass( true );

    /* davep 07-Mar-2013 ; adding CStats */
//    pie_cstats_enable(false);

//    pie_pixels_per_row_in = 1024;
    pie_pixels_per_row_in = 4992;

    if( cmode == SCAN_CMODE_COLOR) {
        /* davep 09-May-2011 ;  TODO add support for RGB, etc, input; for now
         * leave as planar 
         */
        piein = PIE_PIXEL_3_PLANES;
        num_pie_rchannels = 3;
        pie_rchannels[0] = PIE_RDMA_RED_EVEN;
        pie_rchannels[1] = PIE_RDMA_GREEN_EVEN;
        pie_rchannels[2] = PIE_RDMA_BLUE_EVEN;
    }
    else {
        piein = PIE_PIXEL_MONO;
        num_pie_rchannels = 1;
        pie_rchannels[0] = PIE_RDMA_MONO_EVEN;
    }

    memset( &pie_row, 0, sizeof(struct pie_dma_row) );
    pie_row.pixels_in = pie_pixels_per_row_in;
    pie_row.pixels_out = pie_pixels_per_row_in;

    if( pie_pixel_size == 8 ) {
        pie_row.bytes_in = pie_row.pixels_in;
    }
    else {
        XASSERT( pie_pixel_size==16, pie_pixel_size );
        pie_row.bytes_in = 2 * pie_row.pixels_in;
    }

    if( cmode==SCAN_CMODE_COLOR) {
        if( pieout==PIE_PIXEL_XRGB || pieout==PIE_PIXEL_RGBX) {
            /* 32bpp */
            pie_row.bytes_out = 4 * pie_row.pixels_out;
        }
        else if( pieout==PIE_PIXEL_RGB ) {
            pie_row.bytes_out = 3 * pie_row.pixels_out;
        }
        else if( pieout==PIE_PIXEL_3_PLANES ) {
            pie_row.bytes_out = pie_row.pixels_out;
        }
        else {
            /* wtf!? */
            XASSERT( 0, pieout );
        }
    }
    else {
        /* 8bpp */
        XASSERT( pieout==PIE_PIXEL_MONO, pieout );
        pie_row.bytes_out = pie_row.pixels_out;
    }


    /* set the input strip size to the maximum number of rows PIE ->> Write <<-
     * DMA allows (hardware limit)  (we're not scaling or using MultiFilter so
     * go biggest possible the pipe can handle)
     */
    pie_strip_size_in = PIE_WDMA_MAX_ROWS;

    /* maximum number of rows PIE Write DMA allows (hardware limit) */
    pie_strip_size_out = PIE_WDMA_MAX_ROWS;

    read_buffer_cnt = 30;
    write_buffer_cnt = read_buffer_cnt;

    /* davep 26-Apr-2010 ; Beware making this number too large or the test will
     * run out of memory and "pipe_wait no messages; wait some more".  I added
     * some code in the message loop to also recycle the buffers. If we need a
     * test with huge memory, I'll have to turn on that code again.
     */
//    total_rows_per_channel_in = pie_strip_size_in * 10;
    total_rows_per_channel_in = pie_strip_size_in * 12;

    /* round up total rows to match our strip size */
    total_rows_per_channel_in = pietest_round_up( total_rows_per_channel_in, pie_strip_size_in );

    read_rows_per_buffer = pie_strip_size_in;

    dbg2( "%s total_rows=%d num_buffers=%d\n", __FUNCTION__, total_rows_per_channel_in, read_buffer_cnt );

    rdma_rows_per_channel = total_rows_per_channel_in;

    retcode = pie_rdma_open( pie_rchannels, 
                             num_pie_rchannels, 
                             read_buffer_cnt,
                             rdma_rows_per_channel, 
                             pie_row.bytes_in );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* davep 09-May-2011 ; convert to pie_rmda_setup_config() */
    /* set up the PIE read DMA controller. */
    pie_rdma_setup_config( pie_pixel_size, cmode, piein, pie_strip_size_in );

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_enable( pie_rchannels[i] );
    }

    /* pick a pattern to fill our read buffers */
    if( cmode==SCAN_CMODE_MONO ) {
        XASSERT( num_pie_rchannels==1, num_pie_rchannels );
        pixel_values[0] = 0x4d6d; // 'Mm'
    }
    else {
        XASSERT( num_pie_rchannels==3, num_pie_rchannels );
        pixel_values[0] = 0x5272; // 'Rr'
        pixel_values[1] = 0x4767; // 'Gg'
        pixel_values[2] = 0x4262; // 'Bb'
    }

    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        retcode = pie_setup_rdma_buffers( pie_rchannels[i], 
                                          read_buffer_cnt, 
                                          read_rows_per_buffer,
                                          pie_row.pixels_in,
                                          pie_pixel_size,
                                          pixel_values[i]
                                        );

        if( retcode <= 0 ) {
            /* returns fail if we've run out of memory in the setup function */
            return SCANERR_OUT_OF_MEMORY;
        }
    }

    /* set up write */
    switch( pieout ) {
        case PIE_PIXEL_XRGB :
            XASSERT(cmode==SCAN_CMODE_COLOR, cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_xRGB;
            break;

        case PIE_PIXEL_RGBX :
            XASSERT(cmode==SCAN_CMODE_COLOR, cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RGBx;
            break;

        case PIE_PIXEL_3_PLANES :
            XASSERT(cmode==SCAN_CMODE_COLOR, cmode );
            num_pie_wchannels = 3;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RED;
            pie_wchannels[1] = PIE_WDMA_CHANNEL_GREEN;
            pie_wchannels[2] = PIE_WDMA_CHANNEL_BLUE;
            break;

        case PIE_PIXEL_MONO :
            XASSERT(cmode==SCAN_CMODE_MONO, cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_MONO;
            break;

        case PIE_PIXEL_RGB :
            XASSERT(cmode==SCAN_CMODE_COLOR, cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RGB;
            break;

        default :
            /* unknown/unsupported PIE output format */
            XASSERT( 0, pieout );
    }

    /* unless we're scaling, we'll get the same out as in */
    total_rows_per_channel_out = total_rows_per_channel_in;

    retcode = pie_wdma_open( pie_wchannels, 
                             num_pie_wchannels, 
                             write_buffer_cnt, 
                             total_rows_per_channel_out, 
                             pie_row.bytes_out );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    pie_wdma_setup_config( cmode, pieout, pie_strip_size_out, pie_row.pixels_out );

    write_rows_per_buffer = pie_strip_size_out;

    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        retcode = pie_wdma_setup_buffers( pie_wchannels[i], 
                                         write_buffer_cnt, 
                                         write_rows_per_buffer,
                                         pie_row.bytes_out );

        if( retcode <= 0 ) {
            /* returns fail if we've run out of memory in the setup function */
            return SCANERR_OUT_OF_MEMORY;
        }
    }

    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_enable( pie_wchannels[i] );
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_launch( pie_wchannels[i] );
    }

    ATInitList( &pie_rdma_buffers );
    ATInitList( &pie_wdma_buffers );

    /* fire in the hole! */
    pie_rdma_dump();
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_dump( pie_rchannels[i] );
    }
    pie_wdma_dump();
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_dump( pie_wchannels[i] );
    }
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_launch( pie_rchannels[i] );
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_launch( pie_wchannels[i] );
    }

    pie_interrupt_enable();

    pie_dma_message_loop( &pie_rdma_buffers, &pie_wdma_buffers );

    /* shut everything down */
    pie_interrupt_disable();

    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_disable( pie_rchannels[i] );
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_disable( pie_wchannels[i] );
    }

    /* put icebufs back on their lists for later counting & verification */
    while( 1 ) {
        icebuf = icebuf_pop( &pie_rdma_buffers );
        if( icebuf==NULL ) {
            break;
        }
        pie_rdma_add_ready( icebuf->channel, &icebuf );
    }

    while( 1 ) {
        icebuf = icebuf_pop( &pie_wdma_buffers );
        if( icebuf==NULL ) {
            break;
        }

        pie_wdma_add_ready( icebuf->channel, &icebuf );
    }

    pie_rdma_sanity();
    pie_wdma_sanity();

    pie_rdma_close();
    pie_wdma_close();

    return 0;
}

static scan_err_t pie_cstats_test_memory( void )
{
#ifdef HAVE_PIE_CSTATS
    uint32_t *sram;
    int i, sramsize_bytes, sramidx;
    uint32_t *ptr32;

    dbg2( "%s\n", __FUNCTION__ );

    sramsize_bytes = PIE_CSTATS_SRAM_SIZE * sizeof(uint32_t);

    pie_cstats_reset();

    sram = MEM_MALLOC( sramsize_bytes );
    if( sram==NULL ) {
        dbg1( "%s out of memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* set all bits */
    memset( sram, 0xff, sramsize_bytes );
    pie_cstats_set_sram(0,sram,PIE_CSTATS_SRAM_SIZE);
    pie_cstats_set_sram(1,sram,PIE_CSTATS_SRAM_SIZE);
    pie_cstats_set_sram(2,sram,PIE_CSTATS_SRAM_SIZE);

    /* Note sram3 is smaller than sram[012] but we're using the 'sram' array to
     * set it.
     */
    pie_cstats_set_sram(3,sram,PIE_CSTATS_NONWHITE_SRAM_SIZE);

    pie_cstats_dump();

    for( sramidx=0 ; sramidx<3 ; sramidx++ ) {
        memset( sram, 0, sramsize_bytes );
        pie_cstats_get_sram( sramidx, sram, PIE_CSTATS_SRAM_SIZE );
        ptr32 = sram;
        for( i=0 ; i<PIE_CSTATS_SRAM_SIZE ; i++ ) {
            /* 24-bit value stored as 32-bit integer */
            XASSERT( *ptr32==0x00ffffff, *ptr32 );
            ptr32++;
        }
    }
    memset( sram, 0, sramsize_bytes );
    pie_cstats_get_sram( 3, sram, PIE_CSTATS_NONWHITE_SRAM_SIZE );
    ptr32 = sram;
    for( i=0 ; i<PIE_CSTATS_NONWHITE_SRAM_SIZE ; i++ ) {
        /* 12-bit value stored as 32-bit integer */
        XASSERT( *ptr32==0x00000fff, *ptr32 );
        ptr32++;
    }

    pie_cstats_reset();

    pie_cstats_dump();

    PTR_FREE( sram );
#endif
    return SCANERR_NONE;
}

static scan_err_t pie_test_memory( void )
{
    scan_err_t scerr;

    /* poke something into each of PIE's memory spaces, make sure our addresses
     * are where we think they are
     */
    dbg2( "%s\n", __FUNCTION__ );

    scerr = 0;

    /* start in a clean state (will call pie_reset()) */
    pie_soft_setup();

    /* Sanity check some addresses, at least one register in each sub-block.
     * These registers default to non-zero.
     */
//    XASSERT( *PIE_RDMA_CCR==PIE_RDMA_CCR_R, (uint32_t)PIE_RDMA_CCR );
    XASSERT( *PIE_BDE_CFG==PIE_BDE_CFG_R, (uint32_t)PIE_BDE_CFG );
    XASSERT( *PIE_CSC_RGB_CCR==PIE_CSC_RGB_CCR_R, (uint32_t)PIE_CSC_RGB_CCR );
    XASSERT( *PIE_CSC_ESR_CCR==PIE_CSC_ESR_CCR_R, (uint32_t)PIE_CSC_ESR_CCR );
    XASSERT( *PIE_CSC_YCC_CCR==PIE_CSC_YCC_CCR_R, (uint32_t)PIE_CSC_YCC_CCR );
#ifdef HAVE_PIE_CPA
    XASSERT( *PIE_CPA_CR==PIE_CPA_CR_R, (uint32_t)PIE_CPA_CR );
#endif
    XASSERT( *PIE_TCNS_CR==PIE_TCNS_CR_R, (uint32_t)PIE_TCNS_CR );
    XASSERT( *PIE_ALIGN_PACR==PIE_ALIGN_PACR_R, (uint32_t)PIE_ALIGN_PACR );
    XASSERT( *PIE_FILTER_SCR2==PIE_FILTER_SCR2_R, (uint32_t)PIE_FILTER_SCR2 );
    XASSERT( *PIE_SCALE_CFG==PIE_SCALE_CFG_R, (uint32_t)PIE_SCALE_CFG );
//    XASSERT( *PIE_WDMA_DGCR==PIE_WDMA_DGCR_R, (uint32_t)PIE_WDMA_DGCR );

//    scerr = pie_bde_test_memory();
//    XASSERT( scerr==0, scerr );

    /* davep 07-Mar-2013 ; adding CStats */
    scerr = pie_cstats_test_memory();
    XASSERT( scerr==0, scerr );

//    scerr = pie_csc_test_memory();
//    XASSERT( scerr==0, scerr );

//    scerr = pie_tcns_test_memory();
//    XASSERT( scerr==0, scerr );

    /* cleanup after ourselves (will call pie_reset()) */
    pie_soft_setup();

    dbg2( "%s done!\n", __FUNCTION__ );

    return 0;
}


static const uint32_t pie_pixel_size_list[] = { 
    8, 16
};

static const pie_pixel_t pieout_color_list[] = {
    PIE_PIXEL_XRGB,
    PIE_PIXEL_RGBX,
    PIE_PIXEL_RGB,
//    PIE_PIXEL_3_PLANES,
};

static const pie_pixel_t pieout_mono_list[] = {
    PIE_PIXEL_MONO,
};

static scan_err_t pie_test_dma_permutations( void )
{
    scan_err_t scerr;
    scan_cmode_t cmode;
    uint32_t pie_pixel_size;
    pie_pixel_t pieout;
    int ps, po;

    /* davep 25-Aug-2010 ; XXX temp test to turn on rowshift, verify my tests
     * break 
     */
//    pie_rowshift_reset();
//    pie_rowshift_set_bypass( false );
//    pie_rowshift_dump();
//    pie_rowshift_set_bypass( true );
//    pie_rowshift_dump();
//    pie_rowshift_set_bypass( false );
//    pie_rowshift_dump();

//    while( 1 ) {
//        scerr = pie_dma_test( SCAN_CMODE_COLOR, PIE_RDMA_8BIT_PIXEL, PIE_PIXEL_3_PLANES );
//        XASSERT( scerr==0, scerr );
//    }
//    return 0;

    /* run a permutation of color DMA */
    cmode = SCAN_CMODE_COLOR;
    for( ps=0 ; ps<sizeof(pie_pixel_size_list)/sizeof(pie_pixel_size_list[0]) ; ps++ ) {
        pie_pixel_size = pie_pixel_size_list[ps];

        for( po=0 ; po<sizeof(pieout_color_list)/sizeof(pieout_color_list[0]) ; po++ ) {
            pieout = pieout_color_list[po];
        
            scerr = pie_dma_test( cmode, pie_pixel_size, pieout );
            XASSERT( scerr==0, scerr );
        }
    }

    /* run a permutation of mono DMA */
    cmode = SCAN_CMODE_MONO;
    for( ps=0 ; ps<sizeof(pie_pixel_size_list)/sizeof(pie_pixel_size_list[0]) ; ps++ ) {
        pie_pixel_size = pie_pixel_size_list[ps];

        for( po=0 ; po<sizeof(pieout_mono_list)/sizeof(pieout_mono_list[0]) ; po++ ) {
            pieout = pieout_mono_list[po];
        
            scerr = pie_dma_test( cmode, pie_pixel_size, pieout );
            XASSERT( scerr==0, scerr );
        }
    }

    return 0;
}

scan_err_t pie_run_tests( void )
{
    scan_err_t scerr;

    dbg2( "%s\n", __FUNCTION__ );

//    piedma_descrip_test();

    scerr = pie_test_memory();
    XASSERT( scerr==0, scerr );

//    scerr = pie_test_dma_permutations();
//    XASSERT( scerr==0, scerr );

    dbg1( "%s done!\n", __FUNCTION__ );
    return SCANERR_NONE;
}

void pietest_unused( void )
{
    /* don't call this function */
    XASSERT(0,0);
    while(1) {}

    /* call unused functions here to shut up the [bleep] warnings */
    pie_test_memory(); 
    pie_bde_test_memory();
    pie_cstats_test_memory();
    pie_csc_test_memory();
    pie_tcns_test_memory();
    pie_test_dma_permutations();
}

