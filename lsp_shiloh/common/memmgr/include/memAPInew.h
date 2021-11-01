/* 
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *  \file memAPInew.h 
 *  \brief cpp overload functions, included from memAPI.h don't include directly.
 *
 *  Recommended usage is to replace new with limiter_new or sysLimiter_new  
 *  in cpp code rather than overloading per class.  Use of new directly will 
 *  work correctly but the allocations will not have file, line markers for 
 *  debug leak tracking.
 *  
 *  See mlimiter for how to create your own limiter to track how much memory a 
 *  "module/limiter" uses.
 */

#ifdef __cplusplus
#ifndef MEM_API_NEW_H
#define MEM_API_NEW_H


#include <cstring>  // sizeof 

/** global heap both new and sysLimiter_new use this.
 *  limiter_new looks for the variable: limiter 
 *   in the current scope
 */
extern "C" mlimiter_t *sysLimiter; 

/** global heap new use this instead of "new"
 *  variable "sysLimiter" is in global scope.
 */
#define sysLimiter_new new ( sysLimiter, __FILE__, __LINE__ )

/** allocation from limiter instead of global heap use this instead of "new"
 * variable "limiter" must be in scope. 
 * mlimiter_t *limiter = limiter_by_name("network"); // get non-owned reference to limiter
 * char * p = limiter_new char[3000]; // variable must be named "limiter"
 *
 */
#define limiter_new new ( limiter, __FILE__, __LINE__ )



/** overload operator new used with limiter and sysLimiter macros above. 
 */
void *operator new (size_t size,  mlimiter_t *mlimiter, const char *file, int line);

/** new array operator overload 
*/
void *operator new[] (size_t size, mlimiter_t *mlimiter, const char *file, int line);

/** overload operator new, same as sysLimiter_new except the file and line logging is missing 
 * not recommended but its there.
 */
void *operator new (size_t size);

/** overload [] new to use memAPI.c global heap sysLimiter, with poor file and line logging.
 */
void *operator new[] (size_t size);

/** overload delete to use memAPI.c::memFree().
 */
void operator delete (void *p);

#endif  // MEM_API_NEW_H
#endif  // __cplusplus

