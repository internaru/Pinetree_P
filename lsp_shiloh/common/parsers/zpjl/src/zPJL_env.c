/***********************************************************
* (c) Copyright 1998-2008 Marvell International, Ltd. 
*
*                 Marvell Confidential
* ==========================================================
*/

/****************************************************************************\
**                                                                          **
**  zPJL_env.c                                                              **
**                                                                          **
\****************************************************************************/

#include <stdbool.h>
#include <string.h>

#include "zPJL.h"
#include "zPJL_xxx.h"

/* user-defined symbol table */
#include "zPJL_sym.h"


Int32  zpjlConstructEnv(zpjlSystem* systemContext, ZPJLENV* zpenv, ZPJLENV baseEnv,
     ZPJL_EnvItem* initTable, Uint32 length, Uint32 growSize)
    {
    _ZPJLENV*  zpe;

    if (NULL == systemContext || systemContext->memAllocProc == NULL
        || systemContext->memFreeProc == NULL)
        return ZPJL_ENVCONSTRUCT_FAILED;

    zpe = (_ZPJLENV*) (*systemContext->memAllocProc)(
        sizeof(_ZPJLENV), DEFAULT_ALLOCFLAGS, systemContext->refcon);

    /* alloc item tablem, if needed */
    if (0 == growSize)
        zpe->items = (ZPJL_EnvItem*) initTable;
    else
        {
        zpe->items = (ZPJL_EnvItem*) (*systemContext->memAllocProc)(
            sizeof(ZPJL_EnvItem) * length, DEFAULT_ALLOCFLAGS, systemContext->refcon);

        if (NULL == zpe->items)
            {
            (*systemContext->memFreeProc)(zpe, systemContext->refcon);
            zpe = NULL;
            }
        else if (NULL != initTable)
            memcpy(zpe->items, initTable, sizeof(ZPJL_EnvItem) * length);
        }

    if (NULL != zpe)
        *zpenv = zpe;
    else
        return ZPJL_ENVCONSTRUCT_FAILED;

    zpe->growSize = growSize;
    zpe->maxItems = length;
    zpe->numItems = (NULL == initTable) ? 0 : length;
    zpe->antecedent = baseEnv;

    return ZPJL_OK;
    }

Int32  zpjlDestructEnv(zpjlSystem* systemContext, ZPJLENV* zpenv)
    {
    if (BAD_ZPJLENV == *zpenv)
        return ZPJL_OK;

    if (NULL == systemContext || systemContext->memFreeProc == NULL)
        return ZPJL_ENVDESTRUCT_FAILED;

    if (0 != (*zpenv)->growSize)
        (*systemContext->memFreeProc)((*zpenv)->items, systemContext->refcon);

    if ((*systemContext->memFreeProc)(*zpenv, systemContext->refcon))
        *zpenv = BAD_ZPJLENV;

    return ZPJL_OK;
    }

Int32  zpjlZapEnv(zpjlSystem* systemContext, ZPJLENV zpe)
    {
    if (0 == zpe->growSize)
        return FALSE; /*rom*/

    zpe->numItems = 0;

    return ZPJL_OK;
    }

/* ------------------------------------------------------------------------ */

/* if speed is important, keep table sorted (by key) and do binsearch lookup */
/* for now, linear search is fine */
static ZPJL_EnvItem*  zpjlFindVarInTable(const Symbol s, const ZPJLENV zpe)
    {
    Uint32  i;

    if (BAD_ZPJLENV == zpe || 0 == zpe->numItems)
        return NULL;

    for (i = 0; i < zpe->numItems; i++)
        if (s == zpe->items[i].id)
            return &zpe->items[i];

    return NULL;
    }

ZPJL_EnvItem*  zpjlFindVar(ZPJLENV zpenv, Symbol s)
    {
    ZPJL_EnvItem*  ev;

    if (BAD_ZPJLENV == zpenv)
        return NULL;

    ev = zpjlFindVarInTable(s, zpenv);
    return (NULL != ev) ? ev : zpjlFindVar(zpenv->antecedent, s);
    }

ZPJLENV  zpjlEnvironment(ZPJL z, ZPJL_EnvSpec es)
    {
    switch (es)
        {
        case ES_ROM:
            return z->zpjlContext.zpenv_ROM;
        case ES_NVRAM:
            return z->zpjlContext.zpenv_DEFAULT;
        case ES_RAM:
            return z->zpjlContext.zpenv_CURRENT;

        case ES_USTATUS:
            return z->zpjlContext.zpenv_USTATUS;

		case ES_NONE:
		case ES_PDL:
			break;		/* bad */
        }

    return BAD_ZPJLENV;
    }

