/*
 * Left/Right margins in firmware.
 *
 * davep 08-Dec-2010
 *
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

#include "list.h"           
#include "lassert.h"
#include "debug.h"
#include "cpu_api.h"
#include "memAPI.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "scanvars.h"
#include "fwmargin.h"
#include "chipgap.h"

//#define FW_MARGIN_DEBUG

static struct ice_dma_driver fw_rmargin_driver = { 
    .msg_data = SMSG_FW_READ_MARGIN_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      fw_rmargin_channel_reset,
    .enable =     fw_rmargin_channel_enable,
    .disable =    fw_rmargin_channel_disable,
    .load =       fw_rmargin_channel_load, 
    .start =      fw_rmargin_channel_start,
    .is_enabled = fw_rmargin_channel_is_enabled,
    .icebuf_isr = fw_rmargin_isr,
};

static struct ice_dma_driver fw_wmargin_driver = { 
    .msg_data = SMSG_FW_WRITE_MARGIN_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      fw_wmargin_channel_reset,
    .enable =     fw_wmargin_channel_enable,
    .disable =    fw_wmargin_channel_disable,
    .load =       fw_wmargin_channel_load, 
    .start =      fw_wmargin_channel_start,
    .is_enabled = fw_wmargin_channel_is_enabled,
    .icebuf_isr = fw_wmargin_isr,
};

struct dma_transaction {
    uint8_t channel;
    uint8_t *ptr;
    uint32_t num_rows;
    uint32_t bytes_per_row;
    uint32_t row_number;
};

#define RINGBUF_MAX_ELEMENTS (ICE_DMA_REG_DEPTH+1)

struct fw_margin_ringbuf {
    struct dma_transaction transaction[ RINGBUF_MAX_ELEMENTS ];
    int curridx, nextidx;
};

struct fw_margin_channel {

    uint8_t channel;

    /** need to behave as a hardware device so we can interact completely with
     * ice_dma (e.g., ice_dma_close(), ice_dma_sanity()). 'is_enabled' is
     * tracked so we act like as closely as possible to a real hardware device.
     */
    bool is_enabled;

    struct fw_margin_ringbuf ringbuf;
    int num_running;
    int num_interrupt_pending;

    int total_rows;
};

struct ice_dma_mm fw_read_margin_mm;
struct ice_dma_mm fw_write_margin_mm;
static struct ice_dma_channel ice_read_channels[FW_MARGIN_NUM_CHANNELS];
static struct ice_dma_channel ice_write_channels[FW_MARGIN_NUM_CHANNELS];

static struct fw_margin_channel fw_read_margin_channels[FW_MARGIN_NUM_CHANNELS];
static struct fw_margin_channel fw_write_margin_channels[FW_MARGIN_NUM_CHANNELS];

struct fwmargin_members {

    /* Each public member has a getter/setter function.  The setter function
     * will be "private" (declared 'static' in C terms) and is responsible for
     * setting
     *
     * Private members have no externally available getter function.
     *
     * getters will assert if the field is uninitialized.
     *
     * Note: the "setter" functions are named "save" instead of "set" because I
     * didn't want any confusion that the setter function was actually writing
     * to hardware.
     */
    bool is_open;

    int bits_per_pixel;
    bool bpp_not_null;

    int x_pixels, width_pixels;
    bool margin_not_null;

    int midpoint;

    void (*chipgap_interpolate_row)(uint8_t *, uint8_t *, int );
};

/* The data members of the fwmargin "class".
 *
 * Cleared in fw_margin_open() (the "constructor") and filled as we set up the
 * fwmargins.  This is a singleton.
 *
 * Basically a cheap way to do a set of globals with flags to catch
 * uninitialized values.
 */

static struct fwmargin_members fwmargin_self;

static void current_save_bpp( int bits_per_pixel )
{
    fwmargin_self.bits_per_pixel = bits_per_pixel;
    fwmargin_self.bpp_not_null = true;
}

static int current_get_bpp( void )
{
    ASSERT( fwmargin_self.bpp_not_null );
    return fwmargin_self.bits_per_pixel;
}

