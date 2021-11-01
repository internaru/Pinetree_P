/*
 *  $RCSfile: cxshell.h,v $
 * $Revision: 1.13 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:28:15 $
 */
/************************************************************************
 *
 *      cxshell.h
 *
 *  Copyright 2008 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef _CXSHELL_H_
#define _CXSHELL_H_

#include "dc_type.h"

/********************************************
 *   OEM options:                           *
 ********************************************/

#define OEM
#define OEM_FAX_REPORT
#ifdef OEM_FAX_REPORT
 #define SIGNED_LEVEL
 #define RX_LOG  0
 #define TX_LOG  1
 #define CNG     0x11
 #define CED     0x10
 #define TCF     0x16
 #define IMG     0x26
 #define ANSam   0x36
 #define V8CM    0x46
 #define V8JM    0x56
 #define V8CJ    0x66
 #define V8CI    0x76
 #define V34P2   0x96
 #define V34P3   0xA6
 #define V34C1   0xB6
 #define V34CC   0xC6
 #define V34P1   0xD6
 #define V34PC   0xE6
 extern void CxProtocolReport( UINT8 fcf, UINT16 *buffer, UINT8 length, UINT8 direction );
#endif

#define ENABLE_EXT_PHONE_MONITOR
#define ENABLE_EXT_PHONE_RELAY_CONTROL


/********************************************
 *   Debug options:                         *
 ********************************************/

#define CX_DEBUG

#ifdef CX_DEBUG

 extern UINT8 _CxDebugLevel;


 #if defined(RTOS_LINUX)
  //#define CxDebugPrint printf
  #define CxDebugPrint
 #elif defined(RTOS_THREADX)
  void CxDebugPrint(const char *fmt, ...);
 #endif

 #define DEBUG_SHELL    1
 #define DEBUG_PHONE    2
 #define DEBUG_FAX      4
 #define DEBUG_HDLC     8
 #define DEBUG_MODEM    16
 #define DEBUG_SDAA     32
 #define DEBUG_IRQ      64
 #define DEBUG_DEBUG    128

 #define TRACE_SHELL  printf
 #define TRACE_PHONE  printf
 #define TRACE_FAX    printf
 #define TRACE_HDLC   printf
 #define TRACE_MODEM  printf
 #define TRACE_SDAA   printf
 #define TRACE_IRQ    printf
 #define TRACE_DEBUG  printf
 #define TRACE_HDLCDEBUG   printf
 #define TRACE_MODEMDEBUG  printf
 #define TRACE_SDAADEBUG   printf
 #define TRACE_IRQDEBUG    printf
 #define TRACE_CONTINUE_HDLC  printf
 #define TRACE_CONTINUE_IRQ   printf
 #define TRACE_ALWAYS     printf
 #define TRACE_FAXDEBUG     printf
 #define TRACE_T4CODEC     printf

 #ifdef   TRACE_ALWAYS
  #undef  TRACE_ALWAYS
 #endif
 #define  TRACE_ALWAYS CxDebugPrint

 #ifdef   TRACE
  #undef  TRACE
 #endif
 #define  TRACE        CxDebugPrint

#else

 #define TRACE_SHELL
 #define TRACE_PHONE
 #define TRACE_FAX
 #define TRACE_HDLC
 #define TRACE_MODEM
 #define TRACE_SDAA
 #define TRACE_IRQ
 #define TRACE_DEBUG
 #define TRACE_ERROR
#endif

#endif //_CXSHELL_H_
