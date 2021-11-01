/*
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file pipecut.c
 *
 * \brief The pipe cutter cuts tof/bof in ADF scans.
 *
 * davep 08-Jan-2012 ; Remove the callback function pointer. Will now free the
 *       inter-page data or directly call the pipecut function for intra-page data.
 *
 * davep 10-Apr-2013 ; adding dual scan ADF capability (sensor on both sides of
 *                      paper)
 *
 * davep 22-Apr-2013 ; pipe cutter 2.0! Don't release inter-page data back to
 *        the heap. All inter-page icebufs are now sent back to scanpipe to be 
 *        re-used for WDMA. Data inside pages have the icebuf->data detached
 *        and a new buffer must be allocated.
 */

#include <stdlib.h>
#include <string.h>

#include "scos.h"

#include "list.h"
#include "lassert.h"
#include "memAPI.h"
#include "cpu_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantools.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "pipecut.h"
#include "scanpipe.h"
#include "pic.h"

//#define PIPE_CUTTER_DEBUG 

#ifdef PIPE_CUTTER_DEBUG 
#define pipecut_dbg2 dbg2
#else
#define pipecut_dbg2(...)
#endif

#define MAX_MARKERS 64

#define MARKER_COOKIE 0x90FAFBD0 

struct marker {
    /* We're moving things around by void pointers. Trust nothing. */
    uint32_t cookie;

    /* When an icebuf containing row_number is reached, we change how buffers
     * are handled. Buffers between pages are PTR_FREE'd. Buffers within
     * pages are sent back to scanpipe.
     */
    uint32_t row_number;

    /* markers are allocated at compile-time and stored in a linked list */
    ATLISTENTRY listnode;
};

struct pipe_cutter_channel {
    uint8_t channel;

    bool is_open;

    /* incoming icebuf->base_row stamped with this value */
    uint32_t next_base_row;

    ATLISTENTRY running;

    /* use signed int's to catch underrun */
    int num_running;

    /* marker for cutting this channel's data stream */
    struct marker *curr_marker;

    /* davep 07-Jan-2013 ; track tof/bof to be able to handle trimming boundary
     * icebufs
     *      in_page==false => waiting for tof
     *      in_page==true  => waiting for bof
     */
    bool in_page;

    ATLISTENTRY free_marker_list;
    ATLISTENTRY pending_marker_list;
    
    /* storage for the elements in the free/active marker list */
    struct marker marker_array[MAX_MARKERS];

    /* davep 22-Apr-2013 ; send icebuf back to scanpipe to be used for wdma
     * again  
     */
    scanpipe_recycler_callback scanpipe_recycler;

    uint32_t running_rows;

    /* davep 09-Jul-2013 ; handle parallel duplex (2+ sensors scanning
     * simultanouesly)
     */
    uint32_t page_side_offset;
};

struct pipe_cutter_mm {
    uint8_t is_open;

    uint8_t page_side;

    struct pipe_cutter_channel channels[MAX_PIPE_CUTTER_CHANNELS];
};

/* davep 10-Apr-2013 ; as of this writing, paper has two sides */
#define MAX_PIPE_CUTTERS 2

static struct pipe_cutter_mm pipe_cutter_list[MAX_PIPE_CUTTERS];


static void marker_list_print( ATLISTENTRY *list )
{
    int i;
    struct marker *mark;
    ATLISTENTRY *first, *curr;
    
    /* is the list empty? */
    if( list->nextEntry==list ) {
        dbg1( "%s list=%p is empty\n", __FUNCTION__, list );
        return;
    }

    /* walk the icebuf list without destroying it */
    first = list;
    curr = list->nextEntry;

    /* loop through our marker list */
    i=0;
    do {
        mark = CONTAINING_RECORD( curr, struct marker, listnode );
        XASSERT( mark->cookie==MARKER_COOKIE, (uint32_t)mark );

        dbg1( "%s [%d] row=%d\n", __FUNCTION__, i, mark->row_number);

        curr = curr->nextEntry;
        i += 1;
    } while( curr != first );
}

