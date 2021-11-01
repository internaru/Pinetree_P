/*
 *
 * ============================================================================
 * Copyright (c) 1998-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file zPJL_xxx.h
 *
 * \brief 
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ATypes.h"

typedef enum { R_ACCEPT, R_IGNORE, R_REJECT, R_ACCEPT_and_END, R_END } RANGE;

typedef struct {
	uint8_t state;		/* STATE */
	uint8_t aire;		/* RANGE (AIRE = Accept, Ignore, Reject, End) */
	uint8_t first;		/* lowest ASCII character in range */
	uint8_t last;		/* highest ASCII character in range */
	} STATE_RANGE;

/* internal job struct */
#define JOBNAMESIZE  80
#define COMMENTBUFSIZE 256
#define BADJOB  ((Uint32) 0xFFFFFFFFL)

typedef enum { JF_JOBNAME = 1, JF_EOJNAME = 2, 
    JF_JOBSTART = 4, JF_PRINTSTART = 8, 
    JF_JOBEND = 16, JF_PRINTEND = 32 
    } JOB_FLAGS;

typedef struct _Job
    {
    /* from engine - "good" value tracks FW job */
    Uint32  id;

    /* for this job - PJL JOB resets this */
    Uint32  pageCounter;

    /* PJL EOJ arguments */
    char  end[JOBNAMESIZE];

    /* PJL JOB arguments */
    char  start[JOBNAMESIZE];
    Uint32  job_start;
    Uint32  job_end;
    Uint16  job_password;

    /* PJL JOB incs, PJL EOJ decs */
    Byte  refCount;

    /* misc */
    Byte  flags;
    }
    Job;

typedef struct __ZPJL
    {
    /* our environment */
    zpjlContextInfo  zpjlContext;

    /* initial state for grammar state machine */
    const WORD_MATCH*  wm_FIRST;

    /* lexer/parser context */
    int  cPrev;
    int  State;
    STATE_RANGE*  pStateStart[ S_LAST+1 ];
    char  szWord[COMMENTBUFSIZE];	// make buffer larger for large comments.
    int  inWord;
    WORD_MATCH*  pMatchList;

    /* controls some internal initialization in zPJL_putc() */
    bool  First;

    /* buffered up by zPJL_DefaultParserNotify() */
    Modifier*  args; /*size set by zpjlContext.maxArgs; may be 0*/
    Uint32  argCount;

    /* buffered up by zpjlStatus() -- possibly indirectly */
    char**  outbound;  /*size set by zpjlContext.maxArgs; may be 0*/
    int  outboundSP; /*LI-FO*/
    char*  inProgress;  /*locked queue item being sent*/
    char*  inProgressBP;  /*base ptr of item being sent*/

    /* job queue */
    Job*  jobQueue;
    int  enQ;
    int  deQ;

    bool  enterLanguage;
    }
    _ZPJL;

typedef struct __ZPJLENV
    {
    Uint32  maxItems; /* num items alloc'd so far */
    Uint32  growSize; /* == 0 implies no growth (like ROM, or my savings account) */

    Uint32  numItems; /* num items used so far */
    ZPJL_EnvItem*  items; /* must not be NULL!! */

    struct __ZPJLENV*  antecedent; /* NULL for last one */
    }
    _ZPJLENV;

/* internal procs for notifies - kinda like DefWindowProc()
** -- the client can hook any of these
*/
Int32  zPJL_DefaultCommandNotify(ZPJL zpjl, Uint32 command, Uint32 argC, Modifier* argV);
Int32  zPJL_DefaultParserNotify(ZPJL zpjl, Uint32 command, Uint32 argC, void** argV);
Int32  zPJL_DefaultErrorNotify(ZPJL zpjl, Uint32 command, Uint32 argC, void** argV);

/* end listing */

#ifdef __cplusplus
}
#endif
