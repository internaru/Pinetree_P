/**
 * \file minprintf.c
 *
 * \brief  printf()-style functions used by the scan subsystem
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
#include <stdarg.h>

#include "dprintf.h"

/**
 * \brief  like printf()
 *
 * Scan code uses the formats: "%d %ld %s %p %x"
 * A few format flags on %d are used: "#" and "0"  (e.g., "%#02x"
 * Field width is used as well. (e.g., "%04d")
 *
 * Floating point printing support is not necessary.
 *
 * \author David Poole
 * \date 17-Jul-2008
 *
 */

int dbg_printf( const char *format, ...)
{
    int retcode;
    va_list ap;

    va_start( ap, format );
    retcode = vprintf( format, ap );
    va_end( ap );

    return retcode;
}

/**
 * \brief A shortened version of the standard sprintf.
 * The purpose of this routine is to output data the same as the standard printf 
 * function without the overhead most run-time libraries involve. Usually the 
 * printf brings in many kilobytes of code and that is unacceptable in most 
 * embedded systems.  This one only supports integer outputs, floating point
 * has been removed.
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] ctrl A standard printf formatting string.  
 * \param[in] ... The variable paramater list
 * \returns uint 
 * \retval The number of bytes in the output string
 */
int minSprintf(char* buf,  const char* format, ...)
{
    va_list ap;
    int retcode;

    va_start( ap, format );
    retcode = minVsprintf( buf, format, ap );
    va_end( ap );

    return retcode;
}

/**
 * \brief  like sprintf()
 *
 * See minPrintf() for description of what formats the scan code needs.
 *
 * Note! This function is dangerous! There is no range checking done on the
 * length of buf. We need to switch to an 'snprintf' clone instead.
 *
 * \author David Poole
 * \date 17-Jul-2008
 *
 */

int minVsprintf( char *buf, const char *format, va_list ap )
{
    int retcode;

    /* davep 02-Dec-2010 ; update to support the special case behavior of
     * current firmware. If the buffer is null, just do a printf.
     */
    if( buf ) {
        retcode = vsprintf( buf, format, ap );
    }
    else { 
        retcode = vprintf( format, ap );
    }

    return retcode;
}

