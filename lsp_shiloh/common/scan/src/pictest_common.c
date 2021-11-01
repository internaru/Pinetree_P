/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file pictest_common.c
 *
 * \brief PIC regression tests used across multiple test source files.
 *
 * Split from pictest.c ~jul-2012
 *
 * davep 17-Jul-2012
 */

#include "scos.h"

#include "list.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "pic.h"
#include "pichw.h"
#include "pictest.h"
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
#include "picdma_descrip.h"
#endif
#include "cisx.h"

void pic_cleanup_after_test( void )
{
    /* reset PIC to its power-on reset state */
    pic_reset();

    /* re-enable PIC interrupts */
    pic_setup_interrupt();
}

void pic_setup_for_test( void )
{
    /* 3-Apr-05 ; Don't call pic_soft_setup() since it hardwires some settings
     * to connect to the CIS sensor and also does some mono/color hardwires.
     * Eventually that will be fixed. I hope.
     */
     
    /* reset PIC to its power-on reset state */
    pic_reset();

    /* re-enable PIC interrupts */
    pic_setup_interrupt();

    /* put ADC-Norm into test mode, disable Bypass, set to say we have a
     * 16-bit AFE (so we can feed in words; easiest test case), leave rest as
     * defaults 
     *
     * FIXME - shouldn't be hardwiring to 16-bit
     */
    pic_adcnorm_set_test_mode( true );
    pic_adcnorm_set_bypass( false );
    pic_adcnorm_set_abits( 16 );

    /* davep 13-Apr-2008 ; set the burst size */
#if ICE_DMA_BYTE_ALIGN==16
    pic_wdma_set_burst_size( ICE_DMA_BURST_16 );
#elif ICE_DMA_BYTE_ALIGN==32
    pic_wdma_set_burst_size( ICE_DMA_BURST_32 );
#else
#error bad ICE_DMA_BYTE_ALIGN
#endif

    /* davep 01-Apr-2010 ; default to 16-bpp. Adding this because PICDMA2005
     * defaults to 16-bpp and a lot of code is assuming 16-bpp on reset. The
     * descriptor PIC DMA defaults to 1-bpp.
     */
    pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
}

/**
 * \brief  pick pic channels (hee hee) for a test
 *
 *
 * \author David Poole
 * \date 12-Nov-2007
 *
 */

void
pictest_choose_channels( scan_cmode_t cmode , struct pic_dma_test *pictest )
{
    bool dual_scan=false;
    
//    /* davep 13-Mar-2013 ; adding dual scan */
//    dual_scan = cisx_get_dual_flag();
    /* davep 02-Apr-2013 ; TODO add run-time setting of dual scan for testing */

    if( cmode==SCAN_CMODE_MONO ) {
        if( dual_scan ) {
            pictest->channels[0] = PIC_DMA_CHANNEL_MEVEN;
            pictest->channels[1] = PIC_DMA_CHANNEL_MODD;
            pictest->num_channels = 2;
        } 
        else { 
            pictest->channels[0] = PIC_DMA_CHANNEL_MEVEN;
            pictest->num_channels = 1;
        }
    }
    else if( cmode==SCAN_CMODE_COLOR ) {
        if( dual_scan ) {
            pictest->channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
            pictest->channels[1] = PIC_DMA_CHANNEL_CODD_0;
            pictest->channels[2] = PIC_DMA_CHANNEL_CEVEN_1;
            pictest->channels[3] = PIC_DMA_CHANNEL_CODD_1;
            pictest->channels[4] = PIC_DMA_CHANNEL_CEVEN_2;
            pictest->channels[5] = PIC_DMA_CHANNEL_CODD_2;
            pictest->num_channels = 6;
        }
        else { 
            pictest->channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
            pictest->channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
            pictest->channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
            pictest->num_channels = 3;
        }
    }
    else { 
        XASSERT( 0, cmode );
    }
}

//--------------------------------------------------------------------------
// Function     : pictest_setup_dma
//   returns    : none
//   is         : pointer to pic_dma_test structure containing all the buffer
//                counts, list of DMA channels to use, etc.
// Created by   : David Poole
// Date created : 16-Apr-05
// Description  : 
//
//  Simple wrapper around all the functions necessary to set up a set of PIC
//  write DMA channels for a data run.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void 
pictest_setup_dma( struct pic_dma_test *is )
{
    int i;
    int cnt;

    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );

    pic_wdma_open( is->channels, is->num_channels, 
                             is->num_empties, is->total_rows, is->bytes_per_row );

    for( i=0 ; i<is->num_channels ; i++ ) {
        /* We can receive fewer buffers than we asked for (malloc() fails or we
         * don't have enough empties to hold all the data we're asking for) so
         * make sure we save the number of buffers we did receive so we can
         * check for leaks later.
         */
        cnt = pic_wdma_setup_buffers( is->channels[i],
                is->buffers_per_channel, is->rows_per_buffer, 
                is->bytes_per_row );

        /* davep 15-Nov-2007 ; since these tests don't recycle buffers
         * (everything goes into a list and stays there for later data
         * validation), we must get exactly the number of buffers we ask for
         */
        XASSERT( cnt==is->buffers_per_channel, cnt );

        is->num_allocated[i] = cnt;
    }

    /* turn on the channels we want */
    for( i=0 ; i<is->num_channels ; i++ ) {
        pic_wdma_channel_enable( is->channels[i] );
    }

    /* feed in the initial buffers, start DMA */
    for( i=0 ; i<is->num_channels ; i++ ) {
        pic_wdma_channel_launch( is->channels[i] );
    }

    /* davep 14-Jan-2011 ; I *still* don't want to talk about it. >:-(  */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_cwdma_late_launch();
#endif

}

