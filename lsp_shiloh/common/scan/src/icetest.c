/**
 * \file icetest.c
 *
 * \brief Functions to interface to the ICE Test hardware block.
 *
 * ICE Test is a DMA interface that will feed data into the top of the Scan
 * block or CISX.
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
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "list.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "safetylock.h"
#include "ddma.h"
#include "icetest.h"
#include "ICE_test_regmasks.h"
#include "ICE_test_regstructs.h"
#include "scanalyzer.h"

//#define ICETEST_DMA_ISR_DEBUG
#ifdef ICETEST_DMA_ISR_DEBUG
#define isr_dbg2 dbg2
#else
#define isr_dbg2(...)
#endif

static ICE_TEST_REGS_t * const icetest_regs = (ICE_TEST_REGS_t *)ICE_ICE_REGS_BASE;
static TESTIDMA_REGS_t * const icetest_idma_regs = (TESTIDMA_REGS_t *)ICE_ICE_REGS_TESTIDMA_BASE;

//#define MAX_ICETEST_IDMA_DESCRIPTORS 48
#define MAX_ICETEST_IDMA_DESCRIPTORS 16

/* danger! danger! danger! these pending bits mean disaster */
#define ICETEST_ERROR_INTERRUPTS (ICE_TEST_INTPEND_RESERVED1_MASK \
                                  |ICE_TEST_INTPEND_UNDERINTPEND_MASK)

#define IDMA_ERROR_INTERRUPTS (TESTIDMA_INT_ST_RESERVED1_MASK \
                               |TESTIDMA_INT_ST_WHO_INT_ST_MASK)

/* power-on value for some registers as of 28-Mar-2011 */
#define ICE_TEST_TESTCONFIG_R  0

#define ICE_TEST_IDMA_CFG_R TESTIDMA_CFG_REPLICATE_MASK 

#define ICE_TEST_IDMA_STATUS_R ( TESTIDMA_STATUS_PACKER_EMPTY_MASK \
                               | TESTIDMA_STATUS_EMPTY_DBUF_MASK \
                               | TESTIDMA_STATUS_EMPTY_CBUF_MASK \
                               | TESTIDMA_STATUS_SOFTRESET_MASK \
                              )

struct {
    int num_interrupts;
    
    int channel_isr[ICETEST_IDMA_NUM_CHANNELS];

} icetest_interrupt_stats;

static struct ddma_channel icetest_desc_channel;

static struct ice_dma_mm icetest_idma_mm;
static struct ice_dma_channel icetest_idma_channel;

static struct ice_dma_driver icetest_idma_driver = { 
    .msg_data = SMSG_ICETEST_IDMA_DATA,

    /* set the queue depth to a number which, I hope, will keep our chained
     * descriptors far enough a head of the running DMA
     */
    .max_queue_depth = 10,

    .reset =      icetest_idma_channel_reset,
    .enable =     icetest_idma_channel_enable,
    .disable =    icetest_idma_channel_disable,
    .load =       icetest_idma_channel_load, 
    .start =      icetest_idma_channel_start,   /* start; can't use because called for each load */
    .is_enabled = icetest_idma_channel_is_enabled,
    .icebuf_isr = NULL,  /* dma_isr */
};

