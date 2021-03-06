/*
 * ==========================================================
 * (c) Copyright 1998-2010 Marvell International, Ltd. 
 *
 *                 Marvell Confidential
 * ==========================================================
 */
/**
 * \file zPJL.h
 *
 * \brief zPJL library
 *
 */

#ifndef __ZPJL_H__
#define __ZPJL_H__

#include "stdlib.h"
#include "string.h"
#include "ATypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* controls access to internal state */
#define OPAQUE_ZPJL

typedef long            Int32;
typedef unsigned long   Uint32;
typedef unsigned short  Uint16;
typedef unsigned char   Byte;
typedef void*           RefCon;

typedef struct __ZPJL*  ZPJL;
typedef struct __ZPJLENV*  ZPJLENV;
typedef struct _zStr*  ZStrPtr;

/// parser table of registered variables.
typedef struct pjl_vars PJL_VARS;  

typedef enum
{
    e_NotDefined,
    e_VarEnum,
    e_VarNumber,
    e_VarString,
    e_VarCustom,
} PJL_VARIABLE_PROCESS;


typedef enum
{
    e_NormalVar,
    e_DefaultVar,
    e_FactDefaultVar
} PJL_VARIABLE_STORAGE;


#define TAB             0x09
#define LF	            0x0a
#define ESC	            0x1b
#define CR	            0x0d
#define FF	            0x0c

/* environment */  
///
/// WARNING !
/// environment's are NOT used PrintJobInfo_t is used instead.
/// factory defaults are stored by the Vars as are defaults both are
/// accessed via the OID interface and the Set variables and status registrations
/// are in the PrintJobInfo for system/pipe/job scoping.  
/// 
/// todo: remove the remaining vestages of environments while leaving the parser intact.
/// 
/*             *----------------------------------------------------------- */

/**
 * A "PJL Environment" is just an array of \<ZPJL_EnvItem\>'s (defined below).
 * These are fixed-size records.  Currently they are unsorted, but might be
 * sorted by ZPJL_EnvItem.id for faster lookup 
 *
 * There are 3 arrays for the ROM, the NVRAM and the current environments,
 * each with an item count.  Rather than actually copying the contents back
 * and forth a lot, we "imply" the copy by searching the tables in priority
 * order.  So, the count for a table may be 0 (which ends the search quickly).
 * Tables MIGHT GET REALLOCATED if they are grown so DO NOT keep the pointer!
 * Always use the pointer stored in the zPJL context.
 *
 * Note also that tables might be shared between contexts.  This is almost
 * always true of the ROM environment.
 */

/** symbol table (generated by SYMGEN utility) is extremely simple:
 * __SYMBOL_TABLE__[\<s\>] == "\<s\>" -- also, symbols are sorted
 */
typedef unsigned short  Symbol;
extern char*  __SYMBOL_TABLE__[];
extern unsigned short  __SYMBOL_TABLE_SIZE__;

#define SASIZE(symbolArray) (sizeof(symbolArray) / sizeof(Symbol))
extern char * (*LocalSymbols)[];
// Return the string from whichever table has it stored.
#define SYM(_k_)    (_k_ < __DEFAULT_SYMBOL_TABLE_SIZE__)? (__SYMBOL_TABLE__[(_k_)]):\
    (*LocalSymbols)[_k_-__DEFAULT_SYMBOL_TABLE_SIZE__]
#define NULL_SYMBOL     ((Symbol) -1)

typedef enum
    {
    EIT_TERMINATOR = 0,
    EIT_ENUM, /*!< value = data.dword*/
    EIT_NUMBER, /*!< value = data.dword*/
    EIT_RANGEVAL, /*!< value = data.triplet, plus bits from length field*/

    EIT_ENUM_RO, /*!< call (*(data.proc))() to get enum value; length field NOT valid*/
    EIT_INT_RO, /*!< call (*(data.proc))() to get int value; length field NOT valid*/

    EIT_CSTR, /*!< use data.ptr, 0 terminator; length field NOT valid*/

    /*unimplemented*/
    EIT_STR255, /*!< use data.triplet, no terminator; length field valid*/
    EIT_FOURCC, /*!< value = data.fourCC; length field NOT valid*/
    EIT_FLOAT, /*!< value = data.real*/
    EIT_DOUBLE, /*!< value = ((double*) data.ptr); length field NOT valid*/
    EIT_BINARY, /*!< valuePtr = data.ptr; length field valid*/
    EIT_BLOB, /*!< valuePtr = ((Byte*) data.ptr + 4); length field NOT valid*/
              /*!< valueLength = *((Uint32*) data.ptr)*/

    EIT_RESERVED1,
    EIT_RESERVED2
    }
    ZPJL_EnvItemType;

