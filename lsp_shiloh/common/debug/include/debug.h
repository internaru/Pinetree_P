/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved.
*
*               Marvell Confidential
 * ============================================================================
*/
/** 
 * \file debug.h
 *
 * \brief Header file for system debug tools, handy functions and macros to use while
 *  developing code.
 *
 **/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* DMA addresses need to bypass the cache */
#ifdef CACHE_BYPASS_OFFSET
    #define DRAM_START_DMA_ADDR (hwGetRamStartAddress() | CACHE_BYPASS_OFFSET)
    #define DRAM_LAST_DMA_ADDR (debug_get_dram_last_addr()) | CACHE_BYPASS_OFFSET)
#else
    #define DRAM_START_DMA_ADDR (hwGetRamStartAddress())
    #define DRAM_LAST_DMA_ADDR (debug_get_dram_last_addr())
#endif

#ifndef __linux__
#define IS_PTR_VALID(ptr)  ( ((uint32_t)(ptr)) >= hwGetRamStartAddress() \
                             && ((uint32_t)(ptr)) <= debug_get_dram_last_addr() ? 1 : 0 )
#define IS_DMAPTR_VALID(ptr) ( ((uint32_t)(ptr)) >= DRAM_START_DMA_ADDR \
                             && ((uint32_t)(ptr)) <= (DRAM_LAST_DMA_ADDR) ? 1 : 0 )

#define IN_INTERRUPT() (CPU_MODE_INTERRUPT == cpu_get_mode())
#else
// let the mmu do its job.
#define IS_PTR_VALID(ptr)  1
#define IS_DMAPTR_VALID(ptr) 1
#define IN_INTERRUPT() 0
#endif
#ifndef UNUSED_VAR
/*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
#define UNUSED_VAR(x) ((x) = (x))  
#endif
/**
 * \brief Macro for ease of coding "ToString_XXX(enum)" or
 *        "DecodeEnum_XXX(enum)" methods.
 */
#define CASE_ENUM_RETURN_STRING(enumerator)  \
        case enumerator: return #enumerator
/**
 * \def DEFAULT_CASE_ENUM_RETURN_STRING(enumType)
 * \brief Macro for the default case in "ToString_XXX(enum)" or
 *        "DecodeEnum_XXX(enum)" methods.
 * 
 */
#define DEFAULT_CASE_ENUM_RETURN_STRING(enumType)  \
        default: return "*** Unknown value for enum " #enumType " ***"


/**
 * \brief set last valid dram address
 *
 * \param addr new address of last valid DRAM memory
 *
 * I want dram_last_addr to be const so the compiler will complain if anyway
 * fiddles with it. But I still need to assign to it at run-time because we
 * won't know how much memory we have until we can examine the system.
 *
 * Call this function from main() once we know how much memory we have.
 *
 * dram_last_addr() is used to validate memory pointers.
 * see IS_PTR_VALID() in debug.h
 *
 */
void debug_set_dram_end( uintptr_t addr );

/**
 *
 * \brief get last valid dram address
 *
 * \return last valid address of DRAM memory
 *
 */
uintptr_t debug_get_dram_last_addr( void );

/**
 *
 * \brief Dump a named buffer to debug console.
 *  
 * /param start_addr Starting addresses of the buffer to dump. 
 * /param size Size in bytes of the buffer to dump. 
 * /param region_name Name of memory region to dump.  Displayed in the debug output. 
 */
void hex_dump_named(void *start_addr, int size, char* region_name);

/**
 *
 * \brief Dump a buffer to debug console.
 *  
 * /param start_addr Starting addresses of the buffer to dump. 
 * /param size Size in bytes of the buffer to dump. 
 */
static inline void hex_dump(void *start_addr, int size)
{
    hex_dump_named(start_addr, size, 0);
}


void dbg_console_task_init( void );

char const *DecodeSysMsgEnum( uint32_t msg );

/**
 * \brief register a main()-style function with the serial debug task 
 **/
typedef int (*dbg_cmd_f)(int argc, char *argv[]);

/** deprecated.  If you are looking for:
 * 1. HAVE_DBG_CALLBACK_CMDS
 * or 
 * 2. error_type_t dbg_register_cmd( char *cmd, dbg_cmd_f callback);
 * note that these utilities have been deprecated in favor of cmd_register()
 * in cmd_proc_api.h
 */


#ifdef DEBUG
/**
 * \brief Macro for zeroing out memory regions in debug builds.
 * It is compiled out in a release build for performance.
 *
 */
#define DBG_ZEROMEM(memptr, len) memset((memptr), 0, (len))
#else
#define DBG_ZEROMEM(memptr, len)
#endif

#ifdef __linux__
/**
 * \brief Routine to perform emergency HW halt and dump stack on segfaults.
 *
 */
void segfault_handler(int sig);
#endif

#ifdef __cplusplus
}
#endif

#endif
/*
 * End of debug.h
 */

