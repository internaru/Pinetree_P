/*
 * LICENSE NOTICE.
 *
 * Use of the Microsoft Windows Rally Development Kit is covered under
 * the Microsoft Windows Rally Development Kit License Agreement,
 * which is provided within the Microsoft Windows Rally Development
 * Kit or at http://www.microsoft.com/whdc/rally/rallykit.mspx. If you
 * want a license from Microsoft to use the software in the Microsoft
 * Windows Rally Development Kit, you must (1) complete the designated
 * "licensee" information in the Windows Rally Development Kit License
 * Agreement, and (2) sign and return the Agreement AS IS to Microsoft
 * at the address provided in the Agreement.
 */

/*
 * Copyright (c) Microsoft Corporation 2005.  All rights reserved.
 * This software is provided with NO WARRANTY.
 */

#ifndef __MARVELL__
#define _GNU_SOURCE
#endif // __MARVELL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <posix_ostools.h>

#define	DECLARING_GLOBALS
#include "globals.h"
#include "statemachines.h"
#include "packetio.h"

extern void qos_init(void);

bool_t          isConfTest;

#ifndef __MARVELL__
static void
usage(void)
{
    fprintf(stderr, "usage: %s [-d] [-t TRACELEVEL] INTERFACE [WIRELESS-IF]\n"
	    "\tRuns a link-layer topology discovery daemon on INTERFACE (eg eth0)\n"
	    "\t-d : don't background, and log moderate tracing to stdout (debug mode)\n"
	    "\t-t TRACELEVEL : select tracing by adding together:\n"
	    "\t\t0x01 : BAND network load control calculations\n"
	    "\t\t0x02 : packet dump of protocol exchange\n"
	    "\t\t0x04 : Charge mechanism for protection against denial of service\n"
	    "\t\t0x08 : system information TLVs (type-length-value)\n"
	    "\t\t0x10 : State-Machine transitions for smS, smE, and smT\n"
	    "\t\t0x20 : Qos/qWave extensions\n",
	    g_Progname);
    exit(2);
}
#endif // __MARVELL__


static void
init_from_conf_file()
{
    FILE   *conf_file;
    char   *line = NULL;
    #define LINEBUFLEN 256
    char    var[LINEBUFLEN];
    char    val[LINEBUFLEN];
    size_t  len  = 0;
    ssize_t numread;
    int     assigns;
    char    default_icon_path[] = {"/etc/icon.ico"};

    /* Set default values for configuration options */
    /* (avoid strdup() since it doesn't use xmalloc wrapper) */
    g_icon_path = xmalloc(strlen(default_icon_path)+1);
    strcpy(g_icon_path,default_icon_path);

    /* Examine configuration file, if it exists */
    conf_file = fopen("/etc/lld2d.conf", "r");
    if (conf_file == NULL)  return;
    while ((numread = getline(&line, &len, conf_file)) != -1)
    {
        var[0] = val[0] = '\0';
        assigns = sscanf(line, "%s = %s", var, val);

        if (assigns==2)
        {
            /* compare to each of the 2 allowed vars... */
            if (!strcmp(var,"icon")) {
                char *path = NULL;
                char *cur  = NULL;

                path = xmalloc(strlen(val)+6); // always allow enough room for a possible prefix of '/etc/'
                memset((void *) path, 0, strlen(val) + 6);
                cur = path;

                /* Check for leading '/' and prefix '/etc/' if missing */
                if (val[0] != '/')
                {
                    strcpy(cur,"/etc/"); cur += 5;
                }
                strncpy(cur,val,strlen(val));

                if (g_icon_path) xfree(g_icon_path);	// always use the most recent occurrence
                g_icon_path = path;
                DBG_VERBOSE("configvar 'g_icon_path' = %s\n", g_icon_path);
            } else if (!strcmp(var,"jumbo-icon")) {
                char *path = NULL;
                char *cur  = NULL;

                path = xmalloc(strlen(val)+6); // always allow enough room for a possible prefix of '/etc/'
                memset((void *) path, 0, strlen(val) + 6);
                cur = path;

                /* Check for leading '/' and prefix '/etc/' if missing */
                if (val[0] != '/')
                {
                    strcpy(cur,"/etc/"); cur += 5;
                }
                strncpy(cur,val,strlen(val));

                if (g_jumbo_icon_path) xfree(g_jumbo_icon_path);	// always use the most recent occurrence
                g_jumbo_icon_path = path;
                DBG_VERBOSE("configvar 'g_jumbo_icon_path' = %s\n", g_jumbo_icon_path);
            } else {
                warn("line ignored - var does not match a known string\n");
            }
        } else {
            warn("line ignored - var or val was missing or no equals\n");
        }

    }
    if (line!=NULL)  free(line);
    fclose(conf_file);
}

#if 0
void *lltd_task(void *unused)
{
    char  *p;
    int    c;

    g_trace_flags = 0;

    /* initialise OS-glue Layer */
    g_osl = osl_init();

    init_from_conf_file();

    event_init();
    qos_init();

    /* prepare responder interfaces */
    osl_interface_init();

    /* add IO handlers & run main event loop forever */
    event_mainloop();

    return NULL;
}

void spawn_lltd_listener(void)
{
    /* create thread for fax context management */
    /* TODO: to support one thread per responder interface, move OS layer
             init routines before creating threads */
    posix_create_thread(&lltd_thread, lltd_task, (void *) 1, "LLTD",
                      &lltd_stack, sizeof(lltd_stack), POSIX_THR_PRI_NORMAL);
}

void lltd_init(void)
{
    spawn_lltd_listener();
}
#else
void lltd_init(void)
{
    char  *p;
    int    c;

    g_trace_flags = 0;

    /* initialise OS-glue Layer */
    g_osl = osl_init();

    init_from_conf_file();

    event_init();
    qos_init();

    /* prepare responder interfaces */
    osl_interface_init();
}
#endif
