/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Software clone of the Mono HalfPack driver code. Created as an interium
 * halftoning solution until the new hardware is ready.  (HalfPack was removed
 * in recent ASICs.)
 *
 * I gutted the register poking functions. Then I created a read and write
 * "dma" architecture to dither 8-bpp into 1/2/4 bpp.  There is significant
 * room for improvement in my dithering!
 *
 * davep 28-Jun-2010
 *  
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "list.h"           
#include "lassert.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "hwconfig_api.h"
#include "cpu_api.h"
#include "regAddrs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantask.h"
#include "scandbg.h"
#include "piehw.h"
#include "piehphw.h"
#include "safetylock.h"
#include "icedma.h"
#include "pie.h"
#include "piereset.h"
#include "scantools.h"
#include "scanvars.h"

/* if defined, PIEHP_DEBUG will add extra print messages to the functions */
//#define PIEHP_DEBUG

/* forward function decl's for the ice_dma_driver structures */
bool piehp_rdma_is_enabled( uint8_t channel );
bool piehp_wdma_is_enabled( uint8_t channel );

static struct ice_dma_driver piehp_rdma_driver = { 
    .msg_data = SMSG_PIEHP_RDMA_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      piehp_rdma_reset,
    .enable =     piehp_rdma_enable,
    .disable =    piehp_rdma_disable,
    .load =       piehp_rdma_load, 
    .start =      piehp_rdma_start,
    .is_enabled = piehp_rdma_is_enabled,
    .icebuf_isr = piehp_rdma_isr
};

static struct ice_dma_driver piehp_wdma_driver = { 
    .msg_data = SMSG_PIEHP_WDMA_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      piehp_wdma_reset,
    .enable =     piehp_wdma_enable,
    .disable =    piehp_wdma_disable,
    .load =       piehp_wdma_load, 
    .start =      piehp_wdma_start,
    .is_enabled = piehp_wdma_is_enabled,
    .icebuf_isr = piehp_wdma_isr
};

struct ice_dma_mm piehp_rdma_mm;
struct ice_dma_mm piehp_wdma_mm;
static struct ice_dma_channel piehp_rdma_channels[PIEHP_RDMA_NUM_CHANNELS];
static struct ice_dma_channel piehp_wdma_channels[PIEHP_WDMA_NUM_CHANNELS];

/* davep 28-Jun-2010 ; taking a shot at a software clone of HalfPack */
static uint32_t fake_PIE_HP_CFG, fake_PIE_HP_ALGO;

struct dma_transaction {
    /* these fields come from the _load() function */
    uint8_t *ptr;
    uint32_t num_rows;
    uint32_t bytes_per_row;

    /* the following field(s) are used for state */
    uint32_t row_number;
    int rows_remaining; /* signed so can catch < 0 */
    uint8_t *endptr;
};

#define RINGBUF_MAX_ELEMENTS (ICE_DMA_REG_DEPTH+1)

struct soft_halfpack_ringbuf {
    struct dma_transaction transaction[ RINGBUF_MAX_ELEMENTS ];
    int curridx, nextidx;
};

struct soft_halfpack_channel {
    uint8_t channel;
    struct soft_halfpack_ringbuf ringbuf;
    
    int num_running;
    int num_interrupt_pending;

    int total_rows;

    bool enabled;
};

static struct soft_halfpack_channel soft_halfpack_read_channels[ PIEHP_RDMA_NUM_CHANNELS ];
static struct soft_halfpack_channel soft_halfpack_write_channels[ PIEHP_WDMA_NUM_CHANNELS ];

/**
 * \brief Simple boogerhead ringbuf containing a structure.
 *
 * The ringbuf full/empty is determined using the 'ptr' field of the structure.
 *
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 */

static void ringbuf_init( struct soft_halfpack_ringbuf *rb )
{
    memset( rb, 0, sizeof(struct soft_halfpack_ringbuf) );
    rb->curridx = rb->nextidx = 0;
}

static int ringbuf_push( struct soft_halfpack_ringbuf *rb, 
                         struct dma_transaction *push_trans )
{
    struct dma_transaction *trans;

    trans = &rb->transaction[ rb->nextidx ];
    if( trans->ptr != NULL ) {
        /* ringbuf full */
        return -1;
    }

    memcpy( trans, push_trans, sizeof(struct dma_transaction) );
    rb->nextidx = (rb->nextidx+1) % RINGBUF_MAX_ELEMENTS;

    return 0;
}

static int ringbuf_pop( struct soft_halfpack_ringbuf *rb )
{
    struct dma_transaction *trans;

    trans = &rb->transaction[ rb->curridx ];
    if( trans->ptr == NULL ) {
        /* ringbuf empty */
        return -1;
    }

    trans->ptr = NULL; /* slot now empty */
    rb->curridx = (rb->curridx+1) % RINGBUF_MAX_ELEMENTS;

    /* davep 21-Apr-2011 ;  Note I am not returning a copy of the data. In the
     * soft_piehp use, I use ringbuf_get_head_ptr() and modify the head. When
     * the head is consumed, I simply pop it from the list.  See
     * soft_halfpack_start() below.
     */

    return 0;
}

