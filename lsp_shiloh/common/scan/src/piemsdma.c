/**
 * \file piemsdma.c
 *
 * \brief PIE MultiStrip DMA functions
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
#include "cpu_api.h"
#include "interrupt_api.h"
#include "regAddrs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantask.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "piehw.h"
#include "pie.h"
#include "strippr.h"
#include "scanvars.h"
#include "stripdma.h"

/* Define to turn on more verbose debugging */
//#define PIEMS_DMA_DEBUG

#ifdef PIEMS_DMA_DEBUG
    #define pie_dbg1 dbg1 
    #define pie_dbg2 dbg2
#else
    /* davep 18-Jul-2008 ; run the code silently (even dbg1 quieted for
     * performance problems)
     */
    #define pie_dbg1(...) 
    #define pie_dbg2(...) 
#endif

/* PIEMS_DMA_BUFFER_POISON - memset() the DMA data buffers before freeing */
#define PIEMS_DMA_BUFFER_POISON
 
/* 13-June-05 ; shotgun debug. Having a hard crash when leaving the channel
 * launch function. If I only load one DMA transaction into the system at a
 * time, it doesn't happen. I'm fiddling with the number of strips I'm leaving
 * queued thinking it has to do with how many transactions I have running at
 * a time and the fact that I'm queuing the hardware instead of my own internal
 * counter to determine that number.
 *
 * Do not set this less than three! Three shall the number you shall count and the
 * number you should count shall be three. Do not set to two. Zero is right
 * out.
 */
#define FLUSH_THRESHOLD  3

/* forward prototypes */
void piems_rdma_channel_load( uint8_t channel, uint32_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row );
void piems_wdma_channel_load( uint8_t channel, uint32_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row );

/* The original predictor was written by Won Rhee in Perl and translated into a
 * C state machine by DaveP. I tried to keep as closely as possible to Won's
 * Perl.
 */

/* We need to track each channel's state across multiple strips/buffers. I
 * don't want to modify the ice_dma_channel structure just for the MultiStrip
 * driver so I create a set of channel_multistrip structure arrays that mirrors
 * the driver's channels.
 */
struct channel_multistrip {
    uint8_t channel;

    /* Need to be able to assign row numbers to ice_dma_buffers we pull from
     * the ready list. We track the next expected row number here.
     */
    int next_row;

    /* 15-June-05 davep; Image space vs Capture space. CCD sensors' pixels
     * aren't all lined up at exactly the same row. When we capture, we'll see
     * different rows in one snapshot. In order to capture an entire image,
     * we'll wind up capturing data we don't really want to see. The
     * image_offset is the number of rows of capture data before the real image
     * data starts. This number will usually be <=0 since our image space
     * starts at zero.
     */
    int image_offset;
    
    /* 9-June-05 ; Number of rows per strip (in other words, the number of rows
     * read/written in one DMA transaction). Could be zero in older code
     * (before this date) which assumes the number of rows in an ice_dma_buffer
     * is the same as the strip_size. The assumption went out the window once
     * PIE MultiFilter and XYScale were enabled.
     *
     * davep 24-May-2011 ; moved from ice_dma_channel here because piemsdma was
     * the only user
     */
    int dma_strip_size;

    /* When MultiFilter is enabled, the channels' read DMA 'strip_size' will be
     * different than the actual number of rows coming out the bottom of
     * MultiFilter.  MF consumes two rows above and two rows below every row
     * sent into it so we must pad every strip with two above and two below.
     * When we advance the row counter, we must advance by this number then
     * adjust for MultiFilter's needs. 
     *
     * This is the strip_size as it hits the top of XYScale; doesn't matter if 
     * XYScale is in bypass.
     */
    int xy_strip_size;

    /* Want to support any number of rows coming in, even those that don't
     * exactly match the DMA padding. If enable_pad isn't set, we truncate
     * (the default).
     *
     * true  => we pad the Y direction to meet DMA requirements 
     * false => we truncate anything in the last strip that doesn't match DMA
     *          requirements
     */
    /* 21-June-05 davep ; I chucked this feature off the sleigh last night.
     * We'll always pad. No truncation supported. I'll do my best to get rid of
     * the padded data on the write side so we don't have goofy image
     * artifacts.
     */
//    bool enable_pad;

    /* 14-June-05 ; we already sent our last buffer, don't send it again and
     * don't send any more data (this is a bit of a kludge)
     */
    bool last_sent;

    /* when the XYScaler is enabled, have to run the predictor to figure out
     * what the next strip might be
     */
    /* FIXME - move the predict_state out of the channel specific area into its
     * own queue since the predicted strips are exactly the same across each
     * channels (e.g. the 42nd strip prediction will be exactly the same across
     * all the read and write channels so why make that prediction for each
     * channel?)
     */
    bool scaler_enabled;
    struct predict_state state;
    int dma_strip[4];

    /* We set the channel's strip_queued to false to indicate we need to predict
     * another strip the next time the function is run. If true, we have a strip
     * still needing to be serviced. It's possible to predict a strip then have no
     * data space to hold it.
     */
    bool dma_strip_queued;

    /* Caller has told us PIE MultiFilter is enabled. We have to do some
     * extra buffer management to handle the extra 2 lines of context for each
     * line of data.
     */
    bool multifilter_enabled;

    /* 16-June-05; we detect the end of the image by comparing the user
     * specified total_rows against the end of the predicted strip. However,
     * the end of the predicted strip can be adjusted for MultiFilter. The
     * strip predictor will determine is_final_strip before making the
     * adjustment for MultiFilter so the calling code won't need to worry about
     * MultiFilter.
     */
    bool is_final_strip;

    /* 24-June-05 ; in order to handle padding the top of the image for
     * MultiFilter we need to have a starting state
     */
    bool is_first_strip;

    /* 20-June-05 ; if we have to pad the end of the data to meet stripsize
     * requirements, we'll get the data space from here. This is used only
     * in the read DMA and is allocated at the open(). Note this may or may
     * note be used but we have no way to tell until we get to the end of the
     * image.
     */
    struct ice_dma_buffer *pad_icebuf;

    /* 23-June-05 ; temporary data space for the first two lines of image data
     * used with MultiFilter. 
     *
     * Note I'm not using pad_icebuf because the image could be small enough we
     * would have to reuse the pad_icebuf for the write before the read was
     * finished. I cannot create any more dependencies on knowing the number of
     * rows beforehand so I cannot count on knowing the image is too small.
     *
     * Yes, I'm getting lazy.
     */
    uint8_t *top_pad; /* DMA ready pointer! */
    int top_pad_buflen;

    /* davep 25-May-2011 ; adding PIE ColorShift which eats two lines */
    bool colorshift_enabled;
};

//static struct channel_multistrip read_channel_strip_state[PIE_RDMA_NUM_CHANNELS];
//static struct channel_multistrip write_channel_strip_state[PIE_WDMA_NUM_CHANNELS];

//struct ice_dma_mm piems_rdma_mm;
//struct ice_dma_mm piems_wdma_mm;
//static struct ice_dma_channel piems_rdma_channels[PIE_RDMA_NUM_CHANNELS];
//static struct ice_dma_channel piems_wdma_channels[PIE_WDMA_NUM_CHANNELS];

/* davep 24-May-2013 ; In order to multiplex PIE, I'm using bank_list switching. 
 * https://en.wikipedia.org/wiki/Bank_switching
 *
 * In order to maintain state across two (or more) sensors' images, I will bank_list
 * switch these data structures. 
 */
#ifdef HAVE_NSENSOR_SUPPORT
    #define PIEMS_NUM_BANKS 2
#else
    #define PIEMS_NUM_BANKS 1
#endif

struct piems_bank {
    bool is_open;
    uint8_t banknum;
    struct ice_dma_mm piems_rdma_mm;
    struct ice_dma_mm piems_wdma_mm;
    struct ice_dma_channel piems_rdma_channels[PIE_RDMA_NUM_CHANNELS];
    struct ice_dma_channel piems_wdma_channels[PIE_WDMA_NUM_CHANNELS];
    struct channel_multistrip read_channel_strip_state[PIE_RDMA_NUM_CHANNELS];
    struct channel_multistrip write_channel_strip_state[PIE_WDMA_NUM_CHANNELS];

    /* Manage in-flight strips. If a strip is started, it must be allowed to
     * complete. In order to context switch PIE, we must prevent new strips
     * from starting until all existing pending strips are finished. PIE is
     * then idle and we can context switch.
     */
    struct stripdma_list stripdma;
    struct stripdma_strip strip_list[ICE_DMA_REG_DEPTH+1];
};

static struct piems_bank bank_list[PIEMS_NUM_BANKS];
static struct piems_bank *curr_bank;

void piems_dma_set_bank( uint8_t banknum ) 
{
    int i;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

//    dbg1( "%s %d\n", __FUNCTION__, banknum );

    if( curr_bank ) {
        mm = &curr_bank->piems_rdma_mm;
        for( i=0 ; i<mm->num_channels ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }
            /* davep 13-Jun-2013 ; TODO I want this assert back */
//            XASSERT( ch->running_rows==0, ch->channel );
        }
        mm = &curr_bank->piems_wdma_mm;
        for( i=0 ; i<mm->num_channels ; i++ ) {
            ch = &mm->channels[i];
            if( !ch->is_open ) {
                continue;
            }
            /* davep 13-Jun-2013 ; TODO I want this assert back */
//            XASSERT( ch->running_rows==0, ch->channel );
        }
    }

    XASSERT(banknum<PIEMS_NUM_BANKS,banknum);
    curr_bank = &bank_list[banknum];
    XASSERT(curr_bank->banknum==banknum, curr_bank->banknum);
}

scan_err_t piems_dma_open_bank( uint8_t banknum )
{
    struct piems_bank *bank;

    dbg2( "%s banknum=%d\n", __FUNCTION__, banknum );

    /* davep 24-May-2013 ; as of this writing, we only have two banks */
    XASSERT( banknum < PIEMS_NUM_BANKS, banknum );
    bank = &bank_list[banknum];
    XASSERT(!bank->is_open,banknum);
    XASSERT(bank->banknum==banknum, bank->banknum);

    bank->is_open = true;

    /* davep 24-Jun-2013 ; always default to bank 0 (have to initialize this
     * value somewhere)
     */
    curr_bank = &bank_list[0];

    return SCANERR_NONE;
}

void piems_dma_close_bank( uint8_t banknum )
{
    struct piems_bank *bank;

    dbg2( "%s banknum=%d\n", __FUNCTION__, banknum );

    /* davep 24-May-2013 ; as of this writing, we only have two banks */
    XASSERT( banknum < PIEMS_NUM_BANKS, banknum );
    bank = &bank_list[banknum];
    XASSERT(bank->is_open,banknum);
    XASSERT(bank->banknum==banknum, bank->banknum);

    bank->is_open = false;
}

void piems_dma_stall( void )
{
    stripdma_stall( &curr_bank->stripdma );
}

void piems_dma_reload( void )
{
    stripdma_reload( &curr_bank->stripdma );
}


/* Because of the re-entrant functions that could be using the global
 * "curr_bank", we must save/restore the global bank pointer
 */
#define FOR_ALL_BANKS \
    int banknum, old_banknum; \
    old_banknum = curr_bank->banknum;\
    banknum = first_bank();\
    do {\
        piems_dma_set_bank(banknum);

#define END_FOR_ALL_BANKS \
        banknum = next_open_bank(banknum);\
    } while(banknum);\
    piems_dma_set_bank(old_banknum);

static int first_bank( void )
{
    return 0;
}

static int next_open_bank( int curr )
{
    /* next_open_bank() helps build an iterator across the bank_list list 
     *
     *
     * int b;
     * b = first_bank();
     * do {
     *    piems_dma_set_bank(b);
     *    (code)
     *    b = next_open_bank(b);
     * } while(b);   
     *
     * */
#if PIEMS_NUM_BANKS==1 
    /* return a zero value => false */
    return 0;
#else
    if( curr+1 >= PIEMS_NUM_BANKS ) {
        /* return a zero value => false */
        return 0;
    }
    if( !bank_list[curr+1].is_open ) {
        /* return a zero value => false */
        return 0;
    }
    return curr+1;
#endif
}

