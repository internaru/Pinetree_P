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

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "globals.h"

#include "net_iface.h" // net_interface_t

#ifndef __MARVELL__

static event_t *Events;

/* Static list of event_t's, sorted by their firing times */
typedef struct {
    int           eio_fd;
    event_io_fn_t eio_function;
    void         *eio_state;
} event_io_t;

#define NIOS 2
/* Limited to just a handful of IO handler for ease: */
static event_io_t Ios[NIOS];

#else // __MARVELL__

#define NIOS (IF_NUM)

typedef struct {
    int           eio_fd;
    event_io_fn_t eio_function;
    void         *eio_state;
    event_t      *Events;
} event_io_t;

static event_io_t *Ios;

#endif // __MARVELL__


void
event_init(void)
{
    ASSERT(NIOS);

    /* allocate an event struct for each responder inteface */
    Ios = xmalloc(NIOS * sizeof(event_io_t));

    /* mark all IO slots as available */
    int i;
    for (i = 0; i < NIOS; i++)
    {
        Ios[i].eio_fd = -1;
        Ios[i].eio_function = NULL;
        Ios[i].eio_state = NULL;
        Ios[i].Events = NULL;
    }
}


/* Add a new event to the list, calling "function(state)" at "firetime". */
event_t *
event_add(int ifndx, struct timeval *firetime, event_fn_t function, void *state)
{
    if (ifndx < 0)
        return NULL;

    event_t *prev = NULL;
    event_t *here = Ios[ifndx].Events;

    event_t *ne = xmalloc(sizeof(event_t));
    ne->ev_firetime = *firetime;
    ne->ev_function = function;
    ne->ev_state    = state;
    ne->ev_next     = NULL;

    while (here != NULL && timerle(&here->ev_firetime, &ne->ev_firetime))
    {
	prev = here;
	here = here->ev_next;
    }

    ne->ev_next = here;
    if (!prev)
	Ios[ifndx].Events = ne;
    else
	prev->ev_next = ne;

    return ne;
}


/* Cancel a previously requested event; return TRUE if successful, FALSE if the
 * event wasn't found. */
bool_t
event_cancel(event_t *event)
{
    int rif = RIF;

    event_t *prev = NULL;
    event_t *here = Ios[rif].Events;

    while (here != NULL)
    {
	if (here == event)
	{
	    if (prev)
		prev->ev_next = here->ev_next;
	    else
		Ios[rif].Events = here->ev_next;
	    xfree(here);
	    return TRUE;
	}

	prev = here;
	here = here->ev_next;
    }

    return FALSE;
}

/* You can register a handler function to deal with IO on a file descriptor: */
void
event_add_io(int if_ndx, int fd, event_io_fn_t function, void *state)
{
#ifndef __MARVELL__
    int i = 0;

    /* find next free io slot */
    while (i < NIOS && Ios[i].eio_fd != -1)
	i++;
    if (i == NIOS)
	die("event_add_io: no free IO slots (all %d used)\n", NIOS);
#else // __MARVELL__
    if (if_ndx < 0 || fd < 0 || function == NULL)
        return;
#endif // __MARVELL__

    Ios[if_ndx].eio_fd = fd;
    Ios[if_ndx].eio_function = function;
    Ios[if_ndx].eio_state = state;
}

void
event_remove_io(int fd)
{
    int i = 0;

#ifndef __MARVELL__
    while (i < NIOS && Ios[i].eio_fd != fd)
	i++;
    if (i == NIOS)
    {
	warn("event_remove_io: fd %d is unknown; ignoring", fd);
	return;
    }
#else // __MARVELL__
    i = RIF;
#endif // __MARVELL__

    Ios[i].eio_fd = -1;
    Ios[i].eio_function = NULL;
    Ios[i].eio_state = NULL;
}


/* Capture the current thread, and run event and IO handlers forever.
 * Does not return. */


