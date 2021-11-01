/*
 * $Id: dc_type.h,v 1.3 2009/05/07 18:04:34 beliloi Exp $
 *
 * Copyright (C) 2009 Conexant Systems Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _dc_type_h_
# define _dc_type_h_
/**************************************************************************//**
*  @file dc_type.h
*  @brief This file contains the data type definitions used for the SDK.
*
*  For development purposes, support for WIN32 is included to allow source to
*  be compiled on the host. 
*
*  Compiler-specific attributes:
*
*  The PACKED data or datatype attribute means do not add any padding
*  to structs or unions for alignment, and assume addresses may be
*  non-aligned.  This attribute should be used only where absolutely
*  necessary; it leads to larger and slower code.
*
*  The INLINE function attribute means replicate function bodies at
*  call sites, to improve optimization for small frequently called
*  functions.  This attribute should be used only where there is
*  demonstrable benefit; it usually leads to larger code.
*
*  WEAK_FUNCTION declares a weakly bound function, ie a function
*  with a binding decided at link time.
*
*   EG Declare a weakly bound function returning void:
*   WEAK_FUNCTION(myfun, (UINT32 arg));
*
*  ARC_CC is used for specifying ARC compiler only calling convention
*  attributes, it expands to nothing in the case of non-ARC compilers.
*
*  A note regarding Mxx def's:
* 
*  Mxx def's (ie. MUINT8, MUINT16, etc.) should be mapped to the most code
*  efficient datatype for the CPU type which is at least as large as the
*  specified data type.  For instance, on CPU's with 32-bit registers, it is
*  probably most efficient to map all MUINTx to UINT32 and MINTx to INT32.
* 
*  The reason for this mapping is that sometimes it is awkward for 32 bit CPUs
*  to manipulate non-32bit data types.  For instance, if for loop is
*  constructed with UINT8 i as index, the i++ will typically require 2 CPU
*  instructions, one to perform the inc and a second to mask the top 24 bits
*  of the CPU register.  In this case, it is better to use a UINT32 as the
*  loop index variable.

******************************************************************************/
/******************************************************************************
*  FILE FORMAT: <PUBLIC>
*      1. INCLUDES 
*      2. ENUMERATIONS 
*      3. DEFINES
*      4. STRUCTURES
*      5. GLOBALS
*      6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/
/******************************************************************************
*  1. INCLUDES
******************************************************************************/
#include "ATypes.h"

/******************************************************************************
*  2. ENUMERATIONS
******************************************************************************/

//
// This enumeration list defines the priority levels used for the
// function, thread_priority_get() in the SDK. This enumeration is
// not intended to map to the operating system threads and should not
// be used as arguments to operating system function calls.
//
typedef enum
{
    DC_THREAD_PRIORITY_VERY_LOW=32,
    DC_THREAD_PRIORITY_LOW,
    DC_THREAD_PRIORITY_MEDIUM,
    DC_THREAD_PRIORITY_HIGH,
    DC_THREAD_PRIORITY_VERY_HIGH
} DC_THREAD_PRIORITY;

/******************************************************************************
*  3. DEFINES
******************************************************************************/
#ifndef TYPE_H

#define __ARM

# ifdef WIN32
#  ifndef _BASETSD_H  //This include file defines these typedefs as well.
typedef int INT32;
typedef unsigned int UINT32;
#  endif
typedef short INT16;
typedef unsigned short UINT16;
typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned char boolean;
typedef UINT32 MUINT8;
typedef INT32 MINT8;
typedef UINT32 MUINT16;
typedef INT32 MINT16;
typedef UINT32 MUINT32;
typedef INT32 MINT32;

typedef     UINT8    MACADDR[6] ;

#  define BITFIELD_LSBFIRST
#  define CACHE_BYPASS(x) ((UINT32)(x))
#  define CACHED_ADDRESS(x) ((UINT32)(x))
#  define ROM_CACHE_BYPASS(x) ((UINT32)(x))
#  define ROM_CACHED_ADDRESS(x) ((UINT32)(x))
#  define ARM_VOLATILE /**/
# endif //def WIN32

