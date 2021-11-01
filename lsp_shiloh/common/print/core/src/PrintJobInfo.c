/** 
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 **/

#include "PrintJobInfo.h"
#include "agConnectMgr.h" // pipe
#include "memAPI.h"
#include "lassert.h"
#include "string.h"
#include "dprintf.h"
#include "logger.h"
#include "virtual_control_panel.h"

void __attribute__((weak))  virtual_control_panel_register( ) 
{
}

/// define NOP for when net_io_job is not in the system 
void __attribute__((weak))  netiojob_copy_init(struct net_io_job_s *dest, struct net_io_job_s * src_or_NULL ) 
{
}

PrintJobInfo_t *system_pji = 0;

extern void zpjl_init_pwm_papersize();

///< called at pipe registration time once per pipe.
PrintJobInfo_t * ///< return null or new object
PrintJobInfo_construct(IOReg *pipe)
{
    PrintJobInfo_t *pji;
    if (system_pji == 0) {
	    zpjl_init_pwm_papersize(); // fix this init race condition correctly.
	    system_pji = (PrintJobInfo_t*) -1;
	    system_pji = PrintJobInfo_construct(0);
	    system_pji->jobnum = 0;
	    virtual_control_panel_register( );
	    // pji_local_copy_init( &system_pji->local, 0 );
    }

    pji = (PrintJobInfo_t *) MEM_CALLOC(sizeof(PrintJobInfo_t), 1);
    if (pji)
    {
        pji->parent = 0;
        pji->lastchild = 0;

        MInitList( &pji->jobstart_observers );
        MInitList( &pji->jobend_observers );
        MInitList( &pji->pagestart_observers );
        MInitList( &pji->pageend_observers );
        MInitList( &pji->canceljobstart_observers );
        MInitList( &pji->canceljobend_observers );
        MInitList( &pji->planedata_observers );

        pji->subject_observer_oid_key = PrintJobInfo_ID;
        pji->pipe = pipe;
        pji->jobname[0] = 0;

        pji->pages_printed = 0;  
        pji->jobstatus = 0;

        pji->pages_start = 0;
        pji->pages_end = 0xffffffff;
        pji->pages_skipped_and_printed = 0;
        pji->num_bytes = 0;  // num bytes processed in print job.
        pji->jobDocType = e_NONE;  // Document type set by Connection Manager
	//pji->duplex = 2; // DMDUP_SIMPLEX; // lsptodo:read from oid_default...
	//pji->copies = 1;
	//pji->collate = 0;
        pji->foreign_context = 0;

        // 3/2014 rdj TODO: remove callback & foreign context from pji struct (used only by netio at time of writing) 
        pji->copy_notify_func = 0;
        pji->copy_notify_user_ctxt = 0;

	if (pipe) {
	    netiojob_copy_init( pji->netiojob, system_pji->netiojob );
	    pji_local_copy_init( &pji->local, &system_pji->local );
        } else {
	    netiojob_copy_init( pji->netiojob, 0 );
	    pji_local_copy_init( &pji->local, 0 );
	}
    }

    return pji;
}


