#ifndef PrintJobInfo_h
#define PrintJobInfo_h
/** 
 * ============================================================================
 * Copyright (c) 2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * 
 **/

#include "mlist.h"
#include "event_observer.h"
#include "print_job_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PrintJobInfo_t_DEFINED
#define PrintJobInfo_t_DEFINED
///< print job template 
typedef struct PrintJobInfo_s PrintJobInfo_t;
#endif

#ifndef IOReg_DEFINED
#  define IOReg_DEFINED
typedef struct IOStruct_s IOReg;
#endif

#include "pjl_variables_common.h"

/// sanity check id unique is good but not 100% required.
#define PrintJobInfo_ID 0x13413  

// 3/2014 rdj TODO: remove callback & foreign context from pji struct (used only by netio at time of writing) 
// user-registered callback called just after the copy constructor completes
typedef void (*pji_copy_notify_func_t)(PrintJobInfo_t *pji_copy, void *user_ctxt);

/// Used at the print pipe level to setup how a print job should be observered
/// pjl and wsd are examples of clients that want to monitor a print job.
/// Print_JOBMGR will get a copy of the settings since multiple jobs can come from a pipe
/// and be active in the system at a time.


struct PrintJobInfo_s
{
    Subject_T_BaseClassMembers;  ///< the base class members.

    
    // A printer language has entered. PCL, PS, Host based etc. needs conversation...
    MLISTENTRY jobstart_observers; 

    // All pages are safely in the output bin.
    MLISTENTRY jobend_observers; 

    // undefined. is this pdl or only a print job manager message?
    MLISTENTRY pagestart_observers; 

    // a page is safely in the output bin.
    MLISTENTRY pageend_observers; 

    // begin cancel end cancel or just a cancel with state?
    MLISTENTRY canceljobstart_observers; 
    MLISTENTRY canceljobend_observers; 
    MLISTENTRY planedata_observers;  // each chunk of data.
    
    PrintJobInfo_t *parent; ///< null if top of food chain, points to factory otherwise.
    PrintJobInfo_t *lastchild; ///< null if child, if factory points to last child constructed.
    
    IOReg *pipe; ///< not owned, never null.
    // member variables
    // job name
    // job type
    // etc.
    pji_local_t local; ///< PJL variables interface.
    char jobname[81]; ///< pjl likes to name jobs. owned, copied, fixed size for simplicity.
    uint32_t jobnum;  ///< incrementing counter of jobs.
    JOB_DATA *job;    ///< reference
    uint32_t pages_printed; ///< paper in output bin.
    uint32_t jobstatus;   ///< agJob::JOB_STATUS
    uint32_t pages_start; ///< 1 - N ; 1 is default for print all.
    uint32_t pages_end;   ///< 1 - N ; 0xfffff is default, means print all.
    uint32_t pages_skipped_and_printed; ///< count of all skipped and actually printed pages.  
    uint32_t num_bytes;  ///< number of bytes of image data post parser and post image processing.
    DOCTYPE jobDocType;	/// < Document type of current job. Print Manager interest in Document type of current job.
//    int32_t duplex;
//    int32_t copies;
//    int32_t collate;

    struct net_io_job_s *netiojob;
    void * foreign_context; ///< not owned reference to foreign data object.
    
// 3/2014 rdj TODO: remove callback & foreign context from pji struct (used only by netio at time of writing) 
    // user-registered callback from which user can set the opaque foreign_context
    pji_copy_notify_func_t copy_notify_func;
    void *copy_notify_user_ctxt;
};

///< called at pipe registration time once per pipe.
PrintJobInfo_t * ///< return null or new object
PrintJobInfo_construct( IOReg* pipe );

///< Deep copy, called at print job construction time to copy the job
///< settings from the pipe scope to the job scope.
PrintJobInfo_t * ///< return null or new object
PrintJobInfo_copy_constructor(PrintJobInfo_t *parent);

///< Called by print job manager when a job is completely done.
void 
PrintJobInfo_destructor(PrintJobInfo_t *free_me);

///< called by printjobmgr? needs definition!!!
void PrintJobInfo_Notify_jobstart(PrintJobInfo_t *s);

///< jobend is Called by printjobmgr when the print job's last sheet is safely in the output bin.
void 
PrintJobInfo_Notify_jobend(PrintJobInfo_t *s);

///< called by printjobmgr? needs definition!!!
void 
PrintJobInfo_Notify_jobstart(PrintJobInfo_t *s);

///< called by printjobmgr, page is safely in the output bin.
///< This maybe called twice once for the front side and once for the backside of a sheet of paper.
///< 
void 
PrintJobInfo_Notify_pageend(PrintJobInfo_t *s);

///< called by printjobmgr, 
void 
PrintJobInfo_Notify_canceljobend(PrintJobInfo_t *s);



DUPLEX_MODE PrintJobInfo_Get_duplex(PrintJobInfo_t *s);
void PrintJobInfo_Set_duplex(PrintJobInfo_t *s, DUPLEX_MODE duplex);

// observers will need to get information out of the printjobinfo they are watching, hence get functions:

char * PrintJobInfo_Get_jobname( Subject_t *s );

// set by pjl then deep copied to job.
void PrintJobInfo_Set_jobname( PrintJobInfo_t *s, const char *jobname);

void
PrintJobInfo_Set_start_end( PrintJobInfo_t *s, int start, int end );

uint32_t PrintJobInfo_Get_pages_printed( Subject_t *s );

IOReg * PrintJobInfo_Get_pipe( Subject_t *s );

uint32_t PrintJobInfo_Get_jobstatus( Subject_t *s );

// set worst job status for a print job info to be sent back to the host.
// PAPER_OUT_CANCEL_RESEND is worse than CANCELLED so don't overwrite it.
// better status does clear the status.
//
void PrintJobInfo_Set_jobstatus( PrintJobInfo_t *s, JOB_STATUS status );

extern PrintJobInfo_t *system_pji;

///< CMD debug dump 
//void PrintJobInfo_cmd_dump( PrintJobInfo_t *pji );
void PrintJobInfo_cmd_dump( PrintJobInfo_t *pji, int (pf)(const char* format, ...) );


//void PrintJobInfo_Set_( PrintJobInfo_t *s, struct net_io_job_s *netiojob);
void PrintJobInfo_Set_foreign_context( PrintJobInfo_t *s, void *foreign_context);
void *PrintJobInfo_Get_foreign_context( Subject_t *s );


#ifdef __cplusplus
}
#endif

#endif // PrintJobInfo_h
