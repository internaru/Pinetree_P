/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanmem.c
 *
 * \brief Needed a clear and easy way to get memory statistics.
 *
 */

#include <stdint.h>
#include <stdlib.h>

#include "lassert.h"
#include "memAPI.h"
#include "platform.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanmem.h"
#include "notaheap.h"

#if ICE_DMA_BYTE_ALIGN==32
    #define ICE_DMA_ALLOC_ALIGN e_32_byte
#elif ICE_DMA_BYTE_ALIGN==16
    #define ICE_DMA_ALLOC_ALIGN e_16_byte
#else
    #error what is your ICE DMA byte alignment?
#endif

static mlimiter_t *scan_heap;

static struct notaheap scan_fast_memory;

uint32_t scanmem_get_free_size( void ) 
{
//    dbg2( "%s %d max=%d %d %d active=%d current=%d\n", __FUNCTION__, mlimiter_available(scan_heap), scan_heap->max, 
//                scan_heap->highest_max, scan_heap->lowest_max, scan_heap->active, scan_heap->current );

    return mlimiter_available(scan_heap);
}

uint32_t scanmem_get_heap_size( void )
{
    return scan_heap->max;
}

void * scanmem_alloc_aligned( uint32_t datalen )
{
    return MEM_MALLOC_ALIGN_LIMITED( scan_heap, datalen, ICE_DMA_ALLOC_ALIGN );
}

void scanmem_free_aligned( uint8_t **ptr, uint32_t datalen )
{
    PTR_FREE(*ptr);
}

void * scanmem_alloc( uint32_t datalen )
{
    return MEM_MALLOC_LIMITED( scan_heap, datalen );
}

void scanmem_free_fast_memory( scanmem_tag tag, void *ptr, uint32_t datalen, int instance )
{
    notaheap_free( &scan_fast_memory, tag, ptr, datalen, instance );
}

/**
 * \brief  Get a pointer into fast memory, if available.
 *
 * Get a pointer into SRAM, if we have SRAM on this platform. 
 *
 * Originally created to put PRNU/DSNU LUT into LCM to reduce bus bandwidth for
 * color copies.
 *
 * \author David Poole
 * \date 27-Jun-2012
 */

void * scanmem_get_fast_memory( scanmem_tag tag, uint32_t datalen, int instance )
{
    return notaheap_alloc( &scan_fast_memory, tag, datalen, instance );
}

/**
 * \brief  Initialize the scan memory portability layer subsystem
 *
 * \author David Poole
 * \date 23-Apr-2013
 */

scan_err_t scanmem_onetime_init( void )
{
    scan_err_t scerr;
    uint8_t *fastmem;
    uint32_t fastmem_num_bytes;

    fastmem = NULL;
    fastmem_num_bytes = 0;

#ifdef SCAN_DRIVER_MEMORY_START
    fastmem = (void*)SCAN_DRIVER_MEMORY_START;
    fastmem_num_bytes = SCAN_DRIVER_MEMORY_SIZE;
#endif

    /* initialize the fast memory sharing subsystem */
    scerr = notaheap_init( &scan_fast_memory, (uint8_t *)fastmem, fastmem_num_bytes );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* init the memory limiter (component outside scan) */
    if( !scan_heap ) {
        scan_heap = mlimiter_by_name( "scan" );
        ASSERT( scan_heap );

        if( !scan_heap ) {
            return SCANERR_NO_ENTRY;
        }
    }

    return SCANERR_NONE;
}

