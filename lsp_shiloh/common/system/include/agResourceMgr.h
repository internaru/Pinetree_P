/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file agResourceMgr.h
 *
 * \brief Header file for the resource manager.
 *
 */

#ifndef __AGRESOURCEMGR_H__
#define __AGRESOURCEMGR_H__

#include <stdint.h>
#include <stdbool.h>

#include "ResourceMap.h"
#include "agConnectMgr.h"
#include "posix_ostools.h"
#include "memAPI.h"
#include "list.h"
#include "error_types.h"
#include "mlimiter.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MODULE_RESOURCES 0
#define SUBSYSTEM_RESOURCE 0x10000


typedef enum
{
    e_NoResource=MODULE_RESOURCES,
    e_Printer,
    e_DMAChan0,
    e_ZJParser,
    e_CMDParser,
    e_PJLParser,
    e_ACLParser,
    e_UnirastParser,
    e_GGSParser,
    e_Scanner,
    e_ScanParser,
    e_InkParser,
    e_IOScanParser,
    e_IOFaxParser,
    e_InternalPages,
    e_DDParser,
    e_CopyApp,
    e_CopyToHostParser,
    e_FaxPhoneline,
    e_FaxPrintApp,
#ifndef HAVE_SDHFAX
    e_FaxReportsApp,
#else
    e_FaxApp,
#endif
    e_ScanToFax,
    e_ResPhoto,
    e_Engine,
    e_JBIG1Services=SUBSYSTEM_RESOURCE,
    e_JBIG2Services, 
    e_HBParser,
    e_Split,    ///< Split print job resource.
    e_FaxPrintAgent,    
    e_HttpScanApp,

    e_ScanCalGraph,
    e_DemoScanApp,
    e_ScanToMemCardApp,
    e_WsdScanApp,
    e_PhotoCopyApp,
    e_SystemJobMgrTestResource,
    e_GsoapScanApp,
    e_UsbHostParser,
    e_PassThruParser,
    e_GenSkel,       ///< Skeleton example generator.
    e_GenPip,        ///< Print Image Pipeline generator.
    e_GenPipCopy,    ///< Print Image Pipeline generator for copy.
    e_GenPippPipr,   ///< Print Image Pipeline generator for multipass color PIP block
    e_GenCompressJBIG,   ///< JBIG Compression generator
    e_GenDecompressJBIG, ///< JBIG Decompression generator
    e_GenCompressJPEG,   ///< JPEG Compression generator
    e_GenMIP,        ///< Monochrome Image Pipe generator
    e_GenWifiPrint,        ///< Wifi Print Server generator
    e_ScanToEmailApp,
    e_mpdf_parser,
    e_FWUpgradeResource,
    e_PrintAppTest,
    e_JpegParser,
    e_Gen_NUp,      ///< N-UP Page Generator
    e_SmJobScanApp,
    e_PFParser,		// @@ PF	2013.08.29	by Juny
    e_PCFAXParser,	// @@ PCFAX	2014.02.20	by inhwan5
    e_A3MergeLib,
    e_EraseRoller,
#ifndef HAVE_SDHFAX
#else
    e_FaxAppSolution,
#endif
    e_NumResources,
} RESOURCES;



/** Build cancel state */
typedef enum
{
    e_NoCancel,     ///< This job can not be cancelled.
    e_YesCancel     ///< This job can be canceled.
} CANCEL_ACTION;


typedef enum
{
    e_Wait,             ///< Wait forever for a resource to come available
    e_NoWait,           ///< If the resource is not available, return immediatly.
    e_GetInterface,     ///< Return the interface, ignore reserve.
} RESERVE_TYPE;


/** Specify wait options to start recipe. */
typedef enum
{
    e_WaitForever,
    e_DoNotWait     ///< If the resources are not available, nak 
} WAIT_OPTIONS;


typedef enum
{
    e_ResourceIdle = 0,  ///< place holder
    e_ResourceOk,        ///< have resource
    e_ResourceBusy,      ///< have resource but it is busy, call get status
    e_ResourceTimedOut,  ///< couldn't get the resource
} RESOURCESTATUS;


typedef struct
{
    ATLISTENTRY node;
    RESOURCES ResourceType;
    MODULE_ID ModID; ///< how to talk to the resource.
    sem_t Lock;  // unnamed 
} AVAIL_RESOURCE;


/** A list of all resources for a given job. */
typedef struct
{
    ATLISTENTRY node;
    uint32_t ResourceObtained:1;            ///< 0 not obtained, 1 obtained.
    uint32_t PercentCPU:7;                  ///< Amount of cpu needed percent. 7 bits = 128
    AVAIL_RESOURCE ** AvailResourceList;         ///< list of all resources.  This order should match ReqResources
    RESOURCES * ReqResources;               ///< list of all resources for this job
    CANCEL_ACTION CancelAction;             ///< Is this job cancellable?
    MODULE_ID RequestMod;                   ///< This is the moduleid of the requesting module
    WAIT_OPTIONS WaitType;                  ///< What to do if we did not get the resources.
    uint32_t JobState;                      ///< current job state
    uint32_t RecoveryAction;                ///< error recovery action: cancel page, cancel job, restart, retry
    AVAIL_RESOURCE * RecoveryResource;      ///< pointer to resource we are recoverying
    JOB_TYPES JobType;                      ///< The kind of job this is.
    uint32_t  JobId;                        ///< The unique id of this job.
    char  UserId[33];                        ///< The current login user id of this job.

} JOB_RESOURCES;
   

