/**
 * \file ddma.c
 *
 * \brief DMA driver/data structrues for chained descriptor based DMA hardware.
 *
 * As of this writing, used by ICEtest, PIC and PIC PRNU/DSNU. (Haven't started
 * on CISX yet.)
 *
 * davep 28-Mar-2011
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "list.h"
#include "interrupt_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"

/* Define to turn on more verbose debugging */
//#define DDMA_DEBUG  

#ifdef DDMA_DEBUG
  #define ddma_dbg2 dbg2 
#else
  #define ddma_dbg2(...)
#endif

void ddma_desc_dump( struct ddma_descriptor *desc )
{
    dbg2( "desc=%p flags=%#x src=%p len=%d next=%#x\n", 
                desc,
                desc->config_flags,
                (void *)desc->src_addr,
                desc->transfer_len_bytes,
                desc->dma_next_descriptor_addr
                );
    dbg2( "desc=%p self=%p fwnext=%p fwprev=%p fwsrc=%p\n", 
                desc,
                (void *)desc->dma_ptr_self,
                (void *)desc->fw_next_descriptor_addr,
                (void *)desc->fw_prev_descriptor_addr,
                (void *)desc->fw_src_addr
                );

//    scanlog_hex_dump( (unsigned char *)desc, sizeof(struct ddma_descriptor) );

    /* davep 26-Apr-2012 ; sneak in a sanity check */
    XASSERT( desc->dma_ptr_self, (uint32_t)desc );
#ifndef __KERNEL__
    XASSERT( desc->dma_ptr_self==(dma_addr_t)desc, (uint32_t)desc );
#endif
}

void ddma_desc_flush( struct ddma_descriptor *desc )
{
    /* davep 26-Apr-2012 ; sneak in a sanity check */
    XASSERT( desc->dma_ptr_self, (uint32_t)desc );
#ifndef __KERNEL__
    /* davep 14-Jan-2013 ; if we're not in kernel, our DMA pointer should be
     * pointing to ourselves. In kernel, it's a different value (vma/pma). 
     */
    if( (struct ddma_descriptor *)desc->dma_ptr_self != desc ) {
        ddma_desc_dump( desc );
    }
    XASSERT( (struct ddma_descriptor *)desc->dma_ptr_self==desc, (uint32_t)desc );
#endif

    /* flush the descriptor from the cache to main mem where dma can find it */
    cpu_dcache_writeback_region( desc, sizeof(struct ddma_descriptor) );
    cpu_dcache_invalidate_region( desc, sizeof(struct ddma_descriptor) );
}
        
static void ddma_descriptor_list_init( struct ddma_descriptor_list *desc_list, const char *name, uint32_t config_flags )
{
    int i;
    struct ddma_descriptor *desc;

    strncpy( desc_list->name, name, DDMA_NAME_LEN );

    /* make an array of descriptors into a circular list */
    for( i=0 ; i<desc_list->num_descriptors ; i++ ) {
        desc = &desc_list->list[i];

        desc->config_flags = config_flags;

        /* davep 26-Apr-2012 ; adding vma/pma for linux kernel */
        desc->dma_ptr_self = desc_list->dma_handle + i*sizeof(struct ddma_descriptor);

        if( i < desc_list->num_descriptors-1 ) {
            desc->fw_next_descriptor_addr = &desc_list->list[i+1];
            desc->dma_next_descriptor_addr = (uint32_t)(desc_list->dma_handle + (i+1)*sizeof(struct ddma_descriptor));
        }
        else { 
            desc->fw_next_descriptor_addr = &desc_list->list[0];
            desc->dma_next_descriptor_addr = (uint32_t)desc_list->dma_handle;
        }

        if( i > 0 ) {
            desc->fw_prev_descriptor_addr = &desc_list->list[i-1];
        }
        else { 
            /* -1 to convert from count to index */
            desc->fw_prev_descriptor_addr = &desc_list->list[desc_list->num_descriptors-1];
        }

//        ddma_desc_dump( desc );

        ddma_desc_flush( desc );
    }
}

