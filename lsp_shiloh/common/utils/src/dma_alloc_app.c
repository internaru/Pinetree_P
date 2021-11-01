/******************************************************************************
 * Copyright (c)2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "dma_alloc_api.h"
#include <unistd.h> // getpagesize
#include <fcntl.h> // open
#include <sys/mman.h> // mmap
#include "lassert.h" 
#include <errno.h>
#include "memAPI.h"
#include "logger.h"
#include "posix_ostools.h"

#define DBG_PRFX "dma_alloc: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 6 )  // using system/dma_alloc


static pthread_mutex_t the_mutex;
static pthread_mutex_t *mutex = 0;


// return back physical address and the page's address in map_base
// 
static void *mapMem( off_t phys_addr, size_t length, void **map_base )
{
	uint32_t page_size = 4096; // getpagesize();
	int fd;
	
	if((fd = open("/dev/mem", (O_RDWR | O_SYNC | O_CLOEXEC))) < 0) {
		perror("dma_alloc_api:mapmem()  open('dev/mem') failed ");
		ASSERT(fd == 0);

	}
	length += phys_addr & (off_t)(page_size - 1);
	if((*map_base = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
			     phys_addr & ~(off_t)(page_size - 1))) == MAP_FAILED) {
		perror("video_config.c: mmap failed");
		close(fd);
		return(NULL);
	} else {
		close(fd);
		return((void *)(*map_base + (phys_addr & (page_size - 1))));
	}
}

static int unMapMem(void *map_base, size_t length)
{
	off_t page_size = 4096; // getpagesize();
	length += (off_t)map_base & (page_size - 1);
	if(munmap((void*) ((off_t)map_base & ~(page_size - 1)), length) == -1) {
		perror("dma_alloc_app.c: munmap failed");
		return(-1);
	} else {
		return(0);
	}
}



static dma_alloc_t *dma_cmd( dma_alloc_t *dma_alloc )
{
	int retval;
	int fd;

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC | O_CLOEXEC );
	if ( fd < 0 ) {
		perror("open dma_alloc device failed %d\n"); 
		memFree(dma_alloc);
		dma_alloc = 0;
	} else { 
		retval = write( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "write failed" ); 
			printf( "fd %d %p len %d retval %d\n", fd, dma_alloc, sizeof(dma_alloc_t), retval );
            ASSERT(0);
		}
		retval = read( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "read failed\n" );
            ASSERT(0);
		}

		close( fd );
	}

	return dma_alloc;
}

static bool forceleak = false; 		/// BUG BUG LSPTODO : deliberate memory leak 
static int total = 0;  // debug variable
static mlimiter_t * dma_limiter = 0;
static pthread_mutex_t dmaLimLock = POSIX_MUTEX_INITIALIZER;
static pthread_mutex_t *dmaLimLockPtr = 0;

dma_alloc_t *dma_free( dma_alloc_t *dma_alloc )
{
	dma_alloc_t *dma_freeMe = dma_alloc;
	dma_freeMe->operation = dma_operation_free;

	if ( dma_limiter->active ) {
		pthread_mutex_lock( dmaLimLockPtr );
		total -= dma_freeMe->len ;
		dma_limiter->current -= dma_freeMe->len;
		DBG_MEMLOG_NOTICE( "%s %d %x total %d \n", __FUNCTION__, dma_freeMe->len, dma_freeMe->kv_addr, total );
		dma_freeMe = dma_cmd( dma_freeMe );
		pthread_mutex_unlock( dmaLimLockPtr );
	}

	//DBG_MEMLOG_NOTICE( "%s %d %x total %d \n", __FUNCTION__, dma_alloc->len, dma_alloc->kv_addr, total );
	return dma_freeMe;
}

dma_alloc_t *dma_alloc_reuse( dma_alloc_t *reuse, size_t len )
{
	dma_alloc_t *dma_alloc = reuse; 
	bool again;

	if (!dmaLimLockPtr) {
		dmaLimLockPtr = &dmaLimLock;
		posix_mutex_init( dmaLimLockPtr );
	}

	dma_alloc->operation = dma_operation_alloc;
	dma_alloc->len = len;
	dma_alloc->kv_addr = 0;
	dma_alloc->v_addr = 0;
	dma_alloc->hw_addr = 0;
	while (1) {
		if ( dma_limiter->active && len < mlimiter_available( dma_limiter ) ) {
			pthread_mutex_lock( dmaLimLockPtr );
			dma_alloc = dma_cmd( dma_alloc );
			if ( dma_alloc && dma_alloc->kv_addr ) {
				total += dma_alloc->len;
				dma_limiter->current += dma_alloc->len;
				dma_limiter->high_water = dma_limiter->current > dma_limiter->high_water ? 
					dma_limiter->current : dma_limiter->high_water;
				DBG_MEMLOG_NOTICE( "%s %d %x total %d \n", __FUNCTION__,
						   dma_alloc->len, dma_alloc->kv_addr, total );
			} else { 
				DBG_PRINTF_CRIT( "%s no kernel memory ! %d total %d \n",
						 __FUNCTION__, len, total );
			}
			pthread_mutex_unlock( dmaLimLockPtr );
			break;
		} else if ( dma_limiter->strategy) {
			again = dma_limiter->strategy( dma_limiter, len );
			if (!again) break;
		}
	}
	return dma_alloc;
}

dma_alloc_t *dma_alloc( size_t len )
{
    dma_alloc_t *dma_alloc = (dma_alloc_t *) MEM_CALLOC(sizeof(dma_alloc_t), 1);
	REL_ASSERT( dma_alloc );

	if ( ! dma_limiter ) {
		dma_limiter = mlimiter_by_name("dma_limiter");
		mlimiter_start( dma_limiter );
	}
	if ( forceleak && (total == 0)) {  
		/// BUG BUG LSPTODO : deliberate memory leak 
		dma_alloc->operation = dma_operation_alloc;
		dma_alloc->len = 4096;
		dma_alloc->kv_addr = 0;
		dma_alloc->v_addr = 0;
		dma_alloc->hw_addr = 0;
		dma_alloc = dma_cmd( dma_alloc ); // force a leak
        ASSERT(dma_alloc);
        total += dma_alloc->len;          // track the leak
	}
    return dma_alloc_reuse( dma_alloc, len );
}

dma_alloc_t *dma_alloc_alloc_coherent( size_t len )
{
        dma_alloc_t *dma_alloc = (dma_alloc_t *) MEM_CALLOC(sizeof(dma_alloc_t), 1);
	ASSERT( dma_alloc );
  
	dma_alloc->operation = dma_operation_alloc_coherent;
	dma_alloc->len = len;
	dma_alloc->kv_addr = 0;
	dma_alloc->v_addr = 0;
	dma_alloc->hw_addr = 0;
	
	dma_cmd( dma_alloc );
	dma_alloc_mmap_forcpu( dma_alloc );
	return dma_alloc;
}

dma_alloc_t *dma_alloc_free_coherent( dma_alloc_t *dma_alloc )
{
    if (dma_alloc)
    {
        dma_alloc_unmmap_forcpu( dma_alloc ); 
        dma_alloc->operation = dma_operation_free_coherent;
        dma_alloc = dma_cmd( dma_alloc );
        if (dma_alloc)
        {
            memFree(dma_alloc);
        }
    }

	return 0;
}
 
dma_alloc_t *dma_alloc_map_single( dma_alloc_t *dma_alloc, int direction )
{
	dma_alloc_t *result;
	dma_alloc->operation = dma_operation_map;
	dma_alloc->direction = direction;
    ASSERT(dma_alloc->len > 0);
    XASSERT((uint32_t)dma_alloc->kv_addr >= 4096, (uint32_t)dma_alloc->kv_addr);
	result = dma_cmd( dma_alloc );
    if (result != NULL)
    {
        result->direction *= -1;  // mark so we can unmap at free time.
        // ideally the user of dma would unmap after the dma completes
        // marking the block as mapped allows us to unmap it late at free time 
        // this prevents the kernel from hanging on to the allocation.
    }
	return result;
}

dma_alloc_t *dma_alloc_unmap_single( dma_alloc_t *dma_alloc, int direction )
{
	dma_alloc->operation = dma_operation_unmap;
	dma_alloc->direction = direction;

	return dma_cmd( dma_alloc );
	
}

dma_alloc_t *dma_alloc_mmap_forcpu( dma_alloc_t *reuse )
{
	void * base;

	if (reuse->hw_addr && reuse->hw_addr != 0xdeadbeef )
		reuse->v_addr = mapMem( reuse->hw_addr, reuse->len, &base );

	return reuse ;
}

dma_alloc_t *dma_alloc_unmmap_forcpu( dma_alloc_t *reuse )
{
	if (reuse->v_addr && (int)reuse->v_addr != 0xdeadbeef ) {
		unMapMem( reuse->v_addr, reuse->len );
		reuse->v_addr = 0;
	}

	return reuse ;
}

dma_alloc_t *dma_alloc_pool_alloc( dma_alloc_t *dma_alloc, size_t size )
{
	dma_alloc->operation = dma_operation_pool_alloc;
        dma_alloc->len = size;

	dma_cmd( dma_alloc );
        dma_alloc_mmap_forcpu( dma_alloc ); 

        return dma_alloc;
}

dma_alloc_t *dma_alloc_pool_free( dma_alloc_t *dma_alloc )
{
	return 0;
}


//  mutex protected linked list 
static dma_alloc_t *head = 0;

void *memMallocPhysical( void **hw_address, uint32_t size, tAlign align, const char *file, int line )
{

    dma_alloc_t *dma_alloc;

    if (!mutex) {
	    mutex = &the_mutex;
	    posix_mutex_init( mutex );
    }

    pthread_mutex_lock( mutex );
  
    dma_alloc = dma_alloc_alloc_coherent( size );
    *hw_address = (void*)dma_alloc->hw_addr;

    // hack might be nice to save this so we can free it :)
    if (head) {
	    dma_alloc->direction = (int) head;
	    head = dma_alloc;
    } else {
	    dma_alloc->direction = 0;
	    head = dma_alloc;
    }

    pthread_mutex_unlock( mutex );
    return dma_alloc->v_addr;
}

void *memFreePhysical( void *hw_addr, void *v_addr )
{
	dma_alloc_t *p, *prev;

	pthread_mutex_lock( mutex );
	
	for ( p = head, prev = head; 
	      p  ; 
	      prev = p, p = (dma_alloc_t *) p->direction ) {
		DBG_PRINTF_DEBUG(" Walk %s hw_addr %x v_addr %p kv_addr %p next %p\n", 
				 __FUNCTION__, p->hw_addr, p->v_addr, p->kv_addr, (void*)p->direction );
		if ( p->hw_addr == (uint32_t)hw_addr ) {
			DBG_PRINTF_DEBUG(" %s hw_addr %x v_addr %p kv_addr %p \n", 
					 __FUNCTION__, p->hw_addr, p->v_addr, p->kv_addr );
			prev->direction = p->direction;
			if ( p == head ) 
				head = (dma_alloc_t *) p->direction;

            dma_alloc_free_coherent(p);

			pthread_mutex_unlock( mutex );
			return 0; 
		}
	}	

	pthread_mutex_unlock( mutex );
        // not found !!!!
	DBG_PRINTF_ERR(" %s NOT found ! hw_addr %p v_addr %p  \n", __FUNCTION__, hw_addr, v_addr );
	return 0;
}

dma_alloc_t *dma_alloc_write( dma_alloc_t *dma_alloc, const void *buf, size_t count )
{
	int retval;
	int fd;

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC | O_CLOEXEC );
	if ( fd < 0 ) {
		perror("open dma_alloc device failed %d\n"); 
		free(dma_alloc);
		dma_alloc = 0;
	} else {
		dma_alloc->operation = dma_operation_write_to_kernel;
		retval = write( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "write failed" ); 
			printf( "fd %d %p len %d retval %d\n", 
				fd, dma_alloc, sizeof(dma_alloc_t), retval );
		}
		retval = write( fd, buf, count );
		if ( retval < count ) {
			perror( "write failed" ); 
		}
	}
	close( fd );

	return dma_alloc;
}

dma_alloc_t *dma_alloc_read( dma_alloc_t *dma_alloc, void *buf, size_t count )
{
	int retval;
	int fd;

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC | O_CLOEXEC );
	if ( fd < 0 ) {
		perror("open dma_alloc device failed %d\n"); 
		free(dma_alloc);
		dma_alloc = 0;
	} else {
		dma_alloc->operation = dma_operation_read_from_kernel;
		retval = write( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "write failed" ); 
			printf( "fd %d %p len %d retval %d\n", 
				fd, dma_alloc, sizeof(dma_alloc_t), retval );
		}
		retval = read( fd, buf, count );
		if ( retval < count  ) {
			printf( "read failed %d cnt %d\n", retval, count); 
		}
	}
	close( fd );

	return dma_alloc;
}