static int ringbuf_get_head_ptr( struct soft_halfpack_ringbuf *rb, 
                                 struct dma_transaction **pp_head_trans )
{
    struct dma_transaction *trans;

    trans = &rb->transaction[ rb->curridx ];
    if( trans->ptr == NULL ) {
        /* ringbuf empty */
        return -1;
    }

    /* do not modify the ringbuf fields; just want a pointer to the head */
    *pp_head_trans = trans;

    /* davep 21-Apr-2011 ; I need to get a pointer to the head and NOT a copy
     * of the head. I must modify the transaction while it's still in the
     * ringbuf. See soft_halfpack_start() below.
     */

    return 0;
}

/**
 * \brief  Initialize our software "dma" channels
 *
 *
 * \author David Poole
 * \date 28-Jun-2010
 *
 */

static void soft_halfpack_open( uint8_t channels[], uint8_t num_channels, 
                            struct soft_halfpack_channel soft_halfpack_channels[] )
{
    int i;
    struct soft_halfpack_channel *sch;

    for( i=0 ; i<num_channels ; i++ ) {
        sch = &soft_halfpack_channels[i];
        memset( sch, 0, sizeof( struct soft_halfpack_channel ) );

        ringbuf_init( &sch->ringbuf );
        sch->channel = channels[i];
        sch->enabled = false;
    }
}

/**
 * \brief Load a DMA transaction into a channel ringbuf
 *
 * Called from the icedma driver functions piehp_wdma_load() and
 * piehp_rdma_load()
 *
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 */

static void soft_halfpack_load( struct soft_halfpack_channel *sch,
                            uint8_t *ptr, uint32_t num_rows, uint32_t bytes_per_row )
{
    int retcode;
    struct dma_transaction trans;

//    dbg2( "%s ch=%d ptr=%p nr=%d bpr=%d\n", __FUNCTION__, sch->channel, ptr, num_rows, bytes_per_row );

    memset( &trans, 0, sizeof(struct dma_transaction) );
    trans.ptr = ptr;
    trans.num_rows = num_rows;
    trans.bytes_per_row = bytes_per_row;
    trans.rows_remaining = num_rows;
    trans.row_number = sch->total_rows;
    /* used as a sanity check during soft_halfpack_run_dma() */
    trans.endptr = trans.ptr + trans.bytes_per_row * trans.num_rows;

    sch->total_rows += num_rows;

    retcode = ringbuf_push( &sch->ringbuf, &trans );
    XASSERT( retcode==0, sch->channel );

    sch->num_running += 1;
}

static int piehp_get_bit_depth( void )
{
    uint32_t n;

    /* grab two bits at <<2 */
    n = fake_PIE_HP_CFG & (0x03<<2);

//    dbg2( "%s %#x %d\n", __FUNCTION__, fake_PIE_HP_CFG, n );

    switch( n ) {
        case PIE_HP_CFG_BITDEPTH_1BIT :
            return 1;

        case PIE_HP_CFG_BITDEPTH_2BIT :
            return 2;

        case PIE_HP_CFG_BITDEPTH_4BIT :
            return 4;

        default:
            XASSERT(0,n);
            break;
    }
    return 0;
}


/**
 * \brief  Ordered dither.
 *
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 * http://www.efg2.com/Lab/Library/ImageProcessing/DHALF.TXT
 * http://www.imagemagick.org
 *
 * _Digital Halftoning_, Robert Ulichney 
 * # ISBN-10: 0262210096
 *
 */

/* 26 because we want to scale 0-255 into 0-9 */
static const uint8_t threshold_map_b[3][3] = { 
    { 3*26, 7*26, 4*26 },
    { 6*26, 1*26, 9*26 },
    { 2*26, 8*26, 5*26 }
};

//uint8_t threshold_map_b[3][3] = { 
//    { 3, 7, 4 },
//    { 6, 1, 9 },
//    { 2, 8, 5 }
//};

static const uint8_t bpp_to_black[] = {
    0x00, // 0
    0x01, // 1
    0x03, // 2
    0x07, // 3
    0x0f, // 4
    0x1f, // 5
    0x3f, // 6
    0x7f, // 7
    0xff, // 8
};

static void soft_halfpack_run_dma( struct dma_transaction *read_trans,
                                   struct dma_transaction *write_trans )
{
    int read_row_offset, write_row_offset;
    uint8_t *src, *dst, p, *end_dst, *end_src;
    int bits_per_pixel, pixels_per_row, pixels_per_byte;
    int x, y;
    uint8_t black; 
//    uint8_t white;

