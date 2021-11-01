/* 
 * **********************************************************
 * (c) Copyright 2009-2010 Marvell International Ltd.
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \file print_events_common_api.h
 *
 * \brief
 *
 */

#ifndef PRINT_EVENTS_COMMON_API_H
#define PRINT_EVENTS_COMMON_API_H
#include "print_status_codes.h"
#include "event_observer.h"
#include "response_subject.h"
#include "ResourceMap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define pec_num_pages_ID 0x44 ///< The unique id for the numpages object.
#define pec_num_bytes_ID 0x45 ///< The unique id for the numbytes object.

void init_print_events_common();
void print_status_attach();

/* PRINT STATUS EVENTS */

/** 
 * \brief Debug routine to print the current print_sys_status 
 *        to the debug console.
 */
void print_sys_status_dump(void);

#define PrintJobMgr_PrintStatus_ID 0x13419  

/// printstatus 
/// This event notifies on changes of the current status of the print subsystem.
/// Note that the notification fires when the filtered "worst" status changes, not
/// necessarily when the individual print or job status changes.
/// pjl maps the print status into pjl error codes and returns them 
/// on the back channel.
void pec_attachobserver_printstatus(Observer_t *o);
void pec_detachobserver_printstatus(Observer_t *o);

/// get the current printstatus from the subject; this is the filtered
/// most critical or "worst" status of the print job manager and print
/// page manager
uint32_t pec_get_printstatus( Subject_t *s);

/// get the current print job status from the subject (print job manager)
uint32_t pec_get_printjobstatus( Subject_t *s);

/// get the current print page status (print page manager)
uint32_t pec_get_printpagestatus( Subject_t *s);

/// get the current SJM resource Job Type
JOB_TYPES pec_get_resourceJobType( Subject_t *s);

/// get the current SJM resource Job ID
uint32_t pec_get_resourceJobID( Subject_t *s);

/// get the callback to retry a stalled operation (if any); a stalled operation
/// is something like a paper out event. The retry callback would be called
/// to indicate that the system should try to proceed.
TRY_AGAIN pec_get_retry_callback(Subject_t *s);

/// get the callback to cancel current operation (if any); 
USER_CANCEL pec_get_user_cancel_callback(Subject_t *s);

/// get the the retry subject.  this is passed to the retry callback.
/// Note that it should be acquired at the same time as the retry callback. 
Subject_t* pec_get_retry_subject(Subject_t *s);

///< Am I watching ? query.  True if the specified observer is observing
/// the printstatus.
bool pec_is_observing_printstatus(Observer_t *o);

/// Poll the current status of the print subsystem.  This may be called
/// outside the context of the observers and may be called without registering for
/// notification.
uint32_t pec_get_current_printstatus();


#ifdef __cplusplus
}
#endif

#endif // PRINT_EVENTS_COMMON_API_H
