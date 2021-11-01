/**
 * \file lassert.h
 *
 * \brief simple reimplementation of assert()
 *
 */
/* 
 * $Header: //unity/firmware/code/common/scan/include/scantypes.h#12 $
 *
 * ============================================================================
 * Copyright (c) 2007, 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/*
 * THIS IS A WORK IN PROGRESS!!!
 *
 * When the scan firmware is compiled into the BSDK BLOB (libscan-platform.a),
 * the following symbols and functions are used in the compile.  The external
 * BSDK user must create an interface using exactly these symbols.
 *
 * AssertFail
 * XAssertFail
 *
 * davep 15-Sep-2008
 */

#ifndef LASSERT_H
#  define LASSERT_H

#include <stdint.h>

typedef void (*assertfunc)(void);
extern int atassert( assertfunc f );

#  ifdef __cplusplus
      extern "C" void AssertFail(const char *, const char *, int);
      extern "C" void XAssertFail(const char *, const char *, int, uint32_t );
#  else
      extern void AssertFail(const char *, const char *, int);
      extern void XAssertFail(const char *, const char *, int, uint32_t );
#  endif
#else
#  undef ASSERT
#  undef XASSERT
#endif

#ifdef DEBUG
#  ifdef __STDC__
#    define ASSERT(e) ((e) ? (void)0 : AssertFail(#e, __FILE__, __LINE__))
#    define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail(#e, __FILE__,__LINE__,(errvalue)))
#  else
#    define ASSERT(e) ((e) ? (void)0 : AssertFail("e", __FILE__, __LINE__))
#    define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail("e", __FILE__,__LINE__,(errvalue)))
#  endif
#else
#  define ASSERT(ignore) ((void)0)
#  define XASSERT(ignore,errvalue) ((void)0)
#endif

