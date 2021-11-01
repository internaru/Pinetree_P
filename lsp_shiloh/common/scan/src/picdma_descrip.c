/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file picdma_descrip.c
 *
 * \brief PIC Write DMA driver for chained descriptor based DMA hardware
 *
 * davep 28-Mar-2010 ; Added new descriptor based code
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "regAddrs.h"
#include "debug.h"
#include "lassert.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "list.h"
#include "intnums.h"
#include "interrupt_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"
#include "pichw.h"
#include "pic.h"
#include "piccbi.h"
#include "picdma_descrip.h"
#include "scanalyzer.h"

//#define PIC_WDMA_DEBUG
//#define PIC_WDMA_ISR_DEBUG

#ifdef PIC_WDMA_ISR_DEBUG
#define isr_dbg2 dbg2
#else
#define isr_dbg2(...)
#endif

//#define MAX_CWDMA_DESCRIPTORS 512
//#define MAX_CWDMA_DESCRIPTORS 128
//#define MAX_CWDMA_DESCRIPTORS 32
#define MAX_CWDMA_DESCRIPTORS 64
//#define MAX_CWDMA_DESCRIPTORS 16

struct pic_wdma_interrupt_stats {
    uint32_t count;
    uint32_t transfer_end; 
    uint32_t end_of_image;
    uint32_t reset; 

    /* increment counter when we find wdma block idle during channel ISR */
    uint32_t is_idle;
};

static struct pic_wdma_interrupt_stats int_stats[PIC_WDMA_NUM_CHANNELS];

/* array of pointers to our hardware control registers */
static struct pic_cwdma_regs * const dma_registers[ PIC_WDMA_NUM_CHANNELS ] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA0_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA1_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA2_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA3_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA4_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA5_BASE),
#else
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA0_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA1_BASE),
    (struct pic_cwdma_regs *)(ICE_PIC_PIC_WDMA_DMA2_BASE),
#endif
};

static struct ddma_channel desc_channel_list[PIC_WDMA_NUM_CHANNELS];

static struct ice_dma_driver pic_cwdma_driver = { 
    .msg_data  = SMSG_PIC_WDMA_DATA,

    /* set the queue depth to a number which, I hope, will keep our chained
     * descriptors far enough ahead of the running DMA
     */
    .max_queue_depth = MAX_CWDMA_DESCRIPTORS-1,
//    .max_queue_depth = 10,

    .reset =      pic_wdma_channel_reset,
    .enable =     pic_wdma_channel_enable,
    .disable =    pic_wdma_channel_disable,
    .load =       pic_wdma_channel_load, 
    .start =      pic_wdma_channel_start,   /* start; can't use because called for each load */
    .is_enabled = pic_wdma_channel_is_enabled,
    .icebuf_isr = NULL,
};

static struct ice_dma_mm pic_cwdma_mm;
static struct ice_dma_channel pic_cwdma_channels[PIC_WDMA_NUM_CHANNELS];

static struct pic_cwdma_regs * getregs( uint8_t channel )
{
    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel );
    return dma_registers[ channel ];
}

//#define getregs(channel) dma_registers[channel]

struct pic_cwdma_regs *pic_cwdma_get_regs( uint8_t channel ) 
{
    return getregs(channel);
}

uint32_t pic_cwdma_status_get( uint8_t channel )
{
    return getregs(channel)->status;
}

void pic_cwdma_interrupt_stats_reset( void )
{
    /* Note: neither thread safe nor interrupt safe */
    memset( int_stats, 0, sizeof(int_stats) );
}

void pic_cwdma_interrupt_stats_dump( void )
{
    int channel_idx;

    /* Note: neither thread safe nor interrupt safe */

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dbg2( "[%d] total=%d FIN=%d EOI=%d RST=%d idle=%d\n", channel_idx,
                    int_stats[channel_idx].count,
                    int_stats[channel_idx].transfer_end,
                    int_stats[channel_idx].end_of_image,
                    int_stats[channel_idx].reset,
                    int_stats[channel_idx].is_idle );
    }
}

static void channel_dump( struct pic_cwdma_regs *dma_regs )
{
    dbg2( "     cfg=0x%08x    status=0x%08x line_size=0x%08x\n", 
                dma_regs->cfg, dma_regs->status, dma_regs->line_size );
    dbg2( "  int_en=0x%08x  int_pend=0x%08x desc_read=%08x\n", 
                dma_regs->int_en, dma_regs->int_pend, dma_regs->desc_read );
    dbg2( "xfer_len=0x%08x  xferaddr=%08x    xferburs=0x%08x  data_val=%08x\n", 
                dma_regs->xfer_length, dma_regs->xfer_addr, 
                dma_regs->xfer_burst, dma_regs->data_val );
}

static void channel_enable( struct pic_cwdma_regs *dma_regs ) 
{
    dma_regs->cfg |= PIC_WDMA_CFG_ENABLE;

    /* turn on all interrupts */
    dma_regs->int_en = ~0;

//    dma_regs->int_en |= (PIC_WDMA_INT_EN_FIN |
//                         PIC_WDMA_INT_EN_WHO |
//                         PIC_WDMA_INT_EN_EOI );
}

static bool channel_is_enabled( struct pic_cwdma_regs *dma_regs )
{
    return (dma_regs->cfg & PIC_WDMA_CFG_ENABLE) && true;
}

static void channel_disable( struct pic_cwdma_regs *dma_regs )
{
    dma_regs->cfg &= ~PIC_WDMA_CFG_ENABLE;
    dma_regs->int_en = 0;
}

