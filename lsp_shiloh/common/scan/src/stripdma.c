/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/* A PIE DMA strip consists several channels depending on mono or color and
 * which WDMA format is chosen (xrgb, planar, etc). Need to track the status of
 * strips so we can context switch PIE between fully completed strips. Once a
 * strip is started, we must complete the strip's DMA.
 * 
 * The strip list is managed like a game of tetris. New transactions percolate
 * from the bottom to the top. When a strip is fully populated, it disappears
 * from the list. 
 *
 * This is just a management/tracking data structure. No hardware or memory is
 * touched in this code.
 *
 * davep 29-May-2013
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "stripdma.h"

//#define STRIPDMA_DEBUG

#ifdef STRIPDMA_DEBUG
#define strip_dbg2 dbg2
#else
#define strip_dbg2(...)
#endif

/* Store the read in high 16 bits of our uint32_t. We now have space for 16
 * read and 16 write channels.
 */
#define MAKE_READ_BITS(r) (((r)&0xffff)<<16)
#define MAKE_WRITE_BITS(w) ((w)&0xffff)

void stripdma_list_init( struct stripdma_list *stripdma, 
                        const char *name,
                        int num_strips, 
                        struct stripdma_strip list[] )
{
    int i;

    memset( stripdma, 0, sizeof(struct stripdma_list) );
    memset( list, 0, sizeof(struct stripdma_strip) * num_strips );

    strncpy( stripdma->name, name, MAX_STRIPDMA_NAME );

    stripdma->num_strips = num_strips;
    stripdma->list = list;
    
    /* initial pointer is the first element of the list */
    stripdma->bottom = &stripdma->list[0];

    /* initial pointer is the end of the list */
    stripdma->top = &stripdma->list[num_strips-1];

    /* initialize everyone's 'next' */
    for( i=0 ; i<num_strips-1 ; i++ ) {
        stripdma->list[i].next = &stripdma->list[i+1];
    }
    stripdma->list[num_strips-1].next = &stripdma->list[0];
}

void stripdma_set_mask( struct stripdma_list *stripdma, uint32_t read_mask, uint32_t write_mask )
{
    stripdma->strip_complete_mask = MAKE_READ_BITS(read_mask) |
                                    MAKE_WRITE_BITS(write_mask);
}

void stripdma_stall( struct stripdma_list *stripdma ) 
{
    strip_dbg2( "%s %s\n", __FUNCTION__, stripdma->name );

    /* gently ignore repeated stall requests */
    if( stripdma->stall_state ) {
        stripdma->stall_duplicates++;
        return;
    }

    /* tell the stripdma to stop allowing new strips but allow pending strips
     * to complete
     */
    stripdma->stall_state = true;

    /* shrink the list size to max out at whatever we have pending */
    stripdma->top = stripdma->bottom;
    while( stripdma->top->mask != 0 ) {
        stripdma->top = stripdma->top->next;

        /* sanity check against infinite loops (should always have one empty
         * node in the list so a full list should wind up with top right back
         * where he started)
         */
        ASSERT( stripdma->top != stripdma->bottom );
    }
}

void stripdma_reload( struct stripdma_list *stripdma )
{
    strip_dbg2( "%s %s\n", __FUNCTION__, stripdma->name );

    /* gently ignore repeated stall requests */
    if( !stripdma->stall_state ) {
        return;
    }

    /* tell the stripdma to allow new strips once again */
    stripdma->stall_state = false;

    /* move top to behind bottom (list back to max size) */
    while( stripdma->top->next != stripdma->bottom ) {
        stripdma->top = stripdma->top->next;
    }
}

static void stripdma_collapse( struct stripdma_list *stripdma )
{
    /* Walk the list looking for strips with a complete matching set of
     * channels. Think collapsing the filled row in Tetris.
     */
    while( stripdma->bottom != stripdma->top && 
           stripdma->bottom->mask==stripdma->strip_complete_mask ) {

        stripdma->num_pending -= 1;
        strip_dbg2( "%s completed=%p pending=%d\n", __FUNCTION__, 
                        stripdma->bottom, stripdma->num_pending );
        XASSERT( stripdma->num_pending >=0, stripdma->num_pending );

        stripdma->bottom->mask = 0;
        stripdma->bottom = stripdma->bottom->next;

        /* we might have opened up some space */
        if( stripdma->top->next->mask==0 ) {
            /* only allow more space if we're not in a stall state (stall state
             * will allow queue to drain completly)
             */
            if( !stripdma->stall_state ) {
                stripdma->top = stripdma->top->next;
            }
        }

        /* end while */
    }

    /* thanks! come again soon! */
}

static scan_err_t stripdma_push( struct stripdma_list *stripdma, uint32_t read_mask, uint32_t write_mask )
{
    uint32_t new_mask;
    struct stripdma_strip *cur;
    uint32_t xor;

//    dbg2( "%s %s read=0x%04x write=0x%04x\n", __FUNCTION__, stripdma->name, read_mask, write_mask );

    new_mask = MAKE_READ_BITS(read_mask) | MAKE_WRITE_BITS(write_mask);

    /* stupid human check */
    ASSERT( new_mask );

    /* more stupid human check -- any bits in the mask MUST be in our
     * completion mask 
     */
    XASSERT( (new_mask & stripdma->strip_complete_mask) == new_mask, new_mask ); 

//    dbg2( "%s new_mask=0x%08x\n", __FUNCTION__, new_mask );

    cur = stripdma->bottom;
    while( cur != stripdma->top ) {
        xor = cur->mask ^ new_mask;

        if( cur->mask==0 ) {
            /* we're about to consume a new node */
            stripdma->num_pending += 1;
        }

        cur->mask |= xor;
        new_mask &= ~xor;

        if( !new_mask ) {
            break;
        }
        cur = cur->next;
    }

    /* check if any of our strips are complete */
    stripdma_collapse( stripdma );

    /* anything leftover means we ran out of space */
    if( new_mask ) {
        /* overflow! */
        return SCANERR_NO_ENTRY;
    }

    return SCANERR_NONE;
}

