/******************************************************************************
 * Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/// PDF RunLengthDecode decode filter. 
///

#include "bstream_filter_rle.h"
#include "memAPI.h"
#include "lassert.h"
#include "string.h" // memset
#include "logger.h"

// Config the printf's from logger.h 
#define DBG_PRFX "bstream_rle: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 7 )



/// RunLengthDecode state object wraps the stream underneath.
/// bstream->state = wrapping stream object
/// bstream->state->stream == bstream wrapped object.
typedef struct bstream_filter_rle_s bstream_filter_rle_t;
struct bstream_filter_rle_s
{
    bstream_t *stream; //< base stream we are "attached" to.
    int32_t run;     //< initial run "cnt"
    int32_t val;     //< value for repeat counts
    int32_t count;   //< cnt remaining
    int32_t smax;    //< max to read from stream
};


bstream_t *
bstream_filter_rle_open( bstream_t *s )
{
    bstream_filter_rle_t *rle = (bstream_filter_rle_t *) MEM_MALLOC( sizeof(bstream_filter_rle_t) );
    memset(rle, 0, sizeof(bstream_filter_rle_t));
    ASSERT(s);
    rle->stream = s;
    s->state = rle;
    s->close = bstream_filter_rle_close;
    s->decode = bstream_filter_rle_decode;
    return s;
}

void
bstream_filter_rle_close( bstream_t *s )
{
    ASSERT(s);
    bstream_filter_rle_t *rle = s->state;

    memFree(rle);
    s->state = 0;

}

/// negative on error, premature end of stream
/// positive is output byte count placed at dest
///
int32_t
bstream_filter_rle_decode( bstream_t *s, uint8_t *dest, uint32_t dest_len, int32_t smax )
{
    bstream_filter_rle_t *rle = s->state;
    uint8_t *cur = dest;
    uint8_t *end = &dest[dest_len];

    rle->smax = smax;


    // output limited to destination buffer size.
    while (cur < end && rle->smax > 0)
    {
        if ( 0x80 == rle->run ) // eof exit
        {
            DBG_PRINTF_INFO("RLE: 0x80 eof in rle decode cur %p dest %p len %d remain %d smax\n", 
                            cur, dest, cur-dest, end - cur, rle->smax);
            break;
        }
        else if ( 0 == rle->count ) // did last run lenght do next one.
        {
            rle->run = bstream_readbyte(s);
            rle->smax--;
            if ( 0 > rle->run ) // save eof exit on input read is short
                rle->run = 128;
            else if ( 128 > rle->run ) // save new copy count.
                rle->count = rle->run + 1;
            else if ( 128 < rle->run ) // save byte to repeat count times.
            {
                if (0 > (rle->val = bstream_readbyte(s)))
                {
                    DBG_PRINTF_ERR("eof in rle decode cur %p dest %p %d\n", cur, dest, cur-dest);
                    return -1;  
                }
                rle->smax--;
                rle->count = 257 - rle->run; 
            }
        }

        if ( 128 > rle->run ) // do copy 
        {
            uint8_t *ptr;
            int bytes = MIN( rle->count, end - cur );
            XASSERT( bytes <= STREAM_BUFFER_SIZE, bytes );
            bstream_read( s, &ptr, bytes );
            memcpy( cur, ptr, bytes );
            rle->smax -= bytes;
            rle->count -= bytes;
            cur += bytes;
        }
        else if (128 < rle->run ) // do run 
        {
            int bytes = MIN( rle->count, end - cur );
            memset( cur, rle->val, bytes );
            rle->count -= bytes;
            cur += bytes;
        }
    }

    return cur - dest;  // number of bytes output
}
