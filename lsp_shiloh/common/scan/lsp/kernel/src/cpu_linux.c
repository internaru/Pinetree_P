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
#include <linux/delay.h>

#include "cpu_api.h"

void cpu_spin_delay(uint32_t delayInUs)
{
    udelay( delayInUs );
}

void cpu_dcache_writeback_region( void *start_addr, uint32_t size_in_bytes )
{
    /* ??? */
}

void cpu_dcache_invalidate_region( void *start_addr, uint32_t size_in_bytes )
{
    /* ??? */
}

uint32_t cpu_get_dcache_line_size( void )
{
/* common/devices/cpu/88sv331x-v5/src/88sv331x-v5.c */
#define CACHE_LINE_SIZE 32      // the cache line size

    /* documentation says data cache line size is 32 bytes */
    return CACHE_LINE_SIZE;
}


