/***********************************************************
* (c) Copyright 2008-2009 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

/****************************************************************************** 
 */
/****************************************************************************** 
 * @file This program manages access to resources on the system.  To use a 
        resource this must be used.
 *
 *******************************************************************/

#include "agRouter.h"
#include "ATypes.h"
#include "memAPI.h"
#include "mlimiter.h"
#include "agResourceMgr.h"
#include "logger.h"
#include "lassert.h"
#include "ResourceMap.h"
#include "SJMgr.h"
#include "error_types.h"

#define DBG_PRFX "RM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 13 ) 

static ATLISTENTRY resource_list;
static uint32_t NumUsedResources;
static uint8_t  PercentCPUInUse = 0;      // the amount of cpu used

/**
 *
 * \brief This is called by the main routine to start the resource manager.
 *
 */
void ResourceInit(void)
{
    ATInitList(&resource_list);
    
    NumUsedResources = 0;   // the number of resources we have used.
}

/**
 ** ResourceRegister
 *
 *  \param MyThread is deprecated and should not be used further
 *          \param RscType Specifies the type of resource.
 *          \param RsrcFunctons Special functions for use by the resource.
 *          \param Module The module id of the resource.
 *  \param NumThreads is deprecated and should not be used further
 *
 * /brief Register a system resource with the system
 *  DESCRIPTION: Every resource in the system needs to call this register function
        To allow the system to manage the resource.  A resource of this type is the print
 *      subsystem, scan subsystem etc.  The resource structure is created 
        and the semaphore is created.  If you call this with a resource that is already
 *      registered this will replace the task pointers with what is passed in.  This
 *      allows threads that are part of a resource that exist only for a brief time
 *      to be able to update their thread pointers.
 *
 *  RETURNS:
 *
 */
uint32_t rm_register(RESOURCES RscType,  MODULE_ID Module)
{
    ATLISTENTRY* node;
    AVAIL_RESOURCE *MyResource;
    int err = 0; UNUSED_VAR( err ); // release ignore OS error return
    
    //
    // See if the resource already exists, and if it does just update.
    // 
    node = ATListHead(&resource_list);
    while(node != NULL)
    {
        MyResource = CONTAINING_RECORD(node, AVAIL_RESOURCE, node);
        if(MyResource->ResourceType == RscType)
            break;      // found it, get out.
        node = ATListNext(&resource_list, node);
    }
    if(node == NULL)
    {
        //
        // get the memory for the data storage.
        //
        MyResource = MEM_CALLOC(1, sizeof(AVAIL_RESOURCE));
        if(MyResource == NULL)
            return 0;
        ATInsertHeadList(&resource_list, &MyResource->node);  // put it on the list
        //
        // Fill in the resource stuff
        //
        MyResource->ResourceType = RscType;

        err = sem_init( &MyResource->Lock, 0, 1 ); // 
        XASSERT( err != -1, thread_errno );
    }

    MyResource->ModID = Module; // add in the module id.
    return 1;
}
   
#ifdef LSPTODO   
/**
 * \brief Tiny shim to support old rm.  Remove when old is gone.
 */
uint32_t ResourceRegister(RESOURCES RscType, TX_THREAD **MyThread, void *ResrcFunctions, MODULE_ID Module, uint32_t NumThreads)
{
    return rm_register(RscType,Module);
}
#endif
/** \brief This is a special MEMORY_RESOURCE that we need due to the print
 * jobs needing an exception for using memory.  We will keep the 
 * print memory resource around until ALL print jobs have finished */
// static MEMORY_RESOURCE * print_io_memory_resource = NULL;


/** \brief The number of print io jobs that we have running */
// static uint32_t number_of_print_io_jobs = 0;



/**
 *
 * \brief Creates the job's heaps Acquires the memory resource associated
 *
 * \return MEMORY_RESOURCE * A pointer to a valid MEMORY_RESOURCE structure or NULL
 * if the memory resource allocation failed or the job's memory resource size
 * is 0
 *
 */
