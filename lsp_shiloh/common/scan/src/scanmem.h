/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANMEM_H
#define SCANMEM_H

scan_err_t scanmem_onetime_init( void );

uint32_t scanmem_get_free_size( void );
uint32_t scanmem_get_heap_size( void );

void *scanmem_alloc( uint32_t datalen );

/* davep 14-May-2013 ; adding DMA buffer memory counting so I can self limit my
 * memory allocation in linux kernel.
 *
 * Memory allocated by scanmem_alloc_aligned() is counted. The buffer must be
 * freed by scanmem_free_aligned() to track the count. When the count (the self
 * limit) is exceeded, scanmem_alloc_aligned() will return NULL.
 */
void *scanmem_alloc_aligned( uint32_t datalen );
void scanmem_free_aligned( uint8_t **ptr, uint32_t datalen );

/* davep 23-Apr-2013 ; adding 'tags' to support fast memory platforms where the
 * memory is divied up based on function rather than a true heap. Some
 * platforms set aside a compile time amount of space for specific subsystems
 * (e.g., 64kb for scan, 32kb for print, etc).
 */
typedef enum {
    SCANMEM_TAG_NULL = 0,   /* null, zip, zero, nada, empty, unused, blank */
    SCANMEM_TAG_CISX,       /* CISX */
    SCANMEM_TAG_PDLUT       /* PRNU/DSNU */
} scanmem_tag;

/* davep 27-Jun-2012 ; get a pointer into SRAM (or other similar fast memory)
 * if available 
 */
void *scanmem_get_fast_memory( scanmem_tag tag, uint32_t datalen, int instance );
void scanmem_free_fast_memory( scanmem_tag tag, void *ptr, uint32_t datalen, int instance );

#endif

