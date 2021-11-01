/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef FRACTION_H
#define FRACTION_H

struct fraction {
    int numer;
    int denom;
};

void fraction_reduce_copy( const struct fraction *f_in, struct fraction *f_out);

void fraction_reduce( struct fraction *f_inout );

void fraction_multiply( struct fraction *a, struct fraction *b, struct fraction *result );

#endif