static void current_save_margin_pixels( int x_pixels, int width_pixels )
{
    dbg2( "%s %d %d\n", __FUNCTION__, x_pixels, width_pixels );

    fwmargin_self.x_pixels = x_pixels;
    fwmargin_self.width_pixels = width_pixels;
    fwmargin_self.margin_not_null = true;
}

static void current_init( void ) 
{
    /* Sort of like a C++ Constructor.
     *
     * note: always go through this constructor so we can alloc any 
     * necessary memory (if at some point we do have malloc'd memory in
     * fwmargin_members) and set any constants; 
     */
    memset( &fwmargin_self, 0, sizeof(fwmargin_self) );

    /* default to input/output of 8 bits/pixel */
    current_save_bpp( 8 );
}

static void current_delete( void )
{
    /* Sort of like a C++ Destructor.
     *
     * note: always go through this destructor so we can free any malloc'd
     * memory (if at some point we do have malloc'd memory in fwmargin_members)
     */

    memset( &fwmargin_self, 0, sizeof(fwmargin_self) );
}

static void current_sanity( void )
{
    /* if this fails, you need to call fw_margin_set_bpp() */
    ASSERT( fwmargin_self.bpp_not_null );

    /* if this fails, you need to call fw_margin_setup_margins() */
    ASSERT( fwmargin_self.margin_not_null );
}

/**
 * \brief Simple boogerhead ringbuf containing a structure.
 *
 * The ringbuf full/empty is determined using the 'ptr' field of the structure.
 *
 * \author David Poole
 * \date 29-Jun-2010
 *
 */

static void ringbuf_init( struct fw_margin_ringbuf *rb )
{
    memset( rb, 0, sizeof(struct fw_margin_ringbuf) );
    rb->curridx = rb->nextidx = 0;
}

