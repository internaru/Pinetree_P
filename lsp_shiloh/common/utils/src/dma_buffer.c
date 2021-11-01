/*
 * ============================================================================
 * Copyright (c) 2012-2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include "dma_buffer.h"
#include "dma_alloc_api.h"
#include "memAPI.h"

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

BigBuffer_t *dma_buffer_freefunc( BigBuffer_t *bb )
{
    if ( bb ) 
    {
        memFree( bb->data );
        if (bb->dma_alloc) 
        {
            if (bb->dma_alloc->direction < 0 ) // recover from missing unmap calls by user. 
            {
                dma_alloc_unmap_single( bb->dma_alloc, bb->dma_alloc->direction * -1 );
            }
            dma_free( bb->dma_alloc );
            memFree( bb->dma_alloc );
        }
        memFree( bb );
  }
  return 0;  
}

BigBuffer_t *BigBuffer_convert_to_dma_buffer( BigBuffer_t *bb )
{
  if (bb->dma_alloc == 0)
  {
    bb->dma_alloc = dma_alloc( bb->datalen );
    dma_alloc_write( bb->dma_alloc, bb->data, bb->datalen );
    memFree( bb->data );
    bb->data = 0;
    bb->freeFunc = dma_buffer_freefunc;
  }
  return bb;
}

BigBuffer_t *dma_buffer_malloc( mlimiter_t *limiter, int32_t len )
{
  ASSERT(len > 0);  // lengths of 0 do "bad things" later when passed to routines like dma_map_single

  if (!limiter)
    limiter = mlimiter_by_name("print");

  BigBuffer_t *bb = (BigBuffer_t *)MEM_MALLOC_LIMITED(limiter, sizeof(BigBuffer_t));
  if (bb)
    {
      bb->datalen = len;
      bb->dma_alloc = dma_alloc( bb->datalen );
      bb->data = 0;
      bb->freeFunc = dma_buffer_freefunc;
      ATInitNode(&bb->listNode); 
    }
  return bb;
}

BigBuffer_t *dma_buffer_adopt( void *data, int32_t len )
{  
  mlimiter_t *limiter = mlimiter_by_name("print");
  BigBuffer_t *bb = (BigBuffer_t *)MEM_MALLOC_LIMITED(limiter, sizeof(BigBuffer_t));
  if (bb)
  {
      bb->dma_alloc = dma_alloc( len );
      bb->data = 0;
      bb->datalen = len;
      bb->freeFunc = dma_buffer_freefunc;
      ATInitNode(&bb->listNode); 
      dma_alloc_write( bb->dma_alloc, data, len );
      memFree( data );
  }
  return bb;
}

// This doesn't free the original.
BigBuffer_t *dma_buffer_copy_from( void *data, int32_t len )
{  
  mlimiter_t *limiter = mlimiter_by_name("print");
  BigBuffer_t *bb = (BigBuffer_t *)MEM_MALLOC_LIMITED(limiter, sizeof(BigBuffer_t));
  if (bb)
  {
      bb->dma_alloc = dma_alloc( len );
      bb->data = 0;
      bb->datalen = len;
      bb->freeFunc = dma_buffer_freefunc;
      ATInitNode(&bb->listNode); 
      dma_alloc_write( bb->dma_alloc, data, len );
  }
  return bb;
}

BigBuffer_t *dma_buffer_realloc( BigBuffer_t *bb, int32_t len )
{
    if ( bb && bb->data )
    {
        int32_t copy = MIN( bb->datalen, len );

        bb->dma_alloc = dma_alloc( len );
        bb->datalen = len;
        dma_alloc_write( bb->dma_alloc, bb->data, copy );
        memFree(bb->data);
        bb->data = 0;
    }
    else if ( bb && bb->dma_alloc )
    {
        dma_alloc_t *nda = dma_alloc( len );
        ASSERT(len <= bb->datalen);
        int32_t copy = MIN( bb->datalen, len );
        dma_alloc_mmap_forcpu( bb->dma_alloc );
        dma_alloc_write( nda, bb->dma_alloc->v_addr, copy );
        dma_alloc_unmmap_forcpu( bb->dma_alloc );
        memFree( dma_free( bb->dma_alloc ) );
        bb->dma_alloc = nda;
        bb->datalen = len;
    }
    return bb;
}

void *dma_buffer_map_single( BigBuffer_t *bb, int direction )
{
    if ( bb->data )
    {
        ASSERT( bb->data == 0 ); 
    }
    else
    {
        dma_alloc_map_single( bb->dma_alloc, direction );   
    }
    return (void *)bb->dma_alloc->hw_addr;
}

void *dma_buffer_unmap_single( BigBuffer_t *bb, int direction )
{
    if ( bb->data )
    {
        ASSERT( bb->data == 0 ); 
    }
    else
    {
        dma_alloc_unmap_single( bb->dma_alloc, direction );   
    }
    return (void *)bb->dma_alloc->hw_addr;
}

void * dma_buffer_mmap_forcpu( BigBuffer_t *bb )
{
    if ( bb->data )
        return bb->data;
    ASSERT( bb->dma_alloc );
    dma_alloc_mmap_forcpu( bb->dma_alloc );
    return bb->dma_alloc->v_addr;
}

void * dma_buffer_unmmap_forcpu( BigBuffer_t *bb )
{
  if ( bb->data == 0 && bb->dma_alloc )
    dma_alloc_unmmap_forcpu( bb->dma_alloc );
  return 0;
}
