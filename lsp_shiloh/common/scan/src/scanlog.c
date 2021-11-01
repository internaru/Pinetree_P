/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "scos.h"

#include "dprintf.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanlog.h"

static int scanlog_level = 1;

/* davep 11-Aug-2009 ; the scos_in_interrupt() is our jerk checker */

/* davep 19-Jun-2012 ; add Linux kernel support */
#ifdef __KERNEL__
    #define v_printit(fmt,ap) vprintk(fmt,ap)
    #define dbg_printf  printk 
#else
/* davep 04-Dec-2012 ; Originally I was using minVsprintf() to do the printf()
 * style output. That fct stopped working. Now using a stack string. 
 *
 * The assert checks if someone is printf'ing a too long string (want to verify
 * we'll always get what we expect). Using a stack variable instead of static
 * because dbg1/2/3 used by multiple threads.
 */
static void v_printit( const char *fmt, va_list ap )
{
    char s[128];
    int retcode;

    retcode = vsnprintf( s, 127, fmt, ap );
    XASSERT( retcode<127, retcode );

    dbg_printf("%s", s);
}
#endif

#define SCANLOG(P) \
    va_list ap;\
    if( (P) > scanlog_level ) {\
        return;\
    }\
    if( scos_in_interrupt() ) {                                      \
        dbg_printf( ">>> SHAME ON YOU!\r\n" );  \
    }                                                           \
    va_start( ap, format );                                     \
    v_printit( (char *)format, ap );                    \
    va_end( ap );                                               \
    if( scos_in_interrupt() ) {                                      \
        dbg_printf( "<<< SHAME ON YOU!\r\n" );  \
    }

#define xSCANLOG(P) \
    va_list ap;\
    if( (P) > scanlog_level ) {\
        return;\
    }\
    va_start( ap, format );                                     \
    v_printit( (char *)format, ap );                    \
    va_end( ap );                                               

void scanlog( int priority, const char *format, ... )
{
    SCANLOG(priority)
}

void scanlog1( const char *format, ... )
{
    SCANLOG(1)
}

void scanlog2( const char *format, ... )
{
    SCANLOG(2)
}

void scanlog3( const char *format, ... )
{
    SCANLOG(3)
}

void vscanlog1( const char *format, va_list ap )
{
    if( scanlog_level < 1) {
        return;
    }
    v_printit( (char *)format, ap ); 
}

void vscanlog2( const char *format, va_list ap )
{
    if( scanlog_level < 2) {
        return;
    }
    v_printit( (char *)format, ap ); 
}

void scanlog_set_level( int priority )
{
    /* TODO add mutex protect */
    scanlog_level = priority;
}

int scanlog_get_level( void )
{
    /* TODO add mutex protect */
    return scanlog_level;
}

/**************************************************
 * Function name   : scanlog_hex_dump
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

/* davep 19-Jun-2012 ; no ctypes.h in linux kernel */
#define xxx_isprint(c) ((c)>=0x20&&(c)<=0x7e)

void scanlog_hex_dump( unsigned char *ptr, int size ) 
{
    static char hex_ascii[] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr;

    /* small optimization to avoid doing all this work if we're not log level 2 */
    if( scanlog_level < 2 ) {
        return;
    }

    endptr = ptr + size;
    memset( line, ' ', 80 );
    line[69] = 0;
    while( ptr != endptr ) {
        hex = &line[2];
        ascii = &line[52];
        for( i = 0; i < 16; i++ ) {
            if( xxx_isprint( *ptr ) ) {
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
        scanlog1( "0x%08x %s\n", ((uint32_t)ptr)-i, line );
    }
}

