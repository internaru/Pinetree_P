/******************************************************************************
 * Copyright (c)2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "bstream.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "commonio.h"
#include "lassert.h"

bstream_t *bstream_construct( IOReg *pipe )
{
    bstream_t * stream;
    
    stream = (bstream_t *) MEM_MALLOC(sizeof(bstream_t));
    if (stream) 
    {
        ASSERT(pipe);
        stream->state = 0;
        stream->Pipe = pipe;
        stream->cur = &stream->buffer[0];
        stream->have = 0;
        stream->fpos = 0;
        stream->eof = false;
    }
    return stream;
}

bstream_t *bstream_close( bstream_t *stream )
{
    stream->Pipe->pipe_read_rewind(stream->Pipe, stream->cur, stream->have);
    memFree(stream);
    return NULL;
}

int bstream_readbyte_no_timeout( bstream_t *bstr )
{
    DBG_ASSERT(0 == "function cannot be used to find end of stream using zero length timeouts");

    bstr->fpos++;
    if (bstr->have == 0) {
        bstr->cur = &bstr->buffer[0];
        bstr->have = bstr->Pipe->pipe_read(bstr->Pipe, bstr->cur, STREAM_BUFFER_SIZE, PARSEREAD_ANY);
        if (bstr->have <= 0 ) {
	    bstr->have = 0;
            return 0;
        }
    }

    int c = *bstr->cur;
    bstr->cur++;
    bstr->have--;
    return c; 
}

void bstream_rewind( bstream_t *bstr )
{
    if (bstr->have)
    {
        bstr->Pipe->pipe_read_rewind(bstr->Pipe, bstr->cur, bstr->have);
	bstr->fpos -= bstr->have;
	bstr->have = 0; 
	bstr->cur = &bstr->buffer[0];
    }
}


void bstream_unreadbyte( bstream_t *bstr )
{
    if (bstr->cur > bstr->buffer)
    {
        bstr->cur--;
        bstr->have++;
    }
    else
    {
        return; // how do we un read what we don't have?
        // bstr->cur = &bstr->buffer[0];
        // bstr->have = 0;
    }

    bstr->fpos--;
        
}

int bstream_readbyte( bstream_t *bstr )
{
    bstr->fpos++;
    if (bstr->have == 0) {
	if ( bstr->eof == true ) {
	    return myEOF;
	}
        bstr->cur = &bstr->buffer[0];
        bstr->have = bstr->Pipe->pipe_read(bstr->Pipe, bstr->cur, STREAM_BUFFER_SIZE, PARSEREAD_SOME);
        if (bstr->have <= 0) {
            bstr->have = 0;
            bstr->eof = true;                   
            return myEOF;
        }
        bstr->eof = false;                   
    }

    int c = *bstr->cur;
    bstr->cur++;
    bstr->have--;
    return c; 
}

int bstream_read( bstream_t *bstr, uint8_t **ptr, uint32_t inlen )
{
    int bytes_read;
    int32_t len = (int32_t)inlen;
    int32_t total_bytes_read;

    // cap the read length so we don't write where we're not supposed to
    if(len > STREAM_BUFFER_SIZE)
    {
        len = STREAM_BUFFER_SIZE;
    }

    if ( len <= bstr->have )
    {
        *ptr = bstr->cur;
        bstr->cur += len;
        bstr->have -= len;
    }
    else
    {
        if (bstr->have) {
            bstr->Pipe->pipe_read_rewind(bstr->Pipe, bstr->cur, bstr->have);
	} else if ( bstr->eof == true ) {
	    return myEOF;
	}

        bstr->cur = &bstr->buffer[0];
        bytes_read = bstr->Pipe->pipe_read(bstr->Pipe, bstr->cur, STREAM_BUFFER_SIZE , PARSEREAD_SOME);
        total_bytes_read = bytes_read > 0 ? bytes_read : 0;
	bstr->eof = (total_bytes_read == 0);

        while (bytes_read > 0 && total_bytes_read < len) //Did enough data come in for request.
	{
            bytes_read = bstr->Pipe->pipe_read(bstr->Pipe, bstr->cur + total_bytes_read, //Hope for to fill the buffer, but don't need to
					       STREAM_BUFFER_SIZE - total_bytes_read, PARSEREAD_SOME);
            if (bytes_read <= 0) //I/O timeout
            {
                bstr->eof = true;
            }
            else //Data has come in
            {
                total_bytes_read += bytes_read;
            }
        }

        *ptr = bstr->cur;
	bstr->have = total_bytes_read;
        if (total_bytes_read >= len )
        {    
            bstr->cur += len;
            bstr->have -= len;
        }
        else // short read 
        {
            bstr->cur += total_bytes_read;
            bstr->have -= total_bytes_read;
            return total_bytes_read;
        }
    }    
    return len;
}
