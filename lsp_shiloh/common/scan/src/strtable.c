/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file strtable.c
 *
 * \brief Table of null terminated strings in memory. Used by scands.
 *
 **/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "lassert.h"
#include "memAPI.h"
#include "utils.h"

#include "scantypes.h"
#include "scancore.h"
#include "strtable.h"

#define BUFFER_POISON '!'

scan_err_t string_table_new( struct string_table *tbl, int num_bytes, int max_string_length )
{
    memset( tbl, 0, sizeof(struct string_table) );

    tbl->buf = (char *)MEM_MALLOC( num_bytes );
    if( tbl->buf==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    tbl->maxlen_bytes = num_bytes;
    memset( tbl->buf, BUFFER_POISON, tbl->maxlen_bytes );

    tbl->max_string_length = max_string_length;

    return SCANERR_NONE;
}

void string_table_free( struct string_table *tbl )
{
    /* poison the memory */
    memset( tbl->buf, BUFFER_POISON, tbl->maxlen_bytes );

    PTR_FREE( tbl->buf );

    memset( tbl, 0, sizeof(struct string_table) );
}

scan_err_t string_table_copy( struct string_table *dst,
                              struct string_table *src )
{
    if( dst->maxlen_bytes < src->maxlen_bytes ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    memcpy( dst->buf, src->buf, src->maxlen_bytes );
    dst->currlen_bytes = src->currlen_bytes;

    return SCANERR_NONE;
}

scan_err_t string_table_add_string( struct string_table *tbl,
                                    const char *s, 
                                    char **new_ptr )
{
    int len;
    char *endptr;

    /* +1 to check for exceeding name length */
    len = strnlen( s, tbl->max_string_length+1 );
    if( len > tbl->max_string_length ) {
        return SCANERR_INVALID_PARAM; 
    }

    /* need one extra to store NULL */
    len += 1;

    if( tbl->currlen_bytes + len > tbl->maxlen_bytes ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    endptr = tbl->buf + tbl->currlen_bytes;

    memcpy( endptr, s, len );

    tbl->currlen_bytes += len;

    *new_ptr = endptr;

    return SCANERR_NONE;
}

