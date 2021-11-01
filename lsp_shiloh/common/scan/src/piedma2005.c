/*
 * ============================================================================
 * Copyright (c) 2007, 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/** 
 * \file piedma.c
 *
 * \brief PIE DMA functions
 */

#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "list.h"           
#include "lassert.h"
#include "debug.h"
#include "interrupt_api.h"
#include "memAPI.h"
#include "hwconfig_api.h"
#include "cpu_api.h"
#include "regAddrs.h"

#include "PIE_RDMA_ICE_regstructs.h"

/* need to see what my friends see to make sure the definitions are consistent */
#define PIEDMA_FRIEND 

#include "scantypes.h"
#include "scancore.h"
#include "scantask.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "piehw.h"
#include "pie.h"
#include "piereset.h"

//#define PIE_DMA_ISR_DEBUG
//#define PIE_DMA_DEBUG


static volatile PIE_RDMA_ICE_REGS_t * const pie_rdma_common_regs = (volatile PIE_RDMA_ICE_REGS_t*)(ICE_PIE_PIE_RDMA_BASE);

/* 5-Mar-05 ; increment this when we wait for the ready bit to be set in the
 * PIE Read DMA Status Register; if PIE gets stuck somewhere somehow, watch for
 * this counter rolling. It should almost never be non-zero.
 */
uint32_t pie_rdma_dsr_wait;

/* 6-Mar-05 ; same as pie_rdma_dsr_wait 
 */
uint32_t pie_wdma_dsr_wait;

/* 10-Mar-05 ; waits for channel Active bits */
uint32_t pie_rdma_channel_active_wait[PIE_RDMA_NUM_CHANNELS];
 
/* wait this many delays (currenting 1us) until deciding read/write DMA is
 * hosed 
 */
#define MAX_PIE_DMA_WAIT 10

/*******************************/
/* 8-Apr-05; new style ICE DMA */

static struct ice_dma_driver pie_rdma_driver = { 
    .msg_data = SMSG_PIE_RDMA_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      pie_rdma_channel_reset,
    .enable =     pie_rdma_channel_enable,
    .disable =    pie_rdma_channel_disable,
    .load =       pie_rdma_channel_load, 
    .start =      pie_rdma_channel_start,
    .is_enabled = pie_rdma_channel_is_enabled,
    .icebuf_isr = pie_rdma_isr
};

static struct ice_dma_driver pie_wdma_driver = { 
    .msg_data = SMSG_PIE_WDMA_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      pie_wdma_channel_reset,
    .enable =     pie_wdma_channel_enable,
    .disable =    pie_wdma_channel_disable,
    .load =       pie_wdma_channel_load, 
    .start =      pie_wdma_channel_start,
    .is_enabled = pie_wdma_channel_is_enabled,
    .icebuf_isr = pie_wdma_isr
};

#ifdef HAVE_STAGGERED_SENSOR_SUPPORT
    #define PIE_RDMA_VALID_CHANNEL_MASK 0x3f /* b'0011 1111 */
#else 
    #if PIE_RDMA_NUM_CHANNELS==6
        /* davep 23-Mar-2010 ; adding support non-staggered PIE (we have the same
         * number of channels but the formerly odd channels are simply dead)
         */
        #define PIE_RDMA_VALID_CHANNEL_MASK 0x15 /* b'10101 */
    #else
        /* davep 22-Mar-2011 ; yet another change; now the channels have collapsed */
        #define PIE_RDMA_VALID_CHANNEL_MASK 0x07 /* b'00111 */
    #endif
#endif

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 23-Mar-2010
 *
 */

bool pie_rdma_channel_is_valid( uint8_t channel ) 
{
    /* Check if this channel number is valid on this hardware. Valid channels
     * have a bit set in the conditionally compiled mask.
     */
    return ( ((1<<channel) & (~PIE_RDMA_VALID_CHANNEL_MASK))==0 );
}

//--------------------------------------------------------------------------
// Function     : pie_dma_interrupt
//   returns    : none
//   ipend      : "interrupt pending", value of PIE_CI_IPR register
// Created by   : David Poole
// Date created : ~6-Mar-05
// Description  : 
//
//  Called from pie_interrupt() to handle just the DMA interrupts.
//
//   - Pops the top data buffer from the queue that mirrors our DMA registers.
//   - Puts the ice_dma_buffer into a message, sends the message to the Scan
//     task for handling.
//   - if there is still data to read, pushes a new buffer onto the queue
//   - if cannot get a new buffer from the readies list, sends an out of memory
//     message to the Scan task.
//
// Notes        : 
//
//  27-Mar-05 davep ; While adding the messaging and buffer dance code
//  (original code simply ack'd the interrupt) I copied a lot of the internals
//  of this function, including the comments, from the original
//  pic_interrupt().
//  
//--------------------------------------------------------------------------
extern int dbg_cnt_pie_dma_interrupt;	//add.lsh.for pipe debug
void pie_dma_interrupt( uint32_t ipend )
{
    int i;
    struct pie_interrupt_stats *piestats;

    /* FIXME - use ipend instead of reading the status register */

    /* FIXME - there are potentially two transactions queued; I'm currently
     * only checking for one before leaving. Need to grab both if two are
     * asserted.
     */
	dbg_cnt_pie_dma_interrupt++;
    piestats = get_pie_interrupt_stats();

    /* check for pending read DMA interrupts */
    for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {

        /* davep 23-Mar-2010 ; skip dead channels */
        if( !(pie_rdma_channel_is_valid(i))) {
            continue;
        }

        if( !( *PIE_RDMA_DSR(i) & PIE_RDMA_DSR_CMPL) ) {
            /* nothing on this channel, check next */
            continue;
        }

        /* read the results before acking the interrupt */
        piestats->rdma_last_drr[i] = *PIE_RDMA_DRR(i);

        /* 29-Mar-05 ; moved ack to top of loop ; see 29-Mar-05 comment in the
         * WDMA loop below
         */

        /* clear it by writing zero to bit zero */
        *PIE_RDMA_DIR(i) &= ~1;

        /* increment global statistics */
        piestats->rdma[i]++;

        /* FIXME - get rid of this when interrupt is finally working. Will slow
         * things way down otherwise.
         */
#ifdef PIE_DMA_ISR_DEBUG
        dbg1("%s rdma channel %d\n", __FUNCTION__, i);
#endif
        if( pie_rdma_driver.icebuf_isr ) {
            pie_rdma_driver.icebuf_isr( i );
        }
    }

    /* check for pending write DMA interrupts */
    for( i=0 ; i<PIE_WDMA_NUM_CHANNELS ; i++ ) {
        if( !( *PIE_WDMA_DSR(i) & PIE_WDMA_DSR_CMPL) ) {
            /* nothing on this channel, check next */
            continue;
        }

//        dbg1("%d 0x%x\n", i, *PIE_WDMA_DSR(i)));

        /* read the results register before acking the interrupt  */
        piestats->wdma_last_drr[i] = *PIE_WDMA_DRR(i);

        /* 29-Mar-05 ; moved the ack to the top of the check since there seems
         * to be something weird in the Write DMA that causes the hardware to
         * lurch to a halt. I load in a new transaction, start it, and the
         * start hangs. RandyS thinks the queue available bit should clear in
         * this case so he's verifying it. Read DMA has the same weirdness.
         */
        /* clear it by writing zero to bit */
        *PIE_WDMA_DIR(i) &= ~PIE_WDMA_DIR_CLEAR;

        /* increment global statistics */
        piestats->wdma[i]++;

        /* FIXME - get rid of this when interrupt is finally working. Will slow
         * things way down otherwise.
         */
#ifdef PIE_DMA_ISR_DEBUG
        dbg1("%s wdma channel %d\n", __FUNCTION__, i);
#endif
        if( pie_wdma_driver.icebuf_isr ) {
            pie_wdma_driver.icebuf_isr( i );
        }
    }

#ifdef PIE_DMA_ISR_DEBUG
    dbg2( "%s done\n", __FUNCTION__ );
#endif
}

