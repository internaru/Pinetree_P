/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>

#include <linux/slab.h>
#include <linux/spinlock.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "list.h"
#include "memAPI.h"
#include "cpu_api.h"
#include "LCM_regstructs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanmem.h"
#include "notaheap.h"

//#define SCANMEM_DEBUG

#ifdef SCANMEM_DEBUG
#define scanmem_dbg2 dbg2
#else
#define scanmem_dbg2(...)
#endif

/* davep 29-Mar-2013 ; lets try CISX in LCM */
static volatile LCM_REGS_t * const lcm_regs = (volatile LCM_REGS_t *)LCM_BASE ;

/* davep 23-Apr-2013 ; cheesy memory manager to share LCM between CISX and
 * PRNU/DSNU
 */
static struct notaheap scan_fast_memory;

/* davep 14-May-2013 ; track my own memory usage. Only manages the DMA buffers
 * (via scanmem_alloc_aligned)
 *
 * Note signed integer to detect underflow;
 */
//#define SCANMEM_MAX_MEMORY 128*1024*1024
#define SCANMEM_MAX_MEMORY 152*1024*1024	// mod.lsh to prevent EC221(PIC overflow, 600dpi BW photo Dual Scan)
static int max_memory = SCANMEM_MAX_MEMORY;

static spinlock_t scanmem_lock;

#define lock() spin_lock(&scanmem_lock)
#define unlock() spin_unlock(&scanmem_lock)

void *scanmem_alloc( uint32_t datalen )
{
    void *ptr;

    /* davep 14-May-2013 ;  Note this function does not count against the
     * max_memory. Only the DMA buffers count. I can easily track the
     * size of an allocation of the DMA buffers but not the regular heap
     * memory. (I want to maintain the malloc/free semantics as much as
     * possible.)
     */
    ptr = kmalloc( datalen, __GFP_NOWARN|__GFP_NORETRY );
    return ptr;
}

void *scanmem_alloc_aligned( uint32_t datalen )
{
    void *ptr;
    int free_bytes;

    /* davep 14-May-2013 ; adding self limiting memory */
    free_bytes = scanmem_get_free_size();
    if( free_bytes - (int)datalen < 0 ) {
        dbg2( "%s Fail: free = %d, requested = %d\n", __FUNCTION__, free_bytes, datalen );
        return NULL;
    }

    lock();
    max_memory -= datalen;
    unlock();

    ptr = kmalloc( datalen, GFP_DMA|__GFP_NOWARN|__GFP_NORETRY );

    /* if we actually we're able to get it, return the value to our max (our
     * allocation limit doesn't reflect the actual state of the heap)
     */
    if( ptr==NULL ) {
        lock();
        max_memory += datalen;
        unlock();
    }

//    dbg2( "%s %p %d %d\n", __FUNCTION__, ptr, datalen, scanmem_get_free_size());

    return ptr;
}

void scanmem_free_aligned( uint8_t **p_ptr, uint32_t datalen )
{
    uint32_t s;

//    dbg2( "%s %p %d\n", __FUNCTION__, *p_ptr, datalen );

    kfree( *p_ptr );
    *p_ptr = NULL;

    lock();
    max_memory += datalen;
    s = (uint32_t)max_memory;
    unlock();

    /* davep 14-May-2013 ; beware of fake free'ing */
    XASSERT( s <= SCANMEM_MAX_MEMORY, s );
}

uint32_t scanmem_get_heap_size( void )
{
    return SCANMEM_MAX_MEMORY;
}

uint32_t scanmem_get_free_size( void )
{
    uint32_t s;

    lock();
    s = (uint32_t)max_memory;
    unlock();

    return s;
}

void scanmem_free_fast_memory( scanmem_tag tag, void *ptr, uint32_t datalen, int instance )
{
    scanmem_dbg2( "%s tag=%d ptr=%p datalen=%d instance=%d\n", __FUNCTION__,
                        tag, ptr, datalen, instance );

    notaheap_free( &scan_fast_memory, tag, ptr, datalen, instance );
}

/**
 * \brief  Get a pointer into fast memory, if available.
 *
 * Get a DMA'able pointer from some sort of faster-than-DRAM memory such as
 * SRAM. Some platforms may not have this sort of memory.
 *
 * \author David Poole
 * \date 23-Apr-2013
 */

void * scanmem_get_fast_memory( scanmem_tag tag, uint32_t datalen, int instance )
{
    scanmem_dbg2( "%s tag=%d datalen=%d instance=%d\n", __FUNCTION__, 
                    tag, datalen, instance);

    return notaheap_alloc( &scan_fast_memory, tag, datalen, instance );
}

/**
 * \brief  Initialize the scan memory portability layer subsystem
 *
 * \author David Poole
 * \date 23-Apr-2013
 */

scan_err_t scanmem_onetime_init( void )
{
    scan_err_t scerr;

    dbg2( "%s fastmem=%p fastmem_numbytes=%d\n", __FUNCTION__,
            lcm_regs->SRAM, sizeof(lcm_regs->SRAM) );

    spin_lock_init(&scanmem_lock);

    /* davep 23-Apr-2013 ; stomp the entire SRAM to verify linux kernel can
     * address it
     */
    memset( (uint8_t *)lcm_regs->SRAM, 0xee, sizeof(lcm_regs->SRAM) );

    /* initialize the fast memory sharing subsystem */
    scerr = notaheap_init( &scan_fast_memory, (uint8_t *)lcm_regs->SRAM, sizeof(lcm_regs->SRAM) );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    return SCANERR_NONE;
}

