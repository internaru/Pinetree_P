/** 
 * ============================================================================
 * Copyright (c) 2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * @file observers used for pjl ustatus commands.
 **/
#include <stdint.h>
#include "event_observer.h"
#include "pjl_ustatus.h"
#include "PrintJobInfo.h"
#include "lassert.h"
#include "memAPI.h"
#include "print_events_common_api.h"
#include "delay_api.h"
#include "pjl_extra_api.h"

#include "SysServiceMgr.h"

// hopes to observer at init
//extern uint16_t USBStatus_is_ON;		// USB Status Monitor 		2013.12.19	by Juny 

//#define DBG 0
#define DBG ( DBG_LOUD | DBG_OUTPUT )


void pjl_ustatus_jobend_update(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, pjl_ustatus_jobend_ID));

    pjl_cmd_ustatus_job_end(
        PrintJobInfo_Get_pipe(s),
        PrintJobInfo_Get_jobname(s),
        PrintJobInfo_Get_pages_printed(s),
        PrintJobInfo_Get_jobstatus(s));
}

void pjl_ustatus_jobstart_update(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, pjl_ustatus_jobstart_ID));

    /// pjl does this on recieve of JOB instead of actual job start.
    pjl_cmd_ustatus_start_job( PrintJobInfo_Get_pipe(s), PrintJobInfo_Get_jobname(s) );
}

void pjl_ustatus_pageend_update(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, pjl_ustatus_pageend_ID));

    pjl_cmd_ustatus_page( PrintJobInfo_Get_pipe(s), PrintJobInfo_Get_pages_printed(s) );
}

void pjl_ustatus_canceljob_update(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, pjl_ustatus_canceljob_ID));


 //   ag_cmd_ustatus_job_cancel( PrintJobInfo_Get_pipe(s), 
 //                              PrintJobInfo_Get_jobname(s), 
 //                              PrintJobInfo_Get_pages_printed(s) );

}

///////////// begin ustatus timed.

// NB: if pjlparser thread has more than one delay notify object
// then this had better be unique between them.

typedef struct observer_pjl_ustatus_timed_s
{
    Observer_T_BaseClassMembers;
    
    uint32_t time;   // seconds
    delay_msec_timer_t *delay_timer;  
    uint32_t status; // device status
    IOReg* pipe;     // not owned
    
} observer_pjl_ustatus_timed_t;

void pjl_ustatus_device_status_update( Observer_t *o, Subject_t *s )
{
/* hopes to observer at init
   change for USB Status 	@@ 2013.12.19		by Juny */
/*	if ( USBStatus_is_ON )
	{
#if DEBUG 
        dbg_printf("Act USB SM\n");
#endif
*/
    ASSERT(VALIDATE_OBJECT_KEY( o, PipeObserver_t, 
                               subject_observer_oid_key, PipeObserver_ID) );

    PipeObserver_t * po = (PipeObserver_t *)o;

    pjl_cmd_ustatus_device(po->pipe, pec_get_printstatus(s), true);	// fix the previous status		2014.02.27	by Juny
//    pjl_cmd_ustatus_device(po->pipe, GetPrintSystemStatus(), true);
/*
	}
*/
}

// delay notify grabs the saved status when the timer fires.
void pjl_ustatus_timed_delay_notify(void *observer)
{
    observer_pjl_ustatus_timed_t *o = (observer_pjl_ustatus_timed_t*)observer;
    if (o->subject_observer_oid_key == observer_pjl_ustatus_timed_ID)
        pjl_cmd_ustatus_device(o->pipe, o->status, false);

#if DEBUG
    else
        dbg_printf("BAD call bad object %x\n", observer);  // warn then assert, could do a ima switch...

    // ID could be used to switch but currently only one observer is used.
    ASSERT(VALIDATE_OBJECT_KEY(o, Observer_t, 
                               subject_observer_oid_key, observer_pjl_ustatus_timed_ID));
#endif
}

// status notify saves the status when status changes.
static void pjl_ustatus_timed_update(Observer_t *observer, Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, observer_pjl_ustatus_timed_ID));

    observer_pjl_ustatus_timed_t *o = (observer_pjl_ustatus_timed_t*)observer;
    
    o->status = pec_get_printstatus(s);
}