    bits_per_pixel = piehp_get_bit_depth();
    pixels_per_byte = 8/bits_per_pixel;
    pixels_per_row = write_trans->bytes_per_row * pixels_per_byte;

//    dbg2( "%s %d %d %d\n", __FUNCTION__, bits_per_pixel, pixels_per_byte, pixels_per_row );
//    dbg2( "%s src=%p dst=%p %d %d\n", __FUNCTION__, src, dst, read_trans->bytes_per_row , write_trans->bytes_per_row );

    cpu_dcache_invalidate_region( read_trans->ptr, read_trans->bytes_per_row*read_trans->num_rows );

//    white = 0;
    black = bpp_to_black[ bits_per_pixel ];

    /* as long as we have read or write data left, halftone it */
    while( read_trans->rows_remaining!=0 && write_trans->rows_remaining!=0 ) {

        write_row_offset = write_trans->num_rows - write_trans->rows_remaining;
        dst = write_trans->ptr + (write_trans->bytes_per_row * write_row_offset);
        XASSERT( dst < write_trans->endptr, (uint32_t)dst );
        write_trans->rows_remaining -= 1;

        read_row_offset = read_trans->num_rows - read_trans->rows_remaining;
        src = read_trans->ptr + (read_trans->bytes_per_row * read_row_offset);
        XASSERT( src < read_trans->endptr, (uint32_t)src );
        read_trans->rows_remaining -= 1;

        end_dst = dst + write_trans->bytes_per_row;
        end_src = src + read_trans->bytes_per_row;

//        dbg2( "%s src=%p dst=%p\n", __FUNCTION__, src, dst );

        y = read_trans->row_number + read_row_offset;

        p = 0;
        for( x=0 ; x<pixels_per_row ; x++ ) {
            if( *src++ < threshold_map_b[ x % 3 ][ y % 3] ) {
                p = (p<<bits_per_pixel) | black;
            }
            else {
                p = (p<<bits_per_pixel);
            }

            ASSERT( src<=end_src );

            /* pack the bits */
            if( (x+1)%pixels_per_byte == 0 ) {
                *dst++ = p;
                p = 0;
                ASSERT( dst<=end_dst );
            }
        }

        /* davep 12-Jun-2012 ; add 'invert' feature */
        if( fake_PIE_HP_ALGO & PIE_HP_ALGO_INVERT ) {
            /* invert the row we just dithered */
            dst = write_trans->ptr + (write_trans->bytes_per_row * write_row_offset);
            /* end_dst should not have changed */
            while( dst < end_dst ) {
                *dst = ~(*dst);
                dst++;
            }
        }

        /* do next row */
    }

    /* Thanks for dithering with us! Come back soon! */
}

/*
 * \brief Run a "dma" transaction.
 *
 * Will only do a "dma" if there is both a read and a write pending.
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 */

static void soft_halfpack_start( uint8_t channel )
{
    int retcode;
    struct soft_halfpack_channel *read_sch;
    struct soft_halfpack_channel *write_sch;
    struct dma_transaction *read_trans;
    struct dma_transaction *write_trans;

    read_sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( read_sch->channel==channel, channel );

    write_sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( write_sch->channel==channel, channel );

    while( read_sch->num_running && write_sch->num_running ) {
        /* Get a pointer to the head of the ringbuf. If we don't consume the
         * entire transation, the trans will remain on the list for the next go
         * round. If we do consume the entire transaction, pop it from the
         * list. 
         *
         * The incoming read/write buffers can have differing number of rows. 
         *
         * Bug 16720.
         */
        retcode = ringbuf_get_head_ptr( &read_sch->ringbuf, &read_trans );
        XASSERT( retcode==0, read_sch->channel );
        if( retcode ) {
            break;
        }

        retcode = ringbuf_get_head_ptr( &write_sch->ringbuf, &write_trans );
        XASSERT( retcode==0, write_sch->channel );
        if( retcode ) {
            break;
        }

        soft_halfpack_run_dma( read_trans, write_trans );

        if( read_trans->rows_remaining==0 ) {
//            dbg2( "%s %d pop read\n", __FUNCTION__, __LINE__ );
            /* we finished all the read data in this transaction */
            retcode = ringbuf_pop( &read_sch->ringbuf );
            XASSERT( retcode==0, read_sch->channel );
            read_sch->num_running -= 1;
            read_sch->num_interrupt_pending += 1;
        }

        if( write_trans->rows_remaining==0 ) {
//            dbg2( "%s %d pop write\n", __FUNCTION__, __LINE__ );
            /* we finished all the write data in this transaction */
            retcode = ringbuf_pop( &write_sch->ringbuf );
            XASSERT( retcode==0, write_sch->channel );
            write_sch->num_running -= 1;
            write_sch->num_interrupt_pending += 1;
        }
    }
}

