/***********************************************************
* (c) Copyright 1998-2008 Marvell International, Ltd. 
*
*                 Marvell Confidential
* ==========================================================
*/

/****************************************************************************\
**                                                                          **
**  zStr.c                                                                  **
**                                                                          **
**  A variable lengh string class                                           **
**  (NB: depends on definitions in zPJL.h)                                  **
**                                                                          **
\****************************************************************************/

#include <stdbool.h>

#include "zStr.h"
#include "ATypes.h"
#include "string.h"


#define ALLOCFLAGS              DEFAULT_ALLOCFLAGS

#undef max
#define max(a, b)  (((a) > (b)) ? (a) : (b))


static int  makeRoomFor(zStr* zstr, Uint32 n)
    {
    if (zstr->bytesUsed + n >= zstr->allocSize)
        {
        Uint32  a = max(zstr->bytesUsed + n, 2 * zstr->allocSize);
        char*  newGuy = (char*) (*zstr->allocProc)(a, ALLOCFLAGS, NULL);
        char*  deadGuy = zstr->buffer;

        if (NULL == newGuy)
            return FALSE;

        strcpy(newGuy, zstr->buffer);
        zstr->buffer = newGuy;
        zstr->allocSize = a;

        (*zstr->freeProc)(deadGuy, NULL);
        }

    return TRUE;
    }

/* ------------------------------------------------------------------------ */

int  zStr_construct(zStr* zstr, zpjlSystem* system, const char* initialString)
    {
    if (NULL == initialString)
        return zStr_new(zstr, system);

    zstr->allocProc = system->memAllocProc;
    zstr->freeProc = system->memFreeProc;
    zstr->allocSize = strlen(initialString) + 1;
    zstr->bytesUsed = zstr->allocSize;

    zstr->buffer = (char*) (*zstr->allocProc)(zstr->allocSize, ALLOCFLAGS, NULL);
    if (NULL != zstr->buffer)
        {
        strcpy(zstr->buffer, initialString);
        return TRUE;
        }

    return FALSE;
    }

int  zStr_new(zStr* zstr, zpjlSystem* system)
    {
    zstr->allocProc = system->memAllocProc;
    zstr->freeProc = system->memFreeProc;
    zstr->allocSize = INITIAL_SIZE;
    zstr->bytesUsed = 1;

    zstr->buffer = (char*) (*zstr->allocProc)(zstr->allocSize, ALLOCFLAGS, NULL);
    if (NULL != zstr->buffer)
        {
        zstr->buffer[0] = 0;
        return TRUE;
        }

    return FALSE;
    }

int  zStr_putc(zStr* zstr, char c)
    {
    if (!makeRoomFor(zstr, 1))
        return FALSE;

    zstr->buffer[zstr->bytesUsed - 1] = c;
    zstr->buffer[zstr->bytesUsed++] = 0;
    return TRUE;
    }

int  zStr_putstr(zStr* zstr, const char* s)
    {
    if (NULL != s)
        {
        int  n = strlen(s);
        if (!makeRoomFor(zstr, n))
            return FALSE;

        strcpy(&zstr->buffer[zstr->bytesUsed - 1], s);
        zstr->bytesUsed += n;
        }
    return TRUE;
    }

int  zStr_puts(zStr* zstr, Symbol ids)
    {
    if (NULL_SYMBOL != ids)
        {
        const char*  s = SYM(ids);
        int  n = strlen(s);
        if (!makeRoomFor(zstr, n))
            return FALSE;

        strcpy(&zstr->buffer[zstr->bytesUsed - 1], s);
        zstr->bytesUsed += n;
        }
    return TRUE;
    }

int  zStr_putss(zStr* zstr, Symbol ids)
    {
    const char* s = (NULL_SYMBOL != ids) ? SYM(ids) : NULL;
    int  n = (NULL != s) ? strlen(s) + 1 : 1;
    if (!makeRoomFor(zstr, n))
        return FALSE;

    if (NULL_SYMBOL != ids)
        strcpy(&zstr->buffer[zstr->bytesUsed - 1], s);

    zstr->bytesUsed += n;
    zstr->buffer[zstr->bytesUsed - 2] = ' ';
    zstr->buffer[zstr->bytesUsed - 1] = 0;
    return TRUE;
    }

/* okay if s = NULL */
int  zStr_putl(zStr* zstr, const char* s)
    {
    int  n = (NULL != s) ? strlen(s) + 2 : 2;
    if (!makeRoomFor(zstr, n))
        return FALSE;

    if (NULL != s)
        strcpy(&zstr->buffer[zstr->bytesUsed - 1], s);

    zstr->bytesUsed += n;
    zstr->buffer[zstr->bytesUsed - 3] = 0x0d;
    zstr->buffer[zstr->bytesUsed - 2] = 0x0a;
    zstr->buffer[zstr->bytesUsed - 1] = 0;
    return TRUE;
    }

/*sigh*/
char* strrev(char* string)
    {
    char*  start = string;
    char*  left = string;
    char  ch;

    while (*string++); /* find end of string */
    string -= 2;
    while (left < string)
        {
        ch = *left;
        *left++ = *string;
        *string-- = ch;
        }

    return start;
    }

static char*  int2str(int n, char* s)
    {
    char*  p = s;

    if (0 == n)
        *p++ = '0';
    else
        {
        while (n > 0)
            {
            int  nn = n / 10;
            *p++ = '0' + (n - (nn * 10));
            n = nn;
            }
        }
    *p = 0;

    return strrev(s);
    }

int  zStr_putn(zStr* zstr, Uint32 n)
    {
    char  t[17];
    int  len;

    int2str(n, t);
    len = strlen(t);
    if (!makeRoomFor(zstr, len))
        return FALSE;

    strcpy(&zstr->buffer[zstr->bytesUsed - 1], t);
    zstr->bytesUsed += len;
    zstr->buffer[zstr->bytesUsed - 1] = 0;

    return TRUE;
    }


char*  zStr_xfer(zStr* zstr)
    {
    char*  p = zstr->buffer;
    zstr->buffer = NULL;
    return p;
    }


int  zStr_destruct(zStr* zstr)
    {
    if (NULL != zstr->buffer)
        {
        (*zstr->freeProc)(zstr->buffer, NULL);
        zstr->buffer = NULL;
        }
    return TRUE;
    }

/* end listing */