/* IDEA: store the EIT_ENUM value in length plus some bits borrowed from elsewhere
**   as needed (necessary only if  __SYMBOL_TABLE__ > 256; bits could be borrowed
**   from reserved field, or perhaps upper bits of data.ptr/.dword)
**   THEN, use data.ptr to point to a NULL_SYMBOL terminated range table (ROM'able)
**   OR, just have data.ptr point to (currVal, rangeTableSize, rangeTable)
**
** THIS would allow for automatic info dumping and enum range checking
*/

typedef int  (*EvaluateProcT)(ZPJL zpjl);

/* rLLLLLUU UUUUUUUU UUUVVVVV VVVVVVVV */
/** lower is 5 MSB of rangeLB; length field forms 8 LSB of rangeLB */
typedef struct
    {
    Uint32  value : 13;
    Uint32  upper : 13;
    Uint32  lower : 5;
    Uint32  reserved : 1;
    }
    range_triplet;

/* each environment variable is 8 bytes */
typedef struct _ZPJL_EnvItem
    {
    /** 24 bits key (20 bits used) */
    Uint32  id : 16;		/*!<  variable name symbol ID */
    Uint32  type : 4;		/*!<  any of ZPJL_EnvItemType */
    Uint32  reserved : 4;

    /** 8 bits aux data */
    Uint32  length : 8;

    /** 32 bits data */
    union
        {
        Uint32  dword;
        void*  ptr;
        range_triplet  triplet;
        EvaluateProcT  proc;
        Byte  fourCC[4];
        float  real;
        }
        data; /*!< depends on type bitfield*/
    }
    ZPJL_EnvItem;

#define VAL_MASK        0x1FFF
#define UP_MASK         0x03FFE000L
#define LO_MASK         0x7C000000L
#define LOLO_MASK       0x1F00L

#define MAKE_RANGE_VAL(_val_, _lo_, _hi_ ) \
    { \
    ((_val_) & VAL_MASK) |\
    (((_hi_) & VAL_MASK)<<13) |\
    (((_lo_) & LOLO_MASK)<<18) \
    }

#define MAKE_RANGE_LOW_8LSB(_lo_) \
    ((Byte) ((_lo_) & 0xFF))

/**	Short-hand macros for defining ZPJL_EnvItem structures */
#define ENV_ITEM_RANGEVAL(_sym_, _val_, _lo_, _hi_ ) \
    { ((Symbol) _sym_), EIT_RANGEVAL, 0, MAKE_RANGE_LOW_8LSB(_lo_), MAKE_RANGE_VAL(_val_, _lo_, _hi_) }

#define ENV_ITEM_ENUM(_symkey_, _symval_) \
    { ((Symbol) _symkey_), EIT_ENUM, 0, 0, { (_symval_) } }

#define ENV_ITEM_NUMBER(_symkey_, _symval_) \
    { ((Symbol) _symkey_), EIT_NUMBER, 0, 0, { (_symval_) } }

#define ENV_ITEM_ENUM_RO(_symkey_, _routine_) \
    { ((Symbol) _symkey_), EIT_ENUM_RO, 0, 0, { ((unsigned long) (_routine_)) } }

#define ENV_ITEM_INT_RO(_symkey_, _routine_) \
    { ((Symbol) _symkey_), EIT_INT_RO, 0, 0, { ((unsigned long) (_routine_)) } }

#define ENV_ITEM_CSTR(_symkey_, _p_) \
    { ((Symbol) _symkey_), EIT_CSTR, 0, 0, { ((unsigned long) (_p_)) } }

#define ENV_ITEM_TERMINATOR \
    { 0, 0, 0, 0, { 0 } }

typedef enum
    {
    ES_NONE,
    ES_ROM,
    ES_NVRAM,
    ES_RAM,
    ES_PDL,
    ES_USTATUS
    }
    ZPJL_EnvSpec;

/* PJL grammar */
/*             *----------------------------------------------------------- */