static void channel_reset( struct pic_cwdma_regs *dma_regs )
{
    uint32_t sanity;

    /* disable all interrupts */
    dma_regs->int_en = 0;
    /* clear any pending interrupts */
    dma_regs->int_ack = ~0;

    XASSERT( dma_regs->int_pend==0, dma_regs->int_pend );

    /* davep 13-Jan-2011 ; channel must be enabled for the reset to lead to a
     * Soft Reset Complete interrupt 
     */
    channel_enable( dma_regs );

    dma_regs->reset |= PIC_WDMA_RESET_SOFT_RESET;
    cpu_spin_delay( 10 );  /* wait a little while to allow hw to reset */
    dma_regs->reset &= ~PIC_WDMA_RESET_SOFT_RESET;

    /* davep 01-Oct-2010 ; wait for reset to clear (shotgun debug) */
    sanity = 0;
    /* davep 29-Jun-2011 ; wait for Soft Reset complete interrupt */
    while( !(dma_regs->int_pend & PIC_WDMA_INT_PEND_RST) ) {
        cpu_spin_delay( 10 );  /* wait a little while to allow hw to reset */
        sanity += 1;
        channel_dump( dma_regs );
        XASSERT( sanity<100 , dma_regs->status );
    }

    /* clear any pending interrupts */
    dma_regs->int_ack = ~0;

    XASSERT( dma_regs->int_pend==0, dma_regs->int_pend );

//    channel_dump( dma_regs );

    /* reset to power-on defaults */
    dma_regs->cfg = PIC_WDMA_CFG_R;

    /* Restart xfer count to help multiple channels be synchronized to the same point */
    dma_regs->xfer_length = 0;

    /* davep 22-Jun-2012 ; need this to be zero so we can initialize it once
     * and only once 
     */
    dma_regs->line_size = 0;

//    XASSERT( dma_regs->int_pend==0, dma_regs->int_pend );
}

/**
 * \brief Enable ODMA line_rev (line reverse) mode
 *
 * In line_rev mode, DMA is right-to-left instead of left-to-right.
 *
 * We use this to flip data from systems where paper is backwards. For example,
 * on certain ADFs, paper is pulled across the sensor backwards giving an image
 * mirrored about the Y axis. line_rev will flip image for us.
 *
 * \author David Poole
 * \date 21-Feb-2012
 */

void pic_wdma_enable_line_reverse( bool enable ) 
{
    int i;
    struct pic_cwdma_regs *dma_regs;

#ifdef PIC_WDMA_DEBUG
    dbg2( "%s enable=%d\n", __FUNCTION__, enable );
#endif

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        dma_regs = getregs(i);

        if( enable ) {
            dma_regs->cfg |= PIC_WDMA_CFG_LINE_REV;
        }
        else {
            dma_regs->cfg &= ~PIC_WDMA_CFG_LINE_REV;
        }
    }
}

void pic_wdma_enable_line_reverse_by_channel( uint8_t channel, bool enable ) 
{
    struct pic_cwdma_regs *dma_regs;

#ifdef PIC_WDMA_DEBUG
    dbg2( "%s enable=%d\n", __FUNCTION__, enable );
#endif

    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel ) ;

    dma_regs = getregs(channel);

    if( enable ) {
        dma_regs->cfg |= PIC_WDMA_CFG_LINE_REV;
    }
    else {
        dma_regs->cfg &= ~PIC_WDMA_CFG_LINE_REV;
    }
}

void pic_wdma_channel_dump( uint8_t channel )
{
    struct ddma_channel *dch;

    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel ) ;

    channel_dump( getregs( channel ) );

    dch = &desc_channel_list[channel];

    if( dch->is_open ) {
        ddma_channel_sanity( dch );
//        ddma_descriptor_list_dump( &dch->desc_list);
    }
}

void pic_wdma_channel_reset( uint8_t channel )
{
    channel_reset( getregs( channel ) );
}

void pic_wdma_channel_start( uint8_t channel )
{
    /* no op; see also pic_cwdma_late_launch() */
}

/**
 * \brief brute force jam a descriptor into a channel
 *
 * Created for some test code. Might be useful to this driver itself someday?
 *
 * \author David Poole
 * \date 29-Mar-2011
 */

void pic_ddma_desc_write( uint8_t channel, struct ddma_descriptor *desc, uint32_t line_width_bytes )
{
    struct pic_cwdma_regs *dma_regs;

    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel );

    dma_regs = dma_registers[channel];

    /* is this thing on? */
    XASSERT( dma_regs->cfg & PIC_WDMA_CFG_ENABLE, dma_regs->cfg );

//    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK(desc), desc->config_flags );

    ddma_desc_flush( desc );

    /* fire in the hole! */
    dma_regs->line_size = line_width_bytes;

    /* davep 25-Apr-2012 ; adding vma/pma destinction */
    XASSERT( desc->dma_ptr_self, (uint32_t)desc );
    ASSERT( desc->dma_ptr_self );
    ASSERT( desc->src_addr );

    dma_regs->desc_write = (uint32_t)desc->dma_ptr_self;
}

