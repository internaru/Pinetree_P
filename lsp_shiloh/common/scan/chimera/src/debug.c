/** 
 * \file debug.c
 *
 * \brief Debug functions (Assert fail, etc)
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "ATypes.h"
#include "dprintf.h"
#include "lassert.h"

/* 4-Mar-05 David Poole
 * Last valid DRAM memory address. Used in pointer validity checks.
 */
const UINT32 dram_last_addr;

/* 8-Feb-05 David Poole
 * atassert() is modeled after atexit().  When (not if...WHEN) an assertion
 * fails, the AssertFail function will call each of the registered functions. 
 *
 * Registered functions should be emergency shutdown type functions. 
 * e.g.,
 *    atassert( shutdown_scan_motors );
 *    atassert( shutdown_scan_sensor );
 *
 * Interrupts will be disabled by AssertFail by the time the functions are
 * called. Given that we're in an assertion failure situation, don't depend on
 * ANYTHING being correct. The general idea is to poke some registers to stop
 * any running hardware that could be damaged when (not if) the firmware
 * abruptly stops.
 */

#define ATASSERT_MAX 10
static int atassert_count;
static assertfunc atassert_fcts[ATASSERT_MAX];

int atassert( assertfunc f )
{
    if( f == NULL ) {
        return -1;
    }
    if( atassert_count+1 >= ATASSERT_MAX ) {
        return -1;
    }
    atassert_fcts[atassert_count] = f;
    atassert_count++;
    return 0; 
}

static void call_atassert_fcts( void )
{
    int i;

    for(i=0 ; i<atassert_count ; i++ ) {
        atassert_fcts[i]();
    }
}

void AssertFail( const char *expression, const char *file, int line )
{
    // Disable ALL interrupts, so that the code execution spins here. This 
    // allows use of the debugger to look at stack frames in the backtrace
    // after stopping execution here.
//    DISABLE_ALL_INTERRUPTS;

    /* immediately call emergency shutdown functions */
    call_atassert_fcts();

    dbg_printf( ">>>> ASSERTION FAILED: \"%s\", file %s, line %d\n", expression, file, line );

    /* davep 04-Aug-2007 ; XXX new for Chimera */
    abort();

    /* should never ever ever get here */
}

/* 8-Feb-05 David Poole
 *
 * "Embraced and Extended" assert; includes a value so you'll know not only
 * that the expression failed but what the value of the expression was that did
 * fail.
 *
 * For example, 
 *   XASSERT( ptr==NULL, (UINT32)ptr );
 *   XASSERT( channel < PIC_DMA_NUM_CHANNELS, channel );
 */

void XAssertFail( const char *expression, const char *file, int line, uint32_t errvalue )
{
    // Disable ALL interrupts, so that the code execution spins here. This 
    // allows use of the debugger to look at stack frames in the backtrace
    // after stopping execution here.
//    DISABLE_ALL_INTERRUPTS; 

    /* immediately call emergency shutdown functions */
    call_atassert_fcts();
    
    dbg_printf(">>>> ASSERTION FAILED: \"%s\", file %s, line %d, value=0x%lx\n", 
            expression, file, line, errvalue );


    /* davep 04-Aug-2007 ; XXX new for Chimera */
    abort();

    /* should never ever ever get here */
}

/**
 * \brief Deprecated function.  Use AssertFail instead.  
 *
 * Support for older firmware using common/include/lassert.h
 *
 * \author David Poole
 * \date 20-Oct-2008
 *
 */

void _assert(const char *expression, const char *filename, int linenum)
{
    AssertFail( expression, filename, linenum );
}


/**************************************************
 * Function name   : hex_dump
 *    returns      : none
 *    arg1         : ptr - pointer to data to dump
 *    arg2         : size - number of bytes to dump
 * Created by      : David Poole
 * Date created    : cira 1994
 * Description     : 
 *  
 *  Does a simple hex dump out the serial port. Incredibly useful debugging fct
 *  I've dragged around with me for years.
 *
 * Notes           : 
 **************************************************/

void 
hex_dump( unsigned char *ptr, int size ) 
{
    static char hex_ascii[] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr;

    endptr = ptr + size;
    memset( line, ' ', 80 );
    line[69] = 0;
    while( ptr != endptr ) {
        hex = &line[2];
        ascii = &line[52];
        for( i = 0; i < 16; i++ ) {
            if( isprint( *ptr ) ) {
                *ascii++ = *ptr;
            }
            else {
                *ascii++ = '.';
            }
            *hex++ = hex_ascii[*ptr >> 4];
            *hex++ = hex_ascii[*ptr & 0x0f];
            *hex++ = ' ';
            ptr++;
            if( ptr == endptr ) {
                /* clean out whatever is left from the last line */ 
                memset( hex, ' ', ( 15 - i ) * 3 );
                memset( ascii, ' ', 15 - i );
                /* i+1 so we can find the starting pointer correctly in our
                 * ptr-i in the printf below
                 */
                i++;
                break;
            }
        }
        dbg_printf("%#08lx %s\n", ((UINT32)ptr)-i, line);
    }
}

