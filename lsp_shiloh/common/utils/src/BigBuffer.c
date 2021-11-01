/** 
 * $Header: $
 *
 * ============================================================================
 * Copyright (c) 2008   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 * 
 * Description: 

 Handle to a big allocation allows subclasses to have different 
 behaviours caching, static buffers etc. 
 Base class does malloc free behavior. 
              
 * 
 **/
#include "list.h"
#include "BigBuffer.h"
#include "memAPI.h"
#include "lassert.h"
#ifndef __linux__
#include "cpu_api.h"
#endif

// Base class destructor.
// returns 0; usage: ptr = free(ptr); 
static BigBuffer_t *BigBuffer_FreeFunc(BigBuffer_t *bigBuffer)
{
    if (bigBuffer) 
    {
        ASSERT(bigBuffer && bigBuffer->freeFunc);
        ASSERT(bigBuffer->listNode.prevEntry == 0);
        ASSERT(bigBuffer->listNode.nextEntry == 0);
        if (bigBuffer->data != NULL) 
        {
            memFree(bigBuffer->data);
        }
        bigBuffer->data = NULL;
        memFree(bigBuffer);
    }
    return 0;
}

// Constructor 
// allocates a big buffer of len bytes
// returns null on alloc failure.
//

BigBuffer_t *BigBuffer_Malloc(mlimiter_t *limiter, int32_t len)
{
    if (!limiter)
        limiter = mlimiter_by_name("print");

    BigBuffer_t *bb = (BigBuffer_t *)MEM_MALLOC_LIMITED(limiter, sizeof(BigBuffer_t));
    if (bb)
    {
        ASSERT( len > 0 );

        bb->data = (uint8_t*)MEM_MALLOC_ALIGN_LIMITED(limiter, len, 
                                                      cpu_get_dcache_line_size());
        bb->datalen = len;

        bb->freeFunc = BigBuffer_FreeFunc;
        ATInitNode(&bb->listNode);
        bb->dma_alloc = 0;
        if (bb->data == 0) // malloc failure
        {
            memFree(bb);
            bb = 0;
        }
    }
    return bb;
}

// Destructor
// returns 0; usage: ptr = free(ptr); 
BigBuffer_t *BigBuffer_Free(BigBuffer_t *bigBuffer)
{
    ASSERT(bigBuffer && bigBuffer->freeFunc);

    // require the buffer not be in a list when you are freeing it.
    // having pointers to freed memory is a bad thing.
    ASSERT(bigBuffer->listNode.prevEntry == 0);
    ASSERT(bigBuffer->listNode.nextEntry == 0);
    return bigBuffer->freeFunc(bigBuffer);
}

BigBuffer_t *BigBuffer_adopt(void *data, int32_t len)
{
    BigBuffer_t *bb = (BigBuffer_t *) MEM_MALLOC(sizeof(BigBuffer_t));

    if (bb)
    {
        bb->data = (uint8_t*)data;
        bb->datalen = len;
        bb->freeFunc = BigBuffer_FreeFunc;
        ATInitNode(&bb->listNode);
    }
    return bb;   
}

BigBuffer_t *BigBuffer_realloc( BigBuffer_t *bb, int32_t len )
{
    if ( bb ) 
    {
        bb->data = MEM_REALLOC( bb->data, len );
        bb->datalen = len;
    }
    return bb;
}