static void channel_late_launch( struct ddma_channel *dch )
{
    struct pic_cwdma_regs *dma_regs;

#ifdef PIC_WDMA_DEBUG
    if( !IN_INTERRUPT() ) {
        dbg2("%s ch=%d running=%d\n", __FUNCTION__, dch->channel,
                dch->num_running );
    }
#endif

    /* davep 13-Oct-2010 ; oy vey
     *
     * The new descriptor DMAs behave different than the old register DMAs. The
     * previous register DMAs (e.g., picdma2005.c) required a load-start (push
     * dest addr then push start) for each buffer.  
     *
     * The new descriptor DMAs use a chain of data structures in main memory.
     * A load pushes the start of the chain, a start starts the chain. However,
     * our icedma still wants a load-start for each buffer. (See
     * channel_launch()-icedma.c)
     * 
     * The descriptor DMAs will use the load to add the buffer to the end of
     * the descriptor chain. The 'start' will do nothing. This 'late launch'
     * will be called by scan_cmdq_launch(). The scan_cmdq_launch() function is
     * called once all buffers are loaded, cmds are queued into the scandmq.
     * scan_cmdq_launch() calling pic_cwdma_late_launch() is a kludge but a
     * brilliant kludge suggested by BurtP.
     */

    dma_regs = dma_registers[dch->channel];

    XASSERT( dma_regs->cfg & PIC_WDMA_CFG_ENABLE, dma_regs->cfg );

    if( dma_regs->status & PIC_WDMA_STATUS_DMA_BUSY ) {

//#ifdef PIC_WDMA_DEBUG
//        if( !IN_INTERRUPT() ) {
//            dbg2("%s ch=%d already running\n", __FUNCTION__, channel );
//        }
//#endif

        /* DMA already running, no need to start it again */
        return;
    }

    XASSERT( dch->num_running > 0, dch->num_running );
    XASSERT( dch->head_desc != NULL, dch->channel );

    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK(dch->head_desc), dch->head_desc->config_flags );

    /* fire in the hole! */
    /* davep 25-Apr-2012 ; adding vma/pma destinction */
    XASSERT( dch->head_desc->dma_ptr_self, (uint32_t)dch->head_desc );
    dma_regs->desc_write = (uint32_t)dch->head_desc->dma_ptr_self;
    
#ifdef PIC_WDMA_DEBUG
//    if( 1 ) {
    if( !IN_INTERRUPT() ) {

        dbg2( "%s %p\n", __FUNCTION__, dch->head_desc );

        channel_dump( dma_regs ); 
    }
#endif
}

void pic_cwdma_late_launch( void )
{
    int channel_idx;
    struct ddma_channel *dch;

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dch = &desc_channel_list[channel_idx];
        if( !dch->is_open ) {
            continue;
        }
        channel_late_launch( dch );
    }

}
    
void pic_wdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t num_rows, uint32_t bytes_per_row )
{
    scan_err_t scerr;
    struct ddma_channel *dch;
    struct ddma_descriptor *next;
    struct pic_cwdma_regs *dma_regs;
    dma_addr_t dma_handle;

    /* 
     * Warning! Can be called from interrupt context! 
     */

#ifdef PIC_WDMA_DEBUG
    if( !IN_INTERRUPT() ) {
        dbg2("%s ch=%d 0x%x %d %d\n", __FUNCTION__,
                    channel, dma_dest, num_rows, bytes_per_row );
    }
#endif

    dch = &desc_channel_list[channel];
    XASSERT( dch->channel==channel, dch->channel );

    XASSERT( ICE_DMA_IS_ALIGNED((uint32_t)dma_dest), (uint32_t)dma_dest );
    XASSERT( ICE_DMA_IS_ALIGNED(bytes_per_row), bytes_per_row );

    /* davep 05-Jun-2012 ; add channel data structure protection */
    scerr = ddma_channel_try_lock(dch);
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* poke the bytes_per_row into the line_width (should be the same for all
     * load calls for this scan but I won't know the value until load is
     * called)
     */
    /* davep 22-Jun-2012 ; this scares me; troubleshooting this crazy PIC
     * overflow and reading code, trying to find anything hinky. Let's not
     * touch the DMA hardware while it's running.
     */
    dma_regs = dma_registers[dch->channel];
    if( dma_regs->line_size==0 ) {
        XASSERT( !(dma_regs->status & PIC_WDMA_STATUS_DMA_BUSY), dma_regs->status );
        dma_regs->line_size = bytes_per_row;
    }
    else { 
        XASSERT( dma_regs->line_size==bytes_per_row, bytes_per_row );
    }

    /* davep 04-Oct-2010 ; can hit this assert if we're trying to dma without
     * allocating the descriptors via pic_wdma_open() (e.g., scantest.c)
     */
    XASSERT( dch->desc_list.num_descriptors>0, dch->channel );
    XASSERT( dch->desc_list.list, dch->channel );

    XASSERT( dch->num_running < dch->desc_list.num_descriptors, dch->num_running );

    /* make sure the block hasn't been here yet */
    XASSERT( DESCRIPTOR_OWNED_BY_BLOCK(dch->tail_desc), dch->tail_desc->config_flags );

    /* set up tail->next transaction */
    if( dch->tail_desc->src_addr==0 ) {
        /* Empty list. Note head==tail in a list with one element AND an empty
         * list thus the check on src_addr 
         */
        next = dch->head_desc;
    }
    else {
        next = (struct ddma_descriptor *)(dch->tail_desc->fw_next_descriptor_addr);
    }

    /* beware overrunning existing members of the list (src_addr set to zero in
     * ISR when the descriptor completes)
     */
    XASSERT( next->src_addr==0 || next->src_addr==0x42424242, next->src_addr );
//    XASSERT( next->src_addr==SRC_ADDR_EMPTY, next->src_addr );

    dma_handle = (dma_addr_t)dma_dest;

    next->src_addr = (uint32_t)dma_handle;
    next->fw_src_addr = (void *)dma_dest;
    next->transfer_len_bytes = num_rows * bytes_per_row;

    /* burtp 28-Sep-2010: FIXME! The scan block defines each scan command as being an 'image',
     *    problem is the new DMA hangs if EOI is not set when the scan image ends. The old
     *    DMA didn't care. This implies that our scan commands must always line up
     *    perfectly with the queued DMA buffers.
     */
    /* davep 22-Mar-2011 ; work around a discrepency between dma blocks: some
     * barf if EOI set (PIC overflow). Some stall (scan doesn't start) if EOI
     * isn't set. Once the "bound" versions fall out of use, we can get rid of
     * this EOI code.
     */
#ifdef HAVE_SCMD_PICWDMA_BOUND
    next->config_flags |= DDMA_DESCRIPTOR_CONFIG_EOI|DDMA_DESCRIPTOR_CONFIG_STOP
                          |DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;
#else
    next->config_flags |= DDMA_DESCRIPTOR_CONFIG_STOP|DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;
#endif

    /* flush the descriptor from the cache to main mem where dma can find it */
    ddma_desc_flush( next );

    dch->tail_desc->config_flags &= ~DDMA_DESCRIPTOR_CONFIG_STOP;

    /* flush the descriptor from the cache to main mem where dma can find it */
    ddma_desc_flush( dch->tail_desc );

    /* move to next */
    dch->tail_desc = next;

    /* davep 12-Oct-2010 ; XXX screw this; fall back to brute force */
    dch->tail_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_STOP;
    ddma_desc_flush( dch->tail_desc );
    
    dch->num_running += 1;

#ifdef PIC_WDMA_DEBUG
    if( !IN_INTERRUPT() ) {
        ddma_descriptor_list_dump( &dch->desc_list);
        channel_dump( dma_regs );
    }
#endif

//    ddma_channel_sanity( dch );

    ddma_channel_unlock( dch );
}

