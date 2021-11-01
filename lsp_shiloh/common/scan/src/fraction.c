/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>

#include "lassert.h"

#include "fraction.h"
#include "safeint.h"

/**
 * \brief  
 *
 * Euclid's algorithm for finding greatest common divisor
 * _Algorithms in C_, Sedgewick 1990
 *
 * There are probably faster ways of doing this but not sure what the licenses
 * might be.
 *
 * \author David Poole
 * \date 06-Oct-2006
 *
 */

static int gcd( int u, int v ) 
{
    int t;

    while( u>0 ) {
        if( u<v ) {
            t = u;
            u = v;
            v = t;
        }
        u = u - v;
    }
    return v;
}

/**
 * \brief  reduce a fraction
 *
 * Make the scale factors smaller to reduce (but not eliminate!) the
 * possibility of overflow.
 *
 * The PIE XY Scaler firmware could still overflow if given a couple of huge
 * prime numbers.
 *
 * \author David Poole
 * \date 03-Oct-2006
 *
 */

void fraction_reduce_copy( const struct fraction *f_in, struct fraction *f_out )
{
    int divisor;

    divisor = gcd( f_in->numer, f_in->denom );

    XASSERT( f_in->numer%divisor==0, divisor );
    XASSERT( f_in->denom%divisor==0, divisor );

    f_out->numer /= divisor;
    f_out->denom /= divisor;
}

void fraction_reduce( struct fraction *f_inout )
{
    struct fraction f;

    f.numer = f_inout->numer;
    f.denom = f_inout->denom;

    fraction_reduce_copy( &f, f_inout );
}

void fraction_multiply( struct fraction *a, struct fraction *b, struct fraction *result )
{
    result->numer = safeint_scale_int( a->numer, b->numer, 1 );
    result->denom = safeint_scale_int( a->denom, b->denom, 1 );
    fraction_reduce( result );
}