/**
 * \brief  Set the PIE RDMA and WDMA burst size.
 *
 * This feature was added in 2007 so the earlier ASICs don't have it.
 *
 * The PIE DMA burst sizes are selectable between 16 and 32-byte bursts.  Note I'm
 * setting both the read and the write simultaneously. 
 *
 * \author David Poole
 * \date 13-Apr-2008
 *
 */

void pie_dma_set_burst_size( ice_dma_burst_size burst_size )
{
#if HAVE_ICE_DMA_SET_BURST_SIZE
    int i;

    if( burst_size==ICE_DMA_BURST_16 ) {
        /* set to 16-byte bursts by clearing the config bits */
        for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {
            if( pie_rdma_channel_is_valid(i) ) {
                *PIE_RDMA_DCR(i) &= ~PIE_RDMA_DCR_BURST_32;
            }
        }

        for( i=0 ; i<PIE_WDMA_NUM_CHANNELS ; i++ ) {
            *PIE_WDMA_DCR(i) &= ~PIE_WDMA_DCR_BURST_32;
        }
    }
    else if( burst_size==ICE_DMA_BURST_32 ) {
        /* set to 32-byte bursts by setting the config bits */
        for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {
            if( pie_rdma_channel_is_valid(i) ) {
                *PIE_RDMA_DCR(i) |= PIE_RDMA_DCR_BURST_32;
            }
        }

        for( i=0 ; i<PIE_WDMA_NUM_CHANNELS ; i++ ) {
            *PIE_WDMA_DCR(i) |= PIE_WDMA_DCR_BURST_32;
        }
    }
    else {
        /* whozawhatnow? */
        XASSERT(0,(uint32_t)burst_size);
    }
#endif
}

/* ------------------------------------------
 *
 *  Read DMA Functions
 *
 * ------------------------------------------
 */

void pie_rdma_setup_config( uint8_t pie_bpp, scan_cmode_t cmode, pie_pixel_t piein, uint32_t strip_size_in )
{
    uint32_t num32;
    uint32_t ccr_pixel_size;

    /* davep 16-May-2011 ; added piein as part of the shift to the
     * piedma_descrip.c however we can only support planar and mono in this
     * hardware version
     */
    XASSERT( piein==PIE_PIXEL_3_PLANES || piein==PIE_PIXEL_MONO, piein );

    switch( pie_bpp ) {
        case 8 :
            ccr_pixel_size = PIE_RDMA_CCR_8BIT_PIXEL;
            break;

        case 16 :
            ccr_pixel_size = PIE_RDMA_CCR_16BIT_PIXEL;
            break;

        default : 
            XASSERT( 0, pie_bpp );
            /* quiet a warning */
            ccr_pixel_size = 42;
            break;
    }


    XASSERT( strip_size_in <= PIE_RDMA_MAX_ROWS, strip_size_in );

#ifdef PIE_DMA_DEBUG
    dbg2("%s pixel_size=%ld rows=%ld \n", __FUNCTION__, pixel_size, strip_size_in );
#endif

    /* First set up the PIE read DMA controller.
     *
     * Stop/soft reset PIE Read DMA. Because we are required to reset read DMA
     * while changing the row count, we have a function.
     */ 
    *PIE_RDMA_CSR &= ~PIE_RDMA_CSR_START;
    
    num32 = 0;

    if( cmode==SCAN_CMODE_COLOR ) {
        num32 |= PIE_RDMA_CCR_COLOR;
        XASSERT( piein==PIE_PIXEL_3_PLANES, piein );
    }
    else {
        num32 |= PIE_RDMA_CCR_MONO;
        XASSERT( piein==PIE_PIXEL_MONO, piein );
    }

    /* Note that 'rows' is a -1 field. */
    num32 |= ccr_pixel_size
          | PIE_RDMA_CCR_ROWS(strip_size_in-1)
          ;
    *PIE_RDMA_CCR = num32;

    /* restart the PIE Read DMA block */
    *PIE_RDMA_CSR |= PIE_RDMA_CSR_START;
}

