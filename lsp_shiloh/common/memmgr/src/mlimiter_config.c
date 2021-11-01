/* 
 * ============================================================================
 * Copyright (c) 2010  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file mlimiter_config.c
 * 
 */

#include "mlimiter.h"
#include "key_table.h"
#include "memAPI.h"
#include "logger.h"


const const char * mlimiter_key_table[] =
{
    "syslimiter", ///< main heap can return null
    "sys_retry_forever", ///< runs infinite retry loop
    "print", ///<  print_parser(s) starts and stops
    "ipage_print", ///<  Internal page starts and stops
    "scan",  ///<  
    "fax",   ///< 
    "thumbnails", ///< jpeg thumbnails for image display
    "network",  ///<  
    "small_limiter",
    0
};


extern DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t( print_low_memory_strategy );

mlimiter_t my_limiters_data [] = 
{
    /// syslimiter always at position 0
    {
        .max = 0xffffffff,         // 
        .highest_max = 0xffffffff, // as much as possible.
        .lowest_max = 0x200000,    // 2 meg minimum must be left over for system heap, Reserve.
        .strategy = 0,  // must stay 0, assert based
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    /// sys_retry_forever
    {
        .max = 0,
        .highest_max = 0x200000,
        .lowest_max = 0x10000,
        .strategy = printf_warning_retry_forever_strategy,
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    /// limiters after here are all using the syslimiter as parent, one heap.
    /// print
    {
        .max = 0,                     // set at runtime
        .highest_max = 0xffffffff,    // as much as possible
        .lowest_max = 0x100000,       // minimum ram 
        .strategy = printf_warning_retry_forever_strategy, // print_low_memory_strategy,  // may have a low memory strategy
        .active = 0,                  // set at runtime
        .current = 0,                 // set at runtime
        .high_water = 0,              // set at runtime
        .parent = 0,                  // set at runtime, will be syslimiter
    },   
    /// internal page print
    {
        .max = 0xffffffff,            // set at runtime
        .highest_max = 0xffffffff,    // as much as possible
        .lowest_max = 0x100000,       // minimum ram
        .strategy = printf_warning_retry_forever_strategy, //print_low_memory_strategy,  // may have a low memory strategy
        .active = 0,                  // set at runtime
        .current = 0,                 // set at runtime
        .high_water = 0,              // set at runtime
        .parent = 0,                  // set at runtime, will be syslimiter
    },     
    /// scan
    {
        .max = 0,
        .highest_max = 0xffffffff,
        .lowest_max = 0x100000,
        .strategy = 0,
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    /// fax
    {
        .max = 0,
        .highest_max = 0x800000,
        .lowest_max = 0,
        .strategy = printf_warning_return_null_strategy,
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },   
    /// thumbnails
    {
        .max = 0,
        .highest_max = 0x800000,  // upper limit
        .lowest_max  = 0x100000,  // lower limit
        .strategy = printf_warning_return_null_strategy,            // stategy 
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    /// network
    {
        .max = 0,
        .highest_max = 400 * 1024,  // upper and lower limit are the same
        .lowest_max  = 0x40000,  
        .strategy = printf_warning_return_null_strategy,           // strategy is return NULL
        .active = 0,  
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    /// small_limiter
    {
        .max = 0,
        .highest_max = 1 * 1024 * 1024,
        .lowest_max = 200 * 1024,
        .strategy = printf_warning_return_null_strategy, 
        .active = 0,
        .current = 0,
        .high_water = 0,
        .parent = 0,
    },
    { .max = 0, }
};

mlimiter_t *my_limiters[] = { 
    &my_limiters_data[0],
    &my_limiters_data[1],
    &my_limiters_data[2],
    &my_limiters_data[3],
    &my_limiters_data[4],
    &my_limiters_data[5],
    &my_limiters_data[6],
    &my_limiters_data[7],
    &my_limiters_data[8],
    0
};


KEY_TABLE_DEFINE_PRIVATE_2( mlimiter, mlimiter_t, mlimiter_key_table, my_limiters );

