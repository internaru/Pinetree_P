#ifndef MV_BSTREAM_H
#define MV_BSTREAM_H
/******************************************************************************
 * Copyright (c)2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/


#include <stdbool.h>
#include <stdint.h>

// #include "ioutils.h"

#define STREAM_BUFFER_SIZE 1024*2
#define myEOF 0xf00d0000

#ifndef UNITY // Non Unity code bases need this header file for the IOReg definition
#include "agConnectMgr.h"
#endif

typedef struct bstream_s bstream_t;

#ifndef IOReg_DEFINED
#  define IOReg_DEFINED
typedef struct IOStruct_s IOReg;
#endif

struct bstream_s 
{
    void *state; //< attach a filter here.
    IOReg *Pipe; //< pipe to read from
    uint8_t *cur; //< current pointer into buffer.
    int32_t have; //< number of bytes from cur to end of read in data.
    uint32_t fpos; //< "file" pos of *cur
    bool eof;
    int32_t (*decode)( bstream_t *s, uint8_t *dest, uint32_t dest_len, int32_t smax );
    void (*close)( bstream_t *s );
    uint8_t buffer[STREAM_BUFFER_SIZE]; 
};


bstream_t *bstream_construct( IOReg *pipe );
bstream_t *bstream_close( bstream_t *stream );
void bstream_unreadbyte( bstream_t *bstr );
int bstream_readbyte( bstream_t *bstr );
int bstream_readbyte_no_timeout( bstream_t *bstr );
int bstream_read( bstream_t *bstr, uint8_t **ptr, uint32_t inlen );
void bstream_rewind( bstream_t *bstr );

#endif // eof