void *
event_iotask(void *rif)
{
    struct timeval tv, now, *timeout;
    fd_set readfds;
    int ret;
    int i;
    int maxfd;

    int ifndx = *((int *)rif);
#ifdef __linux__
    if (ifndx <= 0 || ifndx > IF_NUM)
    {
        DBG_ASSERT(ifndx >= 0 && ifndx < IF_NUM);
    }
#else
    int ifndx = *((int *) rif);
    dbg_printf("lltd: [%d/%d][%s] Thread ID = %d\n",
            ifndx, IF_NUM, IF_NAME(ifndx), pthread_self());
    ASSERT(ifndx >= 0 && ifndx < IF_NUM);
#endif
    event_io_t *eio = Ios + ifndx;

#ifdef ADJUSTING_FOR_EARLY_WAKEUP
    /* If your OS can wake you before the full timeout has elapsed, we
     * end up looping through and not running any event functions,
     * then setting a really short timeout (~10us!) to get back on
     * track.  We avoid this by adding wake_lead_time to the timeout
     * which we calculate dynamically based on how often we get
     * spurious wakeups.  We reduce the lead time if its been correct
     * for a while. */
    int wake_lead_time = 0;
    int wake_time_correct = 0;
#endif

    while (1)
    {
#ifdef __MARVELL__
        /* wait for the responder interface to be up */
        pthread_mutex_lock(&IF_MUTEX(ifndx));
        while (IF_SOCK(ifndx) == -1)
            pthread_cond_wait(&IF_COND(ifndx), &IF_MUTEX(ifndx));
        pthread_mutex_unlock(&IF_MUTEX(ifndx));
#endif // __MARVELL__

        /* IO activity to monitor */
        FD_ZERO(&readfds);
#ifndef __MARVELL__
        maxfd = -1;
        for (i=0; i<NIOS; i++)
        {
            if (Ios[i].eio_fd != -1)
            {
                FD_SET(Ios[i].eio_fd, &readfds);
                maxfd = TOPOMAX(maxfd, Ios[i].eio_fd);
            }
        }
#else // __MARVELL__
        maxfd = eio->eio_fd;
        if (maxfd != -1)
            FD_SET(maxfd, &readfds);
#endif // __MARVELL__

        /* work out a suitable timeout */
        if (eio->Events == NULL)
        {
            /* no events so block forever */
            timeout = NULL;
        }
        else
        {
            gettimeofday(&now, NULL);
            timeout = &tv;
            /* is event in the past? */
            if (timerle(&(eio->Events->ev_firetime), &now))
            {
                /* don't block, just poll at the select statement below */
                timeout->tv_sec = 0;
                timeout->tv_usec = 0;
            }
            else
            {
                /* how far in the future is it? */
                bsd_timersub(&(eio->Events->ev_firetime), &now, timeout);
#ifdef ADJUSTING_FOR_EARLY_WAKEUP
                timeval_add_ms(timeout, wake_lead_time);
#endif
            }
        }

        ret = select(maxfd + 1, &readfds, NULL, NULL, timeout);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
            die("event_mainloop: select: %s\n", strerror(errno));
        }
        if (ret == 0) /* timeout */
        {
            bool_t did_some_work = FALSE;

            /* run event functions */
            gettimeofday(&now, NULL);
            while (eio->Events != NULL &&
                        timerle(&(eio->Events->ev_firetime), &now))
            {
                event_t *e = eio->Events;
                eio->Events = eio->Events->ev_next;
                e->ev_function(e->ev_state);
                xfree(e);
                did_some_work = TRUE;
                gettimeofday(&now, NULL);
            }

#ifdef ADJUSTING_FOR_EARLY_WAKEUP
            if (!did_some_work)
            {
                /* we woke too early; increase the lead time */
                wake_lead_time++;
                if (wake_lead_time > 50)
                {
                    warn("event_mainloop: wake_lead_time clamped at %dms; "
                         "is your select() implementation really waking that early?!\n",
                         wake_lead_time);
                           wake_lead_time = 50;
                }
            }
            else
            {
                /* If the wake time was right, it might also be too high.
                 * So trim the lead time down a little. */
                wake_time_correct++;
                if (wake_time_correct > 100)
                {
                    wake_time_correct = 0;
                    if (wake_lead_time > 0)
                    {
                        wake_lead_time--;
                    }
                }
            }
#endif
        }
        else
        {
#ifdef SIOCGSTAMP_NOT_AVAILABLE
            /* Capture pkt-IO timestamp as early as possible */
            uint64_t	   temp;

            gettimeofday(&now, NULL);
            temp = now.tv_sec * (uint64_t)1000000UL;
            g_pktio_timestamp = temp + now.tv_usec;
#endif
            /* run IO function(s) */
#ifndef __MARVELL__
            for (i=0; i<NIOS; i++)
                if (Ios[i].eio_fd != -1 && FD_ISSET(Ios[i].eio_fd, &readfds))
                    Ios[i].eio_function(Ios[i].eio_fd, Ios[i].eio_state);
#else // __MARVELL__
            if (eio->eio_fd != -1 && FD_ISSET(eio->eio_fd, &readfds))
                eio->eio_function(eio->eio_fd, eio->eio_state);
#endif // __MARVELL__
        }
    }

    return NULL;
}
