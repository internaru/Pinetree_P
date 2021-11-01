/**
 * \file icefilter.c
 *
 * \brief software image filter
 *
 * A simple in-flight filter. Designed to plug into the icedma architecture by
 * pretending to be a hardware device. 
 *
 * The icefilter is a stopgap measure implemented very close to a firmware
 * release. The hardware smoother in PIE is broken and we're having a lot of
 * trouble with mono copy (laser) of halftoned images.
 *
 * There is a lot of room for improvement in this code.  See smooth_it() for
 * full details.
 *
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>

#include "scos.h"

#include "list.h"           
#include "lassert.h"
#include "debug.h"
#include "memAPI.h"
#include "regAddrs.h"
#include "cpu_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantask.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "icefilter.h"

//#define ICE_FILTER_DEBUG

/** We static allocate our NxN matrix of buffer pointers. This is the largest
 * we can have but we could use less (e.g., a 3x3)
 */
#define ICE_FILTER_MASK_MAX  5

/* davep 7-mar-6; The ICE filter is a software "plugin" into the usual ICE DMA
 * architecture. This is the first time I've tried plugging a non-hardware
 * device into my ICE DMA architecture. There are a few weird spots.
 *
 * I want to be able to use as much of the icedma.c functions as I can (open,
 * close, cancel, etc). So I can use icedma's functions, I need to have a
 * mostly populated driver structure, especially for ice_dma_cancel() which
 * calls disable and reset. 
 *
 */
 
static struct ice_dma_driver ice_filter_driver = { 
    .msg_data = SMSG_ICE_FILTER_DATA,
    .max_queue_depth = ICE_DMA_REG_DEPTH,

    .reset =      ice_filter_channel_reset,   /* NOP */
    .enable =     ice_filter_channel_enable,  
    .disable =    ice_filter_channel_disable, 
    .load =       ice_filter_channel_load,    /* will assert if called */
    .start =      ice_filter_channel_start,   /* NOP */
    .is_enabled = ice_filter_channel_is_enabled, 
    .icebuf_isr = ice_filter_isr  /* will assert if called */
};

struct ice_filter_data {
    /** need to behave as a hardware device so we can interact completely with
     * ice_dma_cancel. 'is_enabled' is tracked so we act like as closely as
     * possible to a real hardware device.
     */
    bool is_enabled;

    /** how large our mask is (NxN matrix); must be odd since we're doing
     * filters 
     */
    int mask_size;

    /** array of pointers to rows of data for the NxN filter mask; this is a
     * COPY of a pointer to data NOT something that has to be freed!
     */
    uint8_t *row_ptrs[ICE_FILTER_MASK_MAX];

    /* timing tools */
    uint32_t launch_time;
    uint32_t smooth_time;

    /* more? */
};

struct ice_dma_mm ice_filter_mm;
static struct ice_dma_channel ice_filter_channels[ICE_FILTER_NUM_CHANNELS];
static struct ice_filter_data ice_filter_data[ICE_FILTER_NUM_CHANNELS];

/**
 * \brief search an icebuf list for a particular row number 
 *
 * \retval pointer to a row's data or NULL if not found in the list
 *
 * Has a few special cases so we can transparently handle padding head and tail
 * of our intput data for the NxN matrix filter.
 *
 * If the row number is less than zero, return a pointer to row zero.
 * If the row number is greater than our max row index, return a pointer to the
 * maximum row index's data.
 *
 * Searches the list backwards. Odds are the row we're trying to find is in the
 * last icebuf on the list.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static uint8_t*
find_row_in_list( int row_idx, int max_row_idx, ATLISTENTRY *first, ATLISTENTRY *last )
{
    struct ice_dma_buffer *icebuf;
    ATLISTENTRY *curr;

    /* Strip search the icebuf list for the first buffer containing the row
     * we're looking for.  Yes this is a linear search but this list is going
     * to be very short, usually N where N is the size of our NxN filter mask.
     *
     * Note our lists are circularly linked. 
     */