/**
 * \brief check for pending "interrupts", buffers that are complete. 
 *
 * Can't call the piehp isr functions from within soft_halfpack_start() because
 * the icedma.c functions semaphore lock the data structures. We would wind up
 * calling the isr functions in icedma which would cause the lock to fail.
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 */

static void soft_halfpack_poll_interrupts( uint8_t channel )
{
    struct soft_halfpack_channel *read_sch;
    struct soft_halfpack_channel *write_sch;

    read_sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( read_sch->channel==channel, channel );

    write_sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( write_sch->channel==channel, channel );

    while( read_sch->num_interrupt_pending>0 ) {
        read_sch->num_interrupt_pending -= 1;
        piehp_rdma_isr( read_sch->channel );
    }

    while( write_sch->num_interrupt_pending>0 ) {
        write_sch->num_interrupt_pending -= 1;
        piehp_wdma_isr( write_sch->channel );
    }
}

/**************************************************
 * Function name   : piehp_reset
 *    returns      : none
 * Created by      : David Poole
 * Date created    : 11-Mar-05
 * Description     : 
 *
 *  Reset the PIE Mono HalfPack block to power-up defaults.
 *  
 * Notes           : 
 **************************************************/

void piehp_reset( void )
{
    dbg1("%s\n", __FUNCTION__ );
    
    fake_PIE_HP_ALGO = PIE_HP_ALGO_R;
    fake_PIE_HP_CFG = PIE_HP_CFG_R;

    /* reset the read/write DMA */
    /* 22-Apr-05 ; I commented these out a few weeks ago because they really
     * aren't necessary since the block level reset handles it.
     */
    /* 1-May-05 ; added back in so we can set registers back to power-on
     * defaults as well
     */
    piehp_rdma_reset(0);
    piehp_wdma_reset(0);

#ifdef PIEHP_DEBUG
    dbg2( "%s finished\n", __FUNCTION__ );
#endif
}

static void piehp_dma_run_forever( struct ice_dma_mm *mm )
{
    ice_dma_run_forever( mm );
}


/*
 *  PIE HalfPack Read DMA
 */

void piehp_rdma_reset( uint8_t channel )
{
#ifdef PIEHP_DEBUG
    dbg1("%s\n", __FUNCTION__ );
#endif

    /* no-op */
}

void piehp_rdma_start( uint8_t channel )
{
    soft_halfpack_start( channel );
}

void piehp_rdma_enable( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    sch->enabled = true;
}

bool piehp_rdma_is_enabled( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    return sch->enabled;
}

void piehp_rdma_disable( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    sch->enabled = false;
}

void piehp_rdma_dump( void )
{
    /* TODO */ 
}

void piehp_rdma_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_src, 
                        uint32_t lines, uint32_t bytes_per_line )
{
    struct soft_halfpack_channel *sch;

#ifdef PIEHP_DEBUG
    dbg1("%s lines=%ld bpl=%ld\n", __FUNCTION__, lines, bytes_per_line );
#endif

    sch = &soft_halfpack_read_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    soft_halfpack_load( sch, (uint8_t *)dma_src, lines, bytes_per_line );
}


/*
 *  PIE HalfPack Write DMA
 */

void piehp_wdma_reset( uint8_t channel )
{
#ifdef PIEHP_DEBUG
    dbg1("%s\n", __FUNCTION__ );
#endif

    /* no-op */
}

void piehp_wdma_start( uint8_t channel )
{
    soft_halfpack_start( channel );
}

void piehp_wdma_enable( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    sch->enabled = true;
}

bool piehp_wdma_is_enabled( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    return sch->enabled;
}

void piehp_wdma_disable( uint8_t channel )
{
    struct soft_halfpack_channel *sch;

    sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    sch->enabled = false;
}

void piehp_wdma_dump( void )
{
    /* TODO */ 
}

void piehp_wdma_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                        uint32_t lines, uint32_t bytes_per_line )
{
    struct soft_halfpack_channel *sch;

#ifdef PIEHP_DEBUG
    dbg1("%s lines=%ld bpl=%ld\n", __FUNCTION__, lines, bytes_per_line );
#endif

    sch = &soft_halfpack_write_channels[ channel ];
    XASSERT( sch->channel==channel, channel );

    soft_halfpack_load( sch, (uint8_t *)dma_dest, lines, bytes_per_line );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 6-July-05
// Description  : 
//
// 
//--------------------------------------------------------------------------
    

/*
 * PIE HalfPack Read DMA
 */

void 
piehp_rdma_init( void ) 
{
    ice_dma_init( &piehp_rdma_mm, "hprdma", &piehp_rdma_driver, 
            piehp_rdma_channels, PIEHP_RDMA_NUM_CHANNELS);
}

int 
piehp_rdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
    soft_halfpack_open( channels, num_channels, soft_halfpack_read_channels );

    return ice_dma_open( &piehp_rdma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row,
                    ICEBUF_TAG_HPRDMA );
}

scan_err_t piehp_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row)
        
