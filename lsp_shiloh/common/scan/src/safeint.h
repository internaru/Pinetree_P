/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SAFEINT_H
#define SAFEINT_H

uint32_t safeint_scale_uint32( uint32_t value, uint32_t numer, uint32_t denom );
int safeint_scale_int( int value, int numer, int denom );

uint32_t safeint_divide_with_round( uint32_t numer, int32_t denom );
uint32_t safeint_divide_with_ceil( uint32_t numer, int32_t denom );

uint64_t safeint_divide_uint64( uint64_t numer, uint64_t denom );

#endif