void pie_rdma_reset( void )
{
    int i;
    uint32_t num32;

    /* reset PIE Read DMA block */
    *PIE_RDMA_CSR &= ~PIE_RDMA_CSR_START;
    /* XXX - I haven't yet learned how long to "hold" the reset on these
     * things. Is 1 microsecond too long? too short? 
     */
    cpu_spin_delay(1);

    /* start PIE Read DMA */
    *PIE_RDMA_CSR |= PIE_RDMA_CSR_START;

    /* set other main registers to power-on defaults just to make sure */
    *PIE_RDMA_CCR = PIE_RDMA_CCR_R;

    /* 5-Feb-05 ; XXX - do I want to trust this? Make sure my channel addresses
     * are correct. According to the read DMA datasheet, DSTR (DMA Start
     * Register) and DIR (DMA Interrupt Register) are write-only and will
     * return 0xdeaddead on read.
     */
    /* 7-Feb-05 ; Randy Schmidt, the designer of the read/write DMA blocks,
     * tells me he doesn't plan on changing this so leave as-is for now.
     */
    for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {

        if( !pie_rdma_channel_is_valid( i ) ) {
            /* skip dead hardware channels */
            continue;
        }

        /* davep 12-Aug-2007 ; can't read or write the start register without
         * triggering a start; my fake PIE hardware isn't smart enough to tell
         * the difference
         */
#ifndef PLAT_CHIMERA
        num32 = *PIE_RDMA_DSTR(i);
        XASSERT( num32 == 0xdeaddeadUL, num32 );
#endif
        num32 = *PIE_RDMA_DIR(i);
        XASSERT( num32 == 0xdeaddeadUL, num32 );

        /* soft reset the pie read dma channel */
        /* 24-Mar-05 ; During some PIE DMA debugging, RandyS and JohnM tell me
         * it is unnecessary and kind of silly to reset each of the channels
         * right after doing a block-level reset.
         */
        /* 1-May-05 ; (is it May already!?) put the channel reset back in so we
         * can set all the registers to a clean state at the same time
         */
        pie_rdma_channel_reset( i );
    }
}

/**
 * \brief  Return true if all of PIE WDMA is idle
 *
 * Created to assist multiplex PIE DMAs
 *
 * \author David Poole
 * \date 28-May-2013
 */

bool pie_rdma_is_idle( void )
{
    int channel;
    bool is_idle;
    uint32_t dsr;

    is_idle = true;

    dsr = ~0;
    for( channel=0 ; channel<PIE_RDMA_NUM_CHANNELS ; channel++ ) {
        if( !pie_rdma_channel_is_enabled( channel ) ) {
            /* skip currently unused hardware channels */
            continue;
        }
        /* want READY==1 ACTIVE==0 INTPEND==0 */
        dsr = *PIE_RDMA_DSR(channel);
        if( !(dsr & PIE_RDMA_DSR_READY) ||  /* ready bit clear */
            (dsr & PIE_RDMA_DSR_ACTIVE) ||  /* active bit set */
            (dsr & PIE_RDMA_DSR_CMPL) ) {   /* interupt pending bit set */
            is_idle = false;
            break;
        }
    }

//    dbg2( "%s dsr=0x%x %d\n", __FUNCTION__, dsr, is_idle );
    return is_idle;
}

void pie_rdma_channel_reset( uint8_t channel )
{
#ifdef PIE_DMA_DEBUG
    dbg2("%s(%d)\n", __FUNCTION__, channel);
#endif

    if( !pie_rdma_channel_is_valid(channel) ) {
        /* gently ignore because called from icedma */
        return;
    }
    
    /* reset channel then wait for ready */
    *PIE_RDMA_DCR(channel) = PIE_RDMA_DCR_RESET;

    /* XXX - datasheet says "assert this signal for at least one cycle and then
     * deassert it" ; is 1us too long?
     */
    cpu_spin_delay(1);

    /* 10-Mar-05 ; XXX should Ready go low while DMA channel is held in reset?
     * As of this writing the following test fails. Should it work?
     */
//    num32 = *PIE_RDMA_DSR(channel) & PIE_RDMA_DSR_READY;
//    XASSERT( !(num32 & PIE_RDMA_DSR_READY), num32 );

    /* 1-May-05; set all registers to a clean power-on state */
    *PIE_RDMA_DAR1(channel) = PIE_RDMA_DAR1_R;
    *PIE_RDMA_DAR2(channel) = PIE_RDMA_DAR2_R;
    *PIE_RDMA_DRCR1(channel) = PIE_RDMA_DRCR1_R;
    *PIE_RDMA_DRCR2(channel) = PIE_RDMA_DRCR2_R;
    *PIE_RDMA_DBCR(channel) = PIE_RDMA_DBCR_R;
    *PIE_RDMA_DTCR(channel) = PIE_RDMA_DTCR_R;
    /* don't change Start (write only; the firmware can't get confused by any
     * leftover weird value); don't set Status since only has Active and Ready,
     * which are owned by the controller, and interrupt clear which should be
     * cleared anyway.
     */
    
    /* clear the reset bit (leaves both the channel and the channel's interrupt
     * disabled until explictly enabled)
     */
    *PIE_RDMA_DCR(channel) = 0;

    /* wait for the ready bit */
    pie_rdma_dsr_wait=0;
    while( !(*PIE_RDMA_DSR(channel) & PIE_RDMA_DSR_READY) ) {

        /* FIXME - infinite loop */
        pie_rdma_dsr_wait++;

        cpu_spin_delay( 1 ); /* wait 1 microsecond */

        XASSERT( pie_rdma_dsr_wait < MAX_PIE_DMA_WAIT, pie_rdma_dsr_wait );
    }
    pie_rdma_dsr_wait=0;

}

void pie_rdma_channel_start( uint8_t channel )
{
    /* brutally fail if someone calls with an invalid channel (should not be
     * called unless setup is wrong)
     */
    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

#ifdef PIE_DMA_DEBUG
    dbg2("%s %d\n", __FUNCTION__, channel );
#endif
    /* start DMA by writing anything to the start register */
    *PIE_RDMA_DSTR(channel) = 1;
}