# ifdef __ARM
typedef int INT32;
//typedef unsigned int UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned char boolean;
typedef unsigned char BOOLEAN;
//typedef char BOOL;
typedef UINT32 MUINT8;
typedef INT32 MINT8;
typedef UINT32 MUINT16;
typedef INT32 MINT16;
typedef UINT32 MUINT32;
typedef INT32 MINT32;

typedef     UINT8    MACADDR[6] ;

#  define BITFIELD_LSBFIRST
#  define ARM_VOLATILE volatile
//#if defined(LINUX_APP)
extern int ADDRESS_IS_CACHEABLE(unsigned int x);
extern unsigned int CACHE_BYPASS(unsigned int x);
extern unsigned int CACHED_ADDRESS(unsigned int x);
//#else
//#  define ADDRESS_IS_CACHEABLE(x) (!(((UINT32)(x)) & 0x40000000))
//#  define CACHE_BYPASS(x) (((UINT32)(x)) | 0x40000000)
//#  define CACHED_ADDRESS(x) (((UINT32)(x)) & (~0x40000000))
//#  define ROM_CACHE_BYPASS(x) (x)
//#  define ROM_CACHED_ADDRESS(x) (x)
//#endif
# endif //def __ARM

# ifdef _ARC
typedef int INT32;
typedef unsigned int UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned char boolean;
typedef unsigned char BOOLEAN;
typedef UINT32 MUINT8;
typedef INT32 MINT8;
typedef UINT32 MUINT16;
typedef INT32 MINT16;
typedef UINT32 MUINT32;
typedef INT32 MINT32;

typedef     UINT8    MACADDR[6] ;

#  define BITFIELD_LSBFIRST
#  define ADDRESS_IS_CACHEABLE(x) (!(((UINT32)(x)) & 0x40000000))
#  define CACHE_BYPASS(x) (((UINT32)(x)) | 0x40000000)
#  define CACHED_ADDRESS(x) (((UINT32)(x)) & (~0x40000000))
#  define ROM_CACHE_BYPASS(x) (((UINT32)(x)) | 0x60000000)
#  define ROM_CACHED_ADDRESS(x) (((UINT32)(x)) & (~0x60000000))
#  define ARM_VOLATILE volatile
#  pragma Offwarn(684)           //Trigraph detected but not replaced

# endif //def _ARC

#define TYPE_H

#endif // TYPE_H

# if defined(FFIGEN) || defined(LINT)
#  define INLINE /**/
#  define PACKED /**/
#  define WEAK_FUNCTION(name, arglist) void name arglist
#  define WEAK_FUNCTION_FULL(ret, name, arglist, retVal) ret name arglist
#  define ARC_CC(_x_) /**/

# elif defined(WIN32)
#  ifdef PACKED
#   undef PACKED
#  endif
#  define PACKED /* VC supports a #pack pragma, but seems to lack a packed attribute. */
#  define INLINE static __inline
#  define WEAK_DATA(type, name) type name
#  define WEAK_FUNCTION(name, arglist) void name arglist
#  define WEAK_FUNCTION_FULL(ret, name, arglist, retVal) ret name arglist
#  define ARC_CC(_x_) /**/

# elif defined(__ARM)
#if defined(__GNUC__)
#  define PACKED __attribute__((packed))
//#  define INLINE static inline
#else
#  define PACKED __packed
#  define INLINE __inline
#endif
#  define WEAK_DATA(type, name) __attribute__((weak)) type name
#  define WEAK_FUNCTION(name, arglist) __attribute__((weak)) void name arglist
#  define WEAK_FUNCTION_FULL(ret, name, arglist, retVal) __attribute__((weak)) ret name arglist
#  define ARC_CC(_x_) /**/

# elif defined(_ARC)
#  define PACKED _Packed
#  define INLINE _Inline
#  define WEAK_DATA(type, name) pragma Weak(name); type name
#  define WEAK_FUNCTION(name, arglist) pragma Weak(name); void name arglist {;} pragma Off_inline(name);
#  define WEAK_FUNCTION_FULL(ret, name, arglist, retVal) pragma Weak(name); ret name arglist {return(retVal);}  pragma Off_inline(name);
#  define ARC_CC(_x_) _CC(_x_)
# endif