{
    return ice_dma_add_buffer( &piehp_rdma_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void 
piehp_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme)
{
    ice_dma_add_ready( &piehp_rdma_mm, channel, addme );
}


void 
piehp_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &piehp_rdma_mm, channel, freeme );
}


void 
piehp_rdma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &piehp_rdma_mm, channel );
    soft_halfpack_poll_interrupts( channel );
}

void
piehp_rdma_close( void )
{
    ice_dma_close( &piehp_rdma_mm );
}

void
piehp_rdma_cancel( void )
{
    ice_dma_cancel( &piehp_rdma_mm );
}

void
piehp_rdma_sanity( void )
{
    ice_dma_sanity( &piehp_rdma_mm );
}

void 
piehp_rdma_debug_log( void )
{
    ice_dma_debug_log( &piehp_rdma_mm );
}

void
piehp_rdma_isr( uint8_t channel )
{
    ice_dma_isr( &piehp_rdma_mm, channel );
}

struct ice_dma_mm *
piehp_rdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &piehp_rdma_mm;
}

void piehp_rdma_run_forever( void )
{
    piehp_dma_run_forever( &piehp_rdma_mm );
}

/*
 * PIE HalfPack Write DMA
 */

void 
piehp_wdma_init( void ) 
{
    ice_dma_init( &piehp_wdma_mm, "hpwdma", &piehp_wdma_driver, 
            piehp_wdma_channels, PIEHP_WDMA_NUM_CHANNELS);
}

/**
 * \brief  Assign a scan_data_type to all channels' ice_dma_channel.
 *
 * The ice_dma_channel's dtype is assigned to incoming ice_dma_buffers.
 *
 * \author David Poole
 * \date 10-Apr-2013
 */

static void piehp_wdma_assign_channels_dtype( struct ice_dma_mm *mm,
                                              uint8_t channels[],
                                              uint8_t num_channels )
{
    struct ice_dma_channel *ch;
    const struct scanvars *sv;

    sv = scanvar_peek();

    /* davep 10-Apr-2013 ; as of this writing HalfPack (even the firmware
     * emulation) only has one channel
     */

    XASSERT(num_channels==1,num_channels);
    XASSERT(channels[0]==0, channels[0] );
    ch = &mm->channels[channels[0]];
    XASSERT(ch->channel==0, ch->channel );
    XASSERT( ch->is_open, ch->channel );
    
    XASSERT(sv->use_hp,sv->id);

    switch( sv->hp_bpp ) {
        case 1 :
            ch->dtype = SCAN_DATA_TYPE_HP1BPP;
            break;
        case 2 :
            ch->dtype = SCAN_DATA_TYPE_HP2BPP ;
            break;
        case 4 :
            ch->dtype = SCAN_DATA_TYPE_HP4BPP ;
            break;
        default :
            XASSERT(0, sv->hp_bpp);
    }
}

int 
piehp_wdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
    int retcode;

    soft_halfpack_open( channels, num_channels, soft_halfpack_write_channels );

    retcode = ice_dma_open( &piehp_wdma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row,
                    ICEBUF_TAG_HPWDMA );

    /* davep 09-Apr-2013 ; assign scan_data_type to channels */
    if( retcode > 0 ) {
        piehp_wdma_assign_channels_dtype( &piehp_wdma_mm, channels, num_channels );
    }

    return retcode;
}

scan_err_t piehp_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row)
{
    return ice_dma_add_buffer( &piehp_wdma_mm, 
            channel, data, datalen, rows, bytes_per_row);
}

void 
piehp_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme)
{
    /* davep 21-Sep-2005 ; mark it empty and ready to be filled */
    (*addme)->num_rows = 0;

    ice_dma_add_ready( &piehp_wdma_mm, channel, addme);
}


void 
piehp_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &piehp_wdma_mm, channel, freeme );
}

void 
piehp_wdma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &piehp_wdma_mm, channel );
    soft_halfpack_poll_interrupts( channel );
}

void
piehp_wdma_close( void )
{
    ice_dma_close( &piehp_wdma_mm );
}

void
piehp_wdma_cancel( void )
{
    ice_dma_cancel( &piehp_wdma_mm );
}

void
piehp_wdma_sanity( void )
{
    ice_dma_sanity( &piehp_wdma_mm );
}

void 
piehp_wdma_debug_log( void )
{
    ice_dma_debug_log( &piehp_wdma_mm );
}

void
piehp_wdma_isr( uint8_t channel )
{
    ice_dma_isr( &piehp_wdma_mm, channel );
}

struct ice_dma_mm *
piehp_wdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &piehp_wdma_mm;
}

void 
piehp_wdma_refill( int rows_per_buffer )
{
    ice_dma_refill( &piehp_wdma_mm, rows_per_buffer,
            piehp_wdma_add_buffer, piehp_wdma_channel_launch );
}

