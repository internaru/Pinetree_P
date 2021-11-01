/***********************************************************
* (c) Copyright 1998-2008 Marvell International, Ltd. 
*
*                 Marvell Confidential
* ==========================================================
*/

/****************************************************************************\
**                                                                          **
**  zStr.h                                                                  **
**                                                                          **
**  A variable lengh string class                                           **
**  (NB: depends on definitions in zPJL.h)                                  **
**                                                                          **
\****************************************************************************/

#ifndef __ZSTR_H__
#define __ZSTR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ack-phht!! */
#include "zPJL_sym.h"
#include "zPJL.h"

#define  INITIAL_SIZE           8

typedef struct _zStr
    {
    MemAllocProcT  allocProc;
    MemFreeProcT  freeProc;
    Uint32  allocSize;
    Uint32  bytesUsed;
    char*  buffer;
    }
    zStr;

int  zStr_construct(zStr* zstr, zpjlSystem* system, const char* initialString);
int  zStr_new(zStr* zstr, zpjlSystem* system);
int  zStr_destruct(zStr* zstr);
char*  zStr_xfer(zStr* zstr);

int  zStr_putc(zStr* zstr, char c);
int  zStr_puts(zStr* zstr, Symbol ids);
int  zStr_putss(zStr* zstr, Symbol ids);

int  zStr_putstr(zStr* zstr, const char* s);

int  zStr_putn(zStr* zstr, Uint32 n);

/* okay if s = NULL */
int  zStr_putl(zStr* zstr, const char* s);

#ifdef __cplusplus
}
#endif

#endif /*__ZSTR_H__*/

/* end listing */