///< copy from one structure to another
void 
PrintJobInfo_copy_init(PrintJobInfo_t *src, PrintJobInfo_t *dest)
{
    ASSERT(dest);


    if (src)
    {
        // this parent child "stuff" is so that pjl can change the job end job name
        // of a job that is about to finish printing, it has left the print parser and is in job manager
        // assuming there wasn't s long time out.
        // parent really is the pjl context
        // childern may be many but there is no more than one last child.
        // pjl is never a child, always the parent and never more that one parent per pipe
        //
        dest->parent = src;
        src->lastchild = dest;

        DPRINTF(DBG_EO, ("PrintJobInfo_copy src %x dest %x jend %x name %s\n", src, dest, &dest->jobend_observers, src->jobname));
        MInitList( &dest->jobstart_observers );
        MInitList( &dest->jobend_observers );
        MInitList( &dest->pagestart_observers );
        MInitList( &dest->pageend_observers );
        MInitList( &dest->canceljobstart_observers );
        MInitList( &dest->canceljobend_observers );
        MInitList( &dest->planedata_observers );


        // for each registered observer copy it to new list.
        DEEPCOPY_OBSERVERS( dest, system_pji, jobstart_observers );
	DEEPCOPY_OBSERVERS( dest, system_pji, jobend_observers );
        DEEPCOPY_OBSERVERS( dest, system_pji, pagestart_observers );
        DEEPCOPY_OBSERVERS( dest, system_pji, pageend_observers );
        DEEPCOPY_OBSERVERS( dest, system_pji, canceljobstart_observers );
        DEEPCOPY_OBSERVERS( dest, system_pji, canceljobend_observers );
        DEEPCOPY_OBSERVERS( dest, system_pji, planedata_observers );
        DEEPCOPY_OBSERVERS( dest, src, jobstart_observers );
	DEEPCOPY_OBSERVERS( dest, src, jobend_observers );
        DEEPCOPY_OBSERVERS( dest, src, pagestart_observers );
        DEEPCOPY_OBSERVERS( dest, src, pageend_observers );
        DEEPCOPY_OBSERVERS( dest, src, canceljobstart_observers );
        DEEPCOPY_OBSERVERS( dest, src, canceljobend_observers );
        DEEPCOPY_OBSERVERS( dest, src, planedata_observers );

        dest->subject_observer_oid_key = PrintJobInfo_ID;

        //copy all the member variables.
        dest->pipe = src->pipe;
        strncpy(&dest->jobname[0], &src->jobname[0], 80);
        dest->pages_start = src->pages_start;
	src->pages_start = 1; // one shot only applies to one job.

	system_pji->jobnum++; // global counter.
	dest->jobnum = system_pji->jobnum;

        dest->pages_end = src->pages_end;
        dest->pages_printed = 0;  
        dest->pages_skipped_and_printed = 0;
        dest->jobDocType = src->jobDocType;	//LALIT 20131029

	pji_local_copy_init( &dest->local, &src->local );
        /*
         * Setting to NULL indicates foreign_context reference has been transferred to the copy of the job
         * info struct.
         * 
         * ISSUE: With this approach we must assume at most one copy is made between calling PipeConnect()
         * and receiving the eClose IOCTL, because otherwise successive jobs will not get a copy of
         * foreign_context. A new copy is made for every instance of a ZJStream job, PCL job, etc. that we
         * get per PipeConnect(). Therefore, we must assume at most one job per PipeConnect(). Whether this
         * is actually true depends on the host software.
         */
	netiojob_copy_init( dest->netiojob, src->netiojob );

        // 3/2014 rdj TODO: remove callback & foreign context from pji struct (used only by netio at time of writing) 
        dest->foreign_context = src->foreign_context;
        dest->copy_notify_func = 0;
        dest->copy_notify_user_ctxt = 0;
        if(src->copy_notify_func)
        {
            src->copy_notify_func(dest, src->copy_notify_user_ctxt);
        }


    }
}

PrintJobInfo_t * ///< return null or new object
PrintJobInfo_copy_constructor(PrintJobInfo_t *parent)
{
    PrintJobInfo_t * pji = (PrintJobInfo_t *)MEM_CALLOC(1, sizeof(PrintJobInfo_t));

    if (pji)
    {
        PrintJobInfo_copy_init(parent, pji);
        NOTIFY_OBSERVERS( pji, jobstart_observers );
    }
    return pji;
}

void PrintJobInfo_destructor(PrintJobInfo_t *free_me)
{
    if (free_me) 
    {
        DPRINTF(DBG_EO, ("PrintJobInfo_destructor %x %s\n", free_me, free_me->jobname));
	// ASSERT
	if (!free_me->parent)
		return; // lets not free this for now
        // if you are wanting to free the factory this is a bad idea since people are registered to the factory.

        if(free_me->parent && free_me->parent->lastchild == free_me)
        {
            free_me->parent->lastchild = 0;
        }

        // iterate through all the observer lists and free them.
        FREE_OBSERVERS( free_me, jobstart_observers );
        FREE_OBSERVERS( free_me, jobend_observers );
        FREE_OBSERVERS( free_me, pagestart_observers );
        FREE_OBSERVERS( free_me, pageend_observers );
        FREE_OBSERVERS( free_me, canceljobstart_observers );
        FREE_OBSERVERS( free_me, canceljobend_observers );

        // don't forget to delete threadx's hooks into the mutexes.        
        MList_DeleteLock( &free_me->jobstart_observers );
        MList_DeleteLock( &free_me->jobend_observers );
        MList_DeleteLock( &free_me->pagestart_observers );
        MList_DeleteLock( &free_me->pageend_observers );
        MList_DeleteLock( &free_me->canceljobstart_observers );
        MList_DeleteLock( &free_me->canceljobend_observers );

        // This was added to support half-close behavior.  However, that
        // currently isn't fully implemented so remove until full support is
        // added since this currently causes problems because we are closing
        // out of sync with the connection manager close.
//-jrs	if (free_me->pipe && free_me->pipe->io.ioctl != NULL )
//-jrs	    free_me->pipe->io.ioctl(free_me->pipe->io.data_read_handle, e_close, NULL);       // tell the guy we are done.

        memFree(free_me);
    }
}


void
PrintJobInfo_AttachObserver_system_jobstart(Observer_t *o) 
{
    ASSERT(VALIDATE_OBJECT_KEY( system_pji, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID));

    ATTACH_OBSERVER( system_pji->jobstart_observers, o );
}



