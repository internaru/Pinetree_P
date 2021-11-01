/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file piemux.c 
 *
 * \brief Multiplex PIE (multiple separate images through PIE simultaneously)
 *
 * davep 23-May-2013
 *
 * Originally created to handle nsensor support (sensors on both sides of page)
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "list.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "pie.h"
#include "piemux.h"
#include "scantools.h"
#include "pipemap.h"

//#define PIEMUX_DEBUG

#ifdef PIEMUX_DEBUG
#define piemux_dbg2 dbg2
#else
#define piemux_dbg2(...)
#endif

#define PIEMUX_NUM_CHANNELS 2

struct piemux_channel {
    bool is_open;

    uint8_t channel;

    ATLISTENTRY readies;
    int num_readies;

    int use_count;

    /* counters for test/debug */ 
    int idle_add_readies;
    int active_add_readies;
    int num_stalls;

    /* for context switching pie */
    struct pie_context regs;
};

struct piemux_mm {
    
    struct piemux_channel channels[PIEMUX_NUM_CHANNELS];

    /* number of times the active channel gets to run before we look for
     * another channel to run
     */
    int slice_count;

    struct piemux_channel *active_ch;

    /* counters to aid debugging */
    int num_swaps;
    int swap_fails;
    int num_scheduled;
    int add_readies;
    int num_timeslice_expired;
};

struct piemux_mm piemux_mm;

static void channel_init( struct piemux_channel *ch, uint8_t channel )
{
    memset( ch, 0, sizeof(struct piemux_channel) );

    ATInitList( &ch->readies );
    ch->channel = channel;
}

static scan_err_t piemux_channel_open( struct piemux_channel *ch, uint8_t channel )
{
    XASSERT( !ch->is_open, ch->channel );
    XASSERT( ch->num_readies==0, ch->num_readies) ;
    XASSERT( ATIsListEmpty( &ch->readies ), (uint32_t)&ch->readies );

    channel_init( ch, channel );
    ch->is_open = true;

    dbg2( "%s ch=%d\n", __FUNCTION__, ch->channel );

    return SCANERR_NONE;
}

static void piemux_channel_close( struct piemux_channel *ch )
{
    XASSERT( ch->is_open, ch->channel );
    XASSERT( ch->num_readies==0, ch->num_readies) ;
    XASSERT( ATIsListEmpty( &ch->readies ), (uint32_t)&ch->readies );

    dbg2( "%s ch=%d\n", __FUNCTION__, ch->channel );

    ch->is_open = false;
}

static void piemux_channel_cancel( struct piemux_channel *ch )
{
    struct ice_dma_buffer *icebuf;

    XASSERT( ch->is_open, ch->channel );

    while(1) {
        icebuf = icebuf_pop( &ch->readies );
        if( !icebuf ) {
            break;
        }
        ch->num_readies--;
        XASSERT( ch->num_readies>=0, ch->channel );

        /* put it back onto PIC WDMA to be freed when we clean up */
        pic_interrupt_disable();
        pic_wdma_add_ready( icebuf->channel, &icebuf );
        pic_interrupt_enable();
    }
}

static scan_err_t send_to_pie( struct ice_dma_buffer **p_icebuf )
{
    uint8_t pie_channel;
    struct ice_dma_buffer *icebuf;

    PTR_ASSIGN( icebuf, *p_icebuf );

//    dbg2( "%s icebuf=%p data=%p\n", __FUNCTION__, icebuf, icebuf->data );

    pie_channel = pipemap_pic_to_pie_channel( icebuf );

    pie_interrupt_disable();
    piems_rdma_add_ready( pie_channel, &icebuf );
    piems_rdma_channel_launch( pie_channel );
    pie_interrupt_enable();

    return SCANERR_NONE;
}