void piehp_wdma_run_forever( void )
{
    piehp_dma_run_forever( &piehp_wdma_mm );
}

/************************
 * End of DMA Functions *
 ************************/

/**
 * \brief dump the PIE HalfPack registers 
 *
 * \author David Poole
 * \date 21-Oct-2005
 *
 */

void
piehp_dump( void )
{
}

/**
 * \brief copy a set of values into the HalfPack Size LUT
 *
 * \param size_values
 * \param num_values
 *
 * \author Brad Smith 
 *
 * David Poole 29-Dec-05 ; added num_values parameter, added assert on
 * num_values
 *
 **/
void
piehp_set_size_sram(uint32_t *size_values, int num_values )
{
}

/**
 * \brief copy a set of values into the HalfPack Signed BIAS LUT
 *
 * \param signed_bias_values
 * \param num_values
 *
 * \author Brad Smith 
 *
 * David Poole 29-Dec-05 ; added num_values parameter, added assert on
 * num_values
 *
 **/
void
piehp_set_signed_bias_sram(uint32_t *signed_bias_values, int num_values )
{
}

/**
 * \brief copy a set of values into the HalfPack Density LUT
 *
 * \param density_values
 * \param num_values
 *
 * \author Brad Smith 
 *
 * David Poole 29-Dec-05 ; added num_values parameter, added assert on
 * num_values
 *
 **/
void
piehp_set_density_sram(uint32_t *density_values, int num_values )
{
}

/**
 * \brief copy a set of values into the HalfPack Error Diffusion LUT
 *
 * \param error_diff_values
 * \param num_values
 *
 * \author Brad Smith 
 *
 * David Poole 29-Dec-05 ; added num_values parameter, added assert on
 * num_values
 **/

void
piehp_set_error_diffusion_sram(uint32_t *error_diff_values, int num_values )
{
}

/**
 * \brief  initialize the unified Size, Error Diffusion, Density table.
 *
 * Instead of having three separate SRAMs, one for each LUT, the three were
 * combined into one LUT.
 *
 * \author David Poole
 * \date 23-Nov-2007
 *
 */

void
piehp_set_szdd_sram( uint32_t *size_values, uint32_t *error_diff_values, 
                     uint32_t *density_values, int num_values )
{
}

/**
 * \brief  set the error diffusion pixel mode
 *
 * We added the option of doing error diffusion on full pixels instead of
 * pixel pairs. No more "pixels come in pears" jokes, I guess. Sad...
 *
 *
 * \author David Poole
 * \date 24-Nov-2007
 *
 */

void
piehp_set_error_diffusion_mode( uint32_t mode )
{
#ifdef HAVE_HALFPACK_FULL_PIXEL_DIFFUSION
    if( mode==PIE_HP_CFG_PAIRED_ERRDIFF ) {
        /* clear the "normal mode" config register bit, putting HalfPack into paired
         * pixel error diffusion mode
         */
        fake_PIE_HP_CFG &= ~PIE_HP_CFG_NORMAL_ERRDIFF; 
    }
    else if( mode==PIE_HP_CFG_NORMAL_ERRDIFF ) {
        /* set the config register bit, putting HalfPack into non-paired (single
         * pixel) error diffusion mode 
         */
        fake_PIE_HP_CFG |= PIE_HP_CFG_NORMAL_ERRDIFF; 
    }
    else {
        XASSERT( 0, mode );
    }
#else
    /* as of this writing, this bit doesn' exist on other ASICs */
    XASSERT( 0, mode );
#endif
}

/**
 * \brief set or clear the Write DMA direction (left->right or right->left) bit
 *
 * \param dir
 *
 * \author Brad Smith 
 *
 **/
void
piehp_set_wdma_direction(HP_DMA_DIR dir)
{
}

/**
 * \brief set or clear the Read DMA direction (left->right or right->left) bit
 *
 * \param dir
 *
 * \author Brad Smith 
 *
 **/
void
piehp_set_rdma_direction(HP_DMA_DIR dir)
{
}

/**
 * \brief set/clear the HalfPack "start of image" flag
 *
 * Internally, when the "start of image" flag is set, HalfPack will clear out
 * internal data from the previous image. Data built up across rows such as the
 * error diffusion will be cleared.
 *
 * I (DP) am not entirely sure of all the internal details. In a nutshell, on
 * the first strip of a new image, set the flag. All subsequent strips, clear
 * the flag.
 * 
 * \author Brad Smith 
 * \author David Poole
 *
 **/
void
piehp_set_start_of_image( bool enable )
{
}

