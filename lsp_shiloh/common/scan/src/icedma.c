/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#include <string.h>

#include "scos.h"

#include "list.h"           
#include "lassert.h"
#include "debug.h"
#include "interrupt_api.h"
#include "memAPI.h"
#include "hwconfig_api.h"
#include "regAddrs.h"
#include "cpu_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scantools.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "scantask.h"
#include "scanalyzer.h"
#include "scanmem.h"

//#define ICE_DMA_DEBUG
//#define ICE_DMA_ISR_DEBUG

#ifdef ICE_DMA_DEBUG
  #define ice_dbg2 dbg2 
#else
  #define ice_dbg2(...) 
#endif

/* ICE_DMA_BUFFER_POISON - memset() the DMA data buffers before freeing */
//#define ICE_DMA_BUFFER_POISON

#if ICE_DMA_BYTE_ALIGN==32
    #define ICE_DMA_ALLOC_ALIGN e_32_byte
#elif ICE_DMA_BYTE_ALIGN==16
    #define ICE_DMA_ALLOC_ALIGN e_16_byte
#else
    #error what is your ICE DMA byte alignment?
#endif

/* ICE_DMA_LOAD_LOG logs the last several pointer and row count loads into DMA.
 * Very useful when something goes wrong.
 */
#ifdef ICE_DMA_LOAD_LOG

void
ice_dma_loadlog2( struct ice_dma_channel *ch, 
                  uint32_t data, uint32_t num_rows, 
                  uint32_t dar2, uint32_t drcr2 )
{
    ch->loadlog.data[ ch->loadlog.idx ] = data;
    ch->loadlog.rows[ ch->loadlog.idx ] = num_rows;

    ch->loadlog.dar2[ ch->loadlog.idx ] = dar2;
    ch->loadlog.drcr2[ ch->loadlog.idx ] = drcr2;

    ch->loadlog.idx = (ch->loadlog.idx+1) % ICE_DMA_LL_MAX;
}

void
ice_dma_loadlog( struct ice_dma_channel *ch, uint32_t data, uint32_t num_rows )
{
    ice_dma_loadlog2( ch, data, num_rows, 0, 0 );
}

void 
ice_dma_dump_loadlogs( struct ice_dma_mm *mm )
{
    int i;
    struct ice_dma_channel *ch;
    struct ice_dma_loadlog *log;
    int start, curr;

    dbg2( "%s %s\n", __FUNCTION__, mm->name );

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            /* not open, ignore it */
            continue;
        }

        /* the loadlog's idx field points to the next open entry, i.e., the
         * "oldest" element in the list
         */
        log = &ch->loadlog;
        start = log->idx;
        curr = (start+1) % ICE_DMA_LL_MAX;

       /* davep 30-Sep-2007 ; am missing the last element of the ringbuf with
         * the original while loop so switching to a do-while instead
         */
        do {
            dbg1( "%s channel=%d index=%d data=%#lx rows=%ld dar2=%#lx drcr2=%ld\n",
                        mm->name, ch->channel, curr, log->data[curr],
                        log->rows[curr], log->dar2[curr], log->drcr2[curr] );
            curr = (curr+1) % ICE_DMA_LL_MAX;
        }
        while( curr != start );
    }
}
#endif

/**
 * \brief  
 *
 * \author David Poole
 * \date 14-Aug-2012
 */

