/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
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

void XAssertFail( const char *expression, const char *file, int line, unsigned long errvalue )
{
    call_atassert_fcts();

    printk( KERN_CRIT "XASSERT failed at %s %d value=0x%x\n", file, line, (unsigned int)errvalue );

    panic("BUG!"); 
}

