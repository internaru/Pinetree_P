/** @file wps_os.c
 *  @brief This file contains timer and socket read functions.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include "raw_socket_api.h"
#include "common.h"

#include <stdlib.h>
//#include <stdio.h>
#include <stdarg.h>
//#include <unistd.h>
#include <string.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <errno.h>
//#include <time.h>
#include <signal.h>
//#include <netinet/in.h>
#include "lassert.h"

#include "wps_def.h"
#include "wps_util.h"
#include "wps_l2.h"
#include "wps_eapol.h"
#include "wps_os.h"

#include "priv_netlink.h"
#include "wps_wlan.h"

void do_gettimeofday(struct timeval *time);
int cancelWPSIfRequested(void);

// (Including mlanevent.h directly causes massive compile conflicts.  Just
// grab the routines we need:
int mlan_event_request_notifications(void *func_ptr);
int mlan_event_cancel_notifications(void *func_ptr);

/********************************************************
        Local Variables
********************************************************/

/* Data structure definition for main loop */
struct wps_sock_s
{
    /** socket no */
    int sock;
    /** private data for callback */
    void *callback_data;
    /** handler */
    void (*handler) (int sock, void *sock_ctx);
};

struct wps_timeout_s
{
    /** next pointer */
    struct wps_timeout_s *next;
    /** time */
    struct timeval time;
    /** private data for callback */
    void *callback_data;
    /** timeout handler */
    void (*handler) (void *timeout_ctx);
};

typedef struct wps_loop_s
{
    /** terminate */
    int terminate;
    /** max socket number */
    int max_sock;
    /** read count */
    int reader_count;
    /** read socket */
    struct wps_sock_s *readers;
    /** timeout */
    struct wps_timeout_s *timeout;
} WPS_LOOP_S;

static WPS_LOOP_S wps_loop;

/********************************************************
        Global Variables
********************************************************/
/** WPS global */
extern WPS_DATA wps_global;
/** IE buffer index */
extern short ie_index;
extern short probe_ie_index;

extern short ap_assocresp_ie_index;
/** Global pwps information */
extern PWPS_INFO gpwps_info;
extern int wps_stop(WPS_DATA * wps_s);

/** Autonomous GO flag */
extern int auto_go;
extern void wfd_peer_ageout_time_handler(void *user_data);

extern int wps_wlan_init(WPS_DATA * wps_s);
extern int wps_clear_running_instance(void);

/********************************************************
        Local Functions
********************************************************/

/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief Free structure used in main loop function
 *
 *  @return         None
 */
void
wps_main_loop_free(void)
{
    struct wps_timeout_s *timeout, *prev;

    ENTER();

    timeout = wps_loop.timeout;
    while (timeout != NULL) {
        prev = timeout;
        timeout = timeout->next;
        os_free(prev);
    }
    if (wps_loop.readers != NULL)
    {
        os_free(wps_loop.readers);
    }

    LEAVE();
}


/** 
 *  @brief Process reception of event from wlan
 *  
 *  @param iface           Interface
 *  @param mlan_event      Event 
 *  @param len             Event length
 *  @return             None
 */
static void 
wps_event_receive(const char *iface, const uint8_t *mlan_event, uint32_t len)
{
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    ENTER();

    wps_hexdump(DEBUG_EVENT, "New event", mlan_event, len);

    wps_wlan_event_parser(gpwps_info, wps_s, (char *) mlan_event, len);

    LEAVE();
    return;
}