# ifndef FALSE
#  define FALSE 0
# endif //ndef FALSE

# ifndef TRUE
#  define TRUE 1
# endif //ndef TRUE

# ifndef NULL
#  define NULL ((void *) 0x00)
# endif //ndef NULL

# ifndef max
/** 
** Do not use pass functions to this macro where the return value is determined by another process.  This macro
** will evaluate the function twice. Once at the comparison and once on the return. 
*/
#  define   max(a,b)    (((a) > (b)) ? (a) : (b))
# endif //ndef max
# ifndef min
/** 
** Do not use pass functions to this macro where the return value is determined by another process.  This macro
** will evaluate the function twice. Once at the comparison and once on the return. 
*/
#  define   min(a,b)    (((a) < (b)) ? (a) : (b))
# endif //ndef min
# ifndef hexchar
#  define hexchar(d)      (((d) < 10) ? ((d) + '0') : (((d)-10) + 'a'));
# endif //ndef hexchar
# ifndef sizeent
#  define sizeent(_array) (sizeof(_array)/sizeof((_array)[0]))
# endif //ndef sizeent

# define STRIDE_FROM_PIXELS(X) ((((((X)+7)/8))+31) & ~31)
# define STRIDE_FROM_BYTES(X)  (((X)+31) & ~31)

/* Worst-case ratio for HW */
# define JD_MAX_EXPANSION 10
# define JD_WORSTCASE     10
# define LLJ_WORSTCASE    2
# define JPEG_WORSTCASE   1
# define SIMPLE_WORSTCASE 2

# define ALIAS(x,y) x

#define DWORD_ENDIAN_SWAP(__x__) (((__x__&0xff)<<24) | \
                                  ((__x__&0xff00)<<8) | \
                                  ((__x__&0xff0000)>>8) | \
                                  ((__x__&0xff000000) >> 24))

#define WORD_ENDIAN_SWAP(__x__) (((__x__&0xff)<<8) | \
                                 ((__x__&0xff00)<<8))


// Type of emulated button event.
typedef UINT8 UIDEF_key;
/******************************************************************************
*  4. STRUCTURES
******************************************************************************/
#if 0
# if !defined(WIN32) || defined(MSDEV_DEBUG)
typedef struct {
  UINT32 lo;
  UINT32 hi;
} UINT64;
# endif /* if !defined(WIN32) || defined(MSDEV_DEBUG) */
#endif

typedef union {
  UINT32 d;
  UINT16 w[2];
  UINT8 b[4];
} BWD_UNION;

typedef struct DC_MEM_BUFFER_STRUCT
{
    /* Buffer name for debugging purposes */
    char name[32];

    /* Base address of the static buffer */
    UINT32 base;

    /* Size in bytes of the static buffer */
    UINT32 size;

    /* 
    ** Alignment requirement for the static buffer in power of 2 bytes
    ** For instance, 32 byte is 2^5 so the alignment would be 5.
    */
    UINT32 alignment;
} DC_MEM_BUFFER_t;

/**
** This structure contains the different permanent static memory 
** buffers that could be required by SDK components. At 
** initialization time, these structures are passed to the setup 
** and init calls of SDK components for memory buffer reservation.
*/
typedef struct DC_MEM_PARAMS_STRUCT
{
    /*
    ** Read/write protected memory space
    */
    DC_MEM_BUFFER_t protect;

    /*
    ** This memory area is not read/write protected
    */
    DC_MEM_BUFFER_t unprotect;
} DC_MEM_PARAMS_t;
/******************************************************************************
*  5. GLOBALS
******************************************************************************/
/******************************************************************************
*  6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/

/*************************************************************************//**
*  This function maps the input priority level to an actual priority level
*  for the operating system.
*
*  @param priority - The desired priority level
*
*  @return Returns the operating system priority level
******************************************************************************/
UINT32 thread_priority_get(DC_THREAD_PRIORITY priority);
#endif /*  _dc_type_h_ */
