/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

//internalPage.h
#ifndef __INTERNALPAGE_H__
#define __INTERNALPAGE_H__

#include "ATypes.h"
#include "agRouter.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "agprint.h"
#include "agJobUtils.h"
#include "renderer.h"


#ifdef __cplusplus
extern "C" {
#endif
#define PAGE_WIDTH       4750

typedef enum  
{ 
    e_ConfirmRpt,
    e_ErrorRpt, 
    e_LastCall
} CALL_RPT_TYPE;

typedef enum  
{ 
   E_DISP, 
   E_HINT 
}DocElementType;

typedef struct DocElementTag{
    DocElementType type;
    int iValue;
    void *pValue;
    struct DocElementTag *next;
} DocElement;


typedef struct DocBufferTag{
    char *buf;
    struct DocBufferTag *next;
}DocBuffer;



void ackIPMCancel(UINT32 param3);
UINT32 checkIPMCancel(UINT32 timeOut);


#ifdef __cplusplus
}
#endif

#endif