void
PrintJobInfo_AttachObserver_jobstart(PrintJobInfo_t *s, Observer_t *o) 
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID));

    ATTACH_OBSERVER( s->jobstart_observers, o );
}
void 
PrintJobInfo_Notify_jobstart(PrintJobInfo_t *s)
{
    if (VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) )
    {
        NOTIFY_OBSERVERS( s, jobstart_observers );
    }
}


void
PrintJobInfo_AttachObserver_jobend(PrintJobInfo_t *s, Observer_t *o) 
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID));

    ATTACH_OBSERVER( s->jobend_observers, o );
}
void 
PrintJobInfo_Notify_jobend(PrintJobInfo_t *s)
{
    if (VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) )
    {
        NOTIFY_OBSERVERS( s, jobend_observers );
    }
}


void
PrintJobInfo_AttachObserver_pageend(PrintJobInfo_t *s, Observer_t *o) 
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID));

    ATTACH_OBSERVER( s->pageend_observers, o );
}
void 
PrintJobInfo_Notify_pageend(PrintJobInfo_t *s)
{
    if (VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) )
    {
        NOTIFY_OBSERVERS( s, pageend_observers );
    }
}

void
PrintJobInfo_AttachObserver_canceljobend(PrintJobInfo_t *s, Observer_t *o) 
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID));

    ATTACH_OBSERVER( s->canceljobend_observers, o );
}
void 
PrintJobInfo_Notify_canceljobend(PrintJobInfo_t *s)
{
    if (VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) )
    {
        NOTIFY_OBSERVERS( s, canceljobend_observers );
    }
}

/*
DUPLEX_MODE PrintJobInfo_Get_duplex(PrintJobInfo_t *s)
{
	return s->duplex;
}
void PrintJobInfo_Set_duplex(PrintJobInfo_t *s, DUPLEX_MODE duplex)
{
	s->duplex = duplex;
}
*/

char *
PrintJobInfo_Get_jobname( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    DPRINTF(DBG_EO, ("pji: pipe %x jobname %s \n", pji->pipe, &pji->jobname[0]));

    return &pji->jobname[0];
}

uint32_t
PrintJobInfo_Get_pages_printed( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    DPRINTF(DBG_EO, ("pji: print pages printed %d %s ---- \n", pji->pages_printed, pji->jobname));

    return pji->pages_printed;
}

uint32_t
PrintJobInfo_Get_jobstatus( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    return pji->jobstatus;
}


IOReg *
PrintJobInfo_Get_pipe( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    return pji->pipe;
}




// set by pjl then deep copied to job.
void 
PrintJobInfo_Set_jobname( PrintJobInfo_t *s, const char *jobname)
{
    ASSERT( s );
    if (jobname)
    {
        strncpy(&s->jobname[0], jobname, 80);  // pjl size constraint
    }
}

/// PJL JOB START END options are saved, used to control which pages in a job should print.
/// start is one based first page to print, default is 1 meaning print all.
/// end is unsupported.
void 
PrintJobInfo_Set_start_end( PrintJobInfo_t *s, int start, int end )
{
    ASSERT( s );

    s->pages_skipped_and_printed = 0;

    if (start <= 1) 
    {
        s->pages_start = 1;
        s->pages_end = 0xffffffff;
    }
    else
    {
        s->pages_start = start;
        s->pages_end = 0xffffffff;
    }
}


// set worst job status, 
// PAPER_OUT_CANCEL_RESEND and PARSEERROR is worse than CANCELLED so don't overwrite it.
//
void 
PrintJobInfo_Set_jobstatus( PrintJobInfo_t *s, JOB_STATUS status )
{
    if (s && 
        !( ( (s->jobstatus == PARSEERROR) || (s->jobstatus == PAPER_OUT_CANCEL_RESEND) ) 
           && status == CANCELLED))
    {
        s->jobstatus = status;
    }
}


PrintJobInfo_t *
PrintJobInfo_Get_PrintJobInfo( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    return pji;
}

// hint:
// PrintJobInfo_Set_foreign_context(PrintJobInfo_Get_PrintJobInfo(s), context);

void PrintJobInfo_Set_foreign_context( PrintJobInfo_t *s, void *foreign_context)
{    
    ASSERT( s );
    s->foreign_context = foreign_context;
}

void *PrintJobInfo_Get_foreign_context( Subject_t *s )
{
    ASSERT(VALIDATE_OBJECT_KEY( s, PrintJobInfo_t, subject_observer_oid_key, PrintJobInfo_ID) );

    PrintJobInfo_t *pji = (PrintJobInfo_t *) s;

    return pji->foreign_context;

}


