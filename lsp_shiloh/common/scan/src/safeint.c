/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"

/* XXX assuming a 32-bit integer */
#ifndef ULONG_MAX
    #define ULONG_MAX (~0)
#endif
#ifndef LONG_MAX
    #define LONG_MAX ((1<<31)-1)
#endif

uint32_t safeint_scale_uint32( uint32_t value, uint32_t numer, uint32_t denom )
{
//    dbg2( "%s %lu %lu %lu\n", __FUNCTION__, value, numer, denom );

    /* Check for overflow. Chapter 5, _Secure Coding in C and C++_ 
     * Robert C. Seacord  ISBN-10:0321335724
     *
     * Note I'm not checking for division underflow. Shame!
     */
    XASSERT( value < ULONG_MAX/numer, value );

    return (uint32_t)( (value * numer) / denom );
}

int safeint_scale_int( int value, int numer, int denom )
{
//    dbg2( "%s %ld %ld %ld\n", __FUNCTION__, value, numer, denom );

    /* Check for overflow. Chapter 5, _Secure Coding in C and C++_ 
     * Robert C. Seacord  ISBN-10:0321335724
     *
     * Note I'm not checking for division underflow. Shame!
     */
    XASSERT( value < LONG_MAX/numer, value );

    return (int)((value * numer)/denom);
}

uint32_t safeint_divide_with_round( uint32_t numer, int32_t denom )
{
    /* division with rounding up */

    /* well duh */
    ASSERT( denom > 0 );

    /* TODO add unsigned long long (uint64_t) to support big big numbers. For
     * now make sure we have the headroom to use the shifting 
     */
    XASSERT( (numer & (0xff<<24))==0, numer );
    XASSERT( (denom & (0xff<<24))==0, denom );

    return (((numer<<8) / (denom<<7))+1)>>1;
}

uint32_t safeint_divide_with_ceil( uint32_t numer, int32_t denom )
{
    /* want e.g. divide_with_ceil(4,2)==2 not 3 */
    if( numer%denom==0 ) {
        return safeint_divide_with_round( numer, denom );
    }

    return safeint_divide_with_round( numer+(denom-1), denom );
}

/**
 * \brief divide two 64-bit integers
 *
 * I'm hiding the Linux kernel specific do_div() so #ifdef won't be littered
 * across multiple files.
 *
 * \author David Poole
 * \date 05-Aug-2013
 */

uint64_t safeint_divide_uint64( uint64_t numer, uint64_t denom )
{
    uint64_t quotient;

    XASSERT( denom>0, denom );

#ifdef __KERNEL__
    /* davep 19-Apr-2013 ; do_div() is a macro; the result of the div is stored
     * in the first parameter. The macro returns the remainder.
     */
    do_div( numer, denom );
    quotient = numer;
#else
    quotient = numer / denom;
#endif

    return quotient;
}