#if 0
static MEMORY_RESOURCE * GetMemoryResource( JOB_RESOURCES * job_resource )
{
    uint32_t size;
    uint32_t limitor_size;
    MEMORY_RESOURCE * memory_resource;
    bool need_to_allocate_memory_resource;

    ASSERT( NULL != job_resource );

    need_to_allocate_memory_resource = true;
    memory_resource = NULL;

    /* If we already have memory allocated for print jobs we will re-use it
     * so check that here */
    if ( ejob_PrintIO == job_resource->JobType )
    {
        if ( NULL == print_io_memory_resource )
        {
            ASSERT( 0 == number_of_print_io_jobs );
            /* this is our first job of type ejob_PrintiO so we'll need
             * to allocate memory */
            need_to_allocate_memory_resource = true;
        }
        else
        {
            ASSERT( number_of_print_io_jobs > 0 );
            /* this is not our first print job so we don't need
             * to allocate memory, just re-use! */
            need_to_allocate_memory_resource = false;
        }

        number_of_print_io_jobs++;
    }

    if ( true == need_to_allocate_memory_resource )
    {
        memory_resource = (MEMORY_RESOURCE *)MEM_MALLOC( sizeof(MEMORY_RESOURCE) );
    }

    if ( NULL != memory_resource )
    {
        size = job_resource->MemoryRequired;

        /* If the size is zero, then we don't need job memory,
         * so we just exit. */
        if (size == 0)
        {
            memory_resource->limiter = NULL;
            MEM_FREE_AND_NULL( memory_resource );
        }
    }

#if 0
    if ( NULL != memory_resource )
    {
        /* Create the job's limiter for data allocations */
        limitor_size = job_resource->MemoryRequired - job_resource->ScratchRequired;

        /* do we need a data heap for this job -- we check based on the
         * recipe size */
        if ( limitor_size > 0 )
        {
            mlimiter_t * limiter;
            limiter = memCreateLimiter( NULL, limitor_size );
            if ( NULL == limiter )
            {
                memory_resource->limiter = NULL;
                MEM_FREE_AND_NULL( memory_resource );
            }
            else
            {
                memory_resource->limiter = limiter;
            }

        }
        else
        {
            memory_resource->limiter = NULL;
        }
    }
#endif


    if ( ejob_PrintIO == job_resource->JobType )
    {
        /* if the job type is print IO and this
         * is our first print job that we have allocated memory for
         * we will save it for future print jobs */
        if ( true == need_to_allocate_memory_resource )
        {
            ASSERT( NULL == print_io_memory_resource );
            print_io_memory_resource = memory_resource;
        }
        /* If this is a print job and this
         * is NOT our first print job then we will use the
         * saved memory resource so print jobs share one memory */
        else
        {
            memory_resource = print_io_memory_resource;
        }
    }

    /* clean up upon error */
    if ( NULL == memory_resource )
    {
        if ( ejob_PrintIO == job_resource->JobType )
        {
            number_of_print_io_jobs--;

            if ( 0 == number_of_print_io_jobs )
            {
                print_io_memory_resource = NULL;
            }
        }
    }

    return memory_resource;
}
#endif
#if 0
error_type_t FreeMemoryResource( JOB_RESOURCES * my_job_resources )
{
    MEMORY_RESOURCE * my_memory_resource;
    bool should_free;

    // If task resource is null, just exit.
    if( NULL == my_job_resources )
    {
        // Should not pass a NULL task resource
        ASSERT( 0 );
        return OK;
    }
    if ( NULL == my_job_resources->MemoryResource )
    {
        // Should not pass a NULL task resource
        ASSERT( 0 );
        return OK;
    }

    should_free = true;
    my_memory_resource = my_job_resources->MemoryResource;

    if ( ejob_PrintIO == my_job_resources->JobType )
    {
        if ( ( number_of_print_io_jobs - 1 ) != 0 )
        {
            /* only free when number of print jobs is 0 */
            should_free = false;
        }
    }

    if ( true == should_free )
    {
        // if a limiter was created, free it here
        if ( NULL != my_memory_resource->limiter )
        {
            // ALL memory was not freed before ending the job
            if ( 0 != my_memory_resource->limiter->current )
            {
                return FAIL;
            }
            MEM_FREE_AND_NULL( my_memory_resource->limiter );
        }

        MEM_FREE_AND_NULL( my_memory_resource );
    }

    if ( ejob_PrintIO == my_job_resources->JobType )
    {
        number_of_print_io_jobs--;

        if ( number_of_print_io_jobs == 0 )
        {
            print_io_memory_resource = NULL;
        }
    }

    return OK;
}
#endif