static observer_pjl_ustatus_timed_t * ///< return null or pointer to base class.
observer_pjl_ustatus_timed_t_Constructor( 
    IOReg *pipe, ///< not owned reference to pipe used to report status.
    uint32_t time ///< 0 is off else time in seconds.
    )
{
    observer_pjl_ustatus_timed_t * p = 
        (observer_pjl_ustatus_timed_t *)MEM_CALLOC(sizeof(observer_pjl_ustatus_timed_t), 1);

    ASSERT(p && pipe);
    if (p) 
    {
        ATInitNode(&p->observer_node);
        p->Update = pjl_ustatus_timed_update;
        p->freeFunc = memFree;
        p->CopyConstructor = 0;  // designed to assert as this should not be copied
        p->subject_observer_oid_key = observer_pjl_ustatus_timed_ID;

        p->pipe = pipe;
        p->time = 0;
        p->delay_timer = 0;
        p->status = 0;  // default 
    }
    return p;
}

// used to print the current ustatus timed timer in seconds.
uint32_t get_observer_pjl_ustatus_timed_seconds( IOReg *pipe )
{
    observer_pjl_ustatus_timed_t * o = 0;
    DBG_ASSERT( pipe && pipe->tdevicestatus);
    o = (observer_pjl_ustatus_timed_t *)(pipe->tdevicestatus); //cast to derived

    if (o) 
        return o->time;
    
    return 0;
}


void set_observer_pjl_ustatus_timed(IOReg *pipe, uint32_t time)
{
    ASSERT(pipe);
    observer_pjl_ustatus_timed_t * o = (observer_pjl_ustatus_timed_t *)(pipe->tdevicestatus); //cast to derived

    if (time == 0)
    {
        if (o)
        {
            o->delay_timer = delay_msec_timer_cancel( o->delay_timer );
            o->time = 0;            
            pec_detachobserver_printstatus( (Observer_t*)o ); // cast to baseclass
            pipe->tdevicestatus = NULL;
            MEM_FREE_AND_NULL(o);
        }
    }
    else
    {
        if (o) 
        {
            o->delay_timer = delay_msec_timer_cancel( o->delay_timer );
            pec_detachobserver_printstatus( (Observer_t*)o ); // cast to baseclass
        }
        else 
        {
            o = observer_pjl_ustatus_timed_t_Constructor(pipe, time);
            pipe->tdevicestatus = (Observer_t *) o;
        }
        o->time = time;
        pec_attachobserver_printstatus( (Observer_t*)o ); //cast to baseclass

        o->delay_timer = delay_msec_timer_non_blocking( 
            time * 100, pjl_ustatus_timed_delay_notify, o, true);
    }
}
/// end ustatus_timed.







Observer_t * ///< return null or pointer to base class.
PipeObserver_CopyConstructor( Observer_t * src_observer ) 
{
    PipeObserver_t * src = (PipeObserver_t *)src_observer;
    PipeObserver_t * p = (PipeObserver_t *)MEM_CALLOC(sizeof(PipeObserver_t), 1);

    ASSERT(p && src);
    if (p) 
    {
        ATInitNode(&p->observer_node);
        p->Update = src->Update;
        p->freeFunc = memFree;
        p->CopyConstructor = PipeObserver_CopyConstructor;  
        p->subject_observer_oid_key = src->subject_observer_oid_key;

        p->pipe = src->pipe;
    }
    return (Observer_t *)p;
}



///< PipeObserver adds reference to pipe to basic observer.
///< begin device status observer for pipe

Observer_t * ///< return null or pointer to base class.
PipeObserver_Constructor( 
    IOReg *pipe, ///< not owned reference to pipe used to report status.
    uint32_t key,
    void (update)( Observer_t *o, Subject_t *s ) ///< update function.
    )
{
    PipeObserver_t * p = (PipeObserver_t *)MEM_CALLOC(sizeof(PipeObserver_t), 1);

    ASSERT(p && pipe && update);
    if (p) 
    {
        ATInitNode(&p->observer_node);
        p->Update = update;
        p->freeFunc = memFree;
        p->CopyConstructor = PipeObserver_CopyConstructor;
        p->subject_observer_oid_key = key;

        p->pipe = pipe;
    }
    return (Observer_t *)p;
}








