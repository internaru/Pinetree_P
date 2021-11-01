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

#include "scos.h"

#include "lassert.h"
#include "list.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "piehw.h"
#include "pie.h"

static struct ice_dma_mm pie_rdma_mm;
static struct ice_dma_mm pie_wdma_mm;
static struct ice_dma_channel pie_rdma_channels[PIE_RDMA_NUM_CHANNELS];
static struct ice_dma_channel pie_wdma_channels[PIE_WDMA_NUM_CHANNELS];

/*******************************/
/* 8-Apr-05; new style ICE DMA */
/* davep 16-May-2011 ; separate piedma into three: piedma.c (upper level
 * software) and piedma2005.c piedma_descrip.c (lower level hardware)
 */

/*
 * PIE Read DMA
 */

void pie_rdma_init( void ) 
{
    ice_dma_init( &pie_rdma_mm, "pierdma", 
            pie_rdma_get_driver(), 
            pie_rdma_channels, PIE_RDMA_NUM_CHANNELS);
}

int pie_rdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
    int i;
    scan_err_t scerr;

    /* davep 30-Aug-2005 ; make sure DMA requirements are met */
    XASSERT( !(bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK), bytes_per_row );

    for( i=0 ; i<num_channels ; i++ ) {
        scerr = pie_rdma_channel_open( channels[i] );
        if( scerr != SCANERR_NONE ) {
            /* count down, closing channels we already opened */
            i--;
            while( i>=0 ) {
                pie_rdma_channel_close( channels[i] );
                i--;
            }
            /* this function returns the number of buffers allocated; 0 for
             * error 
             */
            return 0;
        }
    }

    /* davep 16-May-2011 ; call me! */
    pie_rdma_set_icebuf_isr( pie_rdma_isr );

    return ice_dma_open( &pie_rdma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row, 
                    ICEBUF_TAG_PIERDMA );
}

scan_err_t pie_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &pie_rdma_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void pie_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    ice_dma_add_ready( &pie_rdma_mm, channel, addme );
}

void pie_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &pie_rdma_mm, channel, freeme );
}

void pie_rdma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &pie_rdma_mm, channel );
}

void pie_rdma_close( void )
{
    int i;
    struct ice_dma_channel *ch;

    for( i=0 ; i<pie_rdma_mm.num_channels ; i++ ) {
        ch = &pie_rdma_mm.channels[i];
        pie_rdma_channel_close( ch->channel );
    }

    /* davep 16-May-2011 ; don't call me anymore */
    pie_rdma_set_icebuf_isr( NULL );

    ice_dma_close( &pie_rdma_mm );
}

extern int dbg_cnt_pie_rdma_isr;	//add.lsh.for pipe debug
void pie_rdma_isr( uint8_t channel )
{
	dbg_cnt_pie_rdma_isr++;
    ice_dma_isr( &pie_rdma_mm, channel );
}

void pie_rdma_cancel( void )
{
    ice_dma_cancel( &pie_rdma_mm );
}

void pie_rdma_sanity( void )
{
    ice_dma_sanity( &pie_rdma_mm );
}

void pie_rdma_debug_log( void )
{
    ice_dma_debug_log( &pie_rdma_mm );
}

struct ice_dma_mm * pie_rdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &pie_rdma_mm;
}


/*
 * PIE Write DMA
 */

void pie_wdma_init( void ) 
{
    ice_dma_init( &pie_wdma_mm, "piewdma", 
            pie_wdma_get_driver(), 
            pie_wdma_channels, PIE_WDMA_NUM_CHANNELS);
}

int pie_wdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
    int i;
    scan_err_t scerr;

    /* davep 30-Aug-2005 ; make sure DMA requirements are met */
    XASSERT( !(bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK), bytes_per_row );

    for( i=0 ; i<num_channels ; i++ ) {
        scerr = pie_wdma_channel_open( channels[i] );
        if( scerr != SCANERR_NONE ) {
            /* count down, closing channels we already opened */
            i--;
            while( i>=0 ) {
                pie_wdma_channel_close( channels[i] );
                i--;
            }
            /* this function returns the number of buffers allocated; 0 for
             * error 
             */
            return 0;
        }
    }

    /* davep 16-May-2011 ; call me! */
    pie_wdma_set_icebuf_isr( pie_wdma_isr );

    return ice_dma_open( &pie_wdma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row,
                    ICEBUF_TAG_PIEWDMA );
}

scan_err_t pie_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &pie_wdma_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void pie_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    /* davep 21-Sep-2005 ; mark it empty and ready to be filled */
    (*addme)->num_rows = 0;

    ice_dma_add_ready( &pie_wdma_mm, channel, addme );
}

void pie_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &pie_wdma_mm, channel, freeme );
}


void pie_wdma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &pie_wdma_mm, channel );
}

void pie_wdma_close( void )
{
    int i;
    struct ice_dma_channel *ch;

    for( i=0 ; i<pie_wdma_mm.num_channels; i++ ) {
        ch = &pie_wdma_mm.channels[i];
        pie_wdma_channel_close( ch->channel );
    }

    /* davep 16-May-2011 ; don't call me anymore */
    pie_wdma_set_icebuf_isr( NULL );

    ice_dma_close( &pie_wdma_mm );
}

extern int dbg_cnt_pie_wdma_isr;	//add.lsh.for pipe debug
void pie_wdma_isr( uint8_t channel )
{
	dbg_cnt_pie_wdma_isr++;
    ice_dma_isr( &pie_wdma_mm, channel );
}

void pie_wdma_cancel( void )
{
    ice_dma_cancel( &pie_wdma_mm );
}

void pie_wdma_sanity( void )
{
    ice_dma_sanity( &pie_wdma_mm );
}

void pie_wdma_debug_log( void )
{
    ice_dma_debug_log( &pie_wdma_mm );
}

struct ice_dma_mm * pie_wdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &pie_wdma_mm;
}

void pie_wdma_refill( int rows_per_buffer )
{
    ice_dma_refill( &pie_wdma_mm, rows_per_buffer,
            pie_wdma_add_buffer, pie_wdma_channel_launch );
}


/**
 * \brief  allocate, initialize, and load buffers for PIE Write DMA
 *
 * \author David Poole
 * \date 01-Oct-2005
 *
 * Originally started life as pietest_setup_wdma_buffers(). Moved from test
 * code to mainline code on 1-Oct-05.
 *
 * davep 09-Dec-2010 ; moved to parent class with fct pointer for polymorphism
 */

int pie_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row )
{
    return ice_dma_setup_buffers( &pie_wdma_mm, channel, num_buffers,
                        rows_per_buffer, bytes_per_row, pie_wdma_add_buffer );
}

