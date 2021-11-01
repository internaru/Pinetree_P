/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

#ifndef MBUFFER_H
#define MBUFFER_H

struct mbuffer {
    char *buf;
    int len;
    int max;
};

scan_err_t mbuffer_new( struct mbuffer *buf, int max );
void mbuffer_free( struct mbuffer *buf );
scan_err_t mbuffer_realloc( struct mbuffer *buf );

#endif