static void channel_isr( struct ddma_channel *dch )
{
    struct ddma_descriptor *curr;

    /* 
     * 
     *
     * davep 04-Apr-2011 ; TODO move to ddma.c 
     *
     *
     */

    isr_dbg2( "%s\n", __FUNCTION__ );

    /* drop a log on entry */
//    SCANALYZER_LOG(LOG_ICETEST_CHANNEL_ISR,(1<<16)|dch->channel);

    icetest_interrupt_stats.channel_isr[dch->channel]++;

    /* use a convenience pointer; remember we still have to modify the list
     * itself
     */
    curr = dch->head_desc;
    cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

    /* davep 11-Oct-2010 ; add safety check to make sure the hardware is far
     * enough behind us we won't collide ("listening for the bulldozers")
     */
    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK((struct ddma_descriptor *)curr->fw_prev_descriptor_addr), 
            (uint32_t)curr->fw_prev_descriptor_addr );

    /* we could have more than one completed descriptor so we'll walk the list
     * until we find a descriptor with own bit set
     */
    while( !DESCRIPTOR_OWNED_BY_BLOCK( curr ) )
    {
//        dbg2( "%s %d %p %p %#x %#x\n", __FUNCTION__, dch->channel, curr, 
//                    curr->fw_next_descriptor_addr, curr->config_flags );

        XASSERT( (curr->config_flags & DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK)==0, 
                    curr->config_flags );

        /* davep 27-Apr-2010 ; XXX temp debug */
        if( ! (curr->config_flags & DDMA_DESCRIPTOR_CONFIG_INT) ) {
            scanlog_hex_dump( (unsigned char *)(curr)-32, 64 );
            ddma_descriptor_list_dump( &dch->desc_list );
        }
        XASSERT( curr->config_flags & DDMA_DESCRIPTOR_CONFIG_INT, 
                    curr->config_flags ); 

        /* call the default method to do the buffer dance */
        ice_dma_isr( &icetest_idma_mm, dch->channel );

        curr->src_addr = 0;

        /* Set the "own" bit back on the descriptor so I know I've already
         * handled this buffer. We're in a circular list so need to know when
         * to stop.
         */
        curr->config_flags |= DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;

        ddma_desc_flush( curr );

        /* move head of list to next element */
        dch->head_desc = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        dch->num_running -= 1;
        XASSERT( dch->num_running >=0, dch->channel );

        curr = dch->head_desc;
        cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

        ddma_channel_sanity( dch );
    }

    /* drop another at exit */
//    SCANALYZER_LOG(LOG_ICETEST_CHANNEL_ISR,dch->channel);
}

static void icetest_dma_interrupt( void )
{
    uint32_t ipend, iack;
    uint32_t dma_ipend;
    struct ddma_channel *dch;
    uint32_t icetest_idma_status;

    /* 
     * BIG FAT NOTE!  This is an interrupt handler
     */

    ipend = icetest_regs->IntPend;
    dma_ipend = icetest_idma_regs->int_st;

    /* drop a log on entry */
//    SCANALYZER_LOG(LOG_ICETEST_ISR,(ipend<<16)|dma_ipend);

    icetest_interrupt_stats.num_interrupts++;

    isr_dbg2( "%s %#x %#x\n", __FUNCTION__, ipend, dma_ipend );

//    dbg2( "%s %d %#x %#x %#x %#x %#x\n", __FUNCTION__, 
//                channel_idx, icetest_idma_regs->status, icetest_idma_regs->int_pend,
//                icetest_idma_regs->desc_read, icetest_idma_regs->xfer_length, 
//                icetest_idma_regs->xfer_addr );

    /* fail on bad stuff we don't ever want to see */
    XASSERT( !(ipend & ICETEST_ERROR_INTERRUPTS), ipend );
    XASSERT( !(dma_ipend & IDMA_ERROR_INTERRUPTS), dma_ipend );

    iack = 0;

    /* if we get this far, we should have something to handle */
    XASSERT( ipend, ipend );

    SCANALYZER_LOG(LOG_ICETEST_INTERRUPT,ipend); 
    SCANALYZER_LOG(LOG_ICETEST_STATUS,icetest_idma_regs->status); 

    /* if we have a pending "Transfer End" interrupt, handle it, ack it,
     * love it
     */
    if( ipend & TESTIDMA_INT_ST_FIN_INT_ST_MASK ) {
        dch = &icetest_desc_channel;
        if( dch->is_open ) {
            channel_isr( dch );

            /* davep 13-Oct-2010 ; if we've drained our chain */
            if( dch->num_running==0 ) {
                /* make sure block is truly idle */
                XASSERT( !(icetest_idma_regs->status & TESTIDMA_STATUS_DMA_BUSY_MASK ), icetest_idma_regs->status );
            }
        } 
        else { 
            /* assume we're doing a one-shot (probably via icetest_idma_desc_write())
             * so do some extra sanity checks here
             */
            icetest_idma_status = icetest_idma_regs->status;;
            XASSERT( !(icetest_idma_status & TESTIDMA_STATUS_DMA_BUSY_MASK), icetest_idma_status );

            /* RTOS wake anyone waiting for the interrupt firing */
            icetest_interrupt_rtos_signal();
        }

        iack |= TESTIDMA_INT_CL_FIN_INT_CL_MASK; 
    }

    /* ack em if you got em */
    if( ipend & TESTIDMA_INT_ST_EOI_INT_ST_MASK ) {
        iack |= TESTIDMA_INT_CL_EOI_INT_CL_MASK ; 
    }

    if( ipend & TESTIDMA_INT_ST_RST_INT_ST_MASK ) {
        iack |= TESTIDMA_INT_CL_RST_INT_CL_MASK; 
    }

    /* end of image */
    if( dma_ipend & TESTIDMA_INT_ST_EOI_INT_ST_MASK ) {
        iack |= TESTIDMA_INT_CL_EOI_INT_CL_MASK;
    }

    if( dma_ipend & TESTIDMA_INT_ST_FIN_INT_ST_MASK ) {
        iack |= TESTIDMA_INT_CL_FIN_INT_CL_MASK;
    }

    icetest_idma_regs->int_cl = iack;

    /* drop another log on exit log on entry */
//    SCANALYZER_LOG(LOG_ICETEST_ISR,0);
}