void pie_rdma_channel_enable( uint8_t channel )
{
    /* brutally fail if someone calls with an invalid channel (should not be
     * called unless setup is wrong)
     */
    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

#ifdef PIE_DMA_DEBUG
    dbg2("%s %d\n", __FUNCTION__, channel );
#endif

    /* FIXME - datasheet says to set _EN only when the "Active bit in the DMA
     * Status Register is deasserted". 
     *
     * I'm currently blindly setting it and hoping for the best.
     */
    /* 10-Mar-05 ; add in the wait for Active bit in the hopes that will help
     * some of our bizarre DMA problems.
     */

    pie_rdma_channel_active_wait[channel] = 0;
    while( (*PIE_RDMA_DSR(channel) & PIE_RDMA_DSR_ACTIVE) ) {

        pie_rdma_channel_active_wait[channel]++;

        cpu_spin_delay( 1 ); /* wait 1 microsecond */

        XASSERT( pie_rdma_channel_active_wait[channel]++ < MAX_PIE_DMA_WAIT, 
                pie_rdma_channel_active_wait[channel]++ );
    }
    pie_rdma_channel_active_wait[channel] = 0;

    /* enable DMA, enable DMA interrupt */
    *PIE_RDMA_DCR(channel) = PIE_RDMA_DCR_EN
                           | PIE_RDMA_DCR_CIE; 
}

bool pie_rdma_channel_is_enabled( uint8_t channel )
{
    uint32_t num32;

    if( !pie_rdma_channel_is_valid(channel) ) {
        /* gently ignore because called from icedma */
        return false;
    }
    
//    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

    /* check for enabled DMA, enabled DMA interrupt */
    num32 = *PIE_RDMA_DCR(channel); 

    return (num32 & (PIE_RDMA_DCR_EN | PIE_RDMA_DCR_CIE)) && true; 
}

void pie_rdma_channel_disable( uint8_t channel )
{
    dbg2("%s %d\n", __FUNCTION__, channel );

    if( !pie_rdma_channel_is_valid(channel) ) {
        /* gently ignore since called from icedma */
        return;
    }
    
//    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

#ifdef PIE_DMA_DEBUG
    dbg2("%s %d\n", __FUNCTION__, channel );
#endif

    /* disable DMA, disable DMA interrupt */
    *PIE_RDMA_DCR(channel) &= ~(PIE_RDMA_DCR_EN | PIE_RDMA_DCR_CIE); 
}

