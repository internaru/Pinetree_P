/*
 **************************************************************************************
 *
 * Copyright (c) 2011, 2013 Marvell International, Ltd.
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