void pic_wdma_channel_enable( uint8_t channel ) 
{
    channel_enable( getregs( channel ) );
}

bool pic_wdma_channel_is_enabled( uint8_t channel )
{
    return channel_is_enabled( getregs(channel) );
}

void pic_wdma_channel_disable( uint8_t channel ) 
{
    channel_disable( getregs( channel ) );
}

void pic_wdma_channel_status( uint8_t channel, uint32_t *cnt_status, uint32_t *addr_status)
{
    struct pic_cwdma_regs *dma_regs;

    dma_regs = getregs(channel);

    *cnt_status = dma_regs->xfer_length;
    *addr_status = dma_regs->xfer_addr;
}

void pic_wdma_init( void ) 
{
    ice_dma_init( &pic_cwdma_mm, "piccwdma", &pic_cwdma_driver, 
            pic_cwdma_channels, PIC_WDMA_NUM_CHANNELS );
}

scan_err_t pic_cwdma_channel_open( uint8_t channel )
{
    scan_err_t scerr;
    struct ddma_channel *dch;
    char name[DDMA_NAME_LEN+1];

    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel );

    dch = &desc_channel_list[channel];
    
    strncpy( name, "picwdma ", DDMA_NAME_LEN );
    name[7] = '0' + channel;
    ddma_channel_open( dch, name, channel );

    scerr = ddma_channel_alloc_descriptors( dch, MAX_CWDMA_DESCRIPTORS );
    if( scerr != 0 ){
        /* failure! oh woe! free the channel memory we've already allocated */
        ddma_channel_free_descriptors( dch );
        ddma_channel_close( dch );

        return SCANERR_OUT_OF_MEMORY;
    }

    /* davep 21-Feb-2012 ; suggestion from PaulZ for my line_rev problem */
    dch->head_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_SOI;
    ddma_desc_flush( dch->head_desc );

    return SCANERR_NONE;
}

void pic_cwdma_channel_close( uint8_t channel )
{
    struct ddma_channel *dch;

    XASSERT( channel < PIC_WDMA_NUM_CHANNELS, channel );

    dch = &desc_channel_list[channel];
    XASSERT( dch->channel==channel, dch->channel );
    XASSERT( dch->num_running==0, dch->num_running );

    ddma_channel_free_descriptors( dch );
    ddma_channel_close( dch );
}

/**
 * \brief  
 *
 * Created to assign a scan_data_type to an ice_dma_channel during channel open
 *
 * \author David Poole
 * \date 09-Apr-2013
 */

static scan_data_type pic_channel_to_dtype( uint8_t channel )
{
    int i;
    static const struct {
        uint8_t channel;
        scan_data_type dtype;
    } channel_to_dtype[] = {
        { PIC_DMA_CHANNEL_CEVEN_0, SCAN_DATA_TYPE_RED },
        { PIC_DMA_CHANNEL_CODD_0,  SCAN_DATA_TYPE_RED },
        { PIC_DMA_CHANNEL_CEVEN_1, SCAN_DATA_TYPE_GREEN },
        { PIC_DMA_CHANNEL_CODD_1,  SCAN_DATA_TYPE_GREEN },
        { PIC_DMA_CHANNEL_CEVEN_2, SCAN_DATA_TYPE_BLUE },
        { PIC_DMA_CHANNEL_CODD_2,  SCAN_DATA_TYPE_BLUE },
        
        /* end of list marker */
        { 0, SCAN_DATA_NULL_TYPE },
    };

    /* linear search! hurray! */
    for( i=0 ; ; i++ ) {
        if( channel_to_dtype[i].dtype==SCAN_DATA_NULL_TYPE ) {
            XASSERT( 0, channel );
            return SCAN_DATA_NULL_TYPE;
        }

        if( channel_to_dtype[i].channel==channel ) {
            break;
        }
    }
    return channel_to_dtype[i].dtype;
}

