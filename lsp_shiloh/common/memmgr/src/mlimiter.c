/** 
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 * 
 **/

#include "mlimiter.h"
#include "mlimiter_priv.h"
#include "key_table.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"


#define DBG_PRFX "lim: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 24 )  

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__)

#define mlimiter_lock( l ) pthread_mutex_lock( &l->lock )
#define mlimiter_unlock( l ) pthread_mutex_unlock( &l->lock )
#ifndef MAX 
#define MAX( a, b ) ((a) < (b) ? (b) : (a))
#endif

void mlimiter_track_sub( mlimiter_t *limiter, uint32_t size )
{
    if (!limiter) 
        return;
    mlimiter_lock( limiter );
    limiter->current -= size;   
    mlimiter_unlock( limiter );
}

void mlimiter_track_add( mlimiter_t *limiter, uint32_t size )
{
    if (!limiter) 
        return;
    mlimiter_lock( limiter );
    limiter->current += size;   
    if (limiter->current > limiter->high_water) 
    {
	limiter->high_water = limiter->current;
    }
    mlimiter_unlock( limiter );
}

void mlimiter_set_strategy( mlimiter_t *limiter, mlimiter_low_memory_strategy_t strategy)
{
    if (!limiter) 
        return;
    mlimiter_lock( limiter );
    limiter->strategy = strategy;   
    mlimiter_unlock( limiter );
}

static int mlimiter_job_current(uint32_t *max, uint32_t *sum, uint32_t *min)
{
    uint32_t i = mlimiter_index_by_name("sys_retry_forever");
    uint32_t active = 0;
    mlimiter_t *lim = 0;
    *max = 0;
    *sum = 0;
    *min = 0;
    
    while (0 != (lim = mlimiter_by_index(i)))
    {
        if (lim->active)
        {
            *max += lim->highest_max - lim->current ;
            *min += lim->lowest_max;
	    // add in higher of current or requested lowest_max
            *sum += lim->lowest_max > lim->current ? lim->lowest_max : lim->current;
            ++active;
        }
        else
        {
            *sum += lim->current;
        }
        ++i;
    }
    return active;
}



#define dbg_print_limiter( i ) \
  dbg_mlimiter_print(mlimiter_by_index( i ))

void dbg_mlimiter_print( mlimiter_t *lim )
{
    // default to sysLimiter
    lim = lim ? lim : mlimiter_by_index(1);
    const char *name = mlimiter_name_by_mlimiter_t(lim);
    const char *active = lim->active ? "active" : "INACTIVE";

    dbg_printf("LIM [%s] (%08p)\n"
               "   :      lock 0x%08x     max %08d\n"
               "   :     h_max   %08d   l_max %08d  strategy 0x%08p\n"
               "   :    %s  parent 0x%08x \n"
               "   : highwater   %08d current %08d\n", 
               name, 
               lim,
               0, // lim->lock,
               lim->max, 
               lim->highest_max, 
               lim->lowest_max, 
               lim->strategy,
               active, 
               lim->parent,
               lim->high_water,
               lim->current
        );
}


static void mlimiter_assign_available_memory( uint32_t pool, bool keep_current_memory )
{
    uint32_t pending_factories;
    uint32_t i;
    mlimiter_t *lim;
    uint32_t fair_share;

    pending_factories = 0;
    for (i = mlimiter_index_by_name( "sys_retry_forever" ); 0 != (lim = mlimiter_by_index(i)); i++)
    {
        if ( lim->active)
        {
            // Reset all maxs in preparation  for relevel
            lim->max = 0;
            pending_factories++;
        }
    }

    while ( pending_factories > 0 )
    {
        mlimiter_t *next_lim = NULL;
        uint32_t min_spread = 0xFFFFFFFF;
        uint32_t min_assignment;

        // Find lowest spread between max and min of active factories 
        for (i = mlimiter_index_by_name( "sys_retry_forever" ); 0 != (lim = mlimiter_by_index(i)); i++)
        {
            if ( keep_current_memory )
            {
                min_assignment = MAX( lim->lowest_max, lim->current );
            }
            else
            {
                min_assignment = lim->lowest_max;
            }

            if ( ( lim->active ) && 
                 ( lim->max == 0 ) &&
                 ( ( lim->highest_max - min_assignment ) < min_spread ) )
            {
                next_lim = lim;
                min_spread = lim->highest_max - min_assignment;
            }
        }
#if 0
        ASSERT( next_lim != NULL );
#else
        //lim->active can be decreased by mlimiter_start/_stop of another thread,
        //because mlimiter_start/_stop uses their lock, not syslimiter's lock
        //here's ASSERT can occur.
        if ( next_lim == NULL ) {
            DBG_ERR("[%s %d]\"next_lim is NULL\"\n", __func__, __LINE__);
            pending_factories--;
            continue;
        }
#endif

        // If all remaining factories got an equal share how much would it be
        fair_share = pool / pending_factories;
        if ( keep_current_memory )
        {
            min_assignment = MAX( next_lim->lowest_max, next_lim->current );
        }
        else
        {
            min_assignment = next_lim->lowest_max;
        }

        if ( fair_share > min_spread )
        {
            // There is more available for this factory than it needs
            next_lim->max = next_lim->highest_max;
            pool -= next_lim->highest_max - min_assignment;
        }
        else
        {
            // This factory wants more than its equal share
            next_lim->max = min_assignment + fair_share;
            pool -= fair_share;
        }
        pending_factories--;
    }
}