void pie_rdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t rows, uint32_t bytes_per_row )
{
    /* brutally fail if someone calls with an invalid channel (should not be
     * called unless setup is wrong)
     */
    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

    /* make sure pointer is correctly burst aligned */
    XASSERT( ((uint32_t)dma_dest & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, (uint32_t)dma_dest );

    /* make sure we're up in the cache bypass address area */
    XASSERT( IS_DMAPTR_VALID((uint32_t)dma_dest), (uint32_t)dma_dest );

    /* 31-Mar-05 ; as of this writing, PIE needs to recieve exactly the same
     * number of rows each time (limit of the firmware and of my understanding
     * of how PIE works). We will hit this assert if we have a case where
     * rows is not the same as the row count programmed into the overall PIE
     * read DMA block control register.
     */
    /* 8-Apr-05 ; remember PIE_RDMA_CCR is a -1 register */
    XASSERT( PIE_RDMA_CCR_GET_ROWS(*PIE_RDMA_CCR)==rows-1, rows );

    /* 17-Mar-05 ; We can only transfer in 16-byte bursts so make sure the
     * bytes_per_row is exactly burst sized (caller's responsibility). 
     */
    /* 25-Mar-05 ; looking at the PIE Read DMA MA doc, it actually requires
     * word aligned, not quadword aligned. However, since these buffers need to
     * also flow to other DMA parts of the system which *are* quadword aligned,
     * take the more conservative approach.
     */
    /* 11-Apr-05 ; yeah, but we still have to transfer in 16-byte chunks. Not
     * sure what I was thinking in that previous comment.
     */
    XASSERT( (bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK)==0, bytes_per_row );

#ifdef PIE_DMA_DEBUG 
    dbg2("pierdma load %d 0x%x %ld %ld\n", 
                    channel, dma_dest, rows, bytes_per_row);
#endif

    /* wait for read DMA to become available */
    pie_rdma_dsr_wait=0;
    while( !(*PIE_RDMA_DSR(channel) & PIE_RDMA_DSR_READY) ) {
        /* FIXME - infinite loop */
        pie_rdma_dsr_wait++;

        /* Wait for a "big" chunk of counts before deciding we're hosed.  Need
         * to plan the firmware well enough that we never block waiting for
         * DMA.
         */
        cpu_spin_delay( 1 ); /* wait 1 microsecond */
        XASSERT( pie_rdma_dsr_wait < MAX_PIE_DMA_WAIT, pie_rdma_dsr_wait );
    }
    pie_rdma_dsr_wait=0;

    /* load the pointer to the DMA destination */
    *PIE_RDMA_DAR1(channel) = (uint32_t)dma_dest;

    /* "number of rows to to be stored to main memory */
    /* 7-Mar-05 ; this might be a -1 register */
    /* 25-Mar-05 ; it's not. */
    *PIE_RDMA_DRCR1(channel) = rows;

    /* number of bytes of data that are in each row */
    *PIE_RDMA_DBCR(channel) = bytes_per_row;

    /* set the transfer count; this is a +1 register (counts 0..max-1) so
     * subtract one 
     * XXX - is this calculation correct?
     */
    /* 7-Mar-05 ; I got an email this morning from Randy Schmidt, the owner of
     * the PIE read/write DMA blocks, saying this byte count is indeed the same
     * as bytes_per_row * row_count and that it's simply a convenience for
     * the DMA blocks.
     */
    *PIE_RDMA_DTCR(channel) = rows * bytes_per_row - 1;

    /* XXX - get rid of this debugging statement once DMA is working; it slows
     * us down too much
     */
#ifdef PIE_DMA_DEBUG
//    dbg3( ("%s DAR1=0x%x DRCR1=0x%x DBCR=0x%x DTCR=0x%x\n",
//                __FUNCTION__,
//                *PIE_RDMA_DAR1(channel), 
//                *PIE_RDMA_DRCR1(channel),
//                *PIE_RDMA_DBCR(channel),
//                *PIE_RDMA_DTCR(channel) ) );
#endif

}

void pie_rdma_channel_load2( uint8_t channel, uint32_t dar1, uint32_t drcr1, 
                        uint32_t dar2, uint32_t drcr2, uint32_t bytes_per_row )
{
    int rows;
    
    /* 9-June-05 ; copied this function originally from the regular _load
     * function. Added stuff for the second DMA pointer.
     */

#ifdef PIE_DMA_DEBUG 
    dbg2("pierdma load2 channel=%d dar1=0x%x drcr1=%ld dar2=0x%x drcr2=%ld bpr=%ld\n", 
                    channel, dar1, drcr1, dar2, drcr2, bytes_per_row);
#endif
    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );

    /* make sure pointers are correctly burst (aligned */
    XASSERT( (dar1 & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, dar1 );
    XASSERT( (dar2 & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, dar2 );

    /* make sure we're up in the cache bypass address area */
    XASSERT( IS_DMAPTR_VALID(dar1), dar1);
    XASSERT( dar2==0 || IS_DMAPTR_VALID(dar2), dar2 );

    /* 31-Mar-05 ; as of this writing, PIE needs to recieve exactly the same
     * number of rows each time (limit of the firmware and of my understanding
     * of how PIE works). We will hit this assert if we have a case where
     * rows is not the same as the row count programmed into the overall PIE
     * read DMA block control register.
     */
    /* 8-Apr-05 ; remember PIE_RDMA_CCR is a -1 register */
    rows = drcr1 + drcr2;
    XASSERT( PIE_RDMA_CCR_GET_ROWS(*PIE_RDMA_CCR)==rows-1, rows );

    /* 17-Mar-05 ; We can only transfer in 16-byte bursts so make sure the
     * bytes_per_row is exactly burst sized (caller's responsibility). 
     */
    /* 25-Mar-05 ; looking at the PIE Read DMA MA doc, it actually requires
     * word aligned, not quadword aligned. However, since these buffers need to
     * also flow to other DMA parts of the system which *are* quadword aligned,
     * take the more conservative approach.
     */
    /* 11-Apr-05 ; yeah, but we still have to transfer in 16-byte chunks. Not
     * sure what I was thinking in that previous comment.
     */
    XASSERT( (bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK)==0, bytes_per_row );

    /* wait for read DMA to become available */
    pie_rdma_dsr_wait=0;
    while( !(*PIE_RDMA_DSR(channel) & PIE_RDMA_DSR_READY) ) {
        /* FIXME - infinite loop */
        pie_rdma_dsr_wait++;

        /* Wait for a "big" chunk of counts before deciding we're hosed.  Need
         * to plan the firmware well enough that we never block waiting for
         * DMA.
         */
        cpu_spin_delay( 1 ); /* wait 1 microsecond */

        /* davep 28-May-2013 ; XXX temp debug  */
        if( pie_rdma_dsr_wait >= MAX_PIE_DMA_WAIT ) {
            dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
            pie_rdma_dump();
            pie_rdma_channel_dump(0);
            pie_wdma_dump();
            pie_wdma_channel_dump(0);
            piems_rdma_debug_log();
            piems_wdma_debug_log();
            dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
        }

        XASSERT( pie_rdma_dsr_wait < MAX_PIE_DMA_WAIT, pie_rdma_dsr_wait );
    }
    pie_rdma_dsr_wait=0;

    /* load the pointer to the DMA destination */
    *PIE_RDMA_DAR1(channel) = dar1;
    *PIE_RDMA_DAR2(channel) = dar2;
    /* number of rows to to be stored to main memory; not a -1 register */
    *PIE_RDMA_DRCR1(channel) = drcr1;
    *PIE_RDMA_DRCR2(channel) = drcr2;

    /* number of bytes of data that are in each row */
    *PIE_RDMA_DBCR(channel) = bytes_per_row;

    /* set the transfer count; this is a +1 register (counts 0..max-1) so
     * subtract one 
     * XXX - is this calculation correct?
     */
    /* 7-Mar-05 ; I got an email this morning from Randy Schmidt, the owner of
     * the PIE read/write DMA blocks, saying this byte count is indeed the same
     * as bytes_per_row * row_count and that it's simply a convenience for
     * the DMA blocks.
     */
    *PIE_RDMA_DTCR(channel) = rows * bytes_per_row - 1;

}

void pie_rdma_channel_dump( uint8_t channel ) 
{
    XASSERT( pie_rdma_channel_is_valid(channel), channel );
//    XASSERT( channel<PIE_RDMA_NUM_CHANNELS, channel );

    dbg2( "DCR=0x%x ", *PIE_RDMA_DCR(channel)  );
    dbg2( "DAR1=0x%x ", *PIE_RDMA_DAR1(channel) );
    dbg2( "DAR2=0x%x ", *PIE_RDMA_DAR2(channel) );
    dbg2( "DRCR1=0x%x ", *PIE_RDMA_DRCR1(channel));
    dbg2( "DRCR2=0x%x ", *PIE_RDMA_DRCR2(channel));
    dbg2( "DBCR=0x%x ", *PIE_RDMA_DBCR(channel) );
    dbg2( "DTCR=0x%x ", *PIE_RDMA_DTCR(channel) );
    dbg2( "DASR=0x%x ", *PIE_RDMA_DASR(channel) );
    dbg2( "DCSR=0x%x ", *PIE_RDMA_DCSR(channel) );
    dbg2( "DRCSR=0x%x ", *PIE_RDMA_DRCSR(channel));
    dbg2( "DSR=0x%x ", *PIE_RDMA_DSR(channel)  );
    dbg2( "DRR=0x%x\n", *PIE_RDMA_DRR(channel)  );
}

void pie_rdma_dump( void )
{
    int i;

    /* davep 21-Jun-2013 ; update a tiny bit to the RegBuild headers */

    /* dump the control register(s) */
    dbg1( "pie rdma CCR=0x%08x CSR=0x%08x CIR=0x%08x\n", 
            pie_rdma_common_regs->CCR, 
            pie_rdma_common_regs->CSR, 
            pie_rdma_common_regs->CIR );

    for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {
        if( pie_rdma_channel_is_valid(i) ) {
            pie_rdma_channel_dump(i);
        }
    }
}

const struct ice_dma_driver * pie_rdma_get_driver( void )
{
    return &pie_rdma_driver;
}

void pie_rdma_set_icebuf_isr( iceisr_f icebuf_isr_f )
{
    pie_rdma_driver.icebuf_isr = icebuf_isr_f;
}

scan_err_t pie_rdma_channel_open( uint8_t channel )
{
    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );
    /* no-op */
    return SCANERR_NONE;
}

void pie_rdma_channel_close( uint8_t channel )
{
    XASSERT( channel < PIE_RDMA_NUM_CHANNELS, channel );
    /* no-op */
}


/* ------------------------------------------
 *
 * Write DMA Functions
 *
 * ------------------------------------------
 */

static void pie_wdma_set_control( uint32_t rows_per_write, uint32_t color, uint32_t mode )
{
    XASSERT( color==PIE_WDMA_DGCR_COLOR||color==PIE_WDMA_DGCR_MONO, color );

    /* davep 13-Oct-2008 ; adding support for RGBx and start of support for RGB
     * packed modes 
     */

    /* only these bits are valid for mode */
    XASSERT( (mode & ~PIE_WDMA_DGCR_MODE_MASK)==0, mode );

    /* sanity checks on the availability of the feature */
#ifndef HAVE_PIE_OUTPUT_RGBX
    XASSERT( mode != PIE_WDMA_DGCR_MODE_RGBX, mode );
#endif

#ifndef HAVE_PIE_OUTPUT_RGB
    XASSERT( mode != PIE_WDMA_DGCR_MODE_RGB, mode );
#endif

    XASSERT( rows_per_write <= PIE_WDMA_MAX_ROWS, rows_per_write );

    /* set up the PIE write DMA controller (no reset bit required); 
     * note that 'rows' is NOT a -1 field.
     */
    /* 6-May-05 ; added paramter support for setting color/mono and xrgb/3plane */
    *PIE_WDMA_DGCR = PIE_WDMA_DGCR_ROWS(rows_per_write)
                   | color 
                   | mode 
                   ;
}

/**
 * \brief  Set the PIE WDMA Config/Control register (DGCR)
 *
 *
 * \author David Poole
 * \date 19-Feb-2010
 *
 */

void pie_wdma_setup_config( scan_cmode_t cmode, 
                            pie_pixel_t pieout, 
                            uint32_t strip_size_out,
                            uint32_t pixels_per_row )
{
    if (cmode == SCAN_CMODE_COLOR) {
        switch( pieout ) {
            case PIE_PIXEL_XRGB :
                pie_wdma_set_control( strip_size_out, 
                                      PIE_WDMA_DGCR_COLOR, PIE_WDMA_DGCR_MODE_XRGB );
                break;

            case PIE_PIXEL_RGBX :
#ifdef HAVE_PIE_OUTPUT_RGBX
                pie_wdma_set_control( strip_size_out, 
                                      PIE_WDMA_DGCR_COLOR, PIE_WDMA_DGCR_MODE_RGBX );
#else
                /* RGBX packed not supported on this platform */
                XASSERT( 0, PIE_PIXEL_RGBX );
#endif
                break;

            case PIE_PIXEL_RGB :
#ifdef HAVE_PIE_OUTPUT_RGB  
                pie_wdma_set_control( strip_size_out, 
                                      PIE_WDMA_DGCR_COLOR, PIE_WDMA_DGCR_MODE_RGB );
#else
                /* RGB packed not supported on this platform */
                XASSERT( 0, PIE_PIXEL_RGB );
#endif
                break;

            case PIE_PIXEL_3_PLANES :
                pie_wdma_set_control( strip_size_out, 
                                      PIE_WDMA_DGCR_COLOR, PIE_WDMA_DGCR_MODE_NORM );
                break;
                
            default :
                XASSERT( 0, pieout );
                break;
        }
    } 
    else {
        pie_wdma_set_control( strip_size_out, 
                              PIE_WDMA_DGCR_MONO, PIE_WDMA_DGCR_MODE_NORM );
    }
}

//--------------------------------------------------------------------------
// Function     : pie_wdma_dynamic_row
//   returns    : none
//   flag       : true - turn on PIE write DMA dynamic row count
//                false - turn DRC off
// Created by   : David Poole
// Date created : 24-May-05
// Description  : 
//
//  From the "MA for PIE Write DMA":
//      "If dynamic row count is turned on, the value of row count in the field
//      above is ignored [the PIE_WDMA_DGCR_ROWS field -ed]. The block uses a
//      signal from the data source to determine what the row count is any
//      given time." 
//
//  In a nutshell, due to XYScale, the output row count can change for every
//  strip being fed into PIE read DMA. With dynamic row count enabled, XYScale
//  tells PIE write DMA how many rows to expect in the current strip.
//  
//  This functionality should have been part of pie_wdma_set_control() but
//  _set_control() has been in use for a while and DRC is a new feature. I
//  didn't want to break a lot of existing code.
// 
//--------------------------------------------------------------------------
    
void pie_wdma_dynamic_rows( bool flag )
{
    if( flag ) {
        *PIE_WDMA_DGCR |= PIE_WDMA_DGCR_DRC;
    }
    else {
        *PIE_WDMA_DGCR &= ~PIE_WDMA_DGCR_DRC;
    }
}


void pie_wdma_reset( void )
{
    int i;
    uint32_t num32;

    /* set to power-on value */
    *PIE_WDMA_DGCR = PIE_WDMA_DGCR_R;

    /* 6-Feb-05 ; XXX - I'm doing this same 0xdeaddead trick on the Read DMA
     * registers but, again, do I want to trust this? 
     *
     * Make sure my channel addresses are correct. According to the read DMA
     * datasheet, DSTR (DMA Start Register) and DIR (DMA Interrupt Register)
     * are write-only and will return 0xdeaddead on read. The Write DMA
     * datasheet doesn't say anything about it but I'm seeing 0xdeaddead on the
     * debugger.
     */
    /* 7-Feb-05 ; Randy Schmidt, the designer of the read/write DMA blocks,
     * tells me he doesn't plan on changing this so leave as-is for now.
     */
    for( i=0 ; i<PIE_WDMA_NUM_CHANNELS ; i++ ) {
//        scanlog_hex_dump( (unsigned char *)PIE_WDMA_DCR(i), 12*sizeof(uint32_t));
//        dbg1( "%s 0x%x 0x%x\n", __FUNCTION__, PIE_WDMA_DCR(i), *PIE_WDMA_DCR(i) ));

       /* davep 12-Aug-2007 ; can't read or write the start register without
         * triggering a start; my fake PIE hardware isn't smart enough to tell
         * the difference
         */
#ifndef PLAT_CHIMERA
        num32 = *PIE_WDMA_DSTR(i);
        XASSERT( num32 == 0xdeaddeadUL, num32 );
#endif
        num32 = *PIE_WDMA_DIR(i);
        XASSERT( num32 == 0xdeaddeadUL, num32 );

        /* 24-Mar-05 ; During some PIE DMA debugging, RandyS and JohnM tell me
         * it is unnecessary and kind of silly to reset each of the channels
         * right after doing a block-level reset.
         */
        /* 1-May-05 ; (yup, it's May already) put the channel reset back in so
         * we can set all the registers to a clean state at the same time
         */
        pie_wdma_channel_reset( i );
    }
}

/**
 * \brief  Return true if all of PIE WDMA is idle
 *
 * Created to assist multiplex PIE DMAs
 *
 * \author David Poole
 * \date 28-May-2013
 */

bool pie_wdma_is_idle( void )
{
    int channel;
    bool is_idle;
    uint32_t dsr;

    is_idle = true;

    dsr = ~0;
    for( channel=0 ; channel<PIE_WDMA_NUM_CHANNELS ; channel++ ) {
        if( !pie_wdma_channel_is_enabled( channel ) ) {
            /* skip currently unused hardware channels */
            continue;
        }
        /* want READY==1 ACTIVE==0 INTPEND==0 */
        dsr = *PIE_WDMA_DSR(channel);
        if( !(dsr & PIE_WDMA_DSR_READY) ||  /* ready bit clear */
            (dsr & PIE_WDMA_DSR_ACTIVE) ||  /* active bit set */
            (dsr & PIE_WDMA_DSR_CMPL) ) {   /* interupt pending bit set */
            is_idle = false;
            break;
        }
    }

//    dbg2( "%s dsr=0x%x %d\n", __FUNCTION__, dsr, is_idle );
    return is_idle;
}

void pie_wdma_channel_reset( uint8_t channel )
{
#ifdef PIE_DMA_DEBUG
    dbg2( "%s(%d)\n", __FUNCTION__, channel );
#endif

    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );
    
    /* reset channel then wait for ready */
    *PIE_WDMA_DCR(channel) = PIE_WDMA_DCR_RESET;

    /* set other main registers to power-on defaults */
    *PIE_WDMA_DAR(channel) = PIE_WDMA_DAR_R;
    *PIE_WDMA_DRCR(channel) = PIE_WDMA_DRCR_R;
    *PIE_WDMA_DBCR(channel) = PIE_WDMA_DBCR_R;
    *PIE_WDMA_DTCR(channel) = PIE_WDMA_DTCR_R;

    /* XXX - datasheet says "assert this signal for at least one cycle and then
     * deassert it" ; is 1us too long?
     */
    cpu_spin_delay(1);

    /* clear the reset bit; leaves channel disabled and interrupts disabled */
    *PIE_WDMA_DCR(channel) = PIE_WDMA_DCR_R;

    /* wait for the ready bit */
    pie_wdma_dsr_wait=0;
    while( !(*PIE_WDMA_DSR(channel) & PIE_WDMA_DSR_READY) ) {
        /* FIXME - infinite loop */
        pie_wdma_dsr_wait++;

        cpu_spin_delay( 1 ); /* wait 1 microsecond */

        XASSERT( pie_wdma_dsr_wait < MAX_PIE_DMA_WAIT, pie_wdma_dsr_wait );
    }
    pie_wdma_dsr_wait=0;

}