static void print_channel_strip_state( struct channel_multistrip *ms )
{
    pie_dbg1( "channel=%d next_row=%d dma_strip_size=%d xy_strip_size=%d strip=[%d,%d,%d,%d]\n", 
                ms->channel, ms->next_row, ms->dma_strip_size, ms->xy_strip_size, 
                ms->dma_strip[0], ms->dma_strip[1], ms->dma_strip[2], ms->dma_strip[3] );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------

static void 
print_running( const char *name, struct ice_dma_channel *ch )
{
#ifdef PIEMS_DMA_DEBUG
    struct ice_dma_buffer *icebuf;
    ATLISTENTRY *curr;

    curr = ch->running.nextEntry;
    while( curr != &ch->running ) {
        
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );

        pie_dbg1( "%s %s %d %p buffer=[%d,%ld]\n", __FUNCTION__, name, ch->channel, 
                    icebuf, icebuf->base_row, icebuf->base_row+(icebuf->num_rows-1) );
        curr = curr->nextEntry;
    }
#endif
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static struct ice_dma_buffer *
find_running_row( int row_idx, ATLISTENTRY *first, ATLISTENTRY *last )
{
    struct ice_dma_buffer *icebuf;
    ATLISTENTRY *curr;

    /* Strip search the icebuf list for the first buffer containing the 
     * row we're looking for.  Yes this is a linear search but this list is
     * going to be very short: 1, 2, or 3 entries long (keep reading for
     * details).
     *
     * Note our lists are circularly linked. 
     */

    curr = first;
    while( curr != last ) {

        /* XXX - get rid of the icebuf_sanity() check eventually */
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );

        /* -1 to convert from count to max index */
//        pie_dbg1( "%s find %d in %p [%d,%ld]\n", __FUNCTION__, row_idx, 
//                    icebuf, icebuf->base_row, icebuf->base_row+(icebuf->num_rows-1) );

        if( row_idx >= icebuf->base_row && 
            row_idx < icebuf->base_row+(int)icebuf->num_rows ) {
            /* found it! */
//            pie_dbg1( "%s found %d in %p\n", __FUNCTION__, row_idx, icebuf );
            return icebuf;
        }

        /* Nope, not found. Try the next one */
        curr = curr->nextEntry;

        /* Note: at this point curr might not be pointing into an
         * ice_dma_buffer. Our lists are cirularly linked but the root of the
         * list is a bare ATLISTENTRY pointer, not contained within an icebuf.
         */
    }

    /* not found */
//    pie_dbg1( "%s %d not found\n", __FUNCTION__, row_idx );
    return NULL;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
extern int dbg_cnt_piems_dma_isr;	//add.lsh.for pipe debug
static void 
piems_dma_isr( struct ice_dma_mm *mm, uint8_t channel, uint32_t intmsg )
{
    scan_msg_t msg;
    scan_err_t scerr;
    struct ice_dma_channel *ch;

	dbg_cnt_piems_dma_isr++;
    /* 
     * BIG FAT NOTE: THIS IS AN INTERRUPT HANDLER.
     *
     */

    ch = &mm->channels[channel];

    /* 14-June-05 ; we should only get into this function via a DMA complete
     * interrupt. If we get here any other way, Bad Things have happened and
     * our counters will get screwed up causing Worse Things to happen.
     *
     * Note also I'm overloading the meaning of running_rows. In the MultiStrip
     * driver, running_rows is actually "running_strips". 
     */
    XASSERT( ch->running_rows > 0, 0 ); 
    
    /* note: assuming channel has already been sanity checked by the caller */

    /* don't call memset() on the structure (it's expensive) but make sure
     * all unused message fields are set to empty 
     */
    msg.msgType = intmsg;

    /* davep 27-May-2013 ; adding sensor_num to aid demuxing multiple scan
     * sensors and pic/pie DMA
     */
    msg.param1 = SMSG_PIEMS_INT_SET_SENSOR_NUM(ch->sensor_num); 

    msg.param2 = channel;
    msg.param3 = 0; /* as yet unused */

    /* pass the finished buffer off to a thread */
    scerr = scantask_msg_send_nowait( &msg );

    if( scerr != 0 ) {
        /* Yikes, this is bad. We weren't able to tell the thread about the
         * interrupt. Bump up a counter and hope we can recover later.
         * Otherwise, we run the chance of stalling.
         */
        ch->msg_drops++;
    }
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 28-May-2013
 */

static bool piems_dma_is_idle( struct ice_dma_mm *mm )
{
    int i;
    struct ice_dma_channel *ch;

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }
        if( ch->running_rows != 0 ) {
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static struct ice_dma_buffer *
pop_ready( struct ice_dma_channel *ch )
{
    struct ice_dma_buffer *icebuf;
//    int cnt;

    /* davep 16-Oct-2005 ; XXX get rid of this test before checking in the file */
//    cnt = count_list( &ch->readies );
//    XASSERT( cnt==ch->num_readies, cnt );

    icebuf = icebuf_pop( &ch->readies );
    if( icebuf == NULL ) {
        XASSERT( ch->num_readies==0, ch->num_readies );
        ch->no_readies++;
    }
    else {
        ch->num_readies--;
    }

    /* davep 14-Aug-2012 ; set up the icebuf for device DMA (linux kernel port) */
    if( icebuf ) {
        /* davep 27-Mar-2013 ; moving dma_map_single() to safer location */
        XASSERT( icebuf->dma_handle!=(dma_addr_t)NULL, (uint32_t)icebuf->tag );
//        icebuf_dma_map( icebuf );
    }

//    pie_dbg1( "%s %d %p\n", __FUNCTION__, ch->channel, icebuf );

    return icebuf;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
push_running( struct ice_dma_channel *ch, struct ice_dma_buffer *icebuf )
{
//    pie_dbg1( "%s %d %p\n", __FUNCTION__, ch->channel, icebuf );

    /* davep 14-Aug-2012 ; porting to linux kernel so need 2 memory pointers.
     * Must be set before we arrive in this function.
     */
    XASSERT( icebuf->dma_handle, icebuf->tag );

    ATInsertTailList( &ch->running, &icebuf->listnode );
    ch->num_running++;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
load_read( struct ice_dma_channel *ch,
           struct channel_multistrip *ms,
           uint32_t dar1, uint32_t drcr1, 
           uint32_t dar2, uint32_t drcr2 ) 
{
//    dbg2( "%s ch=%d sensor=%d %x %d %x %d\n", __FUNCTION__, ch->channel,
//                ch->sensor_num, dar1, drcr1, dar2, drcr2 );

    pie_rdma_channel_load2( ch->channel, dar1, drcr1, 
                            dar2, drcr2, ch->bytes_per_row );
    pie_rdma_channel_start( ch->channel );

    ch->buffer_loads++;

#ifdef ICE_DMA_LOAD_LOG
    ice_dma_loadlog2( ch, dar1, drcr1, dar2, drcr2 );
#endif

    /* I'm overloading the meaning of running_rows. In the MultiStrip
     * driver, running_rows is actually "running_strips". 
     */
    ch->running_rows++;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
read_inter_buffer( struct ice_dma_channel *ch,
                   struct channel_multistrip *ms, 
                   struct ice_dma_buffer *leftbuf,
                   struct ice_dma_buffer *rightbuf )
{
    uint32_t dar1, dar2, drcr1, drcr2;
    int left_idx;

    pie_dbg2( "%s channel %d\n", __FUNCTION__, ch->channel );
    pie_dbg2( "leftbase=%d num_rows=%ld rightbase=%d num_rows=%ld\n",
                leftbuf->base_row, leftbuf->num_rows, 
                rightbuf->base_row, rightbuf->num_rows );

    /* davep 14-Aug-2012 ; porting to linux kernel so need 2 memory pointers.
     * Must be set before we arrive in this function.
     */
    XASSERT( leftbuf->dma_handle, leftbuf->tag );
    XASSERT( rightbuf->dma_handle, rightbuf->tag );

    /* left and right should be contiguous */
    XASSERT( leftbuf->base_row+leftbuf->num_rows ==
            rightbuf->base_row, rightbuf->base_row );

    /* calculate DAR1,DRCR1 and DAR2,DRCR2. Load and start the DMA */
    left_idx = ms->dma_strip[STRIP_READ_FIRST] - leftbuf->base_row;

    /* get a pointer to the row we want */
    dar1 = (uint32_t)(leftbuf->dma_handle + left_idx * leftbuf->bytes_per_row);
//    dar1 = (uint32_t)(leftbuf->data + left_idx * leftbuf->bytes_per_row);

    /* number of rows in the first half of the strip; 
     *
     * base_row+num_rows = base_row of next strip 
     *   index +  count  = index
     *
     * (base_row+num_rows) - strip_read_last = drcr1
     * (      index      ) - index           = count
     */
    drcr1 = (leftbuf->base_row + leftbuf->num_rows) - ms->dma_strip[STRIP_READ_FIRST];

    /* If we're using a second buffer, it will ALWAYS start at the begining
     * under our current design. We don't leave any "holes" in our data (rows
     * are contiguous across buffer boundries). Buffers always must contain at
     * least the same number of rows as our stripsize.
     */
    dar2 = (uint32_t)rightbuf->dma_handle;
//    dar2 = (uint32_t)rightbuf->data;

    /* number of rows in the second half of the strip ; +1 to convert index to
     * count 
     */
    drcr2 = (ms->dma_strip[STRIP_READ_LAST] - rightbuf->base_row) +1;

    /* davep 11-Jul-2006 ; catch the case where we're not setting up enough
     * space to meet the stripsize (overrunning the end of rightbuf)
     */
    XASSERT( drcr2 <= rightbuf->num_rows, drcr2 );

    /* avoid anything stupid sneaking out */
    XASSERT( drcr1 <= PIE_RDMA_MAX_ROWS, drcr1 );
    XASSERT( drcr2 <= PIE_RDMA_MAX_ROWS, drcr2 );
    XASSERT( drcr1+drcr2==ms->dma_strip_size, drcr1+drcr2 );
    
    load_read( ch, ms, dar1, drcr1, dar2, drcr2 );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
read_intra_buffer( struct ice_dma_channel *ch, 
                   struct channel_multistrip *ms,
                   struct ice_dma_buffer *icebuf )
{
    uint32_t dar1, drcr1;
    int row_idx;

    pie_dbg2( "%s channel %d\n", __FUNCTION__, ch->channel );

    /* davep 14-Aug-2012 ; porting to linux kernel so need 2 memory pointers.
     * Must be set before we arrive in this function.
     */
    XASSERT( icebuf->dma_handle, icebuf->tag );

    /* only use DAR1, DRCR1 but make sure DAR2, DRCR2 are set to zero */
    row_idx = ms->dma_strip[STRIP_READ_FIRST] - icebuf->base_row;

    /* get a pointer to the row in the buffer where this strip starts */
    dar1 = (uint32_t)(icebuf->dma_handle + row_idx * icebuf->bytes_per_row);
//    dar1 = (uint32_t)(icebuf->data + row_idx * icebuf->bytes_per_row);

    /* XXX - optimization opportunity ; read DMA stripsize is constant, why
     * should we calculate this every time?
     */
    /* +1 to convert from index to count */
    /* 16-June-05 ; ()'s are important! Strip indices can be negative numbers */
    drcr1 = (ms->dma_strip[STRIP_READ_LAST] - ms->dma_strip[STRIP_READ_FIRST]) + 1;

    /* avoid anything stupid sneaking out */
    XASSERT( drcr1 == ms->dma_strip_size, drcr1 );
    
    load_read( ch, ms, dar1, drcr1, 0, 0 );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
write_buffer( struct channel_multistrip *ms,
              struct ice_dma_channel *ch,
              struct ice_dma_buffer *icebuf )
{
    uint32_t dar, drcr;
    int row_idx;

    /* this function is almost exactly the same as read_intra_buffer() */

    pie_dbg1( "%s ch=%d sensor=%d buf=%p\n", __FUNCTION__, ch->channel,
            ch->sensor_num, icebuf );

    /* davep 14-Aug-2012 ; porting to linux kernel so need 2 memory pointers.
     * Must be set before we arrive in this function.
     */
    XASSERT( icebuf->dma_handle, icebuf->tag );

    /* dar  - Data Address Register
     * drcr - Data Row Count Register
     */
    row_idx = ms->dma_strip[STRIP_WRITE_FIRST] - icebuf->base_row;

    /* get a pointer to the row in the buffer where this strip starts */
    dar = (uint32_t)(icebuf->dma_handle + row_idx * icebuf->bytes_per_row);
//    dar = (uint32_t)(icebuf->data + row_idx * icebuf->bytes_per_row);

    /* +1 to convert from index to count */
    drcr = ms->dma_strip[STRIP_WRITE_LAST] - ms->dma_strip[STRIP_WRITE_FIRST] + 1;

    /* davep 01-Jul-2013 ; don't bother calculating the cpu addressible pointer */
    pie_wdma_channel_load( ch->channel, NULL, (dma_addr_t)dar, drcr, icebuf->bytes_per_row );
    pie_wdma_channel_start( ch->channel );

    ch->buffer_loads++;

#ifdef ICE_DMA_LOAD_LOG
    ice_dma_loadlog( ch, dar, drcr );
#endif

    /* We assume the number of rows we just plugged into the write DMA channel
     * will come out eventually. Even if we're simply putting something on the
     * top of the DMA hardware queue, increasing the num_rows effectively
     * "reserves" the space for our current strip. The next strip to come in
     * will check num_rows to see if we have space.
     */
    icebuf->num_rows += drcr;

    XASSERT( icebuf->num_rows <= icebuf->max_rows, icebuf->num_rows );

    /* I'm overloading the meaning of running_rows. In the MultiStrip
     * driver, running_rows is actually "running_strips". 
     */
    ch->running_rows++;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
flush_running( struct ice_dma_mm *mm, 
               struct ice_dma_channel *ch, 
               int max_running )
{
    struct ice_dma_buffer *icebuf;

    while( ch->num_running > max_running ) {
        icebuf = icebuf_pop( &ch->running );
        ch->num_running--;

        /* davep 14-Mar-2007 ; XXX temp debug */
        if( max_running==0 ) {
            pie_dbg1( "%s channel %d flushing %p data %p buffer [%d,%ld]\n", mm->name, ch->channel, 
                        icebuf, icebuf->data, icebuf->base_row, 
                        icebuf->base_row+(icebuf->num_rows-1) );
        }

        /* -1 to get the last row of the current buffer instead of the first
         * row of the next buffer
         */
        pie_dbg2( "%s channel %d flushing %p data %p buffer [%d,%ld]\n", mm->name, ch->channel, 
                    icebuf, icebuf->data, icebuf->base_row, 
                    icebuf->base_row+(icebuf->num_rows-1) );

        /* davep 09-Sep-2005 ; tweak the pending_rows and completed_rows here.
         * Don't touch running_rows since I'm using it in the MultiStrip driver
         * to mean something other than "rows that are running". Yes, it's a
         * kludge.  :-P
         *
         * BIG FAT NOTE: The Write DMA is initialized with the Read DMA's
         * pending_rows because of XYScale. If we're scaling up or down, the
         * Write DMA's pending_rows is going to be inaccurate or even negative.
         * But I still want to track the number of rows completed through Write
         * DMA. I've found these very useful when debugging even though they
         * don't completely match the expected values in the WDMA case.
         */
        /* davep 15-Jun-2009 ; add run forever by ignoring expected_rows */
        if( ch->expected_rows ) {
            ch->pending_rows -= icebuf->num_rows;
        }
        ch->completed_rows += icebuf->num_rows;

        /* davep 27-Mar-2013 ; moving dma_map_single() to safer location */
        XASSERT( icebuf->dma_handle!=(dma_addr_t)NULL, (uint32_t)icebuf->tag );
//        /* davep 14-Aug-2012 ; linux kernel port; make the data cpu accessible
//         * again
//         */
//        icebuf_dma_unmap( icebuf );

        /* Send the completed buffer to the appropriate place.
         *
         * max_running==0 is a special case. 0 means clean out the buffer queue
         * and expect no more data to be pushed into it. ch->num_running==0
         * means there are no current DMA transactions still being chewed by
         * the hardware (or rather, this driver doesn't THINK there are).
         */
        if( max_running==0 && ch->num_running==0 ) {
            pie_dbg2( "%s channel %d %p last buffer!\n", mm->name, ch->channel, icebuf );
            ice_buffer_callback( mm, ch, icebuf, true );
        }
        else {
            ice_buffer_callback( mm, ch, icebuf, false );
        }
    }
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
simple_next_strip( struct ice_dma_mm *mm, 
                   struct ice_dma_channel *ch, 
                   struct channel_multistrip *ms,
                   bool is_read )
{
    int strip_last;

    if( ms->scaler_enabled ) {
        XASSERT( ms->state.strip_height > 0, ms->state.strip_height );

        /* run XYScale strip predictor */
        strip_next( &ms->state, ms->dma_strip );
    }
    else { 
        /* Simply move the strip forward by the set strip size. 
         *
         * iPtr/oPtr is an index. strip_size is a count. strip[] is an index.
         * index = index + (count-1)
         *
         * -1 to convert from count to an index.
         */

        XASSERT( ms->xy_strip_size > 0, ms->xy_strip_size );
         
        /* XXX - eventually get rid of the array and just use ->dma_strip_first and
         * ->dma_strip_last. Want to stay as close to the original Perl predictor
         * until this whole thing is running.
         *
         * "We should forget about small efficiencies, say about 97% of the
         * time: premature optimization is the root of all evil."
         *      - Donald Knuth
         *
         * Make it run correctly then make it run fast.
         */
        ms->dma_strip[STRIP_READ_FIRST] = ms->state.iPtr;
        ms->dma_strip[STRIP_READ_LAST] = ms->state.iPtr + (ms->xy_strip_size-1);

        /* first row of next strip */
        ms->state.iPtr = ms->dma_strip[STRIP_READ_LAST] + 1;

        ms->dma_strip[STRIP_WRITE_FIRST] = ms->state.oPtr;
        ms->dma_strip[STRIP_WRITE_LAST] = ms->state.oPtr + (ms->xy_strip_size-1);

        /* first row of next strip */
        ms->state.oPtr = ms->dma_strip[STRIP_WRITE_LAST] + 1;
    }

    /* Detect end of image before we (possibly) change the strip boundries for
     * MultiFilter.
     */
    /* 18-June-05 davep ; with XYScale, the write cannot know how many rows it
     * will receive ahead of time. Write will have to watch the read.
     */
    strip_last = ms->dma_strip[STRIP_READ_LAST];

    /* 18-June-05 davep ; this assert is a temporary kludge to make sure all my
     * code is correctly passing in the same number of total rows. I'm checking
     * channel zero here because every read DMA uses channel zero.
     */
    XASSERT( ch->expected_rows==mm->channels[0].expected_rows, ch->expected_rows );

    /* -1 to convert from count to index
     * Once is_final_strip is set to true, it shouldn't change. 
     */
//    pie_dbg1( "%s %d last strip? %d >= %d?\n", mm->name, ch->channel, 
//                    strip_last, ch->expected_rows-1 );

    if( ch->expected_rows && strip_last >= ch->expected_rows-1 ) {
        pie_dbg1( "last strip %s ch=%d %d>=%d\n", mm->name, ch->channel,
                    strip_last, ch->expected_rows-1 );

        pie_dbg1("%s current strip=[%d,%d][%d,%d] iPtr=%d oPtr=%d\n", __FUNCTION__,
                    ms->dma_strip[STRIP_READ_FIRST],
                    ms->dma_strip[STRIP_READ_LAST],
                    ms->dma_strip[STRIP_WRITE_FIRST],
                    ms->dma_strip[STRIP_WRITE_LAST],
                    ms->state.iPtr, ms->state.oPtr );

        ms->is_final_strip = true;
    }

    /* Now factor MultiFilter into this stupid thing.
     *
     * MultiFilter only effects read.
     */
    if( ms->multifilter_enabled ) {
        /* The first strip needs to move up by 2 and down by 2.  My hands would
         * fall off from typing if I tried to explain why.
         */
        ms->dma_strip[STRIP_READ_FIRST] -= 2;
        ms->dma_strip[STRIP_READ_LAST] += 2;
    }

    /* davep 24-May-2011 ; throw ColorShift the two lines he eats (nom nom nom)
     */
    if( ms->colorshift_enabled ) {
        ms->dma_strip[STRIP_READ_LAST] += 2;
    }

    pie_dbg2("%s strip=[%d,%d][%d,%d] iPtr=%d oPtr=%d\n", __FUNCTION__,
                ms->dma_strip[STRIP_READ_FIRST],
                ms->dma_strip[STRIP_READ_LAST],
                ms->dma_strip[STRIP_WRITE_FIRST],
                ms->dma_strip[STRIP_WRITE_LAST],
                ms->state.iPtr, ms->state.oPtr );
}

/**
 * \brief replicates the last row of a buffer along the bottom of another
 * buffer. 
 *
 * Necessary to handle weird scaling factors, scan sizes, etc, to keep
 * MultiFilter happy.
 *
 *
 * \author David Poole
 * \date 16-Oct-2005
 *
 */

static void
last_row_replicate( struct ice_dma_buffer *icebuf, 
                    uint8_t *row_ptr,
                    int pad_rows )
{
    int i;
    uint8_t *padptr;

    pie_dbg1( "%s icebuf=%p max_rows=%ld num_rows=%ld base_row=%d row_ptr=%p pad_rows=%d\n", 
                __FUNCTION__, icebuf, icebuf->max_rows, icebuf->num_rows,
                icebuf->base_row, row_ptr, pad_rows );

    /* make sure we have enough space to pad otherwise we're
     * totally screwed. I don't *think* this is possible but...
     */
    XASSERT( pad_rows <= icebuf->max_rows-icebuf->num_rows, pad_rows);

    /* make doubly sure nothing can go wrong (bad bad memory corruption if we
     * screw up here)
     */
    if( pad_rows > icebuf->max_rows-icebuf->num_rows ) {
        /* don't copy anything */
        return;
    }

    padptr = icebuf->data + icebuf->num_rows * icebuf->bytes_per_row;

    for( i=0 ; i<pad_rows ; i++ ) {
        /* this memcpy() should be CPU data cache safe because we're
         * copying between two ice_dma_buffers' data which should ALWAYS be
         * DMA ready data.
         */
        memcpy( padptr, row_ptr, icebuf->bytes_per_row );
        pie_dbg1( "copy %ld bytes padding from %p to %p\n",
                    icebuf->bytes_per_row, row_ptr, padptr );

        /* move to next row */
        padptr += icebuf->bytes_per_row;
    }
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 20-June-05
// Description  : 
//
// 
//
// Note: I really really hate this function; it's ugly. It needs to be combined
// with a more general case of a running image.
//
// 16-Oct-05 davep
// I've run into a case I didn't originally think of back in June. What if the
// last strip spans two buffers AND we need to pad? I've run into this case
// with certain expected_rows values that create an unfilled final buffer.
// 
//    ...bbbbbbbbbb..bbb]           - image
//      [bbbbbbbbbb][bbbxxxxxxx]    - data buffers (x==unused space)
//              [ss..sssss]         - strip prediction
//
// I need to replicate the last row of the last buffer along the last buffer
// instead of my special purpose pad buffer.
//       
//
// 13-Mar-2007 davep
//  You know this is about the second anniversary of when I started working on
//  PIE?  Just in time.  I pretty much completely rewrote this function to be
//  much cleaner.  Needed to handle some new cases caused by the ADF.
//
//--------------------------------------------------------------------------
    
static bool 
send_last_strip( struct ice_dma_mm *mm, 
                 struct ice_dma_channel *ch, 
                 struct channel_multistrip *ms )
{
    struct ice_dma_buffer *left_icebuf;
    struct ice_dma_buffer *right_icebuf;
    int pad_rows;
    uint8_t *last_row_ptr;

    pie_dbg1( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    if( ms->last_sent ) {
        return true;
    }

    /* davep 07-Mar-2007 ; XXX temp debug */
    pie_dbg2( "dma_strip=[%d,%d,%d,%d]\n", ms->dma_strip[STRIP_READ_FIRST],
                ms->dma_strip[STRIP_READ_LAST],
                ms->dma_strip[STRIP_WRITE_FIRST],
                ms->dma_strip[STRIP_WRITE_LAST] );

    /* needed_rows is the number of rows to finish this last strip of this
     * scan; +1 to convert from index to count 
     */
    pie_dbg1( "needed_rows=%d expected_rows=%d\n", (ms->dma_strip[STRIP_READ_LAST]+1), 
                ch->expected_rows );

    pie_dbg1( "ms->next_row=%d\n", ms->next_row );

    print_running( mm->name, ch );

    /* find left (start of last strip should always be in valid data) */
    left_icebuf = find_running_row( ms->dma_strip[STRIP_READ_FIRST],
                                    ch->running.nextEntry, &ch->running );

    if( left_icebuf==NULL ) {

        left_icebuf = pop_ready( ch );
        if( left_icebuf==NULL ) {
            /* data hasn't been queued yet; leave and we'll try again later. */
            pie_dbg1("%s left_icebuf miss; try again later\n", mm->name );
            return false;
        }

        /* make it my own, put into running queue */
        left_icebuf->base_row = ms->next_row;
        ms->next_row += left_icebuf->num_rows;
        push_running( ch, left_icebuf );
    }

    pie_dbg1( "left_icebuf base_row=%d num_rows=%ld\n",
                left_icebuf->base_row, left_icebuf->num_rows );

    /* if our last strip fits entirely within our left_icebuf, we're done. */
    if( ms->dma_strip[STRIP_READ_LAST] < left_icebuf->base_row+left_icebuf->num_rows ) {
        /* the last strip fits entirely in our last buffer */
        pie_dbg1( "%s %d last strip lands in last buffer\n", mm->name, ch->channel );
        read_intra_buffer( ch, ms, left_icebuf );
        ms->last_sent = true;
        return true;
    }

    /* how much more do we need to fill our last strip? */
    pie_dbg1( "left contributes %ld\n", 
                  (left_icebuf->base_row + left_icebuf->num_rows) -
                   ms->dma_strip[STRIP_READ_FIRST] );

    /* search the running list for right icebuf */
    right_icebuf = find_running_row( ms->dma_strip[STRIP_READ_LAST],
                            ch->running.nextEntry, &ch->running );

    if( right_icebuf != NULL ) {
        /* whee! we're done !*/
        pie_dbg1( "found last row in my running list\n" );

        pie_dbg1( "right_icebuf base_row=%d num_rows=%ld\n",
                    right_icebuf->base_row, right_icebuf->num_rows );
    
        read_inter_buffer( ch, ms, left_icebuf, right_icebuf );
        ms->last_sent = true;
        return true;
    }

    /* try to find it in our readies list */
    right_icebuf = pop_ready( ch );
    if( right_icebuf!=NULL ) {
        pie_dbg1( "found right in my ready list\n" );
        /* mark it my own before I use it since I am pulling valid scan data
         * from the ready list. 
         */
        right_icebuf->base_row = ms->next_row;
        ms->next_row += right_icebuf->num_rows;

        /* do I need to pad this last buffer? +1 to convert from index to count */
        pad_rows = (ms->dma_strip[STRIP_READ_LAST] - ms->next_row) + 1;

        if( pad_rows > 0 ) {
            dbg1( "%s %d last buffer needs replicate\n", mm->name, ch->channel );

            last_row_ptr = right_icebuf->data + 
                (right_icebuf->num_rows-1) * right_icebuf->bytes_per_row;

            last_row_replicate( right_icebuf, last_row_ptr, pad_rows );

            right_icebuf->num_rows += pad_rows;

            XASSERT( right_icebuf->num_rows <= right_icebuf->max_rows,
                    right_icebuf->num_rows );
        }

        pie_dbg1( "right_icebuf base_row=%d num_rows=%ld\n",
                    right_icebuf->base_row, right_icebuf->num_rows );

        push_running( ch, right_icebuf );

        read_inter_buffer( ch, ms, left_icebuf, right_icebuf );
        /* we're done */
        ms->last_sent = true;
        return true;
    }

    /* At this point we have a decision to make. We still haven't gotten any
     * data to fill out the right side of the strip. Do we expect any more data
     * to arrive? If so, we can return false and be expected to be called
     * again. Otherwise, we'll have to replicate the last known good data and
     * hope for the best.
     */
    if( ch->expected_rows && ms->next_row < ch->expected_rows ) {
        pie_dbg1( "%s %d right_icebuf miss; try again later\n", mm->name,
                    ch->channel );
        return false;
    }

    /* yup, we're screwed. have to replicate */
    pie_dbg1( "could not find last row so must replicate\n" );
    pie_dbg1( "using pad icebuf for replication\n" );
    
    /* we have to pad so replicate the last row of valid data along our pad
     * buffer
     */
    /* -1 to convert from count to index */
    last_row_ptr = left_icebuf->data + (left_icebuf->num_rows-1) * 
                        left_icebuf->bytes_per_row;

    /* Calculate pad_rows
     *  pad_rows = (strip_size_needed) - (strip_size_have)
     *           = (ch->dma_strip_size)- (rows from left_icebuf)
     *
     * The funny equation for (rows from left_icebuf) was taking from
     * read_inter_buffer(). See that function for the explanation.
     *
     */
    pad_rows = ms->dma_strip_size -  
                ( (left_icebuf->base_row + left_icebuf->num_rows) -
                   ms->dma_strip[STRIP_READ_FIRST] );
    /* we shouldn't have gotten this deeply into this function unless we really
     * needed to replicate
     */
    XASSERT( pad_rows>0 && pad_rows<ms->dma_strip_size, pad_rows );

    last_row_replicate( ms->pad_icebuf, last_row_ptr, pad_rows );

    /* need to set the base_row of our pad to be contiguous with our last
     * buffer
     */
    XASSERT( ms->pad_icebuf->max_rows >= pad_rows, ms->pad_icebuf->max_rows);
    ms->pad_icebuf->base_row = left_icebuf->base_row + left_icebuf->num_rows;
    ms->pad_icebuf->num_rows = pad_rows;

    pie_dbg1( "ms->pad_icebuf base_row=%d num_rows=%ld\n",
                ms->pad_icebuf->base_row, ms->pad_icebuf->num_rows );

    /* davep 03-Jan-2013 ; set up the VMA/PMA addresses in the icebuf (linux
     * kernel port) 
     */
    icebuf_dma_map( ms->pad_icebuf );

    read_inter_buffer( ch, ms, left_icebuf, ms->pad_icebuf );

    /* we will send the pad once and only once */
    ms->last_sent = true;
    return true;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static bool
rdma_ending_state( struct ice_dma_mm *mm, 
                   struct ice_dma_channel *ch, 
                   struct channel_multistrip *ms )
{
    /* The end of the current strip is past the end of the data we expect.
     * This is the last prediction we should make. We need to begin the
     * process of finishing the image run.
     * 
     * At this point we can either truncate the image or pad it to fit DMA
     * requirements. Because of the vagarities of MultiFilter and XYScale,
     * the caller can't send us exactly the data we need to match the DMA
     * requirements so we need to handle adding/removing data to fit. 
     */
    pie_dbg1( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* If we haven't sent the last strip, keep trying. We can't check for the
     * running_rows == 0 until we've put the last strip into the system (the
     * final strip could be delayed because its data hasn't arrived yet)
     */
    if( !send_last_strip( mm, ch, ms ) ) {
        return false;
    }

    /* We're at the last strip and there are no more strips running. Therefore,
     * we must be at the end of the image. Send everything in our buffer queue
     * up and out.
     */
    if( ch->running_rows==0 ) {
        /* 15-June-05 ; Make sure we've sent all data through we expect (this is a
         * double (triple?) check on the strip predictor and my use of it.
         */
        /* davep 10-Oct-2007 ; removed this assert as part of handling the
         * wedgie. It's possible we'll get stopped halfway through a strip and
         * never get these numbers updated correctly
         */
//        XASSERT( ms->next_row >= ch->expected_rows, ms->next_row );

        flush_running( mm, ch, 0 );
    }
    return true;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 24-June-05
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
pad_top( struct ice_dma_channel *ch,
         struct channel_multistrip *ms,
         struct ice_dma_buffer *icebuf )
{
    uint32_t dar1, drcr1, dar2, drcr2;
    int i, padrows, datalen;
    uint8_t *padptr;

    pie_dbg1( "%s start\n", __FUNCTION__ );

    /* if we don't have to pad, we shouldn't get into this function */
    XASSERT( icebuf->base_row > ms->dma_strip[STRIP_READ_FIRST], 
             icebuf->base_row );

    padrows = icebuf->base_row - ms->dma_strip[STRIP_READ_FIRST];

    /* first, calculate the second half of the read (the real, unpadded, data) */
    dar2 = (uint32_t)icebuf->data;

    /* +1 to convert from index to count */
    drcr2 = (ms->dma_strip[STRIP_READ_LAST] - icebuf->base_row) + 1;

    /* create buffer space to hold the top pad */
    if( ms->top_pad==NULL ) {
        datalen = padrows * icebuf->bytes_per_row;

        ms->top_pad = ice_dma_alloc_buffer( datalen );
        if( ms->top_pad != NULL ) {
            ms->top_pad_buflen = datalen;
        }
    }

    if( ms->top_pad==NULL ) {
        /* fall back on using the first part of the data as padding; will
         * create image artifact but we want to keep moving and be fault
         * tolerant
         */
        dbg1( "PIE: failed to get memory for MultiFilter buffer!" );
        dar1 = dar2;
    }
    else { 
        /* replicate the top row of image data onto our pad */
        padptr = ms->top_pad;
        dar1 = (uint32_t)ms->top_pad;
        for( i=0 ; i<padrows ; i++ ) {
            /* this memcpy is CPU data cache safe because both pointers should be
             * up in the cache bypass area
             */
            pie_dbg1( "padding %ld bytes from %p to %p\n", icebuf->bytes_per_row, 
                        icebuf->data, padptr );
            memcpy( padptr, icebuf->data, icebuf->bytes_per_row );
            padptr += icebuf->bytes_per_row;
        }
    }
    drcr1 = padrows;

    load_read( ch, ms, dar1, drcr1, dar2, drcr2 );
}


//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 23-June-05
// Description  : 
//
//  Possible cases ('b'==buffer space, 's'==strip):
//
//  Case #1 : Entire buffer(s) before first strip. Discard buffer(s) before
//  strip. We won't know how many buffers to discard until we reach the start
//  of the strip. Eventually this case will fall into one of the next three
//  cases.
//      [bbbbbbbbbb].....??????
//                      [ssssss]
//      
//  Case #2 : First strip fits somewhere entirely in first buffer.
//      [bbbbbbbbbb]
//        [ssssss]
//  
//  Case #3 : First strip strides across first two buffers.
//      [bbbbbbbbbb][bbbbbbbbbb]
//              [ss..ssss]
//  
//  Case #4 : First strip preceeds first buffer (MultiFilter padding case).
//  Must replicate first row of buffer data onto a pad buffer.
//      [bbbbbbbbbb][bbbbbbbbbb]
//   [ss.ssss]
//
// 
//--------------------------------------------------------------------------
    
#define LOADED_NONE  1
#define LOADED_HALF  2
#define LOADED_ALL   3

static int
rdma_starting_state( struct ice_dma_mm *mm, 
                struct ice_dma_channel *ch, 
                struct channel_multistrip *ms, 
                struct ice_dma_buffer **left_icebuf )
                
{
    struct ice_dma_buffer *icebuf;
    int found_first, found_last;

    *left_icebuf = NULL;

    while( 1 ) {

        icebuf = pop_ready( ch );
        if( icebuf==NULL ) {
            return LOADED_NONE;
        }

        icebuf->base_row = ms->next_row;
        ms->next_row += icebuf->num_rows;

        /* does the start of the strip fit in this buffer? */
        found_first = ms->dma_strip[STRIP_READ_FIRST] >= icebuf->base_row;

        /* does the end of this strip fit in this buffer? */
        found_last = ms->dma_strip[STRIP_READ_LAST] <
                        (icebuf->base_row+(int)icebuf->num_rows);

        pie_dbg1( "strip=[%d,%d] base=%d num_rows=%ld found_first=%d found_last=%d\n",
                    ms->dma_strip[STRIP_READ_FIRST],
                    ms->dma_strip[STRIP_READ_LAST],
                    icebuf->base_row, icebuf->num_rows, 
                    found_first, found_last );

        /* Case #1 */
        if( !found_first && !found_last ) {
            /* Make sure we haven't run completely past the end of expected
             * data, i.e., we consumed an entire set of data without seeing our
             * start of image.
             */
            XASSERT( ms->next_row < ch->expected_rows, ms->next_row );

            /* Nothing of this buffer exists in the strip. Discard and start
             * over.
             */
            ice_buffer_callback( mm, ch, icebuf, false );

            /* Eating the pre-image data is very important to get right (CCD
             * RGB sensors are offset from each other).
             */
            pie_dbg1( "%s %d skipping buffer [%d,%d]\n", mm->name, ch->channel, 
                        icebuf->base_row, icebuf->base_row+(int)icebuf->num_rows );
            /* go back to top of loop and look for another */
            continue;
        }

        /* one way or the other, this icebuf will be running eventually */
        push_running( ch, icebuf );

        /* Case #2 or Case #3 */
        if( found_first ) {
            *left_icebuf = icebuf;

            /* let msrdma_launch() figure out where the other half of the strip
             * lands between Case #2 and Case #3 
             */
            return LOADED_HALF;
        }

        /* Case #4, the hardest. */
        ASSERT( found_last );
        pad_top( ch, ms, icebuf );
        return LOADED_ALL;
    }
    /* davep 02-Nov-2007 ; shouldn't get here; fix for warning in braindead
     * xt-xcc compiler 
     */
    ASSERT(0);
    return -1;
}
 
/**
 * \brief increase read icebuf size to fit a strip 
 *
 *  This function is a bit of a kludge. As part of ADF support, need to handle
 *  getting cut off right in the middle of a strip. Usually, a short strip will
 *  hang the DMA wait for complete. We need to make sure the read DMA is set up
 *  to exactly the correct strip size even though no data might be coming.
 *
 *  XXX note I'm not doing anything with the bottom of the strip. Bottom will be
 *  whatever garbage was left in the buffer when it was alloc'd.
 *
 * \author David Poole
 * \date 10-Oct-2007
 *
 */

static void
pad_read_icebuf( struct channel_multistrip *ms, struct ice_dma_buffer *icebuf )
{
    uint32_t drcr2;

    /* number of rows in the second half of the strip ; +1 to convert index to
     * count 
     */
    drcr2 = (ms->dma_strip[STRIP_READ_LAST] - icebuf->base_row) +1;

    /* don't pad past what we really have */
    XASSERT( drcr2 <= icebuf->max_rows, drcr2 );

    pie_dbg1( "%s setting num_rows from old=%ld to new=%ld\n", __FUNCTION__,
                icebuf->num_rows, drcr2 );
    icebuf->num_rows = drcr2;

    /* davep 09-Oct-2007 ; catch the case where we're not setting up enough
     * space to meet the stripsize (overrunning the end of icebuf)
     */
    XASSERT( drcr2 <= icebuf->num_rows, drcr2 );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static bool 
msrdma_launch( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    scan_err_t scerr;
    struct ice_dma_buffer *left_icebuf;
    struct ice_dma_buffer *right_icebuf;
    struct channel_multistrip *ms;
    int retcode;
    bool retflag;

    /*  NOTE! 
     * 
     *  NOT interrupt/thread safe! Assumes appropriate interrupts are
     *  disabled by the caller!
     */
//    dbg1("%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* davep 30-May-2013 ; adding PIE multiplexing */
    scerr = stripdma_try_push_read( &curr_bank->stripdma, ch->channel );
    if( scerr == SCANERR_NO_ENTRY ) {
        /* we're stalling the pipeline so don't add anything new */
//        dbg1( "%s read stalled bank=%d\n", __FUNCTION__, curr_bank->banknum );
        return false;
    }
    XASSERT(scerr==SCANERR_NONE,scerr);

    ms = &curr_bank->read_channel_strip_state[ch->channel];

    XASSERT( ms->dma_strip_size>0 && ms->dma_strip_size<=PIE_RDMA_MAX_ROWS, ms->dma_strip_size );

    /* Strips can span two buffers or lie entirely within one buffer. Predict a
     * strip (XYScale can cause goofy strip requirements) and decide where the
     * strip lies. left_icebuf holds the first row of the strip, right_icebuf
     * holds the last row of the strip. If left_icebuf==right_icebuf, the strip
     * lies entirely within the buffer.
     */

    /* If we have a predicted strip from the previous run, use it. Otherwise,
     * run the predictor state machine and see what we get.
     *
     * We can have a leftover strip prediction if in the last time we were
     * here, there wasn't enough buffers either running or pending to meet the
     * prediction.
     */
    if( !ms->dma_strip_queued ) {
        simple_next_strip( mm, ch, ms, true );

        /* +1 on the strip_size to convert from index to length */
        pie_dbg1( "%s %d new strip [%d,%d] strip_size=%d\n", mm->name, ch->channel, 
                    ms->dma_strip[STRIP_READ_FIRST], ms->dma_strip[STRIP_READ_LAST],
                    (ms->dma_strip[STRIP_READ_LAST]-ms->dma_strip[STRIP_READ_FIRST])+1 );
//                    ms->xy_strip_size );

        /* Sanity check; if the predictor screws up, we're hosed from here to
         * Sunday. (This assert is kind of ugly because I don't want to use any
         * temporary variables.)
         */
        /* 16-June-05 ; The strip indices may be negative so the ()'s are
         * extremely important.
         * For example, strip=[-2,1]
         * (1 - -2) + 1 = 4
         * 1 - (-2+1) = 2
         */
        XASSERT( (ms->dma_strip[STRIP_READ_LAST] - ms->dma_strip[STRIP_READ_FIRST]) + 1 
                  == ms->dma_strip_size, 
                 (ms->dma_strip[STRIP_READ_LAST] - ms->dma_strip[STRIP_READ_FIRST]) + 1);

        ms->dma_strip_queued = true;
    }

    pie_dbg1( "%s %d strip [%d,%d] next_row=%d\n", mm->name, ch->channel, 
                ms->dma_strip[STRIP_READ_FIRST], 
                ms->dma_strip[STRIP_READ_LAST],
                ms->next_row );

    /* Is this our last strip? (ie, does the end of the strip exceed the end of
     * the image?
     */
    if( ms->is_final_strip ) {
        retflag = rdma_ending_state( mm, ch, ms );
        /* everything we needed to do was done in ending_strip() */

        /* davep 30-May-2013 ; we successfully pushed a read into DMA so
         * update our strip info 
         */
        if( retflag ) {
            scerr = stripdma_push_read( &curr_bank->stripdma, ch->channel );
            XASSERT(scerr==SCANERR_NONE,scerr);
        }

        /* returning false here is a little tricky. */
        return false;
    }

    /* nothing running, start from scratch */
    left_icebuf = NULL;

    /* 24-June-05 ; Is this the first strip? We have to do some funky pading
     * thing when we're doing MultiFilter. 
     *
     * XXX - I really hate one-time inline things like this but I can't think
     * of any other way to handle the starting conditions.
     */
    if( ms->is_first_strip ) {
        retcode = rdma_starting_state( mm, ch, ms, &left_icebuf );

//        pie_dbg1( "starting state=%d\n", retcode );

        if( retcode==LOADED_NONE ) {
            /* we didn't find our starting buffer; come back later and try
             * again
             */
            return false;
        }

        /* at this point, we found our starting buffer so we're no longer in
         * the starting state
         */
        ms->is_first_strip = false;

        if( retcode==LOADED_ALL ) {
            /* The rdma_starting_state() loaded an entire strip into DMA so we can
             * leave now, telling our caller we indeed loaded a buffer. Make
             * sure to make a new strip prediction the next time in since we
             * consumed this strip entirely.
             */
            ms->dma_strip_queued = false;

            /* davep 30-May-2013 ; we successfully pushed a read into DMA so
             * update our strip info 
             */
            scerr = stripdma_push_read( &curr_bank->stripdma, ch->channel );
            XASSERT(scerr==SCANERR_NONE,scerr);

            return true;
        }

        /* at this point, start_state() should have found left_icebuf but left
         * finding right_icebuf up to us.
         */
        XASSERT( retcode==LOADED_HALF, retcode );
    }
    else { 

        /* Find where the first row of the strip lives; start search at the first
         * buffer in the running list. We do a linear search but there can be at
         * most three entries in our running list. (Keep reading to find out
         * why.)
         */
        /* XXX - is the check for ch->num_running necessary or can I call
         * find_running_row() on an empty list?
         */
        if( ch->num_running > 0 ) {
            left_icebuf = find_running_row( ms->dma_strip[STRIP_READ_FIRST],
                                            ch->running.nextEntry, &ch->running );
        }
    }

//    pie_dbg1( "left_icebuf=%p\n", left_icebuf );

    if( left_icebuf == NULL ) {
        /* davep 12-Jan-2006 ; do we have to pad again? MultiFilter + scale
         * factors that give us 1 row (plus 4 multifilter context) in mean we
         * have to feed in strips [-2,x] then [-1,x]. I wasn't really expecting
         * to have to pad twice when I originally wrote the piemsdma driver.
         */
        /* davep 17-Jan-2006 ; assert on this condition. We're always scanning
         * enough rows so we don't need to pad.
         */
        XASSERT(ms->dma_strip[STRIP_READ_FIRST] >= ms->next_row, ms->next_row);
//        if( ms->dma_strip[STRIP_READ_FIRST] < ms->next_row ) {
//            pie_dbg1( "have to double pad!\n" );
//            /* TODO - figure out how to double pad */
//        }

        /* There isn't a running buffer that contains the strip. Try to load
         * one from the pending list. Make sure we consume the pre-image data
         * (e.g., sensors with offset arrays) up to the desired starting row.
         */
        left_icebuf = pop_ready( ch );

        if( left_icebuf==NULL ) {
            /* No buffers; there isn't any data read to run through DMA. Leave
             * and we'll come back when there's data.
             */
            pie_dbg1( "%s %d no readies\n", mm->name, ch->channel );
            /* we didn't queue anything */
            return false;
        }
        /* At this point we have a new buffer containing the left edge of the strip
         * in left_icebuf that we have also put onto the running queue.
         */

        /* Multistrip read DMA needs to know where this buffer starts so we
         * can search the list later. Yes, it's lame.
         */
        left_icebuf->base_row = ms->next_row;
        ms->next_row += left_icebuf->num_rows;

        /* add it to the running row list */
        push_running( ch, left_icebuf );
    }

    /* at this point we better have something in the running queue! */
    XASSERT( ch->num_running > 0, ch->channel );

//    pie_dbg1( "searching for right_icebuf\n" );

    /* remember right_icebuf could be the same as left_icebuf; we want to find
     * the last row in the strip in the buffer list starting with the buffer in
     * which we found the first row in the strip.
     */
    right_icebuf = find_running_row( ms->dma_strip[STRIP_READ_LAST],
                            &left_icebuf->listnode, &ch->running );

//    pie_dbg1( "right_icebuf=%p\n", right_icebuf );

    if( right_icebuf == NULL ) {
        /* There isn't a running buffer that can contain the end of the strip.
         * Try to load one from the pending list.
         */
        right_icebuf = pop_ready( ch );
        if( right_icebuf == NULL ) {
            /* No buffers; there isn't any data read to run through DMA. Leave
             * and we'll come back when there's data. 
             *
             * Note that it's perfectly possible to have left_icebuf in the
             * running list but it not actually be running. The next pass
             * through this function we'll find we don't need to do a strip
             * prediction, we should find the just-loaded left_icebuf at the
             * tail of the running queue and grab hold of it again. Hopefully
             * the next time through, we'll have something on the readies list
             * to be come right_icebuf.
             */

            /* Note I'm not doing a search here because we should ALWAYS find
             * the end of strip either in the current buffer or the next
             * buffer. 
             */

            /* we didn't queue anything */
            return false;
        }

        /* davep 09-Oct-2007 ; handle wedgie case where we get a short icebuf
         * (num_rows of valid data is too small to fit a full strip so we have
         * to make it a full strip)
         */
#if 1
        right_icebuf->base_row = ms->next_row;
        
        /* number of rows in the second half of the strip ; +1 to convert index to
         * count 
         * XXX need to make this expression faster
         */
        if( (ms->dma_strip[STRIP_READ_LAST] - right_icebuf->base_row) +1 > right_icebuf->num_rows ) {
            pad_read_icebuf( ms, right_icebuf );
        }
        ms->next_row += right_icebuf->num_rows;
#else
        right_icebuf->base_row = ms->next_row;
        ms->next_row += right_icebuf->num_rows;
#endif

        /* add it to the running row list */
        push_running( ch, right_icebuf );
    }

    pie_dbg1( "left_icebuf=%p data=%p num_rows=%ld right_icebuf=%p data=%p num_rows=%ld\n", 
                left_icebuf, left_icebuf->data, left_icebuf->num_rows, 
                right_icebuf, right_icebuf->data, right_icebuf->num_rows );

    if( left_icebuf == right_icebuf ) {
        /* entire strip fits into one single buffer; we'll use DAR1 and set 
         * DAR2 to zero.
         */
        read_intra_buffer( ch, ms, left_icebuf );
    }
    else {
        /* strip spans two buffers; start them both via DAR1 and DAR2 */
        read_inter_buffer( ch, ms, left_icebuf, right_icebuf );
    }

    /* Can we pop a buffer? There can be at most 3 buffers running in DMA
     * because of our requirement that the buffers contain at least as many
     * rows as a single strip. 
     *
     * The DMA transation queue is two deep so we can have at most two strips
     * queued at any time.
     *
     * [b] - buffer
     * [s] - strip
     *
     * Case #1 - stripsize==buffersize 
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *  [ssssssssssss]
     *                [ssssssssssss]
     *
     * Case #2 - stripsize==buffersize, strips don't line up with buffer
     * boundries. Worst case.
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *     [sssssssss..sss]
     *                   [sssssssss..sss]
     *
     * Case #3 - stripsize < buffersize, strips contiguous (no XY, no MF)
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *  [ssssssssss]
     *             [s..sssssssss]
     *
     * Case #4 - stripsize < buffersize, strips contiguous but MF
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *  [ssssssssss]
     *        [ssssss..ssss]
     *
     * Case #5 - stripsize==buffersize, but XYScale predictor weirdness
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *     [sssssssss..sss]
     *                [sss.sssssssss]
     *
     * Case #6 - Hold onto your hats, MF+XY! Who knows that the foo might
     * happen? Only guarantee is the 2nd strips first row will be > 1st strip's
     * first row.
     *  [bbbbbbbbbbbb][bbbbbbbbbbbb][bbbbbbbbbbbb]
     *     [sssssssss..sss]
     *          [ssss..ssssssss]
     *
     * More cases? Yeah, probably
     *
     */
    flush_running( mm, ch, FLUSH_THRESHOLD );

    /* We successfully loaded a strip. Mark the channel as having no pending
     * strip.
     */
    ms->dma_strip_queued = false;

    /* davep 30-May-2013 ; we successfully pushed a read into DMA so
     * update our strip info 
     */
    scerr = stripdma_push_read( &curr_bank->stripdma, ch->channel );
    XASSERT(scerr==SCANERR_NONE,scerr);

    /* we queued something */
    return true;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 26-June-05
// Description  : 
//
// 
//--------------------------------------------------------------------------

static void
truncate_write( struct ice_dma_mm *mm, 
                struct ice_dma_channel *ch, 
                struct channel_multistrip *ms )
{
    struct ice_dma_buffer *icebuf;
    int actual_rows;
    int predicted_rows;
    int total_pad;
//    ATLISTENTRY *curr;
    const struct scanvars *sv;
    ATLISTENTRY *entry;

    pie_dbg1( "%s %s ch=%d\n", __FUNCTION__, mm->name, ch->channel );

    /* should only get in here on a last-strip condition */
    XASSERT( ms->is_final_strip, ch->channel );

    /* if we've already truncated and flushed, don't bother */
    if( ch->num_running==0 ) {
        return;
    }
    
    /* +1 to convert from index to count */
    predicted_rows = ms->dma_strip[STRIP_WRITE_LAST] + 1;

    /* Based on the scale factor, how many rows of real output data would we
     * expect? Remember I'm putting READ's expected rows into the WRITE
     * channel's expected_rows field because the write cannot know how many
     * rows to expect when running XYScale. 
     *
     * Confused yet? Yeah, so am I.
     */
    if( ms->scaler_enabled ) {
        pie_xy_calc_expected_rows( ch->expected_rows, ms->state.fracY, 
                scanvar_get()->xy_scaler_mode, &actual_rows);
    }
    else {
        /* if we're not running the scaler, we could still get weird row
         * padding due to buffer sizes and strip sizes
         */
        actual_rows = ch->expected_rows;
    }

    pie_dbg1( "predicted_rows=%d actual_rows=%d\n", predicted_rows, actual_rows );

    /* when using XYScale, we're padding, we should always wind up with the
     * same or more than we expected 
     */
    total_pad = predicted_rows - actual_rows;

    /* davep 11-Jul-2006 ; (experimental) what did the user expect given area
     * scanned, the dpi, and the scale factor?
     */
    sv = scanvar_get();
    if( sv->pie_truncate_rows > 0 ) {
        /* only use if > 0 (zero implies disabled) */
        pie_dbg1( "%s truncate_rows=%d\n", __FUNCTION__, sv->pie_truncate_rows );
        /* this if is a safety check to make sure I didn't do something stupid */
        if( sv->pie_truncate_rows < predicted_rows ) {
            total_pad = predicted_rows - sv->pie_truncate_rows;
            pie_dbg1( "%s setting total_pad to %d\n", __FUNCTION__, total_pad );
        }
    }
    /* end davep 11-Jul-2006 */

    /* davep 06-Jan-2006 ; quick hack while working on solution to the
     * multifilter 1200 DPI 12616->12615 problem (bug 397)
     */
#if 0
    if( total_pad<0 ) {
        /* davep 06-Jan-2006 ; we didn't pad and we didn't need to pad since
         * the output is less than the input (probably because the input had to
         * be adjusted for multifilter
         */
        /* davep 10-Jan-2006 ; not sure this is correct! */
        pie_dbg1( "%s total_pad=%d\n", __FUNCTION__, total_pad );
        return;
    }
#endif
    /* davep 06-Jan-2006 ; end quick hack */

    XASSERT( total_pad >=0, total_pad );

    /* The most padding we will see is strip_size-1. Each buffer must be >=
     * strip size so our running queue should hold all of whatever we needed to
     * pad since we keep at least 3 buffers in the queue.
     */

    print_running( mm->name, ch );

#if 1
    icebuf = NULL;
    while( total_pad > 0 ) {

        /* can't use icebuf_pop() because we want to pull from the tail of the
         * list and icebuf_pop() pulls from the head
         */
        entry = ATRemoveTailList( &ch->running );

        /* important to keep our counters accurate! */
        ch->num_running--;

        /* shouldn't ever run into an empty list; the reason why is left as an
         * exercise for the reader
         */
        ASSERT( entry!=NULL );

        icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );

        /* XXX - get rid of the icebuf_sanity() check eventually */
        icebuf_sanity( icebuf );

        pie_dbg1( "taking %d rows of padding from %p num_rows=%ld\n", 
                    total_pad, icebuf, icebuf->num_rows );

        if( icebuf->num_rows > total_pad ) {

            /* this icebuf contains our entire padding */
            icebuf->num_rows -= total_pad;
            total_pad = 0;
            
            /* we're done at this point so put it back onto the list for later
             * flushing
             */
            ATInsertTailList( &ch->running, &icebuf->listnode );
            /* important to keep our counters accurate! */
            ch->num_running++;
        }
        else {
            /* this icebuf contained a fraction of our padding; get rid of all
             * of the icebuf's data and we'll pull more from the previous.
             */
            total_pad -= icebuf->num_rows;

            pie_dbg1( "releasing completely truncated icebuf %p num_rows=%ld\n",
                        icebuf, icebuf->num_rows );
            icebuf->num_rows = 0;

            /* need to put it back somewhere or we'll have a memory leak */
            piems_wdma_add_ready( icebuf->channel, &icebuf );
            /* icebuf will be NULL here */
        }
    }

#else
    /* search the running list backwards */
    curr = ch->running.prevEntry;

    while( total_pad>0 && curr != &ch->running ) {
        
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );

        /* XXX - get rid of the icebuf_sanity() check eventually */
        icebuf_sanity( icebuf );

        pie_dbg1( "taking %d rows of padding from %p num_rows=%ld\n", 
                    total_pad, icebuf, icebuf->num_rows );

        if( icebuf->num_rows >= total_pad ) {
            icebuf->num_rows -= total_pad;
            /* this icebuf contains our entire padding; we're done here */
            total_pad = 0;
        }
        else {
            /* this icebuf contained a fraction of our padding; get rid of all
             * of the icebuf's data and we'll pull more from the previous.
             */
            total_pad -= icebuf->num_rows;
            icebuf->num_rows = 0;
        }
        
        curr = curr->prevEntry;
    }
#endif

    print_running( mm->name, ch );

    /* make sure we got rid of exactly the right amount */
    XASSERT( total_pad==0, total_pad );
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static bool
load_write( struct ice_dma_mm *mm, 
            struct ice_dma_channel *ch, 
            struct channel_multistrip *ms )
{
    struct ice_dma_buffer *icebuf;

    /* Write is quite a bit simpler than read but we still have a few
     * challenges. 
     *
     * - We can fit multiple DMA transactions in a single buffer however we can't
     * require the buffer to be exactly a factor of the stripsize because of
     * PIE write DMA's changing strip size because of XYScale.  Therefore,
     * there could be slack space at the bottom of the buffer (ie,
     * icebuf->num_rows <= icebuf->max_rows). 
     *
     * - We want to be able to throw away data coming after the end of the
     * image. To satisfy read DMA requirements, the scanner will have to
     * capture extra rows. We need to junk those extra rows.
     * 
     * XXX - I want to make sure this function isn't completely hardwired to
     * MultiStrip PIE write DMA. I'd like to go back and fix the previous DMA
     * drivers to be able to handle buffers bigger than a single strip.
     */

    /* Quick algorithm: 
     * Look at the last buffer in our running list for open space. If we find
     * enough trailing space in a current buffer, take it. Otherwise, try to
     * grab another buffer from the readies list. If the readies list is empty,
     * leave immediately since we have nothing new to load into DMA. Otherwise,
     * get pointers to the strip within the buffer and load those into the DMA
     * hardware.
     *
     * Ta-da!
     */

    pie_dbg1( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* if we have nothing running, start from scratch */
    icebuf = NULL;

    if( ch->num_running > 0 ) {
        /* in write, we have to search the running list from the end backwards */
        icebuf = CONTAINING_RECORD( ch->running.prevEntry, struct ice_dma_buffer, listnode );
        
        /* XXX - get rid of the icebuf_sanity() check eventually */
        icebuf_sanity( icebuf );

        pie_dbg1( "%s find strip in %p [%d,%ld] avail=%ld\n", 
                    __FUNCTION__,
                    icebuf, 
                    icebuf->base_row,
                    icebuf->base_row+(icebuf->max_rows-1), 
                    icebuf->max_rows-icebuf->num_rows );

        if( ms->dma_strip[STRIP_WRITE_FIRST] >= icebuf->base_row && 
            ms->dma_strip[STRIP_WRITE_LAST]  <  icebuf->base_row+icebuf->max_rows ) {

            pie_dbg1( "found strip [%d,%d] in %p\n", ms->dma_strip[STRIP_WRITE_FIRST],
                        ms->dma_strip[STRIP_WRITE_LAST], icebuf );
        }
        else {
            /* nope, we don't have space. As we fall through, we'll grab a new
             * buffer from the readies list.
             */
            icebuf = NULL;
        }
    }

    if( icebuf == NULL ) {

        /* The running buffer(s) don't have any space for the current strip.
         * one from the readies list. Make sure we consume the pre-image data
         * (e.g., staggered arrays) up to the desired starting row.
         */
        icebuf = pop_ready( ch );
        
        /* TODO - discard trailing data */

        if( icebuf==NULL ) {
            /* No buffers; there isn't any data read to run through DMA. Leave
             * and we'll come back when there's data.
             */
            
            /* we didn't queue anything */
            return false;
        }
        /* At this point we have a new buffer containing the left edge of the strip
         * that we have also put onto the running queue.
         *
         * Set it to be "empty" so we can start filling it.
         */
        icebuf->num_rows = 0;

        /* Multistrip DMA needs to know where this buffer starts so we can
         * search the list later. 
         */
        icebuf->base_row = ms->next_row;

        push_running( ch, icebuf );
    }
    
    /* load that puppy into DMA */
    write_buffer( ms, ch, icebuf );

    /* Add in the number of rows in our predicted strip (this can change
     * depending on the XYScale settings) so we know where the next incoming
     * buffer will start. 
     *
     * Note we don't have to modify the strip count to compensate for
     * MultiFilter like we do with the read because the strip predictor
     * should always predict perfectly adjacent strips on write.
     */
    XASSERT( ms->next_row==ms->dma_strip[STRIP_WRITE_FIRST], ms->dma_strip[STRIP_WRITE_FIRST] );

    /* +1 to convert from indices to count */
    ms->next_row += (ms->dma_strip[STRIP_WRITE_LAST] - ms->dma_strip[STRIP_WRITE_FIRST]) + 1;

    /* There can be max two buffers running since write DMA destination memory
     * cannot be split across two pointers and we require a buffer to be >= the
     * maximum write strip size.
     */ 
    flush_running( mm, ch, FLUSH_THRESHOLD );

    return true;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static bool
mswdma_launch( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    scan_err_t scerr;
    struct channel_multistrip *ms;
    bool flag;

    pie_dbg1("%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* davep 30-May-2013 ; adding PIE multiplexing */
    scerr = stripdma_try_push_write( &curr_bank->stripdma, ch->channel );
    if( scerr == SCANERR_NO_ENTRY ) {
        /* we're stalling the pipeline so don't add anything new */
//        dbg1( "%s write stalled bank=%d\n", __FUNCTION__, curr_bank->banknum );
        return false;
    }
    XASSERT(scerr==SCANERR_NONE,scerr);

    ms = &curr_bank->write_channel_strip_state[ch->channel];

    XASSERT( ms->dma_strip_size>0 && ms->dma_strip_size<=PIE_WDMA_MAX_ROWS, ms->dma_strip_size );

    if( !ms->dma_strip_queued ) {
        simple_next_strip( mm, ch, ms, false );

        /* +1 on the strip_size to convert from index to length */
        pie_dbg1( "%s %d new strip [%d,%d] strip_size=%d\n", mm->name, ch->channel, 
                    ms->dma_strip[STRIP_WRITE_FIRST], ms->dma_strip[STRIP_WRITE_LAST],
                    (ms->dma_strip[STRIP_WRITE_LAST]-ms->dma_strip[STRIP_WRITE_FIRST])+1 );

        /* Sanity check; if the predictor screws up, we're hosed from here to
         * Sunday. (This assert is kind of ugly because I don't want to use any
         * temporary variables.)
         *
         * Note I'm checking the predicted strip size is <= the configured
         * strip size since the write size can bounce around depending on what
         * XYScale does. The predicted read strip size must be exactly equal to
         * the configured strip size.
         */
        XASSERT( ms->dma_strip[STRIP_WRITE_LAST] - 
                 ms->dma_strip[STRIP_WRITE_FIRST] + 1 <= ms->dma_strip_size, 
                 ms->dma_strip[STRIP_WRITE_LAST] - ms->dma_strip[STRIP_WRITE_FIRST] + 1);

        ms->dma_strip_queued = true;
    }

    pie_dbg1( "%s %d strip [%d,%d] next_row=%d\n", mm->name, ch->channel,
                ms->dma_strip[STRIP_WRITE_FIRST],
                ms->dma_strip[STRIP_WRITE_LAST],
                ms->next_row );

    /* is this our last strip? (ie, does the end of the strip exceed the end of
     * the image? I hate one-off checks like this but I can't figure out
     * any other way to do this without doing something stupid like a state
     * machine of function pointers. 
     */
    if( ms->is_final_strip ) {
//        pie_dbg1( "%s %d working on final strip\n", mm->name, ch->channel );

        if( !ms->last_sent ) {
            flag = load_write( mm, ch, ms );
            if( flag ) {
                ms->last_sent = true;
//                pie_dbg1( "%s %d final strip sent\n", mm->name, ch->channel );
            }
        }
        else {
            flag = false;

            /* We're at the last strip and there are no more strips running. Therefore,
             * we must be at the end of the image. Send everything in our buffer queue
             * up and out.
             */
            if( ch->running_rows==0 ) {
                /* 15-June-05 ; Make sure we've sent all data through we expect (this is a
                 * double (triple?) check on the strip predictor and my use of it.
                 */
                /* 26-June-05 ; this only works if we're not running the scaler
                 * since write has to watch the read to tell when it's done.
                 */
                //XASSERT( ms->next_row >= ch->expected_rows, ms->next_row );

                /* 26-June-05 ; if we need to truncate because of XYScale's
                 * weirdness, do it now.
                 */
//                if( ms->scaler_enabled ) {
                    truncate_write( mm, ch, ms );
//                }

                flush_running( mm, ch, 0 );
            }
        }
    }
    else { 

        flag = load_write( mm, ch, ms );

        /* We successfully loaded a strip. Mark the channel as having no pending
         * strip so we'll run the predictor next time.
         */
        if( flag ) {
            ms->dma_strip_queued = false;
        }
    }

    /* davep 30-May-2013 ; adding pie multiplexing */
    if( flag ) {
        /* we successfully pushed a write into DMA so update our strip info */
        scerr = stripdma_push_write( &curr_bank->stripdma, ch->channel );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    /* true : we successfully queued up a transaction into the DMA hardware 
     * false : for whatever reason, we didn't queue anything 
     */
    return flag;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void 
launch_loop( struct ice_dma_mm *mm, 
             struct ice_dma_channel *ch,
             bool (*launch_f)(struct ice_dma_mm *, struct ice_dma_channel *) )
{
    int cnt;

    /*  NOTE! 
     * 
     *  NOT interrupt/thread safe! Assumes appropriate interrupts are
     *  disabled by the caller!
     *
     *  Also called from DMA ISR.
     */

    ASSERT( !IN_INTERRUPT() );
    /* 13-June-05 ; XXX this is a stupid ugly function that only exists because
     * I'm debugging some DMA problems and I got tired of copy-and-paste
     * errors. Deep function calls in Xtensa are very very expensive so want to
     * unroll this function back into piems_[rw]dma_channel_launch() as soon as
     * the MS_DMA driver is debugged.
     */

    /* while the driver has space and the driver says it successfully pushed
     * something into the DMA hardware, continue loading
     */
    cnt = 0;
    while( ch->running_rows < mm->drv->max_queue_depth && launch_f( mm, ch ) ) {
        cnt++;
        /* watch for anything dramatically stupid */
        XASSERT( cnt <= mm->drv->max_queue_depth, cnt );
    }

#ifdef PIEMS_DMA_DEBUG
    print_running( mm->name, ch );
#endif
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
init_scaler( struct ice_dma_mm *mm, 
             struct channel_multistrip strip_state[],
             int scaler_strip_size,
             int y_numerator,
             int y_denominator,
             int fracY )
{
    int i;
    struct channel_multistrip *ms;
    struct ice_dma_channel *ch;

    for( i=0 ; i<mm->num_channels ; i++ ) {
        
        ch = &mm->channels[i];

        /* 26-June-05 davep ; don't disturb unopened channels */
        if( !ch->is_open ) {
            continue;
        }
        
        ms = &strip_state[ch->channel];

        /* initialize the strip predictor but change the initial setting to say
         * we don't have a first strip
         */
        ms->dma_strip_queued = false;

        if( scanvar_get()->xy_scaler_mode==PIE_XY_SCALER_LINEAR ) {
            strip_start( &ms->state, scaler_strip_size, y_numerator, 
                    y_denominator, fracY, STRIPPR_MODE_LINEAR );
        }
        else {
            strip_start( &ms->state, scaler_strip_size, y_numerator, 
                    y_denominator, fracY, STRIPPR_MODE_RAPR );
        }

        ms->scaler_enabled = true;

    }
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static int 
piems_dma_open( struct ice_dma_mm *mm,
                struct channel_multistrip strip_state[],
                uint8_t channels[], 
                uint8_t num_channels,
                int num_empties, 
                int total_rows,
                int bytes_per_row,
                int strip_size,
                uint8_t icebuf_tag )
{
    int i;
    int retcode;
    struct ice_dma_channel *ch;
    struct channel_multistrip *ms;

    pie_dbg1( "%s %s num_channels=%d strip_size=%d channels=[%d,%d,%d]\n", __FUNCTION__, mm->name, 
                num_channels, strip_size, channels[0], channels[1], channels[2] );

    /* davep 30-Aug-2005 ; make sure DMA alignment requirements are met */
    XASSERT( ICE_DMA_IS_ALIGNED(bytes_per_row), bytes_per_row );

    retcode = ice_dma_open( mm, channels,
                            num_channels, num_empties,
                            total_rows, bytes_per_row, icebuf_tag );

    /* ice_dma_open() returns the number of empties successfully allocated;
     * we'll return that after doing our own setup
     */

    /* now initialize everything else to our liking */
    for( i=0 ; i<num_channels ; i++ ) {
        /* no need to sanity check the channel numbers since ice_dma_open() has
         * done it already
         */
        ch = &mm->channels[ channels[i] ];

        /* initialize the channels' strip state tracker; almost everything
         * defaults ok to zero or false (disabled) 
         */
        ms = &strip_state[ch->channel];
        
        /* watch for memory leaks */
        XASSERT( ms->pad_icebuf==NULL, (uint32_t)ms->pad_icebuf );
        XASSERT( ms->top_pad==NULL, (uint32_t)ms->top_pad );
        
        memset( ms, 0, sizeof(struct channel_multistrip) );
        ms->channel = ch->channel;

        /* we need to know the strip_size just about everywhere so we can
         * advance the row pointers correctly.
         */
        ms->dma_strip_size = strip_size;

        /* The strip size going into (read DMA) or coming out of (write DMA)
         * the XYScaler is the same as the read/write DMA strip size until
         * someone enables MultiFilter. 
         *
         * In write DMA the XY strip size will be <= the DMA strip size. (Note,
         * however, the write strip size can move around based on XYScale
         * settings.)
         */
        ms->xy_strip_size = ms->dma_strip_size;

//        /* davep 24-May-2011 ; ColorShift eats two lines before the strip hits
//         * the XYScale 
//         */
//        if( ms->colorshift_enabled ) {
//            ms->xy_strip_size -= 2;
//        }

        /* 24-June-05 ; set to starting state */
        ms->is_first_strip = true;

        print_channel_strip_state( ms );
    }

    return retcode;
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : 24-June-05
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
piems_dma_close( struct ice_dma_mm *mm,
                 struct channel_multistrip strip_state[] )
{
    int i;
    struct ice_dma_channel *ch;
    struct channel_multistrip *ms;

#ifdef PIEMS_DMA_DEBUG
    pie_dbg1( "%s %s\n", __FUNCTION__, mm->name );
#endif

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[ i ];

        /* ignore unopened channels */
        if( !ch->is_open ) {
            continue;
        }

        /* free up any multistrip memory we might have allocated */
        ms = &strip_state[ch->channel];

        if( ms->top_pad ) {
#ifdef PIEMS_DMA_BUFFER_POISON
            /* before freeing the pointer, poison the memory so we can catch anyone
             * using a freed pointer 
             */
            memset( ms->top_pad, 0xcc, ms->top_pad_buflen );
#endif
            /* clear out the cache bypass bit so the heap will accept the
             * pointer back
             */
            ms->top_pad = CACHE_BYPASS_CLEAR(ms->top_pad);

            PTR_FREE( ms->top_pad );
        }
        if( ms->pad_icebuf ) {
            icebuf_sanity( ms->pad_icebuf );
            icebuf_free( &ms->pad_icebuf );
        }
    }
}

/**
 * \brief cancel the PIE MultiStrip driver 
 *
 *
 * \author David Poole
 * \date 10-Nov-2005
 *
 */

static void
piems_dma_cancel( struct ice_dma_mm *mm,
                 struct channel_multistrip strip_state[] )
{
    int i;
    struct ice_dma_channel *ch;

#ifdef PIEMS_DMA_DEBUG
    pie_dbg1( "%s %s\n", __FUNCTION__, mm->name );
#endif

    /* XXX do we need to clean up our running queue before we call the icedma's
     * cancel method? PIEMSDMA uses the running queue differently than ICEDMA
     */

    /* I'm using running_rows to mean something different in the PIEMSDMA
     * driver than in the ICEDMA driver. In the MultiStrip driver, running_rows
     * is actually "running_strips". The side effect of the way I'm using
     * running_rows is:
     *
     *  expected_rows <= pending + running + completed
     *
     * rather:
     *  expected_rows == pending + completed
     *
     * Need to mark the running_rows ("running_strips") to zero so our checks
     * later in ice_dma_close() won't fail.
     */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];

        /* ignore unopened channels */
        if( !ch->is_open ) {
            continue;
        }

        ch->running_rows = 0;

        /* davep 14-Mar-2007 ; screw this, I'm sick to @#%@#ing death of PIE.
         * Shotgun the damn thing so I can leave the channel_sanity() checks in
         * place.
         */
        ch->completed_rows = ch->expected_rows;
        ch->pending_rows = 0;
    }
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void 
piems_dma_intmsg( struct ice_dma_mm *mm, 
                  uint8_t channel,
                  bool (*launch_f)(struct ice_dma_mm *, struct ice_dma_channel *) ) 
{
    struct ice_dma_channel *ch;

    /* we want to do a lot of checks at this level because we have to decrement
     * running_rows before calling launch_loop()
     */
    XASSERT( channel < mm->num_channels, channel );
    ch = &mm->channels[channel];

#ifdef PIEMS_DMA_DEBUG
    dbg1( "%s %s ch=%d sensor=%d running_rows=%d\n", __FUNCTION__, 
            mm->name, channel, ch->sensor_num, ch->running_rows );
#endif

    /* if we get here, we should have had something running */
    XASSERT( ch->running_rows > 0, 0 ); 

    /* and we shouldn't have more running that the DMA controller could see */
    // davep commented out 7/1/05 incorrect test
    //XASSERT( ch->running_rows <= mm->max_queue_depth, ch->running_rows ); 

    /* 13-June-05; In order to finish an image correctly, I need to keep track
     * of how many transactions are currently loaded into the DMA controller. I
     * know exactly when I push on into the controller and this function should
     * be called exactly when a transaction finishes (DMA complete interrupt).
     * I didn't want to risk crossing the thread/ISR boundry by modifying data
     * structures in both a thread and an ISR so I'm not doing the
     * 'running_rows--' in the isr.
     */
    ch->running_rows--;

    launch_loop( mm, ch, launch_f ); 
}

//--------------------------------------------------------------------------
// Function     : 
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   :
// Date created :
// Description  : 
//
// 
//--------------------------------------------------------------------------
    
static void
piems_dma_set_image_offsets( struct ice_dma_mm *mm, 
                             struct channel_multistrip strip_state[],
                             int image_offsets[] )
{
    int i;
    struct ice_dma_channel *ch;
    struct channel_multistrip *ms;
    
#ifdef PIEMS_DMA_DEBUG
    pie_dbg1( "%s %s\n", __FUNCTION__, mm->name );

    pie_dbg1( "%s setting offsets to %d,%d,%d,%d,%d,%d\n",
                __FUNCTION__,
                image_offsets[0], image_offsets[1], image_offsets[2],
                image_offsets[3], image_offsets[4], image_offsets[5] );
#endif

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];

        /* don't touch unopened channels */
        if( !ch->is_open ) {
            continue;
        }

        ms = &strip_state[ch->channel];

        ms->image_offset = image_offsets[ ch->channel ];
        
        /* Start our next_row at the image_offset. We'll add rows to next_row
         * as they arrive in ice_dma_buffers. 
         */
        ms->next_row = ms->image_offset;

        /* Here's what happens in msrdma_channel_load():
         *
         *  icebuf->base_row = ms->next_row;
         *  ms->next_row += icebuf->num_rows;
         *
         *  if( strip_first_row >= icebuf->base_row ) {
         *      This icebuf contains the start of our image.  We may have to
         *      throw away some rows at the start of the buffer but at least
         *      we've not hit the start of our image.
         *  }
         *  else {
         *      This icebuf contains none of our image. Discard it entirely.
         *  }
         *
         * This trick works because the strip predictor's first prediction for
         * the first row should be zero. If MultiFilter is enabled, we need
         * two extra leading rows so the first prediction for the first row
         * will be -2.
         */
    }
}

static void piems_dma_init_colorshift( struct ice_dma_mm *mm, 
                                       struct channel_multistrip strip_state[] )
{
    int i;
    struct ice_dma_channel *ch;
    struct channel_multistrip *ms;

#if !defined HAVE_PIE_COLORSHIFT
    ASSERT(0);
#endif

    /* turn on ColorShift */
    for( i=0 ; i<mm->num_channels ; i++ ) {

        ch = &mm->channels[i];

        /* don't change unused channels */
        if( !ch->is_open ) {
            continue;
        }

        ms = &strip_state[ ch->channel ];
        XASSERT( ms->channel==ch->channel, ms->channel );

        /* If already enabled, don't enable again because that will change our
         * data sizes. (Stupid human check.)
         */
        if( ms->colorshift_enabled ) {
            continue;
        }

        ms->colorshift_enabled = true;

        /* colorshift eats two lines before the strip hits XYScale */
        ms->xy_strip_size -= 2;
    }
}

static void piems_dma_run_forever( struct ice_dma_mm *mm )
{
    int i;
    struct ice_dma_channel *ch;

    dbg2("%s %s\n", __FUNCTION__, mm->name );

    for( i=0 ; i<mm->num_channels ; i++ ) {

        ch = &mm->channels[i];

        /* don't change unused channels */
        if( !ch->is_open ) {
            continue;
        }

        /* disable expected rows so we'll run forever */
        ch->expected_rows = 0;
    }
}

static bool piems_dma_is_empty( struct ice_dma_mm *mm )
{
    int i;
    struct ice_dma_channel *ch;

    pie_dbg1("%s %s\n", __FUNCTION__, mm->name );

    for( i=0 ; i<mm->num_channels ; i++ ) {

        ch = &mm->channels[i];

        /* don't change unused channels */
        if( !ch->is_open ) {
            continue;
        }

        if( ch->num_readies || ch->num_running ) {
            return false;
        }
    }

    return true;
}


/*
 * PIE Multi-Strip Read DMA
 * interface functions to ICE DMA
 */

void piems_rdma_channel_load( uint8_t channel, uint32_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row )
{
    /* Shouldn't be getting to the icedma code that would call load. We need to
     * do the load ourselves since we need to use DAR1 and DAR2 while the
     * older, original DMA code only used DAR1.
     */
    XASSERT( 0, channel );
}


void 
piems_rdma_init( void ) 
{
    int i;
    char icedma_name[ICE_NAME_MAX+1];
    char stripdma_name[MAX_STRIPDMA_NAME+1];

    /* davep 24-May-2013 ; initialize the global banks list (only doing this in
     * the rdma init function)
     */
    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        bank_list[i].banknum = i;
    }

    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        piems_dma_set_bank(i);

        /* build names piemsrdma[01] the cheap and easy way */
        memset( icedma_name, 0, sizeof(icedma_name) );
        strcpy( icedma_name, "piemsrdma " );
        icedma_name[9] = 0x30 + curr_bank->banknum;

        ice_dma_init( &curr_bank->piems_rdma_mm, icedma_name, 
                pie_rdma_get_driver(),
                curr_bank->piems_rdma_channels, PIE_RDMA_NUM_CHANNELS);

        /* build names piemsdma0 and piemsdma1 the cheap and easy way */
        memset( stripdma_name, 0, sizeof(stripdma_name) );
        strcpy( stripdma_name, "piemsdma " );
        stripdma_name[8] = 0x30 + curr_bank->banknum;

        /* We initialize the stripdma during the read, but not the write. The
         * stripdma crosses over the read/write boundary and only needs to be
         * initialized once.
         */
        stripdma_list_init( &curr_bank->stripdma, stripdma_name, 
                            ICE_DMA_REG_DEPTH+1, curr_bank->strip_list );
    }

    piems_dma_set_bank(0);
}

int 
piems_rdma_open( uint8_t channels[], 
                 uint8_t num_channels,
                 int num_empties, 
                 int total_rows,
                 int bytes_per_row,
                 int strip_size )
{
    int i;
    int retcode;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    struct ice_dma_buffer *icebuf;
    struct channel_multistrip *ms;
    scan_err_t scerr;
    int banknum, old_banknum;

    XASSERT( strip_size > 0 && strip_size <= PIE_RDMA_MAX_ROWS, strip_size );

    /* this code is touching hardware so only run once */
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

    /* davep 16-May-2011 ; call me!  */
    pie_rdma_set_icebuf_isr( piems_rdma_isr );

    retcode = 0;
    banknum = first_bank();
    old_banknum = curr_bank->banknum;
    do {
        piems_dma_set_bank(banknum);
        mm = &curr_bank->piems_rdma_mm;

        /* piems_dma_open() returns the number of empties successfully allocated;
         * we'll return that after doing our own setup
         */
        retcode = piems_dma_open( mm, 
                                  curr_bank->read_channel_strip_state,
                                  channels, 
                                  num_channels, 
                                  num_empties,
                                  total_rows, 
                                  bytes_per_row, 
                                  strip_size,
                                  ICEBUF_TAG_PIEMSRDMA );
        if( retcode <= 0 ) {
            piems_dma_set_bank(old_banknum);
            return retcode;
        }

        /* create our pad buffer(s) (only read needs this for now) */
        for( i=0 ; i<num_channels ; i++ ) {
            icebuf = icebuf_new( strip_size, bytes_per_row, ICEBUF_TAG_PIEMSPAD );
            if( icebuf==NULL ) {
                pie_dbg1( "%s out of memory\n", curr_bank->piems_rdma_mm.name );

                /* unwind, freeing everything we've allocated so far */
                for( i-- ; i>=0 ; i-- ) {
                    icebuf_free( &curr_bank->read_channel_strip_state[i].pad_icebuf );
                }

                piems_rdma_close();

                return -1;
            }

            ms = &curr_bank->read_channel_strip_state[ channels[i] ];
            PTR_ASSIGN( ms->pad_icebuf, icebuf );
            ms->pad_icebuf->channel = channels[i];

            /* davep 28-May-2013 ; sneak in assigning the sensor number (assume
             * sensor==bank number)
             */
            ch = &mm->channels[ channels[i] ];
            ch->sensor_num = banknum;
        }
        stripdma_open_read( &curr_bank->stripdma, channels, num_channels );

        /* debug */
        for( i=0 ; i<PIE_RDMA_NUM_CHANNELS ; i++ ) {
            print_channel_strip_state( &curr_bank->read_channel_strip_state[i] );
        }

        banknum = next_open_bank(banknum);
    } while(banknum);
    piems_dma_set_bank(old_banknum);

    return retcode;
}

scan_err_t piems_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    struct ice_dma_mm *mm;

    mm = &curr_bank->piems_rdma_mm;

    /* PIE read DMA has a fixed strip size but the strip as fed into read DMA
     * can move back and forth along the image (MultiFilter and/or XYScale). We
     * must constrain the number of rows in a buffer to be >= the strip size so
     * we can make sure we don't try to split a strip across more than two
     * buffers (PIE read DMA only has two source address pointers).
     */
    XASSERT( channel < mm->num_channels, channel );
    XASSERT( rows >= curr_bank->read_channel_strip_state[channel].dma_strip_size, rows );
//    XASSERT( rows >= piems_rdma_mm.channels[channel].dma_strip_size, rows );
    
    return ice_dma_add_buffer( mm, channel, data, datalen, rows, bytes_per_row );
}

void 
piems_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
//    dbg2( "%s %s sensor=%d channel=%d\n", __FUNCTION__,
//            curr_bank->piems_rdma_mm.name, (*addme)->sensor_num, channel );

    XASSERT( (*addme)->sensor_num==curr_bank->banknum, (*addme)->sensor_num);

    ice_dma_add_ready( &curr_bank->piems_rdma_mm, channel, addme );
}

void 
piems_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &curr_bank->piems_rdma_mm, channel, freeme );
}

void
piems_rdma_close( void )
{
    int i;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    int banknum, old_banknum;

    /* this code touches hardware so do only once */
    mm = &bank_list[0].piems_rdma_mm;
    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];
        pie_rdma_channel_close( ch->channel );
    }

    /* davep 16-May-2011 ; don't call me anymore */
    pie_rdma_set_icebuf_isr( NULL );

    banknum = first_bank();
    old_banknum = curr_bank->banknum;
    do {
        piems_dma_set_bank(banknum);

        mm = &curr_bank->piems_rdma_mm;

        /* clean up channels' strip state structure */
        piems_dma_close( mm, curr_bank->read_channel_strip_state );
        ice_dma_close( mm );
        
        /* davep 30-May-2013 ; close the stripdma tracking data structure only on
         * the read side (crosses the read/write boundary)
         */
        stripdma_close( &curr_bank->stripdma );

        banknum = next_open_bank(banknum);
    } while(banknum);
    piems_dma_set_bank(old_banknum);
}

void
piems_rdma_cancel( void )
{
    int i;

    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        /* clean up channels' strip state structure */
        piems_dma_cancel( &bank_list[i].piems_rdma_mm, 
                         bank_list[i].read_channel_strip_state );

        ice_dma_cancel( &bank_list[i].piems_rdma_mm );
    }

#if 0
    FOR_ALL_BANKS
        /* clean up channels' strip state structure */
        piems_dma_cancel( &curr_bank->piems_rdma_mm, 
                         curr_bank->read_channel_strip_state );

        ice_dma_cancel( &curr_bank->piems_rdma_mm );
   END_FOR_ALL_BANKS
#endif
}

void
piems_rdma_sanity( void )
{
    FOR_ALL_BANKS
        ice_dma_sanity( &curr_bank->piems_rdma_mm );
    END_FOR_ALL_BANKS
}

void 
piems_rdma_debug_log( void )
{
    int i;

    /* can only change banks if current bank is idle but want to see all banks
     * so peek at each directly
     */
    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        ice_dma_debug_log( &bank_list[i].piems_rdma_mm );
        stripdma_list_dump( &bank_list[i].stripdma );

//        ice_dma_dump_list( &bank_list[i].piems_rdma_mm, ICE_DMA_LIST_READIES );
    }
}

struct ice_dma_mm *
piems_rdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &curr_bank->piems_rdma_mm;
}

void
piems_rdma_init_scaler( int scaler_strip_size, 
                        int y_numerator, 
                        int y_denominator,
                        int fracY )
{
    FOR_ALL_BANKS
        XASSERT( scaler_strip_size > 0 && scaler_strip_size <= PIE_RDMA_MAX_ROWS, scaler_strip_size );

        /* does not touch hardware so safe to call for each bank */
        init_scaler( &curr_bank->piems_rdma_mm, curr_bank->read_channel_strip_state, 
                      scaler_strip_size, y_numerator, y_denominator, fracY );
    END_FOR_ALL_BANKS
}
extern int dbg_cnt_piems_rdma_isr;	//add.lsh.for pipe debug
void 
piems_rdma_isr( uint8_t channel )
{
	dbg_cnt_piems_rdma_isr++;
    /* send a message to the target thread telling it we have a write DMA
     * needing attention.
     */
    piems_dma_isr( &curr_bank->piems_rdma_mm, channel, SMSG_PIE_RDMA_INT );
}

void 
piems_rdma_channel_launch( uint8_t channel )
{
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

    XASSERT( channel < curr_bank->piems_rdma_mm.num_channels, channel );

    mm = &curr_bank->piems_rdma_mm;
    ch = &mm->channels[channel];

    XASSERT( ch->is_open, ch->channel );
    XASSERT( ch->channel==channel, channel );

    launch_loop( mm, ch, msrdma_launch ); 
}

void piems_rdma_launch( void )
{
    int i;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

    mm = &curr_bank->piems_rdma_mm;

    for( i=0 ; i<mm->num_channels ; i++ ) {    
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }

        piems_rdma_channel_launch(ch->channel);
    }
}

void 
piems_rdma_intmsg( uint8_t channel )
{
    piems_dma_intmsg( &curr_bank->piems_rdma_mm, channel, msrdma_launch );
}

void
piems_rdma_set_image_offsets( int image_offsets[] )
{
    FOR_ALL_BANKS
        /* does not touch hardware so safe to call for each bank */
        /* assuming caller sent us an array of exactly PIE_RDMA_NUM_CHANNELS */
        piems_dma_set_image_offsets( &curr_bank->piems_rdma_mm, 
                                     curr_bank->read_channel_strip_state, 
                                     image_offsets );
    END_FOR_ALL_BANKS
}

void piems_rdma_init_colorshift( void )
{
    FOR_ALL_BANKS
        /* does not touch hardware so safe to call for each bank */
        piems_dma_init_colorshift( &curr_bank->piems_rdma_mm, 
                                   curr_bank->read_channel_strip_state );
    END_FOR_ALL_BANKS
}

static void piems_rdma_mm_init_mf( struct ice_dma_mm *mm,
                                   struct channel_multistrip strip_state[] )
{
    int i;
    struct ice_dma_channel *ch;
    struct channel_multistrip *ms;
    int cnt;

    cnt = 0;

    for( i=0 ; i<mm->num_channels ; i++ ) {

        ch = &mm->channels[i];

        /* don't change unused channels */
        if( !ch->is_open ) {
            continue;
        }

        /* davep 15-Oct-05 ; I've been bitten one too many times by calling
         * this function before calling piems_rmda_open(), I'm finally putting
         * a check in here to catch that particularly boneheaded maneuver.
         */
        cnt++;

        ms = &strip_state[ ch->channel ];

        /* If already enabled, don't enable again because that will change our
         * data sizes. (Stupid human check.)
         */
        if( ms->multifilter_enabled ) {
            continue;
        }
        
        ms->multifilter_enabled = true; 

        /* Adjust the read DMA XY strip size accordingly. */
        ms->xy_strip_size = ms->dma_strip_size - 4;
    }

    /* If this assert fails, you probably called piems_rdma_init_mf() before
     * calling piems_rdma_open().
     */
    ASSERT(cnt>0);
}

void 
piems_rdma_init_mf( void )
{
    FOR_ALL_BANKS
        /* does not touch hardware so safe to call for each bank */
        piems_rdma_mm_init_mf( &curr_bank->piems_rdma_mm, 
                               curr_bank->read_channel_strip_state );
    END_FOR_ALL_BANKS
}

void piems_rdma_run_forever( void )
{
    FOR_ALL_BANKS
        piems_dma_run_forever( &curr_bank->piems_rdma_mm );
    END_FOR_ALL_BANKS
}

bool piems_rdma_is_idle( void )
{
    struct ice_dma_mm *mm;
    bool is_idle;

    print_running( curr_bank->piems_rdma_mm.name, 
            &curr_bank->piems_rdma_mm.channels[0] );

    if( curr_bank ) {
        mm = &curr_bank->piems_rdma_mm;
        is_idle = piems_dma_is_idle( mm );
        if( !is_idle ) {
            return false;
        }
    }

    /* final decision is the hardware */
    return pie_rdma_is_idle();
}

bool piems_rdma_is_empty( void )
{
    return piems_dma_is_empty( &curr_bank->piems_rdma_mm );
}

/*
 * PIE Multi-Strip Write DMA
 * interface functions to ICE DMA
 */

void piems_wdma_channel_load( uint8_t channel, uint32_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row )
{
    /* Shouldn't be getting to the icedma code that would call load. We need to
     * do the load ourselves since we need to use DAR1 and DAR2 while the
     * older, original DMA code only used DAR1.
     */
    XASSERT( 0, channel );
}

void 
piems_wdma_init( void ) 
{
    int i;
    char icedma_name[ICE_NAME_MAX+1];

    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        piems_dma_set_bank(i);

        /* build names piemswdma[01] the cheap and easy way */
        memset( icedma_name, 0, sizeof(icedma_name) );
        strcpy( icedma_name, "piemswdma " );
        icedma_name[9] = 0x30 + curr_bank->banknum;

        ice_dma_init( &curr_bank->piems_wdma_mm, icedma_name, 
                pie_wdma_get_driver(),
                curr_bank->piems_wdma_channels, PIE_WDMA_NUM_CHANNELS);
    }

    piems_dma_set_bank(0);
}

/**
 * \brief  Assign a scan_data_type to all channels' ice_dma_channel.
 *
 * The ice_dma_channel's dtype is assigned to incoming ice_dma_buffers.
 *
 * \author David Poole
 * \date 09-Apr-2013
 */

static void piems_wdma_assign_channels_dtype( struct ice_dma_mm *mm,
                                              uint8_t channels[],
                                              uint8_t num_channels )
{
    int i;
    struct ice_dma_channel *ch;
    const struct scanvars *sv;

    sv = scanvar_peek();

    /* RGB, xRGB, RGBx, Mono are all single channel, all channel zero so we'll
     * have to peek at the scanvar to decide what datatype pops out
     */
    if( num_channels==1 ) {
        XASSERT(channels[0]==0, channels[0] );
        ch = &mm->channels[channels[0]];
        XASSERT(ch->channel==0, ch->channel );
        XASSERT( ch->is_open, ch->channel );

        switch( sv->pieout ) {
            case PIE_PIXEL_MONO:
                ch->dtype = SCAN_DATA_TYPE_MONO;
                break;
            case PIE_PIXEL_XRGB:
                ch->dtype = SCAN_DATA_TYPE_XRGB;
                break;
            case PIE_PIXEL_RGBX:
                ch->dtype = SCAN_DATA_TYPE_RGBX;
                break;
            case PIE_PIXEL_RGB:
                ch->dtype = SCAN_DATA_TYPE_RGB;
                break;
            default :
                XASSERT( 0, sv->pieout );
                ch->dtype = SCAN_DATA_NULL_TYPE;
                break;
        }
    }
    else {
        /* davep 09-Apr-2013 ; as of this writing, PIE can only write three
         * channels max
         */
        XASSERT( num_channels==3, num_channels );

        for( i=0 ; i<num_channels ; i++ ) {
            ch = &mm->channels[channels[i]];
            XASSERT( ch->is_open, ch->channel );

            switch( ch->channel ) {
                case PIE_WDMA_CHANNEL_RED:
                    ch->dtype = SCAN_DATA_TYPE_RED;
                    break;

                case PIE_WDMA_CHANNEL_GREEN:
                    ch->dtype = SCAN_DATA_TYPE_GREEN;
                    break;

                case PIE_WDMA_CHANNEL_BLUE:
                    ch->dtype = SCAN_DATA_TYPE_BLUE;
                    break;

                default :
                    XASSERT(0,ch->channel);
                    break;
            }
        }
    }
}

int 
piems_wdma_open( uint8_t channels[], 
                 uint8_t num_channels,
                 int num_empties, 
                 int total_rows,
                 int bytes_per_row,
                 int strip_size )
{
    int retcode; 
    int i;
    scan_err_t scerr;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    int banknum, old_banknum;

    XASSERT( strip_size > 0 && strip_size <= PIE_WDMA_MAX_ROWS, strip_size );

    /* the pie_wdma_xxx functions touch hardware so only do that once */
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

    /* davep 12-May-2011 ;  call me! */
    pie_wdma_set_icebuf_isr( piems_wdma_isr );

    /* 26-June-05 davep ; note the caller should be passing in the read side's
     * total rows, not the write side. Because of XYScale, the write won't know
     * how many rows to expect until the predictor is run clear through.
     */
    
    retcode = 0;
    banknum = first_bank();
    old_banknum = curr_bank->banknum;
    do {
        piems_dma_set_bank(banknum);
        mm = &curr_bank->piems_wdma_mm;
        retcode = piems_dma_open( mm, 
                                  curr_bank->write_channel_strip_state,
                                  channels, 
                                  num_channels, 
                                  num_empties,
                                  total_rows, 
                                  bytes_per_row, 
                                  strip_size,
                                  ICEBUF_TAG_PIEMSWDMA );
        if( retcode <= 0 ) {
            piems_dma_set_bank(old_banknum);
            return retcode;
        }

        /* davep 09-Apr-2013 ; assign scan_data_type to channels */
        piems_wdma_assign_channels_dtype( mm, channels, num_channels );

        for( i=0 ; i<num_channels ; i++ ) {
            /* davep 28-May-2013 ; sneak in assigning the sensor number (assume
             * sensor==bank number)
             */
            ch = &mm->channels[ channels[i] ];
            ch->sensor_num = banknum;
        }

        stripdma_open_write( &curr_bank->stripdma, channels, num_channels );

        banknum = next_open_bank(banknum);
    } while(banknum);
    piems_dma_set_bank(banknum);

    return retcode;
}

scan_err_t piems_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row )
{
    struct ice_dma_mm *mm;

    mm = &curr_bank->piems_wdma_mm;

    XASSERT( channel < mm->num_channels, channel );
    XASSERT( rows >= curr_bank->write_channel_strip_state[channel].dma_strip_size, rows );
//    XASSERT( rows >= piems_wdma_mm.channels[channel].dma_strip_size, rows );

    return ice_dma_add_buffer( mm, channel, data, datalen, rows, bytes_per_row );
}

void 
piems_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme )
{
    uint8_t banknum;

    banknum = (*addme)->sensor_num;

//    dbg2( "%s %s sensor=%d channel=%d\n", __FUNCTION__,
//            curr_bank->piems_wdma_mm.name, banknum, channel );

//    XASSERT( (*addme)->sensor_num==curr_bank->banknum, (*addme)->sensor_num);

    XASSERT( banknum < PIEMS_NUM_BANKS, banknum );

    ice_dma_add_ready( &bank_list[banknum].piems_wdma_mm, channel, addme );
//    ice_dma_add_ready( &curr_bank->piems_wdma_mm, channel, addme );
}

void 
piems_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme )
{
    ice_dma_free_empty( &curr_bank->piems_wdma_mm, channel, freeme );
}

void
piems_wdma_close( void )
{
    int i;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;
    int banknum, old_banknum;

    /* this code touches hardware so only do it once */
    mm = &bank_list[0].piems_wdma_mm;
    for( i=0 ; i< mm->num_channels; i++ ) {
        ch = &curr_bank->piems_wdma_mm.channels[i];
        pie_wdma_channel_close( ch->channel );
    }

    /* davep 16-May-2011 ; don't call me anymore */
    pie_wdma_set_icebuf_isr( NULL );

    /* close all the data structures */
    banknum = first_bank();
    old_banknum = curr_bank->banknum;
    do {
        piems_dma_set_bank(banknum);

        mm = &curr_bank->piems_wdma_mm;

        /* clean up channels' strip state structure */
        piems_dma_close( &curr_bank->piems_wdma_mm, 
                         curr_bank->write_channel_strip_state );

        ice_dma_close( &curr_bank->piems_wdma_mm );

        banknum = next_open_bank(banknum);
    } while(banknum);
    piems_dma_set_bank(old_banknum);
}

void
piems_wdma_cancel( void )
{
    int i;

    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        /* clean up channels' strip state structure */
        piems_dma_cancel( &bank_list[i].piems_wdma_mm, 
                         bank_list[i].write_channel_strip_state );

        ice_dma_cancel( &bank_list[i].piems_wdma_mm );
    }

#if 0
    FOR_ALL_BANKS 
        /* clean up channels' strip state structure */
        piems_dma_cancel( &curr_bank->piems_wdma_mm, curr_bank->write_channel_strip_state );

        ice_dma_cancel( &curr_bank->piems_wdma_mm );
    END_FOR_ALL_BANKS
#endif
}

void
piems_wdma_sanity( void )
{
    FOR_ALL_BANKS
        ice_dma_sanity( &curr_bank->piems_wdma_mm );
    END_FOR_ALL_BANKS
}

void 
piems_wdma_debug_log( void )
{
    int i;

    /* can only change banks if current bank is idle but want to see all banks
     * so peek at each directly
     */
    for( i=0 ; i<PIEMS_NUM_BANKS ; i++ ) {
        ice_dma_debug_log( &bank_list[i].piems_wdma_mm );

//        ice_dma_dump_list( &bank_list[i].piems_wdma_mm, ICE_DMA_LIST_READIES );
    }
}

struct ice_dma_mm *
piems_wdma_get_mm( void )
{
    /* this function should only be called by code testing the PIE read DMA! */
    return &curr_bank->piems_wdma_mm;
}

void
piems_wdma_init_scaler( int scaler_strip_size, 
                        int y_numerator, 
                        int y_denominator,
                        int fracY )
{
    /* davep 21-Sep-2005 ; I'm mostly sure we need to pass the read strip size
     * into the strip predictor so I shouldn't be checking the Write strip size
     * here
     */
//    XASSERT( scaler_strip_size > 0 && scaler_strip_size <= PIE_WDMA_MAX_ROWS, scaler_strip_size );

    FOR_ALL_BANKS
        XASSERT( scaler_strip_size > 0 && scaler_strip_size <= PIE_RDMA_MAX_ROWS, scaler_strip_size );

        /* Does not touch hardware. Only hits the data structures */
        init_scaler( &curr_bank->piems_wdma_mm, 
                     curr_bank->write_channel_strip_state, 
                     scaler_strip_size, 
                     y_numerator, 
                     y_denominator,
                     fracY );
    END_FOR_ALL_BANKS
}

extern int dbg_cnt_piems_wdma_isr;	//add.lsh.for pipe debug
void 
piems_wdma_isr( uint8_t channel )
{
    /* send a message to the target thread telling it we have a write DMA
     * needing attention.
     */
    dbg_cnt_piems_wdma_isr++;
    piems_dma_isr( &curr_bank->piems_wdma_mm, channel, SMSG_PIE_WDMA_INT );

    /* davep 14-Sep-2005 ; this strictly isn't always required but since
     * it's possible to enable the scaler with 1:1 scaling and DRC won't be
     * required since the strips size won't change. But let's be paranoid.
     *
     * Uncomment this assert if we're scaling, weird corruption problems are
     * happening, and we don't have a clue why.
     */
//    ASSERT( *PIE_WDMA_DGCR & PIE_WDMA_DGCR_DRC );

#if 0
    /* davep 12-Sep-2005 ; debugging code for asserting on memory
     * corruption. Add in extra padding into XYScale's OUTX and check
     * for it here right when the interrupt hits.
     */
    {
        uint8_t last_byte;
        int i;
        struct ice_dma_channel *ch;
        struct ice_dma_buffer *icebuf;
        ATLISTENTRY *curr;

        ch = &curr_bank->piems_wdma_mm.channels[channel];
        curr = ch->running.nextEntry;
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );

        /* the last byte of each data row should be 0xbb because of the
         * extra DMA padding I'm putting in there
         */
        for( i=0 ; i<icebuf->num_rows ; i++ ) {
            last_byte = icebuf->data[ (i+1) * icebuf->bytes_per_row - 1];
            XASSERT( last_byte==0xbb, last_byte );
        }
    }
#endif
}

void 
piems_wdma_channel_launch( uint8_t channel )
{
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

    mm = &curr_bank->piems_wdma_mm;

    XASSERT( channel < mm->num_channels, channel );
    ch = &mm->channels[channel];

    launch_loop( mm, ch, mswdma_launch ); 
}

void piems_wdma_launch( void )
{
    int i;
    struct ice_dma_mm *mm;
    struct ice_dma_channel *ch;

    mm = &curr_bank->piems_wdma_mm;

    for( i=0 ; i<mm->num_channels ; i++ ) {    
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }

        piems_wdma_channel_launch(ch->channel);
    }
}

void 
piems_wdma_intmsg( uint8_t channel )
{
    piems_dma_intmsg( &curr_bank->piems_wdma_mm, channel, mswdma_launch );
}

void piems_wdma_init_colorshift( void )
{
    FOR_ALL_BANKS
        /* Does not touch hardware. Only hits the data structures */
        piems_dma_init_colorshift( &curr_bank->piems_wdma_mm, curr_bank->write_channel_strip_state );
    END_FOR_ALL_BANKS
}

void 
piems_wdma_refill( int rows_per_buffer )
{
//    FOR_ALL_BANKS
        ice_dma_refill( &curr_bank->piems_wdma_mm, rows_per_buffer,
                piems_wdma_add_buffer, piems_wdma_channel_launch );
//    END_FOR_ALL_BANKS
}

void piems_wdma_run_forever( void )
{
    FOR_ALL_BANKS
        piems_dma_run_forever( &curr_bank->piems_wdma_mm );
    END_FOR_ALL_BANKS
}

int piems_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                           int rows_per_buffer, int bytes_per_row )
{
    int retcode;
    int banknum, old_banknum;

    retcode = 0;
    banknum = first_bank();
    old_banknum = curr_bank->banknum;
    do {
        piems_dma_set_bank(banknum);

        retcode = ice_dma_setup_buffers( &curr_bank->piems_wdma_mm, channel, num_buffers,
                        rows_per_buffer, bytes_per_row, piems_wdma_add_buffer );
        if( retcode <= 0 ) {
            piems_dma_set_bank(old_banknum);
            return retcode;
        }
        banknum = next_open_bank(banknum);
    } while(banknum);
    piems_dma_set_bank(old_banknum);

    return retcode;
}

bool piems_wdma_is_idle( void )
{
    struct ice_dma_mm *mm;
    bool is_idle;

    print_running( curr_bank->piems_wdma_mm.name, 
            &curr_bank->piems_wdma_mm.channels[0] );

    if( curr_bank ) {
        mm = &curr_bank->piems_wdma_mm;
        is_idle = piems_dma_is_idle( mm );
        if( !is_idle ) {
            return false;
        }
    }

    return pie_wdma_is_idle();
}

bool piems_wdma_is_empty( void )
{
    return piems_dma_is_empty( &curr_bank->piems_wdma_mm );
}

