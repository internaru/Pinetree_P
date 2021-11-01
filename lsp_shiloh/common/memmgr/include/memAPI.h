/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/**
 * \file memAPI.h
 * 
 * This file exports the API definition for the memory manager component.
 *
 *
 *                           HEAP & LIMITER CREATION AND USAGE
 *
 *  The memory manager allows for flexible heap or limiter creation and usage.
 *  Heap implementation utilizes the function "sbrk", which allows arbitrarily
 *  sized  heaps to grow to consume all available memory. If a heap allocation
 *  will fail, more memory is requested from the system via a call to "sbrk".
 *  If more memory is available, it will be granted to allow the allocation
 *  request to  succeed.
 *
 *  There are three expected product heap & limiter configurations:
 *
 *  1. One "system" heap, consuming all available memory.
 *        For this configuration, the initialization of the system heap is made
 *        using a size value in the range of zero up to the available system
 *        memory. The heap size is specified in the heap creation function call.
 *        As the heap size will grow automatically to satisfy memory allocation
 *        requests, a size of zero is legitimate!
 *
 *  2. More than one heap, sharing all available memory. For this configuration:
 *        a) The system heap must be created first.
 *        b) The sum of all the heap sizes must equal the available system
 *           memory. This will consume all available memory, effectively
 *           preventing "sbrk" from growing heaps. 
 *        c) secondary heaps will alloc free the max size, this will fail
 *           if it is not done at system startup time.
 *        d) heaps cannot be freed once created it and its memory is permanent.   
 *
 *
 *   3. One "system" heap, with "limiters" created from allocation from the
 *      system heap. For this configuration:
 *      One or more limiter handles can be attached to a heap. Anything
 *      allocated under a limiter is limited to a fix pool size of allocations.
 *
 *
 *
 *                          HEAP DEBUG FACILITIES
 *
 *  Each allocation can be marked with the filename and line number from the
 *  source code that made the allocation. Also, the time of the allocation
 *  is recorded for the purpose of determining when the allocation was made. 
 *  The time is represented as operating system ticks and is especially useful 
 *  in tracking down memory leaks. These capabilities are controlled by defining
 *  either DEBUG or MEM_DEBUG. This allows memory debug to be enabled for 
 *  release builds.
 *
 *  To accomodate the debug information, the allocation is increased in size by
 *  8 bytes and an offset pointer is returned from the allocation request. The
 *  file name information is stored as a simple 16 bit hash, the line number is
 *  also stored as 16 bits, these two form the first additional word in the
 *  allocation. The number of the allocation is stored in the second additional
 *  word in the allocation. The existence of the debug information is accounted
 *  for in the allocation and free function call, via macros.
 *
 **/
 

#ifndef MEMAPI_H
#define MEMAPI_H

#include <stdint.h>
#include "mlimiter.h"
#include "lassert.h"

