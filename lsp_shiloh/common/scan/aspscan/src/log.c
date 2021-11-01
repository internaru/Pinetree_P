/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <stdarg.h>

#include "log.h"

#define MAX_ERRMSG 128

static int log_level = 2;

/**
 * \brief  run-time tuneable log level 
 *
 * See also the new '-d' option
 *
 * \author David Poole
 * \date 24-Aug-2012
 */

int log_set_level( int new_log_level )
{
    int old_log_level;

    old_log_level = log_level;
    log_level = new_log_level;

    return old_log_level;
}

static void msgout( const char *msg )
{
    /* eventually need to direct this to syslog() or a log file or both or
     * neither or something
     */
    fputs( msg, stdout );
}

void debuglog( int level, const char *fmt, ... )
{
    va_list arg;
    char errmsg[MAX_ERRMSG];

    if( level > log_level ) {
        return;
    }

    va_start( arg, fmt );

    /* Note I'm not declaring errmsg[] with the usual +1 for NULL since
     * vsnprintf() will use up to MAX_ERRMSG including the terminating NULL.
     */
    vsnprintf( errmsg, MAX_ERRMSG, fmt, arg );

    msgout( errmsg );

    va_end( arg );
}

void errorlog( const char *fmt, ... )
{
    va_list arg;
    char errmsg[MAX_ERRMSG];

    va_start( arg, fmt );

    /* Note I'm not declaring errmsg[] with the usual +1 for NULL since
     * vsnprintf() will use up to MAX_ERRMSG including the terminating NULL.
     */
    vsnprintf( errmsg, MAX_ERRMSG, fmt, arg );

    msgout( errmsg );

    va_end( arg );
}

