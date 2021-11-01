/*
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file memAPInew.cpp
 * operator overload functions map new to memAPI.c heap with limiter.
 *
 */

#include "stdlib.h"  // size_t
#include "memAPI.h"
#include "logger.h"



void *operator new (size_t size, mlimiter_t *mlimiter, const char *file, int line)
{
  return mem_retry_malloc_align_limited(mlimiter, size, e_8_byte, file, line );    
}

void *operator new[] (size_t size, mlimiter_t *mlimiter, const char *file, int line)
{
    return mem_retry_malloc_align_limited(mlimiter, size, e_8_byte, file, line );    
}

void operator delete (void *p)
{
    memFree(p);
}

void operator delete[] (void *p)
{
    memFree(p);
}

#ifdef HAVE_CPP_THROW_EXCEPTION
void *operator new (size_t size) throw (std::bad_alloc)
{
  // todo: replace __LINE__ with the callers return address (except you only
  //       have 16 bits)
  // 
  void *pMem = mem_retry_malloc_align_limited(0, size, e_8_byte, 
					"memmgr/c++wrapper/memAPInew.cpp/new_operator", __LINE__ );
  if (!pMem)
    throw std::bad_alloc();
  return pMem;
}

void *operator new[] (size_t size) throw (std::bad_alloc)
{
  // todo:  see new above
  void *pMem = mem_retry_malloc_align_limited(0, size, e_8_byte, 
                  "memmgr/c++wrapper/memAPInew.cpp/new_operator", __LINE__);
  if (!pMem)
    throw std::bad_alloc();
  return pMem;
}
#else
void *operator new (size_t size)
{
  // todo: replace __LINE__ with the callers return address (except you only
  //       have 16 bits)
  // 
  return mem_retry_malloc_align_limited(0, size, e_8_byte, 
					"memmgr/c++wrapper/memAPInew.cpp/new_operator", __LINE__ );
}

void *operator new[] (size_t size)
{
  // todo:  see new above
  return mem_retry_malloc_align_limited(0, size, e_8_byte, 
					"memmgr/c++wrapper/memAPInew.cpp/new_operator", __LINE__ );
}
#endif

#ifdef DEBUG
class new_test_foobar
{
  int a;
  int b;
  char c[1234];
};
#endif

extern "C" void dbg_new_memapi_test( void )
{ 
#ifdef DEBUG
  {
    // this fails to compile since limiter isn't in scope.
    // char *b = limiter_new char[1111] ;
  }

  // don't do this it is done for you at init time.
  // dmalloc.limiter =  mlimiter_by_name("syslimiter");

  // do this with the limiter of your choice syslimiter is valid as well.
  mlimiter_t *limiter = 0;

  char *null_limiter = 0; // limiter == 0 is identical to limiter == sysLimiter
  null_limiter = limiter_new char[2001] ;
  if ( null_limiter == 0 )
      dbg_printf("fail alloc \n");
  limiter = mlimiter_by_name("network");

  char *a = 0; 
  char *b = 0; 
  mlimiter_t *m = 0; 

  b = limiter_new char[1111] ;
  a = sysLimiter_new char[2222] ;


  if ( a == 0 ) 
    dbg_printf("fail a \n");
  else 
    dbg_printf("ok a\n");

  if ( b == 0 ) 
    dbg_printf("fail b on first call since network isn't started.\n");
  else 
    dbg_printf("ok b\n");

  // doesnt' hurt to start it multiple times but if it is stopped it won't allocate.
  mlimiter_start(mlimiter_by_name("network"));
  
  m = limiter_new mlimiter_t;
  if ( m == 0 ) 
    dbg_printf("fail m\n");
  else
    dbg_printf("ok m\n");

  char *k = 0;
  k = k; //prevent compiler warning
  k = new char[3333];
  
  new_test_foobar *dummy = 0;
  dummy = new new_test_foobar;

  dbg_printf("dummy %p \n", dummy);

  long *along = 0;
  along = new long(42);

  dbg_printf("along %p %d k[0] %c\n", along, *along, k[0]);


  // don't delete so we leak
  delete m;
  delete a;

  delete dummy;

  mlimiter_t *alim = new mlimiter_t[3];

  delete [] alim;
  delete [] k;

  delete b; 
  delete along;
  dbg_printf("memAPInew test done\n");
#endif // DEBUG
}

