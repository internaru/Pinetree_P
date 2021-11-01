/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

/**\file wps_events.h
 * 
 *  Routines for monitoring WPS events.
 *
 */

#ifndef __WPS_EVENTS_H__
#define __WPS_EVENTS_H__

#include "event_observer.h"

extern bool g_wps_session_active;
/********************************************* 
 *  WPS SUBJECT
 *********************************************/

typedef enum wps_event_e
{ 
    WPS_NOT_YET_RUN_EVENT,

    // Attempting to run WPS, Searching	
    WPS_PROCESSING_EVENT,
  
    // Attempting to connect 
    WPS_CONNECTING_EVENT,

    // WPS worked the last time someone tried it 
    WPS_SUCCESS_EVENT,          

    // WPS tried to run, but never found an active AP
    WPS_TIMEOUT_EVENT,

    // WPS found multiple active access points and quit
    WPS_SESSION_OVERLAP_EVENT,

    // Encryption and handshake failures, loss of connectivity, etc.  
    WPS_UNKNOWN_FAILURE_EVENT,

    // User cancelled WPS
    WPS_CANCEL_EVENT,         
    	        
} wps_event_t;

void wps_attach_event_observer(Observer_t *o);
void wps_detach_event_observer(Observer_t *o);

wps_event_t wps_get_latest_event_type(Subject_t *s);
int wps_get_latest_event_details(Subject_t *s);

//Event notifier
void wps_notify_event( wps_event_t event, int details );

//Cancel the WPS timer
void CancelWPStimer(void);

//Cancel the WPS session 
void CancelWPSSession(void);

#endif