static int ringbuf_push( struct fw_margin_ringbuf *rb, 
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

static int ringbuf_pop( struct fw_margin_ringbuf *rb, 
                        struct dma_transaction *pop_trans )
{
    struct dma_transaction *trans;

    trans = &rb->transaction[ rb->curridx ];
    if( trans->ptr == NULL ) {
        /* ringbuf empty */
        return -1;
    }

    memcpy( pop_trans, trans, sizeof(struct dma_transaction) );
    trans->ptr = NULL; /* slot now empty */
    rb->curridx = (rb->curridx+1) % RINGBUF_MAX_ELEMENTS;

    return 0;
}

static void channel_open( uint8_t channels[], uint8_t num_channels, 
                                    struct fw_margin_channel fw_margin_channels[] )
{
    int i;
    struct fw_margin_channel *march;

    for( i=0 ; i<num_channels ; i++ ) {
        XASSERT( channels[i] < FW_MARGIN_NUM_CHANNELS, channels[i] );
        march = &fw_margin_channels[ channels[i] ];

        memset( march, 0, sizeof( struct fw_margin_channel) );

        ringbuf_init( &march->ringbuf );
        march->channel = channels[i];
        march->is_enabled = false;
    }
}


static void channel_load( struct fw_margin_channel *march, 
                          uint8_t *ptr, uint32_t num_rows, uint32_t bytes_per_row )
{
    int retcode;
    struct dma_transaction trans;

//    dbg2( "%s ch=%d ptr=%p rows=%d bpr=%d\n", __FUNCTION__, march->channel, ptr, num_rows,
//            bytes_per_row );

    memset( &trans, 0, sizeof(struct dma_transaction) );
    trans.channel = march->channel;
    trans.ptr = ptr;
    trans.num_rows = num_rows;
    trans.bytes_per_row = bytes_per_row;
    trans.row_number = march->total_rows;
    march->total_rows += num_rows;

    retcode = ringbuf_push( &march->ringbuf, &trans );
    XASSERT( retcode==0, march->channel );

    march->num_running += 1;
}

static void channel_run( struct dma_transaction *read_trans, 
                         struct dma_transaction *write_trans )
{
    int i;
    int read_total_bytes;
    int write_total_bytes;
    uint8_t *inp;
    uint8_t *outp;

#ifdef FW_MARGIN_DEBUG
    dbg2( "%s rch=%d rrows=%d rbpr=%d rptr=%p wch=%d wbpr=%d wrows=%d wptr=%p\n", __FUNCTION__, 
            read_trans->channel, 
            read_trans->bytes_per_row, 
            read_trans->num_rows, 
            read_trans->ptr,
            write_trans->channel, 
            write_trans->bytes_per_row, 
            write_trans->num_rows,
            write_trans->ptr );
#endif

    read_total_bytes = read_trans->bytes_per_row * read_trans->num_rows;
    write_total_bytes = write_trans->bytes_per_row * write_trans->num_rows;

//    dbg2( "%s rbytes=%d wbytes=%d\n", __FUNCTION__, 
//            read_total_bytes, write_total_bytes );

    XASSERT( read_total_bytes>=write_total_bytes, read_total_bytes );
    XASSERT( read_trans->num_rows==write_trans->num_rows, write_trans->num_rows );

    /* Should not have to do an invalidate. The incoming buffer should be a DMA
     * buffer with an address already kicked out of the dcache.
     * See ice_dma_alloc_buffer()-icedma.c
     * cpu_dcache_invalidate( read_trans->ptr, read_total_bytes );
     */

//    /* simple memcpy for now */
//    memcpy( write_trans->ptr, read_trans->ptr, write_total_bytes );

//    /* davep 10-Dec-2010 ; XXX hardwire to 8bpp for now */
//    XASSERT( fwmargin_self.bits_per_pixel==8, fwmargin_self.bits_per_pixel );

#if 1
    /* davep 01-Feb-2012 ; chipgap interpolation */

    ASSERT( fwmargin_self.chipgap_interpolate_row );

    for( i=0 ; i<write_trans->num_rows ; i++ ) {
        inp = read_trans->ptr + i * read_trans->bytes_per_row;
        outp = write_trans->ptr + i * write_trans->bytes_per_row;

        fwmargin_self.chipgap_interpolate_row( inp, outp, write_trans->bytes_per_row );
    }
#else
    uint8_t *inp_list[2];
    uint8_t *outp_end;

    /* copy the pixels and run desplit/merge */
    for( i=0 ; i<write_trans->num_rows ; i++ ) {
        inp_list[0] = read_trans->ptr + i * read_trans->bytes_per_row;
        inp_list[1] = inp_list[0] + fwmargin_self.midpoint;
        outp = write_trans->ptr + i * write_trans->bytes_per_row;
        outp_end = outp + write_trans->bytes_per_row;

        /* hacka hacka hacka ; try to add user margins */
        inp_list[0] += fwmargin_self.x_pixels;
        inp_list[1] += fwmargin_self.x_pixels;

        while( outp < outp_end ) {
            *outp++ = *inp_list[0]++;
            *outp++ = *inp_list[1]++;
        }
    }
#endif

    cpu_dcache_writeback_region( write_trans->ptr, write_total_bytes );
}

static void channel_start( uint8_t channel )
{
    struct fw_margin_channel *read_march, *write_march; 
    int retcode;
    struct dma_transaction read_trans;
    struct dma_transaction write_trans;

    read_march = &fw_read_margin_channels[ channel ];
    XASSERT( read_march->channel==channel, channel );

    write_march = &fw_write_margin_channels[ channel ];
    XASSERT( write_march->channel==channel, channel );

    while( read_march->num_running && write_march->num_running ) {
        retcode = ringbuf_pop( &read_march->ringbuf, &read_trans );
        XASSERT( retcode==0, read_march->channel );
        read_march->num_running -= 1;

        retcode = ringbuf_pop( &write_march->ringbuf, &write_trans );
        XASSERT( retcode==0, write_march->channel );
        write_march->num_running -= 1;

        channel_run( &read_trans, &write_trans );

        read_march->num_interrupt_pending += 1;
        write_march->num_interrupt_pending += 1;
    }
}

/**
 * \brief check for pending "interrupts", buffers that are complete. 
 *
 * Can't call the isr functions from within channel_start() because
 * the icedma.c functions semaphore lock the data structures. We would wind up
 * calling the isr functions in icedma which would cause the lock to fail.
 *
 * \author David Poole
 * \date 08-Dec-2010
 *
 */

static void poll_interrupts( uint8_t channel )
{
    struct fw_margin_channel *read_march, *write_march; 

//    dbg2( "%s ch=%d\n", __FUNCTION__, channel );

    read_march = &fw_read_margin_channels[ channel ];
    write_march = &fw_write_margin_channels[ channel ];

//    dbg2( "%s %d %d\n", __FUNCTION__, 
//                    read_march->num_interrupt_pending, 
//                    write_march->num_interrupt_pending );

    while( read_march->num_interrupt_pending>0 ) {
        read_march->num_interrupt_pending -= 1;
        fw_rmargin_isr( read_march->channel );
    }

    while( write_march->num_interrupt_pending>0 ) {
        write_march->num_interrupt_pending -= 1;
        fw_wmargin_isr( write_march->channel );
    }
}
/*
 *  Read Margin
 *
 */

int fw_rmargin_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
#ifdef FW_MARGIN_DEBUG
    dbg2( "%s total_rows=%d bpr=%d\n", __FUNCTION__, total_rows, bytes_per_row );
#endif

    channel_open( channels, num_channels, fw_read_margin_channels );

    /* davep 10-Dec-2010 ; sanity check ; make sure we've been correctly set up */
    current_sanity();

    return ice_dma_open( &fw_read_margin_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row,
                    ICEBUF_TAG_FW_RMARGIN );
}

