/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_link_inotify.c
  * 
  *  This file contains the runtime file-event detection and reporting routines.
  * 
  *  Utilizes the linux inotify() interface
  *
  */

#ifdef __cplusplus
extern "C" {
#endif

// public headers
#include <sys/inotify.h> // inotify_init()
typedef struct inotify_event inotify_t;

// common headers
#ifdef HAVE_MDNS
#include <stdbool.h>
#include "net_mdns_vars.h" // net_mdns_get_var_service_name()
#include "net_mdns_api.h" // activate_mdns_restart_timer()
#endif
#include "net_link_api.h"
#include "net_iface_api.h" // NET_IFACE_LINK_EVENT_STATUS_CHANGE
#include "net_iface.h" // net_interface_t
#include "os_system.h"

// debug
#define DBG_PRFX "net.link.event: "
#define DBG_ON /* DBG_MSG */
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

#define str_notification_type(type) case type: sprintf(str_buf, #type); break
char* str_file_event(event)
{
    const char* seed = "<reason %#x";
    static char str_buf[64];
    int str_len = strlen(seed);

    sprintf(str_buf, seed, event);
    if (event & IN_CREATE)  strncat(str_buf, " IN_CREATE", sizeof(str_buf)-str_len);
    if (event & IN_MODIFY)  strncat(str_buf, " IN_MODIFY", sizeof(str_buf)-str_len);
    if (event & IN_ATTRIB)  strncat(str_buf, " IN_ATTRIB", sizeof(str_buf)-str_len);
    if (event & IN_IGNORED) strncat(str_buf, " IN_IGNORED", sizeof(str_buf)-str_len);
    if (event & IN_CLOSE_WRITE) strncat(str_buf, " IN_CLOSE_WRITE", sizeof(str_buf)-str_len);
    if (event & IN_DELETE)  strncat(str_buf, " IN_DELETE", sizeof(str_buf)-str_len);

    strncat(str_buf,">", sizeof(str_buf)-str_len);
    return str_buf;
}

#define LAST_HISTORICAL_EVENT 3
typedef void (*file_listener_cb)(void);
static struct last_file_event_s {
    long at_time;
    file_listener_cb sent;
} foi_history[LAST_HISTORICAL_EVENT] = { {0,NULL}, {0,NULL}, {0,NULL} };

typedef struct {
    int wd;
    char* filename;
    file_listener_cb cb; 
} file_of_interest_t;

// notify file-event to additional network layer(s) as defined
#define FILE_EVENT_DEBOUNCE_TIME 100 /* revisit tunable debounce window */
void propogate_file_event(file_of_interest_t* file_event)
{
    static int latest = 0;
    long current_event_time = posix_gettime_ticks();
    long delta_t = current_event_time - foi_history[latest].at_time;

    // how many ticks since last change?
    foi_history[latest].at_time = current_event_time;
    foi_history[latest].sent    = file_event->cb;

    // ignore file event if it happens within FILE_EVENT_DEBOUNCE_TIME
    if (delta_t > FILE_EVENT_DEBOUNCE_TIME)
    {
        DBG_VERBOSE("%s() @ %d file '%s'\n", __func__, current_event_time, file_event->filename);
        if (file_event->cb) file_event->cb(); // tickle if interested
    }

    latest++; if (latest >= LAST_HISTORICAL_EVENT) latest = 0; // adv
}

#ifdef HAVE_MDNS
// the mdns conf file-change trigger
static void renew_mdns_service_cb(void)
{
    DBG_VERBOSE("%s() ..\n", __func__);
    activate_mdns_restart_timer(FILE_EVENT_DEBOUNCE_TIME); // future-start
}
#endif

// listen to file events via inotify()
file_of_interest_t watches[] = {
#ifdef HAVE_MDNS
    { -1, mdns_conf, renew_mdns_service_cb },
#endif // HAVE_MDNS
    {  0, NULL, NULL } // required last
};

__attribute__((aligned(8))) char file_event_buf[256];
void* file_change_listener(void* ignored)
{
    int fd;
    fd = inotify_init();
    int len, sofar;

    int watches_set = 0;
    if (fd > 0)
    {
        int file_of_interest = 0;
        while (file_of_interest < (sizeof(watches)/sizeof(file_of_interest_t)))
        {
            char* filename = watches[file_of_interest].filename;
            if (filename)
            {
                DBG_VERBOSE("%s(%s) ..\n", __func__, filename);
                watches[file_of_interest].wd = inotify_add_watch(fd, filename, IN_MODIFY|IN_CREATE|IN_ATTRIB|IN_CLOSE_WRITE);
                if (watches[file_of_interest].wd < 0)
                {
                    DBG_ERR("%s(%s) %s (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
                    int res = FAIL;
#ifdef HAVE_MDNS
                    // create initial file
                    char* os_request = "mdns_write_conf"; /* create placeholder file */ UNUSED_VAR(os_request);
                    if (!strcmp(mdns_conf, filename))
                    {
                        // use current service name
                        char service_name[NET_MDNS_SERVICE_NAME_SIZE];
                        net_mdns_get_var_service_name(0, service_name, NET_MDNS_SERVICE_NAME_SIZE); // 1st mdns dataset
                        res = mdns_write_conf(service_name);
                    }
                    else
#else
                    char os_request[64] = { "touch " }; // create placeholder file
                    strcat(os_request, filename);
                    res = os_system(os_request);
#endif
                    if (res != OK)
                    {
                        DBG_ERR("%s() err '%s' %d\n", __func__, os_request, res);
                        DBG_ASSERT(res==OK); // fix it now
                    }
                    watches[file_of_interest].wd = inotify_add_watch(fd, filename, IN_MODIFY|IN_CREATE|IN_ATTRIB|IN_CLOSE_WRITE); // 2nd chance
                } // watch desc
                DBG_VERBOSE("%s() added '%s' wd %d\n", __func__, filename, watches[file_of_interest].wd);
                watches_set++; // sofar
            } // filename

            file_of_interest++; // next file
        } // watches[file_of_interest]
    }

    while (fd > 0)
    {
        len = read(fd, file_event_buf, sizeof(file_event_buf));
        sofar = 0;
        while (sofar < len)
        {
            inotify_t* event = (inotify_t*) &file_event_buf[sofar];
            int file_of_interest = 0;
            while (watches[file_of_interest].wd &&
                   event->wd != watches[file_of_interest].wd &&
                   file_of_interest < watches_set)
                file_of_interest++; // find entry

            // matching event?
            if (watches[file_of_interest].filename != NULL)
            {
                DBG_VERBOSE("%s() event %d %s '%s'\n",
                   __func__, event->wd, str_file_event(event->mask), watches[file_of_interest].filename);

                // pass along notification
                if (event->mask & IN_CLOSE_WRITE)
                {
                    propogate_file_event(&watches[file_of_interest]);
                }
            }
            sofar += sizeof(inotify_t);
        }
    }

    //inotify_rm_watch(fd, mdns_conf, IN_MODIFY|IN_CREATE|IN_ATTRIB);

    if (fd == -1)
    {
        DBG_ERR("%s() %s err %d \n", __func__, strerror(thread_errno), thread_errno);
        return NULL;
    }
    return NULL; // unlikely
}

// net link event process data
static pthread_t net_file_change_thread; // thread on which event-listener runs
#define THREAD_STACK_SIZE PTHREAD_STACK_MIN
static ALIGN8 uint8_t net_file_change_stack[THREAD_STACK_SIZE];

void launch_file_change_listener(void)
{
    // launch a network event listener thread
    int ret = posix_create_thread(&net_file_change_thread,
                                   file_change_listener, NULL,
                                   NET_MAC_THREAD_NAME "-file",
                                  &net_file_change_stack,
                                   THREAD_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if (ret != OK)
    {
        DBG_ERR("%s() no %s thread!\n", __func__, NET_MAC_THREAD_NAME "-hotplug" );
    }
}

#ifdef __cplusplus
}
#endif

//eof
