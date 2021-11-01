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
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "lassert.h"
#include "interrupt_api.h"

// TODO: lockme
static bool curr_state[NR_IRQS];

void intAttach( uint32_t int_num, uint32_t flags, PFN_ISR handler, const char* name, void* context )
{
    int retval;
//    printk(KERN_INFO "%s: int_num = %d, flags = %#x, handler = %#x, name = %s, context = %#x\n",
//           __func__, int_num, (unsigned int)flags, (unsigned int)handler, name, (unsigned int)context);
    irq_set_status_flags(int_num, IRQ_NOAUTOEN);
    retval = request_irq(int_num, handler, flags, name, context);
    curr_state[int_num] = false;
    XASSERT(retval == 0, retval);
}

void intDetach( uint32_t int_num, void* context)
{
    XASSERT( !in_interrupt(), int_num );
    free_irq( int_num, context );
    curr_state[int_num] = false;
}

bool intEnable( uint32_t int_num )
{
//    printk( KERN_INFO "%s %d\n", __FUNCTION__, int_num );
    XASSERT( !in_interrupt(), int_num );

    if( curr_state[int_num] ) {
        return true;
    }

    enable_irq( int_num );
    curr_state[int_num] = true;
    return false;
}

bool intDisable( uint32_t int_num )
{
//    printk( KERN_INFO "%s %d\n", __FUNCTION__, int_num );
    XASSERT( !in_interrupt(), int_num );

    if( !curr_state[int_num] ) {
        //printk( KERN_DEBUG "%s %d already disabled\n", __FUNCTION__, int_num );
        return false;
    }

//    printk( KERN_INFO "%s %d\n", __FUNCTION__, __LINE__ );
    disable_irq( int_num );
    curr_state[int_num] = false;
    return true;
}

