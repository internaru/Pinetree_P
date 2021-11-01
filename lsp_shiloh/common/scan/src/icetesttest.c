/**
 * \file icetesttest.c
 *
 * \brief Functions to test icetest.c
 *
 *  Yes, it's a silly name but wanted to keep with my usual "*test.c" naming
 *  convention.
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"
#include "list.h"
#include "io.h"
#include "cpu_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"
#include "icetest.h"
#include "icetesttest.h"
#include "cisx.h"
#include "pic.h"
#include "pichw.h"
#include "piccbi.h"
#include "picdma_descrip.h"
#include "lehmer.h"

#define STARTING_PIXEL_VALUE 0
//#define STARTING_PIXEL_VALUE 50000

/* davep 14-May-2012 ; shotgun debug */
#undef asp_htons
#define asp_htons(n) (n)

static void verify( uint8_t *buf, uint32_t buflen_bytes, uint32_t pixels_per_row )
{
    uint16_t *ptr16, *endptr16;
    uint16_t pixel;
    uint32_t row_counter;

    dbg2( "%s %p buflen=%d ppr=%d\n", __FUNCTION__, buf, buflen_bytes, pixels_per_row );

    ptr16 = (uint16_t *)buf;
    endptr16 = (uint16_t *)(buf + buflen_bytes );
    pixel = STARTING_PIXEL_VALUE;
    row_counter = 0;
    while( ptr16 < endptr16 ) {
        if( *ptr16!=pixel ) {
            dbg1( "%s failed! at offset=%d wanted=%d found=%d\n", 
                    __FUNCTION__, (uint8_t*)ptr16-buf, pixel, *ptr16 );
            scanlog_hex_dump( (uint8_t *)(ptr16-32), 128 );
        }
        XASSERT( *ptr16==pixel, (uint32_t)ptr16 );
        ptr16++;
        pixel++;
        row_counter++;
        if( row_counter >= pixels_per_row ) {
            pixel = STARTING_PIXEL_VALUE;
            row_counter = 0;
        }
    }
}

void icetest_test_buffer_init( scan_cmode_t cmode, uint8_t *buf, int buflen_bytes, int pixels_per_row, int num_rows )
{
    int bytes_per_row;
    uint32_t num32, *ptr32, *endptr32;
    int rcnt, pcnt, ccnt;
    uint16_t pixel;
    int color, num_colors, color_table[3];
    uint32_t sanity;
//    static int row_number=0; /* stamp a row number into the buffer */

    dbg2( "%s cmode=%d buf=%p ppr=%d nr=%d\n", __FUNCTION__, cmode, buf, pixels_per_row, num_rows );

    /* stupid human checks */
    XASSERT( num_rows>0 && num_rows<4000, num_rows );
    XASSERT( pixels_per_row>0, pixels_per_row );

    bytes_per_row = pixels_per_row * sizeof(uint32_t);
    XASSERT( buflen_bytes==bytes_per_row * num_rows, buflen_bytes );

    /* make sure everything is DMA aligned */
    XASSERT( ICE_DMA_IS_ALIGNED((uint32_t)buf), (uint32_t)buf );
//    XASSERT( ICE_DMA_IS_ALIGNED(bytes_per_row), bytes_per_row );
//    XASSERT( ICE_DMA_IS_ALIGNED(buflen_bytes), buflen_bytes );

    ptr32 = (uint32_t *)buf;
    endptr32 = (uint32_t *)(buf + buflen_bytes);

    /* hardwire to mono for now */
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
            pixel++;\
    } while(0);

    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<num_rows ; rcnt++ ) {

        /* send in planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            /* want an entire line of data to be pixels values from
             * 0..pixels_per_row so we can tell exactly where the margins are
             * chopping us
             */
//            pixel = row_number++;
            pixel = STARTING_PIXEL_VALUE;
//            pixel = (uint16_t)(lehmer_frandom() * 0xffff);

            color = color_table[ccnt];

            /* send in a starting pixel */
            num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_FIRP) 
                    | PIC_ADCN_ANT_COLOR(color) 
                    | PIC_ADCN_ANT_DATA(asp_htons(pixel))
                    ;
            PUSH_PIXEL;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<pixels_per_row-2 ; pcnt++ ) {
                num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_NORMP) 
                      | PIC_ADCN_ANT_COLOR( color ) 
                      | PIC_ADCN_ANT_DATA( asp_htons(pixel) )
                      ;
                PUSH_PIXEL;
            }

            /* send in a last pixel */
            num32 = PIC_ADCN_ANT_DATATYPE(PIC_ADCN_TEST_DT_LASTP) 
                  | PIC_ADCN_ANT_COLOR( color ) 
                  | PIC_ADCN_ANT_DATA( asp_htons(pixel) )
                  ;
            PUSH_PIXEL;

        } /* end for ccnt */

    } /* end for rcnt */

    dbg2( "%s sanity=%d ptr=%p endptr=%p\n", __FUNCTION__, sanity, ptr32, endptr32 );

    XASSERT( sanity==pixels_per_row*num_rows*num_colors, sanity );

    XASSERT( ptr32==endptr32, (uint32_t)ptr32 );

    cpu_dcache_writeback_region( buf, buflen_bytes );
}