#ifdef ICE_FILTER_DEBUG
//    dbg1( "%s idx=%d maxidx=%d\n", __FUNCTION__, row_idx, max_row_idx );
#endif
    /* stupid human check */
    ASSERT( first!=NULL );     
    ASSERT( last!=NULL );

    if( first==last ) {
        /* list is empty; don't bother searching */
        return NULL;
    }

    /* if we're trying to get a row ahead of our first row, simply return a
     * pointer to the first row so we'll act like we're replicating the data
     */
    if( row_idx < 0 ) {
        icebuf = CONTAINING_RECORD( first, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );
        return icebuf->data;
    }

    /* If we're trying to get a row past our last row, return a pointer to the
     * last row so we'll act like we're replicating the data.
     */
    if( row_idx > max_row_idx ) {
        /* note: recursive call! */
        return find_row_in_list( max_row_idx, max_row_idx, first, last );
    }

    curr = first;
    while( curr != last ) {

        /* XXX - get rid of the icebuf_sanity() check eventually */
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );

        /* -1 to convert from count to max index */
//        dbg2( "%s find %d in %p [%d,%d]\n", __FUNCTION__, row_idx, 
//                    icebuf, icebuf->base_row, icebuf->base_row+(icebuf->num_rows-1) );

        if( row_idx >= icebuf->base_row && 
            row_idx < icebuf->base_row+(int)icebuf->num_rows ) {
            /* found it! */
//            dbg2( "%s found %d in %p\n", __FUNCTION__, row_idx, icebuf );

            /* return a pointer to the data */
            return icebuf->data + 
                    icebuf->bytes_per_row * (row_idx-icebuf->base_row);
        }

        /* Nope, not found. Try the next one */
        curr = curr->prevEntry;

        /* Note: at this point curr might not be pointing into an
         * ice_dma_buffer. Our lists are cirularly linked but the root of the
         * list is a bare ATLISTENTRY pointer, not contained within an icebuf.
         */
    }

    /* not found */
//    dbg2( "%s %d not found\n", __FUNCTION__, row_idx );
    return NULL;
}

/**
 * \brief return a point to a row in an ice_filter channel 
 *
 * Search the running list for a row based on index. If not found in the
 * running list, attempt to pop a new icebuf from the pending list. If the row
 * cannot be found, return NULL.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static uint8_t *
get_row( struct ice_dma_mm *mm, struct ice_dma_channel *ch, 
         struct ice_filter_data *fd, int row_idx )
{
    ATLISTENTRY *first, *last;
    uint8_t *row_ptr;
    struct ice_dma_buffer *icebuf;

#ifdef ICE_FILTER_DEBUG
//    dbg1( "%s ch=%d idx=%d\n", __FUNCTION__, ch->channel, row_idx );
#endif

    /* we'll search the list backwards since we're probably looking for a next
     * row 
     */
    first = ch->running.prevEntry;
    last = &ch->running;

    /* -1 to convert from count to index */
    row_ptr = find_row_in_list( row_idx, ch->expected_rows-1, first, last );

    if( row_ptr != NULL ) {
        /* found it! */
        return row_ptr;
    }

    /* didn't find it; try to pull the next buffer from the ready list */
    icebuf = ice_dma_get_ready( mm, ch );
    if( icebuf==NULL ) {
        /* no more buffers in ready list so no more data */
        return NULL;
    }

    /* we just got a fresh buffer; the first row of the new buffer better be
     * the next row we're looking for since the rows should all be contiguous
     *
     * Note the range check. We wind up here our very first row retrieval when
     * we start with an empty list. If row_idx<0, we want to return row[0] so
     * we can fake replication.
     */
    if( row_idx>=0 && row_idx<ch->expected_rows ) {
        XASSERT( icebuf->base_row==row_idx, icebuf->base_row );
    }
    row_ptr = icebuf->data;
    ASSERT( row_ptr!=NULL );

    /* attach to the end of our running queue */
    ATInsertTailList( &ch->running, &icebuf->listnode );
    ch->num_running++;
    
    /* icebuf now belongs to the running list */
    icebuf = NULL;

    return row_ptr;
}

/**
 * \brief  Run a simple software smoother over a 3x3 
 *
 * Runs a very simple but (mostly) fast smooth over 3 rows of input.
 *
 * There are a lot of things wrong with this function as of this writing
 * (10-Mar-06).
 * 
 *  - I don't smooth the left and right edge pixels. I do smooth the top and
 *  bottom rows using replication but I don't replicate the first and last
 *  pixels. I start smoothing at pixel[1] and stop at the second to last pixel.
 *
 *  - When I smooth row N into SN (Smoothed N), I wind up using SN to smooth
 *  row N+1. I should be using the original data (N) to smooth N+1. The results
 *  look ok so going to leave as is then fix later.
 *
 *  - This function is brutally hardwired to a 3x3 mask with the coefficients
 *      | 1 2 1 |
 *      | 2 4 2 |
 *      | 1 2 1 | 
 *    which allows for very fast operations.   
 *
 *  - I'm not using the cache very intelligently.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static void
smooth_it( uint8_t *dma_row_ptrs[], int mask_size, int num_pixels )
{
    int xpos, row;
//    int col;
    int avg;
    int midpoint;
    uint8_t* row_ptrs[ICE_FILTER_MASK_MAX];
//    static uint8_t mask[3][3] = {
//        { 1, 2, 1 },
//        { 2, 4, 2 },
//        { 1, 2, 1 }
//    };
    uint8_t *ptr0, *ptr1, *ptr2;


#ifdef ICE_FILTER_DEBUG
    /* XXX - assuming a 3x3 matrix */
    dbg1( "%s %p %p %p\n", __FUNCTION__, dma_row_ptrs[0], dma_row_ptrs[1],
                dma_row_ptrs[2] );