scan_err_t ddma_descriptor_list_alloc( struct ddma_descriptor_list *desc_list, const char *name, int num_descriptors )
{
//    ddma_dbg2( "%s name=\"%s\" num_descrip=%d\n", __FUNCTION__, name, num_descriptors );

    desc_list->num_descriptors = num_descriptors;

    /* beware of memory leaks */
    XASSERT( desc_list->list==NULL, (uint32_t)desc_list->list );

    /* our dma descriptors must be padded to a cache line size so we can flush
     * them from cache without breaking neighboring cache lines
     */
    XASSERT( sizeof(struct ddma_descriptor)==cpu_get_dcache_line_size(), 
            sizeof(struct ddma_descriptor));

#ifdef __KERNEL__
    desc_list->list = dma_alloc_coherent( NULL, desc_list->num_descriptors*sizeof(struct ddma_descriptor), 
                                         &desc_list->dma_handle, GFP_DMA );
#else
    desc_list->list = MEM_MALLOC_ALIGN( desc_list->num_descriptors*sizeof(struct ddma_descriptor), 
                                      e_32_byte );
    desc_list->dma_handle = desc_list->list;
#endif

    if( desc_list->list==NULL ) {
        dbg2( "%s out of memory allocing descriptor list %s\n", __FUNCTION__, name );
        return SCANERR_OUT_OF_MEMORY;
    }

    memset( desc_list->list, 0, desc_list->num_descriptors*sizeof(struct ddma_descriptor) );
    
    ddma_dbg2( "%s %s num=%d list=%p dma_handle=%x\n", __FUNCTION__, 
            desc_list->name, desc_list->num_descriptors, desc_list->list, desc_list->dma_handle );

    /* Link them all together. 
     *
     * I set the transfer int bit on each descriptor so I get an interrupt on
     * each completed transfer. 
     */
    ddma_descriptor_list_init( desc_list, name, DDMA_DESCRIPTOR_CONFIG_INT|DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK ); 

//    ddma_descriptor_list_dump( desc_list );

    /* success! */
    return 0;
}

void ddma_descriptor_list_free( struct ddma_descriptor_list *desc_list )
{
    ddma_dbg2( "%s name=\"%s\"\n", __FUNCTION__, desc_list->name );

    if( desc_list->list==NULL ) {
        /* can happen when we close due to a memory alloc fail */
        ddma_dbg2( "%s no descriptors to free in list=\"%s\"\n", __FUNCTION__, desc_list->name );
        return;
    }

    /* poison the memory */
    memset( desc_list->list, 0x42, desc_list->num_descriptors*sizeof(struct ddma_descriptor) );
    cpu_dcache_writeback_region( desc_list->list, 
                                desc_list->num_descriptors*sizeof(struct ddma_descriptor) );
#ifdef __KERNEL__
    dma_free_coherent( NULL, desc_list->num_descriptors*sizeof(struct ddma_descriptor), 
                desc_list->list, desc_list->dma_handle );
    desc_list->list = NULL;
#else
    PTR_FREE( desc_list->list );
#endif

    desc_list->num_descriptors = 0;
}

void ddma_descriptor_list_dump( struct ddma_descriptor_list *desc_list )
{
    int sanity_count;
    struct ddma_descriptor *head;
    struct ddma_descriptor *curr;

    head = desc_list->list;

    dbg2( "%s name=\"%s\" head=%p count=%d\n", __FUNCTION__, 
            desc_list->name, head, desc_list->num_descriptors );

    ASSERT( head );

    sanity_count = 0;
    curr = head;
    do {
        /* davep 11-Oct-2010 ; XXX shotgun debug for seeing weird values on
         * print
         */
        cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

        ddma_desc_dump( curr );

        /* make sure our list is still properly formed */
        XASSERT( curr->fw_next_descriptor_addr!=0, (uint32_t)curr );

        XASSERT( curr->dma_next_descriptor_addr==(uint32_t)curr->fw_next_descriptor_addr->dma_ptr_self, 
                    (uint32_t)curr );

        curr = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        /* avoid infinite loops on corrupted lists */
        sanity_count += 1;
        XASSERT( sanity_count<=desc_list->num_descriptors, sanity_count );

    } while( curr != head );
}

scan_err_t ddma_channel_alloc_descriptors( struct ddma_channel *dch, int num_descriptors )
{
    scan_err_t scerr;

    XASSERT( dch->is_open, dch->channel );

    scerr = ddma_descriptor_list_alloc( &dch->desc_list, dch->name, num_descriptors );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    dch->head_desc = &dch->desc_list.list[0];
    dch->tail_desc = &dch->desc_list.list[0];

    /* Last link in the chain. We'll move this flag along to indicate the last
     * valid buffer in the chain. The DMA HW will stop after finishing this
     * descriptor. We don't want that. We want to keep feeding DMA, keep it
     * running as long as possible.
     */
    dch->tail_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_STOP; 

    ddma_desc_flush( dch->head_desc );
    ddma_desc_flush( dch->tail_desc );

    /* success! */
    return 0;
}