/**
 * \brief  Assign a scan_data_type to all channels' ice_dma_channel.
 *
 * The ice_dma_channel's dtype is assigned to incoming ice_dma_buffers.
 *
 * \author David Poole
 * \date 09-Apr-2013
 */

static void pic_wdma_assign_channels_dtype( struct ice_dma_mm *mm, 
                                            uint8_t channels[],
                                            uint8_t num_channels )
{
    int i;
    struct ice_dma_channel *ch;

    for( i=0 ; i<num_channels ; i++ ) {
        ch = &mm->channels[channels[i]];
        XASSERT( ch->is_open, ch->channel );

        if( num_channels<=2 ) {
            ch->dtype = SCAN_DATA_TYPE_MONO;
        }
        else {
            ch->dtype = pic_channel_to_dtype( ch->channel );
        }

        /* make sure we found a match */
        XASSERT( ch->dtype!=SCAN_DATA_NULL_TYPE, ch->dtype );
    }
}

/**
 * \brief  Assign a sensor_num to all ice_dma_channels in the mm
 *
 * In order to support platforms with multiple image sensors (e.g., duplex
 * scanning), we need to track which DMA channel belongs to which sensor.
 *
 * Original reason for this code is to preserve the page side because, as of
 * this writing, PIC has six WDMA channels but PIE only has three channels. In
 * order to pass buffers back/forth, the PIE RDMA channels must be remapped
 * back to the correct PIC WDMA channel.
 *
 * \author David Poole
 * \date 23-May-2013
 */

static void pic_wdma_assign_channels_sensor_num( struct ice_dma_mm *mm, 
                                            uint8_t channels[],
                                            uint8_t num_channels )
{
    int i;
    struct ice_dma_channel *ch;

    /* davep 23-May-2013 ; as of this writing, the only active ASIC with six
     * PIC WDMA channels has assigned channels [024] to sensor #0 and [135] to
     * sensor #1. Older ASICs supported six WDMA channels for staggered sensors
     * but that support no longer exists in any chip.
     *
     * If newer ASICs arrive with weirder PIC WDMA support, we'll have to
     * revisit this code.
     */

    for( i=0 ; i<num_channels ; i++ ) {
        ch = &mm->channels[channels[i]];
        XASSERT( ch->is_open, ch->channel );

#ifdef HAVE_NSENSOR_SUPPORT 
        /* even channels are sensor #0, odd channels are sensor #1 */
        ch->sensor_num = ch->channel & 1;
#else
        ch->sensor_num = 0;
#endif
    }
}

int pic_wdma_open( uint8_t channels[],
                   uint8_t num_channels,
                   int num_empties,
                   int total_rows,
                   int bytes_per_row )
{
    int retcode;
    scan_err_t scerr;
    int i;

    XASSERT( num_channels <= PIC_WDMA_NUM_CHANNELS, num_channels );

    /* start our interrupt counters from zero (useful for debugging) */
    pic_cwdma_interrupt_stats_reset();

    for( i=0 ; i<num_channels; i++ ) {

        scerr = pic_cwdma_channel_open( channels[i] );
        if( scerr != 0 ){
            /* failure! oh woe! free the channel memory we've already allocated */
            i -= 1;
            while( i>=0 ) {
                pic_cwdma_channel_close( channels[i] );
                i -= 1;
            }

            /* this function returns the number of icebufs allocated */
            return 0;
        }

        /* davep 02-Apr-2013 ; use combined interrupts to reduce ISR calls */
        *PIC_WDMA_DWCIS |= (1<<channels[i]);
    }

    retcode = ice_dma_open( &pic_cwdma_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row, ICEBUF_TAG_PICWDMA );
    if( retcode <= 0 ) {
        /* error */
        return retcode;
    }

    /* davep 08-Apr-2013 ; assign scan_data_type to channels by
     * inferring from the number of channels
     */
    pic_wdma_assign_channels_dtype( &pic_cwdma_mm, channels, num_channels );

    /* davep 23-May-2013 ; adding sensor_num to ice_dma_channel to better
     * support multiple sensors
     */
    pic_wdma_assign_channels_sensor_num( &pic_cwdma_mm, channels, num_channels );

    return retcode;
}

scan_err_t pic_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &pic_cwdma_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void pic_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    /* mark it empty and ready to be filled */
    (*addme)->num_rows = 0;

    ice_dma_add_ready( &pic_cwdma_mm, channel, addme );
}

void pic_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &pic_cwdma_mm, channel, freeme );
}

void pic_wdma_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &pic_cwdma_mm, channel );
}

void pic_wdma_cancel( void )
{
    int channel_idx;
    struct ddma_channel *dch;

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dch = &desc_channel_list[channel_idx];
        if( !dch->is_open ) {
            continue;
        }

        /* Have to reset the channel BEFORE we disable it! Otherwise we'll get
         * stuck in channel_reset() because the channel never goes back to
         * ready.
         */
        pic_wdma_channel_reset( dch->channel );

        dch->num_running = 0;
    }

    /* now call the parent method */
    ice_dma_cancel( &pic_cwdma_mm );
}

void pic_wdma_sanity( void )
{
    int channel_idx;
    struct ddma_channel *dch;

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dch = &desc_channel_list[channel_idx];

        if( !dch->is_open ) {
            continue;
        }

        ddma_channel_sanity( dch );
    }

    ice_dma_sanity( &pic_cwdma_mm );
}