/** 
 *  @brief Process event socket initialization 
 *  
 *  @param wps_s    A pointer to global WPS structure
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_event_init(WPS_DATA * wps_s)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    mlan_event_request_notifications(wps_event_receive);

    LEAVE();
    return ret;
}

/** 
 *  @brief Process main loop initialization
 *  
 *  @param wps_s    A pointer to global WPS structure
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_loop_init(WPS_DATA * wps_s, int role)
{
    ENTER();

    /* clear data structure for main processing loop */
    memset(&wps_loop, 0, sizeof(wps_loop));

    /* Initial and hook l2 callback function */
    wps_s->l2 = wps_l2_init(wps_s->ifname, role, ETH_P_EAPOL, wps_rx_eapol, 0);

    if (wps_s->l2 == NULL) {
        wps_printf(DEBUG_INIT, "Init l2 return NULL !\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    if (wps_s->l2 && wps_l2_get_mac(wps_s->l2, wps_s->my_mac_addr)) {
        wps_printf(DEBUG_INIT, "Failed to get own L2 address\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    wps_printf(MSG_INFO, "Own MAC address: " MACSTR,
               MAC2STR(wps_s->my_mac_addr));

    if (wps_wlan_init(wps_s) != WPS_STATUS_SUCCESS) {
        wps_printf(DEBUG_INIT, "wps_driver_config_init fail\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    wps_s->scan_results = NULL;
    wps_s->num_scan_results = 0;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}


/** 
 *  @brief Process event handling free
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void
wps_event_deinit(WPS_DATA * wps_s)
{
    ENTER();

    mlan_event_cancel_notifications(wps_event_receive);

    LEAVE();
}


/** 
 *  @brief Process interface deinit
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void
wps_intf_deinit(WPS_DATA * wps_s)
{
    ENTER();

    if (wps_s == NULL) {
        LEAVE();
        return;
    }

    wps_l2_deinit(wps_s->l2);
    wps_wlan_deinit(wps_s);

    LEAVE();
}

/** 
 *  @brief Process main loop free
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void
wps_loop_deinit(WPS_DATA * wps_s)
{
    ENTER();

    if (wps_s == NULL) {
        LEAVE();
        return;
    }

    if (wps_s->scan_results)
        os_free(wps_s->scan_results);

    wps_s->scan_results = NULL;
    wps_s->num_scan_results = 0;

    if (wps_s->l2 != NULL) 
    {
        wps_l2_deinit(wps_s->l2);
        wps_s->l2 = NULL;
    }

    wps_main_loop_free();

    LEAVE();
}

/** 
 *  @brief Disable main loop
 *  
 *  @return         None
 */
void
wps_main_loop_shutdown(void)
{
    ENTER();
    wps_loop.terminate = WPS_SET;
    raw_signal();    // force the select to return immediately
    LEAVE();
}

/** 
 *  @brief Enable main loop
 *  
 *  @return         None
 */
void
wps_main_loop_enable(void)
{
    ENTER();
    wps_loop.terminate = WPS_CANCEL;
    LEAVE();
}

/** 
 *  @brief Register the signal handler to OS
 *  
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_set_signal_handler(void)
{
    ENTER();

    /* 
     * register signal handler to OS for pressing CTRL+C to terminate program
     */

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Register a handler to main loop socket read function
 *  
 *  @param sock             Socket number
 *  @param handler          A function pointer of read handler
 *  @param callback_data    Private data for callback
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_register_rdsock_handler(int sock,
                            void (*handler) (int sock, void *sock_ctx),
                            void *callback_data)
{
    struct wps_sock_s *tmp;

    ENTER();

    tmp = (struct wps_sock_s *) os_realloc(wps_loop.readers,
                                        (wps_loop.reader_count +
                                         1) * sizeof(struct wps_sock_s));
    if (tmp == NULL)
        return WPS_STATUS_FAIL;

    tmp[wps_loop.reader_count].sock = sock;
    tmp[wps_loop.reader_count].callback_data = callback_data;
    tmp[wps_loop.reader_count].handler = handler;
    wps_loop.reader_count++;
    wps_loop.readers = tmp;
    if (sock > wps_loop.max_sock)
        wps_loop.max_sock = sock;

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Unregister a handler to main loop socket read function
 *  
 *  @param sock     Socket number
 *  @return         None
 */
void
wps_unregister_rdsock_handler(int sock)
{
    int i;

    ENTER();

    if (wps_loop.readers == NULL || wps_loop.reader_count == 0) {
        LEAVE();
        return;
    }

    for (i = 0; i < wps_loop.reader_count; i++) {
        if (wps_loop.readers[i].sock == sock)
            break;
    }
    if (i == wps_loop.reader_count)
        return;
    if (i != wps_loop.reader_count - 1) {
        os_memmove(&wps_loop.readers[i], &wps_loop.readers[i + 1],
                (wps_loop.reader_count - i - 1) * sizeof(struct wps_sock_s));
    }
    wps_loop.reader_count--;

    LEAVE();
}

/** 
 *  @brief Register a time-out handler to main loop timer function
 *  
 *  @param secs             Time-out value in seconds
 *  @param usecs            Time-out value in micro-seconds
 *  @param handler          A function pointer of time-out handler
 *  @param callback_data    Private data for callback
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_start_timer(unsigned int secs,
                unsigned int usecs,
                void (*handler) (void *user_data), void *callback_data)
{
    struct wps_timeout_s *timeout, *tmp, *prev;

    ENTER();

    timeout = (struct wps_timeout_s *) os_malloc(sizeof(*timeout));
    if (timeout == NULL)
        return WPS_STATUS_FAIL;

    os_get_time(&timeout->time);
    timeout->time.tv_sec += secs;
    timeout->time.tv_usec += usecs;
    while (timeout->time.tv_usec >= 1000000) {
        timeout->time.tv_sec++;
        timeout->time.tv_usec -= 1000000;
    }

    timeout->callback_data = callback_data;
    timeout->handler = handler;
    timeout->next = NULL;

    if (wps_loop.timeout == NULL) {
        wps_loop.timeout = timeout;
        LEAVE();
        return WPS_STATUS_SUCCESS;
    }

    prev = NULL;
    tmp = wps_loop.timeout;
    while (tmp != NULL) {
        if (timercmp(&timeout->time, &tmp->time, <))
            break;
        prev = tmp;
        tmp = tmp->next;
    }

    if (prev == NULL) {
        timeout->next = wps_loop.timeout;
        wps_loop.timeout = timeout;
    } else {
        timeout->next = prev->next;
        prev->next = timeout;
    }

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Cancel time-out handler to main loop timer function
 *  
 *  @param handler          Time-out handler to be canceled
 *  @param callback_data    Private data for callback
 *  @return         Number of timer being removed
 */
int
wps_cancel_timer(void (*handler) (void *timeout_ctx), void *callback_data)
{
    struct wps_timeout_s *timeout, *prev, *next;
    int removed = 0;

    ENTER();

    prev = NULL;
    timeout = wps_loop.timeout;
    while (timeout != NULL) {
        next = timeout->next;

        if ((timeout->handler == handler) &&
            (timeout->callback_data == callback_data)) {
            if (prev == NULL)
                wps_loop.timeout = next;
            else
                prev->next = next;
            os_free(timeout);
            removed++;
        } else
            prev = timeout;

        timeout = next;
    }

    LEAVE();
    return removed;
}

/** 
 *  @brief Main loop procedure for socket read and timer functions
 *  
 *  @return             None
 */
void
wps_main_loop_proc(void)
{
    int i, res;
    struct timeval tv, now;
    struct timeval tv_nowait;
    fd_set *rfds;

    ENTER();

    rfds = os_malloc(sizeof(*rfds));
    if (rfds == NULL) {
        wps_printf(MSG_ERROR, "wps_main_loop_proc : malloc failed !\n");
        return;
    }

    /* Timeout value for blocking read (select) */
    tv_nowait.tv_sec = 0;
    tv_nowait.tv_usec = 100000;

    while (!wps_loop.terminate &&
           (wps_loop.timeout || wps_loop.reader_count > 0)) {
        if (wps_loop.timeout) {
            os_get_time(&now);
            if (timercmp(&now, &wps_loop.timeout->time, <))
                timersub(&wps_loop.timeout->time, &now, &tv);
            else
                tv.tv_sec = tv.tv_usec = 0;
        }

        // Make sure it doesn't take too long to cancel.  (Don't hang out
        // in select for more than about a second before checking back to
        // see if there was a user abort.
        if (tv.tv_sec > 1)
            tv.tv_sec = 1;
        cancelWPSIfRequested();

        FD_ZERO(rfds);
        for (i = 0; i < wps_loop.reader_count; i++) {
            FD_SET(wps_loop.readers[i].sock, rfds);
        }

//        wps_printf(MSG_INFO, "wps entering select");
        res = select(wps_loop.max_sock + 1, rfds, NULL, NULL,
                     wps_loop.timeout ? &tv : &tv_nowait);
//        wps_printf(MSG_INFO, "select returned: %d", res);

        if (res < 0 /*&& errno != EINTR*/) {
            wps_printf(MSG_ERROR, "select");
            os_free(rfds);
            LEAVE();
            return;
        }

        /* check if some registered timeouts have occurred */
        if (wps_loop.timeout) {
            struct wps_timeout_s *tmp;

            os_get_time(&now);
            if (!timercmp(&now, &wps_loop.timeout->time, <)) {
                tmp = wps_loop.timeout;
                wps_loop.timeout = wps_loop.timeout->next;
                tmp->handler(tmp->callback_data);
                os_free(tmp);
            }
        }

        if (res <= 0) {
            continue;
        }

        /* call socket read handler function */
        for (i = 0; i < wps_loop.reader_count; i++) {
            if (FD_ISSET(wps_loop.readers[i].sock, rfds)) {
                wps_loop.readers[i].handler(wps_loop.readers[i].sock,
                                            wps_loop.readers[i].callback_data);
            }
        }
    }

    os_free(rfds);
    LEAVE();
}
