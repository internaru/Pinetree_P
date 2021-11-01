/* 
 *
 * ============================================================================
 * Copyright (c) 2012  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef DMA_BUFFER_H
#define DMA_BUFFER_H
/** 
 * \file dma_buffer.h
 * 
 * \brief BigBuffer compatible routines for doing dma on linux and threadx.
 *
 *  
 */

#include "memAPI.h"
#include "BigBuffer.h"

#ifndef __linux

#define dma_buffer_malloc BigBuffer_Malloc
#define dma_buffer_free BigBuffer_Free
#define dma_buffer_adopt BigBuffer_adopt
#define dma_buffer_realloc BigBuffer_realloc
#define BigBuffer_convert_to_dma_buffer( bb ) bb


// todo needs to return bb->data
#define dma_buffer_map_single( bb, direction ) \
  ( direction == DMA_FROM_DEVICE ? \
    cpu_dcache_invalidate_region(bb->data, bb->datalen) : \
    cpu_dcache_writeback_region(bb->data, bb->datalen) )
#define dma_buffer_unmap_single( bb, direction ) \
  ( direction == DMA_FROM_DEVICE ? \
    cpu_dcache_invalidate_region(bb->data, bb->datalen) : \
    (void) ) 

#else // linux

#include "dma_alloc_api.h"


BigBuffer_t *dma_buffer_malloc( mlimiter_t *limiter, int32_t len );
BigBuffer_t *dma_buffer_realloc( BigBuffer_t *bigBuffer, int32_t len );
BigBuffer_t *dma_buffer_free( BigBuffer_t *bigBuffer );

/// dma_buffer_malloc or BigBuffer_convert_to_dma_buffer( BigBuffer_Malloc )
BigBuffer_t *BigBuffer_convert_to_dma_buffer( BigBuffer_t *big_buffer );

/// adopt a foreign memory block and convert to a dma buffer, 
/// free the original iff a copy is made
BigBuffer_t *dma_buffer_adopt( void *data, int32_t len );

/// convert a foreign memory block into a dma buffer via a copy.
BigBuffer_t *dma_buffer_copy_from( void *data, int32_t len );


/// must be called pre dma, returns the HW address to program the hw dma,
/// flush/invalidates based on the direction of the dma.
void * dma_buffer_map_single( BigBuffer_t *bigBuffer, int direction ); 

/// must be called post dma 
/// flush/invalidates based on the direction of the dma.
void * dma_buffer_unmap_single( BigBuffer_t *bigBuffer, int direction ); 

/// get the data pointer mapped to application space to allow modification.
void *dma_buffer_mmap_forcpu( BigBuffer_t *bigBuffer );

/// free the mapping when done.
void *dma_buffer_unmmap_forcpu( BigBuffer_t *bigBuffer );

/// get a dma descriptor that is uncached, size is limited.
// defined in memAPI.h
// void *memMallocPhysical( void *hw_address, size_t size, int align, char *file, int line );
// void *memFreePhysical( void *hw_address, void *v_address );
#endif // linux 
#endif
