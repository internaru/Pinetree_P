/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "mbuffer.h"

scan_err_t mbuffer_new( struct mbuffer *buf, int max )
{
    buf->max = 0;
    buf->len = 0;

    memset( buf, 0, sizeof(struct mbuffer) );

    //buf->buf = (char *)malloc( max+1 );
    buf->buf = (char *)malloc( max+2 );	// mod.lsh.to fix Marvell Bug by Ryu
    if( buf->buf==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }
    buf->max = max;
    memset( buf->buf, 0, buf->max );
    return SCANERR_NONE;
}

void mbuffer_free( struct mbuffer *buf )
{
    if( buf->buf ) {
        free( buf->buf );
        buf->buf = NULL;
    }
    buf->len = 0;
    buf->max = 0;
}

scan_err_t mbuffer_realloc( struct mbuffer *buf )
{
    char *new_buf;

    /* double the max */
    new_buf = (char *)realloc( buf->buf, buf->max*2 );
    if( new_buf==NULL ) {
        dbg1( "%s failed to allocate bytes=%d\n", __FUNCTION__, buf->max*2 );
        return SCANERR_OUT_OF_MEMORY;
    }

    buf->buf = new_buf;
    buf->max = buf->max*2;

    return SCANERR_NONE;
}