scan_err_t fw_rmargin_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &fw_read_margin_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void fw_rmargin_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
#ifdef __KERNEL__
    /* davep 18-Jun-2013 ; XXX shotgun debug kick the data out of cache */
    dma_unmap_single( NULL, (*addme)->dma_handle, (*addme)->datalen, DMA_BIDIRECTIONAL );
#endif

    ice_dma_add_ready( &fw_read_margin_mm, channel, addme );
}

void fw_rmargin_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &fw_read_margin_mm, channel, freeme );
}


void fw_rmargin_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &fw_read_margin_mm, channel );
    poll_interrupts( channel );
}

void fw_rmargin_close( void )
{
    ice_dma_close( &fw_read_margin_mm );
}

void fw_rmargin_cancel( void )
{
    ice_dma_cancel( &fw_read_margin_mm );
}

void fw_rmargin_sanity( void )
{
    ice_dma_sanity( &fw_read_margin_mm );
}

void fw_rmargin_channel_reset( uint8_t channel )
{
    /* No-op */
}

void fw_rmargin_channel_enable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_read_margin_mm.num_channels, channel );
    ch = &fw_read_margin_mm.channels[channel];

    fw_read_margin_channels[ ch->channel ].is_enabled = true;
}

void fw_rmargin_channel_disable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_read_margin_mm.num_channels, channel );
    ch = &fw_read_margin_mm.channels[channel];

    fw_read_margin_channels[ ch->channel ].is_enabled = false;
}

void fw_rmargin_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_src, 
                                uint32_t rows, uint32_t bytes_per_row )
{
    struct fw_margin_channel *march;

//    dbg2( "%s ch=%d ptr=%#x rows=%d bpr=%d\n", __FUNCTION__, channel, dma_src, rows, bytes_per_row );

    march = &fw_read_margin_channels[ channel ];
    XASSERT( march->channel==channel, channel );

    /* davep 01-Jul-2013 ; Since this is 100% firmware device, pass the cpu
     * addressible pointer, not the physical DMA device address
     */
    channel_load( march, data_ptr, rows, bytes_per_row );
}

void fw_rmargin_channel_start( uint8_t channel )
{
    channel_start( channel );
}

bool fw_rmargin_channel_is_enabled( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_read_margin_mm.num_channels, channel );
    ch = &fw_read_margin_mm.channels[channel];

    return fw_read_margin_channels[ ch->channel ].is_enabled;
}

void fw_rmargin_isr( uint8_t channel )
{
    ice_dma_isr( &fw_read_margin_mm, channel );
}

void fw_rmargin_debug_log( void )
{
    ice_dma_debug_log( &fw_read_margin_mm );
}

void fw_rmargin_init( void )
{
    ice_dma_init( &fw_read_margin_mm, "fwrmargin", &fw_rmargin_driver, 
            ice_read_channels, FW_MARGIN_NUM_CHANNELS);
}

/*
 *  Write Margin
 *
 */