/**
 *
 * \brief Get the resources needed for a job.
 * This is called by the system job manager to get all the system
 * resources for a job.  If it can't get the resources it returns
 * else with a successful return the resources are reserved and
 * ready to go.
 *
 * \param JobResources The job ticket that contains the resources required
 * for a job.
 *
 * \return FAIL if we could not get the resource. OK if we got them all.
 *
 */
error_type_t GetSystemResource(JOB_RESOURCES *JobResources)
{
    ATLISTENTRY* node;
    AVAIL_RESOURCE* current_resource;
    uint32_t i;
    AVAIL_RESOURCE *RscList[10];
    MODULE_ID ModId[30];
    uint32_t NumResource=0, k;
    int retcode;
    
    // See if we have the mips to run this job.
    if(JobResources->PercentCPU > 100)
    {
        // this job will never run.
        DBG_PRINTF_ERR("RM: Requested CPU percent of %d exceeds max of 100.\n",
		       JobResources->PercentCPU);
        return FAIL;
    }
    if(PercentCPUInUse + JobResources->PercentCPU > 100)
    {
        DBG_PRINTF_NOTICE("RM: Failed to get CPU resource.  Requested = %d, in use = %d.\n", JobResources->PercentCPU, PercentCPUInUse);
        // not enough resources.
        return FAIL;
    }
    PercentCPUInUse += JobResources->PercentCPU;     // now update the amount used.
    i = 0;

    // First try and get all the module resources
    while ((uint32_t)JobResources->ReqResources[i] != e_NoResource)
    {
        node = ATListHead(&resource_list);
        while (node != NULL)
        {
            current_resource = CONTAINING_RECORD(node, AVAIL_RESOURCE, node);
            if (JobResources->ReqResources[i] == current_resource->ResourceType)
            {
               // DBG_PRINTF_INFO( "RM: OnLine %d\n",
               //            StatusGetOnlineState(JobResources->ReqResources[i]));
                // we need this resource for this reservation.  Try and get it.
                //
                // WARNING:  The if statement below must be in the order shown.
                // if not then you can get the semaphore, not be on-line and
                // the semaphore will never be freed.
             //   if ((StatusGetOnlineState(JobResources->ReqResources[i]) == ONLINE) &&
                retcode = sem_trywait(&current_resource->Lock);
                if ( 0 == retcode  ) 
                {
                    // we got the resource!  Now save the information.
                    ModId[NumResource] = current_resource->ModID;
                    RscList[NumResource++] = current_resource;
                    DBG_PRINTF_NOTICE( "Got Resource %#x\n",ModId[NumResource-1]);
                    
                } else
                {
                    // we failed to get the resource, release all resources we have reserved and exit
                    XASSERT( EAGAIN == thread_errno, thread_errno );
                    DBG_PRINTF_NOTICE( "RM: Failed to get Resource Module id = %#x\n", current_resource->ModID);
                    for (k = 0; k < NumResource; k++)
                    {
                        retcode = sem_post( &RscList[k]->Lock );
                        XASSERT( 0 == retcode, thread_errno );
                    }
                    // return the cpu resource stuff
                    // 
                    PercentCPUInUse -= JobResources->PercentCPU;
                    // done get out of here.
                    return FAIL;
                }
                break;  // we are finished with this resource, look at the next one.
            }
            node = ATListNext(&resource_list, node);
        }
        i++;
    }

#if 0
    // if we got to here, everything is reserved.  Now get the memory.
    if (JobResources->MemoryRequired == 0)
    {
        JobResources->MemoryResource = NULL;
    } else
    {
        JobResources->MemoryResource = GetMemoryResource( JobResources );
        if( NULL == JobResources->MemoryResource )
        {
	    DBG_PRINTF_NOTICE("Failed to get Memory %#lx Release %ld Resources\n",
			      JobResources->MemoryRequired, NumResource);

            // We were unable to get the memory, release everything else and get out of here.
            for (k = 0; k < NumResource; k++)
            {
                retcode = sem_post(&RscList[k]->Lock);
                XASSERT( 0 == retcode, thread_errno );
            }
            // return the cpu resource stuff
            // 
            PercentCPU -= JobResources->PercentCPU;
            // done get out of here.
            return FAIL;
        }
    }
#endif

    JobResources->AvailResourceList = MEM_MALLOC(sizeof(uint32_t) * (NumResource+1));
    if (JobResources->AvailResourceList == NULL)
    {
        // error_type_t ErrorRes;
        // ErrorRes = FreeMemoryResource( JobResources );
        // ASSERT( OK == ErrorRes );

        for (k = 0; k < NumResource; k++)
        {
            retcode = sem_post(&RscList[k]->Lock); 
            XASSERT( 0 == retcode, thread_errno );
        }
        if(JobResources->AvailResourceList != NULL)
        {
            MEM_FREE_AND_NULL(JobResources->AvailResourceList);
        }
        PercentCPUInUse -= JobResources->PercentCPU;
        return FAIL;    
    }    
    for (k = 0; k <  NumResource; k++)
    {
        JobResources->AvailResourceList[k] = RscList[k];
        JobResources->AvailResourceList[k]->ModID = ModId[k];
    }
    JobResources->AvailResourceList[NumResource] = NULL;
    JobResources->ResourceObtained = 1;   
    
    // ready to roll!
    return OK;
}
 
