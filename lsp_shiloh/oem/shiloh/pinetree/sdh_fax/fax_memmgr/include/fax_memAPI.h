
#ifndef MEMAPI_H
#define MEMAPI_H

#include "mlimiter.h"

typedef enum 
{
    e_4_byte =  4, 
    e_8_byte =  8,
    e_16_byte = 16,
    e_32_byte = 32,
    e_2048_byte = 2048
} tAlign;


extern void *mem_retry_malloc_align_limited(
    mlimiter_t *lim, 
    int size, 
    int align, 
    const char *file, const int line);

extern void memFree(void *address);     
    
#define MEM_MALLOC(sz) \
    mem_retry_malloc_align_limited(0, sz, e_8_byte, __FILE__, __LINE__)
    
#define MEM_FREE_AND_NULL(x) \
    do{ memFree(x); (x) = NULL;}while(0)
    
/* an attempt to make something like C++ auto_ptr */
#define PTR_ASSIGN(dst,src)  do { (dst)=(src); (src)=NULL; } while(0)
/* davep 02-Jul-2008 ; use the new memory manager functions */
#define PTR_FREE(ptr)  do { memFree((ptr)); (ptr)=NULL; } while(0)

#endif