static void list_push( ATLISTENTRY *list, struct marker **p_marker )
{
    struct marker *marker;

    /* add marker to a list */
    PTR_ASSIGN(marker, *p_marker );
    XASSERT( marker->cookie==MARKER_COOKIE, (uint32_t)marker );
    ATInsertTailList( list, &marker->listnode );
}

static struct marker * list_pop( ATLISTENTRY *list )
{
    ATLISTENTRY *entry;
    struct marker *marker;

    entry = ATRemoveHeadList( list );
    if( !entry ) {
        return NULL;
    }

    marker = CONTAINING_RECORD( entry, struct marker, listnode );
    XASSERT( marker->cookie==MARKER_COOKIE, (uint32_t)marker );
    return marker;
}

static void marker_list_init( struct pipe_cutter_channel *ch )
{
    int i;
    struct marker *marker;

    memset( ch->marker_array, 0, sizeof(ch->marker_array) );

    ATInitList( &ch->free_marker_list );
    ATInitList( &ch->pending_marker_list );

    for( i=0 ; i<MAX_MARKERS ; i++ ) {
        marker = &ch->marker_array[i];
        marker->cookie = MARKER_COOKIE;
        list_push( &ch->free_marker_list, &marker );
    }
}

static void marker_push( struct pipe_cutter_channel *ch, uint32_t row_number )
{
    struct marker *marker;

    dbg2( "%s row_num=%d\n", __FUNCTION__, row_number );

    marker = list_pop( &ch->free_marker_list );

    /* if this assert fails, we need to make MARKER_MAX_COUNT bigger */
    XASSERT( marker!=NULL, ch->channel );

    /* save the new marker */
    marker->row_number = row_number;

    /* if we already have it an active marker, put this new marker onto the
     * pending list.
     *
     * Otherwise, this new marker is our current marker.
     */
    if( ch->curr_marker ) {
        list_push( &ch->pending_marker_list, &marker );
    }
    else {
        PTR_ASSIGN( ch->curr_marker, marker );
    }
}

static void marker_pop( struct pipe_cutter_channel *ch )
{
    struct marker *marker;

    /* Active marker has been consume. Replace it with one from the pending
     * list.
     */
    XASSERT( ch->curr_marker, ch->channel );

    PTR_ASSIGN( marker, ch->curr_marker );

    /* poison it */
    marker->row_number = -1;
    /* free it */
    list_push( &ch->free_marker_list, &marker );

    /* get a new top marker (could be null!) */
    ch->curr_marker = list_pop( &ch->pending_marker_list );
}

static void channel_init( struct pipe_cutter_channel *ch, uint8_t channel )
{
    memset( ch, 0, sizeof(struct pipe_cutter_channel) );

    ch->channel = channel;

    ATInitList( &ch->running );
}

static void channel_open( struct pipe_cutter_channel *ch )
{
    /* before stomping all over the top of the channel, make sure everything is
     * empty (avoid memory leaks)
     */
    XASSERT( ATIsListEmpty( &ch->running ), (uint32_t)&ch->running );

    channel_init( ch, ch->channel );

    marker_list_init(ch);

    ch->is_open = true;

    /* davep 07-Jan-2013 ; (with Jun Wang) track tof/bof to handle buffer
     * splitting (start outside the page)
     */
    ch->in_page = false;
}

static void channel_sanity( struct pipe_cutter_channel *ch )
{
    int num_running;

    dbg2( "%s ch=%d\n", __FUNCTION__, ch->channel );

    /* check for leaks */
    num_running = count_list( &ch->running );

    XASSERT( num_running==ch->num_running, ch->channel );
}

static void channel_close( struct pipe_cutter_channel *ch )
{
    dbg2( "%s ch=%d\n", __FUNCTION__, ch->channel );

    ice_free_counted_list( &ch->running, ch->num_running );

    ch->num_running = 0;
    ch->is_open = false;

    /* davep 07-Jan-2013 ; (with Jun Wang) track tof/bof to handle buffer
     * splitting (assume outside the page)
     */
    ch->in_page = false;
}

