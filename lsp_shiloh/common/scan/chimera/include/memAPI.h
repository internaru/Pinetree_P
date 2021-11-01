/**
 * \file memAPI.h
 *
 * \brief External memory allocation definitions and symbols used in the scan
 * firmware.
 *
 * THIS IS A WORK IN PROGRESS!!!
 *
 * When the scan firmware is compiled into the BSDK BLOB (libscan-platform.a),
 * the following symbols and functions are used in the compile.  The external
 * BSDK user must create a memory interface using exactly these symbols.
 *
 * davep 15-Sep-2008
 */

/*
 * $Header: //sandbox/dpoole/scan/chimera/include/memAPI.h#4 $
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
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

#endif