static scan_err_t stripdma_try_push( struct stripdma_list *stripdma, uint32_t read_mask, uint32_t write_mask )
{
    uint32_t new_mask;
    struct stripdma_strip *cur;
    uint32_t xor;

    strip_dbg2( "%s %s read=0x%04x write=0x%04x\n", __FUNCTION__,
            stripdma->name, read_mask, write_mask );

    new_mask = MAKE_READ_BITS(read_mask) | MAKE_WRITE_BITS(write_mask);

    /* stupid human check */
    ASSERT( new_mask );

    /* more stupid human check -- any bits in the mask MUST be in our
     * completion mask 
     */
    /* davep 24-Jun-2013 ; XXX temp debug on the assert fail */
    if( (new_mask & stripdma->strip_complete_mask) != new_mask ) {
            dbg2( "%s %s read=0x%04x write=0x%04x\n", __FUNCTION__,
                        stripdma->name, read_mask, write_mask );
        stripdma_list_dump(stripdma);
    }
    XASSERT( (new_mask & stripdma->strip_complete_mask) == new_mask, new_mask ); 
    
    /* same thing as stripdma_push() but don't change anything and don't move any
     * pointers
     */
    cur = stripdma->bottom;
    while( cur != stripdma->top ) {
        xor = cur->mask ^ new_mask;
        new_mask &= ~xor;
        if( !new_mask ) {
            break;
        }
        cur = cur->next;
    }

    /* anything leftover means we ran out of space */
    if( new_mask ) {
        /* overflow! */
        return SCANERR_NO_ENTRY;
    }

    return SCANERR_NONE;
}

scan_err_t stripdma_try_push_read( struct stripdma_list *stripdma, uint32_t read_channel )
{
    return stripdma_try_push( stripdma, CHANNEL_TO_MASK(read_channel), 0 );
}

scan_err_t stripdma_try_push_write( struct stripdma_list *stripdma, uint32_t write_channel )
{
    return stripdma_try_push( stripdma, 0, CHANNEL_TO_MASK(write_channel) );
}

scan_err_t stripdma_push_read( struct stripdma_list *stripdma, uint32_t read_channel )
{
    return stripdma_push( stripdma, CHANNEL_TO_MASK(read_channel), 0 );
}

scan_err_t stripdma_push_write( struct stripdma_list *stripdma, uint32_t write_channel )
{
    return stripdma_push( stripdma, 0, CHANNEL_TO_MASK(write_channel) );
}

static uint32_t channel_list_to_mask( uint8_t channels[], uint8_t num_channels)
{
    int i;
    uint32_t new_mask;

    new_mask = 0;
    for( i=0 ; i<num_channels ; i++ ) {
        new_mask |= CHANNEL_TO_MASK( channels[i] );
    }

    return new_mask;
}

void stripdma_open_read( struct stripdma_list *stripdma, uint8_t channels[], uint8_t num_channels )
{
    uint32_t new_mask;

    new_mask = channel_list_to_mask( channels, num_channels );

    /* make sure read side is clear when we get here */
    XASSERT( ((stripdma->strip_complete_mask>>16) & 0xffff) == 0,
            stripdma->strip_complete_mask );

    stripdma->strip_complete_mask |= MAKE_READ_BITS(new_mask);

    dbg2( "%s mask=%x\n", __FUNCTION__, stripdma->strip_complete_mask );
}

void stripdma_open_write( struct stripdma_list *stripdma, uint8_t channels[], uint8_t num_channels )
{
    uint32_t new_mask;

    new_mask = channel_list_to_mask( channels, num_channels );

    /* make sure write side is clear when we get here */
    XASSERT( ((stripdma->strip_complete_mask) & 0xffff) == 0 , 
            stripdma->strip_complete_mask );

    stripdma->strip_complete_mask |= MAKE_WRITE_BITS(new_mask);

    dbg2( "%s mask=%x\n", __FUNCTION__, stripdma->strip_complete_mask );
}

void stripdma_close( struct stripdma_list *stripdma )
{
    char stripdma_name[MAX_STRIPDMA_NAME+1];

    /* gently ignore close of already closed */
    if( !stripdma->strip_complete_mask ) {
        return;
    }

    /* re-init our data structure, preserving critical fields */
    strncpy( stripdma_name, stripdma->name, MAX_STRIPDMA_NAME );
    stripdma_list_init( stripdma, stripdma_name, stripdma->num_strips, stripdma->list );

    stripdma->strip_complete_mask = 0;
    /* more? */
}

void stripdma_list_dump( struct stripdma_list *stripdma )
{
    int i;
    struct stripdma_strip *cur;

    dbg1( "stripdma %s mask=0x%08x num_strips=%d pending=%d stall=%d dupstall=%d\n", stripdma->name,
            stripdma->strip_complete_mask,
            stripdma->num_strips, stripdma->num_pending,
            stripdma->stall_state,
            stripdma->stall_duplicates);

    dbg1( "stripdma %s bottom=%p top=%p\n", stripdma->name, stripdma->bottom, stripdma->top );

    cur = stripdma->bottom;
    i = 0;
    while( cur != stripdma->top ) {
        dbg1( "stripdma %s %d %p 0x%08x\n", stripdma->name, i, cur, cur->mask );
        cur = cur->next;
        i++;
    }
}

