/* 
 * ============================================================================
 * Copyright (c) 2008-2012  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file memAPI.c
 * 
 * \brief This file contains source code implementing the memory manager API.
 *
 **/
#include <lassert.h>
#include <string.h>
#include <stdlib.h>
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "utils.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#define DBG_PRFX "mem: "    
#define LOGGER_SUBMOD 0x40000000   
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMOD
#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__) 
#define DBG_WARN(...) DBG_PRINTF(LOG_WARNING, DBG_PRFX __VA_ARGS__) 

// 4 pointer size header. 
#define alignsize (sizeof(void *) * 4)
#define DBG_MEM_SIZE_BYTES alignsize

mlimiter_t *sysLimiter = 0; 
mlimiter_t *small_limiter = 0;  ///< limiter for 64byte or smaller allocations, has no "extra" debug data

#ifndef INC_CPU_H
uint32_t cpu_get_dcache_line_size(  )
{
  return 32 ; // ( alignsize );
}
#endif

void memFree(void *address)
{
    uintptr_t align;
    uintptr_t size;
    mlimiter_t *lim;
    uintptr_t small_size;
  
    if (address) {
        small_size = *(uintptr_t*)(((uintptr_t)address) - sizeof(void*)) & 0xfffffffc;
        if ( small_size <= 64 ) {  // remove dlmalloc tracking size.
            mlimiter_track_sub( small_limiter, small_size );
            DBG_PRINTF_NOTICE("--- %8d 0x%08x small %d\n", small_size, address, small_limiter->current);
            free(address);
            return;
        }
    }
    if (address) {
        align = *(uintptr_t*)((uintptr_t)address - sizeof(void*));
        align &= 0x0000ffff;
        if ((align & (align-1)) == 0 && align >= alignsize ) // if not power of 2 then it must not be from memAPI
        {
            lim = (mlimiter_t*)*(uintptr_t*)(((uintptr_t)address) - (sizeof(void*)*2));
            size = *(uintptr_t*)(((uintptr_t)address) - (sizeof(void*)*3)) & 0xfffffffc;
#ifdef DEBUG // compiler warnings on unused variables make for ugly code.
            if (DBG_WOULD_PRINTF(LOG_DEBUG)) {
                char *file         =    (char*)*(uintptr_t*)(((uintptr_t)address) - (sizeof(void*)*4));
                uint32_t line = (uint32_t)*(uintptr_t*)(((uintptr_t)address) - (sizeof(void*)));
                DBG_PRINTF_DEBUG("--- %8d 0x%08x 0x%08x %8d %s:%d %d\n", 
                                 size, address, lim, lim->current, file, line>>16, align);
            }
#endif
            mlimiter_track_sub( lim, size );
            address -= align;
        }
        free(address);
    }
}

// returns user pointer exactly 1 * align after the actual allocated pointer
static void *memIdAlloc( void* ptr, 
                         mlimiter_t *mlimiter, 
                         int size, 
                         int align, 
                         const char *file, 
                         const int line)
{
    uintptr_t *pMem;

    if(ptr) 
    {
        // use real size from allocation.
        size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
        align = align < alignsize ? alignsize : align;
        pMem = (uintptr_t*)(((uintptr_t)ptr + align) -  alignsize);
        *pMem++ = (uintptr_t)file;
        *pMem++ = (uintptr_t)size;
        *pMem++ = (uintptr_t)mlimiter;
        *pMem++ = (line << 16) | align;
        ptr += align;
        DBG_PRINTF_DEBUG("+++ %8d 0x%08x 0x%08x %8d %s:%d \n", size, ptr, mlimiter, mlimiter->current, file, line);
        ASSERT( (void*)pMem == ptr ); 
    }
    return ptr;  // pointer to user portion of allocation.
}

#define memCallocLimited(limiter, count, size)  \
  memMallocLimited_imp( limiter, ((count * size) + DBG_MEM_SIZE_BYTES), 1, 0 )
#define memCallocAlignLimited(limiter, count, size, align) \
  memMallocLimited_imp( limiter, ((count * size) + DBG_MEM_SIZE_BYTES), 1, align )
#define memMallocLimited(limiter, size) \
  memMallocLimited_imp( limiter, size + DBG_MEM_SIZE_BYTES, 0, 0 )
#define memMallocAlignLimited(limiter, size, align)                           \
  memMallocLimited_imp( limiter, size + DBG_MEM_SIZE_BYTES, 0, align )

static void *memMallocLimited_imp(mlimiter_t *limiter, uint32_t size, bool clear, size_t align)
{
    void *ptr = 0;

    if (0 == limiter)
    {
        limiter = sysLimiter;
    }
    if ( limiter->active && size < mlimiter_available( limiter ) )
    {
        if (clear) {
            if (align) {
                if ( posix_memalign( &ptr, align, size ) ) {
                    ptr = 0;
                } else {
                    memset( ptr, 0, size);
                }
            } else  {
//                ptr = calloc( 1, size );  //lsptodo: retest calloc.
                ptr = malloc( size );
                memset( ptr, 0, size);
            }
        } else { 
            if (align) {
                if ( posix_memalign( &ptr, align, size ) )
                    ptr = 0;
            } else {
                ptr = malloc( size );
            }
        }
        if ( ptr ) 
        {
            size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
            mlimiter_track_add( limiter, size );
        }
    }
    return ptr;
}