static void piemux_launch( struct piemux_mm *mm, struct piemux_channel *ch )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    int count;

    /* re-launch all the write channels */
    piems_wdma_launch();

    count = 0;
    /* send everything */
    while(1) {
//    /* send up to slice_count pending buffers to PIE RDMA */
//    while(count < mm->slice_count ) {
        icebuf = icebuf_pop( &ch->readies );
        if( !icebuf ) {
            break;
        }
        count++;
        ch->num_readies--;
        XASSERT( ch->num_readies>=0, ch->channel );

        scerr = send_to_pie( &icebuf );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    pie_interrupt_disable();
    piems_rdma_launch();
    pie_interrupt_enable();

//    /* re-launch all the write channels */
//    piems_wdma_launch();
}

static struct piemux_channel *next_channel_to_run( struct piemux_mm *mm, struct piemux_channel *exclude_ch )
{
    struct piemux_channel *next_ch;

    /* davep 19-Jun-2013 ; go simple */
    if( mm->active_ch->channel==0 ) {
        next_ch = &mm->channels[1];
    }
    else {
        next_ch = &mm->channels[0];
    }

#if 0
    struct piemux_channel *next_ch, *ch;
    int channel;

//    dbg2( "%s exclude=%d\n", __FUNCTION__, (exclude_ch?exclude_ch->channel:-1) );

    /* Linear search for channel with the biggest number of num readies. The
     * exclude_ch will allow us to search for the channel other than the active
     * channel (for round robin swapping).
     */
    next_ch = &mm->channels[0];
    XASSERT( next_ch->is_open, next_ch->channel );

    for( channel=1 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {

        ch = &mm->channels[channel];
        if( !ch->is_open ) {
            continue;
        }

        dbg2( "%s next=%d readies=%d ch=%d readies=%d\n", __FUNCTION__,
                next_ch->channel, next_ch->num_readies, 
                ch->channel, ch->num_readies );

        if( ch==exclude_ch ) {
            continue;
        }
        
        if( ch->num_readies >= next_ch->num_readies ) {
            next_ch = ch;
        }
    }

    if( next_ch==exclude_ch ) {
        dbg2( "%s return null\n", __FUNCTION__ );
        return NULL;
    }

    dbg2( "%s return ch=%d\n", __FUNCTION__, next_ch->channel );
#endif
    return next_ch;
}

static void pie_swap_to( struct piemux_mm *mm, struct piemux_channel *next_ch )
{
    struct pie_context *regs;
    bool rdma_idle, wdma_idle;

    ASSERT( mm->active_ch );

    XASSERT( next_ch->is_open, next_ch->channel );

    rdma_idle = piems_rdma_is_idle();
    wdma_idle = piems_wdma_is_idle();

    piemux_dbg2( "%s from ch=%d to ch=%d r=%d w=%d\n", __FUNCTION__, 
                mm->active_ch->channel, next_ch->channel, rdma_idle, wdma_idle );

    /* If PIE isn't yet finished chewing, we'll have to come back later & swap */
    if( !rdma_idle || !wdma_idle ) {
//    if( !piems_rdma_is_idle() || !piems_wdma_is_idle() ) {
        /* Block any new buffers from arriving. We wait for PIE to drain so we
         * can swap channels 
         */
        piems_dma_stall();

        mm->active_ch->num_stalls++;

        mm->swap_fails++;

        return;
    }

    piemux_dbg2( "%s from ch=%d rdy=%d use=%d to ch=%d rdy=%d use=%d\n", __FUNCTION__, 
            mm->active_ch->channel, mm->active_ch->num_readies,
            mm->active_ch->use_count,
            next_ch->channel, next_ch->num_readies,
            next_ch->use_count 
        );

    /* reset the channels' activity counter */
    next_ch->use_count = 0;
    mm->active_ch->use_count = 0;

    /* save PIE hardware context */
    regs = &mm->active_ch->regs;
//    regs->cyr = *PIE_SCALE_CURRDY;
//    regs->yper = *PIE_SCALE_CURRYPER;
    pie_get_context( regs );

    /* now the active channel */
    mm->active_ch = next_ch;

    /* load PIE hardware context */
    regs = &mm->active_ch->regs;
//    *PIE_SCALE_TY = regs->cyr;
//    *PIE_SCALE_INITYPER = regs->yper;
    pie_set_context( regs );

    /* swap the bank */
    piems_dma_set_bank( mm->active_ch->channel );

    /* pump PIE full of buffers */
    piems_dma_reload();

    /* fire in the hole! */
    piemux_launch( mm, mm->active_ch );

    mm->num_swaps++;

//    pie_rdma_dump();
//    pie_wdma_dump();
//    piems_rdma_debug_log();
//    piems_wdma_debug_log();
}

static void pie_swap( struct piemux_mm *mm)
{
    struct piemux_channel *next_ch;

//    dbg2( "%s\n", __FUNCTION__ );

    ASSERT( mm->active_ch );

    /* Find the best channel to run. */
    next_ch = next_channel_to_run(mm, NULL);
    if( next_ch ) {
        /* We found our best, runnable channel. Swap to it */
        pie_swap_to(mm,next_ch);
    }
}

scan_err_t piemux_scheduler( void )
{
    struct piemux_mm *mm;
//    struct piemux_channel *next_ch;

    mm = &piemux_mm;

    /* gently ignore calls if we're not open */
    if( !mm->active_ch ) {
        return SCANERR_NONE;
    }

    ASSERT( mm->active_ch );

    piemux_dbg2( "%s active=%d use=%d\n", __FUNCTION__, 
            mm->active_ch->channel, mm->active_ch->use_count );

    mm->num_scheduled++;

    /* if this channel has used up its alloted time slice, switch to another
     * channel (round robin)
     */
    if( mm->active_ch->use_count > mm->slice_count ) {
//        dbg1( "%s use=%d so swap\n", __FUNCTION__, mm->active_ch->use_count );
        pie_swap(mm);
        mm->num_timeslice_expired++;
        return SCANERR_NONE;
    }

#if 0
    /* if active channel is empty, switch to another channel */
    if( piems_rdma_is_idle() ) {
        /* is there a different channel with something to run? */
        next_ch = next_channel_to_run( mm, mm->active_ch );
        if( next_ch ) {
            dbg2( "%s pie idle so start next_ch=%d\n", __FUNCTION__,
                    next_ch->channel );
            pie_swap_to(mm, next_ch);
        }
        return SCANERR_NONE;
    }
#endif

    return SCANERR_NO_ENTRY;
}

void piemux_first_schedule( void )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel;
    struct pie_context *regs;