void mlimiter_relevel_memory( mlimiter_t *limiter )
{
    uint32_t current;   // Sum of all active limiters MAX(minimums, current)
    uint32_t min;       // Sum of all active limiters minimums
    uint32_t max;       // Sum of all active limiters maximums
    mlimiter_t *sys_lim;

    sys_lim = mlimiter_by_name("syslimiter");
    mlimiter_lock( sys_lim );

    mlimiter_job_current(&max, &current, &min);

    // Make sure all active mins fit in system memory
    if ( sys_lim->max < min )
    {
        DBG_ERR( "Minimum for all limiters (%d bytes) is greater than total system memory (%d bytes)!", min, sys_lim->max );
    }

    // Verify that the minimum limits for active limiters does not exceed system memory
    if( sys_lim->max < current )
    {
        // Isn't enough memory for all mins + current allocations - need to trim max values
        mlimiter_assign_available_memory( sys_lim->max - min, false );
    }
    else
    {
        // Memory exists beyond minimums - and current allocations.
        // Decide how to share this memory
        mlimiter_assign_available_memory( sys_lim->max - current, true );
    }
    
    // UNCOMMENT to print out new values after relevel     
    //mlimiter_t *lim;
    //uint32_t i;
    //for (i = 2, lim = mlimiter_by_index(i); lim ; ++i, lim = mlimiter_by_index(i))
    //{
    //    dbg_mlimiter_print( lim );
    //}
        
    mlimiter_unlock( sys_lim );
}


void mlimiter_adjust_limits( mlimiter_t *limiter, uint32_t lowest_max, uint32_t highest_max )
{
    limiter->lowest_max = lowest_max;
    limiter->highest_max = highest_max;
}

void mlimiter_start( mlimiter_t * limiter )
{
    ASSERT( limiter );
    mlimiter_lock( limiter );
    
    if ( ! limiter->active )
    {
       limiter->active = 1;
       mlimiter_relevel_memory( limiter );
    }
    else 
    {
        limiter->active++;
    }
    
    mlimiter_unlock( limiter );
}

void mlimiter_stop( mlimiter_t * limiter )
{
    ASSERT( limiter );
    mlimiter_lock( limiter );
    if (limiter->active > 0) 
    {
        limiter->active--;
    }
    if ( ! limiter->active )
    {
        mlimiter_relevel_memory( limiter );
    }
    
    mlimiter_unlock( limiter );
}

int32_t mlimiter_available( mlimiter_t *l )
{
    int32_t avail;
    ASSERT( l );
    mlimiter_lock( l );
    avail = l->max - l->current ;
    mlimiter_unlock( l );
    return avail ;
}

DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_return_null_strategy )
{
    DBG_ERR("alloc of %d failed in %s\n"
            "   : on limiter %s current %d max %d %s\n",
            failed_alloc_size, 
            __FUNCTION__,
            mlimiter_name_by_mlimiter_t(lim), lim->current, lim->max, lim->active ? " " :  "ERROR limiter not active!");
    return false; // don't retry
}

bool relevel_retry_strategy(mlimiter_t *lim, uint32_t failed_alloc_size)
{
    mlimiter_relevel_memory( lim );

    if ( lim->max - lim->current < failed_alloc_size )
    {
        // With relevel didn't get enough memory - sleep then retry
        posix_sleep_ticks( 5 );
    }

    return true; 
}

DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_dbgassert_return_null_strategy )
{
    DBG_ERR("alloc of %d failed in %s\n"
            "   : on limiter %p current %d max %d \n",
            failed_alloc_size, 
            __FUNCTION__,
            lim, lim->current, lim->max);
    DBG_ASSERT( lim->active );
    DBG_ASSERT( 0 == 1 );
    return false; // don't retry
}

DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( printf_warning_retry_forever_strategy )
{
    DBG_ERR("alloc of %d failed in %s\n"
            "   : on limiter %p current %d max %d %s \n",
            failed_alloc_size, 
            __FUNCTION__,
            lim, lim->current, lim->max, lim->active ? " " : "ERROR limiter not active!");
    ASSERT(failed_alloc_size < (128*1024*1024));
    posix_sleep_ms(100);
    return true; //
}

/////////////// Test code //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


#ifdef HAVE_UNIT_TEST

static void **aptrs;
static uint32_t aused = 0;
static volatile bool alloc_failed = false;

static DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( retry_test_strategy )
{
    int i=0;
    while (aptrs[i] == 0)
        ++i;
    
    memFree( aptrs[i] ); // free a block to simulate waiting for something to change.

    aptrs[i] = 0;
    alloc_failed = true;

    DBG_ERR("alloc failed in retry_test_strategy %d\n", i);
    return true;
}

static DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( fail_test_strategy )
{
    DBG_ERR("alloc failed in fail_test_strategy \n");
    return false; // don't retry just fail.
}

void mlimiter_test()
{
    uint32_t i;
    uint32_t asize = 1024;
    mlimiter_t *print = mlimiter_by_name("print");
    ASSERT(print);

    aptrs = MEM_CALLOC(asize, sizeof(uint32_t));
    ASSERT(aptrs);

    mlimiter_start( print );
    aptrs[aused++] = MEM_MALLOC_LIMITED( print, 1024*1024 );

    mlimiter_set_strategy( mlimiter_by_name("scan") , retry_test_strategy);
    aptrs[aused++] = MEM_MALLOC_LIMITED(print, 1024*1024 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(print, 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(print, 1024*512 );    
    
    if (DBG_WOULD_PRINTF( LOG_INFO ) )
    {
        dbg_print_limiter(2);
        dbg_print_limiter(3);
        dbg_print_limiter(4);
    }

    DBG("print @ 3meg\n" );
    DBG("mlimiter: starting scan \n");
    mlimiter_start( mlimiter_by_name("scan") );   


    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("print"), 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("scan"), 1024*412 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("print"), 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("scan"), 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("print"), 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("scan"), 1024*512 );

    DBG("Current print %d scan %d fax %d printavail %d\n", 
        print->current, 
        mlimiter_by_name("scan")->current, 
        mlimiter_by_name("fax")->current,
        mlimiter_available(print));

    mlimiter_start( mlimiter_by_name("fax") );
    DBG("started fax \n");

	
    aptrs[aused++] = MEM_MALLOC_LIMITED(print, 1024*512 );
    dbg_print_limiter( mlimiter_index_by_name("fax"));
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("fax"), 1024*512 );
    dbg_print_limiter( mlimiter_index_by_name("fax"));
    if (DBG_WOULD_PRINTF( LOG_INFO ) )
    {
        dbg_print_limiter(2);
        dbg_print_limiter(3);
        dbg_print_limiter(4);

    }
    DBG("Current print %x scan %x fax %x\n", 
            print->current, mlimiter_by_name("scan")->current, mlimiter_by_name("fax")->current);
    aptrs[aused++] = MEM_MALLOC_LIMITED(print, 1024*512 );
    aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("fax"), 1024*512 );
    DBG("Current print %x scan %x fax %x\n", 
            print->current, mlimiter_by_name("scan")->current, mlimiter_by_name("fax")->current);
    while (!alloc_failed)
    {
        aptrs[aused++] = MEM_MALLOC_LIMITED(mlimiter_by_name("scan"), 1024*512 );
    }
    DBG("Current print %x scan %x fax %x\n", 
            print->current, mlimiter_by_name("scan")->current, mlimiter_by_name("fax")->current);
    
    if (DBG_WOULD_PRINTF( LOG_INFO ) )
    {
        dbg_print_limiter(2);
        dbg_print_limiter(3);
        dbg_print_limiter(4);
    }
    for(i=0; i<aused; i++)
    {
        if (aptrs[i])
            memFree(aptrs[i]);
        
    }
    DBG("Current print %x scan %x fax %x\n", 
        print->current, mlimiter_by_name("scan")->current, mlimiter_by_name("fax")->current);
    aused = 0;
    alloc_failed = false;

    if (DBG_WOULD_PRINTF( LOG_INFO ) )
    {
        dbg_print_limiter(2);
        dbg_print_limiter(3);
        dbg_print_limiter(4);
    }
    DBG("test for allocation failure on inactive limiter.\n");
    {
        mlimiter_stop( mlimiter_by_name("scan") );
        mlimiter_set_strategy( mlimiter_by_name("scan") , fail_test_strategy);
        ASSERT( 0 == MEM_MALLOC_LIMITED(mlimiter_by_name("scan"), 1024*512 ));
    }

    DBG("test for allocation failure on HUGE alloc\n");
    {
        mlimiter_t *fax = mlimiter_by_name("fax");
        ASSERT(fax);

        ASSERT(aptrs[0] == MEM_MALLOC_LIMITED(fax, fax->max + 1 ));
    }
    DBG("test for allocation failure on limit \n");
    {
        mlimiter_t *fax = mlimiter_by_name("fax");
        ASSERT(fax);

        aptrs[0] = MEM_MALLOC_LIMITED(fax, fax->max - 1024 );
        aptrs[1] = MEM_MALLOC_LIMITED(fax, 1024 );
        DBG(" [0] %p [1] %p \n", aptrs[0], aptrs[1]);
        ASSERT(aptrs[1] == 0);
        ASSERT(aptrs[0]);
        memFree(aptrs[0]);
        aptrs[0] = 0;

        mlimiter_stop( fax );
	dbg_print_limiter( mlimiter_index_by_name("fax"));
    }

    mlimiter_stop( print );

    memFree(aptrs);
    aptrs = 0;        

    DBG_ERR("%s passed\n", __FUNCTION__);
}
#endif