#ifdef __KERNEL__
static irqreturn_t icetest_interrupt( int irq, void *dev_id ) 
#else
static void icetest_interrupt( uint32_t param )
#endif
{
    uint32_t ipend;

    /* 
     * BIT FAT NOTE: THIS IS AN INTERRUPT HANDLER.
     *
     */

    ipend = icetest_regs->IntPend;

    /* obviously temp debug */
    isr_dbg2( "%s %#x\n", __FUNCTION__, ipend );

    /* fail on unexpected interrupts (stuff we don't ever want to see) */
    if( ipend & ICE_TEST_INTPEND_UNDERINTPEND_MASK ) {
        XASSERT( 0, ipend );
    }

    if( ipend & ICE_TEST_INTPEND_DMAINTPEND_MASK ) {
        icetest_dma_interrupt();
    }

    /* trap on new interrupts we don't yet have code for (the asic folks like
     * to change stuff)
     */
    XASSERT( !(ipend & ~(ICE_TEST_INTPEND_UNDERINTPEND_MASK|ICE_TEST_INTPEND_DMAINTPEND_MASK)), ipend );

    icetest_regs->IntAck = ~0;

#ifdef __KERNEL__
    return IRQ_HANDLED;
#endif
}
    
uint32_t icetest_interrupt_disable( void )
{
    uint32_t was_enabled;
    uint32_t idma_was_enabled;

    was_enabled = icetest_regs->IntEn;

    /* disable interrupt(s) */
    icetest_regs->IntEn = 0;

    idma_was_enabled = icetest_idma_interrupt_disable();

    /* return non-zero if anything was enabled */
    return (was_enabled | idma_was_enabled) != 0;
}

void icetest_interrupt_enable( void )
{
    /* turn on everything */
    icetest_regs->IntEn = ~0;

    icetest_idma_interrupt_enable();
}

#ifdef __KERNEL__
scan_err_t icetest_capture_interrupt( irq_handler_t handler )
{
    int retcode;

    /* wait until explicitly enabled */
    icetest_interrupt_disable();

    retcode = request_irq( INTNUM_ICE_TEST, handler, 0, "icetest", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );
    XASSERT(retcode==0, retcode );

    return SCANERR_NONE;
}