#endif

    for( row=0 ; row<mask_size ; row++ ) {
        row_ptrs[row] = CACHE_BYPASS_CLEAR( (uint32_t)dma_row_ptrs[row] );
    }

    /* XXX in order to make this particular function run as fast as possible, I
     * eventually hardwired it to a 3x3 on 8-bit pixels
     */
    ptr0 = row_ptrs[0];
    ptr1 = row_ptrs[1];
    ptr2 = row_ptrs[2];

    midpoint = mask_size/2;
    
    /* TODO ; smooth pixels at leftmost and right most edges; for now, just
     * start smoothing where we have data
     */
    for( xpos=midpoint ; xpos<num_pixels-midpoint ; xpos++ ) {

#if 0
        /* 3x3 simple average skipping upper left corner (only add and shift) 
         *
         * avg = [0,0]*0 + [0,1]*1 + [0,2]*1 
         *     + [1,0]*1 + [1,1]*1 + [1,2]*1
         *     + [2,0]*1 + [2,1]*1 + [2,2]*1
         *
         * avg /= 8
         *
         */
        avg = 4; /* start at 4 for rounding since we're dividing by 8 */

        /* unrolled */
//        avg += row_ptrs[0][0+xpos]; /* don't use upper left pixel */
        avg += row_ptrs[0][1+xpos];
        avg += row_ptrs[0][2+xpos];
        avg += row_ptrs[1][0+xpos];
        avg += row_ptrs[1][1+xpos];
        avg += row_ptrs[1][2+xpos];
        avg += row_ptrs[2][0+xpos];
        avg += row_ptrs[2][1+xpos];
        avg += row_ptrs[2][2+xpos];

        avg >>= 3;
#endif

        /* 3x3 gaussian */
#if 1
        /* We're doing a 3x3 with values that sum to 16 so we can divide by 16
         * (shift right 4). 
         */
        avg = 8; /* start at 8 for rounding since we're dividing by 16 */

        /* unrolled */
#if 0
        avg += row_ptrs[0][0+xpos]; /* x1 */
        avg += row_ptrs[0][1+xpos]<<1; /* x2 */ 
        avg += row_ptrs[0][2+xpos]; /* x1 */

        avg += row_ptrs[1][0+xpos]<<1; /* x2 */
        avg += row_ptrs[1][1+xpos]<<2; /* x4 */
        avg += row_ptrs[1][2+xpos]<<1; /* x2 */

        avg += row_ptrs[2][0+xpos]; /* x1 */
        avg += row_ptrs[2][1+xpos]<<1; /* x2 */
        avg += row_ptrs[2][2+xpos]; /* x1 */
#endif

        /* even faster */
        /* avg = [0,0]*1 + [0,1]*2 + [0,2]*1 
         *     + [1,0]*2 + [1,1]*4 + [1,2]*2
         *     + [2,0]*1 + [2,1]*2 + [2,2]*1
         *
         * group all the *2 
         * 2 * ([0,1] + [1,0] + [1,2] + [2,1])
         *
         * throw in the *4 as a single shift
         * 2 * ([0,1] + [1,0] + [1,2] + [2,1] + 2*[2,2])
         */
//        avg += (( ptr0[1+xpos] + ptr1[0+xpos] 
//                + ptr1[2+xpos] + ptr2[1+xpos] +
//                (ptr2[2+xpos]<<1) ) << 1)
//
//                + ptr0[0+xpos] + ptr0[2+xpos] 
//                + ptr2[0+xpos] + ptr2[2+xpos];

        avg += (( *(ptr0+1) + *(ptr1+0) + 
                + *(ptr1+2) + *(ptr2+1) +
                ((*(ptr2+2))<<1) ) << 1)

                + *(ptr0+0) + *(ptr0+2) 
                + *(ptr2+0) + *(ptr2+2);

        ptr0++;
        ptr1++;
        ptr2++;
        
//        for( row=0 ; row<mask_size ; row++ ) {
//            for( col=0; col<mask_size ; col++ ) {
//                avg += row_ptrs[row][col+xpos] * mask[row][col];
//            }
//        }

        avg >>= 4;
#endif

        /* set the middle pixel to our smoothed value */
        row_ptrs[midpoint][xpos+midpoint] = avg;
    }

    /* don't cache flush anything until we're completely done with the buffer
     * (happens in _launch()
     */
}