void icebuf_dma_map( struct ice_dma_buffer *icebuf )
{
    XASSERT( icebuf->dma_handle==(dma_addr_t)NULL, (uint32_t)icebuf->dma_handle );

//    dbg2( "%s %p data=%p datalen=%d max_rows=%d bpr=%d\n", __FUNCTION__,
//            icebuf, icebuf->data,
//            icebuf->datalen, icebuf->max_rows, icebuf->bytes_per_row );
//
    XASSERT( icebuf->max_rows*icebuf->bytes_per_row==icebuf->datalen,
            icebuf->datalen );

#ifdef __KERNEL__
    icebuf->dma_handle = dma_map_single( NULL, (void *)icebuf->data, 
                            icebuf->datalen, DMA_BIDIRECTIONAL );
    XASSERT( icebuf->dma_handle, (uint32_t)icebuf->data );
#else
    icebuf->dma_handle = icebuf->data;
#endif
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 14-Aug-2012
 */

void icebuf_dma_unmap( struct ice_dma_buffer *icebuf )
{
    XASSERT( icebuf->dma_handle!=(dma_addr_t)NULL, icebuf->tag );

//    dbg2( "%s %d %d %d\n", __FUNCTION__, icebuf->max_rows,
//            icebuf->bytes_per_row, icebuf->datalen );

    XASSERT( icebuf->max_rows*icebuf->bytes_per_row==icebuf->datalen,
            icebuf->datalen );

#ifdef __KERNEL__
    dma_unmap_single( NULL, icebuf->dma_handle, icebuf->datalen, DMA_BIDIRECTIONAL );
//    dma_unmap_single( NULL, icebuf->dma_handle,
//                            icebuf->num_rows*icebuf->bytes_per_row, DMA_BIDIRECTIONAL );
#else
    /* no-op */
#endif
    icebuf->dma_handle = (dma_addr_t)NULL;
}

/**
 * \brief Prep a buffer with proper DMA hygiene
 *
 * Polish that pointer to a righteous gleam!
 *
 * \author David Poole
 * \date 24-Aug-2011
 *
 */

static void ice_dma_buffer_init( uint8_t *data, int datalen )
{
    /* make sure we have a DMA-able buffer address */
    XASSERT( ICE_DMA_IS_ALIGNED((uint32_t)data), (uint32_t)data );

#ifdef DMA_BUFFER_INIT 
    /* initialize the block to a handy debugging value */
    memset( data, DMA_BUFFER_INIT, datalen );

    /* flush our memset data from cache into main memory */
    cpu_dcache_writeback_region( data, datalen );
#endif
    cpu_dcache_invalidate_region( data, datalen );

    /* any DMA should always be in an area the CPU cache ignores */
    data = CACHE_BYPASS_SET(data);
}

/**
 * \brief Allocate an icebuf header, no data
 *
 * \author David Poole
 * \date 20-Sep-2011
 *
 */

struct ice_dma_buffer * icebuf_new_blank( uint8_t tag )
{
    struct ice_dma_buffer *icebuf;

    /* davep 20-Sep-2011 ; switch to scanmem */
    icebuf = (struct ice_dma_buffer *)scanmem_alloc( sizeof(struct ice_dma_buffer) );
    if( icebuf==NULL ) {
        dbg1( "out of memory for icebuf tag=%d\n", (int)tag );
        return NULL;
    }

    /* icebuf_blank() is paranoid about leaking memory so make sure a few
     * pointers are NULL first
     */
    icebuf->data = NULL;
    icebuf->dma_handle = (dma_addr_t)NULL;

    icebuf_blank( icebuf );
    icebuf->tag = tag;

    return icebuf;
}

/**
 * \brief handy convenience function to allocate specific type of memory
 *
 * \author David Poole
 * \date 08-Mar-2006
 *
 */

uint8_t * ice_dma_alloc_buffer( int datalen )
{
    uint8_t *data;

    /* davep 20-Sep-2011 ; switch to scanmem */
    data = scanmem_alloc_aligned( datalen );

    if( data == NULL ) {
        return NULL;
    }

    ice_dma_buffer_init( data, datalen );

    return data;
}

/**
 * \brief  Allocate an ICE DMA buffer outside the Scan mlimiter context.
 *
 * Converting scan code to the OBA Mem Limiter. Need a function to allocate an
 * ICE DMA buffer outside the Scan Mem Limiter. 
 *
 * As of this writing, Scantask calls mlimiter_start() / mlimiter_stop() for
 * each incoming task (see scan_task()-scantask.c). The cal buffers must
 * persist past the call to mlimiter_stop().
 *
 * \author David Poole
 * \date 24-Aug-2011
 *
 *
 * davep 14-May-2013 ; adding memory self limiting. This function used for cal
 *                      cache and doesn't count against our self limit. Call
 *                      the malloc directly instead of scanmem_alloc_aligned()
 */

uint8_t * ice_dma_alloc_persistent_buffer( int datalen )
{
    uint8_t *data;

    data = MEM_MALLOC_ALIGN( datalen, ICE_DMA_ALLOC_ALIGN );
    if( data==NULL ) {
        return NULL;
    }

    ice_dma_buffer_init( data, datalen );
    return data;
}

/**
 * \brief Convenience function to be called from icebuf_scratch_new() and
 * icebuf_new() to demux the two different icebuf->data memory allocate
 * strategies
 *
 * \author David Poole
 * \date 08-Mar-2006
 *
 * (Originally icebuf_new())
 */

static struct ice_dma_buffer *
allocate_icebuf_with_data( int max_rows, int bytes_per_row, uint8_t tag )
{
    struct ice_dma_buffer *icebuf;
    uint8_t *data;
    int datalen;

    icebuf = icebuf_new_blank( tag );
    if( icebuf==NULL ) {
        dbg1( "out of memory for icebuf tag=%d\n", (int)tag );
        return NULL;
    }

    datalen = max_rows * bytes_per_row;

    data = ice_dma_alloc_buffer( datalen );
    if( data == NULL ) {
        dbg1( "out of memory for icebuf tag=%d data\n", (int)tag );
        PTR_FREE( icebuf );
        return NULL;
    }

    icebuf->data = data;
    icebuf->datalen = datalen;
    icebuf->max_rows = max_rows;
    /* we're a new buffer so we don't contain any real data */
    icebuf->num_rows = 0;
    icebuf->bytes_per_row = bytes_per_row;
    icebuf->tag = tag;

    return icebuf;
}

/**
 * \brief  allocate a fully populated icebuf 
 *
 * \author David Poole
 * \date 20-June-05
 *
 * davep 08-Mar-2006 ; switch to call new allocate_icebuf_with_data() so we can
 * have a function (icebuf_scratch_new()) that always allocates scratch data.
 */

struct ice_dma_buffer *
icebuf_new( int max_rows, int bytes_per_row, uint8_t tag )
{
    return allocate_icebuf_with_data( max_rows, bytes_per_row, tag );
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
    
void icebuf_free( struct ice_dma_buffer **p_icebuf )
{
    struct ice_dma_buffer *icebuf;

    /* Calling icebuf_sanity() is the caller's disgression. Assume we're
     * passed a clean icebuf!
     */

    /* use a convenience pointer */
    PTR_ASSIGN( icebuf, *p_icebuf );
    
    if( icebuf->data ) {
        /* these buffers may be pretty big so don't poison them unless
         * explicitly enabled
         */
#ifdef ICE_DMA_BUFFER_POISON
        memset( icebuf->data, 0xcc, icebuf->datalen );
#endif

        /* clear out the cache bypass bit so the heap will accept the
         * pointer back
         */
        icebuf->data = CACHE_BYPASS_CLEAR( icebuf->data );

        /* davep 02-Apr-2013 ; clear it from the VMA, too */
        if( icebuf->dma_handle ) {
            icebuf_dma_unmap( icebuf );
        }

        /* davep 14-May-2013 ; adding memory self limiting */
        scanmem_free_aligned( &icebuf->data, icebuf->datalen );
    }

    /* before freeing the structure, poison the memory so we can catch anyone
     * using a freed pointer 
     */
    memset( icebuf, 0xcc, sizeof(struct ice_dma_buffer) );

    /* now free the memory management structure itself */
    PTR_FREE( icebuf );
}

/**
 * \brief  Convenience function to VMA unmap the data pointer then free it. 
 *
 * Created to also set both data and dma_handle to NULL.
 *
 * \author David Poole
 * \date 02-Apr-2013
 */

void icebuf_free_data( struct ice_dma_buffer *icebuf )
{
    ASSERT( icebuf!=NULL );
    XASSERT( icebuf->data!=NULL, icebuf->tag );
    XASSERT( icebuf->dma_handle!=(dma_addr_t)NULL, icebuf->tag );

    icebuf_dma_unmap( icebuf );

    /* davep 14-May-2013 ; adding memory self limiting */
    scanmem_free_aligned( &icebuf->data, icebuf->datalen );
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
    
void
icebuf_sanity( struct ice_dma_buffer *icebuf )
{
    /* stupid human checks */
    XASSERT( IS_PTR_VALID( icebuf ), (uint32_t)icebuf );

    XASSERT( icebuf->cookie==ICE_DMA_COOKIE, icebuf->cookie );

    ASSERT( icebuf->tag != 0 );

    if( icebuf->data ) {
        /* make sure we have a correctly populated buffer */

        /* Since the attached data will eventually be pushed into PIE, we must have
         * a pointer that is correctly ICE DMA burst aligned.
         */
        XASSERT( ((uint32_t)icebuf->data & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, (uint32_t)icebuf->data );

        /* any DMA should always be in an area the CPU cache ignores */
        XASSERT( CACHE_BYPASS_ISSET(icebuf->data), (uint32_t)icebuf->data );
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
    
/* 9-June-05 davep ; enable super paranoid to look up the icebuf's data in the
 * heap to make sure the data came from the heap. Slows down the code
 * considerably so be very careful about using this.
 */
//#define SUPER_PARANOID

struct ice_dma_buffer * 
icebuf_pop( ATLISTENTRY *list )
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;
#ifdef SUPER_PARANOID
    uint8_t *data;
    int datalen;
#endif

    /* stupid human check */
    ASSERT( list!=NULL );
    XASSERT( IS_PTR_VALID(list), (uint32_t)list );

    /* get the first buffer and look for our starting signature */
    entry = ATRemoveHeadList( list );

    if( entry == NULL ) {
        /* empty list */
        return NULL;
    }

    /* lotsa tests; we want as much checking as possible in test code */
    XASSERT( IS_PTR_VALID(entry), (uint32_t)entry );

    /* get a pointer to the icebuf, make sure it's a valid structure */
    icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );
    XASSERT( IS_PTR_VALID(icebuf), (uint32_t)icebuf );
    XASSERT( icebuf->cookie == ICE_DMA_COOKIE, (uint32_t)icebuf ); 

    /* if this is a buffer with data, make sure it's correctly DMA-able data */
    if( icebuf->data != NULL ) {
        /* for even more paranoia, turn the DMA pointer back into a regular
         * pointer and verify it with the heap; this is a very expensive
         * operation but this code is only to be meant to run within the
         * regression tests and not shippable firmware
         */
#ifdef SUPER_PARANOID
        data = CACHE_BYPASS_CLEAR( icebuf->data );
        datalen = agPointerVerify( data );
        /* 11-Apr-05 ; instead of trying to guess what malloc() is doing under
         * the covers, just make sure the heap thinks the buffer is at least as
         * big as I expect.
         */
        XASSERT( datalen >= icebuf->datalen, (uint32_t)icebuf );
#endif

        /* 16-Apr-05 ; make sure data field is big enough for what we claim
         * we're holding
         */
        XASSERT( icebuf->datalen >= icebuf->num_rows * icebuf->bytes_per_row,
                icebuf->datalen );
        /* if we have have a data pointer, max_rows should be set */
        XASSERT( icebuf->max_rows > 0, icebuf->max_rows );
        XASSERT( icebuf->num_rows <= icebuf->max_rows, icebuf->datalen );
    }

    return icebuf;
}


//--------------------------------------------------------------------------
// Function     : ice_free_list
//   returns    : none
//   list       : list of ice_dma_buffer, with or without data 
// Created by   : David Poole
// Date created : 25-Apr-05
// Description  : 
//
//  Free a list of struct ice_dma_buffer and, if attached, its data. Sanity
//  checks several fields.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void
ice_free_list( ATLISTENTRY *list )
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;

    while( 1 ) {
        entry = ATRemoveHeadList( list );
        if( entry==NULL ) {
            break;
        }

        icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );

        icebuf_sanity( icebuf );

        icebuf_free( &icebuf );
    }
}

/**
 * \brief free an array of icebuf pointers 
 * 
 * \param icebuf_array ; array of icebuf pointers 
 * \param num_elements ; number of icebuf pointers in icebuf_array
 *
 * Walk an array of icebuf pointers assumed to be of size num_elements. Any
 * non-null pointers are icebuf_free()'d.
 *
 * The elements of icebuf_array[] will be set to NULL once freed.
 *
 * \author David Poole
 * \date 12-Jun-2008
 *
 */

void
ice_free_array( struct ice_dma_buffer *icebuf_array[],
                int num_elements )
{
    int i;

    for( i=0 ; i<num_elements ; i++ ) {
        if( icebuf_array[i] != NULL ) {
            icebuf_free( &icebuf_array[i] );
            /* icebuf_array[i] will be NULL at this point */
        }
    }
}

/**
 * \brief  Count the number of rows in a list of icebufs
 *
 * \param list ; linked list of icebufs
 *
 * A copy/paste/modify from count_list()-scantools.c
 *
 * \author David Poole
 * \date 12-Jun-2008
 *
 */

uint32_t ice_list_count_rows( ATLISTENTRY *list )
{
    struct ice_dma_buffer *icebuf;
    uint32_t total_rows, buffer_count;
    ATLISTENTRY *first;

    total_rows = 0;
    buffer_count = 0;

    /* is the list empty? */
    if( list->nextEntry==list ) {
        return 0;
    }

    first = list;
    list = list->nextEntry;
    do {

        icebuf = CONTAINING_RECORD( list, struct ice_dma_buffer, listnode );
        icebuf_sanity( icebuf );

        total_rows += icebuf->num_rows;

        list = list->nextEntry;

        /* 8-Feb-05 ; I don't know if this is big enough or too big but I need
         * a way to stop dead if there is a list problem. (I'm getting some
         * loops in my list when I add an element onto >1 list.)
         */
        XASSERT( buffer_count<65535, buffer_count );

    } while( list != first );

    return total_rows; 
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
// Notes        : 
//
//--------------------------------------------------------------------------

void
ice_free_counted_list( ATLISTENTRY *list, int expected_cnt )
{
    int cnt;
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;

    cnt = 0;
    while( 1 ) {
        entry = ATRemoveHeadList( list );
        if( entry==NULL ) {
            break;
        }

        icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );

        icebuf_sanity( icebuf );

        icebuf_free( &icebuf );

        cnt++;
    }
    XASSERT( cnt==expected_cnt, cnt );

    ice_dbg2( "freed %d ice buffers\n", cnt );
}

//--------------------------------------------------------------------------
// Function     :  icebuf_blank
//    returns   :  none
//    icebuf    :  ice_dma_buffer to init 
// Created by   : David Poole
// Date created : 20-Apr-05
// Description  : 
//
//   This is a ice_dma_buffer "constructor" and "cleanup" function rolled       
//   together. 
// 
//   The one weird thing is the paranoia check about data at the top of the
//   function. I want to be very careful about avoiding memory leaks so I check
//   to make sure the data field is NULL. 
// 
//   It's a little bit of a cognitive dissonance to set up a field before
//   calling a function to set up the structure.
//  
//  Example:
// 
//    icebuf = (struct pic_dma_buffer *)malloc(sizeof(struct pic_dma_buffer));
//    if( icebuf ) {
//       icebuf->data = NULL;
//       icebuf->dma_handle = NULL;
//       icebuf_blank( icebuf );
//    }
// 
//    Or, to clean up an icebuf before putting back onto an empties list, thus
//    assuring any buffer pulled from the list will be nicely blank:
// 
//       send_data( icebuf->data, icebuf->num_rows*icebuf->bytes_per_row );
//       icebuf->data = NULL;
//       icebuf->dma_handle = NULL;
//       icebuf_blank( icebuf );
//       ATInsertTailList( &pie_rdma_mm.empties, &icebuf->listnode );
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void
icebuf_blank( struct ice_dma_buffer *icebuf ) 
{
    /* Make sure caller explicitly freed any attached memory; I don't want to
     * be leaking anything.
     */
    XASSERT( icebuf->data==NULL, (uint32_t)icebuf->data );
    XASSERT( icebuf->dma_handle==(dma_addr_t)NULL, (uint32_t)icebuf->dma_handle );
    
    /* Don't worry about verifying the cookie. I want this function to be able
     * to be called init a malloc()'d icebuf. I still want to make absolutely
     * sure the data pointer is empty to be really paranoid about memory leaks. 
     */
    memset( icebuf, 0, sizeof(struct ice_dma_buffer) );

    /* everything is now zero so set a few fields to make it a truly valid and
     * empty buffer; setting channel to UNUSED since '0' is a valid channel
     * number.
     */
    icebuf->cookie = ICE_DMA_COOKIE;
    icebuf->channel = CHANNEL_UNUSED;
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
// Notes        : 
//
//--------------------------------------------------------------------------

static scan_err_t
ice_dma_queue_remove( struct ice_dma_mm *mm, int channel, struct ice_dma_buffer **icebuf )
{
    struct ice_dma_channel *ch;
    ATLISTENTRY *entry;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

#ifdef ICE_DMA_ISR_DEBUG
    dbg2("%s %s %d\n", __FUNCTION__, mm->name, channel );
#endif

    XASSERT( channel < mm->num_channels, channel );

    ch = &mm->channels[channel];

    entry = ATRemoveHeadList( &ch->running );
    if( entry==NULL ) {
        /* queue is empty */
        *icebuf = NULL;
        return SCANERR_NO_ENTRY;
    }

    XASSERT( ch->num_running > 0, ch->num_running );
    ch->num_running--;

    *icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );

    XASSERT( *icebuf != NULL, (uint32_t)*icebuf );
    XASSERT( (*icebuf)->cookie==ICE_DMA_COOKIE, (*icebuf)->cookie );
    
    return SCANERR_NONE;
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
// Notes        : 
//
//--------------------------------------------------------------------------

static scan_err_t
ice_dma_queue_add( struct ice_dma_mm *mm, int channel, struct ice_dma_buffer *icebuf )
{
    struct ice_dma_channel *ch;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

#ifdef ICE_DMA_ISR_DEBUG
    dbg2("%s %s %d %p\n", __FUNCTION__, mm->name, channel, icebuf );
#endif

    XASSERT( channel < mm->num_channels, channel );
    XASSERT( icebuf != NULL, (uint32_t)icebuf ); 
    XASSERT( icebuf->cookie==ICE_DMA_COOKIE, icebuf->cookie );

    ch = &mm->channels[channel];

    if( ch->num_running >= mm->drv->max_queue_depth ) {
        /* queue is full */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* remember where this buffer comes from */
    icebuf->channel = channel;

    /* attach to the end of our running queue */
    ATInsertTailList( &ch->running, &icebuf->listnode );
    ch->num_running++;

    return 0;
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
// Notes        : 
//
//--------------------------------------------------------------------------

static int
ice_dma_buffers_init( struct ice_dma_mm *mm, struct ice_dma_channel *ch, 
                      int num_empties, uint8_t tag )
{
    struct ice_dma_buffer *icebuf;
    int cnt;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */
   
    ice_dbg2( "%s %s channel=%d empties=%d\n", __FUNCTION__, mm->name,
                ch->channel, num_empties );

    /* the lists should be empty when init is called (avoid memory leaks) */
    XASSERT( ATIsListEmpty( &ch->empties ), (uint32_t)&ch->empties );
    XASSERT( ch->num_empties==0, ch->num_empties );

    for( cnt=0 ; cnt<num_empties ; cnt++ ) {

        icebuf = icebuf_new_blank( tag );
        if( icebuf == NULL ) {
            dbg1( "ICE: %s channel %d out of memory at %d of %d\n", 
                        mm->name, ch->channel, cnt, num_empties );
            break;
        }
        
        /* everything else zero */

#ifdef ICE_DMA_DEBUG
        dbg1( "%s channel %d add empty %p\n", mm->name, ch->channel, icebuf );
#endif

        ATInsertTailList( &ch->empties, &icebuf->listnode );
        ch->num_empties++;
    }

    return cnt;
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
// Notes        : 
//
//--------------------------------------------------------------------------

scan_err_t ice_dma_add_buffer( struct ice_dma_mm *mm, 
                    uint8_t channel, uint8_t *data, int datalen, 
                    int rows, int bytes_per_row )
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;
    struct ice_dma_channel *ch;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2("%s channel %d add %p len=%d rows=%d bpr=%d\n", 
                mm->name, channel, data, datalen, rows, bytes_per_row );

    /* davep 13-Apr-2008 ; add ability to handle buffers with cache bypass set
     * or clear.  Clear so IS_PTR_VALID will always work.
     */
    if( CACHE_BYPASS_ISSET( data ) ) {
        data = CACHE_BYPASS_CLEAR(data);
    }

    /* stupid human checks */
    XASSERT( channel<mm->num_channels, channel );
    XASSERT( IS_PTR_VALID( data ), (uint32_t)data );

    /* it is the caller's responsibility to send us a pointer that is correctly
     * ICE DMA burst aligned.
     */
    XASSERT( ((uint32_t)data & ~ICE_DMA_BYTE_ALIGN_MASK ) == 0, (uint32_t)data );

    /* we must have enough space to hold our data */
    XASSERT( datalen >= rows * bytes_per_row, datalen );

    ch = &mm->channels[channel];

    /* get a ice_dma_buffer from the empties list */
    entry = ATRemoveHeadList( &ch->empties );
    if( entry==NULL ) {
        /* list empty! */
        ch->no_empties++;
        ice_dbg2( "%s no empties for %s channel %d\n", 
                    __FUNCTION__, mm->name, ch->channel ); 
        return SCANERR_OUT_OF_MEMORY;
    }
    ch->num_empties--;

    icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );
    XASSERT( icebuf->cookie==ICE_DMA_COOKIE, icebuf->cookie );
    /* we should make sure anything we get from the empty list is truly empty */
    XASSERT( icebuf->data == NULL, (uint32_t)icebuf->data );

    /* davep 14-Aug-2012 ; porting to linux kernel so need 2 memory pointers.
     * Must be clear before we arrive in this function.
     */
    XASSERT( icebuf->dma_handle==(dma_addr_t)NULL, (uint32_t)icebuf->dma_handle);

    /* NOTE: Do NOT touch the data! Don't memset() it to something for
     * debugging. Both read and write DMA go through this function. Fiddling
     * with the data will cause read corruption.
     */

    /* populate the new ice_dma_buffer with the data buffer, making the data
     * pointer a correct DMA pointer for our platform
     */
    icebuf->data = data;
    icebuf->datalen = datalen;
    icebuf->max_rows = rows;
    icebuf->num_rows = 0; /* blank, no rows currently used */
    icebuf->bytes_per_row = bytes_per_row;

    /* 13-June-05 davep ; I'm not sure why I wasn't setting the channel at the
     * same time as the rest of the icebuf. I sure should have been.
     */
    icebuf->channel = channel;

    /* davep 08-Apr-2013 ; adding scan_data_type to icebuf */
    icebuf->dtype = ch->dtype;

    /* davep 31-May-2013 ; adding sensor_num to icebuf */
    icebuf->sensor_num = ch->sensor_num;

    /* davep 27-Mar-2013 ; move dma_map_single() to safer context */
    icebuf_dma_map(icebuf);
    
    ice_dbg2( "%s %p data %p now ready\n", mm->name, icebuf, icebuf->data );

    /* put the ice_dma_buffer onto the readies list, ready to be called upon to
     * feed its attached "bucket" into the DMA controller.
     */
    ATInsertTailList( &ch->readies, &icebuf->listnode );
    ch->num_readies++;

    /* success! */
    return SCANERR_NONE;
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
// Notes        : 
//
//--------------------------------------------------------------------------

struct ice_dma_buffer *
ice_dma_get_ready( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    entry = ATRemoveHeadList( &ch->readies );
    if( entry==NULL ) {
        /* list empty! */
        ch->no_readies++;
        XASSERT( ch->num_readies==0, ch->num_readies );

        return NULL;
    }
    ch->num_readies--;

    icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );
    XASSERT( IS_PTR_VALID( icebuf ), (uint32_t)icebuf );
    XASSERT( icebuf->cookie==ICE_DMA_COOKIE, icebuf->cookie );

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
// Notes        : 
//
//--------------------------------------------------------------------------

static void
channel_launch( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    int num_loaded;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    scerr = safety_lock_try_lock(&ch->lock);
    XASSERT( scerr==SCANERR_NONE, scerr );

    ice_dbg2("%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    num_loaded = 0;

    /* 31-Mar-05 ; doesn't make sense to push stuff in unless the channel is
     * emabled.
     */
    XASSERT( mm->drv->is_enabled( ch->channel ), ch->channel );

    /* we shouldn't get here if we're uninitialized */
    XASSERT( ch->is_open, ch->channel );

    /* 11-Apr-05 ; stay in this loop until we've filled the queue as deep as we
     * can
     */
    while( 1 ) {

        /* do we need to push anything into the controller or are we already full? */
        if( ch->num_running == mm->drv->max_queue_depth ) {
            ice_dbg2( "%d full %d %d\n", ch->channel, 
                        ch->num_running, mm->drv->max_queue_depth );
            break;
        }

        /* is this channel already done? If all expected rows have already run
         * through, we shouldn't push anything else into the controller.
         */
        if( ch->pending_rows == 0 ) {
            ice_dbg2("%s expect=%d pend=%d\n", __FUNCTION__, 
                        ch->expected_rows, ch->pending_rows );
            break;
        }

        /* Do we even have anything to run? As of this writing (31-Mar-05), this
         * function will be called from _add_buffer() and _add_ready() so we should
         * always have something to run. Eventually we could get in here from
         * another path so let's err on the side of caution.
         */
        icebuf = ice_dma_get_ready( mm, ch );
        if( icebuf==NULL ) {
            /* list empty! no big deal, nothing to run */
            ice_dbg2( "%s %s %d nothing pending\n", __FUNCTION__, 
                        mm->name, ch->channel );
            break;
        }

        if( icebuf->num_rows==0 ) {
            /* adjust counters */
            if( ch->pending_rows >= icebuf->max_rows ) {
                icebuf->num_rows = icebuf->max_rows;
            }
            else {
                /* take all that's left */
                icebuf->num_rows = ch->pending_rows;
            }
        }

        ch->running_rows += icebuf->num_rows;

        /* davep 15-Jun-2009 ; add run forever by ignoring expected_rows */
        if( ch->expected_rows ) {
            ch->pending_rows -= icebuf->num_rows;
        }

        /* davep 14-Sep-2005 ; added this assert since no longer adjusting
         * num_rows before subtracting
         */
        XASSERT( ch->pending_rows>=0, ch->pending_rows );

        /* davep 27-Mar-2013 ; moving dma_map_single() to safer location */
        XASSERT( icebuf->dma_handle!=(dma_addr_t)NULL, (uint32_t)icebuf->tag );
//        /* davep 13-Aug-2012 ; port to linux kernel VMA/PMA */
//        icebuf_dma_map( icebuf );

        /* load it into our DMA controller but don't start it yet */
        mm->drv->load( ch->channel, icebuf->data, icebuf->dma_handle, 
                   icebuf->num_rows, icebuf->bytes_per_row );
        ch->buffer_loads++;

        num_loaded += 1;

#ifdef ICE_DMA_LOAD_LOG
        ice_dma_loadlog( ch, (uint32_t)icebuf->data, icebuf->num_rows );
#endif

        /* add to our "mirror" queue so we know what finished when the DMA done
         * interrupts us
         */
        scerr = ice_dma_queue_add( mm, ch->channel, icebuf );
        /* we checked at the top of this function if we had enough space so we
         * better not fail now
         */
        XASSERT( scerr==SCANERR_NONE, scerr );
        
        /* everything is ok so start up the controller */
        mm->drv->start( ch->channel );
    } /* end while(1) */

    safety_lock_unlock(&ch->lock);

    /* and we're done */
}

static void
channel_sanity( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    int cnt;

    ice_dbg2("%s %d \n", __FUNCTION__, ch->channel );

    /* if the channel isn't enabled this channel probably isn't being used so
     * don't run any tests 
     */
    if( !ch->is_open ) {
        return;
    }

    /* make sure all the counts match */
//    cnt = ch->pending_rows + ch->running_rows + ch->completed_rows;
//    XASSERT( cnt==ch->expected_rows, cnt );
    
    cnt = count_list( &ch->readies );
    XASSERT( cnt==ch->num_readies, cnt );

    cnt = count_list( &ch->running );
    XASSERT( cnt==ch->num_running, cnt );

    cnt = count_list( &ch->orphans );
    XASSERT( cnt==ch->num_orphans, cnt );

    cnt = count_list( &ch->empties );
    XASSERT( cnt==ch->num_empties, cnt );

    /* davep 19-Apr-2010 ; add a hard fail if we used the orphans to catch
     * lost rows during scans
     */
    XASSERT( ch->num_orphans==0, ch->num_orphans );
    XASSERT( ch->msg_drops==0, ch->msg_drops );

    ice_dbg2( "%s ok\n", __FUNCTION__ );
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
// Notes        : 
//
//--------------------------------------------------------------------------

static void
channel_close( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    ice_dbg2( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* Since we're cleaning up and shutting down the data structure, require
     * all DMA to be complete so there is no possibility of something flying
     * off into memory.
     */
    XASSERT( ch->is_open, ch->channel );
    XASSERT( !mm->drv->is_enabled( ch->channel ), (uint32_t)mm );
    XASSERT( ch->num_running==0, ch->num_running );
    XASSERT( ATIsListEmpty( &ch->running ), (uint32_t)mm );

    /* free the lists */
    ice_free_counted_list( &ch->readies, ch->num_readies );
    ice_free_counted_list( &ch->running, ch->num_running );
    ice_free_counted_list( &ch->orphans, ch->num_orphans );
    ice_free_counted_list( &ch->empties, ch->num_empties );

    safety_lock_free( &ch->lock );

    /* and we're done */
    ch->is_open = false;
}

/**
 * \brief cancel an ICEDMA channel 
 *
 * Assumes the channel's hardware has already been disabled. Pulls any
 * currently running buffers from the running queue and puts them back onto the
 * readies list.
 *
 * \author David Poole
 * \date 10-Nov-2005
 *
 */

static void
channel_cancel( struct ice_dma_mm *mm, struct ice_dma_channel *ch )
{
    scan_err_t scerr;
    int i;
    int num_running;
    int retcode;
    struct ice_dma_buffer *icebuf;

    ice_dbg2( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    scerr = safety_lock_try_lock(&ch->lock);
    XASSERT( scerr==SCANERR_NONE, scerr );

    XASSERT( ch->is_open, ch->channel );
    XASSERT( !mm->drv->is_enabled( ch->channel ), (uint32_t)mm );

    /* ice_dma_queue_remove() changes ch->num_running so save a copy */
    num_running = ch->num_running;

    /* pull buffers off the running queue, put back onto the readies list for
     * later disposal
     */
    for( i=0 ; i<num_running ; i++ ) {
        /* pull from the running queue */
        scerr = ice_dma_queue_remove( mm, ch->channel, &icebuf );

        /* we're walking the list based on a counter so we must have found a
         * buffer on the list
         */
        XASSERT( scerr==SCANERR_NONE, scerr );

        /* NOTE: normally putting something directly onto the readies list
         * instead of going through the proper _add_ready() method would cause
         * problems for any descendent that overloaded the _add_ready() method
         * (e.g., HalfPack's synchronous DMA). However, since we're in a cancel
         * situation, we simply want to put the buffers back into a safe spot
         * to be freed later
         */

        /* pretend it never went into the running queue */
        ch->running_rows -= icebuf->num_rows;
        ch->pending_rows += icebuf->num_rows;

        /* put it on the readies list */
        ATInsertTailList( &ch->readies, &icebuf->listnode );
        ch->num_readies++;
    }

    /* stupid human check */
    retcode = count_list( &ch->running );
    XASSERT( retcode==0, retcode );

    XASSERT( ch->num_running==0, ch->num_running );

    safety_lock_unlock(&ch->lock);
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
// Notes        : 
//
//--------------------------------------------------------------------------

static void
channel_init( struct ice_dma_channel *ch, uint8_t channel )
{
    memset( ch, 0, sizeof(struct ice_dma_channel) );

    /* the structure needs to know which channel number it is so that number
     * can be passed to the hardware functions (load, start, enable, etc)
     */
    ch->channel = channel;

    ATInitList( &ch->readies );
    ATInitList( &ch->running );
    ATInitList( &ch->orphans );
    ATInitList( &ch->empties );
}

static void
channel_open( struct ice_dma_mm *mm, struct ice_dma_channel *ch,
                       uint32_t total_rows, uint32_t bytes_per_row )
{
    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2( "%s %s %d\n", __FUNCTION__, mm->name, ch->channel );

    /* before stomping all over the top of the channel, make sure everything is
     * empty (avoid memory leaks)
     */
    XASSERT( ATIsListEmpty( &ch->readies ), (uint32_t)&ch->readies );
    XASSERT( ATIsListEmpty( &ch->running ), (uint32_t)&ch->running );
    XASSERT( ATIsListEmpty( &ch->orphans ), (uint32_t)&ch->orphans );
    XASSERT( ATIsListEmpty( &ch->empties ), (uint32_t)&ch->empties );

    /* now that we're sure the channel is idle, clean it and set all its fields
     * back to a known state
     */
    channel_init( ch, ch->channel );

    /* This is how many rows we expect to see come through. When we see this
     * many, we stop. Don't change expected_rows.
     *
     * expected_rows = pending_rows + running_rows + completed_rows
     */
    ch->expected_rows = total_rows;
    ch->pending_rows = total_rows;

    ch->bytes_per_row = bytes_per_row;
    
    ATInitList( &ch->readies );
    ATInitList( &ch->running );
    ATInitList( &ch->orphans );
    ATInitList( &ch->empties );

    /* davep 17-Sep-2005 ; create the mutual exclusion sanity semaphore */
    safety_lock_init( &ch->lock );
//    /* ThreadX POSIX layer doesn't support unnamed semaphores so have to
//     * create an name and use sem_open()
//     *
//     * Should get a name like "picdma0" or "piedma1".  Putting the channel
//     * number as the last character. As of this writing, all channel
//     * numbers are single digits.
//     */
//    memset( ch->sem_name, 0, sizeof(ch->sem_name) );
//    /* -1 for NULL and -1 for our channel number character */
//    strncpy( ch->sem_name, mm->name, ICEDMA_SEM_NAME_SIZE-2 );
//    ch->sem_name[strlen(ch->sem_name)] = (char)(ch->channel + 0x30);
//    
//    /* ThreadX doesn't have the mode bits so hardwire it to a magic number
//     * (shame!)
//     *
//     * Note I'm strictly asserting on failure here. I'm assuming the
//     * semaphore does not exist. The channel data structures are
//     * created/destroyed for each scan and I want to make sure they're
//     * freed when we're done (no leaks). Plus, this is a debugging tool for
//     * mutual exclusion on the data structure. Want to make absolutely sure
//     * this semaphore is MINE and only MINE.
//     */
//    ch->lock = sem_open( ch->sem_name, O_CREAT|O_EXCL, 0700, 1 );
//    XASSERT( ch->lock != SEM_FAILED, scos_get_errno() );

    /* we're now open for business */
    ch->is_open = true;
}


void
ice_dma_channel_launch( struct ice_dma_mm *mm, uint8_t channel )
{
    XASSERT( channel < mm->num_channels, channel );
    channel_launch( mm, &mm->channels[channel] );
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
// Notes        : 
//
//--------------------------------------------------------------------------

void
ice_dma_add_ready( struct ice_dma_mm *mm, uint8_t channel, 
                   struct ice_dma_buffer **addme )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    struct ice_dma_channel *ch;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    XASSERT( IS_PTR_VALID( addme ), (uint32_t)addme );
    XASSERT( IS_PTR_VALID( *addme ), (uint32_t)*addme );
    XASSERT( (*addme)->cookie==ICE_DMA_COOKIE, (*addme)->cookie );

    /* make sure this message is AFTER the pointer validation checks! */
    ice_dbg2("%s %s %d %p\n", __FUNCTION__, mm->name, channel, *addme );

    /* stupid human checks */
    XASSERT( channel<mm->num_channels, channel );
    ch = &mm->channels[channel];
    XASSERT( ch->is_open, ch->channel );

    scerr = safety_lock_try_lock(&ch->lock);
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* use a convenience ptr */
    PTR_ASSIGN( icebuf, *addme );

    /* Since we're putting this on the ready list, there better be some buffer
     * space attached.
     */
    XASSERT( icebuf->data != NULL, (uint32_t)(icebuf->data) );
    XASSERT( icebuf->max_rows != 0, (uint32_t)icebuf );

    /* mark it empty */
    /* 21-June-05 davep ; move this to a higher level so we don't destroy read
     * DMA buffers' counts
     */
//    icebuf->num_rows = 0;

    /* I hereby claim this buffer for this channel! */
    icebuf->channel = channel;

    /* davep 08-Apr-2013 ; adding scan_data_type to icebuf */
    icebuf->dtype = ch->dtype;

    /* davep 31-May-2013 ; adding sensor_num to icebuf */
    icebuf->sensor_num = ch->sensor_num;

    /* put it on the readies list */
    ATInsertTailList( &ch->readies, &icebuf->listnode );
    ch->num_readies++;

    safety_lock_unlock(&ch->lock);
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
// Notes        : 
//
//--------------------------------------------------------------------------

void
ice_dma_free_empty( struct ice_dma_mm *mm, uint8_t channel, struct ice_dma_buffer **freeme )
{
    scan_err_t scerr;
    struct ice_dma_buffer *icebuf;
    struct ice_dma_channel *ch;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2("%s %s\n", __FUNCTION__, mm->name );

    XASSERT( IS_PTR_VALID( freeme ), (uint32_t)freeme );
    XASSERT( IS_PTR_VALID( *freeme ), (uint32_t)*freeme );
    XASSERT( (*freeme)->cookie==ICE_DMA_COOKIE, (*freeme)->cookie );

    /* stupid human checks */
    XASSERT( channel<mm->num_channels, channel );
    ch = &mm->channels[channel];
    XASSERT( ch->is_open, ch->channel );

    scerr = safety_lock_try_lock(&ch->lock);
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* use a convenience ptr */
    PTR_ASSIGN( icebuf, *freeme );

    /* it is the caller's responsiblity to make sure they got rid of their
     * attached memory
     */
    XASSERT( icebuf->data == NULL, (uint32_t)(icebuf->data) );

    icebuf->max_rows = 0;
    icebuf->num_rows = 0;
    icebuf->datalen = 0;

    /* put it on the empties list */
    ATInsertTailList( &ch->empties, &icebuf->listnode );
    ch->num_empties++;

    safety_lock_unlock(&ch->lock);
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
// Notes        : 
//
//--------------------------------------------------------------------------

void
ice_dma_debug_log( const struct ice_dma_mm *mm ) 
{
    int i;
    struct ice_dma_channel *ch;

    dbg1("%s: num_channels=%d maxq=%d\n", mm->name, mm->num_channels, mm->drv->max_queue_depth );
                
    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];

        /* only display open channels ; everything should be zero in closed
         * channels 
         */
        if( !ch->is_open ) {
            continue;
        }

        dbg1("%s[%d]: ch=%d expected_rows=%d pending_rows=%d running_rows=%d completed_rows=%d\n",
                    mm->name, i, ch->channel, ch->expected_rows,
                    ch->pending_rows, ch->running_rows, ch->completed_rows );

        dbg1("%s[%d]: num_readies=%d num_running=%d num_empties=%d num_orphans=%d (%d %d %d %d)\n",
                    mm->name, i, 

                    ch->num_readies, ch->num_running, 
                    ch->num_empties, ch->num_orphans, 

                    count_list(&ch->readies), count_list( &ch->running ), 
                    count_list( &ch->empties ), count_list(&ch->orphans)
                    
                    );

        dbg1("%s[%d]: sensor=%d no_ready=%d drops=%d loads=%d\n",
                    mm->name, i, ch->sensor_num, ch->no_readies, 
                    ch->msg_drops, ch->buffer_loads );
    }
}

/**
 * \brief  debug print the readies list
 *
 * \author David Poole
 * \date 30-Jul-2013
 */

static void ice_dma_channel_dump_list( const struct ice_dma_mm *mm, 
                                       const struct ice_dma_channel *ch, 
                                       int which )
{
    const ATLISTENTRY *curr, *head;
    struct ice_dma_buffer *icebuf;
    int sanity;

    switch(which) {
        case ICE_DMA_LIST_READIES:
            head = &ch->readies;
            break;
        case ICE_DMA_LIST_RUNNING:
            head = &ch->readies;
            break;
        case ICE_DMA_LIST_ORPHANS:
            head = &ch->readies;
            break;
        default:
            XASSERT(0,which);
            return;
    }

    if( head->nextEntry==head) {
        /* list is empty */
        return;
    }

    sanity = 0;
    curr = head->nextEntry;
    do {
        icebuf = CONTAINING_RECORD( curr, struct ice_dma_buffer, listnode );
        XASSERT( icebuf, ch->channel );
        icebuf_sanity( icebuf );

        dbg1( "icebuf=%p sensor=%d ch=%d base_row=%d num_rows=%d\n", icebuf,
                icebuf->sensor_num, icebuf->channel, icebuf->base_row,
                icebuf->num_rows );

        /* beware buggy lists with loops */
        sanity++;
        XASSERT(sanity<256,sanity);

        curr = curr->nextEntry;
    } while( curr != head );
}

void ice_dma_dump_list( const struct ice_dma_mm *mm, int which_list )
{
    int i;
    struct ice_dma_channel *ch;

    dbg1( "%s %s\n", __FUNCTION__, mm->name );

    for( i=0 ; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            /* not open, ignore it */
            continue;
        }
        ice_dma_channel_dump_list( mm, &mm->channels[i], which_list );
    }
}

void
ice_dma_sanity( struct ice_dma_mm *mm )
{
    int i;
//    int cnt;

    ice_dbg2( "%s %s\n", __FUNCTION__, mm->name );

    /* make sure all the counts match */
//    cnt = count_list( &mm->empties );
//    XASSERT( cnt==mm->num_empties, cnt );

    for( i=0 ; i<mm->num_channels ; i++ ) {
        channel_sanity( mm, &mm->channels[i] );
    }

    ice_dbg2( "%s end\n", __FUNCTION__ );
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
// Notes        : 
//
//--------------------------------------------------------------------------

void 
ice_dma_init( struct ice_dma_mm *mm, 
              const char *name, 
              const struct ice_dma_driver *drv, 
              struct ice_dma_channel *channels,
              int num_channels )
{
    int i;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2( "%s %s %d\n", __FUNCTION__, name, num_channels );

    /* This function should be called to big-hammer initialize an ICE DMA
     * data structure. Note that I'm not checking if anything is queued up in
     * the lists so make sure everything is freed before calling this function.
     */

    memset( mm, 0, sizeof(struct ice_dma_mm) );
    strncpy( mm->name, name, ICE_NAME_MAX );

    mm->num_channels = num_channels;
    mm->channels = channels;

    for( i=0 ; i<mm->num_channels ; i++ ) {
        channel_init( &mm->channels[i], (uint8_t)i );
    }

    /* 11-Apr-05 ; check each field so we can catch new fields being added to the structure
     * but missed in old code
     *
     * Don't test the icebuf_isr because that can be NULL when the driver isr
     * doesn't call into icedma.c
     */
    XASSERT( drv->msg_data!=0, (uint32_t)drv );
    XASSERT( drv->reset!=NULL, (uint32_t)drv );
    XASSERT( drv->enable!=NULL, (uint32_t)drv );
    XASSERT( drv->disable!=NULL, (uint32_t)drv );
    XASSERT( drv->load!=NULL, (uint32_t)drv );
    XASSERT( drv->start!=NULL, (uint32_t)drv );
    XASSERT( drv->is_enabled!=NULL, (uint32_t)drv );
    XASSERT( drv->max_queue_depth > 0, drv->max_queue_depth );

    mm->drv = drv;
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
// Notes        : 
//
//--------------------------------------------------------------------------

int ice_dma_open( struct ice_dma_mm *mm, uint8_t channels[],
              uint8_t num_channels, int num_empties,
              int total_rows, int bytes_per_row, uint8_t icebuf_tag )
{
    int i;
    int buffers_allocated;
    uint8_t channel;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2( "%s %s %d %d %d\n", __FUNCTION__, mm->name, num_empties, 
                total_rows, bytes_per_row );

    buffers_allocated = 0;

    /* channels[] are used as indices into the mm->channels array */

    for( i=0 ; i<num_channels ; i++ ) {
        channel = channels[i];
        XASSERT( channel < mm->num_channels, channel );
        channel_open( mm, &mm->channels[channel], total_rows, bytes_per_row );

        buffers_allocated = ice_dma_buffers_init( mm, &mm->channels[channel], num_empties, icebuf_tag );
    
    }

    /* start the refill round robin counter from the beginning (see
     * icedma_refill())
     */
    mm->next_refill_channel = 0;

    return buffers_allocated;
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
// Notes        : 
//
//--------------------------------------------------------------------------


void
ice_dma_close( struct ice_dma_mm *mm )
{
    int i;

    /* 
     *  Not interrupt safe!  Assumes proper interrupts disabled by caller.
     */

    ice_dbg2( "%s %s\n", __FUNCTION__, mm->name );

    /* Validate all queues are finished, all ice_dma_buffers have no
     * data attached. For very careful sanity checks, call the _sanity()
     * functions.
     */

    /* free all the channel data */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        if (mm->channels[i].is_open) {
            channel_close( mm, &mm->channels[i] );
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
// Notes        : 
//
//--------------------------------------------------------------------------
extern int dbg_cnt_ice_buffer_callback; //add.lsh.for pipe debug
void ice_buffer_callback( struct ice_dma_mm *mm, struct ice_dma_channel *ch, 
                          struct ice_dma_buffer *icebuf, bool last_buffer )
{
    scan_err_t scerr;
    scan_msg_t msg;

	dbg_cnt_ice_buffer_callback++;
	
#ifdef ICE_DMA_ISR_DEBUG
    dbg2( "%s %s %d %p\n", __FUNCTION__, mm->name, ch->channel, icebuf );
#endif

    /* don't call memset() on the structure (it's expensive) but make sure
     * all unused message fields are set to empty 
     */
    msg.msgType = mm->drv->msg_data;

    XASSERT( icebuf->sensor_num==ch->sensor_num, icebuf->sensor_num );

    /* Channel is newly done; tell the message recipient this is the last
     * buffer so they'll know not to expect any more data from this
     * channel.
     */
    if( last_buffer ) {
        msg.param2 = ICE_DMA_LAST_BUFFER;
    }
    else {
        /* more data to come */
        msg.param2 = 0;
    }

    /* pass the finished buffer off to a thread */
    PTR_ASSIGN( msg.param3, icebuf );
    scerr = scantask_msg_send_nowait( &msg );

    if( scerr == 0 ) {
        ch->buffer_sends++;
    }
    else {
        /* The queue might have been full. We didn't send the message to the
         * task. Now what?
         */
        ch->msg_drops++;

        /* 27-Mar-05 ; 'orphans' is a kludge. I learned long ago never to call
         * anything a "temporary kludge" but I hope I can fix this a better
         * way.
         *
         * 'orphans' simply stores buffers we failed to send to the scanning
         * thread; since I don't want to leak them and don't want to do a
         * free() in an ISR, hang onto it until later.
         */
        ATInsertTailList( &ch->orphans, &icebuf->listnode );
        ch->num_orphans++;
    }
}

extern int dbg_cnt_ice_dma_isr;	//add.lsh.for pipe debug
scan_err_t ice_dma_isr( struct ice_dma_mm *mm, uint8_t channel )
{
    scan_err_t scerr;
    struct ice_dma_buffer *donebuf;
    struct ice_dma_buffer *newbuf;
    struct ice_dma_channel *ch;
    bool last_buffer;

	dbg_cnt_ice_dma_isr++;
	
    /*  
     *   Expected to be called only from a DMA interrupt handler!
     *
     *   NOT interrupt safe!
     */

    ch = &mm->channels[channel];

    /* davep 04-Oct-2010 ; adding this assert to catch a driver improperly
     * calling the isr function 
     */
    XASSERT( ch->is_open, ch->channel );

    scerr = safety_lock_try_lock(&ch->lock);
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* move to next PIE DMA buffer */

    /* adjust the "mirror" copy of the PIE DMA controller buffer queue,
     * popping the completed buffer then pushing a new buffer if
     * necessary
     */
    scerr = ice_dma_queue_remove( mm, channel, &donebuf );

    /* There *must* be something in the channel mirror queue or we
     * wouldn't have gotten this interrupt.  If we hit this assert, my "mirror"
     * isn't correctly mirroring the PIE DMA queue.  Can also get this if I
     * forget to set up the queue.
     */
    XASSERT( scerr==SCANERR_NONE, scerr );

#ifdef ICE_DMA_ISR_DEBUG
    dbg2("pop %s %p %p %d %d %d\n", mm->name, donebuf, donebuf->data,
                ch->pending_rows, ch->running_rows, ch->completed_rows );
#endif

    /* adjust the byte counters to reflect the current situation */
    ch->running_rows -= donebuf->num_rows;
    ch->completed_rows += donebuf->num_rows;

    XASSERT( ch->running_rows >= 0, ch->running_rows );
    if( ch->expected_rows ) {
        XASSERT( ch->completed_rows <= ch->expected_rows, ch->completed_rows );
        XASSERT( ch->pending_rows >= 0, ch->pending_rows );
    }

    newbuf = NULL;
    last_buffer = false;

    if( ch->pending_rows <= 0 ) {
        /* Done, no more data expected to push. 
         *
         * TODO - could add something like the ADC Overflow debugging ; see
         * pic_interrupt().
         */
        /* 10-Apr-05 ; definately need to add overflow prevent for PIC
         * debugging 
         */

        /* In the data message we pass to the thread, we will set a flag
         * telling the thread this channel's data is complete.
         */
        /* 11-Apr-05 ; we can have pending rows and running rows. Need to make
         * sure we only flag this channel as done with *both* pending AND
         * running are complete. 
         */
        if( ch->running_rows <= 0 ) {
            last_buffer = true;
        }
    }
    else {
        /* We have more data to come so push another buffer into the
         * PIE DMA.
         *
         * 27-Mar-05 ; XXX need to better understand how to recover from
         * stalled memory.
         */
        newbuf = ice_dma_get_ready( mm, ch );
        if( newbuf==NULL ) {
            /* 11-Apr-05 ; I'm thinking I don't need to worry about a lack of
             * data to push back into the controller. With the new ICE DMA
             * data structures and the channel_launch() being called on an
             * _add_ready() or _add_buffer(), an idle buffer will be restarted
             * when new data comes along.
             *
             * The caveat is, of course, the ADC Overflow problem but instead
             * of doing an out of memory, we should set up to stop the motor
             * (thus stopping the scan input data) when we hit a low water
             * mark.
             */
        }
        else {
            /* only queue up enough space to capture the remaining 
             * rows so we don't hit an underflow
             */
            /* 29-Mar-05 ; as of this writing the PIE read/write DMA expects to
             * receive exactly the same number of rows every time. In other
             * words, newbuf->num_rows==max_rows every time.
             *
             * I set up the READ_CCR.rows and WRITE_DGCR.rows fields before the
             * entire page starts. Every incoming buffer to _load is supposed
             * to have the same row count as CCR/DGCR. 
             *
             * I'm not sure if my assumption of rows never needing to change
             * is correct. The PIE read DMA block needs to be reset in order to
             * change CCR.rows so for now I'm going to allow this function to
             * assume we might have "leftover" buffers. _load will assert if
             * num_rows doesn't match CCR/DGCR so the code that sets up the
             * page transaction will need to make sure that:
             *      (number of rows on page) % newbuf->max_rows == 0 
             */
            /* davep 14-Sep-2005 ; This is a bit of a kludge but I can't see
             * any other way around it. We cannot modify num_rows in the DMA
             * driver because we invalidate Read DMA data. We must modify
             * num_rows in the DMA driver in order to set the number of valid
             * rows in Write DMA data. Therein lies the problem.  
             * 
             * I'm using num_rows==0 to indicate a "blank" icebuf that needs to
             * have the number of rows stamped on it, i.e., a write icebuf.
             */
            if( newbuf->num_rows==0 ) {
                if( ch->pending_rows > newbuf->max_rows ) {
                    newbuf->num_rows = newbuf->max_rows;
                }
                else {
                    /* last chunk of data in this line */
                    newbuf->num_rows = ch->pending_rows;
                }
            }

            /* we now have this much data left */
            /* davep 15-Jun-2009 ; add run forever by ignoring expected_rows */
            if( ch->expected_rows ) {
                ch->pending_rows -= newbuf->num_rows;
            }
            ch->running_rows += newbuf->num_rows;
            XASSERT( ch->pending_rows>=0, ch->pending_rows );

            /* davep 27-Mar-2013 ; moving dma_map_single() to safer location */
            XASSERT( newbuf->dma_handle!=(dma_addr_t)NULL, (uint32_t)newbuf->tag );
//            /* davep 13-Aug-2012 ; port to linux kernel VMA/PMA */
//            icebuf_dma_map( newbuf );

            /* push the new buffer into the DMA hardware */
            mm->drv->load( channel, newbuf->data, newbuf->dma_handle, newbuf->num_rows, newbuf->bytes_per_row );
            ch->buffer_loads++;

#ifdef ICE_DMA_LOAD_LOG
            ice_dma_loadlog( ch, (uint32_t)newbuf->data, newbuf->num_rows );
#endif

            /* don't start the channel until we have all the data
             * structures set up correctly or we could wind up racing the
             * interrupt.
             */

            /* add the buffer to my dma 'mirror' queue */
            scerr = ice_dma_queue_add( mm, channel, newbuf );

            /* Since we just removed something from the queue, we should
             * always have space in the queue. So we should *never* fail to
             * add something to the queue. If we hit this assert, my
             * "mirror" queue code is broken.
             */
            XASSERT( scerr==SCANERR_NONE, scerr );
        }
    }

    /* NOTE: at this point, newbuf can be NULL (we're out of memory or we
     * don't need to push another buffer) so don't attempt to use any of
     * its fields.
     */

    /* davep 27-Mar-2013 ; moving dma_map_single() to safer location */
    XASSERT( donebuf->dma_handle!=(dma_addr_t)NULL, (uint32_t)donebuf->tag );
//    /* davep 14-Aug-2012 ; un-map from MMU so cpu can see data */
//    icebuf_dma_unmap( donebuf );

    ice_buffer_callback( mm, ch, donebuf, last_buffer );

    /* Now that all data structures and housekeeping is done, start the
     * DMA channel which causes the newbuf address we pushed into the
     * DMA channel to start.
     */
    if( newbuf!=NULL ) {
        mm->drv->start( channel );
    }

    safety_lock_unlock(&ch->lock);

    return SCANERR_NONE;
}

/**
 * \brief attempt to fill each and every channels' empties with new data 
 *
 *  Round-robin through each channel, adding a single buffer to each channel.
 *  Doing the round-robin to try to keep the channels' buffers as balanced as
 *  possible.
 *
 *  On the first malloc failure, assume the heap is drained again and return.
 * 
 * \author David Poole
 * \date 09-Nov-2005
 *
 */

void
ice_dma_refill( struct ice_dma_mm *mm, 
                int rows_per_buffer, 
                ice_add_buffer_f add_buffer_fcn, 
                ice_channel_launch_f channel_launch_fcn )
{
    int i;
    uint8_t *data;
    struct ice_dma_channel *ch;
    bool done;
    int total, datalen, retcode;
    struct ice_dma_channel *open_channels[32];
    int num_open_channels;

    /* davep 10-Nov-2005 ; don't ICE_DMA_DEBUG this message for a while; want
     * to see it until we're sure the refill is working well.
     */
    dbg2( "%s %s rpb=%d\n", __FUNCTION__, mm->name, rows_per_buffer );

    ASSERT( rows_per_buffer>0 );

    /* since we're going to be hammering away at a list of channels, get a
     * simpler list of open channels we can use instead of constantly polling
     * on them.
     */
    memset( open_channels, 0, sizeof(open_channels) );
    XASSERT( mm->num_channels<32, mm->num_channels );
    num_open_channels = 0;
    for( i=0; i<mm->num_channels ; i++ ) {
        ch = &mm->channels[i];
        if( !ch->is_open ) {
            continue;
        }
        /* save a copy of the open channel's pointer */
        open_channels[num_open_channels] = &mm->channels[i];
        num_open_channels++;
    }

    /* we loop on two conditions: the heap still has memory and we still have
     * channels with empties that could be refilled
     */
    done = false;

//    dbg2( "%s %d num_open_channels=%d %d,%d,%d\n", __FUNCTION__,
//            num_open_channels, open_channels[0], open_channels[1],
//            open_channels[2] );

    while( !done ) {

        /* we'll keep trying to fill the channel until there's no more empty
         * space left
         */
        total = 0;

        for( i=0; i<num_open_channels ; i++ ) {

            ch = open_channels[ mm->next_refill_channel ];

            /* move to the next channel in our stateful round robin scheme */
            mm->next_refill_channel = (mm->next_refill_channel+1) % num_open_channels;

            total += ch->num_empties;
            if( ch->num_empties==0 ) {
                continue;
            }

            datalen = ch->bytes_per_row * rows_per_buffer;
            
            /* get a new chunk of data memory */
            data = ice_dma_alloc_buffer( datalen );
            if( data == NULL ) {
                dbg1("no memory for DMA refill channel %d\n", ch->channel );
                done = true;
                break;
            }
        
            /* davep 9-Nov-05
             *
             * I can't simply call ice_dma_add_buffer() directly because this
             * refill function is a "virtual method" that must call the
             * "descendent" method to add the buffer in case the descendent
             * class overrides some behavior.  For example, HalfPack's read and
             * write add_buffer methods have to sychronize themselves in order
             * to set up a correctly sized write buffer for the read buffer.
             * (Buffer sizes to HalfPack can be different because of XYScale.)
             */
            
            retcode = add_buffer_fcn( ch->channel, data, datalen, 
                                rows_per_buffer, ch->bytes_per_row );
            
            /* add_buffer MUST have succeeded because we tested for the
             * availablily of an empty before we called
             */
            XASSERT(retcode==0, retcode );
        }

        /* everyone is full; can leave loop */
        if( total==0 ) {
            done = true;
        }
    }

    /* launch all channels (channels with nothing ready will quietly return); 
     * XXX a slight optimization would be to only launch channels we've added
     * something to
     */
    for( i=0 ; i<num_open_channels ; i++ ) {
        ch = open_channels[i];
        channel_launch_fcn( ch->channel );
    }

    /* and we're done */
}

/**
 * \brief cancel a running ICE DMA session  
 *
 * On a cancel, need to clean up the queues, moving things off the running
 * queues so calling close() later will assume everything is clean.
 *
 * \author David Poole
 * \date 10-Nov-2005
 *
 */

void
ice_dma_cancel( struct ice_dma_mm *mm )
{
    int i;

    ice_dbg2( "%s %s\n", __FUNCTION__, mm->name );

    /* disable all channels's hardware */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        mm->drv->disable( mm->channels[i].channel );
    }

    /* reset all channels' hardware */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        mm->drv->reset( mm->channels[i].channel );
    }

    /* now we have absolutely no possibility (pretty sure anyway) of any stray
     * data arrive, clean up our outstanding buffers 
     */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        if (mm->channels[i].is_open) {
            channel_cancel( mm, &mm->channels[i] );
        }
    }
}

/**
 * \brief Tell ice dma driver to run forever 
 *
 * Original ice dma design (which rippled through the rest of the code) set an
 * expected_rows value. When the expected_rows is hit, scanning would stop.
 *
 * ADF + expected_rows == FAIL.  When running from the ADF, we want to move
 * non-stop.
 *
 * This function tells the ice dma driver to ignore expected_rows.  This is a
 * kludge until I can completely eliminate expected_rows, even from the flatbed
 * scans.
 *
 * \author David Poole
 * \date 15-Jun-2009
 *
 */

void ice_dma_run_forever( struct ice_dma_mm *mm )
{
    int i;
    struct ice_dma_channel *ch;

    ice_dbg2( "%s %s\n", __FUNCTION__, mm->name );

    /* If you didn't disable the appropriate interrupts before calling this
     * function, you're almost certainly going to break something. 
     */
    for( i=0 ; i<mm->num_channels ; i++ ) {
        if( !mm->channels[i].is_open) {
            /* don't disturb unopened channels */
            continue;
        }

        ch = &mm->channels[i];

        /* disable expected_rows (I hope this works...) */
        ch->expected_rows = 0;
    }
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
 * davep 09-Dec-2010 ; moved from pic to parent class (icedma); add_buffer_fcn
 *                      fakes polymorphism (whither C++?)
 */

int ice_dma_setup_buffers( struct ice_dma_mm *mm, 
                            uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row, 
                            ice_add_buffer_f add_buffer_fcn )
{
    int i;
    int datalen;
    int retcode;
    uint8_t *data;
    int total_bytes;

    total_bytes = num_buffers * rows_per_buffer * bytes_per_row;
    dbg1( "%s %s channel=%d buffers=%d rows=%d bpr=%d total_bytes=%d\n", 
                __FUNCTION__, mm->name, channel, num_buffers, rows_per_buffer,
                bytes_per_row, total_bytes );

    XASSERT( (bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK)==0, bytes_per_row );

    datalen = rows_per_buffer * bytes_per_row;

    for( i=0 ; i<num_buffers ; i++ ) {

        data = ice_dma_alloc_buffer( datalen );
        if( data==NULL ) {
            dbg1( "%s no memory for buffers at %d of %d\n",
                        mm->name, i, num_buffers );
            break;
        }

        retcode = add_buffer_fcn( channel, data, datalen, 
                rows_per_buffer, bytes_per_row );

        /* 16-Apr-05 ; tolerate failure */
        if( retcode!=0 ) {
            dbg1("%s channel %d refused buffer at %d of %d\n", 
                        mm->name, channel, i, num_buffers );
            /* davep 14-May-2013 ; adding memory self limiting */
            scanmem_free_aligned( &data, datalen );
            
            /* we return the number of buffers successfully allocated and
             * assigned to the DMA channel.
             */
            i--;
            break;
        }
    }

    /* return the number of buffers we were able to allocate */
    return i;
}