int fw_wmargin_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row )
{
#ifdef FW_MARGIN_DEBUG
    dbg2( "%s total_rows=%d bpr=%d\n", __FUNCTION__, total_rows, bytes_per_row );
#endif

    channel_open( channels, num_channels, fw_write_margin_channels );

    /* davep 10-Dec-2010 ; sanity check ; make sure we've been correctly set up */
    current_sanity();

    return ice_dma_open( &fw_write_margin_mm, channels,
                    num_channels, num_empties,
                    total_rows, bytes_per_row,
                    ICEBUF_TAG_FW_WMARGIN );
}

scan_err_t fw_wmargin_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    return ice_dma_add_buffer( &fw_write_margin_mm, 
            channel, data, datalen, rows, bytes_per_row );
}

void fw_wmargin_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    /* mark it empty and ready to be filled */
    (*addme)->num_rows = 0;

    ice_dma_add_ready( &fw_write_margin_mm, channel, addme );
}

void fw_wmargin_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &fw_write_margin_mm, channel, freeme );
}


void fw_wmargin_channel_launch( uint8_t channel )
{
    ice_dma_channel_launch( &fw_write_margin_mm, channel );
    poll_interrupts( channel );
}

void fw_wmargin_close( void )
{
    ice_dma_close( &fw_write_margin_mm );
}

void fw_wmargin_cancel( void )
{
    ice_dma_cancel( &fw_write_margin_mm );
}

void fw_wmargin_sanity( void )
{
    ice_dma_sanity( &fw_write_margin_mm );
}


void fw_wmargin_channel_reset( uint8_t channel )
{
    /* No-op */
}

void fw_wmargin_channel_enable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_write_margin_mm.num_channels, channel );
    ch = &fw_write_margin_mm.channels[channel];

    fw_write_margin_channels[ ch->channel ].is_enabled = true;
}

void fw_wmargin_channel_disable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_write_margin_mm.num_channels, channel );
    ch = &fw_write_margin_mm.channels[channel];

    fw_write_margin_channels[ ch->channel ].is_enabled = false;
}

void fw_wmargin_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row )
{
    struct fw_margin_channel *march;

//    dbg2( "%s ch=%d ptr=%#x rows=%d bpr=%d\n", __FUNCTION__, channel, dma_dest, rows, bytes_per_row );

    march = &fw_write_margin_channels[ channel ];
    XASSERT( march->channel==channel, channel );

    /* davep 01-Jul-2013 ; Since this is 100% firmware device, pass the cpu
     * addressible pointer, not the physical DMA device address
     */
    channel_load( march, data_ptr, rows, bytes_per_row );
}

void fw_wmargin_channel_start( uint8_t channel )
{
    channel_start( channel );
}

bool fw_wmargin_channel_is_enabled( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<fw_write_margin_mm.num_channels, channel );
    ch = &fw_write_margin_mm.channels[channel];

    return fw_write_margin_channels[ ch->channel ].is_enabled;
}

void fw_wmargin_isr( uint8_t channel )
{
    ice_dma_isr( &fw_write_margin_mm, channel );
}

void fw_wmargin_debug_log( void )
{
    ice_dma_debug_log( &fw_write_margin_mm );
}

void fw_wmargin_init( void )
{
    ice_dma_init( &fw_write_margin_mm, "fwwmargin", &fw_wmargin_driver, 
            ice_write_channels, FW_MARGIN_NUM_CHANNELS);
}

int fw_margin_setup_write_buffers( uint8_t channel, int num_buffers, 
                                    int rows_per_buffer, int bytes_per_row )
{
    return ice_dma_setup_buffers( &fw_write_margin_mm, channel, num_buffers,
                    rows_per_buffer, bytes_per_row, fw_wmargin_add_buffer );
}

/**
 * \brief  
 *
 * \param user_x_pixels     user's area, right edge in pixels
 *
 * \param user_width_pixels user's area, width in pixels
 *
 * \param pixels_in     number of pixels sent to the fwmargin input (e.g., PIC WDMA)
 *
 * \param pixels_out    number of pixels fwmargin sends downstream
 *
 * \author David Poole
 * \date 10-Dec-2010
 *
 */