void pie_wdma_channel_start( uint8_t channel )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );

    /* start DMA by writing anything to the start register */
    *PIE_WDMA_DSTR(channel) = 1;
}

void pie_wdma_channel_enable( uint8_t channel )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );

    /* enable DMA, enable DMA interrupt */
    *PIE_WDMA_DCR(channel) = PIE_WDMA_DCR_EN
                           | PIE_WDMA_DCR_CIE; 
}

bool pie_wdma_channel_is_enabled( uint8_t channel )
{
    uint32_t num32;

    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );

    /* check for enabled DMA, enabled DMA interrupt */
    num32 = *PIE_WDMA_DCR(channel); 

    return (num32 & (PIE_WDMA_DCR_EN | PIE_WDMA_DCR_CIE)) && true; 
}

void pie_wdma_channel_disable( uint8_t channel )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );

    /* disable DMA, disable DMA interrupt */
    *PIE_WDMA_DCR(channel) &= ~(PIE_WDMA_DCR_EN | PIE_WDMA_DCR_CIE); 
}

void pie_wdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t rows, uint32_t bytes_per_row )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );

    /* make sure pointer is correctly longword (32-bit) aligned */
    /* 25-Mar-05 ; looking at the PIE Write DMA MA doc, it actually requires
     * word aligned, not quadword aligned. However, since these buffers need to
     * also flow to other DMA parts of the system which *are* quadword aligned,
     * take the more conservative approach.
     */
    XASSERT( ((uint32_t)dma_dest & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, (uint32_t)dma_dest );

    /* make sure we're up in the cache bypass address area */
    XASSERT( IS_DMAPTR_VALID((uint32_t)dma_dest), (uint32_t)dma_dest );

    /* 11-Apr-05 ; PIE read DMA needs to be reset before changing the row count
     * but PIE write DMA doesn't. I have a lot of checks in read DMA to prevent
     * the rows changing which I thought weren't necessary in the write.
     * However, I've found it's too easy to wind up in a case where write DMA
     * stalls because of short buffers (e.g., 600 rows with 9 rows per buffer
     * gives 66 9-row buffers and 1 six-row buffer) and I'm unsure about
     * changing the DGCR row count while a transaction is currently running. So
     * for now, until I understand the problem better, I'm going to require all
     * transactions to match the DGCR row count the same as read DMA does.
     */
    /* 26-May-05 davep ; initial pass at dynamic row support */
//    XASSERT( PIE_WDMA_DGCR_GET_ROWS(*PIE_WDMA_DGCR)==rows, rows );

    /* 11-Apr-05 ; We can only transfer in 16-byte bursts so make sure the
     * bytes_per_row is exactly burst sized (caller's responsibility). 
     */
    XASSERT( (bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK)==0, bytes_per_row );

#ifdef PIE_DMA_DEBUG 
    dbg2( "piewdma load ch=%d dst=0x%x rows=%ld bpr=%ld\n", channel, dma_dest, rows, bytes_per_row);
#endif
    /* wait for write DMA to become available */
    pie_wdma_dsr_wait=0;
    while( !(*PIE_WDMA_DSR(channel) & PIE_WDMA_DSR_READY) ) {
        /* FIXME - infinite loop */
        pie_wdma_dsr_wait++;

        /* Wait for a "big" chunk of counts before deciding we're hosed.  Need
         * to plan the firmware well enough that we never block waiting for
         * DMA.
         */
        cpu_spin_delay( 1 ); /* wait 1 microsecond */
        XASSERT( pie_wdma_dsr_wait < MAX_PIE_DMA_WAIT, pie_wdma_dsr_wait );
    }
    pie_wdma_dsr_wait=0;

    /* load the pointer to the DMA destination */
    *PIE_WDMA_DAR(channel) = (uint32_t)dma_dest;

    /* "number of rows" to to be stored to main memory */
    /* 7-Mar-05 ; this might be a -1 register */
    /* 25-Mar-05 ; it's not. */
    *PIE_WDMA_DRCR(channel) = rows;

    /* number of bytes of data that are in each row */
    *PIE_WDMA_DBCR(channel) = bytes_per_row;

    /* set the transfer count; this is a +1 register (counts 0..max-1) so
     * subtract one 
     * XXX - is this calculation correct?
     */
    /* 7-Mar-05 ; I got an email this morning from Randy Schmidt, the owner of
     * the PIE read/write DMA blocks, saying this byte count is indeed the same
     * as bytes_per_row * number_of_rows and that it's simply a convenience
     * for the DMA blocks.
     */
    *PIE_WDMA_DTCR(channel) = rows * bytes_per_row - 1;

    /* XXX - get rid of this debugging statement once DMA is working; it slows
     * us down too much
     */
#ifdef PIE_DMA_DEBUG
//    dbg3( ("%s DAR=0x%x DRCR=0x%x DBCR=0x%x DTCR=0x%x\n",
//                __FUNCTION__,
//                *PIE_WDMA_DAR(channel), 
//                *PIE_WDMA_DRCR(channel),
//                *PIE_WDMA_DBCR(channel),
//                *PIE_WDMA_DTCR(channel) ) );
#endif
}

