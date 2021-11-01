/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file ATypes.h
 *
 * \brief Global type declarations.
 *
 */

#ifndef _ATYPES_H
#define _ATYPES_H

#include "error_types.h"
#ifndef __linux__
#include "tx_utils.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define ALIGN(x) __attribute__ ((aligned (x)))
#define ALIGN4 ALIGN(4)
#define ALIGN8 ALIGN(8)
#define ALIGN16 ALIGN(16)
#define ALIGN32 ALIGN(32)

#ifndef INLINE
#define INLINE __inline__
#endif

#ifdef __linux__
typedef char CHAR;
typedef unsigned int UINT;
#else
#if !defined(TX_PORT) && !defined(TX_PORT_H)
typedef char CHAR;
typedef unsigned int UINT;
#endif
#endif

typedef enum
{
    eOff = 0,
    eOn,
    eAuto    
} onoffEnum;

typedef enum
{
    eNo = 0,
    eYes
} yesnoEnum;


//typedef short INT16;
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
typedef unsigned long long UINT64;


#undef FALSE
#undef TRUE
#undef BOOL
// --------------- Boolean declarations --------------- BEGIN ---------------
typedef enum
{
    FALSE=0,
    TRUE=1
}_BOOL;
#define BOOL _BOOL

/**
 * \brief Runtime method to return the string "TRUE" or "FALSE" based on
 *        the truth value of the given boolean condition. (inline)
 *        Argument does not necessarily have to be of type BOOL.
 */
#define ToStringCond(condition) ((condition) ? "TRUE" : "FALSE")
// The above form is preferred. Method names should begin with upper case.
// Delete the following when all references are changed.
#define toStringCond(cond) ((cond) ? "TRUE" : "FALSE")
// --------------- Boolean declarations --------------- END ---------------

/** Return codes */

typedef enum _ATSTATUS
{
    AT_SUCCESS = 0x00,          //TX_SUCCESS
    AT_DELETED = 0x01,          //TX_DELETED
    AT_NO_MEMORY = 0x10,        //TX_NO_MEMORY
    AT_POOL_ERROR = 0x02,       //TX_POOL_ERROR
    AT_PTR_ERROR = 0x03,        //TX_PTR_ERROR
    AT_WAIT_ERROR = 0x04,       //TX_WAIT_ERROR
    AT_SIZE_ERROR = 0x05,       //TX_SIZE_ERROR
    AT_GROUP_ERROR = 0x06,      //TX_GROUP_ERROR
    AT_NO_EVENTS = 0x07,        //TX_NO_EVENTS
    AT_OPTION_ERROR = 0x08,     //TX_OPTION_ERROR
    AT_QUEUE_ERROR = 0x09,      //TX_QUEUE_ERROR
    AT_QUEUE_EMPTY = 0x0A,      //TX_QUEUE_EMPTY
    AT_QUEUE_FULL = 0x0B,       //TX_QUEUE_FULL
    AT_SEMAPHORE_ERROR = 0x0C,  //TX_SEMAPHORE_ERROR
    AT_NO_INSTANCE = 0x0D,      //TX_NO_INSTANCE
    AT_THREAD_ERROR = 0x0E,     //TX_THREAD_ERROR
    AT_PRIORITY_ERROR = 0x0F,   //TX_PRIORITY_ERROR
    AT_START_ERROR = 0x10,      //TX_START_ERROR
    AT_DELETE_ERROR = 0x11,     //TX_DELETE_ERROR
    AT_RESUME_ERROR = 0x12,     //TX_RESUME_ERROR
    AT_CALLER_ERROR = 0x13,     //TX_CALLER_ERROR
    AT_SUSPEND_ERROR = 0x14,    //TX_SUSPEND_ERROR
    AT_TIMER_ERROR = 0x15,      //TX_TIMER_ERROR
    AT_TICK_ERROR = 0x16,       //TX_TICK_ERROR
    AT_ACTIVATE_ERROR = 0x17,   //TX_ACTIVEATE_ERROR
    AT_THRESH_ERROR = 0x18,     //TX_THRESH_ERROR
    AT_SUSPEND_LIFTED = 0x19,   //TX_SUSPEND_LIFTED
    AT_WAIT_ABORTED = 0x1A,     //TX_WAIT_ABORTED
    AT_WAIT_ABORT_ERROR = 0x1B, //TX_WAIT_ABORTED_ERROR
    AT_MUTEX_ERROR = 0x1C,      //TX_MUTEX_ERROR
    AT_NOT_AVAILABLE = 0x1D,    //TX_NOT_AVAILABLE
    AT_NOT_OWNED = 0x1E,        //TX_NOT_OWNED
    AT_INHERIT_ERROR = 0x1F,    //TX_INHERIT_ERROR
    AT_FAILURE = 0x1000,
    AT_PENDING = 0x1001,
    AT_INVALID_PARAM = 0x1002,
	AT_ERROR = 0x1003
}ATSTATUS;


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


#ifdef __cplusplus
}
#endif

#endif // _ATYPES_H