void ddma_channel_free_descriptors( struct ddma_channel *dch )
{
    XASSERT( dch->is_open, dch->channel );
    ddma_descriptor_list_free( &dch->desc_list );
}

scan_err_t ddma_descriptor_alloc( struct ddma_descriptor **pdesc )
{
    dma_addr_t dma_handle;

    /* allocate a single, properly formatted descriptor (used in test code) */

#ifdef __KERNEL__
    *pdesc = dma_alloc_coherent( NULL, sizeof(struct ddma_descriptor), 
                                  &dma_handle, GFP_DMA );
#else
    *pdesc = MEM_MALLOC_ALIGN( sizeof(struct ddma_descriptor), e_32_byte );
    dma_handle = *pdesc;
#endif

    if( *pdesc==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }
    memset( *pdesc, 0, sizeof(struct ddma_descriptor) );

    (*pdesc)->dma_ptr_self = dma_handle;

    return SCANERR_NONE;
}

void ddma_descriptor_free( struct ddma_descriptor **pdesc )
{
#ifdef __KERNEL__
    dma_free_coherent( NULL, sizeof(struct ddma_descriptor), *pdesc, (*pdesc)->dma_ptr_self );
    *pdesc = NULL;
#else
    PTR_FREE( *pdesc );
#endif
}

void ddma_desc_chain_dump( struct ddma_descriptor *head_desc, int num_descriptors )
{
    int sanity_count;
    struct ddma_descriptor *head;
    struct ddma_descriptor *curr;

    head = head_desc;

    dbg2( "%s head=%p\n", __FUNCTION__, head );

    ASSERT( head );

    sanity_count = 0;
    curr = head;
    do {
        /* davep 11-Oct-2010 ; XXX shotgun debug for seeing weird values on
         * print
         */
        cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

        ddma_desc_dump( curr );

        /* make sure our list is still properly formed */
        XASSERT( curr->fw_next_descriptor_addr!=0, (uint32_t)curr );

        curr = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        /* avoid infinite loops on corrupted lists */
        sanity_count += 1;
        XASSERT( sanity_count<=num_descriptors, sanity_count );

    } while( curr != head );
}

void real_ddma_channel_sanity( const char *function, int line, struct ddma_channel *dch )
{
    int sanity_count, stop_count;
    struct ddma_descriptor *curr;
    struct ddma_descriptor *prev;

    ASSERT( dch->head_desc != NULL );
    ASSERT( dch->tail_desc != NULL );

    sanity_count = 0;
    stop_count = 0;
    curr = dch->head_desc;
    prev = dch->head_desc;
    do {
        /* make sure our list is still properly formed */
        cpu_dcache_invalidate_region( curr, sizeof(struct ddma_descriptor) );

        XASSERT( curr->fw_next_descriptor_addr!=0, (uint32_t)curr );

        prev = curr;
        curr = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        /* sanity check my links */
        XASSERT( prev->fw_next_descriptor_addr==curr, (uint32_t)curr );
        XASSERT( curr->fw_prev_descriptor_addr==prev, (uint32_t)prev );

        /* avoid infinite loops on corrupted lists */
        sanity_count += 1;
        XASSERT( sanity_count<=dch->desc_list.num_descriptors, sanity_count );

        /* add count on stop on finish */
        if( curr->config_flags & DDMA_DESCRIPTOR_CONFIG_STOP ) {
            stop_count += 1;
        }

    } while( curr != dch->head_desc );

    XASSERT( stop_count==1, stop_count );
}

scan_err_t ddma_channel_open( struct ddma_channel *dch, const char *name, uint8_t channel ) 
{
    ddma_dbg2( "%s %s %d\n", __FUNCTION__, name, channel );

    XASSERT( !dch->is_open, channel );

    dch->channel = channel;
    dch->is_open = true;

    strncpy( dch->name, name, DDMA_NAME_LEN );

    ddma_channel_lock_init( dch );

    return SCANERR_NONE;
}    

void ddma_channel_close( struct ddma_channel *dch )
{
    XASSERT( dch->is_open, dch->channel );

    ddma_channel_free_descriptors( dch );

    /* beware of memory leaks (caller should explictly call
     * ddma_channel_free_descriptors)
     */
    XASSERT( dch->desc_list.list==NULL, dch->channel );
    XASSERT( dch->desc_list.num_descriptors==0, dch->channel );

    ddma_channel_lock_free( dch );

    dch->is_open = false;
    memset( dch, 0, sizeof(struct ddma_channel) );
}

