/**
 * \file memory.c
 *
 * \brief  memory functions used by the scan subsystem
 *
 */
/* 
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "ATypes.h"
#include "lassert.h"
#include "memAPI.h"

/**
 * \brief  Allocates memory aligned to a certain address width
 *
 * Certain DMA hardware requires memory to be lined up at bus address widths.
 *
 * \author David Poole
 * \date 17-Jul-2008
 *
 */

void *memMallocAlign(uint32_t size, tAlign align)
{
    /* TODO obey alignment */
    return malloc( size );
}

/**
 * \brief  Allocates memory
 *
 * Just like malloc()
 *
 * \author David Poole
 * \date 17-Jul-2008
 *
 */

void *memMalloc(uint32_t size)
{
    return malloc( size );
}

/**
 * \brief  Frees allocated memory
 *
 * Just like free()
 *
 * \author David Poole
 * \date 17-Jul-2008
 *
 */

void memFree(void *address)
{
    free( address );
}

static mlimiter_t scan_mlimiter = {
    .max = ~0
};

mlimiter_t *mlimiter_by_name( const char *name )
{
    return &scan_mlimiter;
}

void mlimiter_start( mlimiter_t *m )
{
}

void mlimiter_stop( mlimiter_t *m )
{
}

int32_t mlimiter_available( mlimiter_t *m )
{
    return INT_MAX;
}

void mlimiter_adjust_limits( mlimiter_t *limiter, uint32_t lowest_max, uint32_t highest_max )
{
}

