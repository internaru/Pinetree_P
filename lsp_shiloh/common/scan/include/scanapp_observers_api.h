/******************************************************************************
 * Copyright (c) 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file scanapp_observers_api.h
 *
 * \brief scan app observer APIs -- uses the event reporting API
 * to expose events within the scan application.  For example, when a scan
 * starts or finishes.
 *
 */

#ifndef SCANAPP_OBSERVERS_API_H
#define SCANAPP_OBSERVERS_API_H


#define SCANAPP_OBSERVER_ID 0x7363616e


/* davep 18-Oct-2010 ; RIP Status Manager. Replace with something new. */
typedef enum {
    SCANAPP_EVENT_NULL=0, /* invalid/uninitialized */
    SCANAPP_EVENT_IDLE=1,
    SCANAPP_EVENT_SCAN_START=2,
    SCANAPP_EVENT_SCAN_RUNNING=3,
    SCANAPP_EVENT_SCAN_COMPLETE=4,
    SCANAPP_EVENT_HOST_CANCEL=5,
    SCANAPP_EVENT_DEVICE_CANCEL=6,
    SCANAPP_EVENT_TIMEOUT=7,
    SCANAPP_EVENT_SCAN_OFFLINE=8,
    SCANAPP_EVENT_SCAN_PAUSED=9,
    SCANAPP_EVENT_WAITING_FOR_USER_INPUT=10,
	SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN=11,
	SCANAPP_EVENT_SCAN_FAILED_FB_EMPTY_FOR_MERGE=12,
	SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_PCSCAN=13,
	SCANAPP_EVENT_SCAN_FAILED_MEMORY_FULL=14,
	SCANAPP_EVENT_SCAN_FAILED_FILE_WRITE=15,
	SCANAPP_EVENT_SCAN_FAILED_ADF_PAPER_IN_MERGE=16,
	SCANAPP_EVENT_SCAN_FAILED_FB_NOT_SUPPORT_SIZE=17,
	SCANAPP_EVENT_SCAN_FAILED_USBTHUMB_NOT_CONNECT=18,
	SCANAPP_EVENT_SCAN_STOP_MEMORY_FULL=21,
	/* more later */
} scanapp_event_t;


/**
 *
 * \brief Register for scan app event notications
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 *
 */
void scanapp_observer_attach( Observer_t *o );


/**
 *
 * \brief Un-register for scan app event notications
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 * also the same one used when registering
 *
 */
void scanapp_observer_detach( Observer_t *o );


/**
 *
 * \brief Get the scan app's event.  This function must only be used when
 * the scan app has issued its callback registered with the
 * scanapp_observer_attach() function.
 *
 * \param s The Subject_t pointer passed to the event callback.
 *
 */
scanapp_event_t scanapp_observer_get_event( Subject_t *s );

void scanapp_log_event( scanapp_event_t evt );

#endif //SCANAPP_OBSERVERS_API_H