void icetest_release_interrupt( void )
{
    icetest_interrupt_disable();
    /* disable at the OS level as well */
    disable_irq( INTNUM_ICE_TEST );
    free_irq( INTNUM_ICE_TEST, NULL );
}
#else
scan_err_t icetest_capture_interrupt( PFN_ISR handler )
{
    intAttach( INTNUM_ICE_TEST, 1, handler, INTNUM_ICE_TEST );
    intEnable( INTNUM_ICE_TEST );
    return SCANERR_NONE;
}

void icetest_release_interrupt( void )
{
    icetest_interrupt_disable();
    /* disable at the OS level as well */
    intDisable( INTNUM_ICE_TEST );
    intDetach( INTNUM_ICE_TEST );
}
#endif
/*
 * ICE Test IDMA
 */

void icetest_idma_reset( void )
{
    icetest_idma_regs->reset = 1;

    /* continuing in my long tradition of 1 usec sleeps on hardware reset */
    cpu_spin_delay( 1 );

    icetest_idma_regs->reset = 0;

    icetest_idma_regs->line_width = 0;

    /* ack any possible interrupts */
    icetest_idma_regs->int_cl = ~0;

    icetest_idma_regs->int_en = 0;

    icetest_idma_regs->cfg = ICE_TEST_IDMA_CFG_R;

//    XASSERT( icetest_idma_regs->status==ICE_TEST_IDMA_STATUS_R, icetest_idma_regs->status );
//    XASSERT( icetest_idma_regs->cfg==ICE_TEST_IDMA_CFG_R, icetest_idma_regs->cfg );

    /* our dma descriptors must be padded to a cache line size so we can flush
     * them from cache without breaking neighboring cache lines
     */
    XASSERT( sizeof(struct ddma_descriptor)==cpu_get_dcache_line_size(), 
            sizeof(struct ddma_descriptor));

}

uint32_t icetest_idma_interrupt_disable( void )
{
    uint32_t was_enabled;

    was_enabled = icetest_idma_regs->int_en;

    /* disable all interrupts */
    icetest_idma_regs->int_en = 0;

    /* return non-zero if anything was enabled */
    return was_enabled != 0;
}

void icetest_idma_interrupt_enable( void )
{
    /* enable selected error interrupts */
    icetest_idma_regs->int_en = TESTIDMA_INT_EN_EOI_INT_EN_MASK  /* end of image (EOI) */
                              | TESTIDMA_INT_EN_FIN_INT_EN_MASK  /* transfer end */
                              | TESTIDMA_INT_EN_WHO_INT_EN_MASK  /* incorrect owner (bad!) */
                              ;
}

void icetest_idma_disable( void )
{
    icetest_idma_regs->cfg = TESTIDMA_CFG_ENABLE_REPLACE_VAL(icetest_idma_regs->cfg,0); 
    icetest_idma_interrupt_disable();
}

void icetest_idma_enable( void )
{
    icetest_idma_regs->cfg = TESTIDMA_CFG_ENABLE_REPLACE_VAL(icetest_idma_regs->cfg,1); 
    icetest_idma_interrupt_enable();
}

void icetest_idma_soft_setup( void )
{
    uint32_t num32;

    num32 = icetest_idma_regs->cfg;

    /* XXX hardwire to 32-bit */
    num32 = TESTIDMA_CFG_OUT_WIDTH_REPLACE_VAL( num32, 5 );

//    /* XXX hardwire to 16-bit */
//    num32 = TESTIDMA_CFG_OUT_WIDTH_REPLACE_VAL( num32, 4 );

    /* hardwire to 16 words (can I get a meh?) */
    num32 = TESTIDMA_CFG_BURST_LEN_REPLACE_VAL( num32, 2 );

    /* davep 25-May-2011 ; copying unit test settings */
    num32 = TESTIDMA_CFG_REPLICATE_REPLACE_VAL( num32, 0 );

    icetest_idma_regs->cfg = num32;
}

