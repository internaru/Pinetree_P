/******************************************************************************
 * Copyright (c)2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#ifndef DMA_ALLOC_API_H
#define DMA_ALLOC_API_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

/// dma_alloc_api violates the kernel's memory space by allowing:
/// user space allocations in the kernel.
/// MUST be root to use as /dev/mem and /dev/dma_alloc require root permission.
///
/// mmap interface allows application rw of the memory at the cost 
///   of using up extra page table space.  Don't keep it mapped forever as this 
///   effectively doubles the "ram" usage of a block while it's mapped.
///
/// 
/// vmalloc and sg lists to come.

#define DMA_ALLOC_DEVICE_FILENAME "/dev/dma_alloc"

enum dma_alloc_operation_e {
	dma_operation_alloc,            ///< kmalloc
	dma_operation_free,             ///< kfree
	dma_operation_map,              ///< dma_map_single
	dma_operation_unmap,            ///< dma_unmap_single
	dma_operation_sync_for_cpu,     ///< dma_sync_single_for_cpu
	dma_operation_sync_for_device,  ///< dma_sync_single_for_device
	dma_operation_write_to_kernel,  ///< memcpy to kernel buffer
	dma_operation_read_from_kernel, ///< memcpy from kernel buffer
	dma_operation_mmap_for_cpu,     ///< mmap kernel buffer into application space
        dma_operation_pool_alloc,       ///< allocate dma descriptor
        dma_operation_pool_free,        ///< free dma descriptor        
	dma_operation_alloc_coherent,   ///< allocate uncached memory
        dma_operation_free_coherent,    ///< free uncached memory
};

/// include kernel header file here...
#ifndef DMA_TO_DEVICE 
#define DMA_TO_DEVICE 1
#define DMA_FROM_DEVICE 2
#define DMA_BIDIRECTIONAL 0
#endif


/// needs to match drivers max, only doing one pool at the moment.
/// #define MAX_DMA_DESCRIPTOR_SIZE 64  


#ifndef defined_dma_alloc_t
#define defined_dma_alloc_t
typedef struct dma_alloc_s  dma_alloc_t;
#endif
struct dma_alloc_s
{
	enum dma_alloc_operation_e operation;  ///< write the command then read result of operation.
	size_t len; ///< length in bytes of the dma buffer
	char *kv_addr;  ///< kernel virtual address
	size_t hw_addr; ///< physical dma'able hw address
	void *v_addr;   ///< user virtual address
	int direction;  ///< DMA_TO_DEVICE DMA_FROM_DEVICE DMA_BIDIRECTIONAL

  // int error_code; ///< kernel error code on operation.  0 is OK. needs work...
};


/// kmalloc a dma'able block of ram. Cached, aligned, and contiguous.
/// Probablility of failure on alloc is a function of size 128K might be a good max, in the megabyte range will rarely ever work. 
/// 
///
dma_alloc_t *dma_alloc( size_t len );

/// same as dma_alloc but reuses an existing dma_alloc_ structure.
///
dma_alloc_t *dma_alloc_reuse( dma_alloc_t *reuse, size_t len );


/// free the dma buffer in the dma_alloc_t structure
/// Does not free the user space dma_alloc_t structure.
///
/// free( dma_free( dma_alloc ) ); // to free both.
/// Do not use memFree. TODO: do we need to use memMALLOC to allocate/free here?
dma_alloc_t *dma_free( dma_alloc_t *dma_alloc );


/// get the hardware physical address in hw_addr, flush/invalidate depending on 
/// direction.
/// 
dma_alloc_t *dma_alloc_map_single( dma_alloc_t *dma_alloc, int direction );

/// call when hw is done with the transfer to flush/invalidate for cpu usage
/// normally called from the completion isr
///
dma_alloc_t *dma_alloc_unmap_single( dma_alloc_t *dma_alloc, int direction );

/// get the user space virtual address in v_addr by mapping the hw address into the 
/// process space of the application.
///
dma_alloc_t *dma_alloc_mmap_forcpu( dma_alloc_t *dma_alloc );

/// unmap the memory from process space, do this prior to freeing 
///
///
dma_alloc_t *dma_alloc_unmmap_forcpu( dma_alloc_t *dma_alloc );

/// allocate a coherent block of uncached memory for use as a dma descriptor.
/// don't try large allocations it will fail this is for descriptors. ie < 4k.
///
dma_alloc_t *dma_alloc_pool_alloc( dma_alloc_t *dma_alloc, size_t size );

/// free a dma descriptor allocated with dma_alloc_pool_alloc()
/// 
///
dma_alloc_t *dma_alloc_pool_free( dma_alloc_t *dma_alloc );

/// write as in copy data 
///
///
dma_alloc_t *dma_alloc_write( dma_alloc_t *dma_alloc, const void *buf, size_t count );

/// read as in copy data
///
///
dma_alloc_t *dma_alloc_read( dma_alloc_t *dma_alloc, void *buf, size_t count );


/// allocate uncached memory
///
///
dma_alloc_t *dma_alloc_alloc_coherent( size_t len );

/// free uncached memory
///
///
dma_alloc_t *dma_alloc_free_coherent( dma_alloc_t *dma_alloc );

#ifdef __cplusplus
}
#endif

#endif // xxx_H