/**
 * \brief Load icebuf data into a convenient array of direct data pointers
 *
 * Builds an array of pointers to row data from the queued icebufs. This is the
 * first step to eventually running the filter NxN mask across a set of data.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static bool
load_row_list( struct ice_dma_mm *mm, 
               struct ice_dma_channel *ch, 
               struct ice_filter_data *fd, 
               int starting_row_idx )
{
    int i;
    int midpoint;
    int row_idx;

    /* make sure our mask size is odd */
    XASSERT( fd->mask_size&1, fd->mask_size );
    midpoint = fd->mask_size / 2;

    /* first row we will load into our array of pointers;
     * BIG FAT NOTE: perfectly ok for this to be negative since get_row() will
     * handle that for us
     */
    row_idx = starting_row_idx - midpoint;

    for( i=0 ; i<fd->mask_size ; i++ ) {
        fd->row_ptrs[i] = get_row( mm, ch, fd, row_idx );
        if( fd->row_ptrs[i]==NULL ) {
            /* nope, we didn't fill up our array. */
            return false;
        }

        /* yup, got a row */
        row_idx++;
    }

    /* hurray! we filled up our row array! return true telling launch he can
     * do a filter
     */
    return true;
}


void 
ice_filter_init( void )
{
    ice_dma_init( &ice_filter_mm, "icefilter", &ice_filter_driver,
            ice_filter_channels, ICE_FILTER_NUM_CHANNELS );
}

int 
ice_filter_open( uint8_t channels[], 
                 uint8_t num_channels,
                 int total_rows, 
                 int bytes_per_row )
{
    struct ice_dma_mm *mm;
    struct ice_filter_data *fd;
    int i, retval;

    XASSERT( num_channels <= ICE_FILTER_NUM_CHANNELS, num_channels );

    dbg2( "%s %d %d\n", __FUNCTION__, total_rows, bytes_per_row );

    /* use a convenience pointer */
    mm = &ice_filter_mm;

    /* We don't need data allocated for filter so pass zero for num_empties */
    retval = ice_dma_open( mm, channels, num_channels, 0, total_rows,
            bytes_per_row, ICEBUF_TAG_FILTER );

    /* ice_dma_open() returns the number of icebufs allocated; since we asked
     * for zero, make sure we get zero.
     */
    XASSERT( retval==0, retval );

    /* initialize our local ice_filter_data array */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        fd = &ice_filter_data[ mm->channels[i].channel ];

        memset( fd, 0, sizeof(struct ice_filter_data) );

        /* immediately turn it on since we're software; the is_enabled is only
         * used to pretend we're a hardware device to some icedma layers
         */
        fd->is_enabled = true;

        /* default to a 3x3 mask
         * TODO eventually support an API we could plug in any size
         */
        fd->mask_size = 3;
    }

    return 0;
}

void 
ice_filter_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    ice_dma_add_ready( &ice_filter_mm, channel, addme );
}