void icetest_idma_desc_write( struct ddma_descriptor *desc, uint32_t line_width_bytes )
{
    uint32_t icetest_idma_status;

    /* davep 10-Feb-2012 ; I /think/ transfer_len_bytes is limited to 24-bits */
    XASSERT( (desc->transfer_len_bytes & 0xff000000)==0, desc->transfer_len_bytes );

    /* is this thing on? */
    XASSERT( icetest_idma_regs->cfg & TESTIDMA_CFG_ENABLE_MASK, icetest_idma_regs->cfg );

    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK(desc), desc->config_flags );

    /* assume we're doing a one-shot so make sure we're idle */
    icetest_idma_status = icetest_idma_regs->status;;
    XASSERT( !(icetest_idma_status & TESTIDMA_STATUS_DMA_BUSY_MASK), icetest_idma_status );

    ddma_desc_flush( desc );

    /* davep 09-Feb-2012 ; sanity check we're not overflowing the max width */
    XASSERT( (line_width_bytes & ~TESTIDMA_LINE_WIDTH_LINE_WIDTH_MASK )==0, 
            line_width_bytes );
    
    /* davep 27-May-2011 ; line_width is actually a don't care */
    icetest_idma_regs->line_width = line_width_bytes;

    XASSERT( desc->dma_ptr_self, (uint32_t)desc );
    ASSERT( desc->src_addr );
    ASSERT( desc->fw_src_addr );

    icetest_idma_regs->desc_write = (uint32_t)desc->dma_ptr_self;
}

void icetest_wait_for_fin( void )
{
    int i;
    uint32_t status;

    /* Blocking wait for icetest FIN. Used by test/debug code. 
     *
     * davep 14-Feb-2012 ; instead of polling, lets be smarter and attach to
     * the ISR.
     */

    /* RTOS wait for icetest ISR */
    icetest_interrupt_rtos_wait();

    SCANALYZER_LOG(LOG_ICETEST_STATUS,icetest_idma_regs->status); 

    /* davep 20-May-2013 ; looks like icetest not idle after the transfer
     * complete interrupt. Poll waiting for the empty flags to be set.
     */
#define ICETEST_IDLE  (TESTIDMA_STATUS_PACKER_EMPTY_MASK \
                    | TESTIDMA_STATUS_EMPTY_DBUF_MASK \
                    | TESTIDMA_STATUS_EMPTY_CBUF_MASK )

    for( i=0 ; i<10 ; i++ ) {
        SCANALYZER_LOG(LOG_ICETEST_STATUS,icetest_idma_regs->status); 
        if( (icetest_idma_regs->status & ICETEST_IDLE) == ICETEST_IDLE ) {
            break;
        }
        scos_sleep_milliseconds(100);
    }
    status = icetest_idma_regs->status;
    /* if we hit this assert, we timed out waiting for ICETest to go idle */
    XASSERT( (status & ICETEST_IDLE)==ICETEST_IDLE, status );
    SCANALYZER_LOG(LOG_ICETEST_STATUS,icetest_idma_regs->status); 
}

void icetest_idma_desc_write_and_wait( struct ddma_descriptor *desc, uint32_t line_width_bytes )
{
    /* ack all interrupts before we start poking the block; assumes ICETest
     * interrupt disabled!
     */
    icetest_idma_regs->int_cl = ~0;

    icetest_idma_desc_write( desc, line_width_bytes );

    icetest_wait_for_fin();
}

uint32_t icetest_idma_status_get( void )
{
    return icetest_idma_regs->status;
}

scan_err_t icetest_idma_channel_open( uint8_t channel )
{
    scan_err_t scerr;
    struct ddma_channel *dch;
    char name[DDMA_NAME_LEN+1];

    XASSERT( channel==0, channel );

    dch = &icetest_desc_channel;
    
    strncpy( name, "icetestidma ", DDMA_NAME_LEN );
    name[11] = '0' + channel;
    ddma_channel_open( dch, name, channel );

    scerr = ddma_channel_alloc_descriptors( dch, MAX_ICETEST_IDMA_DESCRIPTORS );
    if( scerr != 0 ){
        /* failure! oh woe! free the channel memory we've already allocated */
        ddma_channel_free_descriptors( dch );
        ddma_channel_close( dch );

        return SCANERR_OUT_OF_MEMORY;
    }
    return SCANERR_NONE;
}