ZPJL_EnvItem*  zpjlFindEV(ZPJL z, ZPJL_EnvSpec es, Symbol s, bool cascade)
    {
    ZPJLENV  zpe = zpjlEnvironment(z, es);
    return cascade ? zpjlFindVar(zpe, s) : zpjlFindVarInTable(s, zpe);
    }

/* ------------------------------------------------------------------------ */

static bool  makeRoomFor(ZPJL z, ZPJLENV zpe, Uint32 n)
    {
    Uint32  growth = 0;

    if (0 == zpe->growSize)
        return FALSE;

    while (!(zpe->numItems + n <= zpe->maxItems))
        growth += zpe->growSize;

    if (0 != growth)
        {
        ZPJL_EnvItem*  deadGuy = zpe->items;
        ZPJL_EnvItem*  newGuy = (ZPJL_EnvItem*) (z->zpjlContext.system.memAllocProc)(
            sizeof(ZPJL_EnvItem) * (zpe->maxItems + growth),
            DEFAULT_ALLOCFLAGS, z->zpjlContext.system.refcon);

        if (NULL == newGuy)
            return FALSE;

        memcpy(newGuy, zpe->items, sizeof(ZPJL_EnvItem) * zpe->numItems);
        zpe->items = newGuy;
        zpe->maxItems += growth;

        (z->zpjlContext.system.memFreeProc)(deadGuy, z->zpjlContext.system.refcon);
        }

    return TRUE;
    }

ZPJL_EnvItem*  zpjlCopyVar(ZPJL z, ZPJL_EnvSpec destES, ZPJL_EnvSpec sourceES, Symbol s)
    {
    ZPJLENV  dest = zpjlEnvironment(z, destES);
    ZPJLENV  source = zpjlEnvironment(z, sourceES);
    ZPJL_EnvItem*  ev = zpjlFindVar(source, s);

    if (NULL == ev || BAD_ZPJLENV == dest || BAD_ZPJLENV == source
        || !makeRoomFor(z, dest, 1))
        return NULL;

    dest->items[dest->numItems++] = *ev;
    return &dest->items[dest->numItems - 1];
    }

ZPJL_EnvItem*  zpjlMakeVar(ZPJL z, ZPJL_EnvSpec es, Symbol s)
    {
    ZPJLENV  dest = zpjlEnvironment(z, es);
    ZPJLENV  source = dest->antecedent;
    ZPJL_EnvItem*  ev = zpjlFindVar(source, s);

    if (NULL == ev || BAD_ZPJLENV == dest || BAD_ZPJLENV == source
        || !makeRoomFor(z, dest, 1))
        return NULL;

    dest->items[dest->numItems++] = *ev;
    return &dest->items[dest->numItems - 1];
    }

/* ------------------------------------------------------------------------ */

Uint32  rangeValue(ZPJL_EnvItem* ev)
    { return ev->data.triplet.value; }

Uint32  rangeUpper(ZPJL_EnvItem* ev)
    { return ev->data.triplet.upper; }

Uint32  rangeLower(ZPJL_EnvItem* ev)
    { return (((Uint32) ev->length) | (((Uint32) ev->data.triplet.lower)<<8)); }

bool  zpjlPutRangeVal(ZPJL_EnvItem* ev, Uint32 value, bool allowZero)
    {
    if (NULL == ev || EIT_RANGEVAL != ev->type)
        return FALSE;

    if (0 == value && allowZero)
        {
        ev->data.triplet.value = 0;
        return TRUE;
        }

    if (value > ev->data.triplet.upper || value < rangeLower(ev))
        return FALSE;

    // ev->data.triplet.value = value;
    return TRUE;
    }

bool  zpjlPutEnumVal(ZPJL_EnvItem* ev, Symbol s, const Symbol* rangeTable, Uint32 rangeTableSize)
    {
    Uint32  i;

    if (NULL == ev || EIT_ENUM != ev->type)
        return FALSE;

    for (i = 0; i < rangeTableSize; i++)
        if (s == rangeTable[i])
            {
            ev->data.dword = s;
            return TRUE;
            }

    return FALSE;
    }

bool  zpjlPutIntVal(ZPJL_EnvItem* ev, Uint32 n)
    {
    if (NULL == ev || EIT_NUMBER != ev->type)
        return FALSE;

    ev->data.dword = n;
    return TRUE;
    }


/* end listing */