#if 0
/**
 * \brief Software filter to invert incoming data
 *
 * A really simple filter to just test the interface with scanpipe.
 * We'll just pop off all buffers, invert the data, and pass the buffer to
 * the callback
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static void
invert_filter( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    struct ice_dma_buffer *icebuf;
    int i;
    uint8_t *dataptr, *endptr;

    while( 1 ) {
        icebuf = icebuf_pop( &ch->readies );
        if( icebuf==NULL ) {
            break;
        }
        ch->num_readies--;

        dbg2( "filter %p\n", icebuf );

        /* don't bother clearing the cache bypass since we're only going to be
         * hitting each pixel once anyway
         */
        dataptr = icebuf->data;
        
        for( i=0 ; i<icebuf->num_rows ; i++ ) {
            dataptr = icebuf->data + i*icebuf->bytes_per_row;

            /* only whack the data, don't whack any padding 
             * XXX check to make sure my assumption that ch->bytes_per_row
             * doesn't include padding!
             */
            endptr = dataptr + ch->bytes_per_row;

            while( dataptr < endptr ) {
                *dataptr = ~*dataptr;
                dataptr++;
            }
        }

        XASSERT( ch->pending_rows >= icebuf->num_rows, ch->pending_rows );
        ch->pending_rows -= icebuf->num_rows;
        ch->completed_rows += icebuf->num_rows;

        /* is this our last buffer? */
        if( ch->pending_rows==0 ) {
            dbg2( "filter last strip %p\n", icebuf );
            XASSERT( ch->completed_rows==ch->expected_rows, ch->completed_rows );
            ice_buffer_callback( mm, ch, icebuf, true );
        }
        else {
            ice_buffer_callback( mm, ch, icebuf, false );
        }
        icebuf = NULL;
    }
}
#endif

/**
 * \brief  Attempt to start a filtering transaction.
 *
 * Attempts to load a set of data into an array of pointers for smoothing. If
 * there isn't enough data queued to fill the array, leave. We'll get called
 * again when there is more data.
 *
 * Once the list is loaded, run the filter. Currently (10-Mar-06), this is
 * hardwired to a simple smoother. Continue running the smoother as long as we
 * have icebufs in our queue.
 *
 * After consuming all available data, attempt to eject buffers out of our
 * queue via calls to the callback function. For an NxN filter mask, we keep N
 * icebufs in the queue for worst case of 1 row per icebuf.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void 
ice_filter_channel_launch( uint8_t channel )
{
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    struct ice_filter_data *fd;
    struct ice_dma_buffer *icebuf;
    int row_idx;
    bool retflag;
    uint8_t *next_row_ptr;
    int i, max_running_queue;

    mm = &ice_filter_mm;

#ifdef ICE_FILTER_DEBUG
//    dbg1( "%s(%d)\n", __FUNCTION__, channel );
#endif

    XASSERT( channel<mm->num_channels, channel );
    ch = &ice_filter_mm.channels[channel];

    fd = &ice_filter_data[ ch->channel ];

    /* simple test of scanpipe interface; pops buffers out on its own */
//    invert_filter( mm, ch );
//    return;

    /* Convert counter (completed_rows) to index of next row to smooth.
     * ch->completed_rows-1 is the index of last row smoothed. We want the next
     * row which is (ch->completed_rows-1)+1 == ch->completed_rows
     */
    row_idx = ch->completed_rows;

    /* fill up the row_ptrs[] array */
    retflag = load_row_list( mm, ch, fd, row_idx );
    if( !retflag ) {
        /* we didn't get a full compliment of data; we'll come back later and
         * try again.
         */
        return;
    }

    /* Bump up row_idx to point to the last row we fetched; we'll add 1 to
     * row_idx each time to get the next row in the loop below.
     *
     * Using fd->mask_size/2 because load_row_list() will fill row_ptrs[] with
     * the appropriate rows on either side of the starting row.
     *
     * For example, using a 5x5 mask we have 2 rows above and below the row
     * we're immediately trying to filter.
     */
    row_idx += fd->mask_size / 2;

    /* After the filter loop, we'll pop off any unnecessary buffers from the
     * running queue. Use the same code to handle both a normal running queue
     * (e.g., in the middle of an image) and the bottom of the image (when we
     * need to completely eject everything from the running queue), set the max
     * size of the running queue initially to our mask size. 
     *
     * In a 3x3 mask, for example, we'll keep 3 buffers in the running queue in
     * order to handle worse case. 
     *
     * Once pending_rows hits zero, the max_running_queue will be set to zero
     * so the loop after the filter loop will completely empty the running
     * queue.
     */
    max_running_queue = fd->mask_size;

    /* now run our filter algorithm(s) on that puppy */
    while( 1 ) {

        /* transmogrify the data */
        /* TODO ; add support for 16-bit pixels; by passing bytes_per_row, I'm
         * hardwiring this code to 8-bit pixels
         */
        smooth_it( fd->row_ptrs, fd->mask_size, ch->bytes_per_row );

        /* we just finished a row so update our counters */
        ch->completed_rows++;
        ch->pending_rows--;

        if( ch->pending_rows==0 ) {

            /* Force the loop after our filter loop to clean out the running
             * queue. (See comments a few lines up where max_running_queue is
             * initialized to fd->mask_size )
             */
            max_running_queue = 0;

            /* sanity check */
            XASSERT( ch->completed_rows==ch->expected_rows, ch->completed_rows);
            
            break;
        }

        /* the next row we'll try to fetch */
        row_idx++;

        next_row_ptr = get_row( mm, ch, fd, row_idx );
        if( next_row_ptr==NULL ) {
            /* no more data; bail and come back later */
            break;
        }

        /* collapse our row_ptrs[] structure so we can fit in the next row */
        for( i=0 ; i<fd->mask_size-1 ; i++ ) {
            fd->row_ptrs[i] = fd->row_ptrs[i+1];
        }
        fd->row_ptrs[i] = next_row_ptr;
    }

    /* now that we're done filtering our data, go through the running queue to
     * see if there's anything we can eject
     *
     * Alternately, if we've hit the bottom of the image, max_running_queue
     * will be zero and we'll simply clean out the queue completely.
     */
    while( ch->num_running > max_running_queue ) {
        icebuf = icebuf_pop( &ch->running );
        ch->num_running--;

        /* we modified the data so flush to get it out of our cache and into
         * memory again.
         */
        cpu_dcache_writeback_region( CACHE_BYPASS_CLEAR( (uint32_t)icebuf->data ),  
                                        icebuf->datalen );

        /* have we filtered all our data; if so, simply chuck everything we
         * have from the running list
         */
        if( ch->pending_rows==0 && ch->num_running==0 ) {
            dbg2( "filter last strip %p\n", icebuf );
            ice_buffer_callback( mm, ch, icebuf, true );
        }
        else {
            ice_buffer_callback( mm, ch, icebuf, false );
        }
        icebuf = NULL;
    }

}