/**
 * Function name: piehp_set_replicate_vert_data
 *
 * \brief
 *
 * \param replicate
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_replicate_vert_data(bool replicate)
{
}

/**
 * Function name: piehp_set_replicate_horiz_data
 *
 * \brief
 *
 * \param replicate
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_replicate_horiz_data(bool replicate)
{
    if (replicate)
        fake_PIE_HP_CFG |= PIE_HP_CFG_HORIZ_REPL;
    else
        fake_PIE_HP_CFG &= ~PIE_HP_CFG_HORIZ_REPL;
}

/**
 * Function name: piehp_set_bit_depth
 *
 * \brief
 *
 * \param depth
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_bit_depth(int depth)
{
    /* clear old flags */
    fake_PIE_HP_CFG = fake_PIE_HP_CFG & ~(0x03<<2);

    switch (depth)
    {
        case 1:
            fake_PIE_HP_CFG |= PIE_HP_CFG_BITDEPTH_1BIT;
            break;

        case 2:
            fake_PIE_HP_CFG |= PIE_HP_CFG_BITDEPTH_2BIT;
            break;

        case 4:
            fake_PIE_HP_CFG |= PIE_HP_CFG_BITDEPTH_4BIT;
            break;

        default:
            XASSERT(0,depth);
            break;
    }

    dbg2( "%s depth=%d %#x\n", __FUNCTION__, depth, fake_PIE_HP_CFG );

}

/**
 * Function name: piehp_set_bypass
 *
 * \brief
 *
 * \param bypass
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_bypass(bool bypass)
{
    if (bypass)
        fake_PIE_HP_CFG |= PIE_HP_CFG_BYPASS;
    else
        fake_PIE_HP_CFG &= ~PIE_HP_CFG_BYPASS;
}

/**
 * Function name: piehp_set_invert_data
 *
 * \brief
 *
 * \param invert
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_invert_data(bool invert)
{
    dbg2( "%s invert=%d\n", __FUNCTION__, (int)invert );

    if (invert)
        fake_PIE_HP_ALGO |= PIE_HP_ALGO_INVERT;
    else
        fake_PIE_HP_ALGO &= ~PIE_HP_ALGO_INVERT;
}

/**
 * Function name: piehp_set_include_accumerror_idx
 *
 * \brief
 *
 * \param include
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_include_accumerror_idx(bool include)
{
    if (include)
        fake_PIE_HP_ALGO |= PIE_HP_ALGO_ACCUM_ERR;
    else
        fake_PIE_HP_ALGO &= ~PIE_HP_ALGO_ACCUM_ERR;
}

/**
 * Function name: piehp_set_include_signed_bias_idx
 *
 * \brief
 *
 * \param include
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_include_signed_bias_idx(bool include)
{
    if (include)
        fake_PIE_HP_ALGO |= PIE_HP_ALGO_BIAS_IDX;
    else
        fake_PIE_HP_ALGO &= ~PIE_HP_ALGO_BIAS_IDX;
}

/**
 * Function name: piehp_set_include_signed_bias_do_a_dot
 *
 * \brief
 *
 * \param include
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_include_signed_bias_do_a_dot(bool include)
{
    if (include)
        fake_PIE_HP_ALGO |= PIE_HP_ALGO_BIAS_DOT;
    else
        fake_PIE_HP_ALGO &= ~PIE_HP_ALGO_BIAS_DOT;
}

/**
 * Function name: piehp_set_do_a_dot_override
 *
 * \brief
 *
 * \param do_a_dot
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_do_a_dot_override(bool do_a_dot)
{
    if (do_a_dot)
        fake_PIE_HP_ALGO |= PIE_HP_ALGO_DO_A_DOT;
    else
        fake_PIE_HP_ALGO &= ~PIE_HP_ALGO_DO_A_DOT;
}

/**
 * Function name: piehp_set_shift
 *
 * \brief
 *
 * \param shift
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_shift(uint8_t shift)
{
    switch (shift)
    {
        case 4:
            fake_PIE_HP_ALGO |= PIE_HP_SHIFT_VALUE(0);
            break;
        case 5:
            fake_PIE_HP_ALGO |= PIE_HP_SHIFT_VALUE(1);
            break;
        case 6:
            fake_PIE_HP_ALGO |= PIE_HP_SHIFT_VALUE(2);
            break;
        case 7:
            fake_PIE_HP_ALGO |= PIE_HP_SHIFT_VALUE(3);
            break;
        default:
            XASSERT(0,shift);
    }
}
/**
 * Function name: piehp_set_error_weight_1
 *
 * \brief
 *
 * \param weight
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_error_weight_1(uint8_t weight)
{
    fake_PIE_HP_ALGO |= PIE_HP_ALGO_ERR_WT_1(weight);
}

/**
 * Function name: piehp_set_error_weight_2
 *
 * \brief
 *
 * \param weight
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_error_weight_2(uint8_t weight)
{
    fake_PIE_HP_ALGO |= PIE_HP_ALGO_ERR_WT_2(weight);
}

/**
 * Function name: piehp_set_error_weight_3
 *
 * \brief
 *
 * \param weight
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_error_weight_3(uint8_t weight)
{
    fake_PIE_HP_ALGO |= PIE_HP_ALGO_ERR_WT_3(weight);
}

/**
 * Function name: piehp_set_bias_sram_config
 *
 * \brief
 *
 * \param config
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_bias_sram_config(HP_BIAS_CONFIG config)
{
}

/**
 * Function name: piehp_set_bias_reg0
 *
 * \brief
 *
 * \param value
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_bias_reg0(uint8_t value)
{
}

/**
 * Function name: piehp_set_bias_reg1
 *
 * \brief
 *
 * \param value
 *
 * \author Brad Smith 
 *
 * Notes: 
 *
 **/