void pipe_cutter_add_marker( uint32_t row_number )
{
    int i, p;
    struct pipe_cutter_mm *mm;
    struct pipe_cutter_channel *ch;

    dbg2( "%s rownum=%d\n", __FUNCTION__, row_number);

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) {
        /* use a shortcut */
        mm = &pipe_cutter_list[p];

        if( !mm->is_open ) {
            continue;
        }

        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }
            /* davep 09-Jul-2013 ; add page_side_offset for >1 sensor */
            marker_push( ch, row_number + ch->page_side_offset );
        }
    }
}

static int channel_flush_icebuf( struct pipe_cutter_mm *mm, 
                                 struct pipe_cutter_channel *ch )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    bool last_buffer, free_buffer;
    int num_rows_delta;
    int num_running; 

    /*
     *  This function is the core of the pipe cutter. Everything else is
     *  designed simply to get buffers into this function.
     */

    XASSERT( ch->is_open, ch->channel );

    num_running = 0;
#ifdef PIPE_CUTTER_DEBUG
    /* while we're here, run some sanity checks */
    num_running = count_list( &ch->running );
    XASSERT( ch->num_running==num_running, num_running );
#endif
    /* quiet a warning */
    num_running = num_running;

    /* anything flush-able? */
    if( ch->num_running==0 ) {
        return 0;
    }

    icebuf = icebuf_pop( &ch->running );

    /* we verified num_running>0 so icebuf_pop() shouldn't fail */
    XASSERT( icebuf!=NULL, ch->channel );
    ch->num_running--;
    ch->running_rows -= icebuf->num_rows;

    XASSERT( ch->num_running>=0, ch->num_running );
    XASSERT( ch->running_rows>=0, ch->running_rows );

    pipecut_dbg2( "%s icebuf=%p ch=%d base=%d nr=%d\n", __FUNCTION__, 
                icebuf, icebuf->channel, icebuf->base_row, icebuf->num_rows );

    /* 
     * Now, what should we do with this icebuf?  We're doing a FIFO.  We just
     * pulled an icebuf from the front of the list.  Where do we send it?
     */

    /* check for a marker, a position where we have to change where we're
     * sending the data. If this icebuf has jumped across the next marker, we
     * need to start sending the data based on that marker.
     *
     * TODO optimization opportunity ; instead of constantly polling the top of
     * the list we should find a way to cache it (global?) 
     *
     * burtp 13-Jan-2010 ; Note that icebuf->base_row is the starting row of the 
     *  icebuf, check to see if the marker falls inside this buffer.
     */
    last_buffer = false;

    /* davep 07-Jan-2013 ; if we're not in a page, free the buffer (could
     * change the decision later when we've found a TOF marker)
     */
    free_buffer = !ch->in_page;

    if( ch->curr_marker ) {
        pipecut_dbg2( "%s curr=%d base=%d\n", __FUNCTION__, 
                ch->curr_marker->row_number, icebuf->base_row );
    }

    if( ch->curr_marker && 
        ch->curr_marker->row_number >= icebuf->base_row  &&
        ch->curr_marker->row_number <= (icebuf->base_row + icebuf->num_rows - 1) ) {

        /* at this point, we have an icebuf that contains a marker */

//        dbg1( "%s marker row=%d icebuf base=%d nr=%d data=%p bpr=%d\n", __FUNCTION__,
//                ch->curr_marker->row_number, icebuf->base_row, icebuf->num_rows,
//                icebuf->data, icebuf->bytes_per_row );

        /* davep 08-Oct-2009 ; bug 14451; cut the icebuf at the marker position
         * +1 to convert from index to count
         */
        /* davep 07-Jan-2013 ; (with Jun Wang) handle top/bottom icebufs
         * differently. On bof, truncate bottom icebufs to closest row.
         * On tof, copy the best data to the top of the icebuf.
         */
        if(ch->in_page) {
            /* We are in a page so this buffer contains good data followed by
             * bad data. Truncate the icebuf to only contain good data.
             */
            num_rows_delta = (int)ch->curr_marker->row_number - icebuf->base_row + 1;
            if( num_rows_delta > 0 ) {

                dbg2( "%s changing icebuf %p from num_rows=%d to num_rows=%d (max_rows=%d)\n", 
                            __FUNCTION__, icebuf, icebuf->num_rows, num_rows_delta,
                            icebuf->max_rows );

                /* truncate the buffer to make more accurate */
                icebuf->num_rows = num_rows_delta;
            }

            /* this will be the last buffer in the page */
            last_buffer = true;

            /* we are now between pages; will will special case handle TOF next */
            ch->in_page = false;
        }
        else {
            /* davep 07-Jan-2013 ; At top of new page. The icebuf is split
             * between bad data (top of icebuf) and good page data (bottom of
             * icebuf). Copy the good page data to the top of the icebuf buffer
             * then truncate.
             */
            int num_good_rows, num_bad_rows, num_good_bytes;
            uint8_t *bottom_row_ptr;

            /* we want to send this buffer downstream */
            free_buffer = false;

            /* bottom of the icebuf is the top few rows of a new page */
            num_good_rows = icebuf->base_row + icebuf->num_rows - ch->curr_marker->row_number;

            /* top of the icebuf is the press plate (junk) */
            num_bad_rows = icebuf->num_rows - num_good_rows;

            if( num_good_rows > 0 ) { 
                dbg2( "%s copying rows=%d to top of icebuf=%p\n", __FUNCTION__,
                        num_good_rows, icebuf );

                /* skip past the bad data to find page data */
                bottom_row_ptr = icebuf->data + num_bad_rows*icebuf->bytes_per_row;
                /* copy the good to the top of the buffer where downstream code can
                 * find it
                 */
                num_good_bytes = num_good_rows*icebuf->bytes_per_row;

                /* allow the memcpy to fiddle the data */
                icebuf_dma_unmap(icebuf);

                /* copy top of page from bottom of buffer to top of buffer */
                memcpy( icebuf->data, bottom_row_ptr, num_good_bytes  );

                /* push memcpy'd data from cpu cache into ram */
                icebuf_dma_map(icebuf);

                /* truncate to match our new data */
                icebuf->num_rows = num_good_rows;
            }

            /* we are now inside a page; will special case handle BOF next */
            ch->in_page = true;
        }

        /* davep 19-Oct-2009 ; XXX temp debug ; stomp the first and last row so
         * we can see where we're landing
         */
#if 0
        uint8_t *ptr;
        ptr = icebuf->data;
        memset( ptr, 0xff, icebuf->bytes_per_row );
        cpu_dcache_writeback_region( ptr, icebuf->bytes_per_row );
        ptr = icebuf->data + icebuf->bytes_per_row * (icebuf->num_rows-1);
        memset( ptr, 0xff, icebuf->bytes_per_row );
        cpu_dcache_writeback_region( ptr, icebuf->bytes_per_row );
#endif

        /* switch to new marker */
        marker_pop( ch );

        if( !ch->curr_marker ) {
            dbg2( "%s no new marker\n", __FUNCTION__ );
        }
        else {
            dbg2( "%s row_num=%d is now our current marker\n", __FUNCTION__,
                        ch->curr_marker->row_number );
        }
    }