#if 0
scan_err_t old_fw_margin_setup_margins( const struct scanvars *sv, 
                                    int pixels_in, int *pixels_out )
{
    float x_inches, width_inches;
    uint32_t user_x_pixels, user_width_pixels;
    int bits_per_pixel;

    dbg2( "%s x_hin=%d width_hin=%d pin=%d\n", __FUNCTION__, 
                sv->user_area_hinch.x, 
                sv->user_area_hinch.width, pixels_in );

    x_inches = sv->user_area_hinch.x/100.0;
    width_inches = sv->user_area_hinch.width/100.0;

    /* note I'm truncating! */
    user_x_pixels = (uint32_t)(x_inches * sv->hw_dpi_horiz);
    user_width_pixels = (uint32_t)(width_inches * sv->hw_dpi_horiz);

    /* round up to DMA alignment; XXX hardwired to 8-bpp for now! */
    bits_per_pixel = sv->pic_bpp;
    XASSERT( bits_per_pixel==8, bits_per_pixel );
    user_width_pixels = ICE_DMA_ALIGN_ME( user_width_pixels );

    current_save_margin_pixels( user_x_pixels, user_width_pixels );

    XASSERT( pixels_in >= user_width_pixels, user_width_pixels );

    *pixels_out = user_width_pixels;

    /* XXX hardwired to a certain vendor's 2400 dpi sensor */
    fwmargin_self.midpoint = 10320;

    dbg2( "%s user x_pix=%d width_pix=%d pin=%d pout=%d\n", __FUNCTION__,
                user_x_pixels, user_width_pixels, pixels_in, *pixels_out);

    return SCANERR_NONE;
}
#endif

scan_err_t fw_margin_setup_margins( const struct scanvars *sv, 
                                    int pixels_in, int *pixels_out )
{
    uint32_t pic_left_margin, pic_right_margin;

    dbg2( "%s pixels_in=%d\n", __FUNCTION__, pixels_in );

    /* davep 01-Feb-2012 ; adding FW chipgap correction */
    *pixels_out = pixels_in;

    /* these fields aren't used during chipgap interpolation but the sanity
     * checks require them anyway
     */
    current_save_margin_pixels( 0, 0 );
    current_save_bpp( sv->pic_bpp );

    if( sv->pic_bpp==8 ) {
        fwmargin_self.chipgap_interpolate_row = chipgap_interpolate_row_8bpp;
    }
    else {
        XASSERT( sv->pic_bpp==16, sv->pic_bpp );
        fwmargin_self.chipgap_interpolate_row = chipgap_interpolate_row_16bpp;
    }

    /* davep 18-Jun-2013 ; fixing (ha ha) chicken/egg problem with PIC margins.
     * Attempt to guess the PIC margins so we can 
     */
    pic_left_margin = sv->scan_area_pixels.x;
    pic_right_margin = sv->scan_area_pixels.x + sv->scan_area_pixels.width -1; 

    chipgap_precalculate_segment_positions( pic_left_margin, pic_right_margin,
                                            current_get_bpp() );
//    chipgap_debug_print_segments();

    return SCANERR_NONE;
}

scan_err_t fw_margin_open( void )
{
#ifdef FW_MARGIN_DEBUG
    dbg2( "%s\n", __FUNCTION__ );
#endif

    /* stupid human check */
    ASSERT( fwmargin_self.is_open==false );

    /* start our scan run with a clean slate */
    current_init();

    /* we're now open for business */
    fwmargin_self.is_open = true;

    return SCANERR_NONE;
}

scan_err_t fw_margin_close( void )
{
#ifdef FW_MARGIN_DEBUG
    dbg2( "%s\n", __FUNCTION__ );
#endif

    /* stupid human check */
    ASSERT( fwmargin_self.is_open );

    /* clean up our settings */
    current_delete();

    /* be explicit about being closed */
    fwmargin_self.is_open = false;

    return SCANERR_NONE;
}

void fw_wmargin_refill( int rows_per_buffer )
{
    ice_dma_refill( &fw_write_margin_mm, rows_per_buffer,
            fw_wmargin_add_buffer, fw_wmargin_channel_launch );
}


void fw_margin_onetime_init( void )
{
    fw_rmargin_init();
    fw_wmargin_init();
}

