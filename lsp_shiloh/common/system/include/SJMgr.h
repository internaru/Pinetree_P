/*
 * **********************************************************
 * (c) Copyright 2008-2010 Marvell International Ltd. 
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \file SJMgr.h
 *
 * \brief Defines for the system job manager.
 *
 */

#ifndef SJMGR_H
#define SJMGR_H

#include <stdint.h>
#include "ResourceMap.h"
#include "agResourceMgr.h"
#include "agConnectMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t get_max_job_memory(JOB_TYPES job);

//
// Structure for number of jobs to queue.
// 
typedef enum
{
    e_One,      ///< Only 1 type of this job can be queued.
    e_Two,      ///< Two jobs allowed to be in the job queue.  One busy, one pending.
    e_Unlimited ///< Any number of these type of jobs allowed to be queued.
} QUEUE_OPTIONS;

/** Structure of job requests. */
typedef struct
{
    JOB_TYPES myJob;                ///< enum to tell which job this is
    uint8_t   NumResources;           ///< Number of resources required for this job.
    uint8_t   PercentCPU;             ///< The percent of total cpu mips this will use.
    uint32_t  MemoryRequired;         ///< Size of the heap for this job.
    uint32_t  ScratchReserved;        ///< Amount of memory in heap to reserve for scratch allocations.
    RESOURCES   RequiredResources[7]; ///< A list of the resources required.
    CANCEL_ACTION   CancelAction;       ///< If a front panel key can cancel this job.
    QUEUE_OPTIONS Queued;           ///< Options for queueing.
} JOB_RECIPE;

/** Structure of items to send in a startrecipe message. */
typedef struct
{
    IOReg   *Pipe;      ///< IO Pipe to report status for this job.
    JOB_TYPES Job;      ///< The job to start;
    RESOURCES AdditionalResource;  ///< The enum to identify a resource to add at the start of this job
    WAIT_OPTIONS  Wait; ///< What to do is resources are not available.
    MODULE_ID SendingModule;   ///< The module that requested the start recipe.  Used if wait option is set to nowait.
    
} STARTRECIPE;

/** Identify the recipies allowed. */
extern JOB_RECIPE jobPrinting;
extern JOB_RECIPE jobPJL;
extern JOB_RECIPE jobACL;
extern JOB_RECIPE jobCopy;
extern JOB_RECIPE jobScanToHost;

int SJMGetTotalNumJobs( void );
JOB_TYPES SJMGetCurJobType(void);
void SJMGetAllJobInfo(void);

/**
 * 
 * \brief The initialization routine for the System Job Manager.
 *
 */
void SysJbMgrInit(void);


/**
 *
 * \brief The shutdown routine for the System Job Manager.
 *
 */
void SysJbMgrShutdown(void); 


#ifdef __cplusplus
}
#endif

#endif

