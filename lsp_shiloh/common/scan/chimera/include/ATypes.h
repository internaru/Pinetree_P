/**
 * \file ATypes.h
 *
 * \brief Global type declarations.
 *
 */
/* $Header$
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
 * the following symbols are used in the compile.  External code using the scan
 * BSDK library need to use these symbols to call into the scan firmware.
 *
 * davep 15-Sep-2008
 */

#ifndef ATYPES_H
#define ATYPES_H

#ifndef __KERNEL__
typedef char CHAR;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef long SINT32;
typedef short SINT16;
typedef unsigned char UINT8;
typedef signed char SINT8;
typedef unsigned short WCHAR;
typedef float FLOAT;
typedef double DOUBLE;
typedef void * DEVICE_HANDLE;

typedef int ERROR_TYPE;

typedef void VOID;
//typedef char CHAR;

#define FAIL -1

#undef FALSE
#undef TRUE
typedef enum {
    FALSE=0,
    TRUE=1
} BOOL;

/* General defines */
#ifndef NULL
#define NULL 0
#endif

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

#endif /* __KERNEL__ */

#endif // _ATYPES_H

