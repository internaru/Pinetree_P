/*
 ******************************************************************************
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************
 */

#ifdef __linux__

#include "map_mem.h"
#include <unistd.h>   // getpagesize(), usleep()
#include <sys/mman.h> // mmap(), munmap()

void *mapMem(uint32_t phys_addr, size_t length, void **map_base, int fd)
{
    uint32_t page_size = getpagesize();

    length += phys_addr & (off_t)(page_size - 1);
    if((*map_base = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                         phys_addr & ~(off_t)(page_size - 1))) == MAP_FAILED) {
        close(fd);
        return(NULL);
    } else {
        return((void *)(*map_base + (phys_addr & (page_size - 1))));
    }
}

int unMapMem(void *map_base, size_t length)
{
    uint32_t page_size = getpagesize();
	length += (off_t)map_base & (page_size - 1);
    if(munmap((void*) ((off_t)map_base & ~(page_size - 1)), length) == -1) {
        return(-1);
    } else {
        return(0);
    }
}

#endif // linux