//    dbg2( "%s running=%d last_buffer=%d\n", __FUNCTION__, ch->num_running, last_buffer );

    if( free_buffer ) {
        /* this is page gap data; do not send downstream */
        pipecut_dbg2( "%s base=%d page gap; free\n", __FUNCTION__, icebuf->base_row );
    }
    else {
        /* this is valid data; send downstream */
        pipecut_dbg2( "%s base=%d valid; send downstream\n", __FUNCTION__, icebuf->base_row );
        scerr = pipecut_callback_send_to_scanman( mm->page_side, icebuf, last_buffer );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    /* return the icebuf (maybe with, maybe without data) from whence it came */
    ASSERT( ch->scanpipe_recycler );

    /* davep 01-Jul-2013 ; TODO must do careful magic on determining the
     * recycler's channel. For now, just send in the current channel.
     */
    ch->scanpipe_recycler( icebuf->channel, &icebuf );

    XASSERT( icebuf==NULL, icebuf->channel );

    /* return the number of icebufs we flushed */
    return 1;
}

scan_err_t pipe_cutter_open( uint8_t page_side, uint8_t channels[], int num_channels ) 
{
    int i;
    int channel;
    struct pipe_cutter_mm *mm;
    struct pipe_cutter_channel *ch;

    dbg2( "%s nc=%d side=%d\n", __FUNCTION__, num_channels, page_side);

    XASSERT(page_side<MAX_PIPE_CUTTERS, page_side);
    mm = &pipe_cutter_list[page_side];

    XASSERT( !mm->is_open, page_side );

    for( i=0 ; i<num_channels ; i++ ) {
        channel = channels[i];
        XASSERT( channel < MAX_PIPE_CUTTER_CHANNELS, channel );

        ch = &mm->channels[channel];
        XASSERT( !ch->is_open, ch->channel );

        channel_open( ch );
    }

    mm->page_side = page_side;
    mm->is_open = true;

    return 0;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 22-Apr-2013
 */

void pipe_cutter_set_recycler( scanpipe_recycler_callback fn )
{
    struct pipe_cutter_mm *mm;
    struct pipe_cutter_channel *ch;
    uint8_t page_side;
    uint8_t channel;

    for( page_side=0; page_side<MAX_PIPE_CUTTERS; page_side++ ) {
        mm = &pipe_cutter_list[page_side];
        if( !mm->is_open ) {
            continue;
        }

        for( channel=0 ; channel<MAX_PIPE_CUTTER_CHANNELS ; channel++ ) {
            ch = &mm->channels[channel];
            if( !ch->is_open ) {
                continue;
            }

            ch->scanpipe_recycler = fn;
        }
    }
}

static void channel_add_running( struct pipe_cutter_mm *mm, 
                                struct pipe_cutter_channel *ch, 
                               struct ice_dma_buffer **pp_icebuf )
{
    struct ice_dma_buffer *icebuf;

//    dbg2( "%s ch=%d icebuf=%p\n", __FUNCTION__, ch->channel, *pp_icebuf);

    /* use a temp to make life easier */
    PTR_ASSIGN( icebuf, *pp_icebuf );

    if( ch->running_rows > MAX_PIPE_CUTTER_RUNNING_ROWS ) {
        channel_flush_icebuf( mm, ch );
    }

    /* mark where this data starts */
    icebuf->base_row = ch->next_base_row;
    ch->next_base_row += icebuf->num_rows;

    ATInsertTailList( &ch->running, &icebuf->listnode );
    ch->num_running += 1;
    ch->running_rows += icebuf->num_rows;
}

/**
 * \brief  Add an icebuf to pipecutter
 *
 * icebuf will be held in a queue until an ADF event drops marker indicating
 * were we are in the page (between pages or in a page)
 *
 * \author David Poole
 * \date 22-Apr-2013
 */

void pipe_cutter_add_ready( uint8_t page_side, struct ice_dma_buffer **pp_icebuf)
{
    uint8_t channel;
    struct pipe_cutter_mm *mm;
    struct pipe_cutter_channel *ch;
    struct ice_dma_buffer *icebuf;

    PTR_ASSIGN( icebuf, *pp_icebuf );

//    dbg2( "%s page_side=%d icebuf=%p sensor_num=%d channel=%d\n", __FUNCTION__,
//            page_side, icebuf,
//            icebuf->sensor_num, icebuf->channel );

    XASSERT( icebuf->sensor_num==page_side, icebuf->sensor_num );

    XASSERT(page_side<MAX_PIPE_CUTTERS, page_side);
    mm = &pipe_cutter_list[page_side];

    XASSERT( mm->is_open, page_side );

    channel = icebuf->channel;
    XASSERT( channel < MAX_PIPE_CUTTER_CHANNELS, channel );

    ch = &mm->channels[channel];
    XASSERT( ch->is_open, ch->channel );
    XASSERT( ch->channel==channel, channel );

    channel_add_running( mm, ch, &icebuf );
}

void pipe_cutter_flush( void )
{
    int i, p;
    struct pipe_cutter_mm *mm;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_channel *channels_to_flush[MAX_PIPE_CUTTER_CHANNELS ];
    int num_channels_to_flush, buffers_flushed;

    dbg2( "%s\n", __FUNCTION__ );

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) { 
        mm = &pipe_cutter_list[p];
        if( !mm->is_open ) {
            continue;
        }

        /* in order to run the round robin loop below a little less stupid, make a
         * quick list of all open channels 
         */
        num_channels_to_flush = 0;
        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }
            channels_to_flush[num_channels_to_flush++] = ch;
        }

        do {
            buffers_flushed = 0;

            /* round robin through the channels so we make a best effort to send
             * the buffers in channel order.  In other words, we'll send
             * R,G,B,R,G,B,R,G,B buffers instead of R,R,R,G,G,G,B,B,B buffers.
             */
            for( i=0 ; i<num_channels_to_flush ; i++ ) {
                buffers_flushed += channel_flush_icebuf( mm, channels_to_flush[i] );
            }
        } while( buffers_flushed > 0 );
    }
}