void *mem_retry_malloc_align_limited(
    mlimiter_t *lim, 
    int size, 
    int align, 
    const char *file, const int line)
{
    void *ptr = 0;
    bool again = false;

    if (lim == 0)
        lim = sysLimiter;

    int extra =  DBG_MEM_SIZE_BYTES; 
    while (1)     
    {
        if (align > alignsize) {
            ptr = memMallocAlignLimited(lim, size + extra, align);
        } 
        else if (size <= 64 - 8) { // include dlmalloc 2words
            ptr = malloc(size);
            size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
            mlimiter_track_add( small_limiter, size );
            DBG_PRINTF_NOTICE("+++ %8d 0x%08x small %s:%d lim %d\n", size, ptr, file, line, small_limiter->current);
            return ptr;
        }
        else {
            ptr = memMallocLimited(lim, size + extra);
        }
     
        if ( ptr || !lim->strategy)
            break;

        again = (lim->strategy)(lim, size);

        if (!again) 
            break;
    }
    ptr = memIdAlloc(ptr, lim, size, align, file, line);
    return ptr;
}

void *mem_retry_calloc_align_limited(
    mlimiter_t *lim, 
    uint32_t cnt,
    int size, 
    int align, 
    const char *file, const int line)
{
    void *ptr = 0;
    bool again = false;

    if (lim == 0)
        lim = sysLimiter;

    if (size > 64)
        size += (size % alignsize) ? alignsize - (size % alignsize) : alignsize;

    while (1)     
    {
        if (align > alignsize)
            ptr = memCallocAlignLimited(lim, cnt, size, align);
        else if (size <= 64 - 8) { // include dlmalloc 2words
            ptr = malloc(size*cnt);
            memset(ptr, 0, size*cnt);
            size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
            mlimiter_track_add( small_limiter, size );
            DBG_PRINTF_NOTICE("+++ %8d 0x%x small %s:%d lim %d\n", size, ptr, file, line, small_limiter->current);
            return ptr;
        }
        else {
            ptr = memCallocLimited(lim, cnt, size);
            if (ptr)
                size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
        }
        if ( ptr || !lim->strategy)
            break;

        again = (lim->strategy)(lim, size);

        if (!again) 
            break;
    }
    ptr = memIdAlloc(ptr, lim, size, align, file, line);
    return ptr;
}

/// realloc from one limiter to another is not recommended.
/// realloc between the small_limiter and other limiters is prevented.
///
void *memReallocLimited(mlimiter_t *limiter, void *address, uint32_t size, const char *file, const int line)
{   
    void *ptr = 0;
    bool again = false;
    uintptr_t old_size = 0;
    uintptr_t requested_size = size;

    if (limiter == 0)
        limiter = sysLimiter;

    if ( 0 == size )
    {
       memFree( address );
    }
    else    
    {
        uintptr_t align;
        mlimiter_t *lim;
        size = size < 64 ? 64 : size; /// prevent small_limiter usage.

        if (address) 
        {
            old_size = *(uintptr_t*)(((uintptr_t)address) - sizeof(void*)) & 0xfffffffc;
            if ( old_size <= 64 ) {  // remove dlmalloc tracking size.
                mlimiter_track_sub( small_limiter, old_size );
                DBG_PRINTF_NOTICE("--- %8d 0x%08x small %d\n", old_size, address, small_limiter->current);
            }
            else // not from small limiter
            {
                align = *(uintptr_t *)((uintptr_t)address - sizeof(void *));
                align &= 0x0000ffff;
                if ((align & (align - 1)) == 0 && align >= alignsize) // if not power of 2 then it must not be from memAPI
                {
                    lim = (mlimiter_t *)*(uintptr_t *)(((uintptr_t)address) - (sizeof(void *) * 2));
                    old_size = *(uintptr_t *)(((uintptr_t)address) - (sizeof(void *) * 3));
                    mlimiter_track_sub(lim, size);
                    address -= align;
                }
            }
        }
        while (1)     
        {     
            if ( limiter->active && size < mlimiter_available( limiter ) )
                ptr = realloc(address, size + DBG_MEM_SIZE_BYTES);
            if ( ptr || !limiter->strategy)
                break;

            again = (limiter->strategy)(limiter, size);

            if (!again) 
                break;
        }
        if (ptr) 
        {
            size = (*(uintptr_t*)((uintptr_t)ptr - sizeof(void*))) & 0xfffffffc;
            mlimiter_track_add( limiter, size );
            if (old_size <= 64)
            {
                // old allocation did not include tracking info, so need to copy real data down past
                // where the new tracking info will be stamped; use memmove due to overlap of data
                memmove(((char*)ptr)+DBG_MEM_SIZE_BYTES, ptr, MIN(requested_size, old_size));
            }
        }
    }

    ptr = memIdAlloc(ptr, limiter, size, 0, file, line);
    return ptr;
}

