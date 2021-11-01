/*
 ******************************************************************************
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************
 */

#ifdef __linux__

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void *mapMem(uint32_t phys_addr, size_t length, void **map_base, int fd);
int unMapMem(void *map_base, size_t length);

#ifdef __cplusplus
}
#endif
#else

// threadx hwaddr and virtual address assumed to be the same
#define mapMem( addr, len, base, fd ) (void*)addr
#define unMapMem( base, len ) 

#endif // linux