    mm = &piemux_mm;

    /* gently ignore calls if we're not open */
    if( !mm->active_ch ) {
        return;
    }

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        if( !ch->is_open ) {
            continue;
        }

        regs = &ch->regs;
        pie_get_context( regs );
    }
}

void piemux_add_ready( uint8_t pic_channel, struct ice_dma_buffer **p_icebuf )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    uint8_t pm_channel;

    /* use a temporary to mitigate pointer confusion */
    PTR_ASSIGN(icebuf,*p_icebuf);

    mm = &piemux_mm;

    if( !mm->active_ch ) {
        /* not using piemux; pass straight through to PIE */
        scerr = send_to_pie( &icebuf );
        XASSERT(scerr==SCANERR_NONE,scerr);
        return;
    }

    /* even pic channels are piemux channel 0
     *  odd pic channels are piemux channel 1 
     */
    pm_channel = pic_channel & 1;

    mm->add_readies++;

    ch = &mm->channels[ pm_channel ];
    XASSERT( ch->channel==pm_channel, pm_channel );
    XASSERT( ch->is_open, ch->channel );

    if( mm->active_ch->channel==pm_channel ) {
        /* this page is the active page */
        scerr = send_to_pie( &icebuf );
        XASSERT(scerr==SCANERR_NONE,scerr);

        ch->active_add_readies++;
    }
    else {
        ATInsertTailList( &ch->readies, &icebuf->listnode );
        ch->num_readies += 1;
        ch->idle_add_readies++;
    }

    piemux_scheduler();
}

