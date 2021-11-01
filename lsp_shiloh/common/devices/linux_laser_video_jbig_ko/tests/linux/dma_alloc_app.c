/******************************************************************************
 * Copyright (c)2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "dma_alloc_api.h"
#include <unistd.h> // getpagesize
#include <fcntl.h> // open
#include <sys/mman.h> // mmap
#include <errno.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#define ASSERT assert
// return back physical address and the page's address in map_base
// 
static void *mapMem( off_t phys_addr, size_t length, void **map_base )
{
	uint32_t page_size = 4096; // getpagesize();
	int fd;
	
	if((fd = open("/dev/mem", (O_RDWR | O_SYNC))) < 0) {
		perror("dma_alloc_api:mapmem()  open('dev/mem') failed ");
		ASSERT(fd == 0);

	}

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
	if(munmap(map_base, length) == -1) {
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

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC );
	if ( fd < 0 ) {
		perror("open dma_alloc device failed %d\n"); 
		free(dma_alloc);
		dma_alloc = 0;
	} else { 
		retval = write( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "write failed" ); 
			printf( "fd %d %p len %d retval %d\n", fd, dma_alloc, sizeof(dma_alloc_t), retval );
		}
		retval = read( fd, dma_alloc, sizeof(dma_alloc_t) );
		if ( retval < sizeof( dma_alloc_t ) ) {
			perror( "read failed\n" );
		}

		close( fd );
	}

	return dma_alloc;
}

static bool forceleak = 0; 		/// BUG BUG LSPTODO : deliberate memory leak 
static int total = 0;  // debug variable

dma_alloc_t *dma_free( dma_alloc_t *dma_alloc )
{
	dma_alloc->operation = dma_operation_free;
	total -= dma_alloc->len ;
	return dma_cmd(dma_alloc);
}

dma_alloc_t *dma_alloc_reuse( dma_alloc_t *reuse, size_t len )
{
	dma_alloc_t *dma_alloc; 
	reuse->operation = dma_operation_alloc;
	reuse->len = len;
	total += reuse->len ;
	dma_alloc = dma_cmd( reuse );

	return dma_alloc;
}

dma_alloc_t *dma_alloc( size_t len )
{
        dma_alloc_t *dma_alloc = (dma_alloc_t *) calloc(sizeof(dma_alloc_t), 1);
	ASSERT( dma_alloc );
  
	if ( forceleak && (total == 0)) {  
		/// BUG BUG LSPTODO : deliberate memory leak 
		dma_alloc->operation = dma_operation_alloc;
		dma_alloc->len = 4096;
		dma_alloc->kv_addr = 0;
		dma_alloc->v_addr = 0;
		dma_alloc->hw_addr = 0;
		dma_alloc = dma_cmd( dma_alloc ); // force a leak
		total += dma_alloc->len;          // track the leak
	}
	dma_alloc->operation = dma_operation_alloc;
	dma_alloc->len = len;
	dma_alloc->kv_addr = 0;
	dma_alloc->v_addr = 0;
	dma_alloc->hw_addr = 0;
	dma_alloc = dma_cmd( dma_alloc );

	total += dma_alloc->len;
	return dma_alloc;
}

dma_alloc_t *dma_alloc_alloc_coherent( size_t len )
{
        dma_alloc_t *dma_alloc = (dma_alloc_t *) calloc(sizeof(dma_alloc_t), 1);
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
	dma_alloc_unmmap_forcpu( dma_alloc ); 
	dma_alloc->operation = dma_operation_free_coherent;
	dma_cmd( dma_alloc );

	return 0;
}
 
dma_alloc_t *dma_alloc_map_single( dma_alloc_t *dma_alloc, int direction )
{
	dma_alloc_t *result;
	dma_alloc->operation = dma_operation_map;
	dma_alloc->direction = direction;
	result = dma_cmd( dma_alloc );
	result->direction *= -1;  // mark so we can unmap at free time.
	// ideally the user of dma would unmap after the dma completes
	// marking the block as mapped allows us to unmap it late at free time 
	// this prevents the kernel from hanging on to the allocation.
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


#if 0
//  mutex protected linked list 
static dma_alloc_t *head = 0;

void *memMallocPhysical( void **hw_address, uint32_t size, tAlign align, const char *file, int line )
{

    dma_alloc_t *dma_alloc;

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

    return dma_alloc->v_addr;
}

void *memFreePhysical( void *hw_addr, void *v_addr )
{
	dma_alloc_t *p, *prev;

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

			dma_alloc_unmmap_forcpu( p ); 
			p->operation = dma_operation_free_coherent;
			dma_cmd( p );

			return 0; 
		}
	}	

        // not found !!!!
	DBG_PRINTF_ERR(" %s NOT found ! hw_addr %p v_addr %p  \n", __FUNCTION__, hw_addr, v_addr );
	return 0;
}
#endif

dma_alloc_t *dma_alloc_write( dma_alloc_t *dma_alloc, const void *buf, size_t count )
{
	int retval;
	int fd;

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC );
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

	fd = open("/dev/dma_alloc", O_RDWR | O_SYNC );
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