// memMallocPhysical(pptr, sz, align, __FILE__, __LINE__)


/** 
 * \brief Used to create the main system heap at initialization time.
 * 
 * \param size (uint32_t) size of the heap, in bytes
 * 
 * \return void*
 * 
 * \retval address of the "mspace" representing the system heap
 * 
 **/
static mlimiter_t *memCreateSysHeap(uint32_t size)
{
    // Note that "create_mspace" returns type mspace, which is
    // typedef'ed to "void *". Turn on locking when creating
    // this heap.
    //sysHeap = create_mspace(size, 1);
    //ASSERT(sysHeap != 0);
    sysLimiter = mlimiter_by_name("syslimiter");
    ASSERT(sysLimiter);
    //sysLimiter->ms = sysHeap;
    posix_mutex_init( &sysLimiter->lock );
    sysLimiter->max = size;
    sysLimiter->highest_max = size;
    sysLimiter->strategy = 0;
    sysLimiter->active = 1;
    sysLimiter->current = 0;
    sysLimiter->high_water = 0;
    sysLimiter->parent = 0;

    ASSERT(size > sysLimiter->lowest_max);
    size -= sysLimiter->lowest_max;
    
    // initialize all the limiters.

    int i = mlimiter_index_by_name("sys_retry_forever");
    mlimiter_t *lim = mlimiter_by_index(i);
    for (;lim ;  lim = mlimiter_by_index(++i))
    {
      //lim->ms = sysHeap; 
        posix_mutex_init( &lim->lock );
        lim->max = size > lim->max ? lim->max : size;
        lim->highest_max = size > lim->highest_max ? lim->highest_max : size;
        lim->parent = sysLimiter;
    }
    small_limiter = mlimiter_by_name("small_limiter");
    mlimiter_start( mlimiter_by_name("small_limiter"));
    mlimiter_start( mlimiter_by_name("sys_retry_forever"));
    
    return sysLimiter;
}

/* FUNCTION NAME: memGetHeapBiggestAllocAvailable */
 
/** 
 * \brief Return size of largest block of free memory on the specified heap. If 
 * a NULL pointer is provided, the system heap free memory will be returned.
 * 
 * \param heap
 * 
 * \return uint32_t
 * 
 * \retval Largest block of free memory
 **/
uint32_t memGetHeapBiggestAllocAvailable(mlimiter_t *heap)
{
    if (heap == 0)
    {
        return sysLimiter->max - sysLimiter->current;
    }
    else
    {
        return heap->max - heap->current;
    }
}

void memInitMemory(void *memoryStartAddr, uint32_t memSize)
{
    // initialize the sbrk or mmap 
    // mv_sbrk_init(memoryStartAddr, memSize);
  memCreateSysHeap(memSize); /// - memGetMemMgrHeapOverhead());
}
void memInitUncached(void *memoryStartAddr, uint32_t memSize)
{
  ASSERT(0 == 1 );
}



static int memory_cmd_limit_cb( int argc, char *argv[] )
{
    int i;
    mlimiter_t *lim;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    i = 0;
    lim = 0;

    for (i = 0, lim = mlimiter_by_index(i); lim ; ++i, lim = mlimiter_by_index(i))
    {
        dbg_mlimiter_print( lim );
    }

    return CMD_OK;
}

static int memory_cmd_test_lim_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
#ifdef HAVE_UNIT_TEST
    extern void mlimiter_test(void);
    mlimiter_test();
    extern void dbg_new_memapi_test( void );
    dbg_new_memapi_test();
#else
    return CMD_ERROR;
#endif

    return CMD_OK;
}  

static int memory_cmd_relevel_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    mlimiter_relevel_memory( mlimiter_by_name("syslimiter") );

    return CMD_OK;
}

void memory_cmd_init(void)
{
#ifdef HAVE_CMD
    int cmd_res = OK;  UNUSED_VAR(cmd_res);
    cmd_res = cmd_register_cmd( "memory",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "memory",
                                   "limit",
                                   "Display limiter information",
                                   0,
                                   0,
                                   memory_cmd_limit_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "memory",
                                   "relevel",
                                   "Run the limiter releveling routine",
                                   0,
                                   0,
                                   memory_cmd_relevel_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "memory",
                                   "testlim",
                                   "Run the limiter memory test",
                                   0,
                                   0,
                                   memory_cmd_test_lim_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
#if 0
    /* initialize peek, poke cmds */
    extern void mem_peek_poke_init( void );
    mem_peek_poke_init();
#endif
#endif
}

