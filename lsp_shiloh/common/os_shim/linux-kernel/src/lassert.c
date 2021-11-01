/*
 **************************************************************************************
 *
 * Copyright (c) 2012-2013 Marvell International, Ltd.
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

#include <linux/module.h>
#include <linux/kernel.h> 

#include "lassert.h"

#define MAX_ASSERT_FCT 16
static assertfunc assert_fct_list[MAX_ASSERT_FCT];
static int num_assert_fct;

/**
 * \brief  Register a function to be called when assert fails.
 *
 * The function should ONLY do emergency shutdown. The function is called as
 * one of many so please don't take too much time in your function. Don't do
 * any debug logging. Don't do any printfs. Just turn off hardware and return. 
 *
 * The idea was originally created ~2005 to shut down scan motors on an assert
 * fail. 
 *
 * \author David Poole
 * \date 05-Dec-2012
 */

int atassert( assertfunc f )
{
    XASSERT( num_assert_fct < MAX_ASSERT_FCT, num_assert_fct );

    /* Warning! No thread protection! (Yet.) */
    assert_fct_list[num_assert_fct++] = f;

    return 0;
}

static void call_atassert_fcts( void )
{
    int i;

    for( i=0 ; i<num_assert_fct ; i++ ) {
        /* Calling a function pointer! Do you feel lucky, punk? */
        assert_fct_list[i]();
    }
}

void AssertFail( const char *expression, const char *file, int line )
{
    call_atassert_fcts();

    printk( KERN_CRIT "ASSERT failed at %s %d\n", file, line );

    panic("BUG!"); 
}

void XAssertFail( const char *expression, const char *file, int line, unsigned long int errvalue )
{
    call_atassert_fcts();

    printk( KERN_CRIT "XASSERT failed at %s %d value=0x%x\n", file, line, (uint32_t)errvalue );

    panic("BUG!"); 
}

void _assert( const char *expression, const char *file, int line )
{
  XAssertFail(expression, file, line, 0);
}