/** grammar states */
typedef enum
    {
    S_ERROR, S_FIRST, 
    S_START_WORDMATCH, S_WORDMATCH, S_START_WORD, S_WORD, 
    S_START_NUMBER, S_NUMBER,
    S_START_QUOTE, S_QUOTED,
    S_COMMENT, S_LAST
    }
    STATE;

/* for now, don't be too picky; but code the grammar correctly */
#define S_EOL   S_START_WORDMATCH

/** notify proc messsages */
typedef enum
    {
    A_UNKNOWN,
    A_COMMAND, A_KEY, A_VALUE, A_STRING, A_NUMBER,
    A_PJL, A_EXECUTE, A_UNBUFFER, A_DATAREADY, A_COMPLAIN,
    A_STATUSCHANGE, A_TIMED, A_JOBSTARTED, A_JOBCOMPLETE, A_PAGE,
    A_LAST
    }
    ACTION;

/** diagnostic proc messsages */
typedef enum
    {
    D_UNKNOWN,
    D_DEBUG_MESSAGE,
    D_LAST
    }
    DIAGNOSTIC;

typedef struct _WORD_MATCH
    {
	Symbol  key;

    /* action[0] signifies semantic meaning (e.g., A_COMMAND) */
    /* action[1] is for app-specific use */
    Byte  action[2];

	int  NextState;
	struct _WORD_MATCH**  wm_Next;
	}
    WORD_MATCH;

#define END_WORD_MATCH\
    { NULL_SYMBOL }

/* PJL context */
/*             *----------------------------------------------------------- */

typedef struct _Modifier Modifier;

#define DEFAULT_ALLOCFLAGS      0

/** refcon is passed thru; it may be NULL/garbage */
typedef void*  (*MemAllocProcT)(Uint32 numBytes, Uint32 flags, RefCon refcon);
typedef int  (*MemFreeProcT)(void* p, RefCon refcon);
typedef int  (*DiagnosticProcT)(void* p, RefCon refcon);
typedef Uint32  (*StatusProcT)(Uint32 flags, RefCon refcon);
typedef Uint32  (*NotifyProcT)(void* p, RefCon refcon);

/** from caller's perspective, this is for INITIALIZATION */
typedef struct
    {
    /** heap management */
    MemAllocProcT  memAllocProc; /*!< (REQUIRED) */
    MemFreeProcT  memFreeProc;   /*!< (REQUIRED) */

    /** system error / warnings */
    DiagnosticProcT  diagnosticProc; /*!< OPTIONAL, set to NULL if not used */

    /** system interface - \<flags\> indicates status request */
    StatusProcT  getStatus; /*!< OPTIONAL, set to NULL if not used */

    /** system interface */
    NotifyProcT  timerChange; /*!< OPTIONAL, set to NULL if not used */

    /** set up for caller; otherwise not touched */
    RefCon  refcon; /*!< OPTIONAL */
    }
    zpjlSystem;

    typedef Int32  (*CallbackProcCmdT)(ZPJL zpjl, Uint32 msg, Uint32 argC, Modifier* argV);
    typedef Int32  (*CallbackProcT)(ZPJL zpjl, Uint32 msg, Uint32 argC, void** argV);

#define  ZPJL_CONTEXT_DEFAULTFLAGS      (0)

/* from caller's perspective, this is for INITIALIZATION */
typedef struct
    {
    /** system callbacks (REQUIRED - see struct def) */
    zpjlSystem  system;

    /** client callbacks (REQUIRED - set to NULL for no grammar) */
    const WORD_MATCH*  rootGrammar;

    /** size of args cache (REQUIRED - set to 0 for empty cache) */
    Uint32  maxArgs;

    /** size of outbound message stack (REQUIRED - set to 0 for empty cache) */
    Uint32  maxOutbound;

    /** size of job queue (REQUIRED - set to 0 for empty cache) */
    Uint32  maxJobQueue; /*!< defaults to 2 */

    /** controls operation (OPTIONAL - set to ZPJL_CONTEXT_DEFAULTFLAGS if n/a) */
    Uint32  flags;

    /** the evironments */
    ZPJLENV  zpenv_ROM;     /*!< (REQUIRED) (TODO: make this implicit; and require others) */
    ZPJLENV  zpenv_DEFAULT; /*!< OPTIONAL, set to empty if not used */
    ZPJLENV  zpenv_CURRENT; /*!< OPTIONAL, set to empty if not used */
    ZPJLENV  zpenv_USTATUS; /*!< (REQUIRED) */

    /** main hook - signals output ready */
    /** return TRUE to call default handler proc (usually always) */
    /** (default proc clears arg cache) */
    CallbackProcCmdT  commandNotify; /*!< OPTIONAL, set to NULL if not used */

    /** extra hook - parser got something */
    /** client may hook this, but should always signal that */
    /** (default proc buffers up tokens in arg cache) */
    CallbackProcT  parserNotify; /*!< OPTIONAL, set to NULL if not used */

    /** diagnostic hook - parser problems */
    /** client must hook this for diagnostic info */
    CallbackProcT  errorNotify; /*!< OPTIONAL, set to NULL if not used */
    }
    zpjlContextInfo;

