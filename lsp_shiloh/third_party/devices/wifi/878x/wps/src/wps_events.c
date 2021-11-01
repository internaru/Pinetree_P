/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

/**\file wps_events.c
 * 
 *  Routines for monitoring wps events.
 *
 */

#include "wps_events.h"
#include "event_observer.h"
#include "memAPI.h"

/********************************************* 
 *  WPS SUBJECT
 *********************************************/

#define WPS_EVENT_ID 0x18228459 


typedef struct wps_subject_s
{
    // Common values added to all subjects
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;

    // WPS specific event fields
    wps_event_t latest_event;
    int latest_event_details;
} wps_subject_t;

/** Singleton Constructor, initializes, doubles as get struct */
static wps_subject_t *wps_subject()
{
    static wps_subject_t *ptr = 0;
    if ( 0 == ptr)
    {
        // Construct a copy of the subject and populate it with initial values
        ptr = (wps_subject_t *)MEM_MALLOC(sizeof(wps_subject_t));

        // Initialize the general subject fields
        ptr->subject_observer_oid_key = WPS_EVENT_ID;
        MInitList(&(ptr->observers));

        // Initialize WPS specific values
        ptr->latest_event = WPS_NOT_YET_RUN_EVENT;
        ptr->latest_event_details = 0;
    }
    return ptr;
}

/** Notify function */
void wps_notify_event( wps_event_t event, int details )
{   
    // Update the subject
    wps_subject()->latest_event = event;
    wps_subject()->latest_event_details = details;

    // Notify our observers
    NOTIFY_OBSERVERS(wps_subject(), observers);
}

void wps_attach_event_observer(Observer_t *o)
{
    ATTACH_OBSERVER(wps_subject()->observers, o );
}

void wps_detach_event_observer(Observer_t *o)
{
    DETACH_OBSERVER(wps_subject()->observers, o );
}

wps_event_t wps_get_latest_event_type(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, wps_subject_t, subject_observer_oid_key, WPS_EVENT_ID));

    return wps_subject()->latest_event;
}

int wps_get_latest_event_details(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, wps_subject_t, subject_observer_oid_key, WPS_EVENT_ID ));

    return wps_subject()->latest_event_details;
}