void icetest_idma_channel_close( uint8_t channel )
{
    struct ddma_channel *dch;

    XASSERT( channel==0, channel );
    dch = &icetest_desc_channel;

    XASSERT( dch->channel==channel, dch->channel );
    XASSERT( dch->num_running==0, dch->num_running );

    ddma_channel_free_descriptors( dch );
    ddma_channel_close( dch );
}

int icetest_idma_open( uint8_t channels[],
                   uint8_t num_channels,
                   int num_empties,
                   int total_rows,
                   int bytes_per_row )
{
    scan_err_t scerr;
    int i;

    XASSERT( num_channels==1, num_channels );

    for( i=0 ; i<num_channels; i++ ) {

        scerr = icetest_idma_channel_open( channels[i] );
        if( scerr != 0 ){
            /* failure! oh woe! free the channel memory we've already allocated */
            i -= 1;
            while( i>=0 ) {
                icetest_idma_channel_close( channels[i] );
                i -= 1;
            }

            /* this function returns the number of icebufs allocated */
            return 0;
        }
    }

    return ice_dma_open( &icetest_idma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row, ICEBUF_TAG_ICETEST_IDMA );
}

scan_err_t icetest_idma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &icetest_idma_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void icetest_idma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    /* Don't touch num_rows here (like we do in other _add_ready() methods.
     * Because icetest is a input DMA, I'm assuming the buffer we're adding is
     * full of data to be pushed into ICEtest.
     */

    ice_dma_add_ready( &icetest_idma_mm, channel, addme );
}

void icetest_idma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &icetest_idma_mm, channel );
}

void icetest_idma_close( void )
{
    struct ddma_channel *dch;

    dbg2( "%s\n", __FUNCTION__ );

    dch = &icetest_desc_channel;

    if( dch->is_open  ) {
        icetest_idma_channel_close( dch->channel );
    }

    ice_dma_close( &icetest_idma_mm );
}

void icetest_idma_channel_reset( uint8_t channel )
{
    XASSERT( channel==0, channel );

    icetest_idma_reset();
}

void icetest_idma_channel_enable( uint8_t channel ) 
{
    XASSERT( channel==0, channel );

    icetest_idma_enable();
}

void icetest_idma_channel_disable( uint8_t channel ) 
{
    XASSERT( channel==0, channel );

    icetest_idma_disable();
}

void icetest_idma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                                uint32_t rows, uint32_t bytes_per_row )
{
    /* 
     * BIG FAT NOTE! 
     *
     * This can be called from interrupt context.
     *
     */

//    dbg2( "%s %#x %d %d\n", __FUNCTION__, dma_dest, rows, bytes_per_row );

    /* poke the bytes_per_row into the line_width (should be the same for all
     * load calls for this scan but I won't know the value until load is
     * called)
     */
    icetest_idma_regs->line_width = bytes_per_row;

    /* davep 02-Jul-2013 ; ddma needs the CPU pointer (ddma does the Linux
     * kernel dma mapping)
     */
    ddma_channel_load( &icetest_desc_channel, data_ptr, rows, bytes_per_row );
}

void icetest_idma_channel_start( uint8_t channel )
{
    /* no op; see also icetest_idma_late_launch() */
}

void icetest_idma_late_launch( void )
{
    struct ddma_channel *dch;

    dch = &icetest_desc_channel;

    XASSERT( dch->is_open, dch->channel );
    XASSERT( icetest_idma_channel_is_enabled( dch->channel ), dch->channel );

    if( icetest_idma_regs->status & TESTIDMA_STATUS_DMA_BUSY_MASK ) {
        /* DMA already running, no need to start it again */
        return;
    }

    XASSERT( dch->num_running > 0, dch->num_running );
    XASSERT( dch->head_desc != NULL, dch->channel );
    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK(dch->head_desc), dch->head_desc->config_flags );

    /* davep 27-May-2011 ; shotgun debug ; fiddle the flags to match ASIC
     * team's unit test
     */