void
piehp_set_bias_reg1(uint8_t value)
{
}

/**
 * \brief calculate bytes per row for DMA channel read and write 
 *
 * Takes into account the 16-byte DMA alignment requirement.
 *
 * \author David Poole
 * \date 30-Aug-2005
 *
 * 14-Oct-05 davep ; added support for horizontal replication
 */

void
piehp_calc_row( int pixels_per_row_in,
                int bpp,
                bool horiz_rep,
                struct pie_dma_row *row )
{
    int hp_pad_bytes;
    int pixels_per_byte;

    dbg2( "%s ppr=%d bpp=%d\n", __FUNCTION__, pixels_per_row_in, bpp );

    /* DMA must line up on a 16-byte boundry. HalfPack can have X pixels per byte
     * based on bpp:
     *
     * bpp   ppb   input pad bytes required 
     * ---   ---   ------------------------
     *  1     8    16 * 8 = 128  (8:1 - every input byte becomes 1 bit)
     *  2     4    16 * 4 = 64   (4:1 - every input byte becomes 2 bits)
     *  4     2    16 * 2 = 32   (2:1 - every input byte becomes 4 bits)
     *
     *  ppb = pixels per byte
     */

    /* Based on the bpp out (1,2,4), set up the number of pixels per row
     * that will be sent into the block to make the bytes per row coming
     * out of the block 16-byte DMA aligned.
     */
    switch( bpp ) {
        case 1 :
            hp_pad_bytes = 128;
            break;

        case 2 :
            hp_pad_bytes = 64;
            break;

        case 4 :
            hp_pad_bytes = 32;
            break;

        default:
            XASSERT( 0, bpp );
            hp_pad_bytes = 128;
            break;
    }

    /* davep 14-Apr-2008 ; adding support for 32-byte DMA bursts
     *
     * bpp   ppb   input pad bytes required 
     * ---   ---   ------------------------
     *  1     8    32 * 8 = 256  (8:1 - every input byte becomes 1 bit)
     *  2     4    32 * 4 = 128  (4:1 - every input byte becomes 2 bits)
     *  4     2    32 * 2 = 64   (2:1 - every input byte becomes 4 bits)
     */
#if ICE_DMA_BYTE_ALIGN==32
    hp_pad_bytes *= 2;
#endif

    pixels_per_byte = 8 / bpp;

    if( horiz_rep ) {
        /* if we're doing horizontal replication, we're 1:2 so we'll need less
         * pad, among other things.
         */
        hp_pad_bytes /= 2;

        /* round up for DMA */
        row->pixels_out = ((pixels_per_row_in*2 + (hp_pad_bytes-1)) / hp_pad_bytes) * hp_pad_bytes;
        row->bytes_out = row->pixels_out / pixels_per_byte;

        /* we'll get twice as many pixels out as we push in */
        row->pixels_in = row->pixels_out / 2;
        /* HalfPack read DMA is 8bpp */
        row->bytes_in = row->pixels_in;
    }
    else {
        /* HalfPack pixels are 1:1 so the number of pixels in is the number of pixels out
         * adjusted to match the DMA 16-byte requirement
         */

        /* round up for DMA */
        row->pixels_out = ((pixels_per_row_in + (hp_pad_bytes-1)) / hp_pad_bytes) * hp_pad_bytes;
        row->bytes_out = row->pixels_out / pixels_per_byte;

        row->pixels_in = row->pixels_out;
        /* HalfPack read DMA is 8bpp */
        row->bytes_in = row->pixels_in;
    }

    /* someone will have to use PIE's XYScale to pad pixels out of PIE so we get the
     * right number of pixels to feed into HalfPack 
     */

    /* sanity check my math */
    XASSERT( row->bytes_in % ICE_DMA_BYTE_ALIGN == 0, row->bytes_in );
    XASSERT( row->bytes_out % ICE_DMA_BYTE_ALIGN == 0, row->bytes_out );
    XASSERT( row->pixels_out % hp_pad_bytes == 0, row->pixels_out );
}

int piehp_wdma_setup_buffers( uint8_t channel, 
                        int num_buffers, 
                        int rows_per_buffer, 
                        int bytes_per_row )
{
    return ice_dma_setup_buffers( &piehp_wdma_mm, channel, num_buffers,
                        rows_per_buffer, bytes_per_row, piehp_wdma_add_buffer );
}