void pipe_cutter_close( void )
{
    int i, p;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_mm *mm;

    dbg2( "%s\n", __FUNCTION__ );

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) { 
        mm = &pipe_cutter_list[p];
        if( !mm->is_open ) {
            continue;
        }

        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }

            channel_sanity( ch );
            channel_close( ch );
        }

        mm->is_open = false;
    }
}

void pipe_cutter_print_markers( void )
{
    int i, p;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_mm *mm;

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) { 
        mm = &pipe_cutter_list[p];
        if( !mm->is_open ) {
            continue;
        }

        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }

            dbg1( "%s side=%d ch=%d curr_marker=%d\n", __FUNCTION__,
                        mm->page_side, ch->channel, 
                        ch->curr_marker?ch->curr_marker->row_number:-1 
                        );
            marker_list_print( &ch->pending_marker_list );
        }
    }
}

void pipe_cutter_get_num_channels( uint8_t page_side, int *num_channels )
{
    int i;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_mm *mm;

    /* convenience function for scanpipe.c's inpage/!inpage state machine */

    XASSERT(page_side<MAX_PIPE_CUTTERS, page_side);
    mm = &pipe_cutter_list[page_side];

    for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }

        *num_channels += 1;
    }
}

/**
 * \brief  Set the (constant) offset between two sensors. 
 *
 * Used when scanning both sides of the page simultaneously (parallel duplex).
 * Both sides of the page are scanned simultaneously. The sensors are offset
 * from each other. When a marker is dropped, another marker + backside_offset
 * is added to the 2nd pipe cutter.
 *
 * \author David Poole
 * \date 09-Jul-2013
 */