/** for direct calls to notify proc */
typedef enum { VT_COMMAND, VT_SYMBOL, VT_NUMBER, VT_STRING } VALUE_TYPE;

/** for direct calls to notify proc */
struct _Modifier
    {
    union
        {
        Symbol  symbol;
        Uint32  number;
        char*  string;
        }
        value;

    Uint16  type; /*!<VALUE_TYPE*/
    Symbol  key;
    };

/* ------------------------------------------------------------------------ */

/* this is generally kept from public view; clients should not need it */
#ifndef OPAQUE_ZPJL
#include "zPJL_xxx.h"
#endif

/* ------------------------------------------------------------------------ */

/* class methods */

/** one-time module init; callback is NULL for now */
Int32  zpjlStartup(CallbackProcT startupCallback);
Int32  zpjlShutdown(CallbackProcT shutdownCallback);

/** input zpjlContextInfo is copied into zpjl, so caller may discard it */
Int32  zpjlConstruct(zpjlContextInfo* zpjlContext, ZPJL* zpjl);
Int32  zpjlDestruct(ZPJL* zpjl);

/** default grammar (EZ generic PJL interpreter) */
Int32  zPJL_ConstructDefault(ZPJL* zpjl, void* refCon, CallbackProcT commandProc);

/** if \<growSize\> == 0, initTable pointer is copied; otherwise, a table of */
/**   size \<length\> is made and initTable is copied (if it's not NULL) */
Int32  zpjlConstructEnv(zpjlSystem* systemContext, ZPJLENV* zpenv, ZPJLENV baseEnv,
     ZPJL_EnvItem* initTable, Uint32 length, Uint32 growSize);
Int32  zpjlDestructEnv(zpjlSystem* systemContext, ZPJLENV* zpenv);

/* instance methods (processing) */

/** caller supplies its own UEL sniffer (see Sniffer.cpp for an example)
** returns:
**     ZPJL_MORE if we are still in PJL mode (i.e., more input required)
**     ZPJL_PDL if we are exiting PJL mode to enter the PDL
**     ZPJL_CHOKE if we are exiting PJL mode because we gagged
*/
Int32  zpjlPutc(ZPJL zpjl, int c);

/** caller is notified via commandNotify proc that outbound data is ready
** returns the character for the output buffer (NULL if no data ready)
** if \<more\> != NULL, \<more\> set TRUE iff more data is ready
*/
Byte  zpjlGetc(ZPJL zpjl, int* more);

/** caller is notified via commandNotify proc that data is ready */
int  zpjlGotData(ZPJL zpjl);

void  zpjlEnterLanguage(ZPJL z);

/** parser utilities -- suck typed data from arg list */
Symbol  zpjlGetCommand(Uint32 argC, Modifier* args);
Symbol  zpjlGetKey(Uint32 argC, Modifier* args, Uint32 argNum);
Symbol  zpjlSymbolArg(Uint32 argC, Modifier* args, Symbol argID);
Uint32  zpjlIntArg(Uint32 argC, Modifier* args, Symbol argID);

char*  zpjlStringArgEx(Uint32 argC, Modifier* args, Symbol argID, bool xfer);
/**xfers ownership by default!*/
#define zpjlStringArg(argC, args, argID) \
    zpjlStringArgEx(argC, args, argID, TRUE)

Int32  zpjlZapEnv(zpjlSystem* systemContext, ZPJLENV zpe);
ZPJLENV  zpjlEnvironment(ZPJL z, ZPJL_EnvSpec es);
ZPJL_EnvItem*  zpjlFindEV(ZPJL z, ZPJL_EnvSpec esStart, Symbol s, bool cascade);
ZPJL_EnvItem*  zpjlFindVar(ZPJLENV zpenv, Symbol s);
ZPJL_EnvItem*  zpjlCopyVar(ZPJL z, ZPJL_EnvSpec destES, ZPJL_EnvSpec sourceES, Symbol s);
ZPJL_EnvItem*  zpjlMakeVar(ZPJL z, ZPJL_EnvSpec es, Symbol s);