void pic_wdma_debug_log( void )
{
    int channel_idx;
    struct ddma_channel *dch;

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {

        dch = &desc_channel_list[channel_idx];
        if( !dch->is_open ) {
            continue;
        }

        dbg2( "pic wdma idx=%d ch=%d num_running=%d\n", channel_idx, dch->channel, dch->num_running );
//        ddma_desc_chain_dump( dch->desc_list, dch->num_descriptors );

        /* davep 14-Jan-2011 ; XXX temp debug */
//        ddma_data_peek( &dch->desc_list );
    }
    ice_dma_debug_log( &pic_cwdma_mm );
}

struct ice_dma_mm *pic_wdma_get_mm( void )
{
    return &pic_cwdma_mm;
}

/**
 * \brief allocate, initialize, and load buffers for PIC Write DMA 
 *
 * \author David Poole
 * \date 01-Oct-2005
 *
 * Originally started life as pictest_setup_wdma_buffers(). Moved from test
 * code to mainline code on 1-Oct-05.
 *
 * davep 09-Dec-2010 ; moved to parent class with fct pointer for polymorphism
 */

int pic_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row )
{
    return ice_dma_setup_buffers( &pic_cwdma_mm, channel, num_buffers,
                    rows_per_buffer, bytes_per_row, pic_wdma_add_buffer );
}

void pic_wdma_refill( int rows_per_buffer )
{
    ice_dma_refill( &pic_cwdma_mm, rows_per_buffer,
            pic_wdma_add_buffer, pic_wdma_channel_launch );
}

void pic_wdma_run_forever( void )
{
    ice_dma_run_forever( &pic_cwdma_mm );
}

void pic_cwdma_set_config( uint32_t disable_mode )
{
    /* DWOCR changed quite a bit from PIC DMA 2005 */

    /* clear original bits */
    *PIC_WDMA_DWOCR &= ~(PIC_WDMA_DWOCR_DISABLEMODE(~0));

    /* set new bits */
    *PIC_WDMA_DWOCR |= PIC_WDMA_DWOCR_DISABLEMODE(disable_mode);
}

extern int dbg_cnt_channel_isr;	//add.lsh.for pipe debug
static void channel_isr( struct ddma_channel *dch )
{
    scan_err_t scerr;
    struct ddma_descriptor *curr;

	dbg_cnt_channel_isr++;
	
    /* 
     * BIG FAT NOTE!  This is an interrupt handler! 
     *
     */

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
//        dbg2( "%s %p %p %#x\n", __FUNCTION__, curr, 
//                    curr->fw_next_descriptor_addr, curr->config_flags );

        XASSERT( (curr->config_flags & DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK)==0, 
                    curr->config_flags );

        SCANALYZER_LOG( LOG_PIC_DDMA_WRITE_ISR, curr->transfer_len_bytes );

        /* davep 27-Apr-2010 ; XXX temp debug */
        if( ! (curr->config_flags & DDMA_DESCRIPTOR_CONFIG_INT) ) {
            scanlog_hex_dump( (unsigned char *)(curr)-32, 64 );
            ddma_descriptor_list_dump( &dch->desc_list);
        }
        XASSERT( curr->config_flags & DDMA_DESCRIPTOR_CONFIG_INT, 
                    curr->config_flags ); 

        /* pass the DMA done interrupt to its handler (change handler so can
         * run pictest.c and scantest.c)
         */
        /* call the default method to do the buffer dance */
        ice_dma_isr( &pic_cwdma_mm, dch->channel );

        /* davep 22-Jun-2012 ; add channel data structure protection */
        scerr = ddma_channel_try_lock(dch);
        XASSERT( scerr==SCANERR_NONE, scerr );

        /* poison it */
        curr->src_addr = 0x42424242;
//        curr->src_addr = 0;
        curr->transfer_len_bytes = 0;

        /* Set the "own" bit back on the descriptor so I know I've already
         * handled this buffer. We're in a circular list so need to know when
         * to stop.
         */
        curr->config_flags |= DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;

        /* davep 21-Feb-2012 ; suggestion from PaulZ for my line_rev problems ;
         * set SOI on first descriptor (must clear after descriptor used)
         */
        curr->config_flags &= ~DDMA_DESCRIPTOR_CONFIG_SOI;

        ddma_desc_flush( curr );

        /* move head of list to next element */
        dch->head_desc = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        dch->num_running -= 1;
        XASSERT( dch->num_running >=0, dch->channel );

        curr = dch->head_desc;
        cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

        ddma_channel_unlock( dch );

//        ddma_channel_sanity( dch );
    }
}

