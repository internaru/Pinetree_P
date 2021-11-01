/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2011-2013 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file bstream_filter_flate_decode
 *
 */

#include <stdint.h>

#include "bstream_filter_flate.h"
#include "memAPI.h"
#include "zlib.h"

typedef struct bstream_filter_flate_s bstream_filter_flate_t;
struct bstream_filter_flate_s
{
    bstream_t *stream;
    int32_t smax;
    z_stream zstream;
};

bstream_t *
bstream_filter_flate_open( bstream_t *s )
{
    int err;
    bstream_filter_flate_t *flate = (bstream_filter_flate_t *) MEM_MALLOC( sizeof(bstream_filter_flate_t) );

    flate->stream = s;
    s->state = flate;
    flate->smax = -1;
    flate->zstream.zalloc = Z_NULL;
    flate->zstream.zfree = Z_NULL;
    flate->zstream.opaque = Z_NULL;
    
    err = inflateInit( &flate->zstream );
    if (Z_OK != err)
        return 0;
    s->close = bstream_filter_flate_close;
    s->decode = bstream_filter_flate_decode;
    return s;
}

void
bstream_filter_flate_close( bstream_t *s )
{
    int err;
    ASSERT(s);
    bstream_filter_flate_t *flate = s->state;

    err = inflateEnd( &flate->zstream );
	if ( Z_OK != err)
        ; // printf???

    memFree(flate);
    s->state = 0; 
}

/// fills output buffer 
/// return dest_len bytes output to dest
int32_t bstream_filter_flate_decode( 
    bstream_t *s, 
    uint8_t *dest, 
    uint32_t dest_len, //< output length 
    int32_t smax //< input stream max
    )
{
    bstream_filter_flate_t *flate = s->state;
    z_streamp z = &flate->zstream; 
    int err;
    uint8_t *ptr;
    
    z->next_out = dest;
    z->avail_out = dest_len;

    if ( flate->smax <= 0 )
    {
        flate->smax = smax;
    }

    do 
    {
        if ( s->have )
        {
            smax = s->have;
        }
        else
        {
            smax = STREAM_BUFFER_SIZE; 
        }
        err = bstream_read( s, &ptr, smax );
        if ( 0 >= err ) // && s->have == 0 )
            return -1 ; // EOF on input 
        flate->smax -= smax < err ? smax : err;
        z->next_in = ptr; 
        z->avail_in = err; 

        err = inflate( z, Z_SYNC_FLUSH );
        if ( z->avail_in ) // didn't consume all the input data put it back.
        {
            s->cur -= z->avail_in;
            s->have += z->avail_in;
            flate->smax += z->avail_in;
        }
        if (err) 
        {
            DBG_PRINTF_NOTICE( "%s: smax %d total %d err %d have %d in %d out %d \n", __FUNCTION__, 
                               smax, flate->smax, err, s->have, z->avail_in, dest_len - z->avail_out );   
        }

    } while ( z->avail_out && err == Z_OK  /*&& flate->smax > 0*/ ); // fill entire output buffer 
        
    return dest_len - z->avail_out;
}