/* FUNCTION NAME: FreeCPUResource */
 
/** 
 * \brief Decrement the cpu resources used by MIPSPercent
 *
 * \param MIPSPercent The amount to remove
 *
 * \retval None
 *
 **/
void FreeCPUResource(uint8_t MIPSPercent)
{
    ASSERT(MIPSPercent <= 100 && MIPSPercent <= PercentCPUInUse);
    PercentCPUInUse -= MIPSPercent;

    DBG_PRINTF_INFO("free cpu resources used %d, freed %d\n",
		    PercentCPUInUse, MIPSPercent);
    ASSERT(PercentCPUInUse <= 100);
}
 

/* FUNCTION NAME: ReleaseResource */
 
/** 
 * \brief Free the semaphore on a resource.
 * 
 * \param CurrentResource Pointer to the resource.
 * 
 * \retval None
 * 
 **/
void ReleaseResource(CURRENT_RESOURCE *CurrentResource)
{
    int retcode = 0; UNUSED_VAR( retcode ); // release ignore OS error
    retcode = sem_post(&CurrentResource->Me->Lock);
    XASSERT( 0 == retcode, thread_errno );
}


/*
 ** CreateJobResource
 *
 *  PARAMETERS:  NumResources  The number of modules in the job resource
 *
 *  DESCRIPTION:  This makes a JOB_RESOURCES structure with extra memory allocated for
        storage of the resources required.
 *
 *  RETURNS:  A JOB_RESOURCES structure initialized and ready to run.
 *
 */
JOB_RESOURCES *CreateJobResource(uint16_t NumResources)
{
    JOB_RESOURCES *TempResource;

    // get the memory for the data structure
    // calloc shall allocate unused space for an array of (1) elements each of
    // whose size in bytes is (the second parameter). The space shall be
    // initialized to all bits 0 - This lets us skip some variable initialization
    TempResource = (JOB_RESOURCES*)MEM_CALLOC(1, sizeof(JOB_RESOURCES) + ( (NumResources+1) * sizeof(uintptr_t) ) );
    ASSERT(TempResource);
    // Make the pointers for the required resource array.  This follows the resource allocation
    TempResource->ReqResources = (RESOURCES *)((uintptr_t)TempResource + sizeof(JOB_RESOURCES));

    return TempResource;
}
    
JOB_TYPES rm_get_job_type(CURRENT_RESOURCE* cur_resource)
{
    JOB_TYPES cur_job_type = ejob_NULL;
    if ( (cur_resource != NULL) && (cur_resource->JobResources != NULL) ) {
        cur_job_type = cur_resource->JobResources->JobType;
    }
    return cur_job_type;
}