bool  zpjlPutRangeVal(ZPJL_EnvItem* ev, Uint32 value, bool  allowZero);
/*bool  zpjlPutEnumVal(ZPJL_EnvItem* ev, Symbol s);*/
bool  zpjlPutEnumVal(ZPJL_EnvItem* ev, Symbol s,  const Symbol* rangeTable, Uint32 rangeTableSize);

bool  zpjlPutIntVal(ZPJL_EnvItem* ev, Uint32 n);
Uint32  rangeValue(ZPJL_EnvItem* ev);
Uint32  rangeUpper(ZPJL_EnvItem* ev);
Uint32  rangeLower(ZPJL_EnvItem* ev);

/* handy output helpers */
/** output environment variable */
void  outputEV(ZPJL z, Symbol command, ZPJL_EnvItem* ev);
/** output config list */
void  outputCL(ZStrPtr zstr, Symbol category,  Symbol* list, Uint32 listSize, int value);
/** output key=numbervalue */
void  outputKeqV(ZStrPtr zstr, Symbol id, Uint32 value);
/** INFO VARIABLE output for a range variable */
void  outputIV_range(ZPJLENV zpe, ZStrPtr zstr, Symbol id);
/** INFO VARIABLE output for an enum variable */
void  outputIV_enum(ZPJL zpjl, ZPJLENV zpe, ZStrPtr zstr, Symbol id,  Symbol* list, Uint32 listSize, bool readOnly);

/** call this to generate outbound status data */
void  zpjlStatus(ZPJL zpjl);
void  zpjlJobStarted(ZPJL zpjl, Uint32 jobId);
void  zpjlJobComplete(ZPJL zpjl, Uint32 jobId, Uint32 whichCopy);
void  zpjlPage(ZPJL zpjl, Uint32 n, Uint32 jobId, Uint32 whichCopy);
/** call this periodically for USTATUS TIMED */
void  zpjlTick(ZPJL zpjl);

/** call this to queue outbound status data */
/* TODO: semaphore between zpjlGetc() and zpjlBuffer() */
int  zpjlBuffer(ZPJL zpjl, char* s);

/** downloadable symbol table */
typedef enum { LST_AT_REPLACE, LST_AT_APPEND } LST_Option;
Int32  zpjlLoadSymbolTable(unsigned short count, char** symbols, LST_Option option);
char*  zpjlSym(Symbol s);

/* stuff */
RefCon  zpjlRefcon(ZPJL zpjl);
zpjlSystem *zpjlSys(ZPJL zpjl);
int doOutput(char *string);
bool FindParser(char *String);
PJL_VARIABLE_PROCESS FindVariableType(unsigned short Key);
PJL_VARS *FindVariable(uint16_t Key);

void setUstatusJob(bool Value);
void setUstatusPage(bool Value);
void setUstatusTimed(uint32_t Time);
bool isUstatusDeviceOn(void);
bool isUstatusPageOn(void);
bool isUstatusJobOn(void);
unsigned int UstatusTimedState(void);
void setUstatusDevice(bool value);
void getCurStatus(char *string);


/* constants */
/*           *------------------------------------------------------------- */

#define  BAD_ZPJL                       ((ZPJL) NULL)
#define  BAD_ZPJLENV                    ((ZPJLENV) NULL)

#define  ZPJL_OK                        0
#define  ZPJL_OKAY                      0

#define  ZPJL_CONSTRUCT_FAILED          (-1000)
#define  ZPJL_ENVCONSTRUCT_FAILED       (-1001)
#define  ZPJL_ENVDESTRUCT_FAILED        (-1002)
#define  ZPJL_CONSTRUCT_FALTERED        (-1003)

#define  ZPJL_DESTRUCT_FAILED           (-1004)

#define  ZPJL_MORE                      (1)
#define  ZPJL_PDL                       (0)
#define  ZPJL_CHOKE                     (-1)
#define  ZPJL_CANCEL                    (55)

#define  ZPJL_WARNING_SENTINEL          ((Uint32) -1L)

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif /*__ZPJL_H__*/