#ifdef __cplusplus
#include "memAPInew.h"  // cpp new overload: must be outside of extern "C"
extern "C" {
#endif

/**
 * Allocation alignment typedef used in memory allocations that are
 * to be aligned to a specific address as listed in the enum list.
 *
 * 4 bytes is only possible without limiters and headers.
 * 8 bytes is normal
 * larger alignments are used to match cache alignment or dma 
 * channel widths.
 * the enum is used to enforce power of 2 alignments.
 */
typedef enum 
{
    e_4_byte =  4, 
    e_8_byte =  8,
    e_16_byte = 16,
    e_32_byte = 32,
    e_2048_byte = 2048
} tAlign;



/// Use the macros instead of direct function calls!
/// The DEBUG memory allocation macros will cause the allocations to be stamped
/// with file name and line number information. This information is not provided
/// for non-debug builds.

#define MEM_MALLOC(sz) \
    mem_retry_malloc_align_limited(0, sz, e_8_byte, __FILE__, __LINE__)
#define MEM_MALLOC_LIMITED(h,sz) \
    mem_retry_malloc_align_limited(h, sz, e_8_byte, __FILE__, __LINE__)
#define MEM_MALLOC_ALIGN(sz, align) \
    mem_retry_malloc_align_limited(0, sz, align, __FILE__, __LINE__)
#define MEM_MALLOC_ALIGN_LIMITED(h, sz, align) \
    mem_retry_malloc_align_limited(h, sz, align, __FILE__, __LINE__)


#define MEM_CALLOC(cnt, sz) \
    mem_retry_calloc_align_limited(0, cnt, sz, e_8_byte, __FILE__, __LINE__)
#define MEM_CALLOC_LIMITED(heap, cnt, sz) \
    mem_retry_calloc_align_limited(h, cnt, sz, e_8_byte, __FILE__, __LINE__)
#define MEM_CALLOC_ALIGN(cnt, sz, align) \
    mem_retry_calloc_align_limited(0, cnt, sz, align, __FILE__, __LINE__)
#define MEM_CALLOC_ALIGN_LIMITED(heap, cnt, sz, align) \
    mem_retry_calloc_align_limited(heap, cnt, sz, align, __FILE__, __LINE__)

// MEM_REALLOC debug macros will grow the reallocated size by the size of memory
// debug header - DBG_MEM_SIZE_BYTES.
#define MEM_REALLOC(addr, sz) \
    memReallocLimited(0, addr, sz, __FILE__, __LINE__)
#define MEM_REALLOC_LIMITED(h, addr, sz) \
    memReallocLimited(h, addr, sz, __FILE__, __LINE__)

/// allocate from uncached memory pool, used for hw dma descriptors
#define MEM_MALLOC_UNCACHED(pptr, sz, align) \
    memMallocPhysical(pptr, sz, align, __FILE__, __LINE__)

/// free uncached memory allocated by MEM_MALLOC_UNCACHED
#ifdef __linux
#define MEM_FREE_UNCACHED(hw_address, virtual_address)          \
    memFreePhysical(hw_address, virtual_address)
#else
#define MEM_FREE_UNCACHED(hw_address, virtual_address)          \
    memFree( hw_address );
#endif


#define MEM_FREE_AND_NULL(x) \
    do{ASSERT(x); memFree(x); (x) = NULL;}while(0)

#define MEM_FREE_IF_NOT_NULL(x) \
    if((x)) { memFree(x); (x) = NULL; }

/// Configure the start address and size of the heap to be configured in the
/// space.
void memInitMemory(void *memoryStartAddr, uint32_t size);


/// Create an uncached memory heap prior to creating the regular heap.
void memInitUncached(void *memoryStartAddr, uint32_t memSize);


/// Configure additional heap from available main memory. If more than one heap
/// is defined, they should all be created at power on initialization and should
/// be sized to use all available memory. Note that deletion of heaps is not
/// supported at this time.
mlimiter_t *memCreateHeap(uint32_t size, char *limiter_name);

/// Creates a sub pool with a new max memory limit that is less than the base heaps
/// allocation max.  JOB memory pool for instance.
mlimiter_t *memCreateLimiter( mlimiter_t *base, char *limiter_name  );

/// Free the given address.
void memFree(void *address); 


/// Allocate using provided limiter, size bytes using given alignment 
/// and in debug mark the allocation with passed in __FILE__ and __LINE__.
void *mem_retry_malloc_align_limited(
    mlimiter_t *lim, 
    int size, 
    int align, 
    const char *file, const int line);

/// calloc 
void *mem_retry_calloc_align_limited(
    mlimiter_t *lim, 
    uint32_t cnt,
    int size, 
    int align, 
    const char *file, const int line);

/// Change the size of the allocation.
/// This does an inplace shrink, it may NOT respect the original alignment if you choose to 
/// grow the block, so don't try that.  memReallocAlign doesn't exist because it implies 
/// the ability to change the alignment of an existing block.
void *memReallocLimited(
    mlimiter_t *myHeap, 
    void *address, 
    uint32_t size,
    const char *file, const int line);

/// Allocate an uncached memory block.
/// returns virtual address to uncached memory and a physical address for hardware direct access
/// note the CPU virtual address and the Hardware physical address may be different on some 
/// platforms such as linux.
void * memMallocPhysical(
    void **physicalAddress, ///< return physical address may be different from virtual address.
    uint32_t size, ///< number of bytes to allocate
    tAlign align,  ///< alignment in power of 2 
    const char *file, const int line
    );
void *memFreePhysical( void *hw_addr, void *v_addr );


#ifndef __linux__
#include "cpu_api.h"
#else
#ifdef  cpu_get_dcache_line_size
#undef  cpu_get_dcache_line_size
#endif
uint32_t cpu_get_dcache_line_size( void );
#endif


// initialize serial debug commands for memory.
void memory_cmd_init(void);


#if 0 // unimplemented
/// Return amount of free memory on the specified heap. If a NULL
/// pointer is provided, the system heap free memory will be returned.
uint32_t memGetHeapFreeMemorySize(mlimiter_t *heap);

/// Return free memory in the system on the system heap.
///
uint32_t memTotalFreeMemAvailable(void);

/// The size of the ram part not the heap size.
///
uint32_t memGetSystemMemorySize(void);

/// Return size of my heap, null pointer gets the system heap size.
///
uint32_t memGetHeapSize(mlimiter_t *heap);


/* ********** D E B U G  F U N C T I O N S ********** */


// Print the allocations in the specified heap. If no heap
// is specified it will dump the "system" heap. Useful with serial debug.
#ifdef DEBUG
void memDumpHeap(mlimiter_t *heap);  
void mem_dump_heap(mlimiter_t *heap, int less);
#endif

#endif // end unimplemented
#ifdef __cplusplus
}
#endif

#endif
