/** 
 * ============================================================================
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * @file observers used for pjl ustatus commands.
 **/


#ifndef pjl_ustatus_H
#define pjl_ustatus_H

#include "event_observer.h"
#include "agConnectMgr.h" // pipe

#ifdef __cplusplus
extern "C" {
#endif


/// pjl job status update functions:
#define pjl_ustatus_jobend_ID    0x452676
#define pjl_ustatus_jobstart_ID  0x452677
#define pjl_ustatus_canceljob_ID 0x452678
#define pjl_ustatus_pageend_ID   0x452679
#define observer_pjl_ustatus_timed_ID 0x452680
void pjl_ustatus_jobend_update(Observer_t *observer, Subject_t *s);
void pjl_ustatus_jobstart_update(Observer_t *observer, Subject_t *s);
void pjl_ustatus_pageend_update(Observer_t *observer, Subject_t *s);
void pjl_ustatus_canceljob_update(Observer_t *observer, Subject_t *s);

///< Get the singleton for pjl ustatus devicestatus observer notifications.
Observer_t * 
get_observer_pjl_ustatus_devicestatus();

#define PipeObserver_ID 0x4524fec2 

typedef struct PipeObserver_s
{
    Observer_T_BaseClassMembers;

    IOReg *pipe; ///< not owned reference to the pipe we will report backchannel on.
} PipeObserver_t;

Observer_t * ///< return null or pointer to base class.
PipeObserver_Constructor( IOReg *pipe, uint32_t key, void (update)( Observer_t *o, Subject_t *s ));




/// msg_delay_notification with observer in the callbackparm is used by pjl_ustatus_timed.
void pjl_ustatus_timed_delay_notify(void *callbackparm);

/// turn pjl timed ustatus on with a time in seconds or off with 0 seconds.
void set_observer_pjl_ustatus_timed(IOReg *pipe, uint32_t time);
void pjl_ustatus_device_status_update( Observer_t *o, Subject_t *s );
uint32_t get_observer_pjl_ustatus_timed_seconds( IOReg *pipe );


#ifdef __cplusplus
}
#endif

#endif
