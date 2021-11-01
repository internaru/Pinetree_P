/* 
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NOTAHEAP_H
#define NOTAHEAP_H

struct scanmem_block {
    scanmem_tag tag;
    uint32_t size_bytes;
    uint8_t *ptr;
    int instance;
};

#define NUM_SCANMEM_BLOCKS 4

struct notaheap {
    /* the counters are signed int to catch underflow */
    struct scanmem_block blocklist[NUM_SCANMEM_BLOCKS];
    int num_allocated;

    uint8_t *free_ptr;
    int free_size_bytes;

    uint8_t *base_ptr;
    int base_size_bytes;
};

scan_err_t notaheap_init( struct notaheap *scan_mem, uint8_t *mem_base, uint32_t mem_size_bytes );
uint8_t * notaheap_alloc( struct notaheap *scan_mem, scanmem_tag tag, uint32_t datalen, int instance );
void notaheap_free( struct notaheap *scan_mem, scanmem_tag tag, uint8_t *ptr, uint32_t datalen, int instance );
        
#endif

