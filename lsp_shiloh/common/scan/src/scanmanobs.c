/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanmanobs.c
 *
 * \brief Functions used by scanman.c for interfacing with the OBA Observer.
 *
 * Header file stuff is in scanman.h
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "posix_ostools.h"

#include "ATypes.h"
#include "lassert.h"
#include "logger.h"
#include "event_observer.h"
#include "response_subject.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanman.h"
#include "safetylock.h"

static scanman_observer_data_t scanman_observer_data;

static struct safety_lock obs_lock;

static void scanman_observer_constructor( void )
{
    scanman_observer_data.event = SCANMAN_EVENT_NULL;
    scanman_observer_data.subject_observer_oid_key = SCANMAN_OBSERVER_ID;
    MInitList(&scanman_observer_data.observers);
}

void scanman_observer_notify_event( scanman_event_t event, uint32_t value )
{
    pthread_t curr_thread;
    scanman_observer_data_t * my_status;

    /* 
     * Don't be evil.  Only call from scanman! 
     *
     * Want to guarantee the thread context of all the observer callbacks.
     */
    curr_thread = pthread_self();
    XASSERT( curr_thread==scanman_thread_identify(), curr_thread );

    safety_lock_lock( &obs_lock );
    scanman_observer_data.event = event;
    scanman_observer_data.value = value;
    safety_lock_unlock( &obs_lock );

    my_status = &scanman_observer_data;

    NOTIFY_OBSERVERS( my_status, observers );
}

void scanman_observer_attach( Observer_t *o )
{
    ATTACH_OBSERVER( scanman_observer_data.observers, o );
}

void scanman_observer_detach(Observer_t *o)
{
    DETACH_OBSERVER( scanman_observer_data.observers, o );
}

void scanman_observer_get_last_event( scanman_event_t *event, uint32_t *value )
{
    safety_lock_lock( &obs_lock );
    *event = scanman_observer_data.event;
    *value = scanman_observer_data.value;
    safety_lock_unlock( &obs_lock );
}

static void scanman_obs_navel_callback( Observer_t *observer, Subject_t *s)
{
    scanman_observer_data_t *data;

    /* 
     * Note: this function called in scanman thread context. 
     */

    XASSERT( s->subject_observer_oid_key == SCANMAN_OBSERVER_ID, s->subject_observer_oid_key );

    data = (scanman_observer_data_t *)s;

    scanlog( 0, "%s event=%d value=%d\n", __FUNCTION__, data->event, data->value );

    /* feel free to copy/paste this function to your own code */
}

scan_err_t scanman_obs_navel_gazer_init( void )
{
    /* davep 13-Sep-2011 ; add an observer to myself for test/debug */
    Observer_t *scanman_obs;
    scanman_obs = Observer_Constructor( SCANMAN_OBSERVER_ID, scanman_obs_navel_callback );
    if( !scanman_obs ) {
        return SCANERR_OUT_OF_MEMORY;
    }
    scanman_observer_attach( scanman_obs );
    return SCANERR_NONE;
}

scan_err_t scanman_obs_onetime_init( void )
{
    safety_lock_init( &obs_lock );

    /* davep 13-Sep-2011 ; adding observer stuff */
    scanman_observer_constructor();

    return SCANERR_NONE;
}

