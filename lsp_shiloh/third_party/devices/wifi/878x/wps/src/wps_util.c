/** @file wps_util.c
 *  @brief This file contains functions for debugging print.
 *  
 *  Copyright (C) 2003-2008,2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifdef DEBUG// STDOUT_DEBUG

#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

#include "logger.h"

#include "wps_def.h"
#include "wps_util.h"

#include "dprintf.h"

/********************************************************
        Local Variables
********************************************************/
static int wps_debug_level = DEBUG_WPS_STATE | MSG_ERROR;       /* defined in wps_util.h */
//static int wps_debug_timestamp = 0;

/********************************************************
        Global Variables
********************************************************/

/********************************************************
        Local Functions
********************************************************/

/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief Timestamp debug print function
 *
 *  @return         None
 */
void
wps_debug_print_timestamp(void)
{
    //struct timeval tv;
    //char buf[16];
/*
    if (!wps_debug_timestamp)
        return;

    os_get_time(&tv);
    if (strftime(buf, sizeof(buf), "%b %d %H:%M:%S",
                 localtime((const time_t *) &tv.tv_sec)) <= 0) {
        snprintf(buf, sizeof(buf), "%u", (int) tv.tv_sec);
    }
    printf("%s.%06u: ", buf, (unsigned int) tv.tv_usec);
*/
}

/** 
 *  @brief Debug print function
 *         Note: New line '\n' is added to the end of the text when printing to stdout.
 *
 *  @param level    Debugging flag
 *  @param fmt      Printf format string, followed by optional arguments
 *  @return         None
 */
void
wps_printf(int level, char *fmt, ...)
{
#ifdef DEBUG
    va_list ap;

    va_start(ap, fmt);
#if 0
    if (true)
    {
//    if (level & wps_debug_level) {
//        wps_debug_print_timestamp();
        vprintf(fmt, ap );
//        vprintf(fmt, ap);
        printf("\n");
    }
#else
    if (level & wps_debug_level)
    {
        minVsprintf( NULL, fmt, ap );
    }
#endif
    va_end(ap);
#endif
}

/** 
 *  @brief Debug buffer dump function
 *         Note: New line '\n' is added to the end of the text when printing to stdout.
 *
 *  @param level    Debugging flag
 *  @param title    Title of for the message
 *  @param buf      Data buffer to be dumped
 *  @param len      Length of the buf
 *  @return         None
 */
void
wps_hexdump(int level, const char *title, const unsigned char *buf, size_t len)
{
    int i;
    //int j;
    //unsigned char *offset;

    if (level & wps_debug_level)
    {
        DPRINTF( (DBG_LOUD | DBG_OUTPUT), ("%s - hexdump(len=%d):\n", title, len));
        //printf("%s - hexdump(len=%d):", title, len);
        for (i = 0; i < len; i++)
            DPRINTF( (DBG_LOUD | DBG_OUTPUT), (" %02x", buf[i]));
            //printf(" %02x", buf[i]);
        DPRINTF((DBG_LOUD | DBG_OUTPUT), ("\n"));
    }
}

#endif /* STDOUT_DEBUG */
