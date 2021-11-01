/*
 **************************************************************************************
 *
 * Copyright (c) 2008, 2013 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

/*
 * \file memAPI.h
 *
 * \brief External memory allocation definitions and symbols used in the scan
 * firmware.
 *
 */

#ifndef MEMAPI_H
#define MEMAPI_H

#include <stdint.h>
#include <stdbool.h>

/** Allocation memory alignment */
typedef enum {
    e_4_byte =  0x04,
    e_8_byte =  0x08,
    e_16_byte = 0x10,
    e_32_byte = 0x20
} tAlign;

void *memMallocAlign(uint32_t size, tAlign align);
void *memMalloc(uint32_t size);
void memFree(void *address);

/* Wrapper around malloc so can (eventually) add debugging stuff. For now, send
 * directly to memMalloc()
 */
#define MEM_MALLOC(sz) memMalloc(sz)

typedef struct {
    uint32_t max;
} mlimiter_t;

mlimiter_t *mlimiter_by_name( const char *name );
void mlimiter_start( mlimiter_t * );
void mlimiter_stop( mlimiter_t * );

int32_t mlimiter_available( mlimiter_t *limiter );

void mlimiter_adjust_limits( mlimiter_t *limiter, uint32_t lowest_max, uint32_t highest_max );

#define MEM_MALLOC_ALIGN(size,align) memMallocAlign(size,align)
#define MEM_MALLOC_ALIGN_LIMITED(limiter,size,align) memMallocAlign(size,align)
#define MEM_MALLOC_LIMITED(limiter,size) memMalloc(size)

void * memMallocPhysical(
    void **physicalAddress, ///< return physical address may be different from virtual address.
    uint32_t size, ///< number of bytes to allocate
    uint32_t align,  ///< alignment in power of 2; ignored on Linux
    const char *file, const int line  ///< Debug; currently ignored on Linux
    );

void *memFreePhysical( void *hw_addr, void *v_addr );

/// allocate from uncached memory pool, used for hw dma descriptors
#define MEM_MALLOC_UNCACHED(pptr, sz, align) \
    memMallocPhysical(pptr, sz, align, __FILE__, __LINE__)

/// free uncached memory allocated by MEM_MALLOC_UNCACHED
#define MEM_FREE_UNCACHED(hw_address, virtual_address)          \
    memFreePhysical(hw_address, virtual_address)

#ifndef DMA_TO_DEVICE 
// TODO: move to using common definition
enum dma_alloc_operation_e {
	dma_operation_alloc,
	dma_operation_free,
	dma_operation_map,
	dma_operation_unmap,
	dma_operation_sync_for_cpu,
	dma_operation_sync_for_device,
	dma_operation_write_to_kernel,
	dma_operation_read_from_kernel,
	dma_operation_mmap_for_cpu,
        dma_operation_pool_alloc,
        dma_operation_pool_free,
        dma_operation_alloc_coherent,
        dma_operation_free_coherent,

};

typedef struct dma_alloc_s
{
	enum dma_alloc_operation_e operation;
	size_t len;
	char *kv_addr;
	size_t hw_addr;
	void *v_addr;
	int direction;  
} dma_alloc_t;

#define DMA_TO_DEVICE 1
#define DMA_FROM_DEVICE 2
#define DMA_BIDIRECTIONAL 0
void *dma_buffer_map_single( dma_alloc_t *dma_alloc, int direction );
void dma_buffer_unmap_single( dma_alloc_t *dma_alloc );
dma_alloc_t *dma_free( dma_alloc_t *dma_alloc );
dma_alloc_t *dma_alloc( size_t len );
#endif

#endif

