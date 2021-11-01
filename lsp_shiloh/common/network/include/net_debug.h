/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* supply default DBG_... family macros
 */

#ifndef _NET_DEBUG_HEADER_
#define _NET_DEBUG_HEADER_

#include "net_logger.h" // NET_LOGGER_SUBMOD_OVERALL (12)
#include "lassert.h"
#include "debug.h"

#ifndef LOGGER_MODULE_MASK // provide default setting if undefined
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT( 12 )  /* services */
#endif // LOGGER_MODULE_MASK

#ifdef DBG_RELEASE_ON
#define DBG_PRINTF_MACRO(...) dbg_printf(DBG_PRFX __VA_ARGS__)
#else
#define DBG_PRINTF_MACRO(...) dbg_printf(DBG_PRFX __VA_ARGS__)
#endif

#ifdef DBG_ON
#define DBG_MSG(...) DBG_PRINTF_NOTICE(__VA_ARGS__)
#else
#define DBG_MSG(...)
#endif
#ifdef DBG_ERR_ON
#define DBG_ERR(...) DBG_PRINTF_ERR(__VA_ARGS__) /* #define DBG_ERR(...) DBG_PRINTF_MACRO(__VA_ARGS__) */
#else
#define DBG_ERR(...)
#endif
#ifdef DBG_VERBOSE_ON
#define DBG_VERBOSE(...) DBG_PRINTF_DEBUG(__VA_ARGS__)
#else
#define DBG_VERBOSE(...) { (void)0; }
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#define DBG_CMD(...) cmd_printf(__VA_ARGS__) // omit module prefix for debug command output
#endif

#ifndef DBG_IGNORED
#define DBG_IGNORED(...) { (void)0; }
#endif

#endif // _NET_DEBUG_HEADER_
