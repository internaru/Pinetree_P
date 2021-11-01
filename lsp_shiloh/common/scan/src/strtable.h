/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef STRTABLE_H
#define STRTABLE_H

struct string_table {
    char *buf;

    /* current number of bytes in the table (includes string NULLs) */
    int currlen_bytes;

    /* maximum number of bytes in the table (includes string NULLs) */
    int maxlen_bytes;

    /* maximum size of string stored in the table (required nonzero for buffer
     * range checks in string_table_add_string()
     */
    int max_string_length;
};

scan_err_t string_table_new( struct string_table *tbl, int num_bytes, int max_string_length );
void string_table_free( struct string_table *tbl );
scan_err_t string_table_copy( struct string_table *dst,
                              struct string_table *src );
scan_err_t string_table_add_string( struct string_table *tbl,
                                    const char *s, 
                                    char **new_ptr );

#endif