extern int dbg_cnt_pic_wdma_interrupt;	//add.lsh.for pipe debug
void pic_wdma_interrupt( void )
{
    int channel_idx;
    struct pic_cwdma_regs *dma_regs;
    uint32_t ipend, iack;
    struct ddma_channel *dch;

	dbg_cnt_pic_wdma_interrupt++;
	
    /* enter */
    SCANALYZER_LOG( LOG_PIC_WDMA_IPEND, 1 );

    /* 
     * BIG FAT NOTE!  This is an interrupt handler
     */

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dma_regs = getregs(channel_idx);

        isr_dbg2( "%s %d %#x %#x %#x %#x %#x\n", __FUNCTION__, 
                    channel_idx, dma_regs->status, dma_regs->int_pend,
                    dma_regs->desc_read, dma_regs->xfer_length, 
                    dma_regs->xfer_addr );

        ipend = dma_regs->int_pend;
        iack = 0;

        /* if nothing pending, ignore this channel */
        if( !ipend ) {
            continue;
        }

        if( !(dma_regs->status & PIC_WDMA_STATUS_DMA_BUSY) ) {
            int_stats[channel_idx].is_idle++;
        }

        SCANALYZER_LOG( LOG_PIC_WDMA_IPEND, ipend );

        int_stats[channel_idx].count++;

        /* davep 12-Oct-2010 ; XXX temp debug */
        if( ipend & PIC_WDMA_INT_PEND_WHO ) {
            pic_wdma_channel_dump(0);
            pic_wdma_debug_log();
        }

        /* die hard and fast on owner errors (firmware has screwed up) */
        XASSERT( !(ipend & PIC_WDMA_INT_PEND_WHO), ipend );

        /* if we have a pending "Transfer End" interrupt, handle it, ack it,
         * love it
         */
        if( ipend & PIC_WDMA_INT_PEND_FIN) {
            dch = &desc_channel_list[channel_idx];
            XASSERT( dch->channel==channel_idx, dch->channel );
            XASSERT( dch->is_open, dch->channel );

            channel_isr( dch );

            iack |= PIC_WDMA_INT_ACK_FIN; 

            int_stats[channel_idx].transfer_end++;

            /* davep 13-Oct-2010 ; if we've drained our chain */
            if( dch->num_running==0 ) {
                /* make sure block is truly idle */
                XASSERT( !(dma_regs->status & PIC_WDMA_STATUS_DMA_BUSY), dma_regs->status );
            }
        }

        /* ack em if you got em */
        if( ipend & PIC_WDMA_INT_PEND_EOI ) {
            iack |= PIC_WDMA_INT_ACK_EOI; 
            int_stats[channel_idx].end_of_image++;
        }

        if( ipend & PIC_WDMA_INT_PEND_RST ) {
            iack |= PIC_WDMA_INT_ACK_RST; 
            int_stats[channel_idx].reset++;
            /* davep 01-Feb-2013 ; add a log message so these interrupts don't
             * confuse me when chasing over/underflows
             */
            isr_dbg2( "%s ch=%d reset\n", __FUNCTION__, channel_idx );
        }

/* danger! danger! danger! */
#define HAPPY_INTERRUPTS (PIC_WDMA_INT_EN_FIN | PIC_WDMA_INT_PEND_RST | PIC_WDMA_INT_EN_EOI )
        /* Trap on a few interrupts that firmware needs to work to avoid. If
         * this assert fails, firmware is failing in some way.
         */
        if( dma_regs->int_pend & ~HAPPY_INTERRUPTS ) {
            /* insert debug dumps here */
            /* davep 12-Jul-2012 ; */
            channel_dump( dma_regs );
        }
        XASSERT( !(dma_regs->int_pend & ~HAPPY_INTERRUPTS), dma_regs->int_pend );

        dma_regs->int_ack = iack;
    }

    /* exit */
    SCANALYZER_LOG( LOG_PIC_WDMA_IPEND, 0 );
}

void pic_wdma_close( void )
{
    int channel_idx;
    struct ddma_channel *dch;

    for( channel_idx=0 ; channel_idx<PIC_WDMA_NUM_CHANNELS ; channel_idx++ ) {
        dch = &desc_channel_list[channel_idx];

        if( !dch->is_open  ) {
            continue;
        }

        pic_cwdma_channel_close( dch->channel );
    }

    ice_dma_close( &pic_cwdma_mm );
}

void pic_wdma_set_bitpack_mode( pic_bitpack_mode mode )
{
    int i;
    struct pic_cwdma_regs *dma_regs;
    int mode_bits;

    /* use dma_regs.cfg.in_width field to switch between 8-bpp and 16-bpp */

#ifdef PIC_WDMA_DEBUG
    dbg2( "%s mode=%d\n", __FUNCTION__, mode );
#endif

    mode_bits = 0;

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        dma_regs = getregs(i);

        /* clear existing setting */
        dma_regs->cfg &= ~PIC_WDMA_CFG_IN_WIDTH(~0);

        switch( mode ) {
            case PIC_BITPACK_16BIT :
                mode_bits = PIC_WDMA_CFG_IN_WIDTH_16_BIT;
                break;

            case PIC_BITPACK_8BIT :
                mode_bits = PIC_WDMA_CFG_IN_WIDTH_8_BIT;
                break;

            default :
                XASSERT( 0, (uint32_t)mode );
        }

        dma_regs->cfg |= PIC_WDMA_CFG_IN_WIDTH(mode_bits);
    }
}

pic_bitpack_mode pic_wdma_get_bitpack_mode( void )
{
    struct pic_cwdma_regs *dma_regs;
    int mode_bits;
    pic_bitpack_mode mode;

    /* return from channel zero since the set_bitpack_mode() simply writes all
     * channels identically
     */
    dma_regs = getregs(0);

    mode_bits = PIC_WDMA_CFG_GET_IN_WIDTH( dma_regs->cfg );

    switch( mode_bits ) {
        case PIC_WDMA_CFG_IN_WIDTH_8_BIT :
            mode = PIC_BITPACK_8BIT;
            break;

        case PIC_WDMA_CFG_IN_WIDTH_16_BIT :
            mode = PIC_BITPACK_16BIT;
            break;
            
        default :
            mode = PIC_BITPACK_16BIT;
            XASSERT( 0, mode_bits );
            break;
    }

    return mode;
}

/**
 * \brief  Convenience function to set PIC WDMA packing mode.
 *
 * \author David Poole
 * \date 28-Aug-2012
 */