void 
ice_filter_sanity( void )
{
    ice_dma_sanity( &ice_filter_mm );
}


void 
ice_filter_close( void )
{
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    struct ice_filter_data *fd;
    int i;

    mm = &ice_filter_mm;

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];

        if( !ch->is_open ) {
            continue;
        }

        fd = &ice_filter_data[ ch->channel ];

        dbg1( "icef channel=%d total_smooth_time=%d total_launch_time=%d diff=%d\n",
                    ch->channel, fd->smooth_time, fd->launch_time,
                    fd->launch_time-fd->smooth_time );
    }

    ice_dma_close( mm ); 
}

void 
ice_filter_cancel( void )
{
    ice_dma_cancel( &ice_filter_mm );
}

/**
 * \brief Pretend to reset a filter channel.
 *
 * Necessary to be able to interact with lower layer ICEDMA functions. Doesn't
 * need to do anything but will be called from ice_dma_cancel().  
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void 
ice_filter_channel_reset( uint8_t channel )
{
    /* NOOP */
}

/**
 * \brief Pretend to start a filter channel.
 *
 * Necessary to be able to interact with lower layer ICEDMA functions. Doesn't
 * need to do anything and probably won't be called.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void 
ice_filter_channel_start( uint8_t channel )
{
    /* NOOP */
}

/**
 * \brief enable a filter channel 
 *
 * Only really needed to interact correctly with icedma.c functions.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void 
ice_filter_channel_enable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<ice_filter_mm.num_channels, channel );
    ch = &ice_filter_mm.channels[channel];

    ice_filter_data[ ch->channel ].is_enabled = true;
}

/**
 * \brief return enable/disable state of a filter channel
 *
 * Only really needed to interact correctly with icedma.c functions
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

bool 
ice_filter_channel_is_enabled( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<ice_filter_mm.num_channels, channel );
    ch = &ice_filter_mm.channels[channel];

    return ice_filter_data[ ch->channel ].is_enabled;
}

/**
 * \brief Disable a filter channel.
 *
 * Only really needed to interact correctly with icedma.c functions
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void 
ice_filter_channel_disable( uint8_t channel )
{
    struct ice_dma_channel *ch;

    XASSERT( channel<ice_filter_mm.num_channels, channel );
    ch = &ice_filter_mm.channels[channel];

    ice_filter_data[ ch->channel ].is_enabled = false;
}

/**
 * \brief empty filler function 
 *
 * Will assert when called. Needed only to correctly interact with ICEDMA
 * functions.
 *
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

void ice_filter_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row )
{
    /* should not be called */
    ASSERT(0);
}

/**
 * \brief empty filler function 
 *
 * Will assert when called. Needed only to correctly interact with ICEDMA
 * functions.
 *
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */


void 
ice_filter_isr( uint8_t channel )
{
    /* should not be called */
    ASSERT(0);
}

