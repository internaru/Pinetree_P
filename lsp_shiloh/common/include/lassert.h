/*
 * ============================================================================
 * Portions Copyright (c) 2008-2014   Marvell International, Ltd. 
 *                      All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 * assert.h: ANSI 'C' (X3J11 Oct 88) library header section 4.2 
 * Copyright (C) Codemist Ltd., 1988-1993                       
 * Copyright 1991-1993 ARM Limited. All rights reserved.        
 * version 0.04 
 *
 */
/** 
 * \file lassert.h
 * 
 * \brief The assert macro puts diagnostics into programs.
 * 
 * When it is executed, if its argument expression is false, it writes 
 * information about the call that failed (including the text of the argument,
 * the name of the source file, and the source line number - the latter 
 * are respectively the values of the preprocessing macros __FILE__ and __LINE__) 
 * on the standard error stream. It then calls the abort function.If its 
 * argument expression is true, the assert macro returns no value.
 * 
 * \note "lassert.h" may be included more that once in a program with
 * different setting of NDEBUG. Hence the slightly unusual first-time
 * only flag.
 *
 */

#ifndef __assert_h
#  define __assert_h
#  ifdef __cplusplus
      extern "C" void _assert(const char *, const char *, int);
      extern "C" void XAssertFail(const char *, const char *, int, unsigned long int);
#  else
      extern void _assert(const char *, const char *, int);
      extern void XAssertFail(const char *, const char *, int, unsigned long int);
#  endif
#else
#  undef assert
#  undef XASSERT
#endif

#ifndef __DBG_ASSERT_DEFINED__ /// only define the function once.
  #define __DBG_ASSERT_DEFINED__ 1
  static __inline__  void _assert_dummy( const char *expression, const char *file, int line )
  {}
  static __inline__ void XAssertFail_dummy(const char *expression, const char *file, int line,
					 unsigned long int errvalue)
  {}
#  endif // __DBG_ASSERT_DEFINED__



///< REL_ASSERT is NEVER removed.
#define REL_ASSERT(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
#define REL_XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail(#e, __FILE__,__LINE__,(errvalue)))

///< ASSERT may be removed based on the system design.
#if  defined DEBUG || defined REL_ASSERT_ON
#  ifdef __STDC__
#    define ASSERT(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
#    define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail(#e, __FILE__,__LINE__,(errvalue)))
#  else
#    define ASSERT(e) ((e) ? (void)0 : _assert("e", __FILE__, __LINE__))
#    define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail("e", __FILE__,__LINE__,(errvalue)))
#  endif
#else // release
#  if 0 //Enable this if want to use preprocessor removal instead of optimizer: different warnings
#    define ASSERT(ignore) ((void)0)
#    define XASSERT(ignore,errvalue) ((void)0)
#  else
#    ifdef __STDC__
#      define ASSERT(e) ((e) ? (void)0 : _assert_dummy(#e, __FILE__, __LINE__))
#      define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail_dummy(#e, __FILE__,__LINE__,(errvalue)))
#    else
#      define ASSERT(e) ((e) ? (void)0 : _assert_dummy("e", __FILE__, __LINE__))
#      define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail_dummy("e", __FILE__,__LINE__,(errvalue)))
#    endif // __STDC
#  endif // inline assert approach
#endif


///< DBG_ASSERT is ALWAYS removed in shipping systems.
#ifdef DEBUG
#  ifdef __STDC__
#    define DBG_ASSERT(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
#    define DBG_XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail(#e, __FILE__,__LINE__,(errvalue)))
#  else
#    define DBG_ASSERT(e) ((e) ? (void)0 : _assert("e", __FILE__, __LINE__))
#    define DBG_XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail("e", __FILE__,__LINE__,(errvalue)))
#  endif
   /// check for OK return else assert on programing error without extra variable  
#  define DBG_ISZERO_CHECK(e) DBG_ASSERT( 0==e )  
#else ///< REL or ! DEBUG is intended for the release system
#  if 0 //Enable this if want to use preprocessor removal instead of optimizer: different warnings
#    define DBG_ASSERT(ignore) ((void)0)
#    define DBG_XASSERT(ignore,errvalue) ((void)0)
#  else
#    ifdef __STDC__
#      define DBG_ASSERT(e) ((e) ? (void)0 : _assert_dummy(#e, __FILE__, __LINE__))
#      define DBG_XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail_dummy(#e, __FILE__,__LINE__,(errvalue)))
#    else
#      define DBG_ASSERT(e) ((e) ? (void)0 : _assert_dummy("e", __FILE__, __LINE__))
#      define DBG_XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail_dummy("e", __FILE__,__LINE__,(errvalue)))
#    endif // __STDC
#  endif // inline assert approach
   /// disable assert on return without removing the call like an assert would do.
#  define DBG_ISZERO_CHECK(e) e
#endif // release 

/* 
 * \note The following should only be included once or the typedef gives a
 * redefinition error
 */
#ifndef ASSERT_H_ONLY_ONCE
#define ASSERT_H_ONLY_ONCE
typedef void (*assertfunc)(void);
extern int atassert( assertfunc f );
#endif
