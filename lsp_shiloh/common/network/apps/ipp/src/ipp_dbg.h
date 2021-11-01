/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef IPP_DBG_H
#define IPP_DBG_H

#define HAVE_DEBUG 1

#ifdef HAVE_DEBUG

#include "dprintf.h"
#define _ipp_dbg_prt(fmt, ...) dbg_printf(fmt, ## __VA_ARGS__)

#else

#include <stdio.h>
#define _ipp_dbg_prt(fmt, ...) fprintf(stderr, fmt, ## __VA_ARGS__)

#endif

#define ipp_dbg(fmt, ...)\
       _ipp_dbg_prt("IPP: "fmt"\n", ## __VA_ARGS__)
#endif /* IPP_DBG_H */