scan_err_t pic_wdma_set_bpp( int pic_bits_per_pixel )
{
    switch( pic_bits_per_pixel ) {
        case 8 :
            pic_wdma_set_bitpack_mode( PIC_BITPACK_8BIT );
            break;

        case 16 :
            pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
            break;

        default :
            XASSERT(0,pic_bits_per_pixel);
            return SCANERR_INVALID_PARAM;
    }
    return SCANERR_NONE;
}

void pic_wdma_init_routing( void )
{
    uint32_t num32;

#if PIC_WDMA_NUM_CHANNELS==3
    num32 = PIC_WDMA_DWRTBL_CEVEN_0(PIC_DMA_CHANNEL_CEVEN_0)
          | PIC_WDMA_DWRTBL_CEVEN_1(PIC_DMA_CHANNEL_CEVEN_1)
          | PIC_WDMA_DWRTBL_CEVEN_2(PIC_DMA_CHANNEL_CEVEN_2)
          ;
#else
    /* davep 30-Jan-2013 ; adding dual sensor support (six PIC WDMA channels) */
    num32 = PIC_WDMA_DWRTBL_CEVEN_0(PIC_DMA_CHANNEL_CEVEN_0)
          | PIC_WDMA_DWRTBL_CEVEN_1(PIC_DMA_CHANNEL_CEVEN_1)
          | PIC_WDMA_DWRTBL_CEVEN_2(PIC_DMA_CHANNEL_CEVEN_2)
          | PIC_WDMA_DWRTBL_MEVEN(PIC_DMA_CHANNEL_MEVEN)  
          | PIC_WDMA_DWRTBL_CODD_0(PIC_DMA_CHANNEL_CODD_0) 
          | PIC_WDMA_DWRTBL_CODD_1(PIC_DMA_CHANNEL_CODD_1) 
          | PIC_WDMA_DWRTBL_CODD_2(PIC_DMA_CHANNEL_CODD_2) 
          | PIC_WDMA_DWRTBL_MODD(PIC_DMA_CHANNEL_MODD)   
          ;
#endif

    *PIC_WDMA_DWRTBL = num32;

    /* davep 02-Apr-2013 ; use combined interrupts to reduce ISR calls */
    *PIC_WDMA_DWCIS = 0;
}

void pic_wdma_set_burst_size( ice_dma_burst_size burst_size )
{
    int i;
    struct pic_cwdma_regs *dma_regs;
    int bs_bits;

#ifdef PIC_WDMA_DEBUG
    dbg2( "%s burst_size=%d\n", __FUNCTION__, burst_size );
#endif

    bs_bits = 0;

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        dma_regs = getregs(i);

        /* clear existing setting */
        dma_regs->cfg &= ~PIC_WDMA_CFG_BURST_LEN(~0);
        
        /* set new setting */
        switch( burst_size ) {
            case ICE_DMA_BURST_16 :
                /* set burst len to 4 words (16 bytes) */
                bs_bits = PIC_WDMA_CFG_BURST_LEN_4_WORD;
                break;

            case ICE_DMA_BURST_32 :
                /* set burst len to 8 words (32 bytes) */
                bs_bits = PIC_WDMA_CFG_BURST_LEN_8_WORD;
                break;

            default:
                XASSERT(0,(uint32_t)burst_size );
        }

        dma_regs->cfg |= PIC_WDMA_CFG_BURST_LEN(bs_bits);
    }
}

void pic_wdma_reset( void )
{
    int i;
    struct pic_cwdma_regs *dma_regs;
    uint32_t status;

#ifdef PIC_WDMA_DEBUG
    dbg2( "%s\n", __FUNCTION__ );
#endif

    /* our dma descriptors must be padded to a cache line size so we can flush
     * them from cache without breaking neighboring cache lines
     */
    XASSERT( sizeof(struct ddma_descriptor)==cpu_get_dcache_line_size(), 
            sizeof(struct ddma_descriptor));

    ASSERT( !IN_INTERRUPT() );

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        dma_regs = getregs(i);

        /* davep 01-Oct-2010 ; shotgun debug */
//        pic_wdma_channel_enable( i );

//        /* verify the register seem to be pointing the correct place */
//        XASSERT( dma_regs->cfg==PIC_WDMA_CFG_R, dma_regs->cfg );
//        XASSERT( dma_regs->status==PIC_WDMA_STATUS_R, dma_regs->status );

//        pic_wdma_channel_dump( i );
        pic_wdma_channel_disable( i );

        pic_wdma_channel_reset( i );

        /* verify the register seem to be pointing the correct place */
        XASSERT( dma_regs->cfg==PIC_WDMA_CFG_R, dma_regs->cfg );
//        XASSERT( dma_regs->status==PIC_WDMA_STATUS_R, dma_regs->status );
        status = PIC_WDMA_STATUS_PACKER_EMPTY 
               | PIC_WDMA_STATUS_EMPTY_DBUF
               | PIC_WDMA_STATUS_EMPTY_CBUF
               | PIC_WDMA_STATUS_SOFTRESET 
               ; 
        XASSERT( dma_regs->status & status, dma_regs->status );
        XASSERT( !(dma_regs->status & PIC_WDMA_STATUS_DMA_BUSY), dma_regs->status );
    }

    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_ACCEPT);

//    /* davep 26-Apr-2010 ; useful for test/debug */
//    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_DISCARD);
}

void pic_wdma_dump( void )
{
    int i;

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        pic_wdma_channel_dump(i);
    }
}

