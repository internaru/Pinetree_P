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


