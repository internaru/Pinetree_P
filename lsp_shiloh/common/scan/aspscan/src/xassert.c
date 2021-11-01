/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xassert.h"
#include "log.h"

#define MAX_DIE_MSG  255

static char errmsg[MAX_DIE_MSG+1];  

void die( char *failmsg )
{
    /* report the error without using LOG_ macros */
    errorlog( "%s", failmsg );

    /* hang thyself */
    abort();
    /* never returns */
}

void AssertFail( char *assertmsg, char *filename, int linenumber )
{
    memset( errmsg, '*', sizeof(errmsg) );

    /* report error, save log ring buffer to flash perhaps */
    snprintf( errmsg, MAX_DIE_MSG, "%s %d:: Assertion \"%s\" failed", filename, linenumber, assertmsg );

    /* never returns */
    die( errmsg );
}

/* "Embraced and Extended" AssertFail */
void XAssertFail( char *assertmsg, char *filename, int linenumber, unsigned long int data )
{
    memset( errmsg, '*', sizeof(errmsg) );

    /* pthread_t is a long */
    snprintf( errmsg, MAX_DIE_MSG, "%s %d:: Assertion \"%s\" failed (ec=%#lx)", 
            filename, linenumber, assertmsg, data );

    /* never returns */
    die( errmsg );
}

