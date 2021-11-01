/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
#ifndef COMMONIO_H
#define COMMONIO_H

#ifndef be32_to_cpu
#define be32_to_cpu( in ) in
#endif

#define DBG_PRFX "Common:  "
#define LOGGER_SUBMOD 0x00000800
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

//#define DBG_PRINTF_NOTICE(...) DPRINTF(DBG_LOUD | DBG_OUTPUT, ( __VA_ARGS__ ))
//#define DBG_PRINTF_NOTICE(...) dbg_printf( __VA_ARGS__ )
//#define DBG_PRINTF_ERR(...) dbg_printf( __VA_ARGS__ )

#define ALIGNED_PRINT_MEMORY MEM_MALLOC_ALIGN_LIMITED
#define PRINT_MEMORY MEM_MALLOC_LIMITED

#endif // IO_H