void
pie_wdma_channel_dump( uint8_t channel ) 
{
    XASSERT( channel<PIE_WDMA_NUM_CHANNELS, channel );

    dbg2( "DCR=0x%x ", *PIE_WDMA_DCR(channel) );
    dbg2( "DAR=0x%x ", *PIE_WDMA_DAR(channel) );
    dbg2( "DRCR=0x%x ", *PIE_WDMA_DRCR(channel));
    dbg2( "DBCR=0x%x ", *PIE_WDMA_DBCR(channel) );
    dbg2( "DTCR=0x%x ", *PIE_WDMA_DTCR(channel) );
    dbg2( "DASR=0x%x ", *PIE_WDMA_DASR(channel) );
    dbg2( "DCSR=0x%x ", *PIE_WDMA_DCSR(channel) );
    dbg2( "DRCSR=0x%x ", *PIE_WDMA_DRCSR(channel) );
    dbg2( "DSR=0x%x ", *PIE_WDMA_DSR(channel) );
    dbg2( "DRR=0x%x\n", *PIE_WDMA_DRR(channel) );
}

void pie_wdma_dump( void )
{
    int i;

    /* dump the control register(s) */
    dbg2( "pie wdma DGCR=0x%x\n", *PIE_WDMA_DGCR );

    for( i=0 ; i<PIE_WDMA_NUM_CHANNELS ; i++ ) {
        pie_wdma_channel_dump(i);
    }
}

const struct ice_dma_driver * pie_wdma_get_driver( void )
{
    return &pie_wdma_driver;
}

void pie_wdma_set_icebuf_isr( iceisr_f icebuf_isr_f )
{
    pie_wdma_driver.icebuf_isr = icebuf_isr_f;
}

scan_err_t pie_wdma_channel_open( uint8_t channel )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );
    /* no-op */
    return SCANERR_NONE;
}

void pie_wdma_channel_close( uint8_t channel )
{
    XASSERT( channel < PIE_WDMA_NUM_CHANNELS, channel );
    /* no-op */
}