scan_err_t piemux_open( void )
{
    scan_err_t scerr;
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel;

    dbg2( "%s\n", __FUNCTION__ );

    mm = &piemux_mm;

    /* make sure we're not already open */
    XASSERT( mm->active_ch==NULL, mm->active_ch->channel );

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        scerr = piemux_channel_open( ch, channel );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    /* assuming channel 0 is always opened */
    mm->active_ch = &mm->channels[0];

    /* TODO add API function to allow external code to tune this */
    mm->slice_count = 10;

    /* davep 07-Aug-2013 ; reset mm debug counters to zero */
    mm->num_swaps = 0;
    mm->swap_fails = 0;
    mm->num_scheduled = 0;
    mm->add_readies = 0;
    mm->num_timeslice_expired = 0;

    return SCANERR_NONE;
}

void piemux_close( void )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel, cnt;

    dbg2( "%s\n", __FUNCTION__ );

    mm = &piemux_mm;

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        if( !ch->is_open ) {
            continue;
        }

        piemux_channel_close( ch );
    }

    mm->active_ch = NULL;

    /* clean up */
    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];

        XASSERT( ch->num_readies==0, ch->num_readies );
        cnt = count_list( &ch->readies );
        XASSERT( cnt==ch->num_readies, cnt );

        channel_init( ch, channel );
    }
}

void piemux_onetime_init( void )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel;

    mm = &piemux_mm;

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        channel_init( ch, channel );
    }
}

void piemux_cancel( void )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel;

    mm = &piemux_mm;

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        if( !ch->is_open ) {
            continue;
        }

        piemux_channel_cancel( ch );
    }
}

void piemux_rdma_intmsg( uint8_t sensor_num, uint8_t pie_channel )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;

    mm = &piemux_mm;

    if( !mm->active_ch ) {
        /* pass straight through */
        pie_interrupt_disable();
        piems_rdma_intmsg( pie_channel ); 
        pie_interrupt_enable();
        return;
    }
         
    ch = mm->active_ch;
    XASSERT( ch->channel==sensor_num, sensor_num );

    /* A read strip has completed. */
    ch->use_count++;

    piemux_dbg2( "%s sensor=%d pie_channel=%d use=%d\n", __FUNCTION__, 
            sensor_num, pie_channel, ch->use_count );

    pie_interrupt_disable();
    piems_rdma_intmsg( pie_channel ); 
    pie_interrupt_enable();

    piemux_scheduler();
}

void piemux_wdma_intmsg( uint8_t sensor_num, uint8_t pie_channel )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;

    mm = &piemux_mm;

    if( !mm->active_ch ) {
        /* pass straight through */
        pie_interrupt_disable();
        piems_wdma_intmsg( pie_channel ); 
        pie_interrupt_enable();
        return;
    }

    ch = mm->active_ch;
    XASSERT( ch->channel==sensor_num, sensor_num );

    piemux_dbg2( "%s sensor=%d pie_channel=%d use=%d\n", __FUNCTION__, 
            sensor_num, pie_channel, ch->use_count );

    pie_interrupt_disable();
    piems_wdma_intmsg( pie_channel ); 
    pie_interrupt_enable();

    piemux_scheduler();
}

void piemux_dump( void )
{
    struct piemux_mm *mm;
    struct piemux_channel *ch;
    int channel;

    mm = &piemux_mm;

    if( !mm->active_ch ) {
        return;
    }

    dbg1( "piemux active=%p active_ch=%d\n", mm->active_ch, 
            (mm->active_ch?mm->active_ch->channel:-1) );
    dbg1( "piemux rdma_is_idle=%d wdma_is_idle=%d\n", 
            piems_rdma_is_idle(), piems_wdma_is_idle() );

    dbg1( "piemux scheds=%d swaps=%d swap_fails=%d add_readies=%d tsexpire=%d\n", 
            mm->num_scheduled, mm->num_swaps, mm->swap_fails, mm->add_readies,
            mm->num_timeslice_expired );

    for( channel=0 ; channel<PIEMUX_NUM_CHANNELS ; channel++ ) {
        ch = &mm->channels[channel];
        if( !ch->is_open ) {
            continue;
        }

        dbg1( "piemux ch=%d num_readies=%d use_count=%d idle_add=%d active_add=%d stalls=%d\n", 
                ch->channel, ch->num_readies, ch->use_count,
                ch->idle_add_readies, ch->active_add_readies, ch->num_stalls );
    }
}