void ddma_channel_load( struct ddma_channel *dch, uint8_t *dma_dest, 
                            uint32_t num_rows, uint32_t bytes_per_row )
{
    struct ddma_descriptor *next;
    dma_addr_t dma_handle;

    XASSERT( ((uint32_t)dma_dest & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, (uint32_t)dma_dest );
//    XASSERT( (bytes_per_row & ~ICE_DMA_BYTE_ALIGN_MASK)==0, bytes_per_row );

    /* davep 04-Oct-2010 ; can hit this assert if we're trying to dma without
     * allocating the descriptors via pic_wdma_open() (e.g., scantest.c)
     */
    XASSERT( dch->desc_list.num_descriptors>0, dch->channel );

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
    XASSERT( next->src_addr==0, next->src_addr );

#ifdef __KERNEL__
    dma_handle = dma_map_single( NULL, (void *)dma_dest, 
                            num_rows*bytes_per_row, DMA_TO_DEVICE );
    XASSERT( dma_handle, (uint32_t)dma_dest );
#else
    dma_handle = (dma_addr_t)dma_dest;
#endif

    next->src_addr = (uint32_t)dma_handle;
    next->fw_src_addr = (void *)dma_dest;
    next->transfer_len_bytes = num_rows * bytes_per_row;

    next->config_flags |= DDMA_DESCRIPTOR_CONFIG_STOP|DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;

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

    ddma_channel_sanity( dch );

    /* NOTE! The task of actually pushing the desc into hardware (desc_write
     * register) is the responsibility of the caller. This function (and ddma.c
     * itself) simply manages data structures.
     */
}

void real_ddma_data_peek( const char *function, int line_number, 
                          struct ddma_descriptor_list *desc_list, 
                          int bytes_per_row )
{
    struct ddma_descriptor *desc;
    uint8_t *ptr;
    uint8_t *endptr;

    /* peek at the data at the head of the list */
    dbg2( "%s %d\n", function, line_number );

    desc = desc_list->list;
    ASSERT( desc );
    do {
        cpu_dcache_invalidate_region( desc, sizeof(struct ddma_descriptor) );
        ddma_desc_dump( desc );

#ifdef __KERNEL__
        dma_unmap_single( NULL, desc->src_addr, desc->transfer_len_bytes, DMA_BIDIRECTIONAL );
        ptr = (uint8_t *)desc->fw_src_addr;
#else
        ptr = (uint8_t *)desc->src_addr;
#endif
        if( ptr ) {
            cpu_dcache_invalidate_region( ptr, ICE_DMA_ALIGN_ME(desc->transfer_len_bytes) );
//            scanlog_hex_dump( ptr, 64 );

            endptr = ptr + desc->transfer_len_bytes;
            /* davep 08-Jun-2012 ; XXX temp ; dump first+last 64 bytes of each line */
            while( ptr < endptr ) {
                scanlog_hex_dump( ptr, 64 );
                ptr += bytes_per_row;
                scanlog_hex_dump( ptr-64, 64 );
            }
            
//            ptr += desc->transfer_len_bytes - 64;
//            scanlog_hex_dump( ptr, 64 );
        }

        desc = (struct ddma_descriptor *)desc->fw_next_descriptor_addr;

        /* davep 08-Jun-2012 ; XXX temp ; stop at first buffer */
        break;

    } while( desc != desc_list->list );
}

void ddma_channel_lock_init( struct ddma_channel *dch )
{
    safety_lock_init( &dch->lock );
}

void ddma_channel_lock_free( struct ddma_channel *dch )
{
    safety_lock_free( &dch->lock );
}

scan_err_t ddma_channel_try_lock( struct ddma_channel *dch )
{
    return safety_lock_try_lock( &dch->lock );
}

void ddma_channel_lock( struct ddma_channel *dch )
{
    safety_lock_lock( &dch->lock );
}

void ddma_channel_unlock( struct ddma_channel *dch )
{
    safety_lock_unlock( &dch->lock );
}

void ddma_channel_dump( const struct ddma_channel *dch )
{
    dbg2( "%s ch=%d name=\"%s\" head=%p tail=%p num_running=%d\n", 
            __FUNCTION__, dch->channel, dch->name, 
            dch->head_desc, dch->tail_desc, dch->num_running );
}