//    dch->head_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_SOI; 
//    ddma_desc_flush( dch->head_desc );
//    dch->tail_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_EOI;  
//    ddma_desc_flush( dch->tail_desc );

    /* load head of descriptor list into the idma */
    ddma_desc_flush( dch->head_desc );
    /* davep 25-Apr-2012 ; adding vma/pma destinction */
    XASSERT( dch->head_desc->dma_ptr_self, (uint32_t)dch->head_desc );
//    icetest_idma_regs->desc_write = (uint32_t)dch->head_desc;

    ASSERT( dch->head_desc->src_addr );

    icetest_idma_regs->desc_write = (uint32_t)dch->head_desc->dma_ptr_self;
}

bool icetest_idma_channel_is_enabled( uint8_t channel )
{
    XASSERT( channel==0, channel );

    return (icetest_idma_regs->cfg & TESTIDMA_CFG_ENABLE_MASK ) && true;
}

void icetest_idma_cancel( void )
{
    struct ddma_channel *dch;

    dch = &icetest_desc_channel;

    if( dch->is_open ) {
        icetest_idma_channel_reset( dch->channel );
        dch->num_running = 0;
    }

    /* now call the parent method */
    ice_dma_cancel( &icetest_idma_mm );
}

void icetest_idma_sanity( void )
{
    struct ddma_channel *dch;

    dch = &icetest_desc_channel;

    if( dch->is_open ) {
        ddma_channel_sanity( dch );
    }

    ice_dma_sanity( &icetest_idma_mm );
}

int icetest_idma_setup_buffers( uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row )
{
    return ice_dma_setup_buffers( &icetest_idma_mm, channel, num_buffers,
                    rows_per_buffer, bytes_per_row, icetest_idma_add_buffer );
}

void icetest_idma_init( void ) 
{
    ice_dma_init( &icetest_idma_mm, "icetestidma", &icetest_idma_driver, 
            &icetest_idma_channel, 1 );
}

/*
 * ICE Test Config
 */

scan_err_t icetest_onetime_init( void )
{
    icetest_reset();

    /* sanity check to make sure the firmware pointing to correct address(s) */
//    XASSERT( icetest_idma_regs->status==ICE_TEST_IDMA_STATUS_R, icetest_idma_regs->status );
//    XASSERT( icetest_idma_regs->cfg==ICE_TEST_IDMA_CFG_R, icetest_idma_regs->cfg );

    /* hook interrupt */
    icetest_interrupt_disable();
//    icetest_capture_interrupt( simple_icetest_interrupt );
    icetest_capture_interrupt( icetest_interrupt );

    icetest_rtos_init();

    SCANALYZER_ENABLE_LOG(LOG_ICETEST_INTERRUPT);
    SCANALYZER_ENABLE_LOG(LOG_ICETEST_STATUS);

    return SCANERR_NONE;
}

scan_err_t icetest_cleanup_module( void )
{
    icetest_release_interrupt();

    return SCANERR_NONE;
}

scan_err_t icetest_soft_setup( void )
{
    icetest_interrupt_disable();

    icetest_reset();

    /* turn on everything */
    icetest_regs->IntEn = ~0;

    /* davep 31-Jan-2013 ; PICRATE has disappeared in a recent ASIC that can
     * self-pace itself
     */
#ifdef ICE_TEST_TESTCONFIG_PICRATE_REPLACE_VAL
    /* hardwire a few parameters until I know wtf I'm doing */
//    icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_PICRATE_REPLACE_VAL(icetest_regs->TestConfig, 7 );
    icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_PICRATE_REPLACE_VAL(icetest_regs->TestConfig, 128 );
//    icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_PICRATE_REPLACE_VAL(icetest_regs->TestConfig, 1 );
#endif

    icetest_idma_soft_setup();

    return SCANERR_NONE;
}