static scan_err_t icetest_simple( void )
{
    scan_err_t scerr, final_scerr;
    struct ddma_descriptor *icetest_desc, *pic_desc;
    struct dma_ptr icetest_buf, pic_buf;
//    uint8_t *icetest_buf, *pic_buf.ptr;
//    dma_addr_t icetest_dma_handle, pic_dma_handle;
//    int icetest_buf_size, pic_buf_size;
    int icetest_bytes_per_row, pic_bytes_per_row;
    int bits_per_pixel, pixels_per_row, num_rows;
    struct pic_cwdma_regs *pic_wdma_regs;
    uint32_t pic_delay_count;
    uint32_t pic_cwdma_status;

    dbg2( "%s\n", __FUNCTION__ );

    icetest_interrupt_disable();
    pic_interrupt_disable();

    scerr = icetest_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    pic_reset();
    scerr = pic_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 01-Feb-2013 ; pic_soft_setup() re-enables PIC interrupt and we
     * need the interrupt disabled in this test function (we poll on status
     * bits rather than let pic.c handle the interrupt)
     */
    pic_interrupt_disable();

    icetest_enable( false );
//    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_DISCARD);
    pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
    icetest_set_test_mode( ICETEST_CONFIG_DATA_TO_CISX );
#ifdef HAVE_CISX
    cisx_set_bypass( true );
#endif

    memset( &icetest_buf, 0, sizeof(struct dma_ptr) );
    memset( &pic_buf, 0, sizeof(struct dma_ptr) );

    /* allocate dma descriptors */
    icetest_desc = NULL;
    pic_desc = NULL;

    scerr = ddma_descriptor_alloc( &icetest_desc );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto leave;
    }

    scerr = ddma_descriptor_alloc( &pic_desc );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto leave;
    }

    /* set up source and destination memory */
    pixels_per_row = 1024;
    num_rows = 12;
    bits_per_pixel = 16;

    /* icetest input is a 32-bit value containing CBI + pixel */
    icetest_bytes_per_row = pixels_per_row * sizeof(uint32_t);

    /* pic output is simple a 16-bit pixel */
    pic_bytes_per_row = pixels_per_row * (bits_per_pixel/8);

    icetest_buf.size = icetest_bytes_per_row * num_rows;
#ifdef __KERNEL__
    icetest_buf.ptr = dma_alloc_coherent( NULL, icetest_buf.size, &icetest_buf.dma_handle, GFP_DMA );
#else
    icetest_buf.ptr = MEM_MALLOC_ALIGN( icetest_buf.size, e_32_byte );
    icetest_buf.dma_handle = icetest_buf.ptr;
#endif
    dbg2( "%s icetest_buf=%p numbytes=%d\n", __FUNCTION__, icetest_buf.ptr, icetest_buf.size );
    if( icetest_buf.ptr==NULL ) {
        final_scerr = SCANERR_OUT_OF_MEMORY;
        goto leave;
    }
    memset( icetest_buf.ptr, 0, icetest_buf.size );
    icetest_test_buffer_init( SCAN_CMODE_MONO, icetest_buf.ptr, icetest_buf.size, pixels_per_row, num_rows );
    scanlog_hex_dump( icetest_buf.ptr, 64 );
    cpu_dcache_writeback_region( icetest_buf.ptr, icetest_buf.size );

    pic_buf.size = pic_bytes_per_row * num_rows;
#ifdef __KERNEL__
    pic_buf.ptr = dma_alloc_coherent( NULL, pic_buf.size, &pic_buf.dma_handle, GFP_DMA );
