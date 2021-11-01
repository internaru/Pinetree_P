#ifndef mlimiter_H
#define mlimiter_H
/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2012  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/**
 *
 * \file mlimiter.h
 *
 * Memory limiter is a utility of our memory manager to aid in limiting a
 * heap's allocations.  A limiter is created using the memory manager API
 * (see memAPI.h) and is passed to memory limiter API calls to allocate memory.
 *
 * The max parameter within the limiter itself is set to a value, in bytes,
 * to limit the user's memory to that amount.  For example a print module might
 * have a limiter with a "max" value set to 4MB so that any limiter allocation
 * made with this limiter would fail once 4MB of allocations were made.
 *
 * This helps to throttle modules based on their pre-determined needs and
 * helps with our memory resoure arbitration.
 *
 * See ../docs/mlimiter.txt and mlimiter_config.c
 **/
#include <stdint.h>
#include <stdbool.h>
#include "key_table.h"
#include "posix_ostools.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * A mlimiter_t is used to help limit memory resources used by modules
 * within the system.
 *
 */
typedef struct mlimiter_s mlimiter_t;


/** Non-parent or child limiters can set a memory low stategy that gets envoked when an allocation fails.
 @return bool is true the allocation will be tried again, if false then a NULL pointer will be returned.
 Not setting a strategy will always return null on failure.  Setting a strategy might cause you to delay internal 
to a malloc call until the memory is freed or enough attempts have been made to free memory.
The caller should be aware of the strategy being used.

<pre>
do 
{  
    ptr = malloc();
    if (!ptr) 
       again = (strategy)(); # delay; send message to find memory etc.
} while (!ptr && again);     # strategy can allow a null pointer returned.
</pre>

A few different strategies:

1) Count how many time memory runs out and always return NULL forcing the caller to deal with the returned null pointer.

2) Loop with a delay hoping that memory will free up.

3) Loop forever but send a message to change the print behaviour to race.

4) Loop forever but send a message freeing up caches.

5) Loop for a set amount of time or attempts then finally give up and return null.

 */

typedef bool (*mlimiter_low_memory_strategy_t)(mlimiter_t *lim, uint32_t failed_alloc_size); 

/// Helper to declare/define a function matching the signature of the function pointer.
#define DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( function_name ) \
    bool function_name(mlimiter_t *lim, uint32_t failed_alloc_size)

/**
 *
 * A mlimiter_s is used to help limit memory resources used by modules
 * within the system.
 *
 * highest_max and lowest_max are used allow a dynamic setting of memory
 * usage, or throttle a processes memory usage.
 */
struct mlimiter_s 
{
#if 0
    void *ms;               /**< This value must be first. ms represents the
                              memory manager "mspace," or rather, the heap.
                             In most cases this will be the base system heap. */
#else
    pthread_mutex_t lock;
#endif    
    uint32_t max;           /**< The maximum number of bytes the user of this
                              limiter can allocate.  Allocations used in
                              conjunction with this limiter will fail once 
                              the allocations made with this limiter reach
                              this limit. This is the active maximum. 
                              Note it is possible that the max is lower than the current,
                              this happens when a pool is shrunk ie has its limit lowered.
                            */

    uint32_t highest_max;   /**< max will always be <= highest max, 
                             the maximum amount of memory needed. */

    uint32_t lowest_max;    /**< max will always be >= lowest max,
                             the miniumum amount of memory needed. 
                             A parent limiter will reserve lowest_max, 
                             this will never be availiable to its children.
                            */

    mlimiter_low_memory_strategy_t strategy; /**< null or a callback on memory 
                                                out, used to recover. */       

    uint32_t active;            /**< active limiter 
                               set by mlimiter_start() mlimiter_stop()
                               inactive limiters can still have memory but
                               allocations out of an inactive limiter will 
                               have a high probability of being out of memory.
                             */

    uint32_t current;       /**< Current in use bytes (bytes allocated in 
                              conjunction with this limiter). 
                              read only!
                            */

    uint32_t high_water;    /**< The limiter's high water mark, in bytes.  
                             This value represents the highest number of bytes
                             allocated at one time in conjunction with this
                             limiter. 
                             read only!
                            */

    mlimiter_t *parent;     /**< This value represents the limiter's parent.
                             if NULL this is a base/parent heap.
                             one level of childred only, 
                             set at startup time read only afterwards.
                             */

};

/// Make limiter active if it isn't already and rebalance active memory.

void mlimiter_start( mlimiter_t *limiter );

/// Make limiter inactive and rebalance active memory.
void mlimiter_stop( mlimiter_t *limiter );

/// return number of bytes available for allocation, negative numbers
/// indicate that the max has changed under you and you should free some.
/// returns (max - current)
int32_t mlimiter_available( mlimiter_t *limiter );

/// In general the strategy is set at config time, this allows a runtime setting
/// of the out of memory recovery strategy.
///
void mlimiter_set_strategy( mlimiter_t *limiter, mlimiter_low_memory_strategy_t strategy);

/// This function can be called to adjust the lowest and highest  max values for a limiter
void mlimiter_adjust_limits( mlimiter_t *limiter, uint32_t lowest_max, uint32_t highest_max );

/// This function is called everytime a limiter is started or stopped.
/// It can also be called as part of a memory out strategy to attempt to aquire more memory
/// Memory from other limiters that is above their min and unused can then be reassigned 
void mlimiter_relevel_memory( mlimiter_t *limiter );

/// In debug pretty print the object to the serial log.
void dbg_mlimiter_print( mlimiter_t *l );

/// example strategy does a relevel then retries.
bool relevel_retry_strategy(mlimiter_t *lim, uint32_t failed_alloc_size);

/// private function to decrement tracked count
void mlimiter_track_sub( mlimiter_t *limiter, uint32_t size );

/// private function to increment tracked count
void mlimiter_track_add( mlimiter_t *limiter, uint32_t size );

/// example strategy does a dbg_print( LOG_ERR, ... ) and returns null.
DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_return_null_strategy );

/// example strategy does a dbg_print( LOG_ERR, ... ) then either ASSERTS or returns null depending on ASSERT in/out.
DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_dbgassert_return_null_strategy );

/// stategy that will printf and keep trying in a infinite loop 
DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_retry_forever_strategy );

#if 1  // cut n paste expansion of key_table interface for mlimiter_t

 /// Get static non-owned object (mlimiter_t *) pointer by name.
 mlimiter_t * mlimiter_by_name( const char * name );

 /// Get static non-owned object (mlimiter_t *) pointer by zero based array index.
 mlimiter_t * mlimiter_by_index( unsigned index );

 /// Get name of object of type (mlimiter_t *) at zero based array index provided.
 const char * mlimiter_name_by_index( unsigned index ); 

 /// Get index of mlimiter by name.
 unsigned mlimiter_index_by_name( const char * name ); 

 /// Get name of mlimiter via mlimiter_t *pointer.
 const char * mlimiter_name_by_mlimiter_t( mlimiter_t *obj );

#else // macro expansion of key_table interface for mlimiter_t
 KEY_TABLE_DECLARE_PUBLIC_2( mlimiter, mlimiter_t );
#endif

#ifdef __cplusplus
}
#endif

#endif
