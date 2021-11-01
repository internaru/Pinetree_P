/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/slab.h>

#include "memAPI.h"
#include "utils.h"

static mlimiter_t scan_stub_mlimiter;

void *MEM_MALLOC_ALIGN( size_t size, tAlign align )
{
    void *ptr;

    ptr = kmalloc( size, GFP_KERNEL|GFP_DMA );
    return ptr;
}

void *MEM_MALLOC( size_t size )
{
    void *ptr;
    ptr = kmalloc( size, GFP_KERNEL );
    return ptr;
}

void memFree(void *address)
{
    kfree( address );
}

mlimiter_t * mlimiter_by_name( const char * name )
{
    if( str_match( name, "scan", 4 ) ) {
        return &scan_stub_mlimiter;
    }

    return NULL;
}

void mlimiter_start( mlimiter_t *limiter )
{
}

void mlimiter_stop( mlimiter_t *limiter )
{
}

void mlimiter_adjust_limits( mlimiter_t *limiter, uint32_t lowest_max, uint32_t highest_max )
{
}