#else
    pic_buf.ptr = MEM_MALLOC_ALIGN( pic_buf.size, e_32_byte );
    pic_buf.dma_handle = pic_buf.ptr;
#endif
    dbg2( "%s pic_buf.ptr=%p numbytes=%d\n", __FUNCTION__, pic_buf.ptr, pic_buf.size );
    if( pic_buf.ptr==NULL ) {
        final_scerr = SCANERR_OUT_OF_MEMORY;
        goto leave;
    }
    memset( pic_buf.ptr, 0xee, pic_buf.size );
    scanlog_hex_dump( pic_buf.ptr, 64 );
    cpu_dcache_writeback_region( pic_buf.ptr, pic_buf.size );

    /* set up simple icetest input and pic output dma descriptors */
    icetest_desc->config_flags = DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK
                       | DDMA_DESCRIPTOR_CONFIG_STOP 
                       | DDMA_DESCRIPTOR_CONFIG_INT  
                       ;
    icetest_desc->src_addr = (uint32_t)icetest_buf.dma_handle;
    icetest_desc->fw_src_addr = (void *)icetest_desc->src_addr;
    icetest_desc->transfer_len_bytes = icetest_buf.size;

    pic_desc->config_flags = DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK
                       | DDMA_DESCRIPTOR_CONFIG_STOP 
                       | DDMA_DESCRIPTOR_CONFIG_INT  
                       ;
    pic_desc->src_addr = (uint32_t)pic_buf.dma_handle;
    pic_desc->fw_src_addr = (void *)pic_desc->src_addr;
    pic_desc->transfer_len_bytes = pic_buf.size;

    /* start turning everything on */
//    pic_interrupt_enable();
//    icetest_interrupt_enable();
    icetest_enable( true );
    icetest_idma_enable();
    pic_wdma_channel_enable( 0 );

    /* fire in the hole! */
    pic_ddma_desc_write( 0, pic_desc, pic_bytes_per_row );
    icetest_idma_desc_write( icetest_desc, icetest_bytes_per_row );

    /* blocking wait for FIN bit in interrupt status to go "ping!" */
    icetest_wait_for_fin();

    pic_wdma_regs = pic_cwdma_get_regs(0);

    pic_delay_count = 0;
    do {
        cpu_spin_delay(100);

        pic_delay_count += 1;
        /* if we're stuck this long, we're truly hosed */
        XASSERT( pic_delay_count<100, pic_delay_count );

    } while( !(pic_wdma_regs->int_pend & PIC_WDMA_INT_PEND_FIN ) );

    pic_cwdma_status=0;
    pic_cwdma_status=pic_cwdma_status_get(0);
    XASSERT( !(pic_cwdma_status & PIC_WDMA_STATUS_DMA_BUSY), pic_cwdma_status );

    icetest_dump();

    cpu_dcache_invalidate_region( pic_buf.ptr, pic_buf.size );
    scanlog_hex_dump( pic_buf.ptr, 64 );

    verify( pic_buf.ptr, pic_buf.size, pixels_per_row );

//    dbg1( "%s icetest=%d pic=%d\n", __FUNCTION__, icetest_delay_count, pic_delay_count );

    dbg1( "%s done!\n", __FUNCTION__ );

    /* done; clean up */
    final_scerr = SCANERR_NONE;

leave:
    /* clean up */
    pic_interrupt_disable();
    icetest_interrupt_disable();
    icetest_enable( false );
    icetest_idma_disable();
    icetest_reset();
    pic_soft_setup();

    if( icetest_desc ) {
        ddma_descriptor_free( &icetest_desc );
    }
    if( pic_desc ) {
        ddma_descriptor_free( &pic_desc );
    }
    if( icetest_buf.ptr ) {
#ifdef __KERNEL__
        dma_free_coherent( NULL, icetest_buf.size, icetest_buf.ptr, icetest_buf.dma_handle );
#else
        PTR_FREE( icetest_buf.ptr );
#endif
    }
    if( pic_buf.ptr ) {
#ifdef __KERNEL__
        dma_free_coherent( NULL, pic_buf.size, pic_buf.ptr, pic_buf.dma_handle );
#else
        PTR_FREE( pic_buf.ptr );
#endif
    }

    return final_scerr;
}

scan_err_t icetest_test( void )
{
    scan_err_t scerr;

    dbg2( "%s\n", __FUNCTION__ );

    scerr = icetest_simple();

    return SCANERR_NONE;
}

