/** @file  type_def.h 
  * @brief This file contains definition for general data types
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved  
  */

#ifndef _TYPE_DEF_H
#define _TYPE_DEF_H

//#include <sys/types.h>
//#include <asm/types.h>
#include "marvellTypeMap.h"

/** Unsigned char */
#define u8      unsigned char
/** Unsigned short */
#define u16     unsigned short
/** Unsigned long integer */
#define u32     unsigned int
/** Unsigned long long integer */
#define u64     unsigned long long

/** Character pointer */
#define PCHAR   char *
/** Unsigned character pointer */
#define PUCHAR  unsigned char *
/** Unsigned character */
#define UCHAR   unsigned char
/** Unsigned integer */
#define UINT    unsigned int
/** enum : Boolean */
#ifndef BOOL
typedef enum
{
    TRUE = 0,
    FALSE = 1
} BOOL;
#define BOOL BOOL
#endif

#endif /* _TYPE_DEF_H */
