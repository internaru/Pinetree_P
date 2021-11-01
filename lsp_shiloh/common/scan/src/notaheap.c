/* 
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanmem.h"
#include "notaheap.h"

//#define NHEAP_DEBUG

#ifdef NHEAP_DEBUG
#define nheap_dbg2 dbg2
#else
#define nheap_dbg2(...)
#endif

static struct scanmem_block * notaheap_search( struct notaheap *scan_mem, scanmem_tag tag, int instance )
{
    int i;

    /* linear search but this list will be very, very short */
    for( i=0 ; i<NUM_SCANMEM_BLOCKS; i++ ) {
        if(( scan_mem->blocklist[i].tag==tag ) && (scan_mem->blocklist[i].instance==instance)) {
            return &scan_mem->blocklist[i];
        }
    }

    return NULL;
}

static struct scanmem_block * notaheap_find_empty( struct notaheap *scan_mem, 
                                                   scanmem_tag tag, int instance  )
{
    int i;

    /* linear search but this list will be very, very short */
    for( i=0 ; i<NUM_SCANMEM_BLOCKS; i++ ) {

        /* verify we're not double allocating (leaking memory) */
        if ((scan_mem->blocklist[i].tag == tag) && (scan_mem->blocklist[i].instance == instance)) {
            nheap_dbg2( "%s duplicate! tag=%d instance=%d\n", __FUNCTION__,
                    tag, instance );
            XASSERT(0,tag);
        }

        if( scan_mem->blocklist[i].tag==SCANMEM_TAG_NULL ) {
            return &scan_mem->blocklist[i];
        }
    }

    return NULL;
}

uint8_t * notaheap_alloc( struct notaheap *scan_mem, scanmem_tag tag, uint32_t datalen, int instance )
{
    struct scanmem_block *block;

    nheap_dbg2( "%s tag=%d datalen=%d instance=%d\n", __FUNCTION__, 
                tag, datalen, instance);

    /* too bad, so sad, not enough memory */
    if( scan_mem->free_size_bytes < datalen ) {
        nheap_dbg2( "%s not enough memory free=%d requested=%d\n",
                __FUNCTION__, scan_mem->free_size_bytes, datalen );
        return NULL;
    }

    block = notaheap_find_empty( scan_mem, tag, instance );

    /* We should always have a free block because as of this writing
     * (23-Apr-2013), we only have two subsystems (CISX and PRNU/DSNU) that
     * need managed memory. Scan code is required to free the memory. So if
     * this assert fails, we're not free'ing something or we're trying to
     * allocate >2 blocks.
     */
    XASSERT( block, tag );

    /* beware memory leaks */
    XASSERT( block->ptr==NULL, tag );

    block->tag = tag;
    block->instance = instance;
    block->ptr = scan_mem->free_ptr;
    block->size_bytes = datalen;

    scan_mem->free_ptr += datalen;
    scan_mem->free_size_bytes -= datalen;
    XASSERT( scan_mem->free_size_bytes>=0, tag );
    scan_mem->num_allocated += 1;
    XASSERT( scan_mem->num_allocated<=NUM_SCANMEM_BLOCKS, tag );

    dbg2( "%s block tag=%d instance=%d ptr=%p len=%d\n", __FUNCTION__,
            block->tag, block->instance, block->ptr, block->size_bytes );

    return block->ptr;
}

void notaheap_free( struct notaheap *scan_mem, scanmem_tag tag, uint8_t *ptr, uint32_t datalen, int instance ) 
{
    struct scanmem_block *block;

    block = notaheap_search( scan_mem, tag, instance );

    /* if you free it, it had better have been allocated */
    XASSERT( block, tag );

    dbg2( "%s block tag=%d instance=%d ptr=%p len=%d\n", __FUNCTION__,
            block->tag, block->instance, block->ptr, block->size_bytes );

    /* verify, verify, verify. */
    XASSERT( block->tag==tag, block->tag );
    XASSERT( block->instance==instance, block->instance );    
    XASSERT( block->ptr==ptr, block->tag );
    XASSERT( block->size_bytes==datalen, block->tag );

    /* mark it free */
    memset( block, 0, sizeof(struct scanmem_block) );

    /* decrement the number of allocated blocks */
    scan_mem->num_allocated -= 1;

    XASSERT( scan_mem->num_allocated >= 0, tag );

    /* once we hit zero allocates, reset the base pointer */
    if( scan_mem->num_allocated==0 ) {
        scan_mem->free_ptr = scan_mem->base_ptr;
        scan_mem->free_size_bytes = scan_mem->base_size_bytes;
    }
}
 
scan_err_t notaheap_init( struct notaheap *scan_mem, uint8_t *mem_base, uint32_t mem_size_bytes )
{
    dbg2( "%s base=%p size=%d\n", __FUNCTION__, mem_base, mem_size_bytes );

    memset( scan_mem, 0, sizeof(struct notaheap));
    scan_mem->base_ptr = mem_base;
    scan_mem->base_size_bytes = mem_size_bytes;
    scan_mem->free_ptr = mem_base;
    scan_mem->free_size_bytes = mem_size_bytes;

    return SCANERR_NONE;
}