void pipe_cutter_set_page_side_offset( uint8_t page_side, uint32_t offset )
{
    int i;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_mm *mm;

    dbg2( "%s side=%d offset=%d\n", __FUNCTION__, page_side, offset );

    XASSERT(page_side<MAX_PIPE_CUTTERS, page_side);
    mm = &pipe_cutter_list[page_side];

    for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }

        /* Note I'm setting the same offset on all channels (r,g,b). */
        ch->page_side_offset = offset;
    }
}

static void channel_dump( const struct pipe_cutter_mm *mm, 
                          const struct pipe_cutter_channel *ch )
{
    dbg1( "pipecut side=%d ch=%d next_base_row=%d num_running=%d running_rows=%d\n", 
            mm->page_side, ch->channel, ch->next_base_row, ch->num_running, ch->running_rows );
}

void pipe_cutter_dump( void )
{
    int i, p;
    struct pipe_cutter_channel *ch;
    struct pipe_cutter_mm *mm;

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) {
        mm = &pipe_cutter_list[p];

        if( !mm->is_open ) {
            continue;
        }

        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }
            channel_dump( mm, ch );
        }
    }
}

void pipe_cutter_onetime_init( void )
{
    struct pipe_cutter_mm *mm;
    int i, p;

    memset( &pipe_cutter_list, 0, sizeof(pipe_cutter_list) );

    for( p=0 ; p<MAX_PIPE_CUTTERS ; p++ ) {
        mm = &pipe_cutter_list[p];

        for( i=0 ; i<MAX_PIPE_CUTTER_CHANNELS ; i++ ) {
            channel_init( &mm->channels[i], i );
        }
    }
}