scan_err_t icetest_set_test_mode( icetest_mode_t mode )
{
    /* Selects the destination for ICE_test DMA data. Only valid when TestEn is
     * also set. 
     *
     * 0 : ICE_test DMA data is driven into the top of the SCAN block
     * 1 : ICE_test DMA data is driven into the top of the CISX block (note
     * that by putting CISX in bypass, the data can also effectively be driven
     * into the top of PIC)
     */

    switch( mode ) {
        case ICETEST_CONFIG_DATA_TO_SCAN :
            icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_TESTSEL_REPLACE_VAL(icetest_regs->TestConfig,0);
            break;

        case ICETEST_CONFIG_DATA_TO_CISX : 
            icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_TESTSEL_REPLACE_VAL(icetest_regs->TestConfig,1);
            break;

        default :   
            XASSERT( 0, mode );
            return SCANERR_INVALID_PARAM;
    }

    return SCANERR_NONE;
}

void icetest_enable( bool enable )
{
    uint32_t num32;

    num32 = icetest_regs->TestConfig;

    /* 0==normal mode  1==test mode */
    num32 = ICE_TEST_TESTCONFIG_TESTEN_REPLACE_VAL( num32, (enable?1:0) );

    /* 0==reset inactive  1==reset active */
    num32 = ICE_TEST_TESTCONFIG_TESTSR_REPLACE_VAL( num32, (enable?0:1) );

    icetest_regs->TestConfig = num32;

    if( enable ) {
        icetest_interrupt_enable();
    } 
    else {
        icetest_interrupt_disable();
    }
}

void icetest_reset( void )
{
    icetest_idma_reset();

    /* disable interrupt(s) */
    icetest_regs->IntEn = 0;

    /* ack any possible pending interrupts */
    icetest_regs->IntAck = ~0;

    /* set to power-on default config */
    icetest_regs->TestConfig = ICE_TEST_TESTCONFIG_R;

    memset( &icetest_interrupt_stats, 0, sizeof(icetest_interrupt_stats) );
}

void icetest_dump( void )
{
    dbg2( "%s\n", __FUNCTION__ );

    dbg2( "   config=0x%08x     int_en=0x%08x   int_pend=0x%08x\n", 
                icetest_regs->TestConfig, icetest_regs->IntEn, icetest_regs->IntPend );

    dbg2( "   dmacfg=0x%08x  dmastatus=0x%08x line_width=0x%08x\n", 
                icetest_idma_regs->cfg, icetest_idma_regs->status, 
                icetest_idma_regs->line_width );

    dbg2( "dmaint_en=0x%08x  dmaint_st=0x%08x\n", 
                icetest_idma_regs->int_en, icetest_idma_regs->int_st );

    dbg2( "desc_read=0x%08x   xfer_len=0x%08x   xferaddr=0x%08x\n", 
                icetest_idma_regs->desc_read, 
                icetest_idma_regs->xfer_length, icetest_idma_regs->xfer_addr );

    if( icetest_desc_channel.head_desc ) {
        ddma_desc_chain_dump( icetest_desc_channel.head_desc, 
                              icetest_desc_channel.desc_list.num_descriptors );
    }
//    ddma_data_peek( &icetest_desc_channel );
//    ice_dma_debug_log( &icetest_idma_mm );
}

void icetest_ddma_dump( void )
{
    dbg2( "%s\n", __FUNCTION__ );
    ddma_descriptor_list_dump( &icetest_desc_channel.desc_list );
    ddma_data_peek( &icetest_desc_channel.desc_list, 
                    icetest_idma_regs->line_width );
}

void icetest_interrupt_stats_dump( void )
{
    int i;

    dbg2( "icetest_isr=%d\n", icetest_interrupt_stats.num_interrupts );
    for( i=0 ; i<ICETEST_IDMA_NUM_CHANNELS ; i++ ) {
        dbg2( "icetest_isr dma ch=%d count=%d\n", i, icetest_interrupt_stats.channel_isr[i] );
    }
}