/**
 * Allocated Resources, this is the structure of the data that is sent
 * to a module when that module is included in a job.
 */
typedef struct
{
    MODULE_ID       Source;                 ///< module id of data source
    MODULE_ID       Destination;            ///< module id of the destination
    JOB_RESOURCES *JobResources;            ///< job structure, id's which job we are in
    AVAIL_RESOURCE  *Me;                    ///< this is the resource structure for this block.
    bool            FreedResourceNotList;   ///< Used when freeio comes in.  
                                            ///< Leave resource in list since we are not finished.
                                            ///< Allows cal to know what to cancel.
} CURRENT_RESOURCE;     


/**
 * \brief Get the limiter from the CURRENT_RESOURCE pointer and return it
 * through the parameter
 *
 * \param resource The CURRENT_RESOURCE pointer that was passed to the module.
 * Usually the module is a parser that acquires the resource through a start
 * job request.
 *
 * \param *limiter_ptr A pointer to a "mlimiter_t" pointer.  This function will
 * set the value of the "mlimiter_t" pointer so that after the return the
 * caller can use it.
 *
 * \return OK If the "mlimiter_t" pointer has been filled
 * \return FAIL Otherwise
 *
 * \b Example:
 * \code
            CURRENT_RESOURCE * resource;
            mlimiter_t * limiter;
            error_type_t e_res;

            get_resource(&resource);

            e_res = ResourceMgrGetLimiter( resource, &limiter );
            ASSERT( OK == e_res );
            ASSERT( NULL != limiter );
    \endcode
 */
error_type_t ResourceMgrGetLimiter( CURRENT_RESOURCE * resource, mlimiter_t ** limiter_ptr );


/**
 * \brief Get the resources needed for a job.  This is called by the system job
 * managerto get all the system resources for a job. If it can't get the
 * resources it returns else with a successful return the resources are reserved
 * and ready to go. If there are no sub resources registered, this will still
 * allow the job to run.
 *
 * \param JobResources The job ticket that contains the resources required for
 * a job.
 * \return FAIL if we could not get the resource. 
 * \return OK if we got them all.
 *
 **/
error_type_t GetSystemResource(JOB_RESOURCES *JobResources);


/** 
 * \brief Free the semaphore on a resource.
 * 
 * \param CurrentResource is a pointer to the resource.
 * 
 **/
void ReleaseResource(CURRENT_RESOURCE *CurrentResource);

/** 
 * \brief Decrement the cpu resources used by MIPSPercent.
 *
 * \param MIPSPercent is the amount to remove.
 *
 **/
void FreeCPUResource(uint8_t MIPSPercent);


/**
 * \brief Register a system resource with the system.
 *
 * \param MyThread is a list of the threads that the resource uses. This is used
 *        by the memory manager to determine where to allocate data. So if you
 *        have a thread that needs to do allocations, that thread control block
 *        needs to be included in this list.
 * \param RscType Specifies the type of resource.
 * \param ResrcFunctions Special functions for use by the resource.
 * \param Module The module id of the resource.
 * \param NumThreads The number of threads the resource is using.
 *
 * Every resource in the system needs to call this register function. To allow
 * the system to manage the resource. A resource of this type is the print
 * subsystem, scan subsystem etc. The resource structure is created and the
 * semaphore is created. If you call this with a resource that is already
 * registered this will replace the task pointers with what is passed in. This
 * allows threads that are part of a resource that exist only for a brief time
 * to be able to update their thread pointers.
 *
 **/
#ifndef __linux__
uint32_t ResourceRegister(RESOURCES RscType, TX_THREAD **MyThread, void *ResrcFunctions, MODULE_ID Module, uint32_t NumThreads);
#endif

uint32_t rm_register(RESOURCES RscType,  MODULE_ID Module);

/**
 * \param NumResources is the number of modules in the job resource
 *
 * \return A JOB_RESOURCES structure initialized and ready to run.
 *
 * This function makes a JOB_RESOURCES structure with extra memory allocated for
 * storage of the resources and subresources required.
 *
 **/
JOB_RESOURCES *CreateJobResource(uint16_t NumResources);


/**
 * \brief Initializes the resource manager at system startup time
 * This function should only be called once.
 *
 **/
void ResourceInit( void );

/**
 * \brief Read and return the job type associated with the current resource.
 * \param cur_resource
 * \return the JOB_TYPES associated with the current resource
 */
JOB_TYPES rm_get_job_type(CURRENT_RESOURCE* cur_resource);

#ifdef __cplusplus
}
#endif

#endif

